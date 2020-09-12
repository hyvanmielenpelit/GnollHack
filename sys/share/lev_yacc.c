
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
     FLOOR_ID = 336,
     FLOOR_MAIN_TYPE = 337,
     SUBROOM_ID = 338,
     NAME_ID = 339,
     FLAGS_ID = 340,
     FLAG_TYPE = 341,
     MON_ATTITUDE = 342,
     MON_ALERTNESS = 343,
     SUBTYPE_ID = 344,
     MON_APPEARANCE = 345,
     ROOMDOOR_ID = 346,
     IF_ID = 347,
     ELSE_ID = 348,
     TERRAIN_ID = 349,
     HORIZ_OR_VERT = 350,
     REPLACE_TERRAIN_ID = 351,
     EXIT_ID = 352,
     SHUFFLE_ID = 353,
     QUANTITY_ID = 354,
     BURIED_ID = 355,
     LOOP_ID = 356,
     FOR_ID = 357,
     TO_ID = 358,
     SWITCH_ID = 359,
     CASE_ID = 360,
     BREAK_ID = 361,
     DEFAULT_ID = 362,
     ERODED_ID = 363,
     TRAPPED_STATE = 364,
     RECHARGED_ID = 365,
     INVIS_ID = 366,
     GREASED_ID = 367,
     FEMALE_ID = 368,
     WAITFORU_ID = 369,
     CANCELLED_ID = 370,
     REVIVED_ID = 371,
     AVENGE_ID = 372,
     FLEEING_ID = 373,
     BLINDED_ID = 374,
     PARALYZED_ID = 375,
     STUNNED_ID = 376,
     CONFUSED_ID = 377,
     SEENTRAPS_ID = 378,
     ALL_ID = 379,
     MONTYPE_ID = 380,
     GRAVE_ID = 381,
     ERODEPROOF_ID = 382,
     FUNCTION_ID = 383,
     MSG_OUTPUT_TYPE = 384,
     COMPARE_TYPE = 385,
     UNKNOWN_TYPE = 386,
     rect_ID = 387,
     fillrect_ID = 388,
     line_ID = 389,
     randline_ID = 390,
     grow_ID = 391,
     selection_ID = 392,
     flood_ID = 393,
     rndcoord_ID = 394,
     circle_ID = 395,
     ellipse_ID = 396,
     filter_ID = 397,
     complement_ID = 398,
     gradient_ID = 399,
     GRADIENT_TYPE = 400,
     LIMITED = 401,
     HUMIDITY_TYPE = 402,
     STRING = 403,
     MAP_ID = 404,
     NQSTRING = 405,
     VARSTRING = 406,
     CFUNC = 407,
     CFUNC_INT = 408,
     CFUNC_STR = 409,
     CFUNC_COORD = 410,
     CFUNC_REGION = 411,
     VARSTRING_INT = 412,
     VARSTRING_INT_ARRAY = 413,
     VARSTRING_STRING = 414,
     VARSTRING_STRING_ARRAY = 415,
     VARSTRING_VAR = 416,
     VARSTRING_VAR_ARRAY = 417,
     VARSTRING_COORD = 418,
     VARSTRING_COORD_ARRAY = 419,
     VARSTRING_REGION = 420,
     VARSTRING_REGION_ARRAY = 421,
     VARSTRING_MAPCHAR = 422,
     VARSTRING_MAPCHAR_ARRAY = 423,
     VARSTRING_MONST = 424,
     VARSTRING_MONST_ARRAY = 425,
     VARSTRING_OBJ = 426,
     VARSTRING_OBJ_ARRAY = 427,
     VARSTRING_SEL = 428,
     VARSTRING_SEL_ARRAY = 429,
     METHOD_INT = 430,
     METHOD_INT_ARRAY = 431,
     METHOD_STRING = 432,
     METHOD_STRING_ARRAY = 433,
     METHOD_VAR = 434,
     METHOD_VAR_ARRAY = 435,
     METHOD_COORD = 436,
     METHOD_COORD_ARRAY = 437,
     METHOD_REGION = 438,
     METHOD_REGION_ARRAY = 439,
     METHOD_MAPCHAR = 440,
     METHOD_MAPCHAR_ARRAY = 441,
     METHOD_MONST = 442,
     METHOD_MONST_ARRAY = 443,
     METHOD_OBJ = 444,
     METHOD_OBJ_ARRAY = 445,
     METHOD_SEL = 446,
     METHOD_SEL_ARRAY = 447,
     DICE = 448
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
#line 491 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 503 "lev_comp.tab.c"

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
#define YYLAST   1080

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  211
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  169
/* YYNRULES -- Number of rules.  */
#define YYNRULES  425
/* YYNRULES -- Number of states.  */
#define YYNSTATES  923

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   448

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   206,   210,     2,
     150,   151,   204,   202,   148,   203,   208,   205,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   149,     2,
       2,   207,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   152,     2,   153,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   154,   209,   155,     2,     2,     2,     2,
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
     145,   146,   147,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201
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
     239,   241,   243,   245,   249,   253,   259,   263,   269,   275,
     281,   285,   289,   295,   301,   307,   315,   323,   331,   337,
     339,   343,   345,   349,   351,   355,   357,   361,   363,   367,
     369,   373,   375,   379,   380,   381,   390,   395,   397,   398,
     400,   402,   408,   412,   413,   414,   424,   425,   428,   429,
     435,   436,   441,   443,   446,   448,   455,   456,   460,   461,
     468,   469,   474,   475,   480,   482,   483,   488,   492,   494,
     498,   502,   508,   514,   522,   527,   528,   540,   541,   555,
     556,   559,   565,   567,   573,   575,   581,   583,   589,   591,
     601,   607,   609,   611,   613,   615,   617,   621,   623,   625,
     627,   635,   641,   643,   645,   647,   649,   653,   654,   660,
     665,   666,   670,   672,   674,   676,   678,   681,   683,   685,
     687,   689,   691,   695,   699,   703,   705,   707,   711,   713,
     715,   717,   721,   725,   726,   732,   735,   736,   740,   742,
     746,   748,   752,   756,   758,   760,   764,   766,   768,   770,
     774,   776,   778,   780,   786,   794,   800,   809,   811,   815,
     821,   827,   835,   843,   850,   856,   857,   860,   866,   870,
     884,   888,   892,   894,   900,   910,   916,   920,   924,   925,
     937,   938,   940,   948,   952,   960,   966,   972,   978,   984,
     988,   994,  1002,  1012,  1014,  1016,  1018,  1020,  1022,  1023,
    1026,  1028,  1032,  1034,  1036,  1038,  1039,  1043,  1045,  1047,
    1049,  1051,  1053,  1055,  1057,  1059,  1061,  1063,  1067,  1069,
    1071,  1076,  1078,  1080,  1085,  1087,  1089,  1094,  1096,  1101,
    1107,  1109,  1113,  1115,  1119,  1121,  1123,  1128,  1138,  1140,
    1142,  1147,  1149,  1155,  1157,  1159,  1164,  1166,  1168,  1174,
    1176,  1178,  1180,  1185,  1187,  1189,  1195,  1197,  1199,  1201,
    1205,  1207,  1209,  1213,  1215,  1220,  1224,  1228,  1232,  1236,
    1240,  1244,  1246,  1248,  1252,  1254,  1258,  1259,  1261,  1263,
    1265,  1267,  1271,  1272,  1274,  1276,  1279,  1282,  1287,  1294,
    1299,  1306,  1313,  1320,  1327,  1330,  1337,  1346,  1355,  1366,
    1381,  1384,  1386,  1390,  1392,  1396,  1398,  1400,  1402,  1404,
    1406,  1408,  1410,  1412,  1414,  1416,  1418,  1420,  1422,  1424,
    1426,  1428,  1430,  1432,  1434,  1445
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     212,     0,    -1,    -1,   213,    -1,   214,    -1,   214,   213,
      -1,   215,   222,   224,    -1,    16,   149,   156,    -1,    15,
     149,   156,   148,     3,    -1,    17,   149,    11,   148,   316,
      -1,    17,   149,    10,   148,     3,    -1,    17,   149,    13,
      -1,    17,   149,    12,   148,     3,   148,     3,   148,     5,
     148,     5,   148,   341,   148,   221,   220,    -1,    18,   149,
     371,    -1,    -1,   148,   146,    -1,    -1,   148,   348,    -1,
      -1,   148,     3,    -1,     5,    -1,    64,    -1,    -1,    85,
     149,   223,    -1,    86,   148,   223,    -1,    86,    -1,    -1,
     226,   224,    -1,   154,   224,   155,    -1,   267,    -1,   216,
      -1,   217,    -1,   327,    -1,   326,    -1,   324,    -1,   325,
      -1,   328,    -1,   329,    -1,   309,    -1,   269,    -1,   232,
      -1,   231,    -1,   319,    -1,   281,    -1,   301,    -1,   331,
      -1,   332,    -1,   311,    -1,   312,    -1,   313,    -1,   330,
      -1,   247,    -1,   257,    -1,   259,    -1,   263,    -1,   261,
      -1,   244,    -1,   254,    -1,   240,    -1,   243,    -1,   304,
      -1,   286,    -1,   302,    -1,   289,    -1,   295,    -1,   320,
      -1,   315,    -1,   307,    -1,   268,    -1,   321,    -1,   274,
      -1,   272,    -1,   314,    -1,   318,    -1,   317,    -1,   305,
      -1,   306,    -1,   308,    -1,   300,    -1,   303,    -1,   166,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,   176,
      -1,   178,    -1,   180,    -1,   182,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,   177,
      -1,   179,    -1,   181,    -1,   227,    -1,   228,    -1,   159,
      -1,   159,    -1,   228,    -1,    98,   149,   227,    -1,   229,
     207,   360,    -1,   229,   207,   137,   149,   369,    -1,   229,
     207,   359,    -1,   229,   207,   376,   149,   353,    -1,   229,
     207,   375,   149,   355,    -1,   229,   207,   374,   149,   357,
      -1,   229,   207,   348,    -1,   229,   207,   351,    -1,   229,
     207,   154,   238,   155,    -1,   229,   207,   154,   237,   155,
      -1,   229,   207,   154,   236,   155,    -1,   229,   207,   376,
     149,   154,   235,   155,    -1,   229,   207,   375,   149,   154,
     234,   155,    -1,   229,   207,   374,   149,   154,   233,   155,
      -1,   229,   207,   154,   239,   155,    -1,   358,    -1,   233,
     148,   358,    -1,   356,    -1,   234,   148,   356,    -1,   354,
      -1,   235,   148,   354,    -1,   352,    -1,   236,   148,   352,
      -1,   349,    -1,   237,   148,   349,    -1,   360,    -1,   238,
     148,   360,    -1,   359,    -1,   239,   148,   359,    -1,    -1,
      -1,   128,   158,   150,   241,   364,   151,   242,   225,    -1,
     158,   150,   367,   151,    -1,    97,    -1,    -1,     6,    -1,
       6,    -1,   152,   360,   130,   360,   153,    -1,   152,   360,
     153,    -1,    -1,    -1,   104,   248,   152,   347,   153,   249,
     154,   250,   155,    -1,    -1,   251,   250,    -1,    -1,   105,
     372,   149,   252,   224,    -1,    -1,   107,   149,   253,   224,
      -1,   106,    -1,   208,   208,    -1,   103,    -1,   102,   230,
     207,   360,   255,   360,    -1,    -1,   256,   258,   225,    -1,
      -1,   101,   152,   347,   153,   260,   225,    -1,    -1,   246,
     149,   262,   226,    -1,    -1,    92,   246,   264,   265,    -1,
     225,    -1,    -1,   225,   266,    93,   225,    -1,    14,   149,
     359,    -1,    58,    -1,    58,   149,   372,    -1,    58,   149,
      64,    -1,    49,   149,   270,   148,   270,    -1,    49,   149,
     270,   148,   372,    -1,   150,     4,   148,    63,   148,   285,
     151,    -1,   334,   245,   148,   341,    -1,    -1,    83,   149,
     271,   148,   278,   148,   280,   335,   338,   273,   225,    -1,
      -1,    43,   149,   271,   148,   277,   148,   279,   148,   280,
     335,   338,   275,   225,    -1,    -1,   148,     5,    -1,   150,
       4,   148,     4,   151,    -1,    64,    -1,   150,     4,   148,
       4,   151,    -1,    64,    -1,   150,   287,   148,   288,   151,
      -1,    64,    -1,   150,     4,   148,     4,   151,    -1,    64,
      -1,    91,   149,   282,   148,   340,   148,   283,   148,   285,
      -1,    25,   149,   340,   148,   369,    -1,     5,    -1,    64,
      -1,   284,    -1,    64,    -1,    63,    -1,    63,   209,   284,
      -1,     4,    -1,    64,    -1,    20,    -1,    19,   149,   287,
     148,   288,   276,   157,    -1,    19,   149,   348,   276,   157,
      -1,    68,    -1,    69,    -1,    70,    -1,    69,    -1,    23,
     149,   291,    -1,    -1,    23,   149,   291,   290,   225,    -1,
     355,   148,   348,   292,    -1,    -1,   292,   148,   293,    -1,
     359,    -1,    87,    -1,    88,    -1,   343,    -1,    90,   359,
      -1,   113,    -1,   111,    -1,   115,    -1,   116,    -1,   117,
      -1,   118,   149,   347,    -1,   119,   149,   347,    -1,   120,
     149,   347,    -1,   121,    -1,   122,    -1,   123,   149,   294,
      -1,   114,    -1,   156,    -1,   124,    -1,   156,   209,   294,
      -1,    21,   149,   297,    -1,    -1,    22,   149,   297,   296,
     225,    -1,   357,   298,    -1,    -1,   298,   148,   299,    -1,
      61,    -1,   125,   149,   355,    -1,   373,    -1,    84,   149,
     359,    -1,    99,   149,   347,    -1,   100,    -1,    60,    -1,
     108,   149,   347,    -1,   127,    -1,    59,    -1,   109,    -1,
     110,   149,   347,    -1,   111,    -1,   112,    -1,   348,    -1,
      24,   149,   333,   148,   348,    -1,    26,   149,   348,   148,
      63,   148,   340,    -1,    30,   149,   348,   148,    63,    -1,
      30,   149,   348,   148,    63,   148,     5,   220,    -1,    31,
      -1,    31,   149,   369,    -1,    39,   149,   348,   148,    72,
      -1,    40,   149,   348,   148,    72,    -1,    40,   149,   378,
     148,   378,   148,    72,    -1,    44,   149,   378,   148,   378,
     148,   156,    -1,    45,   149,   378,   148,   378,   310,    -1,
      46,   149,   378,   148,   378,    -1,    -1,   148,    72,    -1,
      52,   149,   369,   148,    76,    -1,    53,   149,   369,    -1,
      54,   149,   348,   148,   150,   372,   148,   372,   151,   148,
      74,   148,    73,    -1,    56,   149,   369,    -1,    55,   149,
     369,    -1,     3,    -1,   150,     3,   148,   341,   151,    -1,
      96,   149,   351,   148,   353,   148,   353,   148,     7,    -1,
      94,   149,   369,   148,   353,    -1,    41,   149,   351,    -1,
      42,   149,   351,    -1,    -1,    32,   149,   351,   148,   341,
     148,   334,   335,   338,   322,   323,    -1,    -1,   225,    -1,
      36,   149,   348,   148,   342,   148,   344,    -1,    37,   149,
     348,    -1,    81,   149,   369,   148,    82,   148,    79,    -1,
      89,   149,   369,   148,     4,    -1,    38,   149,    75,   148,
     348,    -1,   126,   149,   348,   148,   359,    -1,   126,   149,
     348,   148,    64,    -1,   126,   149,   348,    -1,    50,   149,
     360,   148,   348,    -1,    51,   149,   348,   148,   377,   148,
     359,    -1,    48,   149,   347,   148,   347,   148,   347,   148,
     347,    -1,    48,    -1,   156,    -1,    64,    -1,   156,    -1,
      64,    -1,    -1,   148,   336,    -1,   337,    -1,   337,   148,
     336,    -1,    33,    -1,    34,    -1,    35,    -1,    -1,    80,
     149,   339,    -1,    79,    -1,    59,    -1,    64,    -1,    60,
      -1,    64,    -1,    67,    -1,   345,    -1,    64,    -1,    67,
      -1,   345,    -1,    66,   149,    64,    -1,    71,    -1,    64,
      -1,    66,   152,     4,   153,    -1,   156,    -1,   167,    -1,
     168,   152,   360,   153,    -1,   360,    -1,   349,    -1,   139,
     150,   369,   151,    -1,   171,    -1,   172,   152,   360,   153,
      -1,   150,     4,   148,     4,   151,    -1,    64,    -1,    65,
     350,   153,    -1,   147,    -1,   147,   148,   350,    -1,   352,
      -1,   173,    -1,   174,   152,   360,   153,    -1,   150,     4,
     148,     4,   148,     4,   148,     4,   151,    -1,   354,    -1,
     175,    -1,   176,   152,   360,   153,    -1,     3,    -1,   150,
       3,   148,   341,   151,    -1,   356,    -1,   177,    -1,   178,
     152,   360,   153,    -1,   156,    -1,     3,    -1,   150,     3,
     148,   156,   151,    -1,    64,    -1,   358,    -1,   179,    -1,
     180,   152,   360,   153,    -1,   156,    -1,     3,    -1,   150,
       3,   148,   156,   151,    -1,    64,    -1,    77,    -1,   346,
      -1,   359,   208,   346,    -1,     4,    -1,   370,    -1,   150,
       8,   151,    -1,   165,    -1,   166,   152,   360,   153,    -1,
     360,   202,   360,    -1,   360,   203,   360,    -1,   360,   204,
     360,    -1,   360,   205,   360,    -1,   360,   206,   360,    -1,
     150,   360,   151,    -1,   161,    -1,   162,    -1,   229,   149,
     361,    -1,   362,    -1,   363,   148,   362,    -1,    -1,   363,
      -1,   360,    -1,   359,    -1,   365,    -1,   366,   148,   365,
      -1,    -1,   366,    -1,   348,    -1,   132,   351,    -1,   133,
     351,    -1,   134,   348,   148,   348,    -1,   135,   348,   148,
     348,   148,   360,    -1,   136,   150,   369,   151,    -1,   136,
     150,   284,   148,   369,   151,    -1,   142,   150,     7,   148,
     369,   151,    -1,   142,   150,   369,   148,   369,   151,    -1,
     142,   150,   353,   148,   369,   151,    -1,   138,   348,    -1,
     140,   150,   348,   148,   360,   151,    -1,   140,   150,   348,
     148,   360,   148,    33,   151,    -1,   141,   150,   348,   148,
     360,   148,   360,   151,    -1,   141,   150,   348,   148,   360,
     148,   360,   148,    33,   151,    -1,   144,   150,   145,   148,
     150,   360,   203,   360,   218,   151,   148,   348,   219,   151,
      -1,   143,   368,    -1,   181,    -1,   150,   369,   151,    -1,
     368,    -1,   368,   210,   369,    -1,   201,    -1,    78,    -1,
       4,    -1,     8,    -1,     9,    -1,     4,    -1,     8,    -1,
       9,    -1,     4,    -1,   370,    -1,    27,    -1,    21,    -1,
      28,    -1,    23,    -1,    29,    -1,    94,    -1,    62,    -1,
      64,    -1,   379,    -1,    47,   150,     4,   148,     4,   148,
       4,   148,     4,   151,    -1,   150,     4,   148,     4,   148,
       4,   148,     4,   151,    -1
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
     519,   520,   521,   522,   523,   524,   525,   526,   527,   528,
     531,   532,   533,   534,   535,   536,   537,   538,   539,   542,
     543,   544,   545,   546,   547,   548,   549,   550,   553,   554,
     555,   558,   559,   562,   578,   584,   590,   596,   602,   608,
     614,   620,   626,   636,   646,   656,   666,   676,   686,   698,
     703,   710,   715,   722,   727,   734,   738,   744,   749,   756,
     760,   766,   770,   777,   799,   776,   813,   868,   875,   878,
     884,   891,   895,   904,   908,   903,   971,   972,   976,   975,
     989,   988,  1003,  1013,  1014,  1017,  1055,  1054,  1089,  1088,
    1119,  1118,  1151,  1150,  1176,  1187,  1186,  1214,  1220,  1225,
    1230,  1237,  1244,  1253,  1261,  1273,  1272,  1291,  1290,  1309,
    1312,  1318,  1328,  1334,  1343,  1349,  1354,  1360,  1365,  1371,
    1382,  1388,  1389,  1392,  1393,  1396,  1400,  1406,  1407,  1410,
    1417,  1425,  1433,  1434,  1437,  1438,  1441,  1446,  1445,  1459,
    1466,  1473,  1481,  1486,  1492,  1498,  1504,  1510,  1515,  1520,
    1525,  1530,  1535,  1540,  1545,  1550,  1555,  1560,  1566,  1573,
    1582,  1586,  1599,  1608,  1607,  1625,  1635,  1641,  1649,  1655,
    1660,  1665,  1670,  1675,  1680,  1685,  1690,  1695,  1706,  1712,
    1717,  1722,  1727,  1734,  1740,  1769,  1774,  1782,  1788,  1794,
    1801,  1808,  1818,  1828,  1843,  1854,  1857,  1863,  1869,  1875,
    1881,  1887,  1893,  1898,  1905,  1912,  1918,  1924,  1931,  1930,
    1955,  1958,  1964,  1971,  1977,  1983,  1989,  1995,  1999,  2004,
    2011,  2017,  2024,  2028,  2035,  2043,  2046,  2056,  2060,  2063,
    2069,  2073,  2080,  2084,  2088,  2095,  2098,  2104,  2110,  2111,
    2114,  2115,  2118,  2119,  2120,  2126,  2127,  2128,  2134,  2135,
    2138,  2147,  2152,  2159,  2170,  2176,  2180,  2184,  2191,  2201,
    2208,  2212,  2218,  2222,  2230,  2234,  2241,  2251,  2264,  2268,
    2275,  2285,  2294,  2305,  2309,  2316,  2326,  2337,  2346,  2356,
    2362,  2366,  2373,  2383,  2394,  2403,  2413,  2417,  2424,  2425,
    2431,  2435,  2439,  2443,  2451,  2460,  2464,  2468,  2472,  2476,
    2480,  2483,  2490,  2499,  2532,  2533,  2536,  2537,  2540,  2544,
    2551,  2558,  2569,  2572,  2580,  2584,  2588,  2592,  2596,  2601,
    2605,  2609,  2614,  2619,  2624,  2628,  2633,  2638,  2642,  2646,
    2651,  2655,  2662,  2668,  2672,  2678,  2685,  2686,  2689,  2690,
    2691,  2694,  2698,  2702,  2706,  2712,  2713,  2716,  2717,  2720,
    2721,  2724,  2725,  2728,  2732,  2758
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
  "FLOOR_TYPE", "FLOOR_TYPE_ID", "FLOOR_ID", "FLOOR_MAIN_TYPE",
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
  "region_detail_end", "altar_detail", "anvil_detail", "floor_detail",
  "subtype_detail", "npc_detail", "grave_detail", "gold_detail",
  "engraving_detail", "mineralize", "trap_name", "room_type",
  "optroomregionflags", "roomregionflags", "roomregionflag",
  "optfloortype", "floortype", "door_state", "light_state", "alignment",
  "alignment_prfx", "altar_type", "a_register", "string_or_var",
  "integer_or_var", "coord_or_var", "encodecoord", "humidity_flags",
  "region_or_var", "encoderegion", "mapchar_or_var", "mapchar",
  "monster_or_var", "encodemonster", "object_or_var", "encodeobj",
  "string_expr", "math_expr_var", "func_param_type", "func_param_part",
  "func_param_list", "func_params_list", "func_call_param_part",
  "func_call_param_list", "func_call_params_list", "ter_selection_x",
  "ter_selection", "dice", "tileset_number", "all_integers",
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
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,    44,    58,
      40,    41,    91,    93,   123,   125,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,    43,    45,    42,    47,    37,    61,    46,   124,
      38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   211,   212,   212,   213,   213,   214,   215,   215,   216,
     216,   216,   216,   217,   218,   218,   219,   219,   220,   220,
     221,   221,   222,   222,   223,   223,   224,   224,   225,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   229,   229,
     229,   230,   230,   231,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   233,
     233,   234,   234,   235,   235,   236,   236,   237,   237,   238,
     238,   239,   239,   241,   242,   240,   243,   244,   245,   245,
     246,   246,   246,   248,   249,   247,   250,   250,   252,   251,
     253,   251,   254,   255,   255,   256,   258,   257,   260,   259,
     262,   261,   264,   263,   265,   266,   265,   267,   268,   268,
     268,   269,   269,   270,   271,   273,   272,   275,   274,   276,
     276,   277,   277,   278,   278,   279,   279,   280,   280,   281,
     281,   282,   282,   283,   283,   284,   284,   285,   285,   286,
     286,   286,   287,   287,   288,   288,   289,   290,   289,   291,
     292,   292,   293,   293,   293,   293,   293,   293,   293,   293,
     293,   293,   293,   293,   293,   293,   293,   293,   293,   294,
     294,   294,   295,   296,   295,   297,   298,   298,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   300,   301,   302,   302,   303,   303,   304,
     305,   306,   307,   308,   309,   310,   310,   311,   312,   313,
     314,   315,   316,   316,   317,   318,   319,   320,   322,   321,
     323,   323,   324,   325,   326,   327,   328,   329,   329,   329,
     330,   331,   332,   332,   333,   333,   334,   334,   335,   335,
     336,   336,   337,   337,   337,   338,   338,   339,   340,   340,
     341,   341,   342,   342,   342,   343,   343,   343,   344,   344,
     345,   346,   346,   346,   347,   348,   348,   348,   348,   349,
     349,   349,   350,   350,   351,   351,   351,   352,   353,   353,
     353,   354,   354,   355,   355,   355,   356,   356,   356,   356,
     357,   357,   357,   358,   358,   358,   358,   358,   359,   359,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   361,   361,   362,   363,   363,   364,   364,   365,   365,
     366,   366,   367,   367,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   369,   369,   370,   371,   371,   372,   372,
     372,   373,   373,   373,   373,   374,   374,   375,   375,   376,
     376,   377,   377,   378,   378,   379
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
       1,     1,     1,     3,     3,     5,     3,     5,     5,     5,
       3,     3,     5,     5,     5,     7,     7,     7,     5,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     0,     0,     8,     4,     1,     0,     1,
       1,     5,     3,     0,     0,     9,     0,     2,     0,     5,
       0,     4,     1,     2,     1,     6,     0,     3,     0,     6,
       0,     4,     0,     4,     1,     0,     4,     3,     1,     3,
       3,     5,     5,     7,     4,     0,    11,     0,    13,     0,
       2,     5,     1,     5,     1,     5,     1,     5,     1,     9,
       5,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       7,     5,     1,     1,     1,     1,     3,     0,     5,     4,
       0,     3,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     3,     3,     3,     1,     1,     3,     1,     1,
       1,     3,     3,     0,     5,     2,     0,     3,     1,     3,
       1,     3,     3,     1,     1,     3,     1,     1,     1,     3,
       1,     1,     1,     5,     7,     5,     8,     1,     3,     5,
       5,     7,     7,     6,     5,     0,     2,     5,     3,    13,
       3,     3,     1,     5,     9,     5,     3,     3,     0,    11,
       0,     1,     7,     3,     7,     5,     5,     5,     5,     3,
       5,     7,     9,     1,     1,     1,     1,     1,     0,     2,
       1,     3,     1,     1,     1,     0,     3,     1,     1,     1,
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
       2,     0,     0,     0,     3,     4,    22,     0,     0,     1,
       5,     0,    26,     0,     7,     0,   140,     0,     0,     0,
       0,   199,     0,     0,     0,     0,     0,     0,     0,   257,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   293,     0,     0,     0,     0,     0,     0,     0,
       0,   168,     0,     0,     0,     0,     0,     0,     0,   137,
       0,     0,     0,   143,   152,     0,     0,     0,     0,   100,
      89,    80,    90,    81,    91,    82,    92,    83,    93,    84,
      94,    85,    95,    86,    96,    87,    97,    88,    30,    31,
       6,    26,    98,    99,     0,    41,    40,    58,    59,    56,
       0,    51,    57,   156,    52,    53,    55,    54,    29,    68,
      39,    71,    70,    43,    61,    63,    64,    78,    44,    62,
      79,    60,    75,    76,    67,    77,    38,    47,    48,    49,
      72,    66,    74,    73,    42,    65,    69,    34,    35,    33,
      32,    36,    37,    50,    45,    46,     0,    25,    23,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   162,     0,     0,
       0,     0,   101,   102,     0,     0,     0,     0,   360,     0,
     363,     0,   405,     0,   361,   382,    27,     0,   160,     0,
       8,     0,   321,   322,     0,   358,   167,     0,     0,     0,
      11,   407,   406,    13,   330,     0,   202,   203,     0,     0,
     327,     0,     0,   179,   325,   354,   356,   357,     0,   353,
     351,     0,   232,   236,   350,   233,   347,   349,     0,   346,
     344,     0,   206,     0,   343,   295,   294,     0,   308,   309,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   401,   384,   403,   258,     0,
     335,     0,     0,   334,     0,   283,     0,     0,     0,     0,
       0,     0,   423,   276,   277,   297,   296,     0,   138,     0,
       0,     0,     0,     0,   324,     0,     0,     0,     0,     0,
     268,     0,   271,   270,   410,   408,   409,   170,   169,     0,
       0,     0,   191,   192,     0,     0,     0,     0,   103,     0,
       0,     0,   289,   133,     0,     0,     0,     0,   142,     0,
       0,     0,     0,     0,   379,   378,   380,   383,     0,   416,
     418,   415,   417,   419,   420,     0,     0,     0,   110,   111,
     106,   104,     0,     0,     0,     0,    26,   157,    24,     0,
       0,     0,     0,     0,   332,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   385,   386,     0,     0,     0,   394,
       0,     0,     0,   400,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   139,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   164,   163,     0,     0,   158,
       0,     0,     0,   376,   362,   370,     0,     0,   365,   366,
     367,   368,   369,     0,   136,     0,   360,     0,     0,     0,
       0,   127,   125,   131,   129,     0,     0,     0,   161,     0,
       0,   359,    10,   272,     0,     9,     0,     0,   331,     0,
       0,     0,   205,   204,   179,   180,   201,     0,     0,     0,
     234,     0,     0,   208,   210,   253,   190,     0,   255,     0,
       0,   195,     0,     0,     0,     0,   341,     0,     0,   339,
       0,     0,   338,     0,     0,   402,   404,     0,     0,   310,
     311,     0,   314,     0,   312,     0,   313,   286,   259,     0,
       0,   260,     0,   182,     0,     0,     0,     0,     0,   265,
     264,     0,     0,   171,   172,   290,   421,   422,     0,   267,
       0,     0,   184,     0,     0,   285,     0,     0,     0,   275,
       0,     0,   154,     0,     0,   144,   288,   287,     0,   374,
     377,     0,   364,   141,   381,   105,     0,     0,   114,     0,
     113,     0,   112,     0,   118,     0,   109,     0,   108,     0,
     107,    28,   323,     0,     0,   333,   326,     0,   328,     0,
       0,   352,   413,   411,   412,   247,   244,   238,     0,     0,
     243,     0,   248,     0,   250,   251,     0,   246,   237,   252,
     414,   240,     0,   345,   209,     0,     0,   387,     0,     0,
       0,   389,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   336,     0,     0,     0,     0,     0,     0,     0,     0,
     174,     0,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   159,   153,   155,     0,     0,
       0,   134,     0,   126,   128,   130,   132,     0,   119,     0,
     121,     0,   123,     0,     0,   329,   200,   355,     0,     0,
       0,     0,     0,   348,     0,   254,    18,     0,   196,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
       0,   319,   318,   282,     0,     0,   261,     0,   186,     0,
       0,   262,   266,     0,     0,   291,     0,   284,     0,   188,
       0,   298,   194,     0,   193,   166,     0,   146,   371,   372,
     373,   375,     0,     0,   117,     0,   116,     0,   115,     0,
       0,   241,   242,   245,   249,   239,     0,   315,   213,   214,
       0,   218,   217,   228,   219,   220,   221,     0,     0,     0,
     225,   226,     0,   211,   215,   316,   212,     0,   256,   388,
     390,     0,   395,     0,   391,     0,   340,   393,   392,     0,
       0,     0,   305,   320,     0,     0,     0,     0,     0,     0,
     197,   198,     0,     0,     0,     0,   305,     0,     0,     0,
       0,     0,   146,   135,   120,   122,   124,   273,     0,     0,
     216,     0,     0,     0,     0,    19,     0,     0,   342,     0,
       0,   302,   303,   304,   299,   300,     0,   278,     0,     0,
     181,     0,   298,   292,   173,     0,   183,     0,   175,   189,
     274,     0,   150,   145,   147,     0,   317,   222,   223,   224,
     230,   229,   227,   396,     0,   397,   366,     0,     0,     0,
     280,     0,     0,     0,   305,     0,     0,     0,   148,    26,
       0,     0,     0,     0,     0,   337,   301,   307,   306,   281,
     279,     0,   425,   185,   177,     0,   187,   176,    26,   151,
       0,   231,   398,    15,     0,   424,     0,     0,   149,     0,
       0,   178,   269,     0,    16,    20,    21,    18,     0,     0,
      12,    17,   399
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    88,    89,   884,   919,   778,
     917,    12,   148,    90,   367,    91,    92,    93,    94,   194,
      95,    96,   677,   679,   681,   457,   458,   459,   460,    97,
     443,   742,    98,    99,   419,   100,   101,   195,   668,   811,
     812,   898,   879,   102,   564,   103,   104,   209,   105,   561,
     106,   365,   107,   325,   436,   557,   108,   109,   110,   306,
     297,   111,   877,   112,   906,   381,   535,   554,   720,   731,
     113,   324,   733,   502,   802,   114,   232,   484,   115,   388,
     252,   624,   773,   862,   116,   385,   242,   384,   618,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   654,
     127,   128,   129,   130,   131,   475,   132,   133,   134,   135,
     136,   870,   890,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   257,   298,   792,   834,   835,   837,   888,   260,
     521,   525,   774,   713,   526,   215,   303,   276,   234,   375,
     282,   283,   511,   512,   253,   254,   243,   244,   344,   304,
     740,   569,   570,   571,   346,   347,   348,   277,   405,   204,
     223,   318,   621,   362,   363,   364,   548,   291,   292
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -724
static const yytype_int16 yypact[] =
{
     249,   -67,   -61,   110,  -724,   249,    51,   -31,     1,  -724,
    -724,    28,   698,    54,  -724,    95,  -724,   102,   132,   141,
     147,  -724,   171,   173,   178,   191,   201,   203,   218,   227,
     234,   238,   242,   243,   250,   251,   256,   266,   270,   271,
     275,   278,   280,   281,   291,   292,   300,   301,   304,   319,
     328,   329,   330,   332,   333,   336,    52,   337,   338,  -724,
     342,    65,   650,  -724,  -724,   348,   140,   122,   224,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,   698,  -724,  -724,   182,  -724,  -724,  -724,  -724,  -724,
     349,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,   375,   306,  -724,   -76,
     305,    66,   245,   189,   189,    38,   -17,    60,   -12,   -12,
     768,   -66,   -12,   -12,   343,   -12,   289,   -66,   -66,   -15,
      -1,    -1,    -1,   122,   276,   122,   -12,   768,   768,   -12,
     768,   768,    68,   768,   -15,   768,    70,  -724,   768,   -66,
     669,   122,  -724,  -724,   293,   295,   -12,   354,  -724,    40,
    -724,   353,  -724,   240,  -724,    53,  -724,    58,  -724,   352,
    -724,    95,  -724,  -724,   356,  -724,   308,   361,   369,   370,
    -724,  -724,  -724,  -724,  -724,   386,  -724,  -724,   382,   531,
    -724,   384,   391,   392,  -724,  -724,  -724,  -724,   549,  -724,
    -724,   402,  -724,  -724,  -724,  -724,  -724,  -724,   552,  -724,
    -724,   405,   406,   413,  -724,  -724,  -724,   416,  -724,  -724,
     417,   418,   419,   -66,   -66,   -12,   -12,   424,   -12,   427,
     428,   430,   768,   431,   530,  -724,  -724,   372,  -724,   579,
    -724,   432,   438,  -724,   445,  -724,   448,   459,   458,   605,
     462,   464,  -724,  -724,  -724,  -724,  -724,   466,   609,   612,
     469,   475,   483,   490,   309,   635,   499,   290,   500,   501,
    -724,   503,  -724,  -724,  -724,  -724,  -724,  -724,  -724,   504,
     505,   506,  -724,  -724,   507,   352,   508,   511,  -724,   522,
     122,   122,   512,  -724,   510,   260,   122,   122,  -724,   122,
     122,   122,   122,   122,   308,   309,  -724,   528,   526,  -724,
    -724,  -724,  -724,  -724,  -724,   529,    96,    33,  -724,  -724,
     308,   309,   542,   543,   544,   698,   698,  -724,  -724,   122,
     -76,   676,    36,   691,   555,   547,   768,   557,   122,   286,
     701,   550,   560,   122,   561,   352,   562,   122,   352,   -12,
     -12,   768,   651,   662,  -724,  -724,   565,   583,   749,  -724,
     -12,   -12,   387,  -724,   582,   581,   768,   585,   122,    30,
     277,   -12,   673,   751,   610,   685,    -1,   -13,  -724,   613,
     614,    -1,    -1,    -1,   122,   615,    57,   -12,   149,   684,
     616,   682,    -5,   761,    60,   675,  -724,   128,   128,  -724,
     -40,   617,   -21,   812,  -724,  -724,   269,   385,   208,   208,
    -724,  -724,  -724,    53,  -724,   768,   621,   -59,   -43,   -39,
     -27,  -724,  -724,   308,   309,   181,   225,   117,  -724,   618,
     395,  -724,  -724,  -724,   771,  -724,   623,   386,  -724,   624,
     772,   400,  -724,  -724,   392,  -724,  -724,   622,   439,   298,
    -724,   626,   479,  -724,  -724,  -724,  -724,   629,   632,   -12,
     -12,   574,   636,   634,   638,   643,  -724,   645,   486,  -724,
     646,   649,  -724,   653,   655,  -724,  -724,   801,   484,  -724,
    -724,   658,  -724,   656,  -724,   663,  -724,  -724,  -724,   668,
     806,  -724,   670,  -724,   816,   674,    30,   824,   686,   688,
    -724,   690,   767,  -724,  -724,  -724,  -724,  -724,   692,  -724,
     320,   694,  -724,   840,   700,  -724,   704,   753,   850,  -724,
     706,   352,  -724,   647,   122,  -724,  -724,   308,   709,  -724,
     711,   710,  -724,  -724,  -724,  -724,   856,   712,  -724,   -10,
    -724,   122,  -724,   -76,  -724,    29,  -724,   169,  -724,    37,
    -724,  -724,  -724,   738,   891,  -724,  -724,   744,  -724,   739,
     746,  -724,  -724,  -724,  -724,  -724,  -724,  -724,   756,   766,
    -724,   770,  -724,   773,  -724,  -724,   774,  -724,  -724,  -724,
    -724,  -724,   747,  -724,   769,    60,   911,  -724,   776,   862,
     768,  -724,   122,   122,   768,   778,   122,   768,   768,   777,
     780,  -724,   -15,   925,   103,   927,   -18,   860,   785,     4,
    -724,   788,   782,   869,  -724,   122,   794,   -76,   795,   865,
     797,     5,   283,   352,   128,  -724,  -724,   309,   792,   210,
     812,  -724,   126,  -724,  -724,   309,   308,    23,  -724,    25,
    -724,    27,  -724,    30,   799,  -724,  -724,  -724,   -76,   122,
     122,   122,    38,  -724,   847,  -724,   800,   122,  -724,   802,
     229,   340,   803,    30,   493,   804,   805,   122,   946,   809,
     798,  -724,  -724,  -724,   811,   948,  -724,   968,  -724,   317,
     825,  -724,  -724,   826,    97,   308,   320,  -724,   971,  -724,
     972,   809,  -724,   848,  -724,  -724,   849,   150,  -724,  -724,
    -724,  -724,   352,    29,  -724,   169,  -724,    37,  -724,   844,
     993,   308,  -724,  -724,  -724,  -724,   142,  -724,  -724,  -724,
     -76,  -724,  -724,  -724,  -724,  -724,  -724,   851,   852,   853,
    -724,  -724,   855,  -724,  -724,  -724,   308,   996,  -724,   309,
    -724,   973,  -724,   122,  -724,   854,  -724,  -724,  -724,   367,
     859,   299,   928,  -724,  1005,   863,   861,   868,     5,   122,
    -724,  -724,   866,   867,   870,   871,   928,    97,  1003,   320,
     864,   872,   150,  -724,  -724,  -724,  -724,  -724,   874,   956,
     308,   122,   122,   122,   -53,  -724,   873,   253,  -724,   122,
    1019,  -724,  -724,  -724,  -724,   877,   879,  -724,   878,  1025,
    -724,   286,   809,  -724,  -724,   882,  -724,  1027,  -724,  -724,
    -724,   883,  -724,  -724,  -724,  1028,  -724,  -724,  -724,  -724,
    -724,   827,  -724,  -724,  1001,  -724,   160,   884,   299,   958,
     352,  1034,   888,   889,   928,   967,   892,   352,  -724,   698,
     894,   -53,   893,   899,   895,  -724,  -724,  -724,  -724,  -724,
    -724,   896,  -724,  -724,  -724,   900,  -724,  -724,   698,  -724,
      30,  -724,  -724,  -724,   901,  -724,   352,   977,  -724,   903,
     -12,  -724,  -724,    94,   904,  -724,  -724,   800,   -12,   902,
    -724,  -724,  -724
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -724,  -724,  1049,  -724,  -724,  -724,  -724,  -724,  -724,   138,
    -724,  -724,   845,   -91,  -321,   693,   875,   995,  -427,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  1004,  -724,  -724,  -724,   247,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,   637,
     880,  -724,  -724,  -724,  -724,   577,  -724,  -724,  -724,   264,
    -724,  -724,  -724,  -551,   259,  -724,   350,   226,  -724,  -724,
    -724,  -724,  -724,   187,  -724,  -724,   916,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,   429,  -689,   204,  -724,  -723,  -724,  -398,
    -527,  -724,  -724,  -724,   379,   705,  -188,  -151,  -344,   597,
     -94,  -335,  -417,  -556,  -436,  -552,   611,  -547,  -147,   -57,
    -724,   407,  -724,  -724,   625,  -724,  -724,   807,  -154,   591,
    -724,  -421,  -724,  -724,  -724,  -724,  -724,  -153,  -724
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -208
static const yytype_int16 yytable[] =
{
     206,   233,   216,   329,   435,   544,   278,   261,   262,   650,
     203,   284,   285,   461,   287,   290,   568,   300,   301,   302,
     559,   560,   462,   309,   310,   308,   312,   313,   311,   319,
     588,   321,   235,   682,   326,   680,   556,   198,   678,   473,
     506,   246,   806,   566,   198,   332,   288,   255,   334,   295,
     590,   533,   224,   225,   224,   225,   358,   198,    16,   552,
     360,   314,   198,   562,   490,   315,   316,   493,   718,   729,
     221,   860,   314,   293,   294,   322,   315,   316,   698,   349,
     212,   350,     7,   848,   279,   351,   352,   353,     8,   577,
     519,   213,   214,   236,   520,   327,   578,   224,   225,   915,
     456,   800,   247,   861,   334,   579,   237,   280,   281,   581,
       9,   734,   580,   359,   396,   397,   582,   399,   307,   258,
     506,   583,   224,   225,   259,    13,   198,   228,   584,   658,
     715,   506,   317,   685,   323,   212,    11,   534,   229,   256,
     229,   296,   335,   441,   222,   553,   213,   214,   345,   299,
     361,   894,   354,   874,   719,   730,   749,    14,   916,   230,
     231,   801,   339,   340,   341,   342,   343,   711,   563,   394,
     395,   743,   246,   745,   712,   747,   785,    15,   744,   238,
     746,   147,   748,   356,   235,   239,   474,   558,   248,   212,
     199,   816,   235,   815,   249,   355,   814,   228,   200,   201,
     213,   214,   146,   199,    67,   200,   201,   305,   356,   212,
     463,   546,   357,   547,   212,   250,   251,   191,   200,   201,
     213,   214,   479,   200,   201,   213,   214,   695,   246,   230,
     231,   280,   281,   247,   202,   674,   541,   496,   494,   495,
     665,   202,   673,   568,   503,   236,   199,   736,   513,   504,
     505,   149,   516,   236,   202,   809,   755,   810,   237,   202,
     527,   200,   201,   532,     1,     2,   237,   558,   538,   539,
     540,   589,   199,   440,   708,   469,   545,   685,   558,   446,
     447,   150,   448,   449,   450,   451,   452,   200,   201,   247,
     151,   819,   509,   510,   643,   567,   152,   202,   197,   335,
     464,   575,   602,   509,   510,   803,   603,   604,   883,   224,
     225,   -14,   470,   226,   227,   217,   218,   219,   220,   248,
     153,   481,   154,   202,   314,   249,   488,   155,   315,   316,
     492,   238,   831,   832,   833,   585,   288,   239,   619,   238,
     156,   522,   735,   523,   524,   239,   501,   732,   627,   628,
     157,   518,   158,   224,   225,   482,   483,   605,   606,   607,
     240,   241,   224,   225,   341,   342,   343,   159,   240,   241,
     337,   738,   739,   909,   205,   248,   160,   781,   210,   587,
     782,   249,   608,   161,   228,   226,   227,   162,   851,   207,
     506,   163,   164,   338,   507,   229,   345,   609,   610,   165,
     166,   864,   250,   251,   865,   167,   611,   612,   613,   614,
     615,   445,   341,   342,   343,   168,   230,   231,   286,   169,
     170,   813,   572,   616,   171,   617,   305,   172,   228,   173,
     174,   339,   340,   341,   342,   343,   676,   228,   427,   289,
     175,   176,   339,   340,   341,   342,   343,   331,   229,   177,
     178,   224,   225,   179,   211,   339,   340,   341,   342,   343,
     230,   231,   339,   340,   341,   342,   343,   723,   180,   230,
     231,   339,   340,   341,   342,   343,   699,   181,   182,   183,
     702,   184,   185,   705,   706,   186,   188,   189,   783,   635,
     377,   190,   339,   340,   341,   342,   343,   196,   208,   202,
     330,   752,   753,   754,   333,   336,   366,   667,   369,   371,
     725,   339,   340,   341,   342,   343,   370,   372,   373,   263,
     264,   265,   266,   267,   675,   268,   228,   269,   270,   271,
     272,   273,   376,   374,   377,   377,   378,   508,   573,   379,
     380,   751,   339,   340,   341,   342,   343,   776,   592,   889,
     224,   225,   382,   598,   383,   386,   897,   387,   230,   231,
    -207,   389,   509,   510,   390,   391,   392,   393,   275,   339,
     829,   341,   342,   343,   398,   700,   701,   400,   401,   704,
     402,   404,   406,   407,   408,   911,   409,   339,   340,   341,
     342,   343,   601,   410,   224,   225,   411,   339,   340,   341,
     342,   343,   339,   340,   341,   342,   343,   412,   413,   414,
     415,   843,   416,   820,   417,   418,   420,   421,   263,   264,
     265,   266,   267,   422,   268,   228,   269,   270,   271,   272,
     273,   423,   623,   857,   858,   859,   274,   641,   424,   425,
     779,   339,   340,   341,   342,   343,   786,   426,   428,   429,
     789,   430,   431,   432,   433,   434,   437,   230,   231,   438,
     442,   444,   263,   264,   265,   266,   267,   275,   268,   228,
     269,   270,   271,   272,   273,   439,   453,   454,   455,   472,
     274,   339,   340,   341,   342,   343,   339,   340,   341,   342,
     343,   465,   466,   467,   476,   339,   340,   341,   342,   343,
     478,   230,   231,   477,    16,   480,   485,   486,   487,   489,
     491,   275,    17,   499,   497,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,   498,   827,   514,    28,    29,
      30,   500,   515,   517,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,   528,    42,    43,    44,    45,
      46,    47,    48,    49,    50,   529,    51,   531,   530,   914,
     549,   536,   537,   542,   551,   555,   550,   921,  -165,   576,
     565,   594,   866,   591,   593,   596,   597,   625,   600,    52,
     626,    53,   622,   629,   630,   631,   632,    54,   899,    55,
      56,   633,    57,   634,    58,    59,    60,   637,   636,    61,
      62,   638,    63,   639,    64,   640,   642,   908,   643,   192,
     646,   644,   501,   224,   225,    70,   645,    72,   647,    74,
     648,    76,   649,    78,    65,    80,    66,    82,   651,    84,
     656,    86,   224,   225,   652,    71,   653,    73,   655,    75,
     657,    77,   659,    79,   660,    81,   663,    83,   661,    85,
      67,    87,   662,   635,   664,   666,    68,    69,   669,   670,
     672,   671,   279,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,   263,   264,   265,   266,   267,   683,   268,   228,   269,
     270,   271,   272,   273,   684,   685,   686,   687,   693,   274,
     263,   264,   265,   266,   267,   688,   268,   228,   269,   270,
     271,   272,   273,   756,   757,   689,   696,   694,   274,   690,
     230,   231,   691,   692,   697,   501,   703,   707,   708,   710,
     275,   714,   716,   717,   758,   759,   715,   760,   721,   230,
     231,   722,   724,   726,   727,   728,   737,   750,   777,   275,
     790,   793,   795,   780,   784,   787,   788,   791,   761,   794,
     762,   763,   764,   765,   766,   767,   768,   769,   770,   771,
     772,    69,   796,   798,   799,   804,   805,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,   817,   807,   808,   818,   825,
     821,   822,   823,   212,   824,   828,   826,   830,   836,   838,
     850,   839,   840,   852,   213,   214,   841,   844,   845,   847,
     856,   846,   855,   867,   863,   868,   871,   853,   869,   872,
     875,   876,   878,   880,   882,   885,   881,   887,   891,   892,
     893,   895,   900,   896,   902,   903,   904,   905,   907,   910,
     912,   913,   918,   922,    10,   920,   368,   193,   468,   854,
     187,   599,   842,   543,   320,   328,   849,   873,   901,   797,
     245,   709,   886,   775,   595,   471,   586,   741,   574,   403,
     620
};

static const yytype_uint16 yycheck[] =
{
      91,   152,   149,   191,   325,   426,   160,   158,   159,   536,
      67,   162,   163,   357,   165,   166,   443,   170,   171,   172,
     437,   438,   357,   177,   178,   176,   180,   181,   179,   183,
     466,   185,     3,   589,   188,   587,   434,     4,   585,     3,
       3,     3,   731,    64,     4,   196,    47,    64,     8,    64,
     467,    64,    64,    65,    64,    65,   207,     4,     6,    64,
     207,     4,     4,   103,   385,     8,     9,   388,    64,    64,
       4,   124,     4,   167,   168,     5,     8,     9,   629,    21,
     156,    23,   149,   806,   150,    27,    28,    29,   149,   148,
      60,   167,   168,    64,    64,   189,   155,    64,    65,     5,
       4,     4,    64,   156,     8,   148,    77,   173,   174,   148,
       0,   662,   155,   207,   265,   266,   155,   268,   175,    59,
       3,   148,    64,    65,    64,   156,     4,   139,   155,   550,
     148,     3,    64,   151,    64,   156,    85,   150,   150,   156,
     150,   156,   199,   331,    78,   150,   167,   168,   205,   150,
     207,   874,    94,   842,   150,   150,   683,   156,    64,   171,
     172,    64,   202,   203,   204,   205,   206,    64,   208,   263,
     264,   148,     3,   148,    71,   148,   703,   149,   155,   150,
     155,    86,   155,   150,     3,   156,   150,   150,   150,   156,
     150,   747,     3,   745,   156,   137,   743,   139,   165,   166,
     167,   168,   148,   150,   152,   165,   166,   150,   150,   156,
     357,    62,   154,    64,   156,   177,   178,   152,   165,   166,
     167,   168,   376,   165,   166,   167,   168,   625,     3,   171,
     172,   173,   174,    64,   201,   579,   424,   391,   389,   390,
     561,   201,   577,   670,   398,    64,   150,   664,   402,   400,
     401,   149,   406,    64,   201,   105,   692,   107,    77,   201,
     411,   165,   166,   416,    15,    16,    77,   150,   421,   422,
     423,   154,   150,   330,   148,   366,   427,   151,   150,   336,
     337,   149,   339,   340,   341,   342,   343,   165,   166,    64,
     149,   149,   175,   176,   152,   442,   149,   201,   158,   356,
     357,   455,     4,   175,   176,   726,     8,     9,   148,    64,
      65,   151,   369,    68,    69,    10,    11,    12,    13,   150,
     149,   378,   149,   201,     4,   156,   383,   149,     8,     9,
     387,   150,    33,    34,    35,   154,    47,   156,   489,   150,
     149,    64,   663,    66,    67,   156,    63,    64,   499,   500,
     149,   408,   149,    64,    65,    69,    70,    59,    60,    61,
     179,   180,    64,    65,   204,   205,   206,   149,   179,   180,
     130,   161,   162,   900,   150,   150,   149,   148,     3,   154,
     151,   156,    84,   149,   139,    68,    69,   149,   809,   207,
       3,   149,   149,   153,     7,   150,   453,    99,   100,   149,
     149,   148,   177,   178,   151,   149,   108,   109,   110,   111,
     112,   151,   204,   205,   206,   149,   171,   172,    75,   149,
     149,   742,   153,   125,   149,   127,   150,   149,   139,   149,
     149,   202,   203,   204,   205,   206,   583,   139,   148,   150,
     149,   149,   202,   203,   204,   205,   206,   152,   150,   149,
     149,    64,    65,   149,   148,   202,   203,   204,   205,   206,
     171,   172,   202,   203,   204,   205,   206,   655,   149,   171,
     172,   202,   203,   204,   205,   206,   630,   149,   149,   149,
     634,   149,   149,   637,   638,   149,   149,   149,   148,     3,
       4,   149,   202,   203,   204,   205,   206,   149,   149,   201,
     207,   689,   690,   691,   150,   152,   154,   564,   152,   148,
     657,   202,   203,   204,   205,   206,   208,   148,   148,   132,
     133,   134,   135,   136,   581,   138,   139,   140,   141,   142,
     143,   144,   150,   147,     4,     4,   152,   150,   153,   148,
     148,   688,   202,   203,   204,   205,   206,   694,   153,   870,
      64,    65,     3,   153,   152,     3,   877,   152,   171,   172,
     154,   148,   175,   176,   148,   148,   148,   148,   181,   202,
     203,   204,   205,   206,   150,   632,   633,   150,   150,   636,
     150,   150,   210,     4,   152,   906,   148,   202,   203,   204,
     205,   206,   153,   148,    64,    65,   148,   202,   203,   204,
     205,   206,   202,   203,   204,   205,   206,   148,   150,     4,
     148,   799,   148,   760,   148,     6,     4,   148,   132,   133,
     134,   135,   136,   148,   138,   139,   140,   141,   142,   143,
     144,   148,   153,   821,   822,   823,   150,   153,   148,     4,
     697,   202,   203,   204,   205,   206,   153,   148,   148,   148,
     707,   148,   148,   148,   148,   148,   148,   171,   172,   148,
     148,   151,   132,   133,   134,   135,   136,   181,   138,   139,
     140,   141,   142,   143,   144,   153,   148,   151,   149,     3,
     150,   202,   203,   204,   205,   206,   202,   203,   204,   205,
     206,   149,   149,   149,     3,   202,   203,   204,   205,   206,
     153,   171,   172,   148,     6,   148,     5,   157,   148,   148,
     148,   181,    14,   148,    63,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    63,   783,   145,    30,    31,
      32,   148,   151,   148,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    72,    48,    49,    50,    51,
      52,    53,    54,    55,    56,     4,    58,    72,   148,   910,
      76,   148,   148,   148,    82,     4,   150,   918,    93,   148,
     153,   148,   829,   155,     3,   151,     4,   148,   156,    81,
     148,    83,   156,   209,   148,   151,   148,    89,   879,    91,
      92,   148,    94,   148,    96,    97,    98,   148,   152,   101,
     102,   148,   104,   148,   106,     4,   148,   898,   152,   159,
       4,   148,    63,    64,    65,   165,   148,   167,   148,   169,
       4,   171,   148,   173,   126,   175,   128,   177,     4,   179,
      63,   181,    64,    65,   148,   166,   148,   168,   148,   170,
     148,   172,   148,   174,     4,   176,    93,   178,   148,   180,
     152,   182,   148,     3,   148,   208,   158,   159,   149,   148,
       4,   151,   150,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   132,   133,   134,   135,   136,   148,   138,   139,   140,
     141,   142,   143,   144,     3,   151,   157,   151,   151,   150,
     132,   133,   134,   135,   136,   149,   138,   139,   140,   141,
     142,   143,   144,    66,    67,   149,     5,   148,   150,   149,
     171,   172,   149,   149,   148,    63,   148,   150,   148,     4,
     181,     4,    72,   148,    87,    88,   148,    90,   156,   171,
     172,    72,   148,   148,    79,   148,   154,   148,   148,   181,
       4,   153,     4,   151,   151,   151,   151,   148,   111,   148,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   159,     4,   148,   148,     4,     4,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   151,   148,   148,     5,     3,
     149,   149,   149,   156,   149,   151,    33,   148,    80,     4,
       7,   148,   151,   149,   167,   168,   148,   151,   151,   148,
      64,   151,   148,     4,   151,   148,   148,   155,   149,     4,
     148,     4,   149,     5,    33,   151,   209,    79,     4,   151,
     151,    74,   148,   151,   151,   146,   151,   151,   148,   148,
      73,   148,   148,   151,     5,   917,   211,    62,   365,   812,
      56,   484,   798,   426,   184,   190,   807,   841,   881,   719,
     154,   642,   868,   694,   477,   370,   465,   670,   453,   272,
     489
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   212,   213,   214,   215,   149,   149,     0,
     213,    85,   222,   156,   156,   149,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    30,    31,
      32,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    58,    81,    83,    89,    91,    92,    94,    96,    97,
      98,   101,   102,   104,   106,   126,   128,   152,   158,   159,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   216,   217,
     224,   226,   227,   228,   229,   231,   232,   240,   243,   244,
     246,   247,   254,   256,   257,   259,   261,   263,   267,   268,
     269,   272,   274,   281,   286,   289,   295,   300,   301,   302,
     303,   304,   305,   306,   307,   308,   309,   311,   312,   313,
     314,   315,   317,   318,   319,   320,   321,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   148,    86,   223,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   246,   149,   149,
     149,   152,   159,   228,   230,   248,   149,   158,     4,   150,
     165,   166,   201,   360,   370,   150,   224,   207,   149,   258,
       3,   148,   156,   167,   168,   346,   359,    10,    11,    12,
      13,     4,    78,   371,    64,    65,    68,    69,   139,   150,
     171,   172,   287,   348,   349,     3,    64,    77,   150,   156,
     179,   180,   297,   357,   358,   297,     3,    64,   150,   156,
     177,   178,   291,   355,   356,    64,   156,   333,    59,    64,
     340,   348,   348,   132,   133,   134,   135,   136,   138,   140,
     141,   142,   143,   144,   150,   181,   348,   368,   369,   150,
     173,   174,   351,   352,   348,   348,    75,   348,    47,   150,
     348,   378,   379,   351,   351,    64,   156,   271,   334,   150,
     378,   378,   378,   347,   360,   150,   270,   360,   348,   369,
     369,   348,   369,   369,     4,     8,     9,    64,   372,   369,
     271,   369,     5,    64,   282,   264,   369,   351,   227,   347,
     207,   152,   348,   150,     8,   360,   152,   130,   153,   202,
     203,   204,   205,   206,   359,   360,   365,   366,   367,    21,
      23,    27,    28,    29,    94,   137,   150,   154,   348,   351,
     359,   360,   374,   375,   376,   262,   154,   225,   223,   152,
     208,   148,   148,   148,   147,   350,   150,     4,   152,   148,
     148,   276,     3,   152,   298,   296,     3,   152,   290,   148,
     148,   148,   148,   148,   351,   351,   348,   348,   150,   348,
     150,   150,   150,   368,   150,   369,   210,     4,   152,   148,
     148,   148,   148,   150,     4,   148,   148,   148,     6,   245,
       4,   148,   148,   148,   148,     4,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   225,   265,   148,   148,   153,
     360,   347,   148,   241,   151,   151,   360,   360,   360,   360,
     360,   360,   360,   148,   151,   149,     4,   236,   237,   238,
     239,   349,   352,   359,   360,   149,   149,   149,   226,   224,
     360,   346,     3,     3,   150,   316,     3,   148,   153,   369,
     148,   360,    69,    70,   288,     5,   157,   148,   360,   148,
     225,   148,   360,   225,   348,   348,   369,    63,    63,   148,
     148,    63,   284,   369,   348,   348,     3,     7,   150,   175,
     176,   353,   354,   369,   145,   151,   369,   148,   360,    60,
      64,   341,    64,    66,    67,   342,   345,   348,    72,     4,
     148,    72,   378,    64,   150,   277,   148,   148,   378,   378,
     378,   347,   148,   270,   372,   348,    62,    64,   377,    76,
     150,    82,    64,   150,   278,     4,   340,   266,   150,   353,
     353,   260,   103,   208,   255,   153,    64,   359,   229,   362,
     363,   364,   153,   153,   365,   369,   148,   148,   155,   148,
     155,   148,   155,   148,   155,   154,   357,   154,   355,   154,
     353,   155,   153,     3,   148,   350,   151,     4,   153,   276,
     156,   153,     4,     8,     9,    59,    60,    61,    84,    99,
     100,   108,   109,   110,   111,   112,   125,   127,   299,   348,
     370,   373,   156,   153,   292,   148,   148,   348,   348,   209,
     148,   151,   148,   148,   148,     3,   152,   148,   148,   148,
       4,   153,   148,   152,   148,   148,     4,   148,     4,   148,
     341,     4,   148,   148,   310,   148,    63,   148,   372,   148,
       4,   148,   148,    93,   148,   225,   208,   360,   249,   149,
     148,   151,     4,   352,   349,   360,   359,   233,   358,   234,
     356,   235,   354,   148,     3,   151,   157,   151,   149,   149,
     149,   149,   149,   151,   148,   340,     5,   148,   284,   369,
     360,   360,   369,   148,   360,   369,   369,   150,   148,   334,
       4,    64,    71,   344,     4,   148,    72,   148,    64,   150,
     279,   156,    72,   347,   148,   359,   148,    79,   148,    64,
     150,   280,    64,   283,   284,   225,   353,   154,   161,   162,
     361,   362,   242,   148,   155,   148,   155,   148,   155,   341,
     148,   359,   347,   347,   347,   355,    66,    67,    87,    88,
      90,   111,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   293,   343,   345,   359,   148,   220,   360,
     151,   148,   151,   148,   151,   341,   153,   151,   151,   360,
       4,   148,   335,   153,   148,     4,     4,   287,   148,   148,
       4,    64,   285,   372,     4,     4,   335,   148,   148,   105,
     107,   250,   251,   225,   358,   356,   354,   151,     5,   149,
     359,   149,   149,   149,   149,     3,    33,   360,   151,   203,
     148,    33,    34,    35,   336,   337,    80,   338,     4,   148,
     151,   148,   280,   347,   151,   151,   151,   148,   338,   285,
       7,   372,   149,   155,   250,   148,    64,   347,   347,   347,
     124,   156,   294,   151,   148,   151,   360,     4,   148,   149,
     322,   148,     4,   288,   335,   148,     4,   273,   149,   253,
       5,   209,    33,   148,   218,   151,   336,    79,   339,   225,
     323,     4,   151,   151,   338,    74,   151,   225,   252,   224,
     148,   294,   151,   146,   151,   151,   275,   148,   224,   341,
     148,   225,    73,   148,   348,     5,    64,   221,   148,   219,
     220,   348,   151
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

  case 103:

/* Line 1455 of yacc.c  */
#line 563 "lev_comp.y"
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

  case 104:

/* Line 1455 of yacc.c  */
#line 579 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 585 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 591 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 597 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 603 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 609 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 615 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 621 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 627 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 637 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 647 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 657 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 667 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 677 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 687 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 699 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 704 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 711 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 716 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 723 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 728 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 735 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 739 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 745 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 750 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 757 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 761 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 767 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 771 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 777 "lev_comp.y"
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

  case 134:

/* Line 1455 of yacc.c  */
#line 799 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 803 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 814 "lev_comp.y"
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

  case 137:

/* Line 1455 of yacc.c  */
#line 869 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 875 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 879 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 885 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 892 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 896 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 904 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 908 "lev_comp.y"
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

  case 145:

/* Line 1455 of yacc.c  */
#line 931 "lev_comp.y"
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

  case 148:

/* Line 1455 of yacc.c  */
#line 976 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 986 "lev_comp.y"
    {
		  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 989 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 999 "lev_comp.y"
    {
		  ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 1004 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 1018 "lev_comp.y"
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

  case 156:

/* Line 1455 of yacc.c  */
#line 1055 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1060 "lev_comp.y"
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

  case 158:

/* Line 1455 of yacc.c  */
#line 1089 "lev_comp.y"
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

  case 159:

/* Line 1455 of yacc.c  */
#line 1103 "lev_comp.y"
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

  case 160:

/* Line 1455 of yacc.c  */
#line 1119 "lev_comp.y"
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

  case 161:

/* Line 1455 of yacc.c  */
#line 1139 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1151 "lev_comp.y"
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

  case 163:

/* Line 1455 of yacc.c  */
#line 1171 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1177 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1187 "lev_comp.y"
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

  case 166:

/* Line 1455 of yacc.c  */
#line 1205 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1215 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1221 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1226 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1231 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1238 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1245 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1254 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1262 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1273 "lev_comp.y"
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

  case 176:

/* Line 1455 of yacc.c  */
#line 1284 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1291 "lev_comp.y"
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

  case 178:

/* Line 1455 of yacc.c  */
#line 1302 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1309 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1313 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1319 "lev_comp.y"
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

  case 182:

/* Line 1455 of yacc.c  */
#line 1329 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1335 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1344 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1350 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1355 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1361 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1366 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1372 "lev_comp.y"
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

  case 190:

/* Line 1455 of yacc.c  */
#line 1383 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (5)].i), SPO_DOOR));
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1397 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1401 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1411 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1418 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1426 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1442 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1446 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1452 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1460 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1466 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1474 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1482 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1487 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1493 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1499 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1505 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1511 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1516 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1521 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1526 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1531 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1536 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1541 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1546 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1551 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1556 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1561 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1567 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1574 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1583 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1587 "lev_comp.y"
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

  case 232:

/* Line 1455 of yacc.c  */
#line 1600 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1608 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1618 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1626 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1635 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1642 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1650 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1656 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1661 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1666 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1671 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1676 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1681 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1686 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1691 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1696 "lev_comp.y"
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

  case 248:

/* Line 1455 of yacc.c  */
#line 1707 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1713 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1718 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1723 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1728 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1735 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1741 "lev_comp.y"
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

  case 255:

/* Line 1455 of yacc.c  */
#line 1770 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1775 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1783 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1789 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1795 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1802 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1809 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1819 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1829 "lev_comp.y"
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

  case 264:

/* Line 1455 of yacc.c  */
#line 1844 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1854 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1858 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1864 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1870 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1876 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int)(yyvsp[(6) - (13)].i), (int)(yyvsp[(8) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1882 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1888 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1894 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1899 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1906 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1913 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1919 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1925 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1931 "lev_comp.y"
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

  case 279:

/* Line 1455 of yacc.c  */
#line 1945 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      if ( (yyvsp[(10) - (11)].i) ) {
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      } else if ( (yyvsp[(11) - (11)].i) )
			  lc_error("Cannot use lev statements in non-permanent REGION");
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1955 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1959 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1965 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1972 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1978 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1984 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1990 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1996 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 2000 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 2005 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 2012 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 2018 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 2025 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 2029 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2036 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2047 "lev_comp.y"
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

  case 298:

/* Line 1455 of yacc.c  */
#line 2060 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2064 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2070 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2074 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2081 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2085 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2089 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2095 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2099 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2105 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2121 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2129 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2139 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2148 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2153 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2160 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2171 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2177 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2181 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2185 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2192 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2202 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2209 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2213 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2219 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2223 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2231 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2235 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2242 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2252 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2265 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2269 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2276 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2286 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2295 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2306 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2310 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2317 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2327 "lev_comp.y"
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

  case 347:

/* Line 1455 of yacc.c  */
#line 2338 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2347 "lev_comp.y"
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

  case 349:

/* Line 1455 of yacc.c  */
#line 2357 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2363 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2367 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2374 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2384 "lev_comp.y"
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

  case 354:

/* Line 1455 of yacc.c  */
#line 2395 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2404 "lev_comp.y"
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

  case 356:

/* Line 1455 of yacc.c  */
#line 2414 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2418 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2424 "lev_comp.y"
    { ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2426 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2432 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2436 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2440 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2444 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2452 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2461 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2465 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2469 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2473 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2477 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2480 "lev_comp.y"
    { ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2484 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2491 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2500 "lev_comp.y"
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

  case 378:

/* Line 1455 of yacc.c  */
#line 2541 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2545 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2552 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2559 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2569 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2573 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2581 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2585 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2589 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2593 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2597 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2602 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2606 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2610 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2615 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2620 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2625 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2629 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2634 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2639 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2643 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2647 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2652 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2656 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2663 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2669 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2673 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2679 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2695 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2699 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2703 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2707 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2729 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2733 "lev_comp.y"
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

  case 425:

/* Line 1455 of yacc.c  */
#line 2759 "lev_comp.y"
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
#line 6219 "lev_comp.tab.c"
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
#line 2787 "lev_comp.y"


/*lev_comp.y*/

