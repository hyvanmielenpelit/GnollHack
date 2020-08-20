
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
     NPC_ID = 292,
     LADDER_ID = 293,
     STAIR_ID = 294,
     NON_DIGGABLE_ID = 295,
     NON_PASSWALL_ID = 296,
     ROOM_ID = 297,
     PORTAL_ID = 298,
     TELEPRT_ID = 299,
     BRANCH_ID = 300,
     LEV = 301,
     MINERALIZE_ID = 302,
     CORRIDOR_ID = 303,
     GOLD_ID = 304,
     ENGRAVING_ID = 305,
     FOUNTAIN_ID = 306,
     THRONE_ID = 307,
     MODRON_PORTAL_ID = 308,
     POOL_ID = 309,
     SINK_ID = 310,
     NONE = 311,
     RAND_CORRIDOR_ID = 312,
     DOOR_STATE = 313,
     LIGHT_STATE = 314,
     CURSE_TYPE = 315,
     ENGRAVING_TYPE = 316,
     DIRECTION = 317,
     RANDOM_TYPE = 318,
     RANDOM_TYPE_BRACKET = 319,
     A_REGISTER = 320,
     ALIGNMENT = 321,
     LEFT_OR_RIGHT = 322,
     CENTER = 323,
     TOP_OR_BOT = 324,
     ALTAR_TYPE = 325,
     UP_OR_DOWN = 326,
     ACTIVE_OR_INACTIVE = 327,
     MODRON_PORTAL_TYPE = 328,
     NPC_TYPE = 329,
     SUBROOM_ID = 330,
     NAME_ID = 331,
     FLAGS_ID = 332,
     FLAG_TYPE = 333,
     MON_ATTITUDE = 334,
     MON_ALERTNESS = 335,
     MON_APPEARANCE = 336,
     ROOMDOOR_ID = 337,
     IF_ID = 338,
     ELSE_ID = 339,
     TERRAIN_ID = 340,
     HORIZ_OR_VERT = 341,
     REPLACE_TERRAIN_ID = 342,
     EXIT_ID = 343,
     SHUFFLE_ID = 344,
     QUANTITY_ID = 345,
     BURIED_ID = 346,
     LOOP_ID = 347,
     FOR_ID = 348,
     TO_ID = 349,
     SWITCH_ID = 350,
     CASE_ID = 351,
     BREAK_ID = 352,
     DEFAULT_ID = 353,
     ERODED_ID = 354,
     TRAPPED_STATE = 355,
     RECHARGED_ID = 356,
     INVIS_ID = 357,
     GREASED_ID = 358,
     FEMALE_ID = 359,
     CANCELLED_ID = 360,
     REVIVED_ID = 361,
     AVENGE_ID = 362,
     FLEEING_ID = 363,
     BLINDED_ID = 364,
     PARALYZED_ID = 365,
     STUNNED_ID = 366,
     CONFUSED_ID = 367,
     SEENTRAPS_ID = 368,
     ALL_ID = 369,
     MONTYPE_ID = 370,
     GRAVE_ID = 371,
     ERODEPROOF_ID = 372,
     FUNCTION_ID = 373,
     MSG_OUTPUT_TYPE = 374,
     COMPARE_TYPE = 375,
     UNKNOWN_TYPE = 376,
     rect_ID = 377,
     fillrect_ID = 378,
     line_ID = 379,
     randline_ID = 380,
     grow_ID = 381,
     selection_ID = 382,
     flood_ID = 383,
     rndcoord_ID = 384,
     circle_ID = 385,
     ellipse_ID = 386,
     filter_ID = 387,
     complement_ID = 388,
     gradient_ID = 389,
     GRADIENT_TYPE = 390,
     LIMITED = 391,
     HUMIDITY_TYPE = 392,
     STRING = 393,
     MAP_ID = 394,
     NQSTRING = 395,
     VARSTRING = 396,
     CFUNC = 397,
     CFUNC_INT = 398,
     CFUNC_STR = 399,
     CFUNC_COORD = 400,
     CFUNC_REGION = 401,
     VARSTRING_INT = 402,
     VARSTRING_INT_ARRAY = 403,
     VARSTRING_STRING = 404,
     VARSTRING_STRING_ARRAY = 405,
     VARSTRING_VAR = 406,
     VARSTRING_VAR_ARRAY = 407,
     VARSTRING_COORD = 408,
     VARSTRING_COORD_ARRAY = 409,
     VARSTRING_REGION = 410,
     VARSTRING_REGION_ARRAY = 411,
     VARSTRING_MAPCHAR = 412,
     VARSTRING_MAPCHAR_ARRAY = 413,
     VARSTRING_MONST = 414,
     VARSTRING_MONST_ARRAY = 415,
     VARSTRING_OBJ = 416,
     VARSTRING_OBJ_ARRAY = 417,
     VARSTRING_SEL = 418,
     VARSTRING_SEL_ARRAY = 419,
     METHOD_INT = 420,
     METHOD_INT_ARRAY = 421,
     METHOD_STRING = 422,
     METHOD_STRING_ARRAY = 423,
     METHOD_VAR = 424,
     METHOD_VAR_ARRAY = 425,
     METHOD_COORD = 426,
     METHOD_COORD_ARRAY = 427,
     METHOD_REGION = 428,
     METHOD_REGION_ARRAY = 429,
     METHOD_MAPCHAR = 430,
     METHOD_MAPCHAR_ARRAY = 431,
     METHOD_MONST = 432,
     METHOD_MONST_ARRAY = 433,
     METHOD_OBJ = 434,
     METHOD_OBJ_ARRAY = 435,
     METHOD_SEL = 436,
     METHOD_SEL_ARRAY = 437,
     DICE = 438
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
#line 481 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 493 "lev_comp.tab.c"

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
#define YYLAST   1053

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  201
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  163
/* YYNRULES -- Number of rules.  */
#define YYNRULES  412
/* YYNRULES -- Number of states.  */
#define YYNSTATES  892

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   438

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   196,   200,     2,
     140,   141,   194,   192,   138,   193,   198,   195,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   139,     2,
       2,   197,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   142,     2,   143,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   144,   199,   145,     2,     2,     2,     2,
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
     135,   136,   137,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191
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
     217,   219,   221,   223,   225,   227,   229,   231,   233,   235,
     239,   243,   249,   253,   259,   265,   271,   275,   279,   285,
     291,   297,   305,   313,   321,   327,   329,   333,   335,   339,
     341,   345,   347,   351,   353,   357,   359,   363,   365,   369,
     370,   371,   380,   385,   387,   388,   390,   392,   398,   402,
     403,   404,   414,   415,   418,   419,   425,   426,   431,   433,
     436,   438,   445,   446,   450,   451,   458,   459,   464,   465,
     470,   472,   473,   478,   482,   484,   488,   492,   498,   504,
     512,   517,   518,   529,   530,   543,   544,   547,   553,   555,
     561,   563,   569,   571,   577,   579,   589,   595,   597,   599,
     601,   603,   605,   609,   611,   613,   615,   623,   629,   631,
     633,   635,   637,   641,   642,   648,   653,   654,   658,   660,
     662,   664,   666,   669,   671,   673,   675,   677,   679,   683,
     687,   691,   693,   695,   699,   701,   703,   707,   711,   712,
     718,   721,   722,   726,   728,   732,   734,   738,   742,   744,
     746,   750,   752,   754,   756,   760,   762,   764,   766,   772,
     780,   786,   795,   797,   801,   807,   813,   821,   829,   836,
     842,   843,   846,   850,   854,   868,   872,   876,   878,   884,
     894,   900,   904,   908,   909,   920,   921,   923,   931,   935,
     941,   947,   953,   957,   963,   971,   981,   983,   985,   987,
     989,   991,   992,   995,   997,  1001,  1003,  1005,  1007,  1009,
    1011,  1013,  1015,  1017,  1019,  1021,  1023,  1025,  1029,  1031,
    1033,  1038,  1040,  1042,  1047,  1049,  1051,  1056,  1058,  1063,
    1069,  1071,  1075,  1077,  1081,  1083,  1085,  1090,  1100,  1102,
    1104,  1109,  1111,  1117,  1119,  1121,  1126,  1128,  1130,  1136,
    1138,  1140,  1142,  1147,  1149,  1151,  1157,  1159,  1161,  1165,
    1167,  1169,  1173,  1175,  1180,  1184,  1188,  1192,  1196,  1200,
    1204,  1206,  1208,  1212,  1214,  1218,  1219,  1221,  1223,  1225,
    1227,  1231,  1232,  1234,  1236,  1239,  1242,  1247,  1254,  1259,
    1266,  1273,  1280,  1287,  1290,  1297,  1306,  1315,  1326,  1341,
    1344,  1346,  1350,  1352,  1356,  1358,  1360,  1362,  1364,  1366,
    1368,  1370,  1372,  1374,  1376,  1378,  1380,  1382,  1384,  1386,
    1388,  1390,  1401
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     202,     0,    -1,    -1,   203,    -1,   204,    -1,   204,   203,
      -1,   205,   211,   213,    -1,    16,   139,   146,    -1,    15,
     139,   146,   138,     3,    -1,    17,   139,    11,   138,   305,
      -1,    17,   139,    10,   138,     3,    -1,    17,   139,    13,
      -1,    17,   139,    12,   138,     3,   138,     3,   138,     5,
     138,     5,   138,   326,   138,   210,   209,    -1,    -1,   138,
     136,    -1,    -1,   138,   333,    -1,    -1,   138,     3,    -1,
       5,    -1,    63,    -1,    -1,    77,   139,   212,    -1,    78,
     138,   212,    -1,    78,    -1,    -1,   215,   213,    -1,   144,
     213,   145,    -1,   256,    -1,   206,    -1,   313,    -1,   314,
      -1,   315,    -1,   316,    -1,   298,    -1,   258,    -1,   221,
      -1,   220,    -1,   308,    -1,   270,    -1,   290,    -1,   318,
      -1,   319,    -1,   300,    -1,   301,    -1,   302,    -1,   317,
      -1,   236,    -1,   246,    -1,   248,    -1,   252,    -1,   250,
      -1,   233,    -1,   243,    -1,   229,    -1,   232,    -1,   293,
      -1,   275,    -1,   291,    -1,   278,    -1,   284,    -1,   309,
      -1,   304,    -1,   296,    -1,   257,    -1,   310,    -1,   263,
      -1,   261,    -1,   303,    -1,   307,    -1,   306,    -1,   294,
      -1,   295,    -1,   297,    -1,   289,    -1,   292,    -1,   156,
      -1,   158,    -1,   160,    -1,   162,    -1,   164,    -1,   166,
      -1,   168,    -1,   170,    -1,   172,    -1,   155,    -1,   157,
      -1,   159,    -1,   161,    -1,   163,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,   216,    -1,   217,    -1,   149,
      -1,   149,    -1,   217,    -1,    89,   139,   216,    -1,   218,
     197,   345,    -1,   218,   197,   127,   139,   354,    -1,   218,
     197,   344,    -1,   218,   197,   360,   139,   338,    -1,   218,
     197,   359,   139,   340,    -1,   218,   197,   358,   139,   342,
      -1,   218,   197,   333,    -1,   218,   197,   336,    -1,   218,
     197,   144,   227,   145,    -1,   218,   197,   144,   226,   145,
      -1,   218,   197,   144,   225,   145,    -1,   218,   197,   360,
     139,   144,   224,   145,    -1,   218,   197,   359,   139,   144,
     223,   145,    -1,   218,   197,   358,   139,   144,   222,   145,
      -1,   218,   197,   144,   228,   145,    -1,   343,    -1,   222,
     138,   343,    -1,   341,    -1,   223,   138,   341,    -1,   339,
      -1,   224,   138,   339,    -1,   337,    -1,   225,   138,   337,
      -1,   334,    -1,   226,   138,   334,    -1,   345,    -1,   227,
     138,   345,    -1,   344,    -1,   228,   138,   344,    -1,    -1,
      -1,   118,   148,   140,   230,   349,   141,   231,   214,    -1,
     148,   140,   352,   141,    -1,    88,    -1,    -1,     6,    -1,
       6,    -1,   142,   345,   120,   345,   143,    -1,   142,   345,
     143,    -1,    -1,    -1,    95,   237,   142,   332,   143,   238,
     144,   239,   145,    -1,    -1,   240,   239,    -1,    -1,    96,
     356,   139,   241,   213,    -1,    -1,    98,   139,   242,   213,
      -1,    97,    -1,   198,   198,    -1,    94,    -1,    93,   219,
     197,   345,   244,   345,    -1,    -1,   245,   247,   214,    -1,
      -1,    92,   142,   332,   143,   249,   214,    -1,    -1,   235,
     139,   251,   215,    -1,    -1,    83,   235,   253,   254,    -1,
     214,    -1,    -1,   214,   255,    84,   214,    -1,    14,   139,
     344,    -1,    57,    -1,    57,   139,   356,    -1,    57,   139,
      63,    -1,    48,   139,   259,   138,   259,    -1,    48,   139,
     259,   138,   356,    -1,   140,     4,   138,    62,   138,   274,
     141,    -1,   321,   234,   138,   326,    -1,    -1,    75,   139,
     260,   138,   267,   138,   269,   322,   262,   214,    -1,    -1,
      42,   139,   260,   138,   266,   138,   268,   138,   269,   322,
     264,   214,    -1,    -1,   138,     5,    -1,   140,     4,   138,
       4,   141,    -1,    63,    -1,   140,     4,   138,     4,   141,
      -1,    63,    -1,   140,   276,   138,   277,   141,    -1,    63,
      -1,   140,     4,   138,     4,   141,    -1,    63,    -1,    82,
     139,   271,   138,   325,   138,   272,   138,   274,    -1,    24,
     139,   325,   138,   354,    -1,     5,    -1,    63,    -1,   273,
      -1,    63,    -1,    62,    -1,    62,   199,   273,    -1,     4,
      -1,    63,    -1,    19,    -1,    18,   139,   276,   138,   277,
     265,   147,    -1,    18,   139,   333,   265,   147,    -1,    67,
      -1,    68,    -1,    69,    -1,    68,    -1,    22,   139,   280,
      -1,    -1,    22,   139,   280,   279,   214,    -1,   340,   138,
     333,   281,    -1,    -1,   281,   138,   282,    -1,   344,    -1,
      79,    -1,    80,    -1,   328,    -1,    81,   344,    -1,   104,
      -1,   102,    -1,   105,    -1,   106,    -1,   107,    -1,   108,
     139,   332,    -1,   109,   139,   332,    -1,   110,   139,   332,
      -1,   111,    -1,   112,    -1,   113,   139,   283,    -1,   146,
      -1,   114,    -1,   146,   199,   283,    -1,    20,   139,   286,
      -1,    -1,    21,   139,   286,   285,   214,    -1,   342,   287,
      -1,    -1,   287,   138,   288,    -1,    60,    -1,   115,   139,
     340,    -1,   357,    -1,    76,   139,   344,    -1,    90,   139,
     332,    -1,    91,    -1,    59,    -1,    99,   139,   332,    -1,
     117,    -1,    58,    -1,   100,    -1,   101,   139,   332,    -1,
     102,    -1,   103,    -1,   333,    -1,    23,   139,   320,   138,
     333,    -1,    25,   139,   333,   138,    62,   138,   325,    -1,
      29,   139,   333,   138,    62,    -1,    29,   139,   333,   138,
      62,   138,     5,   209,    -1,    30,    -1,    30,   139,   354,
      -1,    38,   139,   333,   138,    71,    -1,    39,   139,   333,
     138,    71,    -1,    39,   139,   362,   138,   362,   138,    71,
      -1,    43,   139,   362,   138,   362,   138,   146,    -1,    44,
     139,   362,   138,   362,   299,    -1,    45,   139,   362,   138,
     362,    -1,    -1,   138,    71,    -1,    51,   139,   354,    -1,
      52,   139,   354,    -1,    53,   139,   333,   138,   140,   356,
     138,   356,   141,   138,    73,   138,    72,    -1,    55,   139,
     354,    -1,    54,   139,   354,    -1,     3,    -1,   140,     3,
     138,   326,   141,    -1,    87,   139,   336,   138,   338,   138,
     338,   138,     7,    -1,    85,   139,   354,   138,   338,    -1,
      40,   139,   336,    -1,    41,   139,   336,    -1,    -1,    31,
     139,   336,   138,   326,   138,   321,   322,   311,   312,    -1,
      -1,   214,    -1,    35,   139,   333,   138,   327,   138,   329,
      -1,    36,   139,   333,    -1,    37,   139,   333,   138,    74,
      -1,   116,   139,   333,   138,   344,    -1,   116,   139,   333,
     138,    63,    -1,   116,   139,   333,    -1,    49,   139,   345,
     138,   333,    -1,    50,   139,   333,   138,   361,   138,   344,
      -1,    47,   139,   332,   138,   332,   138,   332,   138,   332,
      -1,    47,    -1,   146,    -1,    63,    -1,   146,    -1,    63,
      -1,    -1,   138,   323,    -1,   324,    -1,   324,   138,   323,
      -1,    32,    -1,    33,    -1,    34,    -1,    58,    -1,    63,
      -1,    59,    -1,    63,    -1,    66,    -1,   330,    -1,    63,
      -1,    66,    -1,   330,    -1,    65,   139,    63,    -1,    70,
      -1,    63,    -1,    65,   142,     4,   143,    -1,   146,    -1,
     157,    -1,   158,   142,   345,   143,    -1,   345,    -1,   334,
      -1,   129,   140,   354,   141,    -1,   161,    -1,   162,   142,
     345,   143,    -1,   140,     4,   138,     4,   141,    -1,    63,
      -1,    64,   335,   143,    -1,   137,    -1,   137,   138,   335,
      -1,   337,    -1,   163,    -1,   164,   142,   345,   143,    -1,
     140,     4,   138,     4,   138,     4,   138,     4,   141,    -1,
     339,    -1,   165,    -1,   166,   142,   345,   143,    -1,     3,
      -1,   140,     3,   138,   326,   141,    -1,   341,    -1,   167,
      -1,   168,   142,   345,   143,    -1,   146,    -1,     3,    -1,
     140,     3,   138,   146,   141,    -1,    63,    -1,   343,    -1,
     169,    -1,   170,   142,   345,   143,    -1,   146,    -1,     3,
      -1,   140,     3,   138,   146,   141,    -1,    63,    -1,   331,
      -1,   344,   198,   331,    -1,     4,    -1,   355,    -1,   140,
       8,   141,    -1,   155,    -1,   156,   142,   345,   143,    -1,
     345,   192,   345,    -1,   345,   193,   345,    -1,   345,   194,
     345,    -1,   345,   195,   345,    -1,   345,   196,   345,    -1,
     140,   345,   141,    -1,   151,    -1,   152,    -1,   218,   139,
     346,    -1,   347,    -1,   348,   138,   347,    -1,    -1,   348,
      -1,   345,    -1,   344,    -1,   350,    -1,   351,   138,   350,
      -1,    -1,   351,    -1,   333,    -1,   122,   336,    -1,   123,
     336,    -1,   124,   333,   138,   333,    -1,   125,   333,   138,
     333,   138,   345,    -1,   126,   140,   354,   141,    -1,   126,
     140,   273,   138,   354,   141,    -1,   132,   140,     7,   138,
     354,   141,    -1,   132,   140,   354,   138,   354,   141,    -1,
     132,   140,   338,   138,   354,   141,    -1,   128,   333,    -1,
     130,   140,   333,   138,   345,   141,    -1,   130,   140,   333,
     138,   345,   138,    32,   141,    -1,   131,   140,   333,   138,
     345,   138,   345,   141,    -1,   131,   140,   333,   138,   345,
     138,   345,   138,    32,   141,    -1,   134,   140,   135,   138,
     140,   345,   193,   345,   207,   141,   138,   333,   208,   141,
      -1,   133,   353,    -1,   171,    -1,   140,   354,   141,    -1,
     353,    -1,   353,   200,   354,    -1,   191,    -1,     8,    -1,
       9,    -1,     4,    -1,     8,    -1,     9,    -1,     4,    -1,
     355,    -1,    26,    -1,    20,    -1,    27,    -1,    22,    -1,
      28,    -1,    85,    -1,    61,    -1,    63,    -1,   363,    -1,
      46,   140,     4,   138,     4,   138,     4,   138,     4,   141,
      -1,   140,     4,   138,     4,   138,     4,   138,     4,   141,
      -1
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
     513,   514,   515,   516,   517,   518,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   532,   533,   534,   535,   536,
     537,   538,   539,   540,   543,   544,   545,   548,   549,   552,
     568,   574,   580,   586,   592,   598,   604,   610,   616,   626,
     636,   646,   656,   666,   676,   688,   693,   700,   705,   712,
     717,   724,   728,   734,   739,   746,   750,   756,   760,   767,
     789,   766,   803,   858,   865,   868,   874,   881,   885,   894,
     898,   893,   961,   962,   966,   965,   979,   978,   993,  1003,
    1004,  1007,  1045,  1044,  1079,  1078,  1109,  1108,  1141,  1140,
    1166,  1177,  1176,  1204,  1210,  1215,  1220,  1227,  1234,  1243,
    1251,  1263,  1262,  1281,  1280,  1299,  1302,  1308,  1318,  1324,
    1333,  1339,  1344,  1350,  1355,  1361,  1372,  1378,  1379,  1382,
    1383,  1386,  1390,  1396,  1397,  1400,  1407,  1415,  1423,  1424,
    1427,  1428,  1431,  1436,  1435,  1449,  1456,  1463,  1471,  1476,
    1482,  1488,  1494,  1500,  1505,  1510,  1515,  1520,  1525,  1530,
    1535,  1540,  1545,  1550,  1558,  1567,  1571,  1584,  1593,  1592,
    1610,  1620,  1626,  1634,  1640,  1645,  1650,  1655,  1660,  1665,
    1670,  1675,  1680,  1691,  1697,  1702,  1707,  1712,  1719,  1725,
    1754,  1759,  1767,  1773,  1779,  1786,  1793,  1803,  1813,  1828,
    1839,  1842,  1848,  1854,  1860,  1866,  1872,  1878,  1883,  1890,
    1897,  1903,  1909,  1916,  1915,  1940,  1943,  1949,  1956,  1962,
    1968,  1972,  1977,  1984,  1990,  1997,  2001,  2008,  2016,  2019,
    2029,  2033,  2036,  2042,  2046,  2053,  2057,  2061,  2067,  2068,
    2071,  2072,  2075,  2076,  2077,  2083,  2084,  2085,  2091,  2092,
    2095,  2104,  2109,  2116,  2127,  2133,  2137,  2141,  2148,  2158,
    2165,  2169,  2175,  2179,  2187,  2191,  2198,  2208,  2221,  2225,
    2232,  2242,  2251,  2262,  2266,  2273,  2283,  2294,  2303,  2313,
    2319,  2323,  2330,  2340,  2351,  2360,  2370,  2377,  2378,  2384,
    2388,  2392,  2396,  2404,  2413,  2417,  2421,  2425,  2429,  2433,
    2436,  2443,  2452,  2485,  2486,  2489,  2490,  2493,  2497,  2504,
    2511,  2522,  2525,  2533,  2537,  2541,  2545,  2549,  2554,  2558,
    2562,  2567,  2572,  2577,  2581,  2586,  2591,  2595,  2599,  2604,
    2608,  2615,  2621,  2625,  2631,  2638,  2639,  2640,  2643,  2647,
    2651,  2655,  2661,  2662,  2665,  2666,  2669,  2670,  2673,  2674,
    2677,  2681,  2707
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
  "NPC_ID", "LADDER_ID", "STAIR_ID", "NON_DIGGABLE_ID", "NON_PASSWALL_ID",
  "ROOM_ID", "PORTAL_ID", "TELEPRT_ID", "BRANCH_ID", "LEV",
  "MINERALIZE_ID", "CORRIDOR_ID", "GOLD_ID", "ENGRAVING_ID", "FOUNTAIN_ID",
  "THRONE_ID", "MODRON_PORTAL_ID", "POOL_ID", "SINK_ID", "NONE",
  "RAND_CORRIDOR_ID", "DOOR_STATE", "LIGHT_STATE", "CURSE_TYPE",
  "ENGRAVING_TYPE", "DIRECTION", "RANDOM_TYPE", "RANDOM_TYPE_BRACKET",
  "A_REGISTER", "ALIGNMENT", "LEFT_OR_RIGHT", "CENTER", "TOP_OR_BOT",
  "ALTAR_TYPE", "UP_OR_DOWN", "ACTIVE_OR_INACTIVE", "MODRON_PORTAL_TYPE",
  "NPC_TYPE", "SUBROOM_ID", "NAME_ID", "FLAGS_ID", "FLAG_TYPE",
  "MON_ATTITUDE", "MON_ALERTNESS", "MON_APPEARANCE", "ROOMDOOR_ID",
  "IF_ID", "ELSE_ID", "TERRAIN_ID", "HORIZ_OR_VERT", "REPLACE_TERRAIN_ID",
  "EXIT_ID", "SHUFFLE_ID", "QUANTITY_ID", "BURIED_ID", "LOOP_ID", "FOR_ID",
  "TO_ID", "SWITCH_ID", "CASE_ID", "BREAK_ID", "DEFAULT_ID", "ERODED_ID",
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
  "throne_detail", "modron_portal_detail", "sink_detail", "pool_detail",
  "terrain_type", "replace_terrain_detail", "terrain_detail",
  "diggable_detail", "passwall_detail", "region_detail", "@16",
  "region_detail_end", "altar_detail", "anvil_detail", "npc_detail",
  "grave_detail", "gold_detail", "engraving_detail", "mineralize",
  "trap_name", "room_type", "optroomregionflags", "roomregionflags",
  "roomregionflag", "door_state", "light_state", "alignment",
  "alignment_prfx", "altar_type", "a_register", "string_or_var",
  "integer_or_var", "coord_or_var", "encodecoord", "humidity_flags",
  "region_or_var", "encoderegion", "mapchar_or_var", "mapchar",
  "monster_or_var", "encodemonster", "object_or_var", "encodeobj",
  "string_expr", "math_expr_var", "func_param_type", "func_param_part",
  "func_param_list", "func_params_list", "func_call_param_part",
  "func_call_param_list", "func_call_params_list", "ter_selection_x",
  "ter_selection", "dice", "all_integers", "all_ints_push", "objectid",
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
     385,   386,   387,   388,   389,   390,   391,   392,    44,    58,
      40,    41,    91,    93,   123,   125,   393,   394,   395,   396,
     397,   398,   399,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,    43,    45,    42,    47,    37,    61,    46,   124,
      38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   201,   202,   202,   203,   203,   204,   205,   205,   206,
     206,   206,   206,   207,   207,   208,   208,   209,   209,   210,
     210,   211,   211,   212,   212,   213,   213,   214,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   218,   218,   218,   219,   219,   220,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   222,   222,   223,   223,   224,
     224,   225,   225,   226,   226,   227,   227,   228,   228,   230,
     231,   229,   232,   233,   234,   234,   235,   235,   235,   237,
     238,   236,   239,   239,   241,   240,   242,   240,   243,   244,
     244,   245,   247,   246,   249,   248,   251,   250,   253,   252,
     254,   255,   254,   256,   257,   257,   257,   258,   258,   259,
     260,   262,   261,   264,   263,   265,   265,   266,   266,   267,
     267,   268,   268,   269,   269,   270,   270,   271,   271,   272,
     272,   273,   273,   274,   274,   275,   275,   275,   276,   276,
     277,   277,   278,   279,   278,   280,   281,   281,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   283,   283,   283,   284,   285,   284,
     286,   287,   287,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   289,   290,
     291,   291,   292,   292,   293,   294,   295,   296,   297,   298,
     299,   299,   300,   301,   302,   303,   304,   305,   305,   306,
     307,   308,   309,   311,   310,   312,   312,   313,   314,   315,
     316,   316,   316,   317,   318,   319,   319,   320,   320,   321,
     321,   322,   322,   323,   323,   324,   324,   324,   325,   325,
     326,   326,   327,   327,   327,   328,   328,   328,   329,   329,
     330,   331,   331,   331,   332,   333,   333,   333,   333,   334,
     334,   334,   335,   335,   336,   336,   336,   337,   338,   338,
     338,   339,   339,   340,   340,   340,   341,   341,   341,   341,
     342,   342,   342,   343,   343,   343,   343,   344,   344,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     346,   346,   347,   348,   348,   349,   349,   350,   350,   351,
     351,   352,   352,   353,   353,   353,   353,   353,   353,   353,
     353,   353,   353,   353,   353,   353,   353,   353,   353,   353,
     353,   353,   354,   354,   355,   356,   356,   356,   357,   357,
     357,   357,   358,   358,   359,   359,   360,   360,   361,   361,
     362,   362,   363
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     5,     3,     5,     5,     5,     3,     3,     5,     5,
       5,     7,     7,     7,     5,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     0,
       0,     8,     4,     1,     0,     1,     1,     5,     3,     0,
       0,     9,     0,     2,     0,     5,     0,     4,     1,     2,
       1,     6,     0,     3,     0,     6,     0,     4,     0,     4,
       1,     0,     4,     3,     1,     3,     3,     5,     5,     7,
       4,     0,    10,     0,    12,     0,     2,     5,     1,     5,
       1,     5,     1,     5,     1,     9,     5,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     7,     5,     1,     1,
       1,     1,     3,     0,     5,     4,     0,     3,     1,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     3,     3,
       3,     1,     1,     3,     1,     1,     3,     3,     0,     5,
       2,     0,     3,     1,     3,     1,     3,     3,     1,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     5,     7,
       5,     8,     1,     3,     5,     5,     7,     7,     6,     5,
       0,     2,     3,     3,    13,     3,     3,     1,     5,     9,
       5,     3,     3,     0,    10,     0,     1,     7,     3,     5,
       5,     5,     3,     5,     7,     9,     1,     1,     1,     1,
       1,     0,     2,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     1,
       4,     1,     1,     4,     1,     1,     4,     1,     4,     5,
       1,     3,     1,     3,     1,     1,     4,     9,     1,     1,
       4,     1,     5,     1,     1,     4,     1,     1,     5,     1,
       1,     1,     4,     1,     1,     5,     1,     1,     3,     1,
       1,     3,     1,     4,     3,     3,     3,     3,     3,     3,
       1,     1,     3,     1,     3,     0,     1,     1,     1,     1,
       3,     0,     1,     1,     2,     2,     4,     6,     4,     6,
       6,     6,     6,     2,     6,     8,     8,    10,    14,     2,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,    10,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    21,     0,     0,     1,
       5,     0,    25,     0,     7,     0,   136,     0,     0,     0,
     195,     0,     0,     0,     0,     0,     0,     0,   252,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   286,     0,     0,     0,     0,     0,     0,     0,     0,
     164,     0,     0,     0,     0,     0,   133,     0,     0,     0,
     139,   148,     0,     0,     0,     0,    96,    85,    76,    86,
      77,    87,    78,    88,    79,    89,    80,    90,    81,    91,
      82,    92,    83,    93,    84,    29,     6,    25,    94,    95,
       0,    37,    36,    54,    55,    52,     0,    47,    53,   152,
      48,    49,    51,    50,    28,    64,    35,    67,    66,    39,
      57,    59,    60,    74,    40,    58,    75,    56,    71,    72,
      63,    73,    34,    43,    44,    45,    68,    62,    70,    69,
      38,    61,    65,    30,    31,    32,    33,    46,    41,    42,
       0,    24,    22,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   158,     0,
       0,     0,     0,    97,    98,     0,     0,     0,     0,   349,
       0,   352,     0,   394,     0,   350,   371,    26,     0,   156,
       0,     8,     0,   311,   312,     0,   347,   163,     0,     0,
       0,    11,   320,     0,   198,   199,     0,     0,   317,     0,
       0,   175,   315,   344,   346,     0,   343,   341,     0,   227,
     231,   340,   228,   337,   339,     0,   336,   334,     0,   202,
       0,   333,   288,   287,     0,   298,   299,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   390,   373,   392,   253,     0,   325,     0,     0,
     324,     0,   278,     0,     0,     0,     0,     0,     0,   410,
     271,   272,   290,   289,     0,   134,     0,     0,     0,     0,
       0,   314,     0,     0,     0,     0,   262,   263,     0,   266,
     265,   397,   395,   396,   166,   165,     0,   187,   188,     0,
       0,     0,     0,    99,     0,     0,     0,   282,   129,     0,
       0,     0,     0,   138,     0,     0,     0,     0,     0,   368,
     367,   369,   372,     0,   403,   405,   402,   404,   406,   407,
       0,     0,     0,   106,   107,   102,   100,     0,     0,     0,
       0,    25,   153,    23,     0,     0,     0,     0,     0,   322,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   230,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   374,
     375,     0,     0,     0,   383,     0,     0,     0,   389,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   135,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   160,   159,     0,
       0,   154,     0,     0,     0,   365,   351,   359,     0,     0,
     354,   355,   356,   357,   358,     0,   132,     0,   349,     0,
       0,     0,     0,   123,   121,   127,   125,     0,     0,     0,
     157,     0,     0,   348,    10,   267,     0,     9,     0,     0,
     321,     0,     0,     0,   201,   200,   175,   176,   197,     0,
       0,     0,   229,     0,     0,   204,   206,   248,   186,     0,
     250,     0,     0,   191,     0,     0,     0,     0,   331,     0,
       0,   329,     0,     0,   328,     0,     0,   391,   393,     0,
       0,   300,   301,     0,   304,     0,   302,     0,   303,   279,
     254,     0,     0,   255,     0,   178,     0,     0,     0,     0,
       0,   260,   259,     0,     0,   167,   168,   283,   408,   409,
       0,     0,   180,     0,     0,     0,     0,     0,   270,     0,
       0,   150,     0,     0,   140,   281,   280,     0,   363,   366,
       0,   353,   137,   370,   101,     0,     0,   110,     0,   109,
       0,   108,     0,   114,     0,   105,     0,   104,     0,   103,
      27,   313,     0,     0,   323,   316,     0,   318,     0,     0,
     342,   400,   398,   399,   242,   239,   233,     0,     0,   238,
       0,   243,     0,   245,   246,     0,   241,   232,   247,   401,
     235,     0,   335,   205,     0,     0,   376,     0,     0,     0,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     326,     0,     0,     0,     0,     0,     0,     0,     0,   170,
       0,     0,     0,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   155,   149,   151,     0,     0,     0,   130,
       0,   122,   124,   126,   128,     0,   115,     0,   117,     0,
     119,     0,     0,   319,   196,   345,     0,     0,     0,     0,
       0,   338,     0,   249,    17,     0,   192,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   291,     0,   309,
     308,   277,     0,     0,   256,     0,   182,     0,     0,   257,
     261,     0,     0,   284,     0,     0,   184,     0,   291,   190,
       0,   189,   162,     0,   142,   360,   361,   362,   364,     0,
       0,   113,     0,   112,     0,   111,     0,     0,   236,   237,
     240,   244,   234,     0,   305,   209,   210,     0,   214,   213,
     215,   216,   217,     0,     0,     0,   221,   222,     0,   207,
     211,   306,   208,     0,   251,   377,   379,     0,   384,     0,
     380,     0,   330,   382,   381,     0,     0,     0,   273,   310,
       0,     0,     0,     0,     0,     0,   193,   194,     0,     0,
       0,     0,   171,     0,     0,     0,     0,     0,   142,   131,
     116,   118,   120,   268,     0,     0,   212,     0,     0,     0,
       0,    18,     0,     0,   332,     0,     0,   295,   296,   297,
     292,   293,   275,     0,     0,   177,     0,   291,   285,   169,
       0,   179,     0,     0,   185,   269,     0,   146,   141,   143,
       0,   307,   218,   219,   220,   225,   224,   223,   385,     0,
     386,   355,     0,     0,   276,   274,     0,     0,     0,   173,
       0,     0,   172,   144,    25,     0,     0,     0,     0,     0,
     327,   294,     0,   412,   181,     0,     0,   183,    25,   147,
       0,   226,   387,    14,     0,   411,   174,     0,   145,     0,
       0,   264,     0,    15,    19,    20,    17,     0,     0,    12,
      16,   388
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    85,   859,   888,   754,   886,
      12,   142,    86,   352,    87,    88,    89,    90,   185,    91,
      92,   655,   657,   659,   439,   440,   441,   442,    93,   425,
     719,    94,    95,   404,    96,    97,   186,   646,   787,   788,
     868,   854,    98,   543,    99,   100,   200,   101,   540,   102,
     350,   103,   310,   418,   536,   104,   105,   106,   293,   284,
     107,   823,   108,   865,   366,   517,   534,   698,   708,   109,
     309,   710,   484,   778,   110,   220,   466,   111,   373,   239,
     603,   749,   837,   112,   370,   229,   369,   597,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   633,   123,
     124,   125,   126,   127,   457,   128,   129,   130,   131,   132,
     812,   845,   133,   134,   135,   136,   137,   138,   139,   244,
     285,   768,   810,   811,   247,   503,   507,   750,   691,   508,
     206,   290,   263,   222,   360,   269,   270,   493,   494,   240,
     241,   230,   231,   329,   291,   717,   548,   549,   550,   331,
     332,   333,   264,   390,   195,   305,   600,   347,   348,   349,
     530,   278,   279
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -677
static const yytype_int16 yypact[] =
{
     344,   -86,   -78,    76,  -677,   344,    21,   -44,   -20,  -677,
    -677,   -24,   677,     9,  -677,    80,  -677,    56,    64,    71,
    -677,    96,   112,   124,   131,   142,   158,   162,   180,   182,
     184,   187,   189,   195,   197,   200,   203,   235,   246,   260,
     272,   281,   282,   285,   286,   287,   289,   291,   303,   304,
     309,   316,   325,    30,   342,   343,  -677,   346,    38,   752,
    -677,  -677,   347,    51,    61,    23,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,   677,  -677,  -677,
      50,  -677,  -677,  -677,  -677,  -677,   358,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
     375,   223,  -677,   -57,   404,    -8,   143,   143,    41,   -16,
      55,   222,   222,   616,   -68,   222,   222,   222,   222,   236,
     -68,   -68,   -15,     3,     3,     3,    61,   256,    61,   222,
     616,   616,   222,   616,   616,    77,   -15,    89,  -677,   616,
     -68,   754,    61,  -677,  -677,   240,   331,   222,   318,  -677,
      58,  -677,   359,  -677,   198,  -677,    65,  -677,   283,  -677,
     356,  -677,    80,  -677,  -677,   362,  -677,   307,   369,   371,
     372,  -677,  -677,   374,  -677,  -677,   373,   511,  -677,   378,
     380,   386,  -677,  -677,  -677,   513,  -677,  -677,   383,  -677,
    -677,  -677,  -677,  -677,  -677,   524,  -677,  -677,   389,   390,
     395,  -677,  -677,  -677,   397,  -677,  -677,   398,   413,   414,
     -68,   -68,   222,   222,   402,   222,   416,   418,   421,   616,
     422,   510,  -677,  -677,   354,  -677,   553,  -677,   423,   428,
    -677,   437,  -677,   440,   441,   454,   576,   462,   463,  -677,
    -677,  -677,  -677,  -677,   464,   592,   611,   478,   484,   485,
     487,   465,   622,   490,   257,   491,  -677,  -677,   493,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,   530,  -677,  -677,   531,
     356,   532,   540,  -677,   508,    61,    61,   544,  -677,   521,
     213,    61,    61,  -677,    61,    61,    61,    61,    61,   307,
     465,  -677,   551,   549,  -677,  -677,  -677,  -677,  -677,  -677,
     554,   152,    27,  -677,  -677,   307,   465,   564,   565,   566,
     677,   677,  -677,  -677,    61,   -57,   689,    60,   706,   572,
     580,   616,   597,    61,   295,   732,   606,   617,    61,   620,
     356,   623,    61,   356,   222,   222,   616,   692,   701,  -677,
    -677,   630,   633,   459,  -677,   222,   222,   337,  -677,   638,
     634,   616,   641,    61,    15,   267,   702,   709,   784,   651,
     719,     3,   -21,  -677,   653,   654,     3,     3,     3,    61,
     656,    84,   222,   164,   657,   -18,    55,   712,  -677,    31,
      31,  -677,   177,   655,   -34,   713,  -677,  -677,   376,   417,
     154,   154,  -677,  -677,  -677,    65,  -677,   616,   661,   -63,
     -39,   -31,   -28,  -677,  -677,   307,   465,    24,   185,   125,
    -677,   660,   453,  -677,  -677,  -677,   803,  -677,   669,   374,
    -677,   667,   805,   460,  -677,  -677,   386,  -677,  -677,   664,
     471,    75,  -677,   665,   481,  -677,  -677,  -677,  -677,   674,
     678,   222,   222,   618,   680,   679,   683,   684,  -677,   685,
     415,  -677,   673,   686,  -677,   693,   725,  -677,  -677,   826,
     590,  -677,  -677,   726,  -677,   723,  -677,   728,  -677,  -677,
    -677,   729,   847,  -677,   749,  -677,   882,   750,    15,   885,
     753,   755,  -677,   757,   828,  -677,  -677,  -677,  -677,  -677,
     758,   153,  -677,   888,   759,   760,   815,   897,  -677,   764,
     356,  -677,   705,    61,  -677,  -677,   307,   765,  -677,   770,
     786,  -677,  -677,  -677,  -677,   921,   788,  -677,   -13,  -677,
      61,  -677,   -57,  -677,   156,  -677,   176,  -677,    67,  -677,
    -677,  -677,   791,   927,  -677,  -677,   790,  -677,   785,   792,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,   795,   796,  -677,
     797,  -677,   798,  -677,  -677,   799,  -677,  -677,  -677,  -677,
    -677,   800,  -677,   801,    55,   935,  -677,   804,   881,   616,
    -677,    61,    61,   616,   806,    61,   616,   616,   807,   808,
    -677,   -15,   941,    74,   944,   -61,   878,   812,   -11,  -677,
     813,   809,   883,  -677,    61,   814,   -57,   818,   819,     5,
     319,   356,    31,  -677,  -677,   465,   816,   237,   713,  -677,
     134,  -677,  -677,   465,   307,    88,  -677,   108,  -677,   150,
    -677,    15,   820,  -677,  -677,  -677,   -57,    61,    61,    61,
      41,  -677,   748,  -677,   821,    61,  -677,   822,   239,   298,
     823,    15,   608,   824,   825,    61,   949,   829,   827,  -677,
    -677,  -677,   830,   957,  -677,   958,  -677,   335,   831,  -677,
    -677,   833,    53,   307,   153,   968,  -677,   969,   829,  -677,
     836,  -677,  -677,   837,   161,  -677,  -677,  -677,  -677,   356,
     156,  -677,   176,  -677,    67,  -677,   835,   972,   307,  -677,
    -677,  -677,  -677,   175,  -677,  -677,  -677,   -57,  -677,  -677,
    -677,  -677,  -677,   839,   840,   841,  -677,  -677,   842,  -677,
    -677,  -677,   307,   979,  -677,   465,  -677,   951,  -677,    61,
    -677,   843,  -677,  -677,  -677,   492,   848,   323,  -677,  -677,
     981,   849,   850,   851,     5,    61,  -677,  -677,   852,   853,
     854,   858,  -677,    53,   983,   153,   859,   855,   161,  -677,
    -677,  -677,  -677,  -677,   861,   925,   307,    61,    61,    61,
     -43,  -677,   856,   412,  -677,    61,   988,  -677,  -677,  -677,
    -677,   863,   356,   864,   999,  -677,   295,   829,  -677,  -677,
     866,  -677,  1001,   356,  -677,  -677,   867,  -677,  -677,  -677,
    1002,  -677,  -677,  -677,  -677,  -677,   810,  -677,  -677,   976,
    -677,   334,   869,   323,  -677,  -677,  1007,   871,   872,  -677,
     942,   873,  -677,  -677,   677,   879,   -43,   875,   884,   877,
    -677,  -677,   880,  -677,  -677,   356,   886,  -677,   677,  -677,
      15,  -677,  -677,  -677,   887,  -677,  -677,   947,  -677,   889,
     222,  -677,    92,   890,  -677,  -677,   821,   222,   891,  -677,
    -677,  -677
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -677,  -677,  1017,  -677,  -677,  -677,  -677,  -677,   137,  -677,
    -677,   832,   -87,  -306,   676,   857,   970,  -404,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,   977,  -677,  -677,  -677,   243,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,   624,   860,
    -677,  -677,  -677,  -677,   567,  -677,  -677,  -677,   263,  -677,
    -677,  -677,  -535,   258,  -677,   345,   224,  -677,  -677,  -677,
    -677,  -677,   183,  -677,  -677,   896,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
     424,  -676,   201,  -677,  -392,  -512,  -677,  -677,  -677,   377,
     691,  -180,  -144,  -320,   588,   206,  -316,  -410,  -538,  -425,
    -533,   601,  -529,  -140,   -48,  -677,   403,  -677,  -677,   615,
    -677,  -677,   793,  -133,   582,  -406,  -677,  -677,  -677,  -677,
    -677,  -146,  -677
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -204
static const yytype_int16 yytable[] =
{
     197,   221,   314,   207,   417,   526,   629,   248,   249,   538,
     539,   271,   272,   273,   274,   277,   194,   287,   288,   289,
     265,   547,   443,   567,   535,   295,   444,   223,   298,   545,
     660,   189,   782,   658,   488,   656,    16,   296,   297,   569,
     299,   300,   515,   317,   233,   532,   311,   242,   282,   275,
     212,   213,   696,     7,   343,   212,   213,   776,   345,   214,
     215,     8,   189,   455,   472,   189,   319,   475,   706,   189,
     488,   835,   266,   676,   501,   556,     9,   693,   502,   581,
     663,   301,   557,   582,   583,   302,   303,   224,   301,   203,
     212,   213,   302,   303,   307,   267,   268,   884,    11,   558,
     204,   205,    13,   836,   234,   711,   559,   560,   381,   382,
     562,   384,   203,   245,   561,    15,   777,   563,   246,   516,
     294,   216,   533,   204,   205,   637,    14,   217,   488,   697,
     243,   283,   217,   584,   585,   586,   423,   689,   212,   213,
     304,   849,   320,   286,   690,   707,   223,   140,   330,   726,
     346,   587,   308,   218,   219,   885,   438,   301,   141,   223,
     319,   302,   303,   196,   225,   588,   589,   341,   564,   761,
     226,   537,    64,   203,   590,   591,   592,   593,   594,   233,
     182,   235,   191,   192,   204,   205,   792,   236,   233,   791,
     595,   790,   596,   227,   228,   143,   491,   492,   190,   188,
     456,   190,   445,   144,   216,   190,   224,   537,   237,   238,
     145,   203,   673,   191,   192,   217,   191,   192,   193,   224,
     191,   192,   204,   205,   292,   528,   720,   529,   461,   523,
     476,   477,   713,   721,   643,   146,   218,   219,   652,   234,
     651,   486,   487,   478,   547,   732,   722,   198,   234,   193,
     485,   147,   193,   723,   495,   514,   193,   785,   498,   786,
     520,   521,   522,   148,   451,   537,   193,   422,   527,   568,
     149,   541,   686,   428,   429,   663,   430,   431,   432,   433,
     434,   150,   275,   225,   546,   212,   213,   189,   724,   226,
     491,   492,   190,   320,   446,   725,   225,   151,   779,   212,
     213,   152,   226,   334,   554,   335,   452,   191,   192,   336,
     337,   338,   227,   228,   795,   463,   235,   622,   322,   153,
     470,   154,   236,   155,   474,   235,   156,   598,   157,   566,
     504,   236,   505,   506,   158,   712,   159,   606,   607,   160,
     488,   323,   161,   193,   489,   500,   212,   213,   326,   327,
     328,   216,   237,   238,   427,   807,   808,   809,   879,     1,
       2,   202,   217,   464,   465,   216,   280,   281,   339,   324,
     325,   326,   327,   328,   162,   542,   276,   757,   201,   826,
     758,   483,   709,   218,   219,   163,   312,   330,   715,   716,
     324,   325,   326,   327,   328,   412,   292,   218,   219,   164,
     212,   213,   214,   215,   344,   324,   325,   326,   327,   328,
     340,   165,   216,   789,   208,   209,   210,   211,   614,   362,
     166,   167,   654,   341,   168,   169,   170,   342,   171,   203,
     172,   324,   325,   326,   327,   328,   759,   315,   191,   192,
     204,   205,   173,   174,   218,   219,   267,   268,   175,   324,
     325,   326,   327,   328,   701,   176,   379,   380,   318,   250,
     251,   252,   253,   254,   177,   255,   216,   256,   257,   258,
     259,   260,   858,   316,   193,   -13,   677,   490,   212,   213,
     680,   179,   180,   683,   684,   181,   187,   729,   730,   731,
     324,   325,   326,   327,   328,   645,   703,   199,   218,   219,
     351,   321,   491,   492,   354,   355,   844,   356,   262,   357,
     358,   359,   653,   361,   362,   362,   367,   852,   364,   551,
     363,   483,   212,   213,   365,   368,   728,   371,   326,   327,
     328,   372,   752,   374,  -203,   375,   376,   250,   251,   252,
     253,   254,   383,   255,   216,   256,   257,   258,   259,   260,
     839,   377,   378,   840,   391,   261,   385,   392,   386,   876,
     552,   387,   389,   678,   679,   393,   394,   682,   324,   325,
     326,   327,   328,   212,   213,   395,   218,   219,   396,   397,
     399,   250,   251,   252,   253,   254,   262,   255,   216,   256,
     257,   258,   259,   260,   398,   818,   571,   796,   403,   261,
     400,   401,   402,   577,   324,   325,   326,   327,   328,   324,
     325,   326,   327,   328,   580,   405,   406,   832,   833,   834,
     218,   219,   407,   408,   602,   409,   410,   755,   411,   413,
     262,   414,   250,   251,   252,   253,   254,   765,   255,   216,
     256,   257,   258,   259,   260,   324,   325,   326,   327,   328,
     261,   421,   324,   325,   326,   327,   328,   324,   325,   326,
     327,   328,   426,   324,   325,   326,   327,   328,   415,   416,
     419,   218,   219,   324,   325,   326,   327,   328,   420,   212,
     213,   262,   424,    16,   324,   805,   326,   327,   328,   435,
     436,    17,   454,   437,    18,    19,    20,    21,    22,    23,
      24,    25,    26,   447,   448,   449,    27,    28,    29,   458,
     459,   803,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,   460,    41,    42,    43,    44,    45,    46,
      47,    48,    49,   620,    50,   462,   883,   467,   250,   251,
     252,   253,   254,   890,   255,   216,   256,   257,   258,   259,
     260,   762,    51,   468,   479,   469,   261,   841,   471,    52,
      53,   473,    54,   480,    55,    56,    57,   869,   481,    58,
      59,   482,    60,   496,    61,   497,   509,   218,   219,   499,
     510,   878,   324,   325,   326,   327,   328,   262,   511,   512,
     513,   518,   519,    62,   524,    63,  -161,   531,   544,   555,
     324,   325,   326,   327,   328,   570,   572,   573,   575,   576,
     579,   601,   604,   733,   734,   615,   605,   608,   609,    64,
     610,   611,   612,   613,   616,    65,    66,   735,   736,   737,
     619,   617,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
     738,   625,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,    66,   618,   621,   622,   623,   624,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,   627,   626,   628,   630,
     635,   631,   638,   632,   203,   634,   636,   639,   640,   641,
     614,   183,   642,   644,   647,   204,   205,    67,   648,    69,
      68,    71,    70,    73,    72,    75,    74,    77,    76,    79,
      78,    81,    80,    83,    82,   650,    84,   649,   266,   661,
     662,   663,   664,   665,   666,   667,   668,   669,   670,   672,
     674,   671,   675,   483,   681,   688,   686,   685,   692,   694,
     695,   693,   702,   766,   700,   699,   704,   705,   727,   753,
     714,   771,   772,   756,   760,   763,   764,   767,   770,   774,
     769,   775,   780,   781,   783,   784,   793,   794,   797,   798,
     799,   800,   801,   802,   804,   813,   806,   814,   831,   816,
     825,   815,   842,   819,   820,   821,   822,   838,   827,   830,
     828,   843,   846,   847,   850,   851,   853,   855,   857,   856,
     860,   862,   863,   864,   867,   866,   872,   870,   874,   881,
     873,   875,    10,   889,   877,   880,   450,   882,   887,   184,
     178,   829,   891,   578,   353,   525,   306,   817,   313,   871,
     848,   824,   773,   232,   861,   687,   453,   574,   565,   751,
     553,   718,   388,   599
};

static const yytype_uint16 yycheck[] =
{
      87,   145,   182,   143,   310,   411,   518,   151,   152,   419,
     420,   155,   156,   157,   158,   159,    64,   163,   164,   165,
     153,   425,   342,   448,   416,   169,   342,     3,   172,    63,
     568,     4,   708,   566,     3,   564,     6,   170,   171,   449,
     173,   174,    63,   187,     3,    63,   179,    63,    63,    46,
      63,    64,    63,   139,   198,    63,    64,     4,   198,    67,
      68,   139,     4,     3,   370,     4,     8,   373,    63,     4,
       3,   114,   140,   608,    59,   138,     0,   138,    63,     4,
     141,     4,   145,     8,     9,     8,     9,    63,     4,   146,
      63,    64,     8,     9,     5,   163,   164,     5,    77,   138,
     157,   158,   146,   146,    63,   640,   145,   138,   252,   253,
     138,   255,   146,    58,   145,   139,    63,   145,    63,   140,
     168,   129,   140,   157,   158,   531,   146,   140,     3,   140,
     146,   146,   140,    58,    59,    60,   316,    63,    63,    64,
      63,   817,   190,   140,    70,   140,     3,   138,   196,   661,
     198,    76,    63,   161,   162,    63,     4,     4,    78,     3,
       8,     8,     9,   140,   140,    90,    91,   140,   144,   681,
     146,   140,   142,   146,    99,   100,   101,   102,   103,     3,
     142,   140,   155,   156,   157,   158,   724,   146,     3,   722,
     115,   720,   117,   169,   170,   139,   165,   166,   140,   148,
     140,   140,   342,   139,   129,   140,    63,   140,   167,   168,
     139,   146,   604,   155,   156,   140,   155,   156,   191,    63,
     155,   156,   157,   158,   140,    61,   138,    63,   361,   409,
     374,   375,   642,   145,   540,   139,   161,   162,   558,    63,
     556,   385,   386,   376,   648,   670,   138,   197,    63,   191,
     383,   139,   191,   145,   387,   401,   191,    96,   391,    98,
     406,   407,   408,   139,   351,   140,   191,   315,   412,   144,
     139,    94,   138,   321,   322,   141,   324,   325,   326,   327,
     328,   139,    46,   140,   424,    63,    64,     4,   138,   146,
     165,   166,   140,   341,   342,   145,   140,   139,   704,    63,
      64,   139,   146,    20,   437,    22,   354,   155,   156,    26,
      27,    28,   169,   170,   139,   363,   140,   142,   120,   139,
     368,   139,   146,   139,   372,   140,   139,   471,   139,   144,
      63,   146,    65,    66,   139,   641,   139,   481,   482,   139,
       3,   143,   139,   191,     7,   393,    63,    64,   194,   195,
     196,   129,   167,   168,   141,    32,    33,    34,   870,    15,
      16,   138,   140,    68,    69,   129,   160,   161,    85,   192,
     193,   194,   195,   196,   139,   198,   140,   138,     3,   785,
     141,    62,    63,   161,   162,   139,   180,   435,   151,   152,
     192,   193,   194,   195,   196,   138,   140,   161,   162,   139,
      63,    64,    67,    68,   198,   192,   193,   194,   195,   196,
     127,   139,   129,   719,    10,    11,    12,    13,     3,     4,
     139,   139,   562,   140,   139,   139,   139,   144,   139,   146,
     139,   192,   193,   194,   195,   196,   138,   197,   155,   156,
     157,   158,   139,   139,   161,   162,   163,   164,   139,   192,
     193,   194,   195,   196,   634,   139,   250,   251,   140,   122,
     123,   124,   125,   126,   139,   128,   129,   130,   131,   132,
     133,   134,   138,   142,   191,   141,   609,   140,    63,    64,
     613,   139,   139,   616,   617,   139,   139,   667,   668,   669,
     192,   193,   194,   195,   196,   543,   636,   139,   161,   162,
     144,   142,   165,   166,   142,   198,   812,   138,   171,   138,
     138,   137,   560,   140,     4,     4,     3,   823,   138,   143,
     142,    62,    63,    64,   138,   142,   666,     3,   194,   195,
     196,   142,   672,   138,   144,   138,   138,   122,   123,   124,
     125,   126,   140,   128,   129,   130,   131,   132,   133,   134,
     138,   138,   138,   141,   200,   140,   140,     4,   140,   865,
     143,   140,   140,   611,   612,   142,   138,   615,   192,   193,
     194,   195,   196,    63,    64,   138,   161,   162,   138,   138,
       4,   122,   123,   124,   125,   126,   171,   128,   129,   130,
     131,   132,   133,   134,   140,   775,   143,   737,     6,   140,
     138,   138,   138,   143,   192,   193,   194,   195,   196,   192,
     193,   194,   195,   196,   143,     4,   138,   797,   798,   799,
     161,   162,   138,   138,   143,   138,     4,   675,   138,   138,
     171,   138,   122,   123,   124,   125,   126,   685,   128,   129,
     130,   131,   132,   133,   134,   192,   193,   194,   195,   196,
     140,   143,   192,   193,   194,   195,   196,   192,   193,   194,
     195,   196,   141,   192,   193,   194,   195,   196,   138,   138,
     138,   161,   162,   192,   193,   194,   195,   196,   138,    63,
      64,   171,   138,     6,   192,   193,   194,   195,   196,   138,
     141,    14,     3,   139,    17,    18,    19,    20,    21,    22,
      23,    24,    25,   139,   139,   139,    29,    30,    31,     3,
     138,   759,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,   143,    47,    48,    49,    50,    51,    52,
      53,    54,    55,   143,    57,   138,   880,     5,   122,   123,
     124,   125,   126,   887,   128,   129,   130,   131,   132,   133,
     134,   143,    75,   147,    62,   138,   140,   805,   138,    82,
      83,   138,    85,    62,    87,    88,    89,   854,   138,    92,
      93,   138,    95,   135,    97,   141,    74,   161,   162,   138,
      71,   868,   192,   193,   194,   195,   196,   171,     4,   138,
      71,   138,   138,   116,   138,   118,    84,   140,   143,   138,
     192,   193,   194,   195,   196,   145,     3,   138,   141,     4,
     146,   146,   138,    65,    66,   142,   138,   199,   138,   142,
     141,   138,   138,   138,   138,   148,   149,    79,    80,    81,
       4,   138,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     102,     4,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   149,   138,   138,   142,   138,   138,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,     4,   138,   138,     4,
      62,   138,     4,   138,   146,   138,   138,   138,   138,    84,
       3,   149,   138,   198,   139,   157,   158,   155,   138,   157,
     156,   159,   158,   161,   160,   163,   162,   165,   164,   167,
     166,   169,   168,   171,   170,     4,   172,   141,   140,   138,
       3,   141,   147,   141,   139,   139,   139,   139,   139,   138,
       5,   141,   138,    62,   138,     4,   138,   140,     4,    71,
     138,   138,   138,     4,    71,   146,   138,   138,   138,   138,
     144,     4,     4,   141,   141,   141,   141,   138,   138,   138,
     143,   138,     4,     4,   138,   138,   141,     5,   139,   139,
     139,   139,     3,    32,   141,     4,   138,   138,    63,   138,
       7,   141,     4,   141,   141,   141,   138,   141,   139,   138,
     145,   138,   138,     4,   138,     4,   139,     5,    32,   199,
     141,     4,   141,   141,   141,    73,   141,   138,   141,    72,
     136,   141,     5,   886,   138,   138,   350,   138,   138,    59,
      53,   788,   141,   466,   202,   411,   176,   774,   181,   856,
     816,   783,   697,   147,   843,   621,   355,   459,   447,   672,
     435,   648,   259,   471
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   202,   203,   204,   205,   139,   139,     0,
     203,    77,   211,   146,   146,   139,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    29,    30,    31,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      57,    75,    82,    83,    85,    87,    88,    89,    92,    93,
      95,    97,   116,   118,   142,   148,   149,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   206,   213,   215,   216,   217,
     218,   220,   221,   229,   232,   233,   235,   236,   243,   245,
     246,   248,   250,   252,   256,   257,   258,   261,   263,   270,
     275,   278,   284,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   300,   301,   302,   303,   304,   306,   307,
     308,   309,   310,   313,   314,   315,   316,   317,   318,   319,
     138,    78,   212,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   235,   139,
     139,   139,   142,   149,   217,   219,   237,   139,   148,     4,
     140,   155,   156,   191,   345,   355,   140,   213,   197,   139,
     247,     3,   138,   146,   157,   158,   331,   344,    10,    11,
      12,    13,    63,    64,    67,    68,   129,   140,   161,   162,
     276,   333,   334,     3,    63,   140,   146,   169,   170,   286,
     342,   343,   286,     3,    63,   140,   146,   167,   168,   280,
     340,   341,    63,   146,   320,    58,    63,   325,   333,   333,
     122,   123,   124,   125,   126,   128,   130,   131,   132,   133,
     134,   140,   171,   333,   353,   354,   140,   163,   164,   336,
     337,   333,   333,   333,   333,    46,   140,   333,   362,   363,
     336,   336,    63,   146,   260,   321,   140,   362,   362,   362,
     332,   345,   140,   259,   345,   333,   354,   354,   333,   354,
     354,     4,     8,     9,    63,   356,   260,     5,    63,   271,
     253,   354,   336,   216,   332,   197,   142,   333,   140,     8,
     345,   142,   120,   143,   192,   193,   194,   195,   196,   344,
     345,   350,   351,   352,    20,    22,    26,    27,    28,    85,
     127,   140,   144,   333,   336,   344,   345,   358,   359,   360,
     251,   144,   214,   212,   142,   198,   138,   138,   138,   137,
     335,   140,     4,   142,   138,   138,   265,     3,   142,   287,
     285,     3,   142,   279,   138,   138,   138,   138,   138,   336,
     336,   333,   333,   140,   333,   140,   140,   140,   353,   140,
     354,   200,     4,   142,   138,   138,   138,   138,   140,     4,
     138,   138,   138,     6,   234,     4,   138,   138,   138,   138,
       4,   138,   138,   138,   138,   138,   138,   214,   254,   138,
     138,   143,   345,   332,   138,   230,   141,   141,   345,   345,
     345,   345,   345,   345,   345,   138,   141,   139,     4,   225,
     226,   227,   228,   334,   337,   344,   345,   139,   139,   139,
     215,   213,   345,   331,     3,     3,   140,   305,     3,   138,
     143,   354,   138,   345,    68,    69,   277,     5,   147,   138,
     345,   138,   214,   138,   345,   214,   333,   333,   354,    62,
      62,   138,   138,    62,   273,   354,   333,   333,     3,     7,
     140,   165,   166,   338,   339,   354,   135,   141,   354,   138,
     345,    59,    63,   326,    63,    65,    66,   327,   330,    74,
      71,     4,   138,    71,   362,    63,   140,   266,   138,   138,
     362,   362,   362,   332,   138,   259,   356,   333,    61,    63,
     361,   140,    63,   140,   267,   325,   255,   140,   338,   338,
     249,    94,   198,   244,   143,    63,   344,   218,   347,   348,
     349,   143,   143,   350,   354,   138,   138,   145,   138,   145,
     138,   145,   138,   145,   144,   342,   144,   340,   144,   338,
     145,   143,     3,   138,   335,   141,     4,   143,   265,   146,
     143,     4,     8,     9,    58,    59,    60,    76,    90,    91,
      99,   100,   101,   102,   103,   115,   117,   288,   333,   355,
     357,   146,   143,   281,   138,   138,   333,   333,   199,   138,
     141,   138,   138,   138,     3,   142,   138,   138,   138,     4,
     143,   138,   142,   138,   138,     4,   138,     4,   138,   326,
       4,   138,   138,   299,   138,    62,   138,   356,     4,   138,
     138,    84,   138,   214,   198,   345,   238,   139,   138,   141,
       4,   337,   334,   345,   344,   222,   343,   223,   341,   224,
     339,   138,     3,   141,   147,   141,   139,   139,   139,   139,
     139,   141,   138,   325,     5,   138,   273,   354,   345,   345,
     354,   138,   345,   354,   354,   140,   138,   321,     4,    63,
      70,   329,     4,   138,    71,   138,    63,   140,   268,   146,
      71,   332,   138,   344,   138,   138,    63,   140,   269,    63,
     272,   273,   214,   338,   144,   151,   152,   346,   347,   231,
     138,   145,   138,   145,   138,   145,   326,   138,   344,   332,
     332,   332,   340,    65,    66,    79,    80,    81,   102,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   282,
     328,   330,   344,   138,   209,   345,   141,   138,   141,   138,
     141,   326,   143,   141,   141,   345,     4,   138,   322,   143,
     138,     4,     4,   276,   138,   138,     4,    63,   274,   356,
       4,     4,   322,   138,   138,    96,    98,   239,   240,   214,
     343,   341,   339,   141,     5,   139,   344,   139,   139,   139,
     139,     3,    32,   345,   141,   193,   138,    32,    33,    34,
     323,   324,   311,     4,   138,   141,   138,   269,   332,   141,
     141,   141,   138,   262,   274,     7,   356,   139,   145,   239,
     138,    63,   332,   332,   332,   114,   146,   283,   141,   138,
     141,   345,     4,   138,   214,   312,   138,     4,   277,   322,
     138,     4,   214,   139,   242,     5,   199,    32,   138,   207,
     141,   323,     4,   141,   141,   264,    73,   141,   241,   213,
     138,   283,   141,   136,   141,   141,   214,   138,   213,   326,
     138,    72,   138,   333,     5,    63,   210,   138,   208,   209,
     333,   141
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

  case 99:

/* Line 1455 of yacc.c  */
#line 553 "lev_comp.y"
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

  case 100:

/* Line 1455 of yacc.c  */
#line 569 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 575 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 581 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 587 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 593 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 599 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 605 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 611 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 617 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 627 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 637 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 647 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 657 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 667 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 677 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 689 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 694 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 701 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 706 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 713 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 718 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 725 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 729 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 735 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 740 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 747 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 751 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 757 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 761 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 767 "lev_comp.y"
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

  case 130:

/* Line 1455 of yacc.c  */
#line 789 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 793 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 804 "lev_comp.y"
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

  case 133:

/* Line 1455 of yacc.c  */
#line 859 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 865 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 869 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 875 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 882 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 886 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 894 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 898 "lev_comp.y"
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

  case 141:

/* Line 1455 of yacc.c  */
#line 921 "lev_comp.y"
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

  case 144:

/* Line 1455 of yacc.c  */
#line 966 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 976 "lev_comp.y"
    {
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 979 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 989 "lev_comp.y"
    {
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 994 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 1008 "lev_comp.y"
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

  case 152:

/* Line 1455 of yacc.c  */
#line 1045 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1050 "lev_comp.y"
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

  case 154:

/* Line 1455 of yacc.c  */
#line 1079 "lev_comp.y"
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

  case 155:

/* Line 1455 of yacc.c  */
#line 1093 "lev_comp.y"
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

  case 156:

/* Line 1455 of yacc.c  */
#line 1109 "lev_comp.y"
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

  case 157:

/* Line 1455 of yacc.c  */
#line 1129 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1141 "lev_comp.y"
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

  case 159:

/* Line 1455 of yacc.c  */
#line 1161 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1167 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1177 "lev_comp.y"
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

  case 162:

/* Line 1455 of yacc.c  */
#line 1195 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1205 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1211 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1216 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1221 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1228 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1235 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1244 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1252 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1263 "lev_comp.y"
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

  case 172:

/* Line 1455 of yacc.c  */
#line 1274 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1281 "lev_comp.y"
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

  case 174:

/* Line 1455 of yacc.c  */
#line 1292 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1299 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1303 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1309 "lev_comp.y"
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

  case 178:

/* Line 1455 of yacc.c  */
#line 1319 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1325 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1334 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1340 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1345 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1351 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1356 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1362 "lev_comp.y"
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

  case 186:

/* Line 1455 of yacc.c  */
#line 1373 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (5)].i), SPO_DOOR));
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1387 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1391 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1401 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1408 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1416 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1432 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1436 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1442 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1450 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1456 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x0000;
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1464 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1472 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1477 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1483 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1489 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1495 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1501 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1506 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1511 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1516 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1521 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1526 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1531 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1536 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1541 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1546 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1551 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1559 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1568 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1572 "lev_comp.y"
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

  case 227:

/* Line 1455 of yacc.c  */
#line 1585 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1593 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1603 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1611 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1620 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1627 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1635 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1641 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1646 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1651 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1656 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1661 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1666 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1671 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1676 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1681 "lev_comp.y"
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

  case 243:

/* Line 1455 of yacc.c  */
#line 1692 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1698 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1703 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1708 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1713 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1720 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1726 "lev_comp.y"
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

  case 250:

/* Line 1455 of yacc.c  */
#line 1755 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1760 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1768 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1774 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1780 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1787 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1794 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1804 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1814 "lev_comp.y"
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

  case 259:

/* Line 1455 of yacc.c  */
#line 1829 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1839 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1843 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1849 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_FOUNTAIN));
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1855 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1861 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int)(yyvsp[(6) - (13)].i), (int)(yyvsp[(8) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1867 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1873 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1879 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1884 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1891 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1898 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1904 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1910 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1916 "lev_comp.y"
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

  case 274:

/* Line 1455 of yacc.c  */
#line 1930 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      if ( (yyvsp[(9) - (10)].i) ) {
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      } else if ( (yyvsp[(10) - (10)].i) )
			  lc_error("Cannot use lev statements in non-permanent REGION");
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1940 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1944 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1950 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1957 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1963 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1969 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1973 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1978 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1985 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1991 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1998 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 2002 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 2009 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 2020 "lev_comp.y"
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

  case 291:

/* Line 1455 of yacc.c  */
#line 2033 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 2037 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 2043 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2047 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2054 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2058 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2062 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2078 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2086 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2096 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2105 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2110 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2117 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2128 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2134 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2138 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2142 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2149 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2159 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2166 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2170 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2176 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2180 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2188 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2192 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2199 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2209 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2222 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2226 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2233 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2243 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2252 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2263 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2267 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2274 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2284 "lev_comp.y"
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

  case 337:

/* Line 1455 of yacc.c  */
#line 2295 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2304 "lev_comp.y"
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

  case 339:

/* Line 1455 of yacc.c  */
#line 2314 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2320 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2324 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2331 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2341 "lev_comp.y"
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

  case 344:

/* Line 1455 of yacc.c  */
#line 2352 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2361 "lev_comp.y"
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

  case 346:

/* Line 1455 of yacc.c  */
#line 2371 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2377 "lev_comp.y"
    { ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2379 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2385 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2389 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2393 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2397 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2405 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2414 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2418 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2422 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2426 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2430 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2433 "lev_comp.y"
    { ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2437 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2444 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2453 "lev_comp.y"
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

  case 367:

/* Line 1455 of yacc.c  */
#line 2494 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2498 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2505 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2512 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2522 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2526 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2534 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2538 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2542 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2546 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2550 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2555 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2559 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2563 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2568 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2573 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2578 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2582 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2587 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2592 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2596 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2600 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2605 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2609 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2616 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2622 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2626 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2632 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2644 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2648 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2652 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2656 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2678 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2682 "lev_comp.y"
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

  case 412:

/* Line 1455 of yacc.c  */
#line 2708 "lev_comp.y"
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
#line 6107 "lev_comp.tab.c"
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
#line 2736 "lev_comp.y"


/*lev_comp.y*/

