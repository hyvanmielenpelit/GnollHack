
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
     KEY_TYPE = 323,
     KEYTYPE_ID = 324,
     DIRECTION = 325,
     RANDOM_TYPE = 326,
     RANDOM_TYPE_BRACKET = 327,
     A_REGISTER = 328,
     ALIGNMENT = 329,
     LEFT_OR_RIGHT = 330,
     CENTER = 331,
     TOP_OR_BOT = 332,
     ALTAR_TYPE = 333,
     UP_OR_DOWN = 334,
     ACTIVE_OR_INACTIVE = 335,
     MODRON_PORTAL_TYPE = 336,
     NPC_TYPE = 337,
     FOUNTAIN_TYPE = 338,
     SPECIAL_OBJECT_TYPE = 339,
     CMAP_TYPE = 340,
     FLOOR_TYPE = 341,
     FLOOR_TYPE_ID = 342,
     FLOOR_ID = 343,
     FLOOR_MAIN_TYPE = 344,
     ELEMENTAL_ENCHANTMENT_TYPE = 345,
     EXCEPTIONALITY_TYPE = 346,
     EXCEPTIONALITY_ID = 347,
     ELEMENTAL_ENCHANTMENT_ID = 348,
     ENCHANTMENT_ID = 349,
     CHARGES_ID = 350,
     SPECIAL_QUALITY_ID = 351,
     SPEFLAGS_ID = 352,
     SUBROOM_ID = 353,
     NAME_ID = 354,
     FLAGS_ID = 355,
     FLAG_TYPE = 356,
     MON_ATTITUDE = 357,
     MON_ALERTNESS = 358,
     SUBTYPE_ID = 359,
     MON_APPEARANCE = 360,
     ROOMDOOR_ID = 361,
     IF_ID = 362,
     ELSE_ID = 363,
     TERRAIN_ID = 364,
     HORIZ_OR_VERT = 365,
     REPLACE_TERRAIN_ID = 366,
     EXIT_ID = 367,
     SHUFFLE_ID = 368,
     QUANTITY_ID = 369,
     BURIED_ID = 370,
     LOOP_ID = 371,
     FOR_ID = 372,
     TO_ID = 373,
     SWITCH_ID = 374,
     CASE_ID = 375,
     BREAK_ID = 376,
     DEFAULT_ID = 377,
     ERODED_ID = 378,
     TRAPPED_STATE = 379,
     RECHARGED_ID = 380,
     INVIS_ID = 381,
     GREASED_ID = 382,
     FEMALE_ID = 383,
     WAITFORU_ID = 384,
     CANCELLED_ID = 385,
     REVIVED_ID = 386,
     AVENGE_ID = 387,
     FLEEING_ID = 388,
     BLINDED_ID = 389,
     PARALYZED_ID = 390,
     STUNNED_ID = 391,
     CONFUSED_ID = 392,
     SEENTRAPS_ID = 393,
     ALL_ID = 394,
     MONTYPE_ID = 395,
     GRAVE_ID = 396,
     ERODEPROOF_ID = 397,
     FUNCTION_ID = 398,
     MSG_OUTPUT_TYPE = 399,
     COMPARE_TYPE = 400,
     UNKNOWN_TYPE = 401,
     rect_ID = 402,
     fillrect_ID = 403,
     line_ID = 404,
     randline_ID = 405,
     grow_ID = 406,
     selection_ID = 407,
     flood_ID = 408,
     rndcoord_ID = 409,
     circle_ID = 410,
     ellipse_ID = 411,
     filter_ID = 412,
     complement_ID = 413,
     gradient_ID = 414,
     GRADIENT_TYPE = 415,
     LIMITED = 416,
     HUMIDITY_TYPE = 417,
     STRING = 418,
     MAP_ID = 419,
     NQSTRING = 420,
     VARSTRING = 421,
     CFUNC = 422,
     CFUNC_INT = 423,
     CFUNC_STR = 424,
     CFUNC_COORD = 425,
     CFUNC_REGION = 426,
     VARSTRING_INT = 427,
     VARSTRING_INT_ARRAY = 428,
     VARSTRING_STRING = 429,
     VARSTRING_STRING_ARRAY = 430,
     VARSTRING_VAR = 431,
     VARSTRING_VAR_ARRAY = 432,
     VARSTRING_COORD = 433,
     VARSTRING_COORD_ARRAY = 434,
     VARSTRING_REGION = 435,
     VARSTRING_REGION_ARRAY = 436,
     VARSTRING_MAPCHAR = 437,
     VARSTRING_MAPCHAR_ARRAY = 438,
     VARSTRING_MONST = 439,
     VARSTRING_MONST_ARRAY = 440,
     VARSTRING_OBJ = 441,
     VARSTRING_OBJ_ARRAY = 442,
     VARSTRING_SEL = 443,
     VARSTRING_SEL_ARRAY = 444,
     METHOD_INT = 445,
     METHOD_INT_ARRAY = 446,
     METHOD_STRING = 447,
     METHOD_STRING_ARRAY = 448,
     METHOD_VAR = 449,
     METHOD_VAR_ARRAY = 450,
     METHOD_COORD = 451,
     METHOD_COORD_ARRAY = 452,
     METHOD_REGION = 453,
     METHOD_REGION_ARRAY = 454,
     METHOD_MAPCHAR = 455,
     METHOD_MAPCHAR_ARRAY = 456,
     METHOD_MONST = 457,
     METHOD_MONST_ARRAY = 458,
     METHOD_OBJ = 459,
     METHOD_OBJ_ARRAY = 460,
     METHOD_SEL = 461,
     METHOD_SEL_ARRAY = 462,
     DICE = 463
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
#line 506 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 518 "lev_comp.tab.c"

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
#define YYLAST   1128

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  226
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  172
/* YYNRULES -- Number of rules.  */
#define YYNRULES  441
/* YYNRULES -- Number of states.  */
#define YYNSTATES  974

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   463

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   221,   225,     2,
     165,   166,   219,   217,   163,   218,   223,   220,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   164,     2,
       2,   222,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   167,     2,   168,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   169,   224,   170,     2,     2,     2,     2,
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
     155,   156,   157,   158,   159,   160,   161,   162,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216
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
     589,   595,   597,   607,   621,   627,   637,   639,   641,   643,
     645,   647,   651,   653,   655,   657,   665,   671,   673,   675,
     677,   679,   683,   684,   690,   695,   696,   700,   702,   704,
     706,   708,   711,   713,   715,   717,   719,   721,   725,   729,
     733,   735,   737,   741,   743,   745,   747,   751,   755,   756,
     762,   765,   766,   770,   772,   776,   778,   782,   786,   788,
     790,   794,   796,   798,   800,   804,   806,   808,   810,   814,
     818,   822,   826,   830,   834,   838,   844,   852,   858,   867,
     869,   873,   879,   885,   893,   901,   908,   914,   915,   918,
     924,   928,   942,   946,   950,   952,   958,   968,   974,   978,
     982,   988,   998,  1004,  1010,  1011,  1023,  1024,  1026,  1034,
    1038,  1046,  1052,  1058,  1064,  1070,  1074,  1080,  1088,  1098,
    1100,  1102,  1104,  1106,  1108,  1109,  1112,  1114,  1118,  1120,
    1122,  1124,  1125,  1129,  1131,  1133,  1135,  1137,  1139,  1141,
    1143,  1145,  1147,  1149,  1153,  1155,  1157,  1162,  1164,  1166,
    1171,  1173,  1175,  1180,  1182,  1187,  1193,  1195,  1199,  1201,
    1205,  1207,  1209,  1214,  1224,  1226,  1228,  1233,  1235,  1241,
    1243,  1245,  1250,  1252,  1254,  1260,  1262,  1264,  1266,  1271,
    1273,  1275,  1281,  1283,  1285,  1287,  1291,  1293,  1295,  1299,
    1301,  1306,  1310,  1314,  1318,  1322,  1326,  1330,  1332,  1334,
    1338,  1340,  1344,  1345,  1347,  1349,  1351,  1353,  1357,  1358,
    1360,  1362,  1365,  1368,  1373,  1380,  1385,  1392,  1399,  1406,
    1413,  1416,  1423,  1432,  1441,  1452,  1467,  1470,  1472,  1476,
    1478,  1482,  1484,  1486,  1488,  1490,  1492,  1494,  1496,  1498,
    1500,  1502,  1504,  1506,  1508,  1510,  1512,  1514,  1516,  1518,
    1520,  1531
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     227,     0,    -1,    -1,   228,    -1,   229,    -1,   229,   228,
      -1,   230,   237,   239,    -1,    16,   164,   171,    -1,    15,
     164,   171,   163,     3,    -1,    17,   164,    11,   163,   331,
      -1,    17,   164,    10,   163,     3,    -1,    17,   164,    13,
      -1,    17,   164,    12,   163,     3,   163,     3,   163,     5,
     163,     5,   163,   359,   163,   236,   235,    -1,    18,   164,
     389,    -1,    -1,   163,   161,    -1,    -1,   163,   366,    -1,
      -1,   163,     3,    -1,     5,    -1,    71,    -1,    -1,   100,
     164,   238,    -1,   101,   163,   238,    -1,   101,    -1,    -1,
     241,   239,    -1,   169,   239,   170,    -1,   282,    -1,   231,
      -1,   232,    -1,   345,    -1,   344,    -1,   342,    -1,   343,
      -1,   346,    -1,   347,    -1,   324,    -1,   284,    -1,   247,
      -1,   246,    -1,   334,    -1,   296,    -1,   316,    -1,   349,
      -1,   350,    -1,   326,    -1,   327,    -1,   328,    -1,   348,
      -1,   262,    -1,   272,    -1,   274,    -1,   278,    -1,   276,
      -1,   259,    -1,   269,    -1,   255,    -1,   258,    -1,   319,
      -1,   301,    -1,   317,    -1,   304,    -1,   310,    -1,   335,
      -1,   330,    -1,   322,    -1,   283,    -1,   336,    -1,   337,
      -1,   338,    -1,   339,    -1,   289,    -1,   287,    -1,   329,
      -1,   333,    -1,   332,    -1,   320,    -1,   321,    -1,   323,
      -1,   315,    -1,   318,    -1,   181,    -1,   183,    -1,   185,
      -1,   187,    -1,   189,    -1,   191,    -1,   193,    -1,   195,
      -1,   197,    -1,   180,    -1,   182,    -1,   184,    -1,   186,
      -1,   188,    -1,   190,    -1,   192,    -1,   194,    -1,   196,
      -1,   242,    -1,   243,    -1,   174,    -1,   174,    -1,   243,
      -1,   113,   164,   242,    -1,   244,   222,   378,    -1,   244,
     222,   152,   164,   387,    -1,   244,   222,   377,    -1,   244,
     222,   394,   164,   371,    -1,   244,   222,   393,   164,   373,
      -1,   244,   222,   392,   164,   375,    -1,   244,   222,   366,
      -1,   244,   222,   369,    -1,   244,   222,   169,   253,   170,
      -1,   244,   222,   169,   252,   170,    -1,   244,   222,   169,
     251,   170,    -1,   244,   222,   394,   164,   169,   250,   170,
      -1,   244,   222,   393,   164,   169,   249,   170,    -1,   244,
     222,   392,   164,   169,   248,   170,    -1,   244,   222,   169,
     254,   170,    -1,   376,    -1,   248,   163,   376,    -1,   374,
      -1,   249,   163,   374,    -1,   372,    -1,   250,   163,   372,
      -1,   370,    -1,   251,   163,   370,    -1,   367,    -1,   252,
     163,   367,    -1,   378,    -1,   253,   163,   378,    -1,   377,
      -1,   254,   163,   377,    -1,    -1,    -1,   143,   173,   165,
     256,   382,   166,   257,   240,    -1,   173,   165,   385,   166,
      -1,   112,    -1,    -1,     6,    -1,     6,    -1,   167,   378,
     145,   378,   168,    -1,   167,   378,   168,    -1,    -1,    -1,
     119,   263,   167,   365,   168,   264,   169,   265,   170,    -1,
      -1,   266,   265,    -1,    -1,   120,   390,   164,   267,   239,
      -1,    -1,   122,   164,   268,   239,    -1,   121,    -1,   223,
     223,    -1,   118,    -1,   117,   245,   222,   378,   270,   378,
      -1,    -1,   271,   273,   240,    -1,    -1,   116,   167,   365,
     168,   275,   240,    -1,    -1,   261,   164,   277,   241,    -1,
      -1,   107,   261,   279,   280,    -1,   240,    -1,    -1,   240,
     281,   108,   240,    -1,    14,   164,   377,    -1,    63,    -1,
      63,   164,   390,    -1,    63,   164,    71,    -1,    54,   164,
     285,   163,   285,    -1,    54,   164,   285,   163,   390,    -1,
     165,     4,   163,    70,   163,   300,   166,    -1,   352,   260,
     163,   359,    -1,    -1,    98,   164,   286,   163,   293,   163,
     295,   353,   356,   288,   240,    -1,    -1,    48,   164,   286,
     163,   292,   163,   294,   163,   295,   353,   356,   290,   240,
      -1,    -1,   163,     5,    -1,   165,     4,   163,     4,   166,
      -1,    71,    -1,   165,     4,   163,     4,   166,    -1,    71,
      -1,   165,   302,   163,   303,   166,    -1,    71,    -1,   165,
       4,   163,     4,   166,    -1,    71,    -1,   106,   164,   297,
     163,   358,   163,   298,   163,   300,    -1,   106,   164,   297,
     163,   358,   163,   298,   163,   300,   163,    68,   163,     4,
      -1,    25,   164,   358,   163,   387,    -1,    25,   164,   358,
     163,   387,   163,    68,   163,     4,    -1,     5,    -1,    71,
      -1,   299,    -1,    71,    -1,    70,    -1,    70,   224,   299,
      -1,     4,    -1,    71,    -1,    20,    -1,    19,   164,   302,
     163,   303,   291,   172,    -1,    19,   164,   366,   291,   172,
      -1,    75,    -1,    76,    -1,    77,    -1,    76,    -1,    23,
     164,   306,    -1,    -1,    23,   164,   306,   305,   240,    -1,
     373,   163,   366,   307,    -1,    -1,   307,   163,   308,    -1,
     377,    -1,   102,    -1,   103,    -1,   361,    -1,   105,   377,
      -1,   128,    -1,   126,    -1,   130,    -1,   131,    -1,   132,
      -1,   133,   164,   365,    -1,   134,   164,   365,    -1,   135,
     164,   365,    -1,   136,    -1,   137,    -1,   138,   164,   309,
      -1,   129,    -1,   171,    -1,   139,    -1,   171,   224,   309,
      -1,    21,   164,   312,    -1,    -1,    22,   164,   312,   311,
     240,    -1,   375,   313,    -1,    -1,   313,   163,   314,    -1,
      66,    -1,   140,   164,   373,    -1,   391,    -1,    99,   164,
     377,    -1,   114,   164,   365,    -1,   115,    -1,    65,    -1,
     123,   164,   365,    -1,   142,    -1,    64,    -1,   124,    -1,
     125,   164,   365,    -1,   126,    -1,   127,    -1,   366,    -1,
      93,   164,    90,    -1,    92,   164,    91,    -1,    94,   164,
     365,    -1,    95,   164,   365,    -1,    96,   164,   365,    -1,
      97,   164,   365,    -1,    69,   164,    68,    -1,    24,   164,
     351,   163,   366,    -1,    26,   164,   366,   163,    70,   163,
     358,    -1,    30,   164,   366,   163,    70,    -1,    30,   164,
     366,   163,    70,   163,     5,   235,    -1,    31,    -1,    31,
     164,   387,    -1,    44,   164,   366,   163,    79,    -1,    45,
     164,   366,   163,    79,    -1,    45,   164,   396,   163,   396,
     163,    79,    -1,    49,   164,   396,   163,   396,   163,   171,
      -1,    50,   164,   396,   163,   396,   325,    -1,    51,   164,
     396,   163,   396,    -1,    -1,   163,    79,    -1,    57,   164,
     387,   163,    83,    -1,    58,   164,   387,    -1,    59,   164,
     366,   163,   165,   390,   163,   390,   166,   163,    81,   163,
      80,    -1,    61,   164,   387,    -1,    60,   164,   387,    -1,
       3,    -1,   165,     3,   163,   359,   166,    -1,   111,   164,
     369,   163,   371,   163,   371,   163,     7,    -1,   109,   164,
     387,   163,   371,    -1,    46,   164,   369,    -1,    47,   164,
     369,    -1,    36,   164,   171,   163,    37,    -1,    36,   164,
     171,   163,    37,   163,   140,   164,   373,    -1,    33,   164,
     369,   163,    35,    -1,    34,   164,   396,   163,    35,    -1,
      -1,    32,   164,   369,   163,   359,   163,   352,   353,   356,
     340,   341,    -1,    -1,   240,    -1,    41,   164,   366,   163,
     360,   163,   362,    -1,    42,   164,   366,    -1,    88,   164,
     387,   163,    89,   163,    86,    -1,   104,   164,   387,   163,
       4,    -1,    43,   164,    82,   163,   366,    -1,   141,   164,
     366,   163,   377,    -1,   141,   164,   366,   163,    71,    -1,
     141,   164,   366,    -1,    55,   164,   378,   163,   366,    -1,
      56,   164,   366,   163,   395,   163,   377,    -1,    53,   164,
     365,   163,   365,   163,   365,   163,   365,    -1,    53,    -1,
     171,    -1,    71,    -1,   171,    -1,    71,    -1,    -1,   163,
     354,    -1,   355,    -1,   355,   163,   354,    -1,    38,    -1,
      39,    -1,    40,    -1,    -1,    87,   164,   357,    -1,    86,
      -1,    64,    -1,    71,    -1,    65,    -1,    71,    -1,    74,
      -1,   363,    -1,    71,    -1,    74,    -1,   363,    -1,    73,
     164,    71,    -1,    78,    -1,    71,    -1,    73,   167,     4,
     168,    -1,   171,    -1,   182,    -1,   183,   167,   378,   168,
      -1,   378,    -1,   367,    -1,   154,   165,   387,   166,    -1,
     186,    -1,   187,   167,   378,   168,    -1,   165,     4,   163,
       4,   166,    -1,    71,    -1,    72,   368,   168,    -1,   162,
      -1,   162,   163,   368,    -1,   370,    -1,   188,    -1,   189,
     167,   378,   168,    -1,   165,     4,   163,     4,   163,     4,
     163,     4,   166,    -1,   372,    -1,   190,    -1,   191,   167,
     378,   168,    -1,     3,    -1,   165,     3,   163,   359,   166,
      -1,   374,    -1,   192,    -1,   193,   167,   378,   168,    -1,
     171,    -1,     3,    -1,   165,     3,   163,   171,   166,    -1,
      71,    -1,   376,    -1,   194,    -1,   195,   167,   378,   168,
      -1,   171,    -1,     3,    -1,   165,     3,   163,   171,   166,
      -1,    71,    -1,    84,    -1,   364,    -1,   377,   223,   364,
      -1,     4,    -1,   388,    -1,   165,     8,   166,    -1,   180,
      -1,   181,   167,   378,   168,    -1,   378,   217,   378,    -1,
     378,   218,   378,    -1,   378,   219,   378,    -1,   378,   220,
     378,    -1,   378,   221,   378,    -1,   165,   378,   166,    -1,
     176,    -1,   177,    -1,   244,   164,   379,    -1,   380,    -1,
     381,   163,   380,    -1,    -1,   381,    -1,   378,    -1,   377,
      -1,   383,    -1,   384,   163,   383,    -1,    -1,   384,    -1,
     366,    -1,   147,   369,    -1,   148,   369,    -1,   149,   366,
     163,   366,    -1,   150,   366,   163,   366,   163,   378,    -1,
     151,   165,   387,   166,    -1,   151,   165,   299,   163,   387,
     166,    -1,   157,   165,     7,   163,   387,   166,    -1,   157,
     165,   387,   163,   387,   166,    -1,   157,   165,   371,   163,
     387,   166,    -1,   153,   366,    -1,   155,   165,   366,   163,
     378,   166,    -1,   155,   165,   366,   163,   378,   163,    38,
     166,    -1,   156,   165,   366,   163,   378,   163,   378,   166,
      -1,   156,   165,   366,   163,   378,   163,   378,   163,    38,
     166,    -1,   159,   165,   160,   163,   165,   378,   218,   378,
     233,   166,   163,   366,   234,   166,    -1,   158,   386,    -1,
     196,    -1,   165,   387,   166,    -1,   386,    -1,   386,   225,
     387,    -1,   216,    -1,    85,    -1,     4,    -1,     8,    -1,
       9,    -1,     4,    -1,     8,    -1,     9,    -1,     4,    -1,
     388,    -1,    27,    -1,    21,    -1,    28,    -1,    23,    -1,
      29,    -1,   109,    -1,    67,    -1,    71,    -1,   397,    -1,
      52,   165,     4,   163,     4,   163,     4,   163,     4,   166,
      -1,   165,     4,   163,     4,   163,     4,   163,     4,   166,
      -1
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
    1365,  1370,  1376,  1387,  1398,  1402,  1408,  1409,  1412,  1413,
    1416,  1420,  1426,  1427,  1430,  1437,  1445,  1453,  1454,  1457,
    1458,  1461,  1466,  1465,  1479,  1486,  1493,  1501,  1506,  1512,
    1518,  1524,  1530,  1535,  1540,  1545,  1550,  1555,  1560,  1565,
    1570,  1575,  1580,  1586,  1593,  1602,  1606,  1619,  1628,  1627,
    1645,  1655,  1661,  1669,  1675,  1680,  1685,  1690,  1695,  1700,
    1705,  1710,  1715,  1726,  1732,  1737,  1742,  1747,  1752,  1757,
    1762,  1767,  1772,  1777,  1782,  1789,  1795,  1824,  1829,  1837,
    1843,  1849,  1856,  1863,  1873,  1883,  1898,  1909,  1912,  1918,
    1924,  1930,  1936,  1942,  1948,  1953,  1960,  1967,  1973,  1979,
    1985,  1989,  1995,  2001,  2012,  2011,  2036,  2039,  2045,  2052,
    2058,  2064,  2070,  2076,  2080,  2085,  2092,  2098,  2105,  2109,
    2116,  2124,  2127,  2137,  2141,  2144,  2150,  2154,  2161,  2165,
    2169,  2176,  2179,  2185,  2191,  2192,  2195,  2196,  2199,  2200,
    2201,  2207,  2208,  2209,  2215,  2216,  2219,  2228,  2233,  2240,
    2251,  2257,  2261,  2265,  2272,  2282,  2289,  2293,  2299,  2303,
    2311,  2315,  2322,  2332,  2345,  2349,  2356,  2366,  2375,  2386,
    2390,  2397,  2407,  2418,  2427,  2437,  2443,  2447,  2454,  2464,
    2475,  2484,  2494,  2498,  2505,  2506,  2512,  2516,  2520,  2524,
    2532,  2541,  2545,  2549,  2553,  2557,  2561,  2564,  2571,  2580,
    2613,  2614,  2617,  2618,  2621,  2625,  2632,  2639,  2650,  2653,
    2661,  2665,  2669,  2673,  2677,  2682,  2686,  2690,  2695,  2700,
    2705,  2709,  2714,  2719,  2723,  2727,  2732,  2736,  2743,  2749,
    2753,  2759,  2766,  2767,  2770,  2771,  2772,  2775,  2779,  2783,
    2787,  2793,  2794,  2797,  2798,  2801,  2802,  2805,  2806,  2809,
    2813,  2839
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
  "DOOR_STATE", "LIGHT_STATE", "CURSE_TYPE", "ENGRAVING_TYPE", "KEY_TYPE",
  "KEYTYPE_ID", "DIRECTION", "RANDOM_TYPE", "RANDOM_TYPE_BRACKET",
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
     415,   416,   417,    44,    58,    40,    41,    91,    93,   123,
     125,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,    43,    45,    42,
      47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   226,   227,   227,   228,   228,   229,   230,   230,   231,
     231,   231,   231,   232,   233,   233,   234,   234,   235,   235,
     236,   236,   237,   237,   238,   238,   239,   239,   240,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   244,   244,   244,   245,   245,   246,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   248,   248,   249,   249,   250,   250,   251,   251,
     252,   252,   253,   253,   254,   254,   256,   257,   255,   258,
     259,   260,   260,   261,   261,   261,   263,   264,   262,   265,
     265,   267,   266,   268,   266,   269,   270,   270,   271,   273,
     272,   275,   274,   277,   276,   279,   278,   280,   281,   280,
     282,   283,   283,   283,   284,   284,   285,   286,   288,   287,
     290,   289,   291,   291,   292,   292,   293,   293,   294,   294,
     295,   295,   296,   296,   296,   296,   297,   297,   298,   298,
     299,   299,   300,   300,   301,   301,   301,   302,   302,   303,
     303,   304,   305,   304,   306,   307,   307,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   309,   309,   309,   310,   311,   310,
     312,   313,   313,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   315,   316,   317,   317,   318,
     318,   319,   320,   321,   322,   323,   324,   325,   325,   326,
     327,   328,   329,   330,   331,   331,   332,   333,   334,   335,
     336,   336,   337,   338,   340,   339,   341,   341,   342,   343,
     344,   345,   346,   347,   347,   347,   348,   349,   350,   350,
     351,   351,   352,   352,   353,   353,   354,   354,   355,   355,
     355,   356,   356,   357,   358,   358,   359,   359,   360,   360,
     360,   361,   361,   361,   362,   362,   363,   364,   364,   364,
     365,   366,   366,   366,   366,   367,   367,   367,   368,   368,
     369,   369,   369,   370,   371,   371,   371,   372,   372,   373,
     373,   373,   374,   374,   374,   374,   375,   375,   375,   376,
     376,   376,   376,   376,   377,   377,   378,   378,   378,   378,
     378,   378,   378,   378,   378,   378,   378,   379,   379,   380,
     381,   381,   382,   382,   383,   383,   384,   384,   385,   385,
     386,   386,   386,   386,   386,   386,   386,   386,   386,   386,
     386,   386,   386,   386,   386,   386,   386,   386,   386,   387,
     387,   388,   389,   389,   390,   390,   390,   391,   391,   391,
     391,   392,   392,   393,   393,   394,   394,   395,   395,   396,
     396,   397
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
       5,     1,     9,    13,     5,     9,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     7,     5,     1,     1,     1,
       1,     3,     0,     5,     4,     0,     3,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     3,     3,     3,
       1,     1,     3,     1,     1,     1,     3,     3,     0,     5,
       2,     0,     3,     1,     3,     1,     3,     3,     1,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     5,     7,     5,     8,     1,
       3,     5,     5,     7,     7,     6,     5,     0,     2,     5,
       3,    13,     3,     3,     1,     5,     9,     5,     3,     3,
       5,     9,     5,     5,     0,    11,     0,     1,     7,     3,
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
       5,     0,    26,     0,     7,     0,   143,     0,     0,     0,
       0,   204,     0,     0,     0,     0,     0,     0,     0,   269,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   309,     0,     0,     0,     0,
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
       0,   104,   105,     0,     0,     0,     0,   376,     0,   379,
       0,   421,     0,   377,   398,    27,     0,   163,     0,     8,
       0,   337,   338,     0,   374,   170,     0,     0,     0,    11,
     423,   422,    13,   346,     0,   207,   208,     0,     0,   343,
       0,     0,   182,   341,   370,   372,   373,     0,   369,   367,
       0,   237,   241,   366,   238,   363,   365,     0,   362,   360,
       0,   211,     0,   359,   311,   310,     0,   324,   325,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   417,   400,   419,   270,     0,   351,
       0,     0,   350,     0,     0,     0,     0,   439,     0,     0,
     299,     0,     0,     0,     0,     0,   288,   289,   313,   312,
       0,   141,     0,     0,     0,     0,   340,     0,     0,     0,
       0,     0,   280,     0,   283,   282,   426,   424,   425,   173,
     172,     0,     0,     0,   196,   197,     0,     0,     0,     0,
     106,     0,     0,     0,   305,   136,     0,     0,     0,     0,
     145,     0,     0,     0,     0,     0,   395,   394,   396,   399,
       0,   432,   434,   431,   433,   435,   436,     0,     0,     0,
     113,   114,   109,   107,     0,     0,     0,     0,    26,   160,
      24,     0,     0,     0,     0,     0,   348,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   240,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   401,   402,     0,     0,
       0,   410,     0,     0,     0,   416,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   142,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     167,   166,     0,     0,   161,     0,     0,     0,   392,   378,
     386,     0,     0,   381,   382,   383,   384,   385,     0,   139,
       0,   376,     0,     0,     0,     0,   130,   128,   134,   132,
       0,     0,     0,   164,     0,     0,   375,    10,   284,     0,
       9,     0,     0,   347,     0,     0,     0,   210,   209,   182,
     183,   206,     0,     0,     0,   239,     0,     0,   213,   215,
     265,   194,     0,   267,     0,     0,   200,     0,     0,     0,
       0,   357,     0,     0,   355,     0,     0,   354,     0,     0,
     418,   420,     0,     0,   326,   327,     0,   292,     0,     0,
     293,   290,   330,     0,   328,     0,   329,   302,   271,     0,
     272,     0,   185,     0,     0,     0,     0,   277,   276,     0,
       0,   174,   175,   306,   437,   438,     0,   279,     0,     0,
     187,     0,     0,   301,     0,     0,     0,   287,     0,     0,
     157,     0,     0,   147,   304,   303,     0,   390,   393,     0,
     380,   144,   397,   108,     0,     0,   117,     0,   116,     0,
     115,     0,   121,     0,   112,     0,   111,     0,   110,    28,
     339,     0,     0,   349,   342,     0,   344,     0,     0,   368,
     429,   427,   428,   252,   249,   243,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   248,     0,   253,     0,   255,
     256,     0,   251,   242,   257,   430,   245,     0,   361,   214,
       0,     0,     0,   403,     0,     0,     0,   405,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   352,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   177,     0,
       0,   275,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   162,   156,   158,     0,     0,     0,   137,     0,
     129,   131,   133,   135,     0,   122,     0,   124,     0,   126,
       0,     0,   345,   205,   371,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   364,     0,     0,
     266,    18,     0,   201,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   314,     0,     0,     0,     0,   335,
     334,   298,   273,     0,   189,     0,     0,   274,   278,     0,
       0,   307,     0,   300,     0,   191,     0,   314,   199,     0,
     198,   169,     0,   149,   387,   388,   389,   391,     0,     0,
     120,     0,   119,     0,   118,     0,     0,   264,   259,   258,
     260,   261,   262,   263,   246,   247,   250,   254,   244,     0,
     331,   218,   219,     0,   223,   222,   233,   224,   225,   226,
       0,     0,     0,   230,   231,     0,   216,   220,   332,   217,
       0,     0,   268,   404,   406,     0,   411,     0,   407,     0,
     356,   409,   408,     0,     0,     0,   321,     0,     0,     0,
     336,     0,     0,     0,     0,   202,   203,     0,     0,     0,
       0,   321,     0,     0,     0,     0,     0,   149,   138,   123,
     125,   127,   285,     0,     0,   221,     0,     0,     0,     0,
     195,    19,     0,     0,   358,     0,     0,   318,   319,   320,
     315,   316,     0,   294,     0,     0,   291,   184,     0,   314,
     308,   176,     0,   186,     0,   178,   192,   286,     0,   153,
     148,   150,     0,   333,   227,   228,   229,   235,   234,   232,
     412,     0,   413,   382,     0,     0,     0,   296,     0,     0,
       0,   321,     0,     0,     0,     0,   151,    26,     0,     0,
       0,     0,     0,   353,   317,   323,   322,   297,   295,     0,
     441,   188,   180,     0,   190,   179,     0,    26,   154,     0,
     236,   414,    15,     0,   440,     0,     0,     0,   152,     0,
       0,   181,   281,   193,     0,    16,    20,    21,    18,     0,
       0,    12,    17,   415
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    91,    92,   932,   970,   822,
     968,    12,   154,    93,   379,    94,    95,    96,    97,   203,
      98,    99,   704,   706,   708,   472,   473,   474,   475,   100,
     458,   778,   101,   102,   435,   103,   104,   204,   695,   856,
     857,   947,   927,   105,   582,   106,   107,   218,   108,   579,
     109,   377,   110,   337,   451,   575,   111,   112,   113,   318,
     310,   114,   924,   115,   955,   393,   554,   572,   756,   767,
     116,   336,   769,   517,   847,   117,   241,   499,   118,   400,
     261,   649,   816,   909,   119,   397,   251,   396,   643,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   681,
     130,   131,   132,   133,   134,   490,   135,   136,   137,   138,
     139,   140,   141,   142,   917,   938,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   266,   311,   836,   880,   881,
     883,   936,   269,   536,   545,   817,   751,   546,   224,   315,
     285,   243,   387,   291,   292,   526,   527,   262,   263,   252,
     253,   356,   316,   776,   587,   588,   589,   358,   359,   360,
     286,   417,   213,   232,   330,   646,   374,   375,   376,   566,
     296,   297
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -814
static const yytype_int16 yypact[] =
{
     160,   -38,   -36,    69,  -814,   160,    41,   -21,     9,  -814,
    -814,   -18,   674,    -1,  -814,   108,  -814,    56,    76,    89,
     140,  -814,   147,   152,   171,   175,   188,   205,   207,   215,
     218,   220,   231,   244,   247,   248,   256,   263,   268,   269,
     276,   294,   315,   323,   324,   325,   330,   331,   339,   345,
     348,   352,   356,   358,   360,   361,   364,   373,   375,    24,
     381,   395,  -814,   396,    22,   636,  -814,  -814,   417,    57,
      36,    49,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,   674,  -814,  -814,    42,  -814,  -814,
    -814,  -814,  -814,   421,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -814,   321,   266,  -814,   -95,   563,    50,   265,   302,
     302,   161,   -46,     7,   -35,   -35,   725,   -75,   -75,    -9,
      95,   -35,   -35,   286,   -35,   227,   -75,   -75,   -39,    -9,
      -9,    -9,    36,   301,    36,   -35,   725,   725,   -35,   725,
     725,    88,   725,   -39,   725,   100,  -814,   725,   -75,   735,
      36,  -814,  -814,   219,   341,   -35,   326,  -814,    25,  -814,
     411,  -814,   217,  -814,    54,  -814,   274,  -814,   422,  -814,
     108,  -814,  -814,   423,  -814,   363,   425,   432,   433,  -814,
    -814,  -814,  -814,  -814,   431,  -814,  -814,   435,   622,  -814,
     460,   465,   466,  -814,  -814,  -814,  -814,   645,  -814,  -814,
     482,  -814,  -814,  -814,  -814,  -814,  -814,   647,  -814,  -814,
     486,   485,   503,  -814,  -814,  -814,   504,  -814,  -814,   505,
     506,   507,   -75,   -75,   -35,   -35,   510,   -35,   511,   517,
     518,   725,   519,   455,  -814,  -814,   461,  -814,   681,  -814,
     520,   526,  -814,   527,   536,   698,   540,  -814,   546,   548,
    -814,   549,   550,   710,   573,   575,  -814,  -814,  -814,  -814,
     576,   720,   577,   578,   579,   580,   398,   740,   582,   170,
     583,   584,  -814,   585,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,   591,   600,   601,  -814,  -814,   605,   422,   606,   607,
    -814,   597,    36,    36,   608,  -814,   609,   185,    36,    36,
    -814,    36,    36,    36,    36,    36,   363,   398,  -814,   610,
     611,  -814,  -814,  -814,  -814,  -814,  -814,   612,    66,    12,
    -814,  -814,   363,   398,   615,   618,   620,   674,   674,  -814,
    -814,    36,   -95,   771,    19,   789,   631,   632,   725,   638,
      36,   200,   794,   630,   641,    36,   642,   422,   643,    36,
     422,   -35,   -35,   725,   737,   738,  -814,  -814,   646,   648,
     602,  -814,   -35,   -35,   393,  -814,   653,   655,   725,   656,
      36,     8,   788,   821,   664,   796,   792,    20,   -35,   755,
     672,   757,    -9,   -32,  -814,   675,    -9,    -9,    -9,    36,
     676,    59,   -35,    60,   754,   677,   751,   -25,   839,     7,
     736,  -814,    17,    17,  -814,   157,   678,    40,   713,  -814,
    -814,  -117,   336,   123,   123,  -814,  -814,  -814,    54,  -814,
     725,   682,   -64,   -54,   -48,   -27,  -814,  -814,   363,   398,
     277,   201,   131,  -814,   679,   349,  -814,  -814,  -814,   847,
    -814,   688,   431,  -814,   686,   873,   404,  -814,  -814,   466,
    -814,  -814,   717,   414,    73,  -814,   718,   419,  -814,  -814,
    -814,   728,   729,   750,   -35,   -35,   695,   760,   759,   764,
     766,  -814,   768,   327,  -814,   767,   770,  -814,   772,   773,
    -814,  -814,   933,   426,  -814,  -814,   776,  -814,   790,   947,
    -814,   791,  -814,   785,  -814,   793,  -814,  -814,  -814,   951,
    -814,   795,  -814,   953,   797,     8,   798,   799,  -814,   800,
     889,  -814,  -814,  -814,  -814,  -814,   801,  -814,   306,   802,
    -814,   962,   804,  -814,   805,   861,   967,  -814,   808,   422,
    -814,   749,    36,  -814,  -814,   363,   809,  -814,   811,   810,
    -814,  -814,  -814,  -814,   971,   813,  -814,    14,  -814,    36,
    -814,   -95,  -814,    77,  -814,    92,  -814,    47,  -814,  -814,
    -814,   814,   976,  -814,  -814,   815,  -814,   816,   818,  -814,
    -814,  -814,  -814,  -814,  -814,  -814,   822,   823,   825,   826,
     827,   828,   829,   832,   833,  -814,   834,  -814,   835,  -814,
    -814,   836,  -814,  -814,  -814,  -814,  -814,   819,  -814,   817,
     914,     7,   996,  -814,   840,   932,   725,  -814,    36,    36,
     725,   841,    36,   725,   725,   842,   843,  -814,   -39,  1001,
     845,   869,  1006,   103,    63,   934,   848,   -16,  -814,   844,
     935,  -814,    36,   849,   -95,   853,   931,   855,    -5,   257,
     422,    17,  -814,  -814,   398,   850,   173,   713,  -814,   120,
    -814,  -814,   398,   363,    15,  -814,    98,  -814,   104,  -814,
       8,   857,  -814,  -814,  -814,   954,   930,   936,    36,    36,
      36,    36,   -95,    36,    36,    36,   161,  -814,   812,   860,
    -814,   862,    36,  -814,   858,   281,   204,   863,     8,   439,
     864,   865,    36,  1023,   870,   871,  1024,   868,   867,  -814,
    -814,  -814,  -814,  1032,  -814,   280,   874,  -814,  -814,   875,
      53,   363,   306,  -814,  1035,  -814,  1036,   870,  -814,   878,
    -814,  -814,   879,   -10,  -814,  -814,  -814,  -814,   422,    77,
    -814,    92,  -814,    47,  -814,   877,  1039,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,   363,  -814,  -814,  -814,  -814,   145,
    -814,  -814,  -814,   -95,  -814,  -814,  -814,  -814,  -814,  -814,
     881,   882,   883,  -814,  -814,   884,  -814,  -814,  -814,   363,
    1045,  1047,  -814,   398,  -814,  1013,  -814,    36,  -814,   886,
    -814,  -814,  -814,   444,   890,   430,   968,  1050,   893,   161,
    -814,   891,   895,    -5,    36,  -814,  -814,   894,   896,   897,
     898,   968,    53,  1052,   306,   900,   899,   -10,  -814,  -814,
    -814,  -814,  -814,   902,   995,   363,    36,    36,    36,   -50,
    -814,  -814,   901,   344,  -814,    36,  1064,  -814,  -814,  -814,
    -814,   907,   908,  -814,   910,  1067,  -814,  -814,   200,   870,
    -814,  -814,   911,  -814,  1071,  -814,   913,  -814,   915,  -814,
    -814,  -814,  1072,  -814,  -814,  -814,  -814,  -814,   854,  -814,
    -814,  1042,  -814,   197,   916,   430,   997,   422,  1077,   918,
     919,   968,  1005,   921,   422,  1020,  -814,   674,   926,   -50,
     924,   937,   925,  -814,  -814,  -814,  -814,  -814,  -814,   927,
    -814,  -814,  -814,   929,  -814,  -814,   938,   674,  -814,     8,
    -814,  -814,  -814,   939,  -814,   422,  1014,  1091,  -814,   940,
     -35,  -814,  -814,  -814,   102,   941,  -814,  -814,   862,   -35,
     942,  -814,  -814,  -814
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -814,  -814,  1092,  -814,  -814,  -814,  -814,  -814,  -814,   128,
    -814,  -814,   880,   -94,  -325,   722,   906,  1041,  -448,  -814,
    -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,  -814,  1048,  -814,  -814,  -814,   252,
    -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,   669,
     920,  -814,  -814,  -814,  -814,   613,  -814,  -814,  -814,   271,
    -814,  -814,  -814,  -591,   259,  -814,   362,   228,  -814,  -814,
    -814,  -814,  -814,   186,  -814,  -814,   958,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,  -814,  -814,   451,  -732,   206,  -814,
    -813,  -814,  -430,  -552,  -814,  -814,  -814,   392,   741,  -189,
    -157,  -364,   633,   233,  -356,  -429,  -581,  -475,  -578,   644,
    -569,  -151,   -61,  -814,   429,  -814,  -814,   654,  -814,  -814,
     846,  -145,   624,  -814,  -439,  -814,  -814,  -814,  -814,  -814,
    -119,  -814
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -213
static const yytype_int16 yytable[] =
{
     215,   242,   562,   678,   225,   476,   606,   270,   271,   212,
     586,   341,   450,   477,   299,   300,   207,   302,   304,   574,
     521,   287,   488,   577,   578,   264,   709,   707,   320,   207,
      16,   323,   308,   346,   705,   851,   233,   234,   895,   552,
     207,   321,   322,   294,   324,   325,   570,   331,   344,   333,
     521,   590,   338,   608,   230,   754,   305,   845,   207,   370,
     312,   313,   314,   326,   733,   372,   765,   327,   328,     9,
     471,   267,   505,   534,   346,   508,   221,   620,   268,   535,
     244,   621,   622,   233,   234,   233,   234,   222,   223,   907,
     288,   542,   326,   543,   544,   255,   327,   328,   770,   595,
     351,   352,   353,   354,   355,   334,   596,   966,   942,   597,
     854,   584,   855,   289,   290,   599,   598,   408,   409,   237,
     411,   908,   600,   319,   846,   265,     7,   564,     8,   685,
     238,   565,   309,   553,   521,   231,   601,   623,   624,   625,
     571,    11,   626,   602,   233,   234,    15,   347,   245,   755,
      13,   239,   240,   357,   456,   373,   295,   921,   785,   329,
     766,   246,   152,   256,   255,   627,   628,   629,   630,   631,
     632,   335,   633,   967,   749,     1,     2,   368,   779,   238,
      14,   750,   576,   221,   489,   780,   829,   634,   635,   200,
     208,    70,   209,   210,   222,   223,   636,   637,   638,   639,
     640,   208,   861,   860,   255,   209,   210,   524,   525,   153,
     859,   221,   576,   641,   214,   642,   209,   210,   478,   208,
     155,   730,   222,   223,   317,   221,   746,   237,   211,   712,
     206,   208,   256,   701,   209,   210,   222,   223,   238,   700,
     156,   211,   247,   494,   509,   510,   209,   210,   248,   586,
     559,   798,   211,   157,   692,   519,   520,   257,   511,   239,
     240,   781,   772,   258,   216,   518,   298,   783,   782,   528,
     211,   547,   256,   531,   784,   580,   497,   498,   207,   294,
     244,   455,   211,   743,   484,   563,   712,   461,   462,   211,
     463,   464,   465,   466,   467,   361,   576,   362,   233,   234,
     607,   363,   364,   365,   158,   244,   585,   347,   479,   864,
     326,   159,   672,   551,   327,   328,   160,   556,   557,   558,
     485,   524,   525,   848,   219,   593,   257,   516,   768,   496,
     661,   389,   258,   442,   503,   161,   233,   234,   507,   162,
     235,   236,   353,   354,   355,   233,   234,   644,   245,   774,
     775,   460,   163,   259,   260,   235,   236,   653,   654,   533,
     931,   246,   349,   -14,   886,   771,   257,   827,   301,   164,
     605,   165,   258,   245,   351,   352,   353,   354,   355,   166,
     581,   237,   167,   366,   168,   350,   246,   351,   352,   353,
     354,   355,   303,   259,   260,   169,   521,   959,   233,   234,
     522,   293,   351,   352,   353,   354,   355,   357,   170,   306,
     307,   171,   172,   239,   240,   898,   353,   354,   355,   237,
     173,   351,   352,   353,   354,   355,   367,   174,   237,   220,
     238,   339,   175,   176,   351,   352,   353,   354,   355,   368,
     177,   342,   247,   369,   825,   221,   603,   826,   248,   371,
     703,   239,   240,   858,   209,   210,   222,   223,   178,   389,
     239,   240,   289,   290,   233,   234,   317,   247,   877,   878,
     879,   249,   250,   248,   272,   273,   274,   275,   276,   179,
     277,   237,   278,   279,   280,   281,   282,   180,   181,   182,
     211,   345,   283,   759,   183,   184,   249,   250,   351,   352,
     353,   354,   355,   185,   591,   406,   407,   911,   343,   186,
     912,   734,   187,   239,   240,   737,   188,   610,   740,   741,
     189,   694,   190,   284,   191,   192,   233,   234,   193,   790,
     791,   792,   793,   761,   795,   796,   797,   194,   702,   195,
     272,   273,   274,   275,   276,   197,   277,   237,   278,   279,
     280,   281,   282,   351,   352,   353,   354,   355,   523,   198,
     199,   351,   352,   353,   354,   355,   351,   352,   353,   354,
     355,   794,   616,   226,   227,   228,   229,   819,   348,   239,
     240,   205,   619,   524,   525,   217,   382,   648,   383,   284,
     381,   378,   937,   386,   667,   384,   385,   735,   736,   945,
     388,   739,   272,   273,   274,   275,   276,   830,   277,   237,
     278,   279,   280,   281,   282,   351,   352,   353,   354,   355,
     283,   351,   352,   353,   354,   355,   389,   390,   391,   392,
     961,   351,   352,   353,   354,   355,   351,   352,   353,   354,
     355,   239,   240,   351,   352,   353,   354,   355,   394,   395,
     398,   284,   865,   399,  -212,   890,   351,   352,   353,   354,
     355,   351,   875,   353,   354,   355,   401,   402,   403,   404,
     405,   823,   516,   233,   234,   410,   412,   904,   905,   906,
      16,   833,   413,   414,   416,   419,   418,   420,    17,   421,
     422,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,   423,   424,   425,    28,    29,    30,    31,    32,   426,
      33,   427,   428,   429,   430,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,   434,    45,    46,    47,
      48,    49,    50,    51,    52,    53,   431,    54,   432,   433,
     436,   437,   438,   439,   440,   441,   443,   444,   445,   272,
     273,   274,   275,   276,   446,   277,   237,   278,   279,   280,
     281,   282,    55,   447,   448,   454,   873,   283,   449,   452,
     453,   457,    56,   468,   487,   459,   470,   469,    57,   480,
      58,    59,   481,    60,   482,    61,    62,    63,   239,   240,
      64,    65,   491,    66,   492,    67,   233,   234,   284,   500,
     493,   495,   501,   965,   502,   504,   506,   512,   513,   514,
     201,   515,   972,   529,   913,    68,    73,    69,    75,   532,
      77,   530,    79,   537,    81,   538,    83,   539,    85,   541,
      87,   540,    89,   948,   548,   549,   550,   567,   555,   560,
     569,    70,   568,   573,  -168,   594,   583,    71,    72,   609,
     611,   612,   614,   958,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,   272,   273,   274,   275,   276,   615,   277,   237,
     278,   279,   280,   281,   282,   799,   800,    72,   618,   647,
     283,   650,   651,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,   239,   240,   652,   801,   802,    74,   803,    76,   655,
      78,   284,    80,   656,    82,   657,    84,   658,    86,   659,
      88,   660,    90,   663,   662,   664,   665,   666,   804,   668,
     805,   806,   807,   808,   809,   810,   811,   812,   813,   814,
     815,   670,   672,   669,   671,   674,   673,   676,   675,   683,
     677,   679,   680,   682,   684,   686,   687,   688,   689,   690,
     661,   691,   693,   696,   697,   699,   698,   710,   288,   711,
     728,   712,   729,   221,   714,   727,   715,   716,   713,   717,
     718,   719,   720,   721,   222,   223,   722,   723,   724,   725,
     726,   731,   516,   732,   738,   745,   743,   742,   746,   747,
     748,   753,   760,   752,   758,   757,   762,   763,   764,   773,
     786,   788,   787,   820,   824,   821,   789,   834,   838,   828,
     831,   832,   839,   835,   837,   840,   841,   843,   844,   849,
     850,   852,   853,   862,   863,   866,   867,   868,   869,   870,
     871,   872,   874,   876,   884,   882,   885,   887,   888,   897,
     891,   894,   892,   893,   899,   902,   903,   910,   914,   900,
     915,   919,   916,   918,   922,   923,   925,   928,   929,   926,
     930,   939,   933,   935,   940,   941,   943,   944,   946,   949,
     951,   953,   956,   954,   962,   963,   971,    10,   952,   483,
     380,   957,   960,   964,   969,   340,   202,   196,   973,   901,
     561,   896,   617,   332,   889,   950,   920,   842,   254,   744,
     818,   934,   592,   486,   604,   613,   777,   415,   645
};

static const yytype_uint16 yycheck[] =
{
      94,   158,   441,   555,   155,   369,   481,   164,   165,    70,
     458,   200,   337,   369,   171,   172,     4,   174,   175,   449,
       3,   166,     3,   452,   453,    71,   607,   605,   185,     4,
       6,   188,    71,     8,   603,   767,    71,    72,   851,    71,
       4,   186,   187,    52,   189,   190,    71,   192,   205,   194,
       3,   168,   197,   482,     4,    71,   175,     4,     4,   216,
     179,   180,   181,     4,   655,   216,    71,     8,     9,     0,
       4,    64,   397,    65,     8,   400,   171,     4,    71,    71,
       3,     8,     9,    71,    72,    71,    72,   182,   183,   139,
     165,    71,     4,    73,    74,     3,     8,     9,   689,   163,
     217,   218,   219,   220,   221,     5,   170,     5,   921,   163,
     120,    71,   122,   188,   189,   163,   170,   274,   275,   154,
     277,   171,   170,   184,    71,   171,   164,    67,   164,   568,
     165,    71,   171,   165,     3,    85,   163,    64,    65,    66,
     165,   100,    69,   170,    71,    72,   164,   208,    71,   165,
     171,   186,   187,   214,   343,   216,   165,   889,   710,    71,
     165,    84,   163,    71,     3,    92,    93,    94,    95,    96,
      97,    71,    99,    71,    71,    15,    16,   165,   163,   165,
     171,    78,   165,   171,   165,   170,   738,   114,   115,   167,
     165,   167,   180,   181,   182,   183,   123,   124,   125,   126,
     127,   165,   783,   781,     3,   180,   181,   190,   191,   101,
     779,   171,   165,   140,   165,   142,   180,   181,   369,   165,
     164,   651,   182,   183,   165,   171,   163,   154,   216,   166,
     173,   165,    71,   597,   180,   181,   182,   183,   165,   595,
     164,   216,   165,   388,   401,   402,   180,   181,   171,   697,
     439,   726,   216,   164,   579,   412,   413,   165,   403,   186,
     187,   163,   691,   171,   222,   410,   171,   163,   170,   414,
     216,   428,    71,   418,   170,   118,    76,    77,     4,    52,
       3,   342,   216,   163,   378,   442,   166,   348,   349,   216,
     351,   352,   353,   354,   355,    21,   165,    23,    71,    72,
     169,    27,    28,    29,   164,     3,   457,   368,   369,   164,
       4,   164,   167,   432,     8,     9,   164,   436,   437,   438,
     381,   190,   191,   762,     3,   470,   165,    70,    71,   390,
       3,     4,   171,   163,   395,   164,    71,    72,   399,   164,
      75,    76,   219,   220,   221,    71,    72,   504,    71,   176,
     177,   166,   164,   192,   193,    75,    76,   514,   515,   420,
     163,    84,   145,   166,   839,   690,   165,   163,    82,   164,
     169,   164,   171,    71,   217,   218,   219,   220,   221,   164,
     223,   154,   164,   109,   164,   168,    84,   217,   218,   219,
     220,   221,   165,   192,   193,   164,     3,   949,    71,    72,
       7,   168,   217,   218,   219,   220,   221,   468,   164,   176,
     177,   164,   164,   186,   187,   854,   219,   220,   221,   154,
     164,   217,   218,   219,   220,   221,   152,   164,   154,   163,
     165,   198,   164,   164,   217,   218,   219,   220,   221,   165,
     164,   222,   165,   169,   163,   171,   169,   166,   171,   216,
     601,   186,   187,   778,   180,   181,   182,   183,   164,     4,
     186,   187,   188,   189,    71,    72,   165,   165,    38,    39,
      40,   194,   195,   171,   147,   148,   149,   150,   151,   164,
     153,   154,   155,   156,   157,   158,   159,   164,   164,   164,
     216,   165,   165,   682,   164,   164,   194,   195,   217,   218,
     219,   220,   221,   164,   168,   272,   273,   163,   167,   164,
     166,   656,   164,   186,   187,   660,   164,   168,   663,   664,
     164,   582,   164,   196,   164,   164,    71,    72,   164,   718,
     719,   720,   721,   684,   723,   724,   725,   164,   599,   164,
     147,   148,   149,   150,   151,   164,   153,   154,   155,   156,
     157,   158,   159,   217,   218,   219,   220,   221,   165,   164,
     164,   217,   218,   219,   220,   221,   217,   218,   219,   220,
     221,   722,   168,    10,    11,    12,    13,   728,   167,   186,
     187,   164,   168,   190,   191,   164,   223,   168,   163,   196,
     167,   169,   917,   162,   168,   163,   163,   658,   659,   924,
     165,   662,   147,   148,   149,   150,   151,   168,   153,   154,
     155,   156,   157,   158,   159,   217,   218,   219,   220,   221,
     165,   217,   218,   219,   220,   221,     4,   167,   163,   163,
     955,   217,   218,   219,   220,   221,   217,   218,   219,   220,
     221,   186,   187,   217,   218,   219,   220,   221,     3,   167,
       3,   196,   803,   167,   169,   844,   217,   218,   219,   220,
     221,   217,   218,   219,   220,   221,   163,   163,   163,   163,
     163,   732,    70,    71,    72,   165,   165,   866,   867,   868,
       6,   742,   165,   165,   165,     4,   225,   167,    14,   163,
     163,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,   165,     4,   163,    30,    31,    32,    33,    34,   163,
      36,   163,   163,   163,     4,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,     6,    53,    54,    55,
      56,    57,    58,    59,    60,    61,   163,    63,   163,   163,
     163,   163,   163,   163,     4,   163,   163,   163,   163,   147,
     148,   149,   150,   151,   163,   153,   154,   155,   156,   157,
     158,   159,    88,   163,   163,   168,   827,   165,   163,   163,
     163,   163,    98,   163,     3,   166,   164,   166,   104,   164,
     106,   107,   164,   109,   164,   111,   112,   113,   186,   187,
     116,   117,     3,   119,   163,   121,    71,    72,   196,     5,
     168,   163,   172,   960,   163,   163,   163,    70,    70,   163,
     174,   163,   969,   160,   875,   141,   180,   143,   182,   163,
     184,   166,   186,    35,   188,     4,   190,   163,   192,    37,
     194,    35,   196,   927,    79,   163,    79,    83,   163,   163,
      89,   167,   165,     4,   108,   163,   168,   173,   174,   170,
       3,   163,   166,   947,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   147,   148,   149,   150,   151,     4,   153,   154,
     155,   156,   157,   158,   159,    73,    74,   174,   171,   171,
     165,   163,   163,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   186,   187,   163,   102,   103,   181,   105,   183,   224,
     185,   196,   187,   163,   189,   166,   191,   163,   193,   163,
     195,   163,   197,   163,   167,   163,   163,     4,   126,   163,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,     4,   167,   163,   163,     4,   163,     4,   163,    70,
     163,   163,   163,   163,   163,   163,     4,   163,   163,   108,
       3,   163,   223,   164,   163,     4,   166,   163,   165,     3,
     163,   166,    68,   171,   166,   166,   164,   164,   172,   164,
     164,   164,   164,   164,   182,   183,   164,   164,   164,   164,
     164,     5,    70,   163,   163,     4,   163,   165,   163,   140,
       4,   163,   163,    79,    79,   171,   163,    86,   163,   169,
     163,    91,    68,   163,   166,   163,    90,     4,     4,   166,
     166,   166,   164,   163,   163,   168,     4,   163,   163,     4,
       4,   163,   163,   166,     5,   164,   164,   164,   164,     4,
       3,    38,   166,   163,     4,    87,   163,   166,   163,     7,
     166,   163,   166,   166,   164,   163,    71,   166,     4,   170,
     163,     4,   164,   163,   163,     4,   163,     5,   224,   164,
      38,     4,   166,    86,   166,   166,    81,   166,    68,   163,
     166,   166,   163,   166,    80,     4,   968,     5,   161,   377,
     220,   163,   163,   163,   163,   199,    65,    59,   166,   857,
     441,   852,   499,   193,   843,   929,   888,   755,   160,   668,
     728,   915,   468,   382,   480,   492,   697,   281,   504
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   227,   228,   229,   230,   164,   164,     0,
     228,   100,   237,   171,   171,   164,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    30,    31,
      32,    33,    34,    36,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    63,    88,    98,   104,   106,   107,
     109,   111,   112,   113,   116,   117,   119,   121,   141,   143,
     167,   173,   174,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   231,   232,   239,   241,   242,   243,   244,   246,   247,
     255,   258,   259,   261,   262,   269,   271,   272,   274,   276,
     278,   282,   283,   284,   287,   289,   296,   301,   304,   310,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     326,   327,   328,   329,   330,   332,   333,   334,   335,   336,
     337,   338,   339,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   163,   101,   238,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   261,   164,   164,   164,
     167,   174,   243,   245,   263,   164,   173,     4,   165,   180,
     181,   216,   378,   388,   165,   239,   222,   164,   273,     3,
     163,   171,   182,   183,   364,   377,    10,    11,    12,    13,
       4,    85,   389,    71,    72,    75,    76,   154,   165,   186,
     187,   302,   366,   367,     3,    71,    84,   165,   171,   194,
     195,   312,   375,   376,   312,     3,    71,   165,   171,   192,
     193,   306,   373,   374,    71,   171,   351,    64,    71,   358,
     366,   366,   147,   148,   149,   150,   151,   153,   155,   156,
     157,   158,   159,   165,   196,   366,   386,   387,   165,   188,
     189,   369,   370,   369,    52,   165,   396,   397,   171,   366,
     366,    82,   366,   165,   366,   396,   369,   369,    71,   171,
     286,   352,   396,   396,   396,   365,   378,   165,   285,   378,
     366,   387,   387,   366,   387,   387,     4,     8,     9,    71,
     390,   387,   286,   387,     5,    71,   297,   279,   387,   369,
     242,   365,   222,   167,   366,   165,     8,   378,   167,   145,
     168,   217,   218,   219,   220,   221,   377,   378,   383,   384,
     385,    21,    23,    27,    28,    29,   109,   152,   165,   169,
     366,   369,   377,   378,   392,   393,   394,   277,   169,   240,
     238,   167,   223,   163,   163,   163,   162,   368,   165,     4,
     167,   163,   163,   291,     3,   167,   313,   311,     3,   167,
     305,   163,   163,   163,   163,   163,   369,   369,   366,   366,
     165,   366,   165,   165,   165,   386,   165,   387,   225,     4,
     167,   163,   163,   165,     4,   163,   163,   163,   163,   163,
       4,   163,   163,   163,     6,   260,   163,   163,   163,   163,
       4,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     240,   280,   163,   163,   168,   378,   365,   163,   256,   166,
     166,   378,   378,   378,   378,   378,   378,   378,   163,   166,
     164,     4,   251,   252,   253,   254,   367,   370,   377,   378,
     164,   164,   164,   241,   239,   378,   364,     3,     3,   165,
     331,     3,   163,   168,   387,   163,   378,    76,    77,   303,
       5,   172,   163,   378,   163,   240,   163,   378,   240,   366,
     366,   387,    70,    70,   163,   163,    70,   299,   387,   366,
     366,     3,     7,   165,   190,   191,   371,   372,   387,   160,
     166,   387,   163,   378,    65,    71,   359,    35,     4,   163,
      35,    37,    71,    73,    74,   360,   363,   366,    79,   163,
      79,   396,    71,   165,   292,   163,   396,   396,   396,   365,
     163,   285,   390,   366,    67,    71,   395,    83,   165,    89,
      71,   165,   293,     4,   358,   281,   165,   371,   371,   275,
     118,   223,   270,   168,    71,   377,   244,   380,   381,   382,
     168,   168,   383,   387,   163,   163,   170,   163,   170,   163,
     170,   163,   170,   169,   375,   169,   373,   169,   371,   170,
     168,     3,   163,   368,   166,     4,   168,   291,   171,   168,
       4,     8,     9,    64,    65,    66,    69,    92,    93,    94,
      95,    96,    97,    99,   114,   115,   123,   124,   125,   126,
     127,   140,   142,   314,   366,   388,   391,   171,   168,   307,
     163,   163,   163,   366,   366,   224,   163,   166,   163,   163,
     163,     3,   167,   163,   163,   163,     4,   168,   163,   163,
       4,   163,   167,   163,     4,   163,     4,   163,   359,   163,
     163,   325,   163,    70,   163,   390,   163,     4,   163,   163,
     108,   163,   240,   223,   378,   264,   164,   163,   166,     4,
     370,   367,   378,   377,   248,   376,   249,   374,   250,   372,
     163,     3,   166,   172,   166,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   166,   163,    68,
     358,     5,   163,   299,   387,   378,   378,   387,   163,   378,
     387,   387,   165,   163,   352,     4,   163,   140,     4,    71,
      78,   362,    79,   163,    71,   165,   294,   171,    79,   365,
     163,   377,   163,    86,   163,    71,   165,   295,    71,   298,
     299,   240,   371,   169,   176,   177,   379,   380,   257,   163,
     170,   163,   170,   163,   170,   359,   163,    68,    91,    90,
     365,   365,   365,   365,   377,   365,   365,   365,   373,    73,
      74,   102,   103,   105,   126,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   308,   361,   363,   377,
     163,   163,   235,   378,   166,   163,   166,   163,   166,   359,
     168,   166,   166,   378,     4,   163,   353,   163,     4,   164,
     168,     4,   302,   163,   163,     4,    71,   300,   390,     4,
       4,   353,   163,   163,   120,   122,   265,   266,   240,   376,
     374,   372,   166,     5,   164,   377,   164,   164,   164,   164,
       4,     3,    38,   378,   166,   218,   163,    38,    39,    40,
     354,   355,    87,   356,     4,   163,   373,   166,   163,   295,
     365,   166,   166,   166,   163,   356,   300,     7,   390,   164,
     170,   265,   163,    71,   365,   365,   365,   139,   171,   309,
     166,   163,   166,   378,     4,   163,   164,   340,   163,     4,
     303,   353,   163,     4,   288,   163,   164,   268,     5,   224,
      38,   163,   233,   166,   354,    86,   357,   240,   341,     4,
     166,   166,   356,    81,   166,   240,    68,   267,   239,   163,
     309,   166,   161,   166,   166,   290,   163,   163,   239,   359,
     163,   240,    80,     4,   163,   366,     5,    71,   236,   163,
     234,   235,   366,   166
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
			    add_opvars(splev, "iiiiiio",
				       VA_PASS7((long)(yyvsp[(9) - (9)].i), (long)(yyvsp[(5) - (9)].i), (long)(yyvsp[(3) - (9)].i),
						(long)(yyvsp[(7) - (9)].i), 0, 0, SPO_ROOM_DOOR));
			}
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1388 "lev_comp.y"
    {
			/* ERR means random here */
			if ((yyvsp[(7) - (13)].i) == ERR && (yyvsp[(9) - (13)].i) != ERR) {
			    lc_error("If the door wall is random, so must be its pos!");
			} else {
			    add_opvars(splev, "iiiiiio",
				       VA_PASS7((long)(yyvsp[(9) - (13)].i), (long)(yyvsp[(5) - (13)].i), (long)(yyvsp[(3) - (13)].i),
						(long)(yyvsp[(7) - (13)].i), (yyvsp[(11) - (13)].i), (yyvsp[(13) - (13)].i), SPO_ROOM_DOOR));
			}
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1399 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((long)(yyvsp[(3) - (5)].i), 0, 0, SPO_DOOR));
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1403 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((long)(yyvsp[(3) - (9)].i), (yyvsp[(7) - (9)].i), (yyvsp[(9) - (9)].i), SPO_DOOR));
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1417 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1421 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1431 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1438 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1446 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1462 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1466 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1472 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1480 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1486 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1494 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1502 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1507 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1513 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1519 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1525 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1531 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1536 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1541 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1546 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1551 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1556 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1561 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1566 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1571 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1576 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1581 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1587 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1594 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1603 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1607 "lev_comp.y"
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

  case 237:

/* Line 1455 of yacc.c  */
#line 1620 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1628 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1638 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1646 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1655 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1662 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1670 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1676 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1681 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1686 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1691 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1696 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1701 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1706 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1711 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1716 "lev_comp.y"
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

  case 253:

/* Line 1455 of yacc.c  */
#line 1727 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1733 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1738 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1743 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1748 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1753 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1758 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1763 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1768 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1773 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1778 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1783 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1790 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1796 "lev_comp.y"
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

  case 267:

/* Line 1455 of yacc.c  */
#line 1825 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1830 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1838 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1844 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1850 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1857 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1864 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1874 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1884 "lev_comp.y"
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

  case 276:

/* Line 1455 of yacc.c  */
#line 1899 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1909 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1913 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1919 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1925 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1931 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int)(yyvsp[(6) - (13)].i), (int)(yyvsp[(8) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1937 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1943 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1949 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1954 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1961 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1968 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1974 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1980 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1986 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1990 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1996 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 2002 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2012 "lev_comp.y"
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

  case 295:

/* Line 1455 of yacc.c  */
#line 2026 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      if ( (yyvsp[(10) - (11)].i) ) {
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      } else if ( (yyvsp[(11) - (11)].i) )
			  lc_error("Cannot use lev statements in non-permanent REGION");
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2036 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2040 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2046 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2053 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2059 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2065 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2071 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2077 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2081 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2086 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2093 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2099 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2106 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2110 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2117 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2128 "lev_comp.y"
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

  case 314:

/* Line 1455 of yacc.c  */
#line 2141 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2145 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2151 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2155 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2162 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2166 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2170 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2176 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2180 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2186 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2202 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2210 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2220 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2229 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2234 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2241 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2252 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2258 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2262 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2266 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2273 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2283 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2290 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2294 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2300 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2304 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2312 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2316 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2323 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2333 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2346 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2350 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2357 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2367 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2376 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2387 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2391 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2398 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2408 "lev_comp.y"
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

  case 363:

/* Line 1455 of yacc.c  */
#line 2419 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2428 "lev_comp.y"
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

  case 365:

/* Line 1455 of yacc.c  */
#line 2438 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2444 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2448 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2455 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2465 "lev_comp.y"
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

  case 370:

/* Line 1455 of yacc.c  */
#line 2476 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2485 "lev_comp.y"
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

  case 372:

/* Line 1455 of yacc.c  */
#line 2495 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2499 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2505 "lev_comp.y"
    { ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2507 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2513 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2517 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2521 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2525 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2533 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2542 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2546 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2550 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2554 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2558 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2561 "lev_comp.y"
    { ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2565 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2572 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2581 "lev_comp.y"
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

  case 394:

/* Line 1455 of yacc.c  */
#line 2622 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2626 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2633 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2640 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2650 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2654 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2662 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2666 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2670 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2674 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2678 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2683 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2687 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2691 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2696 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2701 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2706 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2710 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2715 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2720 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2724 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2728 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2733 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2737 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2744 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2750 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2754 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2760 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2776 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2780 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2784 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2788 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2810 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2814 "lev_comp.y"
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

  case 441:

/* Line 1455 of yacc.c  */
#line 2840 "lev_comp.y"
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
#line 6420 "lev_comp.tab.c"
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
#line 2868 "lev_comp.y"


/*lev_comp.y*/

