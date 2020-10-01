
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
     LEVER_ID = 325,
     DIRECTION = 326,
     RANDOM_TYPE = 327,
     RANDOM_TYPE_BRACKET = 328,
     A_REGISTER = 329,
     ALIGNMENT = 330,
     LEFT_OR_RIGHT = 331,
     CENTER = 332,
     TOP_OR_BOT = 333,
     ALTAR_TYPE = 334,
     UP_OR_DOWN = 335,
     ACTIVE_OR_INACTIVE = 336,
     MODRON_PORTAL_TYPE = 337,
     NPC_TYPE = 338,
     FOUNTAIN_TYPE = 339,
     SPECIAL_OBJECT_TYPE = 340,
     CMAP_TYPE = 341,
     FLOOR_TYPE = 342,
     FLOOR_TYPE_ID = 343,
     FLOOR_ID = 344,
     FLOOR_MAIN_TYPE = 345,
     ELEMENTAL_ENCHANTMENT_TYPE = 346,
     EXCEPTIONALITY_TYPE = 347,
     EXCEPTIONALITY_ID = 348,
     ELEMENTAL_ENCHANTMENT_ID = 349,
     ENCHANTMENT_ID = 350,
     CHARGES_ID = 351,
     SPECIAL_QUALITY_ID = 352,
     SPEFLAGS_ID = 353,
     SUBROOM_ID = 354,
     NAME_ID = 355,
     FLAGS_ID = 356,
     FLAG_TYPE = 357,
     MON_ATTITUDE = 358,
     MON_ALERTNESS = 359,
     SUBTYPE_ID = 360,
     MON_APPEARANCE = 361,
     ROOMDOOR_ID = 362,
     IF_ID = 363,
     ELSE_ID = 364,
     TERRAIN_ID = 365,
     HORIZ_OR_VERT = 366,
     REPLACE_TERRAIN_ID = 367,
     EXIT_ID = 368,
     SHUFFLE_ID = 369,
     QUANTITY_ID = 370,
     BURIED_ID = 371,
     LOOP_ID = 372,
     FOR_ID = 373,
     TO_ID = 374,
     SWITCH_ID = 375,
     CASE_ID = 376,
     BREAK_ID = 377,
     DEFAULT_ID = 378,
     ERODED_ID = 379,
     TRAPPED_STATE = 380,
     RECHARGED_ID = 381,
     INVIS_ID = 382,
     GREASED_ID = 383,
     FEMALE_ID = 384,
     WAITFORU_ID = 385,
     CANCELLED_ID = 386,
     REVIVED_ID = 387,
     AVENGE_ID = 388,
     FLEEING_ID = 389,
     BLINDED_ID = 390,
     PARALYZED_ID = 391,
     STUNNED_ID = 392,
     CONFUSED_ID = 393,
     SEENTRAPS_ID = 394,
     ALL_ID = 395,
     MONTYPE_ID = 396,
     OBJTYPE_ID = 397,
     TERTYPE_ID = 398,
     TERTYPE2_ID = 399,
     LEVER_EFFECT_TYPE = 400,
     SWITCHABLE_ID = 401,
     CONTINUOUSLY_USABLE_ID = 402,
     TARGET_ID = 403,
     TRAPTYPE_ID = 404,
     EFFECT_FLAG_ID = 405,
     GRAVE_ID = 406,
     ERODEPROOF_ID = 407,
     FUNCTION_ID = 408,
     MSG_OUTPUT_TYPE = 409,
     COMPARE_TYPE = 410,
     UNKNOWN_TYPE = 411,
     rect_ID = 412,
     fillrect_ID = 413,
     line_ID = 414,
     randline_ID = 415,
     grow_ID = 416,
     selection_ID = 417,
     flood_ID = 418,
     rndcoord_ID = 419,
     circle_ID = 420,
     ellipse_ID = 421,
     filter_ID = 422,
     complement_ID = 423,
     gradient_ID = 424,
     GRADIENT_TYPE = 425,
     LIMITED = 426,
     HUMIDITY_TYPE = 427,
     STRING = 428,
     MAP_ID = 429,
     NQSTRING = 430,
     VARSTRING = 431,
     CFUNC = 432,
     CFUNC_INT = 433,
     CFUNC_STR = 434,
     CFUNC_COORD = 435,
     CFUNC_REGION = 436,
     VARSTRING_INT = 437,
     VARSTRING_INT_ARRAY = 438,
     VARSTRING_STRING = 439,
     VARSTRING_STRING_ARRAY = 440,
     VARSTRING_VAR = 441,
     VARSTRING_VAR_ARRAY = 442,
     VARSTRING_COORD = 443,
     VARSTRING_COORD_ARRAY = 444,
     VARSTRING_REGION = 445,
     VARSTRING_REGION_ARRAY = 446,
     VARSTRING_MAPCHAR = 447,
     VARSTRING_MAPCHAR_ARRAY = 448,
     VARSTRING_MONST = 449,
     VARSTRING_MONST_ARRAY = 450,
     VARSTRING_OBJ = 451,
     VARSTRING_OBJ_ARRAY = 452,
     VARSTRING_SEL = 453,
     VARSTRING_SEL_ARRAY = 454,
     METHOD_INT = 455,
     METHOD_INT_ARRAY = 456,
     METHOD_STRING = 457,
     METHOD_STRING_ARRAY = 458,
     METHOD_VAR = 459,
     METHOD_VAR_ARRAY = 460,
     METHOD_COORD = 461,
     METHOD_COORD_ARRAY = 462,
     METHOD_REGION = 463,
     METHOD_REGION_ARRAY = 464,
     METHOD_MAPCHAR = 465,
     METHOD_MAPCHAR_ARRAY = 466,
     METHOD_MONST = 467,
     METHOD_MONST_ARRAY = 468,
     METHOD_OBJ = 469,
     METHOD_OBJ_ARRAY = 470,
     METHOD_SEL = 471,
     METHOD_SEL_ARRAY = 472,
     DICE = 473
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
#line 516 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 528 "lev_comp.tab.c"

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
#define YYLAST   1168

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  236
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  175
/* YYNRULES -- Number of rules.  */
#define YYNRULES  456
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1010

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   473

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   231,   235,     2,
     175,   176,   229,   227,   173,   228,   233,   230,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   174,     2,
       2,   232,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   177,     2,   178,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   179,   234,   180,     2,     2,     2,     2,
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
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226
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
     239,   241,   243,   245,   247,   249,   251,   253,   257,   261,
     267,   271,   277,   283,   289,   293,   297,   303,   309,   315,
     323,   331,   339,   345,   347,   351,   353,   357,   359,   363,
     365,   369,   371,   375,   377,   381,   383,   387,   388,   389,
     398,   403,   405,   406,   408,   410,   416,   420,   421,   422,
     432,   433,   436,   437,   443,   444,   449,   451,   454,   456,
     463,   464,   468,   469,   476,   477,   482,   483,   488,   490,
     491,   496,   500,   502,   506,   510,   516,   522,   530,   535,
     536,   548,   549,   563,   564,   567,   573,   575,   581,   583,
     589,   591,   597,   599,   609,   623,   629,   639,   641,   643,
     645,   647,   649,   653,   655,   657,   659,   667,   673,   675,
     677,   679,   681,   685,   686,   692,   697,   698,   702,   704,
     706,   708,   710,   713,   715,   717,   719,   721,   723,   727,
     731,   735,   737,   739,   743,   745,   747,   749,   753,   757,
     758,   764,   767,   768,   772,   774,   778,   780,   784,   788,
     790,   792,   796,   798,   800,   802,   806,   808,   810,   812,
     816,   820,   824,   828,   832,   836,   840,   846,   854,   860,
     869,   871,   875,   881,   887,   895,   903,   910,   916,   917,
     920,   926,   930,   944,   951,   952,   956,   958,   962,   966,
     970,   974,   976,   978,   982,   986,   990,   994,   998,  1002,
    1004,  1010,  1020,  1026,  1030,  1034,  1040,  1050,  1056,  1062,
    1063,  1075,  1076,  1078,  1086,  1090,  1098,  1104,  1110,  1116,
    1122,  1126,  1132,  1140,  1150,  1152,  1154,  1156,  1158,  1160,
    1161,  1164,  1166,  1170,  1172,  1174,  1176,  1177,  1181,  1183,
    1185,  1187,  1189,  1191,  1193,  1195,  1197,  1199,  1201,  1205,
    1207,  1209,  1214,  1216,  1218,  1223,  1225,  1227,  1232,  1234,
    1239,  1245,  1247,  1251,  1253,  1257,  1259,  1261,  1266,  1276,
    1278,  1280,  1285,  1287,  1293,  1295,  1297,  1302,  1304,  1306,
    1312,  1314,  1316,  1318,  1323,  1325,  1327,  1333,  1335,  1337,
    1339,  1343,  1345,  1347,  1351,  1353,  1358,  1362,  1366,  1370,
    1374,  1378,  1382,  1384,  1386,  1390,  1392,  1396,  1397,  1399,
    1401,  1403,  1405,  1409,  1410,  1412,  1414,  1417,  1420,  1425,
    1432,  1437,  1444,  1451,  1458,  1465,  1468,  1475,  1484,  1493,
    1504,  1519,  1522,  1524,  1528,  1530,  1534,  1536,  1538,  1540,
    1542,  1544,  1546,  1548,  1550,  1552,  1554,  1556,  1558,  1560,
    1562,  1564,  1566,  1568,  1570,  1572,  1583
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     237,     0,    -1,    -1,   238,    -1,   239,    -1,   239,   238,
      -1,   240,   247,   249,    -1,    16,   174,   181,    -1,    15,
     174,   181,   173,     3,    -1,    17,   174,    11,   173,   344,
      -1,    17,   174,    10,   173,     3,    -1,    17,   174,    13,
      -1,    17,   174,    12,   173,     3,   173,     3,   173,     5,
     173,     5,   173,   372,   173,   246,   245,    -1,    18,   174,
     402,    -1,    -1,   173,   171,    -1,    -1,   173,   379,    -1,
      -1,   173,     3,    -1,     5,    -1,    72,    -1,    -1,   101,
     174,   248,    -1,   102,   173,   248,    -1,   102,    -1,    -1,
     251,   249,    -1,   179,   249,   180,    -1,   292,    -1,   241,
      -1,   242,    -1,   358,    -1,   357,    -1,   355,    -1,   356,
      -1,   359,    -1,   360,    -1,   334,    -1,   294,    -1,   257,
      -1,   256,    -1,   347,    -1,   306,    -1,   326,    -1,   362,
      -1,   363,    -1,   336,    -1,   337,    -1,   338,    -1,   361,
      -1,   272,    -1,   282,    -1,   284,    -1,   288,    -1,   286,
      -1,   269,    -1,   279,    -1,   265,    -1,   268,    -1,   329,
      -1,   339,    -1,   311,    -1,   327,    -1,   314,    -1,   320,
      -1,   348,    -1,   343,    -1,   332,    -1,   293,    -1,   349,
      -1,   350,    -1,   351,    -1,   352,    -1,   299,    -1,   297,
      -1,   342,    -1,   346,    -1,   345,    -1,   330,    -1,   331,
      -1,   333,    -1,   325,    -1,   328,    -1,   191,    -1,   193,
      -1,   195,    -1,   197,    -1,   199,    -1,   201,    -1,   203,
      -1,   205,    -1,   207,    -1,   190,    -1,   192,    -1,   194,
      -1,   196,    -1,   198,    -1,   200,    -1,   202,    -1,   204,
      -1,   206,    -1,   252,    -1,   253,    -1,   184,    -1,   184,
      -1,   253,    -1,   114,   174,   252,    -1,   254,   232,   391,
      -1,   254,   232,   162,   174,   400,    -1,   254,   232,   390,
      -1,   254,   232,   407,   174,   384,    -1,   254,   232,   406,
     174,   386,    -1,   254,   232,   405,   174,   388,    -1,   254,
     232,   379,    -1,   254,   232,   382,    -1,   254,   232,   179,
     263,   180,    -1,   254,   232,   179,   262,   180,    -1,   254,
     232,   179,   261,   180,    -1,   254,   232,   407,   174,   179,
     260,   180,    -1,   254,   232,   406,   174,   179,   259,   180,
      -1,   254,   232,   405,   174,   179,   258,   180,    -1,   254,
     232,   179,   264,   180,    -1,   389,    -1,   258,   173,   389,
      -1,   387,    -1,   259,   173,   387,    -1,   385,    -1,   260,
     173,   385,    -1,   383,    -1,   261,   173,   383,    -1,   380,
      -1,   262,   173,   380,    -1,   391,    -1,   263,   173,   391,
      -1,   390,    -1,   264,   173,   390,    -1,    -1,    -1,   153,
     183,   175,   266,   395,   176,   267,   250,    -1,   183,   175,
     398,   176,    -1,   113,    -1,    -1,     6,    -1,     6,    -1,
     177,   391,   155,   391,   178,    -1,   177,   391,   178,    -1,
      -1,    -1,   120,   273,   177,   378,   178,   274,   179,   275,
     180,    -1,    -1,   276,   275,    -1,    -1,   121,   403,   174,
     277,   249,    -1,    -1,   123,   174,   278,   249,    -1,   122,
      -1,   233,   233,    -1,   119,    -1,   118,   255,   232,   391,
     280,   391,    -1,    -1,   281,   283,   250,    -1,    -1,   117,
     177,   378,   178,   285,   250,    -1,    -1,   271,   174,   287,
     251,    -1,    -1,   108,   271,   289,   290,    -1,   250,    -1,
      -1,   250,   291,   109,   250,    -1,    14,   174,   390,    -1,
      63,    -1,    63,   174,   403,    -1,    63,   174,    72,    -1,
      54,   174,   295,   173,   295,    -1,    54,   174,   295,   173,
     403,    -1,   175,     4,   173,    71,   173,   310,   176,    -1,
     365,   270,   173,   372,    -1,    -1,    99,   174,   296,   173,
     303,   173,   305,   366,   369,   298,   250,    -1,    -1,    48,
     174,   296,   173,   302,   173,   304,   173,   305,   366,   369,
     300,   250,    -1,    -1,   173,     5,    -1,   175,     4,   173,
       4,   176,    -1,    72,    -1,   175,     4,   173,     4,   176,
      -1,    72,    -1,   175,   312,   173,   313,   176,    -1,    72,
      -1,   175,     4,   173,     4,   176,    -1,    72,    -1,   107,
     174,   307,   173,   371,   173,   308,   173,   310,    -1,   107,
     174,   307,   173,   371,   173,   308,   173,   310,   173,    68,
     173,     4,    -1,    25,   174,   371,   173,   400,    -1,    25,
     174,   371,   173,   400,   173,    68,   173,     4,    -1,     5,
      -1,    72,    -1,   309,    -1,    72,    -1,    71,    -1,    71,
     234,   309,    -1,     4,    -1,    72,    -1,    20,    -1,    19,
     174,   312,   173,   313,   301,   182,    -1,    19,   174,   379,
     301,   182,    -1,    76,    -1,    77,    -1,    78,    -1,    77,
      -1,    23,   174,   316,    -1,    -1,    23,   174,   316,   315,
     250,    -1,   386,   173,   379,   317,    -1,    -1,   317,   173,
     318,    -1,   390,    -1,   103,    -1,   104,    -1,   374,    -1,
     106,   390,    -1,   129,    -1,   127,    -1,   131,    -1,   132,
      -1,   133,    -1,   134,   174,   378,    -1,   135,   174,   378,
      -1,   136,   174,   378,    -1,   137,    -1,   138,    -1,   139,
     174,   319,    -1,   130,    -1,   181,    -1,   140,    -1,   181,
     234,   319,    -1,    21,   174,   322,    -1,    -1,    22,   174,
     322,   321,   250,    -1,   388,   323,    -1,    -1,   323,   173,
     324,    -1,    66,    -1,   141,   174,   386,    -1,   404,    -1,
     100,   174,   390,    -1,   115,   174,   378,    -1,   116,    -1,
      65,    -1,   124,   174,   378,    -1,   152,    -1,    64,    -1,
     125,    -1,   126,   174,   378,    -1,   127,    -1,   128,    -1,
     379,    -1,    94,   174,    91,    -1,    93,   174,    92,    -1,
      95,   174,   378,    -1,    96,   174,   378,    -1,    97,   174,
     378,    -1,    98,   174,   378,    -1,    69,   174,    68,    -1,
      24,   174,   364,   173,   379,    -1,    26,   174,   379,   173,
      71,   173,   371,    -1,    30,   174,   379,   173,    71,    -1,
      30,   174,   379,   173,    71,   173,     5,   245,    -1,    31,
      -1,    31,   174,   400,    -1,    44,   174,   379,   173,    80,
      -1,    45,   174,   379,   173,    80,    -1,    45,   174,   409,
     173,   409,   173,    80,    -1,    49,   174,   409,   173,   409,
     173,   181,    -1,    50,   174,   409,   173,   409,   335,    -1,
      51,   174,   409,   173,   409,    -1,    -1,   173,    80,    -1,
      57,   174,   400,   173,    84,    -1,    58,   174,   400,    -1,
      59,   174,   379,   173,   175,   403,   173,   403,   176,   173,
      82,   173,    81,    -1,    70,   174,   379,   173,   145,   340,
      -1,    -1,   340,   173,   341,    -1,    81,    -1,   141,   174,
     386,    -1,   142,   174,   388,    -1,   143,   174,   344,    -1,
     144,   174,   344,    -1,   146,    -1,   147,    -1,   148,   174,
     379,    -1,   149,   174,   364,    -1,   150,   174,     4,    -1,
      97,   174,     4,    -1,    61,   174,   400,    -1,    60,   174,
     400,    -1,     3,    -1,   175,     3,   173,   372,   176,    -1,
     112,   174,   382,   173,   384,   173,   384,   173,     7,    -1,
     110,   174,   400,   173,   384,    -1,    46,   174,   382,    -1,
      47,   174,   382,    -1,    36,   174,   181,   173,    37,    -1,
      36,   174,   181,   173,    37,   173,   141,   174,   386,    -1,
      33,   174,   382,   173,    35,    -1,    34,   174,   409,   173,
      35,    -1,    -1,    32,   174,   382,   173,   372,   173,   365,
     366,   369,   353,   354,    -1,    -1,   250,    -1,    41,   174,
     379,   173,   373,   173,   375,    -1,    42,   174,   379,    -1,
      89,   174,   400,   173,    90,   173,    87,    -1,   105,   174,
     400,   173,     4,    -1,    43,   174,    83,   173,   379,    -1,
     151,   174,   379,   173,   390,    -1,   151,   174,   379,   173,
      72,    -1,   151,   174,   379,    -1,    55,   174,   391,   173,
     379,    -1,    56,   174,   379,   173,   408,   173,   390,    -1,
      53,   174,   378,   173,   378,   173,   378,   173,   378,    -1,
      53,    -1,   181,    -1,    72,    -1,   181,    -1,    72,    -1,
      -1,   173,   367,    -1,   368,    -1,   368,   173,   367,    -1,
      38,    -1,    39,    -1,    40,    -1,    -1,    88,   174,   370,
      -1,    87,    -1,    64,    -1,    72,    -1,    65,    -1,    72,
      -1,    75,    -1,   376,    -1,    72,    -1,    75,    -1,   376,
      -1,    74,   174,    72,    -1,    79,    -1,    72,    -1,    74,
     177,     4,   178,    -1,   181,    -1,   192,    -1,   193,   177,
     391,   178,    -1,   391,    -1,   380,    -1,   164,   175,   400,
     176,    -1,   196,    -1,   197,   177,   391,   178,    -1,   175,
       4,   173,     4,   176,    -1,    72,    -1,    73,   381,   178,
      -1,   172,    -1,   172,   173,   381,    -1,   383,    -1,   198,
      -1,   199,   177,   391,   178,    -1,   175,     4,   173,     4,
     173,     4,   173,     4,   176,    -1,   385,    -1,   200,    -1,
     201,   177,   391,   178,    -1,     3,    -1,   175,     3,   173,
     372,   176,    -1,   387,    -1,   202,    -1,   203,   177,   391,
     178,    -1,   181,    -1,     3,    -1,   175,     3,   173,   181,
     176,    -1,    72,    -1,   389,    -1,   204,    -1,   205,   177,
     391,   178,    -1,   181,    -1,     3,    -1,   175,     3,   173,
     181,   176,    -1,    72,    -1,    85,    -1,   377,    -1,   390,
     233,   377,    -1,     4,    -1,   401,    -1,   175,     8,   176,
      -1,   190,    -1,   191,   177,   391,   178,    -1,   391,   227,
     391,    -1,   391,   228,   391,    -1,   391,   229,   391,    -1,
     391,   230,   391,    -1,   391,   231,   391,    -1,   175,   391,
     176,    -1,   186,    -1,   187,    -1,   254,   174,   392,    -1,
     393,    -1,   394,   173,   393,    -1,    -1,   394,    -1,   391,
      -1,   390,    -1,   396,    -1,   397,   173,   396,    -1,    -1,
     397,    -1,   379,    -1,   157,   382,    -1,   158,   382,    -1,
     159,   379,   173,   379,    -1,   160,   379,   173,   379,   173,
     391,    -1,   161,   175,   400,   176,    -1,   161,   175,   309,
     173,   400,   176,    -1,   167,   175,     7,   173,   400,   176,
      -1,   167,   175,   400,   173,   400,   176,    -1,   167,   175,
     384,   173,   400,   176,    -1,   163,   379,    -1,   165,   175,
     379,   173,   391,   176,    -1,   165,   175,   379,   173,   391,
     173,    38,   176,    -1,   166,   175,   379,   173,   391,   173,
     391,   176,    -1,   166,   175,   379,   173,   391,   173,   391,
     173,    38,   176,    -1,   169,   175,   170,   173,   175,   391,
     228,   391,   243,   176,   173,   379,   244,   176,    -1,   168,
     399,    -1,   206,    -1,   175,   400,   176,    -1,   399,    -1,
     399,   235,   400,    -1,   226,    -1,    86,    -1,     4,    -1,
       8,    -1,     9,    -1,     4,    -1,     8,    -1,     9,    -1,
       4,    -1,   401,    -1,    27,    -1,    21,    -1,    28,    -1,
      23,    -1,    29,    -1,   110,    -1,    67,    -1,    72,    -1,
     410,    -1,    52,   175,     4,   173,     4,   173,     4,   173,
       4,   176,    -1,   175,     4,   173,     4,   173,     4,   173,
       4,   176,    -1
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
     531,   532,   533,   534,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   559,   560,   561,   564,   565,   568,   584,   590,
     596,   602,   608,   614,   620,   626,   632,   642,   652,   662,
     672,   682,   692,   704,   709,   716,   721,   728,   733,   740,
     744,   750,   755,   762,   766,   772,   776,   783,   805,   782,
     819,   874,   881,   884,   890,   897,   901,   910,   914,   909,
     977,   978,   982,   981,   995,   994,  1009,  1019,  1020,  1023,
    1061,  1060,  1095,  1094,  1125,  1124,  1157,  1156,  1182,  1193,
    1192,  1220,  1226,  1231,  1236,  1243,  1250,  1259,  1267,  1279,
    1278,  1297,  1296,  1315,  1318,  1324,  1334,  1340,  1349,  1355,
    1360,  1366,  1371,  1377,  1388,  1399,  1403,  1409,  1410,  1413,
    1414,  1417,  1421,  1427,  1428,  1431,  1438,  1446,  1454,  1455,
    1458,  1459,  1462,  1467,  1466,  1480,  1487,  1494,  1502,  1507,
    1513,  1519,  1525,  1531,  1536,  1541,  1546,  1551,  1556,  1561,
    1566,  1571,  1576,  1581,  1587,  1594,  1603,  1607,  1620,  1629,
    1628,  1646,  1656,  1662,  1670,  1676,  1681,  1686,  1691,  1696,
    1701,  1706,  1711,  1716,  1727,  1733,  1738,  1743,  1748,  1753,
    1758,  1763,  1768,  1773,  1778,  1783,  1790,  1796,  1825,  1830,
    1838,  1844,  1850,  1857,  1864,  1874,  1884,  1899,  1910,  1913,
    1919,  1925,  1931,  1937,  1944,  1950,  1958,  1964,  1969,  1974,
    1979,  1984,  1990,  1996,  2001,  2006,  2011,  2018,  2024,  2030,
    2035,  2042,  2049,  2055,  2061,  2067,  2071,  2077,  2083,  2094,
    2093,  2118,  2121,  2127,  2134,  2140,  2146,  2152,  2158,  2162,
    2167,  2174,  2180,  2187,  2191,  2198,  2206,  2209,  2219,  2223,
    2226,  2232,  2236,  2243,  2247,  2251,  2258,  2261,  2267,  2273,
    2274,  2277,  2278,  2281,  2282,  2283,  2289,  2290,  2291,  2297,
    2298,  2301,  2310,  2315,  2322,  2333,  2339,  2343,  2347,  2354,
    2364,  2371,  2375,  2381,  2385,  2393,  2397,  2404,  2414,  2427,
    2431,  2438,  2448,  2457,  2468,  2472,  2479,  2489,  2500,  2509,
    2519,  2525,  2529,  2536,  2546,  2557,  2566,  2576,  2580,  2587,
    2588,  2594,  2598,  2602,  2606,  2614,  2623,  2627,  2631,  2635,
    2639,  2643,  2646,  2653,  2662,  2695,  2696,  2699,  2700,  2703,
    2707,  2714,  2721,  2732,  2735,  2743,  2747,  2751,  2755,  2759,
    2764,  2768,  2772,  2777,  2782,  2787,  2791,  2796,  2801,  2805,
    2809,  2814,  2818,  2825,  2831,  2835,  2841,  2848,  2849,  2852,
    2853,  2854,  2857,  2861,  2865,  2869,  2875,  2876,  2879,  2880,
    2883,  2884,  2887,  2888,  2891,  2895,  2921
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
  "KEYTYPE_ID", "LEVER_ID", "DIRECTION", "RANDOM_TYPE",
  "RANDOM_TYPE_BRACKET", "A_REGISTER", "ALIGNMENT", "LEFT_OR_RIGHT",
  "CENTER", "TOP_OR_BOT", "ALTAR_TYPE", "UP_OR_DOWN", "ACTIVE_OR_INACTIVE",
  "MODRON_PORTAL_TYPE", "NPC_TYPE", "FOUNTAIN_TYPE", "SPECIAL_OBJECT_TYPE",
  "CMAP_TYPE", "FLOOR_TYPE", "FLOOR_TYPE_ID", "FLOOR_ID",
  "FLOOR_MAIN_TYPE", "ELEMENTAL_ENCHANTMENT_TYPE", "EXCEPTIONALITY_TYPE",
  "EXCEPTIONALITY_ID", "ELEMENTAL_ENCHANTMENT_ID", "ENCHANTMENT_ID",
  "CHARGES_ID", "SPECIAL_QUALITY_ID", "SPEFLAGS_ID", "SUBROOM_ID",
  "NAME_ID", "FLAGS_ID", "FLAG_TYPE", "MON_ATTITUDE", "MON_ALERTNESS",
  "SUBTYPE_ID", "MON_APPEARANCE", "ROOMDOOR_ID", "IF_ID", "ELSE_ID",
  "TERRAIN_ID", "HORIZ_OR_VERT", "REPLACE_TERRAIN_ID", "EXIT_ID",
  "SHUFFLE_ID", "QUANTITY_ID", "BURIED_ID", "LOOP_ID", "FOR_ID", "TO_ID",
  "SWITCH_ID", "CASE_ID", "BREAK_ID", "DEFAULT_ID", "ERODED_ID",
  "TRAPPED_STATE", "RECHARGED_ID", "INVIS_ID", "GREASED_ID", "FEMALE_ID",
  "WAITFORU_ID", "CANCELLED_ID", "REVIVED_ID", "AVENGE_ID", "FLEEING_ID",
  "BLINDED_ID", "PARALYZED_ID", "STUNNED_ID", "CONFUSED_ID",
  "SEENTRAPS_ID", "ALL_ID", "MONTYPE_ID", "OBJTYPE_ID", "TERTYPE_ID",
  "TERTYPE2_ID", "LEVER_EFFECT_TYPE", "SWITCHABLE_ID",
  "CONTINUOUSLY_USABLE_ID", "TARGET_ID", "TRAPTYPE_ID", "EFFECT_FLAG_ID",
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
  "tileset_detail", "opt_limited", "opt_coord_or_var", "opt_fillchar",
  "walled", "flags", "flag_list", "levstatements", "stmt_block",
  "levstatement", "any_var_array", "any_var", "any_var_or_arr",
  "any_var_or_unk", "shuffle_detail", "variable_define", "encodeobj_list",
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
  "modron_portal_detail", "lever_detail", "lever_infos", "lever_info",
  "sink_detail", "pool_detail", "terrain_type", "replace_terrain_detail",
  "terrain_detail", "diggable_detail", "passwall_detail", "naming_detail",
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
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,    44,    58,    40,    41,    91,    93,   123,
     125,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,    43,    45,    42,
      47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   236,   237,   237,   238,   238,   239,   240,   240,   241,
     241,   241,   241,   242,   243,   243,   244,   244,   245,   245,
     246,   246,   247,   247,   248,   248,   249,   249,   250,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   254,   254,   254,   255,   255,   256,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   258,   258,   259,   259,   260,   260,   261,
     261,   262,   262,   263,   263,   264,   264,   266,   267,   265,
     268,   269,   270,   270,   271,   271,   271,   273,   274,   272,
     275,   275,   277,   276,   278,   276,   279,   280,   280,   281,
     283,   282,   285,   284,   287,   286,   289,   288,   290,   291,
     290,   292,   293,   293,   293,   294,   294,   295,   296,   298,
     297,   300,   299,   301,   301,   302,   302,   303,   303,   304,
     304,   305,   305,   306,   306,   306,   306,   307,   307,   308,
     308,   309,   309,   310,   310,   311,   311,   311,   312,   312,
     313,   313,   314,   315,   314,   316,   317,   317,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   319,   319,   319,   320,   321,
     320,   322,   323,   323,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   325,   326,   327,   327,
     328,   328,   329,   330,   331,   332,   333,   334,   335,   335,
     336,   337,   338,   339,   340,   340,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   341,   342,   343,   344,
     344,   345,   346,   347,   348,   349,   349,   350,   351,   353,
     352,   354,   354,   355,   356,   357,   358,   359,   360,   360,
     360,   361,   362,   363,   363,   364,   364,   365,   365,   366,
     366,   367,   367,   368,   368,   368,   369,   369,   370,   371,
     371,   372,   372,   373,   373,   373,   374,   374,   374,   375,
     375,   376,   377,   377,   377,   378,   379,   379,   379,   379,
     380,   380,   380,   381,   381,   382,   382,   382,   383,   384,
     384,   384,   385,   385,   386,   386,   386,   387,   387,   387,
     387,   388,   388,   388,   389,   389,   389,   389,   389,   390,
     390,   391,   391,   391,   391,   391,   391,   391,   391,   391,
     391,   391,   392,   392,   393,   394,   394,   395,   395,   396,
     396,   397,   397,   398,   398,   399,   399,   399,   399,   399,
     399,   399,   399,   399,   399,   399,   399,   399,   399,   399,
     399,   399,   399,   399,   400,   400,   401,   402,   402,   403,
     403,   403,   404,   404,   404,   404,   405,   405,   406,   406,
     407,   407,   408,   408,   409,   409,   410
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
       1,     1,     1,     1,     1,     1,     1,     3,     3,     5,
       3,     5,     5,     5,     3,     3,     5,     5,     5,     7,
       7,     7,     5,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     0,     0,     8,
       4,     1,     0,     1,     1,     5,     3,     0,     0,     9,
       0,     2,     0,     5,     0,     4,     1,     2,     1,     6,
       0,     3,     0,     6,     0,     4,     0,     4,     1,     0,
       4,     3,     1,     3,     3,     5,     5,     7,     4,     0,
      11,     0,    13,     0,     2,     5,     1,     5,     1,     5,
       1,     5,     1,     9,    13,     5,     9,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     7,     5,     1,     1,
       1,     1,     3,     0,     5,     4,     0,     3,     1,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     3,     3,
       3,     1,     1,     3,     1,     1,     1,     3,     3,     0,
       5,     2,     0,     3,     1,     3,     1,     3,     3,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     5,     7,     5,     8,
       1,     3,     5,     5,     7,     7,     6,     5,     0,     2,
       5,     3,    13,     6,     0,     3,     1,     3,     3,     3,
       3,     1,     1,     3,     3,     3,     3,     3,     3,     1,
       5,     9,     5,     3,     3,     5,     9,     5,     5,     0,
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
       5,     0,    26,     0,     7,     0,   144,     0,     0,     0,
       0,   205,     0,     0,     0,     0,     0,     0,     0,   270,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,     0,     0,     0,     0,
       0,     0,     0,     0,   172,     0,     0,     0,     0,     0,
       0,     0,     0,   141,     0,     0,     0,   147,   156,     0,
       0,     0,     0,   104,    93,    84,    94,    85,    95,    86,
      96,    87,    97,    88,    98,    89,    99,    90,   100,    91,
     101,    92,    30,    31,     6,    26,   102,   103,     0,    41,
      40,    58,    59,    56,     0,    51,    57,   160,    52,    53,
      55,    54,    29,    69,    39,    75,    74,    43,    62,    64,
      65,    82,    44,    63,    83,    60,    79,    80,    68,    81,
      38,    47,    48,    49,    61,    76,    67,    78,    77,    42,
      66,    70,    71,    72,    73,    34,    35,    33,    32,    36,
      37,    50,    45,    46,     0,    25,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   166,
       0,     0,     0,     0,   105,   106,     0,     0,     0,     0,
     391,     0,   394,     0,   436,     0,   392,   413,    27,     0,
     164,     0,     8,     0,   352,   353,     0,   389,   171,     0,
       0,     0,    11,   438,   437,    13,   361,     0,   208,   209,
       0,     0,   358,     0,     0,   183,   356,   385,   387,   388,
       0,   384,   382,     0,   238,   242,   381,   239,   378,   380,
       0,   377,   375,     0,   212,     0,   374,   326,   325,     0,
     339,   340,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   415,   434,
     271,     0,   366,     0,     0,   365,     0,     0,     0,     0,
     454,     0,     0,   314,     0,     0,     0,     0,     0,   303,
     304,   328,   327,     0,   142,     0,     0,     0,     0,   355,
       0,     0,     0,     0,     0,   281,     0,   298,   297,   441,
     439,   440,   174,   173,     0,     0,     0,     0,   197,   198,
       0,     0,     0,     0,   107,     0,     0,     0,   320,   137,
       0,     0,     0,     0,   146,     0,     0,     0,     0,     0,
     410,   409,   411,   414,     0,   447,   449,   446,   448,   450,
     451,     0,     0,     0,   114,   115,   110,   108,     0,     0,
       0,     0,    26,   161,    24,     0,     0,     0,     0,     0,
     363,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     416,   417,     0,     0,     0,   425,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   143,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   168,   167,     0,     0,   162,
       0,     0,     0,   407,   393,   401,     0,     0,   396,   397,
     398,   399,   400,     0,   140,     0,   391,     0,     0,     0,
       0,   131,   129,   135,   133,     0,     0,     0,   165,     0,
       0,   390,    10,   299,     0,     9,     0,     0,   362,     0,
       0,     0,   211,   210,   183,   184,   207,     0,     0,     0,
     240,     0,     0,   214,   216,   266,   195,     0,   268,     0,
       0,   201,     0,     0,     0,     0,   372,     0,     0,   370,
       0,     0,   369,     0,     0,   433,   435,     0,     0,   341,
     342,     0,   307,     0,     0,   308,   305,   345,     0,   343,
       0,   344,   317,   272,     0,   273,     0,   186,     0,     0,
       0,     0,   278,   277,     0,     0,   175,   176,   321,   452,
     453,     0,   280,     0,   284,     0,   188,     0,     0,   316,
       0,     0,     0,   302,     0,     0,   158,     0,     0,   148,
     319,   318,     0,   405,   408,     0,   395,   145,   412,   109,
       0,     0,   118,     0,   117,     0,   116,     0,   122,     0,
     113,     0,   112,     0,   111,    28,   354,     0,     0,   364,
     357,     0,   359,     0,     0,   383,   444,   442,   443,   253,
     250,   244,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   249,     0,   254,     0,   256,   257,     0,   252,   243,
     258,   445,   246,     0,   376,   215,     0,     0,     0,   418,
       0,     0,     0,   420,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   178,     0,     0,   276,     0,     0,
       0,     0,   283,     0,     0,     0,     0,     0,     0,   163,
     157,   159,     0,     0,     0,   138,     0,   130,   132,   134,
     136,     0,   123,     0,   125,     0,   127,     0,     0,   360,
     206,   386,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,     0,     0,   267,    18,     0,
     202,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   329,     0,     0,     0,     0,   350,   349,   313,   274,
       0,   190,     0,     0,   275,   279,     0,     0,   322,     0,
       0,   315,     0,   192,     0,   329,   200,     0,   199,   170,
       0,   150,   402,   403,   404,   406,     0,     0,   121,     0,
     120,     0,   119,     0,     0,   265,   260,   259,   261,   262,
     263,   264,   247,   248,   251,   255,   245,     0,   346,   219,
     220,     0,   224,   223,   234,   225,   226,   227,     0,     0,
       0,   231,   232,     0,   217,   221,   347,   218,     0,     0,
     269,   419,   421,     0,   426,     0,   422,     0,   371,   424,
     423,     0,     0,     0,   336,     0,     0,     0,   351,     0,
       0,     0,     0,   203,   204,     0,     0,   286,     0,     0,
       0,     0,     0,   291,   292,     0,     0,     0,   285,     0,
       0,   336,     0,     0,     0,     0,     0,   150,   139,   124,
     126,   128,   300,     0,     0,   222,     0,     0,     0,     0,
     196,    19,     0,     0,   373,     0,     0,   333,   334,   335,
     330,   331,     0,   309,     0,     0,   306,   185,     0,   329,
     323,   177,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   187,     0,   179,   193,   301,     0,   154,   149,   151,
       0,   348,   228,   229,   230,   236,   235,   233,   427,     0,
     428,   397,     0,     0,     0,   311,     0,     0,     0,   336,
       0,   296,   287,   288,   289,   290,   293,   294,   295,     0,
       0,     0,   152,    26,     0,     0,     0,     0,     0,   368,
     332,   338,   337,   312,   310,     0,   456,   189,   181,     0,
     191,   180,     0,    26,   155,     0,   237,   429,    15,     0,
     455,     0,     0,     0,   153,     0,     0,   182,   282,   194,
       0,    16,    20,    21,    18,     0,     0,    12,    17,   430
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    92,    93,   968,  1006,   830,
    1004,    12,   156,    94,   383,    95,    96,    97,    98,   206,
      99,   100,   711,   713,   715,   477,   478,   479,   480,   101,
     463,   786,   102,   103,   439,   104,   105,   207,   702,   876,
     877,   983,   963,   106,   588,   107,   108,   221,   109,   585,
     110,   381,   111,   341,   456,   581,   112,   113,   114,   321,
     313,   115,   960,   116,   991,   397,   559,   578,   763,   775,
     117,   340,   777,   522,   855,   118,   244,   504,   119,   404,
     264,   655,   824,   937,   120,   401,   254,   400,   649,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   687,
     131,   132,   133,   134,   692,   868,   135,   136,   495,   137,
     138,   139,   140,   141,   142,   143,   144,   945,   974,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   269,   314,
     844,   900,   901,   903,   972,   272,   541,   550,   825,   758,
     551,   227,   318,   288,   246,   391,   294,   295,   531,   532,
     265,   266,   255,   256,   360,   319,   784,   593,   594,   595,
     362,   363,   364,   289,   421,   216,   235,   333,   652,   378,
     379,   380,   571,   299,   300
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -813
static const yytype_int16 yypact[] =
{
     153,   -83,   -71,   115,  -813,   153,    31,   -34,   -32,  -813,
    -813,   -38,   708,     1,  -813,    55,  -813,   -11,    27,    51,
      92,  -813,    96,   128,   132,   144,   148,   154,   176,   185,
     192,   202,   204,   207,   223,   230,   231,   234,   236,   240,
     243,   252,   258,   259,   266,   270,   271,   274,   277,   291,
     293,   294,   295,   296,   304,   313,   314,   323,   328,   337,
      20,   340,   341,  -813,   345,    33,   673,  -813,  -813,   347,
     307,    49,   190,  -813,  -813,  -813,  -813,  -813,  -813,  -813,
    -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,
    -813,  -813,  -813,  -813,  -813,   708,  -813,  -813,   183,  -813,
    -813,  -813,  -813,  -813,   358,  -813,  -813,  -813,  -813,  -813,
    -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,
    -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,
    -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,
    -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,
    -813,  -813,  -813,  -813,   495,   360,  -813,  -100,   324,    35,
     370,    63,    63,    70,   -37,    22,   -24,   -24,   759,   -85,
     -85,   -15,   362,   -24,   -24,   467,   -24,    -9,   -85,   -85,
     -20,   -15,   -15,   -15,    49,   376,    49,   -24,   759,   759,
     -24,   759,   759,    80,   -24,   759,   -20,   759,    67,  -813,
     759,   -85,   679,    49,  -813,  -813,   331,   379,   -24,   387,
    -813,    30,  -813,   388,  -813,   165,  -813,    23,  -813,   356,
    -813,   385,  -813,    55,  -813,  -813,   393,  -813,   338,   399,
     401,   407,  -813,  -813,  -813,  -813,  -813,   425,  -813,  -813,
     408,   594,  -813,   422,   428,   432,  -813,  -813,  -813,  -813,
     597,  -813,  -813,   429,  -813,  -813,  -813,  -813,  -813,  -813,
     610,  -813,  -813,   437,   443,   458,  -813,  -813,  -813,   459,
    -813,  -813,   461,   462,   463,   -85,   -85,   -24,   -24,   441,
     -24,   469,   470,   471,   759,   472,   427,  -813,  -813,   402,
    -813,   644,  -813,   474,   476,  -813,   479,   478,   650,   483,
    -813,   485,   486,  -813,   487,   489,   672,   504,   507,  -813,
    -813,  -813,  -813,   509,   677,   514,   516,   517,   518,   381,
     675,   524,   191,   528,   536,  -813,   537,  -813,  -813,  -813,
    -813,  -813,  -813,  -813,   538,   540,   542,   543,  -813,  -813,
     550,   385,   551,   562,  -813,   558,    49,    49,   564,  -813,
     567,   -35,    49,    49,  -813,    49,    49,    49,    49,    49,
     338,   381,  -813,   572,   570,  -813,  -813,  -813,  -813,  -813,
    -813,   573,    43,    36,  -813,  -813,   338,   381,   574,   596,
     598,   708,   708,  -813,  -813,    49,  -100,   770,    57,   772,
     603,   599,   759,   606,    49,   212,   775,   600,   608,    49,
     616,   385,   625,    49,   385,   -24,   -24,   759,   729,   730,
    -813,  -813,   630,   631,   627,  -813,   -24,   -24,   316,  -813,
     635,   632,   759,   633,    49,    46,   774,   806,   638,   777,
     780,   241,   -24,   734,   646,   749,   -15,   -10,  -813,   654,
     -15,   -15,   -15,    49,   662,    37,   -24,    15,   752,   664,
     692,   750,    -5,   837,    22,   733,  -813,    77,    77,  -813,
     -50,   676,   -17,   747,  -813,  -813,   125,   311,   194,   194,
    -813,  -813,  -813,    23,  -813,   759,   680,   -57,   -56,   -47,
     -27,  -813,  -813,   338,   381,   232,    28,   126,  -813,   678,
     330,  -813,  -813,  -813,   852,  -813,   683,   425,  -813,   684,
     858,   390,  -813,  -813,   432,  -813,  -813,   685,   411,   275,
    -813,   700,   465,  -813,  -813,  -813,   691,   710,   714,   -24,
     -24,   655,   717,   718,   720,   722,  -813,   723,   506,  -813,
     744,   724,  -813,   760,   762,  -813,  -813,   928,   477,  -813,
    -813,   763,  -813,   784,   956,  -813,   789,  -813,   786,  -813,
     791,  -813,  -813,  -813,   962,  -813,   794,  -813,   964,   796,
      46,   797,   798,  -813,   799,   902,  -813,  -813,  -813,  -813,
    -813,   801,  -813,    97,  -813,   802,  -813,   972,   804,  -813,
     805,   870,   977,  -813,   808,   385,  -813,   764,    49,  -813,
    -813,   338,   809,  -813,   822,   820,  -813,  -813,  -813,  -813,
     994,   824,  -813,   -16,  -813,    49,  -813,  -100,  -813,    25,
    -813,    82,  -813,    75,  -813,  -813,  -813,   827,   998,  -813,
    -813,   826,  -813,   821,   828,  -813,  -813,  -813,  -813,  -813,
    -813,  -813,   831,   832,   833,   834,   835,   836,   838,   839,
     840,  -813,   841,  -813,   842,  -813,  -813,   843,  -813,  -813,
    -813,  -813,  -813,   844,  -813,   845,   943,    22,  1014,  -813,
     848,   951,   759,  -813,    49,    49,   759,   850,    49,   759,
     759,   849,   853,  -813,   -20,  1021,   854,   887,  1025,   182,
     -46,   950,   859,    -4,  -813,   856,   953,  -813,    49,   861,
    -100,   862,   865,   944,   866,     7,   238,   385,    77,  -813,
    -813,   381,   863,   145,   747,  -813,    26,  -813,  -813,   381,
     338,    91,  -813,   113,  -813,   127,  -813,    46,   867,  -813,
    -813,  -813,   973,   952,   954,    49,    49,    49,    49,  -100,
      49,    49,    49,    70,  -813,   855,   873,  -813,   876,    49,
    -813,   874,   233,   265,   875,    46,   490,   877,   878,    49,
    1039,   879,   882,  1052,   883,   880,  -813,  -813,  -813,  -813,
    1055,  -813,   269,   888,  -813,  -813,   889,    66,   338,    97,
     702,  -813,  1056,  -813,  1059,   879,  -813,   891,  -813,  -813,
     892,    68,  -813,  -813,  -813,  -813,   385,    25,  -813,    82,
    -813,    75,  -813,   890,  1062,  -813,  -813,  -813,  -813,  -813,
    -813,  -813,   338,  -813,  -813,  -813,  -813,    69,  -813,  -813,
    -813,  -100,  -813,  -813,  -813,  -813,  -813,  -813,   894,   895,
     896,  -813,  -813,   897,  -813,  -813,  -813,   338,  1068,  1070,
    -813,   381,  -813,  1036,  -813,    49,  -813,   899,  -813,  -813,
    -813,   398,   903,   419,   989,  1074,   906,    70,  -813,   904,
     908,     7,    49,  -813,  -813,   907,   909,  -813,   910,   912,
     913,   914,   915,  -813,  -813,   916,   917,   918,  -813,   919,
     920,   989,    66,  1075,    97,   922,   921,    68,  -813,  -813,
    -813,  -813,  -813,   924,  1022,   338,    49,    49,    49,   -98,
    -813,  -813,   923,   276,  -813,    49,  1094,  -813,  -813,  -813,
    -813,   927,   929,  -813,   931,  1098,  -813,  -813,   212,   879,
    -813,  -813,   932,  1102,    70,    63,    57,    57,   -24,   -37,
    1103,  -813,  1104,  -813,   936,  -813,   937,  -813,  -813,  -813,
    1105,  -813,  -813,  -813,  -813,  -813,   881,  -813,  -813,  1076,
    -813,   -45,   940,   419,  1026,   385,  1108,   941,   942,   989,
    1037,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,   945,
     385,  1054,  -813,   708,   947,   -98,   948,   955,   949,  -813,
    -813,  -813,  -813,  -813,  -813,   957,  -813,  -813,  -813,   958,
    -813,  -813,   959,   708,  -813,    46,  -813,  -813,  -813,   961,
    -813,   385,  1042,  1123,  -813,   963,   -24,  -813,  -813,  -813,
      71,   965,  -813,  -813,   876,   -24,   966,  -813,  -813,  -813
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -813,  -813,  1124,  -813,  -813,  -813,  -813,  -813,  -813,   124,
    -813,  -813,   925,   -95,  -330,   754,   935,  1064,  -451,  -813,
    -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,
    -813,  -813,  -813,  -813,  -813,  1079,  -813,  -813,  -813,   263,
    -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,
    -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,   696,
     960,  -813,  -813,  -813,  -813,   639,  -813,  -813,  -813,   298,
    -813,  -813,  -813,  -584,   272,  -813,   383,   239,  -813,  -813,
    -813,  -813,  -813,   181,  -813,  -813,   988,  -813,  -813,  -813,
    -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,
    -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -530,  -813,
    -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,
    -813,  -813,  -813,  -813,  -813,  -813,  -813,  -813,   235,   481,
    -742,   208,  -813,  -812,  -813,  -434,  -555,  -813,  -813,  -813,
     417,   767,  -202,  -158,  -366,   660,   179,  -359,  -433,  -583,
    -473,  -567,  -481,  -551,  -154,   -61,  -813,   454,  -813,  -813,
     686,  -813,  -813,   884,   -93,   651,  -813,  -439,  -813,  -813,
    -813,  -813,  -813,  -160,  -813
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -214
static const yytype_int16 yytable[] =
{
     218,   345,   245,   228,   610,   684,   567,   481,   273,   274,
     215,   455,   592,   612,   482,   302,   303,   308,   305,   307,
     580,   315,   316,   317,   583,   584,    16,   210,   247,   323,
     716,   258,   326,   871,   210,   267,   334,   297,   350,   233,
     210,   329,   935,   297,   714,   330,   331,   476,   236,   237,
     348,   350,   311,   210,   614,   590,   236,   237,   712,   923,
     493,   374,   557,   236,   237,   376,   247,   576,   761,   586,
     853,   510,   338,   258,   513,   290,  1002,   740,   526,   773,
     526,   224,   569,   936,   329,   258,   270,   570,   330,   331,
     291,     7,   225,   226,   271,   324,   325,   248,   327,   328,
     259,   329,   335,     8,   337,   330,   331,   342,   236,   237,
     249,   539,   778,   292,   293,     9,   601,   603,   540,   412,
     413,   234,   415,   602,   604,   322,   605,   753,   967,   526,
     719,   -14,    11,   606,   691,   248,    15,   978,   854,   339,
     240,   465,   259,  1003,   268,   461,   607,    13,   249,    14,
     351,   241,   332,   608,   259,   240,   361,   155,   377,   241,
     298,   312,   793,   157,   224,   558,   306,   949,     1,     2,
     577,   762,   242,   243,   154,   225,   226,   355,   356,   357,
     358,   359,   774,   587,   357,   358,   359,   242,   243,   874,
     837,   875,   355,   356,   357,   358,   359,    71,   211,   750,
     250,   158,   719,   260,   224,   211,   251,   611,   881,   261,
     203,   372,   320,   212,   213,   225,   226,   224,   211,   483,
     212,   213,   880,   737,   211,   159,   212,   213,   225,   226,
     262,   263,   494,   212,   213,   247,   879,   708,   250,   212,
     213,   564,   707,   884,   251,   260,   678,   514,   515,   214,
     582,   261,   582,   592,   756,   699,   214,   260,   524,   525,
     806,   757,   214,   261,   787,   780,   160,   252,   253,   214,
     161,   788,   262,   263,   552,   214,   556,   529,   530,   626,
     561,   562,   563,   627,   628,   460,   789,   489,   568,   502,
     503,   466,   467,   790,   468,   469,   470,   471,   472,   499,
     791,   582,   162,   596,   248,   613,   163,   792,   591,   521,
     776,   351,   484,   547,   516,   548,   549,   249,   164,   526,
     353,   523,   165,   527,   490,   533,   529,   530,   166,   536,
     856,   782,   783,   501,   229,   230,   231,   232,   508,   629,
     630,   631,   512,   354,   632,   238,   239,   236,   237,   296,
     167,   650,   355,   356,   357,   358,   359,   309,   310,   168,
     210,   659,   660,   538,   446,   217,   169,   779,   633,   634,
     635,   636,   637,   638,   906,   639,   170,   365,   171,   366,
     343,   172,   599,   367,   368,   369,   954,   955,   236,   237,
     640,   641,   355,   356,   357,   358,   359,   173,   375,   642,
     643,   644,   645,   646,   174,   175,   833,   250,   176,   834,
     177,   609,   361,   251,   178,   219,   647,   179,   355,   356,
     357,   358,   359,   357,   358,   359,   180,   648,   236,   237,
     995,   393,   181,   182,   953,   926,   252,   253,   835,   240,
     183,   952,   236,   237,   184,   185,   238,   239,   186,   939,
     241,   187,   940,   710,   410,   411,   878,   897,   898,   899,
     355,   356,   357,   358,   359,   188,   370,   189,   190,   191,
     192,   242,   243,   275,   276,   277,   278,   279,   193,   280,
     240,   281,   282,   283,   284,   285,   766,   194,   195,   597,
     209,   528,   355,   356,   357,   358,   359,   196,   222,   236,
     237,   214,   197,   355,   356,   357,   358,   359,   616,   667,
     393,   198,   242,   243,   200,   201,   529,   530,   371,   202,
     240,   208,   287,   798,   799,   800,   801,   701,   803,   804,
     805,   372,   220,   223,   240,   373,   768,   224,   355,   356,
     357,   358,   359,   301,   709,   241,   212,   213,   225,   226,
     304,   320,   242,   243,   292,   293,   347,   355,   356,   357,
     358,   359,   349,   346,   382,   352,   242,   243,   622,   741,
     385,   386,   387,   744,   388,   802,   747,   748,   236,   237,
     389,   827,   214,   392,   275,   276,   277,   278,   279,   625,
     280,   240,   281,   282,   283,   284,   285,   390,   393,   394,
     398,   395,   286,   742,   743,   396,   399,   746,   355,   356,
     357,   358,   359,   402,   403,   973,   414,   355,   356,   357,
     358,   359,  -213,   242,   243,   355,   895,   357,   358,   359,
     981,   405,   406,   287,   407,   408,   409,   422,   355,   356,
     357,   358,   359,   654,   416,   417,   418,   420,   423,   425,
     910,   424,   426,   427,   428,   673,   429,   885,   430,   431,
     432,   997,   433,   275,   276,   277,   278,   279,   838,   280,
     240,   281,   282,   283,   284,   285,   434,   435,   831,   444,
     436,   286,   437,   438,   932,   933,   934,   440,   841,   441,
     442,   443,   355,   356,   357,   358,   359,   445,   521,   236,
     237,   447,   242,   243,   355,   356,   357,   358,   359,   448,
     449,   450,   287,   451,    16,   452,   453,   355,   356,   357,
     358,   359,    17,   454,   457,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,   458,   459,   462,    28,    29,
      30,    31,    32,   464,    33,   473,   474,   475,   485,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
     956,    45,    46,    47,    48,    49,    50,    51,    52,    53,
     486,    54,   487,   492,   893,   496,   497,   498,    55,   500,
     505,   507,   506,   857,   275,   276,   277,   278,   279,   509,
     280,   240,   281,   282,   283,   284,   285,    56,   511,   858,
     517,   518,   286,   519,   520,   534,   537,    57,   535,   542,
     543,   544,   545,    58,   553,    59,    60,   546,    61,   554,
      62,    63,    64,   242,   243,    65,    66,   560,    67,   555,
      68,   236,   237,   287,   941,   565,   572,   574,  1001,   573,
     575,   579,  -169,   859,   860,   861,   862,  1008,   863,   864,
     865,   866,   867,   600,   589,   617,   618,   204,   615,    69,
     620,    70,   621,    74,   656,    76,   624,    78,   984,    80,
      75,    82,    77,    84,    79,    86,    81,    88,    83,    90,
      85,   653,    87,   657,    89,    71,    91,   658,   994,   661,
     662,    72,    73,   664,   663,   665,   666,   669,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,   275,   276,   277,   278,
     279,   668,   280,   240,   281,   282,   283,   284,   285,   807,
     808,    73,   672,   670,   286,   671,   674,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,   242,   243,   675,   809,   810,
     676,   811,   677,   678,   679,   287,   680,   681,   682,   683,
     685,   686,   688,   689,   690,   693,   694,   695,   696,   697,
     667,   698,   812,   703,   813,   814,   815,   816,   817,   818,
     819,   820,   821,   822,   823,   704,   705,   700,   706,   291,
     717,   718,   719,   720,   721,   722,   723,   724,   725,   726,
     727,   736,   728,   729,   730,   731,   732,   733,   735,   738,
     734,   739,   521,   745,   749,   752,   750,   753,   754,   755,
     759,   771,   760,   765,   767,   769,   224,   764,   770,   772,
     794,   795,   781,   842,   796,   797,   828,   225,   226,   829,
     832,   836,   843,   839,   840,   845,   846,   847,   848,   849,
     869,   851,   852,   870,   872,   873,   882,   883,   886,   887,
     888,   889,   890,   891,   892,   894,   896,   902,   904,   905,
     907,   908,   925,   911,   913,   912,   914,   915,   916,   917,
     918,   919,   920,   922,   931,   921,   927,   930,   942,   938,
     943,   928,   947,   944,   946,   950,   951,   958,   959,   961,
     964,   962,   975,   971,   966,   965,   969,   976,   977,   979,
     985,   980,   982,   998,   987,   989,   988,   999,  1007,    10,
     205,   992,   993,   990,   996,   488,  1000,   344,  1005,   199,
     929,   566,  1009,   623,   924,   850,   986,   948,   384,   909,
     257,   970,   826,   491,   957,   751,   336,   619,   785,   598,
     651,     0,     0,     0,     0,     0,     0,     0,   419
};

static const yytype_int16 yycheck[] =
{
      95,   203,   160,   157,   485,   560,   445,   373,   166,   167,
      71,   341,   463,   486,   373,   173,   174,   177,   176,   177,
     454,   181,   182,   183,   457,   458,     6,     4,     3,   187,
     613,     3,   190,   775,     4,    72,   194,    52,     8,     4,
       4,     4,   140,    52,   611,     8,     9,     4,    72,    73,
     208,     8,    72,     4,   487,    72,    72,    73,   609,   871,
       3,   219,    72,    72,    73,   219,     3,    72,    72,   119,
       4,   401,     5,     3,   404,   168,     5,   661,     3,    72,
       3,   181,    67,   181,     4,     3,    64,    72,     8,     9,
     175,   174,   192,   193,    72,   188,   189,    72,   191,   192,
      72,     4,   195,   174,   197,     8,     9,   200,    72,    73,
      85,    65,   696,   198,   199,     0,   173,   173,    72,   277,
     278,    86,   280,   180,   180,   186,   173,   173,   173,     3,
     176,   176,   101,   180,   573,    72,   174,   949,    72,    72,
     164,   176,    72,    72,   181,   347,   173,   181,    85,   181,
     211,   175,    72,   180,    72,   164,   217,   102,   219,   175,
     175,   181,   717,   174,   181,   175,   175,   909,    15,    16,
     175,   175,   196,   197,   173,   192,   193,   227,   228,   229,
     230,   231,   175,   233,   229,   230,   231,   196,   197,   121,
     745,   123,   227,   228,   229,   230,   231,   177,   175,   173,
     175,   174,   176,   175,   181,   175,   181,   179,   791,   181,
     177,   175,   175,   190,   191,   192,   193,   181,   175,   373,
     190,   191,   789,   657,   175,   174,   190,   191,   192,   193,
     202,   203,   175,   190,   191,     3,   787,   603,   175,   190,
     191,   443,   601,   174,   181,   175,   177,   405,   406,   226,
     175,   181,   175,   704,    72,   585,   226,   175,   416,   417,
     733,    79,   226,   181,   173,   698,   174,   204,   205,   226,
     174,   180,   202,   203,   432,   226,   436,   200,   201,     4,
     440,   441,   442,     8,     9,   346,   173,   382,   446,    77,
      78,   352,   353,   180,   355,   356,   357,   358,   359,   392,
     173,   175,   174,   178,    72,   179,   174,   180,   462,    71,
      72,   372,   373,    72,   407,    74,    75,    85,   174,     3,
     155,   414,   174,     7,   385,   418,   200,   201,   174,   422,
     769,   186,   187,   394,    10,    11,    12,    13,   399,    64,
      65,    66,   403,   178,    69,    76,    77,    72,    73,   170,
     174,   509,   227,   228,   229,   230,   231,   178,   179,   174,
       4,   519,   520,   424,   173,   175,   174,   697,    93,    94,
      95,    96,    97,    98,   847,   100,   174,    21,   174,    23,
     201,   174,   475,    27,    28,    29,   916,   917,    72,    73,
     115,   116,   227,   228,   229,   230,   231,   174,   219,   124,
     125,   126,   127,   128,   174,   174,   173,   175,   174,   176,
     174,   179,   473,   181,   174,   232,   141,   174,   227,   228,
     229,   230,   231,   229,   230,   231,   174,   152,    72,    73,
     985,     4,   174,   174,   915,   874,   204,   205,   173,   164,
     174,   914,    72,    73,   174,   174,    76,    77,   174,   173,
     175,   174,   176,   607,   275,   276,   786,    38,    39,    40,
     227,   228,   229,   230,   231,   174,   110,   174,   174,   174,
     174,   196,   197,   157,   158,   159,   160,   161,   174,   163,
     164,   165,   166,   167,   168,   169,   688,   174,   174,   178,
     183,   175,   227,   228,   229,   230,   231,   174,     3,    72,
      73,   226,   174,   227,   228,   229,   230,   231,   178,     3,
       4,   174,   196,   197,   174,   174,   200,   201,   162,   174,
     164,   174,   206,   725,   726,   727,   728,   588,   730,   731,
     732,   175,   174,   173,   164,   179,   690,   181,   227,   228,
     229,   230,   231,   181,   605,   175,   190,   191,   192,   193,
      83,   175,   196,   197,   198,   199,   177,   227,   228,   229,
     230,   231,   175,   232,   179,   177,   196,   197,   178,   662,
     177,   233,   173,   666,   173,   729,   669,   670,    72,    73,
     173,   735,   226,   175,   157,   158,   159,   160,   161,   178,
     163,   164,   165,   166,   167,   168,   169,   172,     4,   177,
       3,   173,   175,   664,   665,   173,   177,   668,   227,   228,
     229,   230,   231,     3,   177,   945,   175,   227,   228,   229,
     230,   231,   179,   196,   197,   227,   228,   229,   230,   231,
     960,   173,   173,   206,   173,   173,   173,   235,   227,   228,
     229,   230,   231,   178,   175,   175,   175,   175,     4,   173,
     852,   177,   173,   175,     4,   178,   173,   811,   173,   173,
     173,   991,   173,   157,   158,   159,   160,   161,   178,   163,
     164,   165,   166,   167,   168,   169,     4,   173,   739,     4,
     173,   175,   173,     6,   886,   887,   888,   173,   749,   173,
     173,   173,   227,   228,   229,   230,   231,   173,    71,    72,
      73,   173,   196,   197,   227,   228,   229,   230,   231,   173,
     173,   173,   206,   173,     6,   173,   173,   227,   228,   229,
     230,   231,    14,   173,   173,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,   173,   178,   173,    30,    31,
      32,    33,    34,   176,    36,   173,   176,   174,   174,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
     918,    53,    54,    55,    56,    57,    58,    59,    60,    61,
     174,    63,   174,     3,   835,     3,   173,   178,    70,   173,
       5,   173,   182,    81,   157,   158,   159,   160,   161,   173,
     163,   164,   165,   166,   167,   168,   169,    89,   173,    97,
      71,    71,   175,   173,   173,   170,   173,    99,   176,    35,
       4,   173,    35,   105,    80,   107,   108,    37,   110,   173,
     112,   113,   114,   196,   197,   117,   118,   173,   120,    80,
     122,    72,    73,   206,   895,   173,    84,   145,   996,   175,
      90,     4,   109,   141,   142,   143,   144,  1005,   146,   147,
     148,   149,   150,   173,   178,     3,   173,   184,   180,   151,
     176,   153,     4,   190,   173,   192,   181,   194,   963,   196,
     191,   198,   193,   200,   195,   202,   197,   204,   199,   206,
     201,   181,   203,   173,   205,   177,   207,   173,   983,   234,
     173,   183,   184,   173,   176,   173,   173,   173,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   157,   158,   159,   160,
     161,   177,   163,   164,   165,   166,   167,   168,   169,    74,
      75,   184,     4,   173,   175,   173,   173,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   196,   197,   173,   103,   104,
       4,   106,   173,   177,   173,   206,     4,   173,     4,   173,
     173,   173,   173,    71,   173,   173,     4,   173,   173,   109,
       3,   173,   127,   174,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   173,   176,   233,     4,   175,
     173,     3,   176,   182,   176,   174,   174,   174,   174,   174,
     174,    68,   174,   174,   174,   174,   174,   174,   173,     5,
     176,   173,    71,   173,   175,     4,   173,   173,   141,     4,
      80,    87,   173,    80,   173,   173,   181,   181,   173,   173,
     173,    68,   179,     4,    92,    91,   173,   192,   193,   173,
     176,   176,   173,   176,   176,   173,     4,   174,   178,     4,
       4,   173,   173,     4,   173,   173,   176,     5,   174,   174,
     174,   174,     4,     3,    38,   176,   173,    88,     4,   173,
     176,   173,     7,   176,   174,   176,   174,   174,   174,   174,
     174,   174,   174,   173,    72,   176,   174,   173,     4,   176,
     173,   180,     4,   174,   173,   173,     4,     4,     4,   173,
       5,   174,     4,    87,    38,   234,   176,   176,   176,    82,
     173,   176,    68,    81,   176,   176,   171,     4,  1004,     5,
      66,   173,   173,   176,   173,   381,   173,   202,   173,    60,
     877,   445,   176,   504,   872,   762,   965,   908,   223,   851,
     162,   943,   735,   386,   919,   674,   196,   497,   704,   473,
     509,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   284
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   237,   238,   239,   240,   174,   174,     0,
     238,   101,   247,   181,   181,   174,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    30,    31,
      32,    33,    34,    36,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    63,    70,    89,    99,   105,   107,
     108,   110,   112,   113,   114,   117,   118,   120,   122,   151,
     153,   177,   183,   184,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   241,   242,   249,   251,   252,   253,   254,   256,
     257,   265,   268,   269,   271,   272,   279,   281,   282,   284,
     286,   288,   292,   293,   294,   297,   299,   306,   311,   314,
     320,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   336,   337,   338,   339,   342,   343,   345,   346,   347,
     348,   349,   350,   351,   352,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   173,   102,   248,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   271,
     174,   174,   174,   177,   184,   253,   255,   273,   174,   183,
       4,   175,   190,   191,   226,   391,   401,   175,   249,   232,
     174,   283,     3,   173,   181,   192,   193,   377,   390,    10,
      11,    12,    13,     4,    86,   402,    72,    73,    76,    77,
     164,   175,   196,   197,   312,   379,   380,     3,    72,    85,
     175,   181,   204,   205,   322,   388,   389,   322,     3,    72,
     175,   181,   202,   203,   316,   386,   387,    72,   181,   364,
      64,    72,   371,   379,   379,   157,   158,   159,   160,   161,
     163,   165,   166,   167,   168,   169,   175,   206,   379,   399,
     400,   175,   198,   199,   382,   383,   382,    52,   175,   409,
     410,   181,   379,   379,    83,   379,   175,   379,   409,   382,
     382,    72,   181,   296,   365,   409,   409,   409,   378,   391,
     175,   295,   391,   379,   400,   400,   379,   400,   400,     4,
       8,     9,    72,   403,   379,   400,   296,   400,     5,    72,
     307,   289,   400,   382,   252,   378,   232,   177,   379,   175,
       8,   391,   177,   155,   178,   227,   228,   229,   230,   231,
     390,   391,   396,   397,   398,    21,    23,    27,    28,    29,
     110,   162,   175,   179,   379,   382,   390,   391,   405,   406,
     407,   287,   179,   250,   248,   177,   233,   173,   173,   173,
     172,   381,   175,     4,   177,   173,   173,   301,     3,   177,
     323,   321,     3,   177,   315,   173,   173,   173,   173,   173,
     382,   382,   379,   379,   175,   379,   175,   175,   175,   399,
     175,   400,   235,     4,   177,   173,   173,   175,     4,   173,
     173,   173,   173,   173,     4,   173,   173,   173,     6,   270,
     173,   173,   173,   173,     4,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   250,   290,   173,   173,   178,
     391,   378,   173,   266,   176,   176,   391,   391,   391,   391,
     391,   391,   391,   173,   176,   174,     4,   261,   262,   263,
     264,   380,   383,   390,   391,   174,   174,   174,   251,   249,
     391,   377,     3,     3,   175,   344,     3,   173,   178,   400,
     173,   391,    77,    78,   313,     5,   182,   173,   391,   173,
     250,   173,   391,   250,   379,   379,   400,    71,    71,   173,
     173,    71,   309,   400,   379,   379,     3,     7,   175,   200,
     201,   384,   385,   400,   170,   176,   400,   173,   391,    65,
      72,   372,    35,     4,   173,    35,    37,    72,    74,    75,
     373,   376,   379,    80,   173,    80,   409,    72,   175,   302,
     173,   409,   409,   409,   378,   173,   295,   403,   379,    67,
      72,   408,    84,   175,   145,    90,    72,   175,   303,     4,
     371,   291,   175,   384,   384,   285,   119,   233,   280,   178,
      72,   390,   254,   393,   394,   395,   178,   178,   396,   400,
     173,   173,   180,   173,   180,   173,   180,   173,   180,   179,
     388,   179,   386,   179,   384,   180,   178,     3,   173,   381,
     176,     4,   178,   301,   181,   178,     4,     8,     9,    64,
      65,    66,    69,    93,    94,    95,    96,    97,    98,   100,
     115,   116,   124,   125,   126,   127,   128,   141,   152,   324,
     379,   401,   404,   181,   178,   317,   173,   173,   173,   379,
     379,   234,   173,   176,   173,   173,   173,     3,   177,   173,
     173,   173,     4,   178,   173,   173,     4,   173,   177,   173,
       4,   173,     4,   173,   372,   173,   173,   335,   173,    71,
     173,   403,   340,   173,     4,   173,   173,   109,   173,   250,
     233,   391,   274,   174,   173,   176,     4,   383,   380,   391,
     390,   258,   389,   259,   387,   260,   385,   173,     3,   176,
     182,   176,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   176,   173,    68,   371,     5,   173,
     309,   400,   391,   391,   400,   173,   391,   400,   400,   175,
     173,   365,     4,   173,   141,     4,    72,    79,   375,    80,
     173,    72,   175,   304,   181,    80,   378,   173,   390,   173,
     173,    87,   173,    72,   175,   305,    72,   308,   309,   250,
     384,   179,   186,   187,   392,   393,   267,   173,   180,   173,
     180,   173,   180,   372,   173,    68,    92,    91,   378,   378,
     378,   378,   390,   378,   378,   378,   386,    74,    75,   103,
     104,   106,   127,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   318,   374,   376,   390,   173,   173,
     245,   391,   176,   173,   176,   173,   176,   372,   178,   176,
     176,   391,     4,   173,   366,   173,     4,   174,   178,     4,
     312,   173,   173,     4,    72,   310,   403,    81,    97,   141,
     142,   143,   144,   146,   147,   148,   149,   150,   341,     4,
       4,   366,   173,   173,   121,   123,   275,   276,   250,   389,
     387,   385,   176,     5,   174,   390,   174,   174,   174,   174,
       4,     3,    38,   391,   176,   228,   173,    38,    39,    40,
     367,   368,    88,   369,     4,   173,   386,   176,   173,   305,
     378,   176,   176,   174,   174,   174,   174,   174,   174,   174,
     174,   176,   173,   369,   310,     7,   403,   174,   180,   275,
     173,    72,   378,   378,   378,   140,   181,   319,   176,   173,
     176,   391,     4,   173,   174,   353,   173,     4,   313,   366,
     173,     4,   386,   388,   344,   344,   379,   364,     4,     4,
     298,   173,   174,   278,     5,   234,    38,   173,   243,   176,
     367,    87,   370,   250,   354,     4,   176,   176,   369,    82,
     176,   250,    68,   277,   249,   173,   319,   176,   171,   176,
     176,   300,   173,   173,   249,   372,   173,   250,    81,     4,
     173,   379,     5,    72,   246,   173,   244,   245,   379,   176
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

  case 107:

/* Line 1455 of yacc.c  */
#line 569 "lev_comp.y"
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

  case 108:

/* Line 1455 of yacc.c  */
#line 585 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 591 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 597 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 603 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 609 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 615 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 621 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 627 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 633 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 643 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 653 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 663 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 673 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 683 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 693 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 705 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 710 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 717 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 722 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 729 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 734 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 741 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 745 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 751 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 756 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 763 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 767 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 773 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 777 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 783 "lev_comp.y"
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

  case 138:

/* Line 1455 of yacc.c  */
#line 805 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 809 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 820 "lev_comp.y"
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

  case 141:

/* Line 1455 of yacc.c  */
#line 875 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 881 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 885 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 891 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 898 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 902 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 910 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 914 "lev_comp.y"
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

  case 149:

/* Line 1455 of yacc.c  */
#line 937 "lev_comp.y"
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

  case 152:

/* Line 1455 of yacc.c  */
#line 982 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 992 "lev_comp.y"
    {
		  ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 995 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 1005 "lev_comp.y"
    {
		  ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1010 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1024 "lev_comp.y"
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

  case 160:

/* Line 1455 of yacc.c  */
#line 1061 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1066 "lev_comp.y"
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

  case 162:

/* Line 1455 of yacc.c  */
#line 1095 "lev_comp.y"
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

  case 163:

/* Line 1455 of yacc.c  */
#line 1109 "lev_comp.y"
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

  case 164:

/* Line 1455 of yacc.c  */
#line 1125 "lev_comp.y"
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

  case 165:

/* Line 1455 of yacc.c  */
#line 1145 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1157 "lev_comp.y"
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

  case 167:

/* Line 1455 of yacc.c  */
#line 1177 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1183 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1193 "lev_comp.y"
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

  case 170:

/* Line 1455 of yacc.c  */
#line 1211 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1221 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1227 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1232 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1237 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1244 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1251 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1260 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1268 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1279 "lev_comp.y"
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

  case 180:

/* Line 1455 of yacc.c  */
#line 1290 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1297 "lev_comp.y"
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

  case 182:

/* Line 1455 of yacc.c  */
#line 1308 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1315 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1319 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1325 "lev_comp.y"
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

  case 186:

/* Line 1455 of yacc.c  */
#line 1335 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1341 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1350 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1356 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1361 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1367 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1372 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1378 "lev_comp.y"
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

  case 194:

/* Line 1455 of yacc.c  */
#line 1389 "lev_comp.y"
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

  case 195:

/* Line 1455 of yacc.c  */
#line 1400 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((long)(yyvsp[(3) - (5)].i), 0, 0, SPO_DOOR));
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1404 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((long)(yyvsp[(3) - (9)].i), (yyvsp[(7) - (9)].i), (yyvsp[(9) - (9)].i), SPO_DOOR));
		  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1418 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1422 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1432 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1439 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1447 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1463 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1467 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1473 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1481 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1487 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1495 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1503 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1508 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1514 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1520 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1526 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1532 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1537 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1542 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1547 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1552 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1557 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1562 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1567 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1572 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1577 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1582 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1588 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1595 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1604 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1608 "lev_comp.y"
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

  case 238:

/* Line 1455 of yacc.c  */
#line 1621 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1629 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1639 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1647 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1656 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1663 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1671 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1677 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1682 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1687 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1692 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1697 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1702 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1707 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1712 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1717 "lev_comp.y"
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

  case 254:

/* Line 1455 of yacc.c  */
#line 1728 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1734 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1739 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1744 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1749 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1754 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1759 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1764 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1769 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1774 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1779 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1784 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1791 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1797 "lev_comp.y"
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

  case 268:

/* Line 1455 of yacc.c  */
#line 1826 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1831 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1839 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1845 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1851 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1858 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1865 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1875 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1885 "lev_comp.y"
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

  case 277:

/* Line 1455 of yacc.c  */
#line 1900 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1910 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1914 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1920 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1926 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1932 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int)(yyvsp[(6) - (13)].i), (int)(yyvsp[(8) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1938 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1944 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1951 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1959 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1965 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1970 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1975 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1980 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1985 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1991 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1997 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2002 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2007 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2012 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2019 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2025 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2031 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2036 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2043 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2050 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2056 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2062 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2068 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2072 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2078 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2084 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2094 "lev_comp.y"
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

  case 310:

/* Line 1455 of yacc.c  */
#line 2108 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      if ( (yyvsp[(10) - (11)].i) ) {
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      } else if ( (yyvsp[(11) - (11)].i) )
			  lc_error("Cannot use lev statements in non-permanent REGION");
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2118 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2122 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2128 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2135 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2141 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2147 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2153 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2159 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2163 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2168 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2175 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2181 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2188 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2192 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2199 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2210 "lev_comp.y"
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

  case 329:

/* Line 1455 of yacc.c  */
#line 2223 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2227 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2233 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2237 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2244 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2248 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2252 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2258 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2262 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2268 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2284 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2292 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2302 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2311 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2316 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2323 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2334 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2340 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2344 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2348 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2355 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2365 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2372 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2376 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2382 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2386 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2394 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2398 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2405 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2415 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2428 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2432 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2439 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2449 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2458 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2469 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2473 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2480 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2490 "lev_comp.y"
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

  case 378:

/* Line 1455 of yacc.c  */
#line 2501 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2510 "lev_comp.y"
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

  case 380:

/* Line 1455 of yacc.c  */
#line 2520 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2526 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2530 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2537 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2547 "lev_comp.y"
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

  case 385:

/* Line 1455 of yacc.c  */
#line 2558 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2567 "lev_comp.y"
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

  case 387:

/* Line 1455 of yacc.c  */
#line 2577 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2581 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2587 "lev_comp.y"
    { ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2589 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2595 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2599 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2603 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2607 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2615 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2624 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2628 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2632 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2636 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2640 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2643 "lev_comp.y"
    { ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2647 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2654 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2663 "lev_comp.y"
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

  case 409:

/* Line 1455 of yacc.c  */
#line 2704 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2708 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2715 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2722 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2732 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2736 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2744 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2748 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2752 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2756 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2760 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2765 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2769 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2773 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2778 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2783 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2788 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2792 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2797 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2802 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2806 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2810 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2815 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2819 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2826 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2832 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2836 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2842 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2858 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2862 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2866 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2870 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2892 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2896 "lev_comp.y"
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

  case 456:

/* Line 1455 of yacc.c  */
#line 2922 "lev_comp.y"
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
#line 6606 "lev_comp.tab.c"
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
#line 2950 "lev_comp.y"


/*lev_comp.y*/

