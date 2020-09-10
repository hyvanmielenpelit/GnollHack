
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
     TILESET_ID = 273,
     GEOMETRY_ID = 274,
     NOMAP_ID = 275,
     OBJECT_ID = 276,
     COBJECT_ID = 277,
     MONSTER_ID = 278,
     TRAP_ID = 279,
     DOOR_ID = 280,
     DRAWBRIDGE_ID = 281,
     object_ID = 282,
     monster_ID = 283,
     terrain_ID = 284,
     MAZEWALK_ID = 285,
     WALLIFY_ID = 286,
     REGION_ID = 287,
     FILLING = 288,
     IRREGULAR = 289,
     JOINED = 290,
     ALTAR_ID = 291,
     ANVIL_ID = 292,
     NPC_ID = 293,
     LADDER_ID = 294,
     STAIR_ID = 295,
     NON_DIGGABLE_ID = 296,
     NON_PASSWALL_ID = 297,
     ROOM_ID = 298,
     PORTAL_ID = 299,
     TELEPRT_ID = 300,
     BRANCH_ID = 301,
     LEV = 302,
     MINERALIZE_ID = 303,
     CORRIDOR_ID = 304,
     GOLD_ID = 305,
     ENGRAVING_ID = 306,
     FOUNTAIN_ID = 307,
     THRONE_ID = 308,
     MODRON_PORTAL_ID = 309,
     POOL_ID = 310,
     SINK_ID = 311,
     NONE = 312,
     RAND_CORRIDOR_ID = 313,
     DOOR_STATE = 314,
     LIGHT_STATE = 315,
     CURSE_TYPE = 316,
     ENGRAVING_TYPE = 317,
     DIRECTION = 318,
     RANDOM_TYPE = 319,
     RANDOM_TYPE_BRACKET = 320,
     A_REGISTER = 321,
     ALIGNMENT = 322,
     LEFT_OR_RIGHT = 323,
     CENTER = 324,
     TOP_OR_BOT = 325,
     ALTAR_TYPE = 326,
     UP_OR_DOWN = 327,
     ACTIVE_OR_INACTIVE = 328,
     MODRON_PORTAL_TYPE = 329,
     NPC_TYPE = 330,
     FOUNTAIN_TYPE = 331,
     SPECIAL_OBJECT_TYPE = 332,
     CMAP_TYPE = 333,
     SUBROOM_ID = 334,
     NAME_ID = 335,
     FLAGS_ID = 336,
     FLAG_TYPE = 337,
     MON_ATTITUDE = 338,
     MON_ALERTNESS = 339,
     SUBTYPE_ID = 340,
     MON_APPEARANCE = 341,
     ROOMDOOR_ID = 342,
     IF_ID = 343,
     ELSE_ID = 344,
     TERRAIN_ID = 345,
     HORIZ_OR_VERT = 346,
     REPLACE_TERRAIN_ID = 347,
     EXIT_ID = 348,
     SHUFFLE_ID = 349,
     QUANTITY_ID = 350,
     BURIED_ID = 351,
     LOOP_ID = 352,
     FOR_ID = 353,
     TO_ID = 354,
     SWITCH_ID = 355,
     CASE_ID = 356,
     BREAK_ID = 357,
     DEFAULT_ID = 358,
     ERODED_ID = 359,
     TRAPPED_STATE = 360,
     RECHARGED_ID = 361,
     INVIS_ID = 362,
     GREASED_ID = 363,
     FEMALE_ID = 364,
     WAITFORU_ID = 365,
     CANCELLED_ID = 366,
     REVIVED_ID = 367,
     AVENGE_ID = 368,
     FLEEING_ID = 369,
     BLINDED_ID = 370,
     PARALYZED_ID = 371,
     STUNNED_ID = 372,
     CONFUSED_ID = 373,
     SEENTRAPS_ID = 374,
     ALL_ID = 375,
     MONTYPE_ID = 376,
     GRAVE_ID = 377,
     ERODEPROOF_ID = 378,
     FUNCTION_ID = 379,
     MSG_OUTPUT_TYPE = 380,
     COMPARE_TYPE = 381,
     UNKNOWN_TYPE = 382,
     rect_ID = 383,
     fillrect_ID = 384,
     line_ID = 385,
     randline_ID = 386,
     grow_ID = 387,
     selection_ID = 388,
     flood_ID = 389,
     rndcoord_ID = 390,
     circle_ID = 391,
     ellipse_ID = 392,
     filter_ID = 393,
     complement_ID = 394,
     gradient_ID = 395,
     GRADIENT_TYPE = 396,
     LIMITED = 397,
     HUMIDITY_TYPE = 398,
     STRING = 399,
     MAP_ID = 400,
     NQSTRING = 401,
     VARSTRING = 402,
     CFUNC = 403,
     CFUNC_INT = 404,
     CFUNC_STR = 405,
     CFUNC_COORD = 406,
     CFUNC_REGION = 407,
     VARSTRING_INT = 408,
     VARSTRING_INT_ARRAY = 409,
     VARSTRING_STRING = 410,
     VARSTRING_STRING_ARRAY = 411,
     VARSTRING_VAR = 412,
     VARSTRING_VAR_ARRAY = 413,
     VARSTRING_COORD = 414,
     VARSTRING_COORD_ARRAY = 415,
     VARSTRING_REGION = 416,
     VARSTRING_REGION_ARRAY = 417,
     VARSTRING_MAPCHAR = 418,
     VARSTRING_MAPCHAR_ARRAY = 419,
     VARSTRING_MONST = 420,
     VARSTRING_MONST_ARRAY = 421,
     VARSTRING_OBJ = 422,
     VARSTRING_OBJ_ARRAY = 423,
     VARSTRING_SEL = 424,
     VARSTRING_SEL_ARRAY = 425,
     METHOD_INT = 426,
     METHOD_INT_ARRAY = 427,
     METHOD_STRING = 428,
     METHOD_STRING_ARRAY = 429,
     METHOD_VAR = 430,
     METHOD_VAR_ARRAY = 431,
     METHOD_COORD = 432,
     METHOD_COORD_ARRAY = 433,
     METHOD_REGION = 434,
     METHOD_REGION_ARRAY = 435,
     METHOD_MAPCHAR = 436,
     METHOD_MAPCHAR_ARRAY = 437,
     METHOD_MONST = 438,
     METHOD_MONST_ARRAY = 439,
     METHOD_OBJ = 440,
     METHOD_OBJ_ARRAY = 441,
     METHOD_SEL = 442,
     METHOD_SEL_ARRAY = 443,
     DICE = 444
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
#line 487 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 499 "lev_comp.tab.c"

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
#define YYLAST   1068

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  207
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  166
/* YYNRULES -- Number of rules.  */
#define YYNRULES  420
/* YYNRULES -- Number of states.  */
#define YYNSTATES  908

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   444

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   202,   206,     2,
     146,   147,   200,   198,   144,   199,   204,   201,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   145,     2,
       2,   203,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   148,     2,   149,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   150,   205,   151,     2,     2,     2,     2,
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
     135,   136,   137,   138,   139,   140,   141,   142,   143,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    15,    19,    25,
      31,    37,    41,    58,    62,    63,    66,    67,    70,    71,
      74,    76,    78,    79,    83,    87,    89,    90,    93,    97,
      99,   101,   103,   105,   107,   109,   111,   113,   115,   117,
     119,   121,   123,   125,   127,   129,   131,   133,   135,   137,
     139,   141,   143,   145,   147,   149,   151,   153,   155,   157,
     159,   161,   163,   165,   167,   169,   171,   173,   175,   177,
     179,   181,   183,   185,   187,   189,   191,   193,   195,   197,
     199,   201,   203,   205,   207,   209,   211,   213,   215,   217,
     219,   221,   223,   225,   227,   229,   231,   233,   235,   237,
     239,   241,   243,   247,   251,   257,   261,   267,   273,   279,
     283,   287,   293,   299,   305,   313,   321,   329,   335,   337,
     341,   343,   347,   349,   353,   355,   359,   361,   365,   367,
     371,   373,   377,   378,   379,   388,   393,   395,   396,   398,
     400,   406,   410,   411,   412,   422,   423,   426,   427,   433,
     434,   439,   441,   444,   446,   453,   454,   458,   459,   466,
     467,   472,   473,   478,   480,   481,   486,   490,   492,   496,
     500,   506,   512,   520,   525,   526,   537,   538,   551,   552,
     555,   561,   563,   569,   571,   577,   579,   585,   587,   597,
     603,   605,   607,   609,   611,   613,   617,   619,   621,   623,
     631,   637,   639,   641,   643,   645,   649,   650,   656,   661,
     662,   666,   668,   670,   672,   674,   677,   679,   681,   683,
     685,   687,   691,   695,   699,   701,   703,   707,   709,   711,
     713,   717,   721,   722,   728,   731,   732,   736,   738,   742,
     744,   748,   752,   754,   756,   760,   762,   764,   766,   770,
     772,   774,   776,   782,   790,   796,   805,   807,   811,   817,
     823,   831,   839,   846,   852,   853,   856,   862,   866,   880,
     884,   888,   890,   896,   906,   912,   916,   920,   921,   932,
     933,   935,   943,   947,   953,   959,   965,   971,   975,   981,
     989,   999,  1001,  1003,  1005,  1007,  1009,  1010,  1013,  1015,
    1019,  1021,  1023,  1025,  1027,  1029,  1031,  1033,  1035,  1037,
    1039,  1041,  1043,  1047,  1049,  1051,  1056,  1058,  1060,  1065,
    1067,  1069,  1074,  1076,  1081,  1087,  1089,  1093,  1095,  1099,
    1101,  1103,  1108,  1118,  1120,  1122,  1127,  1129,  1135,  1137,
    1139,  1144,  1146,  1148,  1154,  1156,  1158,  1160,  1165,  1167,
    1169,  1175,  1177,  1179,  1181,  1185,  1187,  1189,  1193,  1195,
    1200,  1204,  1208,  1212,  1216,  1220,  1224,  1226,  1228,  1232,
    1234,  1238,  1239,  1241,  1243,  1245,  1247,  1251,  1252,  1254,
    1256,  1259,  1262,  1267,  1274,  1279,  1286,  1293,  1300,  1307,
    1310,  1317,  1326,  1335,  1346,  1361,  1364,  1366,  1370,  1372,
    1376,  1378,  1380,  1382,  1384,  1386,  1388,  1390,  1392,  1394,
    1396,  1398,  1400,  1402,  1404,  1406,  1408,  1410,  1412,  1414,
    1425
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     208,     0,    -1,    -1,   209,    -1,   210,    -1,   210,   209,
      -1,   211,   218,   220,    -1,    16,   145,   152,    -1,    15,
     145,   152,   144,     3,    -1,    17,   145,    11,   144,   312,
      -1,    17,   145,    10,   144,     3,    -1,    17,   145,    13,
      -1,    17,   145,    12,   144,     3,   144,     3,   144,     5,
     144,     5,   144,   334,   144,   217,   216,    -1,    18,   145,
     364,    -1,    -1,   144,   142,    -1,    -1,   144,   341,    -1,
      -1,   144,     3,    -1,     5,    -1,    64,    -1,    -1,    81,
     145,   219,    -1,    82,   144,   219,    -1,    82,    -1,    -1,
     222,   220,    -1,   150,   220,   151,    -1,   263,    -1,   212,
      -1,   213,    -1,   322,    -1,   320,    -1,   321,    -1,   323,
      -1,   324,    -1,   305,    -1,   265,    -1,   228,    -1,   227,
      -1,   315,    -1,   277,    -1,   297,    -1,   326,    -1,   327,
      -1,   307,    -1,   308,    -1,   309,    -1,   325,    -1,   243,
      -1,   253,    -1,   255,    -1,   259,    -1,   257,    -1,   240,
      -1,   250,    -1,   236,    -1,   239,    -1,   300,    -1,   282,
      -1,   298,    -1,   285,    -1,   291,    -1,   316,    -1,   311,
      -1,   303,    -1,   264,    -1,   317,    -1,   270,    -1,   268,
      -1,   310,    -1,   314,    -1,   313,    -1,   301,    -1,   302,
      -1,   304,    -1,   296,    -1,   299,    -1,   162,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,   176,    -1,   178,    -1,   161,    -1,   163,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
      -1,   177,    -1,   223,    -1,   224,    -1,   155,    -1,   155,
      -1,   224,    -1,    94,   145,   223,    -1,   225,   203,   353,
      -1,   225,   203,   133,   145,   362,    -1,   225,   203,   352,
      -1,   225,   203,   369,   145,   346,    -1,   225,   203,   368,
     145,   348,    -1,   225,   203,   367,   145,   350,    -1,   225,
     203,   341,    -1,   225,   203,   344,    -1,   225,   203,   150,
     234,   151,    -1,   225,   203,   150,   233,   151,    -1,   225,
     203,   150,   232,   151,    -1,   225,   203,   369,   145,   150,
     231,   151,    -1,   225,   203,   368,   145,   150,   230,   151,
      -1,   225,   203,   367,   145,   150,   229,   151,    -1,   225,
     203,   150,   235,   151,    -1,   351,    -1,   229,   144,   351,
      -1,   349,    -1,   230,   144,   349,    -1,   347,    -1,   231,
     144,   347,    -1,   345,    -1,   232,   144,   345,    -1,   342,
      -1,   233,   144,   342,    -1,   353,    -1,   234,   144,   353,
      -1,   352,    -1,   235,   144,   352,    -1,    -1,    -1,   124,
     154,   146,   237,   357,   147,   238,   221,    -1,   154,   146,
     360,   147,    -1,    93,    -1,    -1,     6,    -1,     6,    -1,
     148,   353,   126,   353,   149,    -1,   148,   353,   149,    -1,
      -1,    -1,   100,   244,   148,   340,   149,   245,   150,   246,
     151,    -1,    -1,   247,   246,    -1,    -1,   101,   365,   145,
     248,   220,    -1,    -1,   103,   145,   249,   220,    -1,   102,
      -1,   204,   204,    -1,    99,    -1,    98,   226,   203,   353,
     251,   353,    -1,    -1,   252,   254,   221,    -1,    -1,    97,
     148,   340,   149,   256,   221,    -1,    -1,   242,   145,   258,
     222,    -1,    -1,    88,   242,   260,   261,    -1,   221,    -1,
      -1,   221,   262,    89,   221,    -1,    14,   145,   352,    -1,
      58,    -1,    58,   145,   365,    -1,    58,   145,    64,    -1,
      49,   145,   266,   144,   266,    -1,    49,   145,   266,   144,
     365,    -1,   146,     4,   144,    63,   144,   281,   147,    -1,
     329,   241,   144,   334,    -1,    -1,    79,   145,   267,   144,
     274,   144,   276,   330,   269,   221,    -1,    -1,    43,   145,
     267,   144,   273,   144,   275,   144,   276,   330,   271,   221,
      -1,    -1,   144,     5,    -1,   146,     4,   144,     4,   147,
      -1,    64,    -1,   146,     4,   144,     4,   147,    -1,    64,
      -1,   146,   283,   144,   284,   147,    -1,    64,    -1,   146,
       4,   144,     4,   147,    -1,    64,    -1,    87,   145,   278,
     144,   333,   144,   279,   144,   281,    -1,    25,   145,   333,
     144,   362,    -1,     5,    -1,    64,    -1,   280,    -1,    64,
      -1,    63,    -1,    63,   205,   280,    -1,     4,    -1,    64,
      -1,    20,    -1,    19,   145,   283,   144,   284,   272,   153,
      -1,    19,   145,   341,   272,   153,    -1,    68,    -1,    69,
      -1,    70,    -1,    69,    -1,    23,   145,   287,    -1,    -1,
      23,   145,   287,   286,   221,    -1,   348,   144,   341,   288,
      -1,    -1,   288,   144,   289,    -1,   352,    -1,    83,    -1,
      84,    -1,   336,    -1,    86,   352,    -1,   109,    -1,   107,
      -1,   111,    -1,   112,    -1,   113,    -1,   114,   145,   340,
      -1,   115,   145,   340,    -1,   116,   145,   340,    -1,   117,
      -1,   118,    -1,   119,   145,   290,    -1,   110,    -1,   152,
      -1,   120,    -1,   152,   205,   290,    -1,    21,   145,   293,
      -1,    -1,    22,   145,   293,   292,   221,    -1,   350,   294,
      -1,    -1,   294,   144,   295,    -1,    61,    -1,   121,   145,
     348,    -1,   366,    -1,    80,   145,   352,    -1,    95,   145,
     340,    -1,    96,    -1,    60,    -1,   104,   145,   340,    -1,
     123,    -1,    59,    -1,   105,    -1,   106,   145,   340,    -1,
     107,    -1,   108,    -1,   341,    -1,    24,   145,   328,   144,
     341,    -1,    26,   145,   341,   144,    63,   144,   333,    -1,
      30,   145,   341,   144,    63,    -1,    30,   145,   341,   144,
      63,   144,     5,   216,    -1,    31,    -1,    31,   145,   362,
      -1,    39,   145,   341,   144,    72,    -1,    40,   145,   341,
     144,    72,    -1,    40,   145,   371,   144,   371,   144,    72,
      -1,    44,   145,   371,   144,   371,   144,   152,    -1,    45,
     145,   371,   144,   371,   306,    -1,    46,   145,   371,   144,
     371,    -1,    -1,   144,    72,    -1,    52,   145,   362,   144,
      76,    -1,    53,   145,   362,    -1,    54,   145,   341,   144,
     146,   365,   144,   365,   147,   144,    74,   144,    73,    -1,
      56,   145,   362,    -1,    55,   145,   362,    -1,     3,    -1,
     146,     3,   144,   334,   147,    -1,    92,   145,   344,   144,
     346,   144,   346,   144,     7,    -1,    90,   145,   362,   144,
     346,    -1,    41,   145,   344,    -1,    42,   145,   344,    -1,
      -1,    32,   145,   344,   144,   334,   144,   329,   330,   318,
     319,    -1,    -1,   221,    -1,    36,   145,   341,   144,   335,
     144,   337,    -1,    37,   145,   341,    -1,    85,   145,   362,
     144,     4,    -1,    38,   145,    75,   144,   341,    -1,   122,
     145,   341,   144,   352,    -1,   122,   145,   341,   144,    64,
      -1,   122,   145,   341,    -1,    50,   145,   353,   144,   341,
      -1,    51,   145,   341,   144,   370,   144,   352,    -1,    48,
     145,   340,   144,   340,   144,   340,   144,   340,    -1,    48,
      -1,   152,    -1,    64,    -1,   152,    -1,    64,    -1,    -1,
     144,   331,    -1,   332,    -1,   332,   144,   331,    -1,    33,
      -1,    34,    -1,    35,    -1,    59,    -1,    64,    -1,    60,
      -1,    64,    -1,    67,    -1,   338,    -1,    64,    -1,    67,
      -1,   338,    -1,    66,   145,    64,    -1,    71,    -1,    64,
      -1,    66,   148,     4,   149,    -1,   152,    -1,   163,    -1,
     164,   148,   353,   149,    -1,   353,    -1,   342,    -1,   135,
     146,   362,   147,    -1,   167,    -1,   168,   148,   353,   149,
      -1,   146,     4,   144,     4,   147,    -1,    64,    -1,    65,
     343,   149,    -1,   143,    -1,   143,   144,   343,    -1,   345,
      -1,   169,    -1,   170,   148,   353,   149,    -1,   146,     4,
     144,     4,   144,     4,   144,     4,   147,    -1,   347,    -1,
     171,    -1,   172,   148,   353,   149,    -1,     3,    -1,   146,
       3,   144,   334,   147,    -1,   349,    -1,   173,    -1,   174,
     148,   353,   149,    -1,   152,    -1,     3,    -1,   146,     3,
     144,   152,   147,    -1,    64,    -1,   351,    -1,   175,    -1,
     176,   148,   353,   149,    -1,   152,    -1,     3,    -1,   146,
       3,   144,   152,   147,    -1,    64,    -1,    77,    -1,   339,
      -1,   352,   204,   339,    -1,     4,    -1,   363,    -1,   146,
       8,   147,    -1,   161,    -1,   162,   148,   353,   149,    -1,
     353,   198,   353,    -1,   353,   199,   353,    -1,   353,   200,
     353,    -1,   353,   201,   353,    -1,   353,   202,   353,    -1,
     146,   353,   147,    -1,   157,    -1,   158,    -1,   225,   145,
     354,    -1,   355,    -1,   356,   144,   355,    -1,    -1,   356,
      -1,   353,    -1,   352,    -1,   358,    -1,   359,   144,   358,
      -1,    -1,   359,    -1,   341,    -1,   128,   344,    -1,   129,
     344,    -1,   130,   341,   144,   341,    -1,   131,   341,   144,
     341,   144,   353,    -1,   132,   146,   362,   147,    -1,   132,
     146,   280,   144,   362,   147,    -1,   138,   146,     7,   144,
     362,   147,    -1,   138,   146,   362,   144,   362,   147,    -1,
     138,   146,   346,   144,   362,   147,    -1,   134,   341,    -1,
     136,   146,   341,   144,   353,   147,    -1,   136,   146,   341,
     144,   353,   144,    33,   147,    -1,   137,   146,   341,   144,
     353,   144,   353,   147,    -1,   137,   146,   341,   144,   353,
     144,   353,   144,    33,   147,    -1,   140,   146,   141,   144,
     146,   353,   199,   353,   214,   147,   144,   341,   215,   147,
      -1,   139,   361,    -1,   177,    -1,   146,   362,   147,    -1,
     361,    -1,   361,   206,   362,    -1,   197,    -1,    78,    -1,
       4,    -1,     8,    -1,     9,    -1,     4,    -1,     8,    -1,
       9,    -1,     4,    -1,   363,    -1,    27,    -1,    21,    -1,
      28,    -1,    23,    -1,    29,    -1,    90,    -1,    62,    -1,
      64,    -1,   372,    -1,    47,   146,     4,   144,     4,   144,
       4,   144,     4,   147,    -1,   146,     4,   144,     4,   144,
       4,   144,     4,   147,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   281,   281,   282,   285,   286,   289,   312,   317,   339,
     352,   364,   370,   399,   406,   409,   416,   420,   427,   430,
     437,   438,   442,   445,   452,   456,   463,   466,   472,   478,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   516,   517,   518,
     519,   520,   521,   522,   523,   524,   525,   526,   527,   530,
     531,   532,   533,   534,   535,   536,   537,   538,   541,   542,
     543,   544,   545,   546,   547,   548,   549,   552,   553,   554,
     557,   558,   561,   577,   583,   589,   595,   601,   607,   613,
     619,   625,   635,   645,   655,   665,   675,   685,   697,   702,
     709,   714,   721,   726,   733,   737,   743,   748,   755,   759,
     765,   769,   776,   798,   775,   812,   867,   874,   877,   883,
     890,   894,   903,   907,   902,   970,   971,   975,   974,   988,
     987,  1002,  1012,  1013,  1016,  1054,  1053,  1088,  1087,  1118,
    1117,  1150,  1149,  1175,  1186,  1185,  1213,  1219,  1224,  1229,
    1236,  1243,  1252,  1260,  1272,  1271,  1290,  1289,  1308,  1311,
    1317,  1327,  1333,  1342,  1348,  1353,  1359,  1364,  1370,  1381,
    1387,  1388,  1391,  1392,  1395,  1399,  1405,  1406,  1409,  1416,
    1424,  1432,  1433,  1436,  1437,  1440,  1445,  1444,  1458,  1465,
    1472,  1480,  1485,  1491,  1497,  1503,  1509,  1514,  1519,  1524,
    1529,  1534,  1539,  1544,  1549,  1554,  1559,  1565,  1572,  1581,
    1585,  1598,  1607,  1606,  1624,  1634,  1640,  1648,  1654,  1659,
    1664,  1669,  1674,  1679,  1684,  1689,  1694,  1705,  1711,  1716,
    1721,  1726,  1733,  1739,  1768,  1773,  1781,  1787,  1793,  1800,
    1807,  1817,  1827,  1842,  1853,  1856,  1862,  1868,  1874,  1880,
    1886,  1892,  1897,  1904,  1911,  1917,  1923,  1930,  1929,  1954,
    1957,  1963,  1970,  1976,  1982,  1988,  1992,  1997,  2004,  2010,
    2017,  2021,  2028,  2036,  2039,  2049,  2053,  2056,  2062,  2066,
    2073,  2077,  2081,  2087,  2088,  2091,  2092,  2095,  2096,  2097,
    2103,  2104,  2105,  2111,  2112,  2115,  2124,  2129,  2136,  2147,
    2153,  2157,  2161,  2168,  2178,  2185,  2189,  2195,  2199,  2207,
    2211,  2218,  2228,  2241,  2245,  2252,  2262,  2271,  2282,  2286,
    2293,  2303,  2314,  2323,  2333,  2339,  2343,  2350,  2360,  2371,
    2380,  2390,  2394,  2401,  2402,  2408,  2412,  2416,  2420,  2428,
    2437,  2441,  2445,  2449,  2453,  2457,  2460,  2467,  2476,  2509,
    2510,  2513,  2514,  2517,  2521,  2528,  2535,  2546,  2549,  2557,
    2561,  2565,  2569,  2573,  2578,  2582,  2586,  2591,  2596,  2601,
    2605,  2610,  2615,  2619,  2623,  2628,  2632,  2639,  2645,  2649,
    2655,  2662,  2663,  2666,  2667,  2668,  2671,  2675,  2679,  2683,
    2689,  2690,  2693,  2694,  2697,  2698,  2701,  2702,  2705,  2709,
    2735
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
  "OBJECT_ID", "COBJECT_ID", "MONSTER_ID", "TRAP_ID", "DOOR_ID",
  "DRAWBRIDGE_ID", "object_ID", "monster_ID", "terrain_ID", "MAZEWALK_ID",
  "WALLIFY_ID", "REGION_ID", "FILLING", "IRREGULAR", "JOINED", "ALTAR_ID",
  "ANVIL_ID", "NPC_ID", "LADDER_ID", "STAIR_ID", "NON_DIGGABLE_ID",
  "NON_PASSWALL_ID", "ROOM_ID", "PORTAL_ID", "TELEPRT_ID", "BRANCH_ID",
  "LEV", "MINERALIZE_ID", "CORRIDOR_ID", "GOLD_ID", "ENGRAVING_ID",
  "FOUNTAIN_ID", "THRONE_ID", "MODRON_PORTAL_ID", "POOL_ID", "SINK_ID",
  "NONE", "RAND_CORRIDOR_ID", "DOOR_STATE", "LIGHT_STATE", "CURSE_TYPE",
  "ENGRAVING_TYPE", "DIRECTION", "RANDOM_TYPE", "RANDOM_TYPE_BRACKET",
  "A_REGISTER", "ALIGNMENT", "LEFT_OR_RIGHT", "CENTER", "TOP_OR_BOT",
  "ALTAR_TYPE", "UP_OR_DOWN", "ACTIVE_OR_INACTIVE", "MODRON_PORTAL_TYPE",
  "NPC_TYPE", "FOUNTAIN_TYPE", "SPECIAL_OBJECT_TYPE", "CMAP_TYPE",
  "SUBROOM_ID", "NAME_ID", "FLAGS_ID", "FLAG_TYPE", "MON_ATTITUDE",
  "MON_ALERTNESS", "SUBTYPE_ID", "MON_APPEARANCE", "ROOMDOOR_ID", "IF_ID",
  "ELSE_ID", "TERRAIN_ID", "HORIZ_OR_VERT", "REPLACE_TERRAIN_ID",
  "EXIT_ID", "SHUFFLE_ID", "QUANTITY_ID", "BURIED_ID", "LOOP_ID", "FOR_ID",
  "TO_ID", "SWITCH_ID", "CASE_ID", "BREAK_ID", "DEFAULT_ID", "ERODED_ID",
  "TRAPPED_STATE", "RECHARGED_ID", "INVIS_ID", "GREASED_ID", "FEMALE_ID",
  "WAITFORU_ID", "CANCELLED_ID", "REVIVED_ID", "AVENGE_ID", "FLEEING_ID",
  "BLINDED_ID", "PARALYZED_ID", "STUNNED_ID", "CONFUSED_ID",
  "SEENTRAPS_ID", "ALL_ID", "MONTYPE_ID", "GRAVE_ID", "ERODEPROOF_ID",
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
  "level_def", "lev_init", "tileset_detail", "opt_limited",
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
  "region_detail_end", "altar_detail", "anvil_detail", "subtype_detail",
  "npc_detail", "grave_detail", "gold_detail", "engraving_detail",
  "mineralize", "trap_name", "room_type", "optroomregionflags",
  "roomregionflags", "roomregionflag", "door_state", "light_state",
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
     395,   396,   397,   398,    44,    58,    40,    41,    91,    93,
     123,   125,   399,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,    43,    45,
      42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   207,   208,   208,   209,   209,   210,   211,   211,   212,
     212,   212,   212,   213,   214,   214,   215,   215,   216,   216,
     217,   217,   218,   218,   219,   219,   220,   220,   221,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   225,   225,   225,
     226,   226,   227,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   229,   229,
     230,   230,   231,   231,   232,   232,   233,   233,   234,   234,
     235,   235,   237,   238,   236,   239,   240,   241,   241,   242,
     242,   242,   244,   245,   243,   246,   246,   248,   247,   249,
     247,   250,   251,   251,   252,   254,   253,   256,   255,   258,
     257,   260,   259,   261,   262,   261,   263,   264,   264,   264,
     265,   265,   266,   267,   269,   268,   271,   270,   272,   272,
     273,   273,   274,   274,   275,   275,   276,   276,   277,   277,
     278,   278,   279,   279,   280,   280,   281,   281,   282,   282,
     282,   283,   283,   284,   284,   285,   286,   285,   287,   288,
     288,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   290,   290,
     290,   291,   292,   291,   293,   294,   294,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   296,   297,   298,   298,   299,   299,   300,   301,
     302,   303,   304,   305,   306,   306,   307,   308,   309,   310,
     311,   312,   312,   313,   314,   315,   316,   318,   317,   319,
     319,   320,   321,   322,   323,   324,   324,   324,   325,   326,
     327,   327,   328,   328,   329,   329,   330,   330,   331,   331,
     332,   332,   332,   333,   333,   334,   334,   335,   335,   335,
     336,   336,   336,   337,   337,   338,   339,   339,   339,   340,
     341,   341,   341,   341,   342,   342,   342,   343,   343,   344,
     344,   344,   345,   346,   346,   346,   347,   347,   348,   348,
     348,   349,   349,   349,   349,   350,   350,   350,   351,   351,
     351,   351,   351,   352,   352,   353,   353,   353,   353,   353,
     353,   353,   353,   353,   353,   353,   354,   354,   355,   356,
     356,   357,   357,   358,   358,   359,   359,   360,   360,   361,
     361,   361,   361,   361,   361,   361,   361,   361,   361,   361,
     361,   361,   361,   361,   361,   361,   361,   361,   362,   362,
     363,   364,   364,   365,   365,   365,   366,   366,   366,   366,
     367,   367,   368,   368,   369,   369,   370,   370,   371,   371,
     372
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     3,     3,     5,     5,
       5,     3,    16,     3,     0,     2,     0,     2,     0,     2,
       1,     1,     0,     3,     3,     1,     0,     2,     3,     1,
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
       5,     5,     7,     4,     0,    10,     0,    12,     0,     2,
       5,     1,     5,     1,     5,     1,     5,     1,     9,     5,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     7,
       5,     1,     1,     1,     1,     3,     0,     5,     4,     0,
       3,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     3,     3,     3,     1,     1,     3,     1,     1,     1,
       3,     3,     0,     5,     2,     0,     3,     1,     3,     1,
       3,     3,     1,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     5,     7,     5,     8,     1,     3,     5,     5,
       7,     7,     6,     5,     0,     2,     5,     3,    13,     3,
       3,     1,     5,     9,     5,     3,     3,     0,    10,     0,
       1,     7,     3,     5,     5,     5,     5,     3,     5,     7,
       9,     1,     1,     1,     1,     1,     0,     2,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     4,     1,     1,     4,     1,
       1,     4,     1,     4,     5,     1,     3,     1,     3,     1,
       1,     4,     9,     1,     1,     4,     1,     5,     1,     1,
       4,     1,     1,     5,     1,     1,     1,     4,     1,     1,
       5,     1,     1,     1,     3,     1,     1,     3,     1,     4,
       3,     3,     3,     3,     3,     3,     1,     1,     3,     1,
       3,     0,     1,     1,     1,     1,     3,     0,     1,     1,
       2,     2,     4,     6,     4,     6,     6,     6,     6,     2,
       6,     8,     8,    10,    14,     2,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,    10,
       9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    22,     0,     0,     1,
       5,     0,    26,     0,     7,     0,   139,     0,     0,     0,
       0,   198,     0,     0,     0,     0,     0,     0,     0,   256,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   291,     0,     0,     0,     0,     0,     0,     0,
       0,   167,     0,     0,     0,     0,     0,     0,   136,     0,
       0,     0,   142,   151,     0,     0,     0,     0,    99,    88,
      79,    89,    80,    90,    81,    91,    82,    92,    83,    93,
      84,    94,    85,    95,    86,    96,    87,    30,    31,     6,
      26,    97,    98,     0,    40,    39,    57,    58,    55,     0,
      50,    56,   155,    51,    52,    54,    53,    29,    67,    38,
      70,    69,    42,    60,    62,    63,    77,    43,    61,    78,
      59,    74,    75,    66,    76,    37,    46,    47,    48,    71,
      65,    73,    72,    41,    64,    68,    33,    34,    32,    35,
      36,    49,    44,    45,     0,    25,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   161,     0,     0,     0,     0,   100,
     101,     0,     0,     0,     0,   355,     0,   358,     0,   400,
       0,   356,   377,    27,     0,   159,     0,     8,     0,   316,
     317,     0,   353,   166,     0,     0,     0,    11,   402,   401,
      13,   325,     0,   201,   202,     0,     0,   322,     0,     0,
     178,   320,   349,   351,   352,     0,   348,   346,     0,   231,
     235,   345,   232,   342,   344,     0,   341,   339,     0,   205,
       0,   338,   293,   292,     0,   303,   304,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   396,   379,   398,   257,     0,   330,     0,     0,
     329,     0,   282,     0,     0,     0,     0,     0,     0,   418,
     275,   276,   295,   294,     0,   137,     0,     0,     0,     0,
       0,   319,     0,     0,     0,     0,     0,   267,     0,   270,
     269,   405,   403,   404,   169,   168,     0,     0,   190,   191,
       0,     0,     0,     0,   102,     0,     0,     0,   287,   132,
       0,     0,     0,     0,   141,     0,     0,     0,     0,     0,
     374,   373,   375,   378,     0,   411,   413,   410,   412,   414,
     415,     0,     0,     0,   109,   110,   105,   103,     0,     0,
       0,     0,    26,   156,    24,     0,     0,     0,     0,     0,
     327,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     380,   381,     0,     0,     0,   389,     0,     0,     0,   395,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     163,   162,     0,     0,   157,     0,     0,     0,   371,   357,
     365,     0,     0,   360,   361,   362,   363,   364,     0,   135,
       0,   355,     0,     0,     0,     0,   126,   124,   130,   128,
       0,     0,     0,   160,     0,     0,   354,    10,   271,     0,
       9,     0,     0,   326,     0,     0,     0,   204,   203,   178,
     179,   200,     0,     0,     0,   233,     0,     0,   207,   209,
     252,   189,     0,   254,     0,     0,   194,     0,     0,     0,
       0,   336,     0,     0,   334,     0,     0,   333,     0,     0,
     397,   399,     0,     0,   305,   306,     0,   309,     0,   307,
       0,   308,   284,   258,     0,     0,   259,     0,   181,     0,
       0,     0,     0,     0,   264,   263,     0,     0,   170,   171,
     288,   416,   417,     0,   266,     0,   183,     0,     0,   283,
       0,     0,     0,   274,     0,     0,   153,     0,     0,   143,
     286,   285,     0,   369,   372,     0,   359,   140,   376,   104,
       0,     0,   113,     0,   112,     0,   111,     0,   117,     0,
     108,     0,   107,     0,   106,    28,   318,     0,     0,   328,
     321,     0,   323,     0,     0,   347,   408,   406,   407,   246,
     243,   237,     0,     0,   242,     0,   247,     0,   249,   250,
       0,   245,   236,   251,   409,   239,     0,   340,   208,     0,
       0,   382,     0,     0,     0,   384,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   331,     0,     0,     0,     0,
       0,     0,     0,     0,   173,     0,     0,     0,   262,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   158,   152,
     154,     0,     0,     0,   133,     0,   125,   127,   129,   131,
       0,   118,     0,   120,     0,   122,     0,     0,   324,   199,
     350,     0,     0,     0,     0,     0,   343,     0,   253,    18,
       0,   195,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   296,     0,   314,   313,   281,     0,     0,   260,
       0,   185,     0,     0,   261,   265,     0,     0,   289,     0,
       0,   187,     0,   296,   193,     0,   192,   165,     0,   145,
     366,   367,   368,   370,     0,     0,   116,     0,   115,     0,
     114,     0,     0,   240,   241,   244,   248,   238,     0,   310,
     212,   213,     0,   217,   216,   227,   218,   219,   220,     0,
       0,     0,   224,   225,     0,   210,   214,   311,   211,     0,
     255,   383,   385,     0,   390,     0,   386,     0,   335,   388,
     387,     0,     0,     0,   277,   315,     0,     0,     0,     0,
       0,     0,   196,   197,     0,     0,     0,     0,   174,     0,
       0,     0,     0,     0,   145,   134,   119,   121,   123,   272,
       0,     0,   215,     0,     0,     0,     0,    19,     0,     0,
     337,     0,     0,   300,   301,   302,   297,   298,   279,     0,
       0,   180,     0,   296,   290,   172,     0,   182,     0,     0,
     188,   273,     0,   149,   144,   146,     0,   312,   221,   222,
     223,   229,   228,   226,   391,     0,   392,   361,     0,     0,
     280,   278,     0,     0,     0,   176,     0,     0,   175,   147,
      26,     0,     0,     0,     0,     0,   332,   299,     0,   420,
     184,     0,     0,   186,    26,   150,     0,   230,   393,    15,
       0,   419,   177,     0,   148,     0,     0,   268,     0,    16,
      20,    21,    18,     0,     0,    12,    17,   394
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    87,    88,   875,   904,   770,
     902,    12,   146,    89,   363,    90,    91,    92,    93,   191,
      94,    95,   670,   672,   674,   452,   453,   454,   455,    96,
     438,   734,    97,    98,   415,    99,   100,   192,   661,   803,
     804,   884,   870,   101,   558,   102,   103,   206,   104,   555,
     105,   361,   106,   321,   431,   551,   107,   108,   109,   303,
     294,   110,   839,   111,   881,   377,   530,   548,   713,   723,
     112,   320,   725,   497,   794,   113,   229,   479,   114,   384,
     249,   618,   765,   853,   115,   381,   239,   380,   612,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   648,
     126,   127,   128,   129,   130,   470,   131,   132,   133,   134,
     135,   828,   861,   136,   137,   138,   139,   140,   141,   142,
     143,   254,   295,   784,   826,   827,   257,   516,   520,   766,
     706,   521,   212,   300,   273,   231,   371,   279,   280,   506,
     507,   250,   251,   240,   241,   340,   301,   732,   563,   564,
     565,   342,   343,   344,   274,   401,   201,   220,   315,   615,
     358,   359,   360,   543,   288,   289
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -683
static const yytype_int16 yypact[] =
{
      70,   -87,   -51,   112,  -683,    70,    49,   -28,     8,  -683,
    -683,    -6,   747,    32,  -683,    80,  -683,    57,   118,   124,
     138,  -683,   159,   162,   166,   170,   173,   201,   203,   218,
     223,   224,   226,   234,   237,   247,   261,   262,   266,   270,
     278,   280,   284,   285,   290,   299,   302,   307,   308,   314,
     324,   325,   328,   329,   330,    29,   332,   333,  -683,   336,
      42,   771,  -683,  -683,   337,    12,    13,   174,  -683,  -683,
    -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,
    -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,
     747,  -683,  -683,    -7,  -683,  -683,  -683,  -683,  -683,   347,
    -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,
    -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,
    -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,
    -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,
    -683,  -683,  -683,  -683,   362,   184,  -683,   -63,   348,    41,
     344,   191,   191,   258,    -2,   158,   -13,   -13,   326,    97,
     -13,   -13,   298,   -13,   259,    97,    97,     1,     0,     0,
       0,    13,   257,    13,   -13,   326,   326,   -13,   326,   326,
      62,     1,   326,    83,  -683,   326,    97,   765,    13,  -683,
    -683,   180,   297,   -13,   282,  -683,    45,  -683,   320,  -683,
     238,  -683,   133,  -683,    51,  -683,   321,  -683,    80,  -683,
    -683,   343,  -683,   293,   354,   355,   360,  -683,  -683,  -683,
    -683,  -683,   363,  -683,  -683,   359,   524,  -683,   381,   387,
     389,  -683,  -683,  -683,  -683,   527,  -683,  -683,   388,  -683,
    -683,  -683,  -683,  -683,  -683,   531,  -683,  -683,   403,   385,
     408,  -683,  -683,  -683,   411,  -683,  -683,   412,   415,   418,
      97,    97,   -13,   -13,   424,   -13,   426,   432,   457,   326,
     458,   523,  -683,  -683,   365,  -683,   607,  -683,   471,   476,
    -683,   477,  -683,   485,   486,   488,   632,   493,   495,  -683,
    -683,  -683,  -683,  -683,   504,   643,   646,   512,   540,   541,
     543,   340,   685,   548,   416,   555,   557,  -683,   558,  -683,
    -683,  -683,  -683,  -683,  -683,  -683,   559,   560,  -683,  -683,
     561,   321,   562,   563,  -683,   565,    13,    13,   564,  -683,
     568,   287,    13,    13,  -683,    13,    13,    13,    13,    13,
     293,   340,  -683,   567,   569,  -683,  -683,  -683,  -683,  -683,
    -683,   572,   119,    19,  -683,  -683,   293,   340,   573,   575,
     576,   747,   747,  -683,  -683,    13,   -63,   709,    22,   719,
     579,   580,   326,   586,    13,   231,   726,   581,   588,    13,
     589,   321,   591,    13,   321,   -13,   -13,   326,   673,   684,
    -683,  -683,   604,   605,   542,  -683,   -13,   -13,   386,  -683,
     609,   616,   326,   610,    13,    44,   274,   -13,   680,   751,
     612,   688,     0,   -10,  -683,   630,   631,     0,     0,     0,
      13,   637,    59,   -13,    94,   700,   636,    -8,   812,   158,
     728,  -683,    21,    21,  -683,   220,   669,   -20,   711,  -683,
    -683,   198,   346,   175,   175,  -683,  -683,  -683,   133,  -683,
     326,   675,   -53,   -49,   -48,   -45,  -683,  -683,   293,   340,
      84,   228,    54,  -683,   670,   496,  -683,  -683,  -683,   817,
    -683,   678,   363,  -683,   676,   820,   526,  -683,  -683,   389,
    -683,  -683,   677,   544,   249,  -683,   679,   608,  -683,  -683,
    -683,  -683,   681,   683,   -13,   -13,   623,   686,   689,   694,
     698,  -683,   699,   445,  -683,   702,   704,  -683,   707,   708,
    -683,  -683,   829,   613,  -683,  -683,   714,  -683,   712,  -683,
     715,  -683,  -683,  -683,   717,   842,  -683,   718,  -683,   859,
     720,    44,   861,   723,   724,  -683,   746,   807,  -683,  -683,
    -683,  -683,  -683,   748,  -683,   323,  -683,   887,   749,  -683,
     750,   808,   893,  -683,   754,   321,  -683,   695,    13,  -683,
    -683,   293,   755,  -683,   759,   757,  -683,  -683,  -683,  -683,
     901,   760,  -683,    11,  -683,    13,  -683,   -63,  -683,    33,
    -683,    43,  -683,    23,  -683,  -683,  -683,   763,   925,  -683,
    -683,   783,  -683,   792,   800,  -683,  -683,  -683,  -683,  -683,
    -683,  -683,   804,   805,  -683,   806,  -683,   809,  -683,  -683,
     810,  -683,  -683,  -683,  -683,  -683,   811,  -683,   813,   158,
     947,  -683,   815,   890,   326,  -683,    13,    13,   326,   816,
      13,   326,   326,   818,   819,  -683,     1,   952,    50,   957,
      26,   894,   821,    17,  -683,   823,   822,   896,  -683,    13,
     825,   -63,   826,   827,    18,   164,   321,    21,  -683,  -683,
     340,   828,   168,   711,  -683,    64,  -683,  -683,   340,   293,
     -16,  -683,   105,  -683,   154,  -683,    44,   831,  -683,  -683,
    -683,   -63,    13,    13,    13,   258,  -683,   483,  -683,   832,
      13,  -683,   830,   442,   466,   833,    44,   655,   834,   835,
      13,   958,   839,   824,  -683,  -683,  -683,   840,   968,  -683,
     975,  -683,   283,   841,  -683,  -683,   843,    65,   293,   323,
     982,  -683,   984,   839,  -683,   845,  -683,  -683,   846,   183,
    -683,  -683,  -683,  -683,   321,    33,  -683,    43,  -683,    23,
    -683,   844,   987,   293,  -683,  -683,  -683,  -683,    99,  -683,
    -683,  -683,   -63,  -683,  -683,  -683,  -683,  -683,  -683,   848,
     849,   850,  -683,  -683,   851,  -683,  -683,  -683,   293,   994,
    -683,   340,  -683,   965,  -683,    13,  -683,   852,  -683,  -683,
    -683,   425,   856,   300,  -683,  -683,   997,   858,   857,   862,
      18,    13,  -683,  -683,   860,   863,   864,   865,  -683,    65,
     996,   323,   867,   854,   183,  -683,  -683,  -683,  -683,  -683,
     869,   944,   293,    13,    13,    13,   -47,  -683,   868,   539,
    -683,    13,  1010,  -683,  -683,  -683,  -683,   872,   321,   873,
    1014,  -683,   231,   839,  -683,  -683,   875,  -683,  1016,   321,
    -683,  -683,   876,  -683,  -683,  -683,  1017,  -683,  -683,  -683,
    -683,  -683,   836,  -683,  -683,   990,  -683,   241,   877,   300,
    -683,  -683,  1021,   879,   880,  -683,   954,   882,  -683,  -683,
     747,   886,   -47,   884,   891,   885,  -683,  -683,   888,  -683,
    -683,   321,   892,  -683,   747,  -683,    44,  -683,  -683,  -683,
     895,  -683,  -683,   961,  -683,   898,   -13,  -683,    85,   899,
    -683,  -683,   832,   -13,   897,  -683,  -683,  -683
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -683,  -683,  1032,  -683,  -683,  -683,  -683,  -683,  -683,   136,
    -683,  -683,   837,   -90,  -320,   687,   853,   985,  -430,  -683,
    -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,
    -683,  -683,  -683,  -683,  -683,   992,  -683,  -683,  -683,   245,
    -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,
    -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,   628,
     870,  -683,  -683,  -683,  -683,   574,  -683,  -683,  -683,   264,
    -683,  -683,  -683,  -546,   253,  -683,   345,   227,  -683,  -683,
    -683,  -683,  -683,   186,  -683,  -683,   903,  -683,  -683,  -683,
    -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,
    -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,
    -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,
    -683,  -683,   420,  -682,   202,  -683,  -396,  -524,  -683,  -683,
    -683,   373,   696,  -182,  -145,  -344,   592,   -73,  -339,  -412,
    -561,  -448,  -539,   603,  -536,  -144,   -62,  -683,   402,  -683,
    -683,   618,  -683,  -683,   798,  -148,   584,  -683,  -420,  -683,
    -683,  -683,  -683,  -683,  -130,  -683
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -207
static const yytype_int16 yytable[] =
{
     203,   430,   539,   213,   200,   230,   325,   644,   562,   456,
     275,   258,   259,   582,   457,   281,   282,   195,   284,   287,
     553,   554,   675,   195,   501,   468,   501,   306,   307,   305,
     309,   310,   308,   550,   317,    16,   232,   322,   297,   298,
     299,   798,   673,   671,   560,   218,   243,   285,   328,   195,
     584,   221,   222,   330,   528,   195,   546,   501,     7,   354,
     356,   485,   252,   311,   488,   292,   311,   312,   313,   792,
     312,   313,   345,   851,   346,   221,   222,   691,   347,   348,
     349,   711,   721,   221,   222,     1,     2,   232,   318,   209,
     900,   571,   290,   291,     8,   573,   575,   233,   572,   577,
     210,   211,   574,   576,   514,   852,   578,   244,   515,   726,
     234,   304,     9,   323,   704,   221,   222,   392,   393,   219,
     395,   705,   225,   451,    13,   652,   314,   330,   735,   793,
      11,   355,   209,   226,   331,   736,   529,   195,   547,    15,
     341,   350,   357,   210,   211,   436,   296,   319,   233,   901,
     253,   865,   741,   293,   227,   228,   541,   226,   542,   196,
      14,   234,   145,   712,   722,   352,   194,   552,   469,   552,
     708,   209,   777,   678,   197,   198,   144,    66,   808,   235,
     197,   198,   210,   211,   351,   236,   225,   390,   391,   245,
     188,   196,   504,   505,   232,   246,   204,   352,   807,   806,
     552,   353,   147,   209,   583,   302,   197,   198,   701,   458,
     199,   678,   197,   198,   210,   211,   199,   255,   227,   228,
     277,   278,   256,   688,   474,   504,   505,   496,   724,   667,
     235,   243,   666,   562,   579,   658,   236,   747,   536,   491,
     489,   490,   199,   276,   811,   728,   498,   637,   199,   737,
     508,   499,   500,   596,   511,   233,   738,   597,   598,   237,
     238,   243,   522,   148,   435,   196,   277,   278,   234,   149,
     441,   442,   464,   443,   444,   445,   446,   447,   540,   196,
     197,   198,   527,   150,   801,   209,   802,   533,   534,   535,
     331,   459,   244,   561,   197,   198,   210,   211,   739,   795,
     477,   478,   569,   465,   151,   740,   285,   152,   599,   600,
     601,   153,   476,   221,   222,   154,   199,   483,   155,   556,
     202,   487,   244,   221,   222,   730,   731,   311,   208,   602,
     199,   312,   313,   823,   824,   825,   727,   235,   517,   613,
     518,   519,   513,   236,   603,   604,   156,   566,   157,   621,
     622,   223,   224,   605,   606,   607,   608,   609,   214,   215,
     216,   217,   895,   158,   333,   207,   237,   238,   159,   160,
     610,   161,   611,   283,   245,   337,   338,   339,   581,   162,
     246,   842,   163,   326,   225,   874,   341,   334,   -14,   501,
     221,   222,   164,   502,   225,   226,   335,   336,   337,   338,
     339,   247,   248,   302,   245,   286,   165,   166,   221,   222,
     246,   167,   223,   224,   805,   168,   227,   228,   335,   336,
     337,   338,   339,   169,   557,   170,   227,   228,   329,   171,
     172,   247,   248,   669,   440,   173,   335,   336,   337,   338,
     339,   337,   338,   339,   174,   327,   199,   175,   629,   373,
     221,   222,   176,   177,   260,   261,   262,   263,   264,   178,
     265,   225,   266,   267,   268,   269,   270,   716,   332,   179,
     180,   362,   271,   181,   182,   183,   692,   185,   186,   225,
     695,   187,   193,   698,   699,   335,   336,   337,   338,   339,
     226,   365,   205,   227,   228,   567,   660,   366,   367,   368,
     744,   745,   746,   272,   369,   372,   370,   718,   860,   221,
     222,   227,   228,   668,   260,   261,   262,   263,   264,   868,
     265,   225,   266,   267,   268,   269,   270,   373,   373,   374,
     378,   375,   503,   376,   382,  -206,   379,   743,   335,   336,
     337,   338,   339,   768,   335,   336,   337,   338,   339,   748,
     749,   383,   385,   227,   228,   386,   387,   504,   505,   388,
     423,   892,   389,   272,   693,   694,   750,   751,   697,   752,
     394,   402,   396,   260,   261,   262,   263,   264,   397,   265,
     225,   266,   267,   268,   269,   270,   773,   221,   222,   774,
     753,   271,   754,   755,   756,   757,   758,   759,   760,   761,
     762,   763,   764,   398,   400,   496,   221,   222,   812,   834,
     775,   403,   227,   228,   335,   336,   337,   338,   339,   404,
     405,   406,   272,   335,   821,   337,   338,   339,   771,   407,
     408,   848,   849,   850,   409,   209,   410,   411,   781,   412,
     335,   336,   337,   338,   339,   586,   210,   211,   413,   414,
     416,   260,   261,   262,   263,   264,   417,   265,   225,   266,
     267,   268,   269,   270,   335,   336,   337,   338,   339,   271,
     260,   261,   262,   263,   264,   592,   265,   225,   266,   267,
     268,   269,   270,   855,   418,   419,   856,   420,   271,   421,
     227,   228,   422,   595,   335,   336,   337,   338,   339,   424,
     272,   425,   426,   427,   428,   429,   432,   433,   437,   227,
     228,   448,   467,   819,   434,   439,   449,   450,   460,   272,
     461,   462,   471,   472,   335,   336,   337,   338,   339,   473,
     475,   480,   482,   484,   481,   486,   492,   335,   336,   337,
     338,   339,   335,   336,   337,   338,   339,   493,   494,   495,
     509,   899,   523,    16,   512,   524,   525,   617,   906,   857,
     526,    17,   635,   510,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,   531,   532,   544,    28,    29,    30,
     885,   537,   545,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,   894,    42,    43,    44,    45,    46,
      47,    48,    49,    50,   778,    51,   335,   336,   337,   338,
     339,   335,   336,   337,   338,   339,   549,  -164,   559,   570,
     587,   585,   588,   590,   591,   619,    52,   620,   623,   594,
     624,   616,    53,   634,    54,    55,   625,    56,   626,    57,
      58,    59,   627,   628,    60,    61,   640,    62,   631,    63,
     630,   632,   633,   335,   336,   337,   338,   339,   636,   638,
     637,   639,   641,   642,   643,   645,    68,   646,   647,    64,
     650,    65,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
     649,   653,   651,   654,   655,    66,   629,   656,   657,   659,
     662,    67,    68,   663,   664,   665,   276,   676,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,   189,    70,   677,    72,
     678,    74,    69,    76,    71,    78,    73,    80,    75,    82,
      77,    84,    79,    86,    81,   679,    83,   680,    85,   681,
     682,   683,   689,   496,   684,   685,   703,   687,   686,   690,
     696,   707,   782,   701,   700,   710,   709,   708,   715,   717,
     719,   720,   787,   785,   714,   742,   769,   772,   729,   788,
     776,   779,   780,   783,   786,   790,   796,   791,   797,   799,
     800,   809,   810,   813,   814,   815,   816,   817,   818,   820,
     822,   829,   830,   841,   831,   844,   832,   835,   847,   838,
     836,   837,   843,   846,   858,   854,   859,   862,   863,   866,
     867,   869,   871,   873,   876,   878,   879,   880,   882,   883,
     886,   888,   890,   889,   897,   891,   893,    10,   905,   896,
     324,   872,   898,   903,   907,   364,   190,   184,   463,   845,
     538,   316,   840,   593,   833,   242,   702,   789,   887,   864,
     767,   877,   466,   580,   589,   733,   568,   399,   614
};

static const yytype_uint16 yycheck[] =
{
      90,   321,   422,   147,    66,   150,   188,   531,   438,   353,
     158,   156,   157,   461,   353,   160,   161,     4,   163,   164,
     432,   433,   583,     4,     3,     3,     3,   175,   176,   174,
     178,   179,   177,   429,   182,     6,     3,   185,   168,   169,
     170,   723,   581,   579,    64,     4,     3,    47,   193,     4,
     462,    64,    65,     8,    64,     4,    64,     3,   145,   204,
     204,   381,    64,     4,   384,    64,     4,     8,     9,     4,
       8,     9,    21,   120,    23,    64,    65,   623,    27,    28,
      29,    64,    64,    64,    65,    15,    16,     3,     5,   152,
       5,   144,   165,   166,   145,   144,   144,    64,   151,   144,
     163,   164,   151,   151,    60,   152,   151,    64,    64,   655,
      77,   173,     0,   186,    64,    64,    65,   262,   263,    78,
     265,    71,   135,     4,   152,   545,    64,     8,   144,    64,
      81,   204,   152,   146,   196,   151,   146,     4,   146,   145,
     202,    90,   204,   163,   164,   327,   146,    64,    64,    64,
     152,   833,   676,   152,   167,   168,    62,   146,    64,   146,
     152,    77,    82,   146,   146,   146,   154,   146,   146,   146,
     144,   152,   696,   147,   161,   162,   144,   148,   739,   146,
     161,   162,   163,   164,   133,   152,   135,   260,   261,   146,
     148,   146,   171,   172,     3,   152,   203,   146,   737,   735,
     146,   150,   145,   152,   150,   146,   161,   162,   144,   353,
     197,   147,   161,   162,   163,   164,   197,    59,   167,   168,
     169,   170,    64,   619,   372,   171,   172,    63,    64,   573,
     146,     3,   571,   663,   150,   555,   152,   685,   420,   387,
     385,   386,   197,   146,   145,   657,   394,   148,   197,   144,
     398,   396,   397,     4,   402,    64,   151,     8,     9,   175,
     176,     3,   407,   145,   326,   146,   169,   170,    77,   145,
     332,   333,   362,   335,   336,   337,   338,   339,   423,   146,
     161,   162,   412,   145,   101,   152,   103,   417,   418,   419,
     352,   353,    64,   437,   161,   162,   163,   164,   144,   719,
      69,    70,   450,   365,   145,   151,    47,   145,    59,    60,
      61,   145,   374,    64,    65,   145,   197,   379,   145,    99,
     146,   383,    64,    64,    65,   157,   158,     4,   144,    80,
     197,     8,     9,    33,    34,    35,   656,   146,    64,   484,
      66,    67,   404,   152,    95,    96,   145,   149,   145,   494,
     495,    68,    69,   104,   105,   106,   107,   108,    10,    11,
      12,    13,   886,   145,   126,     3,   175,   176,   145,   145,
     121,   145,   123,    75,   146,   200,   201,   202,   150,   145,
     152,   801,   145,   203,   135,   144,   448,   149,   147,     3,
      64,    65,   145,     7,   135,   146,   198,   199,   200,   201,
     202,   173,   174,   146,   146,   146,   145,   145,    64,    65,
     152,   145,    68,    69,   734,   145,   167,   168,   198,   199,
     200,   201,   202,   145,   204,   145,   167,   168,   146,   145,
     145,   173,   174,   577,   147,   145,   198,   199,   200,   201,
     202,   200,   201,   202,   145,   148,   197,   145,     3,     4,
      64,    65,   145,   145,   128,   129,   130,   131,   132,   145,
     134,   135,   136,   137,   138,   139,   140,   649,   148,   145,
     145,   150,   146,   145,   145,   145,   624,   145,   145,   135,
     628,   145,   145,   631,   632,   198,   199,   200,   201,   202,
     146,   148,   145,   167,   168,   149,   558,   204,   144,   144,
     682,   683,   684,   177,   144,   146,   143,   651,   828,    64,
      65,   167,   168,   575,   128,   129,   130,   131,   132,   839,
     134,   135,   136,   137,   138,   139,   140,     4,     4,   148,
       3,   144,   146,   144,     3,   150,   148,   681,   198,   199,
     200,   201,   202,   687,   198,   199,   200,   201,   202,    66,
      67,   148,   144,   167,   168,   144,   144,   171,   172,   144,
     144,   881,   144,   177,   626,   627,    83,    84,   630,    86,
     146,   206,   146,   128,   129,   130,   131,   132,   146,   134,
     135,   136,   137,   138,   139,   140,   144,    64,    65,   147,
     107,   146,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   146,   146,    63,    64,    65,   752,   791,
     144,     4,   167,   168,   198,   199,   200,   201,   202,   148,
     144,   144,   177,   198,   199,   200,   201,   202,   690,   144,
     144,   813,   814,   815,   146,   152,     4,   144,   700,   144,
     198,   199,   200,   201,   202,   149,   163,   164,   144,     6,
       4,   128,   129,   130,   131,   132,   144,   134,   135,   136,
     137,   138,   139,   140,   198,   199,   200,   201,   202,   146,
     128,   129,   130,   131,   132,   149,   134,   135,   136,   137,
     138,   139,   140,   144,   144,   144,   147,   144,   146,     4,
     167,   168,   144,   149,   198,   199,   200,   201,   202,   144,
     177,   144,   144,   144,   144,   144,   144,   144,   144,   167,
     168,   144,     3,   775,   149,   147,   147,   145,   145,   177,
     145,   145,     3,   144,   198,   199,   200,   201,   202,   149,
     144,     5,   144,   144,   153,   144,    63,   198,   199,   200,
     201,   202,   198,   199,   200,   201,   202,    63,   144,   144,
     141,   896,    72,     6,   144,     4,   144,   149,   903,   821,
      72,    14,   149,   147,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,   144,   144,    76,    30,    31,    32,
     870,   144,   146,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,   884,    48,    49,    50,    51,    52,
      53,    54,    55,    56,   149,    58,   198,   199,   200,   201,
     202,   198,   199,   200,   201,   202,     4,    89,   149,   144,
       3,   151,   144,   147,     4,   144,    79,   144,   205,   152,
     144,   152,    85,     4,    87,    88,   147,    90,   144,    92,
      93,    94,   144,   144,    97,    98,     4,   100,   144,   102,
     148,   144,   144,   198,   199,   200,   201,   202,   144,   144,
     148,   144,   144,     4,   144,     4,   155,   144,   144,   122,
      63,   124,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     144,     4,   144,   144,   144,   148,     3,    89,   144,   204,
     145,   154,   155,   144,   147,     4,   146,   144,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   155,   162,     3,   164,
     147,   166,   161,   168,   163,   170,   165,   172,   167,   174,
     169,   176,   171,   178,   173,   153,   175,   147,   177,   145,
     145,   145,     5,    63,   145,   145,     4,   144,   147,   144,
     144,     4,     4,   144,   146,   144,    72,   144,    72,   144,
     144,   144,     4,   149,   152,   144,   144,   147,   150,     4,
     147,   147,   147,   144,   144,   144,     4,   144,     4,   144,
     144,   147,     5,   145,   145,   145,   145,     3,    33,   147,
     144,     4,   144,     7,   147,   151,   144,   147,    64,   144,
     147,   147,   145,   144,     4,   147,   144,   144,     4,   144,
       4,   145,     5,    33,   147,     4,   147,   147,    74,   147,
     144,   147,   147,   142,    73,   147,   144,     5,   902,   144,
     187,   205,   144,   144,   147,   208,    61,    55,   361,   804,
     422,   181,   799,   479,   790,   152,   636,   712,   872,   832,
     687,   859,   366,   460,   472,   663,   448,   269,   484
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   208,   209,   210,   211,   145,   145,     0,
     209,    81,   218,   152,   152,   145,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    30,    31,
      32,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    58,    79,    85,    87,    88,    90,    92,    93,    94,
      97,    98,   100,   102,   122,   124,   148,   154,   155,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   212,   213,   220,
     222,   223,   224,   225,   227,   228,   236,   239,   240,   242,
     243,   250,   252,   253,   255,   257,   259,   263,   264,   265,
     268,   270,   277,   282,   285,   291,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   307,   308,   309,   310,
     311,   313,   314,   315,   316,   317,   320,   321,   322,   323,
     324,   325,   326,   327,   144,    82,   219,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   242,   145,   145,   145,   148,   155,
     224,   226,   244,   145,   154,     4,   146,   161,   162,   197,
     353,   363,   146,   220,   203,   145,   254,     3,   144,   152,
     163,   164,   339,   352,    10,    11,    12,    13,     4,    78,
     364,    64,    65,    68,    69,   135,   146,   167,   168,   283,
     341,   342,     3,    64,    77,   146,   152,   175,   176,   293,
     350,   351,   293,     3,    64,   146,   152,   173,   174,   287,
     348,   349,    64,   152,   328,    59,    64,   333,   341,   341,
     128,   129,   130,   131,   132,   134,   136,   137,   138,   139,
     140,   146,   177,   341,   361,   362,   146,   169,   170,   344,
     345,   341,   341,    75,   341,    47,   146,   341,   371,   372,
     344,   344,    64,   152,   267,   329,   146,   371,   371,   371,
     340,   353,   146,   266,   353,   341,   362,   362,   341,   362,
     362,     4,     8,     9,    64,   365,   267,   362,     5,    64,
     278,   260,   362,   344,   223,   340,   203,   148,   341,   146,
       8,   353,   148,   126,   149,   198,   199,   200,   201,   202,
     352,   353,   358,   359,   360,    21,    23,    27,    28,    29,
      90,   133,   146,   150,   341,   344,   352,   353,   367,   368,
     369,   258,   150,   221,   219,   148,   204,   144,   144,   144,
     143,   343,   146,     4,   148,   144,   144,   272,     3,   148,
     294,   292,     3,   148,   286,   144,   144,   144,   144,   144,
     344,   344,   341,   341,   146,   341,   146,   146,   146,   361,
     146,   362,   206,     4,   148,   144,   144,   144,   144,   146,
       4,   144,   144,   144,     6,   241,     4,   144,   144,   144,
     144,     4,   144,   144,   144,   144,   144,   144,   144,   144,
     221,   261,   144,   144,   149,   353,   340,   144,   237,   147,
     147,   353,   353,   353,   353,   353,   353,   353,   144,   147,
     145,     4,   232,   233,   234,   235,   342,   345,   352,   353,
     145,   145,   145,   222,   220,   353,   339,     3,     3,   146,
     312,     3,   144,   149,   362,   144,   353,    69,    70,   284,
       5,   153,   144,   353,   144,   221,   144,   353,   221,   341,
     341,   362,    63,    63,   144,   144,    63,   280,   362,   341,
     341,     3,     7,   146,   171,   172,   346,   347,   362,   141,
     147,   362,   144,   353,    60,    64,   334,    64,    66,    67,
     335,   338,   341,    72,     4,   144,    72,   371,    64,   146,
     273,   144,   144,   371,   371,   371,   340,   144,   266,   365,
     341,    62,    64,   370,    76,   146,    64,   146,   274,     4,
     333,   262,   146,   346,   346,   256,    99,   204,   251,   149,
      64,   352,   225,   355,   356,   357,   149,   149,   358,   362,
     144,   144,   151,   144,   151,   144,   151,   144,   151,   150,
     350,   150,   348,   150,   346,   151,   149,     3,   144,   343,
     147,     4,   149,   272,   152,   149,     4,     8,     9,    59,
      60,    61,    80,    95,    96,   104,   105,   106,   107,   108,
     121,   123,   295,   341,   363,   366,   152,   149,   288,   144,
     144,   341,   341,   205,   144,   147,   144,   144,   144,     3,
     148,   144,   144,   144,     4,   149,   144,   148,   144,   144,
       4,   144,     4,   144,   334,     4,   144,   144,   306,   144,
      63,   144,   365,     4,   144,   144,    89,   144,   221,   204,
     353,   245,   145,   144,   147,     4,   345,   342,   353,   352,
     229,   351,   230,   349,   231,   347,   144,     3,   147,   153,
     147,   145,   145,   145,   145,   145,   147,   144,   333,     5,
     144,   280,   362,   353,   353,   362,   144,   353,   362,   362,
     146,   144,   329,     4,    64,    71,   337,     4,   144,    72,
     144,    64,   146,   275,   152,    72,   340,   144,   352,   144,
     144,    64,   146,   276,    64,   279,   280,   221,   346,   150,
     157,   158,   354,   355,   238,   144,   151,   144,   151,   144,
     151,   334,   144,   352,   340,   340,   340,   348,    66,    67,
      83,    84,    86,   107,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   289,   336,   338,   352,   144,
     216,   353,   147,   144,   147,   144,   147,   334,   149,   147,
     147,   353,     4,   144,   330,   149,   144,     4,     4,   283,
     144,   144,     4,    64,   281,   365,     4,     4,   330,   144,
     144,   101,   103,   246,   247,   221,   351,   349,   347,   147,
       5,   145,   352,   145,   145,   145,   145,     3,    33,   353,
     147,   199,   144,    33,    34,    35,   331,   332,   318,     4,
     144,   147,   144,   276,   340,   147,   147,   147,   144,   269,
     281,     7,   365,   145,   151,   246,   144,    64,   340,   340,
     340,   120,   152,   290,   147,   144,   147,   353,     4,   144,
     221,   319,   144,     4,   284,   330,   144,     4,   221,   145,
     249,     5,   205,    33,   144,   214,   147,   331,     4,   147,
     147,   271,    74,   147,   248,   220,   144,   290,   147,   142,
     147,   147,   221,   144,   220,   334,   144,    73,   144,   341,
       5,    64,   217,   144,   215,   216,   341,   147
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
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_TILESET));
		  ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 406 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 410 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 416 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_COPY));
		      (yyval.i) = 0;
		  ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 421 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 427 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 431 "lev_comp.y"
    {
		      (yyval.i) = what_map_char((char) (yyvsp[(2) - (2)].i));
		  ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 442 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 446 "lev_comp.y"
    {
		      add_opvars(splev, "io",
                                 VA_PASS2((int) (yyvsp[(3) - (3)].i), SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 453 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 457 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 463 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 467 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 473 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (3)].i);
		  ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 562 "lev_comp.y"
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

  case 103:

/* Line 1455 of yacc.c  */
#line 578 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 584 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 590 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 596 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 602 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 608 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 614 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 620 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 626 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 636 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 646 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 656 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 666 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 676 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 686 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 698 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 703 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 710 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 715 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 722 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 727 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 734 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 738 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 744 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 749 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 756 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 760 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 766 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 770 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 776 "lev_comp.y"
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

  case 133:

/* Line 1455 of yacc.c  */
#line 798 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 802 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 813 "lev_comp.y"
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

  case 136:

/* Line 1455 of yacc.c  */
#line 868 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 874 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 878 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 884 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 891 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 895 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 903 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 907 "lev_comp.y"
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

  case 144:

/* Line 1455 of yacc.c  */
#line 930 "lev_comp.y"
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

  case 147:

/* Line 1455 of yacc.c  */
#line 975 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 985 "lev_comp.y"
    {
		  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 988 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 998 "lev_comp.y"
    {
		  ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 1003 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1017 "lev_comp.y"
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

  case 155:

/* Line 1455 of yacc.c  */
#line 1054 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1059 "lev_comp.y"
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

  case 157:

/* Line 1455 of yacc.c  */
#line 1088 "lev_comp.y"
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

  case 158:

/* Line 1455 of yacc.c  */
#line 1102 "lev_comp.y"
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

  case 159:

/* Line 1455 of yacc.c  */
#line 1118 "lev_comp.y"
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

  case 160:

/* Line 1455 of yacc.c  */
#line 1138 "lev_comp.y"
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
#line 1150 "lev_comp.y"
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

  case 162:

/* Line 1455 of yacc.c  */
#line 1170 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1176 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1186 "lev_comp.y"
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

  case 165:

/* Line 1455 of yacc.c  */
#line 1204 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1214 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1220 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1225 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1230 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1237 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1244 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1253 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1261 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1272 "lev_comp.y"
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

  case 175:

/* Line 1455 of yacc.c  */
#line 1283 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1290 "lev_comp.y"
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

  case 177:

/* Line 1455 of yacc.c  */
#line 1301 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1308 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1312 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1318 "lev_comp.y"
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

  case 181:

/* Line 1455 of yacc.c  */
#line 1328 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1334 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1343 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1349 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1354 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1360 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1365 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1371 "lev_comp.y"
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

  case 189:

/* Line 1455 of yacc.c  */
#line 1382 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (5)].i), SPO_DOOR));
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1396 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1400 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1410 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1417 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1425 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1441 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1445 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1451 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1459 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1465 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1473 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1481 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1486 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1492 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1498 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1504 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1510 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1515 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1520 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1525 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1530 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1535 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1540 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1545 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1550 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1555 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1560 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1566 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1573 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1582 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1586 "lev_comp.y"
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

  case 231:

/* Line 1455 of yacc.c  */
#line 1599 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1607 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1617 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1625 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1634 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1641 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1649 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1655 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1660 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1665 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1670 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1675 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1680 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1685 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1690 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1695 "lev_comp.y"
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

  case 247:

/* Line 1455 of yacc.c  */
#line 1706 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1712 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1717 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1722 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1727 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1734 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1740 "lev_comp.y"
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

  case 254:

/* Line 1455 of yacc.c  */
#line 1769 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1774 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1782 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1788 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1794 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1801 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1808 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1818 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1828 "lev_comp.y"
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

  case 263:

/* Line 1455 of yacc.c  */
#line 1843 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1853 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1857 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1863 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1869 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1875 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int)(yyvsp[(6) - (13)].i), (int)(yyvsp[(8) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1881 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1887 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1893 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1898 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1905 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1912 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1918 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1924 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1930 "lev_comp.y"
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

  case 278:

/* Line 1455 of yacc.c  */
#line 1944 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      if ( (yyvsp[(9) - (10)].i) ) {
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      } else if ( (yyvsp[(10) - (10)].i) )
			  lc_error("Cannot use lev statements in non-permanent REGION");
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1954 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1958 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1964 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1971 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1977 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1983 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1989 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1993 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1998 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 2005 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 2011 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 2018 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 2022 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 2029 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2040 "lev_comp.y"
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

  case 296:

/* Line 1455 of yacc.c  */
#line 2053 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2057 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2063 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2067 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2074 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2078 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2082 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2098 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2106 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2116 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2125 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2130 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2137 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2148 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2154 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2158 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2162 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2169 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2179 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2186 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2190 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2196 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2200 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2208 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2212 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2219 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2229 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2242 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2246 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2253 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2263 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2272 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2283 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2287 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2294 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2304 "lev_comp.y"
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

  case 342:

/* Line 1455 of yacc.c  */
#line 2315 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2324 "lev_comp.y"
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

  case 344:

/* Line 1455 of yacc.c  */
#line 2334 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2340 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2344 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2351 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2361 "lev_comp.y"
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

  case 349:

/* Line 1455 of yacc.c  */
#line 2372 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2381 "lev_comp.y"
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

  case 351:

/* Line 1455 of yacc.c  */
#line 2391 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2395 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2401 "lev_comp.y"
    { ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2403 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2409 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2413 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2417 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2421 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2429 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2438 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2442 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2446 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2450 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2454 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2457 "lev_comp.y"
    { ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2461 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2468 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2477 "lev_comp.y"
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

  case 373:

/* Line 1455 of yacc.c  */
#line 2518 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2522 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2529 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2536 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2546 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2550 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2558 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2562 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2566 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2570 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2574 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2579 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2583 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2587 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2592 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2597 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2602 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2606 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2611 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2616 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2620 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2624 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2629 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2633 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2640 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2646 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2650 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2656 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2672 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2676 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2680 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2684 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2706 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2710 "lev_comp.y"
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

  case 420:

/* Line 1455 of yacc.c  */
#line 2736 "lev_comp.y"
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
#line 6164 "lev_comp.tab.c"
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
#line 2764 "lev_comp.y"


/*lev_comp.y*/

