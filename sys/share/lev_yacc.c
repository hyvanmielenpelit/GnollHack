
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
     DIRECTION = 324,
     RANDOM_TYPE = 325,
     RANDOM_TYPE_BRACKET = 326,
     A_REGISTER = 327,
     ALIGNMENT = 328,
     LEFT_OR_RIGHT = 329,
     CENTER = 330,
     TOP_OR_BOT = 331,
     ALTAR_TYPE = 332,
     UP_OR_DOWN = 333,
     ACTIVE_OR_INACTIVE = 334,
     MODRON_PORTAL_TYPE = 335,
     NPC_TYPE = 336,
     FOUNTAIN_TYPE = 337,
     SPECIAL_OBJECT_TYPE = 338,
     CMAP_TYPE = 339,
     FLOOR_TYPE = 340,
     FLOOR_TYPE_ID = 341,
     FLOOR_ID = 342,
     FLOOR_MAIN_TYPE = 343,
     ELEMENTAL_ENCHANTMENT_TYPE = 344,
     EXCEPTIONALITY_TYPE = 345,
     EXCEPTIONALITY_ID = 346,
     ELEMENTAL_ENCHANTMENT_ID = 347,
     ENCHANTMENT_ID = 348,
     CHARGES_ID = 349,
     SPECIAL_QUALITY_ID = 350,
     SPEFLAGS_ID = 351,
     SUBROOM_ID = 352,
     NAME_ID = 353,
     FLAGS_ID = 354,
     FLAG_TYPE = 355,
     MON_ATTITUDE = 356,
     MON_ALERTNESS = 357,
     SUBTYPE_ID = 358,
     MON_APPEARANCE = 359,
     ROOMDOOR_ID = 360,
     IF_ID = 361,
     ELSE_ID = 362,
     TERRAIN_ID = 363,
     HORIZ_OR_VERT = 364,
     REPLACE_TERRAIN_ID = 365,
     EXIT_ID = 366,
     SHUFFLE_ID = 367,
     QUANTITY_ID = 368,
     BURIED_ID = 369,
     LOOP_ID = 370,
     FOR_ID = 371,
     TO_ID = 372,
     SWITCH_ID = 373,
     CASE_ID = 374,
     BREAK_ID = 375,
     DEFAULT_ID = 376,
     ERODED_ID = 377,
     TRAPPED_STATE = 378,
     RECHARGED_ID = 379,
     INVIS_ID = 380,
     GREASED_ID = 381,
     FEMALE_ID = 382,
     WAITFORU_ID = 383,
     CANCELLED_ID = 384,
     REVIVED_ID = 385,
     AVENGE_ID = 386,
     FLEEING_ID = 387,
     BLINDED_ID = 388,
     PARALYZED_ID = 389,
     STUNNED_ID = 390,
     CONFUSED_ID = 391,
     SEENTRAPS_ID = 392,
     ALL_ID = 393,
     MONTYPE_ID = 394,
     GRAVE_ID = 395,
     ERODEPROOF_ID = 396,
     FUNCTION_ID = 397,
     MSG_OUTPUT_TYPE = 398,
     COMPARE_TYPE = 399,
     UNKNOWN_TYPE = 400,
     rect_ID = 401,
     fillrect_ID = 402,
     line_ID = 403,
     randline_ID = 404,
     grow_ID = 405,
     selection_ID = 406,
     flood_ID = 407,
     rndcoord_ID = 408,
     circle_ID = 409,
     ellipse_ID = 410,
     filter_ID = 411,
     complement_ID = 412,
     gradient_ID = 413,
     GRADIENT_TYPE = 414,
     LIMITED = 415,
     HUMIDITY_TYPE = 416,
     STRING = 417,
     MAP_ID = 418,
     NQSTRING = 419,
     VARSTRING = 420,
     CFUNC = 421,
     CFUNC_INT = 422,
     CFUNC_STR = 423,
     CFUNC_COORD = 424,
     CFUNC_REGION = 425,
     VARSTRING_INT = 426,
     VARSTRING_INT_ARRAY = 427,
     VARSTRING_STRING = 428,
     VARSTRING_STRING_ARRAY = 429,
     VARSTRING_VAR = 430,
     VARSTRING_VAR_ARRAY = 431,
     VARSTRING_COORD = 432,
     VARSTRING_COORD_ARRAY = 433,
     VARSTRING_REGION = 434,
     VARSTRING_REGION_ARRAY = 435,
     VARSTRING_MAPCHAR = 436,
     VARSTRING_MAPCHAR_ARRAY = 437,
     VARSTRING_MONST = 438,
     VARSTRING_MONST_ARRAY = 439,
     VARSTRING_OBJ = 440,
     VARSTRING_OBJ_ARRAY = 441,
     VARSTRING_SEL = 442,
     VARSTRING_SEL_ARRAY = 443,
     METHOD_INT = 444,
     METHOD_INT_ARRAY = 445,
     METHOD_STRING = 446,
     METHOD_STRING_ARRAY = 447,
     METHOD_VAR = 448,
     METHOD_VAR_ARRAY = 449,
     METHOD_COORD = 450,
     METHOD_COORD_ARRAY = 451,
     METHOD_REGION = 452,
     METHOD_REGION_ARRAY = 453,
     METHOD_MAPCHAR = 454,
     METHOD_MAPCHAR_ARRAY = 455,
     METHOD_MONST = 456,
     METHOD_MONST_ARRAY = 457,
     METHOD_OBJ = 458,
     METHOD_OBJ_ARRAY = 459,
     METHOD_SEL = 460,
     METHOD_SEL_ARRAY = 461,
     DICE = 462
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
#line 505 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 517 "lev_comp.tab.c"

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
#define YYLAST   1126

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  225
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  172
/* YYNRULES -- Number of rules.  */
#define YYNRULES  440
/* YYNRULES -- Number of states.  */
#define YYNSTATES  971

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   462

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   220,   224,     2,
     164,   165,   218,   216,   162,   217,   222,   219,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   163,     2,
       2,   221,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   166,     2,   167,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   168,   223,   169,     2,     2,     2,     2,
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
     155,   156,   157,   158,   159,   160,   161,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215
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
     818,   822,   826,   830,   834,   840,   848,   854,   863,   865,
     869,   875,   881,   889,   897,   904,   910,   911,   914,   920,
     924,   938,   942,   946,   948,   954,   964,   970,   974,   978,
     984,   994,  1000,  1006,  1007,  1019,  1020,  1022,  1030,  1034,
    1042,  1048,  1054,  1060,  1066,  1070,  1076,  1084,  1094,  1096,
    1098,  1100,  1102,  1104,  1105,  1108,  1110,  1114,  1116,  1118,
    1120,  1121,  1125,  1127,  1129,  1131,  1133,  1135,  1137,  1139,
    1141,  1143,  1145,  1149,  1151,  1153,  1158,  1160,  1162,  1167,
    1169,  1171,  1176,  1178,  1183,  1189,  1191,  1195,  1197,  1201,
    1203,  1205,  1210,  1220,  1222,  1224,  1229,  1231,  1237,  1239,
    1241,  1246,  1248,  1250,  1256,  1258,  1260,  1262,  1267,  1269,
    1271,  1277,  1279,  1281,  1283,  1287,  1289,  1291,  1295,  1297,
    1302,  1306,  1310,  1314,  1318,  1322,  1326,  1328,  1330,  1334,
    1336,  1340,  1341,  1343,  1345,  1347,  1349,  1353,  1354,  1356,
    1358,  1361,  1364,  1369,  1376,  1381,  1388,  1395,  1402,  1409,
    1412,  1419,  1428,  1437,  1448,  1463,  1466,  1468,  1472,  1474,
    1478,  1480,  1482,  1484,  1486,  1488,  1490,  1492,  1494,  1496,
    1498,  1500,  1502,  1504,  1506,  1508,  1510,  1512,  1514,  1516,
    1527
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     226,     0,    -1,    -1,   227,    -1,   228,    -1,   228,   227,
      -1,   229,   236,   238,    -1,    16,   163,   170,    -1,    15,
     163,   170,   162,     3,    -1,    17,   163,    11,   162,   330,
      -1,    17,   163,    10,   162,     3,    -1,    17,   163,    13,
      -1,    17,   163,    12,   162,     3,   162,     3,   162,     5,
     162,     5,   162,   358,   162,   235,   234,    -1,    18,   163,
     388,    -1,    -1,   162,   160,    -1,    -1,   162,   365,    -1,
      -1,   162,     3,    -1,     5,    -1,    70,    -1,    -1,    99,
     163,   237,    -1,   100,   162,   237,    -1,   100,    -1,    -1,
     240,   238,    -1,   168,   238,   169,    -1,   281,    -1,   230,
      -1,   231,    -1,   344,    -1,   343,    -1,   341,    -1,   342,
      -1,   345,    -1,   346,    -1,   323,    -1,   283,    -1,   246,
      -1,   245,    -1,   333,    -1,   295,    -1,   315,    -1,   348,
      -1,   349,    -1,   325,    -1,   326,    -1,   327,    -1,   347,
      -1,   261,    -1,   271,    -1,   273,    -1,   277,    -1,   275,
      -1,   258,    -1,   268,    -1,   254,    -1,   257,    -1,   318,
      -1,   300,    -1,   316,    -1,   303,    -1,   309,    -1,   334,
      -1,   329,    -1,   321,    -1,   282,    -1,   335,    -1,   336,
      -1,   337,    -1,   338,    -1,   288,    -1,   286,    -1,   328,
      -1,   332,    -1,   331,    -1,   319,    -1,   320,    -1,   322,
      -1,   314,    -1,   317,    -1,   180,    -1,   182,    -1,   184,
      -1,   186,    -1,   188,    -1,   190,    -1,   192,    -1,   194,
      -1,   196,    -1,   179,    -1,   181,    -1,   183,    -1,   185,
      -1,   187,    -1,   189,    -1,   191,    -1,   193,    -1,   195,
      -1,   241,    -1,   242,    -1,   173,    -1,   173,    -1,   242,
      -1,   112,   163,   241,    -1,   243,   221,   377,    -1,   243,
     221,   151,   163,   386,    -1,   243,   221,   376,    -1,   243,
     221,   393,   163,   370,    -1,   243,   221,   392,   163,   372,
      -1,   243,   221,   391,   163,   374,    -1,   243,   221,   365,
      -1,   243,   221,   368,    -1,   243,   221,   168,   252,   169,
      -1,   243,   221,   168,   251,   169,    -1,   243,   221,   168,
     250,   169,    -1,   243,   221,   393,   163,   168,   249,   169,
      -1,   243,   221,   392,   163,   168,   248,   169,    -1,   243,
     221,   391,   163,   168,   247,   169,    -1,   243,   221,   168,
     253,   169,    -1,   375,    -1,   247,   162,   375,    -1,   373,
      -1,   248,   162,   373,    -1,   371,    -1,   249,   162,   371,
      -1,   369,    -1,   250,   162,   369,    -1,   366,    -1,   251,
     162,   366,    -1,   377,    -1,   252,   162,   377,    -1,   376,
      -1,   253,   162,   376,    -1,    -1,    -1,   142,   172,   164,
     255,   381,   165,   256,   239,    -1,   172,   164,   384,   165,
      -1,   111,    -1,    -1,     6,    -1,     6,    -1,   166,   377,
     144,   377,   167,    -1,   166,   377,   167,    -1,    -1,    -1,
     118,   262,   166,   364,   167,   263,   168,   264,   169,    -1,
      -1,   265,   264,    -1,    -1,   119,   389,   163,   266,   238,
      -1,    -1,   121,   163,   267,   238,    -1,   120,    -1,   222,
     222,    -1,   117,    -1,   116,   244,   221,   377,   269,   377,
      -1,    -1,   270,   272,   239,    -1,    -1,   115,   166,   364,
     167,   274,   239,    -1,    -1,   260,   163,   276,   240,    -1,
      -1,   106,   260,   278,   279,    -1,   239,    -1,    -1,   239,
     280,   107,   239,    -1,    14,   163,   376,    -1,    63,    -1,
      63,   163,   389,    -1,    63,   163,    70,    -1,    54,   163,
     284,   162,   284,    -1,    54,   163,   284,   162,   389,    -1,
     164,     4,   162,    69,   162,   299,   165,    -1,   351,   259,
     162,   358,    -1,    -1,    97,   163,   285,   162,   292,   162,
     294,   352,   355,   287,   239,    -1,    -1,    48,   163,   285,
     162,   291,   162,   293,   162,   294,   352,   355,   289,   239,
      -1,    -1,   162,     5,    -1,   164,     4,   162,     4,   165,
      -1,    70,    -1,   164,     4,   162,     4,   165,    -1,    70,
      -1,   164,   301,   162,   302,   165,    -1,    70,    -1,   164,
       4,   162,     4,   165,    -1,    70,    -1,   105,   163,   296,
     162,   357,   162,   297,   162,   299,    -1,   105,   163,   296,
     162,   357,   162,   297,   162,   299,   162,    68,   162,     4,
      -1,    25,   163,   357,   162,   386,    -1,    25,   163,   357,
     162,   386,   162,    68,   162,     4,    -1,     5,    -1,    70,
      -1,   298,    -1,    70,    -1,    69,    -1,    69,   223,   298,
      -1,     4,    -1,    70,    -1,    20,    -1,    19,   163,   301,
     162,   302,   290,   171,    -1,    19,   163,   365,   290,   171,
      -1,    74,    -1,    75,    -1,    76,    -1,    75,    -1,    23,
     163,   305,    -1,    -1,    23,   163,   305,   304,   239,    -1,
     372,   162,   365,   306,    -1,    -1,   306,   162,   307,    -1,
     376,    -1,   101,    -1,   102,    -1,   360,    -1,   104,   376,
      -1,   127,    -1,   125,    -1,   129,    -1,   130,    -1,   131,
      -1,   132,   163,   364,    -1,   133,   163,   364,    -1,   134,
     163,   364,    -1,   135,    -1,   136,    -1,   137,   163,   308,
      -1,   128,    -1,   170,    -1,   138,    -1,   170,   223,   308,
      -1,    21,   163,   311,    -1,    -1,    22,   163,   311,   310,
     239,    -1,   374,   312,    -1,    -1,   312,   162,   313,    -1,
      66,    -1,   139,   163,   372,    -1,   390,    -1,    98,   163,
     376,    -1,   113,   163,   364,    -1,   114,    -1,    65,    -1,
     122,   163,   364,    -1,   141,    -1,    64,    -1,   123,    -1,
     124,   163,   364,    -1,   125,    -1,   126,    -1,   365,    -1,
      92,   163,    89,    -1,    91,   163,    90,    -1,    93,   163,
     364,    -1,    94,   163,   364,    -1,    95,   163,   364,    -1,
      96,   163,   364,    -1,    24,   163,   350,   162,   365,    -1,
      26,   163,   365,   162,    69,   162,   357,    -1,    30,   163,
     365,   162,    69,    -1,    30,   163,   365,   162,    69,   162,
       5,   234,    -1,    31,    -1,    31,   163,   386,    -1,    44,
     163,   365,   162,    78,    -1,    45,   163,   365,   162,    78,
      -1,    45,   163,   395,   162,   395,   162,    78,    -1,    49,
     163,   395,   162,   395,   162,   170,    -1,    50,   163,   395,
     162,   395,   324,    -1,    51,   163,   395,   162,   395,    -1,
      -1,   162,    78,    -1,    57,   163,   386,   162,    82,    -1,
      58,   163,   386,    -1,    59,   163,   365,   162,   164,   389,
     162,   389,   165,   162,    80,   162,    79,    -1,    61,   163,
     386,    -1,    60,   163,   386,    -1,     3,    -1,   164,     3,
     162,   358,   165,    -1,   110,   163,   368,   162,   370,   162,
     370,   162,     7,    -1,   108,   163,   386,   162,   370,    -1,
      46,   163,   368,    -1,    47,   163,   368,    -1,    36,   163,
     170,   162,    37,    -1,    36,   163,   170,   162,    37,   162,
     139,   163,   372,    -1,    33,   163,   368,   162,    35,    -1,
      34,   163,   395,   162,    35,    -1,    -1,    32,   163,   368,
     162,   358,   162,   351,   352,   355,   339,   340,    -1,    -1,
     239,    -1,    41,   163,   365,   162,   359,   162,   361,    -1,
      42,   163,   365,    -1,    87,   163,   386,   162,    88,   162,
      85,    -1,   103,   163,   386,   162,     4,    -1,    43,   163,
      81,   162,   365,    -1,   140,   163,   365,   162,   376,    -1,
     140,   163,   365,   162,    70,    -1,   140,   163,   365,    -1,
      55,   163,   377,   162,   365,    -1,    56,   163,   365,   162,
     394,   162,   376,    -1,    53,   163,   364,   162,   364,   162,
     364,   162,   364,    -1,    53,    -1,   170,    -1,    70,    -1,
     170,    -1,    70,    -1,    -1,   162,   353,    -1,   354,    -1,
     354,   162,   353,    -1,    38,    -1,    39,    -1,    40,    -1,
      -1,    86,   163,   356,    -1,    85,    -1,    64,    -1,    70,
      -1,    65,    -1,    70,    -1,    73,    -1,   362,    -1,    70,
      -1,    73,    -1,   362,    -1,    72,   163,    70,    -1,    77,
      -1,    70,    -1,    72,   166,     4,   167,    -1,   170,    -1,
     181,    -1,   182,   166,   377,   167,    -1,   377,    -1,   366,
      -1,   153,   164,   386,   165,    -1,   185,    -1,   186,   166,
     377,   167,    -1,   164,     4,   162,     4,   165,    -1,    70,
      -1,    71,   367,   167,    -1,   161,    -1,   161,   162,   367,
      -1,   369,    -1,   187,    -1,   188,   166,   377,   167,    -1,
     164,     4,   162,     4,   162,     4,   162,     4,   165,    -1,
     371,    -1,   189,    -1,   190,   166,   377,   167,    -1,     3,
      -1,   164,     3,   162,   358,   165,    -1,   373,    -1,   191,
      -1,   192,   166,   377,   167,    -1,   170,    -1,     3,    -1,
     164,     3,   162,   170,   165,    -1,    70,    -1,   375,    -1,
     193,    -1,   194,   166,   377,   167,    -1,   170,    -1,     3,
      -1,   164,     3,   162,   170,   165,    -1,    70,    -1,    83,
      -1,   363,    -1,   376,   222,   363,    -1,     4,    -1,   387,
      -1,   164,     8,   165,    -1,   179,    -1,   180,   166,   377,
     167,    -1,   377,   216,   377,    -1,   377,   217,   377,    -1,
     377,   218,   377,    -1,   377,   219,   377,    -1,   377,   220,
     377,    -1,   164,   377,   165,    -1,   175,    -1,   176,    -1,
     243,   163,   378,    -1,   379,    -1,   380,   162,   379,    -1,
      -1,   380,    -1,   377,    -1,   376,    -1,   382,    -1,   383,
     162,   382,    -1,    -1,   383,    -1,   365,    -1,   146,   368,
      -1,   147,   368,    -1,   148,   365,   162,   365,    -1,   149,
     365,   162,   365,   162,   377,    -1,   150,   164,   386,   165,
      -1,   150,   164,   298,   162,   386,   165,    -1,   156,   164,
       7,   162,   386,   165,    -1,   156,   164,   386,   162,   386,
     165,    -1,   156,   164,   370,   162,   386,   165,    -1,   152,
     365,    -1,   154,   164,   365,   162,   377,   165,    -1,   154,
     164,   365,   162,   377,   162,    38,   165,    -1,   155,   164,
     365,   162,   377,   162,   377,   165,    -1,   155,   164,   365,
     162,   377,   162,   377,   162,    38,   165,    -1,   158,   164,
     159,   162,   164,   377,   217,   377,   232,   165,   162,   365,
     233,   165,    -1,   157,   385,    -1,   195,    -1,   164,   386,
     165,    -1,   385,    -1,   385,   224,   386,    -1,   215,    -1,
      84,    -1,     4,    -1,     8,    -1,     9,    -1,     4,    -1,
       8,    -1,     9,    -1,     4,    -1,   387,    -1,    27,    -1,
      21,    -1,    28,    -1,    23,    -1,    29,    -1,   108,    -1,
      67,    -1,    70,    -1,   396,    -1,    52,   164,     4,   162,
       4,   162,     4,   162,     4,   165,    -1,   164,     4,   162,
       4,   162,     4,   162,     4,   165,    -1
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
    1762,  1767,  1772,  1777,  1784,  1790,  1819,  1824,  1832,  1838,
    1844,  1851,  1858,  1868,  1878,  1893,  1904,  1907,  1913,  1919,
    1925,  1931,  1937,  1943,  1948,  1955,  1962,  1968,  1974,  1980,
    1984,  1990,  1996,  2007,  2006,  2031,  2034,  2040,  2047,  2053,
    2059,  2065,  2071,  2075,  2080,  2087,  2093,  2100,  2104,  2111,
    2119,  2122,  2132,  2136,  2139,  2145,  2149,  2156,  2160,  2164,
    2171,  2174,  2180,  2186,  2187,  2190,  2191,  2194,  2195,  2196,
    2202,  2203,  2204,  2210,  2211,  2214,  2223,  2228,  2235,  2246,
    2252,  2256,  2260,  2267,  2277,  2284,  2288,  2294,  2298,  2306,
    2310,  2317,  2327,  2340,  2344,  2351,  2361,  2370,  2381,  2385,
    2392,  2402,  2413,  2422,  2432,  2438,  2442,  2449,  2459,  2470,
    2479,  2489,  2493,  2500,  2501,  2507,  2511,  2515,  2519,  2527,
    2536,  2540,  2544,  2548,  2552,  2556,  2559,  2566,  2575,  2608,
    2609,  2612,  2613,  2616,  2620,  2627,  2634,  2645,  2648,  2656,
    2660,  2664,  2668,  2672,  2677,  2681,  2685,  2690,  2695,  2700,
    2704,  2709,  2714,  2718,  2722,  2727,  2731,  2738,  2744,  2748,
    2754,  2761,  2762,  2765,  2766,  2767,  2770,  2774,  2778,  2782,
    2788,  2789,  2792,  2793,  2796,  2797,  2800,  2801,  2804,  2808,
    2834
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
  "DIRECTION", "RANDOM_TYPE", "RANDOM_TYPE_BRACKET", "A_REGISTER",
  "ALIGNMENT", "LEFT_OR_RIGHT", "CENTER", "TOP_OR_BOT", "ALTAR_TYPE",
  "UP_OR_DOWN", "ACTIVE_OR_INACTIVE", "MODRON_PORTAL_TYPE", "NPC_TYPE",
  "FOUNTAIN_TYPE", "SPECIAL_OBJECT_TYPE", "CMAP_TYPE", "FLOOR_TYPE",
  "FLOOR_TYPE_ID", "FLOOR_ID", "FLOOR_MAIN_TYPE",
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
     415,   416,    44,    58,    40,    41,    91,    93,   123,   125,
     417,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,    43,    45,    42,    47,
      37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   225,   226,   226,   227,   227,   228,   229,   229,   230,
     230,   230,   230,   231,   232,   232,   233,   233,   234,   234,
     235,   235,   236,   236,   237,   237,   238,   238,   239,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   243,   243,   243,   244,   244,   245,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   247,   247,   248,   248,   249,   249,   250,   250,
     251,   251,   252,   252,   253,   253,   255,   256,   254,   257,
     258,   259,   259,   260,   260,   260,   262,   263,   261,   264,
     264,   266,   265,   267,   265,   268,   269,   269,   270,   272,
     271,   274,   273,   276,   275,   278,   277,   279,   280,   279,
     281,   282,   282,   282,   283,   283,   284,   285,   287,   286,
     289,   288,   290,   290,   291,   291,   292,   292,   293,   293,
     294,   294,   295,   295,   295,   295,   296,   296,   297,   297,
     298,   298,   299,   299,   300,   300,   300,   301,   301,   302,
     302,   303,   304,   303,   305,   306,   306,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   308,   308,   308,   309,   310,   309,
     311,   312,   312,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   314,   315,   316,   316,   317,   317,
     318,   319,   320,   321,   322,   323,   324,   324,   325,   326,
     327,   328,   329,   330,   330,   331,   332,   333,   334,   335,
     335,   336,   337,   339,   338,   340,   340,   341,   342,   343,
     344,   345,   346,   346,   346,   347,   348,   349,   349,   350,
     350,   351,   351,   352,   352,   353,   353,   354,   354,   354,
     355,   355,   356,   357,   357,   358,   358,   359,   359,   359,
     360,   360,   360,   361,   361,   362,   363,   363,   363,   364,
     365,   365,   365,   365,   366,   366,   366,   367,   367,   368,
     368,   368,   369,   370,   370,   370,   371,   371,   372,   372,
     372,   373,   373,   373,   373,   374,   374,   374,   375,   375,
     375,   375,   375,   376,   376,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   378,   378,   379,   380,
     380,   381,   381,   382,   382,   383,   383,   384,   384,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   386,   386,
     387,   388,   388,   389,   389,   389,   390,   390,   390,   390,
     391,   391,   392,   392,   393,   393,   394,   394,   395,   395,
     396
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
       3,     3,     3,     3,     5,     7,     5,     8,     1,     3,
       5,     5,     7,     7,     6,     5,     0,     2,     5,     3,
      13,     3,     3,     1,     5,     9,     5,     3,     3,     5,
       9,     5,     5,     0,    11,     0,     1,     7,     3,     7,
       5,     5,     5,     5,     3,     5,     7,     9,     1,     1,
       1,     1,     1,     0,     2,     1,     3,     1,     1,     1,
       0,     3,     1,     1,     1,     1,     1,     1,     1,     1,
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
       5,     0,    26,     0,     7,     0,   143,     0,     0,     0,
       0,   204,     0,     0,     0,     0,     0,     0,     0,   268,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   308,     0,     0,     0,     0,
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
       0,   104,   105,     0,     0,     0,     0,   375,     0,   378,
       0,   420,     0,   376,   397,    27,     0,   163,     0,     8,
       0,   336,   337,     0,   373,   170,     0,     0,     0,    11,
     422,   421,    13,   345,     0,   207,   208,     0,     0,   342,
       0,     0,   182,   340,   369,   371,   372,     0,   368,   366,
       0,   237,   241,   365,   238,   362,   364,     0,   361,   359,
       0,   211,     0,   358,   310,   309,     0,   323,   324,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   416,   399,   418,   269,     0,   350,
       0,     0,   349,     0,     0,     0,     0,   438,     0,     0,
     298,     0,     0,     0,     0,     0,   287,   288,   312,   311,
       0,   141,     0,     0,     0,     0,   339,     0,     0,     0,
       0,     0,   279,     0,   282,   281,   425,   423,   424,   173,
     172,     0,     0,     0,   196,   197,     0,     0,     0,     0,
     106,     0,     0,     0,   304,   136,     0,     0,     0,     0,
     145,     0,     0,     0,     0,     0,   394,   393,   395,   398,
       0,   431,   433,   430,   432,   434,   435,     0,     0,     0,
     113,   114,   109,   107,     0,     0,     0,     0,    26,   160,
      24,     0,     0,     0,     0,     0,   347,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   240,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,   401,     0,     0,
       0,   409,     0,     0,     0,   415,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   142,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     167,   166,     0,     0,   161,     0,     0,     0,   391,   377,
     385,     0,     0,   380,   381,   382,   383,   384,     0,   139,
       0,   375,     0,     0,     0,     0,   130,   128,   134,   132,
       0,     0,     0,   164,     0,     0,   374,    10,   283,     0,
       9,     0,     0,   346,     0,     0,     0,   210,   209,   182,
     183,   206,     0,     0,     0,   239,     0,     0,   213,   215,
     264,   194,     0,   266,     0,     0,   200,     0,     0,     0,
       0,   356,     0,     0,   354,     0,     0,   353,     0,     0,
     417,   419,     0,     0,   325,   326,     0,   291,     0,     0,
     292,   289,   329,     0,   327,     0,   328,   301,   270,     0,
     271,     0,   185,     0,     0,     0,     0,   276,   275,     0,
       0,   174,   175,   305,   436,   437,     0,   278,     0,     0,
     187,     0,     0,   300,     0,     0,     0,   286,     0,     0,
     157,     0,     0,   147,   303,   302,     0,   389,   392,     0,
     379,   144,   396,   108,     0,     0,   117,     0,   116,     0,
     115,     0,   121,     0,   112,     0,   111,     0,   110,    28,
     338,     0,     0,   348,   341,     0,   343,     0,     0,   367,
     428,   426,   427,   252,   249,   243,     0,     0,     0,     0,
       0,     0,     0,     0,   248,     0,   253,     0,   255,   256,
       0,   251,   242,   257,   429,   245,     0,   360,   214,     0,
       0,     0,   402,     0,     0,     0,   404,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   351,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   177,     0,     0,
     274,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   162,   156,   158,     0,     0,     0,   137,     0,   129,
     131,   133,   135,     0,   122,     0,   124,     0,   126,     0,
       0,   344,   205,   370,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,     0,     0,   265,    18,
       0,   201,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,     0,     0,     0,     0,   334,   333,   297,
     272,     0,   189,     0,     0,   273,   277,     0,     0,   306,
       0,   299,     0,   191,     0,   313,   199,     0,   198,   169,
       0,   149,   386,   387,   388,   390,     0,     0,   120,     0,
     119,     0,   118,     0,     0,   259,   258,   260,   261,   262,
     263,   246,   247,   250,   254,   244,     0,   330,   218,   219,
       0,   223,   222,   233,   224,   225,   226,     0,     0,     0,
     230,   231,     0,   216,   220,   331,   217,     0,     0,   267,
     403,   405,     0,   410,     0,   406,     0,   355,   408,   407,
       0,     0,     0,   320,     0,     0,     0,   335,     0,     0,
       0,     0,   202,   203,     0,     0,     0,     0,   320,     0,
       0,     0,     0,     0,   149,   138,   123,   125,   127,   284,
       0,     0,   221,     0,     0,     0,     0,   195,    19,     0,
       0,   357,     0,     0,   317,   318,   319,   314,   315,     0,
     293,     0,     0,   290,   184,     0,   313,   307,   176,     0,
     186,     0,   178,   192,   285,     0,   153,   148,   150,     0,
     332,   227,   228,   229,   235,   234,   232,   411,     0,   412,
     381,     0,     0,     0,   295,     0,     0,     0,   320,     0,
       0,     0,     0,   151,    26,     0,     0,     0,     0,     0,
     352,   316,   322,   321,   296,   294,     0,   440,   188,   180,
       0,   190,   179,     0,    26,   154,     0,   236,   413,    15,
       0,   439,     0,     0,     0,   152,     0,     0,   181,   280,
     193,     0,    16,    20,    21,    18,     0,     0,    12,    17,
     414
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    91,    92,   929,   967,   819,
     965,    12,   154,    93,   379,    94,    95,    96,    97,   203,
      98,    99,   703,   705,   707,   472,   473,   474,   475,   100,
     458,   776,   101,   102,   435,   103,   104,   204,   694,   853,
     854,   944,   924,   105,   582,   106,   107,   218,   108,   579,
     109,   377,   110,   337,   451,   575,   111,   112,   113,   318,
     310,   114,   921,   115,   952,   393,   554,   572,   754,   765,
     116,   336,   767,   517,   844,   117,   241,   499,   118,   400,
     261,   648,   813,   906,   119,   397,   251,   396,   642,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   680,
     130,   131,   132,   133,   134,   490,   135,   136,   137,   138,
     139,   140,   141,   142,   914,   935,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   266,   311,   833,   877,   878,
     880,   933,   269,   536,   545,   814,   749,   546,   224,   315,
     285,   243,   387,   291,   292,   526,   527,   262,   263,   252,
     253,   356,   316,   774,   587,   588,   589,   358,   359,   360,
     286,   417,   213,   232,   330,   645,   374,   375,   376,   566,
     296,   297
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -791
static const yytype_int16 yypact[] =
{
      95,   -76,   -72,   119,  -791,    95,   -30,   -75,   -40,  -791,
    -791,   -29,   672,   -11,  -791,    42,  -791,     5,    21,    47,
      59,  -791,    79,    99,   109,   117,   123,   136,   168,   179,
     183,   185,   187,   192,   204,   209,   211,   224,   239,   242,
     245,   248,   254,   270,   271,   272,   276,   277,   280,   282,
     287,   293,   298,   299,   300,   314,   325,   326,   330,    37,
     331,   335,  -791,   339,    35,   751,  -791,  -791,   342,    20,
      32,    50,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,
    -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,
    -791,  -791,  -791,  -791,   672,  -791,  -791,    -1,  -791,  -791,
    -791,  -791,  -791,   343,  -791,  -791,  -791,  -791,  -791,  -791,
    -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,
    -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,
    -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,
    -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,
    -791,  -791,   279,   163,  -791,   -84,   473,    38,   262,    82,
      82,   153,   -37,    26,   256,   256,   600,   -85,   -85,   -15,
     191,   256,   256,   323,   256,   354,   -85,   -85,   -35,   -15,
     -15,   -15,    32,   317,    32,   256,   600,   600,   256,   600,
     600,    62,   600,   -35,   600,    56,  -791,   600,   -85,   745,
      32,  -791,  -791,   294,   348,   256,   333,  -791,    49,  -791,
     350,  -791,   393,  -791,    25,  -791,   198,  -791,   351,  -791,
      42,  -791,  -791,   370,  -791,   320,   381,   382,   384,  -791,
    -791,  -791,  -791,  -791,   386,  -791,  -791,   405,   566,  -791,
     406,   418,   427,  -791,  -791,  -791,  -791,   568,  -791,  -791,
     424,  -791,  -791,  -791,  -791,  -791,  -791,   596,  -791,  -791,
     437,   438,   445,  -791,  -791,  -791,   446,  -791,  -791,   454,
     455,   458,   -85,   -85,   256,   256,   457,   256,   459,   461,
     463,   600,   468,   318,  -791,  -791,   398,  -791,   624,  -791,
     469,   472,  -791,   485,   486,   645,   489,  -791,   491,   493,
    -791,   505,   506,   668,   511,   515,  -791,  -791,  -791,  -791,
     525,   694,   539,   545,   547,   550,   306,   720,   572,   426,
     574,   575,  -791,   576,  -791,  -791,  -791,  -791,  -791,  -791,
    -791,   582,   583,   589,  -791,  -791,   598,   351,   603,   605,
    -791,   595,    32,    32,   606,  -791,   601,   174,    32,    32,
    -791,    32,    32,    32,    32,    32,   320,   306,  -791,   608,
     607,  -791,  -791,  -791,  -791,  -791,  -791,   610,    52,    18,
    -791,  -791,   320,   306,   611,   616,   618,   672,   672,  -791,
    -791,    32,   -84,   786,    27,   788,   631,   627,   600,   634,
      32,   162,   792,   628,   636,    32,   639,   351,   640,    32,
     351,   256,   256,   600,   734,   735,  -791,  -791,   643,   644,
     409,  -791,   256,   256,   429,  -791,   648,   667,   600,   646,
      32,    12,   775,   809,   671,   799,   798,    94,   256,   759,
     677,   762,   -15,   -23,  -791,   680,   -15,   -15,   -15,    32,
     681,    30,   256,   203,   765,   684,   761,   -16,   865,    26,
     763,  -791,    89,    89,  -791,   235,   704,   -45,   727,  -791,
    -791,   292,   334,   145,   145,  -791,  -791,  -791,    25,  -791,
     600,   710,   -63,   -57,   -54,   -38,  -791,  -791,   320,   306,
     160,   246,    71,  -791,   705,   604,  -791,  -791,  -791,   870,
    -791,   713,   386,  -791,   711,   873,   609,  -791,  -791,   427,
    -791,  -791,   708,   664,   305,  -791,   709,   669,  -791,  -791,
    -791,   739,   740,   741,   256,   256,   682,   742,   778,   764,
     766,  -791,   783,    23,  -791,   781,   787,  -791,   789,   790,
    -791,  -791,   944,   674,  -791,  -791,   791,  -791,   793,   946,
    -791,   794,  -791,   795,  -791,   796,  -791,  -791,  -791,   950,
    -791,   797,  -791,   953,   800,    12,   801,   802,  -791,   803,
     891,  -791,  -791,  -791,  -791,  -791,   804,  -791,   105,   805,
    -791,   964,   807,  -791,   808,   864,   969,  -791,   811,   351,
    -791,   752,    32,  -791,  -791,   320,   812,  -791,   814,   813,
    -791,  -791,  -791,  -791,   973,   815,  -791,   -20,  -791,    32,
    -791,   -84,  -791,   171,  -791,   113,  -791,    29,  -791,  -791,
    -791,   818,   978,  -791,  -791,   817,  -791,   816,   819,  -791,
    -791,  -791,  -791,  -791,  -791,  -791,   820,   822,   823,   825,
     826,   827,   828,   829,  -791,   830,  -791,   831,  -791,  -791,
     832,  -791,  -791,  -791,  -791,  -791,   833,  -791,   834,   929,
      26,   994,  -791,   838,   932,   600,  -791,    32,    32,   600,
     840,    32,   600,   600,   839,   842,  -791,   -35,  1001,   844,
     868,  1004,    69,   133,   931,   848,    -7,  -791,   841,   934,
    -791,    32,   851,   -84,   852,   930,   854,    -5,   241,   351,
      89,  -791,  -791,   306,   849,   143,   727,  -791,   150,  -791,
    -791,   306,   320,   -19,  -791,    96,  -791,    97,  -791,    12,
     856,  -791,  -791,  -791,   933,   935,    32,    32,    32,    32,
     -84,    32,    32,    32,   153,  -791,   529,   857,  -791,   858,
      32,  -791,   860,   464,   599,   861,    12,   679,   862,   863,
      32,  1017,   867,   869,  1018,   871,   866,  -791,  -791,  -791,
    -791,  1026,  -791,   307,   874,  -791,  -791,   875,    51,   320,
     105,  -791,  1028,  -791,  1031,   867,  -791,   876,  -791,  -791,
     877,    19,  -791,  -791,  -791,  -791,   351,   171,  -791,   113,
    -791,    29,  -791,   878,  1035,  -791,  -791,  -791,  -791,  -791,
    -791,   320,  -791,  -791,  -791,  -791,   177,  -791,  -791,  -791,
     -84,  -791,  -791,  -791,  -791,  -791,  -791,   879,   881,   882,
    -791,  -791,   883,  -791,  -791,  -791,   320,  1037,  1044,  -791,
     306,  -791,  1010,  -791,    32,  -791,   884,  -791,  -791,  -791,
     421,   888,   383,   965,  1048,   892,   153,  -791,   890,   894,
      -5,    32,  -791,  -791,   893,   895,   896,   897,   965,    51,
    1046,   105,   899,   898,    19,  -791,  -791,  -791,  -791,  -791,
     901,   987,   320,    32,    32,    32,   -70,  -791,  -791,   900,
     523,  -791,    32,  1060,  -791,  -791,  -791,  -791,   904,   905,
    -791,   907,  1066,  -791,  -791,   162,   867,  -791,  -791,   909,
    -791,  1068,  -791,   911,  -791,   912,  -791,  -791,  -791,  1069,
    -791,  -791,  -791,  -791,  -791,   853,  -791,  -791,  1039,  -791,
     -58,   913,   383,   995,   351,  1075,   916,   917,   965,  1003,
     919,   351,  1019,  -791,   672,   923,   -70,   921,   928,   924,
    -791,  -791,  -791,  -791,  -791,  -791,   925,  -791,  -791,  -791,
     936,  -791,  -791,   937,   672,  -791,    12,  -791,  -791,  -791,
     938,  -791,   351,  1012,  1088,  -791,   939,   256,  -791,  -791,
    -791,    57,   940,  -791,  -791,   858,   256,   941,  -791,  -791,
    -791
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -791,  -791,  1089,  -791,  -791,  -791,  -791,  -791,  -791,   128,
    -791,  -791,   885,   -94,  -316,   718,   908,  1032,  -445,  -791,
    -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,
    -791,  -791,  -791,  -791,  -791,  1045,  -791,  -791,  -791,   249,
    -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,
    -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,   655,
     915,  -791,  -791,  -791,  -791,   612,  -791,  -791,  -791,   269,
    -791,  -791,  -791,  -587,   261,  -791,   359,   228,  -791,  -791,
    -791,  -791,  -791,   188,  -791,  -791,   955,  -791,  -791,  -791,
    -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,
    -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,
    -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,  -791,
    -791,  -791,  -791,  -791,  -791,  -791,   449,  -741,   205,  -791,
    -790,  -791,  -426,  -551,  -791,  -791,  -791,   392,   737,  -189,
    -157,  -363,   629,   319,  -359,  -433,  -595,  -476,  -589,   642,
    -562,  -152,   -61,  -791,   428,  -791,  -791,   652,  -791,  -791,
     845,  -114,   619,  -791,  -439,  -791,  -791,  -791,  -791,  -791,
    -135,  -791
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -213
static const yytype_int16 yytable[] =
{
     215,   242,   562,   225,   677,   606,   476,   270,   271,   212,
     477,   341,   708,   586,   299,   300,   706,   302,   304,   577,
     578,   450,   207,   574,   848,   584,   660,   389,   320,   207,
     488,   323,   521,   264,   326,   308,   207,   294,   327,   328,
     305,   704,   230,    16,   312,   313,   314,   552,   344,   608,
     233,   234,   287,   207,   570,   842,   471,   346,   892,   370,
     346,   334,   963,   752,   372,   763,   326,   731,   904,    11,
     327,   328,   321,   322,   521,   324,   325,   534,   331,   288,
     333,   505,   535,   338,   508,   244,   221,     7,   233,   234,
     267,     8,   521,   233,   234,    13,   268,   222,   223,   595,
     905,   768,   289,   290,   928,   597,   596,   -14,   599,   326,
       1,     2,   598,   327,   328,   600,   255,   408,   409,     9,
     411,   843,   231,   319,   601,   221,   335,   964,   939,   684,
      14,   602,   329,   265,    15,   309,   222,   223,   851,   747,
     852,   553,   153,   777,   238,   918,   748,   347,   571,   295,
     778,   152,   245,   357,   456,   373,   255,   753,   783,   764,
     353,   354,   355,   244,   542,   246,   543,   544,   155,   272,
     273,   274,   275,   276,   244,   277,   237,   278,   279,   280,
     281,   282,   368,   256,   156,   826,   858,   283,   221,   208,
     857,   489,   206,   576,   317,   221,   208,   209,   210,   222,
     223,   200,   207,    70,   209,   210,   222,   223,   239,   240,
     157,   209,   210,   208,   214,   856,   208,   478,   284,   361,
     216,   362,   158,   256,   728,   363,   364,   365,   209,   210,
     245,   209,   210,   211,   700,   576,   699,   497,   498,   607,
     211,   245,   159,   246,   509,   510,   247,   211,   795,   255,
     559,   586,   248,   576,   246,   519,   520,   770,   779,   781,
     524,   525,   160,   691,   211,   780,   782,   211,   233,   234,
     564,   547,   161,   565,   494,   249,   250,   257,   524,   525,
     162,   455,   219,   258,   484,   563,   163,   461,   462,   511,
     463,   464,   465,   466,   467,   744,   518,   551,   711,   164,
     528,   556,   557,   558,   531,   585,   366,   347,   479,   620,
     516,   766,   741,   621,   622,   711,   256,   257,   772,   773,
     485,   845,   389,   258,   247,   220,   233,   234,   603,   496,
     248,   165,   233,   234,   503,   247,   235,   236,   507,   460,
     861,   248,   166,   671,   259,   260,   167,   643,   168,   367,
     169,   237,   580,   249,   250,   170,   593,   652,   653,   533,
     883,   298,   368,   353,   354,   355,   369,   171,   221,   623,
     624,   625,   172,   769,   173,   233,   234,   209,   210,   222,
     223,   235,   236,   239,   240,   289,   290,   174,   233,   234,
     351,   352,   353,   354,   355,   956,   626,   627,   628,   629,
     630,   631,   175,   632,   301,   176,   294,   357,   177,   237,
     257,   178,   895,   211,   605,   237,   258,   179,   633,   634,
     238,   874,   875,   876,   233,   234,   238,   635,   636,   637,
     638,   639,   521,   180,   181,   182,   522,   259,   260,   183,
     184,   239,   240,   185,   640,   186,   641,   239,   240,   702,
     187,   351,   352,   353,   354,   355,   188,   581,   237,   590,
     855,   189,   190,   191,   272,   273,   274,   275,   276,   238,
     277,   237,   278,   279,   280,   281,   282,   192,   516,   233,
     234,   317,   283,   226,   227,   228,   229,   293,   193,   194,
     239,   240,   757,   195,   197,   306,   307,   345,   198,   233,
     234,   591,   199,   239,   240,   205,   217,   237,   351,   352,
     353,   354,   355,   284,   343,   342,   348,   339,   303,   378,
     211,   693,   351,   352,   353,   354,   355,   787,   788,   789,
     790,   759,   792,   793,   794,   371,   381,   349,   701,   239,
     240,   732,   382,   383,   384,   735,   385,   386,   738,   739,
     351,   352,   353,   354,   355,   272,   273,   274,   275,   276,
     350,   277,   237,   278,   279,   280,   281,   282,   791,   388,
     389,   394,   390,   283,   816,   272,   273,   274,   275,   276,
     391,   277,   237,   278,   279,   280,   281,   282,   442,   392,
     395,   406,   407,   523,   239,   240,   733,   734,   934,   398,
     737,   796,   797,   399,   284,   942,  -212,   401,   402,   351,
     352,   353,   354,   355,   239,   240,   403,   404,   524,   525,
     405,   410,   418,   412,   284,   413,   822,   414,   419,   823,
     798,   799,   416,   800,   421,   420,   958,   351,   872,   353,
     354,   355,   351,   352,   353,   354,   355,   422,   862,   424,
     423,   425,   887,   426,   801,   427,   802,   803,   804,   805,
     806,   807,   808,   809,   810,   811,   812,   428,   429,   820,
     233,   234,   430,   431,   901,   902,   903,   432,    16,   830,
     351,   352,   353,   354,   355,   908,    17,   433,   909,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,   221,
     434,   436,    28,    29,    30,    31,    32,   437,    33,   438,
     222,   223,   439,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,   440,    45,    46,    47,    48,    49,
      50,    51,    52,    53,   441,    54,   443,   444,   445,   351,
     352,   353,   354,   355,   446,   447,   272,   273,   274,   275,
     276,   448,   277,   237,   278,   279,   280,   281,   282,    55,
     449,   824,   454,   870,   283,   452,   459,   453,   457,    56,
     468,   610,   469,   470,   480,    57,   616,    58,    59,   481,
      60,   482,    61,    62,    63,   239,   240,    64,    65,   487,
      66,   491,    67,   492,   493,   284,   495,   500,   502,   501,
     962,   504,   506,   512,   513,   514,   515,   529,   532,   969,
     537,   910,    68,   538,    69,   351,   352,   353,   354,   355,
     351,   352,   353,   354,   355,   351,   352,   353,   354,   355,
     945,   619,   530,   539,   540,   541,   647,   548,    70,   549,
     550,   666,   555,   560,    71,    72,   827,   567,   568,   569,
     955,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,   573,
    -168,   583,   594,   611,   609,   612,   614,   615,   618,   646,
     351,   352,   353,   354,   355,   351,   352,   353,   354,   355,
     351,   352,   353,   354,   355,   351,   352,   353,   354,   355,
      72,   649,   650,   651,   655,   654,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,   201,    74,   657,    76,   658,    78,
      73,    80,    75,    82,    77,    84,    79,    86,    81,    88,
      83,    90,    85,   656,    87,   659,    89,   661,   665,   662,
     669,   663,   664,   667,   673,   668,   670,   675,   672,   674,
     682,   671,   676,   678,   679,   681,   683,   685,   686,   687,
     688,   689,   660,   690,   692,   695,   696,   698,   697,   288,
     709,   710,   711,   714,   713,   715,   716,   712,   717,   718,
     719,   720,   721,   722,   723,   724,   726,   727,   725,   729,
     730,   516,   736,   740,   741,   743,   744,   745,   746,   750,
     751,   755,   756,   758,   760,   761,   762,   771,   784,   817,
     818,   831,   835,   785,   786,   821,   825,   828,   829,   832,
     838,   834,   846,   837,   836,   847,   840,   841,   849,   850,
     860,   867,   863,   859,   864,   865,   866,   868,   869,   871,
     873,   879,   881,   894,   882,   884,   885,   900,   888,   891,
     889,   890,   896,   899,   911,   907,   912,   897,   913,   915,
     916,   919,   920,   922,   925,   923,   926,   927,   930,   936,
     932,   937,   938,   940,   941,   946,   948,   943,   949,   950,
     951,   959,   960,   968,    10,   483,   561,   202,   953,   954,
     957,   961,   966,   898,   196,   380,   970,   340,   332,   886,
     893,   617,   839,   917,   947,   254,   742,   931,   815,   486,
     592,   613,   604,   644,   775,     0,   415
};

static const yytype_int16 yycheck[] =
{
      94,   158,   441,   155,   555,   481,   369,   164,   165,    70,
     369,   200,   607,   458,   171,   172,   605,   174,   175,   452,
     453,   337,     4,   449,   765,    70,     3,     4,   185,     4,
       3,   188,     3,    70,     4,    70,     4,    52,     8,     9,
     175,   603,     4,     6,   179,   180,   181,    70,   205,   482,
      70,    71,   166,     4,    70,     4,     4,     8,   848,   216,
       8,     5,     5,    70,   216,    70,     4,   654,   138,    99,
       8,     9,   186,   187,     3,   189,   190,    65,   192,   164,
     194,   397,    70,   197,   400,     3,   170,   163,    70,    71,
      64,   163,     3,    70,    71,   170,    70,   181,   182,   162,
     170,   688,   187,   188,   162,   162,   169,   165,   162,     4,
      15,    16,   169,     8,     9,   169,     3,   274,   275,     0,
     277,    70,    84,   184,   162,   170,    70,    70,   918,   568,
     170,   169,    70,   170,   163,   170,   181,   182,   119,    70,
     121,   164,   100,   162,   164,   886,    77,   208,   164,   164,
     169,   162,    70,   214,   343,   216,     3,   164,   709,   164,
     218,   219,   220,     3,    70,    83,    72,    73,   163,   146,
     147,   148,   149,   150,     3,   152,   153,   154,   155,   156,
     157,   158,   164,    70,   163,   736,   781,   164,   170,   164,
     779,   164,   172,   164,   164,   170,   164,   179,   180,   181,
     182,   166,     4,   166,   179,   180,   181,   182,   185,   186,
     163,   179,   180,   164,   164,   777,   164,   369,   195,    21,
     221,    23,   163,    70,   650,    27,    28,    29,   179,   180,
      70,   179,   180,   215,   597,   164,   595,    75,    76,   168,
     215,    70,   163,    83,   401,   402,   164,   215,   724,     3,
     439,   696,   170,   164,    83,   412,   413,   690,   162,   162,
     189,   190,   163,   579,   215,   169,   169,   215,    70,    71,
      67,   428,   163,    70,   388,   193,   194,   164,   189,   190,
     163,   342,     3,   170,   378,   442,   163,   348,   349,   403,
     351,   352,   353,   354,   355,   162,   410,   432,   165,   163,
     414,   436,   437,   438,   418,   457,   108,   368,   369,     4,
      69,    70,   162,     8,     9,   165,    70,   164,   175,   176,
     381,   760,     4,   170,   164,   162,    70,    71,   168,   390,
     170,   163,    70,    71,   395,   164,    74,    75,   399,   165,
     163,   170,   163,   166,   191,   192,   163,   504,   163,   151,
     163,   153,   117,   193,   194,   163,   470,   514,   515,   420,
     836,   170,   164,   218,   219,   220,   168,   163,   170,    64,
      65,    66,   163,   689,   163,    70,    71,   179,   180,   181,
     182,    74,    75,   185,   186,   187,   188,   163,    70,    71,
     216,   217,   218,   219,   220,   946,    91,    92,    93,    94,
      95,    96,   163,    98,    81,   163,    52,   468,   163,   153,
     164,   163,   851,   215,   168,   153,   170,   163,   113,   114,
     164,    38,    39,    40,    70,    71,   164,   122,   123,   124,
     125,   126,     3,   163,   163,   163,     7,   191,   192,   163,
     163,   185,   186,   163,   139,   163,   141,   185,   186,   601,
     163,   216,   217,   218,   219,   220,   163,   222,   153,   167,
     776,   163,   163,   163,   146,   147,   148,   149,   150,   164,
     152,   153,   154,   155,   156,   157,   158,   163,    69,    70,
      71,   164,   164,    10,    11,    12,    13,   168,   163,   163,
     185,   186,   681,   163,   163,   176,   177,   164,   163,    70,
      71,   167,   163,   185,   186,   163,   163,   153,   216,   217,
     218,   219,   220,   195,   166,   221,   166,   198,   164,   168,
     215,   582,   216,   217,   218,   219,   220,   716,   717,   718,
     719,   683,   721,   722,   723,   216,   166,   144,   599,   185,
     186,   655,   222,   162,   162,   659,   162,   161,   662,   663,
     216,   217,   218,   219,   220,   146,   147,   148,   149,   150,
     167,   152,   153,   154,   155,   156,   157,   158,   720,   164,
       4,     3,   166,   164,   726,   146,   147,   148,   149,   150,
     162,   152,   153,   154,   155,   156,   157,   158,   162,   162,
     166,   272,   273,   164,   185,   186,   657,   658,   914,     3,
     661,    72,    73,   166,   195,   921,   168,   162,   162,   216,
     217,   218,   219,   220,   185,   186,   162,   162,   189,   190,
     162,   164,   224,   164,   195,   164,   162,   164,     4,   165,
     101,   102,   164,   104,   162,   166,   952,   216,   217,   218,
     219,   220,   216,   217,   218,   219,   220,   162,   800,     4,
     164,   162,   841,   162,   125,   162,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   162,   162,   730,
      70,    71,     4,   162,   863,   864,   865,   162,     6,   740,
     216,   217,   218,   219,   220,   162,    14,   162,   165,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,   170,
       6,   162,    30,    31,    32,    33,    34,   162,    36,   162,
     181,   182,   162,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,     4,    53,    54,    55,    56,    57,
      58,    59,    60,    61,   162,    63,   162,   162,   162,   216,
     217,   218,   219,   220,   162,   162,   146,   147,   148,   149,
     150,   162,   152,   153,   154,   155,   156,   157,   158,    87,
     162,   162,   167,   824,   164,   162,   165,   162,   162,    97,
     162,   167,   165,   163,   163,   103,   167,   105,   106,   163,
     108,   163,   110,   111,   112,   185,   186,   115,   116,     3,
     118,     3,   120,   162,   167,   195,   162,     5,   162,   171,
     957,   162,   162,    69,    69,   162,   162,   159,   162,   966,
      35,   872,   140,     4,   142,   216,   217,   218,   219,   220,
     216,   217,   218,   219,   220,   216,   217,   218,   219,   220,
     924,   167,   165,   162,    35,    37,   167,    78,   166,   162,
      78,   167,   162,   162,   172,   173,   167,    82,   164,    88,
     944,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,     4,
     107,   167,   162,     3,   169,   162,   165,     4,   170,   170,
     216,   217,   218,   219,   220,   216,   217,   218,   219,   220,
     216,   217,   218,   219,   220,   216,   217,   218,   219,   220,
     173,   162,   162,   162,   162,   223,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   173,   180,   162,   182,   162,   184,
     179,   186,   181,   188,   183,   190,   185,   192,   187,   194,
     189,   196,   191,   165,   193,   162,   195,   166,     4,   162,
       4,   162,   162,   162,     4,   162,   162,     4,   162,   162,
      69,   166,   162,   162,   162,   162,   162,   162,     4,   162,
     162,   107,     3,   162,   222,   163,   162,     4,   165,   164,
     162,     3,   165,   163,   165,   163,   163,   171,   163,   163,
     163,   163,   163,   163,   163,   163,   162,    68,   165,     5,
     162,    69,   162,   164,   162,     4,   162,   139,     4,    78,
     162,   170,    78,   162,   162,    85,   162,   168,   162,   162,
     162,     4,     4,    90,    89,   165,   165,   165,   165,   162,
       4,   162,     4,   167,   163,     4,   162,   162,   162,   162,
       5,     4,   163,   165,   163,   163,   163,     3,    38,   165,
     162,    86,     4,     7,   162,   165,   162,    70,   165,   162,
     165,   165,   163,   162,     4,   165,   162,   169,   163,   162,
       4,   162,     4,   162,     5,   163,   223,    38,   165,     4,
      85,   165,   165,    80,   165,   162,   165,    68,   160,   165,
     165,    79,     4,   965,     5,   377,   441,    65,   162,   162,
     162,   162,   162,   854,    59,   220,   165,   199,   193,   840,
     849,   499,   753,   885,   926,   160,   667,   912,   726,   382,
     468,   492,   480,   504,   696,    -1,   281
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   226,   227,   228,   229,   163,   163,     0,
     227,    99,   236,   170,   170,   163,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    30,    31,
      32,    33,    34,    36,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    63,    87,    97,   103,   105,   106,
     108,   110,   111,   112,   115,   116,   118,   120,   140,   142,
     166,   172,   173,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   230,   231,   238,   240,   241,   242,   243,   245,   246,
     254,   257,   258,   260,   261,   268,   270,   271,   273,   275,
     277,   281,   282,   283,   286,   288,   295,   300,   303,   309,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     325,   326,   327,   328,   329,   331,   332,   333,   334,   335,
     336,   337,   338,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   162,   100,   237,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   260,   163,   163,   163,
     166,   173,   242,   244,   262,   163,   172,     4,   164,   179,
     180,   215,   377,   387,   164,   238,   221,   163,   272,     3,
     162,   170,   181,   182,   363,   376,    10,    11,    12,    13,
       4,    84,   388,    70,    71,    74,    75,   153,   164,   185,
     186,   301,   365,   366,     3,    70,    83,   164,   170,   193,
     194,   311,   374,   375,   311,     3,    70,   164,   170,   191,
     192,   305,   372,   373,    70,   170,   350,    64,    70,   357,
     365,   365,   146,   147,   148,   149,   150,   152,   154,   155,
     156,   157,   158,   164,   195,   365,   385,   386,   164,   187,
     188,   368,   369,   368,    52,   164,   395,   396,   170,   365,
     365,    81,   365,   164,   365,   395,   368,   368,    70,   170,
     285,   351,   395,   395,   395,   364,   377,   164,   284,   377,
     365,   386,   386,   365,   386,   386,     4,     8,     9,    70,
     389,   386,   285,   386,     5,    70,   296,   278,   386,   368,
     241,   364,   221,   166,   365,   164,     8,   377,   166,   144,
     167,   216,   217,   218,   219,   220,   376,   377,   382,   383,
     384,    21,    23,    27,    28,    29,   108,   151,   164,   168,
     365,   368,   376,   377,   391,   392,   393,   276,   168,   239,
     237,   166,   222,   162,   162,   162,   161,   367,   164,     4,
     166,   162,   162,   290,     3,   166,   312,   310,     3,   166,
     304,   162,   162,   162,   162,   162,   368,   368,   365,   365,
     164,   365,   164,   164,   164,   385,   164,   386,   224,     4,
     166,   162,   162,   164,     4,   162,   162,   162,   162,   162,
       4,   162,   162,   162,     6,   259,   162,   162,   162,   162,
       4,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     239,   279,   162,   162,   167,   377,   364,   162,   255,   165,
     165,   377,   377,   377,   377,   377,   377,   377,   162,   165,
     163,     4,   250,   251,   252,   253,   366,   369,   376,   377,
     163,   163,   163,   240,   238,   377,   363,     3,     3,   164,
     330,     3,   162,   167,   386,   162,   377,    75,    76,   302,
       5,   171,   162,   377,   162,   239,   162,   377,   239,   365,
     365,   386,    69,    69,   162,   162,    69,   298,   386,   365,
     365,     3,     7,   164,   189,   190,   370,   371,   386,   159,
     165,   386,   162,   377,    65,    70,   358,    35,     4,   162,
      35,    37,    70,    72,    73,   359,   362,   365,    78,   162,
      78,   395,    70,   164,   291,   162,   395,   395,   395,   364,
     162,   284,   389,   365,    67,    70,   394,    82,   164,    88,
      70,   164,   292,     4,   357,   280,   164,   370,   370,   274,
     117,   222,   269,   167,    70,   376,   243,   379,   380,   381,
     167,   167,   382,   386,   162,   162,   169,   162,   169,   162,
     169,   162,   169,   168,   374,   168,   372,   168,   370,   169,
     167,     3,   162,   367,   165,     4,   167,   290,   170,   167,
       4,     8,     9,    64,    65,    66,    91,    92,    93,    94,
      95,    96,    98,   113,   114,   122,   123,   124,   125,   126,
     139,   141,   313,   365,   387,   390,   170,   167,   306,   162,
     162,   162,   365,   365,   223,   162,   165,   162,   162,   162,
       3,   166,   162,   162,   162,     4,   167,   162,   162,     4,
     162,   166,   162,     4,   162,     4,   162,   358,   162,   162,
     324,   162,    69,   162,   389,   162,     4,   162,   162,   107,
     162,   239,   222,   377,   263,   163,   162,   165,     4,   369,
     366,   377,   376,   247,   375,   248,   373,   249,   371,   162,
       3,   165,   171,   165,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   165,   162,    68,   357,     5,
     162,   298,   386,   377,   377,   386,   162,   377,   386,   386,
     164,   162,   351,     4,   162,   139,     4,    70,    77,   361,
      78,   162,    70,   164,   293,   170,    78,   364,   162,   376,
     162,    85,   162,    70,   164,   294,    70,   297,   298,   239,
     370,   168,   175,   176,   378,   379,   256,   162,   169,   162,
     169,   162,   169,   358,   162,    90,    89,   364,   364,   364,
     364,   376,   364,   364,   364,   372,    72,    73,   101,   102,
     104,   125,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   307,   360,   362,   376,   162,   162,   234,
     377,   165,   162,   165,   162,   165,   358,   167,   165,   165,
     377,     4,   162,   352,   162,     4,   163,   167,     4,   301,
     162,   162,     4,    70,   299,   389,     4,     4,   352,   162,
     162,   119,   121,   264,   265,   239,   375,   373,   371,   165,
       5,   163,   376,   163,   163,   163,   163,     4,     3,    38,
     377,   165,   217,   162,    38,    39,    40,   353,   354,    86,
     355,     4,   162,   372,   165,   162,   294,   364,   165,   165,
     165,   162,   355,   299,     7,   389,   163,   169,   264,   162,
      70,   364,   364,   364,   138,   170,   308,   165,   162,   165,
     377,     4,   162,   163,   339,   162,     4,   302,   352,   162,
       4,   287,   162,   163,   267,     5,   223,    38,   162,   232,
     165,   353,    85,   356,   239,   340,     4,   165,   165,   355,
      80,   165,   239,    68,   266,   238,   162,   308,   165,   160,
     165,   165,   289,   162,   162,   238,   358,   162,   239,    79,
       4,   162,   365,     5,    70,   235,   162,   233,   234,   365,
     165
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
#line 1785 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1791 "lev_comp.y"
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

  case 266:

/* Line 1455 of yacc.c  */
#line 1820 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1825 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1833 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1839 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1845 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1852 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1859 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1869 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1879 "lev_comp.y"
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

  case 275:

/* Line 1455 of yacc.c  */
#line 1894 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1904 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1908 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1914 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1920 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1926 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int)(yyvsp[(6) - (13)].i), (int)(yyvsp[(8) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1932 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1938 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1944 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1949 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1956 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1963 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1969 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1975 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1981 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1985 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1991 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1997 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 2007 "lev_comp.y"
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

  case 294:

/* Line 1455 of yacc.c  */
#line 2021 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      if ( (yyvsp[(10) - (11)].i) ) {
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      } else if ( (yyvsp[(11) - (11)].i) )
			  lc_error("Cannot use lev statements in non-permanent REGION");
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2031 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2035 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2041 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2048 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2054 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2060 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2066 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2072 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2076 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2081 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2088 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2094 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2101 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2105 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2112 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2123 "lev_comp.y"
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

  case 313:

/* Line 1455 of yacc.c  */
#line 2136 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2140 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2146 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2150 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2157 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2161 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2165 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2171 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2175 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2181 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2197 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2205 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2215 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2224 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2229 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2236 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2247 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2253 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2257 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2261 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2268 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2278 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2285 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2289 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2295 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2299 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2307 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2311 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2318 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2328 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2341 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2345 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2352 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2362 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2371 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2382 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2386 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2393 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2403 "lev_comp.y"
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

  case 362:

/* Line 1455 of yacc.c  */
#line 2414 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2423 "lev_comp.y"
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

  case 364:

/* Line 1455 of yacc.c  */
#line 2433 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2439 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2443 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2450 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2460 "lev_comp.y"
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

  case 369:

/* Line 1455 of yacc.c  */
#line 2471 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2480 "lev_comp.y"
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

  case 371:

/* Line 1455 of yacc.c  */
#line 2490 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2494 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2500 "lev_comp.y"
    { ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2502 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2508 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2512 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2516 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2520 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2528 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2537 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2541 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2545 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2549 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2553 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2556 "lev_comp.y"
    { ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2560 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2567 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2576 "lev_comp.y"
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

  case 393:

/* Line 1455 of yacc.c  */
#line 2617 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2621 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2628 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2635 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2645 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2649 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2657 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2661 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2665 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2669 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2673 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2678 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2682 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2686 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2691 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2696 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2701 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2705 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2710 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2715 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2719 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2723 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2728 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2732 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2739 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2745 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2749 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2755 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2771 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2775 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2779 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2783 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2805 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2809 "lev_comp.y"
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

  case 440:

/* Line 1455 of yacc.c  */
#line 2835 "lev_comp.y"
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
#line 6408 "lev_comp.tab.c"
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
#line 2863 "lev_comp.y"


/*lev_comp.y*/

