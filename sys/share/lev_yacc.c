
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
     MODRON_PORTAL_ID = 307,
     POOL_ID = 308,
     SINK_ID = 309,
     NONE = 310,
     RAND_CORRIDOR_ID = 311,
     DOOR_STATE = 312,
     LIGHT_STATE = 313,
     CURSE_TYPE = 314,
     ENGRAVING_TYPE = 315,
     DIRECTION = 316,
     RANDOM_TYPE = 317,
     RANDOM_TYPE_BRACKET = 318,
     A_REGISTER = 319,
     ALIGNMENT = 320,
     LEFT_OR_RIGHT = 321,
     CENTER = 322,
     TOP_OR_BOT = 323,
     ALTAR_TYPE = 324,
     UP_OR_DOWN = 325,
     ACTIVE_OR_INACTIVE = 326,
     MODRON_PORTAL_TYPE = 327,
     SUBROOM_ID = 328,
     NAME_ID = 329,
     FLAGS_ID = 330,
     FLAG_TYPE = 331,
     MON_ATTITUDE = 332,
     MON_ALERTNESS = 333,
     MON_APPEARANCE = 334,
     ROOMDOOR_ID = 335,
     IF_ID = 336,
     ELSE_ID = 337,
     TERRAIN_ID = 338,
     HORIZ_OR_VERT = 339,
     REPLACE_TERRAIN_ID = 340,
     EXIT_ID = 341,
     SHUFFLE_ID = 342,
     QUANTITY_ID = 343,
     BURIED_ID = 344,
     LOOP_ID = 345,
     FOR_ID = 346,
     TO_ID = 347,
     SWITCH_ID = 348,
     CASE_ID = 349,
     BREAK_ID = 350,
     DEFAULT_ID = 351,
     ERODED_ID = 352,
     TRAPPED_STATE = 353,
     RECHARGED_ID = 354,
     INVIS_ID = 355,
     GREASED_ID = 356,
     FEMALE_ID = 357,
     CANCELLED_ID = 358,
     REVIVED_ID = 359,
     AVENGE_ID = 360,
     FLEEING_ID = 361,
     BLINDED_ID = 362,
     PARALYZED_ID = 363,
     STUNNED_ID = 364,
     CONFUSED_ID = 365,
     SEENTRAPS_ID = 366,
     ALL_ID = 367,
     MONTYPE_ID = 368,
     GRAVE_ID = 369,
     ERODEPROOF_ID = 370,
     FUNCTION_ID = 371,
     MSG_OUTPUT_TYPE = 372,
     COMPARE_TYPE = 373,
     UNKNOWN_TYPE = 374,
     rect_ID = 375,
     fillrect_ID = 376,
     line_ID = 377,
     randline_ID = 378,
     grow_ID = 379,
     selection_ID = 380,
     flood_ID = 381,
     rndcoord_ID = 382,
     circle_ID = 383,
     ellipse_ID = 384,
     filter_ID = 385,
     complement_ID = 386,
     gradient_ID = 387,
     GRADIENT_TYPE = 388,
     LIMITED = 389,
     HUMIDITY_TYPE = 390,
     STRING = 391,
     MAP_ID = 392,
     NQSTRING = 393,
     VARSTRING = 394,
     CFUNC = 395,
     CFUNC_INT = 396,
     CFUNC_STR = 397,
     CFUNC_COORD = 398,
     CFUNC_REGION = 399,
     VARSTRING_INT = 400,
     VARSTRING_INT_ARRAY = 401,
     VARSTRING_STRING = 402,
     VARSTRING_STRING_ARRAY = 403,
     VARSTRING_VAR = 404,
     VARSTRING_VAR_ARRAY = 405,
     VARSTRING_COORD = 406,
     VARSTRING_COORD_ARRAY = 407,
     VARSTRING_REGION = 408,
     VARSTRING_REGION_ARRAY = 409,
     VARSTRING_MAPCHAR = 410,
     VARSTRING_MAPCHAR_ARRAY = 411,
     VARSTRING_MONST = 412,
     VARSTRING_MONST_ARRAY = 413,
     VARSTRING_OBJ = 414,
     VARSTRING_OBJ_ARRAY = 415,
     VARSTRING_SEL = 416,
     VARSTRING_SEL_ARRAY = 417,
     METHOD_INT = 418,
     METHOD_INT_ARRAY = 419,
     METHOD_STRING = 420,
     METHOD_STRING_ARRAY = 421,
     METHOD_VAR = 422,
     METHOD_VAR_ARRAY = 423,
     METHOD_COORD = 424,
     METHOD_COORD_ARRAY = 425,
     METHOD_REGION = 426,
     METHOD_REGION_ARRAY = 427,
     METHOD_MAPCHAR = 428,
     METHOD_MAPCHAR_ARRAY = 429,
     METHOD_MONST = 430,
     METHOD_MONST_ARRAY = 431,
     METHOD_OBJ = 432,
     METHOD_OBJ_ARRAY = 433,
     METHOD_SEL = 434,
     METHOD_SEL_ARRAY = 435,
     DICE = 436
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
#line 479 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 491 "lev_comp.tab.c"

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
#define YYLAST   1049

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  199
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  162
/* YYNRULES -- Number of rules.  */
#define YYNRULES  410
/* YYNRULES -- Number of states.  */
#define YYNSTATES  886

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   436

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   194,   198,     2,
     138,   139,   192,   190,   136,   191,   196,   193,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   137,     2,
       2,   195,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   140,     2,   141,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   142,   197,   143,     2,     2,     2,     2,
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
     135,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189
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
     217,   219,   221,   223,   225,   227,   229,   231,   233,   237,
     241,   247,   251,   257,   263,   269,   273,   277,   283,   289,
     295,   303,   311,   319,   325,   327,   331,   333,   337,   339,
     343,   345,   349,   351,   355,   357,   361,   363,   367,   368,
     369,   378,   383,   385,   386,   388,   390,   396,   400,   401,
     402,   412,   413,   416,   417,   423,   424,   429,   431,   434,
     436,   443,   444,   448,   449,   456,   457,   462,   463,   468,
     470,   471,   476,   480,   482,   486,   490,   496,   502,   510,
     515,   516,   527,   528,   541,   542,   545,   551,   553,   559,
     561,   567,   569,   575,   577,   587,   593,   595,   597,   599,
     601,   603,   607,   609,   611,   613,   621,   627,   629,   631,
     633,   635,   639,   640,   646,   651,   652,   656,   658,   660,
     662,   664,   667,   669,   671,   673,   675,   677,   681,   685,
     689,   691,   693,   697,   699,   701,   705,   709,   710,   716,
     719,   720,   724,   726,   730,   732,   736,   740,   742,   744,
     748,   750,   752,   754,   758,   760,   762,   764,   770,   778,
     784,   793,   795,   799,   805,   811,   819,   827,   834,   840,
     841,   844,   848,   852,   866,   870,   874,   876,   882,   892,
     898,   902,   906,   907,   918,   919,   921,   929,   933,   939,
     945,   949,   955,   963,   973,   975,   977,   979,   981,   983,
     984,   987,   989,   993,   995,   997,   999,  1001,  1003,  1005,
    1007,  1009,  1011,  1013,  1015,  1017,  1021,  1023,  1025,  1030,
    1032,  1034,  1039,  1041,  1043,  1048,  1050,  1055,  1061,  1063,
    1067,  1069,  1073,  1075,  1077,  1082,  1092,  1094,  1096,  1101,
    1103,  1109,  1111,  1113,  1118,  1120,  1122,  1128,  1130,  1132,
    1134,  1139,  1141,  1143,  1149,  1151,  1153,  1157,  1159,  1161,
    1165,  1167,  1172,  1176,  1180,  1184,  1188,  1192,  1196,  1198,
    1200,  1204,  1206,  1210,  1211,  1213,  1215,  1217,  1219,  1223,
    1224,  1226,  1228,  1231,  1234,  1239,  1246,  1251,  1258,  1265,
    1272,  1279,  1282,  1289,  1298,  1307,  1318,  1333,  1336,  1338,
    1342,  1344,  1348,  1350,  1352,  1354,  1356,  1358,  1360,  1362,
    1364,  1366,  1368,  1370,  1372,  1374,  1376,  1378,  1380,  1382,
    1393
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     200,     0,    -1,    -1,   201,    -1,   202,    -1,   202,   201,
      -1,   203,   209,   211,    -1,    16,   137,   144,    -1,    15,
     137,   144,   136,     3,    -1,    17,   137,    11,   136,   303,
      -1,    17,   137,    10,   136,     3,    -1,    17,   137,    13,
      -1,    17,   137,    12,   136,     3,   136,     3,   136,     5,
     136,     5,   136,   323,   136,   208,   207,    -1,    -1,   136,
     134,    -1,    -1,   136,   330,    -1,    -1,   136,     3,    -1,
       5,    -1,    62,    -1,    -1,    75,   137,   210,    -1,    76,
     136,   210,    -1,    76,    -1,    -1,   213,   211,    -1,   142,
     211,   143,    -1,   254,    -1,   204,    -1,   311,    -1,   312,
      -1,   313,    -1,   296,    -1,   256,    -1,   219,    -1,   218,
      -1,   306,    -1,   268,    -1,   288,    -1,   315,    -1,   316,
      -1,   298,    -1,   299,    -1,   300,    -1,   314,    -1,   234,
      -1,   244,    -1,   246,    -1,   250,    -1,   248,    -1,   231,
      -1,   241,    -1,   227,    -1,   230,    -1,   291,    -1,   273,
      -1,   289,    -1,   276,    -1,   282,    -1,   307,    -1,   302,
      -1,   294,    -1,   255,    -1,   308,    -1,   261,    -1,   259,
      -1,   301,    -1,   305,    -1,   304,    -1,   292,    -1,   293,
      -1,   295,    -1,   287,    -1,   290,    -1,   154,    -1,   156,
      -1,   158,    -1,   160,    -1,   162,    -1,   164,    -1,   166,
      -1,   168,    -1,   170,    -1,   153,    -1,   155,    -1,   157,
      -1,   159,    -1,   161,    -1,   163,    -1,   165,    -1,   167,
      -1,   169,    -1,   214,    -1,   215,    -1,   147,    -1,   147,
      -1,   215,    -1,    87,   137,   214,    -1,   216,   195,   342,
      -1,   216,   195,   125,   137,   351,    -1,   216,   195,   341,
      -1,   216,   195,   357,   137,   335,    -1,   216,   195,   356,
     137,   337,    -1,   216,   195,   355,   137,   339,    -1,   216,
     195,   330,    -1,   216,   195,   333,    -1,   216,   195,   142,
     225,   143,    -1,   216,   195,   142,   224,   143,    -1,   216,
     195,   142,   223,   143,    -1,   216,   195,   357,   137,   142,
     222,   143,    -1,   216,   195,   356,   137,   142,   221,   143,
      -1,   216,   195,   355,   137,   142,   220,   143,    -1,   216,
     195,   142,   226,   143,    -1,   340,    -1,   220,   136,   340,
      -1,   338,    -1,   221,   136,   338,    -1,   336,    -1,   222,
     136,   336,    -1,   334,    -1,   223,   136,   334,    -1,   331,
      -1,   224,   136,   331,    -1,   342,    -1,   225,   136,   342,
      -1,   341,    -1,   226,   136,   341,    -1,    -1,    -1,   116,
     146,   138,   228,   346,   139,   229,   212,    -1,   146,   138,
     349,   139,    -1,    86,    -1,    -1,     6,    -1,     6,    -1,
     140,   342,   118,   342,   141,    -1,   140,   342,   141,    -1,
      -1,    -1,    93,   235,   140,   329,   141,   236,   142,   237,
     143,    -1,    -1,   238,   237,    -1,    -1,    94,   353,   137,
     239,   211,    -1,    -1,    96,   137,   240,   211,    -1,    95,
      -1,   196,   196,    -1,    92,    -1,    91,   217,   195,   342,
     242,   342,    -1,    -1,   243,   245,   212,    -1,    -1,    90,
     140,   329,   141,   247,   212,    -1,    -1,   233,   137,   249,
     213,    -1,    -1,    81,   233,   251,   252,    -1,   212,    -1,
      -1,   212,   253,    82,   212,    -1,    14,   137,   341,    -1,
      56,    -1,    56,   137,   353,    -1,    56,   137,    62,    -1,
      47,   137,   257,   136,   257,    -1,    47,   137,   257,   136,
     353,    -1,   138,     4,   136,    61,   136,   272,   139,    -1,
     318,   232,   136,   323,    -1,    -1,    73,   137,   258,   136,
     265,   136,   267,   319,   260,   212,    -1,    -1,    41,   137,
     258,   136,   264,   136,   266,   136,   267,   319,   262,   212,
      -1,    -1,   136,     5,    -1,   138,     4,   136,     4,   139,
      -1,    62,    -1,   138,     4,   136,     4,   139,    -1,    62,
      -1,   138,   274,   136,   275,   139,    -1,    62,    -1,   138,
       4,   136,     4,   139,    -1,    62,    -1,    80,   137,   269,
     136,   322,   136,   270,   136,   272,    -1,    24,   137,   322,
     136,   351,    -1,     5,    -1,    62,    -1,   271,    -1,    62,
      -1,    61,    -1,    61,   197,   271,    -1,     4,    -1,    62,
      -1,    19,    -1,    18,   137,   274,   136,   275,   263,   145,
      -1,    18,   137,   330,   263,   145,    -1,    66,    -1,    67,
      -1,    68,    -1,    67,    -1,    22,   137,   278,    -1,    -1,
      22,   137,   278,   277,   212,    -1,   337,   136,   330,   279,
      -1,    -1,   279,   136,   280,    -1,   341,    -1,    77,    -1,
      78,    -1,   325,    -1,    79,   341,    -1,   102,    -1,   100,
      -1,   103,    -1,   104,    -1,   105,    -1,   106,   137,   329,
      -1,   107,   137,   329,    -1,   108,   137,   329,    -1,   109,
      -1,   110,    -1,   111,   137,   281,    -1,   144,    -1,   112,
      -1,   144,   197,   281,    -1,    20,   137,   284,    -1,    -1,
      21,   137,   284,   283,   212,    -1,   339,   285,    -1,    -1,
     285,   136,   286,    -1,    59,    -1,   113,   137,   337,    -1,
     354,    -1,    74,   137,   341,    -1,    88,   137,   329,    -1,
      89,    -1,    58,    -1,    97,   137,   329,    -1,   115,    -1,
      57,    -1,    98,    -1,    99,   137,   329,    -1,   100,    -1,
     101,    -1,   330,    -1,    23,   137,   317,   136,   330,    -1,
      25,   137,   330,   136,    61,   136,   322,    -1,    29,   137,
     330,   136,    61,    -1,    29,   137,   330,   136,    61,   136,
       5,   207,    -1,    30,    -1,    30,   137,   351,    -1,    37,
     137,   330,   136,    70,    -1,    38,   137,   330,   136,    70,
      -1,    38,   137,   359,   136,   359,   136,    70,    -1,    42,
     137,   359,   136,   359,   136,   144,    -1,    43,   137,   359,
     136,   359,   297,    -1,    44,   137,   359,   136,   359,    -1,
      -1,   136,    70,    -1,    50,   137,   351,    -1,    51,   137,
     351,    -1,    52,   137,   330,   136,   138,   353,   136,   353,
     139,   136,    72,   136,    71,    -1,    54,   137,   351,    -1,
      53,   137,   351,    -1,     3,    -1,   138,     3,   136,   323,
     139,    -1,    85,   137,   333,   136,   335,   136,   335,   136,
       7,    -1,    83,   137,   351,   136,   335,    -1,    39,   137,
     333,    -1,    40,   137,   333,    -1,    -1,    31,   137,   333,
     136,   323,   136,   318,   319,   309,   310,    -1,    -1,   212,
      -1,    35,   137,   330,   136,   324,   136,   326,    -1,    36,
     137,   330,    -1,   114,   137,   330,   136,   341,    -1,   114,
     137,   330,   136,    62,    -1,   114,   137,   330,    -1,    48,
     137,   342,   136,   330,    -1,    49,   137,   330,   136,   358,
     136,   341,    -1,    46,   137,   329,   136,   329,   136,   329,
     136,   329,    -1,    46,    -1,   144,    -1,    62,    -1,   144,
      -1,    62,    -1,    -1,   136,   320,    -1,   321,    -1,   321,
     136,   320,    -1,    32,    -1,    33,    -1,    34,    -1,    57,
      -1,    62,    -1,    58,    -1,    62,    -1,    65,    -1,   327,
      -1,    62,    -1,    65,    -1,   327,    -1,    64,   137,    62,
      -1,    69,    -1,    62,    -1,    64,   140,     4,   141,    -1,
     144,    -1,   155,    -1,   156,   140,   342,   141,    -1,   342,
      -1,   331,    -1,   127,   138,   351,   139,    -1,   159,    -1,
     160,   140,   342,   141,    -1,   138,     4,   136,     4,   139,
      -1,    62,    -1,    63,   332,   141,    -1,   135,    -1,   135,
     136,   332,    -1,   334,    -1,   161,    -1,   162,   140,   342,
     141,    -1,   138,     4,   136,     4,   136,     4,   136,     4,
     139,    -1,   336,    -1,   163,    -1,   164,   140,   342,   141,
      -1,     3,    -1,   138,     3,   136,   323,   139,    -1,   338,
      -1,   165,    -1,   166,   140,   342,   141,    -1,   144,    -1,
       3,    -1,   138,     3,   136,   144,   139,    -1,    62,    -1,
     340,    -1,   167,    -1,   168,   140,   342,   141,    -1,   144,
      -1,     3,    -1,   138,     3,   136,   144,   139,    -1,    62,
      -1,   328,    -1,   341,   196,   328,    -1,     4,    -1,   352,
      -1,   138,     8,   139,    -1,   153,    -1,   154,   140,   342,
     141,    -1,   342,   190,   342,    -1,   342,   191,   342,    -1,
     342,   192,   342,    -1,   342,   193,   342,    -1,   342,   194,
     342,    -1,   138,   342,   139,    -1,   149,    -1,   150,    -1,
     216,   137,   343,    -1,   344,    -1,   345,   136,   344,    -1,
      -1,   345,    -1,   342,    -1,   341,    -1,   347,    -1,   348,
     136,   347,    -1,    -1,   348,    -1,   330,    -1,   120,   333,
      -1,   121,   333,    -1,   122,   330,   136,   330,    -1,   123,
     330,   136,   330,   136,   342,    -1,   124,   138,   351,   139,
      -1,   124,   138,   271,   136,   351,   139,    -1,   130,   138,
       7,   136,   351,   139,    -1,   130,   138,   351,   136,   351,
     139,    -1,   130,   138,   335,   136,   351,   139,    -1,   126,
     330,    -1,   128,   138,   330,   136,   342,   139,    -1,   128,
     138,   330,   136,   342,   136,    32,   139,    -1,   129,   138,
     330,   136,   342,   136,   342,   139,    -1,   129,   138,   330,
     136,   342,   136,   342,   136,    32,   139,    -1,   132,   138,
     133,   136,   138,   342,   191,   342,   205,   139,   136,   330,
     206,   139,    -1,   131,   350,    -1,   169,    -1,   138,   351,
     139,    -1,   350,    -1,   350,   198,   351,    -1,   189,    -1,
       8,    -1,     9,    -1,     4,    -1,     8,    -1,     9,    -1,
       4,    -1,   352,    -1,    26,    -1,    20,    -1,    27,    -1,
      22,    -1,    28,    -1,    83,    -1,    60,    -1,    62,    -1,
     360,    -1,    45,   138,     4,   136,     4,   136,     4,   136,
       4,   139,    -1,   138,     4,   136,     4,   136,     4,   136,
       4,   139,    -1
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
     513,   514,   515,   516,   517,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   531,   532,   533,   534,   535,   536,
     537,   538,   539,   542,   543,   544,   547,   548,   551,   567,
     573,   579,   585,   591,   597,   603,   609,   615,   625,   635,
     645,   655,   665,   675,   687,   692,   699,   704,   711,   716,
     723,   727,   733,   738,   745,   749,   755,   759,   766,   788,
     765,   802,   857,   864,   867,   873,   880,   884,   893,   897,
     892,   960,   961,   965,   964,   978,   977,   992,  1002,  1003,
    1006,  1044,  1043,  1078,  1077,  1108,  1107,  1140,  1139,  1165,
    1176,  1175,  1203,  1209,  1214,  1219,  1226,  1233,  1242,  1250,
    1262,  1261,  1280,  1279,  1298,  1301,  1307,  1317,  1323,  1332,
    1338,  1343,  1349,  1354,  1360,  1371,  1377,  1378,  1381,  1382,
    1385,  1389,  1395,  1396,  1399,  1406,  1414,  1422,  1423,  1426,
    1427,  1430,  1435,  1434,  1448,  1455,  1462,  1470,  1475,  1481,
    1487,  1493,  1499,  1504,  1509,  1514,  1519,  1524,  1529,  1534,
    1539,  1544,  1549,  1557,  1566,  1570,  1583,  1592,  1591,  1609,
    1619,  1625,  1633,  1639,  1644,  1649,  1654,  1659,  1664,  1669,
    1674,  1679,  1690,  1696,  1701,  1706,  1711,  1718,  1724,  1753,
    1758,  1766,  1772,  1778,  1785,  1792,  1802,  1812,  1827,  1838,
    1841,  1847,  1853,  1859,  1865,  1871,  1877,  1882,  1889,  1896,
    1902,  1908,  1915,  1914,  1939,  1942,  1948,  1955,  1961,  1965,
    1970,  1977,  1983,  1990,  1994,  2001,  2009,  2012,  2022,  2026,
    2029,  2035,  2039,  2046,  2050,  2054,  2060,  2061,  2064,  2065,
    2068,  2069,  2070,  2076,  2077,  2078,  2084,  2085,  2088,  2097,
    2102,  2109,  2120,  2126,  2130,  2134,  2141,  2151,  2158,  2162,
    2168,  2172,  2180,  2184,  2191,  2201,  2214,  2218,  2225,  2235,
    2244,  2255,  2259,  2266,  2276,  2287,  2296,  2306,  2312,  2316,
    2323,  2333,  2344,  2353,  2363,  2370,  2371,  2377,  2381,  2385,
    2389,  2397,  2406,  2410,  2414,  2418,  2422,  2426,  2429,  2436,
    2445,  2478,  2479,  2482,  2483,  2486,  2490,  2497,  2504,  2515,
    2518,  2526,  2530,  2534,  2538,  2542,  2547,  2551,  2555,  2560,
    2565,  2570,  2574,  2579,  2584,  2588,  2592,  2597,  2601,  2608,
    2614,  2618,  2624,  2631,  2632,  2633,  2636,  2640,  2644,  2648,
    2654,  2655,  2658,  2659,  2662,  2663,  2666,  2667,  2670,  2674,
    2700
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
  "MODRON_PORTAL_ID", "POOL_ID", "SINK_ID", "NONE", "RAND_CORRIDOR_ID",
  "DOOR_STATE", "LIGHT_STATE", "CURSE_TYPE", "ENGRAVING_TYPE", "DIRECTION",
  "RANDOM_TYPE", "RANDOM_TYPE_BRACKET", "A_REGISTER", "ALIGNMENT",
  "LEFT_OR_RIGHT", "CENTER", "TOP_OR_BOT", "ALTAR_TYPE", "UP_OR_DOWN",
  "ACTIVE_OR_INACTIVE", "MODRON_PORTAL_TYPE", "SUBROOM_ID", "NAME_ID",
  "FLAGS_ID", "FLAG_TYPE", "MON_ATTITUDE", "MON_ALERTNESS",
  "MON_APPEARANCE", "ROOMDOOR_ID", "IF_ID", "ELSE_ID", "TERRAIN_ID",
  "HORIZ_OR_VERT", "REPLACE_TERRAIN_ID", "EXIT_ID", "SHUFFLE_ID",
  "QUANTITY_ID", "BURIED_ID", "LOOP_ID", "FOR_ID", "TO_ID", "SWITCH_ID",
  "CASE_ID", "BREAK_ID", "DEFAULT_ID", "ERODED_ID", "TRAPPED_STATE",
  "RECHARGED_ID", "INVIS_ID", "GREASED_ID", "FEMALE_ID", "CANCELLED_ID",
  "REVIVED_ID", "AVENGE_ID", "FLEEING_ID", "BLINDED_ID", "PARALYZED_ID",
  "STUNNED_ID", "CONFUSED_ID", "SEENTRAPS_ID", "ALL_ID", "MONTYPE_ID",
  "GRAVE_ID", "ERODEPROOF_ID", "FUNCTION_ID", "MSG_OUTPUT_TYPE",
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
  "teleprt_detail", "fountain_detail", "throne_detail",
  "modron_portal_detail", "sink_detail", "pool_detail", "terrain_type",
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
     385,   386,   387,   388,   389,   390,    44,    58,    40,    41,
      91,    93,   123,   125,   391,   392,   393,   394,   395,   396,
     397,   398,   399,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
      43,    45,    42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   199,   200,   200,   201,   201,   202,   203,   203,   204,
     204,   204,   204,   205,   205,   206,   206,   207,   207,   208,
     208,   209,   209,   210,   210,   211,   211,   212,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   216,   216,   216,   217,   217,   218,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   220,   220,   221,   221,   222,   222,
     223,   223,   224,   224,   225,   225,   226,   226,   228,   229,
     227,   230,   231,   232,   232,   233,   233,   233,   235,   236,
     234,   237,   237,   239,   238,   240,   238,   241,   242,   242,
     243,   245,   244,   247,   246,   249,   248,   251,   250,   252,
     253,   252,   254,   255,   255,   255,   256,   256,   257,   258,
     260,   259,   262,   261,   263,   263,   264,   264,   265,   265,
     266,   266,   267,   267,   268,   268,   269,   269,   270,   270,
     271,   271,   272,   272,   273,   273,   273,   274,   274,   275,
     275,   276,   277,   276,   278,   279,   279,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   281,   281,   281,   282,   283,   282,   284,
     285,   285,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   287,   288,   289,
     289,   290,   290,   291,   292,   293,   294,   295,   296,   297,
     297,   298,   299,   300,   301,   302,   303,   303,   304,   305,
     306,   307,   309,   308,   310,   310,   311,   312,   313,   313,
     313,   314,   315,   316,   316,   317,   317,   318,   318,   319,
     319,   320,   320,   321,   321,   321,   322,   322,   323,   323,
     324,   324,   324,   325,   325,   325,   326,   326,   327,   328,
     328,   328,   329,   330,   330,   330,   330,   331,   331,   331,
     332,   332,   333,   333,   333,   334,   335,   335,   335,   336,
     336,   337,   337,   337,   338,   338,   338,   338,   339,   339,
     339,   340,   340,   340,   340,   341,   341,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   343,   343,
     344,   345,   345,   346,   346,   347,   347,   348,   348,   349,
     349,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     351,   351,   352,   353,   353,   353,   354,   354,   354,   354,
     355,   355,   356,   356,   357,   357,   358,   358,   359,   359,
     360
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
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       5,     3,     5,     5,     5,     3,     3,     5,     5,     5,
       7,     7,     7,     5,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     0,     0,
       8,     4,     1,     0,     1,     1,     5,     3,     0,     0,
       9,     0,     2,     0,     5,     0,     4,     1,     2,     1,
       6,     0,     3,     0,     6,     0,     4,     0,     4,     1,
       0,     4,     3,     1,     3,     3,     5,     5,     7,     4,
       0,    10,     0,    12,     0,     2,     5,     1,     5,     1,
       5,     1,     5,     1,     9,     5,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     7,     5,     1,     1,     1,
       1,     3,     0,     5,     4,     0,     3,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     3,     3,     3,
       1,     1,     3,     1,     1,     3,     3,     0,     5,     2,
       0,     3,     1,     3,     1,     3,     3,     1,     1,     3,
       1,     1,     1,     3,     1,     1,     1,     5,     7,     5,
       8,     1,     3,     5,     5,     7,     7,     6,     5,     0,
       2,     3,     3,    13,     3,     3,     1,     5,     9,     5,
       3,     3,     0,    10,     0,     1,     7,     3,     5,     5,
       3,     5,     7,     9,     1,     1,     1,     1,     1,     0,
       2,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     4,     1,
       1,     4,     1,     1,     4,     1,     4,     5,     1,     3,
       1,     3,     1,     1,     4,     9,     1,     1,     4,     1,
       5,     1,     1,     4,     1,     1,     5,     1,     1,     1,
       4,     1,     1,     5,     1,     1,     3,     1,     1,     3,
       1,     4,     3,     3,     3,     3,     3,     3,     1,     1,
       3,     1,     3,     0,     1,     1,     1,     1,     3,     0,
       1,     1,     2,     2,     4,     6,     4,     6,     6,     6,
       6,     2,     6,     8,     8,    10,    14,     2,     1,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,    10,
       9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    21,     0,     0,     1,
       5,     0,    25,     0,     7,     0,   135,     0,     0,     0,
     194,     0,     0,     0,     0,     0,     0,     0,   251,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     284,     0,     0,     0,     0,     0,     0,     0,     0,   163,
       0,     0,     0,     0,     0,   132,     0,     0,     0,   138,
     147,     0,     0,     0,     0,    95,    84,    75,    85,    76,
      86,    77,    87,    78,    88,    79,    89,    80,    90,    81,
      91,    82,    92,    83,    29,     6,    25,    93,    94,     0,
      36,    35,    53,    54,    51,     0,    46,    52,   151,    47,
      48,    50,    49,    28,    63,    34,    66,    65,    38,    56,
      58,    59,    73,    39,    57,    74,    55,    70,    71,    62,
      72,    33,    42,    43,    44,    67,    61,    69,    68,    37,
      60,    64,    30,    31,    32,    45,    40,    41,     0,    24,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   157,     0,     0,     0,     0,
      96,    97,     0,     0,     0,     0,   347,     0,   350,     0,
     392,     0,   348,   369,    26,     0,   155,     0,     8,     0,
     309,   310,     0,   345,   162,     0,     0,     0,    11,   318,
       0,   197,   198,     0,     0,   315,     0,     0,   174,   313,
     342,   344,     0,   341,   339,     0,   226,   230,   338,   227,
     335,   337,     0,   334,   332,     0,   201,     0,   331,   286,
     285,     0,   296,   297,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   388,
     371,   390,   252,     0,   323,     0,     0,   322,     0,   277,
       0,     0,     0,     0,     0,   408,   270,   271,   288,   287,
       0,   133,     0,     0,     0,     0,     0,   312,     0,     0,
       0,     0,   261,   262,     0,   265,   264,   395,   393,   394,
     165,   164,     0,   186,   187,     0,     0,     0,     0,    98,
       0,     0,     0,   280,   128,     0,     0,     0,     0,   137,
       0,     0,     0,     0,     0,   366,   365,   367,   370,     0,
     401,   403,   400,   402,   404,   405,     0,     0,     0,   105,
     106,   101,    99,     0,     0,     0,     0,    25,   152,    23,
       0,     0,     0,     0,     0,   320,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   229,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   372,   373,     0,     0,     0,
     381,     0,     0,     0,   387,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   134,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   159,   158,     0,     0,   153,     0,     0,     0,
     363,   349,   357,     0,     0,   352,   353,   354,   355,   356,
       0,   131,     0,   347,     0,     0,     0,     0,   122,   120,
     126,   124,     0,     0,     0,   156,     0,     0,   346,    10,
     266,     0,     9,     0,     0,   319,     0,     0,     0,   200,
     199,   174,   175,   196,     0,     0,     0,   228,     0,     0,
     203,   205,   247,   185,     0,   249,     0,     0,   190,     0,
       0,     0,     0,   329,     0,     0,   327,     0,     0,   326,
       0,     0,   389,   391,     0,     0,   298,   299,     0,   302,
       0,   300,     0,   301,   253,     0,     0,   254,     0,   177,
       0,     0,     0,     0,     0,   259,   258,     0,     0,   166,
     167,   281,   406,   407,     0,     0,   179,     0,     0,     0,
       0,     0,   269,     0,     0,   149,     0,     0,   139,   279,
     278,     0,   361,   364,     0,   351,   136,   368,   100,     0,
       0,   109,     0,   108,     0,   107,     0,   113,     0,   104,
       0,   103,     0,   102,    27,   311,     0,     0,   321,   314,
       0,   316,     0,     0,   340,   398,   396,   397,   241,   238,
     232,     0,     0,   237,     0,   242,     0,   244,   245,     0,
     240,   231,   246,   399,   234,     0,   333,   204,     0,     0,
     374,     0,     0,     0,   376,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   324,     0,     0,     0,     0,     0,
       0,     0,     0,   169,     0,     0,     0,   257,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   154,   148,   150,
       0,     0,     0,   129,     0,   121,   123,   125,   127,     0,
     114,     0,   116,     0,   118,     0,     0,   317,   195,   343,
       0,     0,     0,     0,     0,   336,     0,   248,    17,     0,
     191,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   289,     0,   307,   306,   276,     0,     0,   255,     0,
     181,     0,     0,   256,   260,     0,     0,   282,     0,     0,
     183,     0,   289,   189,     0,   188,   161,     0,   141,   358,
     359,   360,   362,     0,     0,   112,     0,   111,     0,   110,
       0,     0,   235,   236,   239,   243,   233,     0,   303,   208,
     209,     0,   213,   212,   214,   215,   216,     0,     0,     0,
     220,   221,     0,   206,   210,   304,   207,     0,   250,   375,
     377,     0,   382,     0,   378,     0,   328,   380,   379,     0,
       0,     0,   272,   308,     0,     0,     0,     0,     0,     0,
     192,   193,     0,     0,     0,     0,   170,     0,     0,     0,
       0,     0,   141,   130,   115,   117,   119,   267,     0,     0,
     211,     0,     0,     0,     0,    18,     0,     0,   330,     0,
       0,   293,   294,   295,   290,   291,   274,     0,     0,   176,
       0,   289,   283,   168,     0,   178,     0,     0,   184,   268,
       0,   145,   140,   142,     0,   305,   217,   218,   219,   224,
     223,   222,   383,     0,   384,   353,     0,     0,   275,   273,
       0,     0,     0,   172,     0,     0,   171,   143,    25,     0,
       0,     0,     0,     0,   325,   292,     0,   410,   180,     0,
       0,   182,    25,   146,     0,   225,   385,    14,     0,   409,
     173,     0,   144,     0,     0,   263,     0,    15,    19,    20,
      17,     0,     0,    12,    16,   386
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    84,   853,   882,   748,   880,
      12,   140,    85,   348,    86,    87,    88,    89,   182,    90,
      91,   649,   651,   653,   434,   435,   436,   437,    92,   420,
     713,    93,    94,   399,    95,    96,   183,   640,   781,   782,
     862,   848,    97,   537,    98,    99,   197,   100,   534,   101,
     346,   102,   306,   413,   530,   103,   104,   105,   289,   280,
     106,   817,   107,   859,   362,   511,   528,   692,   702,   108,
     305,   704,   479,   772,   109,   217,   461,   110,   369,   236,
     597,   743,   831,   111,   366,   226,   365,   591,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   627,   122,
     123,   124,   125,   126,   452,   127,   128,   129,   130,   131,
     806,   839,   132,   133,   134,   135,   136,   137,   241,   281,
     762,   804,   805,   244,   498,   502,   744,   685,   503,   203,
     286,   260,   219,   356,   266,   267,   488,   489,   237,   238,
     227,   228,   325,   287,   711,   542,   543,   544,   327,   328,
     329,   261,   386,   192,   301,   594,   343,   344,   345,   524,
     274,   275
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -662
static const yytype_int16 yypact[] =
{
     117,   -61,   -36,   113,  -662,   117,    82,    41,    48,  -662,
    -662,    69,   673,    68,  -662,   149,  -662,    99,   105,   120,
    -662,   134,   140,   155,   163,   171,   173,   180,   193,   202,
     203,   210,   219,   221,   222,   229,   251,   253,   255,   258,
     263,   266,   267,   273,   282,   283,   284,   285,   289,   300,
     324,   334,    22,   338,   346,  -662,   351,   107,   783,  -662,
    -662,   357,   223,   126,   160,  -662,  -662,  -662,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,   673,  -662,  -662,   184,
    -662,  -662,  -662,  -662,  -662,   358,  -662,  -662,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,   390,   275,
    -662,   -50,   373,   298,    78,    78,   232,   -32,    11,   305,
     305,   742,   -64,   305,   305,   305,   264,   -64,   -64,   -15,
     -14,   -14,   -14,   126,   297,   126,   305,   742,   742,   305,
     742,   742,    84,   -15,    54,  -662,   742,   -64,   777,   126,
    -662,  -662,   218,   315,   305,   322,  -662,    25,  -662,   365,
    -662,   276,  -662,    42,  -662,    28,  -662,   349,  -662,   149,
    -662,  -662,   367,  -662,   301,   372,   375,   376,  -662,  -662,
     384,  -662,  -662,   391,   526,  -662,   415,   420,   427,  -662,
    -662,  -662,   561,  -662,  -662,   425,  -662,  -662,  -662,  -662,
    -662,  -662,   563,  -662,  -662,   441,   440,   431,  -662,  -662,
    -662,   447,  -662,  -662,   448,   449,   450,   -64,   -64,   305,
     305,   452,   305,   453,   456,   470,   742,   471,   506,  -662,
    -662,   389,  -662,   606,  -662,   472,   475,  -662,   481,  -662,
     483,   482,   609,   486,   487,  -662,  -662,  -662,  -662,  -662,
     488,   619,   636,   505,   507,   516,   517,   310,   638,   518,
     360,   519,  -662,  -662,   525,  -662,  -662,  -662,  -662,  -662,
    -662,  -662,   527,  -662,  -662,   528,   349,   536,   537,  -662,
     521,   126,   126,   538,  -662,   539,   215,   126,   126,  -662,
     126,   126,   126,   126,   126,   301,   310,  -662,   540,   541,
    -662,  -662,  -662,  -662,  -662,  -662,   544,    96,    21,  -662,
    -662,   301,   310,   545,   546,   548,   673,   673,  -662,  -662,
     126,   -50,   674,    31,   683,   552,   558,   742,   553,   126,
     187,   695,   556,   570,   126,   571,   349,   582,   126,   349,
     305,   305,   742,   669,   670,  -662,  -662,   602,   604,   729,
    -662,   305,   305,   318,  -662,   608,   610,   742,   607,   126,
      37,   242,   672,   741,   611,   678,   -14,   -18,  -662,   614,
     616,   -14,   -14,   -14,   126,   621,    71,   305,   111,   617,
     -10,    11,   679,  -662,    55,    55,  -662,   159,   624,   -13,
     759,  -662,  -662,   152,   237,    19,    19,  -662,  -662,  -662,
      42,  -662,   742,   631,   -58,   -54,   -34,   -21,  -662,  -662,
     301,   310,   161,   153,   124,  -662,   626,   455,  -662,  -662,
    -662,   767,  -662,   635,   384,  -662,   633,   769,   466,  -662,
    -662,   427,  -662,  -662,   630,   477,   274,  -662,   639,   543,
    -662,  -662,  -662,  -662,   646,   648,   305,   305,   578,   649,
     647,   652,   662,  -662,   663,   411,  -662,   660,   665,  -662,
     666,   667,  -662,  -662,   802,   587,  -662,  -662,   671,  -662,
     668,  -662,   675,  -662,  -662,   676,   805,  -662,   680,  -662,
     806,   681,    37,   810,   682,   685,  -662,   686,   754,  -662,
    -662,  -662,  -662,  -662,   687,   110,  -662,   820,   689,   708,
     763,   843,  -662,   711,   349,  -662,   658,   126,  -662,  -662,
     301,   738,  -662,   712,   737,  -662,  -662,  -662,  -662,   873,
     740,  -662,    -2,  -662,   126,  -662,   -50,  -662,    63,  -662,
      64,  -662,    67,  -662,  -662,  -662,   743,   878,  -662,  -662,
     744,  -662,   739,   746,  -662,  -662,  -662,  -662,  -662,  -662,
    -662,   745,   749,  -662,   750,  -662,   753,  -662,  -662,   755,
    -662,  -662,  -662,  -662,  -662,   752,  -662,   757,    11,   889,
    -662,   760,   834,   742,  -662,   126,   126,   742,   761,   126,
     742,   742,   762,   768,  -662,   -15,   895,    75,   899,    13,
     835,   771,     0,  -662,   772,   765,   840,  -662,   126,   796,
     -50,   798,   813,     3,   206,   349,    55,  -662,  -662,   310,
     809,   -63,   759,  -662,    85,  -662,  -662,   310,   301,    15,
    -662,    18,  -662,    24,  -662,    37,   817,  -662,  -662,  -662,
     -50,   126,   126,   126,   232,  -662,   495,  -662,   818,   126,
    -662,   816,   323,   385,   819,    37,   603,   821,   822,   126,
     952,   823,   824,  -662,  -662,  -662,   826,   953,  -662,   959,
    -662,   256,   828,  -662,  -662,   830,    59,   301,   110,   963,
    -662,   964,   823,  -662,   833,  -662,  -662,   836,   190,  -662,
    -662,  -662,  -662,   349,    63,  -662,    64,  -662,    67,  -662,
     831,   966,   301,  -662,  -662,  -662,  -662,    93,  -662,  -662,
    -662,   -50,  -662,  -662,  -662,  -662,  -662,   837,   838,   839,
    -662,  -662,   841,  -662,  -662,  -662,   301,   970,  -662,   310,
    -662,   945,  -662,   126,  -662,   842,  -662,  -662,  -662,   333,
     844,   280,  -662,  -662,   975,   846,   845,   847,     3,   126,
    -662,  -662,   848,   849,   850,   854,  -662,    59,   978,   110,
     855,   851,   190,  -662,  -662,  -662,  -662,  -662,   857,   924,
     301,   126,   126,   126,   -41,  -662,   852,   354,  -662,   126,
     991,  -662,  -662,  -662,  -662,   860,   349,   861,   994,  -662,
     187,   823,  -662,  -662,   863,  -662,   996,   349,  -662,  -662,
     864,  -662,  -662,  -662,   997,  -662,  -662,  -662,  -662,  -662,
     807,  -662,  -662,   971,  -662,   260,   866,   280,  -662,  -662,
    1002,   868,   869,  -662,   937,   871,  -662,  -662,   673,   875,
     -41,   874,   880,   876,  -662,  -662,   877,  -662,  -662,   349,
     881,  -662,   673,  -662,    37,  -662,  -662,  -662,   882,  -662,
    -662,   941,  -662,   883,   305,  -662,    72,   884,  -662,  -662,
     818,   305,   885,  -662,  -662,  -662
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -662,  -662,  1016,  -662,  -662,  -662,  -662,  -662,   142,  -662,
    -662,   827,   -86,  -297,   677,   853,   967,  -410,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,
    -662,  -662,  -662,  -662,   976,  -662,  -662,  -662,   245,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,   623,   859,
    -662,  -662,  -662,  -662,   569,  -662,  -662,  -662,   265,  -662,
    -662,  -662,  -538,   257,  -662,   344,   226,  -662,  -662,  -662,
    -662,  -662,   188,  -662,  -662,   892,  -662,  -662,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,   424,
    -661,   204,  -662,  -395,  -507,  -662,  -662,  -662,   374,   691,
    -177,  -142,  -332,   589,   341,  -312,  -393,  -527,  -420,  -522,
     605,  -515,  -138,   -48,  -662,   402,  -662,  -662,   615,  -662,
    -662,   790,  -131,   583,  -402,  -662,  -662,  -662,  -662,  -662,
    -143,  -662
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -203
static const yytype_int16 yytable[] =
{
     194,   218,   310,   204,   520,   623,   438,   245,   246,   412,
     541,   268,   269,   270,   273,   191,   529,   283,   284,   285,
     262,   532,   533,   561,   291,   186,   439,   294,    16,   186,
     239,   271,   186,   315,   450,   654,   292,   293,   652,   295,
     296,   776,   313,   650,   509,   307,   186,   278,   330,   539,
     331,   563,   526,   339,   332,   333,   334,   341,   483,   303,
     209,   210,   690,   770,   670,   700,   220,   230,   242,   467,
     483,   829,   470,   243,   263,   297,     7,   878,   550,   298,
     299,   220,   552,   209,   210,   551,   709,   710,   297,   553,
     209,   210,   298,   299,   200,   496,   705,   264,   265,   497,
     433,     8,   554,   830,   315,   201,   202,   377,   378,   555,
     380,   335,   240,     9,   297,   556,   304,   290,   298,   299,
     510,   771,   557,   631,   282,   221,   231,   483,   527,   279,
     186,   200,     1,     2,   879,   418,   214,   683,   691,   316,
     221,   701,   201,   202,   684,   326,   300,   342,   720,   687,
     843,   714,   657,   336,   716,   213,   230,    11,   715,   337,
     718,   717,    63,   187,   220,   200,   337,   719,   755,   451,
     338,   522,   200,   523,   188,   189,   201,   202,   188,   189,
     187,   188,   189,   201,   202,    13,   200,   215,   216,   264,
     265,   786,    14,   531,   785,   188,   189,   201,   202,   784,
     440,   222,   232,   667,   138,   531,    15,   223,   233,   288,
     190,   322,   323,   324,   190,   231,   222,   190,   486,   487,
     646,   680,   223,   221,   657,   139,   456,   517,   471,   472,
     789,   190,   541,   616,   187,   230,   141,   637,   645,   481,
     482,   473,   142,   707,   726,   224,   225,   179,   480,   188,
     189,   535,   490,   508,   459,   460,   493,   143,   514,   515,
     516,   446,   531,   417,   187,   521,   562,   478,   703,   423,
     424,   144,   425,   426,   427,   428,   429,   145,   575,   188,
     189,   540,   576,   577,   779,   190,   780,   486,   487,   316,
     441,   232,   146,   545,   231,   560,   773,   233,   193,   222,
     147,   548,   447,   558,   499,   223,   500,   501,   148,   271,
     149,   458,   801,   802,   803,   190,   465,   150,   234,   235,
     469,   483,   211,   212,   592,   484,   209,   210,   224,   225,
     151,   578,   579,   580,   600,   601,   209,   210,   706,   152,
     153,   495,   320,   321,   322,   323,   324,   154,   581,   320,
     321,   322,   323,   324,   422,   536,   155,   873,   156,   157,
     209,   210,   582,   583,   211,   212,   158,   209,   210,   185,
     232,   584,   585,   586,   587,   588,   233,   820,   546,   195,
     209,   210,   326,   205,   206,   207,   208,   589,   159,   590,
     160,   213,   161,   198,   318,   162,   852,   234,   235,   -13,
     163,   213,   272,   164,   165,   320,   321,   322,   323,   324,
     166,   199,   214,   311,   608,   358,   783,   319,   648,   167,
     168,   169,   170,   215,   216,   213,   171,   320,   321,   322,
     323,   324,   213,   215,   216,   288,   214,   172,   247,   248,
     249,   250,   251,   214,   252,   213,   253,   254,   255,   256,
     257,   695,   322,   323,   324,   312,   485,   215,   216,   751,
     314,   173,   752,   190,   215,   216,   320,   321,   322,   323,
     324,   174,   671,   209,   210,   176,   674,   215,   216,   677,
     678,   486,   487,   177,   723,   724,   725,   259,   178,   639,
     833,   347,   697,   834,   184,   196,   407,   351,   276,   277,
     320,   321,   322,   323,   324,   317,   647,   350,   352,   838,
     358,   353,   354,   320,   321,   322,   323,   324,   308,   355,
     846,   753,   722,   320,   799,   322,   323,   324,   746,   357,
     358,   247,   248,   249,   250,   251,   340,   252,   213,   253,
     254,   255,   256,   257,   320,   321,   322,   323,   324,   258,
     320,   321,   322,   323,   324,   359,   360,   672,   673,   727,
     728,   676,   870,   361,   363,   364,   367,   370,   209,   210,
     215,   216,   729,   730,   731,   320,   321,   322,   323,   324,
     259,   368,  -202,   371,   372,   373,   374,   387,   375,   376,
     379,   381,   812,   790,   382,   732,   565,   733,   734,   735,
     736,   737,   738,   739,   740,   741,   742,   571,   383,   385,
     388,   390,   389,   394,   826,   827,   828,   391,   574,   392,
     393,   749,   395,   396,   397,   398,   247,   248,   249,   250,
     251,   759,   252,   213,   253,   254,   255,   256,   257,   200,
     400,   401,   405,   402,   258,   320,   321,   322,   323,   324,
     201,   202,   403,   404,   406,   408,   320,   321,   322,   323,
     324,   409,   416,   410,   411,   215,   216,   320,   321,   322,
     323,   324,   414,   415,   419,   259,   430,   449,   421,    16,
     431,   432,   442,   443,   596,   444,   453,    17,   454,   457,
      18,    19,    20,    21,    22,    23,    24,    25,    26,   455,
     462,   463,    27,    28,    29,   797,   464,   466,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   468,    40,
      41,    42,    43,    44,    45,    46,    47,    48,   614,    49,
     474,   475,   877,   320,   321,   322,   323,   324,   476,   884,
     477,   491,   504,   494,   756,   505,    50,   506,   507,   492,
     512,   835,   513,    51,    52,   525,    53,   518,    54,    55,
      56,  -160,   863,    57,    58,   538,    59,   549,    60,   564,
     566,   567,   569,   570,   573,   602,   872,   320,   321,   322,
     323,   324,   598,   595,   599,   603,   604,    61,   605,    62,
     478,   209,   210,   320,   321,   322,   323,   324,   606,   607,
     609,   610,   611,   612,   209,   210,   613,   615,   616,   619,
     621,   617,   618,    63,   624,   629,   620,   622,   625,    64,
      65,   626,   628,   630,   632,   633,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,   634,   635,   608,   636,   642,   247,
     248,   249,   250,   251,   638,   252,   213,   253,   254,   255,
     256,   257,   247,   248,   249,   250,   251,   258,   252,   213,
     253,   254,   255,   256,   257,   641,   643,   644,   263,   655,
     258,   656,   660,   657,   658,   659,   661,   662,   215,   216,
     663,   665,   664,   666,   668,   478,   669,   675,   259,   682,
     679,   215,   216,   686,   680,   688,    65,   689,   687,   693,
     694,   259,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
     180,    67,   696,    69,   698,    71,    66,    73,    68,    75,
      70,    77,    72,    79,    74,    81,    76,    83,    78,   699,
      80,   708,    82,   721,   747,   750,   760,   765,   754,   761,
     757,   758,   764,   766,   768,   763,   769,   774,   775,   777,
     787,   788,   778,   795,   791,   792,   793,   796,   794,   807,
     800,   798,   808,   810,   809,   819,   825,   813,   814,   815,
     816,   832,   821,   824,   822,   836,   837,   840,   841,   844,
     845,   847,   849,   851,   850,   854,   856,   857,   858,   860,
     861,   864,   875,   866,   867,   868,   869,   871,   874,   876,
     881,    10,   883,   445,   885,   181,   349,   823,   175,   519,
     572,   309,   302,   811,   818,   767,   842,   229,   865,   681,
     745,   855,   448,   568,   712,   547,   384,   559,     0,   593
};

static const yytype_int16 yycheck[] =
{
      86,   143,   179,   141,   406,   512,   338,   149,   150,   306,
     420,   153,   154,   155,   156,    63,   411,   160,   161,   162,
     151,   414,   415,   443,   166,     4,   338,   169,     6,     4,
      62,    45,     4,     8,     3,   562,   167,   168,   560,   170,
     171,   702,   184,   558,    62,   176,     4,    62,    20,    62,
      22,   444,    62,   195,    26,    27,    28,   195,     3,     5,
      62,    63,    62,     4,   602,    62,     3,     3,    57,   366,
       3,   112,   369,    62,   138,     4,   137,     5,   136,     8,
       9,     3,   136,    62,    63,   143,   149,   150,     4,   143,
      62,    63,     8,     9,   144,    58,   634,   161,   162,    62,
       4,   137,   136,   144,     8,   155,   156,   249,   250,   143,
     252,    83,   144,     0,     4,   136,    62,   165,     8,     9,
     138,    62,   143,   525,   138,    62,    62,     3,   138,   144,
       4,   144,    15,    16,    62,   312,   138,    62,   138,   187,
      62,   138,   155,   156,    69,   193,    62,   195,   655,   136,
     811,   136,   139,   125,   136,   127,     3,    75,   143,   138,
     136,   143,   140,   138,     3,   144,   138,   143,   675,   138,
     142,    60,   144,    62,   153,   154,   155,   156,   153,   154,
     138,   153,   154,   155,   156,   144,   144,   159,   160,   161,
     162,   718,   144,   138,   716,   153,   154,   155,   156,   714,
     338,   138,   138,   598,   136,   138,   137,   144,   144,   138,
     189,   192,   193,   194,   189,    62,   138,   189,   163,   164,
     552,   136,   144,    62,   139,    76,   357,   404,   370,   371,
     137,   189,   642,   140,   138,     3,   137,   534,   550,   381,
     382,   372,   137,   636,   664,   167,   168,   140,   379,   153,
     154,    92,   383,   396,    67,    68,   387,   137,   401,   402,
     403,   347,   138,   311,   138,   407,   142,    61,    62,   317,
     318,   137,   320,   321,   322,   323,   324,   137,     4,   153,
     154,   419,     8,     9,    94,   189,    96,   163,   164,   337,
     338,   138,   137,   141,    62,   142,   698,   144,   138,   138,
     137,   432,   350,   142,    62,   144,    64,    65,   137,    45,
     137,   359,    32,    33,    34,   189,   364,   137,   165,   166,
     368,     3,    66,    67,   466,     7,    62,    63,   167,   168,
     137,    57,    58,    59,   476,   477,    62,    63,   635,   137,
     137,   389,   190,   191,   192,   193,   194,   137,    74,   190,
     191,   192,   193,   194,   139,   196,   137,   864,   137,   137,
      62,    63,    88,    89,    66,    67,   137,    62,    63,   146,
     138,    97,    98,    99,   100,   101,   144,   779,   141,   195,
      62,    63,   430,    10,    11,    12,    13,   113,   137,   115,
     137,   127,   137,     3,   118,   137,   136,   165,   166,   139,
     137,   127,   138,   137,   137,   190,   191,   192,   193,   194,
     137,   136,   138,   195,     3,     4,   713,   141,   556,   137,
     137,   137,   137,   159,   160,   127,   137,   190,   191,   192,
     193,   194,   127,   159,   160,   138,   138,   137,   120,   121,
     122,   123,   124,   138,   126,   127,   128,   129,   130,   131,
     132,   628,   192,   193,   194,   140,   138,   159,   160,   136,
     138,   137,   139,   189,   159,   160,   190,   191,   192,   193,
     194,   137,   603,    62,    63,   137,   607,   159,   160,   610,
     611,   163,   164,   137,   661,   662,   663,   169,   137,   537,
     136,   142,   630,   139,   137,   137,   136,   196,   157,   158,
     190,   191,   192,   193,   194,   140,   554,   140,   136,   806,
       4,   136,   136,   190,   191,   192,   193,   194,   177,   135,
     817,   136,   660,   190,   191,   192,   193,   194,   666,   138,
       4,   120,   121,   122,   123,   124,   195,   126,   127,   128,
     129,   130,   131,   132,   190,   191,   192,   193,   194,   138,
     190,   191,   192,   193,   194,   140,   136,   605,   606,    64,
      65,   609,   859,   136,     3,   140,     3,   136,    62,    63,
     159,   160,    77,    78,    79,   190,   191,   192,   193,   194,
     169,   140,   142,   136,   136,   136,   136,   198,   247,   248,
     138,   138,   769,   731,   138,   100,   141,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   141,   138,   138,
       4,   136,   140,     4,   791,   792,   793,   136,   141,   136,
     138,   669,   136,   136,   136,     6,   120,   121,   122,   123,
     124,   679,   126,   127,   128,   129,   130,   131,   132,   144,
       4,   136,     4,   136,   138,   190,   191,   192,   193,   194,
     155,   156,   136,   136,   136,   136,   190,   191,   192,   193,
     194,   136,   141,   136,   136,   159,   160,   190,   191,   192,
     193,   194,   136,   136,   136,   169,   136,     3,   139,     6,
     139,   137,   137,   137,   141,   137,     3,    14,   136,   136,
      17,    18,    19,    20,    21,    22,    23,    24,    25,   141,
       5,   145,    29,    30,    31,   753,   136,   136,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,   136,    46,
      47,    48,    49,    50,    51,    52,    53,    54,   141,    56,
      61,    61,   874,   190,   191,   192,   193,   194,   136,   881,
     136,   133,    70,   136,   141,     4,    73,   136,    70,   139,
     136,   799,   136,    80,    81,   138,    83,   136,    85,    86,
      87,    82,   848,    90,    91,   141,    93,   136,    95,   143,
       3,   136,   139,     4,   144,   197,   862,   190,   191,   192,
     193,   194,   136,   144,   136,   136,   139,   114,   136,   116,
      61,    62,    63,   190,   191,   192,   193,   194,   136,   136,
     140,   136,   136,   136,    62,    63,     4,   136,   140,     4,
       4,   136,   136,   140,     4,    61,   136,   136,   136,   146,
     147,   136,   136,   136,     4,   136,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   136,    82,     3,   136,   136,   120,
     121,   122,   123,   124,   196,   126,   127,   128,   129,   130,
     131,   132,   120,   121,   122,   123,   124,   138,   126,   127,
     128,   129,   130,   131,   132,   137,   139,     4,   138,   136,
     138,     3,   137,   139,   145,   139,   137,   137,   159,   160,
     137,   139,   137,   136,     5,    61,   136,   136,   169,     4,
     138,   159,   160,     4,   136,    70,   147,   136,   136,   144,
      70,   169,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     147,   154,   136,   156,   136,   158,   153,   160,   155,   162,
     157,   164,   159,   166,   161,   168,   163,   170,   165,   136,
     167,   142,   169,   136,   136,   139,     4,     4,   139,   136,
     139,   139,   136,     4,   136,   141,   136,     4,     4,   136,
     139,     5,   136,     3,   137,   137,   137,    32,   137,     4,
     136,   139,   136,   136,   139,     7,    62,   139,   139,   139,
     136,   139,   137,   136,   143,     4,   136,   136,     4,   136,
       4,   137,     5,    32,   197,   139,     4,   139,   139,    72,
     139,   136,    71,   139,   134,   139,   139,   136,   136,   136,
     136,     5,   880,   346,   139,    58,   199,   782,    52,   406,
     461,   178,   173,   768,   777,   691,   810,   145,   850,   615,
     666,   837,   351,   454,   642,   430,   256,   442,    -1,   466
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   200,   201,   202,   203,   137,   137,     0,
     201,    75,   209,   144,   144,   137,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    29,    30,    31,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    56,
      73,    80,    81,    83,    85,    86,    87,    90,    91,    93,
      95,   114,   116,   140,   146,   147,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   204,   211,   213,   214,   215,   216,
     218,   219,   227,   230,   231,   233,   234,   241,   243,   244,
     246,   248,   250,   254,   255,   256,   259,   261,   268,   273,
     276,   282,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   298,   299,   300,   301,   302,   304,   305,   306,
     307,   308,   311,   312,   313,   314,   315,   316,   136,    76,
     210,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   233,   137,   137,   137,   140,
     147,   215,   217,   235,   137,   146,     4,   138,   153,   154,
     189,   342,   352,   138,   211,   195,   137,   245,     3,   136,
     144,   155,   156,   328,   341,    10,    11,    12,    13,    62,
      63,    66,    67,   127,   138,   159,   160,   274,   330,   331,
       3,    62,   138,   144,   167,   168,   284,   339,   340,   284,
       3,    62,   138,   144,   165,   166,   278,   337,   338,    62,
     144,   317,    57,    62,   322,   330,   330,   120,   121,   122,
     123,   124,   126,   128,   129,   130,   131,   132,   138,   169,
     330,   350,   351,   138,   161,   162,   333,   334,   330,   330,
     330,    45,   138,   330,   359,   360,   333,   333,    62,   144,
     258,   318,   138,   359,   359,   359,   329,   342,   138,   257,
     342,   330,   351,   351,   330,   351,   351,     4,     8,     9,
      62,   353,   258,     5,    62,   269,   251,   351,   333,   214,
     329,   195,   140,   330,   138,     8,   342,   140,   118,   141,
     190,   191,   192,   193,   194,   341,   342,   347,   348,   349,
      20,    22,    26,    27,    28,    83,   125,   138,   142,   330,
     333,   341,   342,   355,   356,   357,   249,   142,   212,   210,
     140,   196,   136,   136,   136,   135,   332,   138,     4,   140,
     136,   136,   263,     3,   140,   285,   283,     3,   140,   277,
     136,   136,   136,   136,   136,   333,   333,   330,   330,   138,
     330,   138,   138,   138,   350,   138,   351,   198,     4,   140,
     136,   136,   136,   138,     4,   136,   136,   136,     6,   232,
       4,   136,   136,   136,   136,     4,   136,   136,   136,   136,
     136,   136,   212,   252,   136,   136,   141,   342,   329,   136,
     228,   139,   139,   342,   342,   342,   342,   342,   342,   342,
     136,   139,   137,     4,   223,   224,   225,   226,   331,   334,
     341,   342,   137,   137,   137,   213,   211,   342,   328,     3,
       3,   138,   303,     3,   136,   141,   351,   136,   342,    67,
      68,   275,     5,   145,   136,   342,   136,   212,   136,   342,
     212,   330,   330,   351,    61,    61,   136,   136,    61,   271,
     351,   330,   330,     3,     7,   138,   163,   164,   335,   336,
     351,   133,   139,   351,   136,   342,    58,    62,   323,    62,
      64,    65,   324,   327,    70,     4,   136,    70,   359,    62,
     138,   264,   136,   136,   359,   359,   359,   329,   136,   257,
     353,   330,    60,    62,   358,   138,    62,   138,   265,   322,
     253,   138,   335,   335,   247,    92,   196,   242,   141,    62,
     341,   216,   344,   345,   346,   141,   141,   347,   351,   136,
     136,   143,   136,   143,   136,   143,   136,   143,   142,   339,
     142,   337,   142,   335,   143,   141,     3,   136,   332,   139,
       4,   141,   263,   144,   141,     4,     8,     9,    57,    58,
      59,    74,    88,    89,    97,    98,    99,   100,   101,   113,
     115,   286,   330,   352,   354,   144,   141,   279,   136,   136,
     330,   330,   197,   136,   139,   136,   136,   136,     3,   140,
     136,   136,   136,     4,   141,   136,   140,   136,   136,     4,
     136,     4,   136,   323,     4,   136,   136,   297,   136,    61,
     136,   353,     4,   136,   136,    82,   136,   212,   196,   342,
     236,   137,   136,   139,     4,   334,   331,   342,   341,   220,
     340,   221,   338,   222,   336,   136,     3,   139,   145,   139,
     137,   137,   137,   137,   137,   139,   136,   322,     5,   136,
     271,   351,   342,   342,   351,   136,   342,   351,   351,   138,
     136,   318,     4,    62,    69,   326,     4,   136,    70,   136,
      62,   138,   266,   144,    70,   329,   136,   341,   136,   136,
      62,   138,   267,    62,   270,   271,   212,   335,   142,   149,
     150,   343,   344,   229,   136,   143,   136,   143,   136,   143,
     323,   136,   341,   329,   329,   329,   337,    64,    65,    77,
      78,    79,   100,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   280,   325,   327,   341,   136,   207,   342,
     139,   136,   139,   136,   139,   323,   141,   139,   139,   342,
       4,   136,   319,   141,   136,     4,     4,   274,   136,   136,
       4,    62,   272,   353,     4,     4,   319,   136,   136,    94,
      96,   237,   238,   212,   340,   338,   336,   139,     5,   137,
     341,   137,   137,   137,   137,     3,    32,   342,   139,   191,
     136,    32,    33,    34,   320,   321,   309,     4,   136,   139,
     136,   267,   329,   139,   139,   139,   136,   260,   272,     7,
     353,   137,   143,   237,   136,    62,   329,   329,   329,   112,
     144,   281,   139,   136,   139,   342,     4,   136,   212,   310,
     136,     4,   275,   319,   136,     4,   212,   137,   240,     5,
     197,    32,   136,   205,   139,   320,     4,   139,   139,   262,
      72,   139,   239,   211,   136,   281,   139,   134,   139,   139,
     212,   136,   211,   323,   136,    71,   136,   330,     5,    62,
     208,   136,   206,   207,   330,   139
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

  case 98:

/* Line 1455 of yacc.c  */
#line 552 "lev_comp.y"
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

  case 99:

/* Line 1455 of yacc.c  */
#line 568 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 574 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 580 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 586 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 592 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 598 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 604 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 610 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 616 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 626 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 636 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 646 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 656 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 666 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 676 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 688 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 693 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 700 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 705 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 712 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 717 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 724 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 728 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 734 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 739 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 746 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 750 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 756 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 760 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 766 "lev_comp.y"
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

  case 129:

/* Line 1455 of yacc.c  */
#line 788 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 792 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 803 "lev_comp.y"
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

  case 132:

/* Line 1455 of yacc.c  */
#line 858 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 864 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 868 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 874 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 881 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 885 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 893 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 897 "lev_comp.y"
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

  case 140:

/* Line 1455 of yacc.c  */
#line 920 "lev_comp.y"
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

  case 143:

/* Line 1455 of yacc.c  */
#line 965 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 975 "lev_comp.y"
    {
		  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 978 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 988 "lev_comp.y"
    {
		  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 993 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 1007 "lev_comp.y"
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

  case 151:

/* Line 1455 of yacc.c  */
#line 1044 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 1049 "lev_comp.y"
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

  case 153:

/* Line 1455 of yacc.c  */
#line 1078 "lev_comp.y"
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

  case 154:

/* Line 1455 of yacc.c  */
#line 1092 "lev_comp.y"
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

  case 155:

/* Line 1455 of yacc.c  */
#line 1108 "lev_comp.y"
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

  case 156:

/* Line 1455 of yacc.c  */
#line 1128 "lev_comp.y"
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
#line 1140 "lev_comp.y"
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

  case 158:

/* Line 1455 of yacc.c  */
#line 1160 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1166 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1176 "lev_comp.y"
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

  case 161:

/* Line 1455 of yacc.c  */
#line 1194 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1204 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1210 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1215 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1220 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1227 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1234 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1243 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1251 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1262 "lev_comp.y"
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

  case 171:

/* Line 1455 of yacc.c  */
#line 1273 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1280 "lev_comp.y"
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

  case 173:

/* Line 1455 of yacc.c  */
#line 1291 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1298 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1302 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1308 "lev_comp.y"
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

  case 177:

/* Line 1455 of yacc.c  */
#line 1318 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1324 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1333 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1339 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1344 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1350 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1355 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1361 "lev_comp.y"
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

  case 185:

/* Line 1455 of yacc.c  */
#line 1372 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (5)].i), SPO_DOOR));
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1386 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1390 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1400 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1407 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1415 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1431 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1435 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1441 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1449 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1455 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x0000;
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1463 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1471 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1476 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1482 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1488 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1494 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1500 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1505 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1510 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1515 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1520 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1525 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1530 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1535 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1540 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1545 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1550 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1558 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1567 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1571 "lev_comp.y"
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

  case 226:

/* Line 1455 of yacc.c  */
#line 1584 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1592 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1602 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1610 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1619 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1626 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1634 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1640 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1645 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1650 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1655 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1660 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1665 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1670 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1675 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1680 "lev_comp.y"
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

  case 242:

/* Line 1455 of yacc.c  */
#line 1691 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1697 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1702 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1707 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1712 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1719 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1725 "lev_comp.y"
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

  case 249:

/* Line 1455 of yacc.c  */
#line 1754 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1759 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1767 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1773 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1779 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1786 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1793 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1803 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1813 "lev_comp.y"
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

  case 258:

/* Line 1455 of yacc.c  */
#line 1828 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1838 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1842 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1848 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_FOUNTAIN));
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1854 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1860 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int)(yyvsp[(6) - (13)].i), (int)(yyvsp[(8) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1866 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1872 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1878 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1883 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1890 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1897 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1903 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1909 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1915 "lev_comp.y"
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

  case 273:

/* Line 1455 of yacc.c  */
#line 1929 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      if ( (yyvsp[(9) - (10)].i) ) {
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      } else if ( (yyvsp[(10) - (10)].i) )
			  lc_error("Cannot use lev statements in non-permanent REGION");
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1939 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1943 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1949 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1956 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1962 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1966 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1971 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1978 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1984 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1991 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1995 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 2002 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 2013 "lev_comp.y"
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

  case 289:

/* Line 1455 of yacc.c  */
#line 2026 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 2030 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 2036 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 2040 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 2047 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2051 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2055 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2071 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2079 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2089 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2098 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2103 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2110 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2121 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2127 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2131 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2135 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2142 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2152 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2159 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2163 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2169 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2173 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2181 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2185 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2192 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2202 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2215 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2219 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2226 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2236 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2245 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2256 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2260 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2267 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2277 "lev_comp.y"
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

  case 335:

/* Line 1455 of yacc.c  */
#line 2288 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2297 "lev_comp.y"
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

  case 337:

/* Line 1455 of yacc.c  */
#line 2307 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2313 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2317 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2324 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2334 "lev_comp.y"
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

  case 342:

/* Line 1455 of yacc.c  */
#line 2345 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2354 "lev_comp.y"
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

  case 344:

/* Line 1455 of yacc.c  */
#line 2364 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2370 "lev_comp.y"
    { ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2372 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2378 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2382 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2386 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2390 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2398 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2407 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2411 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2415 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2419 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2423 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2426 "lev_comp.y"
    { ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2430 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2437 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2446 "lev_comp.y"
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

  case 365:

/* Line 1455 of yacc.c  */
#line 2487 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2491 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2498 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2505 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2515 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2519 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2527 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2531 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2535 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2539 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2543 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2548 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2552 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2556 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2561 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2566 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2571 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2575 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2580 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2585 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2589 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2593 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2598 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2602 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2609 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2615 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2619 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2625 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2637 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2641 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2645 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2649 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2671 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2675 "lev_comp.y"
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

  case 410:

/* Line 1455 of yacc.c  */
#line 2701 "lev_comp.y"
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
#line 6089 "lev_comp.tab.c"
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
#line 2729 "lev_comp.y"


/*lev_comp.y*/

