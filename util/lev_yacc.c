
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
     FLOOR_MAIN_TYPE_ID = 346,
     ELEMENTAL_ENCHANTMENT_TYPE = 347,
     EXCEPTIONALITY_TYPE = 348,
     EXCEPTIONALITY_ID = 349,
     ELEMENTAL_ENCHANTMENT_ID = 350,
     ENCHANTMENT_ID = 351,
     CHARGES_ID = 352,
     SPECIAL_QUALITY_ID = 353,
     SPEFLAGS_ID = 354,
     SUBROOM_ID = 355,
     NAME_ID = 356,
     FLAGS_ID = 357,
     FLAG_TYPE = 358,
     MON_ATTITUDE = 359,
     MON_ALERTNESS = 360,
     SUBTYPE_ID = 361,
     MON_APPEARANCE = 362,
     ROOMDOOR_ID = 363,
     IF_ID = 364,
     ELSE_ID = 365,
     TERRAIN_ID = 366,
     HORIZ_OR_VERT = 367,
     REPLACE_TERRAIN_ID = 368,
     LOCATION_SUBTYPE_ID = 369,
     DOOR_SUBTYPE = 370,
     EXIT_ID = 371,
     SHUFFLE_ID = 372,
     QUANTITY_ID = 373,
     BURIED_ID = 374,
     LOOP_ID = 375,
     FOR_ID = 376,
     TO_ID = 377,
     SWITCH_ID = 378,
     CASE_ID = 379,
     BREAK_ID = 380,
     DEFAULT_ID = 381,
     ERODED_ID = 382,
     TRAPPED_STATE = 383,
     RECHARGED_ID = 384,
     INVIS_ID = 385,
     GREASED_ID = 386,
     FEMALE_ID = 387,
     WAITFORU_ID = 388,
     CANCELLED_ID = 389,
     REVIVED_ID = 390,
     AVENGE_ID = 391,
     FLEEING_ID = 392,
     BLINDED_ID = 393,
     PARALYZED_ID = 394,
     STUNNED_ID = 395,
     CONFUSED_ID = 396,
     SEENTRAPS_ID = 397,
     ALL_ID = 398,
     MONTYPE_ID = 399,
     OBJTYPE_ID = 400,
     TERTYPE_ID = 401,
     TERTYPE2_ID = 402,
     LEVER_EFFECT_TYPE = 403,
     SWITCHABLE_ID = 404,
     CONTINUOUSLY_USABLE_ID = 405,
     TARGET_ID = 406,
     TRAPTYPE_ID = 407,
     EFFECT_FLAG_ID = 408,
     GRAVE_ID = 409,
     ERODEPROOF_ID = 410,
     FUNCTION_ID = 411,
     MSG_OUTPUT_TYPE = 412,
     COMPARE_TYPE = 413,
     UNKNOWN_TYPE = 414,
     rect_ID = 415,
     fillrect_ID = 416,
     line_ID = 417,
     randline_ID = 418,
     grow_ID = 419,
     selection_ID = 420,
     flood_ID = 421,
     rndcoord_ID = 422,
     circle_ID = 423,
     ellipse_ID = 424,
     filter_ID = 425,
     complement_ID = 426,
     gradient_ID = 427,
     GRADIENT_TYPE = 428,
     LIMITED = 429,
     HUMIDITY_TYPE = 430,
     STRING = 431,
     MAP_ID = 432,
     NQSTRING = 433,
     VARSTRING = 434,
     CFUNC = 435,
     CFUNC_INT = 436,
     CFUNC_STR = 437,
     CFUNC_COORD = 438,
     CFUNC_REGION = 439,
     VARSTRING_INT = 440,
     VARSTRING_INT_ARRAY = 441,
     VARSTRING_STRING = 442,
     VARSTRING_STRING_ARRAY = 443,
     VARSTRING_VAR = 444,
     VARSTRING_VAR_ARRAY = 445,
     VARSTRING_COORD = 446,
     VARSTRING_COORD_ARRAY = 447,
     VARSTRING_REGION = 448,
     VARSTRING_REGION_ARRAY = 449,
     VARSTRING_MAPCHAR = 450,
     VARSTRING_MAPCHAR_ARRAY = 451,
     VARSTRING_MONST = 452,
     VARSTRING_MONST_ARRAY = 453,
     VARSTRING_OBJ = 454,
     VARSTRING_OBJ_ARRAY = 455,
     VARSTRING_SEL = 456,
     VARSTRING_SEL_ARRAY = 457,
     METHOD_INT = 458,
     METHOD_INT_ARRAY = 459,
     METHOD_STRING = 460,
     METHOD_STRING_ARRAY = 461,
     METHOD_VAR = 462,
     METHOD_VAR_ARRAY = 463,
     METHOD_COORD = 464,
     METHOD_COORD_ARRAY = 465,
     METHOD_REGION = 466,
     METHOD_REGION_ARRAY = 467,
     METHOD_MAPCHAR = 468,
     METHOD_MAPCHAR_ARRAY = 469,
     METHOD_MONST = 470,
     METHOD_MONST_ARRAY = 471,
     METHOD_OBJ = 472,
     METHOD_OBJ_ARRAY = 473,
     METHOD_SEL = 474,
     METHOD_SEL_ARRAY = 475,
     DICE = 476
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
#line 519 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 531 "lev_comp.tab.c"

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
#define YYLAST   1176

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  239
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  180
/* YYNRULES -- Number of rules.  */
#define YYNRULES  468
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1030

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   476

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   234,   238,     2,
     178,   179,   232,   230,   176,   231,   236,   233,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   177,     2,
       2,   235,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   180,     2,   181,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   182,   237,   183,     2,     2,     2,     2,
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
     175,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229
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
     240,     0,    -1,    -1,   241,    -1,   242,    -1,   242,   241,
      -1,   243,   250,   252,    -1,    16,   177,   184,    -1,    15,
     177,   184,   176,     3,    -1,    17,   177,    11,   176,   350,
      -1,    17,   177,    10,   176,     3,    -1,    17,   177,    13,
      -1,    17,   177,    12,   176,     3,   176,     3,   176,     5,
     176,     5,   176,   380,   176,   249,   248,    -1,    18,   177,
     410,    -1,    -1,   176,   174,    -1,    -1,   176,   387,    -1,
      -1,   176,     3,    -1,     5,    -1,    72,    -1,    -1,   102,
     177,   251,    -1,   103,   176,   251,    -1,   103,    -1,    -1,
     254,   252,    -1,   182,   252,   183,    -1,   295,    -1,   244,
      -1,   245,    -1,   364,    -1,   363,    -1,   361,    -1,   362,
      -1,   365,    -1,   366,    -1,   339,    -1,   297,    -1,   260,
      -1,   259,    -1,   353,    -1,   309,    -1,   331,    -1,   368,
      -1,   369,    -1,   341,    -1,   342,    -1,   343,    -1,   367,
      -1,   275,    -1,   285,    -1,   287,    -1,   291,    -1,   289,
      -1,   272,    -1,   282,    -1,   268,    -1,   271,    -1,   334,
      -1,   344,    -1,   316,    -1,   332,    -1,   319,    -1,   325,
      -1,   354,    -1,   349,    -1,   337,    -1,   296,    -1,   355,
      -1,   356,    -1,   357,    -1,   358,    -1,   302,    -1,   300,
      -1,   348,    -1,   352,    -1,   351,    -1,   335,    -1,   336,
      -1,   338,    -1,   330,    -1,   333,    -1,   194,    -1,   196,
      -1,   198,    -1,   200,    -1,   202,    -1,   204,    -1,   206,
      -1,   208,    -1,   210,    -1,   193,    -1,   195,    -1,   197,
      -1,   199,    -1,   201,    -1,   203,    -1,   205,    -1,   207,
      -1,   209,    -1,   255,    -1,   256,    -1,   187,    -1,   187,
      -1,   256,    -1,   117,   177,   255,    -1,   257,   235,   399,
      -1,   257,   235,   165,   177,   408,    -1,   257,   235,   398,
      -1,   257,   235,   415,   177,   392,    -1,   257,   235,   414,
     177,   394,    -1,   257,   235,   413,   177,   396,    -1,   257,
     235,   387,    -1,   257,   235,   390,    -1,   257,   235,   182,
     266,   183,    -1,   257,   235,   182,   265,   183,    -1,   257,
     235,   182,   264,   183,    -1,   257,   235,   415,   177,   182,
     263,   183,    -1,   257,   235,   414,   177,   182,   262,   183,
      -1,   257,   235,   413,   177,   182,   261,   183,    -1,   257,
     235,   182,   267,   183,    -1,   397,    -1,   261,   176,   397,
      -1,   395,    -1,   262,   176,   395,    -1,   393,    -1,   263,
     176,   393,    -1,   391,    -1,   264,   176,   391,    -1,   388,
      -1,   265,   176,   388,    -1,   399,    -1,   266,   176,   399,
      -1,   398,    -1,   267,   176,   398,    -1,    -1,    -1,   156,
     186,   178,   269,   403,   179,   270,   253,    -1,   186,   178,
     406,   179,    -1,   116,    -1,    -1,     6,    -1,     6,    -1,
     180,   399,   158,   399,   181,    -1,   180,   399,   181,    -1,
      -1,    -1,   123,   276,   180,   386,   181,   277,   182,   278,
     183,    -1,    -1,   279,   278,    -1,    -1,   124,   411,   177,
     280,   252,    -1,    -1,   126,   177,   281,   252,    -1,   125,
      -1,   236,   236,    -1,   122,    -1,   121,   258,   235,   399,
     283,   399,    -1,    -1,   284,   286,   253,    -1,    -1,   120,
     180,   386,   181,   288,   253,    -1,    -1,   274,   177,   290,
     254,    -1,    -1,   109,   274,   292,   293,    -1,   253,    -1,
      -1,   253,   294,   110,   253,    -1,    14,   177,   398,    -1,
      63,    -1,    63,   177,   411,    -1,    63,   177,    72,    -1,
      54,   177,   298,   176,   298,    -1,    54,   177,   298,   176,
     411,    -1,   178,     4,   176,    71,   176,   313,   179,    -1,
     371,   273,   176,   380,    -1,    -1,   100,   177,   299,   176,
     306,   176,   308,   372,   375,   377,   301,   253,    -1,    -1,
      48,   177,   299,   176,   305,   176,   307,   176,   308,   372,
     375,   377,   303,   253,    -1,    -1,   176,     5,    -1,   178,
       4,   176,     4,   179,    -1,    72,    -1,   178,     4,   176,
       4,   179,    -1,    72,    -1,   178,   317,   176,   318,   179,
      -1,    72,    -1,   178,     4,   176,     4,   179,    -1,    72,
      -1,   108,   177,   310,   176,   379,   176,   311,   176,   313,
     314,    -1,    25,   177,   379,   176,   408,   314,    -1,     5,
      -1,    72,    -1,   312,    -1,    72,    -1,    71,    -1,    71,
     237,   312,    -1,     4,    -1,    72,    -1,    -1,   314,   176,
     315,    -1,   114,   177,   115,    -1,    69,   177,    68,    -1,
      98,   177,     4,    -1,    20,    -1,    19,   177,   317,   176,
     318,   304,   185,    -1,    19,   177,   387,   304,   185,    -1,
      76,    -1,    77,    -1,    78,    -1,    77,    -1,    23,   177,
     321,    -1,    -1,    23,   177,   321,   320,   253,    -1,   394,
     176,   387,   322,    -1,    -1,   322,   176,   323,    -1,   398,
      -1,   104,    -1,   105,    -1,   382,    -1,   107,   398,    -1,
     132,    -1,   130,    -1,   134,    -1,   135,    -1,   136,    -1,
     137,   177,   386,    -1,   138,   177,   386,    -1,   139,   177,
     386,    -1,   140,    -1,   141,    -1,   142,   177,   324,    -1,
     133,    -1,   184,    -1,   143,    -1,   184,   237,   324,    -1,
      21,   177,   327,    -1,    -1,    22,   177,   327,   326,   253,
      -1,   396,   328,    -1,    -1,   328,   176,   329,    -1,    66,
      -1,   144,   177,   394,    -1,   412,    -1,   101,   177,   398,
      -1,   118,   177,   386,    -1,   119,    -1,    65,    -1,   127,
     177,   386,    -1,   155,    -1,    64,    -1,   128,    -1,   129,
     177,   386,    -1,   130,    -1,   131,    -1,   387,    -1,    95,
     177,    92,    -1,    94,   177,    93,    -1,    96,   177,   386,
      -1,    97,   177,   386,    -1,    98,   177,   386,    -1,    99,
     177,   386,    -1,    69,   177,    68,    -1,    24,   177,   370,
     176,   387,    -1,    26,   177,   387,   176,    71,   176,   379,
      -1,    30,   177,   387,   176,    71,    -1,    30,   177,   387,
     176,    71,   176,     5,   248,    -1,    31,    -1,    31,   177,
     408,    -1,    44,   177,   387,   176,    80,    -1,    45,   177,
     387,   176,    80,    -1,    45,   177,   417,   176,   417,   176,
      80,    -1,    49,   177,   417,   176,   417,   176,   184,    -1,
      50,   177,   417,   176,   417,   340,    -1,    51,   177,   417,
     176,   417,    -1,    -1,   176,    80,    -1,    57,   177,   408,
     176,    84,    -1,    58,   177,   408,    -1,    59,   177,   387,
     176,   151,   177,   387,   176,    82,   176,    81,    -1,    70,
     177,   387,   176,   148,   345,    -1,    -1,   345,   176,   346,
      -1,    81,    -1,   144,   177,   394,    -1,   145,   177,   396,
      -1,   146,   177,   350,    -1,   147,   177,   350,    -1,   149,
      -1,   150,    -1,   151,   177,   387,    -1,   152,   177,   370,
      -1,   114,   177,   347,    -1,    88,   177,   378,    -1,   153,
     177,     4,    -1,    98,   177,     4,    -1,     4,    -1,    82,
      -1,   115,    -1,    84,    -1,    61,   177,   408,    -1,    60,
     177,   408,    -1,     3,    -1,   178,     3,   176,   380,   179,
      -1,   113,   177,   390,   176,   392,   176,   392,   176,     7,
      -1,   111,   177,   408,   176,   392,    -1,    46,   177,   390,
      -1,    47,   177,   390,    -1,    36,   177,   184,   176,    37,
      -1,    36,   177,   184,   176,    37,   176,   144,   177,   394,
      -1,    33,   177,   390,   176,    35,    -1,    34,   177,   417,
     176,    35,    -1,    -1,    32,   177,   390,   176,   380,   176,
     371,   372,   375,   377,   359,   360,    -1,    -1,   253,    -1,
      41,   177,   387,   176,   381,   176,   383,    -1,    42,   177,
     387,    -1,    89,   177,   408,   176,    90,   176,    87,    -1,
     106,   177,   408,   176,     4,    -1,    43,   177,    83,   176,
     387,    -1,   154,   177,   387,   176,   398,    -1,   154,   177,
     387,   176,    72,    -1,   154,   177,   387,    -1,    55,   177,
     399,   176,   387,    -1,    56,   177,   387,   176,   416,   176,
     398,    -1,    53,   177,   386,   176,   386,   176,   386,   176,
     386,    -1,    53,    -1,   184,    -1,    72,    -1,   184,    -1,
      72,    -1,    -1,   176,   373,    -1,   374,    -1,   374,   176,
     373,    -1,    38,    -1,    39,    -1,    40,    -1,    -1,    91,
     177,   376,    -1,    90,    -1,    -1,    88,   177,   378,    -1,
      87,    -1,    64,    -1,    72,    -1,    65,    -1,    72,    -1,
      75,    -1,   384,    -1,    72,    -1,    75,    -1,   384,    -1,
      74,   177,    72,    -1,    79,    -1,    72,    -1,    74,   180,
       4,   181,    -1,   184,    -1,   195,    -1,   196,   180,   399,
     181,    -1,   399,    -1,   388,    -1,   167,   178,   408,   179,
      -1,   199,    -1,   200,   180,   399,   181,    -1,   178,     4,
     176,     4,   179,    -1,    72,    -1,    73,   389,   181,    -1,
     175,    -1,   175,   176,   389,    -1,   391,    -1,   201,    -1,
     202,   180,   399,   181,    -1,   178,     4,   176,     4,   176,
       4,   176,     4,   179,    -1,   393,    -1,   203,    -1,   204,
     180,   399,   181,    -1,     3,    -1,   178,     3,   176,   380,
     179,    -1,   395,    -1,   205,    -1,   206,   180,   399,   181,
      -1,   184,    -1,     3,    -1,   178,     3,   176,   184,   179,
      -1,    72,    -1,   397,    -1,   207,    -1,   208,   180,   399,
     181,    -1,   184,    -1,     3,    -1,   178,     3,   176,   184,
     179,    -1,    72,    -1,    85,    -1,   385,    -1,   398,   236,
     385,    -1,     4,    -1,   409,    -1,   178,     8,   179,    -1,
     193,    -1,   194,   180,   399,   181,    -1,   399,   230,   399,
      -1,   399,   231,   399,    -1,   399,   232,   399,    -1,   399,
     233,   399,    -1,   399,   234,   399,    -1,   178,   399,   179,
      -1,   189,    -1,   190,    -1,   257,   177,   400,    -1,   401,
      -1,   402,   176,   401,    -1,    -1,   402,    -1,   399,    -1,
     398,    -1,   404,    -1,   405,   176,   404,    -1,    -1,   405,
      -1,   387,    -1,   160,   390,    -1,   161,   390,    -1,   162,
     387,   176,   387,    -1,   163,   387,   176,   387,   176,   399,
      -1,   164,   178,   408,   179,    -1,   164,   178,   312,   176,
     408,   179,    -1,   170,   178,     7,   176,   408,   179,    -1,
     170,   178,   408,   176,   408,   179,    -1,   170,   178,   392,
     176,   408,   179,    -1,   166,   387,    -1,   168,   178,   387,
     176,   399,   179,    -1,   168,   178,   387,   176,   399,   176,
      38,   179,    -1,   169,   178,   387,   176,   399,   176,   399,
     179,    -1,   169,   178,   387,   176,   399,   176,   399,   176,
      38,   179,    -1,   172,   178,   173,   176,   178,   399,   231,
     399,   246,   179,   176,   387,   247,   179,    -1,   171,   407,
      -1,   209,    -1,   178,   408,   179,    -1,   407,    -1,   407,
     238,   408,    -1,   229,    -1,    86,    -1,     4,    -1,     8,
      -1,     9,    -1,     4,    -1,     8,    -1,     9,    -1,     4,
      -1,   409,    -1,    27,    -1,    21,    -1,    28,    -1,    23,
      -1,    29,    -1,   111,    -1,    67,    -1,    72,    -1,   418,
      -1,    52,   178,     4,   176,     4,   176,     4,   176,     4,
     179,    -1,   178,     4,   176,     4,   176,     4,   176,     4,
     179,    -1
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
  "DOOR_STATE", "LIGHT_STATE", "CURSE_TYPE", "ENGRAVING_TYPE", "KEY_TYPE",
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
     425,   426,   427,   428,   429,   430,    44,    58,    40,    41,
      91,    93,   123,   125,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
      43,    45,    42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   239,   240,   240,   241,   241,   242,   243,   243,   244,
     244,   244,   244,   245,   246,   246,   247,   247,   248,   248,
     249,   249,   250,   250,   251,   251,   252,   252,   253,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   256,   256,   256,   256,   256,   256,   256,
     256,   256,   257,   257,   257,   258,   258,   259,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   261,   261,   262,   262,   263,   263,   264,
     264,   265,   265,   266,   266,   267,   267,   269,   270,   268,
     271,   272,   273,   273,   274,   274,   274,   276,   277,   275,
     278,   278,   280,   279,   281,   279,   282,   283,   283,   284,
     286,   285,   288,   287,   290,   289,   292,   291,   293,   294,
     293,   295,   296,   296,   296,   297,   297,   298,   299,   301,
     300,   303,   302,   304,   304,   305,   305,   306,   306,   307,
     307,   308,   308,   309,   309,   310,   310,   311,   311,   312,
     312,   313,   313,   314,   314,   315,   315,   315,   316,   316,
     316,   317,   317,   318,   318,   319,   320,   319,   321,   322,
     322,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   324,   324,
     324,   325,   326,   325,   327,   328,   328,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,   329,   329,   330,
     331,   332,   332,   333,   333,   334,   335,   336,   337,   338,
     339,   340,   340,   341,   342,   343,   344,   345,   345,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   347,   347,   347,   347,   348,   349,   350,   350,
     351,   352,   353,   354,   355,   355,   356,   357,   359,   358,
     360,   360,   361,   362,   363,   364,   365,   366,   366,   366,
     367,   368,   369,   369,   370,   370,   371,   371,   372,   372,
     373,   373,   374,   374,   374,   375,   375,   376,   377,   377,
     378,   379,   379,   380,   380,   381,   381,   381,   382,   382,
     382,   383,   383,   384,   385,   385,   385,   386,   387,   387,
     387,   387,   388,   388,   388,   389,   389,   390,   390,   390,
     391,   392,   392,   392,   393,   393,   394,   394,   394,   395,
     395,   395,   395,   396,   396,   396,   397,   397,   397,   397,
     397,   398,   398,   399,   399,   399,   399,   399,   399,   399,
     399,   399,   399,   399,   400,   400,   401,   402,   402,   403,
     403,   404,   404,   405,   405,   406,   406,   407,   407,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   407,   407,   407,   408,   408,   409,   410,
     410,   411,   411,   411,   412,   412,   412,   412,   413,   413,
     414,   414,   415,   415,   416,   416,   417,   417,   418
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
#define YYPACT_NINF -878
static const yytype_int16 yypact[] =
{
     210,  -104,   -90,    97,  -878,   210,    -1,   -57,   -53,  -878,
    -878,    -4,   740,   -40,  -878,    75,  -878,     5,    25,    39,
      57,  -878,    59,    62,   113,   116,   136,   138,   158,   179,
     203,   207,   221,   233,   234,   236,   237,   243,   245,   246,
     247,   248,   249,   251,   253,   254,   261,   262,   264,   265,
     266,   268,   269,   270,   272,   274,   286,   287,   291,   292,
      27,   293,   294,  -878,   297,    33,   875,  -878,  -878,   301,
      79,   125,   132,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,   740,  -878,  -878,   -24,  -878,
    -878,  -878,  -878,  -878,   302,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,   324,   174,  -878,  -106,   354,    48,
     400,   192,   192,    65,   -46,    12,   -13,   -13,   796,   -84,
     -84,   -27,   217,   -13,   -13,   392,   -13,   -15,   -84,   -84,
     -41,   -27,   -27,   -27,   125,   191,   125,   -13,   796,   796,
     -13,   796,   796,    77,   -13,   796,   -41,   796,    67,  -878,
     796,   -84,   822,   125,  -878,  -878,   250,   304,   -13,   309,
    -878,    16,  -878,   313,  -878,   271,  -878,    88,  -878,   316,
    -878,   314,  -878,    75,  -878,  -878,   315,  -878,   244,   306,
     321,   323,  -878,  -878,  -878,  -878,  -878,   331,  -878,  -878,
     329,   504,  -878,   333,   338,   343,  -878,  -878,  -878,  -878,
     517,  -878,  -878,   341,  -878,  -878,  -878,  -878,  -878,  -878,
     519,  -878,  -878,   351,   350,   361,  -878,  -878,  -878,   362,
    -878,  -878,   363,   365,   366,   -84,   -84,   -13,   -13,   368,
     -13,   374,   383,   386,   796,   388,   536,  -878,  -878,   305,
    -878,   558,  -878,   390,   395,  -878,   396,   401,   564,   398,
    -878,   404,   406,  -878,   407,   408,   581,   413,   416,  -878,
    -878,  -878,  -878,   418,   589,   421,   422,   425,   426,   405,
     601,   430,   412,   434,   435,  -878,   436,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,   437,   443,   444,   445,  -878,  -878,
     451,   314,   465,   476,  -878,   473,   125,   125,   480,  -878,
     484,   258,   125,   125,  -878,   125,   125,   125,   125,   125,
     244,   405,  -878,   482,   485,  -878,  -878,  -878,  -878,  -878,
    -878,   478,    34,    60,  -878,  -878,   244,   405,   488,   490,
     491,   740,   740,  -878,  -878,   125,  -106,   666,    36,   670,
     496,   493,   796,   499,   125,   173,   679,   500,   510,   125,
     511,   314,   525,   125,   314,   -13,   -13,   796,   622,   624,
    -878,  -878,   539,   543,   645,  -878,   -13,   -13,   387,  -878,
     547,   542,   796,   550,   125,    28,   697,   729,   561,   699,
     701,    85,   -13,   659,   565,   660,   -27,   -38,  -878,   566,
     -27,   -27,   -27,   125,   567,    45,   -13,    42,   663,   593,
     600,   661,   -16,   745,    12,   642,  -878,    64,    64,  -878,
     185,   569,    -3,   851,  -878,  -878,   224,   479,   159,   159,
    -878,  -878,  -878,    88,  -878,   796,   577,   -48,   -35,     7,
     133,  -878,  -878,   244,   405,   150,   201,   144,  -878,   572,
     594,  -878,  -878,  -878,   753,  -878,   591,   331,  -878,   599,
     764,   640,  -878,  -878,   343,  -878,  -878,   595,   649,   102,
    -878,   596,   669,  -878,  -878,  -878,  -878,   616,   626,   -13,
     -13,   582,   628,   639,   644,   646,  -878,   655,   462,  -878,
     652,   657,  -878,   658,   662,  -878,  -878,   831,   681,  -878,
    -878,   667,  -878,   671,   832,  -878,   676,  -878,   675,  -878,
     682,  -878,  -878,  -878,   833,  -878,   688,  -878,   835,   690,
      28,   700,   702,  -878,   708,   771,  -878,  -878,  -878,  -878,
    -878,   709,  -878,   698,  -878,   710,  -878,   873,   711,  -878,
     713,   781,   887,  -878,   716,   314,  -878,   668,   125,  -878,
    -878,   244,   718,  -878,   717,   719,  -878,  -878,  -878,  -878,
     893,   727,  -878,    -2,  -878,   125,  -878,  -106,  -878,   283,
    -878,    40,  -878,    37,  -878,  -878,  -878,   730,   905,  -878,
    -878,   731,  -878,   724,   737,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,   741,   742,   746,   747,   748,   751,   752,   754,
     755,  -878,   776,  -878,   778,  -878,  -878,   792,  -878,  -878,
    -878,  -878,  -878,   738,  -878,   794,   795,    12,   925,  -878,
     797,   901,   796,  -878,   125,   125,   796,   799,   125,   796,
     796,   798,   802,  -878,   -41,   986,   815,   848,   989,    98,
      99,   914,   821,    -9,  -878,   814,   919,  -878,   125,   824,
    -106,   -13,   825,   915,   827,    -6,   300,   314,    64,  -878,
    -878,   405,   826,   197,   851,  -878,   100,  -878,  -878,   405,
     244,   147,  -878,   176,  -878,   177,  -878,    28,   828,  -878,
    -878,  -878,   938,   916,   923,   125,   125,   125,   125,  -106,
     125,   125,   125,    65,  -878,   847,    10,  -878,   841,   125,
    -878,   840,   202,   417,   842,    28,   780,   844,   846,   125,
    1003,   853,   857,  1023,   858,   855,  -878,  -878,  -878,  -878,
    1030,  -878,   298,   861,  -878,  -878,   863,    44,   244,   864,
     578,  -878,  1037,  -878,  1059,   853,  -878,   888,  -878,  -878,
     889,    18,  -878,  -878,  -878,  -878,   314,   283,  -878,    40,
    -878,    37,  -878,   890,  1061,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,   244,  -878,  -878,  -878,  -878,    11,  -878,  -878,
    -878,  -106,  -878,  -878,  -878,  -878,  -878,  -878,   894,   896,
     898,  -878,  -878,   900,  -878,  -878,  -878,   244,   902,   904,
     906,  -878,  1064,  -878,   405,  -878,  1047,  -878,   125,  -878,
     907,  -878,  -878,  -878,   449,   911,   357,   997,  1085,   917,
      65,  -878,   912,   918,    -6,   125,  -878,  -878,   913,  1008,
    -878,   920,   921,   922,   924,   926,   927,   928,  -878,  -878,
     929,   930,   931,  -878,   932,   933,   997,    44,  1088,   332,
     935,   934,    18,  -878,  -878,  -878,  -878,  -878,   937,  1024,
     244,   125,   125,   125,   -81,  1032,  1098,   995,  -878,   936,
     384,  -878,   125,  1110,  -878,  -878,  -878,  -878,   940,   941,
    1031,   944,  1117,  -878,  -878,   173,   853,  -878,  -878,   946,
    1036,  1120,    31,    65,   192,    36,    36,   -13,   -46,  1121,
    -878,  1122,  1031,  -878,  -878,   950,  -878,  -878,  -878,  1123,
    -878,  -878,  -878,  -878,  -878,   892,  -878,  -878,  -878,  -878,
    -878,  1092,  -878,   170,   952,   357,  1042,   956,  -878,  1130,
     957,   958,   997,  1054,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,   959,
    -878,   795,  -878,   740,   963,   -81,   961,   967,   964,  -878,
    -878,  -878,  -878,  1036,   314,   965,  -878,  -878,  1031,  -878,
    -878,   314,   740,  -878,    28,  -878,  -878,  -878,   966,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,   969,   -13,   314,    83,
     970,  -878,  -878,  -878,   841,   -13,   968,  -878,  -878,  -878
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -878,  -878,  1143,  -878,  -878,  -878,  -878,  -878,  -878,   126,
    -878,  -878,   939,   -95,  -324,   768,   949,  1086,  -452,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  1093,  -878,  -878,  -878,   273,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,   712,
     960,  -878,  -878,  -878,  -878,   650,  -878,  -878,  -878,   307,
    -878,  -878,  -878,  -570,   281,   226,  -878,  -878,   402,   252,
    -878,  -878,  -878,  -878,  -878,   175,  -878,  -878,  1001,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -517,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,   238,   494,  -752,   214,  -878,  -832,  -878,  -877,   172,
    -440,  -556,  -878,  -878,  -878,   438,   784,  -202,  -158,  -366,
     674,   -96,  -361,  -436,  -586,  -473,  -583,  -480,  -579,  -154,
     -61,  -878,   468,  -878,  -878,   703,  -878,  -878,   891,   -93,
     665,  -878,  -439,  -878,  -878,  -878,  -878,  -878,  -136,  -878
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -217
static const yytype_int16 yytable[] =
{
     218,   345,   245,   228,   684,   610,   567,   481,   273,   274,
     215,   592,   482,   612,   580,   302,   303,   455,   305,   307,
     210,   583,   584,   876,   350,   297,   267,   716,   714,   323,
     712,   311,   326,    16,   557,   967,   334,   297,   476,   493,
     526,   308,   350,   258,   932,   315,   316,   317,   856,   329,
     348,   614,   233,   330,   331,   980,   576,   236,   237,   236,
     237,   374,   944,   761,   210,   376,   773,   526,   258,   590,
     236,   237,   338,     7,   296,   290,   270,   510,   224,   828,
     513,   329,   309,   310,   271,   330,   331,     8,  1022,   225,
     226,   740,   210,   539,   291,   324,   325,     9,   327,   328,
     540,    11,   335,   945,   337,   343,   626,   342,   829,   569,
     627,   628,   259,   968,   570,   969,   857,   292,   293,   412,
     413,  1013,   415,   375,   830,   322,   778,    13,   601,   210,
     998,    14,   236,   237,   234,   602,   154,   259,   268,   339,
     558,   603,   879,   312,   880,   461,   970,   526,   604,   332,
     351,   298,   240,   247,   240,  1023,   361,   547,   377,   548,
     549,   793,   577,   306,   962,   241,   629,   630,   631,   762,
     756,   632,   774,    15,   236,   237,   241,   757,   155,   410,
     411,   224,   157,   605,   242,   243,   242,   243,   889,   840,
     606,   678,   225,   226,   211,   247,   633,   634,   635,   636,
     637,   638,   158,   639,   258,   886,   885,    71,   884,   212,
     213,   219,   211,   203,   494,   582,   159,   737,   260,   483,
     640,   641,   248,   320,   261,     1,     2,   212,   213,   642,
     643,   644,   645,   646,   160,   249,   161,   708,   372,   162,
     707,   564,   582,   260,   224,   214,   647,   514,   515,   261,
     502,   503,   592,   212,   213,   225,   226,   648,   524,   525,
     806,   699,   780,   214,   248,   209,   211,   529,   530,   240,
     262,   263,   224,   259,   552,   753,   750,   249,   719,   719,
     241,   212,   213,   225,   226,   460,   247,   489,   568,   214,
     163,   466,   467,   164,   468,   469,   470,   471,   472,   499,
     556,   242,   243,   211,   561,   562,   563,   586,   591,   607,
     217,   351,   484,   165,   516,   166,   608,   214,   212,   213,
     210,   523,   582,   787,   490,   533,   613,   222,   250,   536,
     788,   214,   609,   501,   251,   167,   329,   365,   508,   366,
     330,   331,   512,   367,   368,   369,   987,   529,   530,   -14,
     223,   650,   789,   791,   214,   248,   168,   252,   253,   790,
     792,   659,   660,   538,   229,   230,   231,   232,   249,   320,
     250,   521,   776,   779,   238,   239,   251,   913,   836,   260,
     169,   837,   599,   611,   170,   261,   782,   783,   236,   237,
     526,   357,   358,   359,   527,   904,   905,   906,   171,   252,
     253,   301,   357,   358,   359,   596,   262,   263,   974,   975,
     172,   173,   361,   174,   175,   355,   356,   357,   358,   359,
     176,   587,   177,   178,   179,   180,   181,   370,   182,   353,
     183,   184,   355,   356,   357,   358,   359,   465,   185,   186,
     935,   187,   188,   189,   973,   190,   191,   192,  1016,   193,
     972,   194,   354,   710,   355,   356,   357,   358,   359,   236,
     237,   250,   883,   195,   196,   667,   393,   251,   197,   198,
     200,   201,   236,   237,   202,   304,   238,   239,   208,   220,
     386,   371,   387,   240,   347,   346,   766,   349,   355,   356,
     357,   358,   359,   352,   372,   385,   382,   388,   373,   389,
     224,   355,   356,   357,   358,   359,   390,   392,   393,   212,
     213,   225,   226,   394,   395,   242,   243,   292,   293,   396,
     398,   399,   402,   798,   799,   800,   801,   701,   803,   804,
     805,   403,  -216,   769,   236,   237,   768,   405,   406,   407,
     393,   408,   409,   422,   709,   214,   414,   275,   276,   277,
     278,   279,   416,   280,   240,   281,   282,   283,   284,   285,
     951,   417,   423,   952,   418,   528,   420,   240,   428,   741,
     424,   425,   426,   744,   429,   802,   747,   748,   241,   427,
     430,   827,   431,   432,   433,   434,   242,   243,   446,   435,
     529,   530,   436,   838,   437,   438,   287,   440,   441,   242,
     243,   442,   443,   742,   743,   444,   445,   746,   236,   237,
     447,   448,   449,   450,   355,   356,   357,   358,   359,   451,
     452,   453,   275,   276,   277,   278,   279,   454,   280,   240,
     281,   282,   283,   284,   285,   355,   356,   357,   358,   359,
     286,   457,   355,   356,   357,   358,   359,   355,   356,   357,
     358,   359,   458,   917,   459,   475,   462,   890,   473,   860,
     597,   242,   243,   464,   474,   485,   861,   486,   487,   492,
    1010,   287,   497,   496,   498,   500,   862,  1014,   834,   355,
     902,   357,   358,   359,   505,   506,   507,   509,   844,   941,
     942,   943,   863,   517,  1021,   518,   275,   276,   277,   278,
     279,   511,   280,   240,   281,   282,   283,   284,   285,   355,
     356,   357,   358,   359,   286,   519,   521,   236,   237,   520,
     534,   535,   864,   865,   866,   867,   537,   868,   869,   870,
     871,   872,   542,   543,   545,   242,   243,   544,   546,   553,
     555,   554,   560,   565,   573,   287,    16,   572,   574,   579,
     589,   575,  -169,   600,    17,   615,   617,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,   618,   621,   976,
      28,    29,    30,    31,    32,   616,    33,   900,   620,   624,
     653,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,   657,    45,    46,    47,    48,    49,    50,    51,
      52,    53,   658,    54,   662,   275,   276,   277,   278,   279,
      55,   280,   240,   281,   282,   283,   284,   285,   663,   661,
     664,   622,   665,   286,   355,   356,   357,   358,   359,    56,
     625,   666,   668,   669,   670,   672,   676,   680,   671,   682,
      57,   953,   689,   674,   242,   243,    58,   675,    59,    60,
     654,    61,   677,    62,   287,   678,    63,    64,   679,  1020,
      65,    66,   673,    67,   681,    68,   683,  1028,   236,   237,
     355,   356,   357,   358,   359,   691,   685,   694,   686,   355,
     356,   357,   358,   359,   688,   690,   693,   695,  1003,   696,
     667,   697,   698,   704,    69,   703,    70,   706,   705,   355,
     356,   357,   358,   359,   700,   291,   717,  1015,   718,   720,
     719,   355,   356,   357,   358,   359,   721,   734,   722,   723,
      71,   807,   808,   724,   725,   726,    72,    73,   727,   728,
     738,   729,   730,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,   809,   810,   731,   811,   732,   275,   276,   277,   278,
     279,   841,   280,   240,   281,   282,   283,   284,   285,   733,
     735,   736,   521,   739,   286,   745,   749,   812,   750,   813,
     814,   815,   816,   817,   818,   819,   820,   821,   822,   823,
     752,   753,   754,   755,   759,   242,   243,   760,   764,   765,
     767,   770,   771,   772,   794,   287,   795,   845,   781,   796,
     355,   356,   357,   358,   359,   797,    75,   832,    77,   835,
      79,   839,    81,   842,    83,   843,    85,   849,    87,   846,
      89,   224,    91,   848,   852,   850,   851,   854,    73,   855,
     859,   874,   225,   226,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,   204,   875,   877,   878,   888,   898,    74,   887,
      76,   891,    78,   892,    80,   893,    82,   894,    84,   895,
      86,   896,    88,   897,    90,   899,   901,   903,   909,   911,
     919,   914,   918,   912,   915,   934,   940,   920,   921,   922,
     947,   923,   948,   924,   925,   926,   927,   928,   929,   931,
     949,   930,   936,   939,   954,   950,   955,   937,   956,   957,
     959,   960,   963,   964,   966,   978,   979,   982,   984,   985,
     986,   989,   991,   993,   995,   999,   996,   997,  1000,  1004,
    1006,  1007,  1017,  1008,  1012,  1019,  1025,  1029,    10,   488,
    1027,   344,   205,   199,   623,   938,   336,   566,   933,   981,
    1005,   916,   384,   257,   853,  1009,   977,   961,   751,   990,
     491,   619,   785,   826,   651,   419,   598
};

static const yytype_uint16 yycheck[] =
{
      95,   203,   160,   157,   560,   485,   445,   373,   166,   167,
      71,   463,   373,   486,   454,   173,   174,   341,   176,   177,
       4,   457,   458,   775,     8,    52,    72,   613,   611,   187,
     609,    72,   190,     6,    72,     4,   194,    52,     4,     3,
       3,   177,     8,     3,   876,   181,   182,   183,     4,     4,
     208,   487,     4,     8,     9,   932,    72,    72,    73,    72,
      73,   219,   143,    72,     4,   219,    72,     3,     3,    72,
      72,    73,     5,   177,   170,   168,    64,   401,   184,    69,
     404,     4,   178,   179,    72,     8,     9,   177,     5,   195,
     196,   661,     4,    65,   178,   188,   189,     0,   191,   192,
      72,   102,   195,   184,   197,   201,     4,   200,    98,    67,
       8,     9,    72,    82,    72,    84,    72,   201,   202,   277,
     278,   998,   280,   219,   114,   186,   696,   184,   176,     4,
     962,   184,    72,    73,    86,   183,   176,    72,   184,    72,
     178,   176,   124,   184,   126,   347,   115,     3,   183,    72,
     211,   178,   167,     3,   167,    72,   217,    72,   219,    74,
      75,   717,   178,   178,   916,   178,    64,    65,    66,   178,
      72,    69,   178,   177,    72,    73,   178,    79,   103,   275,
     276,   184,   177,   176,   199,   200,   199,   200,   177,   745,
     183,   180,   195,   196,   178,     3,    94,    95,    96,    97,
      98,    99,   177,   101,     3,   791,   789,   180,   787,   193,
     194,   235,   178,   180,   178,   178,   177,   657,   178,   373,
     118,   119,    72,   178,   184,    15,    16,   193,   194,   127,
     128,   129,   130,   131,   177,    85,   177,   603,   178,   177,
     601,   443,   178,   178,   184,   229,   144,   405,   406,   184,
      77,    78,   704,   193,   194,   195,   196,   155,   416,   417,
     733,   585,   698,   229,    72,   186,   178,   203,   204,   167,
     205,   206,   184,    72,   432,   176,   176,    85,   179,   179,
     178,   193,   194,   195,   196,   346,     3,   382,   446,   229,
     177,   352,   353,   177,   355,   356,   357,   358,   359,   392,
     436,   199,   200,   178,   440,   441,   442,   122,   462,   176,
     178,   372,   373,   177,   407,   177,   183,   229,   193,   194,
       4,   414,   178,   176,   385,   418,   182,     3,   178,   422,
     183,   229,   182,   394,   184,   177,     4,    21,   399,    23,
       8,     9,   403,    27,    28,    29,   176,   203,   204,   179,
     176,   509,   176,   176,   229,    72,   177,   207,   208,   183,
     183,   519,   520,   424,    10,    11,    12,    13,    85,   178,
     178,    71,    72,   697,    76,    77,   184,   850,   176,   178,
     177,   179,   475,   182,   177,   184,   189,   190,    72,    73,
       3,   232,   233,   234,     7,    38,    39,    40,   177,   207,
     208,   184,   232,   233,   234,   181,   205,   206,   925,   926,
     177,   177,   473,   177,   177,   230,   231,   232,   233,   234,
     177,   236,   177,   177,   177,   177,   177,   111,   177,   158,
     177,   177,   230,   231,   232,   233,   234,   179,   177,   177,
     879,   177,   177,   177,   924,   177,   177,   177,  1004,   177,
     923,   177,   181,   607,   230,   231,   232,   233,   234,    72,
      73,   178,   786,   177,   177,     3,     4,   184,   177,   177,
     177,   177,    72,    73,   177,    83,    76,    77,   177,   177,
     236,   165,   176,   167,   180,   235,   688,   178,   230,   231,
     232,   233,   234,   180,   178,   180,   182,   176,   182,   176,
     184,   230,   231,   232,   233,   234,   175,   178,     4,   193,
     194,   195,   196,   180,   176,   199,   200,   201,   202,   176,
       3,   180,     3,   725,   726,   727,   728,   588,   730,   731,
     732,   180,   182,   691,    72,    73,   690,   176,   176,   176,
       4,   176,   176,   238,   605,   229,   178,   160,   161,   162,
     163,   164,   178,   166,   167,   168,   169,   170,   171,   172,
     176,   178,     4,   179,   178,   178,   178,   167,     4,   662,
     180,   176,   176,   666,   176,   729,   669,   670,   178,   178,
     176,   735,   176,   176,   176,     4,   199,   200,   176,   176,
     203,   204,   176,   176,   176,     6,   209,   176,   176,   199,
     200,   176,   176,   664,   665,     4,   176,   668,    72,    73,
     176,   176,   176,   176,   230,   231,   232,   233,   234,   176,
     176,   176,   160,   161,   162,   163,   164,   176,   166,   167,
     168,   169,   170,   171,   172,   230,   231,   232,   233,   234,
     178,   176,   230,   231,   232,   233,   234,   230,   231,   232,
     233,   234,   176,   855,   181,   177,   176,   811,   176,    81,
     181,   199,   200,   179,   179,   177,    88,   177,   177,     3,
     994,   209,   176,     3,   181,   176,    98,  1001,   739,   230,
     231,   232,   233,   234,     5,   185,   176,   176,   749,   891,
     892,   893,   114,    71,  1018,    71,   160,   161,   162,   163,
     164,   176,   166,   167,   168,   169,   170,   171,   172,   230,
     231,   232,   233,   234,   178,   176,    71,    72,    73,   176,
     173,   179,   144,   145,   146,   147,   176,   149,   150,   151,
     152,   153,    35,     4,    35,   199,   200,   176,    37,    80,
      80,   176,   176,   176,   151,   209,     6,    84,   148,     4,
     181,    90,   110,   176,    14,   183,     3,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,   176,     4,   927,
      30,    31,    32,    33,    34,   181,    36,   838,   179,   184,
     184,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,   176,    53,    54,    55,    56,    57,    58,    59,
      60,    61,   176,    63,   176,   160,   161,   162,   163,   164,
      70,   166,   167,   168,   169,   170,   171,   172,   179,   237,
     176,   181,   176,   178,   230,   231,   232,   233,   234,    89,
     181,   176,   180,   176,   176,     4,     4,     4,   176,     4,
     100,   902,    71,   176,   199,   200,   106,   176,   108,   109,
     181,   111,   176,   113,   209,   180,   116,   117,   176,  1017,
     120,   121,   181,   123,   176,   125,   176,  1025,    72,    73,
     230,   231,   232,   233,   234,   177,   176,     4,   176,   230,
     231,   232,   233,   234,   176,   176,   176,   176,   983,   176,
       3,   110,   176,   176,   154,   177,   156,     4,   179,   230,
     231,   232,   233,   234,   236,   178,   176,  1002,     3,   185,
     179,   230,   231,   232,   233,   234,   179,   179,   177,   177,
     180,    74,    75,   177,   177,   177,   186,   187,   177,   177,
       5,   177,   177,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   104,   105,   177,   107,   177,   160,   161,   162,   163,
     164,   181,   166,   167,   168,   169,   170,   171,   172,   177,
     176,   176,    71,   176,   178,   176,   178,   130,   176,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
       4,   176,   144,     4,    80,   199,   200,   176,   184,    80,
     176,   176,    87,   176,   176,   209,    68,     4,   182,    93,
     230,   231,   232,   233,   234,    92,   194,   176,   196,   179,
     198,   179,   200,   179,   202,   179,   204,     4,   206,   176,
     208,   184,   210,   176,     4,   177,   181,   176,   187,   176,
     176,     4,   195,   196,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   187,     4,   176,   176,     5,     3,   193,   179,
     195,   177,   197,   177,   199,   177,   201,   177,   203,   177,
     205,   177,   207,   177,   209,    38,   179,   176,    91,     4,
      82,   179,   179,   176,   176,     7,    72,   177,   177,   177,
      68,   177,     4,   177,   177,   177,   177,   177,   177,   176,
     115,   179,   177,   176,     4,   179,   176,   183,   177,    88,
     176,     4,   176,    87,     4,     4,     4,   177,     5,   237,
      38,   179,    90,   177,     4,    81,   179,   179,   179,   176,
     179,   174,   176,   179,   179,   176,   176,   179,     5,   381,
    1024,   202,    66,    60,   504,   882,   196,   445,   877,   933,
     985,   854,   223,   162,   762,   993,   928,   915,   674,   955,
     386,   497,   704,   735,   509,   284,   473
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   240,   241,   242,   243,   177,   177,     0,
     241,   102,   250,   184,   184,   177,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    30,    31,
      32,    33,    34,    36,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    63,    70,    89,   100,   106,   108,
     109,   111,   113,   116,   117,   120,   121,   123,   125,   154,
     156,   180,   186,   187,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   244,   245,   252,   254,   255,   256,   257,   259,
     260,   268,   271,   272,   274,   275,   282,   284,   285,   287,
     289,   291,   295,   296,   297,   300,   302,   309,   316,   319,
     325,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   341,   342,   343,   344,   348,   349,   351,   352,   353,
     354,   355,   356,   357,   358,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   176,   103,   251,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   274,
     177,   177,   177,   180,   187,   256,   258,   276,   177,   186,
       4,   178,   193,   194,   229,   399,   409,   178,   252,   235,
     177,   286,     3,   176,   184,   195,   196,   385,   398,    10,
      11,    12,    13,     4,    86,   410,    72,    73,    76,    77,
     167,   178,   199,   200,   317,   387,   388,     3,    72,    85,
     178,   184,   207,   208,   327,   396,   397,   327,     3,    72,
     178,   184,   205,   206,   321,   394,   395,    72,   184,   370,
      64,    72,   379,   387,   387,   160,   161,   162,   163,   164,
     166,   168,   169,   170,   171,   172,   178,   209,   387,   407,
     408,   178,   201,   202,   390,   391,   390,    52,   178,   417,
     418,   184,   387,   387,    83,   387,   178,   387,   417,   390,
     390,    72,   184,   299,   371,   417,   417,   417,   386,   399,
     178,   298,   399,   387,   408,   408,   387,   408,   408,     4,
       8,     9,    72,   411,   387,   408,   299,   408,     5,    72,
     310,   292,   408,   390,   255,   386,   235,   180,   387,   178,
       8,   399,   180,   158,   181,   230,   231,   232,   233,   234,
     398,   399,   404,   405,   406,    21,    23,    27,    28,    29,
     111,   165,   178,   182,   387,   390,   398,   399,   413,   414,
     415,   290,   182,   253,   251,   180,   236,   176,   176,   176,
     175,   389,   178,     4,   180,   176,   176,   304,     3,   180,
     328,   326,     3,   180,   320,   176,   176,   176,   176,   176,
     390,   390,   387,   387,   178,   387,   178,   178,   178,   407,
     178,   408,   238,     4,   180,   176,   176,   178,     4,   176,
     176,   176,   176,   176,     4,   176,   176,   176,     6,   273,
     176,   176,   176,   176,     4,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   253,   293,   176,   176,   181,
     399,   386,   176,   269,   179,   179,   399,   399,   399,   399,
     399,   399,   399,   176,   179,   177,     4,   264,   265,   266,
     267,   388,   391,   398,   399,   177,   177,   177,   254,   252,
     399,   385,     3,     3,   178,   350,     3,   176,   181,   408,
     176,   399,    77,    78,   318,     5,   185,   176,   399,   176,
     253,   176,   399,   253,   387,   387,   408,    71,    71,   176,
     176,    71,   312,   408,   387,   387,     3,     7,   178,   203,
     204,   392,   393,   408,   173,   179,   408,   176,   399,    65,
      72,   380,    35,     4,   176,    35,    37,    72,    74,    75,
     381,   384,   387,    80,   176,    80,   417,    72,   178,   305,
     176,   417,   417,   417,   386,   176,   298,   411,   387,    67,
      72,   416,    84,   151,   148,    90,    72,   178,   306,     4,
     379,   294,   178,   392,   392,   288,   122,   236,   283,   181,
      72,   398,   257,   401,   402,   403,   181,   181,   404,   408,
     176,   176,   183,   176,   183,   176,   183,   176,   183,   182,
     396,   182,   394,   182,   392,   183,   181,     3,   176,   389,
     179,     4,   181,   304,   184,   181,     4,     8,     9,    64,
      65,    66,    69,    94,    95,    96,    97,    98,    99,   101,
     118,   119,   127,   128,   129,   130,   131,   144,   155,   329,
     387,   409,   412,   184,   181,   322,   314,   176,   176,   387,
     387,   237,   176,   179,   176,   176,   176,     3,   180,   176,
     176,   176,     4,   181,   176,   176,     4,   176,   180,   176,
       4,   176,     4,   176,   380,   176,   176,   340,   176,    71,
     176,   177,   345,   176,     4,   176,   176,   110,   176,   253,
     236,   399,   277,   177,   176,   179,     4,   391,   388,   399,
     398,   261,   397,   262,   395,   263,   393,   176,     3,   179,
     185,   179,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   179,   176,   176,   379,     5,   176,
     312,   408,   399,   399,   408,   176,   399,   408,   408,   178,
     176,   371,     4,   176,   144,     4,    72,    79,   383,    80,
     176,    72,   178,   307,   184,    80,   386,   176,   398,   387,
     176,    87,   176,    72,   178,   308,    72,   311,   312,   253,
     392,   182,   189,   190,   400,   401,   270,   176,   183,   176,
     183,   176,   183,   380,   176,    68,    93,    92,   386,   386,
     386,   386,   398,   386,   386,   386,   394,    74,    75,   104,
     105,   107,   130,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   323,   382,   384,   398,    69,    98,
     114,   315,   176,   248,   399,   179,   176,   179,   176,   179,
     380,   181,   179,   179,   399,     4,   176,   372,   176,     4,
     177,   181,     4,   317,   176,   176,     4,    72,   313,   176,
      81,    88,    98,   114,   144,   145,   146,   147,   149,   150,
     151,   152,   153,   346,     4,     4,   372,   176,   176,   124,
     126,   278,   279,   253,   397,   395,   393,   179,     5,   177,
     398,   177,   177,   177,   177,   177,   177,   177,     3,    38,
     399,   179,   231,   176,    38,    39,    40,   373,   374,    91,
     375,     4,   176,   394,   179,   176,   308,   386,   179,    82,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     179,   176,   375,   313,     7,   411,   177,   183,   278,   176,
      72,   386,   386,   386,   143,   184,   324,    68,     4,   115,
     179,   176,   179,   399,     4,   176,   177,    88,   377,   176,
       4,   318,   372,   176,    87,   378,     4,     4,    82,    84,
     115,   347,   394,   396,   350,   350,   387,   370,     4,     4,
     377,   314,   177,   281,     5,   237,    38,   176,   246,   179,
     373,    90,   376,   177,   359,     4,   179,   179,   375,    81,
     179,   301,   280,   252,   176,   324,   179,   174,   179,   378,
     253,   360,   179,   377,   253,   252,   380,   176,   303,   176,
     387,   253,     5,    72,   249,   176,   247,   248,   387,   179
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
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_KEY_TYPE));
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
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_KEY_TYPE));
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
#line 6715 "lev_comp.tab.c"
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

