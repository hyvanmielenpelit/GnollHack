
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
     KEYTYPE_ID = 323,
     LEVER_ID = 324,
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
     FLOOR_MAIN_TYPE_ID = 345,
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
     LOCATION_SUBTYPE_ID = 368,
     DOOR_SUBTYPE = 369,
     EXIT_ID = 370,
     SHUFFLE_ID = 371,
     QUANTITY_ID = 372,
     BURIED_ID = 373,
     LOOP_ID = 374,
     FOR_ID = 375,
     TO_ID = 376,
     SWITCH_ID = 377,
     CASE_ID = 378,
     BREAK_ID = 379,
     DEFAULT_ID = 380,
     ERODED_ID = 381,
     TRAPPED_STATE = 382,
     RECHARGED_ID = 383,
     INVIS_ID = 384,
     GREASED_ID = 385,
     FEMALE_ID = 386,
     WAITFORU_ID = 387,
     CANCELLED_ID = 388,
     REVIVED_ID = 389,
     AVENGE_ID = 390,
     FLEEING_ID = 391,
     BLINDED_ID = 392,
     PARALYZED_ID = 393,
     STUNNED_ID = 394,
     CONFUSED_ID = 395,
     SEENTRAPS_ID = 396,
     ALL_ID = 397,
     MONTYPE_ID = 398,
     OBJTYPE_ID = 399,
     TERTYPE_ID = 400,
     TERTYPE2_ID = 401,
     LEVER_EFFECT_TYPE = 402,
     SWITCHABLE_ID = 403,
     CONTINUOUSLY_USABLE_ID = 404,
     TARGET_ID = 405,
     TRAPTYPE_ID = 406,
     EFFECT_FLAG_ID = 407,
     GRAVE_ID = 408,
     ERODEPROOF_ID = 409,
     FUNCTION_ID = 410,
     MSG_OUTPUT_TYPE = 411,
     COMPARE_TYPE = 412,
     UNKNOWN_TYPE = 413,
     rect_ID = 414,
     fillrect_ID = 415,
     line_ID = 416,
     randline_ID = 417,
     grow_ID = 418,
     selection_ID = 419,
     flood_ID = 420,
     rndcoord_ID = 421,
     circle_ID = 422,
     ellipse_ID = 423,
     filter_ID = 424,
     complement_ID = 425,
     gradient_ID = 426,
     GRADIENT_TYPE = 427,
     LIMITED = 428,
     HUMIDITY_TYPE = 429,
     STRING = 430,
     MAP_ID = 431,
     NQSTRING = 432,
     VARSTRING = 433,
     CFUNC = 434,
     CFUNC_INT = 435,
     CFUNC_STR = 436,
     CFUNC_COORD = 437,
     CFUNC_REGION = 438,
     VARSTRING_INT = 439,
     VARSTRING_INT_ARRAY = 440,
     VARSTRING_STRING = 441,
     VARSTRING_STRING_ARRAY = 442,
     VARSTRING_VAR = 443,
     VARSTRING_VAR_ARRAY = 444,
     VARSTRING_COORD = 445,
     VARSTRING_COORD_ARRAY = 446,
     VARSTRING_REGION = 447,
     VARSTRING_REGION_ARRAY = 448,
     VARSTRING_MAPCHAR = 449,
     VARSTRING_MAPCHAR_ARRAY = 450,
     VARSTRING_MONST = 451,
     VARSTRING_MONST_ARRAY = 452,
     VARSTRING_OBJ = 453,
     VARSTRING_OBJ_ARRAY = 454,
     VARSTRING_SEL = 455,
     VARSTRING_SEL_ARRAY = 456,
     METHOD_INT = 457,
     METHOD_INT_ARRAY = 458,
     METHOD_STRING = 459,
     METHOD_STRING_ARRAY = 460,
     METHOD_VAR = 461,
     METHOD_VAR_ARRAY = 462,
     METHOD_COORD = 463,
     METHOD_COORD_ARRAY = 464,
     METHOD_REGION = 465,
     METHOD_REGION_ARRAY = 466,
     METHOD_MAPCHAR = 467,
     METHOD_MAPCHAR_ARRAY = 468,
     METHOD_MONST = 469,
     METHOD_MONST_ARRAY = 470,
     METHOD_OBJ = 471,
     METHOD_OBJ_ARRAY = 472,
     METHOD_SEL = 473,
     METHOD_SEL_ARRAY = 474,
     DICE = 475
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
#line 518 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 530 "lev_comp.tab.c"

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
#define YYLAST   1182

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  238
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  180
/* YYNRULES -- Number of rules.  */
#define YYNRULES  468
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1030

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   475

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   233,   237,     2,
     177,   178,   231,   229,   175,   230,   235,   232,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   176,     2,
       2,   234,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   179,     2,   180,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   181,   236,   182,     2,     2,     2,     2,
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
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228
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
     536,   549,   550,   565,   566,   569,   575,   577,   583,   585,
     591,   593,   599,   601,   612,   619,   621,   623,   625,   627,
     629,   633,   635,   637,   638,   642,   646,   650,   654,   656,
     664,   670,   672,   674,   676,   678,   682,   683,   689,   694,
     695,   699,   701,   703,   705,   707,   710,   712,   714,   716,
     718,   720,   724,   728,   732,   734,   736,   740,   742,   744,
     746,   750,   754,   755,   761,   764,   765,   769,   771,   775,
     777,   781,   785,   787,   789,   793,   795,   797,   799,   803,
     805,   807,   809,   813,   817,   821,   825,   829,   833,   837,
     843,   851,   857,   866,   868,   872,   878,   884,   892,   900,
     907,   913,   914,   917,   923,   927,   939,   946,   947,   951,
     953,   957,   961,   965,   969,   971,   973,   977,   981,   985,
     989,   993,   997,   999,  1001,  1003,  1005,  1009,  1013,  1015,
    1021,  1031,  1037,  1041,  1045,  1051,  1061,  1067,  1073,  1074,
    1087,  1088,  1090,  1098,  1102,  1110,  1116,  1122,  1128,  1134,
    1138,  1144,  1152,  1162,  1164,  1166,  1168,  1170,  1172,  1173,
    1176,  1178,  1182,  1184,  1186,  1188,  1189,  1193,  1195,  1196,
    1200,  1202,  1204,  1206,  1208,  1210,  1212,  1214,  1216,  1218,
    1220,  1224,  1226,  1228,  1233,  1235,  1237,  1242,  1244,  1246,
    1251,  1253,  1258,  1264,  1266,  1270,  1272,  1276,  1278,  1280,
    1285,  1295,  1297,  1299,  1304,  1306,  1312,  1314,  1316,  1321,
    1323,  1325,  1331,  1333,  1335,  1337,  1342,  1344,  1346,  1352,
    1354,  1356,  1358,  1362,  1364,  1366,  1370,  1372,  1377,  1381,
    1385,  1389,  1393,  1397,  1401,  1403,  1405,  1409,  1411,  1415,
    1416,  1418,  1420,  1422,  1424,  1428,  1429,  1431,  1433,  1436,
    1439,  1444,  1451,  1456,  1463,  1470,  1477,  1484,  1487,  1494,
    1503,  1512,  1523,  1538,  1541,  1543,  1547,  1549,  1553,  1555,
    1557,  1559,  1561,  1563,  1565,  1567,  1569,  1571,  1573,  1575,
    1577,  1579,  1581,  1583,  1585,  1587,  1589,  1591,  1602
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     239,     0,    -1,    -1,   240,    -1,   241,    -1,   241,   240,
      -1,   242,   249,   251,    -1,    16,   176,   183,    -1,    15,
     176,   183,   175,     3,    -1,    17,   176,    11,   175,   349,
      -1,    17,   176,    10,   175,     3,    -1,    17,   176,    13,
      -1,    17,   176,    12,   175,     3,   175,     3,   175,     5,
     175,     5,   175,   379,   175,   248,   247,    -1,    18,   176,
     409,    -1,    -1,   175,   173,    -1,    -1,   175,   386,    -1,
      -1,   175,     3,    -1,     5,    -1,    71,    -1,    -1,   101,
     176,   250,    -1,   102,   175,   250,    -1,   102,    -1,    -1,
     253,   251,    -1,   181,   251,   182,    -1,   294,    -1,   243,
      -1,   244,    -1,   363,    -1,   362,    -1,   360,    -1,   361,
      -1,   364,    -1,   365,    -1,   338,    -1,   296,    -1,   259,
      -1,   258,    -1,   352,    -1,   308,    -1,   330,    -1,   367,
      -1,   368,    -1,   340,    -1,   341,    -1,   342,    -1,   366,
      -1,   274,    -1,   284,    -1,   286,    -1,   290,    -1,   288,
      -1,   271,    -1,   281,    -1,   267,    -1,   270,    -1,   333,
      -1,   343,    -1,   315,    -1,   331,    -1,   318,    -1,   324,
      -1,   353,    -1,   348,    -1,   336,    -1,   295,    -1,   354,
      -1,   355,    -1,   356,    -1,   357,    -1,   301,    -1,   299,
      -1,   347,    -1,   351,    -1,   350,    -1,   334,    -1,   335,
      -1,   337,    -1,   329,    -1,   332,    -1,   193,    -1,   195,
      -1,   197,    -1,   199,    -1,   201,    -1,   203,    -1,   205,
      -1,   207,    -1,   209,    -1,   192,    -1,   194,    -1,   196,
      -1,   198,    -1,   200,    -1,   202,    -1,   204,    -1,   206,
      -1,   208,    -1,   254,    -1,   255,    -1,   186,    -1,   186,
      -1,   255,    -1,   116,   176,   254,    -1,   256,   234,   398,
      -1,   256,   234,   164,   176,   407,    -1,   256,   234,   397,
      -1,   256,   234,   414,   176,   391,    -1,   256,   234,   413,
     176,   393,    -1,   256,   234,   412,   176,   395,    -1,   256,
     234,   386,    -1,   256,   234,   389,    -1,   256,   234,   181,
     265,   182,    -1,   256,   234,   181,   264,   182,    -1,   256,
     234,   181,   263,   182,    -1,   256,   234,   414,   176,   181,
     262,   182,    -1,   256,   234,   413,   176,   181,   261,   182,
      -1,   256,   234,   412,   176,   181,   260,   182,    -1,   256,
     234,   181,   266,   182,    -1,   396,    -1,   260,   175,   396,
      -1,   394,    -1,   261,   175,   394,    -1,   392,    -1,   262,
     175,   392,    -1,   390,    -1,   263,   175,   390,    -1,   387,
      -1,   264,   175,   387,    -1,   398,    -1,   265,   175,   398,
      -1,   397,    -1,   266,   175,   397,    -1,    -1,    -1,   155,
     185,   177,   268,   402,   178,   269,   252,    -1,   185,   177,
     405,   178,    -1,   115,    -1,    -1,     6,    -1,     6,    -1,
     179,   398,   157,   398,   180,    -1,   179,   398,   180,    -1,
      -1,    -1,   122,   275,   179,   385,   180,   276,   181,   277,
     182,    -1,    -1,   278,   277,    -1,    -1,   123,   410,   176,
     279,   251,    -1,    -1,   125,   176,   280,   251,    -1,   124,
      -1,   235,   235,    -1,   121,    -1,   120,   257,   234,   398,
     282,   398,    -1,    -1,   283,   285,   252,    -1,    -1,   119,
     179,   385,   180,   287,   252,    -1,    -1,   273,   176,   289,
     253,    -1,    -1,   108,   273,   291,   292,    -1,   252,    -1,
      -1,   252,   293,   109,   252,    -1,    14,   176,   397,    -1,
      63,    -1,    63,   176,   410,    -1,    63,   176,    71,    -1,
      54,   176,   297,   175,   297,    -1,    54,   176,   297,   175,
     410,    -1,   177,     4,   175,    70,   175,   312,   178,    -1,
     370,   272,   175,   379,    -1,    -1,    99,   176,   298,   175,
     305,   175,   307,   371,   374,   376,   300,   252,    -1,    -1,
      48,   176,   298,   175,   304,   175,   306,   175,   307,   371,
     374,   376,   302,   252,    -1,    -1,   175,     5,    -1,   177,
       4,   175,     4,   178,    -1,    71,    -1,   177,     4,   175,
       4,   178,    -1,    71,    -1,   177,   316,   175,   317,   178,
      -1,    71,    -1,   177,     4,   175,     4,   178,    -1,    71,
      -1,   107,   176,   309,   175,   378,   175,   310,   175,   312,
     313,    -1,    25,   176,   378,   175,   407,   313,    -1,     5,
      -1,    71,    -1,   311,    -1,    71,    -1,    70,    -1,    70,
     236,   311,    -1,     4,    -1,    71,    -1,    -1,   313,   175,
     314,    -1,   113,   176,   114,    -1,    68,   176,   395,    -1,
      97,   176,     4,    -1,    20,    -1,    19,   176,   316,   175,
     317,   303,   184,    -1,    19,   176,   386,   303,   184,    -1,
      75,    -1,    76,    -1,    77,    -1,    76,    -1,    23,   176,
     320,    -1,    -1,    23,   176,   320,   319,   252,    -1,   393,
     175,   386,   321,    -1,    -1,   321,   175,   322,    -1,   397,
      -1,   103,    -1,   104,    -1,   381,    -1,   106,   397,    -1,
     131,    -1,   129,    -1,   133,    -1,   134,    -1,   135,    -1,
     136,   176,   385,    -1,   137,   176,   385,    -1,   138,   176,
     385,    -1,   139,    -1,   140,    -1,   141,   176,   323,    -1,
     132,    -1,   183,    -1,   142,    -1,   183,   236,   323,    -1,
      21,   176,   326,    -1,    -1,    22,   176,   326,   325,   252,
      -1,   395,   327,    -1,    -1,   327,   175,   328,    -1,    66,
      -1,   143,   176,   393,    -1,   411,    -1,   100,   176,   397,
      -1,   117,   176,   385,    -1,   118,    -1,    65,    -1,   126,
     176,   385,    -1,   154,    -1,    64,    -1,   127,    -1,   128,
     176,   385,    -1,   129,    -1,   130,    -1,   386,    -1,    94,
     176,    91,    -1,    93,   176,    92,    -1,    95,   176,   385,
      -1,    96,   176,   385,    -1,    97,   176,   385,    -1,    98,
     176,   385,    -1,    68,   176,   395,    -1,    24,   176,   369,
     175,   386,    -1,    26,   176,   386,   175,    70,   175,   378,
      -1,    30,   176,   386,   175,    70,    -1,    30,   176,   386,
     175,    70,   175,     5,   247,    -1,    31,    -1,    31,   176,
     407,    -1,    44,   176,   386,   175,    79,    -1,    45,   176,
     386,   175,    79,    -1,    45,   176,   416,   175,   416,   175,
      79,    -1,    49,   176,   416,   175,   416,   175,   183,    -1,
      50,   176,   416,   175,   416,   339,    -1,    51,   176,   416,
     175,   416,    -1,    -1,   175,    79,    -1,    57,   176,   407,
     175,    83,    -1,    58,   176,   407,    -1,    59,   176,   386,
     175,   150,   176,   386,   175,    81,   175,    80,    -1,    69,
     176,   386,   175,   147,   344,    -1,    -1,   344,   175,   345,
      -1,    80,    -1,   143,   176,   393,    -1,   144,   176,   395,
      -1,   145,   176,   349,    -1,   146,   176,   349,    -1,   148,
      -1,   149,    -1,   150,   176,   386,    -1,   151,   176,   369,
      -1,   113,   176,   346,    -1,    87,   176,   377,    -1,   152,
     176,     4,    -1,    97,   176,     4,    -1,     4,    -1,    81,
      -1,   114,    -1,    83,    -1,    61,   176,   407,    -1,    60,
     176,   407,    -1,     3,    -1,   177,     3,   175,   379,   178,
      -1,   112,   176,   389,   175,   391,   175,   391,   175,     7,
      -1,   110,   176,   407,   175,   391,    -1,    46,   176,   389,
      -1,    47,   176,   389,    -1,    36,   176,   183,   175,    37,
      -1,    36,   176,   183,   175,    37,   175,   143,   176,   393,
      -1,    33,   176,   389,   175,    35,    -1,    34,   176,   416,
     175,    35,    -1,    -1,    32,   176,   389,   175,   379,   175,
     370,   371,   374,   376,   358,   359,    -1,    -1,   252,    -1,
      41,   176,   386,   175,   380,   175,   382,    -1,    42,   176,
     386,    -1,    88,   176,   407,   175,    89,   175,    86,    -1,
     105,   176,   407,   175,     4,    -1,    43,   176,    82,   175,
     386,    -1,   153,   176,   386,   175,   397,    -1,   153,   176,
     386,   175,    71,    -1,   153,   176,   386,    -1,    55,   176,
     398,   175,   386,    -1,    56,   176,   386,   175,   415,   175,
     397,    -1,    53,   176,   385,   175,   385,   175,   385,   175,
     385,    -1,    53,    -1,   183,    -1,    71,    -1,   183,    -1,
      71,    -1,    -1,   175,   372,    -1,   373,    -1,   373,   175,
     372,    -1,    38,    -1,    39,    -1,    40,    -1,    -1,    90,
     176,   375,    -1,    89,    -1,    -1,    87,   176,   377,    -1,
      86,    -1,    64,    -1,    71,    -1,    65,    -1,    71,    -1,
      74,    -1,   383,    -1,    71,    -1,    74,    -1,   383,    -1,
      73,   176,    71,    -1,    78,    -1,    71,    -1,    73,   179,
       4,   180,    -1,   183,    -1,   194,    -1,   195,   179,   398,
     180,    -1,   398,    -1,   387,    -1,   166,   177,   407,   178,
      -1,   198,    -1,   199,   179,   398,   180,    -1,   177,     4,
     175,     4,   178,    -1,    71,    -1,    72,   388,   180,    -1,
     174,    -1,   174,   175,   388,    -1,   390,    -1,   200,    -1,
     201,   179,   398,   180,    -1,   177,     4,   175,     4,   175,
       4,   175,     4,   178,    -1,   392,    -1,   202,    -1,   203,
     179,   398,   180,    -1,     3,    -1,   177,     3,   175,   379,
     178,    -1,   394,    -1,   204,    -1,   205,   179,   398,   180,
      -1,   183,    -1,     3,    -1,   177,     3,   175,   183,   178,
      -1,    71,    -1,   396,    -1,   206,    -1,   207,   179,   398,
     180,    -1,   183,    -1,     3,    -1,   177,     3,   175,   183,
     178,    -1,    71,    -1,    84,    -1,   384,    -1,   397,   235,
     384,    -1,     4,    -1,   408,    -1,   177,     8,   178,    -1,
     192,    -1,   193,   179,   398,   180,    -1,   398,   229,   398,
      -1,   398,   230,   398,    -1,   398,   231,   398,    -1,   398,
     232,   398,    -1,   398,   233,   398,    -1,   177,   398,   178,
      -1,   188,    -1,   189,    -1,   256,   176,   399,    -1,   400,
      -1,   401,   175,   400,    -1,    -1,   401,    -1,   398,    -1,
     397,    -1,   403,    -1,   404,   175,   403,    -1,    -1,   404,
      -1,   386,    -1,   159,   389,    -1,   160,   389,    -1,   161,
     386,   175,   386,    -1,   162,   386,   175,   386,   175,   398,
      -1,   163,   177,   407,   178,    -1,   163,   177,   311,   175,
     407,   178,    -1,   169,   177,     7,   175,   407,   178,    -1,
     169,   177,   407,   175,   407,   178,    -1,   169,   177,   391,
     175,   407,   178,    -1,   165,   386,    -1,   167,   177,   386,
     175,   398,   178,    -1,   167,   177,   386,   175,   398,   175,
      38,   178,    -1,   168,   177,   386,   175,   398,   175,   398,
     178,    -1,   168,   177,   386,   175,   398,   175,   398,   175,
      38,   178,    -1,   171,   177,   172,   175,   177,   398,   230,
     398,   245,   178,   175,   386,   246,   178,    -1,   170,   406,
      -1,   208,    -1,   177,   407,   178,    -1,   406,    -1,   406,
     237,   407,    -1,   228,    -1,    85,    -1,     4,    -1,     8,
      -1,     9,    -1,     4,    -1,     8,    -1,     9,    -1,     4,
      -1,   408,    -1,    27,    -1,    21,    -1,    28,    -1,    23,
      -1,    29,    -1,   110,    -1,    67,    -1,    71,    -1,   417,
      -1,    52,   177,     4,   175,     4,   175,     4,   175,     4,
     178,    -1,   177,     4,   175,     4,   175,     4,   175,     4,
     178,    -1
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
    1278,  1302,  1301,  1323,  1326,  1332,  1342,  1348,  1357,  1363,
    1368,  1374,  1379,  1385,  1396,  1402,  1403,  1406,  1407,  1410,
    1414,  1420,  1421,  1425,  1431,  1439,  1444,  1449,  1457,  1464,
    1472,  1480,  1481,  1484,  1485,  1488,  1493,  1492,  1506,  1513,
    1520,  1528,  1533,  1539,  1545,  1551,  1557,  1562,  1567,  1572,
    1577,  1582,  1587,  1592,  1597,  1602,  1607,  1613,  1620,  1629,
    1633,  1646,  1655,  1654,  1672,  1682,  1688,  1696,  1702,  1707,
    1712,  1717,  1722,  1727,  1732,  1737,  1742,  1753,  1759,  1764,
    1769,  1774,  1779,  1784,  1789,  1794,  1799,  1804,  1809,  1816,
    1822,  1851,  1856,  1864,  1870,  1876,  1883,  1890,  1900,  1910,
    1925,  1936,  1939,  1945,  1951,  1957,  1963,  1970,  1976,  1984,
    1990,  1995,  2000,  2005,  2010,  2016,  2022,  2027,  2032,  2037,
    2042,  2047,  2054,  2054,  2054,  2054,  2057,  2063,  2069,  2074,
    2081,  2088,  2094,  2100,  2106,  2110,  2116,  2122,  2133,  2132,
    2164,  2167,  2173,  2180,  2186,  2192,  2198,  2204,  2208,  2213,
    2220,  2226,  2233,  2237,  2244,  2252,  2255,  2265,  2269,  2272,
    2278,  2282,  2289,  2293,  2297,  2304,  2307,  2313,  2320,  2323,
    2329,  2335,  2336,  2339,  2340,  2343,  2344,  2345,  2351,  2352,
    2353,  2359,  2360,  2363,  2372,  2377,  2384,  2395,  2401,  2405,
    2409,  2416,  2426,  2433,  2437,  2443,  2447,  2455,  2459,  2466,
    2476,  2489,  2493,  2500,  2510,  2519,  2530,  2534,  2541,  2551,
    2562,  2571,  2581,  2587,  2591,  2598,  2608,  2619,  2628,  2638,
    2642,  2649,  2650,  2656,  2660,  2664,  2668,  2676,  2685,  2689,
    2693,  2697,  2701,  2705,  2708,  2715,  2724,  2757,  2758,  2761,
    2762,  2765,  2769,  2776,  2783,  2794,  2797,  2805,  2809,  2813,
    2817,  2821,  2826,  2830,  2834,  2839,  2844,  2849,  2853,  2858,
    2863,  2867,  2871,  2876,  2880,  2887,  2893,  2897,  2903,  2910,
    2911,  2914,  2915,  2916,  2919,  2923,  2927,  2931,  2937,  2938,
    2941,  2942,  2945,  2946,  2949,  2950,  2953,  2957,  2983
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
  "DOOR_STATE", "LIGHT_STATE", "CURSE_TYPE", "ENGRAVING_TYPE",
  "KEYTYPE_ID", "LEVER_ID", "DIRECTION", "RANDOM_TYPE",
  "RANDOM_TYPE_BRACKET", "A_REGISTER", "ALIGNMENT", "LEFT_OR_RIGHT",
  "CENTER", "TOP_OR_BOT", "ALTAR_TYPE", "UP_OR_DOWN", "ACTIVE_OR_INACTIVE",
  "MODRON_PORTAL_TYPE", "NPC_TYPE", "FOUNTAIN_TYPE", "SPECIAL_OBJECT_TYPE",
  "CMAP_TYPE", "FLOOR_TYPE", "FLOOR_TYPE_ID", "FLOOR_ID",
  "FLOOR_MAIN_TYPE", "FLOOR_MAIN_TYPE_ID", "ELEMENTAL_ENCHANTMENT_TYPE",
  "EXCEPTIONALITY_TYPE", "EXCEPTIONALITY_ID", "ELEMENTAL_ENCHANTMENT_ID",
  "ENCHANTMENT_ID", "CHARGES_ID", "SPECIAL_QUALITY_ID", "SPEFLAGS_ID",
  "SUBROOM_ID", "NAME_ID", "FLAGS_ID", "FLAG_TYPE", "MON_ATTITUDE",
  "MON_ALERTNESS", "SUBTYPE_ID", "MON_APPEARANCE", "ROOMDOOR_ID", "IF_ID",
  "ELSE_ID", "TERRAIN_ID", "HORIZ_OR_VERT", "REPLACE_TERRAIN_ID",
  "LOCATION_SUBTYPE_ID", "DOOR_SUBTYPE", "EXIT_ID", "SHUFFLE_ID",
  "QUANTITY_ID", "BURIED_ID", "LOOP_ID", "FOR_ID", "TO_ID", "SWITCH_ID",
  "CASE_ID", "BREAK_ID", "DEFAULT_ID", "ERODED_ID", "TRAPPED_STATE",
  "RECHARGED_ID", "INVIS_ID", "GREASED_ID", "FEMALE_ID", "WAITFORU_ID",
  "CANCELLED_ID", "REVIVED_ID", "AVENGE_ID", "FLEEING_ID", "BLINDED_ID",
  "PARALYZED_ID", "STUNNED_ID", "CONFUSED_ID", "SEENTRAPS_ID", "ALL_ID",
  "MONTYPE_ID", "OBJTYPE_ID", "TERTYPE_ID", "TERTYPE2_ID",
  "LEVER_EFFECT_TYPE", "SWITCHABLE_ID", "CONTINUOUSLY_USABLE_ID",
  "TARGET_ID", "TRAPTYPE_ID", "EFFECT_FLAG_ID", "GRAVE_ID",
  "ERODEPROOF_ID", "FUNCTION_ID", "MSG_OUTPUT_TYPE", "COMPARE_TYPE",
  "UNKNOWN_TYPE", "rect_ID", "fillrect_ID", "line_ID", "randline_ID",
  "grow_ID", "selection_ID", "flood_ID", "rndcoord_ID", "circle_ID",
  "ellipse_ID", "filter_ID", "complement_ID", "gradient_ID",
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
  "door_wall", "dir_list", "door_pos", "door_infos", "door_info",
  "map_definition", "h_justif", "v_justif", "monster_detail", "$@14",
  "monster_desc", "monster_infos", "monster_info", "seen_trap_mask",
  "object_detail", "$@15", "object_desc", "object_infos", "object_info",
  "trap_detail", "drawbridge_detail", "mazewalk_detail", "wallify_detail",
  "ladder_detail", "stair_detail", "stair_region", "portal_region",
  "teleprt_region", "branch_region", "teleprt_detail", "fountain_detail",
  "throne_detail", "modron_portal_detail", "lever_detail", "lever_infos",
  "lever_info", "valid_subtype", "sink_detail", "pool_detail",
  "terrain_type", "replace_terrain_detail", "terrain_detail",
  "diggable_detail", "passwall_detail", "naming_detail",
  "special_region_detail", "special_levregion_detail", "region_detail",
  "@16", "region_detail_end", "altar_detail", "anvil_detail",
  "floor_detail", "subtype_detail", "npc_detail", "grave_detail",
  "gold_detail", "engraving_detail", "mineralize", "trap_name",
  "room_type", "optroomregionflags", "roomregionflags", "roomregionflag",
  "optfloormaintype", "floormaintype", "optfloortype", "floortype",
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
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,    44,    58,    40,    41,    91,
      93,   123,   125,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,    43,
      45,    42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   238,   239,   239,   240,   240,   241,   242,   242,   243,
     243,   243,   243,   244,   245,   245,   246,   246,   247,   247,
     248,   248,   249,   249,   250,   250,   251,   251,   252,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   256,   256,   256,   257,   257,   258,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   260,   260,   261,   261,   262,   262,   263,
     263,   264,   264,   265,   265,   266,   266,   268,   269,   267,
     270,   271,   272,   272,   273,   273,   273,   275,   276,   274,
     277,   277,   279,   278,   280,   278,   281,   282,   282,   283,
     285,   284,   287,   286,   289,   288,   291,   290,   292,   293,
     292,   294,   295,   295,   295,   296,   296,   297,   298,   300,
     299,   302,   301,   303,   303,   304,   304,   305,   305,   306,
     306,   307,   307,   308,   308,   309,   309,   310,   310,   311,
     311,   312,   312,   313,   313,   314,   314,   314,   315,   315,
     315,   316,   316,   317,   317,   318,   319,   318,   320,   321,
     321,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   323,   323,
     323,   324,   325,   324,   326,   327,   327,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   329,
     330,   331,   331,   332,   332,   333,   334,   335,   336,   337,
     338,   339,   339,   340,   341,   342,   343,   344,   344,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   346,   346,   346,   346,   347,   348,   349,   349,
     350,   351,   352,   353,   354,   354,   355,   356,   358,   357,
     359,   359,   360,   361,   362,   363,   364,   365,   365,   365,
     366,   367,   368,   368,   369,   369,   370,   370,   371,   371,
     372,   372,   373,   373,   373,   374,   374,   375,   376,   376,
     377,   378,   378,   379,   379,   380,   380,   380,   381,   381,
     381,   382,   382,   383,   384,   384,   384,   385,   386,   386,
     386,   386,   387,   387,   387,   388,   388,   389,   389,   389,
     390,   391,   391,   391,   392,   392,   393,   393,   393,   394,
     394,   394,   394,   395,   395,   395,   396,   396,   396,   396,
     396,   397,   397,   398,   398,   398,   398,   398,   398,   398,
     398,   398,   398,   398,   399,   399,   400,   401,   401,   402,
     402,   403,   403,   404,   404,   405,   405,   406,   406,   406,
     406,   406,   406,   406,   406,   406,   406,   406,   406,   406,
     406,   406,   406,   406,   406,   406,   407,   407,   408,   409,
     409,   410,   410,   410,   411,   411,   411,   411,   412,   412,
     413,   413,   414,   414,   415,   415,   416,   416,   417
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
      12,     0,    14,     0,     2,     5,     1,     5,     1,     5,
       1,     5,     1,    10,     6,     1,     1,     1,     1,     1,
       3,     1,     1,     0,     3,     3,     3,     3,     1,     7,
       5,     1,     1,     1,     1,     3,     0,     5,     4,     0,
       3,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     3,     3,     3,     1,     1,     3,     1,     1,     1,
       3,     3,     0,     5,     2,     0,     3,     1,     3,     1,
       3,     3,     1,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     5,
       7,     5,     8,     1,     3,     5,     5,     7,     7,     6,
       5,     0,     2,     5,     3,    11,     6,     0,     3,     1,
       3,     3,     3,     3,     1,     1,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     3,     3,     1,     5,
       9,     5,     3,     3,     5,     9,     5,     5,     0,    12,
       0,     1,     7,     3,     7,     5,     5,     5,     5,     3,
       5,     7,     9,     1,     1,     1,     1,     1,     0,     2,
       1,     3,     1,     1,     1,     0,     3,     1,     0,     3,
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
       5,     0,    26,     0,     7,     0,   144,     0,     0,     0,
       0,   208,     0,     0,     0,     0,     0,     0,     0,   273,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   333,     0,     0,     0,     0,
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
     403,     0,   406,     0,   448,     0,   404,   425,    27,     0,
     164,     0,     8,     0,   364,   365,     0,   401,   171,     0,
       0,     0,    11,   450,   449,    13,   373,     0,   211,   212,
       0,     0,   370,     0,     0,   183,   368,   397,   399,   400,
       0,   396,   394,     0,   241,   245,   393,   242,   390,   392,
       0,   389,   387,     0,   215,     0,   386,   335,   334,     0,
     351,   352,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   444,   427,   446,
     274,     0,   378,     0,     0,   377,     0,     0,     0,     0,
     466,     0,     0,   323,     0,     0,     0,     0,     0,   312,
     313,   337,   336,     0,   142,     0,     0,     0,     0,   367,
       0,     0,     0,     0,     0,   284,     0,   307,   306,   453,
     451,   452,   174,   173,     0,     0,     0,     0,   195,   196,
       0,     0,     0,     0,   107,     0,     0,     0,   329,   137,
       0,     0,     0,     0,   146,     0,     0,     0,     0,     0,
     422,   421,   423,   426,     0,   459,   461,   458,   460,   462,
     463,     0,     0,     0,   114,   115,   110,   108,     0,     0,
       0,     0,    26,   161,    24,     0,     0,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     244,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     428,   429,     0,     0,     0,   437,     0,     0,     0,   443,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   143,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   168,   167,     0,     0,   162,
       0,     0,     0,   419,   405,   413,     0,     0,   408,   409,
     410,   411,   412,     0,   140,     0,   403,     0,     0,     0,
       0,   131,   129,   135,   133,     0,     0,     0,   165,     0,
       0,   402,    10,   308,     0,     9,     0,     0,   374,     0,
       0,     0,   214,   213,   183,   184,   210,     0,     0,     0,
     243,     0,     0,   217,   219,   269,   203,     0,   271,     0,
       0,   199,     0,     0,     0,     0,   384,     0,     0,   382,
       0,     0,   381,     0,     0,   445,   447,     0,     0,   353,
     354,     0,   316,     0,     0,   317,   314,   357,     0,   355,
       0,   356,   326,   275,     0,   276,     0,   186,     0,     0,
       0,     0,   281,   280,     0,     0,   175,   176,   330,   464,
     465,     0,   283,     0,   287,     0,   188,     0,     0,   325,
       0,     0,     0,   311,     0,     0,   158,     0,     0,   148,
     328,   327,     0,   417,   420,     0,   407,   145,   424,   109,
       0,     0,   118,     0,   117,     0,   116,     0,   122,     0,
     113,     0,   112,     0,   111,    28,   366,     0,     0,   376,
     369,     0,   371,     0,     0,   395,   456,   454,   455,   256,
     253,   247,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   252,     0,   257,     0,   259,   260,     0,   255,   246,
     261,   457,   249,     0,   388,   218,   194,     0,     0,   430,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   178,     0,     0,   279,     0,     0,
       0,     0,   286,     0,     0,     0,     0,     0,     0,   163,
     157,   159,     0,     0,     0,   138,     0,   130,   132,   134,
     136,     0,   123,     0,   125,     0,   127,     0,     0,   372,
     209,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   391,     0,     0,   270,    18,     0,
     200,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   338,     0,     0,     0,     0,   362,   361,   322,   277,
       0,   190,     0,     0,   278,   282,     0,     0,   331,     0,
       0,   324,     0,   192,     0,   338,   198,     0,   197,   170,
       0,   150,   414,   415,   416,   418,     0,     0,   121,     0,
     120,     0,   119,     0,     0,   268,   263,   262,   264,   265,
     266,   267,   250,   251,   254,   258,   248,     0,   358,   222,
     223,     0,   227,   226,   237,   228,   229,   230,     0,     0,
       0,   234,   235,     0,   220,   224,   359,   221,     0,     0,
       0,   204,     0,   272,   431,   433,     0,   438,     0,   434,
       0,   383,   436,   435,     0,     0,     0,   345,     0,     0,
       0,   363,     0,     0,     0,     0,   201,   202,     0,     0,
     289,     0,     0,     0,     0,     0,     0,     0,   294,   295,
       0,     0,     0,   288,     0,     0,   345,     0,     0,     0,
       0,     0,   150,   139,   124,   126,   128,   309,     0,     0,
     225,     0,     0,     0,     0,     0,     0,     0,    19,     0,
       0,   385,     0,     0,   342,   343,   344,   339,   340,     0,
     348,     0,     0,   315,   185,     0,   338,   332,   177,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     187,     0,   348,   203,   310,     0,   154,   149,   151,     0,
     360,   231,   232,   233,   239,   238,   236,   206,   207,   205,
     439,     0,   440,   409,     0,     0,     0,     0,   318,     0,
       0,     0,   345,     0,   350,   299,   301,   302,   303,   305,
     304,   298,   290,   291,   292,   293,   296,   297,   300,     0,
     179,   193,   152,    26,     0,     0,     0,     0,     0,   380,
     341,   347,   346,     0,   320,     0,   468,   189,   348,   285,
     191,     0,    26,   155,     0,   240,   441,    15,     0,   349,
     321,   319,   467,   181,   180,   153,     0,     0,     0,     0,
      16,   182,    20,    21,    18,     0,     0,    12,    17,   442
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    92,    93,   988,  1026,   833,
    1024,    12,   156,    94,   383,    95,    96,    97,    98,   206,
      99,   100,   711,   713,   715,   477,   478,   479,   480,   101,
     463,   786,   102,   103,   439,   104,   105,   207,   702,   881,
     882,  1002,   983,   106,   588,   107,   108,   221,   109,   585,
     110,   381,   111,   341,   456,   581,   112,   113,   114,   321,
     313,   115,  1001,   116,  1018,   397,   559,   578,   763,   775,
     117,   340,   777,   522,   858,   656,   831,   118,   244,   504,
     119,   404,   264,   655,   824,   946,   120,   401,   254,   400,
     649,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   687,   131,   132,   133,   134,   692,   873,   971,   135,
     136,   495,   137,   138,   139,   140,   141,   142,   143,   144,
     994,  1011,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   269,   314,   847,   907,   908,   910,   992,   958,   965,
     272,   541,   550,   825,   758,   551,   227,   318,   288,   246,
     391,   294,   295,   531,   532,   265,   266,   255,   256,   360,
     319,   784,   593,   594,   595,   362,   363,   364,   289,   421,
     216,   235,   333,   652,   378,   379,   380,   571,   299,   300
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -869
static const yytype_int16 yypact[] =
{
      70,   -82,   -76,   106,  -869,    70,    74,   -26,    30,  -869,
    -869,    20,   761,    50,  -869,   129,  -869,    69,    99,   101,
     117,  -869,   142,   144,   146,   147,   152,   154,   156,   163,
     179,   182,   184,   190,   191,   195,   196,   202,   214,   220,
     221,   232,   233,   237,   238,   244,   246,   247,   249,   255,
     265,   266,   267,   270,   271,   273,   275,   276,   278,   285,
      31,   303,   307,  -869,   308,    88,   816,  -869,  -869,   309,
      97,    79,   112,  -869,  -869,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,  -869,   761,  -869,  -869,   120,  -869,
    -869,  -869,  -869,  -869,   311,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,   378,   283,  -869,  -105,   324,    29,
     293,   173,   173,   212,   -45,    41,   -15,   -15,   666,   -84,
     -84,   -17,   236,   -15,   -15,   382,   -15,   -18,   -84,   -84,
     -12,   -17,   -17,   -17,    79,   312,    79,   -15,   666,   666,
     -15,   666,   666,    83,   -15,   666,   -12,   666,    47,  -869,
     666,   -84,   810,    79,  -869,  -869,   254,   318,   -15,   313,
    -869,    40,  -869,   319,  -869,   169,  -869,    35,  -869,   193,
    -869,   341,  -869,   129,  -869,  -869,   352,  -869,   297,   325,
     359,   360,  -869,  -869,  -869,  -869,  -869,   366,  -869,  -869,
     370,   539,  -869,   369,   374,   380,  -869,  -869,  -869,  -869,
     554,  -869,  -869,   379,  -869,  -869,  -869,  -869,  -869,  -869,
     562,  -869,  -869,   391,   387,   396,  -869,  -869,  -869,   397,
    -869,  -869,   399,   403,   404,   -84,   -84,   -15,   -15,   410,
     -15,   421,   422,   424,   666,   425,   537,  -869,  -869,   343,
    -869,   601,  -869,   427,   445,  -869,   448,   447,   626,   457,
    -869,   463,   477,  -869,   479,   481,   651,   483,   484,  -869,
    -869,  -869,  -869,   486,   656,   488,   489,   490,   491,   331,
     663,   493,   243,   494,   496,  -869,   499,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,   501,   504,   506,   508,  -869,  -869,
     509,   341,   510,   511,  -869,   495,    79,    79,   512,  -869,
     514,   282,    79,    79,  -869,    79,    79,    79,    79,    79,
     297,   331,  -869,   518,   517,  -869,  -869,  -869,  -869,  -869,
    -869,   563,    58,    10,  -869,  -869,   297,   331,   564,   565,
     566,   761,   761,  -869,  -869,    79,  -105,   712,    22,   740,
     569,   567,   666,   571,    79,   178,   743,   573,   574,    79,
     583,   341,   584,    79,   341,   -15,   -15,   666,   690,   691,
    -869,  -869,   587,   588,   557,  -869,   -15,   -15,   423,  -869,
     592,   590,   666,   591,    79,    32,   741,   784,   614,   755,
     759,    61,   -15,   719,   624,   721,   -17,   -22,  -869,   638,
     -17,   -17,   -17,    79,   648,    34,   -15,   119,   756,   688,
     693,   753,    -3,   840,    41,   736,  -869,    76,    76,  -869,
     -63,   668,   -31,   792,  -869,  -869,   386,   480,   113,   113,
    -869,  -869,  -869,    35,  -869,   666,   671,   -67,   -54,   -51,
     -39,  -869,  -869,   297,   331,    17,   136,   150,  -869,   673,
     500,  -869,  -869,  -869,   853,  -869,   682,   366,  -869,   680,
     859,   521,  -869,  -869,   380,  -869,  -869,   687,   541,   339,
    -869,   689,   621,  -869,  -869,  -869,  -869,   700,   703,   -15,
     -15,   643,   709,   708,   714,   715,  -869,   718,   474,  -869,
     716,   726,  -869,   727,   728,  -869,  -869,   883,   667,  -869,
    -869,   729,  -869,   730,   902,  -869,   733,  -869,   731,  -869,
     734,  -869,  -869,  -869,   907,  -869,   737,  -869,   909,   762,
      32,   763,   764,  -869,   766,   845,  -869,  -869,  -869,  -869,
    -869,   767,  -869,   742,  -869,   768,  -869,   926,   769,  -869,
     770,   827,   945,  -869,   774,   341,  -869,   717,    79,  -869,
    -869,   297,   775,  -869,   797,   772,  -869,  -869,  -869,  -869,
     969,   798,  -869,     0,  -869,    79,  -869,  -105,  -869,   107,
    -869,    66,  -869,    24,  -869,  -869,  -869,   799,   973,  -869,
    -869,   801,  -869,   793,   802,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,   805,   828,   830,   847,   849,   850,   851,   852,
     854,  -869,   855,  -869,   856,  -869,  -869,   857,  -869,  -869,
    -869,  -869,  -869,   843,  -869,   860,   861,    41,  1024,  -869,
     862,   964,   666,  -869,    79,    79,   666,   863,    79,   666,
     666,   864,   865,  -869,   -12,  1035,   867,   900,  1040,   114,
     -52,   966,   871,     2,  -869,   866,   968,  -869,    79,   873,
    -105,   -15,   875,   965,   877,     5,   199,   341,    76,  -869,
    -869,   331,   872,   121,   792,  -869,   -29,  -869,  -869,   331,
     297,    13,  -869,    64,  -869,    91,  -869,    32,   879,  -869,
    -869,  -869,   173,   963,   967,    79,    79,    79,    79,  -105,
      79,    79,    79,   212,  -869,   788,    16,  -869,   881,    79,
    -869,   882,   321,   288,   884,    32,   702,   885,   886,    79,
    1053,   890,   891,  1055,   892,   887,  -869,  -869,  -869,  -869,
    1057,  -869,   226,   894,  -869,  -869,   895,    56,   297,   896,
     358,  -869,  1068,  -869,  1069,   890,  -869,   899,  -869,  -869,
     901,   111,  -869,  -869,  -869,  -869,   341,   107,  -869,    66,
    -869,    24,  -869,   897,  1072,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,   297,  -869,  -869,  -869,  -869,   -32,  -869,  -869,
    -869,  -105,  -869,  -869,  -869,  -869,  -869,  -869,   903,   904,
     905,  -869,  -869,   906,  -869,  -869,  -869,   297,   908,   910,
     911,  -869,  1075,  -869,   331,  -869,  1045,  -869,    79,  -869,
     912,  -869,  -869,  -869,   417,   913,   389,   995,  1085,   916,
     212,  -869,   914,   918,     5,    79,  -869,  -869,   917,  1013,
    -869,   920,   921,   922,   923,   924,   925,   927,  -869,  -869,
     928,   929,   930,  -869,   931,   932,   995,    56,  1095,   272,
     934,   933,   111,  -869,  -869,  -869,  -869,  -869,   936,  1037,
     297,    79,    79,    79,   -72,   173,  1108,   999,  -869,   938,
     381,  -869,    79,  1110,  -869,  -869,  -869,  -869,   942,   943,
    1031,   946,  1116,  -869,  -869,   178,   890,  -869,  -869,   947,
    1038,  1119,    59,   212,   173,    22,    22,   -15,   -45,  1121,
    -869,  1122,  1031,  -869,  -869,   951,  -869,  -869,  -869,  1123,
    -869,  -869,  -869,  -869,  -869,   893,  -869,  -869,  -869,  -869,
    -869,  1092,  -869,   364,   953,   389,  1043,   957,  -869,  1130,
     958,   959,   995,  1058,  -869,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,   961,
    -869,   861,  -869,   761,   960,   -72,   962,   970,   971,  -869,
    -869,  -869,  -869,  1038,   341,   972,  -869,  -869,  1031,  -869,
    -869,   341,   761,  -869,    32,  -869,  -869,  -869,   976,  -869,
    -869,  -869,  -869,  -869,  -869,  -869,   977,   -15,   341,    62,
     978,  -869,  -869,  -869,   881,   -15,   979,  -869,  -869,  -869
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -869,  -869,  1136,  -869,  -869,  -869,  -869,  -869,  -869,   118,
    -869,  -869,   935,   -95,  -324,   765,   952,  1078,  -452,  -869,
    -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,  -869,  1087,  -869,  -869,  -869,   263,
    -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,   710,
     974,  -869,  -869,  -869,  -869,   644,  -869,  -869,  -869,   302,
    -869,  -869,  -869,  -587,   284,   227,  -869,  -869,   400,   248,
    -869,  -869,  -869,  -869,  -869,   174,  -869,  -869,  1002,  -869,
    -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,
    -869,  -610,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,
    -869,   239,   492,  -751,   210,  -869,  -821,  -869,  -868,   175,
    -431,  -556,  -869,  -869,  -869,   434,   785,  -202,  -158,  -366,
     675,   205,  -361,  -436,  -585,  -473,  -581,  -480,  -578,  -154,
     -61,  -869,   469,  -869,  -869,   701,  -869,  -869,   898,   -93,
     669,  -869,  -439,  -869,  -869,  -869,  -869,  -869,  -136,  -869
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -217
static const yytype_int16 yytable[] =
{
     218,   345,   245,   228,   684,   610,   567,   481,   273,   274,
     215,   592,   482,   612,   210,   302,   303,   455,   305,   307,
     247,   583,   584,   580,   876,   493,   267,   526,   716,   323,
     714,   712,   326,   233,   297,   297,   334,    16,   329,   210,
     590,   308,   330,   331,   210,   315,   316,   317,   350,   557,
     348,   614,   338,   236,   237,   932,   236,   237,   586,   311,
     856,   374,   476,   967,   980,   376,   350,  1022,   576,   258,
     944,   236,   237,   761,   740,   290,   773,   510,   224,   526,
     513,   236,   237,   210,   828,     1,     2,   329,   248,   225,
     226,   330,   331,   291,     7,   324,   325,   539,   327,   328,
       8,   249,   335,   540,   337,   270,     9,   342,   601,   778,
     247,   945,   271,   829,   234,   602,   292,   293,   339,   412,
     413,   603,   415,   753,   605,   322,   719,   857,   604,   830,
    1013,   606,   547,  1023,   548,   549,   607,   259,   268,   258,
     968,   998,   969,   608,   889,   461,   750,   678,   240,   719,
     351,   240,   224,   526,   332,   558,   361,    13,   377,   306,
     298,   793,   241,   225,   226,   962,   355,   356,   357,   358,
     359,   312,   587,   970,   577,    11,   247,   241,   248,   762,
     242,   243,   774,   242,   243,   756,   569,   372,   787,   840,
     570,   249,   757,   224,   250,   788,    15,   210,   609,   494,
     251,   582,   212,   213,   225,   226,   886,   259,   885,   884,
      71,   320,   211,    14,   365,   258,   366,   211,   224,   483,
     367,   368,   369,   252,   253,   154,   737,   212,   213,   225,
     226,   155,   212,   213,   879,   211,   880,   708,   214,   789,
     707,   564,   795,   260,   248,   157,   790,   514,   515,   261,
     212,   213,   592,   582,   502,   503,   211,   249,   524,   525,
     806,   699,   780,   214,   236,   237,   791,   203,   214,   521,
     776,   212,   213,   792,   552,   158,   329,   159,   529,   530,
     330,   331,   209,   259,   250,   460,   214,   489,   568,   217,
     251,   466,   467,   160,   468,   469,   470,   471,   472,   499,
     556,   238,   239,   370,   561,   562,   563,   214,   591,   782,
     783,   351,   484,   260,   516,   974,   975,   611,   161,   261,
     162,   523,   163,   164,   490,   533,   353,   582,   165,   536,
     166,   613,   167,   501,   229,   230,   231,   232,   508,   168,
     262,   263,   512,   626,   357,   358,   359,   627,   628,   354,
     250,   650,   529,   530,   219,   169,   251,   371,   170,   240,
     171,   659,   660,   538,   236,   237,   172,   173,   238,   239,
     372,   174,   175,   779,   373,   296,   224,   913,   176,   252,
     253,   222,   599,   309,   310,   212,   213,   225,   226,   260,
     177,   242,   243,   292,   293,   261,   178,   179,   355,   356,
     357,   358,   359,   629,   630,   631,   343,   632,   180,   181,
     236,   237,   361,   182,   183,   947,   262,   263,   446,   301,
     184,   214,   185,   186,   375,   187,   526,   904,   905,   906,
     527,   188,   633,   634,   635,   636,   637,   638,   860,   639,
     935,   189,   190,   191,   973,   861,   192,   193,  1016,   194,
     972,   195,   196,   710,   197,   862,   640,   641,   223,   240,
     465,   198,   883,   838,   304,   642,   643,   644,   645,   646,
     241,   863,   355,   356,   357,   358,   359,   667,   393,   200,
     410,   411,   647,   201,   202,   208,   766,   220,   346,   320,
     349,   242,   243,   648,   236,   237,   836,   347,   352,   837,
     387,   864,   865,   866,   867,   240,   868,   869,   870,   871,
     872,   355,   356,   357,   358,   359,   241,   355,   356,   357,
     358,   359,   382,   798,   799,   800,   801,   701,   803,   804,
     805,   385,   386,   769,   388,   389,   768,   242,   243,   987,
     390,   393,   -14,   393,   709,   236,   237,   392,   394,   395,
     355,   356,   357,   358,   359,   396,   951,   398,   399,   952,
     355,   356,   357,   358,   359,   402,   596,   214,  -216,   741,
     403,   405,   406,   744,   407,   802,   747,   748,   408,   409,
     422,   827,   275,   276,   277,   278,   279,   414,   280,   240,
     281,   282,   283,   284,   285,   357,   358,   359,   416,   417,
     528,   418,   420,   742,   743,   423,   424,   746,   236,   237,
     355,   356,   357,   358,   359,   355,   356,   357,   358,   359,
     425,   242,   243,   426,   427,   529,   530,   521,   236,   237,
     428,   287,   429,   275,   276,   277,   278,   279,   430,   280,
     240,   281,   282,   283,   284,   285,   355,   902,   357,   358,
     359,   286,   431,   917,   432,   434,   433,   890,   435,   436,
     597,   437,   438,   440,   441,   442,   443,   444,   445,   447,
    1010,   448,   242,   243,   449,   459,   450,  1014,   834,   451,
     616,   452,   287,   453,   454,   457,   458,   462,   844,   941,
     942,   943,   464,   473,  1021,   474,   275,   276,   277,   278,
     279,   622,   280,   240,   281,   282,   283,   284,   285,   355,
     356,   357,   358,   359,   286,   492,   275,   276,   277,   278,
     279,   625,   280,   240,   281,   282,   283,   284,   285,   355,
     356,   357,   358,   359,   286,   242,   243,   236,   237,   475,
     485,   486,   487,   496,   497,   287,   500,   498,   505,   507,
     355,   356,   357,   358,   359,   242,   243,   506,   509,   511,
     517,   518,   519,   520,   534,   287,   537,    16,   535,   976,
     355,   356,   357,   358,   359,    17,   542,   900,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,   543,   544,
     545,    28,    29,    30,    31,    32,   546,    33,   553,   554,
     555,   654,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,   560,    45,    46,    47,    48,    49,    50,
      51,    52,    53,   565,    54,   275,   276,   277,   278,   279,
      55,   280,   240,   281,   282,   283,   284,   285,   573,   572,
     574,   953,   575,   286,   579,  -169,   600,   673,   589,    56,
     355,   356,   357,   358,   359,   615,   617,   618,   620,  1020,
      57,   807,   808,   621,   242,   243,    58,  1028,    59,    60,
     624,    61,   653,    62,   287,   657,    63,    64,   658,   661,
      65,    66,   841,    67,   662,    68,   663,   672,  1003,   664,
     665,   809,   810,   666,   811,   668,   355,   356,   357,   358,
     359,   669,   670,   671,   674,   675,   676,  1015,   677,   679,
     678,   680,   681,   682,    69,   689,    70,   812,   691,   813,
     814,   815,   816,   817,   818,   819,   820,   821,   822,   823,
     694,   355,   356,   357,   358,   359,   697,   683,   685,   686,
      71,   688,   690,   693,   695,   696,    72,    73,   667,   698,
     705,   703,   700,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,   224,   704,   706,   717,   291,   718,   720,    73,   719,
     721,   722,   225,   226,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,   204,    75,   723,    77,   724,    79,    74,    81,
      76,    83,    78,    85,    80,    87,    82,    89,    84,    91,
      86,   734,    88,   725,    90,   726,   727,   728,   729,   738,
     730,   731,   732,   733,   521,   735,   736,   739,   745,   752,
     750,   749,   753,   754,   755,   759,   760,   765,   767,   764,
     770,   771,   772,   781,   794,   796,   832,   845,   797,   849,
     835,   852,   839,   842,   843,   846,   848,   851,   850,   854,
     855,   859,   874,   875,   877,   887,   878,   888,   898,   891,
     892,   893,   894,   899,   895,   909,   896,   897,   903,   911,
     901,   912,   914,   915,   919,   918,   920,   921,   922,   923,
     924,   925,   934,   926,   927,   928,   929,   931,   940,   930,
     936,   939,   948,   949,   954,   937,   950,   955,   957,   956,
     960,   959,   963,   966,   964,   978,   979,   982,   984,   985,
     986,   989,   991,   993,   995,  1004,   996,   997,   999,  1000,
    1006,    10,  1027,  1007,   205,   938,   488,   199,   623,  1008,
    1012,  1017,  1019,  1025,   344,   566,   916,  1029,   384,  1005,
     981,   933,   853,   961,   257,   990,   751,   977,  1009,   826,
     336,   491,   619,   785,   598,     0,     0,     0,   651,     0,
       0,     0,   419
};

static const yytype_int16 yycheck[] =
{
      95,   203,   160,   157,   560,   485,   445,   373,   166,   167,
      71,   463,   373,   486,     4,   173,   174,   341,   176,   177,
       3,   457,   458,   454,   775,     3,    71,     3,   613,   187,
     611,   609,   190,     4,    52,    52,   194,     6,     4,     4,
      71,   177,     8,     9,     4,   181,   182,   183,     8,    71,
     208,   487,     5,    71,    72,   876,    71,    72,   121,    71,
       4,   219,     4,     4,   932,   219,     8,     5,    71,     3,
     142,    71,    72,    71,   661,   168,    71,   401,   183,     3,
     404,    71,    72,     4,    68,    15,    16,     4,    71,   194,
     195,     8,     9,   177,   176,   188,   189,    65,   191,   192,
     176,    84,   195,    71,   197,    64,     0,   200,   175,   696,
       3,   183,    71,    97,    85,   182,   200,   201,    71,   277,
     278,   175,   280,   175,   175,   186,   178,    71,   182,   113,
     998,   182,    71,    71,    73,    74,   175,    71,   183,     3,
      81,   962,    83,   182,   176,   347,   175,   179,   166,   178,
     211,   166,   183,     3,    71,   177,   217,   183,   219,   177,
     177,   717,   177,   194,   195,   916,   229,   230,   231,   232,
     233,   183,   235,   114,   177,   101,     3,   177,    71,   177,
     198,   199,   177,   198,   199,    71,    67,   177,   175,   745,
      71,    84,    78,   183,   177,   182,   176,     4,   181,   177,
     183,   177,   192,   193,   194,   195,   791,    71,   789,   787,
     179,   177,   177,   183,    21,     3,    23,   177,   183,   373,
      27,    28,    29,   206,   207,   175,   657,   192,   193,   194,
     195,   102,   192,   193,   123,   177,   125,   603,   228,   175,
     601,   443,   722,   177,    71,   176,   182,   405,   406,   183,
     192,   193,   704,   177,    76,    77,   177,    84,   416,   417,
     733,   585,   698,   228,    71,    72,   175,   179,   228,    70,
      71,   192,   193,   182,   432,   176,     4,   176,   202,   203,
       8,     9,   185,    71,   177,   346,   228,   382,   446,   177,
     183,   352,   353,   176,   355,   356,   357,   358,   359,   392,
     436,    75,    76,   110,   440,   441,   442,   228,   462,   188,
     189,   372,   373,   177,   407,   925,   926,   181,   176,   183,
     176,   414,   176,   176,   385,   418,   157,   177,   176,   422,
     176,   181,   176,   394,    10,    11,    12,    13,   399,   176,
     204,   205,   403,     4,   231,   232,   233,     8,     9,   180,
     177,   509,   202,   203,   234,   176,   183,   164,   176,   166,
     176,   519,   520,   424,    71,    72,   176,   176,    75,    76,
     177,   176,   176,   697,   181,   170,   183,   850,   176,   206,
     207,     3,   475,   178,   179,   192,   193,   194,   195,   177,
     176,   198,   199,   200,   201,   183,   176,   176,   229,   230,
     231,   232,   233,    64,    65,    66,   201,    68,   176,   176,
      71,    72,   473,   176,   176,   895,   204,   205,   175,   183,
     176,   228,   176,   176,   219,   176,     3,    38,    39,    40,
       7,   176,    93,    94,    95,    96,    97,    98,    80,   100,
     879,   176,   176,   176,   924,    87,   176,   176,  1004,   176,
     923,   176,   176,   607,   176,    97,   117,   118,   175,   166,
     178,   176,   786,   175,    82,   126,   127,   128,   129,   130,
     177,   113,   229,   230,   231,   232,   233,     3,     4,   176,
     275,   276,   143,   176,   176,   176,   688,   176,   234,   177,
     177,   198,   199,   154,    71,    72,   175,   179,   179,   178,
     175,   143,   144,   145,   146,   166,   148,   149,   150,   151,
     152,   229,   230,   231,   232,   233,   177,   229,   230,   231,
     232,   233,   181,   725,   726,   727,   728,   588,   730,   731,
     732,   179,   235,   691,   175,   175,   690,   198,   199,   175,
     174,     4,   178,     4,   605,    71,    72,   177,   179,   175,
     229,   230,   231,   232,   233,   175,   175,     3,   179,   178,
     229,   230,   231,   232,   233,     3,   180,   228,   181,   662,
     179,   175,   175,   666,   175,   729,   669,   670,   175,   175,
     237,   735,   159,   160,   161,   162,   163,   177,   165,   166,
     167,   168,   169,   170,   171,   231,   232,   233,   177,   177,
     177,   177,   177,   664,   665,     4,   179,   668,    71,    72,
     229,   230,   231,   232,   233,   229,   230,   231,   232,   233,
     175,   198,   199,   175,   177,   202,   203,    70,    71,    72,
       4,   208,   175,   159,   160,   161,   162,   163,   175,   165,
     166,   167,   168,   169,   170,   171,   229,   230,   231,   232,
     233,   177,   175,   855,   175,     4,   175,   811,   175,   175,
     180,   175,     6,   175,   175,   175,   175,     4,   175,   175,
     994,   175,   198,   199,   175,   180,   175,  1001,   739,   175,
     180,   175,   208,   175,   175,   175,   175,   175,   749,   891,
     892,   893,   178,   175,  1018,   178,   159,   160,   161,   162,
     163,   180,   165,   166,   167,   168,   169,   170,   171,   229,
     230,   231,   232,   233,   177,     3,   159,   160,   161,   162,
     163,   180,   165,   166,   167,   168,   169,   170,   171,   229,
     230,   231,   232,   233,   177,   198,   199,    71,    72,   176,
     176,   176,   176,     3,   175,   208,   175,   180,     5,   175,
     229,   230,   231,   232,   233,   198,   199,   184,   175,   175,
      70,    70,   175,   175,   172,   208,   175,     6,   178,   927,
     229,   230,   231,   232,   233,    14,    35,   838,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     4,   175,
      35,    30,    31,    32,    33,    34,    37,    36,    79,   175,
      79,   180,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,   175,    53,    54,    55,    56,    57,    58,
      59,    60,    61,   175,    63,   159,   160,   161,   162,   163,
      69,   165,   166,   167,   168,   169,   170,   171,   150,    83,
     147,   902,    89,   177,     4,   109,   175,   180,   180,    88,
     229,   230,   231,   232,   233,   182,     3,   175,   178,  1017,
      99,    73,    74,     4,   198,   199,   105,  1025,   107,   108,
     183,   110,   183,   112,   208,   175,   115,   116,   175,   236,
     119,   120,   180,   122,   175,   124,   178,     4,   983,   175,
     175,   103,   104,   175,   106,   179,   229,   230,   231,   232,
     233,   175,   175,   175,   175,   175,     4,  1002,   175,   175,
     179,     4,   175,     4,   153,    70,   155,   129,   176,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
       4,   229,   230,   231,   232,   233,   109,   175,   175,   175,
     179,   175,   175,   175,   175,   175,   185,   186,     3,   175,
     178,   176,   235,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   183,   175,     4,   175,   177,     3,   184,   186,   178,
     178,   176,   194,   195,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   186,   193,   176,   195,   176,   197,   192,   199,
     194,   201,   196,   203,   198,   205,   200,   207,   202,   209,
     204,   178,   206,   176,   208,   176,   176,   176,   176,     5,
     176,   176,   176,   176,    70,   175,   175,   175,   175,     4,
     175,   177,   175,   143,     4,    79,   175,    79,   175,   183,
     175,    86,   175,   181,   175,    92,   175,     4,    91,     4,
     178,     4,   178,   178,   178,   175,   175,   180,   176,   175,
     175,   175,     4,     4,   175,   178,   175,     5,     3,   176,
     176,   176,   176,    38,   176,    90,   176,   176,   175,     4,
     178,   175,   178,   175,    81,   178,   176,   176,   176,   176,
     176,   176,     7,   176,   176,   176,   176,   175,    71,   178,
     176,   175,     4,   114,     4,   182,   178,   175,    87,   176,
       4,   175,   175,     4,    86,     4,     4,   176,     5,   236,
      38,   178,    89,   176,     4,   175,   178,   178,    80,   178,
     178,     5,  1024,   173,    66,   882,   381,    60,   504,   178,
     178,   175,   175,   175,   202,   445,   854,   178,   223,   985,
     933,   877,   762,   915,   162,   955,   674,   928,   993,   735,
     196,   386,   497,   704,   473,    -1,    -1,    -1,   509,    -1,
      -1,    -1,   284
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   239,   240,   241,   242,   176,   176,     0,
     240,   101,   249,   183,   183,   176,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    30,    31,
      32,    33,    34,    36,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    63,    69,    88,    99,   105,   107,
     108,   110,   112,   115,   116,   119,   120,   122,   124,   153,
     155,   179,   185,   186,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   243,   244,   251,   253,   254,   255,   256,   258,
     259,   267,   270,   271,   273,   274,   281,   283,   284,   286,
     288,   290,   294,   295,   296,   299,   301,   308,   315,   318,
     324,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   340,   341,   342,   343,   347,   348,   350,   351,   352,
     353,   354,   355,   356,   357,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   175,   102,   250,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   273,
     176,   176,   176,   179,   186,   255,   257,   275,   176,   185,
       4,   177,   192,   193,   228,   398,   408,   177,   251,   234,
     176,   285,     3,   175,   183,   194,   195,   384,   397,    10,
      11,    12,    13,     4,    85,   409,    71,    72,    75,    76,
     166,   177,   198,   199,   316,   386,   387,     3,    71,    84,
     177,   183,   206,   207,   326,   395,   396,   326,     3,    71,
     177,   183,   204,   205,   320,   393,   394,    71,   183,   369,
      64,    71,   378,   386,   386,   159,   160,   161,   162,   163,
     165,   167,   168,   169,   170,   171,   177,   208,   386,   406,
     407,   177,   200,   201,   389,   390,   389,    52,   177,   416,
     417,   183,   386,   386,    82,   386,   177,   386,   416,   389,
     389,    71,   183,   298,   370,   416,   416,   416,   385,   398,
     177,   297,   398,   386,   407,   407,   386,   407,   407,     4,
       8,     9,    71,   410,   386,   407,   298,   407,     5,    71,
     309,   291,   407,   389,   254,   385,   234,   179,   386,   177,
       8,   398,   179,   157,   180,   229,   230,   231,   232,   233,
     397,   398,   403,   404,   405,    21,    23,    27,    28,    29,
     110,   164,   177,   181,   386,   389,   397,   398,   412,   413,
     414,   289,   181,   252,   250,   179,   235,   175,   175,   175,
     174,   388,   177,     4,   179,   175,   175,   303,     3,   179,
     327,   325,     3,   179,   319,   175,   175,   175,   175,   175,
     389,   389,   386,   386,   177,   386,   177,   177,   177,   406,
     177,   407,   237,     4,   179,   175,   175,   177,     4,   175,
     175,   175,   175,   175,     4,   175,   175,   175,     6,   272,
     175,   175,   175,   175,     4,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   252,   292,   175,   175,   180,
     398,   385,   175,   268,   178,   178,   398,   398,   398,   398,
     398,   398,   398,   175,   178,   176,     4,   263,   264,   265,
     266,   387,   390,   397,   398,   176,   176,   176,   253,   251,
     398,   384,     3,     3,   177,   349,     3,   175,   180,   407,
     175,   398,    76,    77,   317,     5,   184,   175,   398,   175,
     252,   175,   398,   252,   386,   386,   407,    70,    70,   175,
     175,    70,   311,   407,   386,   386,     3,     7,   177,   202,
     203,   391,   392,   407,   172,   178,   407,   175,   398,    65,
      71,   379,    35,     4,   175,    35,    37,    71,    73,    74,
     380,   383,   386,    79,   175,    79,   416,    71,   177,   304,
     175,   416,   416,   416,   385,   175,   297,   410,   386,    67,
      71,   415,    83,   150,   147,    89,    71,   177,   305,     4,
     378,   293,   177,   391,   391,   287,   121,   235,   282,   180,
      71,   397,   256,   400,   401,   402,   180,   180,   403,   407,
     175,   175,   182,   175,   182,   175,   182,   175,   182,   181,
     395,   181,   393,   181,   391,   182,   180,     3,   175,   388,
     178,     4,   180,   303,   183,   180,     4,     8,     9,    64,
      65,    66,    68,    93,    94,    95,    96,    97,    98,   100,
     117,   118,   126,   127,   128,   129,   130,   143,   154,   328,
     386,   408,   411,   183,   180,   321,   313,   175,   175,   386,
     386,   236,   175,   178,   175,   175,   175,     3,   179,   175,
     175,   175,     4,   180,   175,   175,     4,   175,   179,   175,
       4,   175,     4,   175,   379,   175,   175,   339,   175,    70,
     175,   176,   344,   175,     4,   175,   175,   109,   175,   252,
     235,   398,   276,   176,   175,   178,     4,   390,   387,   398,
     397,   260,   396,   261,   394,   262,   392,   175,     3,   178,
     184,   178,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   178,   175,   175,   378,     5,   175,
     311,   407,   398,   398,   407,   175,   398,   407,   407,   177,
     175,   370,     4,   175,   143,     4,    71,    78,   382,    79,
     175,    71,   177,   306,   183,    79,   385,   175,   397,   386,
     175,    86,   175,    71,   177,   307,    71,   310,   311,   252,
     391,   181,   188,   189,   399,   400,   269,   175,   182,   175,
     182,   175,   182,   379,   175,   395,    92,    91,   385,   385,
     385,   385,   397,   385,   385,   385,   393,    73,    74,   103,
     104,   106,   129,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   322,   381,   383,   397,    68,    97,
     113,   314,   175,   247,   398,   178,   175,   178,   175,   178,
     379,   180,   178,   178,   398,     4,   175,   371,   175,     4,
     176,   180,     4,   316,   175,   175,     4,    71,   312,   175,
      80,    87,    97,   113,   143,   144,   145,   146,   148,   149,
     150,   151,   152,   345,     4,     4,   371,   175,   175,   123,
     125,   277,   278,   252,   396,   394,   392,   178,     5,   176,
     397,   176,   176,   176,   176,   176,   176,   176,     3,    38,
     398,   178,   230,   175,    38,    39,    40,   372,   373,    90,
     374,     4,   175,   393,   178,   175,   307,   385,   178,    81,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     178,   175,   374,   312,     7,   410,   176,   182,   277,   175,
      71,   385,   385,   385,   142,   183,   323,   395,     4,   114,
     178,   175,   178,   398,     4,   175,   176,    87,   376,   175,
       4,   317,   371,   175,    86,   377,     4,     4,    81,    83,
     114,   346,   393,   395,   349,   349,   386,   369,     4,     4,
     376,   313,   176,   280,     5,   236,    38,   175,   245,   178,
     372,    89,   375,   176,   358,     4,   178,   178,   374,    80,
     178,   300,   279,   251,   175,   323,   178,   173,   178,   377,
     252,   359,   178,   376,   252,   251,   379,   175,   302,   175,
     386,   252,     5,    71,   248,   175,   246,   247,   386,   178
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
		      long rflags = (yyvsp[(8) - (10)].i);
		      long flmt = (long)(yyvsp[(9) - (10)].i);
		      long flt = (long)(yyvsp[(10) - (10)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      if (flmt == -1) flmt = ROOM;
		      if (flt == -1) flt = 0;

		      add_opvars(splev, "iiiiiiiiio",
				 VA_PASS10(flt, flmt, rflags, ERR, ERR,
					  (yyvsp[(5) - (10)].crd).x, (yyvsp[(5) - (10)].crd).y, (yyvsp[(7) - (10)].sze).width, (yyvsp[(7) - (10)].sze).height,
					  SPO_SUBROOM));
		      break_stmt_start();
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1295 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1302 "lev_comp.y"
    {
		      long rflags = (yyvsp[(10) - (12)].i);
		      long flmt = (long)(yyvsp[(11) - (12)].i);
		      long flt = (long)(yyvsp[(12) - (12)].i);

		      if (rflags == -1) rflags = (1 << 0);

		      add_opvars(splev, "iiiiiiiiio",
				 VA_PASS10(flt, flmt, rflags,
					  (yyvsp[(7) - (12)].crd).x, (yyvsp[(7) - (12)].crd).y, (yyvsp[(5) - (12)].crd).x, (yyvsp[(5) - (12)].crd).y,
					  (yyvsp[(9) - (12)].sze).width, (yyvsp[(9) - (12)].sze).height, SPO_ROOM));
		      break_stmt_start();
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1316 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1323 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1327 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1333 "lev_comp.y"
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
#line 1343 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1349 "lev_comp.y"
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
#line 1358 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1364 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1369 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1375 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1380 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1386 "lev_comp.y"
    {
			/* ERR means random here */
			if ((yyvsp[(7) - (10)].i) == ERR && (yyvsp[(9) - (10)].i) != ERR) {
			    lc_error("If the door wall is random, so must be its pos!");
			} else {
			    add_opvars(splev, "iiiio",
				       VA_PASS5((long)(yyvsp[(9) - (10)].i), (long)(yyvsp[(5) - (10)].i), (long)(yyvsp[(3) - (10)].i),
						(long)(yyvsp[(7) - (10)].i), SPO_ROOM_DOOR));
			}
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1397 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1411 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1415 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1425 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1432 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1440 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1445 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1450 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1458 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1465 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1473 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1489 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1493 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1499 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1507 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1513 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1521 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1529 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1534 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1540 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1546 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1552 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1558 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1563 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1568 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1573 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1578 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1583 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1588 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1593 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1598 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1603 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1608 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1614 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1621 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1630 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1634 "lev_comp.y"
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

  case 241:

/* Line 1455 of yacc.c  */
#line 1647 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1655 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1665 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1673 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1682 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1689 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1697 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1703 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1708 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1713 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1718 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1723 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1728 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1733 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1738 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1743 "lev_comp.y"
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

  case 257:

/* Line 1455 of yacc.c  */
#line 1754 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1760 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1765 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1770 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1775 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1780 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1785 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1790 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1795 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1800 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1805 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1810 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1817 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1823 "lev_comp.y"
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

  case 271:

/* Line 1455 of yacc.c  */
#line 1852 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1857 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1865 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1871 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1877 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1884 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1891 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1901 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1911 "lev_comp.y"
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

  case 280:

/* Line 1455 of yacc.c  */
#line 1926 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1936 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1940 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1946 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1952 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1958 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1964 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1970 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1977 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1985 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1991 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1996 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 2001 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 2006 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2011 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2017 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2023 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2028 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2033 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2038 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_CATEGORY));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2043 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2048 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2058 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2064 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2070 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2075 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2082 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2089 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2095 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2101 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2107 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2111 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2117 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2123 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2133 "lev_comp.y"
    {
		      long irr;
		      long rt = (yyvsp[(7) - (10)].i);
		      long rflags = (yyvsp[(8) - (10)].i);
		      long flmt = (long)(yyvsp[(9) - (10)].i);
		      long flt = (long)(yyvsp[(10) - (10)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      if (flmt == -1) flmt = ROOM;
		      if (flt == -1) flt = 0;

		      if (!(rflags & 1)) rt += MAXRTYPE+1;
		      irr = ((rflags & 2) != 0);
		      add_opvars(splev, "iiiiio",
				 VA_PASS6((long)(yyvsp[(5) - (10)].i), rt, rflags, flmt, flt, SPO_REGION));
		      (yyval.i) = (irr || (rflags & 1) || rt != OROOM);
		      break_stmt_start();
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2152 "lev_comp.y"
    {
		      break_stmt_end(splev);
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      /*if ( $<i>9 ||  $<i>10 ||  $<i>11 ) {
			  	add_opcode(splev, SPO_ENDROOM, NULL);
		       } else if ( $<i>12 )
			  	lc_error("Cannot use lev statements in non-permanent REGION");
		      */
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2164 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2168 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2174 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2181 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2187 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2193 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2199 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2205 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2209 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2214 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2221 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2227 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2234 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2238 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2245 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2256 "lev_comp.y"
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

  case 338:

/* Line 1455 of yacc.c  */
#line 2269 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2273 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2279 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2283 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2290 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2294 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2298 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2304 "lev_comp.y"
    {
			(yyval.i) = ROOM;
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2308 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2314 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2320 "lev_comp.y"
    {
			(yyval.i) = 0;
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2324 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2330 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2346 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2354 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2364 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2373 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2378 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2385 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2396 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2402 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2406 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2410 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2417 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2427 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2434 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2438 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2444 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2448 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2456 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2460 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2467 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2477 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2490 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2494 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2501 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2511 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2520 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2531 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2535 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2542 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2552 "lev_comp.y"
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

  case 390:

/* Line 1455 of yacc.c  */
#line 2563 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2572 "lev_comp.y"
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

  case 392:

/* Line 1455 of yacc.c  */
#line 2582 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2588 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2592 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2599 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2609 "lev_comp.y"
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

  case 397:

/* Line 1455 of yacc.c  */
#line 2620 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2629 "lev_comp.y"
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

  case 399:

/* Line 1455 of yacc.c  */
#line 2639 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2643 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2649 "lev_comp.y"
    { ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2651 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2657 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2661 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2665 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2669 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2677 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2686 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2690 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2694 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2698 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2702 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2705 "lev_comp.y"
    { ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2709 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2716 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2725 "lev_comp.y"
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

  case 421:

/* Line 1455 of yacc.c  */
#line 2766 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2770 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2777 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2784 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2794 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2798 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2806 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2810 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2814 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2818 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2822 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2827 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2831 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2835 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2840 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2845 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2850 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2854 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2859 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2864 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2868 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2872 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2877 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2881 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2888 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2894 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2898 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2904 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2920 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2924 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2928 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2932 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2954 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2958 "lev_comp.y"
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

  case 468:

/* Line 1455 of yacc.c  */
#line 2984 "lev_comp.y"
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
#line 6716 "lev_comp.tab.c"
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
#line 3012 "lev_comp.y"


/*lev_comp.y*/

