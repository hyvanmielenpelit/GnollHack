
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
     SPECIAL_LEVREGION_ID = 289,
     SPECIAL_REGION_TYPE = 290,
     NAMING_ID = 291,
     NAMING_TYPE = 292,
     FILLING = 293,
     IRREGULAR = 294,
     JOINED = 295,
     ALTAR_ID = 296,
     ANVIL_ID = 297,
     NPC_ID = 298,
     LADDER_ID = 299,
     STAIR_ID = 300,
     NON_DIGGABLE_ID = 301,
     NON_PASSWALL_ID = 302,
     ROOM_ID = 303,
     PORTAL_ID = 304,
     TELEPRT_ID = 305,
     BRANCH_ID = 306,
     LEV = 307,
     MINERALIZE_ID = 308,
     CORRIDOR_ID = 309,
     GOLD_ID = 310,
     ENGRAVING_ID = 311,
     FOUNTAIN_ID = 312,
     THRONE_ID = 313,
     MODRON_PORTAL_ID = 314,
     POOL_ID = 315,
     SINK_ID = 316,
     NONE = 317,
     RAND_CORRIDOR_ID = 318,
     DOOR_STATE = 319,
     LIGHT_STATE = 320,
     CURSE_TYPE = 321,
     ENGRAVING_TYPE = 322,
     DIRECTION = 323,
     RANDOM_TYPE = 324,
     RANDOM_TYPE_BRACKET = 325,
     A_REGISTER = 326,
     ALIGNMENT = 327,
     LEFT_OR_RIGHT = 328,
     CENTER = 329,
     TOP_OR_BOT = 330,
     ALTAR_TYPE = 331,
     UP_OR_DOWN = 332,
     ACTIVE_OR_INACTIVE = 333,
     MODRON_PORTAL_TYPE = 334,
     NPC_TYPE = 335,
     FOUNTAIN_TYPE = 336,
     SPECIAL_OBJECT_TYPE = 337,
     CMAP_TYPE = 338,
     FLOOR_TYPE = 339,
     FLOOR_TYPE_ID = 340,
     FLOOR_ID = 341,
     FLOOR_MAIN_TYPE = 342,
     ELEMENTAL_ENCHANTMENT_TYPE = 343,
     EXCEPTIONALITY_TYPE = 344,
     EXCEPTIONALITY_ID = 345,
     ELEMENTAL_ENCHANTMENT_ID = 346,
     ENCHANTMENT_ID = 347,
     CHARGES_ID = 348,
     SPECIAL_QUALITY_ID = 349,
     SPEFLAGS_ID = 350,
     SUBROOM_ID = 351,
     NAME_ID = 352,
     FLAGS_ID = 353,
     FLAG_TYPE = 354,
     MON_ATTITUDE = 355,
     MON_ALERTNESS = 356,
     SUBTYPE_ID = 357,
     MON_APPEARANCE = 358,
     ROOMDOOR_ID = 359,
     IF_ID = 360,
     ELSE_ID = 361,
     TERRAIN_ID = 362,
     HORIZ_OR_VERT = 363,
     REPLACE_TERRAIN_ID = 364,
     EXIT_ID = 365,
     SHUFFLE_ID = 366,
     QUANTITY_ID = 367,
     BURIED_ID = 368,
     LOOP_ID = 369,
     FOR_ID = 370,
     TO_ID = 371,
     SWITCH_ID = 372,
     CASE_ID = 373,
     BREAK_ID = 374,
     DEFAULT_ID = 375,
     ERODED_ID = 376,
     TRAPPED_STATE = 377,
     RECHARGED_ID = 378,
     INVIS_ID = 379,
     GREASED_ID = 380,
     FEMALE_ID = 381,
     WAITFORU_ID = 382,
     CANCELLED_ID = 383,
     REVIVED_ID = 384,
     AVENGE_ID = 385,
     FLEEING_ID = 386,
     BLINDED_ID = 387,
     PARALYZED_ID = 388,
     STUNNED_ID = 389,
     CONFUSED_ID = 390,
     SEENTRAPS_ID = 391,
     ALL_ID = 392,
     MONTYPE_ID = 393,
     GRAVE_ID = 394,
     ERODEPROOF_ID = 395,
     FUNCTION_ID = 396,
     MSG_OUTPUT_TYPE = 397,
     COMPARE_TYPE = 398,
     UNKNOWN_TYPE = 399,
     rect_ID = 400,
     fillrect_ID = 401,
     line_ID = 402,
     randline_ID = 403,
     grow_ID = 404,
     selection_ID = 405,
     flood_ID = 406,
     rndcoord_ID = 407,
     circle_ID = 408,
     ellipse_ID = 409,
     filter_ID = 410,
     complement_ID = 411,
     gradient_ID = 412,
     GRADIENT_TYPE = 413,
     LIMITED = 414,
     HUMIDITY_TYPE = 415,
     STRING = 416,
     MAP_ID = 417,
     NQSTRING = 418,
     VARSTRING = 419,
     CFUNC = 420,
     CFUNC_INT = 421,
     CFUNC_STR = 422,
     CFUNC_COORD = 423,
     CFUNC_REGION = 424,
     VARSTRING_INT = 425,
     VARSTRING_INT_ARRAY = 426,
     VARSTRING_STRING = 427,
     VARSTRING_STRING_ARRAY = 428,
     VARSTRING_VAR = 429,
     VARSTRING_VAR_ARRAY = 430,
     VARSTRING_COORD = 431,
     VARSTRING_COORD_ARRAY = 432,
     VARSTRING_REGION = 433,
     VARSTRING_REGION_ARRAY = 434,
     VARSTRING_MAPCHAR = 435,
     VARSTRING_MAPCHAR_ARRAY = 436,
     VARSTRING_MONST = 437,
     VARSTRING_MONST_ARRAY = 438,
     VARSTRING_OBJ = 439,
     VARSTRING_OBJ_ARRAY = 440,
     VARSTRING_SEL = 441,
     VARSTRING_SEL_ARRAY = 442,
     METHOD_INT = 443,
     METHOD_INT_ARRAY = 444,
     METHOD_STRING = 445,
     METHOD_STRING_ARRAY = 446,
     METHOD_VAR = 447,
     METHOD_VAR_ARRAY = 448,
     METHOD_COORD = 449,
     METHOD_COORD_ARRAY = 450,
     METHOD_REGION = 451,
     METHOD_REGION_ARRAY = 452,
     METHOD_MAPCHAR = 453,
     METHOD_MAPCHAR_ARRAY = 454,
     METHOD_MONST = 455,
     METHOD_MONST_ARRAY = 456,
     METHOD_OBJ = 457,
     METHOD_OBJ_ARRAY = 458,
     METHOD_SEL = 459,
     METHOD_SEL_ARRAY = 460,
     DICE = 461
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
#line 504 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 516 "lev_comp.tab.c"

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
#define YYLAST   1118

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  224
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  172
/* YYNRULES -- Number of rules.  */
#define YYNRULES  438
/* YYNRULES -- Number of states.  */
#define YYNSTATES  963

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   461

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   219,   223,     2,
     163,   164,   217,   215,   161,   216,   221,   218,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   162,     2,
       2,   220,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   165,     2,   166,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   167,   222,   168,     2,     2,     2,     2,
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
     155,   156,   157,   158,   159,   160,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214
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
     239,   241,   243,   245,   247,   249,   251,   255,   259,   265,
     269,   275,   281,   287,   291,   295,   301,   307,   313,   321,
     329,   337,   343,   345,   349,   351,   355,   357,   361,   363,
     367,   369,   373,   375,   379,   381,   385,   386,   387,   396,
     401,   403,   404,   406,   408,   414,   418,   419,   420,   430,
     431,   434,   435,   441,   442,   447,   449,   452,   454,   461,
     462,   466,   467,   474,   475,   480,   481,   486,   488,   489,
     494,   498,   500,   504,   508,   514,   520,   528,   533,   534,
     546,   547,   561,   562,   565,   571,   573,   579,   581,   587,
     589,   595,   597,   607,   613,   615,   617,   619,   621,   623,
     627,   629,   631,   633,   641,   647,   649,   651,   653,   655,
     659,   660,   666,   671,   672,   676,   678,   680,   682,   684,
     687,   689,   691,   693,   695,   697,   701,   705,   709,   711,
     713,   717,   719,   721,   723,   727,   731,   732,   738,   741,
     742,   746,   748,   752,   754,   758,   762,   764,   766,   770,
     772,   774,   776,   780,   782,   784,   786,   790,   794,   798,
     802,   806,   810,   816,   824,   830,   839,   841,   845,   851,
     857,   865,   873,   880,   886,   887,   890,   896,   900,   914,
     918,   922,   924,   930,   940,   946,   950,   954,   960,   970,
     976,   982,   983,   995,   996,   998,  1006,  1010,  1018,  1024,
    1030,  1036,  1042,  1046,  1052,  1060,  1070,  1072,  1074,  1076,
    1078,  1080,  1081,  1084,  1086,  1090,  1092,  1094,  1096,  1097,
    1101,  1103,  1105,  1107,  1109,  1111,  1113,  1115,  1117,  1119,
    1121,  1125,  1127,  1129,  1134,  1136,  1138,  1143,  1145,  1147,
    1152,  1154,  1159,  1165,  1167,  1171,  1173,  1177,  1179,  1181,
    1186,  1196,  1198,  1200,  1205,  1207,  1213,  1215,  1217,  1222,
    1224,  1226,  1232,  1234,  1236,  1238,  1243,  1245,  1247,  1253,
    1255,  1257,  1259,  1263,  1265,  1267,  1271,  1273,  1278,  1282,
    1286,  1290,  1294,  1298,  1302,  1304,  1306,  1310,  1312,  1316,
    1317,  1319,  1321,  1323,  1325,  1329,  1330,  1332,  1334,  1337,
    1340,  1345,  1352,  1357,  1364,  1371,  1378,  1385,  1388,  1395,
    1404,  1413,  1424,  1439,  1442,  1444,  1448,  1450,  1454,  1456,
    1458,  1460,  1462,  1464,  1466,  1468,  1470,  1472,  1474,  1476,
    1478,  1480,  1482,  1484,  1486,  1488,  1490,  1492,  1503
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     225,     0,    -1,    -1,   226,    -1,   227,    -1,   227,   226,
      -1,   228,   235,   237,    -1,    16,   162,   169,    -1,    15,
     162,   169,   161,     3,    -1,    17,   162,    11,   161,   329,
      -1,    17,   162,    10,   161,     3,    -1,    17,   162,    13,
      -1,    17,   162,    12,   161,     3,   161,     3,   161,     5,
     161,     5,   161,   357,   161,   234,   233,    -1,    18,   162,
     387,    -1,    -1,   161,   159,    -1,    -1,   161,   364,    -1,
      -1,   161,     3,    -1,     5,    -1,    69,    -1,    -1,    98,
     162,   236,    -1,    99,   161,   236,    -1,    99,    -1,    -1,
     239,   237,    -1,   167,   237,   168,    -1,   280,    -1,   229,
      -1,   230,    -1,   343,    -1,   342,    -1,   340,    -1,   341,
      -1,   344,    -1,   345,    -1,   322,    -1,   282,    -1,   245,
      -1,   244,    -1,   332,    -1,   294,    -1,   314,    -1,   347,
      -1,   348,    -1,   324,    -1,   325,    -1,   326,    -1,   346,
      -1,   260,    -1,   270,    -1,   272,    -1,   276,    -1,   274,
      -1,   257,    -1,   267,    -1,   253,    -1,   256,    -1,   317,
      -1,   299,    -1,   315,    -1,   302,    -1,   308,    -1,   333,
      -1,   328,    -1,   320,    -1,   281,    -1,   334,    -1,   335,
      -1,   336,    -1,   337,    -1,   287,    -1,   285,    -1,   327,
      -1,   331,    -1,   330,    -1,   318,    -1,   319,    -1,   321,
      -1,   313,    -1,   316,    -1,   179,    -1,   181,    -1,   183,
      -1,   185,    -1,   187,    -1,   189,    -1,   191,    -1,   193,
      -1,   195,    -1,   178,    -1,   180,    -1,   182,    -1,   184,
      -1,   186,    -1,   188,    -1,   190,    -1,   192,    -1,   194,
      -1,   240,    -1,   241,    -1,   172,    -1,   172,    -1,   241,
      -1,   111,   162,   240,    -1,   242,   220,   376,    -1,   242,
     220,   150,   162,   385,    -1,   242,   220,   375,    -1,   242,
     220,   392,   162,   369,    -1,   242,   220,   391,   162,   371,
      -1,   242,   220,   390,   162,   373,    -1,   242,   220,   364,
      -1,   242,   220,   367,    -1,   242,   220,   167,   251,   168,
      -1,   242,   220,   167,   250,   168,    -1,   242,   220,   167,
     249,   168,    -1,   242,   220,   392,   162,   167,   248,   168,
      -1,   242,   220,   391,   162,   167,   247,   168,    -1,   242,
     220,   390,   162,   167,   246,   168,    -1,   242,   220,   167,
     252,   168,    -1,   374,    -1,   246,   161,   374,    -1,   372,
      -1,   247,   161,   372,    -1,   370,    -1,   248,   161,   370,
      -1,   368,    -1,   249,   161,   368,    -1,   365,    -1,   250,
     161,   365,    -1,   376,    -1,   251,   161,   376,    -1,   375,
      -1,   252,   161,   375,    -1,    -1,    -1,   141,   171,   163,
     254,   380,   164,   255,   238,    -1,   171,   163,   383,   164,
      -1,   110,    -1,    -1,     6,    -1,     6,    -1,   165,   376,
     143,   376,   166,    -1,   165,   376,   166,    -1,    -1,    -1,
     117,   261,   165,   363,   166,   262,   167,   263,   168,    -1,
      -1,   264,   263,    -1,    -1,   118,   388,   162,   265,   237,
      -1,    -1,   120,   162,   266,   237,    -1,   119,    -1,   221,
     221,    -1,   116,    -1,   115,   243,   220,   376,   268,   376,
      -1,    -1,   269,   271,   238,    -1,    -1,   114,   165,   363,
     166,   273,   238,    -1,    -1,   259,   162,   275,   239,    -1,
      -1,   105,   259,   277,   278,    -1,   238,    -1,    -1,   238,
     279,   106,   238,    -1,    14,   162,   375,    -1,    63,    -1,
      63,   162,   388,    -1,    63,   162,    69,    -1,    54,   162,
     283,   161,   283,    -1,    54,   162,   283,   161,   388,    -1,
     163,     4,   161,    68,   161,   298,   164,    -1,   350,   258,
     161,   357,    -1,    -1,    96,   162,   284,   161,   291,   161,
     293,   351,   354,   286,   238,    -1,    -1,    48,   162,   284,
     161,   290,   161,   292,   161,   293,   351,   354,   288,   238,
      -1,    -1,   161,     5,    -1,   163,     4,   161,     4,   164,
      -1,    69,    -1,   163,     4,   161,     4,   164,    -1,    69,
      -1,   163,   300,   161,   301,   164,    -1,    69,    -1,   163,
       4,   161,     4,   164,    -1,    69,    -1,   104,   162,   295,
     161,   356,   161,   296,   161,   298,    -1,    25,   162,   356,
     161,   385,    -1,     5,    -1,    69,    -1,   297,    -1,    69,
      -1,    68,    -1,    68,   222,   297,    -1,     4,    -1,    69,
      -1,    20,    -1,    19,   162,   300,   161,   301,   289,   170,
      -1,    19,   162,   364,   289,   170,    -1,    73,    -1,    74,
      -1,    75,    -1,    74,    -1,    23,   162,   304,    -1,    -1,
      23,   162,   304,   303,   238,    -1,   371,   161,   364,   305,
      -1,    -1,   305,   161,   306,    -1,   375,    -1,   100,    -1,
     101,    -1,   359,    -1,   103,   375,    -1,   126,    -1,   124,
      -1,   128,    -1,   129,    -1,   130,    -1,   131,   162,   363,
      -1,   132,   162,   363,    -1,   133,   162,   363,    -1,   134,
      -1,   135,    -1,   136,   162,   307,    -1,   127,    -1,   169,
      -1,   137,    -1,   169,   222,   307,    -1,    21,   162,   310,
      -1,    -1,    22,   162,   310,   309,   238,    -1,   373,   311,
      -1,    -1,   311,   161,   312,    -1,    66,    -1,   138,   162,
     371,    -1,   389,    -1,    97,   162,   375,    -1,   112,   162,
     363,    -1,   113,    -1,    65,    -1,   121,   162,   363,    -1,
     140,    -1,    64,    -1,   122,    -1,   123,   162,   363,    -1,
     124,    -1,   125,    -1,   364,    -1,    91,   162,    88,    -1,
      90,   162,    89,    -1,    92,   162,   363,    -1,    93,   162,
     363,    -1,    94,   162,   363,    -1,    95,   162,   363,    -1,
      24,   162,   349,   161,   364,    -1,    26,   162,   364,   161,
      68,   161,   356,    -1,    30,   162,   364,   161,    68,    -1,
      30,   162,   364,   161,    68,   161,     5,   233,    -1,    31,
      -1,    31,   162,   385,    -1,    44,   162,   364,   161,    77,
      -1,    45,   162,   364,   161,    77,    -1,    45,   162,   394,
     161,   394,   161,    77,    -1,    49,   162,   394,   161,   394,
     161,   169,    -1,    50,   162,   394,   161,   394,   323,    -1,
      51,   162,   394,   161,   394,    -1,    -1,   161,    77,    -1,
      57,   162,   385,   161,    81,    -1,    58,   162,   385,    -1,
      59,   162,   364,   161,   163,   388,   161,   388,   164,   161,
      79,   161,    78,    -1,    61,   162,   385,    -1,    60,   162,
     385,    -1,     3,    -1,   163,     3,   161,   357,   164,    -1,
     109,   162,   367,   161,   369,   161,   369,   161,     7,    -1,
     107,   162,   385,   161,   369,    -1,    46,   162,   367,    -1,
      47,   162,   367,    -1,    36,   162,   169,   161,    37,    -1,
      36,   162,   169,   161,    37,   161,   138,   162,   371,    -1,
      33,   162,   367,   161,    35,    -1,    34,   162,   394,   161,
      35,    -1,    -1,    32,   162,   367,   161,   357,   161,   350,
     351,   354,   338,   339,    -1,    -1,   238,    -1,    41,   162,
     364,   161,   358,   161,   360,    -1,    42,   162,   364,    -1,
      86,   162,   385,   161,    87,   161,    84,    -1,   102,   162,
     385,   161,     4,    -1,    43,   162,    80,   161,   364,    -1,
     139,   162,   364,   161,   375,    -1,   139,   162,   364,   161,
      69,    -1,   139,   162,   364,    -1,    55,   162,   376,   161,
     364,    -1,    56,   162,   364,   161,   393,   161,   375,    -1,
      53,   162,   363,   161,   363,   161,   363,   161,   363,    -1,
      53,    -1,   169,    -1,    69,    -1,   169,    -1,    69,    -1,
      -1,   161,   352,    -1,   353,    -1,   353,   161,   352,    -1,
      38,    -1,    39,    -1,    40,    -1,    -1,    85,   162,   355,
      -1,    84,    -1,    64,    -1,    69,    -1,    65,    -1,    69,
      -1,    72,    -1,   361,    -1,    69,    -1,    72,    -1,   361,
      -1,    71,   162,    69,    -1,    76,    -1,    69,    -1,    71,
     165,     4,   166,    -1,   169,    -1,   180,    -1,   181,   165,
     376,   166,    -1,   376,    -1,   365,    -1,   152,   163,   385,
     164,    -1,   184,    -1,   185,   165,   376,   166,    -1,   163,
       4,   161,     4,   164,    -1,    69,    -1,    70,   366,   166,
      -1,   160,    -1,   160,   161,   366,    -1,   368,    -1,   186,
      -1,   187,   165,   376,   166,    -1,   163,     4,   161,     4,
     161,     4,   161,     4,   164,    -1,   370,    -1,   188,    -1,
     189,   165,   376,   166,    -1,     3,    -1,   163,     3,   161,
     357,   164,    -1,   372,    -1,   190,    -1,   191,   165,   376,
     166,    -1,   169,    -1,     3,    -1,   163,     3,   161,   169,
     164,    -1,    69,    -1,   374,    -1,   192,    -1,   193,   165,
     376,   166,    -1,   169,    -1,     3,    -1,   163,     3,   161,
     169,   164,    -1,    69,    -1,    82,    -1,   362,    -1,   375,
     221,   362,    -1,     4,    -1,   386,    -1,   163,     8,   164,
      -1,   178,    -1,   179,   165,   376,   166,    -1,   376,   215,
     376,    -1,   376,   216,   376,    -1,   376,   217,   376,    -1,
     376,   218,   376,    -1,   376,   219,   376,    -1,   163,   376,
     164,    -1,   174,    -1,   175,    -1,   242,   162,   377,    -1,
     378,    -1,   379,   161,   378,    -1,    -1,   379,    -1,   376,
      -1,   375,    -1,   381,    -1,   382,   161,   381,    -1,    -1,
     382,    -1,   364,    -1,   145,   367,    -1,   146,   367,    -1,
     147,   364,   161,   364,    -1,   148,   364,   161,   364,   161,
     376,    -1,   149,   163,   385,   164,    -1,   149,   163,   297,
     161,   385,   164,    -1,   155,   163,     7,   161,   385,   164,
      -1,   155,   163,   385,   161,   385,   164,    -1,   155,   163,
     369,   161,   385,   164,    -1,   151,   364,    -1,   153,   163,
     364,   161,   376,   164,    -1,   153,   163,   364,   161,   376,
     161,    38,   164,    -1,   154,   163,   364,   161,   376,   161,
     376,   164,    -1,   154,   163,   364,   161,   376,   161,   376,
     161,    38,   164,    -1,   157,   163,   158,   161,   163,   376,
     216,   376,   231,   164,   161,   364,   232,   164,    -1,   156,
     384,    -1,   194,    -1,   163,   385,   164,    -1,   384,    -1,
     384,   223,   385,    -1,   214,    -1,    83,    -1,     4,    -1,
       8,    -1,     9,    -1,     4,    -1,     8,    -1,     9,    -1,
       4,    -1,   386,    -1,    27,    -1,    21,    -1,    28,    -1,
      23,    -1,    29,    -1,   107,    -1,    67,    -1,    69,    -1,
     395,    -1,    52,   163,     4,   161,     4,   161,     4,   161,
       4,   164,    -1,   163,     4,   161,     4,   161,     4,   161,
       4,   164,    -1
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
     531,   532,   533,   536,   537,   538,   539,   540,   541,   542,
     543,   544,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   558,   559,   560,   563,   564,   567,   583,   589,   595,
     601,   607,   613,   619,   625,   631,   641,   651,   661,   671,
     681,   691,   703,   708,   715,   720,   727,   732,   739,   743,
     749,   754,   761,   765,   771,   775,   782,   804,   781,   818,
     873,   880,   883,   889,   896,   900,   909,   913,   908,   976,
     977,   981,   980,   994,   993,  1008,  1018,  1019,  1022,  1060,
    1059,  1094,  1093,  1124,  1123,  1156,  1155,  1181,  1192,  1191,
    1219,  1225,  1230,  1235,  1242,  1249,  1258,  1266,  1278,  1277,
    1296,  1295,  1314,  1317,  1323,  1333,  1339,  1348,  1354,  1359,
    1365,  1370,  1376,  1387,  1393,  1394,  1397,  1398,  1401,  1405,
    1411,  1412,  1415,  1422,  1430,  1438,  1439,  1442,  1443,  1446,
    1451,  1450,  1464,  1471,  1478,  1486,  1491,  1497,  1503,  1509,
    1515,  1520,  1525,  1530,  1535,  1540,  1545,  1550,  1555,  1560,
    1565,  1571,  1578,  1587,  1591,  1604,  1613,  1612,  1630,  1640,
    1646,  1654,  1660,  1665,  1670,  1675,  1680,  1685,  1690,  1695,
    1700,  1711,  1717,  1722,  1727,  1732,  1737,  1742,  1747,  1752,
    1757,  1762,  1769,  1775,  1804,  1809,  1817,  1823,  1829,  1836,
    1843,  1853,  1863,  1878,  1889,  1892,  1898,  1904,  1910,  1916,
    1922,  1928,  1933,  1940,  1947,  1953,  1959,  1965,  1969,  1975,
    1981,  1992,  1991,  2016,  2019,  2025,  2032,  2038,  2044,  2050,
    2056,  2060,  2065,  2072,  2078,  2085,  2089,  2096,  2104,  2107,
    2117,  2121,  2124,  2130,  2134,  2141,  2145,  2149,  2156,  2159,
    2165,  2171,  2172,  2175,  2176,  2179,  2180,  2181,  2187,  2188,
    2189,  2195,  2196,  2199,  2208,  2213,  2220,  2231,  2237,  2241,
    2245,  2252,  2262,  2269,  2273,  2279,  2283,  2291,  2295,  2302,
    2312,  2325,  2329,  2336,  2346,  2355,  2366,  2370,  2377,  2387,
    2398,  2407,  2417,  2423,  2427,  2434,  2444,  2455,  2464,  2474,
    2478,  2485,  2486,  2492,  2496,  2500,  2504,  2512,  2521,  2525,
    2529,  2533,  2537,  2541,  2544,  2551,  2560,  2593,  2594,  2597,
    2598,  2601,  2605,  2612,  2619,  2630,  2633,  2641,  2645,  2649,
    2653,  2657,  2662,  2666,  2670,  2675,  2680,  2685,  2689,  2694,
    2699,  2703,  2707,  2712,  2716,  2723,  2729,  2733,  2739,  2746,
    2747,  2750,  2751,  2752,  2755,  2759,  2763,  2767,  2773,  2774,
    2777,  2778,  2781,  2782,  2785,  2786,  2789,  2793,  2819
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
  "WALLIFY_ID", "REGION_ID", "SPECIAL_REGION_ID", "SPECIAL_LEVREGION_ID",
  "SPECIAL_REGION_TYPE", "NAMING_ID", "NAMING_TYPE", "FILLING",
  "IRREGULAR", "JOINED", "ALTAR_ID", "ANVIL_ID", "NPC_ID", "LADDER_ID",
  "STAIR_ID", "NON_DIGGABLE_ID", "NON_PASSWALL_ID", "ROOM_ID", "PORTAL_ID",
  "TELEPRT_ID", "BRANCH_ID", "LEV", "MINERALIZE_ID", "CORRIDOR_ID",
  "GOLD_ID", "ENGRAVING_ID", "FOUNTAIN_ID", "THRONE_ID",
  "MODRON_PORTAL_ID", "POOL_ID", "SINK_ID", "NONE", "RAND_CORRIDOR_ID",
  "DOOR_STATE", "LIGHT_STATE", "CURSE_TYPE", "ENGRAVING_TYPE", "DIRECTION",
  "RANDOM_TYPE", "RANDOM_TYPE_BRACKET", "A_REGISTER", "ALIGNMENT",
  "LEFT_OR_RIGHT", "CENTER", "TOP_OR_BOT", "ALTAR_TYPE", "UP_OR_DOWN",
  "ACTIVE_OR_INACTIVE", "MODRON_PORTAL_TYPE", "NPC_TYPE", "FOUNTAIN_TYPE",
  "SPECIAL_OBJECT_TYPE", "CMAP_TYPE", "FLOOR_TYPE", "FLOOR_TYPE_ID",
  "FLOOR_ID", "FLOOR_MAIN_TYPE", "ELEMENTAL_ENCHANTMENT_TYPE",
  "EXCEPTIONALITY_TYPE", "EXCEPTIONALITY_ID", "ELEMENTAL_ENCHANTMENT_ID",
  "ENCHANTMENT_ID", "CHARGES_ID", "SPECIAL_QUALITY_ID", "SPEFLAGS_ID",
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
  "diggable_detail", "passwall_detail", "naming_detail",
  "special_region_detail", "special_levregion_detail", "region_detail",
  "@16", "region_detail_end", "altar_detail", "anvil_detail",
  "floor_detail", "subtype_detail", "npc_detail", "grave_detail",
  "gold_detail", "engraving_detail", "mineralize", "trap_name",
  "room_type", "optroomregionflags", "roomregionflags", "roomregionflag",
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
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,    44,    58,    40,    41,    91,    93,   123,   125,   416,
     417,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,    43,    45,    42,    47,    37,
      61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   224,   225,   225,   226,   226,   227,   228,   228,   229,
     229,   229,   229,   230,   231,   231,   232,   232,   233,   233,
     234,   234,   235,   235,   236,   236,   237,   237,   238,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   242,   242,   242,   243,   243,   244,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   246,   246,   247,   247,   248,   248,   249,   249,
     250,   250,   251,   251,   252,   252,   254,   255,   253,   256,
     257,   258,   258,   259,   259,   259,   261,   262,   260,   263,
     263,   265,   264,   266,   264,   267,   268,   268,   269,   271,
     270,   273,   272,   275,   274,   277,   276,   278,   279,   278,
     280,   281,   281,   281,   282,   282,   283,   284,   286,   285,
     288,   287,   289,   289,   290,   290,   291,   291,   292,   292,
     293,   293,   294,   294,   295,   295,   296,   296,   297,   297,
     298,   298,   299,   299,   299,   300,   300,   301,   301,   302,
     303,   302,   304,   305,   305,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   307,   307,   307,   308,   309,   308,   310,   311,
     311,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   313,   314,   315,   315,   316,   316,   317,   318,
     319,   320,   321,   322,   323,   323,   324,   325,   326,   327,
     328,   329,   329,   330,   331,   332,   333,   334,   334,   335,
     336,   338,   337,   339,   339,   340,   341,   342,   343,   344,
     345,   345,   345,   346,   347,   348,   348,   349,   349,   350,
     350,   351,   351,   352,   352,   353,   353,   353,   354,   354,
     355,   356,   356,   357,   357,   358,   358,   358,   359,   359,
     359,   360,   360,   361,   362,   362,   362,   363,   364,   364,
     364,   364,   365,   365,   365,   366,   366,   367,   367,   367,
     368,   369,   369,   369,   370,   370,   371,   371,   371,   372,
     372,   372,   372,   373,   373,   373,   374,   374,   374,   374,
     374,   375,   375,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   377,   377,   378,   379,   379,   380,
     380,   381,   381,   382,   382,   383,   383,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   385,   385,   386,   387,
     387,   388,   388,   388,   389,   389,   389,   389,   390,   390,
     391,   391,   392,   392,   393,   393,   394,   394,   395
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
       1,     1,     1,     1,     1,     1,     3,     3,     5,     3,
       5,     5,     5,     3,     3,     5,     5,     5,     7,     7,
       7,     5,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     0,     0,     8,     4,
       1,     0,     1,     1,     5,     3,     0,     0,     9,     0,
       2,     0,     5,     0,     4,     1,     2,     1,     6,     0,
       3,     0,     6,     0,     4,     0,     4,     1,     0,     4,
       3,     1,     3,     3,     5,     5,     7,     4,     0,    11,
       0,    13,     0,     2,     5,     1,     5,     1,     5,     1,
       5,     1,     9,     5,     1,     1,     1,     1,     1,     3,
       1,     1,     1,     7,     5,     1,     1,     1,     1,     3,
       0,     5,     4,     0,     3,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     3,     3,     3,     1,     1,
       3,     1,     1,     1,     3,     3,     0,     5,     2,     0,
       3,     1,     3,     1,     3,     3,     1,     1,     3,     1,
       1,     1,     3,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     5,     7,     5,     8,     1,     3,     5,     5,
       7,     7,     6,     5,     0,     2,     5,     3,    13,     3,
       3,     1,     5,     9,     5,     3,     3,     5,     9,     5,
       5,     0,    11,     0,     1,     7,     3,     7,     5,     5,
       5,     5,     3,     5,     7,     9,     1,     1,     1,     1,
       1,     0,     2,     1,     3,     1,     1,     1,     0,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     4,     1,     1,     4,     1,     1,     4,
       1,     4,     5,     1,     3,     1,     3,     1,     1,     4,
       9,     1,     1,     4,     1,     5,     1,     1,     4,     1,
       1,     5,     1,     1,     1,     4,     1,     1,     5,     1,
       1,     1,     3,     1,     1,     3,     1,     4,     3,     3,
       3,     3,     3,     3,     1,     1,     3,     1,     3,     0,
       1,     1,     1,     1,     3,     0,     1,     1,     2,     2,
       4,     6,     4,     6,     6,     6,     6,     2,     6,     8,
       8,    10,    14,     2,     1,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,    10,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    22,     0,     0,     1,
       5,     0,    26,     0,     7,     0,   143,     0,     0,     0,
       0,   202,     0,     0,     0,     0,     0,     0,     0,   266,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   306,     0,     0,     0,     0,
       0,     0,     0,     0,   171,     0,     0,     0,     0,     0,
       0,     0,   140,     0,     0,     0,   146,   155,     0,     0,
       0,     0,   103,    92,    83,    93,    84,    94,    85,    95,
      86,    96,    87,    97,    88,    98,    89,    99,    90,   100,
      91,    30,    31,     6,    26,   101,   102,     0,    41,    40,
      58,    59,    56,     0,    51,    57,   159,    52,    53,    55,
      54,    29,    68,    39,    74,    73,    43,    61,    63,    64,
      81,    44,    62,    82,    60,    78,    79,    67,    80,    38,
      47,    48,    49,    75,    66,    77,    76,    42,    65,    69,
      70,    71,    72,    34,    35,    33,    32,    36,    37,    50,
      45,    46,     0,    25,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   165,     0,     0,     0,
       0,   104,   105,     0,     0,     0,     0,   373,     0,   376,
       0,   418,     0,   374,   395,    27,     0,   163,     0,     8,
       0,   334,   335,     0,   371,   170,     0,     0,     0,    11,
     420,   419,    13,   343,     0,   205,   206,     0,     0,   340,
       0,     0,   182,   338,   367,   369,   370,     0,   366,   364,
       0,   235,   239,   363,   236,   360,   362,     0,   359,   357,
       0,   209,     0,   356,   308,   307,     0,   321,   322,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   414,   397,   416,   267,     0,   348,
       0,     0,   347,     0,     0,     0,     0,   436,     0,     0,
     296,     0,     0,     0,     0,     0,   285,   286,   310,   309,
       0,   141,     0,     0,     0,     0,   337,     0,     0,     0,
       0,     0,   277,     0,   280,   279,   423,   421,   422,   173,
     172,     0,     0,     0,   194,   195,     0,     0,     0,     0,
     106,     0,     0,     0,   302,   136,     0,     0,     0,     0,
     145,     0,     0,     0,     0,     0,   392,   391,   393,   396,
       0,   429,   431,   428,   430,   432,   433,     0,     0,     0,
     113,   114,   109,   107,     0,     0,     0,     0,    26,   160,
      24,     0,     0,     0,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   398,   399,     0,     0,
       0,   407,     0,     0,     0,   413,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   142,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     167,   166,     0,     0,   161,     0,     0,     0,   389,   375,
     383,     0,     0,   378,   379,   380,   381,   382,     0,   139,
       0,   373,     0,     0,     0,     0,   130,   128,   134,   132,
       0,     0,     0,   164,     0,     0,   372,    10,   281,     0,
       9,     0,     0,   344,     0,     0,     0,   208,   207,   182,
     183,   204,     0,     0,     0,   237,     0,     0,   211,   213,
     262,   193,     0,   264,     0,     0,   198,     0,     0,     0,
       0,   354,     0,     0,   352,     0,     0,   351,     0,     0,
     415,   417,     0,     0,   323,   324,     0,   289,     0,     0,
     290,   287,   327,     0,   325,     0,   326,   299,   268,     0,
     269,     0,   185,     0,     0,     0,     0,   274,   273,     0,
       0,   174,   175,   303,   434,   435,     0,   276,     0,     0,
     187,     0,     0,   298,     0,     0,     0,   284,     0,     0,
     157,     0,     0,   147,   301,   300,     0,   387,   390,     0,
     377,   144,   394,   108,     0,     0,   117,     0,   116,     0,
     115,     0,   121,     0,   112,     0,   111,     0,   110,    28,
     336,     0,     0,   346,   339,     0,   341,     0,     0,   365,
     426,   424,   425,   250,   247,   241,     0,     0,     0,     0,
       0,     0,     0,     0,   246,     0,   251,     0,   253,   254,
       0,   249,   240,   255,   427,   243,     0,   358,   212,     0,
       0,   400,     0,     0,     0,   402,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   349,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   177,     0,     0,   272,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     162,   156,   158,     0,     0,     0,   137,     0,   129,   131,
     133,   135,     0,   122,     0,   124,     0,   126,     0,     0,
     342,   203,   368,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   361,     0,   263,    18,     0,   199,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     311,     0,     0,     0,     0,   332,   331,   295,   270,     0,
     189,     0,     0,   271,   275,     0,     0,   304,     0,   297,
       0,   191,     0,   311,   197,     0,   196,   169,     0,   149,
     384,   385,   386,   388,     0,     0,   120,     0,   119,     0,
     118,     0,     0,   257,   256,   258,   259,   260,   261,   244,
     245,   248,   252,   242,     0,   328,   216,   217,     0,   221,
     220,   231,   222,   223,   224,     0,     0,     0,   228,   229,
       0,   214,   218,   329,   215,     0,   265,   401,   403,     0,
     408,     0,   404,     0,   353,   406,   405,     0,     0,     0,
     318,     0,     0,     0,   333,     0,     0,     0,     0,   200,
     201,     0,     0,     0,     0,   318,     0,     0,     0,     0,
       0,   149,   138,   123,   125,   127,   282,     0,     0,   219,
       0,     0,     0,     0,    19,     0,     0,   355,     0,     0,
     315,   316,   317,   312,   313,     0,   291,     0,     0,   288,
     184,     0,   311,   305,   176,     0,   186,     0,   178,   192,
     283,     0,   153,   148,   150,     0,   330,   225,   226,   227,
     233,   232,   230,   409,     0,   410,   379,     0,     0,     0,
     293,     0,     0,     0,   318,     0,     0,     0,   151,    26,
       0,     0,     0,     0,     0,   350,   314,   320,   319,   294,
     292,     0,   438,   188,   180,     0,   190,   179,    26,   154,
       0,   234,   411,    15,     0,   437,     0,     0,   152,     0,
       0,   181,   278,     0,    16,    20,    21,    18,     0,     0,
      12,    17,   412
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    91,    92,   924,   959,   816,
     957,    12,   154,    93,   379,    94,    95,    96,    97,   203,
      98,    99,   702,   704,   706,   472,   473,   474,   475,   100,
     458,   774,   101,   102,   435,   103,   104,   204,   693,   850,
     851,   938,   919,   105,   582,   106,   107,   218,   108,   579,
     109,   377,   110,   337,   451,   575,   111,   112,   113,   318,
     310,   114,   917,   115,   946,   393,   554,   572,   752,   763,
     116,   336,   765,   517,   841,   117,   241,   499,   118,   400,
     261,   648,   811,   902,   119,   397,   251,   396,   642,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   679,
     130,   131,   132,   133,   134,   490,   135,   136,   137,   138,
     139,   140,   141,   142,   910,   930,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   266,   311,   830,   873,   874,
     876,   928,   269,   536,   545,   812,   747,   546,   224,   315,
     285,   243,   387,   291,   292,   526,   527,   262,   263,   252,
     253,   356,   316,   772,   587,   588,   589,   358,   359,   360,
     286,   417,   213,   232,   330,   645,   374,   375,   376,   566,
     296,   297
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -783
static const yytype_int16 yypact[] =
{
      97,   -90,   -81,    92,  -783,    97,    94,   -15,     3,  -783,
    -783,   -33,   677,    48,  -783,   115,  -783,    73,    99,   108,
     160,  -783,   173,   177,   188,   200,   202,   204,   205,   209,
     210,   211,   215,   216,   219,   222,   224,   234,   235,   238,
     239,   241,   242,   243,   244,   246,   252,   262,   263,   264,
     266,   267,   268,   277,   278,   279,   286,   289,   290,    38,
     291,   295,  -783,   296,    62,   785,  -783,  -783,   299,    86,
     117,   112,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,
    -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,
    -783,  -783,  -783,  -783,   677,  -783,  -783,   145,  -783,  -783,
    -783,  -783,  -783,   300,  -783,  -783,  -783,  -783,  -783,  -783,
    -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,
    -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,
    -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,
    -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,
    -783,  -783,   377,   227,  -783,   -84,   342,    47,   275,   105,
     105,   113,   -23,    64,   247,   247,   742,   -60,   -60,    -7,
     240,   247,   247,   370,   247,   271,   -60,   -60,   -17,    -7,
      -7,    -7,   117,   308,   117,   247,   742,   742,   247,   742,
     742,    65,   742,   -17,   742,    56,  -783,   742,   -60,   779,
     117,  -783,  -783,   259,   298,   247,   317,  -783,    58,  -783,
     316,  -783,   203,  -783,    53,  -783,   347,  -783,   318,  -783,
     115,  -783,  -783,   322,  -783,   265,   327,   330,   331,  -783,
    -783,  -783,  -783,  -783,   323,  -783,  -783,   332,   489,  -783,
     338,   335,   343,  -783,  -783,  -783,  -783,   504,  -783,  -783,
     344,  -783,  -783,  -783,  -783,  -783,  -783,   505,  -783,  -783,
     346,   345,   352,  -783,  -783,  -783,   368,  -783,  -783,   374,
     375,   376,   -60,   -60,   247,   247,   382,   247,   391,   394,
     396,   742,   397,   496,  -783,  -783,   341,  -783,   535,  -783,
     390,   414,  -783,   415,   419,   582,   426,  -783,   427,   429,
    -783,   432,   436,   594,   445,   451,  -783,  -783,  -783,  -783,
     452,   610,   472,   473,   500,   507,   353,   661,   508,   420,
     509,   512,  -783,   513,  -783,  -783,  -783,  -783,  -783,  -783,
    -783,   514,   515,   517,  -783,  -783,   518,   318,   521,   523,
    -783,   519,   117,   117,   525,  -783,   502,   228,   117,   117,
    -783,   117,   117,   117,   117,   117,   265,   353,  -783,   527,
     529,  -783,  -783,  -783,  -783,  -783,  -783,   543,    85,    32,
    -783,  -783,   265,   353,   544,   550,   552,   677,   677,  -783,
    -783,   117,   -84,   684,    36,   686,   555,   551,   742,   568,
     117,   226,   734,   576,   586,   117,   587,   318,   588,   117,
     318,   247,   247,   742,   682,   683,  -783,  -783,   591,   597,
     729,  -783,   247,   247,   395,  -783,   601,   598,   742,   600,
     117,    79,   736,   765,   613,   737,   738,   261,   247,   699,
     619,   701,    -7,   -22,  -783,   624,    -7,    -7,    -7,   117,
     628,    75,   247,   121,   709,   632,   715,     1,   796,    64,
     697,  -783,    77,    77,  -783,   -40,   638,   -19,   761,  -783,
    -783,   526,   538,    95,    95,  -783,  -783,  -783,    53,  -783,
     742,   644,   -86,   -74,   -63,    12,  -783,  -783,   265,   353,
     220,    35,   148,  -783,   640,   549,  -783,  -783,  -783,   803,
    -783,   648,   323,  -783,   646,   809,   604,  -783,  -783,   343,
    -783,  -783,   645,   611,    45,  -783,   655,   617,  -783,  -783,
    -783,  -783,   654,   656,   247,   247,   609,   676,   674,   678,
     679,  -783,   680,   321,  -783,   681,   689,  -783,   691,   692,
    -783,  -783,   839,   685,  -783,  -783,   693,  -783,   712,   841,
    -783,   745,  -783,   743,  -783,   746,  -783,  -783,  -783,   843,
    -783,   748,  -783,   906,   750,    79,   751,   754,  -783,   755,
     849,  -783,  -783,  -783,  -783,  -783,   757,  -783,    82,   758,
    -783,   916,   760,  -783,   763,   816,   922,  -783,   773,   318,
    -783,   714,   117,  -783,  -783,   265,   775,  -783,   777,   795,
    -783,  -783,  -783,  -783,   957,   813,  -783,   -14,  -783,   117,
    -783,   -84,  -783,    37,  -783,    55,  -783,    57,  -783,  -783,
    -783,   817,   977,  -783,  -783,   818,  -783,   811,   819,  -783,
    -783,  -783,  -783,  -783,  -783,  -783,   822,   823,   824,   825,
     826,   827,   828,   829,  -783,   830,  -783,   831,  -783,  -783,
     832,  -783,  -783,  -783,  -783,  -783,   833,  -783,   834,    64,
     991,  -783,   837,   931,   742,  -783,   117,   117,   742,   840,
     117,   742,   742,   842,   845,  -783,   -17,   996,   846,   864,
     999,    31,    78,   927,   847,     2,  -783,   844,   932,  -783,
     117,   850,   -84,   851,   926,   853,     8,   250,   318,    77,
    -783,  -783,   353,   848,   168,   761,  -783,   122,  -783,  -783,
     353,   265,    23,  -783,    25,  -783,   101,  -783,    79,   855,
    -783,  -783,  -783,   928,   930,   117,   117,   117,   117,   -84,
     117,   117,   117,   113,  -783,   491,  -783,   858,   117,  -783,
     856,   392,   439,   857,    79,   713,   859,   860,   117,  1018,
     865,   866,  1021,   867,   862,  -783,  -783,  -783,  -783,  1026,
    -783,   287,   870,  -783,  -783,   871,    74,   265,    82,  -783,
    1029,  -783,  1030,   865,  -783,   874,  -783,  -783,   875,    76,
    -783,  -783,  -783,  -783,   318,    37,  -783,    55,  -783,    57,
    -783,   873,  1033,  -783,  -783,  -783,  -783,  -783,  -783,   265,
    -783,  -783,  -783,  -783,   144,  -783,  -783,  -783,   -84,  -783,
    -783,  -783,  -783,  -783,  -783,   877,   878,   879,  -783,  -783,
     880,  -783,  -783,  -783,   265,  1040,  -783,   353,  -783,  1006,
    -783,   117,  -783,   881,  -783,  -783,  -783,   386,   885,   288,
     962,  1044,   888,   113,  -783,   886,   890,     8,   117,  -783,
    -783,   889,   891,   892,   893,   962,    74,  1045,    82,   895,
     894,    76,  -783,  -783,  -783,  -783,  -783,   897,   990,   265,
     117,   117,   117,   -70,  -783,   896,   413,  -783,   117,  1057,
    -783,  -783,  -783,  -783,   902,   903,  -783,   905,  1060,  -783,
    -783,   226,   865,  -783,  -783,   907,  -783,  1063,  -783,  -783,
    -783,   908,  -783,  -783,  -783,  1064,  -783,  -783,  -783,  -783,
    -783,   852,  -783,  -783,  1034,  -783,   218,   909,   288,   987,
     318,  1071,   912,   913,   962,  1000,   914,   318,  -783,   677,
     919,   -70,   917,   923,   920,  -783,  -783,  -783,  -783,  -783,
    -783,   921,  -783,  -783,  -783,   925,  -783,  -783,   677,  -783,
      79,  -783,  -783,  -783,   929,  -783,   318,  1005,  -783,   933,
     247,  -783,  -783,    90,   934,  -783,  -783,   858,   247,   924,
    -783,  -783,  -783
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -783,  -783,  1082,  -783,  -783,  -783,  -783,  -783,  -783,   132,
    -783,  -783,   872,   -94,  -332,   716,   898,  1031,  -435,  -783,
    -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,
    -783,  -783,  -783,  -783,  -783,  1032,  -783,  -783,  -783,   248,
    -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,
    -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,   657,
     910,  -783,  -783,  -783,  -783,   602,  -783,  -783,  -783,   269,
    -783,  -783,  -783,  -565,   254,  -783,   351,   223,  -783,  -783,
    -783,  -783,  -783,   184,  -783,  -783,   947,  -783,  -783,  -783,
    -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,
    -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,
    -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,  -783,
    -783,  -783,  -783,  -783,  -783,  -783,   442,  -722,   201,  -783,
    -782,  -783,  -430,  -545,  -783,  -783,  -783,   385,   730,  -198,
    -157,  -345,   621,   217,  -344,  -440,  -574,  -470,  -570,   631,
    -560,  -152,   -61,  -783,   421,  -783,  -783,   647,  -783,  -783,
     836,  -160,   614,  -783,  -437,  -783,  -783,  -783,  -783,  -783,
    -159,  -783
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -211
static const yytype_int16 yytable[] =
{
     215,   242,   341,   225,   562,   450,   287,   270,   271,   212,
     676,   606,   577,   578,   299,   300,   305,   302,   304,   574,
     312,   313,   314,   586,   476,   477,   321,   322,   320,   324,
     325,   323,   331,   707,   333,   705,   207,   338,   255,   488,
     244,   845,   608,   703,    16,   294,   264,   552,   344,   620,
     584,   230,   308,   621,   622,   233,   234,   207,   255,   370,
     521,   334,   207,   888,   372,   505,   346,   900,   508,   326,
     570,   750,     7,   327,   328,   595,   580,   761,   839,   326,
     521,     8,   596,   327,   328,   221,   326,   597,   729,   471,
     327,   328,     9,   346,   598,   955,   222,   223,   599,   901,
     745,   233,   234,   288,   256,   600,   245,   746,   244,   623,
     624,   625,     1,     2,   233,   234,   255,   408,   409,   246,
     411,   207,   766,   319,   256,   335,   289,   290,   267,    15,
     231,   683,   934,   268,   329,   626,   627,   628,   629,   630,
     631,   553,   632,   840,   534,   456,   265,   347,   535,   238,
     221,   521,   309,   357,    13,   373,   295,   633,   634,   956,
     914,   222,   223,   781,   571,   751,   635,   636,   637,   638,
     639,   762,    14,   601,   245,   351,   352,   353,   354,   355,
     602,   581,   256,   640,   775,   641,   777,   246,   564,   823,
     565,   776,    11,   778,   848,   368,   849,   237,   257,   489,
     247,   221,   605,    70,   258,   855,   248,   854,   238,   152,
     209,   210,   222,   223,   153,   853,   208,   478,   257,   726,
     576,   208,   221,   244,   258,   259,   260,   200,   494,   239,
     240,   209,   210,   222,   223,   155,   209,   210,   317,   742,
     576,   559,   710,   511,   509,   510,   211,   690,   208,   768,
     518,   698,   699,   793,   528,   519,   520,   206,   531,   211,
     586,   156,   779,   209,   210,   524,   525,   211,   247,   780,
     157,   547,   211,   551,   248,   214,   257,   556,   557,   558,
     208,   455,   258,   739,   484,   563,   710,   461,   462,   245,
     463,   464,   465,   466,   467,   209,   210,   249,   250,   211,
     497,   498,   246,   259,   260,   585,   858,   347,   479,   670,
     593,   576,   353,   354,   355,   607,   233,   234,   516,   764,
     485,   842,   158,   294,   659,   389,   870,   871,   872,   496,
     542,   211,   543,   544,   503,   159,   524,   525,   507,   160,
     233,   234,   770,   771,   233,   234,   349,   643,   235,   236,
     161,   207,   226,   227,   228,   229,   767,   651,   652,   533,
     235,   236,   162,   879,   163,   216,   164,   165,   361,   350,
     362,   166,   167,   168,   363,   364,   365,   169,   170,   923,
     219,   171,   -14,   247,   172,   293,   173,   603,   220,   248,
     233,   234,   460,   306,   307,   949,   174,   175,   521,   237,
     176,   177,   522,   178,   179,   180,   181,   357,   182,   298,
     238,   891,   249,   250,   183,   339,   233,   234,   351,   352,
     353,   354,   355,   237,   184,   185,   186,   237,   187,   188,
     189,   239,   240,   371,   303,   353,   354,   355,   238,   190,
     191,   192,   852,   351,   352,   353,   354,   355,   193,   701,
     301,   194,   195,   197,   366,   239,   240,   198,   199,   239,
     240,   205,   217,   343,   233,   234,   272,   273,   274,   275,
     276,   317,   277,   237,   278,   279,   280,   281,   282,   342,
     345,   348,   755,   386,   283,   378,   382,   381,   383,   406,
     407,   384,   385,   389,   730,   388,   391,   367,   733,   237,
     389,   736,   737,   390,   392,   239,   240,   394,   398,   395,
     368,   399,  -210,   401,   369,   284,   221,   785,   786,   787,
     788,   692,   790,   791,   792,   209,   210,   222,   223,   402,
     757,   239,   240,   289,   290,   403,   404,   405,   700,   419,
     272,   273,   274,   275,   276,   410,   277,   237,   278,   279,
     280,   281,   282,   819,   412,   420,   820,   413,   523,   414,
     416,   211,   794,   795,   418,   233,   234,   789,   351,   352,
     353,   354,   355,   814,   904,   421,   422,   905,   929,   239,
     240,   442,   423,   524,   525,   937,   424,   425,   426,   284,
     427,   796,   797,   428,   798,   731,   732,   429,   430,   735,
     821,   351,   868,   353,   354,   355,   431,   351,   352,   353,
     354,   355,   432,   433,   951,   799,   434,   800,   801,   802,
     803,   804,   805,   806,   807,   808,   809,   810,   351,   352,
     353,   354,   355,   436,   437,   351,   352,   353,   354,   355,
     883,   272,   273,   274,   275,   276,   859,   277,   237,   278,
     279,   280,   281,   282,   351,   352,   353,   354,   355,   283,
     221,   438,   897,   898,   899,   440,   459,   817,   439,   441,
     443,   222,   223,   444,   445,   446,   447,   827,   448,   449,
     239,   240,   452,    16,   453,   454,   457,   487,   468,   491,
     284,    17,   590,   469,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,   591,   470,   480,    28,    29,    30,
      31,    32,   481,    33,   482,   610,   492,   493,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,   495,
      45,    46,    47,    48,    49,    50,    51,    52,    53,   500,
      54,   351,   352,   353,   354,   355,   501,   502,   504,   506,
     512,   513,   514,   351,   352,   353,   354,   355,   515,   529,
     866,   532,   530,    55,   351,   352,   353,   354,   355,   538,
     616,   537,   540,    56,   539,   541,   548,   619,   550,    57,
     549,    58,    59,   647,    60,   555,    61,    62,    63,   560,
     567,    64,    65,   954,    66,   568,    67,   516,   233,   234,
     573,   961,   569,  -168,   583,   594,   611,   906,   609,   612,
     614,   233,   234,   615,   618,   649,    68,   650,    69,   351,
     352,   353,   354,   355,   646,   939,   351,   352,   353,   354,
     355,   653,   351,   352,   353,   354,   355,   654,   655,   656,
     657,   658,    70,   664,   948,   668,   660,   672,    71,    72,
     661,   665,   662,   663,   666,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,   667,   272,   273,   274,   275,   276,   824,
     277,   237,   278,   279,   280,   281,   282,   272,   273,   274,
     275,   276,   283,   277,   237,   278,   279,   280,   281,   282,
     351,   352,   353,   354,   355,   283,   669,   671,   670,   673,
     674,   675,   677,   239,   240,   678,   680,   681,   682,   684,
     685,   686,   688,   284,   687,   659,   239,   240,   351,   352,
     353,   354,   355,    72,   689,   691,   284,   694,   695,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,   201,    74,   696,
      76,   697,    78,    73,    80,    75,    82,    77,    84,    79,
      86,    81,    88,    83,    90,    85,   288,    87,   708,    89,
     709,   711,   710,   712,   713,   714,   715,   716,   717,   718,
     719,   720,   721,   722,   723,   725,   727,   724,   728,   516,
     741,   734,   743,   744,   748,   738,   739,   742,   749,   754,
     759,   756,   758,   753,   760,   769,   782,   783,   784,   815,
     818,   822,   828,   825,   826,   832,   829,   831,   834,   833,
     835,   837,   838,   843,   844,   846,   847,   856,   857,   860,
     861,   862,   863,   864,   865,   867,   869,   875,   877,   878,
     880,   881,   890,   884,   887,   885,   886,   892,   895,   896,
     903,   907,   893,   908,   912,   909,   911,   916,   915,   920,
     918,   927,   922,   925,   921,   931,   932,   933,   936,   935,
     940,   942,   943,   952,   944,   945,   947,    10,   962,   960,
     950,   196,   380,   483,   953,   958,   202,   340,   561,   894,
     889,   617,   836,   332,   913,   941,   882,   254,   740,   926,
     813,   604,   486,   613,     0,   592,   773,   415,   644
};

static const yytype_int16 yycheck[] =
{
      94,   158,   200,   155,   441,   337,   166,   164,   165,    70,
     555,   481,   452,   453,   171,   172,   175,   174,   175,   449,
     179,   180,   181,   458,   369,   369,   186,   187,   185,   189,
     190,   188,   192,   607,   194,   605,     4,   197,     3,     3,
       3,   763,   482,   603,     6,    52,    69,    69,   205,     4,
      69,     4,    69,     8,     9,    69,    70,     4,     3,   216,
       3,     5,     4,   845,   216,   397,     8,   137,   400,     4,
      69,    69,   162,     8,     9,   161,   116,    69,     4,     4,
       3,   162,   168,     8,     9,   169,     4,   161,   653,     4,
       8,     9,     0,     8,   168,     5,   180,   181,   161,   169,
      69,    69,    70,   163,    69,   168,    69,    76,     3,    64,
      65,    66,    15,    16,    69,    70,     3,   274,   275,    82,
     277,     4,   687,   184,    69,    69,   186,   187,    64,   162,
      83,   568,   914,    69,    69,    90,    91,    92,    93,    94,
      95,   163,    97,    69,    65,   343,   169,   208,    69,   163,
     169,     3,   169,   214,   169,   216,   163,   112,   113,    69,
     882,   180,   181,   708,   163,   163,   121,   122,   123,   124,
     125,   163,   169,   161,    69,   215,   216,   217,   218,   219,
     168,   221,    69,   138,   161,   140,   161,    82,    67,   734,
      69,   168,    98,   168,   118,   163,   120,   152,   163,   163,
     163,   169,   167,   165,   169,   779,   169,   777,   163,   161,
     178,   179,   180,   181,    99,   775,   163,   369,   163,   649,
     163,   163,   169,     3,   169,   190,   191,   165,   388,   184,
     185,   178,   179,   180,   181,   162,   178,   179,   163,   161,
     163,   439,   164,   403,   401,   402,   214,   579,   163,   689,
     410,   595,   597,   723,   414,   412,   413,   171,   418,   214,
     695,   162,   161,   178,   179,   188,   189,   214,   163,   168,
     162,   428,   214,   432,   169,   163,   163,   436,   437,   438,
     163,   342,   169,   161,   378,   442,   164,   348,   349,    69,
     351,   352,   353,   354,   355,   178,   179,   192,   193,   214,
      74,    75,    82,   190,   191,   457,   162,   368,   369,   165,
     470,   163,   217,   218,   219,   167,    69,    70,    68,    69,
     381,   758,   162,    52,     3,     4,    38,    39,    40,   390,
      69,   214,    71,    72,   395,   162,   188,   189,   399,   162,
      69,    70,   174,   175,    69,    70,   143,   504,    73,    74,
     162,     4,    10,    11,    12,    13,   688,   514,   515,   420,
      73,    74,   162,   833,   162,   220,   162,   162,    21,   166,
      23,   162,   162,   162,    27,    28,    29,   162,   162,   161,
       3,   162,   164,   163,   162,   168,   162,   167,   161,   169,
      69,    70,   164,   176,   177,   940,   162,   162,     3,   152,
     162,   162,     7,   162,   162,   162,   162,   468,   162,   169,
     163,   848,   192,   193,   162,   198,    69,    70,   215,   216,
     217,   218,   219,   152,   162,   162,   162,   152,   162,   162,
     162,   184,   185,   216,   163,   217,   218,   219,   163,   162,
     162,   162,   774,   215,   216,   217,   218,   219,   162,   601,
      80,   162,   162,   162,   107,   184,   185,   162,   162,   184,
     185,   162,   162,   165,    69,    70,   145,   146,   147,   148,
     149,   163,   151,   152,   153,   154,   155,   156,   157,   220,
     163,   165,   680,   160,   163,   167,   221,   165,   161,   272,
     273,   161,   161,     4,   654,   163,   161,   150,   658,   152,
       4,   661,   662,   165,   161,   184,   185,     3,     3,   165,
     163,   165,   167,   161,   167,   194,   169,   715,   716,   717,
     718,   582,   720,   721,   722,   178,   179,   180,   181,   161,
     682,   184,   185,   186,   187,   161,   161,   161,   599,     4,
     145,   146,   147,   148,   149,   163,   151,   152,   153,   154,
     155,   156,   157,   161,   163,   165,   164,   163,   163,   163,
     163,   214,    71,    72,   223,    69,    70,   719,   215,   216,
     217,   218,   219,   725,   161,   161,   161,   164,   910,   184,
     185,   161,   163,   188,   189,   917,     4,   161,   161,   194,
     161,   100,   101,   161,   103,   656,   657,   161,     4,   660,
     161,   215,   216,   217,   218,   219,   161,   215,   216,   217,
     218,   219,   161,   161,   946,   124,     6,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   215,   216,
     217,   218,   219,   161,   161,   215,   216,   217,   218,   219,
     838,   145,   146,   147,   148,   149,   798,   151,   152,   153,
     154,   155,   156,   157,   215,   216,   217,   218,   219,   163,
     169,   161,   860,   861,   862,     4,   164,   728,   161,   161,
     161,   180,   181,   161,   161,   161,   161,   738,   161,   161,
     184,   185,   161,     6,   161,   166,   161,     3,   161,     3,
     194,    14,   166,   164,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,   166,   162,   162,    30,    31,    32,
      33,    34,   162,    36,   162,   166,   161,   166,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,   161,
      53,    54,    55,    56,    57,    58,    59,    60,    61,     5,
      63,   215,   216,   217,   218,   219,   170,   161,   161,   161,
      68,    68,   161,   215,   216,   217,   218,   219,   161,   158,
     821,   161,   164,    86,   215,   216,   217,   218,   219,     4,
     166,    35,    35,    96,   161,    37,    77,   166,    77,   102,
     161,   104,   105,   166,   107,   161,   109,   110,   111,   161,
      81,   114,   115,   950,   117,   163,   119,    68,    69,    70,
       4,   958,    87,   106,   166,   161,     3,   868,   168,   161,
     164,    69,    70,     4,   169,   161,   139,   161,   141,   215,
     216,   217,   218,   219,   169,   919,   215,   216,   217,   218,
     219,   222,   215,   216,   217,   218,   219,   161,   164,   161,
     161,   161,   165,     4,   938,     4,   165,     4,   171,   172,
     161,   166,   161,   161,   161,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   161,   145,   146,   147,   148,   149,   166,
     151,   152,   153,   154,   155,   156,   157,   145,   146,   147,
     148,   149,   163,   151,   152,   153,   154,   155,   156,   157,
     215,   216,   217,   218,   219,   163,   161,   161,   165,   161,
       4,   161,   161,   184,   185,   161,   161,    68,   161,   161,
       4,   161,   106,   194,   161,     3,   184,   185,   215,   216,
     217,   218,   219,   172,   161,   221,   194,   162,   161,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   172,   179,   164,
     181,     4,   183,   178,   185,   180,   187,   182,   189,   184,
     191,   186,   193,   188,   195,   190,   163,   192,   161,   194,
       3,   170,   164,   164,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   161,     5,   164,   161,    68,
       4,   161,   138,     4,    77,   163,   161,   161,   161,    77,
      84,   161,   161,   169,   161,   167,   161,    89,    88,   161,
     164,   164,     4,   164,   164,     4,   161,   161,   166,   162,
       4,   161,   161,     4,     4,   161,   161,   164,     5,   162,
     162,   162,   162,     3,    38,   164,   161,    85,     4,   161,
     164,   161,     7,   164,   161,   164,   164,   162,   161,    69,
     164,     4,   168,   161,     4,   162,   161,     4,   161,     5,
     162,    84,    38,   164,   222,     4,   164,   164,   164,    79,
     161,   164,   159,    78,   164,   164,   161,     5,   164,   957,
     161,    59,   220,   377,   161,   161,    65,   199,   441,   851,
     846,   499,   751,   193,   881,   921,   837,   160,   666,   908,
     725,   480,   382,   492,    -1,   468,   695,   281,   504
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   225,   226,   227,   228,   162,   162,     0,
     226,    98,   235,   169,   169,   162,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    30,    31,
      32,    33,    34,    36,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    63,    86,    96,   102,   104,   105,
     107,   109,   110,   111,   114,   115,   117,   119,   139,   141,
     165,   171,   172,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   229,   230,   237,   239,   240,   241,   242,   244,   245,
     253,   256,   257,   259,   260,   267,   269,   270,   272,   274,
     276,   280,   281,   282,   285,   287,   294,   299,   302,   308,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     324,   325,   326,   327,   328,   330,   331,   332,   333,   334,
     335,   336,   337,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   161,    99,   236,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   259,   162,   162,   162,
     165,   172,   241,   243,   261,   162,   171,     4,   163,   178,
     179,   214,   376,   386,   163,   237,   220,   162,   271,     3,
     161,   169,   180,   181,   362,   375,    10,    11,    12,    13,
       4,    83,   387,    69,    70,    73,    74,   152,   163,   184,
     185,   300,   364,   365,     3,    69,    82,   163,   169,   192,
     193,   310,   373,   374,   310,     3,    69,   163,   169,   190,
     191,   304,   371,   372,    69,   169,   349,    64,    69,   356,
     364,   364,   145,   146,   147,   148,   149,   151,   153,   154,
     155,   156,   157,   163,   194,   364,   384,   385,   163,   186,
     187,   367,   368,   367,    52,   163,   394,   395,   169,   364,
     364,    80,   364,   163,   364,   394,   367,   367,    69,   169,
     284,   350,   394,   394,   394,   363,   376,   163,   283,   376,
     364,   385,   385,   364,   385,   385,     4,     8,     9,    69,
     388,   385,   284,   385,     5,    69,   295,   277,   385,   367,
     240,   363,   220,   165,   364,   163,     8,   376,   165,   143,
     166,   215,   216,   217,   218,   219,   375,   376,   381,   382,
     383,    21,    23,    27,    28,    29,   107,   150,   163,   167,
     364,   367,   375,   376,   390,   391,   392,   275,   167,   238,
     236,   165,   221,   161,   161,   161,   160,   366,   163,     4,
     165,   161,   161,   289,     3,   165,   311,   309,     3,   165,
     303,   161,   161,   161,   161,   161,   367,   367,   364,   364,
     163,   364,   163,   163,   163,   384,   163,   385,   223,     4,
     165,   161,   161,   163,     4,   161,   161,   161,   161,   161,
       4,   161,   161,   161,     6,   258,   161,   161,   161,   161,
       4,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     238,   278,   161,   161,   166,   376,   363,   161,   254,   164,
     164,   376,   376,   376,   376,   376,   376,   376,   161,   164,
     162,     4,   249,   250,   251,   252,   365,   368,   375,   376,
     162,   162,   162,   239,   237,   376,   362,     3,     3,   163,
     329,     3,   161,   166,   385,   161,   376,    74,    75,   301,
       5,   170,   161,   376,   161,   238,   161,   376,   238,   364,
     364,   385,    68,    68,   161,   161,    68,   297,   385,   364,
     364,     3,     7,   163,   188,   189,   369,   370,   385,   158,
     164,   385,   161,   376,    65,    69,   357,    35,     4,   161,
      35,    37,    69,    71,    72,   358,   361,   364,    77,   161,
      77,   394,    69,   163,   290,   161,   394,   394,   394,   363,
     161,   283,   388,   364,    67,    69,   393,    81,   163,    87,
      69,   163,   291,     4,   356,   279,   163,   369,   369,   273,
     116,   221,   268,   166,    69,   375,   242,   378,   379,   380,
     166,   166,   381,   385,   161,   161,   168,   161,   168,   161,
     168,   161,   168,   167,   373,   167,   371,   167,   369,   168,
     166,     3,   161,   366,   164,     4,   166,   289,   169,   166,
       4,     8,     9,    64,    65,    66,    90,    91,    92,    93,
      94,    95,    97,   112,   113,   121,   122,   123,   124,   125,
     138,   140,   312,   364,   386,   389,   169,   166,   305,   161,
     161,   364,   364,   222,   161,   164,   161,   161,   161,     3,
     165,   161,   161,   161,     4,   166,   161,   161,     4,   161,
     165,   161,     4,   161,     4,   161,   357,   161,   161,   323,
     161,    68,   161,   388,   161,     4,   161,   161,   106,   161,
     238,   221,   376,   262,   162,   161,   164,     4,   368,   365,
     376,   375,   246,   374,   247,   372,   248,   370,   161,     3,
     164,   170,   164,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   164,   161,   356,     5,   161,   297,
     385,   376,   376,   385,   161,   376,   385,   385,   163,   161,
     350,     4,   161,   138,     4,    69,    76,   360,    77,   161,
      69,   163,   292,   169,    77,   363,   161,   375,   161,    84,
     161,    69,   163,   293,    69,   296,   297,   238,   369,   167,
     174,   175,   377,   378,   255,   161,   168,   161,   168,   161,
     168,   357,   161,    89,    88,   363,   363,   363,   363,   375,
     363,   363,   363,   371,    71,    72,   100,   101,   103,   124,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   306,   359,   361,   375,   161,   233,   376,   164,   161,
     164,   161,   164,   357,   166,   164,   164,   376,     4,   161,
     351,   161,     4,   162,   166,     4,   300,   161,   161,     4,
      69,   298,   388,     4,     4,   351,   161,   161,   118,   120,
     263,   264,   238,   374,   372,   370,   164,     5,   162,   375,
     162,   162,   162,   162,     3,    38,   376,   164,   216,   161,
      38,    39,    40,   352,   353,    85,   354,     4,   161,   371,
     164,   161,   293,   363,   164,   164,   164,   161,   354,   298,
       7,   388,   162,   168,   263,   161,    69,   363,   363,   363,
     137,   169,   307,   164,   161,   164,   376,     4,   161,   162,
     338,   161,     4,   301,   351,   161,     4,   286,   162,   266,
       5,   222,    38,   161,   231,   164,   352,    84,   355,   238,
     339,     4,   164,   164,   354,    79,   164,   238,   265,   237,
     161,   307,   164,   159,   164,   164,   288,   161,   237,   357,
     161,   238,    78,   161,   364,     5,    69,   234,   161,   232,
     233,   364,   164
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

  case 106:

/* Line 1455 of yacc.c  */
#line 568 "lev_comp.y"
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

  case 107:

/* Line 1455 of yacc.c  */
#line 584 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 590 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 596 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 602 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 608 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 614 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 620 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 626 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 632 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 642 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 652 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 662 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 672 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 682 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 692 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 704 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 709 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 716 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 721 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 728 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 733 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 740 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 744 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 750 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 755 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 762 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 766 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 772 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 776 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 782 "lev_comp.y"
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

  case 137:

/* Line 1455 of yacc.c  */
#line 804 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 808 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 819 "lev_comp.y"
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

  case 140:

/* Line 1455 of yacc.c  */
#line 874 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 880 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 884 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 890 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 897 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 901 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 909 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 913 "lev_comp.y"
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

  case 148:

/* Line 1455 of yacc.c  */
#line 936 "lev_comp.y"
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

  case 151:

/* Line 1455 of yacc.c  */
#line 981 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 991 "lev_comp.y"
    {
		  ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 994 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1004 "lev_comp.y"
    {
		  ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 1009 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1023 "lev_comp.y"
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

  case 159:

/* Line 1455 of yacc.c  */
#line 1060 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1065 "lev_comp.y"
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

  case 161:

/* Line 1455 of yacc.c  */
#line 1094 "lev_comp.y"
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

  case 162:

/* Line 1455 of yacc.c  */
#line 1108 "lev_comp.y"
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

  case 163:

/* Line 1455 of yacc.c  */
#line 1124 "lev_comp.y"
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

  case 164:

/* Line 1455 of yacc.c  */
#line 1144 "lev_comp.y"
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
#line 1156 "lev_comp.y"
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

  case 166:

/* Line 1455 of yacc.c  */
#line 1176 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1182 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1192 "lev_comp.y"
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

  case 169:

/* Line 1455 of yacc.c  */
#line 1210 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1220 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1226 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1231 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1236 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1243 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1250 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1259 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1267 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1278 "lev_comp.y"
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

  case 179:

/* Line 1455 of yacc.c  */
#line 1289 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1296 "lev_comp.y"
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

  case 181:

/* Line 1455 of yacc.c  */
#line 1307 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1314 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1318 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1324 "lev_comp.y"
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

  case 185:

/* Line 1455 of yacc.c  */
#line 1334 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1340 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1349 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1355 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1360 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1366 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1371 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1377 "lev_comp.y"
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

  case 193:

/* Line 1455 of yacc.c  */
#line 1388 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (5)].i), SPO_DOOR));
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1402 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1406 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1416 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1423 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1431 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1447 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1451 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1457 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1465 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1471 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1479 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1487 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1492 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1498 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1504 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1510 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1516 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1521 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1526 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1531 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1536 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1541 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1546 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1551 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1556 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1561 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1566 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1572 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1579 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1588 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1592 "lev_comp.y"
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

  case 235:

/* Line 1455 of yacc.c  */
#line 1605 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1613 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1623 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1631 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1640 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1647 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1655 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1661 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1666 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1671 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1676 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1681 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1686 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1691 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1696 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1701 "lev_comp.y"
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

  case 251:

/* Line 1455 of yacc.c  */
#line 1712 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1718 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1723 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1728 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1733 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1738 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1743 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1748 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1753 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1758 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1763 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1770 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1776 "lev_comp.y"
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

  case 264:

/* Line 1455 of yacc.c  */
#line 1805 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1810 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1818 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1824 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1830 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1837 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1844 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1854 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1864 "lev_comp.y"
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

  case 273:

/* Line 1455 of yacc.c  */
#line 1879 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1889 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1893 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1899 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1905 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1911 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int)(yyvsp[(6) - (13)].i), (int)(yyvsp[(8) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1917 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1923 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1929 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1934 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1941 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1948 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1954 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1960 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1966 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1970 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1976 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1982 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1992 "lev_comp.y"
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

  case 292:

/* Line 1455 of yacc.c  */
#line 2006 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      if ( (yyvsp[(10) - (11)].i) ) {
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      } else if ( (yyvsp[(11) - (11)].i) )
			  lc_error("Cannot use lev statements in non-permanent REGION");
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 2016 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2020 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2026 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2033 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2039 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2045 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2051 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2057 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2061 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2066 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2073 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2079 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2086 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2090 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2097 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2108 "lev_comp.y"
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

  case 311:

/* Line 1455 of yacc.c  */
#line 2121 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2125 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2131 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2135 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2142 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2146 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2150 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2156 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2160 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2166 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2182 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2190 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2200 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2209 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2214 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2221 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2232 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2238 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2242 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2246 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2253 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2263 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2270 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2274 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2280 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2284 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2292 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2296 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2303 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2313 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2326 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2330 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2337 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2347 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2356 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2367 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2371 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2378 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2388 "lev_comp.y"
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

  case 360:

/* Line 1455 of yacc.c  */
#line 2399 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2408 "lev_comp.y"
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

  case 362:

/* Line 1455 of yacc.c  */
#line 2418 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2424 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2428 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2435 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2445 "lev_comp.y"
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

  case 367:

/* Line 1455 of yacc.c  */
#line 2456 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2465 "lev_comp.y"
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

  case 369:

/* Line 1455 of yacc.c  */
#line 2475 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2479 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2485 "lev_comp.y"
    { ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2487 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2493 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2497 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2501 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2505 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2513 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2522 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2526 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2530 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2534 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2538 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2541 "lev_comp.y"
    { ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2545 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2552 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2561 "lev_comp.y"
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

  case 391:

/* Line 1455 of yacc.c  */
#line 2602 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2606 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2613 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2620 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2630 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2634 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2642 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2646 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2650 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2654 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2658 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2663 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2667 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2671 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2676 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2681 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2686 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2690 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2695 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2700 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2704 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2708 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2713 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2717 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2724 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2730 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2734 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2740 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2756 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2760 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2764 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2768 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2790 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2794 "lev_comp.y"
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

  case 438:

/* Line 1455 of yacc.c  */
#line 2820 "lev_comp.y"
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
#line 6370 "lev_comp.tab.c"
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
#line 2848 "lev_comp.y"


/*lev_comp.y*/

