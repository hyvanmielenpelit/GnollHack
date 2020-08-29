
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
     FOUNTAIN_TYPE = 330,
     SPECIAL_OBJECT_TYPE = 331,
     SUBROOM_ID = 332,
     NAME_ID = 333,
     FLAGS_ID = 334,
     FLAG_TYPE = 335,
     MON_ATTITUDE = 336,
     MON_ALERTNESS = 337,
     MON_APPEARANCE = 338,
     ROOMDOOR_ID = 339,
     IF_ID = 340,
     ELSE_ID = 341,
     TERRAIN_ID = 342,
     HORIZ_OR_VERT = 343,
     REPLACE_TERRAIN_ID = 344,
     EXIT_ID = 345,
     SHUFFLE_ID = 346,
     QUANTITY_ID = 347,
     BURIED_ID = 348,
     LOOP_ID = 349,
     FOR_ID = 350,
     TO_ID = 351,
     SWITCH_ID = 352,
     CASE_ID = 353,
     BREAK_ID = 354,
     DEFAULT_ID = 355,
     ERODED_ID = 356,
     TRAPPED_STATE = 357,
     RECHARGED_ID = 358,
     INVIS_ID = 359,
     GREASED_ID = 360,
     FEMALE_ID = 361,
     CANCELLED_ID = 362,
     REVIVED_ID = 363,
     AVENGE_ID = 364,
     FLEEING_ID = 365,
     BLINDED_ID = 366,
     PARALYZED_ID = 367,
     STUNNED_ID = 368,
     CONFUSED_ID = 369,
     SEENTRAPS_ID = 370,
     ALL_ID = 371,
     MONTYPE_ID = 372,
     GRAVE_ID = 373,
     ERODEPROOF_ID = 374,
     FUNCTION_ID = 375,
     MSG_OUTPUT_TYPE = 376,
     COMPARE_TYPE = 377,
     UNKNOWN_TYPE = 378,
     rect_ID = 379,
     fillrect_ID = 380,
     line_ID = 381,
     randline_ID = 382,
     grow_ID = 383,
     selection_ID = 384,
     flood_ID = 385,
     rndcoord_ID = 386,
     circle_ID = 387,
     ellipse_ID = 388,
     filter_ID = 389,
     complement_ID = 390,
     gradient_ID = 391,
     GRADIENT_TYPE = 392,
     LIMITED = 393,
     HUMIDITY_TYPE = 394,
     STRING = 395,
     MAP_ID = 396,
     NQSTRING = 397,
     VARSTRING = 398,
     CFUNC = 399,
     CFUNC_INT = 400,
     CFUNC_STR = 401,
     CFUNC_COORD = 402,
     CFUNC_REGION = 403,
     VARSTRING_INT = 404,
     VARSTRING_INT_ARRAY = 405,
     VARSTRING_STRING = 406,
     VARSTRING_STRING_ARRAY = 407,
     VARSTRING_VAR = 408,
     VARSTRING_VAR_ARRAY = 409,
     VARSTRING_COORD = 410,
     VARSTRING_COORD_ARRAY = 411,
     VARSTRING_REGION = 412,
     VARSTRING_REGION_ARRAY = 413,
     VARSTRING_MAPCHAR = 414,
     VARSTRING_MAPCHAR_ARRAY = 415,
     VARSTRING_MONST = 416,
     VARSTRING_MONST_ARRAY = 417,
     VARSTRING_OBJ = 418,
     VARSTRING_OBJ_ARRAY = 419,
     VARSTRING_SEL = 420,
     VARSTRING_SEL_ARRAY = 421,
     METHOD_INT = 422,
     METHOD_INT_ARRAY = 423,
     METHOD_STRING = 424,
     METHOD_STRING_ARRAY = 425,
     METHOD_VAR = 426,
     METHOD_VAR_ARRAY = 427,
     METHOD_COORD = 428,
     METHOD_COORD_ARRAY = 429,
     METHOD_REGION = 430,
     METHOD_REGION_ARRAY = 431,
     METHOD_MAPCHAR = 432,
     METHOD_MAPCHAR_ARRAY = 433,
     METHOD_MONST = 434,
     METHOD_MONST_ARRAY = 435,
     METHOD_OBJ = 436,
     METHOD_OBJ_ARRAY = 437,
     METHOD_SEL = 438,
     METHOD_SEL_ARRAY = 439,
     DICE = 440
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
#line 483 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 495 "lev_comp.tab.c"

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
#define YYLAST   1062

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  203
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  163
/* YYNRULES -- Number of rules.  */
#define YYNRULES  413
/* YYNRULES -- Number of states.  */
#define YYNSTATES  895

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   440

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   198,   202,     2,
     142,   143,   196,   194,   140,   195,   200,   197,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   141,     2,
       2,   199,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   144,     2,   145,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   146,   201,   147,     2,     2,     2,     2,
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
     135,   136,   137,   138,   139,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193
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
     842,   843,   846,   852,   856,   870,   874,   878,   880,   886,
     896,   902,   906,   910,   911,   922,   923,   925,   933,   937,
     943,   949,   955,   959,   965,   973,   983,   985,   987,   989,
     991,   993,   994,   997,   999,  1003,  1005,  1007,  1009,  1011,
    1013,  1015,  1017,  1019,  1021,  1023,  1025,  1027,  1031,  1033,
    1035,  1040,  1042,  1044,  1049,  1051,  1053,  1058,  1060,  1065,
    1071,  1073,  1077,  1079,  1083,  1085,  1087,  1092,  1102,  1104,
    1106,  1111,  1113,  1119,  1121,  1123,  1128,  1130,  1132,  1138,
    1140,  1142,  1144,  1149,  1151,  1153,  1159,  1161,  1163,  1165,
    1169,  1171,  1173,  1177,  1179,  1184,  1188,  1192,  1196,  1200,
    1204,  1208,  1210,  1212,  1216,  1218,  1222,  1223,  1225,  1227,
    1229,  1231,  1235,  1236,  1238,  1240,  1243,  1246,  1251,  1258,
    1263,  1270,  1277,  1284,  1291,  1294,  1301,  1310,  1319,  1330,
    1345,  1348,  1350,  1354,  1356,  1360,  1362,  1364,  1366,  1368,
    1370,  1372,  1374,  1376,  1378,  1380,  1382,  1384,  1386,  1388,
    1390,  1392,  1394,  1405
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     204,     0,    -1,    -1,   205,    -1,   206,    -1,   206,   205,
      -1,   207,   213,   215,    -1,    16,   141,   148,    -1,    15,
     141,   148,   140,     3,    -1,    17,   141,    11,   140,   307,
      -1,    17,   141,    10,   140,     3,    -1,    17,   141,    13,
      -1,    17,   141,    12,   140,     3,   140,     3,   140,     5,
     140,     5,   140,   328,   140,   212,   211,    -1,    -1,   140,
     138,    -1,    -1,   140,   335,    -1,    -1,   140,     3,    -1,
       5,    -1,    63,    -1,    -1,    79,   141,   214,    -1,    80,
     140,   214,    -1,    80,    -1,    -1,   217,   215,    -1,   146,
     215,   147,    -1,   258,    -1,   208,    -1,   315,    -1,   316,
      -1,   317,    -1,   318,    -1,   300,    -1,   260,    -1,   223,
      -1,   222,    -1,   310,    -1,   272,    -1,   292,    -1,   320,
      -1,   321,    -1,   302,    -1,   303,    -1,   304,    -1,   319,
      -1,   238,    -1,   248,    -1,   250,    -1,   254,    -1,   252,
      -1,   235,    -1,   245,    -1,   231,    -1,   234,    -1,   295,
      -1,   277,    -1,   293,    -1,   280,    -1,   286,    -1,   311,
      -1,   306,    -1,   298,    -1,   259,    -1,   312,    -1,   265,
      -1,   263,    -1,   305,    -1,   309,    -1,   308,    -1,   296,
      -1,   297,    -1,   299,    -1,   291,    -1,   294,    -1,   158,
      -1,   160,    -1,   162,    -1,   164,    -1,   166,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,   157,    -1,   159,
      -1,   161,    -1,   163,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,   218,    -1,   219,    -1,   151,
      -1,   151,    -1,   219,    -1,    91,   141,   218,    -1,   220,
     199,   347,    -1,   220,   199,   129,   141,   356,    -1,   220,
     199,   346,    -1,   220,   199,   362,   141,   340,    -1,   220,
     199,   361,   141,   342,    -1,   220,   199,   360,   141,   344,
      -1,   220,   199,   335,    -1,   220,   199,   338,    -1,   220,
     199,   146,   229,   147,    -1,   220,   199,   146,   228,   147,
      -1,   220,   199,   146,   227,   147,    -1,   220,   199,   362,
     141,   146,   226,   147,    -1,   220,   199,   361,   141,   146,
     225,   147,    -1,   220,   199,   360,   141,   146,   224,   147,
      -1,   220,   199,   146,   230,   147,    -1,   345,    -1,   224,
     140,   345,    -1,   343,    -1,   225,   140,   343,    -1,   341,
      -1,   226,   140,   341,    -1,   339,    -1,   227,   140,   339,
      -1,   336,    -1,   228,   140,   336,    -1,   347,    -1,   229,
     140,   347,    -1,   346,    -1,   230,   140,   346,    -1,    -1,
      -1,   120,   150,   142,   232,   351,   143,   233,   216,    -1,
     150,   142,   354,   143,    -1,    90,    -1,    -1,     6,    -1,
       6,    -1,   144,   347,   122,   347,   145,    -1,   144,   347,
     145,    -1,    -1,    -1,    97,   239,   144,   334,   145,   240,
     146,   241,   147,    -1,    -1,   242,   241,    -1,    -1,    98,
     358,   141,   243,   215,    -1,    -1,   100,   141,   244,   215,
      -1,    99,    -1,   200,   200,    -1,    96,    -1,    95,   221,
     199,   347,   246,   347,    -1,    -1,   247,   249,   216,    -1,
      -1,    94,   144,   334,   145,   251,   216,    -1,    -1,   237,
     141,   253,   217,    -1,    -1,    85,   237,   255,   256,    -1,
     216,    -1,    -1,   216,   257,    86,   216,    -1,    14,   141,
     346,    -1,    57,    -1,    57,   141,   358,    -1,    57,   141,
      63,    -1,    48,   141,   261,   140,   261,    -1,    48,   141,
     261,   140,   358,    -1,   142,     4,   140,    62,   140,   276,
     143,    -1,   323,   236,   140,   328,    -1,    -1,    77,   141,
     262,   140,   269,   140,   271,   324,   264,   216,    -1,    -1,
      42,   141,   262,   140,   268,   140,   270,   140,   271,   324,
     266,   216,    -1,    -1,   140,     5,    -1,   142,     4,   140,
       4,   143,    -1,    63,    -1,   142,     4,   140,     4,   143,
      -1,    63,    -1,   142,   278,   140,   279,   143,    -1,    63,
      -1,   142,     4,   140,     4,   143,    -1,    63,    -1,    84,
     141,   273,   140,   327,   140,   274,   140,   276,    -1,    24,
     141,   327,   140,   356,    -1,     5,    -1,    63,    -1,   275,
      -1,    63,    -1,    62,    -1,    62,   201,   275,    -1,     4,
      -1,    63,    -1,    19,    -1,    18,   141,   278,   140,   279,
     267,   149,    -1,    18,   141,   335,   267,   149,    -1,    67,
      -1,    68,    -1,    69,    -1,    68,    -1,    22,   141,   282,
      -1,    -1,    22,   141,   282,   281,   216,    -1,   342,   140,
     335,   283,    -1,    -1,   283,   140,   284,    -1,   346,    -1,
      81,    -1,    82,    -1,   330,    -1,    83,   346,    -1,   106,
      -1,   104,    -1,   107,    -1,   108,    -1,   109,    -1,   110,
     141,   334,    -1,   111,   141,   334,    -1,   112,   141,   334,
      -1,   113,    -1,   114,    -1,   115,   141,   285,    -1,   148,
      -1,   116,    -1,   148,   201,   285,    -1,    20,   141,   288,
      -1,    -1,    21,   141,   288,   287,   216,    -1,   344,   289,
      -1,    -1,   289,   140,   290,    -1,    60,    -1,   117,   141,
     342,    -1,   359,    -1,    78,   141,   346,    -1,    92,   141,
     334,    -1,    93,    -1,    59,    -1,   101,   141,   334,    -1,
     119,    -1,    58,    -1,   102,    -1,   103,   141,   334,    -1,
     104,    -1,   105,    -1,   335,    -1,    23,   141,   322,   140,
     335,    -1,    25,   141,   335,   140,    62,   140,   327,    -1,
      29,   141,   335,   140,    62,    -1,    29,   141,   335,   140,
      62,   140,     5,   211,    -1,    30,    -1,    30,   141,   356,
      -1,    38,   141,   335,   140,    71,    -1,    39,   141,   335,
     140,    71,    -1,    39,   141,   364,   140,   364,   140,    71,
      -1,    43,   141,   364,   140,   364,   140,   148,    -1,    44,
     141,   364,   140,   364,   301,    -1,    45,   141,   364,   140,
     364,    -1,    -1,   140,    71,    -1,    51,   141,   356,   140,
      75,    -1,    52,   141,   356,    -1,    53,   141,   335,   140,
     142,   358,   140,   358,   143,   140,    73,   140,    72,    -1,
      55,   141,   356,    -1,    54,   141,   356,    -1,     3,    -1,
     142,     3,   140,   328,   143,    -1,    89,   141,   338,   140,
     340,   140,   340,   140,     7,    -1,    87,   141,   356,   140,
     340,    -1,    40,   141,   338,    -1,    41,   141,   338,    -1,
      -1,    31,   141,   338,   140,   328,   140,   323,   324,   313,
     314,    -1,    -1,   216,    -1,    35,   141,   335,   140,   329,
     140,   331,    -1,    36,   141,   335,    -1,    37,   141,    74,
     140,   335,    -1,   118,   141,   335,   140,   346,    -1,   118,
     141,   335,   140,    63,    -1,   118,   141,   335,    -1,    49,
     141,   347,   140,   335,    -1,    50,   141,   335,   140,   363,
     140,   346,    -1,    47,   141,   334,   140,   334,   140,   334,
     140,   334,    -1,    47,    -1,   148,    -1,    63,    -1,   148,
      -1,    63,    -1,    -1,   140,   325,    -1,   326,    -1,   326,
     140,   325,    -1,    32,    -1,    33,    -1,    34,    -1,    58,
      -1,    63,    -1,    59,    -1,    63,    -1,    66,    -1,   332,
      -1,    63,    -1,    66,    -1,   332,    -1,    65,   141,    63,
      -1,    70,    -1,    63,    -1,    65,   144,     4,   145,    -1,
     148,    -1,   159,    -1,   160,   144,   347,   145,    -1,   347,
      -1,   336,    -1,   131,   142,   356,   143,    -1,   163,    -1,
     164,   144,   347,   145,    -1,   142,     4,   140,     4,   143,
      -1,    63,    -1,    64,   337,   145,    -1,   139,    -1,   139,
     140,   337,    -1,   339,    -1,   165,    -1,   166,   144,   347,
     145,    -1,   142,     4,   140,     4,   140,     4,   140,     4,
     143,    -1,   341,    -1,   167,    -1,   168,   144,   347,   145,
      -1,     3,    -1,   142,     3,   140,   328,   143,    -1,   343,
      -1,   169,    -1,   170,   144,   347,   145,    -1,   148,    -1,
       3,    -1,   142,     3,   140,   148,   143,    -1,    63,    -1,
     345,    -1,   171,    -1,   172,   144,   347,   145,    -1,   148,
      -1,     3,    -1,   142,     3,   140,   148,   143,    -1,    63,
      -1,    76,    -1,   333,    -1,   346,   200,   333,    -1,     4,
      -1,   357,    -1,   142,     8,   143,    -1,   157,    -1,   158,
     144,   347,   145,    -1,   347,   194,   347,    -1,   347,   195,
     347,    -1,   347,   196,   347,    -1,   347,   197,   347,    -1,
     347,   198,   347,    -1,   142,   347,   143,    -1,   153,    -1,
     154,    -1,   220,   141,   348,    -1,   349,    -1,   350,   140,
     349,    -1,    -1,   350,    -1,   347,    -1,   346,    -1,   352,
      -1,   353,   140,   352,    -1,    -1,   353,    -1,   335,    -1,
     124,   338,    -1,   125,   338,    -1,   126,   335,   140,   335,
      -1,   127,   335,   140,   335,   140,   347,    -1,   128,   142,
     356,   143,    -1,   128,   142,   275,   140,   356,   143,    -1,
     134,   142,     7,   140,   356,   143,    -1,   134,   142,   356,
     140,   356,   143,    -1,   134,   142,   340,   140,   356,   143,
      -1,   130,   335,    -1,   132,   142,   335,   140,   347,   143,
      -1,   132,   142,   335,   140,   347,   140,    32,   143,    -1,
     133,   142,   335,   140,   347,   140,   347,   143,    -1,   133,
     142,   335,   140,   347,   140,   347,   140,    32,   143,    -1,
     136,   142,   137,   140,   142,   347,   195,   347,   209,   143,
     140,   335,   210,   143,    -1,   135,   355,    -1,   173,    -1,
     142,   356,   143,    -1,   355,    -1,   355,   202,   356,    -1,
     193,    -1,     8,    -1,     9,    -1,     4,    -1,     8,    -1,
       9,    -1,     4,    -1,   357,    -1,    26,    -1,    20,    -1,
      27,    -1,    22,    -1,    28,    -1,    87,    -1,    61,    -1,
      63,    -1,   365,    -1,    46,   142,     4,   140,     4,   140,
       4,   140,     4,   143,    -1,   142,     4,   140,     4,   140,
       4,   140,     4,   143,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   281,   281,   282,   285,   286,   289,   312,   317,   339,
     352,   364,   370,   400,   403,   410,   414,   421,   424,   431,
     432,   436,   439,   446,   450,   457,   460,   466,   472,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,   517,   518,   519,   522,   523,   524,   525,
     526,   527,   528,   529,   530,   533,   534,   535,   536,   537,
     538,   539,   540,   541,   544,   545,   546,   549,   550,   553,
     569,   575,   581,   587,   593,   599,   605,   611,   617,   627,
     637,   647,   657,   667,   677,   689,   694,   701,   706,   713,
     718,   725,   729,   735,   740,   747,   751,   757,   761,   768,
     790,   767,   804,   859,   866,   869,   875,   882,   886,   895,
     899,   894,   962,   963,   967,   966,   980,   979,   994,  1004,
    1005,  1008,  1046,  1045,  1080,  1079,  1110,  1109,  1142,  1141,
    1167,  1178,  1177,  1205,  1211,  1216,  1221,  1228,  1235,  1244,
    1252,  1264,  1263,  1282,  1281,  1300,  1303,  1309,  1319,  1325,
    1334,  1340,  1345,  1351,  1356,  1362,  1373,  1379,  1380,  1383,
    1384,  1387,  1391,  1397,  1398,  1401,  1408,  1416,  1424,  1425,
    1428,  1429,  1432,  1437,  1436,  1450,  1457,  1464,  1472,  1477,
    1483,  1489,  1495,  1501,  1506,  1511,  1516,  1521,  1526,  1531,
    1536,  1541,  1546,  1551,  1559,  1568,  1572,  1585,  1594,  1593,
    1611,  1621,  1627,  1635,  1641,  1646,  1651,  1656,  1661,  1666,
    1671,  1676,  1681,  1692,  1698,  1703,  1708,  1713,  1720,  1726,
    1755,  1760,  1768,  1774,  1780,  1787,  1794,  1804,  1814,  1829,
    1840,  1843,  1849,  1855,  1861,  1867,  1873,  1879,  1884,  1891,
    1898,  1904,  1910,  1917,  1916,  1941,  1944,  1950,  1957,  1963,
    1969,  1973,  1978,  1985,  1991,  1998,  2002,  2009,  2017,  2020,
    2030,  2034,  2037,  2043,  2047,  2054,  2058,  2062,  2068,  2069,
    2072,  2073,  2076,  2077,  2078,  2084,  2085,  2086,  2092,  2093,
    2096,  2105,  2110,  2117,  2128,  2134,  2138,  2142,  2149,  2159,
    2166,  2170,  2176,  2180,  2188,  2192,  2199,  2209,  2222,  2226,
    2233,  2243,  2252,  2263,  2267,  2274,  2284,  2295,  2304,  2314,
    2320,  2324,  2331,  2341,  2352,  2361,  2371,  2375,  2382,  2383,
    2389,  2393,  2397,  2401,  2409,  2418,  2422,  2426,  2430,  2434,
    2438,  2441,  2448,  2457,  2490,  2491,  2494,  2495,  2498,  2502,
    2509,  2516,  2527,  2530,  2538,  2542,  2546,  2550,  2554,  2559,
    2563,  2567,  2572,  2577,  2582,  2586,  2591,  2596,  2600,  2604,
    2609,  2613,  2620,  2626,  2630,  2636,  2643,  2644,  2645,  2648,
    2652,  2656,  2660,  2666,  2667,  2670,  2671,  2674,  2675,  2678,
    2679,  2682,  2686,  2712
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
  "NPC_TYPE", "FOUNTAIN_TYPE", "SPECIAL_OBJECT_TYPE", "SUBROOM_ID",
  "NAME_ID", "FLAGS_ID", "FLAG_TYPE", "MON_ATTITUDE", "MON_ALERTNESS",
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
  "altar_detail", "anvil_detail", "npc_detail", "grave_detail",
  "gold_detail", "engraving_detail", "mineralize", "trap_name",
  "room_type", "optroomregionflags", "roomregionflags", "roomregionflag",
  "door_state", "light_state", "alignment", "alignment_prfx", "altar_type",
  "a_register", "string_or_var", "integer_or_var", "coord_or_var",
  "encodecoord", "humidity_flags", "region_or_var", "encoderegion",
  "mapchar_or_var", "mapchar", "monster_or_var", "encodemonster",
  "object_or_var", "encodeobj", "string_expr", "math_expr_var",
  "func_param_type", "func_param_part", "func_param_list",
  "func_params_list", "func_call_param_part", "func_call_param_list",
  "func_call_params_list", "ter_selection_x", "ter_selection", "dice",
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
      44,    58,    40,    41,    91,    93,   123,   125,   395,   396,
     397,   398,   399,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,    43,    45,    42,    47,    37,    61,
      46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   203,   204,   204,   205,   205,   206,   207,   207,   208,
     208,   208,   208,   209,   209,   210,   210,   211,   211,   212,
     212,   213,   213,   214,   214,   215,   215,   216,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   220,   220,   220,   221,   221,   222,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   224,   224,   225,   225,   226,
     226,   227,   227,   228,   228,   229,   229,   230,   230,   232,
     233,   231,   234,   235,   236,   236,   237,   237,   237,   239,
     240,   238,   241,   241,   243,   242,   244,   242,   245,   246,
     246,   247,   249,   248,   251,   250,   253,   252,   255,   254,
     256,   257,   256,   258,   259,   259,   259,   260,   260,   261,
     262,   264,   263,   266,   265,   267,   267,   268,   268,   269,
     269,   270,   270,   271,   271,   272,   272,   273,   273,   274,
     274,   275,   275,   276,   276,   277,   277,   277,   278,   278,
     279,   279,   280,   281,   280,   282,   283,   283,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   285,   285,   285,   286,   287,   286,
     288,   289,   289,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   291,   292,
     293,   293,   294,   294,   295,   296,   297,   298,   299,   300,
     301,   301,   302,   303,   304,   305,   306,   307,   307,   308,
     309,   310,   311,   313,   312,   314,   314,   315,   316,   317,
     318,   318,   318,   319,   320,   321,   321,   322,   322,   323,
     323,   324,   324,   325,   325,   326,   326,   326,   327,   327,
     328,   328,   329,   329,   329,   330,   330,   330,   331,   331,
     332,   333,   333,   333,   334,   335,   335,   335,   335,   336,
     336,   336,   337,   337,   338,   338,   338,   339,   340,   340,
     340,   341,   341,   342,   342,   342,   343,   343,   343,   343,
     344,   344,   344,   345,   345,   345,   345,   345,   346,   346,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   348,   348,   349,   350,   350,   351,   351,   352,   352,
     353,   353,   354,   354,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   356,   356,   357,   358,   358,   358,   359,
     359,   359,   359,   360,   360,   361,   361,   362,   362,   363,
     363,   364,   364,   365
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
       0,     2,     5,     3,    13,     3,     3,     1,     5,     9,
       5,     3,     3,     0,    10,     0,     1,     7,     3,     5,
       5,     5,     3,     5,     7,     9,     1,     1,     1,     1,
       1,     0,     2,     1,     3,     1,     1,     1,     1,     1,
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
       1,     1,    10,     9
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
       0,     0,     0,    97,    98,     0,     0,     0,     0,   350,
       0,   353,     0,   395,     0,   351,   372,    26,     0,   156,
       0,     8,     0,   311,   312,     0,   348,   163,     0,     0,
       0,    11,   320,     0,   198,   199,     0,     0,   317,     0,
       0,   175,   315,   344,   346,   347,     0,   343,   341,     0,
     227,   231,   340,   228,   337,   339,     0,   336,   334,     0,
     202,     0,   333,   288,   287,     0,   298,   299,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   391,   374,   393,   253,     0,   325,     0,
       0,   324,     0,   278,     0,     0,     0,     0,     0,     0,
     411,   271,   272,   290,   289,     0,   134,     0,     0,     0,
       0,     0,   314,     0,     0,     0,     0,     0,   263,     0,
     266,   265,   398,   396,   397,   166,   165,     0,   187,   188,
       0,     0,     0,     0,    99,     0,     0,     0,   282,   129,
       0,     0,     0,     0,   138,     0,     0,     0,     0,     0,
     369,   368,   370,   373,     0,   404,   406,   403,   405,   407,
     408,     0,     0,     0,   106,   107,   102,   100,     0,     0,
       0,     0,    25,   153,    23,     0,     0,     0,     0,     0,
     322,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     230,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,   376,     0,     0,     0,   384,     0,     0,     0,   390,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   135,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   160,
     159,     0,     0,   154,     0,     0,     0,   366,   352,   360,
       0,     0,   355,   356,   357,   358,   359,     0,   132,     0,
     350,     0,     0,     0,     0,   123,   121,   127,   125,     0,
       0,     0,   157,     0,     0,   349,    10,   267,     0,     9,
       0,     0,   321,     0,     0,     0,   201,   200,   175,   176,
     197,     0,     0,     0,   229,     0,     0,   204,   206,   248,
     186,     0,   250,     0,     0,   191,     0,     0,     0,     0,
     331,     0,     0,   329,     0,     0,   328,     0,     0,   392,
     394,     0,     0,   300,   301,     0,   304,     0,   302,     0,
     303,   279,   254,     0,     0,   255,     0,   178,     0,     0,
       0,     0,     0,   260,   259,     0,     0,   167,   168,   283,
     409,   410,     0,   262,     0,   180,     0,     0,     0,     0,
       0,   270,     0,     0,   150,     0,     0,   140,   281,   280,
       0,   364,   367,     0,   354,   137,   371,   101,     0,     0,
     110,     0,   109,     0,   108,     0,   114,     0,   105,     0,
     104,     0,   103,    27,   313,     0,     0,   323,   316,     0,
     318,     0,     0,   342,   401,   399,   400,   242,   239,   233,
       0,     0,   238,     0,   243,     0,   245,   246,     0,   241,
     232,   247,   402,   235,     0,   335,   205,     0,     0,   377,
       0,     0,     0,   379,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   326,     0,     0,     0,     0,     0,     0,
       0,     0,   170,     0,     0,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   155,   149,   151,     0,
       0,     0,   130,     0,   122,   124,   126,   128,     0,   115,
       0,   117,     0,   119,     0,     0,   319,   196,   345,     0,
       0,     0,     0,     0,   338,     0,   249,    17,     0,   192,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     291,     0,   309,   308,   277,     0,     0,   256,     0,   182,
       0,     0,   257,   261,     0,     0,   284,     0,     0,   184,
       0,   291,   190,     0,   189,   162,     0,   142,   361,   362,
     363,   365,     0,     0,   113,     0,   112,     0,   111,     0,
       0,   236,   237,   240,   244,   234,     0,   305,   209,   210,
       0,   214,   213,   215,   216,   217,     0,     0,     0,   221,
     222,     0,   207,   211,   306,   208,     0,   251,   378,   380,
       0,   385,     0,   381,     0,   330,   383,   382,     0,     0,
       0,   273,   310,     0,     0,     0,     0,     0,     0,   193,
     194,     0,     0,     0,     0,   171,     0,     0,     0,     0,
       0,   142,   131,   116,   118,   120,   268,     0,     0,   212,
       0,     0,     0,     0,    18,     0,     0,   332,     0,     0,
     295,   296,   297,   292,   293,   275,     0,     0,   177,     0,
     291,   285,   169,     0,   179,     0,     0,   185,   269,     0,
     146,   141,   143,     0,   307,   218,   219,   220,   225,   224,
     223,   386,     0,   387,   356,     0,     0,   276,   274,     0,
       0,     0,   173,     0,     0,   172,   144,    25,     0,     0,
       0,     0,     0,   327,   294,     0,   413,   181,     0,     0,
     183,    25,   147,     0,   226,   388,    14,     0,   412,   174,
       0,   145,     0,     0,   264,     0,    15,    19,    20,    17,
       0,     0,    12,    16,   389
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    85,   862,   891,   757,   889,
      12,   142,    86,   353,    87,    88,    89,    90,   185,    91,
      92,   658,   660,   662,   441,   442,   443,   444,    93,   427,
     722,    94,    95,   405,    96,    97,   186,   649,   790,   791,
     871,   857,    98,   546,    99,   100,   200,   101,   543,   102,
     351,   103,   311,   420,   539,   104,   105,   106,   294,   285,
     107,   826,   108,   868,   367,   519,   537,   701,   711,   109,
     310,   713,   486,   781,   110,   220,   468,   111,   374,   240,
     606,   752,   840,   112,   371,   230,   370,   600,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   636,   123,
     124,   125,   126,   127,   459,   128,   129,   130,   131,   132,
     815,   848,   133,   134,   135,   136,   137,   138,   139,   245,
     286,   771,   813,   814,   248,   505,   509,   753,   694,   510,
     206,   291,   264,   222,   361,   270,   271,   495,   496,   241,
     242,   231,   232,   330,   292,   720,   551,   552,   553,   332,
     333,   334,   265,   391,   195,   306,   603,   348,   349,   350,
     532,   279,   280
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -682
static const yytype_int16 yypact[] =
{
     311,   -93,   -88,   100,  -682,   311,    45,    -5,    22,  -682,
    -682,   -36,   757,    40,  -682,   138,  -682,    82,    86,    92,
    -682,    94,    98,   117,   119,   129,   131,   135,   150,   152,
     167,   176,   178,   191,   193,   195,   197,   206,   216,   228,
     269,   272,   304,   305,   306,   315,   319,   320,   327,   329,
     341,   343,   344,    21,   348,   359,  -682,   362,   122,   554,
    -682,  -682,   363,    71,    55,   221,  -682,  -682,  -682,  -682,
    -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,
    -682,  -682,  -682,  -682,  -682,  -682,  -682,   757,  -682,  -682,
     225,  -682,  -682,  -682,  -682,  -682,   368,  -682,  -682,  -682,
    -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,
    -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,
    -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,
    -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,
     464,   333,  -682,   -52,   515,   443,   208,   208,   332,   -14,
      16,   285,   285,   545,    -4,   285,   285,   438,   285,   259,
      -4,    -4,     3,   -10,   -10,   -10,    55,   371,    55,   285,
     545,   545,   285,   545,   545,    77,     3,    73,  -682,   545,
      -4,   655,    55,  -682,  -682,   317,   370,   285,   375,  -682,
      80,  -682,   374,  -682,   188,  -682,    48,  -682,    35,  -682,
     390,  -682,   138,  -682,  -682,   393,  -682,   351,   413,   415,
     418,  -682,  -682,   420,  -682,  -682,   412,   553,  -682,   416,
     423,   428,  -682,  -682,  -682,  -682,   566,  -682,  -682,   427,
    -682,  -682,  -682,  -682,  -682,  -682,   569,  -682,  -682,   429,
     430,   435,  -682,  -682,  -682,   439,  -682,  -682,   440,   441,
     446,    -4,    -4,   285,   285,   447,   285,   448,   449,   455,
     545,   457,   501,  -682,  -682,   386,  -682,   606,  -682,   472,
     477,  -682,   478,  -682,   479,   483,   482,   634,   502,   517,
    -682,  -682,  -682,  -682,  -682,   518,   635,   656,   519,   521,
     523,   526,   299,   663,   528,   -80,   546,   555,  -682,   556,
    -682,  -682,  -682,  -682,  -682,  -682,  -682,   557,  -682,  -682,
     564,   390,   567,   570,  -682,   540,    55,    55,   572,  -682,
     551,   326,    55,    55,  -682,    55,    55,    55,    55,    55,
     351,   299,  -682,   576,   563,  -682,  -682,  -682,  -682,  -682,
    -682,   579,   125,    27,  -682,  -682,   351,   299,   581,   583,
     585,   757,   757,  -682,  -682,    55,   -52,   725,    62,   726,
     590,   591,   545,   597,    55,   283,   733,   592,   600,    55,
     602,   390,   603,    55,   390,   285,   285,   545,   682,   683,
    -682,  -682,   607,   608,   520,  -682,   285,   285,   308,  -682,
     612,   614,   545,   610,    55,   108,   265,   285,   680,   754,
     619,   690,   -10,   -21,  -682,   622,   624,   -10,   -10,   -10,
      55,   625,    68,   285,    50,   691,   626,     5,    16,   681,
    -682,    47,    47,  -682,   -40,   627,   -29,   788,  -682,  -682,
     257,   398,   223,   223,  -682,  -682,  -682,    48,  -682,   545,
     629,   -65,   -53,   -43,   -12,  -682,  -682,   351,   299,   222,
     261,   146,  -682,   636,   407,  -682,  -682,  -682,   770,  -682,
     645,   420,  -682,   646,   786,   417,  -682,  -682,   428,  -682,
    -682,   643,   494,   295,  -682,   668,   504,  -682,  -682,  -682,
    -682,   678,   684,   285,   285,   621,   686,   660,   688,   692,
    -682,   693,   414,  -682,   676,   695,  -682,   696,   697,  -682,
    -682,   826,   537,  -682,  -682,   698,  -682,   687,  -682,   699,
    -682,  -682,  -682,   700,   839,  -682,   705,  -682,   849,   715,
     108,   853,   718,   719,  -682,   720,   799,  -682,  -682,  -682,
    -682,  -682,   722,  -682,   293,  -682,   859,   724,   728,   783,
     867,  -682,   731,   390,  -682,   672,    55,  -682,  -682,   351,
     732,  -682,   734,   735,  -682,  -682,  -682,  -682,   872,   737,
    -682,   -19,  -682,    55,  -682,   -52,  -682,    26,  -682,    30,
    -682,    67,  -682,  -682,  -682,   740,   878,  -682,  -682,   739,
    -682,   736,   741,  -682,  -682,  -682,  -682,  -682,  -682,  -682,
     742,   745,  -682,   746,  -682,   748,  -682,  -682,   759,  -682,
    -682,  -682,  -682,  -682,   760,  -682,   762,    16,   899,  -682,
     765,   844,   545,  -682,    55,    55,   545,   769,    55,   545,
     545,   768,   771,  -682,     3,   908,   156,   909,    36,   862,
     794,    10,  -682,   795,   789,   865,  -682,    55,   798,   -52,
     800,   801,    17,   298,   390,    47,  -682,  -682,   299,   796,
     238,   788,  -682,   109,  -682,  -682,   299,   351,   107,  -682,
     110,  -682,   149,  -682,   108,   823,  -682,  -682,  -682,   -52,
      55,    55,    55,   332,  -682,   784,  -682,   824,    55,  -682,
     822,   147,   180,   825,   108,   558,   827,   828,    55,   962,
     829,   830,  -682,  -682,  -682,   832,   963,  -682,   969,  -682,
     361,   834,  -682,  -682,   836,    43,   351,   293,   973,  -682,
     974,   829,  -682,   840,  -682,  -682,   841,    46,  -682,  -682,
    -682,  -682,   390,    26,  -682,    30,  -682,    67,  -682,   842,
     977,   351,  -682,  -682,  -682,  -682,   -49,  -682,  -682,  -682,
     -52,  -682,  -682,  -682,  -682,  -682,   838,   843,   845,  -682,
    -682,   846,  -682,  -682,  -682,   351,   980,  -682,   299,  -682,
     956,  -682,    55,  -682,   847,  -682,  -682,  -682,   336,   851,
     372,  -682,  -682,   985,   852,   850,   854,    17,    55,  -682,
    -682,   855,   856,   857,   861,  -682,    43,   988,   293,   863,
     858,    46,  -682,  -682,  -682,  -682,  -682,   866,   933,   351,
      55,    55,    55,   -47,  -682,   860,   268,  -682,    55,   993,
    -682,  -682,  -682,  -682,   868,   390,   869,   998,  -682,   283,
     829,  -682,  -682,   870,  -682,  1003,   390,  -682,  -682,   871,
    -682,  -682,  -682,  1006,  -682,  -682,  -682,  -682,  -682,   812,
    -682,  -682,   982,  -682,   169,   873,   372,  -682,  -682,  1011,
     874,   875,  -682,   946,   877,  -682,  -682,   757,   881,   -47,
     879,   885,   882,  -682,  -682,   883,  -682,  -682,   390,   884,
    -682,   757,  -682,   108,  -682,  -682,  -682,   887,  -682,  -682,
     957,  -682,   888,   285,  -682,    78,   890,  -682,  -682,   824,
     285,   889,  -682,  -682,  -682
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -682,  -682,  1026,  -682,  -682,  -682,  -682,  -682,   144,  -682,
    -682,   833,   -87,  -307,   685,   864,   975,  -417,  -682,  -682,
    -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,
    -682,  -682,  -682,  -682,   984,  -682,  -682,  -682,   247,  -682,
    -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,
    -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,   628,   876,
    -682,  -682,  -682,  -682,   571,  -682,  -682,  -682,   264,  -682,
    -682,  -682,  -540,   256,  -682,   346,   224,  -682,  -682,  -682,
    -682,  -682,   185,  -682,  -682,   900,  -682,  -682,  -682,  -682,
    -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,
    -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,
    -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,  -682,
     424,  -681,   203,  -682,  -405,  -511,  -682,  -682,  -682,   376,
     694,  -180,  -144,  -337,   593,   -35,  -319,  -400,  -545,  -427,
    -537,   604,  -532,  -140,   -48,  -682,   404,  -682,  -682,   620,
    -682,  -682,   802,  -133,   586,  -407,  -682,  -682,  -682,  -682,
    -682,  -146,  -682
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -204
static const yytype_int16 yytable[] =
{
     197,   221,   315,   207,   419,   528,   445,   249,   250,   632,
     550,   272,   273,   538,   275,   278,   194,   288,   289,   290,
     266,   541,   542,   570,   446,   296,   663,    16,   299,   223,
     785,   189,   661,   234,   548,   659,   276,   297,   298,   189,
     300,   301,   517,   318,   212,   213,   312,   779,     7,   243,
     490,   572,   189,     8,   344,   335,   544,   336,   346,   189,
     413,   337,   338,   339,   474,   457,   283,   477,   535,   838,
     490,   679,   302,   699,   246,   559,   303,   304,   308,   247,
     709,   302,   560,   887,   189,   303,   304,   561,   320,   224,
     212,   213,   798,   235,   562,   625,   203,   563,   212,   213,
       9,   839,   225,   714,   564,    15,   780,   204,   205,   382,
     383,   530,   385,   531,   325,   326,   327,   328,   329,   203,
     295,   518,   340,   217,    11,   281,   282,   640,   565,   440,
     204,   205,   287,   320,   244,   566,   309,   425,   267,   852,
     305,   888,   321,    13,   788,   313,   789,   536,   331,   490,
     347,   284,   700,   729,   325,   326,   327,   328,   329,   710,
     545,   268,   269,   345,   341,    64,   216,   503,   226,   342,
      14,   504,   236,   764,   227,   203,   696,   342,   237,   666,
     140,   343,   795,   203,   191,   192,   204,   205,   794,   540,
     190,   793,   191,   192,   204,   205,   203,   190,   218,   219,
     268,   269,   676,   447,   458,   191,   192,   204,   205,   540,
     293,   223,   191,   192,   493,   494,   380,   381,   141,   692,
     193,   188,   190,   143,   655,   223,   693,   144,   193,   463,
     525,   478,   479,   145,   550,   146,   646,   191,   192,   147,
     654,   193,   488,   489,   480,   716,   735,   723,   193,   689,
     725,   487,   666,   511,   724,   497,   516,   726,   148,   500,
     149,   522,   523,   524,   234,   453,   182,   190,   424,   529,
     150,   224,   151,   193,   430,   431,   152,   432,   433,   434,
     435,   436,   191,   192,   225,   224,   549,   760,   540,   727,
     761,   153,   571,   154,   321,   448,   728,   302,   225,   584,
     782,   303,   304,   585,   586,   276,   557,   454,   155,   861,
     323,   490,   -13,   493,   494,   491,   465,   156,   193,   157,
     762,   472,   212,   213,   235,   476,     1,     2,   506,   601,
     507,   508,   158,   324,   159,   234,   160,   715,   161,   609,
     610,   325,   326,   327,   328,   329,   502,   162,   212,   213,
     226,   466,   467,   587,   588,   589,   227,   163,   212,   213,
     485,   712,   882,   196,   226,   327,   328,   329,   567,   164,
     227,   212,   213,   590,   325,   326,   327,   328,   329,   228,
     229,   829,   325,   326,   327,   328,   329,   591,   592,   331,
     216,   718,   719,   228,   229,   235,   593,   594,   595,   596,
     597,   277,   554,   236,   810,   811,   812,   569,   842,   237,
     165,   843,   598,   166,   599,   792,   216,   617,   363,   327,
     328,   329,   218,   219,   198,   657,   216,   217,   214,   215,
     238,   239,   251,   252,   253,   254,   255,   217,   256,   216,
     257,   258,   259,   260,   261,   167,   168,   169,   218,   219,
     492,   325,   326,   327,   328,   329,   170,   704,   218,   219,
     171,   172,   325,   326,   327,   328,   329,   201,   173,   429,
     174,   218,   219,   202,   236,   493,   494,   212,   213,   680,
     237,   263,   175,   683,   176,   177,   686,   687,   193,   179,
     732,   733,   734,   325,   326,   327,   328,   329,   648,   706,
     180,   238,   239,   181,   187,   363,   212,   213,   847,   199,
     214,   215,   274,   293,   317,   656,   316,   319,   322,   855,
     325,   326,   327,   328,   329,   208,   209,   210,   211,   731,
     325,   808,   327,   328,   329,   755,   352,   355,   251,   252,
     253,   254,   255,   555,   256,   216,   257,   258,   259,   260,
     261,   356,   574,   357,   362,   358,   262,   363,   359,   360,
     364,   879,   580,   365,   212,   213,   681,   682,   366,   368,
     685,   369,   372,   373,   216,   375,  -203,   218,   219,   376,
     377,   378,   485,   212,   213,   217,   379,   263,   392,   384,
     386,   387,   325,   326,   327,   328,   329,   388,   821,   390,
     799,   325,   326,   327,   328,   329,   218,   219,   212,   213,
     393,   325,   326,   327,   328,   329,   394,   395,   396,   397,
     835,   836,   837,   398,   399,   251,   252,   253,   254,   255,
     758,   256,   216,   257,   258,   259,   260,   261,   400,   583,
     768,   404,   401,   262,   251,   252,   253,   254,   255,   605,
     256,   216,   257,   258,   259,   260,   261,   402,   403,   407,
     406,   408,   262,   409,   218,   219,   410,   411,   412,   251,
     252,   253,   254,   255,   263,   256,   216,   257,   258,   259,
     260,   261,   623,   218,   219,   423,   414,   262,   325,   326,
     327,   328,   329,   263,   428,   415,   416,   417,   325,   326,
     327,   328,   329,   765,   418,   183,   438,   421,   218,   219,
     422,    67,   426,    69,   806,    71,   437,    73,   263,    75,
     439,    77,   449,    79,   450,    81,   451,    83,   456,   460,
     461,   325,   326,   327,   328,   329,   462,   464,   469,   886,
     471,   470,   473,   475,   481,   482,   893,   483,   484,   498,
     501,   512,   325,   326,   327,   328,   329,   499,   513,   514,
     844,   515,   520,    16,   521,   526,   533,  -161,   534,   558,
     872,    17,   547,   575,    18,    19,    20,    21,    22,    23,
      24,    25,    26,   573,   881,   576,    27,    28,    29,   578,
     579,   582,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,   613,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    68,    50,    70,   604,    72,   607,    74,
     618,    76,   611,    78,   608,    80,   612,    82,   614,    84,
     622,   625,   615,   616,    51,   619,   620,   621,   624,   626,
     627,    52,    53,   628,    54,   629,    55,    56,    57,   736,
     737,    58,    59,   630,    60,   631,    61,   633,   634,   635,
     637,   638,   639,   641,   642,   738,   739,   740,   643,   644,
     617,   645,   647,   650,   651,    62,   653,    63,   652,   267,
     664,   665,   666,   669,   668,   667,   670,   671,   741,   672,
     742,   743,   744,   745,   746,   747,   748,   749,   750,   751,
     673,    64,   675,   674,   677,   678,   485,    65,    66,   684,
     688,   689,   691,   695,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,   203,   697,   698,   696,   703,   702,   705,    66,
     707,   708,   717,   204,   205,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,   730,   756,   759,   769,   774,   763,   770,
     766,   767,   773,   775,   777,   772,   778,   783,   784,   800,
     786,   787,   797,   804,   801,   796,   802,   803,   805,   816,
     807,   809,   817,   818,   819,   828,   834,   845,   822,   823,
     824,   825,   850,   841,   830,   831,   833,   854,   846,   849,
     853,   858,   856,   859,   860,   865,   863,   866,   867,   869,
     870,   873,   875,   876,   880,   877,   878,   883,   885,   884,
     890,    10,   894,   892,   184,   354,   452,   178,   832,   581,
     527,   820,   827,   851,   874,   314,   776,   233,   690,   864,
     455,   754,   307,   568,   577,   721,     0,   556,     0,   602,
       0,     0,   389
};

static const yytype_int16 yycheck[] =
{
      87,   145,   182,   143,   311,   412,   343,   151,   152,   520,
     427,   155,   156,   418,   158,   159,    64,   163,   164,   165,
     153,   421,   422,   450,   343,   169,   571,     6,   172,     3,
     711,     4,   569,     3,    63,   567,    46,   170,   171,     4,
     173,   174,    63,   187,    63,    64,   179,     4,   141,    63,
       3,   451,     4,   141,   198,    20,    96,    22,   198,     4,
     140,    26,    27,    28,   371,     3,    63,   374,    63,   116,
       3,   611,     4,    63,    58,   140,     8,     9,     5,    63,
      63,     4,   147,     5,     4,     8,     9,   140,     8,    63,
      63,    64,   141,    63,   147,   144,   148,   140,    63,    64,
       0,   148,    76,   643,   147,   141,    63,   159,   160,   253,
     254,    61,   256,    63,   194,   195,   196,   197,   198,   148,
     168,   142,    87,   142,    79,   160,   161,   534,   140,     4,
     159,   160,   142,     8,   148,   147,    63,   317,   142,   820,
      63,    63,   190,   148,    98,   180,   100,   142,   196,     3,
     198,   148,   142,   664,   194,   195,   196,   197,   198,   142,
     200,   165,   166,   198,   129,   144,   131,    59,   142,   142,
     148,    63,   142,   684,   148,   148,   140,   142,   148,   143,
     140,   146,   727,   148,   157,   158,   159,   160,   725,   142,
     142,   723,   157,   158,   159,   160,   148,   142,   163,   164,
     165,   166,   607,   343,   142,   157,   158,   159,   160,   142,
     142,     3,   157,   158,   167,   168,   251,   252,    80,    63,
     193,   150,   142,   141,   561,     3,    70,   141,   193,   362,
     410,   375,   376,   141,   651,   141,   543,   157,   158,   141,
     559,   193,   386,   387,   377,   645,   673,   140,   193,   140,
     140,   384,   143,   397,   147,   388,   402,   147,   141,   392,
     141,   407,   408,   409,     3,   352,   144,   142,   316,   413,
     141,    63,   141,   193,   322,   323,   141,   325,   326,   327,
     328,   329,   157,   158,    76,    63,   426,   140,   142,   140,
     143,   141,   146,   141,   342,   343,   147,     4,    76,     4,
     707,     8,     9,     8,     9,    46,   439,   355,   141,   140,
     122,     3,   143,   167,   168,     7,   364,   141,   193,   141,
     140,   369,    63,    64,    63,   373,    15,    16,    63,   473,
      65,    66,   141,   145,   141,     3,   141,   644,   141,   483,
     484,   194,   195,   196,   197,   198,   394,   141,    63,    64,
     142,    68,    69,    58,    59,    60,   148,   141,    63,    64,
      62,    63,   873,   142,   142,   196,   197,   198,   146,   141,
     148,    63,    64,    78,   194,   195,   196,   197,   198,   171,
     172,   788,   194,   195,   196,   197,   198,    92,    93,   437,
     131,   153,   154,   171,   172,    63,   101,   102,   103,   104,
     105,   142,   145,   142,    32,    33,    34,   146,   140,   148,
     141,   143,   117,   141,   119,   722,   131,     3,     4,   196,
     197,   198,   163,   164,   199,   565,   131,   142,    67,    68,
     169,   170,   124,   125,   126,   127,   128,   142,   130,   131,
     132,   133,   134,   135,   136,   141,   141,   141,   163,   164,
     142,   194,   195,   196,   197,   198,   141,   637,   163,   164,
     141,   141,   194,   195,   196,   197,   198,     3,   141,   143,
     141,   163,   164,   140,   142,   167,   168,    63,    64,   612,
     148,   173,   141,   616,   141,   141,   619,   620,   193,   141,
     670,   671,   672,   194,   195,   196,   197,   198,   546,   639,
     141,   169,   170,   141,   141,     4,    63,    64,   815,   141,
      67,    68,    74,   142,   144,   563,   199,   142,   144,   826,
     194,   195,   196,   197,   198,    10,    11,    12,    13,   669,
     194,   195,   196,   197,   198,   675,   146,   144,   124,   125,
     126,   127,   128,   145,   130,   131,   132,   133,   134,   135,
     136,   200,   145,   140,   142,   140,   142,     4,   140,   139,
     144,   868,   145,   140,    63,    64,   614,   615,   140,     3,
     618,   144,     3,   144,   131,   140,   146,   163,   164,   140,
     140,   140,    62,    63,    64,   142,   140,   173,   202,   142,
     142,   142,   194,   195,   196,   197,   198,   142,   778,   142,
     740,   194,   195,   196,   197,   198,   163,   164,    63,    64,
       4,   194,   195,   196,   197,   198,   144,   140,   140,   140,
     800,   801,   802,   140,   142,   124,   125,   126,   127,   128,
     678,   130,   131,   132,   133,   134,   135,   136,     4,   145,
     688,     6,   140,   142,   124,   125,   126,   127,   128,   145,
     130,   131,   132,   133,   134,   135,   136,   140,   140,   140,
       4,   140,   142,   140,   163,   164,   140,     4,   140,   124,
     125,   126,   127,   128,   173,   130,   131,   132,   133,   134,
     135,   136,   145,   163,   164,   145,   140,   142,   194,   195,
     196,   197,   198,   173,   143,   140,   140,   140,   194,   195,
     196,   197,   198,   145,   140,   151,   143,   140,   163,   164,
     140,   157,   140,   159,   762,   161,   140,   163,   173,   165,
     141,   167,   141,   169,   141,   171,   141,   173,     3,     3,
     140,   194,   195,   196,   197,   198,   145,   140,     5,   883,
     140,   149,   140,   140,    62,    62,   890,   140,   140,   137,
     140,    71,   194,   195,   196,   197,   198,   143,     4,   140,
     808,    71,   140,     6,   140,   140,    75,    86,   142,   140,
     857,    14,   145,     3,    17,    18,    19,    20,    21,    22,
      23,    24,    25,   147,   871,   140,    29,    30,    31,   143,
       4,   148,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,   143,    47,    48,    49,    50,    51,    52,
      53,    54,    55,   158,    57,   160,   148,   162,   140,   164,
     144,   166,   201,   168,   140,   170,   140,   172,   140,   174,
       4,   144,   140,   140,    77,   140,   140,   140,   140,   140,
     140,    84,    85,     4,    87,   140,    89,    90,    91,    65,
      66,    94,    95,     4,    97,   140,    99,     4,   140,   140,
     140,    62,   140,     4,   140,    81,    82,    83,   140,    86,
       3,   140,   200,   141,   140,   118,     4,   120,   143,   142,
     140,     3,   143,   141,   143,   149,   141,   141,   104,   141,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     141,   144,   140,   143,     5,   140,    62,   150,   151,   140,
     142,   140,     4,     4,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   148,    71,   140,   140,    71,   148,   140,   151,
     140,   140,   146,   159,   160,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   140,   140,   143,     4,     4,   143,   140,
     143,   143,   140,     4,   140,   145,   140,     4,     4,   141,
     140,   140,     5,     3,   141,   143,   141,   141,    32,     4,
     143,   140,   140,   143,   140,     7,    63,     4,   143,   143,
     143,   140,     4,   143,   141,   147,   140,     4,   140,   140,
     140,     5,   141,   201,    32,     4,   143,   143,   143,    73,
     143,   140,   143,   138,   140,   143,   143,   140,   140,    72,
     140,     5,   143,   889,    59,   202,   351,    53,   791,   468,
     412,   777,   786,   819,   859,   181,   700,   147,   624,   846,
     356,   675,   176,   449,   461,   651,    -1,   437,    -1,   473,
      -1,    -1,   260
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   204,   205,   206,   207,   141,   141,     0,
     205,    79,   213,   148,   148,   141,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    29,    30,    31,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      57,    77,    84,    85,    87,    89,    90,    91,    94,    95,
      97,    99,   118,   120,   144,   150,   151,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   208,   215,   217,   218,   219,
     220,   222,   223,   231,   234,   235,   237,   238,   245,   247,
     248,   250,   252,   254,   258,   259,   260,   263,   265,   272,
     277,   280,   286,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   302,   303,   304,   305,   306,   308,   309,
     310,   311,   312,   315,   316,   317,   318,   319,   320,   321,
     140,    80,   214,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   237,   141,
     141,   141,   144,   151,   219,   221,   239,   141,   150,     4,
     142,   157,   158,   193,   347,   357,   142,   215,   199,   141,
     249,     3,   140,   148,   159,   160,   333,   346,    10,    11,
      12,    13,    63,    64,    67,    68,   131,   142,   163,   164,
     278,   335,   336,     3,    63,    76,   142,   148,   171,   172,
     288,   344,   345,   288,     3,    63,   142,   148,   169,   170,
     282,   342,   343,    63,   148,   322,    58,    63,   327,   335,
     335,   124,   125,   126,   127,   128,   130,   132,   133,   134,
     135,   136,   142,   173,   335,   355,   356,   142,   165,   166,
     338,   339,   335,   335,    74,   335,    46,   142,   335,   364,
     365,   338,   338,    63,   148,   262,   323,   142,   364,   364,
     364,   334,   347,   142,   261,   347,   335,   356,   356,   335,
     356,   356,     4,     8,     9,    63,   358,   262,     5,    63,
     273,   255,   356,   338,   218,   334,   199,   144,   335,   142,
       8,   347,   144,   122,   145,   194,   195,   196,   197,   198,
     346,   347,   352,   353,   354,    20,    22,    26,    27,    28,
      87,   129,   142,   146,   335,   338,   346,   347,   360,   361,
     362,   253,   146,   216,   214,   144,   200,   140,   140,   140,
     139,   337,   142,     4,   144,   140,   140,   267,     3,   144,
     289,   287,     3,   144,   281,   140,   140,   140,   140,   140,
     338,   338,   335,   335,   142,   335,   142,   142,   142,   355,
     142,   356,   202,     4,   144,   140,   140,   140,   140,   142,
       4,   140,   140,   140,     6,   236,     4,   140,   140,   140,
     140,     4,   140,   140,   140,   140,   140,   140,   140,   216,
     256,   140,   140,   145,   347,   334,   140,   232,   143,   143,
     347,   347,   347,   347,   347,   347,   347,   140,   143,   141,
       4,   227,   228,   229,   230,   336,   339,   346,   347,   141,
     141,   141,   217,   215,   347,   333,     3,     3,   142,   307,
       3,   140,   145,   356,   140,   347,    68,    69,   279,     5,
     149,   140,   347,   140,   216,   140,   347,   216,   335,   335,
     356,    62,    62,   140,   140,    62,   275,   356,   335,   335,
       3,     7,   142,   167,   168,   340,   341,   356,   137,   143,
     356,   140,   347,    59,    63,   328,    63,    65,    66,   329,
     332,   335,    71,     4,   140,    71,   364,    63,   142,   268,
     140,   140,   364,   364,   364,   334,   140,   261,   358,   335,
      61,    63,   363,    75,   142,    63,   142,   269,   327,   257,
     142,   340,   340,   251,    96,   200,   246,   145,    63,   346,
     220,   349,   350,   351,   145,   145,   352,   356,   140,   140,
     147,   140,   147,   140,   147,   140,   147,   146,   344,   146,
     342,   146,   340,   147,   145,     3,   140,   337,   143,     4,
     145,   267,   148,   145,     4,     8,     9,    58,    59,    60,
      78,    92,    93,   101,   102,   103,   104,   105,   117,   119,
     290,   335,   357,   359,   148,   145,   283,   140,   140,   335,
     335,   201,   140,   143,   140,   140,   140,     3,   144,   140,
     140,   140,     4,   145,   140,   144,   140,   140,     4,   140,
       4,   140,   328,     4,   140,   140,   301,   140,    62,   140,
     358,     4,   140,   140,    86,   140,   216,   200,   347,   240,
     141,   140,   143,     4,   339,   336,   347,   346,   224,   345,
     225,   343,   226,   341,   140,     3,   143,   149,   143,   141,
     141,   141,   141,   141,   143,   140,   327,     5,   140,   275,
     356,   347,   347,   356,   140,   347,   356,   356,   142,   140,
     323,     4,    63,    70,   331,     4,   140,    71,   140,    63,
     142,   270,   148,    71,   334,   140,   346,   140,   140,    63,
     142,   271,    63,   274,   275,   216,   340,   146,   153,   154,
     348,   349,   233,   140,   147,   140,   147,   140,   147,   328,
     140,   346,   334,   334,   334,   342,    65,    66,    81,    82,
      83,   104,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   284,   330,   332,   346,   140,   211,   347,   143,
     140,   143,   140,   143,   328,   145,   143,   143,   347,     4,
     140,   324,   145,   140,     4,     4,   278,   140,   140,     4,
      63,   276,   358,     4,     4,   324,   140,   140,    98,   100,
     241,   242,   216,   345,   343,   341,   143,     5,   141,   346,
     141,   141,   141,   141,     3,    32,   347,   143,   195,   140,
      32,    33,    34,   325,   326,   313,     4,   140,   143,   140,
     271,   334,   143,   143,   143,   140,   264,   276,     7,   358,
     141,   147,   241,   140,    63,   334,   334,   334,   116,   148,
     285,   143,   140,   143,   347,     4,   140,   216,   314,   140,
       4,   279,   324,   140,     4,   216,   141,   244,     5,   201,
      32,   140,   209,   143,   325,     4,   143,   143,   266,    73,
     143,   243,   215,   140,   285,   143,   138,   143,   143,   216,
     140,   215,   328,   140,    72,   140,   335,     5,    63,   212,
     140,   210,   211,   335,   143
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
#line 290 "lev_comp.y"
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
#line 313 "lev_comp.y"
    {
		      start_level_def(&splev, (yyvsp[(3) - (3)].map));
		      (yyval.map) = (yyvsp[(3) - (3)].map);
		  ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 318 "lev_comp.y"
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
#line 340 "lev_comp.y"
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
#line 353 "lev_comp.y"
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
#line 365 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_ROGUE,0,0,0,
					  0,0,0,0, SPO_INITLEVEL));
		  ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 371 "lev_comp.y"
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
#line 400 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 404 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 410 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_COPY));
		      (yyval.i) = 0;
		  ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 415 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 421 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 425 "lev_comp.y"
    {
		      (yyval.i) = what_map_char((char) (yyvsp[(2) - (2)].i));
		  ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 436 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 440 "lev_comp.y"
    {
		      add_opvars(splev, "io",
                                 VA_PASS2((int) (yyvsp[(3) - (3)].i), SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 447 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 451 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 457 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 461 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 467 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (3)].i);
		  ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 554 "lev_comp.y"
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
#line 570 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 576 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 582 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 588 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 594 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 600 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 606 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 612 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 618 "lev_comp.y"
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
#line 628 "lev_comp.y"
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
#line 638 "lev_comp.y"
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
#line 648 "lev_comp.y"
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
#line 658 "lev_comp.y"
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
#line 668 "lev_comp.y"
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
#line 678 "lev_comp.y"
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
#line 690 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 695 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 702 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 707 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 714 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 719 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 726 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 730 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 736 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 741 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 748 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 752 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 758 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 762 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 768 "lev_comp.y"
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
#line 790 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 794 "lev_comp.y"
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
#line 805 "lev_comp.y"
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
#line 860 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 866 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 870 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 876 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 883 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 887 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 895 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 899 "lev_comp.y"
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
#line 922 "lev_comp.y"
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
#line 967 "lev_comp.y"
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
#line 977 "lev_comp.y"
    {
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 980 "lev_comp.y"
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
#line 990 "lev_comp.y"
    {
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 995 "lev_comp.y"
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
#line 1009 "lev_comp.y"
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
#line 1046 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1051 "lev_comp.y"
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
#line 1080 "lev_comp.y"
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
#line 1094 "lev_comp.y"
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
#line 1110 "lev_comp.y"
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
#line 1130 "lev_comp.y"
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
#line 1142 "lev_comp.y"
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
#line 1162 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1168 "lev_comp.y"
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
#line 1178 "lev_comp.y"
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
#line 1196 "lev_comp.y"
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
#line 1206 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1212 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1217 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1222 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1229 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1236 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1245 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1253 "lev_comp.y"
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
#line 1264 "lev_comp.y"
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
#line 1275 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1282 "lev_comp.y"
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
#line 1293 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1300 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1304 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1310 "lev_comp.y"
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
#line 1320 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1326 "lev_comp.y"
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
#line 1335 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1341 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1346 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1352 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1357 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1363 "lev_comp.y"
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
#line 1374 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (5)].i), SPO_DOOR));
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1388 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1392 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1402 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1409 "lev_comp.y"
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
#line 1417 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1433 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1437 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1443 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1451 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1457 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x0000;
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1465 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1473 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1478 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1484 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1490 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1496 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1502 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1507 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1512 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1517 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1522 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1527 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1532 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1537 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1542 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1547 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1552 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1560 "lev_comp.y"
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
#line 1569 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1573 "lev_comp.y"
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
#line 1586 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1594 "lev_comp.y"
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
#line 1604 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1612 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1621 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1628 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1636 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1642 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1647 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1652 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1657 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1662 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1667 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1672 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1677 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1682 "lev_comp.y"
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
#line 1693 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1699 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1704 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1709 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1714 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1721 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1727 "lev_comp.y"
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
#line 1756 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1761 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1769 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1775 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1781 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1788 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1795 "lev_comp.y"
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
#line 1805 "lev_comp.y"
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
#line 1815 "lev_comp.y"
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
#line 1830 "lev_comp.y"
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
#line 1840 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1844 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1850 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1856 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1862 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int)(yyvsp[(6) - (13)].i), (int)(yyvsp[(8) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1868 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1874 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1880 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1885 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1892 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1899 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1905 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1911 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1917 "lev_comp.y"
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
#line 1931 "lev_comp.y"
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
#line 1941 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1945 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1951 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1958 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1964 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1970 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1974 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1979 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1986 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1992 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1999 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 2003 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 2010 "lev_comp.y"
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
#line 2021 "lev_comp.y"
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
#line 2034 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 2038 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 2044 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2048 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2055 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2059 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2063 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2079 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2087 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2097 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2106 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2111 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2118 "lev_comp.y"
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
#line 2129 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2135 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2139 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2143 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2150 "lev_comp.y"
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
#line 2160 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2167 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2171 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2177 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2181 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2189 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2193 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2200 "lev_comp.y"
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
#line 2210 "lev_comp.y"
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
#line 2223 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2227 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2234 "lev_comp.y"
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
#line 2244 "lev_comp.y"
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
#line 2253 "lev_comp.y"
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
#line 2264 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2268 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2275 "lev_comp.y"
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
#line 2285 "lev_comp.y"
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
#line 2296 "lev_comp.y"
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
#line 2305 "lev_comp.y"
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
#line 2315 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2321 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2325 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2332 "lev_comp.y"
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
#line 2342 "lev_comp.y"
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
#line 2353 "lev_comp.y"
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
#line 2362 "lev_comp.y"
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
#line 2372 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2376 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2382 "lev_comp.y"
    { ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2384 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2390 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2394 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2398 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2402 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2410 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2419 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2423 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2427 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2431 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2435 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2438 "lev_comp.y"
    { ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2442 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2449 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2458 "lev_comp.y"
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

  case 368:

/* Line 1455 of yacc.c  */
#line 2499 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2503 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2510 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2517 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2527 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2531 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2539 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2543 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2547 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2551 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2555 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2560 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2564 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2568 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2573 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2578 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2583 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2587 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2592 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2597 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2601 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2605 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2610 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2614 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2621 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2627 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2631 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2637 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2649 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2653 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2657 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2661 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2683 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2687 "lev_comp.y"
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

  case 413:

/* Line 1455 of yacc.c  */
#line 2713 "lev_comp.y"
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
#line 6122 "lev_comp.tab.c"
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
#line 2741 "lev_comp.y"


/*lev_comp.y*/

