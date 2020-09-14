
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
     ELEMENTAL_ENCHANTMENT_TYPE = 338,
     EXCEPTIONALITY_TYPE = 339,
     EXCEPTIONALITY_ID = 340,
     ELEMENTAL_ENCHANTMENT_ID = 341,
     ENCHANTMENT_ID = 342,
     CHARGES_ID = 343,
     SPECIAL_QUALITY_ID = 344,
     SPEFLAGS_ID = 345,
     SUBROOM_ID = 346,
     NAME_ID = 347,
     FLAGS_ID = 348,
     FLAG_TYPE = 349,
     MON_ATTITUDE = 350,
     MON_ALERTNESS = 351,
     SUBTYPE_ID = 352,
     MON_APPEARANCE = 353,
     ROOMDOOR_ID = 354,
     IF_ID = 355,
     ELSE_ID = 356,
     TERRAIN_ID = 357,
     HORIZ_OR_VERT = 358,
     REPLACE_TERRAIN_ID = 359,
     EXIT_ID = 360,
     SHUFFLE_ID = 361,
     QUANTITY_ID = 362,
     BURIED_ID = 363,
     LOOP_ID = 364,
     FOR_ID = 365,
     TO_ID = 366,
     SWITCH_ID = 367,
     CASE_ID = 368,
     BREAK_ID = 369,
     DEFAULT_ID = 370,
     ERODED_ID = 371,
     TRAPPED_STATE = 372,
     RECHARGED_ID = 373,
     INVIS_ID = 374,
     GREASED_ID = 375,
     FEMALE_ID = 376,
     WAITFORU_ID = 377,
     CANCELLED_ID = 378,
     REVIVED_ID = 379,
     AVENGE_ID = 380,
     FLEEING_ID = 381,
     BLINDED_ID = 382,
     PARALYZED_ID = 383,
     STUNNED_ID = 384,
     CONFUSED_ID = 385,
     SEENTRAPS_ID = 386,
     ALL_ID = 387,
     MONTYPE_ID = 388,
     GRAVE_ID = 389,
     ERODEPROOF_ID = 390,
     FUNCTION_ID = 391,
     MSG_OUTPUT_TYPE = 392,
     COMPARE_TYPE = 393,
     UNKNOWN_TYPE = 394,
     rect_ID = 395,
     fillrect_ID = 396,
     line_ID = 397,
     randline_ID = 398,
     grow_ID = 399,
     selection_ID = 400,
     flood_ID = 401,
     rndcoord_ID = 402,
     circle_ID = 403,
     ellipse_ID = 404,
     filter_ID = 405,
     complement_ID = 406,
     gradient_ID = 407,
     GRADIENT_TYPE = 408,
     LIMITED = 409,
     HUMIDITY_TYPE = 410,
     STRING = 411,
     MAP_ID = 412,
     NQSTRING = 413,
     VARSTRING = 414,
     CFUNC = 415,
     CFUNC_INT = 416,
     CFUNC_STR = 417,
     CFUNC_COORD = 418,
     CFUNC_REGION = 419,
     VARSTRING_INT = 420,
     VARSTRING_INT_ARRAY = 421,
     VARSTRING_STRING = 422,
     VARSTRING_STRING_ARRAY = 423,
     VARSTRING_VAR = 424,
     VARSTRING_VAR_ARRAY = 425,
     VARSTRING_COORD = 426,
     VARSTRING_COORD_ARRAY = 427,
     VARSTRING_REGION = 428,
     VARSTRING_REGION_ARRAY = 429,
     VARSTRING_MAPCHAR = 430,
     VARSTRING_MAPCHAR_ARRAY = 431,
     VARSTRING_MONST = 432,
     VARSTRING_MONST_ARRAY = 433,
     VARSTRING_OBJ = 434,
     VARSTRING_OBJ_ARRAY = 435,
     VARSTRING_SEL = 436,
     VARSTRING_SEL_ARRAY = 437,
     METHOD_INT = 438,
     METHOD_INT_ARRAY = 439,
     METHOD_STRING = 440,
     METHOD_STRING_ARRAY = 441,
     METHOD_VAR = 442,
     METHOD_VAR_ARRAY = 443,
     METHOD_COORD = 444,
     METHOD_COORD_ARRAY = 445,
     METHOD_REGION = 446,
     METHOD_REGION_ARRAY = 447,
     METHOD_MAPCHAR = 448,
     METHOD_MAPCHAR_ARRAY = 449,
     METHOD_MONST = 450,
     METHOD_MONST_ARRAY = 451,
     METHOD_OBJ = 452,
     METHOD_OBJ_ARRAY = 453,
     METHOD_SEL = 454,
     METHOD_SEL_ARRAY = 455,
     DICE = 456
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
#line 499 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 511 "lev_comp.tab.c"

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
#define YYLAST   1102

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  219
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  169
/* YYNRULES -- Number of rules.  */
#define YYNRULES  431
/* YYNRULES -- Number of states.  */
#define YYNSTATES  941

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   456

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   214,   218,     2,
     158,   159,   212,   210,   156,   211,   216,   213,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   157,     2,
       2,   215,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   160,     2,   161,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   162,   217,   163,     2,     2,     2,     2,
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
     155,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209
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
     774,   776,   778,   780,   784,   788,   792,   796,   800,   804,
     810,   818,   824,   833,   835,   839,   845,   851,   859,   867,
     874,   880,   881,   884,   890,   894,   908,   912,   916,   918,
     924,   934,   940,   944,   948,   949,   961,   962,   964,   972,
     976,   984,   990,   996,  1002,  1008,  1012,  1018,  1026,  1036,
    1038,  1040,  1042,  1044,  1046,  1047,  1050,  1052,  1056,  1058,
    1060,  1062,  1063,  1067,  1069,  1071,  1073,  1075,  1077,  1079,
    1081,  1083,  1085,  1087,  1091,  1093,  1095,  1100,  1102,  1104,
    1109,  1111,  1113,  1118,  1120,  1125,  1131,  1133,  1137,  1139,
    1143,  1145,  1147,  1152,  1162,  1164,  1166,  1171,  1173,  1179,
    1181,  1183,  1188,  1190,  1192,  1198,  1200,  1202,  1204,  1209,
    1211,  1213,  1219,  1221,  1223,  1225,  1229,  1231,  1233,  1237,
    1239,  1244,  1248,  1252,  1256,  1260,  1264,  1268,  1270,  1272,
    1276,  1278,  1282,  1283,  1285,  1287,  1289,  1291,  1295,  1296,
    1298,  1300,  1303,  1306,  1311,  1318,  1323,  1330,  1337,  1344,
    1351,  1354,  1361,  1370,  1379,  1390,  1405,  1408,  1410,  1414,
    1416,  1420,  1422,  1424,  1426,  1428,  1430,  1432,  1434,  1436,
    1438,  1440,  1442,  1444,  1446,  1448,  1450,  1452,  1454,  1456,
    1458,  1469
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     220,     0,    -1,    -1,   221,    -1,   222,    -1,   222,   221,
      -1,   223,   230,   232,    -1,    16,   157,   164,    -1,    15,
     157,   164,   156,     3,    -1,    17,   157,    11,   156,   324,
      -1,    17,   157,    10,   156,     3,    -1,    17,   157,    13,
      -1,    17,   157,    12,   156,     3,   156,     3,   156,     5,
     156,     5,   156,   349,   156,   229,   228,    -1,    18,   157,
     379,    -1,    -1,   156,   154,    -1,    -1,   156,   356,    -1,
      -1,   156,     3,    -1,     5,    -1,    64,    -1,    -1,    93,
     157,   231,    -1,    94,   156,   231,    -1,    94,    -1,    -1,
     234,   232,    -1,   162,   232,   163,    -1,   275,    -1,   224,
      -1,   225,    -1,   335,    -1,   334,    -1,   332,    -1,   333,
      -1,   336,    -1,   337,    -1,   317,    -1,   277,    -1,   240,
      -1,   239,    -1,   327,    -1,   289,    -1,   309,    -1,   339,
      -1,   340,    -1,   319,    -1,   320,    -1,   321,    -1,   338,
      -1,   255,    -1,   265,    -1,   267,    -1,   271,    -1,   269,
      -1,   252,    -1,   262,    -1,   248,    -1,   251,    -1,   312,
      -1,   294,    -1,   310,    -1,   297,    -1,   303,    -1,   328,
      -1,   323,    -1,   315,    -1,   276,    -1,   329,    -1,   282,
      -1,   280,    -1,   322,    -1,   326,    -1,   325,    -1,   313,
      -1,   314,    -1,   316,    -1,   308,    -1,   311,    -1,   174,
      -1,   176,    -1,   178,    -1,   180,    -1,   182,    -1,   184,
      -1,   186,    -1,   188,    -1,   190,    -1,   173,    -1,   175,
      -1,   177,    -1,   179,    -1,   181,    -1,   183,    -1,   185,
      -1,   187,    -1,   189,    -1,   235,    -1,   236,    -1,   167,
      -1,   167,    -1,   236,    -1,   106,   157,   235,    -1,   237,
     215,   368,    -1,   237,   215,   145,   157,   377,    -1,   237,
     215,   367,    -1,   237,   215,   384,   157,   361,    -1,   237,
     215,   383,   157,   363,    -1,   237,   215,   382,   157,   365,
      -1,   237,   215,   356,    -1,   237,   215,   359,    -1,   237,
     215,   162,   246,   163,    -1,   237,   215,   162,   245,   163,
      -1,   237,   215,   162,   244,   163,    -1,   237,   215,   384,
     157,   162,   243,   163,    -1,   237,   215,   383,   157,   162,
     242,   163,    -1,   237,   215,   382,   157,   162,   241,   163,
      -1,   237,   215,   162,   247,   163,    -1,   366,    -1,   241,
     156,   366,    -1,   364,    -1,   242,   156,   364,    -1,   362,
      -1,   243,   156,   362,    -1,   360,    -1,   244,   156,   360,
      -1,   357,    -1,   245,   156,   357,    -1,   368,    -1,   246,
     156,   368,    -1,   367,    -1,   247,   156,   367,    -1,    -1,
      -1,   136,   166,   158,   249,   372,   159,   250,   233,    -1,
     166,   158,   375,   159,    -1,   105,    -1,    -1,     6,    -1,
       6,    -1,   160,   368,   138,   368,   161,    -1,   160,   368,
     161,    -1,    -1,    -1,   112,   256,   160,   355,   161,   257,
     162,   258,   163,    -1,    -1,   259,   258,    -1,    -1,   113,
     380,   157,   260,   232,    -1,    -1,   115,   157,   261,   232,
      -1,   114,    -1,   216,   216,    -1,   111,    -1,   110,   238,
     215,   368,   263,   368,    -1,    -1,   264,   266,   233,    -1,
      -1,   109,   160,   355,   161,   268,   233,    -1,    -1,   254,
     157,   270,   234,    -1,    -1,   100,   254,   272,   273,    -1,
     233,    -1,    -1,   233,   274,   101,   233,    -1,    14,   157,
     367,    -1,    58,    -1,    58,   157,   380,    -1,    58,   157,
      64,    -1,    49,   157,   278,   156,   278,    -1,    49,   157,
     278,   156,   380,    -1,   158,     4,   156,    63,   156,   293,
     159,    -1,   342,   253,   156,   349,    -1,    -1,    91,   157,
     279,   156,   286,   156,   288,   343,   346,   281,   233,    -1,
      -1,    43,   157,   279,   156,   285,   156,   287,   156,   288,
     343,   346,   283,   233,    -1,    -1,   156,     5,    -1,   158,
       4,   156,     4,   159,    -1,    64,    -1,   158,     4,   156,
       4,   159,    -1,    64,    -1,   158,   295,   156,   296,   159,
      -1,    64,    -1,   158,     4,   156,     4,   159,    -1,    64,
      -1,    99,   157,   290,   156,   348,   156,   291,   156,   293,
      -1,    25,   157,   348,   156,   377,    -1,     5,    -1,    64,
      -1,   292,    -1,    64,    -1,    63,    -1,    63,   217,   292,
      -1,     4,    -1,    64,    -1,    20,    -1,    19,   157,   295,
     156,   296,   284,   165,    -1,    19,   157,   356,   284,   165,
      -1,    68,    -1,    69,    -1,    70,    -1,    69,    -1,    23,
     157,   299,    -1,    -1,    23,   157,   299,   298,   233,    -1,
     363,   156,   356,   300,    -1,    -1,   300,   156,   301,    -1,
     367,    -1,    95,    -1,    96,    -1,   351,    -1,    98,   367,
      -1,   121,    -1,   119,    -1,   123,    -1,   124,    -1,   125,
      -1,   126,   157,   355,    -1,   127,   157,   355,    -1,   128,
     157,   355,    -1,   129,    -1,   130,    -1,   131,   157,   302,
      -1,   122,    -1,   164,    -1,   132,    -1,   164,   217,   302,
      -1,    21,   157,   305,    -1,    -1,    22,   157,   305,   304,
     233,    -1,   365,   306,    -1,    -1,   306,   156,   307,    -1,
      61,    -1,   133,   157,   363,    -1,   381,    -1,    92,   157,
     367,    -1,   107,   157,   355,    -1,   108,    -1,    60,    -1,
     116,   157,   355,    -1,   135,    -1,    59,    -1,   117,    -1,
     118,   157,   355,    -1,   119,    -1,   120,    -1,   356,    -1,
      86,   157,    83,    -1,    85,   157,    84,    -1,    87,   157,
     355,    -1,    88,   157,   355,    -1,    89,   157,   355,    -1,
      90,   157,   355,    -1,    24,   157,   341,   156,   356,    -1,
      26,   157,   356,   156,    63,   156,   348,    -1,    30,   157,
     356,   156,    63,    -1,    30,   157,   356,   156,    63,   156,
       5,   228,    -1,    31,    -1,    31,   157,   377,    -1,    39,
     157,   356,   156,    72,    -1,    40,   157,   356,   156,    72,
      -1,    40,   157,   386,   156,   386,   156,    72,    -1,    44,
     157,   386,   156,   386,   156,   164,    -1,    45,   157,   386,
     156,   386,   318,    -1,    46,   157,   386,   156,   386,    -1,
      -1,   156,    72,    -1,    52,   157,   377,   156,    76,    -1,
      53,   157,   377,    -1,    54,   157,   356,   156,   158,   380,
     156,   380,   159,   156,    74,   156,    73,    -1,    56,   157,
     377,    -1,    55,   157,   377,    -1,     3,    -1,   158,     3,
     156,   349,   159,    -1,   104,   157,   359,   156,   361,   156,
     361,   156,     7,    -1,   102,   157,   377,   156,   361,    -1,
      41,   157,   359,    -1,    42,   157,   359,    -1,    -1,    32,
     157,   359,   156,   349,   156,   342,   343,   346,   330,   331,
      -1,    -1,   233,    -1,    36,   157,   356,   156,   350,   156,
     352,    -1,    37,   157,   356,    -1,    81,   157,   377,   156,
      82,   156,    79,    -1,    97,   157,   377,   156,     4,    -1,
      38,   157,    75,   156,   356,    -1,   134,   157,   356,   156,
     367,    -1,   134,   157,   356,   156,    64,    -1,   134,   157,
     356,    -1,    50,   157,   368,   156,   356,    -1,    51,   157,
     356,   156,   385,   156,   367,    -1,    48,   157,   355,   156,
     355,   156,   355,   156,   355,    -1,    48,    -1,   164,    -1,
      64,    -1,   164,    -1,    64,    -1,    -1,   156,   344,    -1,
     345,    -1,   345,   156,   344,    -1,    33,    -1,    34,    -1,
      35,    -1,    -1,    80,   157,   347,    -1,    79,    -1,    59,
      -1,    64,    -1,    60,    -1,    64,    -1,    67,    -1,   353,
      -1,    64,    -1,    67,    -1,   353,    -1,    66,   157,    64,
      -1,    71,    -1,    64,    -1,    66,   160,     4,   161,    -1,
     164,    -1,   175,    -1,   176,   160,   368,   161,    -1,   368,
      -1,   357,    -1,   147,   158,   377,   159,    -1,   179,    -1,
     180,   160,   368,   161,    -1,   158,     4,   156,     4,   159,
      -1,    64,    -1,    65,   358,   161,    -1,   155,    -1,   155,
     156,   358,    -1,   360,    -1,   181,    -1,   182,   160,   368,
     161,    -1,   158,     4,   156,     4,   156,     4,   156,     4,
     159,    -1,   362,    -1,   183,    -1,   184,   160,   368,   161,
      -1,     3,    -1,   158,     3,   156,   349,   159,    -1,   364,
      -1,   185,    -1,   186,   160,   368,   161,    -1,   164,    -1,
       3,    -1,   158,     3,   156,   164,   159,    -1,    64,    -1,
     366,    -1,   187,    -1,   188,   160,   368,   161,    -1,   164,
      -1,     3,    -1,   158,     3,   156,   164,   159,    -1,    64,
      -1,    77,    -1,   354,    -1,   367,   216,   354,    -1,     4,
      -1,   378,    -1,   158,     8,   159,    -1,   173,    -1,   174,
     160,   368,   161,    -1,   368,   210,   368,    -1,   368,   211,
     368,    -1,   368,   212,   368,    -1,   368,   213,   368,    -1,
     368,   214,   368,    -1,   158,   368,   159,    -1,   169,    -1,
     170,    -1,   237,   157,   369,    -1,   370,    -1,   371,   156,
     370,    -1,    -1,   371,    -1,   368,    -1,   367,    -1,   373,
      -1,   374,   156,   373,    -1,    -1,   374,    -1,   356,    -1,
     140,   359,    -1,   141,   359,    -1,   142,   356,   156,   356,
      -1,   143,   356,   156,   356,   156,   368,    -1,   144,   158,
     377,   159,    -1,   144,   158,   292,   156,   377,   159,    -1,
     150,   158,     7,   156,   377,   159,    -1,   150,   158,   377,
     156,   377,   159,    -1,   150,   158,   361,   156,   377,   159,
      -1,   146,   356,    -1,   148,   158,   356,   156,   368,   159,
      -1,   148,   158,   356,   156,   368,   156,    33,   159,    -1,
     149,   158,   356,   156,   368,   156,   368,   159,    -1,   149,
     158,   356,   156,   368,   156,   368,   156,    33,   159,    -1,
     152,   158,   153,   156,   158,   368,   211,   368,   226,   159,
     156,   356,   227,   159,    -1,   151,   376,    -1,   189,    -1,
     158,   377,   159,    -1,   376,    -1,   376,   218,   377,    -1,
     209,    -1,    78,    -1,     4,    -1,     8,    -1,     9,    -1,
       4,    -1,     8,    -1,     9,    -1,     4,    -1,   378,    -1,
      27,    -1,    21,    -1,    28,    -1,    23,    -1,    29,    -1,
     102,    -1,    62,    -1,    64,    -1,   387,    -1,    47,   158,
       4,   156,     4,   156,     4,   156,     4,   159,    -1,   158,
       4,   156,     4,   156,     4,   156,     4,   159,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   283,   283,   284,   287,   288,   291,   314,   319,   341,
     354,   366,   372,   401,   408,   411,   418,   422,   429,   432,
     439,   440,   444,   447,   454,   458,   465,   468,   474,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   496,   497,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516,   517,   518,   519,   520,
     521,   522,   523,   524,   525,   526,   527,   528,   529,   530,
     533,   534,   535,   536,   537,   538,   539,   540,   541,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   555,   556,
     557,   560,   561,   564,   580,   586,   592,   598,   604,   610,
     616,   622,   628,   638,   648,   658,   668,   678,   688,   700,
     705,   712,   717,   724,   729,   736,   740,   746,   751,   758,
     762,   768,   772,   779,   801,   778,   815,   870,   877,   880,
     886,   893,   897,   906,   910,   905,   973,   974,   978,   977,
     991,   990,  1005,  1015,  1016,  1019,  1057,  1056,  1091,  1090,
    1121,  1120,  1153,  1152,  1178,  1189,  1188,  1216,  1222,  1227,
    1232,  1239,  1246,  1255,  1263,  1275,  1274,  1293,  1292,  1311,
    1314,  1320,  1330,  1336,  1345,  1351,  1356,  1362,  1367,  1373,
    1384,  1390,  1391,  1394,  1395,  1398,  1402,  1408,  1409,  1412,
    1419,  1427,  1435,  1436,  1439,  1440,  1443,  1448,  1447,  1461,
    1468,  1475,  1483,  1488,  1494,  1500,  1506,  1512,  1517,  1522,
    1527,  1532,  1537,  1542,  1547,  1552,  1557,  1562,  1568,  1575,
    1584,  1588,  1601,  1610,  1609,  1627,  1637,  1643,  1651,  1657,
    1662,  1667,  1672,  1677,  1682,  1687,  1692,  1697,  1708,  1714,
    1719,  1724,  1729,  1734,  1739,  1744,  1749,  1754,  1759,  1766,
    1772,  1801,  1806,  1814,  1820,  1826,  1833,  1840,  1850,  1860,
    1875,  1886,  1889,  1895,  1901,  1907,  1913,  1919,  1925,  1930,
    1937,  1944,  1950,  1956,  1963,  1962,  1987,  1990,  1996,  2003,
    2009,  2015,  2021,  2027,  2031,  2036,  2043,  2049,  2056,  2060,
    2067,  2075,  2078,  2088,  2092,  2095,  2101,  2105,  2112,  2116,
    2120,  2127,  2130,  2136,  2142,  2143,  2146,  2147,  2150,  2151,
    2152,  2158,  2159,  2160,  2166,  2167,  2170,  2179,  2184,  2191,
    2202,  2208,  2212,  2216,  2223,  2233,  2240,  2244,  2250,  2254,
    2262,  2266,  2273,  2283,  2296,  2300,  2307,  2317,  2326,  2337,
    2341,  2348,  2358,  2369,  2378,  2388,  2394,  2398,  2405,  2415,
    2426,  2435,  2445,  2449,  2456,  2457,  2463,  2467,  2471,  2475,
    2483,  2492,  2496,  2500,  2504,  2508,  2512,  2515,  2522,  2531,
    2564,  2565,  2568,  2569,  2572,  2576,  2583,  2590,  2601,  2604,
    2612,  2616,  2620,  2624,  2628,  2633,  2637,  2641,  2646,  2651,
    2656,  2660,  2665,  2670,  2674,  2678,  2683,  2687,  2694,  2700,
    2704,  2710,  2717,  2718,  2721,  2722,  2723,  2726,  2730,  2734,
    2738,  2744,  2745,  2748,  2749,  2752,  2753,  2756,  2757,  2760,
    2764,  2790
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
  "ELEMENTAL_ENCHANTMENT_TYPE", "EXCEPTIONALITY_TYPE", "EXCEPTIONALITY_ID",
  "ELEMENTAL_ENCHANTMENT_ID", "ENCHANTMENT_ID", "CHARGES_ID",
  "SPECIAL_QUALITY_ID", "SPEFLAGS_ID", "SUBROOM_ID", "NAME_ID", "FLAGS_ID",
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
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,    44,    58,    40,    41,
      91,    93,   123,   125,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
      43,    45,    42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   219,   220,   220,   221,   221,   222,   223,   223,   224,
     224,   224,   224,   225,   226,   226,   227,   227,   228,   228,
     229,   229,   230,   230,   231,   231,   232,   232,   233,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   237,   237,
     237,   238,   238,   239,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   241,
     241,   242,   242,   243,   243,   244,   244,   245,   245,   246,
     246,   247,   247,   249,   250,   248,   251,   252,   253,   253,
     254,   254,   254,   256,   257,   255,   258,   258,   260,   259,
     261,   259,   262,   263,   263,   264,   266,   265,   268,   267,
     270,   269,   272,   271,   273,   274,   273,   275,   276,   276,
     276,   277,   277,   278,   279,   281,   280,   283,   282,   284,
     284,   285,   285,   286,   286,   287,   287,   288,   288,   289,
     289,   290,   290,   291,   291,   292,   292,   293,   293,   294,
     294,   294,   295,   295,   296,   296,   297,   298,   297,   299,
     300,   300,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   302,
     302,   302,   303,   304,   303,   305,   306,   306,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   308,
     309,   310,   310,   311,   311,   312,   313,   314,   315,   316,
     317,   318,   318,   319,   320,   321,   322,   323,   324,   324,
     325,   326,   327,   328,   330,   329,   331,   331,   332,   333,
     334,   335,   336,   337,   337,   337,   338,   339,   340,   340,
     341,   341,   342,   342,   343,   343,   344,   344,   345,   345,
     345,   346,   346,   347,   348,   348,   349,   349,   350,   350,
     350,   351,   351,   351,   352,   352,   353,   354,   354,   354,
     355,   356,   356,   356,   356,   357,   357,   357,   358,   358,
     359,   359,   359,   360,   361,   361,   361,   362,   362,   363,
     363,   363,   364,   364,   364,   364,   365,   365,   365,   366,
     366,   366,   366,   366,   367,   367,   368,   368,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   369,   369,   370,
     371,   371,   372,   372,   373,   373,   374,   374,   375,   375,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   377,
     377,   378,   379,   379,   380,   380,   380,   381,   381,   381,
     381,   382,   382,   383,   383,   384,   384,   385,   385,   386,
     386,   387
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
       1,     1,     1,     3,     3,     3,     3,     3,     3,     5,
       7,     5,     8,     1,     3,     5,     5,     7,     7,     6,
       5,     0,     2,     5,     3,    13,     3,     3,     1,     5,
       9,     5,     3,     3,     0,    11,     0,     1,     7,     3,
       7,     5,     5,     5,     5,     3,     5,     7,     9,     1,
       1,     1,     1,     1,     0,     2,     1,     3,     1,     1,
       1,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     4,     1,     1,     4,
       1,     1,     4,     1,     4,     5,     1,     3,     1,     3,
       1,     1,     4,     9,     1,     1,     4,     1,     5,     1,
       1,     4,     1,     1,     5,     1,     1,     1,     4,     1,
       1,     5,     1,     1,     1,     3,     1,     1,     3,     1,
       4,     3,     3,     3,     3,     3,     3,     1,     1,     3,
       1,     3,     0,     1,     1,     1,     1,     3,     0,     1,
       1,     2,     2,     4,     6,     4,     6,     6,     6,     6,
       2,     6,     8,     8,    10,    14,     2,     1,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
      10,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    22,     0,     0,     1,
       5,     0,    26,     0,     7,     0,   140,     0,     0,     0,
       0,   199,     0,     0,     0,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   299,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,   101,   102,     0,     0,     0,     0,   366,     0,
     369,     0,   411,     0,   367,   388,    27,     0,   160,     0,
       8,     0,   327,   328,     0,   364,   167,     0,     0,     0,
      11,   413,   412,    13,   336,     0,   202,   203,     0,     0,
     333,     0,     0,   179,   331,   360,   362,   363,     0,   359,
     357,     0,   232,   236,   356,   233,   353,   355,     0,   352,
     350,     0,   206,     0,   349,   301,   300,     0,   314,   315,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   407,   390,   409,   264,     0,
     341,     0,     0,   340,     0,   289,     0,     0,     0,     0,
       0,     0,   429,   282,   283,   303,   302,     0,   138,     0,
       0,     0,     0,     0,   330,     0,     0,     0,     0,     0,
     274,     0,   277,   276,   416,   414,   415,   170,   169,     0,
       0,     0,   191,   192,     0,     0,     0,     0,   103,     0,
       0,     0,   295,   133,     0,     0,     0,     0,   142,     0,
       0,     0,     0,     0,   385,   384,   386,   389,     0,   422,
     424,   421,   423,   425,   426,     0,     0,     0,   110,   111,
     106,   104,     0,     0,     0,     0,    26,   157,    24,     0,
       0,     0,     0,     0,   338,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   391,   392,     0,     0,     0,   400,
       0,     0,     0,   406,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   139,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   164,   163,     0,     0,   158,
       0,     0,     0,   382,   368,   376,     0,     0,   371,   372,
     373,   374,   375,     0,   136,     0,   366,     0,     0,     0,
       0,   127,   125,   131,   129,     0,     0,     0,   161,     0,
       0,   365,    10,   278,     0,     9,     0,     0,   337,     0,
       0,     0,   205,   204,   179,   180,   201,     0,     0,     0,
     234,     0,     0,   208,   210,   259,   190,     0,   261,     0,
       0,   195,     0,     0,     0,     0,   347,     0,     0,   345,
       0,     0,   344,     0,     0,   408,   410,     0,     0,   316,
     317,     0,   320,     0,   318,     0,   319,   292,   265,     0,
       0,   266,     0,   182,     0,     0,     0,     0,     0,   271,
     270,     0,     0,   171,   172,   296,   427,   428,     0,   273,
       0,     0,   184,     0,     0,   291,     0,     0,     0,   281,
       0,     0,   154,     0,     0,   144,   294,   293,     0,   380,
     383,     0,   370,   141,   387,   105,     0,     0,   114,     0,
     113,     0,   112,     0,   118,     0,   109,     0,   108,     0,
     107,    28,   329,     0,     0,   339,   332,     0,   334,     0,
       0,   358,   419,   417,   418,   247,   244,   238,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,   248,     0,
     250,   251,     0,   246,   237,   252,   420,   240,     0,   351,
     209,     0,     0,   393,     0,     0,     0,   395,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,   174,     0,     0,     0,
     269,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   159,   153,   155,     0,     0,     0,   134,     0,   126,
     128,   130,   132,     0,   119,     0,   121,     0,   123,     0,
       0,   335,   200,   361,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   354,     0,   260,    18,     0,
     196,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,     0,   325,   324,   288,     0,     0,   267,     0,
     186,     0,     0,   268,   272,     0,     0,   297,     0,   290,
       0,   188,     0,   304,   194,     0,   193,   166,     0,   146,
     377,   378,   379,   381,     0,     0,   117,     0,   116,     0,
     115,     0,     0,   254,   253,   255,   256,   257,   258,   241,
     242,   245,   249,   239,     0,   321,   213,   214,     0,   218,
     217,   228,   219,   220,   221,     0,     0,     0,   225,   226,
       0,   211,   215,   322,   212,     0,   262,   394,   396,     0,
     401,     0,   397,     0,   346,   399,   398,     0,     0,     0,
     311,   326,     0,     0,     0,     0,     0,     0,   197,   198,
       0,     0,     0,     0,   311,     0,     0,     0,     0,     0,
     146,   135,   120,   122,   124,   279,     0,     0,   216,     0,
       0,     0,     0,    19,     0,     0,   348,     0,     0,   308,
     309,   310,   305,   306,     0,   284,     0,     0,   181,     0,
     304,   298,   173,     0,   183,     0,   175,   189,   280,     0,
     150,   145,   147,     0,   323,   222,   223,   224,   230,   229,
     227,   402,     0,   403,   372,     0,     0,     0,   286,     0,
       0,     0,   311,     0,     0,     0,   148,    26,     0,     0,
       0,     0,     0,   343,   307,   313,   312,   287,   285,     0,
     431,   185,   177,     0,   187,   176,    26,   151,     0,   231,
     404,    15,     0,   430,     0,     0,   149,     0,     0,   178,
     275,     0,    16,    20,    21,    18,     0,     0,    12,    17,
     405
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    88,    89,   902,   937,   796,
     935,    12,   148,    90,   367,    91,    92,    93,    94,   194,
      95,    96,   683,   685,   687,   457,   458,   459,   460,    97,
     443,   754,    98,    99,   419,   100,   101,   195,   674,   829,
     830,   916,   897,   102,   564,   103,   104,   209,   105,   561,
     106,   365,   107,   325,   436,   557,   108,   109,   110,   306,
     297,   111,   895,   112,   924,   381,   535,   554,   732,   743,
     113,   324,   745,   502,   820,   114,   232,   484,   115,   388,
     252,   630,   791,   880,   116,   385,   242,   384,   624,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   660,
     127,   128,   129,   130,   131,   475,   132,   133,   134,   135,
     136,   888,   908,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   257,   298,   810,   852,   853,   855,   906,   260,
     521,   525,   792,   725,   526,   215,   303,   276,   234,   375,
     282,   283,   511,   512,   253,   254,   243,   244,   344,   304,
     752,   569,   570,   571,   346,   347,   348,   277,   405,   204,
     223,   318,   627,   362,   363,   364,   548,   291,   292
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -766
static const yytype_int16 yypact[] =
{
     319,   -60,   -49,   123,  -766,   319,    46,   -39,    -6,  -766,
    -766,    13,   655,    35,  -766,   106,  -766,    18,    51,   115,
     121,  -766,   144,   172,   193,   195,   227,   231,   242,   244,
     249,   259,   261,   271,   273,   275,   276,   279,   280,   282,
     296,   297,   303,   317,   318,   320,   321,   330,   333,   334,
     335,   345,   346,   348,   349,   351,    33,   352,   353,  -766,
     354,   157,   758,  -766,  -766,   365,   357,    28,   257,  -766,
    -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,
    -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,
    -766,   655,  -766,  -766,   212,  -766,  -766,  -766,  -766,  -766,
     368,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,
    -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,
    -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,
    -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,
    -766,  -766,  -766,  -766,  -766,  -766,   528,   395,  -766,   -75,
     411,    66,   255,    77,    77,   169,   -21,   202,   228,   228,
     706,   -87,   228,   228,   478,   228,   240,   -87,   -87,   -13,
      11,    11,    11,    28,   396,    28,   228,   706,   706,   228,
     706,   706,   302,   706,   -13,   706,    32,  -766,   706,   -87,
     716,    28,  -766,  -766,   341,   399,   228,   402,  -766,    36,
    -766,   403,  -766,   407,  -766,    31,  -766,   267,  -766,   400,
    -766,   106,  -766,  -766,   404,  -766,   355,   401,   413,   414,
    -766,  -766,  -766,  -766,  -766,   417,  -766,  -766,   409,   570,
    -766,   416,   421,   422,  -766,  -766,  -766,  -766,   577,  -766,
    -766,   423,  -766,  -766,  -766,  -766,  -766,  -766,   581,  -766,
    -766,   427,   431,   456,  -766,  -766,  -766,   457,  -766,  -766,
     460,   473,   474,   -87,   -87,   228,   228,   476,   228,   480,
     481,   482,   706,   483,   448,  -766,  -766,   418,  -766,   627,
    -766,   472,   486,  -766,   487,  -766,   488,   489,   491,   646,
     495,   497,  -766,  -766,  -766,  -766,  -766,   498,   649,   652,
     504,   514,   515,   526,   160,   679,   532,   133,   533,   534,
    -766,   546,  -766,  -766,  -766,  -766,  -766,  -766,  -766,   556,
     558,   559,  -766,  -766,   560,   400,   566,   572,  -766,   523,
      28,    28,   584,  -766,   578,   258,    28,    28,  -766,    28,
      28,    28,    28,    28,   355,   160,  -766,   586,   579,  -766,
    -766,  -766,  -766,  -766,  -766,   588,    53,    23,  -766,  -766,
     355,   160,   590,   592,   593,   655,   655,  -766,  -766,    28,
     -75,   736,    62,   748,   600,   605,   706,   612,    28,     3,
     768,   609,   619,    28,   620,   400,   622,    28,   400,   228,
     228,   706,   690,   717,  -766,  -766,   623,   625,   583,  -766,
     228,   228,   315,  -766,   629,   624,   706,   628,    28,    47,
     107,   228,   714,   783,   632,   730,    11,   -12,  -766,   647,
     648,    11,    11,    11,    28,   656,    73,   228,    29,   729,
     658,   731,     2,   810,   202,   718,  -766,    64,    64,  -766,
     270,   657,    -9,   694,  -766,  -766,   391,   397,   145,   145,
    -766,  -766,  -766,    31,  -766,   706,   661,   -64,   -44,   -43,
     -16,  -766,  -766,   355,   160,    26,    72,   119,  -766,   660,
     412,  -766,  -766,  -766,   817,  -766,   668,   417,  -766,   667,
     823,   453,  -766,  -766,   422,  -766,  -766,   687,   507,    45,
    -766,   698,   582,  -766,  -766,  -766,  -766,   707,   709,   228,
     228,   670,   710,   734,   741,   743,  -766,   745,   386,  -766,
     747,   749,  -766,   752,   753,  -766,  -766,   899,   587,  -766,
    -766,   754,  -766,   751,  -766,   757,  -766,  -766,  -766,   770,
     923,  -766,   772,  -766,   925,   774,    47,   928,   778,   780,
    -766,   782,   877,  -766,  -766,  -766,  -766,  -766,   786,  -766,
     120,   788,  -766,   942,   792,  -766,   794,   848,   948,  -766,
     796,   400,  -766,   737,    28,  -766,  -766,   355,   797,  -766,
     799,   800,  -766,  -766,  -766,  -766,   952,   802,  -766,    -1,
    -766,    28,  -766,   -75,  -766,    21,  -766,    57,  -766,    65,
    -766,  -766,  -766,   805,   955,  -766,  -766,   803,  -766,   798,
     806,  -766,  -766,  -766,  -766,  -766,  -766,  -766,   807,   809,
     813,   814,   815,   816,   818,   819,  -766,   820,  -766,   821,
    -766,  -766,   822,  -766,  -766,  -766,  -766,  -766,   808,  -766,
     824,   202,   969,  -766,   825,   919,   706,  -766,    28,    28,
     706,   827,    28,   706,   706,   826,   829,  -766,   -13,   982,
     112,   983,   180,   916,   833,    10,  -766,   834,   828,   921,
    -766,    28,   835,   -75,   838,   917,   839,    19,   278,   400,
      64,  -766,  -766,   160,   836,   198,   694,  -766,   241,  -766,
    -766,   160,   355,   111,  -766,   135,  -766,   153,  -766,    47,
     841,  -766,  -766,  -766,   915,   918,    28,    28,    28,    28,
     -75,    28,    28,    28,   169,  -766,   793,  -766,   844,    28,
    -766,   843,   181,   151,   845,    47,   597,   846,   847,    28,
     999,   851,   849,  -766,  -766,  -766,   852,  1005,  -766,  1007,
    -766,   314,   856,  -766,  -766,   857,    74,   355,   120,  -766,
    1010,  -766,  1011,   851,  -766,   860,  -766,  -766,   861,   200,
    -766,  -766,  -766,  -766,   400,    21,  -766,    57,  -766,    65,
    -766,   859,  1014,  -766,  -766,  -766,  -766,  -766,  -766,   355,
    -766,  -766,  -766,  -766,   168,  -766,  -766,  -766,   -75,  -766,
    -766,  -766,  -766,  -766,  -766,   863,   864,   865,  -766,  -766,
     866,  -766,  -766,  -766,   355,  1021,  -766,   160,  -766,   992,
    -766,    28,  -766,   867,  -766,  -766,  -766,   336,   871,   343,
     949,  -766,  1024,   874,   872,   876,    19,    28,  -766,  -766,
     875,   878,   879,   880,   949,    74,  1026,   120,   882,   881,
     200,  -766,  -766,  -766,  -766,  -766,   884,   971,   355,    28,
      28,    28,   -48,  -766,   883,   329,  -766,    28,  1037,  -766,
    -766,  -766,  -766,   887,   888,  -766,   890,  1043,  -766,     3,
     851,  -766,  -766,   892,  -766,  1045,  -766,  -766,  -766,   893,
    -766,  -766,  -766,  1046,  -766,  -766,  -766,  -766,  -766,   837,
    -766,  -766,  1019,  -766,   197,   894,   343,   976,   400,  1052,
     898,   900,   949,   984,   901,   400,  -766,   655,   905,   -48,
     903,   909,   906,  -766,  -766,  -766,  -766,  -766,  -766,   907,
    -766,  -766,  -766,   908,  -766,  -766,   655,  -766,    47,  -766,
    -766,  -766,   911,  -766,   400,   995,  -766,   913,   228,  -766,
    -766,    81,   914,  -766,  -766,   844,   228,   912,  -766,  -766,
    -766
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -766,  -766,  1067,  -766,  -766,  -766,  -766,  -766,  -766,   138,
    -766,  -766,   868,   -91,  -309,   711,   885,  1012,  -420,  -766,
    -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,
    -766,  -766,  -766,  -766,  -766,  1022,  -766,  -766,  -766,   247,
    -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,
    -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,   654,
     897,  -766,  -766,  -766,  -766,   598,  -766,  -766,  -766,   268,
    -766,  -766,  -766,  -566,   260,  -766,   356,   224,  -766,  -766,
    -766,  -766,  -766,   187,  -766,  -766,   934,  -766,  -766,  -766,
    -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,
    -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,
    -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,  -766,
    -766,  -766,  -766,   441,  -701,   204,  -766,  -765,  -766,  -412,
    -533,  -766,  -766,  -766,   385,   722,  -182,  -151,  -351,   616,
     237,  -331,  -417,  -543,  -461,  -540,   630,  -537,  -145,   -57,
    -766,   420,  -766,  -766,   641,  -766,  -766,   830,  -147,   608,
    -766,  -424,  -766,  -766,  -766,  -766,  -766,  -153,  -766
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -208
static const yytype_int16 yytable[] =
{
     206,   233,   544,   656,   216,   588,   461,   261,   262,   329,
     203,   284,   285,   278,   287,   290,   435,   300,   301,   302,
     559,   560,   556,   568,   235,   308,   462,   198,   311,   235,
     309,   310,   198,   312,   313,   198,   319,   322,   321,    16,
     198,   326,   824,   255,   334,   332,   688,   686,   684,   602,
     590,   295,   533,   603,   604,   566,   358,   456,   288,   866,
     246,   334,   360,   224,   225,   473,   552,   506,   506,   710,
     221,   279,   482,   483,   730,   246,   490,   314,   818,   493,
     235,   315,   316,   741,   878,   236,   933,   224,   225,   212,
     236,   546,   577,   547,   280,   281,   323,     7,   237,   578,
     213,   214,   746,   237,   605,   606,   607,   519,     8,   224,
     225,   520,   579,   581,   396,   397,   879,   399,   307,   580,
     582,   247,   506,     9,   314,    13,   664,   912,   315,   316,
     608,   609,   610,   611,   612,   613,   247,   614,   819,    11,
     583,   236,   335,   256,   222,   934,   534,   584,   345,   441,
     361,   296,   615,   616,   237,   212,   761,   229,    14,   892,
     553,   617,   618,   619,   620,   621,   213,   214,   731,   299,
      15,   522,   246,   523,   524,   149,   723,   742,   622,   238,
     623,   356,   803,   724,   238,   239,   199,   212,   585,   199,
     239,   146,   228,    67,   199,   212,   200,   201,   213,   214,
     147,   200,   201,   229,   200,   201,   213,   214,   150,   200,
     201,   199,   463,   240,   241,   248,   834,   833,   832,   707,
     474,   249,   558,   558,   230,   231,   200,   201,   680,   479,
     248,   305,   202,   247,   587,   238,   249,   202,   494,   495,
     202,   239,   541,   773,   496,   202,   679,   509,   510,   504,
     505,   503,   671,   748,   202,   513,   568,   250,   251,   516,
     527,   258,   202,   532,   240,   241,   259,   755,   538,   539,
     540,   198,   151,   440,   756,   469,   545,   558,   152,   446,
     447,   589,   448,   449,   450,   451,   452,   288,   349,   427,
     350,   757,   224,   225,   351,   352,   353,   567,   758,   335,
     464,   153,   509,   510,   224,   225,   314,   801,   575,   759,
     315,   316,   470,   827,   821,   828,   760,   191,   506,   224,
     225,   481,   507,   226,   227,   837,   488,   248,   649,   154,
     492,   224,   225,   249,     1,     2,   727,   799,   625,   691,
     800,   501,   744,   339,   340,   341,   342,   343,   633,   634,
     155,   518,   156,   901,   250,   251,   -14,   341,   342,   343,
     747,   339,   340,   341,   342,   343,   317,   750,   751,   354,
     339,   340,   341,   342,   343,   228,   849,   850,   851,   224,
     225,   562,   226,   227,   157,   927,   229,   228,   158,   641,
     377,   339,   340,   341,   342,   343,   345,   720,   289,   159,
     691,   160,   228,   869,   293,   294,   161,   230,   231,   341,
     342,   343,   355,   229,   228,   205,   162,   445,   163,   230,
     231,   217,   218,   219,   220,   356,   327,   207,   164,   357,
     165,   212,   166,   167,   230,   231,   168,   169,   682,   170,
     200,   201,   213,   214,   359,   831,   230,   231,   280,   281,
     224,   225,   377,   171,   172,   263,   264,   265,   266,   267,
     173,   268,   228,   269,   270,   271,   272,   273,   339,   340,
     341,   342,   343,   508,   174,   175,   202,   176,   177,   735,
     339,   340,   341,   342,   343,   882,   563,   178,   883,   711,
     179,   180,   181,   714,   230,   231,   717,   718,   509,   510,
     394,   395,   182,   183,   275,   184,   185,   673,   186,   188,
     189,   190,   224,   225,   765,   766,   767,   768,   737,   770,
     771,   772,   196,   197,   681,   208,   263,   264,   265,   266,
     267,   210,   268,   228,   269,   270,   271,   272,   273,   339,
     340,   341,   342,   343,   274,   337,   339,   847,   341,   342,
     343,   211,   572,   286,   305,   769,   330,   371,   573,   331,
     333,   794,   366,   336,   369,   230,   231,   376,   338,   372,
     373,   370,   374,   592,   377,   275,   378,   379,   380,   907,
     382,   712,   713,   383,   386,   716,   915,   387,   263,   264,
     265,   266,   267,  -207,   268,   228,   269,   270,   271,   272,
     273,   339,   340,   341,   342,   343,   274,   339,   340,   341,
     342,   343,   389,   390,   598,   929,   391,   339,   340,   341,
     342,   343,   339,   340,   341,   342,   343,   230,   231,   392,
     393,   407,   408,   838,   398,   861,   406,   275,   400,   401,
     402,   404,   409,   410,   411,   412,   501,   224,   225,   413,
     414,   415,   797,   416,   417,   418,   420,   875,   876,   877,
     421,    16,   807,   339,   340,   341,   342,   343,   601,    17,
     422,   423,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,   424,   425,   439,    28,    29,    30,   426,   428,
     429,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,   430,    42,    43,    44,    45,    46,    47,    48,
      49,    50,   431,    51,   432,   433,   434,   339,   340,   341,
     342,   343,   437,   263,   264,   265,   266,   267,   438,   268,
     228,   269,   270,   271,   272,   273,    52,   444,   454,   472,
     442,   274,   453,   629,   845,   455,    53,   465,   647,   466,
     467,   476,    54,   497,    55,    56,   477,    57,   804,    58,
      59,    60,   230,   231,    61,    62,   478,    63,   480,    64,
     224,   225,   275,   485,   486,   487,   489,   932,   491,   499,
     498,   500,   514,   515,   517,   939,   528,   529,   530,    65,
     884,    66,   339,   340,   341,   342,   343,   339,   340,   341,
     342,   343,   531,   536,   537,   549,   917,   339,   340,   341,
     342,   343,   542,   551,   555,    67,   550,   576,   565,  -165,
     593,    68,    69,   591,   594,   926,   596,   597,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,   263,   264,   265,   266,
     267,   600,   268,   228,   269,   270,   271,   272,   273,   774,
     775,    69,   628,   631,   274,   632,   636,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,   230,   231,   635,   776,   777,
      71,   778,    73,   637,    75,   275,    77,   638,    79,   639,
      81,   640,    83,   646,    85,   643,    87,   642,   644,   645,
     648,   649,   779,   650,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,   790,   192,   651,   652,   653,   654,
     655,    70,   657,    72,   658,    74,   659,    76,   661,    78,
     662,    80,   663,    82,   665,    84,   666,    86,   667,   669,
     668,   641,   670,   672,   675,   676,   678,   212,   690,   677,
     279,   689,   691,   692,   694,   693,   695,   705,   213,   214,
     696,   697,   698,   699,   708,   700,   701,   702,   703,   704,
     706,   709,   501,   715,   719,   720,   722,   726,   728,   729,
     727,   736,   733,   734,   738,   740,   739,   762,   749,   763,
     795,   764,   798,   808,   802,   805,   806,   809,   812,   813,
     811,   814,   816,   817,   822,   823,   825,   826,   835,   836,
     839,   840,   841,   842,   843,   844,   846,   848,   856,   854,
     857,   858,   859,   868,   862,   874,   865,   863,   864,   870,
     873,   885,   881,   886,   871,   887,   889,   890,   893,   894,
     896,   898,   900,   903,   899,   905,   909,   910,   913,   911,
     914,   918,   920,   921,   925,   922,   923,   928,   930,   931,
     936,   940,    10,   938,   193,   328,   468,   872,   187,   368,
     543,   320,   599,   891,   860,   867,   919,   815,   245,   721,
     904,   793,   471,   595,   574,   586,   753,   626,     0,     0,
       0,     0,   403
};

static const yytype_int16 yycheck[] =
{
      91,   152,   426,   536,   149,   466,   357,   158,   159,   191,
      67,   162,   163,   160,   165,   166,   325,   170,   171,   172,
     437,   438,   434,   443,     3,   176,   357,     4,   179,     3,
     177,   178,     4,   180,   181,     4,   183,     5,   185,     6,
       4,   188,   743,    64,     8,   196,   589,   587,   585,     4,
     467,    64,    64,     8,     9,    64,   207,     4,    47,   824,
       3,     8,   207,    64,    65,     3,    64,     3,     3,   635,
       4,   158,    69,    70,    64,     3,   385,     4,     4,   388,
       3,     8,     9,    64,   132,    64,     5,    64,    65,   164,
      64,    62,   156,    64,   181,   182,    64,   157,    77,   163,
     175,   176,   668,    77,    59,    60,    61,    60,   157,    64,
      65,    64,   156,   156,   265,   266,   164,   268,   175,   163,
     163,    64,     3,     0,     4,   164,   550,   892,     8,     9,
      85,    86,    87,    88,    89,    90,    64,    92,    64,    93,
     156,    64,   199,   164,    78,    64,   158,   163,   205,   331,
     207,   164,   107,   108,    77,   164,   689,   158,   164,   860,
     158,   116,   117,   118,   119,   120,   175,   176,   158,   158,
     157,    64,     3,    66,    67,   157,    64,   158,   133,   158,
     135,   158,   715,    71,   158,   164,   158,   164,   162,   158,
     164,   156,   147,   160,   158,   164,   173,   174,   175,   176,
      94,   173,   174,   158,   173,   174,   175,   176,   157,   173,
     174,   158,   357,   187,   188,   158,   759,   757,   755,   631,
     158,   164,   158,   158,   179,   180,   173,   174,   579,   376,
     158,   158,   209,    64,   162,   158,   164,   209,   389,   390,
     209,   164,   424,   704,   391,   209,   577,   183,   184,   400,
     401,   398,   561,   670,   209,   402,   676,   185,   186,   406,
     411,    59,   209,   416,   187,   188,    64,   156,   421,   422,
     423,     4,   157,   330,   163,   366,   427,   158,   157,   336,
     337,   162,   339,   340,   341,   342,   343,    47,    21,   156,
      23,   156,    64,    65,    27,    28,    29,   442,   163,   356,
     357,   157,   183,   184,    64,    65,     4,   156,   455,   156,
       8,     9,   369,   113,   738,   115,   163,   160,     3,    64,
      65,   378,     7,    68,    69,   157,   383,   158,   160,   157,
     387,    64,    65,   164,    15,    16,   156,   156,   489,   159,
     159,    63,    64,   210,   211,   212,   213,   214,   499,   500,
     157,   408,   157,   156,   185,   186,   159,   212,   213,   214,
     669,   210,   211,   212,   213,   214,    64,   169,   170,   102,
     210,   211,   212,   213,   214,   147,    33,    34,    35,    64,
      65,   111,    68,    69,   157,   918,   158,   147,   157,     3,
       4,   210,   211,   212,   213,   214,   453,   156,   158,   157,
     159,   157,   147,   827,   167,   168,   157,   179,   180,   212,
     213,   214,   145,   158,   147,   158,   157,   159,   157,   179,
     180,    10,    11,    12,    13,   158,   189,   215,   157,   162,
     157,   164,   157,   157,   179,   180,   157,   157,   583,   157,
     173,   174,   175,   176,   207,   754,   179,   180,   181,   182,
      64,    65,     4,   157,   157,   140,   141,   142,   143,   144,
     157,   146,   147,   148,   149,   150,   151,   152,   210,   211,
     212,   213,   214,   158,   157,   157,   209,   157,   157,   661,
     210,   211,   212,   213,   214,   156,   216,   157,   159,   636,
     157,   157,   157,   640,   179,   180,   643,   644,   183,   184,
     263,   264,   157,   157,   189,   157,   157,   564,   157,   157,
     157,   157,    64,    65,   696,   697,   698,   699,   663,   701,
     702,   703,   157,   166,   581,   157,   140,   141,   142,   143,
     144,     3,   146,   147,   148,   149,   150,   151,   152,   210,
     211,   212,   213,   214,   158,   138,   210,   211,   212,   213,
     214,   156,   161,    75,   158,   700,   215,   156,   161,   160,
     158,   706,   162,   160,   160,   179,   180,   158,   161,   156,
     156,   216,   155,   161,     4,   189,   160,   156,   156,   888,
       3,   638,   639,   160,     3,   642,   895,   160,   140,   141,
     142,   143,   144,   162,   146,   147,   148,   149,   150,   151,
     152,   210,   211,   212,   213,   214,   158,   210,   211,   212,
     213,   214,   156,   156,   161,   924,   156,   210,   211,   212,
     213,   214,   210,   211,   212,   213,   214,   179,   180,   156,
     156,     4,   160,   778,   158,   817,   218,   189,   158,   158,
     158,   158,   156,   156,   156,   156,    63,    64,    65,   158,
       4,   156,   709,   156,   156,     6,     4,   839,   840,   841,
     156,     6,   719,   210,   211,   212,   213,   214,   161,    14,
     156,   156,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,   156,     4,   161,    30,    31,    32,   156,   156,
     156,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,   156,    48,    49,    50,    51,    52,    53,    54,
      55,    56,   156,    58,   156,   156,   156,   210,   211,   212,
     213,   214,   156,   140,   141,   142,   143,   144,   156,   146,
     147,   148,   149,   150,   151,   152,    81,   159,   159,     3,
     156,   158,   156,   161,   801,   157,    91,   157,   161,   157,
     157,     3,    97,    63,    99,   100,   156,   102,   161,   104,
     105,   106,   179,   180,   109,   110,   161,   112,   156,   114,
      64,    65,   189,     5,   165,   156,   156,   928,   156,   156,
      63,   156,   153,   159,   156,   936,    72,     4,   156,   134,
     847,   136,   210,   211,   212,   213,   214,   210,   211,   212,
     213,   214,    72,   156,   156,    76,   897,   210,   211,   212,
     213,   214,   156,    82,     4,   160,   158,   156,   161,   101,
       3,   166,   167,   163,   156,   916,   159,     4,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   140,   141,   142,   143,
     144,   164,   146,   147,   148,   149,   150,   151,   152,    66,
      67,   167,   164,   156,   158,   156,   156,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   179,   180,   217,    95,    96,
     174,    98,   176,   159,   178,   189,   180,   156,   182,   156,
     184,   156,   186,     4,   188,   156,   190,   160,   156,   156,
     156,   160,   119,   156,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   167,   156,     4,   156,     4,
     156,   173,     4,   175,   156,   177,   156,   179,   156,   181,
      63,   183,   156,   185,   156,   187,     4,   189,   156,   101,
     156,     3,   156,   216,   157,   156,     4,   164,     3,   159,
     158,   156,   159,   165,   157,   159,   157,   159,   175,   176,
     157,   157,   157,   157,     5,   157,   157,   157,   157,   157,
     156,   156,    63,   156,   158,   156,     4,     4,    72,   156,
     156,   156,   164,    72,   156,   156,    79,   156,   162,    84,
     156,    83,   159,     4,   159,   159,   159,   156,   156,     4,
     161,     4,   156,   156,     4,     4,   156,   156,   159,     5,
     157,   157,   157,   157,     3,    33,   159,   156,     4,    80,
     156,   159,   156,     7,   159,    64,   156,   159,   159,   157,
     156,     4,   159,   156,   163,   157,   156,     4,   156,     4,
     157,     5,    33,   159,   217,    79,     4,   159,    74,   159,
     159,   156,   159,   154,   156,   159,   159,   156,    73,   156,
     156,   159,     5,   935,    62,   190,   365,   830,    56,   211,
     426,   184,   484,   859,   816,   825,   899,   731,   154,   648,
     886,   706,   370,   477,   453,   465,   676,   489,    -1,    -1,
      -1,    -1,   272
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   220,   221,   222,   223,   157,   157,     0,
     221,    93,   230,   164,   164,   157,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    30,    31,
      32,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    58,    81,    91,    97,    99,   100,   102,   104,   105,
     106,   109,   110,   112,   114,   134,   136,   160,   166,   167,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   224,   225,
     232,   234,   235,   236,   237,   239,   240,   248,   251,   252,
     254,   255,   262,   264,   265,   267,   269,   271,   275,   276,
     277,   280,   282,   289,   294,   297,   303,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   319,   320,   321,
     322,   323,   325,   326,   327,   328,   329,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   156,    94,   231,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   254,   157,   157,
     157,   160,   167,   236,   238,   256,   157,   166,     4,   158,
     173,   174,   209,   368,   378,   158,   232,   215,   157,   266,
       3,   156,   164,   175,   176,   354,   367,    10,    11,    12,
      13,     4,    78,   379,    64,    65,    68,    69,   147,   158,
     179,   180,   295,   356,   357,     3,    64,    77,   158,   164,
     187,   188,   305,   365,   366,   305,     3,    64,   158,   164,
     185,   186,   299,   363,   364,    64,   164,   341,    59,    64,
     348,   356,   356,   140,   141,   142,   143,   144,   146,   148,
     149,   150,   151,   152,   158,   189,   356,   376,   377,   158,
     181,   182,   359,   360,   356,   356,    75,   356,    47,   158,
     356,   386,   387,   359,   359,    64,   164,   279,   342,   158,
     386,   386,   386,   355,   368,   158,   278,   368,   356,   377,
     377,   356,   377,   377,     4,     8,     9,    64,   380,   377,
     279,   377,     5,    64,   290,   272,   377,   359,   235,   355,
     215,   160,   356,   158,     8,   368,   160,   138,   161,   210,
     211,   212,   213,   214,   367,   368,   373,   374,   375,    21,
      23,    27,    28,    29,   102,   145,   158,   162,   356,   359,
     367,   368,   382,   383,   384,   270,   162,   233,   231,   160,
     216,   156,   156,   156,   155,   358,   158,     4,   160,   156,
     156,   284,     3,   160,   306,   304,     3,   160,   298,   156,
     156,   156,   156,   156,   359,   359,   356,   356,   158,   356,
     158,   158,   158,   376,   158,   377,   218,     4,   160,   156,
     156,   156,   156,   158,     4,   156,   156,   156,     6,   253,
       4,   156,   156,   156,   156,     4,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   233,   273,   156,   156,   161,
     368,   355,   156,   249,   159,   159,   368,   368,   368,   368,
     368,   368,   368,   156,   159,   157,     4,   244,   245,   246,
     247,   357,   360,   367,   368,   157,   157,   157,   234,   232,
     368,   354,     3,     3,   158,   324,     3,   156,   161,   377,
     156,   368,    69,    70,   296,     5,   165,   156,   368,   156,
     233,   156,   368,   233,   356,   356,   377,    63,    63,   156,
     156,    63,   292,   377,   356,   356,     3,     7,   158,   183,
     184,   361,   362,   377,   153,   159,   377,   156,   368,    60,
      64,   349,    64,    66,    67,   350,   353,   356,    72,     4,
     156,    72,   386,    64,   158,   285,   156,   156,   386,   386,
     386,   355,   156,   278,   380,   356,    62,    64,   385,    76,
     158,    82,    64,   158,   286,     4,   348,   274,   158,   361,
     361,   268,   111,   216,   263,   161,    64,   367,   237,   370,
     371,   372,   161,   161,   373,   377,   156,   156,   163,   156,
     163,   156,   163,   156,   163,   162,   365,   162,   363,   162,
     361,   163,   161,     3,   156,   358,   159,     4,   161,   284,
     164,   161,     4,     8,     9,    59,    60,    61,    85,    86,
      87,    88,    89,    90,    92,   107,   108,   116,   117,   118,
     119,   120,   133,   135,   307,   356,   378,   381,   164,   161,
     300,   156,   156,   356,   356,   217,   156,   159,   156,   156,
     156,     3,   160,   156,   156,   156,     4,   161,   156,   160,
     156,   156,     4,   156,     4,   156,   349,     4,   156,   156,
     318,   156,    63,   156,   380,   156,     4,   156,   156,   101,
     156,   233,   216,   368,   257,   157,   156,   159,     4,   360,
     357,   368,   367,   241,   366,   242,   364,   243,   362,   156,
       3,   159,   165,   159,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   159,   156,   348,     5,   156,
     292,   377,   368,   368,   377,   156,   368,   377,   377,   158,
     156,   342,     4,    64,    71,   352,     4,   156,    72,   156,
      64,   158,   287,   164,    72,   355,   156,   367,   156,    79,
     156,    64,   158,   288,    64,   291,   292,   233,   361,   162,
     169,   170,   369,   370,   250,   156,   163,   156,   163,   156,
     163,   349,   156,    84,    83,   355,   355,   355,   355,   367,
     355,   355,   355,   363,    66,    67,    95,    96,    98,   119,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   301,   351,   353,   367,   156,   228,   368,   159,   156,
     159,   156,   159,   349,   161,   159,   159,   368,     4,   156,
     343,   161,   156,     4,     4,   295,   156,   156,     4,    64,
     293,   380,     4,     4,   343,   156,   156,   113,   115,   258,
     259,   233,   366,   364,   362,   159,     5,   157,   367,   157,
     157,   157,   157,     3,    33,   368,   159,   211,   156,    33,
      34,    35,   344,   345,    80,   346,     4,   156,   159,   156,
     288,   355,   159,   159,   159,   156,   346,   293,     7,   380,
     157,   163,   258,   156,    64,   355,   355,   355,   132,   164,
     302,   159,   156,   159,   368,     4,   156,   157,   330,   156,
       4,   296,   343,   156,     4,   281,   157,   261,     5,   217,
      33,   156,   226,   159,   344,    79,   347,   233,   331,     4,
     159,   159,   346,    74,   159,   233,   260,   232,   156,   302,
     159,   154,   159,   159,   283,   156,   232,   349,   156,   233,
      73,   156,   356,     5,    64,   229,   156,   227,   228,   356,
     159
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
#line 292 "lev_comp.y"
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
#line 315 "lev_comp.y"
    {
		      start_level_def(&splev, (yyvsp[(3) - (3)].map));
		      (yyval.map) = (yyvsp[(3) - (3)].map);
		  ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 320 "lev_comp.y"
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
#line 342 "lev_comp.y"
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
#line 355 "lev_comp.y"
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
#line 367 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_ROGUE,0,0,0,
					  0,0,0,0, SPO_INITLEVEL));
		  ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 373 "lev_comp.y"
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
#line 402 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_TILESET));
		  ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 408 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 412 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 418 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_COPY));
		      (yyval.i) = 0;
		  ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 423 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 429 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 433 "lev_comp.y"
    {
		      (yyval.i) = what_map_char((char) (yyvsp[(2) - (2)].i));
		  ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 444 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 448 "lev_comp.y"
    {
		      add_opvars(splev, "io",
                                 VA_PASS2((int) (yyvsp[(3) - (3)].i), SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 455 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 459 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 465 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 469 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 475 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (3)].i);
		  ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 565 "lev_comp.y"
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
#line 581 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 587 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 593 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 599 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 605 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 611 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 617 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 623 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 629 "lev_comp.y"
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
#line 639 "lev_comp.y"
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
#line 649 "lev_comp.y"
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
#line 659 "lev_comp.y"
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
#line 669 "lev_comp.y"
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
#line 679 "lev_comp.y"
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
#line 689 "lev_comp.y"
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
#line 701 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 706 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 713 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 718 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 725 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 730 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 737 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 741 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 747 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 752 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 759 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 763 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 769 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 773 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 779 "lev_comp.y"
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
#line 801 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 805 "lev_comp.y"
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
#line 816 "lev_comp.y"
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
#line 871 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 877 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 881 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 887 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 894 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 898 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 906 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 910 "lev_comp.y"
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
#line 933 "lev_comp.y"
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
#line 978 "lev_comp.y"
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
#line 988 "lev_comp.y"
    {
		  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 991 "lev_comp.y"
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
#line 1001 "lev_comp.y"
    {
		  ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 1006 "lev_comp.y"
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
#line 1020 "lev_comp.y"
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
#line 1057 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1062 "lev_comp.y"
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
#line 1091 "lev_comp.y"
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
#line 1105 "lev_comp.y"
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
#line 1121 "lev_comp.y"
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
#line 1141 "lev_comp.y"
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
#line 1153 "lev_comp.y"
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
#line 1173 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1179 "lev_comp.y"
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
#line 1189 "lev_comp.y"
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
#line 1207 "lev_comp.y"
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
#line 1217 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1223 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1228 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1233 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1240 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1247 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1256 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1264 "lev_comp.y"
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
#line 1275 "lev_comp.y"
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
#line 1286 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1293 "lev_comp.y"
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
#line 1304 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1311 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1315 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1321 "lev_comp.y"
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
#line 1331 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1337 "lev_comp.y"
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
#line 1346 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1352 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1357 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1363 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1368 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1374 "lev_comp.y"
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
#line 1385 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (5)].i), SPO_DOOR));
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1399 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1403 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1413 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1420 "lev_comp.y"
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
#line 1428 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1444 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1448 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1454 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1462 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1468 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1476 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1484 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1489 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1495 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1501 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1507 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1513 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1518 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1523 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1528 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1533 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1538 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1543 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1548 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1553 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1558 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1563 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1569 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1576 "lev_comp.y"
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
#line 1585 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1589 "lev_comp.y"
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
#line 1602 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1610 "lev_comp.y"
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
#line 1620 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1628 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1637 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1644 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1652 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1658 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1663 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1668 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1673 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1678 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1683 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1688 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1693 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1698 "lev_comp.y"
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
#line 1709 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1715 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1720 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1725 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1730 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1735 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1740 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1745 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1750 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1755 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1760 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1767 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1773 "lev_comp.y"
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

  case 261:

/* Line 1455 of yacc.c  */
#line 1802 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1807 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1815 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1821 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1827 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1834 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1841 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1851 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1861 "lev_comp.y"
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

  case 270:

/* Line 1455 of yacc.c  */
#line 1876 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1886 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1890 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1896 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1902 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1908 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int)(yyvsp[(6) - (13)].i), (int)(yyvsp[(8) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1914 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1920 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1926 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1931 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1938 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1945 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1951 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1957 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1963 "lev_comp.y"
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

  case 285:

/* Line 1455 of yacc.c  */
#line 1977 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      if ( (yyvsp[(10) - (11)].i) ) {
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      } else if ( (yyvsp[(11) - (11)].i) )
			  lc_error("Cannot use lev statements in non-permanent REGION");
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1987 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1991 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1997 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 2004 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 2010 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 2016 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 2022 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 2028 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2032 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2037 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2044 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2050 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2057 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2061 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2068 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2079 "lev_comp.y"
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

  case 304:

/* Line 1455 of yacc.c  */
#line 2092 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2096 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2102 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2106 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2113 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2117 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2121 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2127 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2131 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2137 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2153 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2161 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2171 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2180 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2185 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2192 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2203 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2209 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2213 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2217 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2224 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2234 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2241 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2245 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2251 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2255 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2263 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2267 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2274 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2284 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2297 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2301 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2308 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2318 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2327 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2338 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2342 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2349 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2359 "lev_comp.y"
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

  case 353:

/* Line 1455 of yacc.c  */
#line 2370 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2379 "lev_comp.y"
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

  case 355:

/* Line 1455 of yacc.c  */
#line 2389 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2395 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2399 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2406 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2416 "lev_comp.y"
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

  case 360:

/* Line 1455 of yacc.c  */
#line 2427 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2436 "lev_comp.y"
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

  case 362:

/* Line 1455 of yacc.c  */
#line 2446 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2450 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2456 "lev_comp.y"
    { ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2458 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2464 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2468 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2472 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2476 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2484 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2493 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2497 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2501 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2505 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2509 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2512 "lev_comp.y"
    { ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2516 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2523 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2532 "lev_comp.y"
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

  case 384:

/* Line 1455 of yacc.c  */
#line 2573 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2577 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2584 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2591 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2601 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2605 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2613 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2617 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2621 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2625 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2629 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2634 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2638 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2642 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2647 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2652 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2657 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2661 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2666 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2671 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2675 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2679 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2684 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2688 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2695 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2701 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2705 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2711 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2727 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2731 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2735 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2739 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2761 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2765 "lev_comp.y"
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

  case 431:

/* Line 1455 of yacc.c  */
#line 2791 "lev_comp.y"
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
#line 6307 "lev_comp.tab.c"
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
#line 2819 "lev_comp.y"


/*lev_comp.y*/

