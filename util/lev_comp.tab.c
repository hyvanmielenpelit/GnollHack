
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

/* NetHack 3.6  lev_comp.y	$NHDT-Date: 1543371691 2018/11/28 02:21:31 $  $NHDT-Branch: NetHack-3.6.2-beta01 $:$NHDT-Revision: 1.22 $ */
/*      Copyright (c) 1989 by Jean-Christophe Collet */
/* NetHack may be freely redistributed.  See license for details. */

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
     GEOMETRY_ID = 273,
     NOMAP_ID = 274,
     OBJECT_ID = 275,
     COBJECT_ID = 276,
     MONSTER_ID = 277,
     TRAP_ID = 278,
     DOOR_ID = 279,
     DRAWBRIDGE_ID = 280,
     object_ID = 281,
     monster_ID = 282,
     terrain_ID = 283,
     MAZEWALK_ID = 284,
     WALLIFY_ID = 285,
     REGION_ID = 286,
     FILLING = 287,
     IRREGULAR = 288,
     JOINED = 289,
     ALTAR_ID = 290,
     ANVIL_ID = 291,
     LADDER_ID = 292,
     STAIR_ID = 293,
     NON_DIGGABLE_ID = 294,
     NON_PASSWALL_ID = 295,
     ROOM_ID = 296,
     PORTAL_ID = 297,
     TELEPRT_ID = 298,
     BRANCH_ID = 299,
     LEV = 300,
     MINERALIZE_ID = 301,
     CORRIDOR_ID = 302,
     GOLD_ID = 303,
     ENGRAVING_ID = 304,
     FOUNTAIN_ID = 305,
     THRONE_ID = 306,
     POOL_ID = 307,
     SINK_ID = 308,
     NONE = 309,
     RAND_CORRIDOR_ID = 310,
     DOOR_STATE = 311,
     LIGHT_STATE = 312,
     CURSE_TYPE = 313,
     ENGRAVING_TYPE = 314,
     DIRECTION = 315,
     RANDOM_TYPE = 316,
     RANDOM_TYPE_BRACKET = 317,
     A_REGISTER = 318,
     ALIGNMENT = 319,
     LEFT_OR_RIGHT = 320,
     CENTER = 321,
     TOP_OR_BOT = 322,
     ALTAR_TYPE = 323,
     UP_OR_DOWN = 324,
     SUBROOM_ID = 325,
     NAME_ID = 326,
     FLAGS_ID = 327,
     FLAG_TYPE = 328,
     MON_ATTITUDE = 329,
     MON_ALERTNESS = 330,
     MON_APPEARANCE = 331,
     ROOMDOOR_ID = 332,
     IF_ID = 333,
     ELSE_ID = 334,
     TERRAIN_ID = 335,
     HORIZ_OR_VERT = 336,
     REPLACE_TERRAIN_ID = 337,
     EXIT_ID = 338,
     SHUFFLE_ID = 339,
     QUANTITY_ID = 340,
     BURIED_ID = 341,
     LOOP_ID = 342,
     FOR_ID = 343,
     TO_ID = 344,
     SWITCH_ID = 345,
     CASE_ID = 346,
     BREAK_ID = 347,
     DEFAULT_ID = 348,
     ERODED_ID = 349,
     TRAPPED_STATE = 350,
     RECHARGED_ID = 351,
     INVIS_ID = 352,
     GREASED_ID = 353,
     FEMALE_ID = 354,
     CANCELLED_ID = 355,
     REVIVED_ID = 356,
     AVENGE_ID = 357,
     FLEEING_ID = 358,
     BLINDED_ID = 359,
     PARALYZED_ID = 360,
     STUNNED_ID = 361,
     CONFUSED_ID = 362,
     SEENTRAPS_ID = 363,
     ALL_ID = 364,
     MONTYPE_ID = 365,
     GRAVE_ID = 366,
     ERODEPROOF_ID = 367,
     FUNCTION_ID = 368,
     MSG_OUTPUT_TYPE = 369,
     COMPARE_TYPE = 370,
     UNKNOWN_TYPE = 371,
     rect_ID = 372,
     fillrect_ID = 373,
     line_ID = 374,
     randline_ID = 375,
     grow_ID = 376,
     selection_ID = 377,
     flood_ID = 378,
     rndcoord_ID = 379,
     circle_ID = 380,
     ellipse_ID = 381,
     filter_ID = 382,
     complement_ID = 383,
     gradient_ID = 384,
     GRADIENT_TYPE = 385,
     LIMITED = 386,
     HUMIDITY_TYPE = 387,
     STRING = 388,
     MAP_ID = 389,
     NQSTRING = 390,
     VARSTRING = 391,
     CFUNC = 392,
     CFUNC_INT = 393,
     CFUNC_STR = 394,
     CFUNC_COORD = 395,
     CFUNC_REGION = 396,
     VARSTRING_INT = 397,
     VARSTRING_INT_ARRAY = 398,
     VARSTRING_STRING = 399,
     VARSTRING_STRING_ARRAY = 400,
     VARSTRING_VAR = 401,
     VARSTRING_VAR_ARRAY = 402,
     VARSTRING_COORD = 403,
     VARSTRING_COORD_ARRAY = 404,
     VARSTRING_REGION = 405,
     VARSTRING_REGION_ARRAY = 406,
     VARSTRING_MAPCHAR = 407,
     VARSTRING_MAPCHAR_ARRAY = 408,
     VARSTRING_MONST = 409,
     VARSTRING_MONST_ARRAY = 410,
     VARSTRING_OBJ = 411,
     VARSTRING_OBJ_ARRAY = 412,
     VARSTRING_SEL = 413,
     VARSTRING_SEL_ARRAY = 414,
     METHOD_INT = 415,
     METHOD_INT_ARRAY = 416,
     METHOD_STRING = 417,
     METHOD_STRING_ARRAY = 418,
     METHOD_VAR = 419,
     METHOD_VAR_ARRAY = 420,
     METHOD_COORD = 421,
     METHOD_COORD_ARRAY = 422,
     METHOD_REGION = 423,
     METHOD_REGION_ARRAY = 424,
     METHOD_MAPCHAR = 425,
     METHOD_MAPCHAR_ARRAY = 426,
     METHOD_MONST = 427,
     METHOD_MONST_ARRAY = 428,
     METHOD_OBJ = 429,
     METHOD_OBJ_ARRAY = 430,
     METHOD_SEL = 431,
     METHOD_SEL_ARRAY = 432,
     DICE = 433
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
#line 476 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 488 "lev_comp.tab.c"

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
#define YYLAST   1040

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  196
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  161
/* YYNRULES -- Number of rules.  */
#define YYNRULES  408
/* YYNRULES -- Number of states.  */
#define YYNSTATES  872

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   433

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   191,   195,     2,
     135,   136,   189,   187,   133,   188,   193,   190,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   134,     2,
       2,   192,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   137,     2,   138,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   139,   194,   140,     2,     2,     2,     2,
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
     125,   126,   127,   128,   129,   130,   131,   132,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    15,    19,    25,
      31,    37,    41,    58,    59,    62,    63,    66,    67,    70,
      72,    74,    75,    79,    83,    85,    86,    89,    93,    95,
      97,    99,   101,   103,   105,   107,   109,   111,   113,   115,
     117,   119,   121,   123,   125,   127,   129,   131,   133,   135,
     137,   139,   141,   143,   145,   147,   149,   151,   153,   155,
     157,   159,   161,   163,   165,   167,   169,   171,   173,   175,
     177,   179,   181,   183,   185,   187,   189,   191,   193,   195,
     197,   199,   201,   203,   205,   207,   209,   211,   213,   215,
     217,   219,   221,   223,   225,   227,   229,   231,   235,   239,
     245,   249,   255,   261,   267,   271,   275,   281,   287,   293,
     301,   309,   317,   323,   325,   329,   331,   335,   337,   341,
     343,   347,   349,   353,   355,   359,   361,   365,   366,   367,
     376,   381,   383,   384,   386,   388,   394,   398,   399,   400,
     410,   411,   414,   415,   421,   422,   427,   429,   432,   434,
     441,   442,   446,   447,   454,   455,   460,   461,   466,   468,
     469,   474,   478,   480,   484,   488,   494,   500,   508,   513,
     514,   525,   526,   539,   540,   543,   549,   551,   557,   559,
     565,   567,   573,   575,   585,   591,   593,   595,   597,   599,
     601,   605,   607,   609,   611,   619,   625,   627,   629,   631,
     633,   637,   638,   644,   649,   650,   654,   656,   658,   660,
     662,   665,   667,   669,   671,   673,   675,   679,   683,   687,
     689,   691,   695,   697,   699,   703,   707,   708,   714,   717,
     718,   722,   724,   728,   730,   734,   738,   740,   742,   746,
     748,   750,   752,   756,   758,   760,   762,   768,   776,   782,
     791,   793,   797,   803,   809,   817,   825,   832,   838,   839,
     842,   846,   850,   854,   858,   860,   866,   876,   882,   886,
     890,   891,   902,   903,   905,   913,   917,   923,   929,   933,
     939,   947,   957,   959,   961,   963,   965,   967,   968,   971,
     973,   977,   979,   981,   983,   985,   987,   989,   991,   993,
     995,   997,   999,  1001,  1005,  1007,  1009,  1014,  1016,  1018,
    1023,  1025,  1027,  1032,  1034,  1039,  1045,  1047,  1051,  1053,
    1057,  1059,  1061,  1066,  1076,  1078,  1080,  1085,  1087,  1093,
    1095,  1097,  1102,  1104,  1106,  1112,  1114,  1116,  1118,  1123,
    1125,  1127,  1133,  1135,  1137,  1141,  1143,  1145,  1149,  1151,
    1156,  1160,  1164,  1168,  1172,  1176,  1180,  1182,  1184,  1188,
    1190,  1194,  1195,  1197,  1199,  1201,  1203,  1207,  1208,  1210,
    1212,  1215,  1218,  1223,  1230,  1235,  1242,  1249,  1256,  1263,
    1266,  1273,  1282,  1291,  1302,  1317,  1320,  1322,  1326,  1328,
    1332,  1334,  1336,  1338,  1340,  1342,  1344,  1346,  1348,  1350,
    1352,  1354,  1356,  1358,  1360,  1362,  1364,  1366,  1377
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     197,     0,    -1,    -1,   198,    -1,   199,    -1,   199,   198,
      -1,   200,   206,   208,    -1,    16,   134,   141,    -1,    15,
     134,   141,   133,     3,    -1,    17,   134,    11,   133,   299,
      -1,    17,   134,    10,   133,     3,    -1,    17,   134,    13,
      -1,    17,   134,    12,   133,     3,   133,     3,   133,     5,
     133,     5,   133,   319,   133,   205,   204,    -1,    -1,   133,
     131,    -1,    -1,   133,   326,    -1,    -1,   133,     3,    -1,
       5,    -1,    61,    -1,    -1,    72,   134,   207,    -1,    73,
     133,   207,    -1,    73,    -1,    -1,   210,   208,    -1,   139,
     208,   140,    -1,   251,    -1,   201,    -1,   307,    -1,   308,
      -1,   309,    -1,   293,    -1,   253,    -1,   216,    -1,   215,
      -1,   302,    -1,   265,    -1,   285,    -1,   311,    -1,   312,
      -1,   295,    -1,   296,    -1,   310,    -1,   231,    -1,   241,
      -1,   243,    -1,   247,    -1,   245,    -1,   228,    -1,   238,
      -1,   224,    -1,   227,    -1,   288,    -1,   270,    -1,   286,
      -1,   273,    -1,   279,    -1,   303,    -1,   298,    -1,   291,
      -1,   252,    -1,   304,    -1,   258,    -1,   256,    -1,   297,
      -1,   301,    -1,   300,    -1,   289,    -1,   290,    -1,   292,
      -1,   284,    -1,   287,    -1,   151,    -1,   153,    -1,   155,
      -1,   157,    -1,   159,    -1,   161,    -1,   163,    -1,   165,
      -1,   167,    -1,   150,    -1,   152,    -1,   154,    -1,   156,
      -1,   158,    -1,   160,    -1,   162,    -1,   164,    -1,   166,
      -1,   211,    -1,   212,    -1,   144,    -1,   144,    -1,   212,
      -1,    84,   134,   211,    -1,   213,   192,   338,    -1,   213,
     192,   122,   134,   347,    -1,   213,   192,   337,    -1,   213,
     192,   353,   134,   331,    -1,   213,   192,   352,   134,   333,
      -1,   213,   192,   351,   134,   335,    -1,   213,   192,   326,
      -1,   213,   192,   329,    -1,   213,   192,   139,   222,   140,
      -1,   213,   192,   139,   221,   140,    -1,   213,   192,   139,
     220,   140,    -1,   213,   192,   353,   134,   139,   219,   140,
      -1,   213,   192,   352,   134,   139,   218,   140,    -1,   213,
     192,   351,   134,   139,   217,   140,    -1,   213,   192,   139,
     223,   140,    -1,   336,    -1,   217,   133,   336,    -1,   334,
      -1,   218,   133,   334,    -1,   332,    -1,   219,   133,   332,
      -1,   330,    -1,   220,   133,   330,    -1,   327,    -1,   221,
     133,   327,    -1,   338,    -1,   222,   133,   338,    -1,   337,
      -1,   223,   133,   337,    -1,    -1,    -1,   113,   143,   135,
     225,   342,   136,   226,   209,    -1,   143,   135,   345,   136,
      -1,    83,    -1,    -1,     6,    -1,     6,    -1,   137,   338,
     115,   338,   138,    -1,   137,   338,   138,    -1,    -1,    -1,
      90,   232,   137,   325,   138,   233,   139,   234,   140,    -1,
      -1,   235,   234,    -1,    -1,    91,   349,   134,   236,   208,
      -1,    -1,    93,   134,   237,   208,    -1,    92,    -1,   193,
     193,    -1,    89,    -1,    88,   214,   192,   338,   239,   338,
      -1,    -1,   240,   242,   209,    -1,    -1,    87,   137,   325,
     138,   244,   209,    -1,    -1,   230,   134,   246,   210,    -1,
      -1,    78,   230,   248,   249,    -1,   209,    -1,    -1,   209,
     250,    79,   209,    -1,    14,   134,   337,    -1,    55,    -1,
      55,   134,   349,    -1,    55,   134,    61,    -1,    47,   134,
     254,   133,   254,    -1,    47,   134,   254,   133,   349,    -1,
     135,     4,   133,    60,   133,   269,   136,    -1,   314,   229,
     133,   319,    -1,    -1,    70,   134,   255,   133,   262,   133,
     264,   315,   257,   209,    -1,    -1,    41,   134,   255,   133,
     261,   133,   263,   133,   264,   315,   259,   209,    -1,    -1,
     133,     5,    -1,   135,     4,   133,     4,   136,    -1,    61,
      -1,   135,     4,   133,     4,   136,    -1,    61,    -1,   135,
     271,   133,   272,   136,    -1,    61,    -1,   135,     4,   133,
       4,   136,    -1,    61,    -1,    77,   134,   266,   133,   318,
     133,   267,   133,   269,    -1,    24,   134,   318,   133,   347,
      -1,     5,    -1,    61,    -1,   268,    -1,    61,    -1,    60,
      -1,    60,   194,   268,    -1,     4,    -1,    61,    -1,    19,
      -1,    18,   134,   271,   133,   272,   260,   142,    -1,    18,
     134,   326,   260,   142,    -1,    65,    -1,    66,    -1,    67,
      -1,    66,    -1,    22,   134,   275,    -1,    -1,    22,   134,
     275,   274,   209,    -1,   333,   133,   326,   276,    -1,    -1,
     276,   133,   277,    -1,   337,    -1,    74,    -1,    75,    -1,
     321,    -1,    76,   337,    -1,    99,    -1,    97,    -1,   100,
      -1,   101,    -1,   102,    -1,   103,   134,   325,    -1,   104,
     134,   325,    -1,   105,   134,   325,    -1,   106,    -1,   107,
      -1,   108,   134,   278,    -1,   141,    -1,   109,    -1,   141,
     194,   278,    -1,    20,   134,   281,    -1,    -1,    21,   134,
     281,   280,   209,    -1,   335,   282,    -1,    -1,   282,   133,
     283,    -1,    58,    -1,   110,   134,   333,    -1,   350,    -1,
      71,   134,   337,    -1,    85,   134,   325,    -1,    86,    -1,
      57,    -1,    94,   134,   325,    -1,   112,    -1,    56,    -1,
      95,    -1,    96,   134,   325,    -1,    97,    -1,    98,    -1,
     326,    -1,    23,   134,   313,   133,   326,    -1,    25,   134,
     326,   133,    60,   133,   318,    -1,    29,   134,   326,   133,
      60,    -1,    29,   134,   326,   133,    60,   133,     5,   204,
      -1,    30,    -1,    30,   134,   347,    -1,    37,   134,   326,
     133,    69,    -1,    38,   134,   326,   133,    69,    -1,    38,
     134,   355,   133,   355,   133,    69,    -1,    42,   134,   355,
     133,   355,   133,   141,    -1,    43,   134,   355,   133,   355,
     294,    -1,    44,   134,   355,   133,   355,    -1,    -1,   133,
      69,    -1,    50,   134,   347,    -1,    51,   134,   347,    -1,
      53,   134,   347,    -1,    52,   134,   347,    -1,     3,    -1,
     135,     3,   133,   319,   136,    -1,    82,   134,   329,   133,
     331,   133,   331,   133,     7,    -1,    80,   134,   347,   133,
     331,    -1,    39,   134,   329,    -1,    40,   134,   329,    -1,
      -1,    31,   134,   329,   133,   319,   133,   314,   315,   305,
     306,    -1,    -1,   209,    -1,    35,   134,   326,   133,   320,
     133,   322,    -1,    36,   134,   326,    -1,   111,   134,   326,
     133,   337,    -1,   111,   134,   326,   133,    61,    -1,   111,
     134,   326,    -1,    48,   134,   338,   133,   326,    -1,    49,
     134,   326,   133,   354,   133,   337,    -1,    46,   134,   325,
     133,   325,   133,   325,   133,   325,    -1,    46,    -1,   141,
      -1,    61,    -1,   141,    -1,    61,    -1,    -1,   133,   316,
      -1,   317,    -1,   317,   133,   316,    -1,    32,    -1,    33,
      -1,    34,    -1,    56,    -1,    61,    -1,    57,    -1,    61,
      -1,    64,    -1,   323,    -1,    61,    -1,    64,    -1,   323,
      -1,    63,   134,    61,    -1,    68,    -1,    61,    -1,    63,
     137,     4,   138,    -1,   141,    -1,   152,    -1,   153,   137,
     338,   138,    -1,   338,    -1,   327,    -1,   124,   135,   347,
     136,    -1,   156,    -1,   157,   137,   338,   138,    -1,   135,
       4,   133,     4,   136,    -1,    61,    -1,    62,   328,   138,
      -1,   132,    -1,   132,   133,   328,    -1,   330,    -1,   158,
      -1,   159,   137,   338,   138,    -1,   135,     4,   133,     4,
     133,     4,   133,     4,   136,    -1,   332,    -1,   160,    -1,
     161,   137,   338,   138,    -1,     3,    -1,   135,     3,   133,
     319,   136,    -1,   334,    -1,   162,    -1,   163,   137,   338,
     138,    -1,   141,    -1,     3,    -1,   135,     3,   133,   141,
     136,    -1,    61,    -1,   336,    -1,   164,    -1,   165,   137,
     338,   138,    -1,   141,    -1,     3,    -1,   135,     3,   133,
     141,   136,    -1,    61,    -1,   324,    -1,   337,   193,   324,
      -1,     4,    -1,   348,    -1,   135,     8,   136,    -1,   150,
      -1,   151,   137,   338,   138,    -1,   338,   187,   338,    -1,
     338,   188,   338,    -1,   338,   189,   338,    -1,   338,   190,
     338,    -1,   338,   191,   338,    -1,   135,   338,   136,    -1,
     146,    -1,   147,    -1,   213,   134,   339,    -1,   340,    -1,
     341,   133,   340,    -1,    -1,   341,    -1,   338,    -1,   337,
      -1,   343,    -1,   344,   133,   343,    -1,    -1,   344,    -1,
     326,    -1,   117,   329,    -1,   118,   329,    -1,   119,   326,
     133,   326,    -1,   120,   326,   133,   326,   133,   338,    -1,
     121,   135,   347,   136,    -1,   121,   135,   268,   133,   347,
     136,    -1,   127,   135,     7,   133,   347,   136,    -1,   127,
     135,   347,   133,   347,   136,    -1,   127,   135,   331,   133,
     347,   136,    -1,   123,   326,    -1,   125,   135,   326,   133,
     338,   136,    -1,   125,   135,   326,   133,   338,   133,    32,
     136,    -1,   126,   135,   326,   133,   338,   133,   338,   136,
      -1,   126,   135,   326,   133,   338,   133,   338,   133,    32,
     136,    -1,   129,   135,   130,   133,   135,   338,   188,   338,
     202,   136,   133,   326,   203,   136,    -1,   128,   346,    -1,
     166,    -1,   135,   347,   136,    -1,   346,    -1,   346,   195,
     347,    -1,   186,    -1,     8,    -1,     9,    -1,     4,    -1,
       8,    -1,     9,    -1,     4,    -1,   348,    -1,    26,    -1,
      20,    -1,    27,    -1,    22,    -1,    28,    -1,    80,    -1,
      59,    -1,    61,    -1,   356,    -1,    45,   135,     4,   133,
       4,   133,     4,   133,     4,   136,    -1,   135,     4,   133,
       4,   133,     4,   133,     4,   136,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   280,   280,   281,   284,   285,   288,   311,   316,   338,
     351,   363,   369,   399,   402,   409,   413,   420,   423,   430,
     431,   435,   438,   445,   449,   456,   459,   465,   471,   472,
     473,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   497,   498,   499,   500,   501,   502,
     503,   504,   505,   506,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   516,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   541,   542,   543,   546,   547,   550,   566,   572,
     578,   584,   590,   596,   602,   608,   614,   624,   634,   644,
     654,   664,   674,   686,   691,   698,   703,   710,   715,   722,
     726,   732,   737,   744,   748,   754,   758,   765,   787,   764,
     801,   856,   863,   866,   872,   879,   883,   892,   896,   891,
     959,   960,   964,   963,   977,   976,   991,  1001,  1002,  1005,
    1043,  1042,  1077,  1076,  1107,  1106,  1139,  1138,  1164,  1175,
    1174,  1202,  1208,  1213,  1218,  1225,  1232,  1241,  1249,  1261,
    1260,  1279,  1278,  1297,  1300,  1306,  1316,  1322,  1331,  1337,
    1342,  1348,  1353,  1359,  1370,  1376,  1377,  1380,  1381,  1384,
    1388,  1394,  1395,  1398,  1405,  1413,  1421,  1422,  1425,  1426,
    1429,  1434,  1433,  1447,  1454,  1461,  1469,  1474,  1480,  1486,
    1492,  1498,  1503,  1508,  1513,  1518,  1523,  1528,  1533,  1538,
    1543,  1548,  1556,  1565,  1569,  1582,  1591,  1590,  1608,  1618,
    1624,  1632,  1638,  1643,  1648,  1653,  1658,  1663,  1668,  1673,
    1678,  1689,  1695,  1700,  1705,  1710,  1717,  1723,  1752,  1757,
    1765,  1771,  1777,  1784,  1791,  1801,  1811,  1826,  1837,  1840,
    1846,  1852,  1858,  1864,  1870,  1875,  1882,  1889,  1895,  1901,
    1908,  1907,  1932,  1935,  1941,  1948,  1954,  1958,  1963,  1970,
    1976,  1983,  1987,  1994,  2002,  2005,  2015,  2019,  2022,  2028,
    2032,  2039,  2043,  2047,  2053,  2054,  2057,  2058,  2061,  2062,
    2063,  2069,  2070,  2071,  2077,  2078,  2081,  2090,  2095,  2102,
    2113,  2119,  2123,  2127,  2134,  2144,  2151,  2155,  2161,  2165,
    2173,  2177,  2184,  2194,  2207,  2211,  2218,  2228,  2237,  2248,
    2252,  2259,  2269,  2280,  2289,  2299,  2305,  2309,  2316,  2326,
    2337,  2346,  2356,  2363,  2364,  2370,  2374,  2378,  2382,  2390,
    2399,  2403,  2407,  2411,  2415,  2419,  2422,  2429,  2438,  2471,
    2472,  2475,  2476,  2479,  2483,  2490,  2497,  2508,  2511,  2519,
    2523,  2527,  2531,  2535,  2540,  2544,  2548,  2553,  2558,  2563,
    2567,  2572,  2577,  2581,  2585,  2590,  2594,  2601,  2607,  2611,
    2617,  2624,  2625,  2626,  2629,  2633,  2637,  2641,  2647,  2648,
    2651,  2652,  2655,  2656,  2659,  2660,  2663,  2667,  2693
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
  "LEVEL_ID", "LEV_INIT_ID", "GEOMETRY_ID", "NOMAP_ID", "OBJECT_ID",
  "COBJECT_ID", "MONSTER_ID", "TRAP_ID", "DOOR_ID", "DRAWBRIDGE_ID",
  "object_ID", "monster_ID", "terrain_ID", "MAZEWALK_ID", "WALLIFY_ID",
  "REGION_ID", "FILLING", "IRREGULAR", "JOINED", "ALTAR_ID", "ANVIL_ID",
  "LADDER_ID", "STAIR_ID", "NON_DIGGABLE_ID", "NON_PASSWALL_ID", "ROOM_ID",
  "PORTAL_ID", "TELEPRT_ID", "BRANCH_ID", "LEV", "MINERALIZE_ID",
  "CORRIDOR_ID", "GOLD_ID", "ENGRAVING_ID", "FOUNTAIN_ID", "THRONE_ID",
  "POOL_ID", "SINK_ID", "NONE", "RAND_CORRIDOR_ID", "DOOR_STATE",
  "LIGHT_STATE", "CURSE_TYPE", "ENGRAVING_TYPE", "DIRECTION",
  "RANDOM_TYPE", "RANDOM_TYPE_BRACKET", "A_REGISTER", "ALIGNMENT",
  "LEFT_OR_RIGHT", "CENTER", "TOP_OR_BOT", "ALTAR_TYPE", "UP_OR_DOWN",
  "SUBROOM_ID", "NAME_ID", "FLAGS_ID", "FLAG_TYPE", "MON_ATTITUDE",
  "MON_ALERTNESS", "MON_APPEARANCE", "ROOMDOOR_ID", "IF_ID", "ELSE_ID",
  "TERRAIN_ID", "HORIZ_OR_VERT", "REPLACE_TERRAIN_ID", "EXIT_ID",
  "SHUFFLE_ID", "QUANTITY_ID", "BURIED_ID", "LOOP_ID", "FOR_ID", "TO_ID",
  "SWITCH_ID", "CASE_ID", "BREAK_ID", "DEFAULT_ID", "ERODED_ID",
  "TRAPPED_STATE", "RECHARGED_ID", "INVIS_ID", "GREASED_ID", "FEMALE_ID",
  "CANCELLED_ID", "REVIVED_ID", "AVENGE_ID", "FLEEING_ID", "BLINDED_ID",
  "PARALYZED_ID", "STUNNED_ID", "CONFUSED_ID", "SEENTRAPS_ID", "ALL_ID",
  "MONTYPE_ID", "GRAVE_ID", "ERODEPROOF_ID", "FUNCTION_ID",
  "MSG_OUTPUT_TYPE", "COMPARE_TYPE", "UNKNOWN_TYPE", "rect_ID",
  "fillrect_ID", "line_ID", "randline_ID", "grow_ID", "selection_ID",
  "flood_ID", "rndcoord_ID", "circle_ID", "ellipse_ID", "filter_ID",
  "complement_ID", "gradient_ID", "GRADIENT_TYPE", "LIMITED",
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
  "level_def", "lev_init", "opt_limited", "opt_coord_or_var",
  "opt_fillchar", "walled", "flags", "flag_list", "levstatements",
  "stmt_block", "levstatement", "any_var_array", "any_var",
  "any_var_or_arr", "any_var_or_unk", "shuffle_detail", "variable_define",
  "encodeobj_list", "encodemonster_list", "mapchar_list",
  "encoderegion_list", "encodecoord_list", "integer_list", "string_list",
  "function_define", "$@1", "$@2", "function_call", "exitstatement",
  "opt_percent", "comparestmt", "switchstatement", "$@3", "$@4",
  "switchcases", "switchcase", "$@5", "$@6", "breakstatement",
  "for_to_span", "forstmt_start", "forstatement", "$@7", "loopstatement",
  "$@8", "chancestatement", "$@9", "ifstatement", "$@10", "if_ending",
  "$@11", "message", "random_corridors", "corridor", "corr_spec",
  "room_begin", "subroom_def", "$@12", "room_def", "$@13", "roomfill",
  "room_pos", "subroom_pos", "room_align", "room_size", "door_detail",
  "secret", "door_wall", "dir_list", "door_pos", "map_definition",
  "h_justif", "v_justif", "monster_detail", "$@14", "monster_desc",
  "monster_infos", "monster_info", "seen_trap_mask", "object_detail",
  "$@15", "object_desc", "object_infos", "object_info", "trap_detail",
  "drawbridge_detail", "mazewalk_detail", "wallify_detail",
  "ladder_detail", "stair_detail", "stair_region", "portal_region",
  "teleprt_region", "branch_region", "teleprt_detail", "fountain_detail",
  "throne_detail", "sink_detail", "pool_detail", "terrain_type",
  "replace_terrain_detail", "terrain_detail", "diggable_detail",
  "passwall_detail", "region_detail", "@16", "region_detail_end",
  "altar_detail", "anvil_detail", "grave_detail", "gold_detail",
  "engraving_detail", "mineralize", "trap_name", "room_type",
  "optroomregionflags", "roomregionflags", "roomregionflag", "door_state",
  "light_state", "alignment", "alignment_prfx", "altar_type", "a_register",
  "string_or_var", "integer_or_var", "coord_or_var", "encodecoord",
  "humidity_flags", "region_or_var", "encoderegion", "mapchar_or_var",
  "mapchar", "monster_or_var", "encodemonster", "object_or_var",
  "encodeobj", "string_expr", "math_expr_var", "func_param_type",
  "func_param_part", "func_param_list", "func_params_list",
  "func_call_param_part", "func_call_param_list", "func_call_params_list",
  "ter_selection_x", "ter_selection", "dice", "all_integers",
  "all_ints_push", "objectid", "monsterid", "terrainid", "engraving_type",
  "lev_region", "region", 0
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
     385,   386,   387,    44,    58,    40,    41,    91,    93,   123,
     125,   388,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,    43,    45,    42,
      47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   196,   197,   197,   198,   198,   199,   200,   200,   201,
     201,   201,   201,   202,   202,   203,   203,   204,   204,   205,
     205,   206,   206,   207,   207,   208,   208,   209,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   213,   213,   213,   214,   214,   215,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   217,   217,   218,   218,   219,   219,   220,
     220,   221,   221,   222,   222,   223,   223,   225,   226,   224,
     227,   228,   229,   229,   230,   230,   230,   232,   233,   231,
     234,   234,   236,   235,   237,   235,   238,   239,   239,   240,
     242,   241,   244,   243,   246,   245,   248,   247,   249,   250,
     249,   251,   252,   252,   252,   253,   253,   254,   255,   257,
     256,   259,   258,   260,   260,   261,   261,   262,   262,   263,
     263,   264,   264,   265,   265,   266,   266,   267,   267,   268,
     268,   269,   269,   270,   270,   270,   271,   271,   272,   272,
     273,   274,   273,   275,   276,   276,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   278,   278,   278,   279,   280,   279,   281,   282,
     282,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   284,   285,   286,   286,
     287,   287,   288,   289,   290,   291,   292,   293,   294,   294,
     295,   296,   297,   298,   299,   299,   300,   301,   302,   303,
     305,   304,   306,   306,   307,   308,   309,   309,   309,   310,
     311,   312,   312,   313,   313,   314,   314,   315,   315,   316,
     316,   317,   317,   317,   318,   318,   319,   319,   320,   320,
     320,   321,   321,   321,   322,   322,   323,   324,   324,   324,
     325,   326,   326,   326,   326,   327,   327,   327,   328,   328,
     329,   329,   329,   330,   331,   331,   331,   332,   332,   333,
     333,   333,   334,   334,   334,   334,   335,   335,   335,   336,
     336,   336,   336,   337,   337,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   339,   339,   340,   341,
     341,   342,   342,   343,   343,   344,   344,   345,   345,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   347,   347,
     348,   349,   349,   349,   350,   350,   350,   350,   351,   351,
     352,   352,   353,   353,   354,   354,   355,   355,   356
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     3,     3,     5,     5,
       5,     3,    16,     0,     2,     0,     2,     0,     2,     1,
       1,     0,     3,     3,     1,     0,     2,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     5,
       3,     5,     5,     5,     3,     3,     5,     5,     5,     7,
       7,     7,     5,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     0,     0,     8,
       4,     1,     0,     1,     1,     5,     3,     0,     0,     9,
       0,     2,     0,     5,     0,     4,     1,     2,     1,     6,
       0,     3,     0,     6,     0,     4,     0,     4,     1,     0,
       4,     3,     1,     3,     3,     5,     5,     7,     4,     0,
      10,     0,    12,     0,     2,     5,     1,     5,     1,     5,
       1,     5,     1,     9,     5,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     7,     5,     1,     1,     1,     1,
       3,     0,     5,     4,     0,     3,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     3,     3,     3,     1,
       1,     3,     1,     1,     3,     3,     0,     5,     2,     0,
       3,     1,     3,     1,     3,     3,     1,     1,     3,     1,
       1,     1,     3,     1,     1,     1,     5,     7,     5,     8,
       1,     3,     5,     5,     7,     7,     6,     5,     0,     2,
       3,     3,     3,     3,     1,     5,     9,     5,     3,     3,
       0,    10,     0,     1,     7,     3,     5,     5,     3,     5,
       7,     9,     1,     1,     1,     1,     1,     0,     2,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     4,     1,     1,     4,
       1,     1,     4,     1,     4,     5,     1,     3,     1,     3,
       1,     1,     4,     9,     1,     1,     4,     1,     5,     1,
       1,     4,     1,     1,     5,     1,     1,     1,     4,     1,
       1,     5,     1,     1,     3,     1,     1,     3,     1,     4,
       3,     3,     3,     3,     3,     3,     1,     1,     3,     1,
       3,     0,     1,     1,     1,     1,     3,     0,     1,     1,
       2,     2,     4,     6,     4,     6,     6,     6,     6,     2,
       6,     8,     8,    10,    14,     2,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,    10,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    21,     0,     0,     1,
       5,     0,    25,     0,     7,     0,   134,     0,     0,     0,
     193,     0,     0,     0,     0,     0,     0,     0,   250,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     282,     0,     0,     0,     0,     0,     0,     0,   162,     0,
       0,     0,     0,     0,   131,     0,     0,     0,   137,   146,
       0,     0,     0,     0,    94,    83,    74,    84,    75,    85,
      76,    86,    77,    87,    78,    88,    79,    89,    80,    90,
      81,    91,    82,    29,     6,    25,    92,    93,     0,    36,
      35,    52,    53,    50,     0,    45,    51,   150,    46,    47,
      49,    48,    28,    62,    34,    65,    64,    38,    55,    57,
      58,    72,    39,    56,    73,    54,    69,    70,    61,    71,
      33,    42,    43,    66,    60,    68,    67,    37,    59,    63,
      30,    31,    32,    44,    40,    41,     0,    24,    22,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   156,     0,     0,     0,     0,    95,    96,     0,
       0,     0,     0,   345,     0,   348,     0,   390,     0,   346,
     367,    26,     0,   154,     0,     8,     0,   307,   308,     0,
     343,   161,     0,     0,     0,    11,   316,     0,   196,   197,
       0,     0,   313,     0,     0,   173,   311,   340,   342,     0,
     339,   337,     0,   225,   229,   336,   226,   333,   335,     0,
     332,   330,     0,   200,     0,   329,   284,   283,     0,   294,
     295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   386,   369,   388,   251,
       0,   321,     0,     0,   320,     0,   275,     0,     0,     0,
       0,     0,   406,   268,   269,   286,   285,     0,   132,     0,
       0,     0,     0,     0,   310,     0,     0,     0,     0,   260,
     261,   263,   262,   393,   391,   392,   164,   163,     0,   185,
     186,     0,     0,     0,     0,    97,     0,     0,     0,   278,
     127,     0,     0,     0,     0,   136,     0,     0,     0,     0,
       0,   364,   363,   365,   368,     0,   399,   401,   398,   400,
     402,   403,     0,     0,     0,   104,   105,   100,    98,     0,
       0,     0,     0,    25,   151,    23,     0,     0,     0,     0,
       0,   318,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   370,   371,     0,     0,     0,   379,     0,     0,     0,
     385,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   158,   157,     0,
       0,   152,     0,     0,     0,   361,   347,   355,     0,     0,
     350,   351,   352,   353,   354,     0,   130,     0,   345,     0,
       0,     0,     0,   121,   119,   125,   123,     0,     0,     0,
     155,     0,     0,   344,    10,   264,     0,     9,     0,     0,
     317,     0,     0,     0,   199,   198,   173,   174,   195,     0,
       0,     0,   227,     0,     0,   202,   204,   246,   184,     0,
     248,     0,     0,   189,     0,     0,     0,     0,   327,     0,
       0,   325,     0,     0,   324,     0,     0,   387,   389,     0,
       0,   296,   297,     0,   300,     0,   298,     0,   299,   252,
       0,     0,   253,     0,   176,     0,     0,     0,     0,     0,
     258,   257,     0,     0,   165,   166,   279,   404,   405,     0,
     178,     0,     0,     0,     0,     0,   267,     0,     0,   148,
       0,     0,   138,   277,   276,     0,   359,   362,     0,   349,
     135,   366,    99,     0,     0,   108,     0,   107,     0,   106,
       0,   112,     0,   103,     0,   102,     0,   101,    27,   309,
       0,     0,   319,   312,     0,   314,     0,     0,   338,   396,
     394,   395,   240,   237,   231,     0,     0,   236,     0,   241,
       0,   243,   244,     0,   239,   230,   245,   397,   233,     0,
     331,   203,     0,     0,   372,     0,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   322,     0,
       0,     0,     0,     0,     0,     0,     0,   168,     0,     0,
       0,   256,     0,     0,     0,     0,     0,     0,     0,     0,
     153,   147,   149,     0,     0,     0,   128,     0,   120,   122,
     124,   126,     0,   113,     0,   115,     0,   117,     0,     0,
     315,   194,   341,     0,     0,     0,     0,     0,   334,     0,
     247,    17,     0,   190,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   287,     0,   305,   304,   274,     0,
       0,   254,     0,   180,     0,     0,   255,   259,     0,     0,
     280,     0,   182,     0,   287,   188,     0,   187,   160,     0,
     140,   356,   357,   358,   360,     0,     0,   111,     0,   110,
       0,   109,     0,     0,   234,   235,   238,   242,   232,     0,
     301,   207,   208,     0,   212,   211,   213,   214,   215,     0,
       0,     0,   219,   220,     0,   205,   209,   302,   206,     0,
     249,   373,   375,     0,   380,     0,   376,     0,   326,   378,
     377,     0,     0,     0,   270,   306,     0,     0,     0,     0,
       0,     0,   191,   192,     0,     0,     0,   169,     0,     0,
       0,     0,     0,   140,   129,   114,   116,   118,   265,     0,
       0,   210,     0,     0,     0,     0,    18,     0,     0,   328,
       0,     0,   291,   292,   293,   288,   289,   272,     0,     0,
     175,     0,   287,   281,   167,   177,     0,     0,   183,   266,
       0,   144,   139,   141,     0,   303,   216,   217,   218,   223,
     222,   221,   381,     0,   382,   351,     0,     0,   273,   271,
       0,     0,     0,   171,     0,   170,   142,    25,     0,     0,
       0,     0,     0,   323,   290,     0,   408,   179,     0,   181,
      25,   145,     0,   224,   383,    14,     0,   407,   172,   143,
       0,     0,     0,    15,    19,    20,    17,     0,     0,    12,
      16,   384
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    83,   842,   868,   740,   866,
      12,   138,    84,   344,    85,    86,    87,    88,   179,    89,
      90,   642,   644,   646,   429,   430,   431,   432,    91,   415,
     705,    92,    93,   395,    94,    95,   180,   633,   772,   773,
     850,   837,    96,   531,    97,    98,   194,    99,   528,   100,
     342,   101,   302,   408,   524,   102,   103,   104,   286,   277,
     105,   807,   106,   848,   358,   506,   522,   685,   694,   107,
     301,   696,   474,   764,   108,   214,   456,   109,   365,   233,
     591,   735,   821,   110,   362,   223,   361,   585,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   621,   121,
     122,   123,   124,   447,   125,   126,   127,   128,   129,   797,
     829,   130,   131,   132,   133,   134,   135,   238,   278,   754,
     795,   796,   241,   493,   497,   736,   678,   498,   200,   283,
     257,   216,   352,   263,   264,   483,   484,   234,   235,   224,
     225,   321,   284,   703,   536,   537,   538,   323,   324,   325,
     258,   382,   189,   297,   588,   339,   340,   341,   519,   271,
     272
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -671
static const yytype_int16 yypact[] =
{
     218,   -44,    10,   117,  -671,   218,    50,    29,    33,  -671,
    -671,    86,   616,   116,  -671,   169,  -671,    90,   159,   167,
    -671,   181,   182,   190,   191,   196,   197,   200,   205,   211,
     233,   236,   239,   240,   242,   255,   256,   263,   270,   272,
     275,   277,   278,   281,   301,   304,   307,   311,   313,   314,
     315,    21,   317,   319,  -671,   320,   209,   741,  -671,  -671,
     323,   296,    59,   250,  -671,  -671,  -671,  -671,  -671,  -671,
    -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,
    -671,  -671,  -671,  -671,  -671,   616,  -671,  -671,   266,  -671,
    -671,  -671,  -671,  -671,   330,  -671,  -671,  -671,  -671,  -671,
    -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,
    -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,
    -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,
    -671,  -671,  -671,  -671,  -671,  -671,   463,   337,  -671,   -70,
     352,   326,    52,    52,   111,   -26,    91,     7,     7,   669,
     109,     7,     7,     7,   251,   109,   109,   -18,   -17,   -17,
     -17,    59,   336,    59,     7,   669,   669,   669,   669,    76,
     -18,    44,  -671,   669,   109,   743,    59,  -671,  -671,   283,
     341,     7,   344,  -671,    18,  -671,   343,  -671,   212,  -671,
      62,  -671,    32,  -671,   342,  -671,   169,  -671,  -671,   350,
    -671,   321,   356,   357,   374,  -671,  -671,   376,  -671,  -671,
     381,   529,  -671,   397,   402,   405,  -671,  -671,  -671,   536,
    -671,  -671,   407,  -671,  -671,  -671,  -671,  -671,  -671,   544,
    -671,  -671,   411,   410,   417,  -671,  -671,  -671,   426,  -671,
    -671,   432,   433,   440,   109,   109,     7,     7,   453,     7,
     454,   455,   457,   669,   466,   401,  -671,  -671,   408,  -671,
     598,  -671,   467,   473,  -671,   476,  -671,   477,   481,   607,
     486,   487,  -671,  -671,  -671,  -671,  -671,   488,   612,   620,
     492,   493,   494,   496,   394,   627,   499,   -89,   509,  -671,
    -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,   510,  -671,
    -671,   511,   342,   515,   516,  -671,   512,    59,    59,   537,
    -671,   546,   409,    59,    59,  -671,    59,    59,    59,    59,
      59,   321,   394,  -671,   550,   548,  -671,  -671,  -671,  -671,
    -671,  -671,   553,    57,    25,  -671,  -671,   321,   394,   555,
     558,   563,   616,   616,  -671,  -671,    59,   -70,   688,    27,
     698,   569,   571,   669,   574,    59,   187,   715,   579,   590,
      59,   591,   342,   592,    59,   342,     7,     7,   669,   666,
     672,  -671,  -671,   617,   618,   451,  -671,     7,     7,   299,
    -671,   619,   625,   669,   629,    59,    68,   274,   694,   760,
     651,   716,   -17,   -11,  -671,   658,   670,   -17,   -17,   -17,
      59,   673,    88,     7,   297,     1,    91,   723,  -671,    61,
      61,  -671,   130,   667,   -13,   703,  -671,  -671,   485,   490,
     204,   204,  -671,  -671,  -671,    62,  -671,   669,   674,   -45,
     -29,   -14,    17,  -671,  -671,   321,   394,   140,   305,    20,
    -671,   668,   523,  -671,  -671,  -671,   806,  -671,   677,   376,
    -671,   678,   811,   547,  -671,  -671,   405,  -671,  -671,   675,
     552,   286,  -671,   676,   557,  -671,  -671,  -671,  -671,   685,
     686,     7,     7,   626,   689,   687,   691,   695,  -671,   696,
     375,  -671,   684,   697,  -671,   699,   700,  -671,  -671,   823,
     567,  -671,  -671,   713,  -671,   734,  -671,   739,  -671,  -671,
     740,   827,  -671,   742,  -671,   870,   744,    68,   872,   746,
     747,  -671,   748,   822,  -671,  -671,  -671,  -671,  -671,   750,
    -671,   880,   753,   754,   809,   889,  -671,   776,   342,  -671,
     718,    59,  -671,  -671,   321,   778,  -671,   780,   779,  -671,
    -671,  -671,  -671,   910,   781,  -671,    14,  -671,    59,  -671,
     -70,  -671,   100,  -671,   145,  -671,    64,  -671,  -671,  -671,
     784,   915,  -671,  -671,   783,  -671,   782,   785,  -671,  -671,
    -671,  -671,  -671,  -671,  -671,   786,   788,  -671,   789,  -671,
     791,  -671,  -671,   792,  -671,  -671,  -671,  -671,  -671,   793,
    -671,   794,    91,   923,  -671,   797,   871,   669,  -671,    59,
      59,   669,   799,    59,   669,   669,   798,   801,  -671,   -18,
     931,    73,   932,   133,   868,   805,    11,  -671,   807,   800,
     873,  -671,    59,   810,   -70,   812,    16,   222,   342,    61,
    -671,  -671,   394,   808,   141,   703,  -671,   178,  -671,  -671,
     394,   321,    46,  -671,   151,  -671,   152,  -671,    68,   813,
    -671,  -671,  -671,   -70,    59,    59,    59,   111,  -671,   737,
    -671,   815,    59,  -671,   803,   164,   364,   814,    68,   661,
     816,   817,    59,   940,   818,   819,  -671,  -671,  -671,   821,
     945,  -671,   951,  -671,   244,   825,  -671,  -671,   826,    30,
     321,   952,  -671,   956,   818,  -671,   828,  -671,  -671,   829,
     -12,  -671,  -671,  -671,  -671,   342,   100,  -671,   145,  -671,
      64,  -671,   830,   958,   321,  -671,  -671,  -671,  -671,   192,
    -671,  -671,  -671,   -70,  -671,  -671,  -671,  -671,  -671,   831,
     833,   834,  -671,  -671,   835,  -671,  -671,  -671,   321,   961,
    -671,   394,  -671,   938,  -671,    59,  -671,   836,  -671,  -671,
    -671,   528,   838,   399,  -671,  -671,   969,   841,   839,   843,
      16,    59,  -671,  -671,   842,   844,   846,  -671,    30,   970,
     112,   847,   845,   -12,  -671,  -671,  -671,  -671,  -671,   849,
     922,   321,    59,    59,    59,   -52,  -671,   848,   373,  -671,
      59,   982,  -671,  -671,  -671,  -671,   854,   342,   855,   985,
    -671,   187,   818,  -671,  -671,  -671,   986,   342,  -671,  -671,
     857,  -671,  -671,  -671,   987,  -671,  -671,  -671,  -671,  -671,
     802,  -671,  -671,   962,  -671,   404,   859,   399,  -671,  -671,
     989,   861,   862,  -671,   863,  -671,  -671,   616,   867,   -52,
     865,   874,   866,  -671,  -671,   875,  -671,  -671,   342,  -671,
     616,  -671,    68,  -671,  -671,  -671,   876,  -671,  -671,  -671,
     877,     7,    69,   879,  -671,  -671,   815,     7,   878,  -671,
    -671,  -671
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -671,  -671,   998,  -671,  -671,  -671,  -671,  -671,   138,  -671,
    -671,   820,   -85,  -292,   664,   832,   960,  -404,  -671,  -671,
    -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,
    -671,  -671,  -671,  -671,   957,  -671,  -671,  -671,   245,  -671,
    -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,
    -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,   611,   850,
    -671,  -671,  -671,  -671,   559,  -671,  -671,  -671,   259,  -671,
    -671,  -671,  -518,   253,  -671,   338,   223,  -671,  -671,  -671,
    -671,  -671,   184,  -671,  -671,   882,  -671,  -671,  -671,  -671,
    -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,
    -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,
    -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,   418,  -670,
     199,  -671,  -390,  -503,  -671,  -671,  -671,   369,   682,  -170,
    -139,  -314,   581,   185,  -315,  -392,  -515,  -431,  -508,   594,
    -501,  -136,   -57,  -671,   398,  -671,  -671,   609,  -671,  -671,
     787,  -128,   575,  -401,  -671,  -671,  -671,  -671,  -671,  -127,
    -671
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -202
static const yytype_int16 yytable[] =
{
     191,   515,   215,   201,   617,   188,   306,   555,   242,   243,
     407,   535,   265,   266,   267,   270,   523,   526,   527,   434,
     433,   259,   183,   478,   767,   288,   311,    16,   268,   183,
     445,   280,   281,   282,   762,   236,   183,   289,   290,   291,
     292,   647,   309,   275,   403,   303,   645,   557,   533,   299,
     504,   643,   326,   335,   327,   217,   337,   819,   328,   329,
     330,   428,   520,   183,   478,   311,   183,   478,   206,   207,
     462,   197,   683,   465,   864,   206,   207,   692,   663,   770,
     293,   771,   198,   199,   294,   295,   206,   207,   544,   820,
       7,   763,   293,   206,   207,   545,   294,   295,   316,   317,
     318,   319,   320,   217,   546,   300,   287,   373,   374,   697,
     376,   547,   331,   218,   227,   237,   293,     9,   279,   548,
     294,   295,    11,   276,   505,   491,   549,   312,   197,   492,
     865,   210,   833,   322,   676,   338,   521,   296,   413,   198,
     199,   677,   211,   217,     8,   712,   684,   239,   227,   211,
     550,   693,   240,   184,   332,   525,   210,   551,    62,   556,
     333,   218,   446,   212,   213,   747,   197,   333,   185,   186,
      13,   334,   228,   197,    14,   185,   186,   198,   199,   706,
     481,   482,   185,   186,   198,   199,   707,   219,   212,   213,
     261,   262,   184,   220,   184,   777,   525,   184,   435,   525,
     776,   218,   660,   197,   187,   775,   228,   185,   186,   185,
     186,   187,   185,   186,   198,   199,   221,   222,   187,   529,
      15,   481,   482,   285,   139,   451,   718,   466,   467,   638,
     512,   535,   639,     1,     2,   219,   630,   699,   476,   477,
     468,   220,   137,   187,   260,   187,   229,   475,   187,   136,
     412,   485,   230,   454,   455,   488,   418,   419,   441,   420,
     421,   422,   423,   424,   516,   503,   680,   261,   262,   650,
     509,   510,   511,   231,   232,   219,   312,   436,   534,   552,
     229,   220,   473,   695,   708,   710,   230,   701,   702,   442,
     569,   709,   711,   140,   570,   571,   268,   743,   453,   542,
     744,   141,   478,   460,   221,   222,   479,   464,   227,   208,
     209,   673,   206,   207,   650,   142,   143,   316,   317,   318,
     319,   320,   586,   530,   144,   145,   780,   314,   490,   610,
     146,   147,   594,   595,   148,   494,   698,   495,   496,   149,
     273,   274,   572,   573,   574,   150,   176,   206,   207,   860,
     315,   316,   317,   318,   319,   320,   517,   575,   518,   304,
     206,   207,   202,   203,   204,   205,   228,   151,   322,   810,
     152,   576,   577,   153,   154,   210,   155,   336,   602,   354,
     578,   579,   580,   581,   582,   190,   269,   206,   207,   156,
     157,   208,   209,   318,   319,   320,   583,   158,   584,   316,
     317,   318,   319,   320,   159,   354,   160,   212,   213,   161,
     210,   162,   163,   774,   641,   164,   244,   245,   246,   247,
     248,   211,   249,   210,   250,   251,   252,   253,   254,   371,
     372,   792,   793,   794,   480,   165,   206,   207,   166,   182,
     229,   167,   212,   213,   554,   168,   230,   169,   170,   171,
     210,   173,   688,   174,   175,   212,   213,   181,   192,   481,
     482,   211,   206,   207,   193,   256,   195,   231,   232,   664,
     196,   285,   187,   667,   632,   307,   670,   671,   308,   310,
     313,   343,   212,   213,   715,   716,   717,   346,   690,   348,
     349,   640,   244,   245,   246,   247,   248,   745,   249,   210,
     250,   251,   252,   253,   254,   828,   823,   350,   351,   824,
     255,   473,   206,   207,   347,   835,   353,   714,   244,   245,
     246,   247,   248,   738,   249,   210,   250,   251,   252,   253,
     254,   212,   213,   354,   355,   356,   255,   841,   357,   359,
     -13,   256,   665,   666,   360,   417,   669,   363,   364,  -201,
     366,   316,   317,   318,   319,   320,   858,   212,   213,   367,
     316,   317,   318,   319,   320,   368,   369,   256,   244,   245,
     246,   247,   248,   370,   249,   210,   250,   251,   252,   253,
     254,   316,   317,   318,   319,   320,   255,   781,   375,   377,
     378,   803,   379,   318,   319,   320,   316,   317,   318,   319,
     320,   381,   384,   383,   385,   741,   386,   212,   213,   387,
     388,   390,   816,   817,   818,   751,   389,   256,   394,   391,
     392,   393,    16,   539,   396,   397,   398,   399,   540,   400,
      17,   401,   402,    18,    19,    20,    21,    22,    23,    24,
      25,    26,   404,   405,   406,    27,    28,    29,   409,   410,
     411,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   559,    40,    41,    42,    43,    44,    45,    46,    47,
     414,    48,   316,   317,   318,   319,   320,   316,   317,   318,
     319,   320,   416,   425,   426,   565,    49,   427,   788,   437,
     568,   444,   438,    50,    51,   590,    52,   439,    53,    54,
      55,   448,   449,    56,    57,   608,    58,   452,    59,   450,
     316,   317,   318,   319,   320,   316,   790,   318,   319,   320,
     457,   458,   863,   459,   461,   463,   469,    60,   870,    61,
     206,   207,   470,   825,   316,   317,   318,   319,   320,   316,
     317,   318,   319,   320,   316,   317,   318,   319,   320,   486,
     471,   472,   851,    62,   316,   317,   318,   319,   320,    63,
      64,   487,   489,   499,   500,   859,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,   501,   502,   244,   245,   246,   247,
     248,   507,   249,   210,   250,   251,   252,   253,   254,   748,
     719,   720,  -159,   508,   255,   532,   513,   543,   558,   560,
     561,   721,   722,   723,   563,   564,   567,   589,   592,   593,
     596,   603,   597,   598,   599,   212,   213,   607,   600,   601,
     604,   613,   605,   606,   724,   256,   725,   726,   727,   728,
     729,   730,   731,   732,   733,   734,   609,    64,   316,   317,
     318,   319,   320,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,   610,   611,   612,   615,   614,   618,   616,   197,   619,
     620,   622,   623,   624,   625,   177,   626,   627,   628,   198,
     199,    65,   602,    67,    66,    69,    68,    71,    70,    73,
      72,    75,    74,    77,    76,    79,    78,    81,    80,   629,
      82,   631,   634,   635,   637,   636,   260,   648,   649,   650,
     653,   652,   654,   655,   651,   656,   657,   659,   661,   658,
     662,   473,   668,   672,   673,   675,   679,   681,   682,   742,
     680,   686,   687,   689,   752,   691,   713,   700,   739,   757,
     746,   753,   749,   750,   756,   758,   765,   755,   760,   761,
     766,   768,   769,   779,   786,   782,   778,   783,   784,   785,
     787,   791,   789,   798,   799,   800,   801,   809,   804,   806,
     805,   811,   814,   815,   822,   812,   826,   827,   830,   831,
     834,   836,   838,   845,   840,   843,   839,   846,   847,   849,
     852,   854,   856,    10,   869,   855,   440,   305,   172,   861,
     862,   857,   867,   514,   871,   566,   345,   178,   813,   802,
     298,   808,   759,   853,   832,   226,   844,   674,   737,   443,
     562,   553,     0,   704,   541,     0,   587,     0,     0,     0,
     380
};

static const yytype_int16 yycheck[] =
{
      85,   402,   141,   139,   507,    62,   176,   438,   147,   148,
     302,   415,   151,   152,   153,   154,   406,   409,   410,   334,
     334,   149,     4,     3,   694,   164,     8,     6,    45,     4,
       3,   158,   159,   160,     4,    61,     4,   165,   166,   167,
     168,   556,   181,    61,   133,   173,   554,   439,    61,     5,
      61,   552,    20,   192,    22,     3,   192,   109,    26,    27,
      28,     4,    61,     4,     3,     8,     4,     3,    61,    62,
     362,   141,    61,   365,     5,    61,    62,    61,   596,    91,
       4,    93,   152,   153,     8,     9,    61,    62,   133,   141,
     134,    61,     4,    61,    62,   140,     8,     9,   187,   188,
     189,   190,   191,     3,   133,    61,   163,   246,   247,   627,
     249,   140,    80,    61,     3,   141,     4,     0,   135,   133,
       8,     9,    72,   141,   135,    57,   140,   184,   141,    61,
      61,   124,   802,   190,    61,   192,   135,    61,   308,   152,
     153,    68,   135,     3,   134,   648,   135,    56,     3,   135,
     133,   135,    61,   135,   122,   135,   124,   140,   137,   139,
     135,    61,   135,   156,   157,   668,   141,   135,   150,   151,
     141,   139,    61,   141,   141,   150,   151,   152,   153,   133,
     160,   161,   150,   151,   152,   153,   140,   135,   156,   157,
     158,   159,   135,   141,   135,   710,   135,   135,   334,   135,
     708,    61,   592,   141,   186,   706,    61,   150,   151,   150,
     151,   186,   150,   151,   152,   153,   164,   165,   186,    89,
     134,   160,   161,   135,   134,   353,   657,   366,   367,   544,
     400,   635,   546,    15,    16,   135,   528,   629,   377,   378,
     368,   141,    73,   186,   135,   186,   135,   375,   186,   133,
     307,   379,   141,    66,    67,   383,   313,   314,   343,   316,
     317,   318,   319,   320,   403,   392,   133,   158,   159,   136,
     397,   398,   399,   162,   163,   135,   333,   334,   414,   139,
     135,   141,    60,    61,   133,   133,   141,   146,   147,   346,
       4,   140,   140,   134,     8,     9,    45,   133,   355,   427,
     136,   134,     3,   360,   164,   165,     7,   364,     3,    65,
      66,   133,    61,    62,   136,   134,   134,   187,   188,   189,
     190,   191,   461,   193,   134,   134,   134,   115,   385,   137,
     134,   134,   471,   472,   134,    61,   628,    63,    64,   134,
     155,   156,    56,    57,    58,   134,   137,    61,    62,   852,
     138,   187,   188,   189,   190,   191,    59,    71,    61,   174,
      61,    62,    10,    11,    12,    13,    61,   134,   425,   770,
     134,    85,    86,   134,   134,   124,   134,   192,     3,     4,
      94,    95,    96,    97,    98,   135,   135,    61,    62,   134,
     134,    65,    66,   189,   190,   191,   110,   134,   112,   187,
     188,   189,   190,   191,   134,     4,   134,   156,   157,   134,
     124,   134,   134,   705,   550,   134,   117,   118,   119,   120,
     121,   135,   123,   124,   125,   126,   127,   128,   129,   244,
     245,    32,    33,    34,   135,   134,    61,    62,   134,   143,
     135,   134,   156,   157,   139,   134,   141,   134,   134,   134,
     124,   134,   622,   134,   134,   156,   157,   134,   192,   160,
     161,   135,    61,    62,   134,   166,     3,   162,   163,   597,
     133,   135,   186,   601,   531,   192,   604,   605,   137,   135,
     137,   139,   156,   157,   654,   655,   656,   137,   624,   133,
     133,   548,   117,   118,   119,   120,   121,   133,   123,   124,
     125,   126,   127,   128,   129,   797,   133,   133,   132,   136,
     135,    60,    61,    62,   193,   807,   135,   653,   117,   118,
     119,   120,   121,   659,   123,   124,   125,   126,   127,   128,
     129,   156,   157,     4,   137,   133,   135,   133,   133,     3,
     136,   166,   599,   600,   137,   136,   603,     3,   137,   139,
     133,   187,   188,   189,   190,   191,   848,   156,   157,   133,
     187,   188,   189,   190,   191,   133,   133,   166,   117,   118,
     119,   120,   121,   133,   123,   124,   125,   126,   127,   128,
     129,   187,   188,   189,   190,   191,   135,   723,   135,   135,
     135,   761,   135,   189,   190,   191,   187,   188,   189,   190,
     191,   135,     4,   195,   137,   662,   133,   156,   157,   133,
     133,     4,   782,   783,   784,   672,   135,   166,     6,   133,
     133,   133,     6,   138,     4,   133,   133,   133,   138,   133,
      14,     4,   133,    17,    18,    19,    20,    21,    22,    23,
      24,    25,   133,   133,   133,    29,    30,    31,   133,   133,
     138,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,   138,    46,    47,    48,    49,    50,    51,    52,    53,
     133,    55,   187,   188,   189,   190,   191,   187,   188,   189,
     190,   191,   136,   133,   136,   138,    70,   134,   745,   134,
     138,     3,   134,    77,    78,   138,    80,   134,    82,    83,
      84,     3,   133,    87,    88,   138,    90,   133,    92,   138,
     187,   188,   189,   190,   191,   187,   188,   189,   190,   191,
       5,   142,   861,   133,   133,   133,    60,   111,   867,   113,
      61,    62,    60,   790,   187,   188,   189,   190,   191,   187,
     188,   189,   190,   191,   187,   188,   189,   190,   191,   130,
     133,   133,   837,   137,   187,   188,   189,   190,   191,   143,
     144,   136,   133,    69,     4,   850,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   133,    69,   117,   118,   119,   120,
     121,   133,   123,   124,   125,   126,   127,   128,   129,   138,
      63,    64,    79,   133,   135,   138,   133,   133,   140,     3,
     133,    74,    75,    76,   136,     4,   141,   141,   133,   133,
     194,   137,   133,   136,   133,   156,   157,     4,   133,   133,
     133,     4,   133,   133,    97,   166,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   133,   144,   187,   188,
     189,   190,   191,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   137,   133,   133,     4,   133,     4,   133,   141,   133,
     133,   133,    60,   133,     4,   144,   133,   133,    79,   152,
     153,   150,     3,   152,   151,   154,   153,   156,   155,   158,
     157,   160,   159,   162,   161,   164,   163,   166,   165,   133,
     167,   193,   134,   133,     4,   136,   135,   133,     3,   136,
     134,   136,   134,   134,   142,   134,   134,   133,     5,   136,
     133,    60,   133,   135,   133,     4,     4,    69,   133,   136,
     133,   141,    69,   133,     4,   133,   133,   139,   133,     4,
     136,   133,   136,   136,   133,     4,     4,   138,   133,   133,
       4,   133,   133,     5,     3,   134,   136,   134,   134,   134,
      32,   133,   136,     4,   133,   136,   133,     7,   136,   133,
     136,   134,   133,    61,   136,   140,     4,   133,   133,     4,
       4,   134,     5,     4,    32,   136,   194,   136,   136,   136,
     133,   136,   136,     5,   866,   131,   342,   175,    51,   133,
     133,   136,   133,   402,   136,   456,   196,    57,   773,   760,
     170,   768,   684,   839,   801,   143,   827,   609,   659,   347,
     449,   437,    -1,   635,   425,    -1,   461,    -1,    -1,    -1,
     253
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   197,   198,   199,   200,   134,   134,     0,
     198,    72,   206,   141,   141,   134,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    29,    30,    31,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      46,    47,    48,    49,    50,    51,    52,    53,    55,    70,
      77,    78,    80,    82,    83,    84,    87,    88,    90,    92,
     111,   113,   137,   143,   144,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   201,   208,   210,   211,   212,   213,   215,
     216,   224,   227,   228,   230,   231,   238,   240,   241,   243,
     245,   247,   251,   252,   253,   256,   258,   265,   270,   273,
     279,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   295,   296,   297,   298,   300,   301,   302,   303,   304,
     307,   308,   309,   310,   311,   312,   133,    73,   207,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   230,   134,   134,   134,   137,   144,   212,   214,
     232,   134,   143,     4,   135,   150,   151,   186,   338,   348,
     135,   208,   192,   134,   242,     3,   133,   141,   152,   153,
     324,   337,    10,    11,    12,    13,    61,    62,    65,    66,
     124,   135,   156,   157,   271,   326,   327,     3,    61,   135,
     141,   164,   165,   281,   335,   336,   281,     3,    61,   135,
     141,   162,   163,   275,   333,   334,    61,   141,   313,    56,
      61,   318,   326,   326,   117,   118,   119,   120,   121,   123,
     125,   126,   127,   128,   129,   135,   166,   326,   346,   347,
     135,   158,   159,   329,   330,   326,   326,   326,    45,   135,
     326,   355,   356,   329,   329,    61,   141,   255,   314,   135,
     355,   355,   355,   325,   338,   135,   254,   338,   326,   347,
     347,   347,   347,     4,     8,     9,    61,   349,   255,     5,
      61,   266,   248,   347,   329,   211,   325,   192,   137,   326,
     135,     8,   338,   137,   115,   138,   187,   188,   189,   190,
     191,   337,   338,   343,   344,   345,    20,    22,    26,    27,
      28,    80,   122,   135,   139,   326,   329,   337,   338,   351,
     352,   353,   246,   139,   209,   207,   137,   193,   133,   133,
     133,   132,   328,   135,     4,   137,   133,   133,   260,     3,
     137,   282,   280,     3,   137,   274,   133,   133,   133,   133,
     133,   329,   329,   326,   326,   135,   326,   135,   135,   135,
     346,   135,   347,   195,     4,   137,   133,   133,   133,   135,
       4,   133,   133,   133,     6,   229,     4,   133,   133,   133,
     133,     4,   133,   133,   133,   133,   133,   209,   249,   133,
     133,   138,   338,   325,   133,   225,   136,   136,   338,   338,
     338,   338,   338,   338,   338,   133,   136,   134,     4,   220,
     221,   222,   223,   327,   330,   337,   338,   134,   134,   134,
     210,   208,   338,   324,     3,     3,   135,   299,     3,   133,
     138,   347,   133,   338,    66,    67,   272,     5,   142,   133,
     338,   133,   209,   133,   338,   209,   326,   326,   347,    60,
      60,   133,   133,    60,   268,   347,   326,   326,     3,     7,
     135,   160,   161,   331,   332,   347,   130,   136,   347,   133,
     338,    57,    61,   319,    61,    63,    64,   320,   323,    69,
       4,   133,    69,   355,    61,   135,   261,   133,   133,   355,
     355,   355,   325,   133,   254,   349,   326,    59,    61,   354,
      61,   135,   262,   318,   250,   135,   331,   331,   244,    89,
     193,   239,   138,    61,   337,   213,   340,   341,   342,   138,
     138,   343,   347,   133,   133,   140,   133,   140,   133,   140,
     133,   140,   139,   335,   139,   333,   139,   331,   140,   138,
       3,   133,   328,   136,     4,   138,   260,   141,   138,     4,
       8,     9,    56,    57,    58,    71,    85,    86,    94,    95,
      96,    97,    98,   110,   112,   283,   326,   348,   350,   141,
     138,   276,   133,   133,   326,   326,   194,   133,   136,   133,
     133,   133,     3,   137,   133,   133,   133,     4,   138,   133,
     137,   133,   133,     4,   133,     4,   133,   319,     4,   133,
     133,   294,   133,    60,   133,     4,   133,   133,    79,   133,
     209,   193,   338,   233,   134,   133,   136,     4,   330,   327,
     338,   337,   217,   336,   218,   334,   219,   332,   133,     3,
     136,   142,   136,   134,   134,   134,   134,   134,   136,   133,
     318,     5,   133,   268,   347,   338,   338,   347,   133,   338,
     347,   347,   135,   133,   314,     4,    61,    68,   322,     4,
     133,    69,   133,    61,   135,   263,   141,    69,   325,   133,
     337,   133,    61,   135,   264,    61,   267,   268,   209,   331,
     139,   146,   147,   339,   340,   226,   133,   140,   133,   140,
     133,   140,   319,   133,   337,   325,   325,   325,   333,    63,
      64,    74,    75,    76,    97,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   277,   321,   323,   337,   133,
     204,   338,   136,   133,   136,   133,   136,   319,   138,   136,
     136,   338,     4,   133,   315,   138,   133,     4,     4,   271,
     133,   133,     4,    61,   269,     4,     4,   315,   133,   133,
      91,    93,   234,   235,   209,   336,   334,   332,   136,     5,
     134,   337,   134,   134,   134,   134,     3,    32,   338,   136,
     188,   133,    32,    33,    34,   316,   317,   305,     4,   133,
     136,   133,   264,   325,   136,   136,   133,   257,   269,     7,
     349,   134,   140,   234,   133,    61,   325,   325,   325,   109,
     141,   278,   136,   133,   136,   338,     4,   133,   209,   306,
     133,     4,   272,   315,     4,   209,   134,   237,     5,   194,
      32,   133,   202,   136,   316,     4,   136,   136,   259,   136,
     236,   208,   133,   278,   136,   131,   136,   136,   209,   208,
     319,   133,   133,   326,     5,    61,   205,   133,   203,   204,
     326,   136
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
#line 289 "lev_comp.y"
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
#line 312 "lev_comp.y"
    {
		      start_level_def(&splev, (yyvsp[(3) - (3)].map));
		      (yyval.map) = (yyvsp[(3) - (3)].map);
		  ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 317 "lev_comp.y"
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
#line 339 "lev_comp.y"
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
#line 352 "lev_comp.y"
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
#line 364 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_ROGUE,0,0,0,
					  0,0,0,0, SPO_INITLEVEL));
		  ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 370 "lev_comp.y"
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
		      if (joined && fg != CORR && fg != ROOM && fg != GRASS && fg != GROUND)
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
#line 399 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 403 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 409 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_COPY));
		      (yyval.i) = 0;
		  ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 414 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 420 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 424 "lev_comp.y"
    {
		      (yyval.i) = what_map_char((char) (yyvsp[(2) - (2)].i));
		  ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 435 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 439 "lev_comp.y"
    {
		      add_opvars(splev, "io",
                                 VA_PASS2((int) (yyvsp[(3) - (3)].i), SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 446 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 450 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 456 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 460 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 466 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (3)].i);
		  ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 551 "lev_comp.y"
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

  case 98:

/* Line 1455 of yacc.c  */
#line 567 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 573 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 579 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 585 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 591 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 597 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 603 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 609 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 615 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 625 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 635 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 645 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 655 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 665 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 675 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 687 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 692 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 699 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 704 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 711 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 716 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 723 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 727 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 733 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 738 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 745 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 749 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 755 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 759 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 765 "lev_comp.y"
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

  case 128:

/* Line 1455 of yacc.c  */
#line 787 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 791 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 802 "lev_comp.y"
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

  case 131:

/* Line 1455 of yacc.c  */
#line 857 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 863 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 867 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 873 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 880 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 884 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 892 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 896 "lev_comp.y"
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

  case 139:

/* Line 1455 of yacc.c  */
#line 919 "lev_comp.y"
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

  case 142:

/* Line 1455 of yacc.c  */
#line 964 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 974 "lev_comp.y"
    {
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 977 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 987 "lev_comp.y"
    {
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 992 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 1006 "lev_comp.y"
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

  case 150:

/* Line 1455 of yacc.c  */
#line 1043 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 1048 "lev_comp.y"
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

  case 152:

/* Line 1455 of yacc.c  */
#line 1077 "lev_comp.y"
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

  case 153:

/* Line 1455 of yacc.c  */
#line 1091 "lev_comp.y"
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

  case 154:

/* Line 1455 of yacc.c  */
#line 1107 "lev_comp.y"
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

  case 155:

/* Line 1455 of yacc.c  */
#line 1127 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1139 "lev_comp.y"
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

  case 157:

/* Line 1455 of yacc.c  */
#line 1159 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1165 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1175 "lev_comp.y"
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

  case 160:

/* Line 1455 of yacc.c  */
#line 1193 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1203 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1209 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1214 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1219 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1226 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1233 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1242 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1250 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1261 "lev_comp.y"
    {
		      long rflags = (yyvsp[(8) - (8)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      add_opvars(splev, "iiiiiiio",
				 VA_PASS8(rflags, ERR, ERR,
					  (yyvsp[(5) - (8)].crd).x, (yyvsp[(5) - (8)].crd).y, (yyvsp[(7) - (8)].sze).width, (yyvsp[(7) - (8)].sze).height,
					  SPO_SUBROOM));
		      break_stmt_start();
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1272 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1279 "lev_comp.y"
    {
		      long rflags = (yyvsp[(8) - (10)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      add_opvars(splev, "iiiiiiio",
				 VA_PASS8(rflags,
					  (yyvsp[(7) - (10)].crd).x, (yyvsp[(7) - (10)].crd).y, (yyvsp[(5) - (10)].crd).x, (yyvsp[(5) - (10)].crd).y,
					  (yyvsp[(9) - (10)].sze).width, (yyvsp[(9) - (10)].sze).height, SPO_ROOM));
		      break_stmt_start();
		  ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1290 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1297 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1301 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1307 "lev_comp.y"
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

  case 176:

/* Line 1455 of yacc.c  */
#line 1317 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1323 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1332 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1338 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1343 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1349 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1354 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1360 "lev_comp.y"
    {
			/* ERR means random here */
			if ((yyvsp[(7) - (9)].i) == ERR && (yyvsp[(9) - (9)].i) != ERR) {
			    lc_error("If the door wall is random, so must be its pos!");
			} else {
			    add_opvars(splev, "iiiio",
				       VA_PASS5((long)(yyvsp[(9) - (9)].i), (long)(yyvsp[(5) - (9)].i), (long)(yyvsp[(3) - (9)].i),
						(long)(yyvsp[(7) - (9)].i), SPO_ROOM_DOOR));
			}
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1371 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (5)].i), SPO_DOOR));
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1385 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1389 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1399 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1406 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1414 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1430 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1434 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1440 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1448 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1454 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x0000;
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1462 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1470 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1475 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1481 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1487 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1493 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1499 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1504 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1509 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1514 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1519 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1524 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1529 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1534 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1539 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1544 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1549 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1557 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1566 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1570 "lev_comp.y"
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

  case 225:

/* Line 1455 of yacc.c  */
#line 1583 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1591 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1601 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1609 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1618 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1625 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1633 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1639 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1644 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1649 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1654 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1659 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1664 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1669 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1674 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1679 "lev_comp.y"
    {
		      if ((yyvsp[(1) - (1)].i) == D_LOCKED) {
			  add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_LOCKED));
			  (yyval.i) = 0x0100;
		      } else if ((yyvsp[(1) - (1)].i) == D_BROKEN) {
			  add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BROKEN));
			  (yyval.i) = 0x0200;
		      } else
			  lc_error("DOOR state can only be locked or broken.");
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1690 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1696 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1701 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1706 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1711 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1718 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1724 "lev_comp.y"
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

  case 248:

/* Line 1455 of yacc.c  */
#line 1753 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1758 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1766 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1772 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1778 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1785 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1792 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1802 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1812 "lev_comp.y"
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

  case 257:

/* Line 1455 of yacc.c  */
#line 1827 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1837 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1841 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1847 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_FOUNTAIN));
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1853 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1859 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1865 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1871 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1876 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1883 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1890 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1896 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1902 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1908 "lev_comp.y"
    {
		      long irr;
		      long rt = (yyvsp[(7) - (8)].i);
		      long rflags = (yyvsp[(8) - (8)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      if (!(rflags & 1)) rt += MAXRTYPE+1;
		      irr = ((rflags & 2) != 0);
		      add_opvars(splev, "iiio",
				 VA_PASS4((long)(yyvsp[(5) - (8)].i), rt, rflags, SPO_REGION));
		      (yyval.i) = (irr || (rflags & 1) || rt != OROOM);
		      break_stmt_start();
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1922 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      if ( (yyvsp[(9) - (10)].i) ) {
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      } else if ( (yyvsp[(10) - (10)].i) )
			  lc_error("Cannot use lev statements in non-permanent REGION");
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1932 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1936 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1942 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1949 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1955 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1959 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1964 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1971 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1977 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1984 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1988 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1995 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 2006 "lev_comp.y"
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

  case 287:

/* Line 1455 of yacc.c  */
#line 2019 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 2023 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 2029 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 2033 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 2040 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 2044 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 2048 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2064 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2072 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2082 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2091 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2096 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2103 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2114 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2120 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2124 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2128 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2135 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2145 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2152 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2156 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2162 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2166 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2174 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2178 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2185 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2195 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2208 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2212 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2219 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2229 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2238 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2249 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2253 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2260 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2270 "lev_comp.y"
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

  case 333:

/* Line 1455 of yacc.c  */
#line 2281 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2290 "lev_comp.y"
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

  case 335:

/* Line 1455 of yacc.c  */
#line 2300 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2306 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2310 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2317 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2327 "lev_comp.y"
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

  case 340:

/* Line 1455 of yacc.c  */
#line 2338 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2347 "lev_comp.y"
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

  case 342:

/* Line 1455 of yacc.c  */
#line 2357 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2363 "lev_comp.y"
    { ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2365 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2371 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2375 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2379 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2383 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2391 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2400 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2404 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2408 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2412 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2416 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2419 "lev_comp.y"
    { ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2423 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2430 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2439 "lev_comp.y"
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

  case 363:

/* Line 1455 of yacc.c  */
#line 2480 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2484 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2491 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2498 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2508 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2512 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2520 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2524 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2528 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2532 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2536 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2541 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2545 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2549 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2554 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2559 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2564 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2568 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2573 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2578 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2582 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2586 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2591 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2595 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2602 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2608 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2612 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2618 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2630 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2634 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2638 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2642 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2664 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2668 "lev_comp.y"
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

  case 408:

/* Line 1455 of yacc.c  */
#line 2694 "lev_comp.y"
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
#line 6067 "lev_comp.tab.c"
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
#line 2722 "lev_comp.y"


/*lev_comp.y*/

