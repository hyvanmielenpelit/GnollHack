
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
     FLOOR_TYPE = 334,
     FLOOR_TYPE_ID = 335,
     SUBROOM_ID = 336,
     NAME_ID = 337,
     FLAGS_ID = 338,
     FLAG_TYPE = 339,
     MON_ATTITUDE = 340,
     MON_ALERTNESS = 341,
     SUBTYPE_ID = 342,
     MON_APPEARANCE = 343,
     ROOMDOOR_ID = 344,
     IF_ID = 345,
     ELSE_ID = 346,
     TERRAIN_ID = 347,
     HORIZ_OR_VERT = 348,
     REPLACE_TERRAIN_ID = 349,
     EXIT_ID = 350,
     SHUFFLE_ID = 351,
     QUANTITY_ID = 352,
     BURIED_ID = 353,
     LOOP_ID = 354,
     FOR_ID = 355,
     TO_ID = 356,
     SWITCH_ID = 357,
     CASE_ID = 358,
     BREAK_ID = 359,
     DEFAULT_ID = 360,
     ERODED_ID = 361,
     TRAPPED_STATE = 362,
     RECHARGED_ID = 363,
     INVIS_ID = 364,
     GREASED_ID = 365,
     FEMALE_ID = 366,
     WAITFORU_ID = 367,
     CANCELLED_ID = 368,
     REVIVED_ID = 369,
     AVENGE_ID = 370,
     FLEEING_ID = 371,
     BLINDED_ID = 372,
     PARALYZED_ID = 373,
     STUNNED_ID = 374,
     CONFUSED_ID = 375,
     SEENTRAPS_ID = 376,
     ALL_ID = 377,
     MONTYPE_ID = 378,
     GRAVE_ID = 379,
     ERODEPROOF_ID = 380,
     FUNCTION_ID = 381,
     MSG_OUTPUT_TYPE = 382,
     COMPARE_TYPE = 383,
     UNKNOWN_TYPE = 384,
     rect_ID = 385,
     fillrect_ID = 386,
     line_ID = 387,
     randline_ID = 388,
     grow_ID = 389,
     selection_ID = 390,
     flood_ID = 391,
     rndcoord_ID = 392,
     circle_ID = 393,
     ellipse_ID = 394,
     filter_ID = 395,
     complement_ID = 396,
     gradient_ID = 397,
     GRADIENT_TYPE = 398,
     LIMITED = 399,
     HUMIDITY_TYPE = 400,
     STRING = 401,
     MAP_ID = 402,
     NQSTRING = 403,
     VARSTRING = 404,
     CFUNC = 405,
     CFUNC_INT = 406,
     CFUNC_STR = 407,
     CFUNC_COORD = 408,
     CFUNC_REGION = 409,
     VARSTRING_INT = 410,
     VARSTRING_INT_ARRAY = 411,
     VARSTRING_STRING = 412,
     VARSTRING_STRING_ARRAY = 413,
     VARSTRING_VAR = 414,
     VARSTRING_VAR_ARRAY = 415,
     VARSTRING_COORD = 416,
     VARSTRING_COORD_ARRAY = 417,
     VARSTRING_REGION = 418,
     VARSTRING_REGION_ARRAY = 419,
     VARSTRING_MAPCHAR = 420,
     VARSTRING_MAPCHAR_ARRAY = 421,
     VARSTRING_MONST = 422,
     VARSTRING_MONST_ARRAY = 423,
     VARSTRING_OBJ = 424,
     VARSTRING_OBJ_ARRAY = 425,
     VARSTRING_SEL = 426,
     VARSTRING_SEL_ARRAY = 427,
     METHOD_INT = 428,
     METHOD_INT_ARRAY = 429,
     METHOD_STRING = 430,
     METHOD_STRING_ARRAY = 431,
     METHOD_VAR = 432,
     METHOD_VAR_ARRAY = 433,
     METHOD_COORD = 434,
     METHOD_COORD_ARRAY = 435,
     METHOD_REGION = 436,
     METHOD_REGION_ARRAY = 437,
     METHOD_MAPCHAR = 438,
     METHOD_MAPCHAR_ARRAY = 439,
     METHOD_MONST = 440,
     METHOD_MONST_ARRAY = 441,
     METHOD_OBJ = 442,
     METHOD_OBJ_ARRAY = 443,
     METHOD_SEL = 444,
     METHOD_SEL_ARRAY = 445,
     DICE = 446
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
#line 489 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 501 "lev_comp.tab.c"

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
#define YYLAST   1075

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  209
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  168
/* YYNRULES -- Number of rules.  */
#define YYNRULES  423
/* YYNRULES -- Number of states.  */
#define YYNSTATES  915

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   446

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   204,   208,     2,
     148,   149,   202,   200,   146,   201,   206,   203,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   147,     2,
       2,   205,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   150,     2,   151,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   152,   207,   153,     2,     2,     2,     2,
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
     145,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199
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
     500,   506,   512,   520,   525,   526,   538,   539,   553,   554,
     557,   563,   565,   571,   573,   579,   581,   587,   589,   599,
     605,   607,   609,   611,   613,   615,   619,   621,   623,   625,
     633,   639,   641,   643,   645,   647,   651,   652,   658,   663,
     664,   668,   670,   672,   674,   676,   679,   681,   683,   685,
     687,   689,   693,   697,   701,   703,   705,   709,   711,   713,
     715,   719,   723,   724,   730,   733,   734,   738,   740,   744,
     746,   750,   754,   756,   758,   762,   764,   766,   768,   772,
     774,   776,   778,   784,   792,   798,   807,   809,   813,   819,
     825,   833,   841,   848,   854,   855,   858,   864,   868,   882,
     886,   890,   892,   898,   908,   914,   918,   922,   923,   935,
     936,   938,   946,   950,   956,   962,   968,   974,   978,   984,
     992,  1002,  1004,  1006,  1008,  1010,  1012,  1013,  1016,  1018,
    1022,  1024,  1026,  1028,  1029,  1033,  1035,  1037,  1039,  1041,
    1043,  1045,  1047,  1049,  1051,  1053,  1057,  1059,  1061,  1066,
    1068,  1070,  1075,  1077,  1079,  1084,  1086,  1091,  1097,  1099,
    1103,  1105,  1109,  1111,  1113,  1118,  1128,  1130,  1132,  1137,
    1139,  1145,  1147,  1149,  1154,  1156,  1158,  1164,  1166,  1168,
    1170,  1175,  1177,  1179,  1185,  1187,  1189,  1191,  1195,  1197,
    1199,  1203,  1205,  1210,  1214,  1218,  1222,  1226,  1230,  1234,
    1236,  1238,  1242,  1244,  1248,  1249,  1251,  1253,  1255,  1257,
    1261,  1262,  1264,  1266,  1269,  1272,  1277,  1284,  1289,  1296,
    1303,  1310,  1317,  1320,  1327,  1336,  1345,  1356,  1371,  1374,
    1376,  1380,  1382,  1386,  1388,  1390,  1392,  1394,  1396,  1398,
    1400,  1402,  1404,  1406,  1408,  1410,  1412,  1414,  1416,  1418,
    1420,  1422,  1424,  1435
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     210,     0,    -1,    -1,   211,    -1,   212,    -1,   212,   211,
      -1,   213,   220,   222,    -1,    16,   147,   154,    -1,    15,
     147,   154,   146,     3,    -1,    17,   147,    11,   146,   314,
      -1,    17,   147,    10,   146,     3,    -1,    17,   147,    13,
      -1,    17,   147,    12,   146,     3,   146,     3,   146,     5,
     146,     5,   146,   338,   146,   219,   218,    -1,    18,   147,
     368,    -1,    -1,   146,   144,    -1,    -1,   146,   345,    -1,
      -1,   146,     3,    -1,     5,    -1,    64,    -1,    -1,    83,
     147,   221,    -1,    84,   146,   221,    -1,    84,    -1,    -1,
     224,   222,    -1,   152,   222,   153,    -1,   265,    -1,   214,
      -1,   215,    -1,   324,    -1,   322,    -1,   323,    -1,   325,
      -1,   326,    -1,   307,    -1,   267,    -1,   230,    -1,   229,
      -1,   317,    -1,   279,    -1,   299,    -1,   328,    -1,   329,
      -1,   309,    -1,   310,    -1,   311,    -1,   327,    -1,   245,
      -1,   255,    -1,   257,    -1,   261,    -1,   259,    -1,   242,
      -1,   252,    -1,   238,    -1,   241,    -1,   302,    -1,   284,
      -1,   300,    -1,   287,    -1,   293,    -1,   318,    -1,   313,
      -1,   305,    -1,   266,    -1,   319,    -1,   272,    -1,   270,
      -1,   312,    -1,   316,    -1,   315,    -1,   303,    -1,   304,
      -1,   306,    -1,   298,    -1,   301,    -1,   164,    -1,   166,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,   176,
      -1,   178,    -1,   180,    -1,   163,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,   177,
      -1,   179,    -1,   225,    -1,   226,    -1,   157,    -1,   157,
      -1,   226,    -1,    96,   147,   225,    -1,   227,   205,   357,
      -1,   227,   205,   135,   147,   366,    -1,   227,   205,   356,
      -1,   227,   205,   373,   147,   350,    -1,   227,   205,   372,
     147,   352,    -1,   227,   205,   371,   147,   354,    -1,   227,
     205,   345,    -1,   227,   205,   348,    -1,   227,   205,   152,
     236,   153,    -1,   227,   205,   152,   235,   153,    -1,   227,
     205,   152,   234,   153,    -1,   227,   205,   373,   147,   152,
     233,   153,    -1,   227,   205,   372,   147,   152,   232,   153,
      -1,   227,   205,   371,   147,   152,   231,   153,    -1,   227,
     205,   152,   237,   153,    -1,   355,    -1,   231,   146,   355,
      -1,   353,    -1,   232,   146,   353,    -1,   351,    -1,   233,
     146,   351,    -1,   349,    -1,   234,   146,   349,    -1,   346,
      -1,   235,   146,   346,    -1,   357,    -1,   236,   146,   357,
      -1,   356,    -1,   237,   146,   356,    -1,    -1,    -1,   126,
     156,   148,   239,   361,   149,   240,   223,    -1,   156,   148,
     364,   149,    -1,    95,    -1,    -1,     6,    -1,     6,    -1,
     150,   357,   128,   357,   151,    -1,   150,   357,   151,    -1,
      -1,    -1,   102,   246,   150,   344,   151,   247,   152,   248,
     153,    -1,    -1,   249,   248,    -1,    -1,   103,   369,   147,
     250,   222,    -1,    -1,   105,   147,   251,   222,    -1,   104,
      -1,   206,   206,    -1,   101,    -1,   100,   228,   205,   357,
     253,   357,    -1,    -1,   254,   256,   223,    -1,    -1,    99,
     150,   344,   151,   258,   223,    -1,    -1,   244,   147,   260,
     224,    -1,    -1,    90,   244,   262,   263,    -1,   223,    -1,
      -1,   223,   264,    91,   223,    -1,    14,   147,   356,    -1,
      58,    -1,    58,   147,   369,    -1,    58,   147,    64,    -1,
      49,   147,   268,   146,   268,    -1,    49,   147,   268,   146,
     369,    -1,   148,     4,   146,    63,   146,   283,   149,    -1,
     331,   243,   146,   338,    -1,    -1,    81,   147,   269,   146,
     276,   146,   278,   332,   335,   271,   223,    -1,    -1,    43,
     147,   269,   146,   275,   146,   277,   146,   278,   332,   335,
     273,   223,    -1,    -1,   146,     5,    -1,   148,     4,   146,
       4,   149,    -1,    64,    -1,   148,     4,   146,     4,   149,
      -1,    64,    -1,   148,   285,   146,   286,   149,    -1,    64,
      -1,   148,     4,   146,     4,   149,    -1,    64,    -1,    89,
     147,   280,   146,   337,   146,   281,   146,   283,    -1,    25,
     147,   337,   146,   366,    -1,     5,    -1,    64,    -1,   282,
      -1,    64,    -1,    63,    -1,    63,   207,   282,    -1,     4,
      -1,    64,    -1,    20,    -1,    19,   147,   285,   146,   286,
     274,   155,    -1,    19,   147,   345,   274,   155,    -1,    68,
      -1,    69,    -1,    70,    -1,    69,    -1,    23,   147,   289,
      -1,    -1,    23,   147,   289,   288,   223,    -1,   352,   146,
     345,   290,    -1,    -1,   290,   146,   291,    -1,   356,    -1,
      85,    -1,    86,    -1,   340,    -1,    88,   356,    -1,   111,
      -1,   109,    -1,   113,    -1,   114,    -1,   115,    -1,   116,
     147,   344,    -1,   117,   147,   344,    -1,   118,   147,   344,
      -1,   119,    -1,   120,    -1,   121,   147,   292,    -1,   112,
      -1,   154,    -1,   122,    -1,   154,   207,   292,    -1,    21,
     147,   295,    -1,    -1,    22,   147,   295,   294,   223,    -1,
     354,   296,    -1,    -1,   296,   146,   297,    -1,    61,    -1,
     123,   147,   352,    -1,   370,    -1,    82,   147,   356,    -1,
      97,   147,   344,    -1,    98,    -1,    60,    -1,   106,   147,
     344,    -1,   125,    -1,    59,    -1,   107,    -1,   108,   147,
     344,    -1,   109,    -1,   110,    -1,   345,    -1,    24,   147,
     330,   146,   345,    -1,    26,   147,   345,   146,    63,   146,
     337,    -1,    30,   147,   345,   146,    63,    -1,    30,   147,
     345,   146,    63,   146,     5,   218,    -1,    31,    -1,    31,
     147,   366,    -1,    39,   147,   345,   146,    72,    -1,    40,
     147,   345,   146,    72,    -1,    40,   147,   375,   146,   375,
     146,    72,    -1,    44,   147,   375,   146,   375,   146,   154,
      -1,    45,   147,   375,   146,   375,   308,    -1,    46,   147,
     375,   146,   375,    -1,    -1,   146,    72,    -1,    52,   147,
     366,   146,    76,    -1,    53,   147,   366,    -1,    54,   147,
     345,   146,   148,   369,   146,   369,   149,   146,    74,   146,
      73,    -1,    56,   147,   366,    -1,    55,   147,   366,    -1,
       3,    -1,   148,     3,   146,   338,   149,    -1,    94,   147,
     348,   146,   350,   146,   350,   146,     7,    -1,    92,   147,
     366,   146,   350,    -1,    41,   147,   348,    -1,    42,   147,
     348,    -1,    -1,    32,   147,   348,   146,   338,   146,   331,
     332,   335,   320,   321,    -1,    -1,   223,    -1,    36,   147,
     345,   146,   339,   146,   341,    -1,    37,   147,   345,    -1,
      87,   147,   366,   146,     4,    -1,    38,   147,    75,   146,
     345,    -1,   124,   147,   345,   146,   356,    -1,   124,   147,
     345,   146,    64,    -1,   124,   147,   345,    -1,    50,   147,
     357,   146,   345,    -1,    51,   147,   345,   146,   374,   146,
     356,    -1,    48,   147,   344,   146,   344,   146,   344,   146,
     344,    -1,    48,    -1,   154,    -1,    64,    -1,   154,    -1,
      64,    -1,    -1,   146,   333,    -1,   334,    -1,   334,   146,
     333,    -1,    33,    -1,    34,    -1,    35,    -1,    -1,    80,
     147,   336,    -1,    79,    -1,    59,    -1,    64,    -1,    60,
      -1,    64,    -1,    67,    -1,   342,    -1,    64,    -1,    67,
      -1,   342,    -1,    66,   147,    64,    -1,    71,    -1,    64,
      -1,    66,   150,     4,   151,    -1,   154,    -1,   165,    -1,
     166,   150,   357,   151,    -1,   357,    -1,   346,    -1,   137,
     148,   366,   149,    -1,   169,    -1,   170,   150,   357,   151,
      -1,   148,     4,   146,     4,   149,    -1,    64,    -1,    65,
     347,   151,    -1,   145,    -1,   145,   146,   347,    -1,   349,
      -1,   171,    -1,   172,   150,   357,   151,    -1,   148,     4,
     146,     4,   146,     4,   146,     4,   149,    -1,   351,    -1,
     173,    -1,   174,   150,   357,   151,    -1,     3,    -1,   148,
       3,   146,   338,   149,    -1,   353,    -1,   175,    -1,   176,
     150,   357,   151,    -1,   154,    -1,     3,    -1,   148,     3,
     146,   154,   149,    -1,    64,    -1,   355,    -1,   177,    -1,
     178,   150,   357,   151,    -1,   154,    -1,     3,    -1,   148,
       3,   146,   154,   149,    -1,    64,    -1,    77,    -1,   343,
      -1,   356,   206,   343,    -1,     4,    -1,   367,    -1,   148,
       8,   149,    -1,   163,    -1,   164,   150,   357,   151,    -1,
     357,   200,   357,    -1,   357,   201,   357,    -1,   357,   202,
     357,    -1,   357,   203,   357,    -1,   357,   204,   357,    -1,
     148,   357,   149,    -1,   159,    -1,   160,    -1,   227,   147,
     358,    -1,   359,    -1,   360,   146,   359,    -1,    -1,   360,
      -1,   357,    -1,   356,    -1,   362,    -1,   363,   146,   362,
      -1,    -1,   363,    -1,   345,    -1,   130,   348,    -1,   131,
     348,    -1,   132,   345,   146,   345,    -1,   133,   345,   146,
     345,   146,   357,    -1,   134,   148,   366,   149,    -1,   134,
     148,   282,   146,   366,   149,    -1,   140,   148,     7,   146,
     366,   149,    -1,   140,   148,   366,   146,   366,   149,    -1,
     140,   148,   350,   146,   366,   149,    -1,   136,   345,    -1,
     138,   148,   345,   146,   357,   149,    -1,   138,   148,   345,
     146,   357,   146,    33,   149,    -1,   139,   148,   345,   146,
     357,   146,   357,   149,    -1,   139,   148,   345,   146,   357,
     146,   357,   146,    33,   149,    -1,   142,   148,   143,   146,
     148,   357,   201,   357,   216,   149,   146,   345,   217,   149,
      -1,   141,   365,    -1,   179,    -1,   148,   366,   149,    -1,
     365,    -1,   365,   208,   366,    -1,   199,    -1,    78,    -1,
       4,    -1,     8,    -1,     9,    -1,     4,    -1,     8,    -1,
       9,    -1,     4,    -1,   367,    -1,    27,    -1,    21,    -1,
      28,    -1,    23,    -1,    29,    -1,    92,    -1,    62,    -1,
      64,    -1,   376,    -1,    47,   148,     4,   146,     4,   146,
       4,   146,     4,   149,    -1,   148,     4,   146,     4,   146,
       4,   146,     4,   149,    -1
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
    2073,  2077,  2081,  2088,  2091,  2097,  2103,  2104,  2107,  2108,
    2111,  2112,  2113,  2119,  2120,  2121,  2127,  2128,  2131,  2140,
    2145,  2152,  2163,  2169,  2173,  2177,  2184,  2194,  2201,  2205,
    2211,  2215,  2223,  2227,  2234,  2244,  2257,  2261,  2268,  2278,
    2287,  2298,  2302,  2309,  2319,  2330,  2339,  2349,  2355,  2359,
    2366,  2376,  2387,  2396,  2406,  2410,  2417,  2418,  2424,  2428,
    2432,  2436,  2444,  2453,  2457,  2461,  2465,  2469,  2473,  2476,
    2483,  2492,  2525,  2526,  2529,  2530,  2533,  2537,  2544,  2551,
    2562,  2565,  2573,  2577,  2581,  2585,  2589,  2594,  2598,  2602,
    2607,  2612,  2617,  2621,  2626,  2631,  2635,  2639,  2644,  2648,
    2655,  2661,  2665,  2671,  2678,  2679,  2682,  2683,  2684,  2687,
    2691,  2695,  2699,  2705,  2706,  2709,  2710,  2713,  2714,  2717,
    2718,  2721,  2725,  2751
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
  "FLOOR_TYPE", "FLOOR_TYPE_ID", "SUBROOM_ID", "NAME_ID", "FLAGS_ID",
  "FLAG_TYPE", "MON_ATTITUDE", "MON_ALERTNESS", "SUBTYPE_ID",
  "MON_APPEARANCE", "ROOMDOOR_ID", "IF_ID", "ELSE_ID", "TERRAIN_ID",
  "HORIZ_OR_VERT", "REPLACE_TERRAIN_ID", "EXIT_ID", "SHUFFLE_ID",
  "QUANTITY_ID", "BURIED_ID", "LOOP_ID", "FOR_ID", "TO_ID", "SWITCH_ID",
  "CASE_ID", "BREAK_ID", "DEFAULT_ID", "ERODED_ID", "TRAPPED_STATE",
  "RECHARGED_ID", "INVIS_ID", "GREASED_ID", "FEMALE_ID", "WAITFORU_ID",
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
  "roomregionflags", "roomregionflag", "optfloortype", "floortype",
  "door_state", "light_state", "alignment", "alignment_prfx", "altar_type",
  "a_register", "string_or_var", "integer_or_var", "coord_or_var",
  "encodecoord", "humidity_flags", "region_or_var", "encoderegion",
  "mapchar_or_var", "mapchar", "monster_or_var", "encodemonster",
  "object_or_var", "encodeobj", "string_expr", "math_expr_var",
  "func_param_type", "func_param_part", "func_param_list",
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
     395,   396,   397,   398,   399,   400,    44,    58,    40,    41,
      91,    93,   123,   125,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
      43,    45,    42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   209,   210,   210,   211,   211,   212,   213,   213,   214,
     214,   214,   214,   215,   216,   216,   217,   217,   218,   218,
     219,   219,   220,   220,   221,   221,   222,   222,   223,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   227,   227,   227,
     228,   228,   229,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   231,   231,
     232,   232,   233,   233,   234,   234,   235,   235,   236,   236,
     237,   237,   239,   240,   238,   241,   242,   243,   243,   244,
     244,   244,   246,   247,   245,   248,   248,   250,   249,   251,
     249,   252,   253,   253,   254,   256,   255,   258,   257,   260,
     259,   262,   261,   263,   264,   263,   265,   266,   266,   266,
     267,   267,   268,   269,   271,   270,   273,   272,   274,   274,
     275,   275,   276,   276,   277,   277,   278,   278,   279,   279,
     280,   280,   281,   281,   282,   282,   283,   283,   284,   284,
     284,   285,   285,   286,   286,   287,   288,   287,   289,   290,
     290,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   292,   292,
     292,   293,   294,   293,   295,   296,   296,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   298,   299,   300,   300,   301,   301,   302,   303,
     304,   305,   306,   307,   308,   308,   309,   310,   311,   312,
     313,   314,   314,   315,   316,   317,   318,   320,   319,   321,
     321,   322,   323,   324,   325,   326,   326,   326,   327,   328,
     329,   329,   330,   330,   331,   331,   332,   332,   333,   333,
     334,   334,   334,   335,   335,   336,   337,   337,   338,   338,
     339,   339,   339,   340,   340,   340,   341,   341,   342,   343,
     343,   343,   344,   345,   345,   345,   345,   346,   346,   346,
     347,   347,   348,   348,   348,   349,   350,   350,   350,   351,
     351,   352,   352,   352,   353,   353,   353,   353,   354,   354,
     354,   355,   355,   355,   355,   355,   356,   356,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   358,
     358,   359,   360,   360,   361,   361,   362,   362,   363,   363,
     364,   364,   365,   365,   365,   365,   365,   365,   365,   365,
     365,   365,   365,   365,   365,   365,   365,   365,   365,   365,
     365,   366,   366,   367,   368,   368,   369,   369,   369,   370,
     370,   370,   370,   371,   371,   372,   372,   373,   373,   374,
     374,   375,   375,   376
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
       5,     5,     7,     4,     0,    11,     0,    13,     0,     2,
       5,     1,     5,     1,     5,     1,     5,     1,     9,     5,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     7,
       5,     1,     1,     1,     1,     3,     0,     5,     4,     0,
       3,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     3,     3,     3,     1,     1,     3,     1,     1,     1,
       3,     3,     0,     5,     2,     0,     3,     1,     3,     1,
       3,     3,     1,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     5,     7,     5,     8,     1,     3,     5,     5,
       7,     7,     6,     5,     0,     2,     5,     3,    13,     3,
       3,     1,     5,     9,     5,     3,     3,     0,    11,     0,
       1,     7,     3,     5,     5,     5,     5,     3,     5,     7,
       9,     1,     1,     1,     1,     1,     0,     2,     1,     3,
       1,     1,     1,     0,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     4,     1,
       1,     4,     1,     1,     4,     1,     4,     5,     1,     3,
       1,     3,     1,     1,     4,     9,     1,     1,     4,     1,
       5,     1,     1,     4,     1,     1,     5,     1,     1,     1,
       4,     1,     1,     5,     1,     1,     1,     3,     1,     1,
       3,     1,     4,     3,     3,     3,     3,     3,     3,     1,
       1,     3,     1,     3,     0,     1,     1,     1,     1,     3,
       0,     1,     1,     2,     2,     4,     6,     4,     6,     6,
       6,     6,     2,     6,     8,     8,    10,    14,     2,     1,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,    10,     9
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
     101,     0,     0,     0,     0,   358,     0,   361,     0,   403,
       0,   359,   380,    27,     0,   159,     0,     8,     0,   319,
     320,     0,   356,   166,     0,     0,     0,    11,   405,   404,
      13,   328,     0,   201,   202,     0,     0,   325,     0,     0,
     178,   323,   352,   354,   355,     0,   351,   349,     0,   231,
     235,   348,   232,   345,   347,     0,   344,   342,     0,   205,
       0,   341,   293,   292,     0,   306,   307,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,   382,   401,   257,     0,   333,     0,     0,
     332,     0,   282,     0,     0,     0,     0,     0,     0,   421,
     275,   276,   295,   294,     0,   137,     0,     0,     0,     0,
       0,   322,     0,     0,     0,     0,     0,   267,     0,   270,
     269,   408,   406,   407,   169,   168,     0,     0,   190,   191,
       0,     0,     0,     0,   102,     0,     0,     0,   287,   132,
       0,     0,     0,     0,   141,     0,     0,     0,     0,     0,
     377,   376,   378,   381,     0,   414,   416,   413,   415,   417,
     418,     0,     0,     0,   109,   110,   105,   103,     0,     0,
       0,     0,    26,   156,    24,     0,     0,     0,     0,     0,
     330,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     383,   384,     0,     0,     0,   392,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     163,   162,     0,     0,   157,     0,     0,     0,   374,   360,
     368,     0,     0,   363,   364,   365,   366,   367,     0,   135,
       0,   358,     0,     0,     0,     0,   126,   124,   130,   128,
       0,     0,     0,   160,     0,     0,   357,    10,   271,     0,
       9,     0,     0,   329,     0,     0,     0,   204,   203,   178,
     179,   200,     0,     0,     0,   233,     0,     0,   207,   209,
     252,   189,     0,   254,     0,     0,   194,     0,     0,     0,
       0,   339,     0,     0,   337,     0,     0,   336,     0,     0,
     400,   402,     0,     0,   308,   309,     0,   312,     0,   310,
       0,   311,   284,   258,     0,     0,   259,     0,   181,     0,
       0,     0,     0,     0,   264,   263,     0,     0,   170,   171,
     288,   419,   420,     0,   266,     0,   183,     0,     0,   283,
       0,     0,     0,   274,     0,     0,   153,     0,     0,   143,
     286,   285,     0,   372,   375,     0,   362,   140,   379,   104,
       0,     0,   113,     0,   112,     0,   111,     0,   117,     0,
     108,     0,   107,     0,   106,    28,   321,     0,     0,   331,
     324,     0,   326,     0,     0,   350,   411,   409,   410,   246,
     243,   237,     0,     0,   242,     0,   247,     0,   249,   250,
       0,   245,   236,   251,   412,   239,     0,   343,   208,     0,
       0,   385,     0,     0,     0,   387,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   334,     0,     0,     0,     0,
       0,     0,     0,     0,   173,     0,     0,     0,   262,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   158,   152,
     154,     0,     0,     0,   133,     0,   125,   127,   129,   131,
       0,   118,     0,   120,     0,   122,     0,     0,   327,   199,
     353,     0,     0,     0,     0,     0,   346,     0,   253,    18,
       0,   195,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   296,     0,   317,   316,   281,     0,     0,   260,
       0,   185,     0,     0,   261,   265,     0,     0,   289,     0,
       0,   187,     0,   296,   193,     0,   192,   165,     0,   145,
     369,   370,   371,   373,     0,     0,   116,     0,   115,     0,
     114,     0,     0,   240,   241,   244,   248,   238,     0,   313,
     212,   213,     0,   217,   216,   227,   218,   219,   220,     0,
       0,     0,   224,   225,     0,   210,   214,   314,   211,     0,
     255,   386,   388,     0,   393,     0,   389,     0,   338,   391,
     390,     0,     0,     0,   303,   318,     0,     0,     0,     0,
       0,     0,   196,   197,     0,     0,     0,     0,   303,     0,
       0,     0,     0,     0,   145,   134,   119,   121,   123,   272,
       0,     0,   215,     0,     0,     0,     0,    19,     0,     0,
     340,     0,     0,   300,   301,   302,   297,   298,     0,   277,
       0,     0,   180,     0,   296,   290,   172,     0,   182,     0,
     174,   188,   273,     0,   149,   144,   146,     0,   315,   221,
     222,   223,   229,   228,   226,   394,     0,   395,   364,     0,
       0,     0,   279,     0,     0,     0,   303,     0,     0,     0,
     147,    26,     0,     0,     0,     0,     0,   335,   299,   305,
     304,   280,   278,     0,   423,   184,   176,     0,   186,   175,
      26,   150,     0,   230,   396,    15,     0,   422,     0,     0,
     148,     0,     0,   177,   268,     0,    16,    20,    21,    18,
       0,     0,    12,    17,   397
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    87,    88,   876,   911,   770,
     909,    12,   146,    89,   363,    90,    91,    92,    93,   191,
      94,    95,   670,   672,   674,   452,   453,   454,   455,    96,
     438,   734,    97,    98,   415,    99,   100,   192,   661,   803,
     804,   890,   871,   101,   558,   102,   103,   206,   104,   555,
     105,   361,   106,   321,   431,   551,   107,   108,   109,   303,
     294,   110,   869,   111,   898,   377,   530,   548,   713,   723,
     112,   320,   725,   497,   794,   113,   229,   479,   114,   384,
     249,   618,   765,   854,   115,   381,   239,   380,   612,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   648,
     126,   127,   128,   129,   130,   470,   131,   132,   133,   134,
     135,   862,   882,   136,   137,   138,   139,   140,   141,   142,
     143,   254,   295,   784,   826,   827,   829,   880,   257,   516,
     520,   766,   706,   521,   212,   300,   273,   231,   371,   279,
     280,   506,   507,   250,   251,   240,   241,   340,   301,   732,
     563,   564,   565,   342,   343,   344,   274,   401,   201,   220,
     315,   615,   358,   359,   360,   543,   288,   289
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -730
static const yytype_int16 yypact[] =
{
     295,   -72,   -38,   115,  -730,   295,    49,   -12,    -2,  -730,
    -730,    84,   648,    34,  -730,   105,  -730,   143,   147,   177,
     189,  -730,   196,   208,   229,   231,   235,   250,   252,   254,
     258,   260,   282,   285,   292,   300,   303,   306,   309,   315,
     318,   321,   322,   324,   328,   334,   335,   340,   341,   342,
     343,   346,   348,   354,   356,    36,   357,   358,  -730,   359,
      18,   622,  -730,  -730,   366,    98,    64,   153,  -730,  -730,
    -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,
    -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,
     648,  -730,  -730,   203,  -730,  -730,  -730,  -730,  -730,   381,
    -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,
    -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,
    -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,
    -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,
    -730,  -730,  -730,  -730,   448,   320,  -730,   -58,   339,    66,
     261,   180,   180,   316,   -24,    42,   -19,   -19,   712,   -60,
     -19,   -19,   402,   -19,   330,   -60,   -60,    -1,    15,    15,
      15,    64,   383,    64,   -19,   712,   712,   -19,   712,   712,
      96,    -1,   712,    67,  -730,   712,   -60,   614,    64,  -730,
    -730,   289,   379,   -19,   386,  -730,    27,  -730,   380,  -730,
     171,  -730,    50,  -730,    30,  -730,   396,  -730,   105,  -730,
    -730,   385,  -730,   344,   390,   405,   409,  -730,  -730,  -730,
    -730,  -730,   411,  -730,  -730,   412,   555,  -730,   419,   415,
     416,  -730,  -730,  -730,  -730,   569,  -730,  -730,   423,  -730,
    -730,  -730,  -730,  -730,  -730,   572,  -730,  -730,   453,   452,
     461,  -730,  -730,  -730,   463,  -730,  -730,   464,   465,   471,
     -60,   -60,   -19,   -19,   477,   -19,   478,   483,   484,   712,
     485,   304,  -730,  -730,   427,  -730,   637,  -730,   492,   497,
    -730,   499,  -730,   500,   501,   502,   644,   503,   507,  -730,
    -730,  -730,  -730,  -730,   517,   658,   672,   531,   535,   536,
     537,   364,   691,   559,   308,   561,   562,  -730,   568,  -730,
    -730,  -730,  -730,  -730,  -730,  -730,   570,   571,  -730,  -730,
     574,   396,   575,   576,  -730,   564,    64,    64,   577,  -730,
     581,   218,    64,    64,  -730,    64,    64,    64,    64,    64,
     344,   364,  -730,   585,   583,  -730,  -730,  -730,  -730,  -730,
    -730,   586,    44,   141,  -730,  -730,   344,   364,   587,   589,
     592,   648,   648,  -730,  -730,    64,   -58,   715,    40,   738,
     599,   595,   712,   603,    64,    21,   746,   600,   608,    64,
     610,   396,   611,    64,   396,   -19,   -19,   712,   695,   696,
    -730,  -730,   620,   621,   699,  -730,   -19,   -19,   384,  -730,
     617,   619,   712,   623,    64,    29,   319,   -19,   698,   767,
     627,   703,    15,   -25,  -730,   650,   656,    15,    15,    15,
      64,   657,    75,   -19,   107,   707,   659,   -15,   802,    42,
     717,  -730,    82,    82,  -730,   -45,   683,   -27,   754,  -730,
    -730,   376,   382,    83,    83,  -730,  -730,  -730,    50,  -730,
     712,   663,   -54,    20,    26,    86,  -730,  -730,   344,   364,
      33,   248,   140,  -730,   704,   393,  -730,  -730,  -730,   807,
    -730,   713,   411,  -730,   709,   857,   398,  -730,  -730,   416,
    -730,  -730,   708,   436,   256,  -730,   710,   455,  -730,  -730,
    -730,  -730,   719,   720,   -19,   -19,   660,   724,   714,   725,
     726,  -730,   727,   482,  -730,   729,   730,  -730,   734,   737,
    -730,  -730,   880,   509,  -730,  -730,   739,  -730,   736,  -730,
     741,  -730,  -730,  -730,   742,   885,  -730,   744,  -730,   888,
     747,    29,   890,   749,   750,  -730,   751,   836,  -730,  -730,
    -730,  -730,  -730,   766,  -730,   333,  -730,   909,   768,  -730,
     769,   825,   932,  -730,   790,   396,  -730,   731,    64,  -730,
    -730,   344,   791,  -730,   793,   792,  -730,  -730,  -730,  -730,
     936,   794,  -730,    22,  -730,    64,  -730,   -58,  -730,    70,
    -730,    71,  -730,    73,  -730,  -730,  -730,   798,   942,  -730,
    -730,   797,  -730,   795,   799,  -730,  -730,  -730,  -730,  -730,
    -730,  -730,   800,   804,  -730,   805,  -730,   806,  -730,  -730,
     809,  -730,  -730,  -730,  -730,  -730,   808,  -730,   803,    42,
     953,  -730,   813,   897,   712,  -730,    64,    64,   712,   815,
      64,   712,   712,   814,   817,  -730,    -1,   960,    55,   961,
     154,   894,   821,    14,  -730,   822,   816,   899,  -730,    64,
     823,   -58,   826,   827,    16,   297,   396,    82,  -730,  -730,
     364,   824,   268,   754,  -730,   198,  -730,  -730,   364,   344,
     106,  -730,   116,  -730,   138,  -730,    29,   828,  -730,  -730,
    -730,   -58,    64,    64,    64,   316,  -730,   789,  -730,   829,
      64,  -730,   830,   210,   338,   831,    29,   524,   832,   833,
      64,   973,   837,   834,  -730,  -730,  -730,   838,   974,  -730,
     982,  -730,   347,   841,  -730,  -730,   842,    61,   344,   333,
     985,  -730,   986,   837,  -730,   845,  -730,  -730,   846,   168,
    -730,  -730,  -730,  -730,   396,    70,  -730,    71,  -730,    73,
    -730,   844,   989,   344,  -730,  -730,  -730,  -730,   -26,  -730,
    -730,  -730,   -58,  -730,  -730,  -730,  -730,  -730,  -730,   848,
     849,   850,  -730,  -730,   851,  -730,  -730,  -730,   344,   996,
    -730,   364,  -730,   967,  -730,    64,  -730,   852,  -730,  -730,
    -730,   388,   856,   355,   923,  -730,  1000,   859,   858,   860,
      16,    64,  -730,  -730,   861,   862,   863,   867,   923,    61,
    1001,   333,   868,   864,   168,  -730,  -730,  -730,  -730,  -730,
     870,   945,   344,    64,    64,    64,   -51,  -730,   865,   257,
    -730,    64,  1014,  -730,  -730,  -730,  -730,   873,   874,  -730,
     876,  1016,  -730,    21,   837,  -730,  -730,   877,  -730,  1020,
    -730,  -730,  -730,   878,  -730,  -730,  -730,  1021,  -730,  -730,
    -730,  -730,  -730,   820,  -730,  -730,   995,  -730,   129,   881,
     355,   950,   396,  1027,   883,   884,   923,   962,   886,   396,
    -730,   648,   891,   -51,   889,   895,   892,  -730,  -730,  -730,
    -730,  -730,  -730,   893,  -730,  -730,  -730,   898,  -730,  -730,
     648,  -730,    29,  -730,  -730,  -730,   900,  -730,   396,   970,
    -730,   901,   -19,  -730,  -730,    72,   902,  -730,  -730,   829,
     -19,   896,  -730,  -730,  -730
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -730,  -730,  1029,  -730,  -730,  -730,  -730,  -730,  -730,   131,
    -730,  -730,   843,   -90,  -317,   688,   866,   991,  -422,  -730,
    -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,
    -730,  -730,  -730,  -730,  -730,   999,  -730,  -730,  -730,   246,
    -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,
    -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,   633,
     875,  -730,  -730,  -730,  -730,   578,  -730,  -730,  -730,   269,
    -730,  -730,  -730,  -557,   259,  -730,   349,   227,  -730,  -730,
    -730,  -730,  -730,   190,  -730,  -730,   910,  -730,  -730,  -730,
    -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,
    -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,
    -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,
    -730,  -730,   428,  -685,   205,  -730,  -729,  -730,  -397,  -522,
    -730,  -730,  -730,   387,   700,  -186,  -149,  -340,   596,   -84,
    -331,  -412,  -542,  -432,  -534,   607,  -518,  -144,   -56,  -730,
     406,  -730,  -730,   624,  -730,  -730,   801,  -152,   591,  -730,
    -417,  -730,  -730,  -730,  -730,  -730,  -151,  -730
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -207
static const yytype_int16 yytable[] =
{
     203,   230,   325,   213,   430,   539,   275,   258,   259,   644,
     200,   281,   282,   456,   284,   287,   562,   297,   298,   299,
     553,   554,   457,   306,   307,   305,   309,   310,   308,   582,
     317,   195,   550,   322,   195,   330,   232,   560,   798,   528,
     252,   675,    16,   468,   328,   221,   222,   673,   451,   546,
     584,   345,   330,   346,   195,   354,   556,   347,   348,   349,
     356,   671,   285,   292,   485,   792,   691,   488,   195,   840,
     218,   852,   318,   232,   243,     7,   501,   907,   711,   311,
     721,   290,   291,   312,   313,   501,   221,   222,   276,   514,
     477,   478,   571,   515,   221,   222,   209,   233,   726,   572,
     311,   255,   323,   853,   312,   313,   256,   210,   211,     8,
     234,   277,   278,   392,   393,     9,   395,   304,   225,   704,
     355,   811,   350,   529,   637,   793,   705,   209,   652,   226,
     253,   319,    11,   547,   233,   244,   908,   886,   210,   211,
     331,   436,    13,   501,   219,   195,   341,   234,   357,   866,
     227,   228,    14,   293,   741,   335,   336,   337,   338,   339,
     314,   557,   712,   296,   722,   351,   573,   225,   188,   541,
     226,   542,   575,   574,   777,   196,   390,   391,   352,   576,
     144,   235,   353,   232,   209,   579,    66,   236,   469,   145,
     197,   198,   196,   197,   198,   210,   211,   808,   196,   227,
     228,   277,   278,   807,   209,   221,   222,   197,   198,   458,
     237,   238,   196,   197,   198,   210,   211,   806,   235,   245,
     474,   552,   688,   302,   236,   246,   199,   197,   198,   199,
     552,    15,   577,   667,   536,   491,   489,   490,   658,   578,
     666,   562,   498,   199,   233,   728,   508,   499,   500,   199,
     511,   243,   735,   747,   194,   504,   505,   234,   522,   736,
     596,   527,   737,   199,   597,   598,   533,   534,   535,   738,
     435,   801,   464,   802,   540,   875,   441,   442,   -14,   443,
     444,   445,   446,   447,   739,   337,   338,   339,   552,   352,
     147,   740,   583,   561,   148,   209,   331,   459,   569,   333,
     708,   202,   795,   678,   197,   198,   210,   211,   373,   465,
       1,     2,   244,   504,   505,   599,   600,   601,   476,   243,
     221,   222,   334,   483,   149,   221,   222,   487,   235,   223,
     224,   337,   338,   339,   236,   613,   150,   311,   602,   727,
     199,   312,   313,   151,   701,   621,   622,   678,   513,   214,
     215,   216,   217,   603,   604,   152,   773,   237,   238,   774,
     496,   724,   605,   606,   607,   608,   609,   440,   221,   222,
     901,   335,   336,   337,   338,   339,   153,   285,   154,   610,
     244,   611,   155,   517,   843,   518,   519,   501,   823,   824,
     825,   502,   341,   225,   221,   222,   245,   156,   225,   157,
     581,   158,   246,   856,   226,   159,   857,   160,   204,   226,
     335,   336,   337,   338,   339,   223,   224,   805,   335,   336,
     337,   338,   339,   247,   248,   227,   228,   730,   731,   161,
     227,   228,   162,   669,   260,   261,   262,   263,   264,   163,
     265,   225,   266,   267,   268,   269,   270,   164,   221,   222,
     165,   207,   271,   166,   423,   199,   167,   335,   336,   337,
     338,   339,   168,   716,   245,   169,   208,   225,   170,   171,
     246,   172,   692,   227,   228,   173,   695,   283,   286,   698,
     699,   174,   175,   272,   775,   629,   373,   176,   177,   178,
     179,   247,   248,   180,   326,   181,   744,   745,   746,   227,
     228,   182,   660,   183,   185,   186,   187,   718,   335,   336,
     337,   338,   339,   193,   260,   261,   262,   263,   264,   668,
     265,   225,   266,   267,   268,   269,   270,   566,   205,   327,
     332,   302,   503,   567,   329,   365,   367,   743,   335,   336,
     337,   338,   339,   768,   586,   881,   221,   222,   362,   592,
     366,   368,   889,   227,   228,   369,   370,   504,   505,   373,
     372,   375,   376,   272,   335,   336,   337,   338,   339,   374,
     693,   694,   378,   379,   697,   382,   335,   336,   337,   338,
     339,   903,   335,   336,   337,   338,   339,   595,   335,   821,
     337,   338,   339,   335,   336,   337,   338,   339,   335,   336,
     337,   338,   339,   383,  -206,   835,   617,   385,   812,   386,
     387,   388,   260,   261,   262,   263,   264,   389,   265,   225,
     266,   267,   268,   269,   270,   394,   396,   849,   850,   851,
     271,   397,   398,   400,   771,   402,   335,   336,   337,   338,
     339,   403,   404,   405,   781,   406,   407,   408,   410,   411,
     409,   227,   228,   412,    16,   335,   336,   337,   338,   339,
     635,   272,    17,   413,   414,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,   778,   416,   417,    28,    29,
      30,   418,   419,   420,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,   421,    42,    43,    44,    45,
      46,    47,    48,    49,    50,   422,    51,   424,   425,   335,
     336,   337,   338,   339,   426,   434,   427,   428,   467,   819,
     429,   432,   433,   437,   335,   336,   337,   338,   339,    52,
     439,   448,   449,   450,   460,    53,   461,    54,    55,   462,
      56,   471,    57,    58,    59,   472,   473,    60,    61,   475,
      62,   480,    63,   906,   482,   481,   484,   486,   492,   493,
     509,   913,   496,   221,   222,   858,   494,   495,   510,   512,
     523,   524,    64,   525,    65,   526,   221,   222,    70,   189,
      72,   891,    74,   544,    76,    69,    78,    71,    80,    73,
      82,    75,    84,    77,    86,    79,   531,    81,    66,    83,
     900,    85,   532,   537,    67,    68,   549,   545,  -164,   570,
     587,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,   260,
     261,   262,   263,   264,   559,   265,   225,   266,   267,   268,
     269,   270,   260,   261,   262,   263,   264,   271,   265,   225,
     266,   267,   268,   269,   270,   748,   749,   585,   590,   588,
     271,   591,   594,   625,   616,   619,   620,   623,   227,   228,
     624,   626,   627,   628,   750,   751,   631,   752,   272,   630,
     632,   227,   228,   633,   634,   636,   637,   638,   639,   640,
     641,   272,   642,   643,   645,   646,   647,   649,   753,   650,
     754,   755,   756,   757,   758,   759,   760,   761,   762,   763,
     764,    68,   651,   653,   654,   655,   656,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,   629,   657,   659,   662,   663,
     665,   664,   276,   209,   676,   677,   678,   681,   680,   687,
     679,   682,   683,   684,   210,   211,   685,   686,   689,   690,
     496,   696,   700,   701,   703,   707,   709,   710,   708,   717,
     714,   715,   719,   720,   742,   769,   729,   782,   787,   772,
     776,   779,   780,   783,   786,   785,   788,   790,   791,   796,
     797,   799,   800,   809,   810,   813,   814,   815,   816,   817,
     818,   820,   822,   828,   830,   831,   833,   832,   842,   848,
     836,   837,   838,   839,   855,   844,   847,   845,   859,   860,
     864,   861,   863,   867,   868,   870,   872,   873,   874,   879,
     877,   883,   884,   885,    10,   888,   887,   892,   894,   895,
     912,   896,   897,   904,   899,   914,   902,   905,   910,   463,
     846,   364,   190,   324,   184,   538,   316,   593,   841,   834,
     865,   789,   242,   893,   702,   878,   466,   580,   589,   733,
     399,     0,   568,     0,   767,   614
};

static const yytype_int16 yycheck[] =
{
      90,   150,   188,   147,   321,   422,   158,   156,   157,   531,
      66,   160,   161,   353,   163,   164,   438,   168,   169,   170,
     432,   433,   353,   175,   176,   174,   178,   179,   177,   461,
     182,     4,   429,   185,     4,     8,     3,    64,   723,    64,
      64,   583,     6,     3,   193,    64,    65,   581,     4,    64,
     462,    21,     8,    23,     4,   204,   101,    27,    28,    29,
     204,   579,    47,    64,   381,     4,   623,   384,     4,   798,
       4,   122,     5,     3,     3,   147,     3,     5,    64,     4,
      64,   165,   166,     8,     9,     3,    64,    65,   148,    60,
      69,    70,   146,    64,    64,    65,   154,    64,   655,   153,
       4,    59,   186,   154,     8,     9,    64,   165,   166,   147,
      77,   171,   172,   262,   263,     0,   265,   173,   137,    64,
     204,   147,    92,   148,   150,    64,    71,   154,   545,   148,
     154,    64,    83,   148,    64,    64,    64,   866,   165,   166,
     196,   327,   154,     3,    78,     4,   202,    77,   204,   834,
     169,   170,   154,   154,   676,   200,   201,   202,   203,   204,
      64,   206,   148,   148,   148,   135,   146,   137,   150,    62,
     148,    64,   146,   153,   696,   148,   260,   261,   148,   153,
     146,   148,   152,     3,   154,   152,   150,   154,   148,    84,
     163,   164,   148,   163,   164,   165,   166,   739,   148,   169,
     170,   171,   172,   737,   154,    64,    65,   163,   164,   353,
     177,   178,   148,   163,   164,   165,   166,   735,   148,   148,
     372,   148,   619,   148,   154,   154,   199,   163,   164,   199,
     148,   147,   146,   573,   420,   387,   385,   386,   555,   153,
     571,   663,   394,   199,    64,   657,   398,   396,   397,   199,
     402,     3,   146,   685,   156,   173,   174,    77,   407,   153,
       4,   412,   146,   199,     8,     9,   417,   418,   419,   153,
     326,   103,   362,   105,   423,   146,   332,   333,   149,   335,
     336,   337,   338,   339,   146,   202,   203,   204,   148,   148,
     147,   153,   152,   437,   147,   154,   352,   353,   450,   128,
     146,   148,   719,   149,   163,   164,   165,   166,     4,   365,
      15,    16,    64,   173,   174,    59,    60,    61,   374,     3,
      64,    65,   151,   379,   147,    64,    65,   383,   148,    68,
      69,   202,   203,   204,   154,   484,   147,     4,    82,   656,
     199,     8,     9,   147,   146,   494,   495,   149,   404,    10,
      11,    12,    13,    97,    98,   147,   146,   177,   178,   149,
      63,    64,   106,   107,   108,   109,   110,   149,    64,    65,
     892,   200,   201,   202,   203,   204,   147,    47,   147,   123,
      64,   125,   147,    64,   801,    66,    67,     3,    33,    34,
      35,     7,   448,   137,    64,    65,   148,   147,   137,   147,
     152,   147,   154,   146,   148,   147,   149,   147,   205,   148,
     200,   201,   202,   203,   204,    68,    69,   734,   200,   201,
     202,   203,   204,   175,   176,   169,   170,   159,   160,   147,
     169,   170,   147,   577,   130,   131,   132,   133,   134,   147,
     136,   137,   138,   139,   140,   141,   142,   147,    64,    65,
     147,     3,   148,   147,   146,   199,   147,   200,   201,   202,
     203,   204,   147,   649,   148,   147,   146,   137,   147,   147,
     154,   147,   624,   169,   170,   147,   628,    75,   148,   631,
     632,   147,   147,   179,   146,     3,     4,   147,   147,   147,
     147,   175,   176,   147,   205,   147,   682,   683,   684,   169,
     170,   147,   558,   147,   147,   147,   147,   651,   200,   201,
     202,   203,   204,   147,   130,   131,   132,   133,   134,   575,
     136,   137,   138,   139,   140,   141,   142,   151,   147,   150,
     150,   148,   148,   151,   148,   150,   146,   681,   200,   201,
     202,   203,   204,   687,   151,   862,    64,    65,   152,   151,
     206,   146,   869,   169,   170,   146,   145,   173,   174,     4,
     148,   146,   146,   179,   200,   201,   202,   203,   204,   150,
     626,   627,     3,   150,   630,     3,   200,   201,   202,   203,
     204,   898,   200,   201,   202,   203,   204,   151,   200,   201,
     202,   203,   204,   200,   201,   202,   203,   204,   200,   201,
     202,   203,   204,   150,   152,   791,   151,   146,   752,   146,
     146,   146,   130,   131,   132,   133,   134,   146,   136,   137,
     138,   139,   140,   141,   142,   148,   148,   813,   814,   815,
     148,   148,   148,   148,   690,   208,   200,   201,   202,   203,
     204,     4,   150,   146,   700,   146,   146,   146,     4,   146,
     148,   169,   170,   146,     6,   200,   201,   202,   203,   204,
     151,   179,    14,   146,     6,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,   151,     4,   146,    30,    31,
      32,   146,   146,   146,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,     4,    48,    49,    50,    51,
      52,    53,    54,    55,    56,   146,    58,   146,   146,   200,
     201,   202,   203,   204,   146,   151,   146,   146,     3,   775,
     146,   146,   146,   146,   200,   201,   202,   203,   204,    81,
     149,   146,   149,   147,   147,    87,   147,    89,    90,   147,
      92,     3,    94,    95,    96,   146,   151,    99,   100,   146,
     102,     5,   104,   902,   146,   155,   146,   146,    63,    63,
     143,   910,    63,    64,    65,   821,   146,   146,   149,   146,
      72,     4,   124,   146,   126,    72,    64,    65,   164,   157,
     166,   871,   168,    76,   170,   163,   172,   165,   174,   167,
     176,   169,   178,   171,   180,   173,   146,   175,   150,   177,
     890,   179,   146,   146,   156,   157,     4,   148,    91,   146,
       3,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   130,
     131,   132,   133,   134,   151,   136,   137,   138,   139,   140,
     141,   142,   130,   131,   132,   133,   134,   148,   136,   137,
     138,   139,   140,   141,   142,    66,    67,   153,   149,   146,
     148,     4,   154,   149,   154,   146,   146,   207,   169,   170,
     146,   146,   146,   146,    85,    86,   146,    88,   179,   150,
     146,   169,   170,   146,     4,   146,   150,   146,   146,     4,
     146,   179,     4,   146,     4,   146,   146,   146,   109,    63,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   157,   146,     4,   146,   146,    91,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,     3,   146,   206,   147,   146,
       4,   149,   148,   154,   146,     3,   149,   147,   149,   146,
     155,   147,   147,   147,   165,   166,   147,   149,     5,   146,
      63,   146,   148,   146,     4,     4,    72,   146,   146,   146,
     154,    72,   146,   146,   146,   146,   152,     4,     4,   149,
     149,   149,   149,   146,   146,   151,     4,   146,   146,     4,
       4,   146,   146,   149,     5,   147,   147,   147,   147,     3,
      33,   149,   146,    80,     4,   146,   146,   149,     7,    64,
     149,   149,   149,   146,   149,   147,   146,   153,     4,   146,
       4,   147,   146,   146,     4,   147,     5,   207,    33,    79,
     149,     4,   149,   149,     5,   149,    74,   146,   149,   144,
     909,   149,   149,    73,   146,   149,   146,   146,   146,   361,
     804,   208,    61,   187,    55,   422,   181,   479,   799,   790,
     833,   712,   152,   873,   636,   860,   366,   460,   472,   663,
     269,    -1,   448,    -1,   687,   484
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   210,   211,   212,   213,   147,   147,     0,
     211,    83,   220,   154,   154,   147,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    30,    31,
      32,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    58,    81,    87,    89,    90,    92,    94,    95,    96,
      99,   100,   102,   104,   124,   126,   150,   156,   157,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   214,   215,   222,
     224,   225,   226,   227,   229,   230,   238,   241,   242,   244,
     245,   252,   254,   255,   257,   259,   261,   265,   266,   267,
     270,   272,   279,   284,   287,   293,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   309,   310,   311,   312,
     313,   315,   316,   317,   318,   319,   322,   323,   324,   325,
     326,   327,   328,   329,   146,    84,   221,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   244,   147,   147,   147,   150,   157,
     226,   228,   246,   147,   156,     4,   148,   163,   164,   199,
     357,   367,   148,   222,   205,   147,   256,     3,   146,   154,
     165,   166,   343,   356,    10,    11,    12,    13,     4,    78,
     368,    64,    65,    68,    69,   137,   148,   169,   170,   285,
     345,   346,     3,    64,    77,   148,   154,   177,   178,   295,
     354,   355,   295,     3,    64,   148,   154,   175,   176,   289,
     352,   353,    64,   154,   330,    59,    64,   337,   345,   345,
     130,   131,   132,   133,   134,   136,   138,   139,   140,   141,
     142,   148,   179,   345,   365,   366,   148,   171,   172,   348,
     349,   345,   345,    75,   345,    47,   148,   345,   375,   376,
     348,   348,    64,   154,   269,   331,   148,   375,   375,   375,
     344,   357,   148,   268,   357,   345,   366,   366,   345,   366,
     366,     4,     8,     9,    64,   369,   269,   366,     5,    64,
     280,   262,   366,   348,   225,   344,   205,   150,   345,   148,
       8,   357,   150,   128,   151,   200,   201,   202,   203,   204,
     356,   357,   362,   363,   364,    21,    23,    27,    28,    29,
      92,   135,   148,   152,   345,   348,   356,   357,   371,   372,
     373,   260,   152,   223,   221,   150,   206,   146,   146,   146,
     145,   347,   148,     4,   150,   146,   146,   274,     3,   150,
     296,   294,     3,   150,   288,   146,   146,   146,   146,   146,
     348,   348,   345,   345,   148,   345,   148,   148,   148,   365,
     148,   366,   208,     4,   150,   146,   146,   146,   146,   148,
       4,   146,   146,   146,     6,   243,     4,   146,   146,   146,
     146,     4,   146,   146,   146,   146,   146,   146,   146,   146,
     223,   263,   146,   146,   151,   357,   344,   146,   239,   149,
     149,   357,   357,   357,   357,   357,   357,   357,   146,   149,
     147,     4,   234,   235,   236,   237,   346,   349,   356,   357,
     147,   147,   147,   224,   222,   357,   343,     3,     3,   148,
     314,     3,   146,   151,   366,   146,   357,    69,    70,   286,
       5,   155,   146,   357,   146,   223,   146,   357,   223,   345,
     345,   366,    63,    63,   146,   146,    63,   282,   366,   345,
     345,     3,     7,   148,   173,   174,   350,   351,   366,   143,
     149,   366,   146,   357,    60,    64,   338,    64,    66,    67,
     339,   342,   345,    72,     4,   146,    72,   375,    64,   148,
     275,   146,   146,   375,   375,   375,   344,   146,   268,   369,
     345,    62,    64,   374,    76,   148,    64,   148,   276,     4,
     337,   264,   148,   350,   350,   258,   101,   206,   253,   151,
      64,   356,   227,   359,   360,   361,   151,   151,   362,   366,
     146,   146,   153,   146,   153,   146,   153,   146,   153,   152,
     354,   152,   352,   152,   350,   153,   151,     3,   146,   347,
     149,     4,   151,   274,   154,   151,     4,     8,     9,    59,
      60,    61,    82,    97,    98,   106,   107,   108,   109,   110,
     123,   125,   297,   345,   367,   370,   154,   151,   290,   146,
     146,   345,   345,   207,   146,   149,   146,   146,   146,     3,
     150,   146,   146,   146,     4,   151,   146,   150,   146,   146,
       4,   146,     4,   146,   338,     4,   146,   146,   308,   146,
      63,   146,   369,     4,   146,   146,    91,   146,   223,   206,
     357,   247,   147,   146,   149,     4,   349,   346,   357,   356,
     231,   355,   232,   353,   233,   351,   146,     3,   149,   155,
     149,   147,   147,   147,   147,   147,   149,   146,   337,     5,
     146,   282,   366,   357,   357,   366,   146,   357,   366,   366,
     148,   146,   331,     4,    64,    71,   341,     4,   146,    72,
     146,    64,   148,   277,   154,    72,   344,   146,   356,   146,
     146,    64,   148,   278,    64,   281,   282,   223,   350,   152,
     159,   160,   358,   359,   240,   146,   153,   146,   153,   146,
     153,   338,   146,   356,   344,   344,   344,   352,    66,    67,
      85,    86,    88,   109,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   291,   340,   342,   356,   146,
     218,   357,   149,   146,   149,   146,   149,   338,   151,   149,
     149,   357,     4,   146,   332,   151,   146,     4,     4,   285,
     146,   146,     4,    64,   283,   369,     4,     4,   332,   146,
     146,   103,   105,   248,   249,   223,   355,   353,   351,   149,
       5,   147,   356,   147,   147,   147,   147,     3,    33,   357,
     149,   201,   146,    33,    34,    35,   333,   334,    80,   335,
       4,   146,   149,   146,   278,   344,   149,   149,   149,   146,
     335,   283,     7,   369,   147,   153,   248,   146,    64,   344,
     344,   344,   122,   154,   292,   149,   146,   149,   357,     4,
     146,   147,   320,   146,     4,   286,   332,   146,     4,   271,
     147,   251,     5,   207,    33,   146,   216,   149,   333,    79,
     336,   223,   321,     4,   149,   149,   335,    74,   149,   223,
     250,   222,   146,   292,   149,   144,   149,   149,   273,   146,
     222,   338,   146,   223,    73,   146,   345,     5,    64,   219,
     146,   217,   218,   345,   149
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
		      long rflags = (yyvsp[(8) - (9)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9((yyvsp[(9) - (9)].i), rflags, ERR, ERR,
					  (yyvsp[(5) - (9)].crd).x, (yyvsp[(5) - (9)].crd).y, (yyvsp[(7) - (9)].sze).width, (yyvsp[(7) - (9)].sze).height,
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
		      long rflags = (yyvsp[(8) - (11)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9((yyvsp[(9) - (11)].i), rflags,
					  (yyvsp[(7) - (11)].crd).x, (yyvsp[(7) - (11)].crd).y, (yyvsp[(5) - (11)].crd).x, (yyvsp[(5) - (11)].crd).y,
					  (yyvsp[(9) - (11)].sze).width, (yyvsp[(9) - (11)].sze).height, SPO_ROOM));
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
		      long rt = (yyvsp[(7) - (9)].i);
		      long rflags = (yyvsp[(8) - (9)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      if (!(rflags & 1)) rt += MAXRTYPE+1;
		      irr = ((rflags & 2) != 0);
		      add_opvars(splev, "iiiio",
				 VA_PASS5((long)(yyvsp[(5) - (9)].i), rt, rflags, (yyvsp[(9) - (9)].i), SPO_REGION));
		      (yyval.i) = (irr || (rflags & 1) || rt != OROOM);
		      break_stmt_start();
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1944 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      if ( (yyvsp[(9) - (11)].i) ) {
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      } else if ( (yyvsp[(10) - (11)].i) )
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

  case 303:

/* Line 1455 of yacc.c  */
#line 2088 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2092 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2098 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2114 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2122 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2132 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2141 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2146 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2153 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2164 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2170 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2174 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2178 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2185 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2195 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2202 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2206 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2212 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2216 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2224 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2228 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2235 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2245 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2258 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2262 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2269 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2279 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2288 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2299 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2303 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2310 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2320 "lev_comp.y"
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

  case 345:

/* Line 1455 of yacc.c  */
#line 2331 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2340 "lev_comp.y"
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

  case 347:

/* Line 1455 of yacc.c  */
#line 2350 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2356 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2360 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2367 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2377 "lev_comp.y"
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

  case 352:

/* Line 1455 of yacc.c  */
#line 2388 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2397 "lev_comp.y"
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

  case 354:

/* Line 1455 of yacc.c  */
#line 2407 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2411 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2417 "lev_comp.y"
    { ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2419 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2425 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2429 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2433 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2437 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2445 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2454 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2458 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2462 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2466 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2470 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2473 "lev_comp.y"
    { ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2477 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2484 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2493 "lev_comp.y"
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

  case 376:

/* Line 1455 of yacc.c  */
#line 2534 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2538 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2545 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2552 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2562 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2566 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2574 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2578 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2582 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2586 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2590 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2595 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2599 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2603 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2608 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2613 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2618 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2622 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2627 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2632 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2636 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2640 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2645 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2649 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2656 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2662 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2666 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2672 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2688 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2692 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2696 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2700 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2722 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2726 "lev_comp.y"
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

  case 423:

/* Line 1455 of yacc.c  */
#line 2752 "lev_comp.y"
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
#line 6200 "lev_comp.tab.c"
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
#line 2780 "lev_comp.y"


/*lev_comp.y*/

