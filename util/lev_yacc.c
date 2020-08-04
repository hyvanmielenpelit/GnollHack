
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
     LADDER_ID = 291,
     STAIR_ID = 292,
     NON_DIGGABLE_ID = 293,
     NON_PASSWALL_ID = 294,
     ROOM_ID = 295,
     PORTAL_ID = 296,
     TELEPRT_ID = 297,
     BRANCH_ID = 298,
     LEV = 299,
     MINERALIZE_ID = 300,
     CORRIDOR_ID = 301,
     GOLD_ID = 302,
     ENGRAVING_ID = 303,
     FOUNTAIN_ID = 304,
     POOL_ID = 305,
     SINK_ID = 306,
     NONE = 307,
     RAND_CORRIDOR_ID = 308,
     DOOR_STATE = 309,
     LIGHT_STATE = 310,
     CURSE_TYPE = 311,
     ENGRAVING_TYPE = 312,
     DIRECTION = 313,
     RANDOM_TYPE = 314,
     RANDOM_TYPE_BRACKET = 315,
     A_REGISTER = 316,
     ALIGNMENT = 317,
     LEFT_OR_RIGHT = 318,
     CENTER = 319,
     TOP_OR_BOT = 320,
     ALTAR_TYPE = 321,
     UP_OR_DOWN = 322,
     SUBROOM_ID = 323,
     NAME_ID = 324,
     FLAGS_ID = 325,
     FLAG_TYPE = 326,
     MON_ATTITUDE = 327,
     MON_ALERTNESS = 328,
     MON_APPEARANCE = 329,
     ROOMDOOR_ID = 330,
     IF_ID = 331,
     ELSE_ID = 332,
     TERRAIN_ID = 333,
     HORIZ_OR_VERT = 334,
     REPLACE_TERRAIN_ID = 335,
     EXIT_ID = 336,
     SHUFFLE_ID = 337,
     QUANTITY_ID = 338,
     BURIED_ID = 339,
     LOOP_ID = 340,
     FOR_ID = 341,
     TO_ID = 342,
     SWITCH_ID = 343,
     CASE_ID = 344,
     BREAK_ID = 345,
     DEFAULT_ID = 346,
     ERODED_ID = 347,
     TRAPPED_STATE = 348,
     RECHARGED_ID = 349,
     INVIS_ID = 350,
     GREASED_ID = 351,
     FEMALE_ID = 352,
     CANCELLED_ID = 353,
     REVIVED_ID = 354,
     AVENGE_ID = 355,
     FLEEING_ID = 356,
     BLINDED_ID = 357,
     PARALYZED_ID = 358,
     STUNNED_ID = 359,
     CONFUSED_ID = 360,
     SEENTRAPS_ID = 361,
     ALL_ID = 362,
     MONTYPE_ID = 363,
     GRAVE_ID = 364,
     ERODEPROOF_ID = 365,
     FUNCTION_ID = 366,
     MSG_OUTPUT_TYPE = 367,
     COMPARE_TYPE = 368,
     UNKNOWN_TYPE = 369,
     rect_ID = 370,
     fillrect_ID = 371,
     line_ID = 372,
     randline_ID = 373,
     grow_ID = 374,
     selection_ID = 375,
     flood_ID = 376,
     rndcoord_ID = 377,
     circle_ID = 378,
     ellipse_ID = 379,
     filter_ID = 380,
     complement_ID = 381,
     gradient_ID = 382,
     GRADIENT_TYPE = 383,
     LIMITED = 384,
     HUMIDITY_TYPE = 385,
     STRING = 386,
     MAP_ID = 387,
     NQSTRING = 388,
     VARSTRING = 389,
     CFUNC = 390,
     CFUNC_INT = 391,
     CFUNC_STR = 392,
     CFUNC_COORD = 393,
     CFUNC_REGION = 394,
     VARSTRING_INT = 395,
     VARSTRING_INT_ARRAY = 396,
     VARSTRING_STRING = 397,
     VARSTRING_STRING_ARRAY = 398,
     VARSTRING_VAR = 399,
     VARSTRING_VAR_ARRAY = 400,
     VARSTRING_COORD = 401,
     VARSTRING_COORD_ARRAY = 402,
     VARSTRING_REGION = 403,
     VARSTRING_REGION_ARRAY = 404,
     VARSTRING_MAPCHAR = 405,
     VARSTRING_MAPCHAR_ARRAY = 406,
     VARSTRING_MONST = 407,
     VARSTRING_MONST_ARRAY = 408,
     VARSTRING_OBJ = 409,
     VARSTRING_OBJ_ARRAY = 410,
     VARSTRING_SEL = 411,
     VARSTRING_SEL_ARRAY = 412,
     METHOD_INT = 413,
     METHOD_INT_ARRAY = 414,
     METHOD_STRING = 415,
     METHOD_STRING_ARRAY = 416,
     METHOD_VAR = 417,
     METHOD_VAR_ARRAY = 418,
     METHOD_COORD = 419,
     METHOD_COORD_ARRAY = 420,
     METHOD_REGION = 421,
     METHOD_REGION_ARRAY = 422,
     METHOD_MAPCHAR = 423,
     METHOD_MAPCHAR_ARRAY = 424,
     METHOD_MONST = 425,
     METHOD_MONST_ARRAY = 426,
     METHOD_OBJ = 427,
     METHOD_OBJ_ARRAY = 428,
     METHOD_SEL = 429,
     METHOD_SEL_ARRAY = 430,
     DICE = 431
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
#line 474 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 486 "lev_comp.tab.c"

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
#define YYLAST   1037

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  194
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  159
/* YYNRULES -- Number of rules.  */
#define YYNRULES  404
/* YYNRULES -- Number of states.  */
#define YYNSTATES  864

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   431

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   189,   193,     2,
     133,   134,   187,   185,   131,   186,   191,   188,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   132,     2,
       2,   190,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   135,     2,   136,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   137,   192,   138,     2,     2,     2,     2,
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
     125,   126,   127,   128,   129,   130,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184
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
     217,   219,   221,   223,   225,   227,   231,   235,   241,   245,
     251,   257,   263,   267,   271,   277,   283,   289,   297,   305,
     313,   319,   321,   325,   327,   331,   333,   337,   339,   343,
     345,   349,   351,   355,   357,   361,   362,   363,   372,   377,
     379,   380,   382,   384,   390,   394,   395,   396,   406,   407,
     410,   411,   417,   418,   423,   425,   428,   430,   437,   438,
     442,   443,   450,   451,   456,   457,   462,   464,   465,   470,
     474,   476,   480,   484,   490,   496,   504,   509,   510,   521,
     522,   535,   536,   539,   545,   547,   553,   555,   561,   563,
     569,   571,   581,   587,   589,   591,   593,   595,   597,   601,
     603,   605,   607,   615,   621,   623,   625,   627,   629,   633,
     634,   640,   645,   646,   650,   652,   654,   656,   658,   661,
     663,   665,   667,   669,   671,   675,   679,   683,   685,   687,
     691,   693,   695,   699,   703,   704,   710,   713,   714,   718,
     720,   724,   726,   730,   734,   736,   738,   742,   744,   746,
     748,   752,   754,   756,   758,   764,   772,   778,   787,   789,
     793,   799,   805,   813,   821,   828,   834,   835,   838,   842,
     846,   850,   852,   858,   868,   874,   878,   882,   883,   894,
     895,   897,   905,   911,   917,   921,   927,   935,   945,   947,
     949,   951,   953,   955,   956,   959,   961,   965,   967,   969,
     971,   973,   975,   977,   979,   981,   983,   985,   987,   989,
     993,   995,   997,  1002,  1004,  1006,  1011,  1013,  1015,  1020,
    1022,  1027,  1033,  1035,  1039,  1041,  1045,  1047,  1049,  1054,
    1064,  1066,  1068,  1073,  1075,  1081,  1083,  1085,  1090,  1092,
    1094,  1100,  1102,  1104,  1106,  1111,  1113,  1115,  1121,  1123,
    1125,  1129,  1131,  1133,  1137,  1139,  1144,  1148,  1152,  1156,
    1160,  1164,  1168,  1170,  1172,  1176,  1178,  1182,  1183,  1185,
    1187,  1189,  1191,  1195,  1196,  1198,  1200,  1203,  1206,  1211,
    1218,  1223,  1230,  1237,  1244,  1251,  1254,  1261,  1270,  1279,
    1290,  1305,  1308,  1310,  1314,  1316,  1320,  1322,  1324,  1326,
    1328,  1330,  1332,  1334,  1336,  1338,  1340,  1342,  1344,  1346,
    1348,  1350,  1352,  1354,  1365
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     195,     0,    -1,    -1,   196,    -1,   197,    -1,   197,   196,
      -1,   198,   204,   206,    -1,    16,   132,   139,    -1,    15,
     132,   139,   131,     3,    -1,    17,   132,    11,   131,   296,
      -1,    17,   132,    10,   131,     3,    -1,    17,   132,    13,
      -1,    17,   132,    12,   131,     3,   131,     3,   131,     5,
     131,     5,   131,   315,   131,   203,   202,    -1,    -1,   131,
     129,    -1,    -1,   131,   322,    -1,    -1,   131,     3,    -1,
       5,    -1,    59,    -1,    -1,    70,   132,   205,    -1,    71,
     131,   205,    -1,    71,    -1,    -1,   208,   206,    -1,   137,
     206,   138,    -1,   249,    -1,   199,    -1,   304,    -1,   305,
      -1,   291,    -1,   251,    -1,   214,    -1,   213,    -1,   299,
      -1,   263,    -1,   283,    -1,   307,    -1,   308,    -1,   293,
      -1,   306,    -1,   229,    -1,   239,    -1,   241,    -1,   245,
      -1,   243,    -1,   226,    -1,   236,    -1,   222,    -1,   225,
      -1,   286,    -1,   268,    -1,   284,    -1,   271,    -1,   277,
      -1,   300,    -1,   295,    -1,   289,    -1,   250,    -1,   301,
      -1,   256,    -1,   254,    -1,   294,    -1,   298,    -1,   297,
      -1,   287,    -1,   288,    -1,   290,    -1,   282,    -1,   285,
      -1,   149,    -1,   151,    -1,   153,    -1,   155,    -1,   157,
      -1,   159,    -1,   161,    -1,   163,    -1,   165,    -1,   148,
      -1,   150,    -1,   152,    -1,   154,    -1,   156,    -1,   158,
      -1,   160,    -1,   162,    -1,   164,    -1,   209,    -1,   210,
      -1,   142,    -1,   142,    -1,   210,    -1,    82,   132,   209,
      -1,   211,   190,   334,    -1,   211,   190,   120,   132,   343,
      -1,   211,   190,   333,    -1,   211,   190,   349,   132,   327,
      -1,   211,   190,   348,   132,   329,    -1,   211,   190,   347,
     132,   331,    -1,   211,   190,   322,    -1,   211,   190,   325,
      -1,   211,   190,   137,   220,   138,    -1,   211,   190,   137,
     219,   138,    -1,   211,   190,   137,   218,   138,    -1,   211,
     190,   349,   132,   137,   217,   138,    -1,   211,   190,   348,
     132,   137,   216,   138,    -1,   211,   190,   347,   132,   137,
     215,   138,    -1,   211,   190,   137,   221,   138,    -1,   332,
      -1,   215,   131,   332,    -1,   330,    -1,   216,   131,   330,
      -1,   328,    -1,   217,   131,   328,    -1,   326,    -1,   218,
     131,   326,    -1,   323,    -1,   219,   131,   323,    -1,   334,
      -1,   220,   131,   334,    -1,   333,    -1,   221,   131,   333,
      -1,    -1,    -1,   111,   141,   133,   223,   338,   134,   224,
     207,    -1,   141,   133,   341,   134,    -1,    81,    -1,    -1,
       6,    -1,     6,    -1,   135,   334,   113,   334,   136,    -1,
     135,   334,   136,    -1,    -1,    -1,    88,   230,   135,   321,
     136,   231,   137,   232,   138,    -1,    -1,   233,   232,    -1,
      -1,    89,   345,   132,   234,   206,    -1,    -1,    91,   132,
     235,   206,    -1,    90,    -1,   191,   191,    -1,    87,    -1,
      86,   212,   190,   334,   237,   334,    -1,    -1,   238,   240,
     207,    -1,    -1,    85,   135,   321,   136,   242,   207,    -1,
      -1,   228,   132,   244,   208,    -1,    -1,    76,   228,   246,
     247,    -1,   207,    -1,    -1,   207,   248,    77,   207,    -1,
      14,   132,   333,    -1,    53,    -1,    53,   132,   345,    -1,
      53,   132,    59,    -1,    46,   132,   252,   131,   252,    -1,
      46,   132,   252,   131,   345,    -1,   133,     4,   131,    58,
     131,   267,   134,    -1,   310,   227,   131,   315,    -1,    -1,
      68,   132,   253,   131,   260,   131,   262,   311,   255,   207,
      -1,    -1,    40,   132,   253,   131,   259,   131,   261,   131,
     262,   311,   257,   207,    -1,    -1,   131,     5,    -1,   133,
       4,   131,     4,   134,    -1,    59,    -1,   133,     4,   131,
       4,   134,    -1,    59,    -1,   133,   269,   131,   270,   134,
      -1,    59,    -1,   133,     4,   131,     4,   134,    -1,    59,
      -1,    75,   132,   264,   131,   314,   131,   265,   131,   267,
      -1,    24,   132,   314,   131,   343,    -1,     5,    -1,    59,
      -1,   266,    -1,    59,    -1,    58,    -1,    58,   192,   266,
      -1,     4,    -1,    59,    -1,    19,    -1,    18,   132,   269,
     131,   270,   258,   140,    -1,    18,   132,   322,   258,   140,
      -1,    63,    -1,    64,    -1,    65,    -1,    64,    -1,    22,
     132,   273,    -1,    -1,    22,   132,   273,   272,   207,    -1,
     329,   131,   322,   274,    -1,    -1,   274,   131,   275,    -1,
     333,    -1,    72,    -1,    73,    -1,   317,    -1,    74,   333,
      -1,    97,    -1,    95,    -1,    98,    -1,    99,    -1,   100,
      -1,   101,   132,   321,    -1,   102,   132,   321,    -1,   103,
     132,   321,    -1,   104,    -1,   105,    -1,   106,   132,   276,
      -1,   139,    -1,   107,    -1,   139,   192,   276,    -1,    20,
     132,   279,    -1,    -1,    21,   132,   279,   278,   207,    -1,
     331,   280,    -1,    -1,   280,   131,   281,    -1,    56,    -1,
     108,   132,   329,    -1,   346,    -1,    69,   132,   333,    -1,
      83,   132,   321,    -1,    84,    -1,    55,    -1,    92,   132,
     321,    -1,   110,    -1,    54,    -1,    93,    -1,    94,   132,
     321,    -1,    95,    -1,    96,    -1,   322,    -1,    23,   132,
     309,   131,   322,    -1,    25,   132,   322,   131,    58,   131,
     314,    -1,    29,   132,   322,   131,    58,    -1,    29,   132,
     322,   131,    58,   131,     5,   202,    -1,    30,    -1,    30,
     132,   343,    -1,    36,   132,   322,   131,    67,    -1,    37,
     132,   322,   131,    67,    -1,    37,   132,   351,   131,   351,
     131,    67,    -1,    41,   132,   351,   131,   351,   131,   139,
      -1,    42,   132,   351,   131,   351,   292,    -1,    43,   132,
     351,   131,   351,    -1,    -1,   131,    67,    -1,    49,   132,
     343,    -1,    51,   132,   343,    -1,    50,   132,   343,    -1,
       3,    -1,   133,     3,   131,   315,   134,    -1,    80,   132,
     325,   131,   327,   131,   327,   131,     7,    -1,    78,   132,
     343,   131,   327,    -1,    38,   132,   325,    -1,    39,   132,
     325,    -1,    -1,    31,   132,   325,   131,   315,   131,   310,
     311,   302,   303,    -1,    -1,   207,    -1,    35,   132,   322,
     131,   316,   131,   318,    -1,   109,   132,   322,   131,   333,
      -1,   109,   132,   322,   131,    59,    -1,   109,   132,   322,
      -1,    47,   132,   334,   131,   322,    -1,    48,   132,   322,
     131,   350,   131,   333,    -1,    45,   132,   321,   131,   321,
     131,   321,   131,   321,    -1,    45,    -1,   139,    -1,    59,
      -1,   139,    -1,    59,    -1,    -1,   131,   312,    -1,   313,
      -1,   313,   131,   312,    -1,    32,    -1,    33,    -1,    34,
      -1,    54,    -1,    59,    -1,    55,    -1,    59,    -1,    62,
      -1,   319,    -1,    59,    -1,    62,    -1,   319,    -1,    61,
     132,    59,    -1,    66,    -1,    59,    -1,    61,   135,     4,
     136,    -1,   139,    -1,   150,    -1,   151,   135,   334,   136,
      -1,   334,    -1,   323,    -1,   122,   133,   343,   134,    -1,
     154,    -1,   155,   135,   334,   136,    -1,   133,     4,   131,
       4,   134,    -1,    59,    -1,    60,   324,   136,    -1,   130,
      -1,   130,   131,   324,    -1,   326,    -1,   156,    -1,   157,
     135,   334,   136,    -1,   133,     4,   131,     4,   131,     4,
     131,     4,   134,    -1,   328,    -1,   158,    -1,   159,   135,
     334,   136,    -1,     3,    -1,   133,     3,   131,   315,   134,
      -1,   330,    -1,   160,    -1,   161,   135,   334,   136,    -1,
     139,    -1,     3,    -1,   133,     3,   131,   139,   134,    -1,
      59,    -1,   332,    -1,   162,    -1,   163,   135,   334,   136,
      -1,   139,    -1,     3,    -1,   133,     3,   131,   139,   134,
      -1,    59,    -1,   320,    -1,   333,   191,   320,    -1,     4,
      -1,   344,    -1,   133,     8,   134,    -1,   148,    -1,   149,
     135,   334,   136,    -1,   334,   185,   334,    -1,   334,   186,
     334,    -1,   334,   187,   334,    -1,   334,   188,   334,    -1,
     334,   189,   334,    -1,   133,   334,   134,    -1,   144,    -1,
     145,    -1,   211,   132,   335,    -1,   336,    -1,   337,   131,
     336,    -1,    -1,   337,    -1,   334,    -1,   333,    -1,   339,
      -1,   340,   131,   339,    -1,    -1,   340,    -1,   322,    -1,
     115,   325,    -1,   116,   325,    -1,   117,   322,   131,   322,
      -1,   118,   322,   131,   322,   131,   334,    -1,   119,   133,
     343,   134,    -1,   119,   133,   266,   131,   343,   134,    -1,
     125,   133,     7,   131,   343,   134,    -1,   125,   133,   343,
     131,   343,   134,    -1,   125,   133,   327,   131,   343,   134,
      -1,   121,   322,    -1,   123,   133,   322,   131,   334,   134,
      -1,   123,   133,   322,   131,   334,   131,    32,   134,    -1,
     124,   133,   322,   131,   334,   131,   334,   134,    -1,   124,
     133,   322,   131,   334,   131,   334,   131,    32,   134,    -1,
     127,   133,   128,   131,   133,   334,   186,   334,   200,   134,
     131,   322,   201,   134,    -1,   126,   342,    -1,   164,    -1,
     133,   343,   134,    -1,   342,    -1,   342,   193,   343,    -1,
     184,    -1,     8,    -1,     9,    -1,     4,    -1,     8,    -1,
       9,    -1,     4,    -1,   344,    -1,    26,    -1,    20,    -1,
      27,    -1,    22,    -1,    28,    -1,    78,    -1,    57,    -1,
      59,    -1,   352,    -1,    44,   133,     4,   131,     4,   131,
       4,   131,     4,   134,    -1,   133,     4,   131,     4,   131,
       4,   131,     4,   134,    -1
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
     513,   514,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     539,   540,   541,   544,   545,   548,   564,   570,   576,   582,
     588,   594,   600,   606,   612,   622,   632,   642,   652,   662,
     672,   684,   689,   696,   701,   708,   713,   720,   724,   730,
     735,   742,   746,   752,   756,   763,   785,   762,   799,   854,
     861,   864,   870,   877,   881,   890,   894,   889,   957,   958,
     962,   961,   975,   974,   989,   999,  1000,  1003,  1041,  1040,
    1075,  1074,  1105,  1104,  1137,  1136,  1162,  1173,  1172,  1200,
    1206,  1211,  1216,  1223,  1230,  1239,  1247,  1259,  1258,  1277,
    1276,  1295,  1298,  1304,  1314,  1320,  1329,  1335,  1340,  1346,
    1351,  1357,  1368,  1374,  1375,  1378,  1379,  1382,  1386,  1392,
    1393,  1396,  1403,  1411,  1419,  1420,  1423,  1424,  1427,  1432,
    1431,  1445,  1452,  1459,  1467,  1472,  1478,  1484,  1490,  1496,
    1501,  1506,  1511,  1516,  1521,  1526,  1531,  1536,  1541,  1546,
    1554,  1563,  1567,  1580,  1589,  1588,  1606,  1616,  1622,  1630,
    1636,  1641,  1646,  1651,  1656,  1661,  1666,  1671,  1676,  1687,
    1693,  1698,  1703,  1708,  1715,  1721,  1750,  1755,  1763,  1769,
    1775,  1782,  1789,  1799,  1809,  1824,  1835,  1838,  1844,  1850,
    1856,  1862,  1867,  1874,  1881,  1887,  1893,  1900,  1899,  1924,
    1927,  1933,  1940,  1944,  1949,  1956,  1962,  1969,  1973,  1980,
    1988,  1991,  2001,  2005,  2008,  2014,  2018,  2025,  2029,  2033,
    2039,  2040,  2043,  2044,  2047,  2048,  2049,  2055,  2056,  2057,
    2063,  2064,  2067,  2076,  2081,  2088,  2099,  2105,  2109,  2113,
    2120,  2130,  2137,  2141,  2147,  2151,  2159,  2163,  2170,  2180,
    2193,  2197,  2204,  2214,  2223,  2234,  2238,  2245,  2255,  2266,
    2275,  2285,  2291,  2295,  2302,  2312,  2323,  2332,  2342,  2349,
    2350,  2356,  2360,  2364,  2368,  2376,  2385,  2389,  2393,  2397,
    2401,  2405,  2408,  2415,  2424,  2457,  2458,  2461,  2462,  2465,
    2469,  2476,  2483,  2494,  2497,  2505,  2509,  2513,  2517,  2521,
    2526,  2530,  2534,  2539,  2544,  2549,  2553,  2558,  2563,  2567,
    2571,  2576,  2580,  2587,  2593,  2597,  2603,  2610,  2611,  2612,
    2615,  2619,  2623,  2627,  2633,  2634,  2637,  2638,  2641,  2642,
    2645,  2646,  2649,  2653,  2679
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
  "REGION_ID", "FILLING", "IRREGULAR", "JOINED", "ALTAR_ID", "LADDER_ID",
  "STAIR_ID", "NON_DIGGABLE_ID", "NON_PASSWALL_ID", "ROOM_ID", "PORTAL_ID",
  "TELEPRT_ID", "BRANCH_ID", "LEV", "MINERALIZE_ID", "CORRIDOR_ID",
  "GOLD_ID", "ENGRAVING_ID", "FOUNTAIN_ID", "POOL_ID", "SINK_ID", "NONE",
  "RAND_CORRIDOR_ID", "DOOR_STATE", "LIGHT_STATE", "CURSE_TYPE",
  "ENGRAVING_TYPE", "DIRECTION", "RANDOM_TYPE", "RANDOM_TYPE_BRACKET",
  "A_REGISTER", "ALIGNMENT", "LEFT_OR_RIGHT", "CENTER", "TOP_OR_BOT",
  "ALTAR_TYPE", "UP_OR_DOWN", "SUBROOM_ID", "NAME_ID", "FLAGS_ID",
  "FLAG_TYPE", "MON_ATTITUDE", "MON_ALERTNESS", "MON_APPEARANCE",
  "ROOMDOOR_ID", "IF_ID", "ELSE_ID", "TERRAIN_ID", "HORIZ_OR_VERT",
  "REPLACE_TERRAIN_ID", "EXIT_ID", "SHUFFLE_ID", "QUANTITY_ID",
  "BURIED_ID", "LOOP_ID", "FOR_ID", "TO_ID", "SWITCH_ID", "CASE_ID",
  "BREAK_ID", "DEFAULT_ID", "ERODED_ID", "TRAPPED_STATE", "RECHARGED_ID",
  "INVIS_ID", "GREASED_ID", "FEMALE_ID", "CANCELLED_ID", "REVIVED_ID",
  "AVENGE_ID", "FLEEING_ID", "BLINDED_ID", "PARALYZED_ID", "STUNNED_ID",
  "CONFUSED_ID", "SEENTRAPS_ID", "ALL_ID", "MONTYPE_ID", "GRAVE_ID",
  "ERODEPROOF_ID", "FUNCTION_ID", "MSG_OUTPUT_TYPE", "COMPARE_TYPE",
  "UNKNOWN_TYPE", "rect_ID", "fillrect_ID", "line_ID", "randline_ID",
  "grow_ID", "selection_ID", "flood_ID", "rndcoord_ID", "circle_ID",
  "ellipse_ID", "filter_ID", "complement_ID", "gradient_ID",
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
  "opt_limited", "opt_coord_or_var", "opt_fillchar", "walled", "flags",
  "flag_list", "levstatements", "stmt_block", "levstatement",
  "any_var_array", "any_var", "any_var_or_arr", "any_var_or_unk",
  "shuffle_detail", "variable_define", "encodeobj_list",
  "encodemonster_list", "mapchar_list", "encoderegion_list",
  "encodecoord_list", "integer_list", "string_list", "function_define",
  "$@1", "$@2", "function_call", "exitstatement", "opt_percent",
  "comparestmt", "switchstatement", "$@3", "$@4", "switchcases",
  "switchcase", "$@5", "$@6", "breakstatement", "for_to_span",
  "forstmt_start", "forstatement", "$@7", "loopstatement", "$@8",
  "chancestatement", "$@9", "ifstatement", "$@10", "if_ending", "$@11",
  "message", "random_corridors", "corridor", "corr_spec", "room_begin",
  "subroom_def", "$@12", "room_def", "$@13", "roomfill", "room_pos",
  "subroom_pos", "room_align", "room_size", "door_detail", "secret",
  "door_wall", "dir_list", "door_pos", "map_definition", "h_justif",
  "v_justif", "monster_detail", "$@14", "monster_desc", "monster_infos",
  "monster_info", "seen_trap_mask", "object_detail", "$@15", "object_desc",
  "object_infos", "object_info", "trap_detail", "drawbridge_detail",
  "mazewalk_detail", "wallify_detail", "ladder_detail", "stair_detail",
  "stair_region", "portal_region", "teleprt_region", "branch_region",
  "teleprt_detail", "fountain_detail", "sink_detail", "pool_detail",
  "terrain_type", "replace_terrain_detail", "terrain_detail",
  "diggable_detail", "passwall_detail", "region_detail", "@16",
  "region_detail_end", "altar_detail", "grave_detail", "gold_detail",
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
     385,    44,    58,    40,    41,    91,    93,   123,   125,   386,
     387,   388,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,    43,    45,    42,    47,    37,
      61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   194,   195,   195,   196,   196,   197,   198,   198,   199,
     199,   199,   199,   200,   200,   201,   201,   202,   202,   203,
     203,   204,   204,   205,   205,   206,   206,   207,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     211,   211,   211,   212,   212,   213,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   215,   215,   216,   216,   217,   217,   218,   218,   219,
     219,   220,   220,   221,   221,   223,   224,   222,   225,   226,
     227,   227,   228,   228,   228,   230,   231,   229,   232,   232,
     234,   233,   235,   233,   236,   237,   237,   238,   240,   239,
     242,   241,   244,   243,   246,   245,   247,   248,   247,   249,
     250,   250,   250,   251,   251,   252,   253,   255,   254,   257,
     256,   258,   258,   259,   259,   260,   260,   261,   261,   262,
     262,   263,   263,   264,   264,   265,   265,   266,   266,   267,
     267,   268,   268,   268,   269,   269,   270,   270,   271,   272,
     271,   273,   274,   274,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     276,   276,   276,   277,   278,   277,   279,   280,   280,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   282,   283,   284,   284,   285,   285,
     286,   287,   288,   289,   290,   291,   292,   292,   293,   294,
     295,   296,   296,   297,   298,   299,   300,   302,   301,   303,
     303,   304,   305,   305,   305,   306,   307,   308,   308,   309,
     309,   310,   310,   311,   311,   312,   312,   313,   313,   313,
     314,   314,   315,   315,   316,   316,   316,   317,   317,   317,
     318,   318,   319,   320,   320,   320,   321,   322,   322,   322,
     322,   323,   323,   323,   324,   324,   325,   325,   325,   326,
     327,   327,   327,   328,   328,   329,   329,   329,   330,   330,
     330,   330,   331,   331,   331,   332,   332,   332,   332,   333,
     333,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   335,   335,   336,   337,   337,   338,   338,   339,
     339,   340,   340,   341,   341,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   343,   343,   344,   345,   345,   345,
     346,   346,   346,   346,   347,   347,   348,   348,   349,   349,
     350,   350,   351,   351,   352
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
       1,     1,     1,     1,     1,     3,     3,     5,     3,     5,
       5,     5,     3,     3,     5,     5,     5,     7,     7,     7,
       5,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     0,     0,     8,     4,     1,
       0,     1,     1,     5,     3,     0,     0,     9,     0,     2,
       0,     5,     0,     4,     1,     2,     1,     6,     0,     3,
       0,     6,     0,     4,     0,     4,     1,     0,     4,     3,
       1,     3,     3,     5,     5,     7,     4,     0,    10,     0,
      12,     0,     2,     5,     1,     5,     1,     5,     1,     5,
       1,     9,     5,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     7,     5,     1,     1,     1,     1,     3,     0,
       5,     4,     0,     3,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     3,     3,     3,     1,     1,     3,
       1,     1,     3,     3,     0,     5,     2,     0,     3,     1,
       3,     1,     3,     3,     1,     1,     3,     1,     1,     1,
       3,     1,     1,     1,     5,     7,     5,     8,     1,     3,
       5,     5,     7,     7,     6,     5,     0,     2,     3,     3,
       3,     1,     5,     9,     5,     3,     3,     0,    10,     0,
       1,     7,     5,     5,     3,     5,     7,     9,     1,     1,
       1,     1,     1,     0,     2,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     1,     4,     1,     1,     4,     1,     1,     4,     1,
       4,     5,     1,     3,     1,     3,     1,     1,     4,     9,
       1,     1,     4,     1,     5,     1,     1,     4,     1,     1,
       5,     1,     1,     1,     4,     1,     1,     5,     1,     1,
       3,     1,     1,     3,     1,     4,     3,     3,     3,     3,
       3,     3,     1,     1,     3,     1,     3,     0,     1,     1,
       1,     1,     3,     0,     1,     1,     2,     2,     4,     6,
       4,     6,     6,     6,     6,     2,     6,     8,     8,    10,
      14,     2,     1,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,    10,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    21,     0,     0,     1,
       5,     0,    25,     0,     7,     0,   132,     0,     0,     0,
     191,     0,     0,     0,     0,     0,     0,     0,   248,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   278,
       0,     0,     0,     0,     0,     0,   160,     0,     0,     0,
       0,     0,   129,     0,     0,     0,   135,   144,     0,     0,
       0,     0,    92,    81,    72,    82,    73,    83,    74,    84,
      75,    85,    76,    86,    77,    87,    78,    88,    79,    89,
      80,    29,     6,    25,    90,    91,     0,    35,    34,    50,
      51,    48,     0,    43,    49,   148,    44,    45,    47,    46,
      28,    60,    33,    63,    62,    37,    53,    55,    56,    70,
      38,    54,    71,    52,    67,    68,    59,    69,    32,    41,
      64,    58,    66,    65,    36,    57,    61,    30,    31,    42,
      39,    40,     0,    24,    22,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   154,     0,     0,     0,
       0,    93,    94,     0,     0,     0,     0,   341,     0,   344,
       0,   386,     0,   342,   363,    26,     0,   152,     0,     8,
       0,   303,   304,     0,   339,   159,     0,     0,     0,    11,
     312,     0,   194,   195,     0,     0,   309,     0,     0,   171,
     307,   336,   338,     0,   335,   333,     0,   223,   227,   332,
     224,   329,   331,     0,   328,   326,     0,   198,     0,   325,
     280,   279,     0,   290,   291,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     382,   365,   384,   249,     0,   317,     0,     0,   316,     0,
       0,     0,     0,     0,     0,   402,   265,   266,   282,   281,
       0,   130,     0,     0,     0,     0,     0,   306,     0,     0,
       0,     0,   258,   260,   259,   389,   387,   388,   162,   161,
       0,   183,   184,     0,     0,     0,     0,    95,     0,     0,
       0,   274,   125,     0,     0,     0,     0,   134,     0,     0,
       0,     0,     0,   360,   359,   361,   364,     0,   395,   397,
     394,   396,   398,   399,     0,     0,     0,   102,   103,    98,
      96,     0,     0,     0,     0,    25,   149,    23,     0,     0,
       0,     0,     0,   314,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   226,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,   367,     0,     0,     0,   375,     0,
       0,     0,   381,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   156,
     155,     0,     0,   150,     0,     0,     0,   357,   343,   351,
       0,     0,   346,   347,   348,   349,   350,     0,   128,     0,
     341,     0,     0,     0,     0,   119,   117,   123,   121,     0,
       0,     0,   153,     0,     0,   340,    10,   261,     0,     9,
       0,     0,   313,     0,     0,     0,   197,   196,   171,   172,
     193,     0,     0,     0,   225,     0,     0,   200,   202,   244,
     182,     0,   246,     0,     0,   187,     0,     0,     0,     0,
     323,     0,     0,   321,     0,     0,   320,     0,     0,   383,
     385,     0,     0,   292,   293,     0,   296,     0,   294,     0,
     295,   250,     0,     0,   251,     0,   174,     0,     0,     0,
       0,     0,   256,   255,     0,     0,   163,   164,   275,   400,
     401,     0,   176,     0,     0,     0,     0,     0,   264,     0,
       0,   146,     0,     0,   136,   273,   272,     0,   355,   358,
       0,   345,   133,   362,    97,     0,     0,   106,     0,   105,
       0,   104,     0,   110,     0,   101,     0,   100,     0,    99,
      27,   305,     0,     0,   315,   308,     0,   310,     0,     0,
     334,   392,   390,   391,   238,   235,   229,     0,     0,   234,
       0,   239,     0,   241,   242,     0,   237,   228,   243,   393,
     231,     0,   327,   201,     0,     0,   368,     0,     0,     0,
     370,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     318,     0,     0,     0,     0,     0,     0,     0,     0,   166,
       0,     0,     0,   254,     0,     0,     0,     0,     0,     0,
       0,     0,   151,   145,   147,     0,     0,     0,   126,     0,
     118,   120,   122,   124,     0,   111,     0,   113,     0,   115,
       0,     0,   311,   192,   337,     0,     0,     0,     0,     0,
     330,     0,   245,    17,     0,   188,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   283,     0,   301,   300,
     271,     0,     0,   252,     0,   178,     0,     0,   253,   257,
       0,     0,   276,     0,   180,     0,   283,   186,     0,   185,
     158,     0,   138,   352,   353,   354,   356,     0,     0,   109,
       0,   108,     0,   107,     0,     0,   232,   233,   236,   240,
     230,     0,   297,   205,   206,     0,   210,   209,   211,   212,
     213,     0,     0,     0,   217,   218,     0,   203,   207,   298,
     204,     0,   247,   369,   371,     0,   376,     0,   372,     0,
     322,   374,   373,     0,     0,     0,   267,   302,     0,     0,
       0,     0,     0,     0,   189,   190,     0,     0,     0,   167,
       0,     0,     0,     0,     0,   138,   127,   112,   114,   116,
     262,     0,     0,   208,     0,     0,     0,     0,    18,     0,
       0,   324,     0,     0,   287,   288,   289,   284,   285,   269,
       0,     0,   173,     0,   283,   277,   165,   175,     0,     0,
     181,   263,     0,   142,   137,   139,     0,   299,   214,   215,
     216,   221,   220,   219,   377,     0,   378,   347,     0,     0,
     270,   268,     0,     0,     0,   169,     0,   168,   140,    25,
       0,     0,     0,     0,     0,   319,   286,     0,   404,   177,
       0,   179,    25,   143,     0,   222,   379,    14,     0,   403,
     170,   141,     0,     0,     0,    15,    19,    20,    17,     0,
       0,    12,    16,   380
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    81,   834,   860,   732,   858,
      12,   134,    82,   336,    83,    84,    85,    86,   173,    87,
      88,   634,   636,   638,   421,   422,   423,   424,    89,   407,
     697,    90,    91,   387,    92,    93,   174,   625,   764,   765,
     842,   829,    94,   523,    95,    96,   188,    97,   520,    98,
     334,    99,   294,   400,   516,   100,   101,   102,   279,   270,
     103,   799,   104,   840,   350,   498,   514,   677,   686,   105,
     293,   688,   466,   756,   106,   208,   448,   107,   357,   227,
     583,   727,   813,   108,   354,   217,   353,   577,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   613,   119,
     120,   121,   439,   122,   123,   124,   125,   126,   789,   821,
     127,   128,   129,   130,   131,   232,   271,   746,   787,   788,
     235,   485,   489,   728,   670,   490,   194,   276,   251,   210,
     344,   257,   258,   475,   476,   228,   229,   218,   219,   313,
     277,   695,   528,   529,   530,   315,   316,   317,   252,   374,
     183,   289,   580,   331,   332,   333,   511,   264,   265
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -652
static const yytype_int16 yypact[] =
{
     264,   -49,   -43,   122,  -652,   264,    28,    22,    42,  -652,
    -652,    58,   731,    74,  -652,   147,  -652,    97,   109,   113,
    -652,   132,   164,   179,   188,   193,   197,   221,   241,   248,
     271,   277,   279,   288,   302,   303,   310,   312,   313,   320,
     323,   324,   325,   327,   328,   331,   335,   337,   343,    63,
     344,   345,  -652,   349,   256,   508,  -652,  -652,   350,   195,
     129,   374,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,
    -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,
    -652,  -652,  -652,   731,  -652,  -652,   293,  -652,  -652,  -652,
    -652,  -652,   376,  -652,  -652,  -652,  -652,  -652,  -652,  -652,
    -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,
    -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,
    -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,
    -652,  -652,   507,   380,  -652,    21,   476,   230,   152,   152,
      60,   -22,   222,   259,   259,   671,   -60,   259,   259,   228,
     -60,   -60,   -21,    15,    15,    15,   129,   379,   129,   259,
     671,   671,   671,    86,   -21,    71,  -652,   671,   -60,   553,
     129,  -652,  -652,   351,   398,   259,   401,  -652,    43,  -652,
     400,  -652,   190,  -652,    64,  -652,   282,  -652,   406,  -652,
     147,  -652,  -652,   407,  -652,   358,   421,   422,   423,  -652,
    -652,   425,  -652,  -652,   428,   554,  -652,   427,   434,   435,
    -652,  -652,  -652,   564,  -652,  -652,   433,  -652,  -652,  -652,
    -652,  -652,  -652,   567,  -652,  -652,   436,   437,   441,  -652,
    -652,  -652,   442,  -652,  -652,   444,   446,   447,   -60,   -60,
     259,   259,   449,   259,   451,   453,   454,   671,   460,   521,
    -652,  -652,   386,  -652,   590,  -652,   461,   464,  -652,   466,
     467,   472,   606,   480,   486,  -652,  -652,  -652,  -652,  -652,
     500,   629,   637,   518,   522,   524,   526,   212,   655,   532,
     240,   534,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,
     536,  -652,  -652,   538,   406,   540,   542,  -652,   541,   129,
     129,   543,  -652,   544,   -77,   129,   129,  -652,   129,   129,
     129,   129,   129,   358,   212,  -652,   548,   546,  -652,  -652,
    -652,  -652,  -652,  -652,   549,    98,    18,  -652,  -652,   358,
     212,   551,   552,   555,   731,   731,  -652,  -652,   129,    21,
     683,    31,   685,   563,   569,   671,   572,   129,   300,   704,
     571,   582,   129,   584,   406,   588,   129,   406,   259,   259,
     671,   662,   663,  -652,  -652,   591,   593,   574,  -652,   259,
     259,   405,  -652,   597,   592,   671,   601,   129,   184,   272,
     666,   730,   604,   669,    15,   -13,  -652,   608,   609,    15,
      15,    15,   129,   610,    52,   259,   160,    16,   222,   665,
    -652,    30,    30,  -652,   181,   607,    -4,   695,  -652,  -652,
     159,   170,   151,   151,  -652,  -652,  -652,    64,  -652,   671,
     613,   -66,   -57,   -17,   111,  -652,  -652,   358,   212,   136,
      40,    25,  -652,   619,   305,  -652,  -652,  -652,   744,  -652,
     627,   425,  -652,   630,   759,   317,  -652,  -652,   435,  -652,
    -652,   626,   359,    32,  -652,   636,   403,  -652,  -652,  -652,
    -652,   652,   654,   259,   259,   599,   670,   668,   672,   674,
    -652,   677,   497,  -652,   675,   684,  -652,   687,   689,  -652,
    -652,   796,   415,  -652,  -652,   691,  -652,   679,  -652,   692,
    -652,  -652,   693,   823,  -652,   697,  -652,   825,   699,   184,
     827,   701,   702,  -652,   703,   778,  -652,  -652,  -652,  -652,
    -652,   707,  -652,   835,   710,   734,   784,   859,  -652,   736,
     406,  -652,   678,   129,  -652,  -652,   358,   738,  -652,   737,
     743,  -652,  -652,  -652,  -652,   867,   745,  -652,    11,  -652,
     129,  -652,    21,  -652,    41,  -652,   204,  -652,    49,  -652,
    -652,  -652,   767,   906,  -652,  -652,   776,  -652,   771,   779,
    -652,  -652,  -652,  -652,  -652,  -652,  -652,   780,   782,  -652,
     783,  -652,   785,  -652,  -652,   786,  -652,  -652,  -652,  -652,
    -652,   787,  -652,   788,   222,   911,  -652,   789,   864,   671,
    -652,   129,   129,   671,   792,   129,   671,   671,   791,   794,
    -652,   -21,   922,    93,   923,     7,   861,   798,    20,  -652,
     799,   793,   866,  -652,   129,   800,    21,   803,    23,   336,
     406,    30,  -652,  -652,   212,   801,   261,   695,  -652,   127,
    -652,  -652,   212,   358,   169,  -652,   174,  -652,   185,  -652,
     184,   804,  -652,  -652,  -652,    21,   129,   129,   129,    60,
    -652,   802,  -652,   805,   129,  -652,   806,   201,   285,   808,
     184,   440,   809,   810,   129,   933,   814,   811,  -652,  -652,
    -652,   815,   935,  -652,   944,  -652,   354,   818,  -652,  -652,
     819,    54,   358,   947,  -652,   950,   814,  -652,   824,  -652,
    -652,   826,   260,  -652,  -652,  -652,  -652,   406,    41,  -652,
     204,  -652,    49,  -652,   822,   953,   358,  -652,  -652,  -652,
    -652,   189,  -652,  -652,  -652,    21,  -652,  -652,  -652,  -652,
    -652,   828,   829,   830,  -652,  -652,   831,  -652,  -652,  -652,
     358,   956,  -652,   212,  -652,   932,  -652,   129,  -652,   832,
    -652,  -652,  -652,   329,   834,   390,  -652,  -652,   963,   837,
     836,   838,    23,   129,  -652,  -652,   839,   840,   841,  -652,
      54,   964,   200,   843,   842,   260,  -652,  -652,  -652,  -652,
    -652,   845,   918,   358,   129,   129,   129,   -54,  -652,   844,
     262,  -652,   129,   975,  -652,  -652,  -652,  -652,   850,   406,
     851,   979,  -652,   300,   814,  -652,  -652,  -652,   980,   406,
    -652,  -652,   853,  -652,  -652,  -652,   981,  -652,  -652,  -652,
    -652,  -652,   795,  -652,  -652,   957,  -652,   309,   854,   390,
    -652,  -652,   986,   857,   858,  -652,   860,  -652,  -652,   731,
     862,   -54,   863,   869,   865,  -652,  -652,   868,  -652,  -652,
     406,  -652,   731,  -652,   184,  -652,  -652,  -652,   870,  -652,
    -652,  -652,   872,   259,    75,   873,  -652,  -652,   805,   259,
     871,  -652,  -652,  -652
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -652,  -652,   990,  -652,  -652,  -652,  -652,  -652,   138,  -652,
    -652,   816,   -83,  -290,   673,   846,   945,  -390,  -652,  -652,
    -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,
    -652,  -652,  -652,  -652,   959,  -652,  -652,  -652,   244,  -652,
    -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,
    -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,   616,   847,
    -652,  -652,  -652,  -652,   565,  -652,  -652,  -652,   265,  -652,
    -652,  -652,  -526,   252,  -652,   338,   223,  -652,  -652,  -652,
    -652,  -652,   187,  -652,  -652,   880,  -652,  -652,  -652,  -652,
    -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,
    -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,
    -652,  -652,  -652,  -652,  -652,  -652,   419,  -651,   202,  -652,
    -378,  -490,  -652,  -652,  -652,   371,   686,  -168,  -136,  -312,
     583,  -102,  -308,  -386,  -527,  -411,  -522,   594,  -502,  -132,
     -55,  -652,   399,  -652,  -652,   611,  -652,  -652,   790,  -135,
     576,  -388,  -652,  -652,  -652,  -652,  -652,  -124,  -652
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -200
static const yytype_int16 yytable[] =
{
     185,   209,   298,   195,   399,   182,   507,   236,   237,   609,
     253,   259,   260,   263,   425,   518,   519,   527,   426,   547,
     515,   639,   177,   281,   637,   282,   283,   284,   470,   273,
     274,   275,   295,   470,   437,   759,   561,   230,   268,   301,
     562,   563,   635,   221,   211,   549,   496,   177,   266,   267,
     327,   303,   470,   811,   329,   525,   285,   409,   754,   261,
     286,   287,   655,   221,   454,   536,   296,   457,   177,    16,
     200,   201,   537,   254,   538,   512,   291,   200,   201,   675,
     856,   539,   684,     7,   328,   812,   564,   565,   566,     8,
     285,   200,   201,   689,   286,   287,   255,   256,    11,   222,
     212,   567,   420,   280,   365,   366,   303,   368,   308,   309,
     310,   311,   312,   755,   540,   568,   569,   231,   269,   222,
     497,   541,     9,   304,   570,   571,   572,   573,   574,   314,
     292,   330,   405,   177,   857,   191,   363,   364,   672,   211,
     575,   642,   576,   825,   205,   288,   192,   193,   272,   513,
     704,   325,   668,   676,   204,   211,   685,   191,   517,   669,
     191,    13,   548,   517,   438,   205,   179,   180,   192,   193,
     739,   192,   193,   223,   213,   769,   178,   546,   768,   224,
     214,    14,   517,   473,   474,   278,   206,   207,   473,   474,
      15,   179,   180,   223,   427,   212,   767,   178,    60,   224,
     225,   226,   181,   191,   285,   132,   652,   221,   286,   287,
     443,   212,   179,   180,   192,   193,   181,   509,   133,   510,
     225,   226,   458,   459,   504,   460,   631,   181,   630,   135,
     622,   178,   467,   468,   469,   691,   477,   527,   710,   483,
     480,   136,   542,   484,   404,   137,   179,   180,   181,   543,
     410,   411,   433,   412,   413,   414,   415,   416,   665,   508,
     495,   642,   178,   222,   138,   501,   502,   503,   521,   213,
     304,   428,   261,   544,   526,   214,   233,   179,   180,     1,
       2,   234,   181,   434,   534,   213,   177,   200,   201,   200,
     201,   214,   445,   202,   203,   531,   139,   452,   215,   216,
     698,   456,   318,   306,   319,   700,   532,   699,   320,   321,
     322,   140,   701,   181,   215,   216,   702,   578,   200,   201,
     141,   772,   482,   703,   602,   142,   307,   586,   587,   143,
     690,   486,   735,   487,   488,   736,   176,   223,   310,   311,
     312,   200,   201,   224,   308,   309,   310,   311,   312,   762,
     204,   763,   204,   144,   852,   308,   309,   310,   311,   312,
     323,   262,   314,   205,   446,   447,   308,   309,   310,   311,
     312,   395,   522,   145,   802,   308,   309,   310,   311,   312,
     146,   204,   206,   207,   206,   207,   308,   309,   310,   311,
     312,   170,   205,   815,   465,   687,   816,   308,   309,   310,
     311,   312,   324,   147,   204,   693,   694,   766,   470,   148,
     633,   149,   471,   206,   207,   325,   737,   202,   203,   326,
     150,   191,   784,   785,   786,   308,   309,   310,   311,   312,
     179,   180,   192,   193,   151,   152,   206,   207,   255,   256,
     833,   551,   153,   -13,   154,   155,   680,   308,   309,   310,
     311,   312,   156,   557,   656,   157,   158,   159,   659,   160,
     161,   662,   663,   162,   200,   201,   181,   163,   624,   164,
     308,   309,   310,   311,   312,   165,   167,   168,   707,   708,
     709,   169,   175,   186,   682,   632,   196,   197,   198,   199,
     308,   309,   310,   311,   312,   560,   310,   311,   312,   820,
     594,   346,   308,   309,   310,   311,   312,   184,   187,   827,
     189,   190,   278,   706,   308,   782,   310,   311,   312,   730,
     238,   239,   240,   241,   242,   346,   243,   204,   244,   245,
     246,   247,   248,   300,   302,   305,   657,   658,   472,   582,
     661,   299,   338,   335,   308,   309,   310,   311,   312,   339,
     850,   600,   340,   341,   342,   343,   200,   201,   346,   206,
     207,   345,   347,   473,   474,   348,   349,   351,   352,   250,
     355,   356,   358,   359,  -199,   360,   740,   361,   362,   375,
     200,   201,   367,   773,   369,   795,   370,   371,   308,   309,
     310,   311,   312,   373,   376,   378,   377,   379,   380,   733,
     308,   309,   310,   311,   312,   381,   808,   809,   810,   743,
     382,   383,   238,   239,   240,   241,   242,   384,   243,   204,
     244,   245,   246,   247,   248,   308,   309,   310,   311,   312,
     249,   385,   465,   200,   201,   386,   238,   239,   240,   241,
     242,   388,   243,   204,   244,   245,   246,   247,   248,   389,
     171,   206,   207,   390,   249,   391,    63,   392,    65,   393,
      67,   250,    69,   394,    71,   396,    73,   397,    75,   398,
      77,   401,    79,   402,   406,   206,   207,   403,   408,   417,
     418,   419,   780,   429,   430,   250,   436,   431,   440,   238,
     239,   240,   241,   242,   441,   243,   204,   244,   245,   246,
     247,   248,    64,   444,    66,   442,    68,   249,    70,   449,
      72,   450,    74,   451,    76,   453,    78,   855,    80,   455,
     461,   462,   463,   862,   464,   478,   479,   817,   206,   207,
     200,   201,   481,   491,   492,   493,   494,    16,   250,   499,
     500,   505,  -157,   524,   535,    17,   843,   552,    18,    19,
      20,    21,    22,    23,    24,    25,    26,   550,   553,   851,
      27,    28,    29,   556,   555,   559,    30,    31,    32,    33,
      34,    35,    36,    37,    38,   581,    39,    40,    41,    42,
      43,    44,    45,   584,    46,   585,   238,   239,   240,   241,
     242,   588,   243,   204,   244,   245,   246,   247,   248,    47,
     599,   589,   590,   591,   249,   592,    48,    49,   593,    50,
     595,    51,    52,    53,   602,   596,    54,    55,   597,    56,
     598,    57,   601,   603,   604,   206,   207,   605,   606,   607,
     608,   610,   611,   612,   614,   250,   615,    62,   616,   617,
      58,   618,    59,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,   620,   594,   711,   712,   619,    60,   621,   627,   623,
     626,   629,    61,    62,   713,   714,   715,   628,   254,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,   716,   640,   717,
     718,   719,   720,   721,   722,   723,   724,   725,   726,   641,
     642,   643,   645,   644,   646,   647,   653,   648,   649,   651,
     654,   650,   465,   660,   664,   665,   667,   671,   673,   674,
     672,   681,   678,   679,   683,   705,   731,   744,   692,   749,
     734,   191,   738,   741,   742,   745,   748,   747,   750,   752,
     753,   757,   192,   193,   758,   760,   770,   761,   771,   778,
     774,   775,   776,   777,   779,   783,   781,   790,   791,   793,
     792,   801,   798,   796,   797,   803,   806,   807,   814,   818,
     804,   819,   822,   823,   826,   828,   830,   831,   835,   832,
     837,   838,   839,   844,   841,    10,   861,   846,   847,   848,
     172,   853,   849,   854,   859,   863,   337,   432,   166,   805,
     506,   290,   800,   558,   751,   297,   824,   794,   845,   220,
     666,   836,   729,   545,   554,   435,   696,     0,   533,   579,
       0,     0,     0,     0,     0,     0,     0,   372
};

static const yytype_int16 yycheck[] =
{
      83,   137,   170,   135,   294,    60,   394,   143,   144,   499,
     145,   147,   148,   149,   326,   401,   402,   407,   326,   430,
     398,   548,     4,   159,   546,   160,   161,   162,     3,   153,
     154,   155,   167,     3,     3,   686,     4,    59,    59,   175,
       8,     9,   544,     3,     3,   431,    59,     4,   150,   151,
     186,     8,     3,   107,   186,    59,     4,   134,     4,    44,
       8,     9,   588,     3,   354,   131,   168,   357,     4,     6,
      59,    60,   138,   133,   131,    59,     5,    59,    60,    59,
       5,   138,    59,   132,   186,   139,    54,    55,    56,   132,
       4,    59,    60,   619,     8,     9,   156,   157,    70,    59,
      59,    69,     4,   158,   240,   241,     8,   243,   185,   186,
     187,   188,   189,    59,   131,    83,    84,   139,   139,    59,
     133,   138,     0,   178,    92,    93,    94,    95,    96,   184,
      59,   186,   300,     4,    59,   139,   238,   239,   131,     3,
     108,   134,   110,   794,   133,    59,   150,   151,   133,   133,
     640,   133,    59,   133,   122,     3,   133,   139,   133,    66,
     139,   139,   137,   133,   133,   133,   148,   149,   150,   151,
     660,   150,   151,   133,   133,   702,   133,   137,   700,   139,
     139,   139,   133,   158,   159,   133,   154,   155,   158,   159,
     132,   148,   149,   133,   326,    59,   698,   133,   135,   139,
     160,   161,   184,   139,     4,   131,   584,     3,     8,     9,
     345,    59,   148,   149,   150,   151,   184,    57,    71,    59,
     160,   161,   358,   359,   392,   360,   538,   184,   536,   132,
     520,   133,   367,   369,   370,   621,   371,   627,   649,    55,
     375,   132,   131,    59,   299,   132,   148,   149,   184,   138,
     305,   306,   335,   308,   309,   310,   311,   312,   131,   395,
     384,   134,   133,    59,   132,   389,   390,   391,    87,   133,
     325,   326,    44,   137,   406,   139,    54,   148,   149,    15,
      16,    59,   184,   338,   419,   133,     4,    59,    60,    59,
      60,   139,   347,    63,    64,   136,   132,   352,   162,   163,
     131,   356,    20,   113,    22,   131,   136,   138,    26,    27,
      28,   132,   138,   184,   162,   163,   131,   453,    59,    60,
     132,   132,   377,   138,   135,   132,   136,   463,   464,   132,
     620,    59,   131,    61,    62,   134,   141,   133,   187,   188,
     189,    59,    60,   139,   185,   186,   187,   188,   189,    89,
     122,    91,   122,   132,   844,   185,   186,   187,   188,   189,
      78,   133,   417,   133,    64,    65,   185,   186,   187,   188,
     189,   131,   191,   132,   762,   185,   186,   187,   188,   189,
     132,   122,   154,   155,   154,   155,   185,   186,   187,   188,
     189,   135,   133,   131,    58,    59,   134,   185,   186,   187,
     188,   189,   120,   132,   122,   144,   145,   697,     3,   132,
     542,   132,     7,   154,   155,   133,   131,    63,    64,   137,
     132,   139,    32,    33,    34,   185,   186,   187,   188,   189,
     148,   149,   150,   151,   132,   132,   154,   155,   156,   157,
     131,   136,   132,   134,   132,   132,   614,   185,   186,   187,
     188,   189,   132,   136,   589,   132,   132,   132,   593,   132,
     132,   596,   597,   132,    59,    60,   184,   132,   523,   132,
     185,   186,   187,   188,   189,   132,   132,   132,   646,   647,
     648,   132,   132,   190,   616,   540,    10,    11,    12,    13,
     185,   186,   187,   188,   189,   136,   187,   188,   189,   789,
       3,     4,   185,   186,   187,   188,   189,   133,   132,   799,
       3,   131,   133,   645,   185,   186,   187,   188,   189,   651,
     115,   116,   117,   118,   119,     4,   121,   122,   123,   124,
     125,   126,   127,   135,   133,   135,   591,   592,   133,   136,
     595,   190,   135,   137,   185,   186,   187,   188,   189,   191,
     840,   136,   131,   131,   131,   130,    59,    60,     4,   154,
     155,   133,   135,   158,   159,   131,   131,     3,   135,   164,
       3,   135,   131,   131,   137,   131,   136,   131,   131,   193,
      59,    60,   133,   715,   133,   753,   133,   133,   185,   186,
     187,   188,   189,   133,     4,   131,   135,   131,   131,   654,
     185,   186,   187,   188,   189,   133,   774,   775,   776,   664,
       4,   131,   115,   116,   117,   118,   119,   131,   121,   122,
     123,   124,   125,   126,   127,   185,   186,   187,   188,   189,
     133,   131,    58,    59,    60,     6,   115,   116,   117,   118,
     119,     4,   121,   122,   123,   124,   125,   126,   127,   131,
     142,   154,   155,   131,   133,   131,   148,   131,   150,     4,
     152,   164,   154,   131,   156,   131,   158,   131,   160,   131,
     162,   131,   164,   131,   131,   154,   155,   136,   134,   131,
     134,   132,   737,   132,   132,   164,     3,   132,     3,   115,
     116,   117,   118,   119,   131,   121,   122,   123,   124,   125,
     126,   127,   149,   131,   151,   136,   153,   133,   155,     5,
     157,   140,   159,   131,   161,   131,   163,   853,   165,   131,
      58,    58,   131,   859,   131,   128,   134,   782,   154,   155,
      59,    60,   131,    67,     4,   131,    67,     6,   164,   131,
     131,   131,    77,   136,   131,    14,   829,     3,    17,    18,
      19,    20,    21,    22,    23,    24,    25,   138,   131,   842,
      29,    30,    31,     4,   134,   139,    35,    36,    37,    38,
      39,    40,    41,    42,    43,   139,    45,    46,    47,    48,
      49,    50,    51,   131,    53,   131,   115,   116,   117,   118,
     119,   192,   121,   122,   123,   124,   125,   126,   127,    68,
       4,   131,   134,   131,   133,   131,    75,    76,   131,    78,
     135,    80,    81,    82,   135,   131,    85,    86,   131,    88,
     131,    90,   131,   131,   131,   154,   155,     4,   131,     4,
     131,     4,   131,   131,   131,   164,    58,   142,   131,     4,
     109,   131,   111,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    77,     3,    61,    62,   131,   135,   131,   131,   191,
     132,     4,   141,   142,    72,    73,    74,   134,   133,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    95,   131,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,     3,
     134,   140,   132,   134,   132,   132,     5,   132,   132,   131,
     131,   134,    58,   131,   133,   131,     4,     4,    67,   131,
     131,   131,   139,    67,   131,   131,   131,     4,   137,     4,
     134,   139,   134,   134,   134,   131,   131,   136,     4,   131,
     131,     4,   150,   151,     4,   131,   134,   131,     5,     3,
     132,   132,   132,   132,    32,   131,   134,     4,   131,   131,
     134,     7,   131,   134,   134,   132,   131,    59,   134,     4,
     138,   131,   131,     4,     4,   132,     5,   192,   134,    32,
       4,   134,   134,   131,   134,     5,   858,   134,   129,   134,
      55,   131,   134,   131,   131,   134,   190,   334,    49,   765,
     394,   164,   760,   448,   676,   169,   793,   752,   831,   139,
     601,   819,   651,   429,   441,   339,   627,    -1,   417,   453,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   247
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   195,   196,   197,   198,   132,   132,     0,
     196,    70,   204,   139,   139,   132,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    29,    30,    31,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    45,
      46,    47,    48,    49,    50,    51,    53,    68,    75,    76,
      78,    80,    81,    82,    85,    86,    88,    90,   109,   111,
     135,   141,   142,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   199,   206,   208,   209,   210,   211,   213,   214,   222,
     225,   226,   228,   229,   236,   238,   239,   241,   243,   245,
     249,   250,   251,   254,   256,   263,   268,   271,   277,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   293,
     294,   295,   297,   298,   299,   300,   301,   304,   305,   306,
     307,   308,   131,    71,   205,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   228,   132,   132,   132,
     135,   142,   210,   212,   230,   132,   141,     4,   133,   148,
     149,   184,   334,   344,   133,   206,   190,   132,   240,     3,
     131,   139,   150,   151,   320,   333,    10,    11,    12,    13,
      59,    60,    63,    64,   122,   133,   154,   155,   269,   322,
     323,     3,    59,   133,   139,   162,   163,   279,   331,   332,
     279,     3,    59,   133,   139,   160,   161,   273,   329,   330,
      59,   139,   309,    54,    59,   314,   322,   322,   115,   116,
     117,   118,   119,   121,   123,   124,   125,   126,   127,   133,
     164,   322,   342,   343,   133,   156,   157,   325,   326,   322,
     322,    44,   133,   322,   351,   352,   325,   325,    59,   139,
     253,   310,   133,   351,   351,   351,   321,   334,   133,   252,
     334,   322,   343,   343,   343,     4,     8,     9,    59,   345,
     253,     5,    59,   264,   246,   343,   325,   209,   321,   190,
     135,   322,   133,     8,   334,   135,   113,   136,   185,   186,
     187,   188,   189,   333,   334,   339,   340,   341,    20,    22,
      26,    27,    28,    78,   120,   133,   137,   322,   325,   333,
     334,   347,   348,   349,   244,   137,   207,   205,   135,   191,
     131,   131,   131,   130,   324,   133,     4,   135,   131,   131,
     258,     3,   135,   280,   278,     3,   135,   272,   131,   131,
     131,   131,   131,   325,   325,   322,   322,   133,   322,   133,
     133,   133,   342,   133,   343,   193,     4,   135,   131,   131,
     131,   133,     4,   131,   131,   131,     6,   227,     4,   131,
     131,   131,   131,     4,   131,   131,   131,   131,   131,   207,
     247,   131,   131,   136,   334,   321,   131,   223,   134,   134,
     334,   334,   334,   334,   334,   334,   334,   131,   134,   132,
       4,   218,   219,   220,   221,   323,   326,   333,   334,   132,
     132,   132,   208,   206,   334,   320,     3,     3,   133,   296,
       3,   131,   136,   343,   131,   334,    64,    65,   270,     5,
     140,   131,   334,   131,   207,   131,   334,   207,   322,   322,
     343,    58,    58,   131,   131,    58,   266,   343,   322,   322,
       3,     7,   133,   158,   159,   327,   328,   343,   128,   134,
     343,   131,   334,    55,    59,   315,    59,    61,    62,   316,
     319,    67,     4,   131,    67,   351,    59,   133,   259,   131,
     131,   351,   351,   351,   321,   131,   252,   345,   322,    57,
      59,   350,    59,   133,   260,   314,   248,   133,   327,   327,
     242,    87,   191,   237,   136,    59,   333,   211,   336,   337,
     338,   136,   136,   339,   343,   131,   131,   138,   131,   138,
     131,   138,   131,   138,   137,   331,   137,   329,   137,   327,
     138,   136,     3,   131,   324,   134,     4,   136,   258,   139,
     136,     4,     8,     9,    54,    55,    56,    69,    83,    84,
      92,    93,    94,    95,    96,   108,   110,   281,   322,   344,
     346,   139,   136,   274,   131,   131,   322,   322,   192,   131,
     134,   131,   131,   131,     3,   135,   131,   131,   131,     4,
     136,   131,   135,   131,   131,     4,   131,     4,   131,   315,
       4,   131,   131,   292,   131,    58,   131,     4,   131,   131,
      77,   131,   207,   191,   334,   231,   132,   131,   134,     4,
     326,   323,   334,   333,   215,   332,   216,   330,   217,   328,
     131,     3,   134,   140,   134,   132,   132,   132,   132,   132,
     134,   131,   314,     5,   131,   266,   343,   334,   334,   343,
     131,   334,   343,   343,   133,   131,   310,     4,    59,    66,
     318,     4,   131,    67,   131,    59,   133,   261,   139,    67,
     321,   131,   333,   131,    59,   133,   262,    59,   265,   266,
     207,   327,   137,   144,   145,   335,   336,   224,   131,   138,
     131,   138,   131,   138,   315,   131,   333,   321,   321,   321,
     329,    61,    62,    72,    73,    74,    95,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   275,   317,   319,
     333,   131,   202,   334,   134,   131,   134,   131,   134,   315,
     136,   134,   134,   334,     4,   131,   311,   136,   131,     4,
       4,   269,   131,   131,     4,    59,   267,     4,     4,   311,
     131,   131,    89,    91,   232,   233,   207,   332,   330,   328,
     134,     5,   132,   333,   132,   132,   132,   132,     3,    32,
     334,   134,   186,   131,    32,    33,    34,   312,   313,   302,
       4,   131,   134,   131,   262,   321,   134,   134,   131,   255,
     267,     7,   345,   132,   138,   232,   131,    59,   321,   321,
     321,   107,   139,   276,   134,   131,   134,   334,     4,   131,
     207,   303,   131,     4,   270,   311,     4,   207,   132,   235,
       5,   192,    32,   131,   200,   134,   312,     4,   134,   134,
     257,   134,   234,   206,   131,   276,   134,   129,   134,   134,
     207,   206,   315,   131,   131,   322,     5,    59,   203,   131,
     201,   202,   322,   134
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

  case 95:

/* Line 1455 of yacc.c  */
#line 549 "lev_comp.y"
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

  case 96:

/* Line 1455 of yacc.c  */
#line 565 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 571 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 577 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 583 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 589 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 595 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 601 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 607 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 613 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 623 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 633 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 643 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 653 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 663 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 673 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 685 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 690 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 697 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 702 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 709 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 714 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 721 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 725 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 731 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 736 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 743 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 747 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 753 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 757 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 763 "lev_comp.y"
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

  case 126:

/* Line 1455 of yacc.c  */
#line 785 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 789 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 800 "lev_comp.y"
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

  case 129:

/* Line 1455 of yacc.c  */
#line 855 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 861 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 865 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 871 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 878 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 882 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 890 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 894 "lev_comp.y"
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

  case 137:

/* Line 1455 of yacc.c  */
#line 917 "lev_comp.y"
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

  case 140:

/* Line 1455 of yacc.c  */
#line 962 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 972 "lev_comp.y"
    {
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 975 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 985 "lev_comp.y"
    {
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 990 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 1004 "lev_comp.y"
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

  case 148:

/* Line 1455 of yacc.c  */
#line 1041 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 1046 "lev_comp.y"
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

  case 150:

/* Line 1455 of yacc.c  */
#line 1075 "lev_comp.y"
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

  case 151:

/* Line 1455 of yacc.c  */
#line 1089 "lev_comp.y"
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

  case 152:

/* Line 1455 of yacc.c  */
#line 1105 "lev_comp.y"
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

  case 153:

/* Line 1455 of yacc.c  */
#line 1125 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1137 "lev_comp.y"
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

  case 155:

/* Line 1455 of yacc.c  */
#line 1157 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1163 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1173 "lev_comp.y"
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

  case 158:

/* Line 1455 of yacc.c  */
#line 1191 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1201 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1207 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1212 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1217 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1224 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1231 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1240 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1248 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1259 "lev_comp.y"
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

  case 168:

/* Line 1455 of yacc.c  */
#line 1270 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1277 "lev_comp.y"
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

  case 170:

/* Line 1455 of yacc.c  */
#line 1288 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1295 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1299 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1305 "lev_comp.y"
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

  case 174:

/* Line 1455 of yacc.c  */
#line 1315 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1321 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1330 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1336 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1341 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1347 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1352 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1358 "lev_comp.y"
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

  case 182:

/* Line 1455 of yacc.c  */
#line 1369 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (5)].i), SPO_DOOR));
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1383 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1387 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1397 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1404 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1412 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1428 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1432 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1438 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1446 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1452 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x0000;
		  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1460 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1468 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1473 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1479 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1485 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1491 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1497 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1502 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1507 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1512 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1517 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1522 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1527 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1532 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1537 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1542 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1547 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1555 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1564 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1568 "lev_comp.y"
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

  case 223:

/* Line 1455 of yacc.c  */
#line 1581 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1589 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1599 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1607 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1616 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1623 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1631 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1637 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1642 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1647 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1652 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1657 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1662 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1667 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1672 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1677 "lev_comp.y"
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

  case 239:

/* Line 1455 of yacc.c  */
#line 1688 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1694 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1699 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1704 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1709 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1716 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1722 "lev_comp.y"
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

  case 246:

/* Line 1455 of yacc.c  */
#line 1751 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1756 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1764 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1770 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1776 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1783 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1790 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1800 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1810 "lev_comp.y"
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

  case 255:

/* Line 1455 of yacc.c  */
#line 1825 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1835 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1839 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1845 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_FOUNTAIN));
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1851 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1857 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1863 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1868 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1875 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1882 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1888 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1894 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1900 "lev_comp.y"
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

  case 268:

/* Line 1455 of yacc.c  */
#line 1914 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      if ( (yyvsp[(9) - (10)].i) ) {
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      } else if ( (yyvsp[(10) - (10)].i) )
			  lc_error("Cannot use lev statements in non-permanent REGION");
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1924 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1928 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1934 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1941 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1945 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1950 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1957 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1963 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1970 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1974 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1981 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1992 "lev_comp.y"
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

  case 283:

/* Line 1455 of yacc.c  */
#line 2005 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 2009 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 2015 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 2019 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 2026 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 2030 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 2034 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2050 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2058 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2068 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2077 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2082 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2089 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2100 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2106 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2110 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2114 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2121 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2131 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2138 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2142 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2148 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2152 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2160 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2164 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2171 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2181 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2194 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2198 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2205 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2215 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2224 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2235 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2239 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2246 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2256 "lev_comp.y"
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

  case 329:

/* Line 1455 of yacc.c  */
#line 2267 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2276 "lev_comp.y"
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

  case 331:

/* Line 1455 of yacc.c  */
#line 2286 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2292 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2296 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2303 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2313 "lev_comp.y"
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

  case 336:

/* Line 1455 of yacc.c  */
#line 2324 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2333 "lev_comp.y"
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

  case 338:

/* Line 1455 of yacc.c  */
#line 2343 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2349 "lev_comp.y"
    { ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2351 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2357 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2361 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2365 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2369 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2377 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2386 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2390 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2394 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2398 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2402 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2405 "lev_comp.y"
    { ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2409 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2416 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2425 "lev_comp.y"
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

  case 359:

/* Line 1455 of yacc.c  */
#line 2466 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2470 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2477 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2484 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2494 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2498 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2506 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2510 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2514 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2518 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2522 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2527 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2531 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2535 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2540 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2545 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2550 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2554 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2559 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2564 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2568 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2572 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2577 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2581 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2588 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2594 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2598 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2604 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2616 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2620 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2624 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2628 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2650 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2654 "lev_comp.y"
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

  case 404:

/* Line 1455 of yacc.c  */
#line 2680 "lev_comp.y"
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
#line 6038 "lev_comp.tab.c"
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
#line 2708 "lev_comp.y"


/*lev_comp.y*/

