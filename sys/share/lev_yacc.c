
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
     SPECIAL_REGION_ID = 288,
     SPECIAL_REGION_TYPE = 289,
     NAMING_ID = 290,
     NAMING_TYPE = 291,
     FILLING = 292,
     IRREGULAR = 293,
     JOINED = 294,
     ALTAR_ID = 295,
     ANVIL_ID = 296,
     NPC_ID = 297,
     LADDER_ID = 298,
     STAIR_ID = 299,
     NON_DIGGABLE_ID = 300,
     NON_PASSWALL_ID = 301,
     ROOM_ID = 302,
     PORTAL_ID = 303,
     TELEPRT_ID = 304,
     BRANCH_ID = 305,
     LEV = 306,
     MINERALIZE_ID = 307,
     CORRIDOR_ID = 308,
     GOLD_ID = 309,
     ENGRAVING_ID = 310,
     FOUNTAIN_ID = 311,
     THRONE_ID = 312,
     MODRON_PORTAL_ID = 313,
     POOL_ID = 314,
     SINK_ID = 315,
     NONE = 316,
     RAND_CORRIDOR_ID = 317,
     DOOR_STATE = 318,
     LIGHT_STATE = 319,
     CURSE_TYPE = 320,
     ENGRAVING_TYPE = 321,
     DIRECTION = 322,
     RANDOM_TYPE = 323,
     RANDOM_TYPE_BRACKET = 324,
     A_REGISTER = 325,
     ALIGNMENT = 326,
     LEFT_OR_RIGHT = 327,
     CENTER = 328,
     TOP_OR_BOT = 329,
     ALTAR_TYPE = 330,
     UP_OR_DOWN = 331,
     ACTIVE_OR_INACTIVE = 332,
     MODRON_PORTAL_TYPE = 333,
     NPC_TYPE = 334,
     FOUNTAIN_TYPE = 335,
     SPECIAL_OBJECT_TYPE = 336,
     CMAP_TYPE = 337,
     FLOOR_TYPE = 338,
     FLOOR_TYPE_ID = 339,
     FLOOR_ID = 340,
     FLOOR_MAIN_TYPE = 341,
     ELEMENTAL_ENCHANTMENT_TYPE = 342,
     EXCEPTIONALITY_TYPE = 343,
     EXCEPTIONALITY_ID = 344,
     ELEMENTAL_ENCHANTMENT_ID = 345,
     ENCHANTMENT_ID = 346,
     CHARGES_ID = 347,
     SPECIAL_QUALITY_ID = 348,
     SPEFLAGS_ID = 349,
     SUBROOM_ID = 350,
     NAME_ID = 351,
     FLAGS_ID = 352,
     FLAG_TYPE = 353,
     MON_ATTITUDE = 354,
     MON_ALERTNESS = 355,
     SUBTYPE_ID = 356,
     MON_APPEARANCE = 357,
     ROOMDOOR_ID = 358,
     IF_ID = 359,
     ELSE_ID = 360,
     TERRAIN_ID = 361,
     HORIZ_OR_VERT = 362,
     REPLACE_TERRAIN_ID = 363,
     EXIT_ID = 364,
     SHUFFLE_ID = 365,
     QUANTITY_ID = 366,
     BURIED_ID = 367,
     LOOP_ID = 368,
     FOR_ID = 369,
     TO_ID = 370,
     SWITCH_ID = 371,
     CASE_ID = 372,
     BREAK_ID = 373,
     DEFAULT_ID = 374,
     ERODED_ID = 375,
     TRAPPED_STATE = 376,
     RECHARGED_ID = 377,
     INVIS_ID = 378,
     GREASED_ID = 379,
     FEMALE_ID = 380,
     WAITFORU_ID = 381,
     CANCELLED_ID = 382,
     REVIVED_ID = 383,
     AVENGE_ID = 384,
     FLEEING_ID = 385,
     BLINDED_ID = 386,
     PARALYZED_ID = 387,
     STUNNED_ID = 388,
     CONFUSED_ID = 389,
     SEENTRAPS_ID = 390,
     ALL_ID = 391,
     MONTYPE_ID = 392,
     GRAVE_ID = 393,
     ERODEPROOF_ID = 394,
     FUNCTION_ID = 395,
     MSG_OUTPUT_TYPE = 396,
     COMPARE_TYPE = 397,
     UNKNOWN_TYPE = 398,
     rect_ID = 399,
     fillrect_ID = 400,
     line_ID = 401,
     randline_ID = 402,
     grow_ID = 403,
     selection_ID = 404,
     flood_ID = 405,
     rndcoord_ID = 406,
     circle_ID = 407,
     ellipse_ID = 408,
     filter_ID = 409,
     complement_ID = 410,
     gradient_ID = 411,
     GRADIENT_TYPE = 412,
     LIMITED = 413,
     HUMIDITY_TYPE = 414,
     STRING = 415,
     MAP_ID = 416,
     NQSTRING = 417,
     VARSTRING = 418,
     CFUNC = 419,
     CFUNC_INT = 420,
     CFUNC_STR = 421,
     CFUNC_COORD = 422,
     CFUNC_REGION = 423,
     VARSTRING_INT = 424,
     VARSTRING_INT_ARRAY = 425,
     VARSTRING_STRING = 426,
     VARSTRING_STRING_ARRAY = 427,
     VARSTRING_VAR = 428,
     VARSTRING_VAR_ARRAY = 429,
     VARSTRING_COORD = 430,
     VARSTRING_COORD_ARRAY = 431,
     VARSTRING_REGION = 432,
     VARSTRING_REGION_ARRAY = 433,
     VARSTRING_MAPCHAR = 434,
     VARSTRING_MAPCHAR_ARRAY = 435,
     VARSTRING_MONST = 436,
     VARSTRING_MONST_ARRAY = 437,
     VARSTRING_OBJ = 438,
     VARSTRING_OBJ_ARRAY = 439,
     VARSTRING_SEL = 440,
     VARSTRING_SEL_ARRAY = 441,
     METHOD_INT = 442,
     METHOD_INT_ARRAY = 443,
     METHOD_STRING = 444,
     METHOD_STRING_ARRAY = 445,
     METHOD_VAR = 446,
     METHOD_VAR_ARRAY = 447,
     METHOD_COORD = 448,
     METHOD_COORD_ARRAY = 449,
     METHOD_REGION = 450,
     METHOD_REGION_ARRAY = 451,
     METHOD_MAPCHAR = 452,
     METHOD_MAPCHAR_ARRAY = 453,
     METHOD_MONST = 454,
     METHOD_MONST_ARRAY = 455,
     METHOD_OBJ = 456,
     METHOD_OBJ_ARRAY = 457,
     METHOD_SEL = 458,
     METHOD_SEL_ARRAY = 459,
     DICE = 460
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
#line 503 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 515 "lev_comp.tab.c"

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
#define YYLAST   1115

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  223
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  171
/* YYNRULES -- Number of rules.  */
#define YYNRULES  436
/* YYNRULES -- Number of states.  */
#define YYNSTATES  957

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   460

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   218,   222,     2,
     162,   163,   216,   214,   160,   215,   220,   217,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   161,     2,
       2,   219,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   164,     2,   165,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   166,   221,   167,     2,     2,     2,     2,
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
     155,   156,   157,   158,   159,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213
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
     239,   241,   243,   245,   247,   249,   253,   257,   263,   267,
     273,   279,   285,   289,   293,   299,   305,   311,   319,   327,
     335,   341,   343,   347,   349,   353,   355,   359,   361,   365,
     367,   371,   373,   377,   379,   383,   384,   385,   394,   399,
     401,   402,   404,   406,   412,   416,   417,   418,   428,   429,
     432,   433,   439,   440,   445,   447,   450,   452,   459,   460,
     464,   465,   472,   473,   478,   479,   484,   486,   487,   492,
     496,   498,   502,   506,   512,   518,   526,   531,   532,   544,
     545,   559,   560,   563,   569,   571,   577,   579,   585,   587,
     593,   595,   605,   611,   613,   615,   617,   619,   621,   625,
     627,   629,   631,   639,   645,   647,   649,   651,   653,   657,
     658,   664,   669,   670,   674,   676,   678,   680,   682,   685,
     687,   689,   691,   693,   695,   699,   703,   707,   709,   711,
     715,   717,   719,   721,   725,   729,   730,   736,   739,   740,
     744,   746,   750,   752,   756,   760,   762,   764,   768,   770,
     772,   774,   778,   780,   782,   784,   788,   792,   796,   800,
     804,   808,   814,   822,   828,   837,   839,   843,   849,   855,
     863,   871,   878,   884,   885,   888,   894,   898,   912,   916,
     920,   922,   928,   938,   944,   948,   952,   958,   968,   974,
     975,   987,   988,   990,   998,  1002,  1010,  1016,  1022,  1028,
    1034,  1038,  1044,  1052,  1062,  1064,  1066,  1068,  1070,  1072,
    1073,  1076,  1078,  1082,  1084,  1086,  1088,  1089,  1093,  1095,
    1097,  1099,  1101,  1103,  1105,  1107,  1109,  1111,  1113,  1117,
    1119,  1121,  1126,  1128,  1130,  1135,  1137,  1139,  1144,  1146,
    1151,  1157,  1159,  1163,  1165,  1169,  1171,  1173,  1178,  1188,
    1190,  1192,  1197,  1199,  1205,  1207,  1209,  1214,  1216,  1218,
    1224,  1226,  1228,  1230,  1235,  1237,  1239,  1245,  1247,  1249,
    1251,  1255,  1257,  1259,  1263,  1265,  1270,  1274,  1278,  1282,
    1286,  1290,  1294,  1296,  1298,  1302,  1304,  1308,  1309,  1311,
    1313,  1315,  1317,  1321,  1322,  1324,  1326,  1329,  1332,  1337,
    1344,  1349,  1356,  1363,  1370,  1377,  1380,  1387,  1396,  1405,
    1416,  1431,  1434,  1436,  1440,  1442,  1446,  1448,  1450,  1452,
    1454,  1456,  1458,  1460,  1462,  1464,  1466,  1468,  1470,  1472,
    1474,  1476,  1478,  1480,  1482,  1484,  1495
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     224,     0,    -1,    -1,   225,    -1,   226,    -1,   226,   225,
      -1,   227,   234,   236,    -1,    16,   161,   168,    -1,    15,
     161,   168,   160,     3,    -1,    17,   161,    11,   160,   328,
      -1,    17,   161,    10,   160,     3,    -1,    17,   161,    13,
      -1,    17,   161,    12,   160,     3,   160,     3,   160,     5,
     160,     5,   160,   355,   160,   233,   232,    -1,    18,   161,
     385,    -1,    -1,   160,   158,    -1,    -1,   160,   362,    -1,
      -1,   160,     3,    -1,     5,    -1,    68,    -1,    -1,    97,
     161,   235,    -1,    98,   160,   235,    -1,    98,    -1,    -1,
     238,   236,    -1,   166,   236,   167,    -1,   279,    -1,   228,
      -1,   229,    -1,   341,    -1,   340,    -1,   338,    -1,   339,
      -1,   342,    -1,   343,    -1,   321,    -1,   281,    -1,   244,
      -1,   243,    -1,   331,    -1,   293,    -1,   313,    -1,   345,
      -1,   346,    -1,   323,    -1,   324,    -1,   325,    -1,   344,
      -1,   259,    -1,   269,    -1,   271,    -1,   275,    -1,   273,
      -1,   256,    -1,   266,    -1,   252,    -1,   255,    -1,   316,
      -1,   298,    -1,   314,    -1,   301,    -1,   307,    -1,   332,
      -1,   327,    -1,   319,    -1,   280,    -1,   333,    -1,   334,
      -1,   335,    -1,   286,    -1,   284,    -1,   326,    -1,   330,
      -1,   329,    -1,   317,    -1,   318,    -1,   320,    -1,   312,
      -1,   315,    -1,   178,    -1,   180,    -1,   182,    -1,   184,
      -1,   186,    -1,   188,    -1,   190,    -1,   192,    -1,   194,
      -1,   177,    -1,   179,    -1,   181,    -1,   183,    -1,   185,
      -1,   187,    -1,   189,    -1,   191,    -1,   193,    -1,   239,
      -1,   240,    -1,   171,    -1,   171,    -1,   240,    -1,   110,
     161,   239,    -1,   241,   219,   374,    -1,   241,   219,   149,
     161,   383,    -1,   241,   219,   373,    -1,   241,   219,   390,
     161,   367,    -1,   241,   219,   389,   161,   369,    -1,   241,
     219,   388,   161,   371,    -1,   241,   219,   362,    -1,   241,
     219,   365,    -1,   241,   219,   166,   250,   167,    -1,   241,
     219,   166,   249,   167,    -1,   241,   219,   166,   248,   167,
      -1,   241,   219,   390,   161,   166,   247,   167,    -1,   241,
     219,   389,   161,   166,   246,   167,    -1,   241,   219,   388,
     161,   166,   245,   167,    -1,   241,   219,   166,   251,   167,
      -1,   372,    -1,   245,   160,   372,    -1,   370,    -1,   246,
     160,   370,    -1,   368,    -1,   247,   160,   368,    -1,   366,
      -1,   248,   160,   366,    -1,   363,    -1,   249,   160,   363,
      -1,   374,    -1,   250,   160,   374,    -1,   373,    -1,   251,
     160,   373,    -1,    -1,    -1,   140,   170,   162,   253,   378,
     163,   254,   237,    -1,   170,   162,   381,   163,    -1,   109,
      -1,    -1,     6,    -1,     6,    -1,   164,   374,   142,   374,
     165,    -1,   164,   374,   165,    -1,    -1,    -1,   116,   260,
     164,   361,   165,   261,   166,   262,   167,    -1,    -1,   263,
     262,    -1,    -1,   117,   386,   161,   264,   236,    -1,    -1,
     119,   161,   265,   236,    -1,   118,    -1,   220,   220,    -1,
     115,    -1,   114,   242,   219,   374,   267,   374,    -1,    -1,
     268,   270,   237,    -1,    -1,   113,   164,   361,   165,   272,
     237,    -1,    -1,   258,   161,   274,   238,    -1,    -1,   104,
     258,   276,   277,    -1,   237,    -1,    -1,   237,   278,   105,
     237,    -1,    14,   161,   373,    -1,    62,    -1,    62,   161,
     386,    -1,    62,   161,    68,    -1,    53,   161,   282,   160,
     282,    -1,    53,   161,   282,   160,   386,    -1,   162,     4,
     160,    67,   160,   297,   163,    -1,   348,   257,   160,   355,
      -1,    -1,    95,   161,   283,   160,   290,   160,   292,   349,
     352,   285,   237,    -1,    -1,    47,   161,   283,   160,   289,
     160,   291,   160,   292,   349,   352,   287,   237,    -1,    -1,
     160,     5,    -1,   162,     4,   160,     4,   163,    -1,    68,
      -1,   162,     4,   160,     4,   163,    -1,    68,    -1,   162,
     299,   160,   300,   163,    -1,    68,    -1,   162,     4,   160,
       4,   163,    -1,    68,    -1,   103,   161,   294,   160,   354,
     160,   295,   160,   297,    -1,    25,   161,   354,   160,   383,
      -1,     5,    -1,    68,    -1,   296,    -1,    68,    -1,    67,
      -1,    67,   221,   296,    -1,     4,    -1,    68,    -1,    20,
      -1,    19,   161,   299,   160,   300,   288,   169,    -1,    19,
     161,   362,   288,   169,    -1,    72,    -1,    73,    -1,    74,
      -1,    73,    -1,    23,   161,   303,    -1,    -1,    23,   161,
     303,   302,   237,    -1,   369,   160,   362,   304,    -1,    -1,
     304,   160,   305,    -1,   373,    -1,    99,    -1,   100,    -1,
     357,    -1,   102,   373,    -1,   125,    -1,   123,    -1,   127,
      -1,   128,    -1,   129,    -1,   130,   161,   361,    -1,   131,
     161,   361,    -1,   132,   161,   361,    -1,   133,    -1,   134,
      -1,   135,   161,   306,    -1,   126,    -1,   168,    -1,   136,
      -1,   168,   221,   306,    -1,    21,   161,   309,    -1,    -1,
      22,   161,   309,   308,   237,    -1,   371,   310,    -1,    -1,
     310,   160,   311,    -1,    65,    -1,   137,   161,   369,    -1,
     387,    -1,    96,   161,   373,    -1,   111,   161,   361,    -1,
     112,    -1,    64,    -1,   120,   161,   361,    -1,   139,    -1,
      63,    -1,   121,    -1,   122,   161,   361,    -1,   123,    -1,
     124,    -1,   362,    -1,    90,   161,    87,    -1,    89,   161,
      88,    -1,    91,   161,   361,    -1,    92,   161,   361,    -1,
      93,   161,   361,    -1,    94,   161,   361,    -1,    24,   161,
     347,   160,   362,    -1,    26,   161,   362,   160,    67,   160,
     354,    -1,    30,   161,   362,   160,    67,    -1,    30,   161,
     362,   160,    67,   160,     5,   232,    -1,    31,    -1,    31,
     161,   383,    -1,    43,   161,   362,   160,    76,    -1,    44,
     161,   362,   160,    76,    -1,    44,   161,   392,   160,   392,
     160,    76,    -1,    48,   161,   392,   160,   392,   160,   168,
      -1,    49,   161,   392,   160,   392,   322,    -1,    50,   161,
     392,   160,   392,    -1,    -1,   160,    76,    -1,    56,   161,
     383,   160,    80,    -1,    57,   161,   383,    -1,    58,   161,
     362,   160,   162,   386,   160,   386,   163,   160,    78,   160,
      77,    -1,    60,   161,   383,    -1,    59,   161,   383,    -1,
       3,    -1,   162,     3,   160,   355,   163,    -1,   108,   161,
     365,   160,   367,   160,   367,   160,     7,    -1,   106,   161,
     383,   160,   367,    -1,    45,   161,   365,    -1,    46,   161,
     365,    -1,    35,   161,   168,   160,    36,    -1,    35,   161,
     168,   160,    36,   160,   137,   161,   369,    -1,    33,   161,
     365,   160,    34,    -1,    -1,    32,   161,   365,   160,   355,
     160,   348,   349,   352,   336,   337,    -1,    -1,   237,    -1,
      40,   161,   362,   160,   356,   160,   358,    -1,    41,   161,
     362,    -1,    85,   161,   383,   160,    86,   160,    83,    -1,
     101,   161,   383,   160,     4,    -1,    42,   161,    79,   160,
     362,    -1,   138,   161,   362,   160,   373,    -1,   138,   161,
     362,   160,    68,    -1,   138,   161,   362,    -1,    54,   161,
     374,   160,   362,    -1,    55,   161,   362,   160,   391,   160,
     373,    -1,    52,   161,   361,   160,   361,   160,   361,   160,
     361,    -1,    52,    -1,   168,    -1,    68,    -1,   168,    -1,
      68,    -1,    -1,   160,   350,    -1,   351,    -1,   351,   160,
     350,    -1,    37,    -1,    38,    -1,    39,    -1,    -1,    84,
     161,   353,    -1,    83,    -1,    63,    -1,    68,    -1,    64,
      -1,    68,    -1,    71,    -1,   359,    -1,    68,    -1,    71,
      -1,   359,    -1,    70,   161,    68,    -1,    75,    -1,    68,
      -1,    70,   164,     4,   165,    -1,   168,    -1,   179,    -1,
     180,   164,   374,   165,    -1,   374,    -1,   363,    -1,   151,
     162,   383,   163,    -1,   183,    -1,   184,   164,   374,   165,
      -1,   162,     4,   160,     4,   163,    -1,    68,    -1,    69,
     364,   165,    -1,   159,    -1,   159,   160,   364,    -1,   366,
      -1,   185,    -1,   186,   164,   374,   165,    -1,   162,     4,
     160,     4,   160,     4,   160,     4,   163,    -1,   368,    -1,
     187,    -1,   188,   164,   374,   165,    -1,     3,    -1,   162,
       3,   160,   355,   163,    -1,   370,    -1,   189,    -1,   190,
     164,   374,   165,    -1,   168,    -1,     3,    -1,   162,     3,
     160,   168,   163,    -1,    68,    -1,   372,    -1,   191,    -1,
     192,   164,   374,   165,    -1,   168,    -1,     3,    -1,   162,
       3,   160,   168,   163,    -1,    68,    -1,    81,    -1,   360,
      -1,   373,   220,   360,    -1,     4,    -1,   384,    -1,   162,
       8,   163,    -1,   177,    -1,   178,   164,   374,   165,    -1,
     374,   214,   374,    -1,   374,   215,   374,    -1,   374,   216,
     374,    -1,   374,   217,   374,    -1,   374,   218,   374,    -1,
     162,   374,   163,    -1,   173,    -1,   174,    -1,   241,   161,
     375,    -1,   376,    -1,   377,   160,   376,    -1,    -1,   377,
      -1,   374,    -1,   373,    -1,   379,    -1,   380,   160,   379,
      -1,    -1,   380,    -1,   362,    -1,   144,   365,    -1,   145,
     365,    -1,   146,   362,   160,   362,    -1,   147,   362,   160,
     362,   160,   374,    -1,   148,   162,   383,   163,    -1,   148,
     162,   296,   160,   383,   163,    -1,   154,   162,     7,   160,
     383,   163,    -1,   154,   162,   383,   160,   383,   163,    -1,
     154,   162,   367,   160,   383,   163,    -1,   150,   362,    -1,
     152,   162,   362,   160,   374,   163,    -1,   152,   162,   362,
     160,   374,   160,    37,   163,    -1,   153,   162,   362,   160,
     374,   160,   374,   163,    -1,   153,   162,   362,   160,   374,
     160,   374,   160,    37,   163,    -1,   156,   162,   157,   160,
     162,   374,   215,   374,   230,   163,   160,   362,   231,   163,
      -1,   155,   382,    -1,   193,    -1,   162,   383,   163,    -1,
     382,    -1,   382,   222,   383,    -1,   213,    -1,    82,    -1,
       4,    -1,     8,    -1,     9,    -1,     4,    -1,     8,    -1,
       9,    -1,     4,    -1,   384,    -1,    27,    -1,    21,    -1,
      28,    -1,    23,    -1,    29,    -1,   106,    -1,    66,    -1,
      68,    -1,   393,    -1,    51,   162,     4,   160,     4,   160,
       4,   160,     4,   163,    -1,   162,     4,   160,     4,   160,
       4,   160,     4,   163,    -1
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
     531,   532,   535,   536,   537,   538,   539,   540,   541,   542,
     543,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     557,   558,   559,   562,   563,   566,   582,   588,   594,   600,
     606,   612,   618,   624,   630,   640,   650,   660,   670,   680,
     690,   702,   707,   714,   719,   726,   731,   738,   742,   748,
     753,   760,   764,   770,   774,   781,   803,   780,   817,   872,
     879,   882,   888,   895,   899,   908,   912,   907,   975,   976,
     980,   979,   993,   992,  1007,  1017,  1018,  1021,  1059,  1058,
    1093,  1092,  1123,  1122,  1155,  1154,  1180,  1191,  1190,  1218,
    1224,  1229,  1234,  1241,  1248,  1257,  1265,  1277,  1276,  1295,
    1294,  1313,  1316,  1322,  1332,  1338,  1347,  1353,  1358,  1364,
    1369,  1375,  1386,  1392,  1393,  1396,  1397,  1400,  1404,  1410,
    1411,  1414,  1421,  1429,  1437,  1438,  1441,  1442,  1445,  1450,
    1449,  1463,  1470,  1477,  1485,  1490,  1496,  1502,  1508,  1514,
    1519,  1524,  1529,  1534,  1539,  1544,  1549,  1554,  1559,  1564,
    1570,  1577,  1586,  1590,  1603,  1612,  1611,  1629,  1639,  1645,
    1653,  1659,  1664,  1669,  1674,  1679,  1684,  1689,  1694,  1699,
    1710,  1716,  1721,  1726,  1731,  1736,  1741,  1746,  1751,  1756,
    1761,  1768,  1774,  1803,  1808,  1816,  1822,  1828,  1835,  1842,
    1852,  1862,  1877,  1888,  1891,  1897,  1903,  1909,  1915,  1921,
    1927,  1932,  1939,  1946,  1952,  1958,  1964,  1968,  1974,  1981,
    1980,  2005,  2008,  2014,  2021,  2027,  2033,  2039,  2045,  2049,
    2054,  2061,  2067,  2074,  2078,  2085,  2093,  2096,  2106,  2110,
    2113,  2119,  2123,  2130,  2134,  2138,  2145,  2148,  2154,  2160,
    2161,  2164,  2165,  2168,  2169,  2170,  2176,  2177,  2178,  2184,
    2185,  2188,  2197,  2202,  2209,  2220,  2226,  2230,  2234,  2241,
    2251,  2258,  2262,  2268,  2272,  2280,  2284,  2291,  2301,  2314,
    2318,  2325,  2335,  2344,  2355,  2359,  2366,  2376,  2387,  2396,
    2406,  2412,  2416,  2423,  2433,  2444,  2453,  2463,  2467,  2474,
    2475,  2481,  2485,  2489,  2493,  2501,  2510,  2514,  2518,  2522,
    2526,  2530,  2533,  2540,  2549,  2582,  2583,  2586,  2587,  2590,
    2594,  2601,  2608,  2619,  2622,  2630,  2634,  2638,  2642,  2646,
    2651,  2655,  2659,  2664,  2669,  2674,  2678,  2683,  2688,  2692,
    2696,  2701,  2705,  2712,  2718,  2722,  2728,  2735,  2736,  2739,
    2740,  2741,  2744,  2748,  2752,  2756,  2762,  2763,  2766,  2767,
    2770,  2771,  2774,  2775,  2778,  2782,  2808
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
  "WALLIFY_ID", "REGION_ID", "SPECIAL_REGION_ID", "SPECIAL_REGION_TYPE",
  "NAMING_ID", "NAMING_TYPE", "FILLING", "IRREGULAR", "JOINED", "ALTAR_ID",
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
  "diggable_detail", "passwall_detail", "naming_detail",
  "special_region_detail", "region_detail", "@16", "region_detail_end",
  "altar_detail", "anvil_detail", "floor_detail", "subtype_detail",
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
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
      44,    58,    40,    41,    91,    93,   123,   125,   415,   416,
     417,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,    43,    45,    42,    47,    37,    61,
      46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   223,   224,   224,   225,   225,   226,   227,   227,   228,
     228,   228,   228,   229,   230,   230,   231,   231,   232,   232,
     233,   233,   234,   234,   235,   235,   236,   236,   237,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     241,   241,   241,   242,   242,   243,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   245,   245,   246,   246,   247,   247,   248,   248,   249,
     249,   250,   250,   251,   251,   253,   254,   252,   255,   256,
     257,   257,   258,   258,   258,   260,   261,   259,   262,   262,
     264,   263,   265,   263,   266,   267,   267,   268,   270,   269,
     272,   271,   274,   273,   276,   275,   277,   278,   277,   279,
     280,   280,   280,   281,   281,   282,   283,   285,   284,   287,
     286,   288,   288,   289,   289,   290,   290,   291,   291,   292,
     292,   293,   293,   294,   294,   295,   295,   296,   296,   297,
     297,   298,   298,   298,   299,   299,   300,   300,   301,   302,
     301,   303,   304,   304,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   306,   306,   306,   307,   308,   307,   309,   310,   310,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   312,   313,   314,   314,   315,   315,   316,   317,   318,
     319,   320,   321,   322,   322,   323,   324,   325,   326,   327,
     328,   328,   329,   330,   331,   332,   333,   333,   334,   336,
     335,   337,   337,   338,   339,   340,   341,   342,   343,   343,
     343,   344,   345,   346,   346,   347,   347,   348,   348,   349,
     349,   350,   350,   351,   351,   351,   352,   352,   353,   354,
     354,   355,   355,   356,   356,   356,   357,   357,   357,   358,
     358,   359,   360,   360,   360,   361,   362,   362,   362,   362,
     363,   363,   363,   364,   364,   365,   365,   365,   366,   367,
     367,   367,   368,   368,   369,   369,   369,   370,   370,   370,
     370,   371,   371,   371,   372,   372,   372,   372,   372,   373,
     373,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   375,   375,   376,   377,   377,   378,   378,   379,
     379,   380,   380,   381,   381,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   383,   383,   384,   385,   385,   386,
     386,   386,   387,   387,   387,   387,   388,   388,   389,   389,
     390,   390,   391,   391,   392,   392,   393
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
       1,     1,     1,     1,     1,     3,     3,     5,     3,     5,
       5,     5,     3,     3,     5,     5,     5,     7,     7,     7,
       5,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     0,     0,     8,     4,     1,
       0,     1,     1,     5,     3,     0,     0,     9,     0,     2,
       0,     5,     0,     4,     1,     2,     1,     6,     0,     3,
       0,     6,     0,     4,     0,     4,     1,     0,     4,     3,
       1,     3,     3,     5,     5,     7,     4,     0,    11,     0,
      13,     0,     2,     5,     1,     5,     1,     5,     1,     5,
       1,     9,     5,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     7,     5,     1,     1,     1,     1,     3,     0,
       5,     4,     0,     3,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     3,     3,     3,     1,     1,     3,
       1,     1,     1,     3,     3,     0,     5,     2,     0,     3,
       1,     3,     1,     3,     3,     1,     1,     3,     1,     1,
       1,     3,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     5,     7,     5,     8,     1,     3,     5,     5,     7,
       7,     6,     5,     0,     2,     5,     3,    13,     3,     3,
       1,     5,     9,     5,     3,     3,     5,     9,     5,     0,
      11,     0,     1,     7,     3,     7,     5,     5,     5,     5,
       3,     5,     7,     9,     1,     1,     1,     1,     1,     0,
       2,     1,     3,     1,     1,     1,     0,     3,     1,     1,
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
       2,     0,     0,     0,     3,     4,    22,     0,     0,     1,
       5,     0,    26,     0,     7,     0,   142,     0,     0,     0,
       0,   201,     0,     0,     0,     0,     0,     0,     0,   265,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,     0,     0,     0,     0,     0,
       0,     0,     0,   170,     0,     0,     0,     0,     0,     0,
       0,   139,     0,     0,     0,   145,   154,     0,     0,     0,
       0,   102,    91,    82,    92,    83,    93,    84,    94,    85,
      95,    86,    96,    87,    97,    88,    98,    89,    99,    90,
      30,    31,     6,    26,   100,   101,     0,    41,    40,    58,
      59,    56,     0,    51,    57,   158,    52,    53,    55,    54,
      29,    68,    39,    73,    72,    43,    61,    63,    64,    80,
      44,    62,    81,    60,    77,    78,    67,    79,    38,    47,
      48,    49,    74,    66,    76,    75,    42,    65,    69,    70,
      71,    34,    35,    33,    32,    36,    37,    50,    45,    46,
       0,    25,    23,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   164,     0,     0,     0,     0,   103,   104,
       0,     0,     0,     0,   371,     0,   374,     0,   416,     0,
     372,   393,    27,     0,   162,     0,     8,     0,   332,   333,
       0,   369,   169,     0,     0,     0,    11,   418,   417,    13,
     341,     0,   204,   205,     0,     0,   338,     0,     0,   181,
     336,   365,   367,   368,     0,   364,   362,     0,   234,   238,
     361,   235,   358,   360,     0,   357,   355,     0,   208,     0,
     354,   306,   305,     0,   319,   320,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   412,   395,   414,   266,     0,   346,     0,     0,   345,
       0,     0,     0,   294,     0,     0,     0,     0,     0,     0,
     434,   284,   285,   308,   307,     0,   140,     0,     0,     0,
       0,     0,   335,     0,     0,     0,     0,     0,   276,     0,
     279,   278,   421,   419,   420,   172,   171,     0,     0,     0,
     193,   194,     0,     0,     0,     0,   105,     0,     0,     0,
     300,   135,     0,     0,     0,     0,   144,     0,     0,     0,
       0,     0,   390,   389,   391,   394,     0,   427,   429,   426,
     428,   430,   431,     0,     0,     0,   112,   113,   108,   106,
       0,     0,     0,     0,    26,   159,    24,     0,     0,     0,
       0,     0,   343,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   396,   397,     0,     0,     0,   405,     0,     0,
       0,   411,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   141,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   166,   165,     0,     0,   160,
       0,     0,     0,   387,   373,   381,     0,     0,   376,   377,
     378,   379,   380,     0,   138,     0,   371,     0,     0,     0,
       0,   129,   127,   133,   131,     0,     0,     0,   163,     0,
       0,   370,    10,   280,     0,     9,     0,     0,   342,     0,
       0,     0,   207,   206,   181,   182,   203,     0,     0,     0,
     236,     0,     0,   210,   212,   261,   192,     0,   263,     0,
       0,   197,     0,     0,     0,     0,   352,     0,     0,   350,
       0,     0,   349,     0,     0,   413,   415,     0,     0,   321,
     322,     0,   288,   286,   325,     0,   323,     0,   324,   297,
     267,     0,     0,   268,     0,   184,     0,     0,     0,     0,
       0,   273,   272,     0,     0,   173,   174,   301,   432,   433,
       0,   275,     0,     0,   186,     0,     0,   296,     0,     0,
       0,   283,     0,     0,   156,     0,     0,   146,   299,   298,
       0,   385,   388,     0,   375,   143,   392,   107,     0,     0,
     116,     0,   115,     0,   114,     0,   120,     0,   111,     0,
     110,     0,   109,    28,   334,     0,     0,   344,   337,     0,
     339,     0,     0,   363,   424,   422,   423,   249,   246,   240,
       0,     0,     0,     0,     0,     0,     0,     0,   245,     0,
     250,     0,   252,   253,     0,   248,   239,   254,   425,   242,
       0,   356,   211,     0,     0,   398,     0,     0,     0,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   176,
       0,     0,     0,   271,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   161,   155,   157,     0,     0,     0,
     136,     0,   128,   130,   132,   134,     0,   121,     0,   123,
       0,   125,     0,     0,   340,   202,   366,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,     0,
     262,    18,     0,   198,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   309,     0,     0,   330,   329,   293,
       0,     0,   269,     0,   188,     0,     0,   270,   274,     0,
       0,   302,     0,   295,     0,   190,     0,   309,   196,     0,
     195,   168,     0,   148,   382,   383,   384,   386,     0,     0,
     119,     0,   118,     0,   117,     0,     0,   256,   255,   257,
     258,   259,   260,   243,   244,   247,   251,   241,     0,   326,
     215,   216,     0,   220,   219,   230,   221,   222,   223,     0,
       0,     0,   227,   228,     0,   213,   217,   327,   214,     0,
     264,   399,   401,     0,   406,     0,   402,     0,   351,   404,
     403,     0,     0,     0,   316,     0,   331,     0,     0,     0,
       0,     0,     0,   199,   200,     0,     0,     0,     0,   316,
       0,     0,     0,     0,     0,   148,   137,   122,   124,   126,
     281,     0,     0,   218,     0,     0,     0,     0,    19,     0,
       0,   353,     0,     0,   313,   314,   315,   310,   311,     0,
     289,   287,     0,     0,   183,     0,   309,   303,   175,     0,
     185,     0,   177,   191,   282,     0,   152,   147,   149,     0,
     328,   224,   225,   226,   232,   231,   229,   407,     0,   408,
     377,     0,     0,     0,   291,     0,     0,     0,   316,     0,
       0,     0,   150,    26,     0,     0,     0,     0,     0,   348,
     312,   318,   317,   292,   290,     0,   436,   187,   179,     0,
     189,   178,    26,   153,     0,   233,   409,    15,     0,   435,
       0,     0,   151,     0,     0,   180,   277,     0,    16,    20,
      21,    18,     0,     0,    12,    17,   410
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    90,    91,   918,   953,   810,
     951,    12,   152,    92,   375,    93,    94,    95,    96,   200,
      97,    98,   696,   698,   700,   467,   468,   469,   470,    99,
     453,   768,   100,   101,   429,   102,   103,   201,   687,   844,
     845,   932,   913,   104,   576,   105,   106,   215,   107,   573,
     108,   373,   109,   333,   446,   569,   110,   111,   112,   314,
     305,   113,   911,   114,   940,   389,   547,   566,   746,   757,
     115,   332,   759,   512,   835,   116,   238,   494,   117,   396,
     258,   642,   805,   896,   118,   393,   248,   392,   636,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   673,
     129,   130,   131,   132,   133,   485,   134,   135,   136,   137,
     138,   139,   140,   904,   924,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   263,   306,   824,   867,   868,   870,
     922,   266,   531,   537,   806,   739,   538,   221,   311,   282,
     240,   383,   288,   289,   521,   522,   259,   260,   249,   250,
     352,   312,   766,   581,   582,   583,   354,   355,   356,   283,
     413,   210,   229,   326,   639,   370,   371,   372,   560,   299,
     300
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -759
static const yytype_int16 yypact[] =
{
     110,   -91,   -25,   142,  -759,   110,    58,    -8,     1,  -759,
    -759,    39,   665,    27,  -759,    98,  -759,    59,    71,    77,
      84,  -759,    89,   118,   138,   188,   198,   202,   205,   214,
     220,   227,   228,   239,   241,   244,   246,   248,   270,   271,
     274,   276,   277,   282,   292,   304,   308,   310,   312,   314,
     316,   318,   331,   332,   343,   351,   352,   353,    67,   371,
     376,  -759,   379,   186,   741,  -759,  -759,   380,   191,    44,
     203,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,
    -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,
    -759,  -759,  -759,   665,  -759,  -759,   173,  -759,  -759,  -759,
    -759,  -759,   383,  -759,  -759,  -759,  -759,  -759,  -759,  -759,
    -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,
    -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,
    -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,
    -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,
     401,   280,  -759,   -81,   330,    72,   279,   124,   124,    40,
     -27,    52,   -19,   -19,   730,   -76,   -76,   266,   -19,   -19,
     412,   -19,     6,   -76,   -76,     4,     8,     8,     8,    44,
     334,    44,   -19,   730,   730,   -19,   730,   730,    80,   730,
       4,   730,    88,  -759,   730,   -76,   286,    44,  -759,  -759,
     328,   367,   -19,   399,  -759,    47,  -759,   398,  -759,   268,
    -759,   159,  -759,    33,  -759,   415,  -759,    98,  -759,  -759,
     419,  -759,   369,   432,   433,   448,  -759,  -759,  -759,  -759,
    -759,   450,  -759,  -759,   452,   606,  -759,   453,   455,   456,
    -759,  -759,  -759,  -759,   609,  -759,  -759,   454,  -759,  -759,
    -759,  -759,  -759,  -759,   616,  -759,  -759,   462,   470,   460,
    -759,  -759,  -759,   478,  -759,  -759,   480,   481,   483,   -76,
     -76,   -19,   -19,   475,   -19,   484,   485,   486,   730,   492,
     477,  -759,  -759,   434,  -759,   651,  -759,   493,   498,  -759,
     499,   503,   504,  -759,   505,   506,   511,   670,   516,   517,
    -759,  -759,  -759,  -759,  -759,   518,   669,   677,   533,   534,
     539,   541,   340,   698,   543,   204,   544,   556,  -759,   566,
    -759,  -759,  -759,  -759,  -759,  -759,  -759,   568,   574,   575,
    -759,  -759,   576,   415,   577,   578,  -759,   581,    44,    44,
     579,  -759,   584,   243,    44,    44,  -759,    44,    44,    44,
      44,    44,   369,   340,  -759,   580,   585,  -759,  -759,  -759,
    -759,  -759,  -759,   588,    87,    93,  -759,  -759,   369,   340,
     590,   591,   593,   665,   665,  -759,  -759,    44,   -81,   750,
      31,   753,   598,   594,   730,   601,    44,   189,   758,   595,
     605,    44,   607,   415,   610,    44,   415,   -19,   -19,   730,
     705,   709,  -759,  -759,   617,   620,   717,  -759,   -19,   -19,
     355,  -759,   625,   624,   730,   628,    44,    43,   756,   755,
     225,   -19,   716,   789,   634,   719,     8,   -22,  -759,   636,
     640,     8,     8,     8,    44,   641,    36,   -19,    26,   724,
     654,   731,   -15,   814,    52,   714,  -759,   146,   146,  -759,
     162,   656,    -2,   782,  -759,  -759,   350,   356,    94,    94,
    -759,  -759,  -759,   159,  -759,   730,   662,   -64,   -55,   -54,
     -37,  -759,  -759,   369,   340,   368,   135,   132,  -759,   657,
     370,  -759,  -759,  -759,   820,  -759,   666,   450,  -759,   664,
     821,   435,  -759,  -759,   456,  -759,  -759,   660,   515,   305,
    -759,   663,   527,  -759,  -759,  -759,  -759,   672,   673,   -19,
     -19,   613,   678,   667,   680,   681,  -759,   700,   451,  -759,
     702,   729,  -759,   733,   734,  -759,  -759,   833,   592,  -759,
    -759,   735,  -759,   736,  -759,   726,  -759,   737,  -759,  -759,
    -759,   738,   887,  -759,   739,  -759,   898,   743,    43,   900,
     745,   746,  -759,   747,   841,  -759,  -759,  -759,  -759,  -759,
     749,  -759,   231,   751,  -759,   911,   761,  -759,   765,   822,
     926,  -759,   771,   415,  -759,   713,    44,  -759,  -759,   369,
     774,  -759,   776,   775,  -759,  -759,  -759,  -759,   933,   777,
    -759,    14,  -759,    44,  -759,   -81,  -759,    65,  -759,    66,
    -759,    35,  -759,  -759,  -759,   781,   951,  -759,  -759,   792,
    -759,   787,   794,  -759,  -759,  -759,  -759,  -759,  -759,  -759,
     797,   816,   818,   819,   823,   825,   826,   827,  -759,   828,
    -759,   829,  -759,  -759,   830,  -759,  -759,  -759,  -759,  -759,
     815,  -759,   832,    52,   976,  -759,   834,   915,   730,  -759,
      44,    44,   730,   835,    44,   730,   730,   831,   838,  -759,
       4,   846,   995,    56,   996,    23,   925,   842,     9,  -759,
     843,   836,   929,  -759,    44,   847,   -81,   848,   923,   849,
      17,   200,   415,   146,  -759,  -759,   340,   844,   149,   782,
    -759,   120,  -759,  -759,   340,   369,   -23,  -759,    13,  -759,
      21,  -759,    43,   851,  -759,  -759,  -759,   924,   927,    44,
      44,    44,    44,   -81,    44,    44,    44,    40,  -759,   817,
    -759,   853,    44,  -759,   852,   169,   335,   854,    43,   597,
     855,   856,    44,  1012,   860,   861,   858,  -759,  -759,  -759,
     864,  1017,  -759,  1021,  -759,   273,   866,  -759,  -759,   867,
      61,   369,   231,  -759,  1024,  -759,  1025,   860,  -759,   870,
    -759,  -759,   871,   211,  -759,  -759,  -759,  -759,   415,    65,
    -759,    66,  -759,    35,  -759,   869,  1028,  -759,  -759,  -759,
    -759,  -759,  -759,   369,  -759,  -759,  -759,  -759,    30,  -759,
    -759,  -759,   -81,  -759,  -759,  -759,  -759,  -759,  -759,   873,
     874,   875,  -759,  -759,   876,  -759,  -759,  -759,   369,  1035,
    -759,   340,  -759,  1002,  -759,    44,  -759,   877,  -759,  -759,
    -759,   362,   881,   413,   958,    40,  -759,  1039,   884,   882,
     886,    17,    44,  -759,  -759,   885,   888,   889,   890,   958,
      61,  1040,   231,   892,   891,   211,  -759,  -759,  -759,  -759,
    -759,   894,   981,   369,    44,    44,    44,   -46,  -759,   893,
     197,  -759,    44,  1051,  -759,  -759,  -759,  -759,   897,   899,
    -759,  -759,   901,  1055,  -759,   189,   860,  -759,  -759,   902,
    -759,  1059,  -759,  -759,  -759,   903,  -759,  -759,  -759,  1060,
    -759,  -759,  -759,  -759,  -759,   845,  -759,  -759,  1030,  -759,
     230,   905,   413,   986,   415,  1066,   908,   909,   958,   997,
     910,   415,  -759,   665,   914,   -46,   913,   919,   916,  -759,
    -759,  -759,  -759,  -759,  -759,   917,  -759,  -759,  -759,   918,
    -759,  -759,   665,  -759,    43,  -759,  -759,  -759,   921,  -759,
     415,  1005,  -759,   928,   -19,  -759,  -759,    99,   930,  -759,
    -759,   853,   -19,   920,  -759,  -759,  -759
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -759,  -759,  1079,  -759,  -759,  -759,  -759,  -759,  -759,   134,
    -759,  -759,   872,   -93,  -329,   718,   896,  1022,  -438,  -759,
    -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,
    -759,  -759,  -759,  -759,  -759,  1029,  -759,  -759,  -759,   249,
    -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,
    -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,   659,
     906,  -759,  -759,  -759,  -759,   599,  -759,  -759,  -759,   267,
    -759,  -759,  -759,  -567,   257,  -759,   354,   226,  -759,  -759,
    -759,  -759,  -759,   185,  -759,  -759,   944,  -759,  -759,  -759,
    -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,
    -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,
    -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,  -759,
    -759,  -759,  -759,  -759,  -759,   443,  -718,   206,  -759,  -758,
    -759,  -420,  -543,  -759,  -759,  -759,   385,   727,  -187,  -155,
    -354,   619,   -95,  -353,  -425,  -569,  -458,  -564,   632,  -555,
    -150,   -60,  -759,   420,  -759,  -759,   647,  -759,  -759,   837,
    -158,   612,  -759,  -434,  -759,  -759,  -759,  -759,  -759,  -157,
    -759
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -210
static const yytype_int16 yytable[] =
{
     212,   239,   556,   222,   445,   669,   284,   267,   268,   209,
     337,   471,   472,   292,   293,   580,   295,   298,   600,   308,
     309,   310,   571,   572,   568,   317,   318,   316,   320,   321,
     319,   327,   701,   329,   483,   699,   334,   204,   516,   839,
     322,   261,   697,   252,   323,   324,   545,   340,   204,   230,
     231,   204,   602,   564,   357,   342,   358,   296,   366,   296,
     359,   360,   361,   368,   500,   833,   578,   503,   241,   252,
       7,   290,   303,    16,   230,   231,   227,   744,   301,   302,
     723,   882,   230,   231,   322,   755,   285,   218,   323,   324,
     894,   466,   558,   330,   559,   342,   589,   204,   219,   220,
     335,   230,   231,   590,   949,   591,   593,   529,   253,   286,
     287,   530,   592,   594,   760,   264,   404,   405,   367,   407,
     265,   315,   895,   595,   737,     1,     2,   241,   677,   834,
     596,   738,   234,   242,   253,   516,     8,   769,   252,   362,
     546,   262,     9,   235,   770,   343,   243,   565,   325,   516,
     928,   353,   451,   369,   228,    11,   331,   234,   908,   775,
      13,   230,   231,   204,   236,   237,   218,   950,   297,    14,
     307,   745,   304,   771,   402,   403,   235,   219,   220,   756,
     772,   773,   363,   741,   234,   817,   704,   150,   774,   236,
     237,   852,   242,   484,   662,   364,   151,   570,   313,   365,
      15,   218,   254,   253,   849,   243,   205,   848,   255,   205,
     206,   207,   219,   220,   847,   473,   236,   237,   286,   287,
     153,   206,   207,   720,   206,   207,   489,   244,   254,   256,
     257,    69,   154,   245,   255,   322,   692,   693,   155,   323,
     324,   506,   504,   505,   684,   156,   208,   553,   513,   205,
     157,   580,   523,   514,   515,   364,   526,   208,   762,   787,
     208,   218,   492,   493,   206,   207,   539,   511,   758,   544,
     206,   207,   219,   220,   550,   551,   552,   574,   450,   158,
     733,   479,   557,   704,   456,   457,   244,   458,   459,   460,
     461,   462,   245,   534,   570,   535,   536,   254,   601,   159,
     208,   599,   579,   255,   343,   474,   208,   587,   570,   614,
     349,   350,   351,   615,   616,   246,   247,   480,   836,   519,
     520,   205,   764,   765,   256,   257,   491,   218,   842,   813,
     843,   498,   814,   519,   520,   502,   206,   207,   219,   220,
     223,   224,   225,   226,   637,   232,   233,   230,   231,   160,
     197,   232,   233,   761,   645,   646,   528,   898,   516,   161,
     899,   203,   517,   162,   437,   211,   163,   871,   617,   618,
     619,   241,   208,   230,   231,   164,   347,   348,   349,   350,
     351,   165,   575,   347,   348,   349,   350,   351,   166,   167,
     917,   943,   213,   -14,   620,   621,   622,   623,   624,   625,
     168,   626,   169,   353,   216,   170,   455,   171,   885,   172,
     345,   347,   348,   349,   350,   351,   627,   628,   347,   348,
     349,   350,   351,   230,   231,   629,   630,   631,   632,   633,
     234,   173,   174,   346,   291,   175,   242,   176,   177,   846,
     217,   235,   634,   178,   635,   695,   349,   350,   351,   243,
     864,   865,   866,   179,   653,   385,   234,   347,   348,   349,
     350,   351,   236,   237,    73,   180,    75,   235,    77,   181,
      79,   182,    81,   183,    83,   184,    85,   185,    87,   186,
      89,   385,   347,   348,   349,   350,   351,   749,   236,   237,
     724,   294,   187,   188,   727,   815,   313,   730,   731,   269,
     270,   271,   272,   273,   189,   274,   234,   275,   276,   277,
     278,   279,   190,   191,   192,   584,   686,   518,   208,   230,
     231,   585,   779,   780,   781,   782,   751,   784,   785,   786,
     244,   339,   194,   694,   597,   604,   245,   195,   236,   237,
     196,   202,   519,   520,   214,   230,   231,   338,   281,   347,
     348,   349,   350,   351,   347,   348,   349,   350,   351,   246,
     247,   341,   344,   783,   347,   348,   349,   350,   351,   808,
     347,   348,   349,   350,   351,   923,   347,   862,   349,   350,
     351,   374,   931,   377,   347,   348,   349,   350,   351,   378,
     725,   726,   379,   380,   729,   269,   270,   271,   272,   273,
     610,   274,   234,   275,   276,   277,   278,   279,   381,   382,
     385,   945,   390,   280,   384,   387,   388,   386,   391,   394,
     397,   269,   270,   271,   272,   273,   395,   274,   234,   275,
     276,   277,   278,   279,   236,   237,  -209,   406,   398,   280,
     399,   400,   853,   401,   281,   877,   408,   409,   410,   347,
     348,   349,   350,   351,   412,   415,   414,   416,   417,   418,
     236,   237,   811,   419,   420,   421,   422,   891,   892,   893,
     281,    16,   821,   423,   424,   428,   425,   426,   427,    17,
     613,   430,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,   641,   431,   432,    28,    29,    30,    31,   433,
      32,   434,   435,   436,   438,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,   439,    44,    45,    46,
      47,    48,    49,    50,    51,    52,   440,    53,   441,   347,
     348,   349,   350,   351,   442,   443,   444,   447,   448,   452,
     463,   347,   348,   349,   350,   351,   449,   454,   464,   465,
      54,   475,   476,   482,   477,   860,   486,   659,   487,   488,
      55,   490,   818,   495,   496,   497,    56,   499,    57,    58,
     501,    59,   507,    60,    61,    62,   508,   509,    63,    64,
     510,    65,   524,    66,   511,   230,   231,   525,   527,   948,
     532,   533,   540,   541,   542,   543,   548,   955,   230,   231,
     549,   554,   900,    67,   561,    68,   347,   348,   349,   350,
     351,   347,   348,   349,   350,   351,   562,   563,   567,  -167,
     933,   577,   588,   605,   603,   609,   606,   608,   612,    69,
     649,   640,   643,   644,   647,    70,    71,   658,   648,   942,
     650,   651,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
     652,   269,   270,   271,   272,   273,   654,   274,   234,   275,
     276,   277,   278,   279,   269,   270,   271,   272,   273,   280,
     274,   234,   275,   276,   277,   278,   279,   788,   789,   655,
     662,   665,   280,   656,   657,   660,   661,   663,   664,   666,
     236,   237,   667,   668,   670,   671,   672,   674,   675,   676,
     281,   678,   198,   236,   237,   679,   790,   791,    72,   792,
      74,   680,    76,   281,    78,   681,    80,   682,    82,   653,
      84,   683,    86,   685,    88,   688,   689,   691,   690,   285,
     793,   702,   794,   795,   796,   797,   798,   799,   800,   801,
     802,   803,   804,    71,   703,   704,   705,   706,   707,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,   708,   718,   709,
     710,   721,   511,   735,   711,   218,   712,   713,   714,   715,
     716,   717,   719,   732,   722,   728,   219,   220,   733,   736,
     740,   742,   743,   741,   747,   748,   753,   750,   752,   754,
     763,   776,   777,   809,   778,   812,   822,   816,   819,   820,
     823,   828,   825,   826,   827,   829,   831,   832,   837,   838,
     840,   841,   850,   851,   854,   855,   856,   857,   858,   859,
     861,   863,   869,   872,   873,   874,   875,   884,   878,   890,
     881,   879,   880,   886,   889,   901,   897,   902,   887,   906,
     903,   905,   909,   910,   912,   914,   915,   916,   919,   921,
     925,   926,   927,   930,   934,   929,   936,   937,   941,   938,
     939,   944,   946,   956,    10,   954,   199,   193,   947,   376,
     952,   478,   336,   611,   888,   555,   328,   883,   876,   830,
     935,   907,   251,   734,   807,   481,   607,   598,   920,   767,
     586,   638,     0,     0,     0,   411
};

static const yytype_int16 yycheck[] =
{
      93,   156,   436,   153,   333,   548,   164,   162,   163,    69,
     197,   365,   365,   168,   169,   453,   171,   172,   476,   176,
     177,   178,   447,   448,   444,   183,   184,   182,   186,   187,
     185,   189,   601,   191,     3,   599,   194,     4,     3,   757,
       4,    68,   597,     3,     8,     9,    68,   202,     4,    68,
      69,     4,   477,    68,    21,     8,    23,    51,   213,    51,
      27,    28,    29,   213,   393,     4,    68,   396,     3,     3,
     161,   166,    68,     6,    68,    69,     4,    68,   173,   174,
     647,   839,    68,    69,     4,    68,   162,   168,     8,     9,
     136,     4,    66,     5,    68,     8,   160,     4,   179,   180,
     195,    68,    69,   167,     5,   160,   160,    64,    68,   185,
     186,    68,   167,   167,   681,    63,   271,   272,   213,   274,
      68,   181,   168,   160,    68,    15,    16,     3,   562,    68,
     167,    75,   151,    68,    68,     3,   161,   160,     3,   106,
     162,   168,     0,   162,   167,   205,    81,   162,    68,     3,
     908,   211,   339,   213,    82,    97,    68,   151,   876,   702,
     168,    68,    69,     4,   183,   184,   168,    68,   162,   168,
     162,   162,   168,   160,   269,   270,   162,   179,   180,   162,
     167,   160,   149,   160,   151,   728,   163,   160,   167,   183,
     184,   161,    68,   162,   164,   162,    98,   162,   162,   166,
     161,   168,   162,    68,   773,    81,   162,   771,   168,   162,
     177,   178,   179,   180,   769,   365,   183,   184,   185,   186,
     161,   177,   178,   643,   177,   178,   384,   162,   162,   189,
     190,   164,   161,   168,   168,     4,   589,   591,   161,     8,
       9,   399,   397,   398,   573,   161,   213,   434,   406,   162,
     161,   689,   410,   408,   409,   162,   414,   213,   683,   717,
     213,   168,    73,    74,   177,   178,   421,    67,    68,   426,
     177,   178,   179,   180,   431,   432,   433,   115,   338,   161,
     160,   374,   437,   163,   344,   345,   162,   347,   348,   349,
     350,   351,   168,    68,   162,    70,    71,   162,   166,   161,
     213,   166,   452,   168,   364,   365,   213,   465,   162,     4,
     216,   217,   218,     8,     9,   191,   192,   377,   752,   187,
     188,   162,   173,   174,   189,   190,   386,   168,   117,   160,
     119,   391,   163,   187,   188,   395,   177,   178,   179,   180,
      10,    11,    12,    13,   499,    72,    73,    68,    69,   161,
     164,    72,    73,   682,   509,   510,   416,   160,     3,   161,
     163,   170,     7,   161,   160,   162,   161,   825,    63,    64,
      65,     3,   213,    68,    69,   161,   214,   215,   216,   217,
     218,   161,   220,   214,   215,   216,   217,   218,   161,   161,
     160,   934,   219,   163,    89,    90,    91,    92,    93,    94,
     161,    96,   161,   463,     3,   161,   163,   161,   842,   161,
     142,   214,   215,   216,   217,   218,   111,   112,   214,   215,
     216,   217,   218,    68,    69,   120,   121,   122,   123,   124,
     151,   161,   161,   165,   168,   161,    68,   161,   161,   768,
     160,   162,   137,   161,   139,   595,   216,   217,   218,    81,
      37,    38,    39,   161,     3,     4,   151,   214,   215,   216,
     217,   218,   183,   184,   178,   161,   180,   162,   182,   161,
     184,   161,   186,   161,   188,   161,   190,   161,   192,   161,
     194,     4,   214,   215,   216,   217,   218,   674,   183,   184,
     648,    79,   161,   161,   652,   160,   162,   655,   656,   144,
     145,   146,   147,   148,   161,   150,   151,   152,   153,   154,
     155,   156,   161,   161,   161,   165,   576,   162,   213,    68,
      69,   165,   709,   710,   711,   712,   676,   714,   715,   716,
     162,   164,   161,   593,   166,   165,   168,   161,   183,   184,
     161,   161,   187,   188,   161,    68,    69,   219,   193,   214,
     215,   216,   217,   218,   214,   215,   216,   217,   218,   191,
     192,   162,   164,   713,   214,   215,   216,   217,   218,   719,
     214,   215,   216,   217,   218,   904,   214,   215,   216,   217,
     218,   166,   911,   164,   214,   215,   216,   217,   218,   220,
     650,   651,   160,   160,   654,   144,   145,   146,   147,   148,
     165,   150,   151,   152,   153,   154,   155,   156,   160,   159,
       4,   940,     3,   162,   162,   160,   160,   164,   164,     3,
     160,   144,   145,   146,   147,   148,   164,   150,   151,   152,
     153,   154,   155,   156,   183,   184,   166,   162,   160,   162,
     160,   160,   792,   160,   193,   832,   162,   162,   162,   214,
     215,   216,   217,   218,   162,     4,   222,   164,   160,   160,
     183,   184,   722,   160,   160,   160,   160,   854,   855,   856,
     193,     6,   732,   162,     4,     6,   160,   160,   160,    14,
     165,     4,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,   165,   160,   160,    30,    31,    32,    33,   160,
      35,   160,     4,   160,   160,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,   160,    52,    53,    54,
      55,    56,    57,    58,    59,    60,   160,    62,   160,   214,
     215,   216,   217,   218,   160,   160,   160,   160,   160,   160,
     160,   214,   215,   216,   217,   218,   165,   163,   163,   161,
      85,   161,   161,     3,   161,   815,     3,   165,   160,   165,
      95,   160,   165,     5,   169,   160,   101,   160,   103,   104,
     160,   106,    67,   108,   109,   110,    67,   160,   113,   114,
     160,   116,   157,   118,    67,    68,    69,   163,   160,   944,
      34,    36,    76,     4,   160,    76,   160,   952,    68,    69,
     160,   160,   862,   138,    80,   140,   214,   215,   216,   217,
     218,   214,   215,   216,   217,   218,   162,    86,     4,   105,
     913,   165,   160,     3,   167,     4,   160,   163,   168,   164,
     163,   168,   160,   160,   221,   170,   171,     4,   160,   932,
     160,   160,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     160,   144,   145,   146,   147,   148,   164,   150,   151,   152,
     153,   154,   155,   156,   144,   145,   146,   147,   148,   162,
     150,   151,   152,   153,   154,   155,   156,    70,    71,   160,
     164,     4,   162,   160,   160,   160,   160,   160,   160,   160,
     183,   184,     4,   160,     4,   160,   160,   160,    67,   160,
     193,   160,   171,   183,   184,     4,    99,   100,   177,   102,
     179,   160,   181,   193,   183,   160,   185,   105,   187,     3,
     189,   160,   191,   220,   193,   161,   160,     4,   163,   162,
     123,   160,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   171,     3,   163,   169,   163,   161,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   161,   163,   161,
     161,     5,    67,   137,   161,   168,   161,   161,   161,   161,
     161,   161,   160,   162,   160,   160,   179,   180,   160,     4,
       4,    76,   160,   160,   168,    76,    83,   160,   160,   160,
     166,   160,    88,   160,    87,   163,     4,   163,   163,   163,
     160,     4,   161,   165,   160,     4,   160,   160,     4,     4,
     160,   160,   163,     5,   161,   161,   161,   161,     3,    37,
     163,   160,    84,     4,   160,   163,   160,     7,   163,    68,
     160,   163,   163,   161,   160,     4,   163,   160,   167,     4,
     161,   160,   160,     4,   161,     5,   221,    37,   163,    83,
       4,   163,   163,   163,   160,    78,   163,   158,   160,   163,
     163,   160,    77,   163,     5,   951,    64,    58,   160,   217,
     160,   373,   196,   494,   845,   436,   190,   840,   831,   745,
     915,   875,   158,   660,   719,   378,   487,   475,   902,   689,
     463,   499,    -1,    -1,    -1,   278
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   224,   225,   226,   227,   161,   161,     0,
     225,    97,   234,   168,   168,   161,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    30,    31,
      32,    33,    35,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    62,    85,    95,   101,   103,   104,   106,
     108,   109,   110,   113,   114,   116,   118,   138,   140,   164,
     170,   171,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     228,   229,   236,   238,   239,   240,   241,   243,   244,   252,
     255,   256,   258,   259,   266,   268,   269,   271,   273,   275,
     279,   280,   281,   284,   286,   293,   298,   301,   307,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   323,
     324,   325,   326,   327,   329,   330,   331,   332,   333,   334,
     335,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     160,    98,   235,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   258,   161,   161,   161,   164,   171,   240,
     242,   260,   161,   170,     4,   162,   177,   178,   213,   374,
     384,   162,   236,   219,   161,   270,     3,   160,   168,   179,
     180,   360,   373,    10,    11,    12,    13,     4,    82,   385,
      68,    69,    72,    73,   151,   162,   183,   184,   299,   362,
     363,     3,    68,    81,   162,   168,   191,   192,   309,   371,
     372,   309,     3,    68,   162,   168,   189,   190,   303,   369,
     370,    68,   168,   347,    63,    68,   354,   362,   362,   144,
     145,   146,   147,   148,   150,   152,   153,   154,   155,   156,
     162,   193,   362,   382,   383,   162,   185,   186,   365,   366,
     365,   168,   362,   362,    79,   362,    51,   162,   362,   392,
     393,   365,   365,    68,   168,   283,   348,   162,   392,   392,
     392,   361,   374,   162,   282,   374,   362,   383,   383,   362,
     383,   383,     4,     8,     9,    68,   386,   383,   283,   383,
       5,    68,   294,   276,   383,   365,   239,   361,   219,   164,
     362,   162,     8,   374,   164,   142,   165,   214,   215,   216,
     217,   218,   373,   374,   379,   380,   381,    21,    23,    27,
      28,    29,   106,   149,   162,   166,   362,   365,   373,   374,
     388,   389,   390,   274,   166,   237,   235,   164,   220,   160,
     160,   160,   159,   364,   162,     4,   164,   160,   160,   288,
       3,   164,   310,   308,     3,   164,   302,   160,   160,   160,
     160,   160,   365,   365,   362,   362,   162,   362,   162,   162,
     162,   382,   162,   383,   222,     4,   164,   160,   160,   160,
     160,   160,   160,   162,     4,   160,   160,   160,     6,   257,
       4,   160,   160,   160,   160,     4,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   237,   277,   160,   160,   165,
     374,   361,   160,   253,   163,   163,   374,   374,   374,   374,
     374,   374,   374,   160,   163,   161,     4,   248,   249,   250,
     251,   363,   366,   373,   374,   161,   161,   161,   238,   236,
     374,   360,     3,     3,   162,   328,     3,   160,   165,   383,
     160,   374,    73,    74,   300,     5,   169,   160,   374,   160,
     237,   160,   374,   237,   362,   362,   383,    67,    67,   160,
     160,    67,   296,   383,   362,   362,     3,     7,   162,   187,
     188,   367,   368,   383,   157,   163,   383,   160,   374,    64,
      68,   355,    34,    36,    68,    70,    71,   356,   359,   362,
      76,     4,   160,    76,   392,    68,   162,   289,   160,   160,
     392,   392,   392,   361,   160,   282,   386,   362,    66,    68,
     391,    80,   162,    86,    68,   162,   290,     4,   354,   278,
     162,   367,   367,   272,   115,   220,   267,   165,    68,   373,
     241,   376,   377,   378,   165,   165,   379,   383,   160,   160,
     167,   160,   167,   160,   167,   160,   167,   166,   371,   166,
     369,   166,   367,   167,   165,     3,   160,   364,   163,     4,
     165,   288,   168,   165,     4,     8,     9,    63,    64,    65,
      89,    90,    91,    92,    93,    94,    96,   111,   112,   120,
     121,   122,   123,   124,   137,   139,   311,   362,   384,   387,
     168,   165,   304,   160,   160,   362,   362,   221,   160,   163,
     160,   160,   160,     3,   164,   160,   160,   160,     4,   165,
     160,   160,   164,   160,   160,     4,   160,     4,   160,   355,
       4,   160,   160,   322,   160,    67,   160,   386,   160,     4,
     160,   160,   105,   160,   237,   220,   374,   261,   161,   160,
     163,     4,   366,   363,   374,   373,   245,   372,   246,   370,
     247,   368,   160,     3,   163,   169,   163,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   163,   160,
     354,     5,   160,   296,   383,   374,   374,   383,   160,   374,
     383,   383,   162,   160,   348,   137,     4,    68,    75,   358,
       4,   160,    76,   160,    68,   162,   291,   168,    76,   361,
     160,   373,   160,    83,   160,    68,   162,   292,    68,   295,
     296,   237,   367,   166,   173,   174,   375,   376,   254,   160,
     167,   160,   167,   160,   167,   355,   160,    88,    87,   361,
     361,   361,   361,   373,   361,   361,   361,   369,    70,    71,
      99,   100,   102,   123,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   305,   357,   359,   373,   160,
     232,   374,   163,   160,   163,   160,   163,   355,   165,   163,
     163,   374,     4,   160,   349,   161,   165,   160,     4,     4,
     299,   160,   160,     4,    68,   297,   386,     4,     4,   349,
     160,   160,   117,   119,   262,   263,   237,   372,   370,   368,
     163,     5,   161,   373,   161,   161,   161,   161,     3,    37,
     374,   163,   215,   160,    37,    38,    39,   350,   351,    84,
     352,   369,     4,   160,   163,   160,   292,   361,   163,   163,
     163,   160,   352,   297,     7,   386,   161,   167,   262,   160,
      68,   361,   361,   361,   136,   168,   306,   163,   160,   163,
     374,     4,   160,   161,   336,   160,     4,   300,   349,   160,
       4,   285,   161,   265,     5,   221,    37,   160,   230,   163,
     350,    83,   353,   237,   337,     4,   163,   163,   352,    78,
     163,   237,   264,   236,   160,   306,   163,   158,   163,   163,
     287,   160,   236,   355,   160,   237,    77,   160,   362,     5,
      68,   233,   160,   231,   232,   362,   163
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

  case 105:

/* Line 1455 of yacc.c  */
#line 567 "lev_comp.y"
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

  case 106:

/* Line 1455 of yacc.c  */
#line 583 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 589 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 595 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 601 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 607 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 613 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 619 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 625 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 631 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 641 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 651 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 661 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 671 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 681 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 691 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 703 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 708 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 715 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 720 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 727 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 732 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 739 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 743 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 749 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 754 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 761 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 765 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 771 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 775 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 781 "lev_comp.y"
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

  case 136:

/* Line 1455 of yacc.c  */
#line 803 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 807 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 818 "lev_comp.y"
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

  case 139:

/* Line 1455 of yacc.c  */
#line 873 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 879 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 883 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 889 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 896 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 900 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 908 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 912 "lev_comp.y"
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

  case 147:

/* Line 1455 of yacc.c  */
#line 935 "lev_comp.y"
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

  case 150:

/* Line 1455 of yacc.c  */
#line 980 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 990 "lev_comp.y"
    {
		  ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 993 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1003 "lev_comp.y"
    {
		  ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1008 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1022 "lev_comp.y"
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

  case 158:

/* Line 1455 of yacc.c  */
#line 1059 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1064 "lev_comp.y"
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

  case 160:

/* Line 1455 of yacc.c  */
#line 1093 "lev_comp.y"
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

  case 161:

/* Line 1455 of yacc.c  */
#line 1107 "lev_comp.y"
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

  case 162:

/* Line 1455 of yacc.c  */
#line 1123 "lev_comp.y"
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

  case 163:

/* Line 1455 of yacc.c  */
#line 1143 "lev_comp.y"
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
#line 1155 "lev_comp.y"
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

  case 165:

/* Line 1455 of yacc.c  */
#line 1175 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1181 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1191 "lev_comp.y"
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

  case 168:

/* Line 1455 of yacc.c  */
#line 1209 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1219 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1225 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1230 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1235 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1242 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1249 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1258 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1266 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1277 "lev_comp.y"
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

  case 178:

/* Line 1455 of yacc.c  */
#line 1288 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1295 "lev_comp.y"
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

  case 180:

/* Line 1455 of yacc.c  */
#line 1306 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1313 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1317 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1323 "lev_comp.y"
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

  case 184:

/* Line 1455 of yacc.c  */
#line 1333 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1339 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1348 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1354 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1359 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1365 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1370 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1376 "lev_comp.y"
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

  case 192:

/* Line 1455 of yacc.c  */
#line 1387 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (5)].i), SPO_DOOR));
		  ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1401 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1405 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1415 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1422 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1430 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1446 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1450 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1456 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1464 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1470 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1478 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1486 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1491 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1497 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1503 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1509 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1515 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1520 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1525 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1530 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1535 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1540 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1545 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1550 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1555 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1560 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1565 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1571 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1578 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1587 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1591 "lev_comp.y"
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

  case 234:

/* Line 1455 of yacc.c  */
#line 1604 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1612 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1622 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1630 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1639 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1646 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1654 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1660 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1665 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1670 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1675 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1680 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1685 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1690 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1695 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1700 "lev_comp.y"
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

  case 250:

/* Line 1455 of yacc.c  */
#line 1711 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1717 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1722 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1727 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1732 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1737 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1742 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1747 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1752 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1757 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1762 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1769 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1775 "lev_comp.y"
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

  case 263:

/* Line 1455 of yacc.c  */
#line 1804 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1809 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1817 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1823 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1829 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1836 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1843 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1853 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1863 "lev_comp.y"
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

  case 272:

/* Line 1455 of yacc.c  */
#line 1878 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1888 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1892 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1898 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1904 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1910 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int)(yyvsp[(6) - (13)].i), (int)(yyvsp[(8) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1916 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1922 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1928 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1933 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1940 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1947 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1953 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1959 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1965 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1969 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1975 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1981 "lev_comp.y"
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

  case 290:

/* Line 1455 of yacc.c  */
#line 1995 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      if ( (yyvsp[(10) - (11)].i) ) {
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      } else if ( (yyvsp[(11) - (11)].i) )
			  lc_error("Cannot use lev statements in non-permanent REGION");
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 2005 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 2009 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 2015 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2022 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2028 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2034 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2040 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2046 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2050 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2055 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2062 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2068 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2075 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2079 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2086 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2097 "lev_comp.y"
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

  case 309:

/* Line 1455 of yacc.c  */
#line 2110 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2114 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2120 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2124 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2131 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2135 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2139 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2145 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2149 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2155 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2171 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2179 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2189 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2198 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2203 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2210 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2221 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2227 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2231 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2235 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2242 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2252 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2259 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2263 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2269 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2273 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2281 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2285 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2292 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2302 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2315 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2319 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2326 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2336 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2345 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2356 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2360 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2367 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2377 "lev_comp.y"
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

  case 358:

/* Line 1455 of yacc.c  */
#line 2388 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2397 "lev_comp.y"
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

  case 360:

/* Line 1455 of yacc.c  */
#line 2407 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2413 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2417 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2424 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2434 "lev_comp.y"
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

  case 365:

/* Line 1455 of yacc.c  */
#line 2445 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2454 "lev_comp.y"
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

  case 367:

/* Line 1455 of yacc.c  */
#line 2464 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2468 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2474 "lev_comp.y"
    { ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2476 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2482 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2486 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2490 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2494 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2502 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2511 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2515 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2519 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2523 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2527 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2530 "lev_comp.y"
    { ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2534 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2541 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2550 "lev_comp.y"
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

  case 389:

/* Line 1455 of yacc.c  */
#line 2591 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2595 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2602 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2609 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2619 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2623 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2631 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2635 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2639 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2643 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2647 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2652 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2656 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2660 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2665 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2670 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2675 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2679 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2684 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2689 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2693 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2697 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2702 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2706 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2713 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2719 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2723 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2729 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2745 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2749 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2753 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2757 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2779 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2783 "lev_comp.y"
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

  case 436:

/* Line 1455 of yacc.c  */
#line 2809 "lev_comp.y"
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
#line 6351 "lev_comp.tab.c"
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
#line 2837 "lev_comp.y"


/*lev_comp.y*/

