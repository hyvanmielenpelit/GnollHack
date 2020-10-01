
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
#define YYLAST   1172

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  238
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  178
/* YYNRULES -- Number of rules.  */
#define YYNRULES  465
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1023

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
     536,   548,   549,   563,   564,   567,   573,   575,   581,   583,
     589,   591,   597,   599,   610,   617,   619,   621,   623,   625,
     627,   631,   633,   635,   636,   640,   644,   648,   652,   654,
     662,   668,   670,   672,   674,   676,   680,   681,   687,   692,
     693,   697,   699,   701,   703,   705,   708,   710,   712,   714,
     716,   718,   722,   726,   730,   732,   734,   738,   740,   742,
     744,   748,   752,   753,   759,   762,   763,   767,   769,   773,
     775,   779,   783,   785,   787,   791,   793,   795,   797,   801,
     803,   805,   807,   811,   815,   819,   823,   827,   831,   835,
     841,   849,   855,   864,   866,   870,   876,   882,   890,   898,
     905,   911,   912,   915,   921,   925,   937,   944,   945,   949,
     951,   955,   959,   963,   967,   969,   971,   975,   979,   983,
     987,   991,   995,   997,   999,  1001,  1003,  1007,  1011,  1013,
    1019,  1029,  1035,  1039,  1043,  1049,  1059,  1065,  1071,  1072,
    1084,  1085,  1087,  1095,  1099,  1107,  1113,  1119,  1125,  1131,
    1135,  1141,  1149,  1159,  1161,  1163,  1165,  1167,  1169,  1170,
    1173,  1175,  1179,  1181,  1183,  1185,  1186,  1190,  1192,  1194,
    1196,  1198,  1200,  1202,  1204,  1206,  1208,  1210,  1214,  1216,
    1218,  1223,  1225,  1227,  1232,  1234,  1236,  1241,  1243,  1248,
    1254,  1256,  1260,  1262,  1266,  1268,  1270,  1275,  1285,  1287,
    1289,  1294,  1296,  1302,  1304,  1306,  1311,  1313,  1315,  1321,
    1323,  1325,  1327,  1332,  1334,  1336,  1342,  1344,  1346,  1348,
    1352,  1354,  1356,  1360,  1362,  1367,  1371,  1375,  1379,  1383,
    1387,  1391,  1393,  1395,  1399,  1401,  1405,  1406,  1408,  1410,
    1412,  1414,  1418,  1419,  1421,  1423,  1426,  1429,  1434,  1441,
    1446,  1453,  1460,  1467,  1474,  1477,  1484,  1493,  1502,  1513,
    1528,  1531,  1533,  1537,  1539,  1543,  1545,  1547,  1549,  1551,
    1553,  1555,  1557,  1559,  1561,  1563,  1565,  1567,  1569,  1571,
    1573,  1575,  1577,  1579,  1581,  1592
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     239,     0,    -1,    -1,   240,    -1,   241,    -1,   241,   240,
      -1,   242,   249,   251,    -1,    16,   176,   183,    -1,    15,
     176,   183,   175,     3,    -1,    17,   176,    11,   175,   349,
      -1,    17,   176,    10,   175,     3,    -1,    17,   176,    13,
      -1,    17,   176,    12,   175,     3,   175,     3,   175,     5,
     175,     5,   175,   377,   175,   248,   247,    -1,    18,   176,
     407,    -1,    -1,   175,   173,    -1,    -1,   175,   384,    -1,
      -1,   175,     3,    -1,     5,    -1,    72,    -1,    -1,   101,
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
      -1,   255,    -1,   116,   176,   254,    -1,   256,   234,   396,
      -1,   256,   234,   164,   176,   405,    -1,   256,   234,   395,
      -1,   256,   234,   412,   176,   389,    -1,   256,   234,   411,
     176,   391,    -1,   256,   234,   410,   176,   393,    -1,   256,
     234,   384,    -1,   256,   234,   387,    -1,   256,   234,   181,
     265,   182,    -1,   256,   234,   181,   264,   182,    -1,   256,
     234,   181,   263,   182,    -1,   256,   234,   412,   176,   181,
     262,   182,    -1,   256,   234,   411,   176,   181,   261,   182,
      -1,   256,   234,   410,   176,   181,   260,   182,    -1,   256,
     234,   181,   266,   182,    -1,   394,    -1,   260,   175,   394,
      -1,   392,    -1,   261,   175,   392,    -1,   390,    -1,   262,
     175,   390,    -1,   388,    -1,   263,   175,   388,    -1,   385,
      -1,   264,   175,   385,    -1,   396,    -1,   265,   175,   396,
      -1,   395,    -1,   266,   175,   395,    -1,    -1,    -1,   155,
     185,   177,   268,   400,   178,   269,   252,    -1,   185,   177,
     403,   178,    -1,   115,    -1,    -1,     6,    -1,     6,    -1,
     179,   396,   157,   396,   180,    -1,   179,   396,   180,    -1,
      -1,    -1,   122,   275,   179,   383,   180,   276,   181,   277,
     182,    -1,    -1,   278,   277,    -1,    -1,   123,   408,   176,
     279,   251,    -1,    -1,   125,   176,   280,   251,    -1,   124,
      -1,   235,   235,    -1,   121,    -1,   120,   257,   234,   396,
     282,   396,    -1,    -1,   283,   285,   252,    -1,    -1,   119,
     179,   383,   180,   287,   252,    -1,    -1,   273,   176,   289,
     253,    -1,    -1,   108,   273,   291,   292,    -1,   252,    -1,
      -1,   252,   293,   109,   252,    -1,    14,   176,   395,    -1,
      63,    -1,    63,   176,   408,    -1,    63,   176,    72,    -1,
      54,   176,   297,   175,   297,    -1,    54,   176,   297,   175,
     408,    -1,   177,     4,   175,    71,   175,   312,   178,    -1,
     370,   272,   175,   377,    -1,    -1,    99,   176,   298,   175,
     305,   175,   307,   371,   374,   300,   252,    -1,    -1,    48,
     176,   298,   175,   304,   175,   306,   175,   307,   371,   374,
     302,   252,    -1,    -1,   175,     5,    -1,   177,     4,   175,
       4,   178,    -1,    72,    -1,   177,     4,   175,     4,   178,
      -1,    72,    -1,   177,   316,   175,   317,   178,    -1,    72,
      -1,   177,     4,   175,     4,   178,    -1,    72,    -1,   107,
     176,   309,   175,   376,   175,   310,   175,   312,   313,    -1,
      25,   176,   376,   175,   405,   313,    -1,     5,    -1,    72,
      -1,   311,    -1,    72,    -1,    71,    -1,    71,   236,   311,
      -1,     4,    -1,    72,    -1,    -1,   313,   175,   314,    -1,
     113,   176,   114,    -1,    69,   176,    68,    -1,    97,   176,
       4,    -1,    20,    -1,    19,   176,   316,   175,   317,   303,
     184,    -1,    19,   176,   384,   303,   184,    -1,    76,    -1,
      77,    -1,    78,    -1,    77,    -1,    23,   176,   320,    -1,
      -1,    23,   176,   320,   319,   252,    -1,   391,   175,   384,
     321,    -1,    -1,   321,   175,   322,    -1,   395,    -1,   103,
      -1,   104,    -1,   379,    -1,   106,   395,    -1,   131,    -1,
     129,    -1,   133,    -1,   134,    -1,   135,    -1,   136,   176,
     383,    -1,   137,   176,   383,    -1,   138,   176,   383,    -1,
     139,    -1,   140,    -1,   141,   176,   323,    -1,   132,    -1,
     183,    -1,   142,    -1,   183,   236,   323,    -1,    21,   176,
     326,    -1,    -1,    22,   176,   326,   325,   252,    -1,   393,
     327,    -1,    -1,   327,   175,   328,    -1,    66,    -1,   143,
     176,   391,    -1,   409,    -1,   100,   176,   395,    -1,   117,
     176,   383,    -1,   118,    -1,    65,    -1,   126,   176,   383,
      -1,   154,    -1,    64,    -1,   127,    -1,   128,   176,   383,
      -1,   129,    -1,   130,    -1,   384,    -1,    94,   176,    91,
      -1,    93,   176,    92,    -1,    95,   176,   383,    -1,    96,
     176,   383,    -1,    97,   176,   383,    -1,    98,   176,   383,
      -1,    69,   176,    68,    -1,    24,   176,   369,   175,   384,
      -1,    26,   176,   384,   175,    71,   175,   376,    -1,    30,
     176,   384,   175,    71,    -1,    30,   176,   384,   175,    71,
     175,     5,   247,    -1,    31,    -1,    31,   176,   405,    -1,
      44,   176,   384,   175,    80,    -1,    45,   176,   384,   175,
      80,    -1,    45,   176,   414,   175,   414,   175,    80,    -1,
      49,   176,   414,   175,   414,   175,   183,    -1,    50,   176,
     414,   175,   414,   339,    -1,    51,   176,   414,   175,   414,
      -1,    -1,   175,    80,    -1,    57,   176,   405,   175,    84,
      -1,    58,   176,   405,    -1,    59,   176,   384,   175,   150,
     176,   384,   175,    82,   175,    81,    -1,    70,   176,   384,
     175,   147,   344,    -1,    -1,   344,   175,   345,    -1,    81,
      -1,   143,   176,   391,    -1,   144,   176,   393,    -1,   145,
     176,   349,    -1,   146,   176,   349,    -1,   148,    -1,   149,
      -1,   150,   176,   384,    -1,   151,   176,   369,    -1,   113,
     176,   346,    -1,    88,   176,   375,    -1,   152,   176,     4,
      -1,    97,   176,     4,    -1,     4,    -1,    82,    -1,   114,
      -1,    84,    -1,    61,   176,   405,    -1,    60,   176,   405,
      -1,     3,    -1,   177,     3,   175,   377,   178,    -1,   112,
     176,   387,   175,   389,   175,   389,   175,     7,    -1,   110,
     176,   405,   175,   389,    -1,    46,   176,   387,    -1,    47,
     176,   387,    -1,    36,   176,   183,   175,    37,    -1,    36,
     176,   183,   175,    37,   175,   143,   176,   391,    -1,    33,
     176,   387,   175,    35,    -1,    34,   176,   414,   175,    35,
      -1,    -1,    32,   176,   387,   175,   377,   175,   370,   371,
     374,   358,   359,    -1,    -1,   252,    -1,    41,   176,   384,
     175,   378,   175,   380,    -1,    42,   176,   384,    -1,    89,
     176,   405,   175,    90,   175,    87,    -1,   105,   176,   405,
     175,     4,    -1,    43,   176,    83,   175,   384,    -1,   153,
     176,   384,   175,   395,    -1,   153,   176,   384,   175,    72,
      -1,   153,   176,   384,    -1,    55,   176,   396,   175,   384,
      -1,    56,   176,   384,   175,   413,   175,   395,    -1,    53,
     176,   383,   175,   383,   175,   383,   175,   383,    -1,    53,
      -1,   183,    -1,    72,    -1,   183,    -1,    72,    -1,    -1,
     175,   372,    -1,   373,    -1,   373,   175,   372,    -1,    38,
      -1,    39,    -1,    40,    -1,    -1,    88,   176,   375,    -1,
      87,    -1,    64,    -1,    72,    -1,    65,    -1,    72,    -1,
      75,    -1,   381,    -1,    72,    -1,    75,    -1,   381,    -1,
      74,   176,    72,    -1,    79,    -1,    72,    -1,    74,   179,
       4,   180,    -1,   183,    -1,   194,    -1,   195,   179,   396,
     180,    -1,   396,    -1,   385,    -1,   166,   177,   405,   178,
      -1,   198,    -1,   199,   179,   396,   180,    -1,   177,     4,
     175,     4,   178,    -1,    72,    -1,    73,   386,   180,    -1,
     174,    -1,   174,   175,   386,    -1,   388,    -1,   200,    -1,
     201,   179,   396,   180,    -1,   177,     4,   175,     4,   175,
       4,   175,     4,   178,    -1,   390,    -1,   202,    -1,   203,
     179,   396,   180,    -1,     3,    -1,   177,     3,   175,   377,
     178,    -1,   392,    -1,   204,    -1,   205,   179,   396,   180,
      -1,   183,    -1,     3,    -1,   177,     3,   175,   183,   178,
      -1,    72,    -1,   394,    -1,   206,    -1,   207,   179,   396,
     180,    -1,   183,    -1,     3,    -1,   177,     3,   175,   183,
     178,    -1,    72,    -1,    85,    -1,   382,    -1,   395,   235,
     382,    -1,     4,    -1,   406,    -1,   177,     8,   178,    -1,
     192,    -1,   193,   179,   396,   180,    -1,   396,   229,   396,
      -1,   396,   230,   396,    -1,   396,   231,   396,    -1,   396,
     232,   396,    -1,   396,   233,   396,    -1,   177,   396,   178,
      -1,   188,    -1,   189,    -1,   256,   176,   397,    -1,   398,
      -1,   399,   175,   398,    -1,    -1,   399,    -1,   396,    -1,
     395,    -1,   401,    -1,   402,   175,   401,    -1,    -1,   402,
      -1,   384,    -1,   159,   387,    -1,   160,   387,    -1,   161,
     384,   175,   384,    -1,   162,   384,   175,   384,   175,   396,
      -1,   163,   177,   405,   178,    -1,   163,   177,   311,   175,
     405,   178,    -1,   169,   177,     7,   175,   405,   178,    -1,
     169,   177,   405,   175,   405,   178,    -1,   169,   177,   389,
     175,   405,   178,    -1,   165,   384,    -1,   167,   177,   384,
     175,   396,   178,    -1,   167,   177,   384,   175,   396,   175,
      38,   178,    -1,   168,   177,   384,   175,   396,   175,   396,
     178,    -1,   168,   177,   384,   175,   396,   175,   396,   175,
      38,   178,    -1,   171,   177,   172,   175,   177,   396,   230,
     396,   245,   178,   175,   384,   246,   178,    -1,   170,   404,
      -1,   208,    -1,   177,   405,   178,    -1,   404,    -1,   404,
     237,   405,    -1,   228,    -1,    86,    -1,     4,    -1,     8,
      -1,     9,    -1,     4,    -1,     8,    -1,     9,    -1,     4,
      -1,   406,    -1,    27,    -1,    21,    -1,    28,    -1,    23,
      -1,    29,    -1,   110,    -1,    67,    -1,    72,    -1,   415,
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
    1278,  1297,  1296,  1315,  1318,  1324,  1334,  1340,  1349,  1355,
    1360,  1366,  1371,  1377,  1388,  1394,  1395,  1398,  1399,  1402,
    1406,  1412,  1413,  1417,  1423,  1431,  1436,  1441,  1449,  1456,
    1464,  1472,  1473,  1476,  1477,  1480,  1485,  1484,  1498,  1505,
    1512,  1520,  1525,  1531,  1537,  1543,  1549,  1554,  1559,  1564,
    1569,  1574,  1579,  1584,  1589,  1594,  1599,  1605,  1612,  1621,
    1625,  1638,  1647,  1646,  1664,  1674,  1680,  1688,  1694,  1699,
    1704,  1709,  1714,  1719,  1724,  1729,  1734,  1745,  1751,  1756,
    1761,  1766,  1771,  1776,  1781,  1786,  1791,  1796,  1801,  1808,
    1814,  1843,  1848,  1856,  1862,  1868,  1875,  1882,  1892,  1902,
    1917,  1928,  1931,  1937,  1943,  1949,  1955,  1962,  1968,  1976,
    1982,  1987,  1992,  1997,  2002,  2008,  2014,  2019,  2024,  2029,
    2034,  2039,  2046,  2046,  2046,  2046,  2049,  2055,  2061,  2066,
    2073,  2080,  2086,  2092,  2098,  2102,  2108,  2114,  2125,  2124,
    2149,  2152,  2158,  2165,  2171,  2177,  2183,  2189,  2193,  2198,
    2205,  2211,  2218,  2222,  2229,  2237,  2240,  2250,  2254,  2257,
    2263,  2267,  2274,  2278,  2282,  2289,  2292,  2298,  2304,  2305,
    2308,  2309,  2312,  2313,  2314,  2320,  2321,  2322,  2328,  2329,
    2332,  2341,  2346,  2353,  2364,  2370,  2374,  2378,  2385,  2395,
    2402,  2406,  2412,  2416,  2424,  2428,  2435,  2445,  2458,  2462,
    2469,  2479,  2488,  2499,  2503,  2510,  2520,  2531,  2540,  2550,
    2556,  2560,  2567,  2577,  2588,  2597,  2607,  2611,  2618,  2619,
    2625,  2629,  2633,  2637,  2645,  2654,  2658,  2662,  2666,  2670,
    2674,  2677,  2684,  2693,  2726,  2727,  2730,  2731,  2734,  2738,
    2745,  2752,  2763,  2766,  2774,  2778,  2782,  2786,  2790,  2795,
    2799,  2803,  2808,  2813,  2818,  2822,  2827,  2832,  2836,  2840,
    2845,  2849,  2856,  2862,  2866,  2872,  2879,  2880,  2883,  2884,
    2885,  2888,  2892,  2896,  2900,  2906,  2907,  2910,  2911,  2914,
    2915,  2918,  2919,  2922,  2926,  2952
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
  "TERRAIN_ID", "HORIZ_OR_VERT", "REPLACE_TERRAIN_ID",
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
     377,   377,   378,   378,   378,   379,   379,   379,   380,   380,
     381,   382,   382,   382,   383,   384,   384,   384,   384,   385,
     385,   385,   386,   386,   387,   387,   387,   388,   389,   389,
     389,   390,   390,   391,   391,   391,   392,   392,   392,   392,
     393,   393,   393,   394,   394,   394,   394,   394,   395,   395,
     396,   396,   396,   396,   396,   396,   396,   396,   396,   396,
     396,   397,   397,   398,   399,   399,   400,   400,   401,   401,
     402,   402,   403,   403,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   405,   405,   406,   407,   407,   408,   408,
     408,   409,   409,   409,   409,   410,   410,   411,   411,   412,
     412,   413,   413,   414,   414,   415
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
       9,     5,     3,     3,     5,     9,     5,     5,     0,    11,
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
     400,     0,   403,     0,   445,     0,   401,   422,    27,     0,
     164,     0,     8,     0,   361,   362,     0,   398,   171,     0,
       0,     0,    11,   447,   446,    13,   370,     0,   211,   212,
       0,     0,   367,     0,     0,   183,   365,   394,   396,   397,
       0,   393,   391,     0,   241,   245,   390,   242,   387,   389,
       0,   386,   384,     0,   215,     0,   383,   335,   334,     0,
     348,   349,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,   424,   443,
     274,     0,   375,     0,     0,   374,     0,     0,     0,     0,
     463,     0,     0,   323,     0,     0,     0,     0,     0,   312,
     313,   337,   336,     0,   142,     0,     0,     0,     0,   364,
       0,     0,     0,     0,     0,   284,     0,   307,   306,   450,
     448,   449,   174,   173,     0,     0,     0,     0,   195,   196,
       0,     0,     0,     0,   107,     0,     0,     0,   329,   137,
       0,     0,     0,     0,   146,     0,     0,     0,     0,     0,
     419,   418,   420,   423,     0,   456,   458,   455,   457,   459,
     460,     0,     0,     0,   114,   115,   110,   108,     0,     0,
       0,     0,    26,   161,    24,     0,     0,     0,     0,     0,
     372,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     244,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     425,   426,     0,     0,     0,   434,     0,     0,     0,   440,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   143,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   168,   167,     0,     0,   162,
       0,     0,     0,   416,   402,   410,     0,     0,   405,   406,
     407,   408,   409,     0,   140,     0,   400,     0,     0,     0,
       0,   131,   129,   135,   133,     0,     0,     0,   165,     0,
       0,   399,    10,   308,     0,     9,     0,     0,   371,     0,
       0,     0,   214,   213,   183,   184,   210,     0,     0,     0,
     243,     0,     0,   217,   219,   269,   203,     0,   271,     0,
       0,   199,     0,     0,     0,     0,   381,     0,     0,   379,
       0,     0,   378,     0,     0,   442,   444,     0,     0,   350,
     351,     0,   316,     0,     0,   317,   314,   354,     0,   352,
       0,   353,   326,   275,     0,   276,     0,   186,     0,     0,
       0,     0,   281,   280,     0,     0,   175,   176,   330,   461,
     462,     0,   283,     0,   287,     0,   188,     0,     0,   325,
       0,     0,     0,   311,     0,     0,   158,     0,     0,   148,
     328,   327,     0,   414,   417,     0,   404,   145,   421,   109,
       0,     0,   118,     0,   117,     0,   116,     0,   122,     0,
     113,     0,   112,     0,   111,    28,   363,     0,     0,   373,
     366,     0,   368,     0,     0,   392,   453,   451,   452,   256,
     253,   247,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   252,     0,   257,     0,   259,   260,     0,   255,   246,
     261,   454,   249,     0,   385,   218,   194,     0,     0,   427,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   376,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   178,     0,     0,   279,     0,     0,
       0,     0,   286,     0,     0,     0,     0,     0,     0,   163,
     157,   159,     0,     0,     0,   138,     0,   130,   132,   134,
     136,     0,   123,     0,   125,     0,   127,     0,     0,   369,
     209,   395,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   388,     0,     0,   270,    18,     0,
     200,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   338,     0,     0,     0,     0,   359,   358,   322,   277,
       0,   190,     0,     0,   278,   282,     0,     0,   331,     0,
       0,   324,     0,   192,     0,   338,   198,     0,   197,   170,
       0,   150,   411,   412,   413,   415,     0,     0,   121,     0,
     120,     0,   119,     0,     0,   268,   263,   262,   264,   265,
     266,   267,   250,   251,   254,   258,   248,     0,   355,   222,
     223,     0,   227,   226,   237,   228,   229,   230,     0,     0,
       0,   234,   235,     0,   220,   224,   356,   221,     0,     0,
       0,   204,     0,   272,   428,   430,     0,   435,     0,   431,
       0,   380,   433,   432,     0,     0,     0,   345,     0,     0,
       0,   360,     0,     0,     0,     0,   201,   202,     0,     0,
     289,     0,     0,     0,     0,     0,     0,     0,   294,   295,
       0,     0,     0,   288,     0,     0,   345,     0,     0,     0,
       0,     0,   150,   139,   124,   126,   128,   309,     0,     0,
     225,     0,     0,     0,     0,     0,     0,     0,    19,     0,
       0,   382,     0,     0,   342,   343,   344,   339,   340,     0,
     318,     0,     0,   315,   185,     0,   338,   332,   177,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     187,     0,   179,   203,   310,     0,   154,   149,   151,     0,
     357,   231,   232,   233,   239,   238,   236,   206,   207,   205,
     436,     0,   437,   406,     0,     0,     0,   320,     0,     0,
       0,   345,     0,   347,   299,   301,   302,   303,   305,   304,
     298,   290,   291,   292,   293,   296,   297,   300,     0,     0,
     193,   152,    26,     0,     0,     0,     0,     0,   377,   341,
     346,   321,   319,     0,   465,   189,   181,   285,   191,   180,
      26,   155,     0,   240,   438,    15,     0,   464,     0,   153,
       0,     0,   182,     0,    16,    20,    21,    18,     0,     0,
      12,    17,   439
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    92,    93,   987,  1019,   833,
    1017,    12,   156,    94,   383,    95,    96,    97,    98,   206,
      99,   100,   711,   713,   715,   477,   478,   479,   480,   101,
     463,   786,   102,   103,   439,   104,   105,   207,   702,   881,
     882,  1000,   982,   106,   588,   107,   108,   221,   109,   585,
     110,   381,   111,   341,   456,   581,   112,   113,   114,   321,
     313,   115,   979,   116,  1008,   397,   559,   578,   763,   775,
     117,   340,   777,   522,   858,   656,   831,   118,   244,   504,
     119,   404,   264,   655,   824,   946,   120,   401,   254,   400,
     649,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   687,   131,   132,   133,   134,   692,   873,   970,   135,
     136,   495,   137,   138,   139,   140,   141,   142,   143,   144,
     957,   992,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   269,   314,   847,   907,   908,   910,   964,   272,   541,
     550,   825,   758,   551,   227,   318,   288,   246,   391,   294,
     295,   531,   532,   265,   266,   255,   256,   360,   319,   784,
     593,   594,   595,   362,   363,   364,   289,   421,   216,   235,
     333,   652,   378,   379,   380,   571,   299,   300
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -834
static const yytype_int16 yypact[] =
{
      97,   -83,   -55,   143,  -834,    97,    37,   -36,   -28,  -834,
    -834,     0,   762,    -7,  -834,    78,  -834,    21,    27,    35,
      53,  -834,   102,   113,   156,   164,   168,   172,   189,   206,
     212,   230,   271,   273,   275,   276,   284,   285,   288,   289,
     292,   295,   297,   302,   307,   311,   312,   315,   316,   317,
     321,   322,   326,   327,   329,   332,   338,   339,   344,   345,
      57,   346,   355,  -834,   356,   -20,   817,  -834,  -834,   358,
      98,    62,   268,  -834,  -834,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,   762,  -834,  -834,   224,  -834,
    -834,  -834,  -834,  -834,   361,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,   491,   370,  -834,   -98,   426,    58,
     -25,   146,   146,   166,   -44,    46,   308,   308,   549,  -110,
    -110,   -17,   313,   308,   308,   421,   308,   282,  -110,  -110,
     -16,   -17,   -17,   -17,    62,   378,    62,   308,   549,   549,
     308,   549,   549,    65,   308,   549,   -16,   549,    52,  -834,
     549,  -110,   811,    62,  -834,  -834,   323,   377,   308,   383,
    -834,    33,  -834,   389,  -834,   170,  -834,    29,  -834,   318,
    -834,   388,  -834,    78,  -834,  -834,   394,  -834,   365,   399,
     427,   430,  -834,  -834,  -834,  -834,  -834,   432,  -834,  -834,
     424,   609,  -834,   435,   442,   443,  -834,  -834,  -834,  -834,
     616,  -834,  -834,   441,  -834,  -834,  -834,  -834,  -834,  -834,
     620,  -834,  -834,   445,   446,   451,  -834,  -834,  -834,   454,
    -834,  -834,   455,   456,   457,  -110,  -110,   308,   308,   462,
     308,   470,   472,   477,   549,   478,   475,  -834,  -834,   396,
    -834,   644,  -834,   484,   481,  -834,   496,   498,   673,   506,
    -834,   507,   509,  -834,   511,   518,   683,   519,   520,  -834,
    -834,  -834,  -834,   521,   693,   530,   531,   552,   553,   310,
     735,   565,   185,   571,   574,  -834,   575,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,   576,   577,   578,   579,  -834,  -834,
     580,   388,   581,   583,  -834,   587,    62,    62,   584,  -834,
     582,   178,    62,    62,  -834,    62,    62,    62,    62,    62,
     365,   310,  -834,   586,   592,  -834,  -834,  -834,  -834,  -834,
    -834,   595,    51,    22,  -834,  -834,   365,   310,   596,   597,
     598,   762,   762,  -834,  -834,    62,   -98,   772,    50,   775,
     614,   610,   549,   622,    62,   192,   786,   615,   625,    62,
     626,   388,   627,    62,   388,   308,   308,   549,   743,   753,
    -834,  -834,   651,   652,   499,  -834,   308,   308,   264,  -834,
     656,   653,   549,   654,    62,    34,   795,   829,   659,   800,
     799,   304,   308,   757,   663,   760,   -17,   -26,  -834,   667,
     -17,   -17,   -17,    62,   668,    30,   308,   110,   761,   694,
     700,   759,     6,   844,    46,   741,  -834,   100,   100,  -834,
     154,   672,    -8,   793,  -834,  -834,   320,   379,   126,   126,
    -834,  -834,  -834,    29,  -834,   549,   679,    10,    13,    18,
      19,  -834,  -834,   365,   310,   124,    68,   123,  -834,   674,
     471,  -834,  -834,  -834,   854,  -834,   684,   432,  -834,   680,
     858,   492,  -834,  -834,   443,  -834,  -834,   682,   500,    36,
    -834,   685,   505,  -834,  -834,  -834,  -834,   691,   698,   308,
     308,   639,   701,   702,   704,   708,  -834,   710,   417,  -834,
     709,   714,  -834,   715,   716,  -834,  -834,   890,   512,  -834,
    -834,   721,  -834,   722,   894,  -834,   724,  -834,   732,  -834,
     725,  -834,  -834,  -834,   908,  -834,   738,  -834,   910,   744,
      34,   756,   763,  -834,   764,   845,  -834,  -834,  -834,  -834,
    -834,   765,  -834,   766,  -834,   768,  -834,   928,   769,  -834,
     770,   824,   931,  -834,   771,   388,  -834,   717,    62,  -834,
    -834,   365,   773,  -834,   776,   796,  -834,  -834,  -834,  -834,
     932,   798,  -834,     4,  -834,    62,  -834,   -98,  -834,    85,
    -834,   132,  -834,    55,  -834,  -834,  -834,   778,   934,  -834,
    -834,   802,  -834,   751,   803,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,   774,   797,   801,   806,   831,   846,   848,   850,
     851,  -834,   852,  -834,   853,  -834,  -834,   855,  -834,  -834,
    -834,  -834,  -834,   827,  -834,   857,   859,    46,   971,  -834,
     860,   907,   549,  -834,    62,    62,   549,   861,    62,   549,
     549,   856,   862,  -834,   -16,  1026,   863,   896,  1036,   161,
     138,   961,   867,     7,  -834,   864,   963,  -834,    62,   869,
     -98,   308,   870,   959,   873,    14,   303,   388,   100,  -834,
    -834,   310,   868,   209,   793,  -834,   191,  -834,  -834,   310,
     365,    71,  -834,   111,  -834,   153,  -834,    34,   875,  -834,
    -834,  -834,   983,   960,   962,    62,    62,    62,    62,   -98,
      62,    62,    62,   166,  -834,   789,     1,  -834,   879,    62,
    -834,   877,    88,   334,   878,    34,   533,   880,   881,    62,
    1053,   885,   886,  1058,   887,   884,  -834,  -834,  -834,  -834,
    1061,  -834,   328,   891,  -834,  -834,   892,    45,   365,   893,
     758,  -834,  1065,  -834,  1066,   885,  -834,   897,  -834,  -834,
     898,   269,  -834,  -834,  -834,  -834,   388,    85,  -834,   132,
    -834,    55,  -834,   899,  1069,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,   365,  -834,  -834,  -834,  -834,   217,  -834,  -834,
    -834,   -98,  -834,  -834,  -834,  -834,  -834,  -834,   895,   900,
     902,  -834,  -834,   903,  -834,  -834,  -834,   365,   904,   905,
     906,  -834,  1072,  -834,   310,  -834,  1045,  -834,    62,  -834,
     909,  -834,  -834,  -834,   366,   911,   416,   996,  1081,   913,
     166,  -834,   912,   914,    14,    62,  -834,  -834,   915,  1009,
    -834,   916,   918,   919,   920,   921,   922,   923,  -834,  -834,
     924,   925,   926,  -834,   927,   929,   996,    45,  1096,   107,
     930,   933,   269,  -834,  -834,  -834,  -834,  -834,   935,  1035,
     365,    62,    62,    62,   -60,  1040,  1105,   997,  -834,   936,
     360,  -834,    62,  1108,  -834,  -834,  -834,  -834,   938,   940,
    -834,   942,  1114,  -834,  -834,   192,   885,  -834,  -834,   944,
    1033,  1117,    64,   166,   146,    50,    50,   308,   -44,  1118,
    -834,  1119,  -834,  -834,  -834,   948,  -834,  -834,  -834,  1120,
    -834,  -834,  -834,  -834,  -834,   901,  -834,  -834,  -834,  -834,
    -834,  1088,  -834,   244,   949,   416,  1033,   388,  1124,   951,
     952,   996,  1050,  -834,  -834,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,   954,   388,
     859,  -834,   762,   958,   -60,   956,   965,   957,  -834,  -834,
    -834,  -834,  -834,   964,  -834,  -834,  -834,  -834,  -834,  -834,
     762,  -834,    34,  -834,  -834,  -834,   966,  -834,   388,  -834,
     968,   308,  -834,    99,   969,  -834,  -834,   879,   308,   967,
    -834,  -834,  -834
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -834,  -834,  1131,  -834,  -834,  -834,  -834,  -834,  -834,   122,
    -834,  -834,   917,   -95,  -329,   767,   945,  1080,  -452,  -834,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,  1089,  -834,  -834,  -834,   270,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,   705,
     955,  -834,  -834,  -834,  -834,   649,  -834,  -834,  -834,   300,
    -834,  -834,  -834,  -554,   278,   223,  -834,  -834,   395,   243,
    -834,  -834,  -834,  -834,  -834,   175,  -834,  -834,   998,  -834,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,
    -834,  -483,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,
    -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,  -834,
    -834,   233,   488,  -744,   208,  -834,  -833,   210,  -427,  -556,
    -834,  -834,  -834,   429,   779,  -202,  -158,  -366,   670,   194,
    -359,  -433,  -593,  -473,  -581,  -480,  -567,  -154,   -61,  -834,
     464,  -834,  -834,   696,  -834,  -834,   888,  -108,   661,  -834,
    -439,  -834,  -834,  -834,  -834,  -834,  -160,  -834
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -217
static const yytype_int16 yytable[] =
{
     218,   345,   245,   228,   684,   610,   567,   481,   273,   274,
     215,   592,   455,   612,   482,   302,   303,   308,   305,   307,
     716,   315,   316,   317,   583,   584,   210,   580,   267,   323,
     714,   876,   326,   210,   329,   297,   334,   210,   330,   331,
     626,   350,   712,   932,   627,   628,   557,   236,   237,   856,
     348,   238,   239,   493,   614,   476,   311,   338,   526,   350,
     290,   374,   233,    16,   590,   376,   210,   291,   966,   329,
     828,   258,   510,   330,   331,   513,   236,   237,   576,   761,
     324,   325,   944,   327,   328,   224,   773,   335,   247,   337,
     292,   293,   342,     7,   236,   237,   225,   226,   829,   539,
     629,   630,   631,   526,  1015,   632,   540,   740,   236,   237,
     270,   329,     1,     2,   830,   330,   331,   857,   271,   412,
     413,     8,   415,   945,   339,   322,   526,   247,   996,   633,
     634,   635,   636,   637,   638,   258,   639,   332,    11,   268,
     259,   240,   778,     9,   234,   461,   967,    13,   968,   247,
     351,   558,   241,   640,   641,    14,   361,   248,   377,   203,
     298,   793,   642,   643,   644,   645,   646,   312,   154,   258,
     249,  1016,   961,   242,   243,   224,    15,   569,   969,   647,
     155,   241,   570,   577,   762,   601,   225,   226,   603,   840,
     648,   774,   602,   605,   607,   604,   248,   157,   886,   372,
     606,   608,   240,   158,   259,   224,   211,   320,   885,   249,
     211,   159,   224,   241,   212,   213,   225,   226,   248,   483,
     884,   212,   213,   225,   226,   212,   213,   494,   211,   160,
     737,   249,   582,   756,   242,   243,    71,   708,   259,   211,
     757,   564,   707,   212,   213,   260,   787,   514,   515,   611,
     214,   261,   592,   788,   212,   213,   699,   214,   524,   525,
     806,   214,   250,   836,   214,   780,   837,   526,   251,   502,
     503,   527,   262,   263,   552,   586,   556,   582,   161,   214,
     561,   562,   563,   209,   499,   460,   789,   489,   568,   162,
     214,   466,   467,   790,   468,   469,   470,   471,   472,   516,
     582,   250,   529,   530,   613,   609,   523,   251,   591,   260,
     533,   351,   484,   753,   536,   261,   719,   355,   356,   357,
     358,   359,   210,   250,   490,   529,   530,   353,   791,   251,
     252,   253,   163,   501,   297,   792,   236,   237,   508,   365,
     164,   366,   512,   260,   165,   367,   368,   369,   166,   261,
     354,   650,   252,   253,   236,   237,   465,   357,   358,   359,
     446,   659,   660,   538,   296,   167,   750,   599,   779,   719,
     262,   263,   309,   310,   521,   776,   547,   913,   548,   549,
     236,   237,   168,   355,   356,   357,   358,   359,   169,   587,
     236,   237,   879,   889,   880,   343,   678,   782,   783,   355,
     356,   357,   358,   359,   238,   239,   170,   355,   356,   357,
     358,   359,   361,   375,   355,   356,   357,   358,   359,   986,
     667,   393,   -14,   275,   276,   277,   278,   279,   370,   280,
     240,   281,   282,   283,   284,   285,   229,   230,   231,   232,
     935,   528,   973,   974,   972,   217,  1010,   171,   240,   172,
     971,   173,   174,   710,   904,   905,   906,   883,   219,   306,
     175,   176,   242,   243,   177,   178,   529,   530,   179,   410,
     411,   180,   287,   181,   240,   357,   358,   359,   182,   393,
     242,   243,   371,   183,   240,   241,   766,   184,   185,   236,
     237,   186,   187,   188,   222,   372,   301,   189,   190,   373,
     596,   224,   191,   192,   304,   193,   242,   243,   194,   838,
     212,   213,   225,   226,   195,   196,   242,   243,   292,   293,
     197,   198,   200,   798,   799,   800,   801,   701,   803,   804,
     805,   201,   202,   769,   208,   951,   768,   220,   952,   355,
     356,   357,   358,   359,   709,   223,   214,   236,   237,   355,
     356,   357,   358,   359,   741,   320,   347,   346,   744,   597,
     349,   747,   748,   355,   356,   357,   358,   359,   352,   382,
     521,   236,   237,   385,   387,   802,   275,   276,   277,   278,
     279,   827,   280,   240,   281,   282,   283,   284,   285,   355,
     356,   357,   358,   359,   286,   355,   902,   357,   358,   359,
     386,   392,   388,   742,   743,   389,   390,   746,   355,   356,
     357,   358,   359,   393,   394,   242,   243,   395,   396,   398,
     399,   236,   237,   402,   403,   287,   405,  -216,   991,   406,
     407,   408,   409,   422,   275,   276,   277,   278,   279,   414,
     280,   240,   281,   282,   283,   284,   285,   416,   423,   417,
     999,   616,   286,   917,   418,   420,   425,   890,   275,   276,
     277,   278,   279,   424,   280,   240,   281,   282,   283,   284,
     285,   426,   622,   242,   243,   427,   286,   428,   834,  1012,
     625,   429,   430,   287,   431,   654,   432,   434,   844,   941,
     942,   943,   673,   433,   435,   436,   437,   242,   243,   438,
     355,   356,   357,   358,   359,   440,   441,   287,   275,   276,
     277,   278,   279,   841,   280,   240,   281,   282,   283,   284,
     285,   355,   356,   357,   358,   359,   286,   442,   443,   355,
     356,   357,   358,   359,   355,   356,   357,   358,   359,   444,
     445,   355,   356,   357,   358,   359,   447,   242,   243,   448,
     449,   450,   451,   452,   453,   454,   457,   287,   458,   462,
     464,   473,   355,   356,   357,   358,   359,   459,    16,   975,
     474,   475,   485,   486,   487,   492,    17,   900,   496,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,   497,
     498,   505,    28,    29,    30,    31,    32,   500,    33,   506,
     507,   509,   511,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,   517,    45,    46,    47,    48,    49,
      50,    51,    52,    53,   518,    54,   519,   520,   534,   537,
     542,   535,    55,   543,   544,   545,   546,   553,   554,   860,
     555,   953,   560,   565,   573,   572,   861,   574,   579,   575,
    -169,    56,   589,  1014,   600,   862,   615,   617,   620,   618,
    1021,    57,   621,   807,   808,   624,   657,    58,   653,    59,
      60,   863,    61,   658,    62,   661,   662,    63,    64,   664,
     663,    65,    66,   665,    67,   666,    68,  1001,   668,   669,
     670,   671,   809,   810,   672,   811,   674,   675,   676,   677,
     679,   864,   865,   866,   867,  1009,   868,   869,   870,   871,
     872,   678,   680,   681,   682,    69,   689,    70,   812,   683,
     813,   814,   815,   816,   817,   818,   819,   820,   821,   822,
     823,   685,   694,   697,   667,   720,   706,   718,   686,   688,
     690,    71,   691,   693,   695,   696,   698,    72,    73,   703,
     722,   704,   700,   717,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,   224,   723,   705,   291,   738,   724,   521,    73,
     719,   721,   725,   225,   226,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,   204,    75,   734,    77,   726,    79,    74,
      81,    76,    83,    78,    85,    80,    87,    82,    89,    84,
      91,    86,   727,    88,   728,    90,   729,   730,   731,   732,
     752,   733,   735,   749,   736,   739,   745,   750,   753,   754,
     755,   759,   760,   765,   767,   770,   771,   764,   772,   781,
     794,   795,   796,   797,   832,   835,   839,   845,   842,   843,
     846,   848,   849,   850,   851,   852,   854,   855,   859,   874,
     875,   891,   877,   878,   888,   898,   892,   887,   893,   894,
     895,   896,   897,   899,   909,   911,   903,   901,   912,   915,
     914,   919,   920,   918,   921,   922,   923,   924,   925,   926,
     927,   928,   929,   934,   931,   930,   936,   940,   947,   948,
     939,   949,   954,   955,   950,   937,   956,   958,   959,   962,
     963,   965,   977,   978,   981,   983,   985,   988,   993,   994,
     995,   997,   998,  1002,  1004,  1006,    10,   984,  1005,  1020,
     384,  1011,  1007,  1013,  1018,  1022,   205,   344,   488,   199,
     566,   336,   938,   623,   916,   933,   980,   853,   960,  1003,
     257,   976,   751,   989,   826,   491,   990,   619,   785,   598,
     651,     0,   419
};

static const yytype_int16 yycheck[] =
{
      95,   203,   160,   157,   560,   485,   445,   373,   166,   167,
      71,   463,   341,   486,   373,   173,   174,   177,   176,   177,
     613,   181,   182,   183,   457,   458,     4,   454,    72,   187,
     611,   775,   190,     4,     4,    52,   194,     4,     8,     9,
       4,     8,   609,   876,     8,     9,    72,    72,    73,     4,
     208,    76,    77,     3,   487,     4,    72,     5,     3,     8,
     168,   219,     4,     6,    72,   219,     4,   177,     4,     4,
      69,     3,   401,     8,     9,   404,    72,    73,    72,    72,
     188,   189,   142,   191,   192,   183,    72,   195,     3,   197,
     200,   201,   200,   176,    72,    73,   194,   195,    97,    65,
      64,    65,    66,     3,     5,    69,    72,   661,    72,    73,
      64,     4,    15,    16,   113,     8,     9,    72,    72,   277,
     278,   176,   280,   183,    72,   186,     3,     3,   961,    93,
      94,    95,    96,    97,    98,     3,   100,    72,   101,   183,
      72,   166,   696,     0,    86,   347,    82,   183,    84,     3,
     211,   177,   177,   117,   118,   183,   217,    72,   219,   179,
     177,   717,   126,   127,   128,   129,   130,   183,   175,     3,
      85,    72,   916,   198,   199,   183,   176,    67,   114,   143,
     102,   177,    72,   177,   177,   175,   194,   195,   175,   745,
     154,   177,   182,   175,   175,   182,    72,   176,   791,   177,
     182,   182,   166,   176,    72,   183,   177,   177,   789,    85,
     177,   176,   183,   177,   192,   193,   194,   195,    72,   373,
     787,   192,   193,   194,   195,   192,   193,   177,   177,   176,
     657,    85,   177,    72,   198,   199,   179,   603,    72,   177,
      79,   443,   601,   192,   193,   177,   175,   405,   406,   181,
     228,   183,   704,   182,   192,   193,   585,   228,   416,   417,
     733,   228,   177,   175,   228,   698,   178,     3,   183,    77,
      78,     7,   204,   205,   432,   121,   436,   177,   176,   228,
     440,   441,   442,   185,   392,   346,   175,   382,   446,   176,
     228,   352,   353,   182,   355,   356,   357,   358,   359,   407,
     177,   177,   202,   203,   181,   181,   414,   183,   462,   177,
     418,   372,   373,   175,   422,   183,   178,   229,   230,   231,
     232,   233,     4,   177,   385,   202,   203,   157,   175,   183,
     206,   207,   176,   394,    52,   182,    72,    73,   399,    21,
     176,    23,   403,   177,   176,    27,    28,    29,   176,   183,
     180,   509,   206,   207,    72,    73,   178,   231,   232,   233,
     175,   519,   520,   424,   170,   176,   175,   475,   697,   178,
     204,   205,   178,   179,    71,    72,    72,   850,    74,    75,
      72,    73,   176,   229,   230,   231,   232,   233,   176,   235,
      72,    73,   123,   176,   125,   201,   179,   188,   189,   229,
     230,   231,   232,   233,    76,    77,   176,   229,   230,   231,
     232,   233,   473,   219,   229,   230,   231,   232,   233,   175,
       3,     4,   178,   159,   160,   161,   162,   163,   110,   165,
     166,   167,   168,   169,   170,   171,    10,    11,    12,    13,
     879,   177,   925,   926,   924,   177,  1002,   176,   166,   176,
     923,   176,   176,   607,    38,    39,    40,   786,   234,   177,
     176,   176,   198,   199,   176,   176,   202,   203,   176,   275,
     276,   176,   208,   176,   166,   231,   232,   233,   176,     4,
     198,   199,   164,   176,   166,   177,   688,   176,   176,    72,
      73,   176,   176,   176,     3,   177,   183,   176,   176,   181,
     180,   183,   176,   176,    83,   176,   198,   199,   176,   175,
     192,   193,   194,   195,   176,   176,   198,   199,   200,   201,
     176,   176,   176,   725,   726,   727,   728,   588,   730,   731,
     732,   176,   176,   691,   176,   175,   690,   176,   178,   229,
     230,   231,   232,   233,   605,   175,   228,    72,    73,   229,
     230,   231,   232,   233,   662,   177,   179,   234,   666,   180,
     177,   669,   670,   229,   230,   231,   232,   233,   179,   181,
      71,    72,    73,   179,   175,   729,   159,   160,   161,   162,
     163,   735,   165,   166,   167,   168,   169,   170,   171,   229,
     230,   231,   232,   233,   177,   229,   230,   231,   232,   233,
     235,   177,   175,   664,   665,   175,   174,   668,   229,   230,
     231,   232,   233,     4,   179,   198,   199,   175,   175,     3,
     179,    72,    73,     3,   179,   208,   175,   181,   957,   175,
     175,   175,   175,   237,   159,   160,   161,   162,   163,   177,
     165,   166,   167,   168,   169,   170,   171,   177,     4,   177,
     979,   180,   177,   855,   177,   177,   175,   811,   159,   160,
     161,   162,   163,   179,   165,   166,   167,   168,   169,   170,
     171,   175,   180,   198,   199,   177,   177,     4,   739,  1008,
     180,   175,   175,   208,   175,   180,   175,     4,   749,   891,
     892,   893,   180,   175,   175,   175,   175,   198,   199,     6,
     229,   230,   231,   232,   233,   175,   175,   208,   159,   160,
     161,   162,   163,   180,   165,   166,   167,   168,   169,   170,
     171,   229,   230,   231,   232,   233,   177,   175,   175,   229,
     230,   231,   232,   233,   229,   230,   231,   232,   233,     4,
     175,   229,   230,   231,   232,   233,   175,   198,   199,   175,
     175,   175,   175,   175,   175,   175,   175,   208,   175,   175,
     178,   175,   229,   230,   231,   232,   233,   180,     6,   927,
     178,   176,   176,   176,   176,     3,    14,   838,     3,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,   175,
     180,     5,    30,    31,    32,    33,    34,   175,    36,   184,
     175,   175,   175,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    71,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    71,    63,   175,   175,   172,   175,
      35,   178,    70,     4,   175,    35,    37,    80,   175,    81,
      80,   902,   175,   175,   150,    84,    88,   147,     4,    90,
     109,    89,   180,  1011,   175,    97,   182,     3,   178,   175,
    1018,    99,     4,    74,    75,   183,   175,   105,   183,   107,
     108,   113,   110,   175,   112,   236,   175,   115,   116,   175,
     178,   119,   120,   175,   122,   175,   124,   982,   179,   175,
     175,   175,   103,   104,     4,   106,   175,   175,     4,   175,
     175,   143,   144,   145,   146,  1000,   148,   149,   150,   151,
     152,   179,     4,   175,     4,   153,    71,   155,   129,   175,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   175,     4,   109,     3,   184,     4,     3,   175,   175,
     175,   179,   176,   175,   175,   175,   175,   185,   186,   176,
     176,   175,   235,   175,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   183,   176,   178,   177,     5,   176,    71,   186,
     178,   178,   176,   194,   195,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   186,   193,   178,   195,   176,   197,   192,
     199,   194,   201,   196,   203,   198,   205,   200,   207,   202,
     209,   204,   176,   206,   176,   208,   176,   176,   176,   176,
       4,   176,   175,   177,   175,   175,   175,   175,   175,   143,
       4,    80,   175,    80,   175,   175,    87,   183,   175,   181,
     175,    68,    92,    91,   175,   178,   178,     4,   178,   178,
     175,   175,     4,   176,   180,     4,   175,   175,   175,     4,
       4,   176,   175,   175,     5,     3,   176,   178,   176,   176,
     176,   176,   176,    38,    88,     4,   175,   178,   175,   175,
     178,    82,   176,   178,   176,   176,   176,   176,   176,   176,
     176,   176,   176,     7,   175,   178,   176,    72,    68,     4,
     175,   114,     4,   175,   178,   182,   176,   175,     4,   175,
      87,     4,     4,     4,   176,     5,    38,   178,     4,   178,
     178,    81,   178,   175,   178,   178,     5,   236,   173,  1017,
     223,   175,   178,   175,   175,   178,    66,   202,   381,    60,
     445,   196,   882,   504,   854,   877,   933,   762,   915,   984,
     162,   928,   674,   955,   735,   386,   956,   497,   704,   473,
     509,    -1,   284
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
      58,    59,    60,    61,    63,    70,    89,    99,   105,   107,
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
       4,   177,   192,   193,   228,   396,   406,   177,   251,   234,
     176,   285,     3,   175,   183,   194,   195,   382,   395,    10,
      11,    12,    13,     4,    86,   407,    72,    73,    76,    77,
     166,   177,   198,   199,   316,   384,   385,     3,    72,    85,
     177,   183,   206,   207,   326,   393,   394,   326,     3,    72,
     177,   183,   204,   205,   320,   391,   392,    72,   183,   369,
      64,    72,   376,   384,   384,   159,   160,   161,   162,   163,
     165,   167,   168,   169,   170,   171,   177,   208,   384,   404,
     405,   177,   200,   201,   387,   388,   387,    52,   177,   414,
     415,   183,   384,   384,    83,   384,   177,   384,   414,   387,
     387,    72,   183,   298,   370,   414,   414,   414,   383,   396,
     177,   297,   396,   384,   405,   405,   384,   405,   405,     4,
       8,     9,    72,   408,   384,   405,   298,   405,     5,    72,
     309,   291,   405,   387,   254,   383,   234,   179,   384,   177,
       8,   396,   179,   157,   180,   229,   230,   231,   232,   233,
     395,   396,   401,   402,   403,    21,    23,    27,    28,    29,
     110,   164,   177,   181,   384,   387,   395,   396,   410,   411,
     412,   289,   181,   252,   250,   179,   235,   175,   175,   175,
     174,   386,   177,     4,   179,   175,   175,   303,     3,   179,
     327,   325,     3,   179,   319,   175,   175,   175,   175,   175,
     387,   387,   384,   384,   177,   384,   177,   177,   177,   404,
     177,   405,   237,     4,   179,   175,   175,   177,     4,   175,
     175,   175,   175,   175,     4,   175,   175,   175,     6,   272,
     175,   175,   175,   175,     4,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   252,   292,   175,   175,   180,
     396,   383,   175,   268,   178,   178,   396,   396,   396,   396,
     396,   396,   396,   175,   178,   176,     4,   263,   264,   265,
     266,   385,   388,   395,   396,   176,   176,   176,   253,   251,
     396,   382,     3,     3,   177,   349,     3,   175,   180,   405,
     175,   396,    77,    78,   317,     5,   184,   175,   396,   175,
     252,   175,   396,   252,   384,   384,   405,    71,    71,   175,
     175,    71,   311,   405,   384,   384,     3,     7,   177,   202,
     203,   389,   390,   405,   172,   178,   405,   175,   396,    65,
      72,   377,    35,     4,   175,    35,    37,    72,    74,    75,
     378,   381,   384,    80,   175,    80,   414,    72,   177,   304,
     175,   414,   414,   414,   383,   175,   297,   408,   384,    67,
      72,   413,    84,   150,   147,    90,    72,   177,   305,     4,
     376,   293,   177,   389,   389,   287,   121,   235,   282,   180,
      72,   395,   256,   398,   399,   400,   180,   180,   401,   405,
     175,   175,   182,   175,   182,   175,   182,   175,   182,   181,
     393,   181,   391,   181,   389,   182,   180,     3,   175,   386,
     178,     4,   180,   303,   183,   180,     4,     8,     9,    64,
      65,    66,    69,    93,    94,    95,    96,    97,    98,   100,
     117,   118,   126,   127,   128,   129,   130,   143,   154,   328,
     384,   406,   409,   183,   180,   321,   313,   175,   175,   384,
     384,   236,   175,   178,   175,   175,   175,     3,   179,   175,
     175,   175,     4,   180,   175,   175,     4,   175,   179,   175,
       4,   175,     4,   175,   377,   175,   175,   339,   175,    71,
     175,   176,   344,   175,     4,   175,   175,   109,   175,   252,
     235,   396,   276,   176,   175,   178,     4,   388,   385,   396,
     395,   260,   394,   261,   392,   262,   390,   175,     3,   178,
     184,   178,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   178,   175,   175,   376,     5,   175,
     311,   405,   396,   396,   405,   175,   396,   405,   405,   177,
     175,   370,     4,   175,   143,     4,    72,    79,   380,    80,
     175,    72,   177,   306,   183,    80,   383,   175,   395,   384,
     175,    87,   175,    72,   177,   307,    72,   310,   311,   252,
     389,   181,   188,   189,   397,   398,   269,   175,   182,   175,
     182,   175,   182,   377,   175,    68,    92,    91,   383,   383,
     383,   383,   395,   383,   383,   383,   391,    74,    75,   103,
     104,   106,   129,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   322,   379,   381,   395,    69,    97,
     113,   314,   175,   247,   396,   178,   175,   178,   175,   178,
     377,   180,   178,   178,   396,     4,   175,   371,   175,     4,
     176,   180,     4,   316,   175,   175,     4,    72,   312,   175,
      81,    88,    97,   113,   143,   144,   145,   146,   148,   149,
     150,   151,   152,   345,     4,     4,   371,   175,   175,   123,
     125,   277,   278,   252,   394,   392,   390,   178,     5,   176,
     395,   176,   176,   176,   176,   176,   176,   176,     3,    38,
     396,   178,   230,   175,    38,    39,    40,   372,   373,    88,
     374,     4,   175,   391,   178,   175,   307,   383,   178,    82,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     178,   175,   374,   312,     7,   408,   176,   182,   277,   175,
      72,   383,   383,   383,   142,   183,   323,    68,     4,   114,
     178,   175,   178,   396,     4,   175,   176,   358,   175,     4,
     317,   371,   175,    87,   375,     4,     4,    82,    84,   114,
     346,   391,   393,   349,   349,   384,   369,     4,     4,   300,
     313,   176,   280,     5,   236,    38,   175,   245,   178,   372,
     375,   252,   359,     4,   178,   178,   374,    81,   178,   252,
     279,   251,   175,   323,   178,   173,   178,   178,   302,   251,
     377,   175,   252,   175,   384,     5,    72,   248,   175,   246,
     247,   384,   178
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
#line 1389 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1403 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1407 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1417 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1424 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1432 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1437 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1442 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1450 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1457 "lev_comp.y"
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
#line 1465 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1481 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1485 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1491 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1499 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1505 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1513 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1521 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1526 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1532 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1538 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1544 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1550 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1555 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1560 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1565 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1570 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1575 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1580 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1585 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1590 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1595 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1600 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1606 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1613 "lev_comp.y"
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
#line 1622 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1626 "lev_comp.y"
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
#line 1639 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1647 "lev_comp.y"
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
#line 1657 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1665 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1674 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1681 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1689 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1695 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1700 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1705 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1710 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1715 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1720 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1725 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1730 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1735 "lev_comp.y"
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
#line 1746 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1752 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1757 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1762 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1767 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1772 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1777 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1782 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1787 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1792 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1797 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1802 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1809 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1815 "lev_comp.y"
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
#line 1844 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1849 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1857 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1863 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1869 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1876 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1883 "lev_comp.y"
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
#line 1893 "lev_comp.y"
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
#line 1903 "lev_comp.y"
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
#line 1918 "lev_comp.y"
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
#line 1928 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1932 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1938 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1944 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1950 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1956 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1962 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1969 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1977 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1983 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1988 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1993 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1998 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2003 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2009 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2015 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2020 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2025 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2030 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_CATEGORY));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2035 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2040 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2050 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2056 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2062 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2067 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2074 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2081 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2087 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2093 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2099 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2103 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2109 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2115 "lev_comp.y"
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
#line 2125 "lev_comp.y"
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

  case 319:

/* Line 1455 of yacc.c  */
#line 2139 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      if ( (yyvsp[(10) - (11)].i) ) {
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      } else if ( (yyvsp[(11) - (11)].i) )
			  lc_error("Cannot use lev statements in non-permanent REGION");
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2149 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2153 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2159 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2166 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2172 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2178 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2184 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2190 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2194 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2199 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2206 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2212 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2219 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2223 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2230 "lev_comp.y"
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
#line 2241 "lev_comp.y"
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
#line 2254 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2258 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2264 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2268 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2275 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2279 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2283 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2289 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2293 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2299 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2315 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2323 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2333 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2342 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2347 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2354 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2365 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2371 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2375 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2379 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2386 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2396 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2403 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2407 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2413 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2417 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2425 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2429 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2436 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2446 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2459 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2463 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2470 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2480 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2489 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2500 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2504 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2511 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2521 "lev_comp.y"
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

  case 387:

/* Line 1455 of yacc.c  */
#line 2532 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2541 "lev_comp.y"
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

  case 389:

/* Line 1455 of yacc.c  */
#line 2551 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2557 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2561 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2568 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2578 "lev_comp.y"
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

  case 394:

/* Line 1455 of yacc.c  */
#line 2589 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2598 "lev_comp.y"
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

  case 396:

/* Line 1455 of yacc.c  */
#line 2608 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2612 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2618 "lev_comp.y"
    { ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2620 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2626 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2630 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2634 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2638 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2646 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2655 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2659 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2663 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2667 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2671 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2674 "lev_comp.y"
    { ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2678 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2685 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2694 "lev_comp.y"
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

  case 418:

/* Line 1455 of yacc.c  */
#line 2735 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2739 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2746 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2753 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2763 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2767 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2775 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2779 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2783 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2787 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2791 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2796 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2800 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2804 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2809 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2814 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2819 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2823 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2828 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2833 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2837 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2841 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2846 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2850 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2857 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2863 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2867 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2873 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2889 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2893 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2897 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2901 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2923 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2927 "lev_comp.y"
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

  case 465:

/* Line 1455 of yacc.c  */
#line 2953 "lev_comp.y"
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
#line 6671 "lev_comp.tab.c"
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
#line 2981 "lev_comp.y"


/*lev_comp.y*/

