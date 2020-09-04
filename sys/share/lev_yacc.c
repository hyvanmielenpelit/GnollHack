
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
     MON_APPEARANCE = 340,
     ROOMDOOR_ID = 341,
     IF_ID = 342,
     ELSE_ID = 343,
     TERRAIN_ID = 344,
     HORIZ_OR_VERT = 345,
     REPLACE_TERRAIN_ID = 346,
     EXIT_ID = 347,
     SHUFFLE_ID = 348,
     QUANTITY_ID = 349,
     BURIED_ID = 350,
     LOOP_ID = 351,
     FOR_ID = 352,
     TO_ID = 353,
     SWITCH_ID = 354,
     CASE_ID = 355,
     BREAK_ID = 356,
     DEFAULT_ID = 357,
     ERODED_ID = 358,
     TRAPPED_STATE = 359,
     RECHARGED_ID = 360,
     INVIS_ID = 361,
     GREASED_ID = 362,
     FEMALE_ID = 363,
     WAITFORU_ID = 364,
     CANCELLED_ID = 365,
     REVIVED_ID = 366,
     AVENGE_ID = 367,
     FLEEING_ID = 368,
     BLINDED_ID = 369,
     PARALYZED_ID = 370,
     STUNNED_ID = 371,
     CONFUSED_ID = 372,
     SEENTRAPS_ID = 373,
     ALL_ID = 374,
     MONTYPE_ID = 375,
     GRAVE_ID = 376,
     ERODEPROOF_ID = 377,
     FUNCTION_ID = 378,
     MSG_OUTPUT_TYPE = 379,
     COMPARE_TYPE = 380,
     UNKNOWN_TYPE = 381,
     rect_ID = 382,
     fillrect_ID = 383,
     line_ID = 384,
     randline_ID = 385,
     grow_ID = 386,
     selection_ID = 387,
     flood_ID = 388,
     rndcoord_ID = 389,
     circle_ID = 390,
     ellipse_ID = 391,
     filter_ID = 392,
     complement_ID = 393,
     gradient_ID = 394,
     GRADIENT_TYPE = 395,
     LIMITED = 396,
     HUMIDITY_TYPE = 397,
     STRING = 398,
     MAP_ID = 399,
     NQSTRING = 400,
     VARSTRING = 401,
     CFUNC = 402,
     CFUNC_INT = 403,
     CFUNC_STR = 404,
     CFUNC_COORD = 405,
     CFUNC_REGION = 406,
     VARSTRING_INT = 407,
     VARSTRING_INT_ARRAY = 408,
     VARSTRING_STRING = 409,
     VARSTRING_STRING_ARRAY = 410,
     VARSTRING_VAR = 411,
     VARSTRING_VAR_ARRAY = 412,
     VARSTRING_COORD = 413,
     VARSTRING_COORD_ARRAY = 414,
     VARSTRING_REGION = 415,
     VARSTRING_REGION_ARRAY = 416,
     VARSTRING_MAPCHAR = 417,
     VARSTRING_MAPCHAR_ARRAY = 418,
     VARSTRING_MONST = 419,
     VARSTRING_MONST_ARRAY = 420,
     VARSTRING_OBJ = 421,
     VARSTRING_OBJ_ARRAY = 422,
     VARSTRING_SEL = 423,
     VARSTRING_SEL_ARRAY = 424,
     METHOD_INT = 425,
     METHOD_INT_ARRAY = 426,
     METHOD_STRING = 427,
     METHOD_STRING_ARRAY = 428,
     METHOD_VAR = 429,
     METHOD_VAR_ARRAY = 430,
     METHOD_COORD = 431,
     METHOD_COORD_ARRAY = 432,
     METHOD_REGION = 433,
     METHOD_REGION_ARRAY = 434,
     METHOD_MAPCHAR = 435,
     METHOD_MAPCHAR_ARRAY = 436,
     METHOD_MONST = 437,
     METHOD_MONST_ARRAY = 438,
     METHOD_OBJ = 439,
     METHOD_OBJ_ARRAY = 440,
     METHOD_SEL = 441,
     METHOD_SEL_ARRAY = 442,
     DICE = 443
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
#line 486 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 498 "lev_comp.tab.c"

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
#define YYLAST   1064

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  206
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  165
/* YYNRULES -- Number of rules.  */
#define YYNRULES  418
/* YYNRULES -- Number of states.  */
#define YYNSTATES  902

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   443

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   201,   205,     2,
     145,   146,   199,   197,   143,   198,   203,   200,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   144,     2,
       2,   202,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   147,     2,   148,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   149,   204,   150,     2,     2,     2,     2,
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
     135,   136,   137,   138,   139,   140,   141,   142,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196
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
     239,   241,   245,   249,   255,   259,   265,   271,   277,   281,
     285,   291,   297,   303,   311,   319,   327,   333,   335,   339,
     341,   345,   347,   351,   353,   357,   359,   363,   365,   369,
     371,   375,   376,   377,   386,   391,   393,   394,   396,   398,
     404,   408,   409,   410,   420,   421,   424,   425,   431,   432,
     437,   439,   442,   444,   451,   452,   456,   457,   464,   465,
     470,   471,   476,   478,   479,   484,   488,   490,   494,   498,
     504,   510,   518,   523,   524,   535,   536,   549,   550,   553,
     559,   561,   567,   569,   575,   577,   583,   585,   595,   601,
     603,   605,   607,   609,   611,   615,   617,   619,   621,   629,
     635,   637,   639,   641,   643,   647,   648,   654,   659,   660,
     664,   666,   668,   670,   672,   675,   677,   679,   681,   683,
     685,   689,   693,   697,   699,   701,   705,   707,   709,   711,
     715,   719,   720,   726,   729,   730,   734,   736,   740,   742,
     746,   750,   752,   754,   758,   760,   762,   764,   768,   770,
     772,   774,   780,   788,   794,   803,   805,   809,   815,   821,
     829,   837,   844,   850,   851,   854,   860,   864,   878,   882,
     886,   888,   894,   904,   910,   914,   918,   919,   930,   931,
     933,   941,   945,   951,   957,   963,   967,   973,   981,   991,
     993,   995,   997,   999,  1001,  1002,  1005,  1007,  1011,  1013,
    1015,  1017,  1019,  1021,  1023,  1025,  1027,  1029,  1031,  1033,
    1035,  1039,  1041,  1043,  1048,  1050,  1052,  1057,  1059,  1061,
    1066,  1068,  1073,  1079,  1081,  1085,  1087,  1091,  1093,  1095,
    1100,  1110,  1112,  1114,  1119,  1121,  1127,  1129,  1131,  1136,
    1138,  1140,  1146,  1148,  1150,  1152,  1157,  1159,  1161,  1167,
    1169,  1171,  1173,  1177,  1179,  1181,  1185,  1187,  1192,  1196,
    1200,  1204,  1208,  1212,  1216,  1218,  1220,  1224,  1226,  1230,
    1231,  1233,  1235,  1237,  1239,  1243,  1244,  1246,  1248,  1251,
    1254,  1259,  1266,  1271,  1278,  1285,  1292,  1299,  1302,  1309,
    1318,  1327,  1338,  1353,  1356,  1358,  1362,  1364,  1368,  1370,
    1372,  1374,  1376,  1378,  1380,  1382,  1384,  1386,  1388,  1390,
    1392,  1394,  1396,  1398,  1400,  1402,  1404,  1406,  1417
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     207,     0,    -1,    -1,   208,    -1,   209,    -1,   209,   208,
      -1,   210,   217,   219,    -1,    16,   144,   151,    -1,    15,
     144,   151,   143,     3,    -1,    17,   144,    11,   143,   311,
      -1,    17,   144,    10,   143,     3,    -1,    17,   144,    13,
      -1,    17,   144,    12,   143,     3,   143,     3,   143,     5,
     143,     5,   143,   332,   143,   216,   215,    -1,    18,   144,
     362,    -1,    -1,   143,   141,    -1,    -1,   143,   339,    -1,
      -1,   143,     3,    -1,     5,    -1,    64,    -1,    -1,    81,
     144,   218,    -1,    82,   143,   218,    -1,    82,    -1,    -1,
     221,   219,    -1,   149,   219,   150,    -1,   262,    -1,   211,
      -1,   212,    -1,   319,    -1,   320,    -1,   321,    -1,   322,
      -1,   304,    -1,   264,    -1,   227,    -1,   226,    -1,   314,
      -1,   276,    -1,   296,    -1,   324,    -1,   325,    -1,   306,
      -1,   307,    -1,   308,    -1,   323,    -1,   242,    -1,   252,
      -1,   254,    -1,   258,    -1,   256,    -1,   239,    -1,   249,
      -1,   235,    -1,   238,    -1,   299,    -1,   281,    -1,   297,
      -1,   284,    -1,   290,    -1,   315,    -1,   310,    -1,   302,
      -1,   263,    -1,   316,    -1,   269,    -1,   267,    -1,   309,
      -1,   313,    -1,   312,    -1,   300,    -1,   301,    -1,   303,
      -1,   295,    -1,   298,    -1,   161,    -1,   163,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
      -1,   177,    -1,   160,    -1,   162,    -1,   164,    -1,   166,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,   176,
      -1,   222,    -1,   223,    -1,   154,    -1,   154,    -1,   223,
      -1,    93,   144,   222,    -1,   224,   202,   351,    -1,   224,
     202,   132,   144,   360,    -1,   224,   202,   350,    -1,   224,
     202,   367,   144,   344,    -1,   224,   202,   366,   144,   346,
      -1,   224,   202,   365,   144,   348,    -1,   224,   202,   339,
      -1,   224,   202,   342,    -1,   224,   202,   149,   233,   150,
      -1,   224,   202,   149,   232,   150,    -1,   224,   202,   149,
     231,   150,    -1,   224,   202,   367,   144,   149,   230,   150,
      -1,   224,   202,   366,   144,   149,   229,   150,    -1,   224,
     202,   365,   144,   149,   228,   150,    -1,   224,   202,   149,
     234,   150,    -1,   349,    -1,   228,   143,   349,    -1,   347,
      -1,   229,   143,   347,    -1,   345,    -1,   230,   143,   345,
      -1,   343,    -1,   231,   143,   343,    -1,   340,    -1,   232,
     143,   340,    -1,   351,    -1,   233,   143,   351,    -1,   350,
      -1,   234,   143,   350,    -1,    -1,    -1,   123,   153,   145,
     236,   355,   146,   237,   220,    -1,   153,   145,   358,   146,
      -1,    92,    -1,    -1,     6,    -1,     6,    -1,   147,   351,
     125,   351,   148,    -1,   147,   351,   148,    -1,    -1,    -1,
      99,   243,   147,   338,   148,   244,   149,   245,   150,    -1,
      -1,   246,   245,    -1,    -1,   100,   363,   144,   247,   219,
      -1,    -1,   102,   144,   248,   219,    -1,   101,    -1,   203,
     203,    -1,    98,    -1,    97,   225,   202,   351,   250,   351,
      -1,    -1,   251,   253,   220,    -1,    -1,    96,   147,   338,
     148,   255,   220,    -1,    -1,   241,   144,   257,   221,    -1,
      -1,    87,   241,   259,   260,    -1,   220,    -1,    -1,   220,
     261,    88,   220,    -1,    14,   144,   350,    -1,    58,    -1,
      58,   144,   363,    -1,    58,   144,    64,    -1,    49,   144,
     265,   143,   265,    -1,    49,   144,   265,   143,   363,    -1,
     145,     4,   143,    63,   143,   280,   146,    -1,   327,   240,
     143,   332,    -1,    -1,    79,   144,   266,   143,   273,   143,
     275,   328,   268,   220,    -1,    -1,    43,   144,   266,   143,
     272,   143,   274,   143,   275,   328,   270,   220,    -1,    -1,
     143,     5,    -1,   145,     4,   143,     4,   146,    -1,    64,
      -1,   145,     4,   143,     4,   146,    -1,    64,    -1,   145,
     282,   143,   283,   146,    -1,    64,    -1,   145,     4,   143,
       4,   146,    -1,    64,    -1,    86,   144,   277,   143,   331,
     143,   278,   143,   280,    -1,    25,   144,   331,   143,   360,
      -1,     5,    -1,    64,    -1,   279,    -1,    64,    -1,    63,
      -1,    63,   204,   279,    -1,     4,    -1,    64,    -1,    20,
      -1,    19,   144,   282,   143,   283,   271,   152,    -1,    19,
     144,   339,   271,   152,    -1,    68,    -1,    69,    -1,    70,
      -1,    69,    -1,    23,   144,   286,    -1,    -1,    23,   144,
     286,   285,   220,    -1,   346,   143,   339,   287,    -1,    -1,
     287,   143,   288,    -1,   350,    -1,    83,    -1,    84,    -1,
     334,    -1,    85,   350,    -1,   108,    -1,   106,    -1,   110,
      -1,   111,    -1,   112,    -1,   113,   144,   338,    -1,   114,
     144,   338,    -1,   115,   144,   338,    -1,   116,    -1,   117,
      -1,   118,   144,   289,    -1,   109,    -1,   151,    -1,   119,
      -1,   151,   204,   289,    -1,    21,   144,   292,    -1,    -1,
      22,   144,   292,   291,   220,    -1,   348,   293,    -1,    -1,
     293,   143,   294,    -1,    61,    -1,   120,   144,   346,    -1,
     364,    -1,    80,   144,   350,    -1,    94,   144,   338,    -1,
      95,    -1,    60,    -1,   103,   144,   338,    -1,   122,    -1,
      59,    -1,   104,    -1,   105,   144,   338,    -1,   106,    -1,
     107,    -1,   339,    -1,    24,   144,   326,   143,   339,    -1,
      26,   144,   339,   143,    63,   143,   331,    -1,    30,   144,
     339,   143,    63,    -1,    30,   144,   339,   143,    63,   143,
       5,   215,    -1,    31,    -1,    31,   144,   360,    -1,    39,
     144,   339,   143,    72,    -1,    40,   144,   339,   143,    72,
      -1,    40,   144,   369,   143,   369,   143,    72,    -1,    44,
     144,   369,   143,   369,   143,   151,    -1,    45,   144,   369,
     143,   369,   305,    -1,    46,   144,   369,   143,   369,    -1,
      -1,   143,    72,    -1,    52,   144,   360,   143,    76,    -1,
      53,   144,   360,    -1,    54,   144,   339,   143,   145,   363,
     143,   363,   146,   143,    74,   143,    73,    -1,    56,   144,
     360,    -1,    55,   144,   360,    -1,     3,    -1,   145,     3,
     143,   332,   146,    -1,    91,   144,   342,   143,   344,   143,
     344,   143,     7,    -1,    89,   144,   360,   143,   344,    -1,
      41,   144,   342,    -1,    42,   144,   342,    -1,    -1,    32,
     144,   342,   143,   332,   143,   327,   328,   317,   318,    -1,
      -1,   220,    -1,    36,   144,   339,   143,   333,   143,   335,
      -1,    37,   144,   339,    -1,    38,   144,    75,   143,   339,
      -1,   121,   144,   339,   143,   350,    -1,   121,   144,   339,
     143,    64,    -1,   121,   144,   339,    -1,    50,   144,   351,
     143,   339,    -1,    51,   144,   339,   143,   368,   143,   350,
      -1,    48,   144,   338,   143,   338,   143,   338,   143,   338,
      -1,    48,    -1,   151,    -1,    64,    -1,   151,    -1,    64,
      -1,    -1,   143,   329,    -1,   330,    -1,   330,   143,   329,
      -1,    33,    -1,    34,    -1,    35,    -1,    59,    -1,    64,
      -1,    60,    -1,    64,    -1,    67,    -1,   336,    -1,    64,
      -1,    67,    -1,   336,    -1,    66,   144,    64,    -1,    71,
      -1,    64,    -1,    66,   147,     4,   148,    -1,   151,    -1,
     162,    -1,   163,   147,   351,   148,    -1,   351,    -1,   340,
      -1,   134,   145,   360,   146,    -1,   166,    -1,   167,   147,
     351,   148,    -1,   145,     4,   143,     4,   146,    -1,    64,
      -1,    65,   341,   148,    -1,   142,    -1,   142,   143,   341,
      -1,   343,    -1,   168,    -1,   169,   147,   351,   148,    -1,
     145,     4,   143,     4,   143,     4,   143,     4,   146,    -1,
     345,    -1,   170,    -1,   171,   147,   351,   148,    -1,     3,
      -1,   145,     3,   143,   332,   146,    -1,   347,    -1,   172,
      -1,   173,   147,   351,   148,    -1,   151,    -1,     3,    -1,
     145,     3,   143,   151,   146,    -1,    64,    -1,   349,    -1,
     174,    -1,   175,   147,   351,   148,    -1,   151,    -1,     3,
      -1,   145,     3,   143,   151,   146,    -1,    64,    -1,    77,
      -1,   337,    -1,   350,   203,   337,    -1,     4,    -1,   361,
      -1,   145,     8,   146,    -1,   160,    -1,   161,   147,   351,
     148,    -1,   351,   197,   351,    -1,   351,   198,   351,    -1,
     351,   199,   351,    -1,   351,   200,   351,    -1,   351,   201,
     351,    -1,   145,   351,   146,    -1,   156,    -1,   157,    -1,
     224,   144,   352,    -1,   353,    -1,   354,   143,   353,    -1,
      -1,   354,    -1,   351,    -1,   350,    -1,   356,    -1,   357,
     143,   356,    -1,    -1,   357,    -1,   339,    -1,   127,   342,
      -1,   128,   342,    -1,   129,   339,   143,   339,    -1,   130,
     339,   143,   339,   143,   351,    -1,   131,   145,   360,   146,
      -1,   131,   145,   279,   143,   360,   146,    -1,   137,   145,
       7,   143,   360,   146,    -1,   137,   145,   360,   143,   360,
     146,    -1,   137,   145,   344,   143,   360,   146,    -1,   133,
     339,    -1,   135,   145,   339,   143,   351,   146,    -1,   135,
     145,   339,   143,   351,   143,    33,   146,    -1,   136,   145,
     339,   143,   351,   143,   351,   146,    -1,   136,   145,   339,
     143,   351,   143,   351,   143,    33,   146,    -1,   139,   145,
     140,   143,   145,   351,   198,   351,   213,   146,   143,   339,
     214,   146,    -1,   138,   359,    -1,   176,    -1,   145,   360,
     146,    -1,   359,    -1,   359,   205,   360,    -1,   196,    -1,
      78,    -1,     4,    -1,     8,    -1,     9,    -1,     4,    -1,
       8,    -1,     9,    -1,     4,    -1,   361,    -1,    27,    -1,
      21,    -1,    28,    -1,    23,    -1,    29,    -1,    89,    -1,
      62,    -1,    64,    -1,   370,    -1,    47,   145,     4,   143,
       4,   143,     4,   143,     4,   146,    -1,   145,     4,   143,
       4,   143,     4,   143,     4,   146,    -1
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
     519,   520,   521,   522,   523,   524,   525,   526,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   540,   541,   542,
     543,   544,   545,   546,   547,   548,   551,   552,   553,   556,
     557,   560,   576,   582,   588,   594,   600,   606,   612,   618,
     624,   634,   644,   654,   664,   674,   684,   696,   701,   708,
     713,   720,   725,   732,   736,   742,   747,   754,   758,   764,
     768,   775,   797,   774,   811,   866,   873,   876,   882,   889,
     893,   902,   906,   901,   969,   970,   974,   973,   987,   986,
    1001,  1011,  1012,  1015,  1053,  1052,  1087,  1086,  1117,  1116,
    1149,  1148,  1174,  1185,  1184,  1212,  1218,  1223,  1228,  1235,
    1242,  1251,  1259,  1271,  1270,  1289,  1288,  1307,  1310,  1316,
    1326,  1332,  1341,  1347,  1352,  1358,  1363,  1369,  1380,  1386,
    1387,  1390,  1391,  1394,  1398,  1404,  1405,  1408,  1415,  1423,
    1431,  1432,  1435,  1436,  1439,  1444,  1443,  1457,  1464,  1471,
    1479,  1484,  1490,  1496,  1502,  1508,  1513,  1518,  1523,  1528,
    1533,  1538,  1543,  1548,  1553,  1558,  1564,  1571,  1580,  1584,
    1597,  1606,  1605,  1623,  1633,  1639,  1647,  1653,  1658,  1663,
    1668,  1673,  1678,  1683,  1688,  1693,  1704,  1710,  1715,  1720,
    1725,  1732,  1738,  1767,  1772,  1780,  1786,  1792,  1799,  1806,
    1816,  1826,  1841,  1852,  1855,  1861,  1867,  1873,  1879,  1885,
    1891,  1896,  1903,  1910,  1916,  1922,  1929,  1928,  1953,  1956,
    1962,  1969,  1975,  1981,  1985,  1990,  1997,  2003,  2010,  2014,
    2021,  2029,  2032,  2042,  2046,  2049,  2055,  2059,  2066,  2070,
    2074,  2080,  2081,  2084,  2085,  2088,  2089,  2090,  2096,  2097,
    2098,  2104,  2105,  2108,  2117,  2122,  2129,  2140,  2146,  2150,
    2154,  2161,  2171,  2178,  2182,  2188,  2192,  2200,  2204,  2211,
    2221,  2234,  2238,  2245,  2255,  2264,  2275,  2279,  2286,  2296,
    2307,  2316,  2326,  2332,  2336,  2343,  2353,  2364,  2373,  2383,
    2387,  2394,  2395,  2401,  2405,  2409,  2413,  2421,  2430,  2434,
    2438,  2442,  2446,  2450,  2453,  2460,  2469,  2502,  2503,  2506,
    2507,  2510,  2514,  2521,  2528,  2539,  2542,  2550,  2554,  2558,
    2562,  2566,  2571,  2575,  2579,  2584,  2589,  2594,  2598,  2603,
    2608,  2612,  2616,  2621,  2625,  2632,  2638,  2642,  2648,  2655,
    2656,  2659,  2660,  2661,  2664,  2668,  2672,  2676,  2682,  2683,
    2686,  2687,  2690,  2691,  2694,  2695,  2698,  2702,  2728
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
  "MON_ALERTNESS", "MON_APPEARANCE", "ROOMDOOR_ID", "IF_ID", "ELSE_ID",
  "TERRAIN_ID", "HORIZ_OR_VERT", "REPLACE_TERRAIN_ID", "EXIT_ID",
  "SHUFFLE_ID", "QUANTITY_ID", "BURIED_ID", "LOOP_ID", "FOR_ID", "TO_ID",
  "SWITCH_ID", "CASE_ID", "BREAK_ID", "DEFAULT_ID", "ERODED_ID",
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
     395,   396,   397,    44,    58,    40,    41,    91,    93,   123,
     125,   398,   399,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,    43,    45,    42,
      47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   206,   207,   207,   208,   208,   209,   210,   210,   211,
     211,   211,   211,   212,   213,   213,   214,   214,   215,   215,
     216,   216,   217,   217,   218,   218,   219,   219,   220,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   224,   224,   224,   225,
     225,   226,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   228,   228,   229,
     229,   230,   230,   231,   231,   232,   232,   233,   233,   234,
     234,   236,   237,   235,   238,   239,   240,   240,   241,   241,
     241,   243,   244,   242,   245,   245,   247,   246,   248,   246,
     249,   250,   250,   251,   253,   252,   255,   254,   257,   256,
     259,   258,   260,   261,   260,   262,   263,   263,   263,   264,
     264,   265,   266,   268,   267,   270,   269,   271,   271,   272,
     272,   273,   273,   274,   274,   275,   275,   276,   276,   277,
     277,   278,   278,   279,   279,   280,   280,   281,   281,   281,
     282,   282,   283,   283,   284,   285,   284,   286,   287,   287,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   289,   289,   289,
     290,   291,   290,   292,   293,   293,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   295,   296,   297,   297,   298,   298,   299,   300,   301,
     302,   303,   304,   305,   305,   306,   307,   308,   309,   310,
     311,   311,   312,   313,   314,   315,   317,   316,   318,   318,
     319,   320,   321,   322,   322,   322,   323,   324,   325,   325,
     326,   326,   327,   327,   328,   328,   329,   329,   330,   330,
     330,   331,   331,   332,   332,   333,   333,   333,   334,   334,
     334,   335,   335,   336,   337,   337,   337,   338,   339,   339,
     339,   339,   340,   340,   340,   341,   341,   342,   342,   342,
     343,   344,   344,   344,   345,   345,   346,   346,   346,   347,
     347,   347,   347,   348,   348,   348,   349,   349,   349,   349,
     349,   350,   350,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   352,   352,   353,   354,   354,   355,
     355,   356,   356,   357,   357,   358,   358,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   360,   360,   361,   362,
     362,   363,   363,   363,   364,   364,   364,   364,   365,   365,
     366,   366,   367,   367,   368,   368,   369,   369,   370
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
       1,     3,     3,     5,     3,     5,     5,     5,     3,     3,
       5,     5,     5,     7,     7,     7,     5,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     0,     0,     8,     4,     1,     0,     1,     1,     5,
       3,     0,     0,     9,     0,     2,     0,     5,     0,     4,
       1,     2,     1,     6,     0,     3,     0,     6,     0,     4,
       0,     4,     1,     0,     4,     3,     1,     3,     3,     5,
       5,     7,     4,     0,    10,     0,    12,     0,     2,     5,
       1,     5,     1,     5,     1,     5,     1,     9,     5,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     7,     5,
       1,     1,     1,     1,     3,     0,     5,     4,     0,     3,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       3,     3,     3,     1,     1,     3,     1,     1,     1,     3,
       3,     0,     5,     2,     0,     3,     1,     3,     1,     3,
       3,     1,     1,     3,     1,     1,     1,     3,     1,     1,
       1,     5,     7,     5,     8,     1,     3,     5,     5,     7,
       7,     6,     5,     0,     2,     5,     3,    13,     3,     3,
       1,     5,     9,     5,     3,     3,     0,    10,     0,     1,
       7,     3,     5,     5,     5,     3,     5,     7,     9,     1,
       1,     1,     1,     1,     0,     2,     1,     3,     1,     1,
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
       5,     0,    26,     0,     7,     0,   138,     0,     0,     0,
       0,   197,     0,     0,     0,     0,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   289,     0,     0,     0,     0,     0,     0,     0,
       0,   166,     0,     0,     0,     0,     0,   135,     0,     0,
       0,   141,   150,     0,     0,     0,     0,    98,    87,    78,
      88,    79,    89,    80,    90,    81,    91,    82,    92,    83,
      93,    84,    94,    85,    95,    86,    30,    31,     6,    26,
      96,    97,     0,    39,    38,    56,    57,    54,     0,    49,
      55,   154,    50,    51,    53,    52,    29,    66,    37,    69,
      68,    41,    59,    61,    62,    76,    42,    60,    77,    58,
      73,    74,    65,    75,    36,    45,    46,    47,    70,    64,
      72,    71,    40,    63,    67,    32,    33,    34,    35,    48,
      43,    44,     0,    25,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   160,     0,     0,     0,     0,    99,   100,     0,     0,
       0,     0,   353,     0,   356,     0,   398,     0,   354,   375,
      27,     0,   158,     0,     8,     0,   314,   315,     0,   351,
     165,     0,     0,     0,    11,   400,   399,    13,   323,     0,
     200,   201,     0,     0,   320,     0,     0,   177,   318,   347,
     349,   350,     0,   346,   344,     0,   230,   234,   343,   231,
     340,   342,     0,   339,   337,     0,   204,     0,   336,   291,
     290,     0,   301,   302,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   394,
     377,   396,   256,     0,   328,     0,     0,   327,     0,   281,
       0,     0,     0,     0,     0,     0,   416,   274,   275,   293,
     292,     0,   136,     0,     0,     0,     0,     0,   317,     0,
       0,     0,     0,     0,   266,     0,   269,   268,   403,   401,
     402,   168,   167,     0,   189,   190,     0,     0,     0,     0,
     101,     0,     0,     0,   285,   131,     0,     0,     0,     0,
     140,     0,     0,     0,     0,     0,   372,   371,   373,   376,
       0,   409,   411,   408,   410,   412,   413,     0,     0,     0,
     108,   109,   104,   102,     0,     0,     0,     0,    26,   155,
      24,     0,     0,     0,     0,     0,   325,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,   379,     0,     0,
       0,   387,     0,     0,     0,   393,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     137,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   162,   161,     0,     0,   156,
       0,     0,     0,   369,   355,   363,     0,     0,   358,   359,
     360,   361,   362,     0,   134,     0,   353,     0,     0,     0,
       0,   125,   123,   129,   127,     0,     0,     0,   159,     0,
       0,   352,    10,   270,     0,     9,     0,     0,   324,     0,
       0,     0,   203,   202,   177,   178,   199,     0,     0,     0,
     232,     0,     0,   206,   208,   251,   188,     0,   253,     0,
       0,   193,     0,     0,     0,     0,   334,     0,     0,   332,
       0,     0,   331,     0,     0,   395,   397,     0,     0,   303,
     304,     0,   307,     0,   305,     0,   306,   282,   257,     0,
       0,   258,     0,   180,     0,     0,     0,     0,     0,   263,
     262,     0,     0,   169,   170,   286,   414,   415,     0,   265,
       0,   182,     0,     0,     0,     0,     0,   273,     0,     0,
     152,     0,     0,   142,   284,   283,     0,   367,   370,     0,
     357,   139,   374,   103,     0,     0,   112,     0,   111,     0,
     110,     0,   116,     0,   107,     0,   106,     0,   105,    28,
     316,     0,     0,   326,   319,     0,   321,     0,     0,   345,
     406,   404,   405,   245,   242,   236,     0,     0,   241,     0,
     246,     0,   248,   249,     0,   244,   235,   250,   407,   238,
       0,   338,   207,     0,     0,   380,     0,     0,     0,   382,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   329,
       0,     0,     0,     0,     0,     0,     0,     0,   172,     0,
       0,     0,   261,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   157,   151,   153,     0,     0,     0,   132,     0,
     124,   126,   128,   130,     0,   117,     0,   119,     0,   121,
       0,     0,   322,   198,   348,     0,     0,     0,     0,     0,
     341,     0,   252,    18,     0,   194,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   294,     0,   312,   311,
     280,     0,     0,   259,     0,   184,     0,     0,   260,   264,
       0,     0,   287,     0,     0,   186,     0,   294,   192,     0,
     191,   164,     0,   144,   364,   365,   366,   368,     0,     0,
     115,     0,   114,     0,   113,     0,     0,   239,   240,   243,
     247,   237,     0,   308,   211,   212,     0,   216,   215,   226,
     217,   218,   219,     0,     0,     0,   223,   224,     0,   209,
     213,   309,   210,     0,   254,   381,   383,     0,   388,     0,
     384,     0,   333,   386,   385,     0,     0,     0,   276,   313,
       0,     0,     0,     0,     0,     0,   195,   196,     0,     0,
       0,     0,   173,     0,     0,     0,     0,     0,   144,   133,
     118,   120,   122,   271,     0,     0,   214,     0,     0,     0,
       0,    19,     0,     0,   335,     0,     0,   298,   299,   300,
     295,   296,   278,     0,     0,   179,     0,   294,   288,   171,
       0,   181,     0,     0,   187,   272,     0,   148,   143,   145,
       0,   310,   220,   221,   222,   228,   227,   225,   389,     0,
     390,   359,     0,     0,   279,   277,     0,     0,     0,   175,
       0,     0,   174,   146,    26,     0,     0,     0,     0,     0,
     330,   297,     0,   418,   183,     0,     0,   185,    26,   149,
       0,   229,   391,    15,     0,   417,   176,     0,   147,     0,
       0,   267,     0,    16,    20,    21,    18,     0,     0,    12,
      17,   392
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    86,    87,   869,   898,   764,
     896,    12,   144,    88,   359,    89,    90,    91,    92,   188,
      93,    94,   664,   666,   668,   447,   448,   449,   450,    95,
     433,   728,    96,    97,   411,    98,    99,   189,   655,   797,
     798,   878,   864,   100,   552,   101,   102,   203,   103,   549,
     104,   357,   105,   317,   426,   545,   106,   107,   108,   300,
     291,   109,   833,   110,   875,   373,   525,   543,   707,   717,
     111,   316,   719,   492,   788,   112,   226,   474,   113,   380,
     246,   612,   759,   847,   114,   377,   236,   376,   606,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   642,
     125,   126,   127,   128,   129,   465,   130,   131,   132,   133,
     134,   822,   855,   135,   136,   137,   138,   139,   140,   141,
     251,   292,   778,   820,   821,   254,   511,   515,   760,   700,
     516,   209,   297,   270,   228,   367,   276,   277,   501,   502,
     247,   248,   237,   238,   336,   298,   726,   557,   558,   559,
     338,   339,   340,   271,   397,   198,   217,   312,   609,   354,
     355,   356,   538,   285,   286
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -693
static const yytype_int16 yypact[] =
{
     250,   -93,   -55,    80,  -693,   250,    16,   -45,   -40,  -693,
    -693,   -11,   630,    19,  -693,    74,  -693,    29,    62,    75,
      89,  -693,    94,   113,   131,   137,   163,   192,   198,   203,
     236,   240,   252,   259,   261,   268,   281,   297,   311,   314,
     315,   316,   348,   349,   353,   354,   360,   362,   363,   367,
     368,   369,   370,   383,    57,   389,   390,  -693,   398,    48,
     759,  -693,  -693,   399,   217,    52,    56,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,   630,
    -693,  -693,    26,  -693,  -693,  -693,  -693,  -693,   400,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,  -693,   391,   325,  -693,   162,   340,    67,   290,    51,
      51,   246,   -37,    20,   226,   226,   681,   -64,   226,   226,
     470,   226,    10,   -64,   -64,   -24,   -19,   -19,   -19,    52,
     406,    52,   226,   681,   681,   226,   681,   681,    87,   -24,
      73,  -693,   681,   -64,   761,    52,  -693,  -693,   359,   415,
     226,   421,  -693,    84,  -693,   420,  -693,   216,  -693,   166,
    -693,    18,  -693,   422,  -693,    74,  -693,  -693,   423,  -693,
     366,   434,   444,   446,  -693,  -693,  -693,  -693,  -693,   449,
    -693,  -693,   447,   589,  -693,   450,   451,   453,  -693,  -693,
    -693,  -693,   595,  -693,  -693,   452,  -693,  -693,  -693,  -693,
    -693,  -693,   597,  -693,  -693,   454,   455,   459,  -693,  -693,
    -693,   462,  -693,  -693,   464,   465,   466,   -64,   -64,   226,
     226,   468,   226,   474,   475,   477,   681,   479,   445,  -693,
    -693,   405,  -693,   624,  -693,   485,   491,  -693,   492,  -693,
     499,   500,   501,   641,   514,   515,  -693,  -693,  -693,  -693,
    -693,   516,   657,   660,   522,   534,   546,   547,   319,   687,
     549,   289,   555,   563,  -693,   564,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,   569,  -693,  -693,   570,   422,   571,   575,
    -693,   576,    52,    52,   582,  -693,   584,   331,    52,    52,
    -693,    52,    52,    52,    52,    52,   366,   319,  -693,   585,
     588,  -693,  -693,  -693,  -693,  -693,  -693,   591,   185,    60,
    -693,  -693,   366,   319,   599,   600,   603,   630,   630,  -693,
    -693,    52,   162,   738,    45,   745,   607,   604,   681,   612,
      52,   307,   752,   606,   631,    52,   633,   422,   635,    52,
     422,   226,   226,   681,   716,   717,  -693,  -693,   638,   639,
     566,  -693,   226,   226,   309,  -693,   645,   640,   681,   644,
      52,    70,   256,   226,   741,   784,   680,   753,   -19,    -9,
    -693,   684,   685,   -19,   -19,   -19,    52,   686,    90,   226,
     191,   748,   688,     3,    20,   742,  -693,    47,    47,  -693,
     201,   683,   -20,   720,  -693,  -693,   272,   302,   168,   168,
    -693,  -693,  -693,   166,  -693,   681,   689,   -57,   -41,   -30,
       4,  -693,  -693,   366,   319,   212,   234,    12,  -693,   691,
     417,  -693,  -693,  -693,   831,  -693,   692,   449,  -693,   690,
     833,   440,  -693,  -693,   453,  -693,  -693,   693,   539,   199,
    -693,   694,   562,  -693,  -693,  -693,  -693,   699,   700,   226,
     226,   642,   706,   704,   708,   709,  -693,   710,   419,  -693,
     707,   712,  -693,   713,   715,  -693,  -693,   855,   567,  -693,
    -693,   719,  -693,   728,  -693,   733,  -693,  -693,  -693,   734,
     856,  -693,   735,  -693,   875,   756,    70,   894,   757,   758,
    -693,   760,   839,  -693,  -693,  -693,  -693,  -693,   762,  -693,
     108,  -693,   900,   764,   765,   821,   907,  -693,   768,   422,
    -693,   711,    52,  -693,  -693,   366,   771,  -693,   769,   770,
    -693,  -693,  -693,  -693,   916,   792,  -693,    -6,  -693,    52,
    -693,   162,  -693,    23,  -693,    65,  -693,    49,  -693,  -693,
    -693,   796,   937,  -693,  -693,   795,  -693,   790,   797,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,   800,   801,  -693,   802,
    -693,   803,  -693,  -693,   804,  -693,  -693,  -693,  -693,  -693,
     805,  -693,   806,    20,   945,  -693,   809,   890,   681,  -693,
      52,    52,   681,   811,    52,   681,   681,   810,   813,  -693,
     -24,   953,   100,   954,   188,   887,   817,     8,  -693,   818,
     812,   892,  -693,    52,   819,   162,   822,   823,    13,   222,
     422,    47,  -693,  -693,   319,   820,   253,   720,  -693,   194,
    -693,  -693,   319,   366,    22,  -693,    93,  -693,   165,  -693,
      70,   824,  -693,  -693,  -693,   162,    52,    52,    52,   246,
    -693,   755,  -693,   825,    52,  -693,   826,   229,   324,   827,
      70,   572,   828,   829,    52,   966,   834,   830,  -693,  -693,
    -693,   836,   967,  -693,   972,  -693,   365,   837,  -693,  -693,
     838,    58,   366,   108,   978,  -693,   979,   834,  -693,   841,
    -693,  -693,   842,   195,  -693,  -693,  -693,  -693,   422,    23,
    -693,    65,  -693,    49,  -693,   840,   982,   366,  -693,  -693,
    -693,  -693,    44,  -693,  -693,  -693,   162,  -693,  -693,  -693,
    -693,  -693,  -693,   844,   845,   846,  -693,  -693,   847,  -693,
    -693,  -693,   366,   989,  -693,   319,  -693,   960,  -693,    52,
    -693,   848,  -693,  -693,  -693,   339,   852,   418,  -693,  -693,
     992,   854,   853,   857,    13,    52,  -693,  -693,   858,   859,
     860,   864,  -693,    58,   991,   108,   865,   851,   195,  -693,
    -693,  -693,  -693,  -693,   867,   938,   366,    52,    52,    52,
     -50,  -693,   862,   265,  -693,    52,   999,  -693,  -693,  -693,
    -693,   868,   422,   869,  1009,  -693,   307,   834,  -693,  -693,
     871,  -693,  1011,   422,  -693,  -693,   872,  -693,  -693,  -693,
    1012,  -693,  -693,  -693,  -693,  -693,   814,  -693,  -693,   986,
    -693,   189,   874,   418,  -693,  -693,  1017,   876,   877,  -693,
     950,   879,  -693,  -693,   630,   883,   -50,   881,   888,   882,
    -693,  -693,   884,  -693,  -693,   422,   889,  -693,   630,  -693,
      70,  -693,  -693,  -693,   891,  -693,  -693,   958,  -693,   893,
     226,  -693,    85,   895,  -693,  -693,   825,   226,   896,  -693,
    -693,  -693
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -693,  -693,  1028,  -693,  -693,  -693,  -693,  -693,  -693,   139,
    -693,  -693,   832,   -89,  -307,   682,   861,   980,  -427,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,   987,  -693,  -693,  -693,   245,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,   626,
     870,  -693,  -693,  -693,  -693,   573,  -693,  -693,  -693,   262,
    -693,  -693,  -693,  -541,   255,  -693,   344,   225,  -693,  -693,
    -693,  -693,  -693,   186,  -693,  -693,   903,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,   424,  -692,   202,  -693,  -413,  -524,  -693,  -693,  -693,
     375,   695,  -182,  -141,  -340,   592,   -98,  -333,  -404,  -558,
    -448,  -533,   605,  -530,  -140,   -61,  -693,   401,  -693,  -693,
     618,  -693,  -693,   798,  -144,   583,  -693,  -417,  -693,  -693,
    -693,  -693,  -693,  -131,  -693
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -206
static const yytype_int16 yytable[] =
{
     200,   534,   638,   321,   197,   210,   556,   227,   576,   451,
     425,   544,   272,   255,   256,   496,   452,   278,   279,   669,
     281,   284,   192,   547,   548,   792,   229,   249,   282,   303,
     304,   302,   306,   307,   305,   294,   295,   296,   318,   341,
     289,   342,   667,   665,   554,   343,   344,   345,   463,   324,
     496,     7,   496,   578,   229,   523,   192,   282,   218,   219,
     350,   352,   786,    16,   192,   287,   288,   541,   240,   845,
     480,   215,   705,   483,   218,   219,   685,   715,   314,   252,
       9,   273,   218,   219,   253,   319,   565,   230,   192,     8,
     894,   308,   326,   566,   308,   309,   310,    11,   309,   310,
     231,   846,   567,   351,   274,   275,    13,   346,   720,   568,
     301,    14,   308,   569,   250,   230,   309,   310,   388,   389,
     570,   391,   787,   646,   218,   219,   293,   290,   231,   241,
     509,   206,   327,    15,   510,   859,   524,   315,   337,   223,
     353,   431,   207,   208,   222,   216,   735,   571,   542,   895,
     347,   311,   222,   706,   572,   283,   143,   546,   716,   386,
     387,   577,   142,   348,   698,   729,   771,   349,   232,   206,
     192,   699,   730,   145,   233,   802,   224,   225,   194,   195,
     207,   208,   499,   500,   224,   225,   274,   275,   805,   446,
     464,   631,   546,   326,   546,   185,   232,   193,   801,   800,
     682,   199,   233,   590,    65,   348,   146,   591,   592,   453,
     242,   206,   194,   195,   196,   229,   243,   499,   500,   147,
     194,   195,   207,   208,   469,   234,   235,   661,   201,   193,
     556,   741,   660,   148,   531,   299,   731,   240,   149,   486,
     484,   485,   652,   732,   194,   195,   493,   722,   196,   240,
     503,   494,   495,   536,   506,   537,   196,   150,   593,   594,
     595,   430,   517,   218,   219,     1,     2,   436,   437,   459,
     438,   439,   440,   441,   442,   151,   230,   522,   535,   596,
     196,   152,   528,   529,   530,   491,   718,   327,   454,   231,
     218,   219,   555,   597,   598,   795,   789,   796,   241,   550,
     460,   563,   599,   600,   601,   602,   603,   153,   733,   471,
     241,   193,   496,   206,   478,   734,   497,   206,   482,   604,
     512,   605,   513,   514,   207,   208,   194,   195,   207,   208,
     193,   702,   868,   222,   672,   -14,   154,   695,   607,   508,
     672,   329,   155,   721,   223,   194,   195,   156,   615,   616,
     211,   212,   213,   214,   218,   219,   889,   232,   220,   221,
     222,   573,   196,   233,   330,   224,   225,   333,   334,   335,
     191,   223,   767,   218,   219,   768,   472,   473,   836,   242,
     157,   196,   337,   575,   158,   243,   234,   235,   333,   334,
     335,   242,   224,   225,   204,   196,   159,   243,   331,   332,
     333,   334,   335,   160,   551,   161,   244,   245,   849,   724,
     725,   850,   162,   331,   332,   333,   334,   335,   244,   245,
     560,   799,   623,   369,   222,   163,   331,   332,   333,   334,
     335,   663,   419,   220,   221,   223,   257,   258,   259,   260,
     261,   164,   262,   222,   263,   264,   265,   266,   267,   369,
     561,   817,   818,   819,   498,   165,   224,   225,   166,   167,
     168,   710,   331,   332,   333,   334,   335,   769,   205,   331,
     332,   333,   334,   335,   686,   224,   225,   435,   689,   499,
     500,   692,   693,   218,   219,   269,   331,   332,   333,   334,
     335,   654,   169,   170,   738,   739,   740,   171,   172,   331,
     332,   333,   334,   335,   173,   712,   174,   175,   662,   218,
     219,   176,   177,   178,   179,   854,   331,   332,   333,   334,
     335,   331,   332,   333,   334,   335,   862,   180,   331,   332,
     333,   334,   335,   182,   183,   737,   331,   815,   333,   334,
     335,   762,   184,   190,   202,   280,   257,   258,   259,   260,
     261,   299,   262,   222,   263,   264,   265,   266,   267,   687,
     688,   322,   323,   691,   268,   580,   325,   328,   886,   362,
     361,   358,   257,   258,   259,   260,   261,   363,   262,   222,
     263,   264,   265,   266,   267,   224,   225,   364,   586,   365,
     268,   366,   368,   369,   371,   269,   372,   370,   374,   375,
     378,   379,   381,   828,  -205,   382,   806,   383,   384,   385,
     398,   224,   225,   390,   331,   332,   333,   334,   335,   392,
     393,   269,   394,   765,   396,   842,   843,   844,   399,   491,
     218,   219,   400,   775,   401,   402,    16,   331,   332,   333,
     334,   335,   403,   404,    17,   406,   405,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,   407,   408,   409,
      28,    29,    30,   410,   412,   413,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,   414,    42,    43,
      44,    45,    46,    47,    48,    49,    50,   589,    51,   415,
     416,   417,   418,   257,   258,   259,   260,   261,   420,   262,
     222,   263,   264,   265,   266,   267,   421,   422,   813,    52,
     611,   268,   423,   424,   427,   629,    53,    54,   428,    55,
     772,    56,    57,    58,   429,   432,    59,    60,   443,    61,
     434,    62,   224,   225,   444,   445,   331,   332,   333,   334,
     335,   462,   269,   455,   456,   218,   219,   457,   466,   893,
     467,    63,   468,    64,   851,   470,   900,   475,   476,   331,
     332,   333,   334,   335,   331,   332,   333,   334,   335,   331,
     332,   333,   334,   335,   477,   879,   479,    65,   481,   487,
     488,   489,   490,    66,    67,   504,   505,   507,   519,   888,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,   257,   258,
     259,   260,   261,   518,   262,   222,   263,   264,   265,   266,
     267,   742,   743,   520,   539,   521,   268,   526,   527,   532,
    -163,   553,   564,   540,   581,   582,   584,   585,   744,   745,
     746,   579,   613,   614,   588,   610,   617,   224,   225,   618,
     619,   620,   621,   622,   624,   625,   626,   269,   627,   628,
     634,   747,   630,   748,   749,   750,   751,   752,   753,   754,
     755,   756,   757,   758,    67,   631,   632,   633,   635,   636,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,   639,   637,
     640,   641,   644,   643,   647,   645,   206,   648,   649,   650,
     623,   651,   657,   186,   653,   656,   658,   207,   208,    68,
     659,    70,    69,    72,    71,    74,    73,    76,    75,    78,
      77,    80,    79,    82,    81,    84,    83,   273,    85,   670,
     671,   672,   673,   674,   675,   676,   677,   678,   679,   681,
     683,   680,   684,   491,   690,   694,   695,   697,   701,   703,
     704,   702,   711,   708,   709,   713,   714,   736,   763,   723,
     776,   781,   766,   770,   773,   774,   782,   777,   779,   780,
     784,   785,   790,   791,   793,   794,   803,   804,   807,   808,
     809,   810,   811,   812,   814,   816,   823,   824,   835,   825,
     826,   838,   841,   852,   829,   830,   831,   832,   848,   837,
     840,   853,   856,   857,   860,   861,   863,   865,   866,   867,
     870,   872,   873,   874,   876,   877,   880,   882,   884,   883,
     885,   891,   887,    10,   890,   899,   892,   360,   897,   458,
     187,   181,   901,   839,   533,   320,   827,   587,   834,   313,
     783,   858,   881,   239,   696,   871,   761,   461,   727,   583,
     574,   562,   608,     0,   395
};

static const yytype_int16 yycheck[] =
{
      89,   418,   526,   185,    65,   145,   433,   148,   456,   349,
     317,   424,   156,   154,   155,     3,   349,   158,   159,   577,
     161,   162,     4,   427,   428,   717,     3,    64,    47,   173,
     174,   172,   176,   177,   175,   166,   167,   168,   182,    21,
      64,    23,   575,   573,    64,    27,    28,    29,     3,   190,
       3,   144,     3,   457,     3,    64,     4,    47,    64,    65,
     201,   201,     4,     6,     4,   163,   164,    64,     3,   119,
     377,     4,    64,   380,    64,    65,   617,    64,     5,    59,
       0,   145,    64,    65,    64,   183,   143,    64,     4,   144,
       5,     4,     8,   150,     4,     8,     9,    81,     8,     9,
      77,   151,   143,   201,   168,   169,   151,    89,   649,   150,
     171,   151,     4,   143,   151,    64,     8,     9,   259,   260,
     150,   262,    64,   540,    64,    65,   145,   151,    77,    64,
      60,   151,   193,   144,    64,   827,   145,    64,   199,   145,
     201,   323,   162,   163,   134,    78,   670,   143,   145,    64,
     132,    64,   134,   145,   150,   145,    82,   145,   145,   257,
     258,   149,   143,   145,    64,   143,   690,   149,   145,   151,
       4,    71,   150,   144,   151,   733,   166,   167,   160,   161,
     162,   163,   170,   171,   166,   167,   168,   169,   144,     4,
     145,   147,   145,     8,   145,   147,   145,   145,   731,   729,
     613,   145,   151,     4,   147,   145,   144,     8,     9,   349,
     145,   151,   160,   161,   196,     3,   151,   170,   171,   144,
     160,   161,   162,   163,   368,   174,   175,   567,   202,   145,
     657,   679,   565,   144,   416,   145,   143,     3,   144,   383,
     381,   382,   549,   150,   160,   161,   390,   651,   196,     3,
     394,   392,   393,    62,   398,    64,   196,   144,    59,    60,
      61,   322,   403,    64,    65,    15,    16,   328,   329,   358,
     331,   332,   333,   334,   335,   144,    64,   408,   419,    80,
     196,   144,   413,   414,   415,    63,    64,   348,   349,    77,
      64,    65,   432,    94,    95,   100,   713,   102,    64,    98,
     361,   445,   103,   104,   105,   106,   107,   144,   143,   370,
      64,   145,     3,   151,   375,   150,     7,   151,   379,   120,
      64,   122,    66,    67,   162,   163,   160,   161,   162,   163,
     145,   143,   143,   134,   146,   146,   144,   143,   479,   400,
     146,   125,   144,   650,   145,   160,   161,   144,   489,   490,
      10,    11,    12,    13,    64,    65,   880,   145,    68,    69,
     134,   149,   196,   151,   148,   166,   167,   199,   200,   201,
     153,   145,   143,    64,    65,   146,    69,    70,   795,   145,
     144,   196,   443,   149,   144,   151,   174,   175,   199,   200,
     201,   145,   166,   167,     3,   196,   144,   151,   197,   198,
     199,   200,   201,   144,   203,   144,   172,   173,   143,   156,
     157,   146,   144,   197,   198,   199,   200,   201,   172,   173,
     148,   728,     3,     4,   134,   144,   197,   198,   199,   200,
     201,   571,   143,    68,    69,   145,   127,   128,   129,   130,
     131,   144,   133,   134,   135,   136,   137,   138,   139,     4,
     148,    33,    34,    35,   145,   144,   166,   167,   144,   144,
     144,   643,   197,   198,   199,   200,   201,   143,   143,   197,
     198,   199,   200,   201,   618,   166,   167,   146,   622,   170,
     171,   625,   626,    64,    65,   176,   197,   198,   199,   200,
     201,   552,   144,   144,   676,   677,   678,   144,   144,   197,
     198,   199,   200,   201,   144,   645,   144,   144,   569,    64,
      65,   144,   144,   144,   144,   822,   197,   198,   199,   200,
     201,   197,   198,   199,   200,   201,   833,   144,   197,   198,
     199,   200,   201,   144,   144,   675,   197,   198,   199,   200,
     201,   681,   144,   144,   144,    75,   127,   128,   129,   130,
     131,   145,   133,   134,   135,   136,   137,   138,   139,   620,
     621,   202,   147,   624,   145,   148,   145,   147,   875,   203,
     147,   149,   127,   128,   129,   130,   131,   143,   133,   134,
     135,   136,   137,   138,   139,   166,   167,   143,   148,   143,
     145,   142,   145,     4,   143,   176,   143,   147,     3,   147,
       3,   147,   143,   785,   149,   143,   746,   143,   143,   143,
     205,   166,   167,   145,   197,   198,   199,   200,   201,   145,
     145,   176,   145,   684,   145,   807,   808,   809,     4,    63,
      64,    65,   147,   694,   143,   143,     6,   197,   198,   199,
     200,   201,   143,   143,    14,     4,   145,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,   143,   143,   143,
      30,    31,    32,     6,     4,   143,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,   143,    48,    49,
      50,    51,    52,    53,    54,    55,    56,   148,    58,   143,
     143,     4,   143,   127,   128,   129,   130,   131,   143,   133,
     134,   135,   136,   137,   138,   139,   143,   143,   769,    79,
     148,   145,   143,   143,   143,   148,    86,    87,   143,    89,
     148,    91,    92,    93,   148,   143,    96,    97,   143,    99,
     146,   101,   166,   167,   146,   144,   197,   198,   199,   200,
     201,     3,   176,   144,   144,    64,    65,   144,     3,   890,
     143,   121,   148,   123,   815,   143,   897,     5,   152,   197,
     198,   199,   200,   201,   197,   198,   199,   200,   201,   197,
     198,   199,   200,   201,   143,   864,   143,   147,   143,    63,
      63,   143,   143,   153,   154,   140,   146,   143,     4,   878,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   127,   128,
     129,   130,   131,    72,   133,   134,   135,   136,   137,   138,
     139,    66,    67,   143,    76,    72,   145,   143,   143,   143,
      88,   148,   143,   145,     3,   143,   146,     4,    83,    84,
      85,   150,   143,   143,   151,   151,   204,   166,   167,   143,
     146,   143,   143,   143,   147,   143,   143,   176,   143,     4,
       4,   106,   143,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   154,   147,   143,   143,   143,     4,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     4,   143,
     143,   143,    63,   143,     4,   143,   151,   143,   143,    88,
       3,   143,   143,   154,   203,   144,   146,   162,   163,   160,
       4,   162,   161,   164,   163,   166,   165,   168,   167,   170,
     169,   172,   171,   174,   173,   176,   175,   145,   177,   143,
       3,   146,   152,   146,   144,   144,   144,   144,   144,   143,
       5,   146,   143,    63,   143,   145,   143,     4,     4,    72,
     143,   143,   143,   151,    72,   143,   143,   143,   143,   149,
       4,     4,   146,   146,   146,   146,     4,   143,   148,   143,
     143,   143,     4,     4,   143,   143,   146,     5,   144,   144,
     144,   144,     3,    33,   146,   143,     4,   143,     7,   146,
     143,   150,    64,     4,   146,   146,   146,   143,   146,   144,
     143,   143,   143,     4,   143,     4,   144,     5,   204,    33,
     146,     4,   146,   146,    74,   146,   143,   146,   146,   141,
     146,    73,   143,     5,   143,   896,   143,   205,   143,   357,
      60,    54,   146,   798,   418,   184,   784,   474,   793,   179,
     706,   826,   866,   150,   630,   853,   681,   362,   657,   467,
     455,   443,   479,    -1,   266
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   207,   208,   209,   210,   144,   144,     0,
     208,    81,   217,   151,   151,   144,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    30,    31,
      32,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    58,    79,    86,    87,    89,    91,    92,    93,    96,
      97,    99,   101,   121,   123,   147,   153,   154,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   211,   212,   219,   221,
     222,   223,   224,   226,   227,   235,   238,   239,   241,   242,
     249,   251,   252,   254,   256,   258,   262,   263,   264,   267,
     269,   276,   281,   284,   290,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   306,   307,   308,   309,   310,
     312,   313,   314,   315,   316,   319,   320,   321,   322,   323,
     324,   325,   143,    82,   218,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   241,   144,   144,   144,   147,   154,   223,   225,   243,
     144,   153,     4,   145,   160,   161,   196,   351,   361,   145,
     219,   202,   144,   253,     3,   143,   151,   162,   163,   337,
     350,    10,    11,    12,    13,     4,    78,   362,    64,    65,
      68,    69,   134,   145,   166,   167,   282,   339,   340,     3,
      64,    77,   145,   151,   174,   175,   292,   348,   349,   292,
       3,    64,   145,   151,   172,   173,   286,   346,   347,    64,
     151,   326,    59,    64,   331,   339,   339,   127,   128,   129,
     130,   131,   133,   135,   136,   137,   138,   139,   145,   176,
     339,   359,   360,   145,   168,   169,   342,   343,   339,   339,
      75,   339,    47,   145,   339,   369,   370,   342,   342,    64,
     151,   266,   327,   145,   369,   369,   369,   338,   351,   145,
     265,   351,   339,   360,   360,   339,   360,   360,     4,     8,
       9,    64,   363,   266,     5,    64,   277,   259,   360,   342,
     222,   338,   202,   147,   339,   145,     8,   351,   147,   125,
     148,   197,   198,   199,   200,   201,   350,   351,   356,   357,
     358,    21,    23,    27,    28,    29,    89,   132,   145,   149,
     339,   342,   350,   351,   365,   366,   367,   257,   149,   220,
     218,   147,   203,   143,   143,   143,   142,   341,   145,     4,
     147,   143,   143,   271,     3,   147,   293,   291,     3,   147,
     285,   143,   143,   143,   143,   143,   342,   342,   339,   339,
     145,   339,   145,   145,   145,   359,   145,   360,   205,     4,
     147,   143,   143,   143,   143,   145,     4,   143,   143,   143,
       6,   240,     4,   143,   143,   143,   143,     4,   143,   143,
     143,   143,   143,   143,   143,   220,   260,   143,   143,   148,
     351,   338,   143,   236,   146,   146,   351,   351,   351,   351,
     351,   351,   351,   143,   146,   144,     4,   231,   232,   233,
     234,   340,   343,   350,   351,   144,   144,   144,   221,   219,
     351,   337,     3,     3,   145,   311,     3,   143,   148,   360,
     143,   351,    69,    70,   283,     5,   152,   143,   351,   143,
     220,   143,   351,   220,   339,   339,   360,    63,    63,   143,
     143,    63,   279,   360,   339,   339,     3,     7,   145,   170,
     171,   344,   345,   360,   140,   146,   360,   143,   351,    60,
      64,   332,    64,    66,    67,   333,   336,   339,    72,     4,
     143,    72,   369,    64,   145,   272,   143,   143,   369,   369,
     369,   338,   143,   265,   363,   339,    62,    64,   368,    76,
     145,    64,   145,   273,   331,   261,   145,   344,   344,   255,
      98,   203,   250,   148,    64,   350,   224,   353,   354,   355,
     148,   148,   356,   360,   143,   143,   150,   143,   150,   143,
     150,   143,   150,   149,   348,   149,   346,   149,   344,   150,
     148,     3,   143,   341,   146,     4,   148,   271,   151,   148,
       4,     8,     9,    59,    60,    61,    80,    94,    95,   103,
     104,   105,   106,   107,   120,   122,   294,   339,   361,   364,
     151,   148,   287,   143,   143,   339,   339,   204,   143,   146,
     143,   143,   143,     3,   147,   143,   143,   143,     4,   148,
     143,   147,   143,   143,     4,   143,     4,   143,   332,     4,
     143,   143,   305,   143,    63,   143,   363,     4,   143,   143,
      88,   143,   220,   203,   351,   244,   144,   143,   146,     4,
     343,   340,   351,   350,   228,   349,   229,   347,   230,   345,
     143,     3,   146,   152,   146,   144,   144,   144,   144,   144,
     146,   143,   331,     5,   143,   279,   360,   351,   351,   360,
     143,   351,   360,   360,   145,   143,   327,     4,    64,    71,
     335,     4,   143,    72,   143,    64,   145,   274,   151,    72,
     338,   143,   350,   143,   143,    64,   145,   275,    64,   278,
     279,   220,   344,   149,   156,   157,   352,   353,   237,   143,
     150,   143,   150,   143,   150,   332,   143,   350,   338,   338,
     338,   346,    66,    67,    83,    84,    85,   106,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   288,
     334,   336,   350,   143,   215,   351,   146,   143,   146,   143,
     146,   332,   148,   146,   146,   351,     4,   143,   328,   148,
     143,     4,     4,   282,   143,   143,     4,    64,   280,   363,
       4,     4,   328,   143,   143,   100,   102,   245,   246,   220,
     349,   347,   345,   146,     5,   144,   350,   144,   144,   144,
     144,     3,    33,   351,   146,   198,   143,    33,    34,    35,
     329,   330,   317,     4,   143,   146,   143,   275,   338,   146,
     146,   146,   143,   268,   280,     7,   363,   144,   150,   245,
     143,    64,   338,   338,   338,   119,   151,   289,   146,   143,
     146,   351,     4,   143,   220,   318,   143,     4,   283,   328,
     143,     4,   220,   144,   248,     5,   204,    33,   143,   213,
     146,   329,     4,   146,   146,   270,    74,   146,   247,   219,
     143,   289,   146,   141,   146,   146,   220,   143,   219,   332,
     143,    73,   143,   339,     5,    64,   216,   143,   214,   215,
     339,   146
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

  case 101:

/* Line 1455 of yacc.c  */
#line 561 "lev_comp.y"
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

  case 102:

/* Line 1455 of yacc.c  */
#line 577 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 583 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 589 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 595 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 601 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 607 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 613 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 619 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 625 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 635 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 645 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 655 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 665 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 675 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 685 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 697 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 702 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 709 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 714 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 721 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 726 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 733 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 737 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 743 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 748 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 755 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 759 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 765 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 769 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 775 "lev_comp.y"
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

  case 132:

/* Line 1455 of yacc.c  */
#line 797 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 801 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 812 "lev_comp.y"
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

  case 135:

/* Line 1455 of yacc.c  */
#line 867 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 873 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 877 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 883 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 890 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 894 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 902 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 906 "lev_comp.y"
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

  case 143:

/* Line 1455 of yacc.c  */
#line 929 "lev_comp.y"
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

  case 146:

/* Line 1455 of yacc.c  */
#line 974 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 984 "lev_comp.y"
    {
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 987 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 997 "lev_comp.y"
    {
		  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 1002 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1016 "lev_comp.y"
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

  case 154:

/* Line 1455 of yacc.c  */
#line 1053 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 1058 "lev_comp.y"
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

  case 156:

/* Line 1455 of yacc.c  */
#line 1087 "lev_comp.y"
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

  case 157:

/* Line 1455 of yacc.c  */
#line 1101 "lev_comp.y"
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

  case 158:

/* Line 1455 of yacc.c  */
#line 1117 "lev_comp.y"
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

  case 159:

/* Line 1455 of yacc.c  */
#line 1137 "lev_comp.y"
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
#line 1149 "lev_comp.y"
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

  case 161:

/* Line 1455 of yacc.c  */
#line 1169 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1175 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1185 "lev_comp.y"
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

  case 164:

/* Line 1455 of yacc.c  */
#line 1203 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1213 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1219 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1224 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1229 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1236 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1243 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1252 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1260 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1271 "lev_comp.y"
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

  case 174:

/* Line 1455 of yacc.c  */
#line 1282 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1289 "lev_comp.y"
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

  case 176:

/* Line 1455 of yacc.c  */
#line 1300 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1307 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1311 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1317 "lev_comp.y"
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

  case 180:

/* Line 1455 of yacc.c  */
#line 1327 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1333 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1342 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1348 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1353 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1359 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1364 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1370 "lev_comp.y"
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

  case 188:

/* Line 1455 of yacc.c  */
#line 1381 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (5)].i), SPO_DOOR));
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1395 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1399 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1409 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1416 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1424 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1440 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1444 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1450 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1458 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1464 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1472 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1480 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1485 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1491 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1497 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1503 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1509 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1514 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1519 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1524 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1529 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1534 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1539 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1544 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1549 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1554 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1559 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1565 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1572 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1581 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1585 "lev_comp.y"
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

  case 230:

/* Line 1455 of yacc.c  */
#line 1598 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1606 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1616 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1624 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1633 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1640 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1648 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1654 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1659 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1664 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1669 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1674 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1679 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1684 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1689 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1694 "lev_comp.y"
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

  case 246:

/* Line 1455 of yacc.c  */
#line 1705 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1711 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1716 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1721 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1726 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1733 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1739 "lev_comp.y"
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

  case 253:

/* Line 1455 of yacc.c  */
#line 1768 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1773 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1781 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1787 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1793 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1800 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1807 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1817 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1827 "lev_comp.y"
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

  case 262:

/* Line 1455 of yacc.c  */
#line 1842 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1852 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1856 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1862 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1868 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1874 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int)(yyvsp[(6) - (13)].i), (int)(yyvsp[(8) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1880 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1886 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1892 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1897 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1904 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1911 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1917 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1923 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1929 "lev_comp.y"
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

  case 277:

/* Line 1455 of yacc.c  */
#line 1943 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      if ( (yyvsp[(9) - (10)].i) ) {
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      } else if ( (yyvsp[(10) - (10)].i) )
			  lc_error("Cannot use lev statements in non-permanent REGION");
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1953 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1957 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1963 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1970 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1976 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1982 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1986 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1991 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1998 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 2004 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 2011 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 2015 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 2022 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 2033 "lev_comp.y"
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

  case 294:

/* Line 1455 of yacc.c  */
#line 2046 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2050 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2056 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2060 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2067 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2071 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2075 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2091 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2099 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2109 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2118 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2123 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2130 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2141 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2147 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2151 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2155 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2162 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2172 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2179 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2183 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2189 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2193 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2201 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2205 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2212 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2222 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2235 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2239 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2246 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2256 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2265 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2276 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2280 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2287 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2297 "lev_comp.y"
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

  case 340:

/* Line 1455 of yacc.c  */
#line 2308 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2317 "lev_comp.y"
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

  case 342:

/* Line 1455 of yacc.c  */
#line 2327 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2333 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2337 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2344 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2354 "lev_comp.y"
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

  case 347:

/* Line 1455 of yacc.c  */
#line 2365 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2374 "lev_comp.y"
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

  case 349:

/* Line 1455 of yacc.c  */
#line 2384 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2388 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2394 "lev_comp.y"
    { ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2396 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2402 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2406 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2410 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2414 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2422 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2431 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2435 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2439 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2443 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2447 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2450 "lev_comp.y"
    { ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2454 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2461 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2470 "lev_comp.y"
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

  case 371:

/* Line 1455 of yacc.c  */
#line 2511 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2515 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2522 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2529 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2539 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2543 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2551 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2555 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2559 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2563 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2567 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2572 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2576 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2580 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2585 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2590 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2595 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2599 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2604 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2609 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2613 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2617 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2622 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2626 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2633 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2639 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2643 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2649 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2665 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2669 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2673 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2677 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2699 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2703 "lev_comp.y"
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

  case 418:

/* Line 1455 of yacc.c  */
#line 2729 "lev_comp.y"
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
#line 6149 "lev_comp.tab.c"
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
#line 2757 "lev_comp.y"


/*lev_comp.y*/

