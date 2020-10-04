
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
     BOUNDARY_TYPE_ID = 276,
     OBJECT_ID = 277,
     COBJECT_ID = 278,
     MONSTER_ID = 279,
     TRAP_ID = 280,
     DOOR_ID = 281,
     DRAWBRIDGE_ID = 282,
     object_ID = 283,
     monster_ID = 284,
     terrain_ID = 285,
     MAZEWALK_ID = 286,
     WALLIFY_ID = 287,
     REGION_ID = 288,
     SPECIAL_REGION_ID = 289,
     SPECIAL_LEVREGION_ID = 290,
     SPECIAL_REGION_TYPE = 291,
     NAMING_ID = 292,
     NAMING_TYPE = 293,
     FILLING = 294,
     IRREGULAR = 295,
     JOINED = 296,
     ALTAR_ID = 297,
     ANVIL_ID = 298,
     NPC_ID = 299,
     LADDER_ID = 300,
     STAIR_ID = 301,
     NON_DIGGABLE_ID = 302,
     NON_PASSWALL_ID = 303,
     ROOM_ID = 304,
     PORTAL_ID = 305,
     TELEPRT_ID = 306,
     BRANCH_ID = 307,
     LEV = 308,
     MINERALIZE_ID = 309,
     CORRIDOR_ID = 310,
     GOLD_ID = 311,
     ENGRAVING_ID = 312,
     FOUNTAIN_ID = 313,
     THRONE_ID = 314,
     MODRON_PORTAL_ID = 315,
     POOL_ID = 316,
     SINK_ID = 317,
     NONE = 318,
     RAND_CORRIDOR_ID = 319,
     DOOR_STATE = 320,
     LIGHT_STATE = 321,
     CURSE_TYPE = 322,
     ENGRAVING_TYPE = 323,
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
     INDESTRUCTIBLE_ID = 387,
     FEMALE_ID = 388,
     WAITFORU_ID = 389,
     CANCELLED_ID = 390,
     REVIVED_ID = 391,
     AVENGE_ID = 392,
     FLEEING_ID = 393,
     BLINDED_ID = 394,
     PARALYZED_ID = 395,
     STUNNED_ID = 396,
     CONFUSED_ID = 397,
     SEENTRAPS_ID = 398,
     ALL_ID = 399,
     MONTYPE_ID = 400,
     OBJTYPE_ID = 401,
     TERTYPE_ID = 402,
     TERTYPE2_ID = 403,
     LEVER_EFFECT_TYPE = 404,
     SWITCHABLE_ID = 405,
     CONTINUOUSLY_USABLE_ID = 406,
     TARGET_ID = 407,
     TRAPTYPE_ID = 408,
     EFFECT_FLAG_ID = 409,
     GRAVE_ID = 410,
     ERODEPROOF_ID = 411,
     FUNCTION_ID = 412,
     MSG_OUTPUT_TYPE = 413,
     COMPARE_TYPE = 414,
     UNKNOWN_TYPE = 415,
     rect_ID = 416,
     fillrect_ID = 417,
     line_ID = 418,
     randline_ID = 419,
     grow_ID = 420,
     selection_ID = 421,
     flood_ID = 422,
     rndcoord_ID = 423,
     circle_ID = 424,
     ellipse_ID = 425,
     filter_ID = 426,
     complement_ID = 427,
     gradient_ID = 428,
     GRADIENT_TYPE = 429,
     LIMITED = 430,
     HUMIDITY_TYPE = 431,
     STRING = 432,
     MAP_ID = 433,
     NQSTRING = 434,
     VARSTRING = 435,
     CFUNC = 436,
     CFUNC_INT = 437,
     CFUNC_STR = 438,
     CFUNC_COORD = 439,
     CFUNC_REGION = 440,
     VARSTRING_INT = 441,
     VARSTRING_INT_ARRAY = 442,
     VARSTRING_STRING = 443,
     VARSTRING_STRING_ARRAY = 444,
     VARSTRING_VAR = 445,
     VARSTRING_VAR_ARRAY = 446,
     VARSTRING_COORD = 447,
     VARSTRING_COORD_ARRAY = 448,
     VARSTRING_REGION = 449,
     VARSTRING_REGION_ARRAY = 450,
     VARSTRING_MAPCHAR = 451,
     VARSTRING_MAPCHAR_ARRAY = 452,
     VARSTRING_MONST = 453,
     VARSTRING_MONST_ARRAY = 454,
     VARSTRING_OBJ = 455,
     VARSTRING_OBJ_ARRAY = 456,
     VARSTRING_SEL = 457,
     VARSTRING_SEL_ARRAY = 458,
     METHOD_INT = 459,
     METHOD_INT_ARRAY = 460,
     METHOD_STRING = 461,
     METHOD_STRING_ARRAY = 462,
     METHOD_VAR = 463,
     METHOD_VAR_ARRAY = 464,
     METHOD_COORD = 465,
     METHOD_COORD_ARRAY = 466,
     METHOD_REGION = 467,
     METHOD_REGION_ARRAY = 468,
     METHOD_MAPCHAR = 469,
     METHOD_MAPCHAR_ARRAY = 470,
     METHOD_MONST = 471,
     METHOD_MONST_ARRAY = 472,
     METHOD_OBJ = 473,
     METHOD_OBJ_ARRAY = 474,
     METHOD_SEL = 475,
     METHOD_SEL_ARRAY = 476,
     DICE = 477
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
#line 520 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 532 "lev_comp.tab.c"

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
#define YYNTOKENS  240
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  181
/* YYNRULES -- Number of rules.  */
#define YYNRULES  471
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1035

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   477

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   235,   239,     2,
     179,   180,   233,   231,   177,   232,   237,   234,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   178,     2,
       2,   236,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   181,     2,   182,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   183,   238,   184,     2,     2,     2,     2,
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
     175,   176,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    15,    19,    25,
      31,    37,    41,    58,    62,    66,    67,    70,    71,    74,
      75,    78,    80,    82,    83,    87,    91,    93,    94,    97,
     101,   103,   105,   107,   109,   111,   113,   115,   117,   119,
     121,   123,   125,   127,   129,   131,   133,   135,   137,   139,
     141,   143,   145,   147,   149,   151,   153,   155,   157,   159,
     161,   163,   165,   167,   169,   171,   173,   175,   177,   179,
     181,   183,   185,   187,   189,   191,   193,   195,   197,   199,
     201,   203,   205,   207,   209,   211,   213,   215,   217,   219,
     221,   223,   225,   227,   229,   231,   233,   235,   237,   239,
     241,   243,   245,   247,   249,   251,   253,   255,   257,   259,
     263,   267,   273,   277,   283,   289,   295,   299,   303,   309,
     315,   321,   329,   337,   345,   351,   353,   357,   359,   363,
     365,   369,   371,   375,   377,   381,   383,   387,   389,   393,
     394,   395,   404,   409,   411,   412,   414,   416,   422,   426,
     427,   428,   438,   439,   442,   443,   449,   450,   455,   457,
     460,   462,   469,   470,   474,   475,   482,   483,   488,   489,
     494,   496,   497,   502,   506,   508,   512,   516,   522,   528,
     536,   541,   542,   555,   556,   571,   572,   575,   581,   583,
     589,   591,   597,   599,   605,   607,   618,   625,   627,   629,
     631,   633,   635,   639,   641,   643,   644,   648,   652,   656,
     660,   662,   670,   676,   678,   680,   682,   684,   688,   689,
     695,   700,   701,   705,   707,   709,   711,   713,   716,   718,
     720,   722,   724,   726,   730,   734,   738,   740,   742,   746,
     748,   750,   752,   756,   760,   761,   767,   770,   771,   775,
     777,   781,   783,   787,   791,   793,   795,   799,   801,   803,
     805,   809,   811,   813,   815,   819,   823,   827,   831,   835,
     839,   843,   845,   851,   859,   865,   874,   876,   880,   886,
     892,   900,   908,   915,   921,   922,   925,   931,   935,   947,
     954,   955,   959,   961,   965,   969,   973,   977,   979,   981,
     985,   989,   993,   997,  1001,  1005,  1007,  1009,  1011,  1013,
    1017,  1021,  1023,  1029,  1039,  1045,  1049,  1053,  1059,  1069,
    1075,  1081,  1082,  1095,  1096,  1098,  1106,  1110,  1118,  1124,
    1130,  1136,  1142,  1146,  1152,  1160,  1170,  1172,  1174,  1176,
    1178,  1180,  1181,  1184,  1186,  1190,  1192,  1194,  1196,  1197,
    1201,  1203,  1204,  1208,  1210,  1212,  1214,  1216,  1218,  1220,
    1222,  1224,  1226,  1228,  1232,  1234,  1236,  1241,  1243,  1245,
    1250,  1252,  1254,  1259,  1261,  1266,  1272,  1274,  1278,  1280,
    1284,  1286,  1288,  1293,  1303,  1305,  1307,  1312,  1314,  1320,
    1322,  1324,  1329,  1331,  1333,  1339,  1341,  1343,  1345,  1350,
    1352,  1354,  1360,  1362,  1364,  1366,  1370,  1372,  1374,  1378,
    1380,  1385,  1389,  1393,  1397,  1401,  1405,  1409,  1411,  1413,
    1417,  1419,  1423,  1424,  1426,  1428,  1430,  1432,  1436,  1437,
    1439,  1441,  1444,  1447,  1452,  1459,  1464,  1471,  1478,  1485,
    1492,  1495,  1502,  1511,  1520,  1531,  1546,  1549,  1551,  1555,
    1557,  1561,  1563,  1565,  1567,  1569,  1571,  1573,  1575,  1577,
    1579,  1581,  1583,  1585,  1587,  1589,  1591,  1593,  1595,  1597,
    1599,  1610
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     241,     0,    -1,    -1,   242,    -1,   243,    -1,   243,   242,
      -1,   244,   252,   254,    -1,    16,   178,   185,    -1,    15,
     178,   185,   177,     3,    -1,    17,   178,    11,   177,   352,
      -1,    17,   178,    10,   177,     3,    -1,    17,   178,    13,
      -1,    17,   178,    12,   177,     3,   177,     3,   177,     5,
     177,     5,   177,   382,   177,   251,   250,    -1,    18,   178,
     412,    -1,    21,   178,    90,    -1,    -1,   177,   175,    -1,
      -1,   177,   389,    -1,    -1,   177,     3,    -1,     5,    -1,
      72,    -1,    -1,   102,   178,   253,    -1,   103,   177,   253,
      -1,   103,    -1,    -1,   256,   254,    -1,   183,   254,   184,
      -1,   297,    -1,   245,    -1,   246,    -1,   247,    -1,   366,
      -1,   365,    -1,   363,    -1,   364,    -1,   367,    -1,   368,
      -1,   341,    -1,   299,    -1,   262,    -1,   261,    -1,   355,
      -1,   311,    -1,   333,    -1,   370,    -1,   371,    -1,   343,
      -1,   344,    -1,   345,    -1,   369,    -1,   277,    -1,   287,
      -1,   289,    -1,   293,    -1,   291,    -1,   274,    -1,   284,
      -1,   270,    -1,   273,    -1,   336,    -1,   346,    -1,   318,
      -1,   334,    -1,   321,    -1,   327,    -1,   356,    -1,   351,
      -1,   339,    -1,   298,    -1,   357,    -1,   358,    -1,   359,
      -1,   360,    -1,   304,    -1,   302,    -1,   350,    -1,   354,
      -1,   353,    -1,   337,    -1,   338,    -1,   340,    -1,   332,
      -1,   335,    -1,   195,    -1,   197,    -1,   199,    -1,   201,
      -1,   203,    -1,   205,    -1,   207,    -1,   209,    -1,   211,
      -1,   194,    -1,   196,    -1,   198,    -1,   200,    -1,   202,
      -1,   204,    -1,   206,    -1,   208,    -1,   210,    -1,   257,
      -1,   258,    -1,   188,    -1,   188,    -1,   258,    -1,   117,
     178,   257,    -1,   259,   236,   401,    -1,   259,   236,   166,
     178,   410,    -1,   259,   236,   400,    -1,   259,   236,   417,
     178,   394,    -1,   259,   236,   416,   178,   396,    -1,   259,
     236,   415,   178,   398,    -1,   259,   236,   389,    -1,   259,
     236,   392,    -1,   259,   236,   183,   268,   184,    -1,   259,
     236,   183,   267,   184,    -1,   259,   236,   183,   266,   184,
      -1,   259,   236,   417,   178,   183,   265,   184,    -1,   259,
     236,   416,   178,   183,   264,   184,    -1,   259,   236,   415,
     178,   183,   263,   184,    -1,   259,   236,   183,   269,   184,
      -1,   399,    -1,   263,   177,   399,    -1,   397,    -1,   264,
     177,   397,    -1,   395,    -1,   265,   177,   395,    -1,   393,
      -1,   266,   177,   393,    -1,   390,    -1,   267,   177,   390,
      -1,   401,    -1,   268,   177,   401,    -1,   400,    -1,   269,
     177,   400,    -1,    -1,    -1,   157,   187,   179,   271,   405,
     180,   272,   255,    -1,   187,   179,   408,   180,    -1,   116,
      -1,    -1,     6,    -1,     6,    -1,   181,   401,   159,   401,
     182,    -1,   181,   401,   182,    -1,    -1,    -1,   123,   278,
     181,   388,   182,   279,   183,   280,   184,    -1,    -1,   281,
     280,    -1,    -1,   124,   413,   178,   282,   254,    -1,    -1,
     126,   178,   283,   254,    -1,   125,    -1,   237,   237,    -1,
     122,    -1,   121,   260,   236,   401,   285,   401,    -1,    -1,
     286,   288,   255,    -1,    -1,   120,   181,   388,   182,   290,
     255,    -1,    -1,   276,   178,   292,   256,    -1,    -1,   109,
     276,   294,   295,    -1,   255,    -1,    -1,   255,   296,   110,
     255,    -1,    14,   178,   400,    -1,    64,    -1,    64,   178,
     413,    -1,    64,   178,    72,    -1,    55,   178,   300,   177,
     300,    -1,    55,   178,   300,   177,   413,    -1,   179,     4,
     177,    71,   177,   315,   180,    -1,   373,   275,   177,   382,
      -1,    -1,   100,   178,   301,   177,   308,   177,   310,   374,
     377,   379,   303,   255,    -1,    -1,    49,   178,   301,   177,
     307,   177,   309,   177,   310,   374,   377,   379,   305,   255,
      -1,    -1,   177,     5,    -1,   179,     4,   177,     4,   180,
      -1,    72,    -1,   179,     4,   177,     4,   180,    -1,    72,
      -1,   179,   319,   177,   320,   180,    -1,    72,    -1,   179,
       4,   177,     4,   180,    -1,    72,    -1,   108,   178,   312,
     177,   381,   177,   313,   177,   315,   316,    -1,    26,   178,
     381,   177,   410,   316,    -1,     5,    -1,    72,    -1,   314,
      -1,    72,    -1,    71,    -1,    71,   238,   314,    -1,     4,
      -1,    72,    -1,    -1,   316,   177,   317,    -1,   114,   178,
     115,    -1,    69,   178,   398,    -1,    98,   178,     4,    -1,
      20,    -1,    19,   178,   319,   177,   320,   306,   186,    -1,
      19,   178,   389,   306,   186,    -1,    76,    -1,    77,    -1,
      78,    -1,    77,    -1,    24,   178,   323,    -1,    -1,    24,
     178,   323,   322,   255,    -1,   396,   177,   389,   324,    -1,
      -1,   324,   177,   325,    -1,   400,    -1,   104,    -1,   105,
      -1,   384,    -1,   107,   400,    -1,   133,    -1,   130,    -1,
     135,    -1,   136,    -1,   137,    -1,   138,   178,   388,    -1,
     139,   178,   388,    -1,   140,   178,   388,    -1,   141,    -1,
     142,    -1,   143,   178,   326,    -1,   134,    -1,   185,    -1,
     144,    -1,   185,   238,   326,    -1,    22,   178,   329,    -1,
      -1,    23,   178,   329,   328,   255,    -1,   398,   330,    -1,
      -1,   330,   177,   331,    -1,    67,    -1,   145,   178,   396,
      -1,   414,    -1,   101,   178,   400,    -1,   118,   178,   388,
      -1,   119,    -1,    66,    -1,   127,   178,   388,    -1,   156,
      -1,    65,    -1,   128,    -1,   129,   178,   388,    -1,   130,
      -1,   131,    -1,   389,    -1,    95,   178,    92,    -1,    94,
     178,    93,    -1,    96,   178,   388,    -1,    97,   178,   388,
      -1,    98,   178,   388,    -1,    99,   178,   388,    -1,    69,
     178,   398,    -1,   132,    -1,    25,   178,   372,   177,   389,
      -1,    27,   178,   389,   177,    71,   177,   381,    -1,    31,
     178,   389,   177,    71,    -1,    31,   178,   389,   177,    71,
     177,     5,   250,    -1,    32,    -1,    32,   178,   410,    -1,
      45,   178,   389,   177,    80,    -1,    46,   178,   389,   177,
      80,    -1,    46,   178,   419,   177,   419,   177,    80,    -1,
      50,   178,   419,   177,   419,   177,   185,    -1,    51,   178,
     419,   177,   419,   342,    -1,    52,   178,   419,   177,   419,
      -1,    -1,   177,    80,    -1,    58,   178,   410,   177,    84,
      -1,    59,   178,   410,    -1,    60,   178,   389,   177,   152,
     178,   389,   177,    82,   177,    81,    -1,    70,   178,   389,
     177,   149,   347,    -1,    -1,   347,   177,   348,    -1,    81,
      -1,   145,   178,   396,    -1,   146,   178,   398,    -1,   147,
     178,   352,    -1,   148,   178,   352,    -1,   150,    -1,   151,
      -1,   152,   178,   389,    -1,   153,   178,   372,    -1,   114,
     178,   349,    -1,    88,   178,   380,    -1,   154,   178,     4,
      -1,    98,   178,     4,    -1,     4,    -1,    82,    -1,   115,
      -1,    84,    -1,    62,   178,   410,    -1,    61,   178,   410,
      -1,     3,    -1,   179,     3,   177,   382,   180,    -1,   113,
     178,   392,   177,   394,   177,   394,   177,     7,    -1,   111,
     178,   410,   177,   394,    -1,    47,   178,   392,    -1,    48,
     178,   392,    -1,    37,   178,   185,   177,    38,    -1,    37,
     178,   185,   177,    38,   177,   145,   178,   396,    -1,    34,
     178,   392,   177,    36,    -1,    35,   178,   419,   177,    36,
      -1,    -1,    33,   178,   392,   177,   382,   177,   373,   374,
     377,   379,   361,   362,    -1,    -1,   255,    -1,    42,   178,
     389,   177,   383,   177,   385,    -1,    43,   178,   389,    -1,
      89,   178,   410,   177,    90,   177,    87,    -1,   106,   178,
     410,   177,     4,    -1,    44,   178,    83,   177,   389,    -1,
     155,   178,   389,   177,   400,    -1,   155,   178,   389,   177,
      72,    -1,   155,   178,   389,    -1,    56,   178,   401,   177,
     389,    -1,    57,   178,   389,   177,   418,   177,   400,    -1,
      54,   178,   388,   177,   388,   177,   388,   177,   388,    -1,
      54,    -1,   185,    -1,    72,    -1,   185,    -1,    72,    -1,
      -1,   177,   375,    -1,   376,    -1,   376,   177,   375,    -1,
      39,    -1,    40,    -1,    41,    -1,    -1,    91,   178,   378,
      -1,    90,    -1,    -1,    88,   178,   380,    -1,    87,    -1,
      65,    -1,    72,    -1,    66,    -1,    72,    -1,    75,    -1,
     386,    -1,    72,    -1,    75,    -1,   386,    -1,    74,   178,
      72,    -1,    79,    -1,    72,    -1,    74,   181,     4,   182,
      -1,   185,    -1,   196,    -1,   197,   181,   401,   182,    -1,
     401,    -1,   390,    -1,   168,   179,   410,   180,    -1,   200,
      -1,   201,   181,   401,   182,    -1,   179,     4,   177,     4,
     180,    -1,    72,    -1,    73,   391,   182,    -1,   176,    -1,
     176,   177,   391,    -1,   393,    -1,   202,    -1,   203,   181,
     401,   182,    -1,   179,     4,   177,     4,   177,     4,   177,
       4,   180,    -1,   395,    -1,   204,    -1,   205,   181,   401,
     182,    -1,     3,    -1,   179,     3,   177,   382,   180,    -1,
     397,    -1,   206,    -1,   207,   181,   401,   182,    -1,   185,
      -1,     3,    -1,   179,     3,   177,   185,   180,    -1,    72,
      -1,   399,    -1,   208,    -1,   209,   181,   401,   182,    -1,
     185,    -1,     3,    -1,   179,     3,   177,   185,   180,    -1,
      72,    -1,    85,    -1,   387,    -1,   400,   237,   387,    -1,
       4,    -1,   411,    -1,   179,     8,   180,    -1,   194,    -1,
     195,   181,   401,   182,    -1,   401,   231,   401,    -1,   401,
     232,   401,    -1,   401,   233,   401,    -1,   401,   234,   401,
      -1,   401,   235,   401,    -1,   179,   401,   180,    -1,   190,
      -1,   191,    -1,   259,   178,   402,    -1,   403,    -1,   404,
     177,   403,    -1,    -1,   404,    -1,   401,    -1,   400,    -1,
     406,    -1,   407,   177,   406,    -1,    -1,   407,    -1,   389,
      -1,   161,   392,    -1,   162,   392,    -1,   163,   389,   177,
     389,    -1,   164,   389,   177,   389,   177,   401,    -1,   165,
     179,   410,   180,    -1,   165,   179,   314,   177,   410,   180,
      -1,   171,   179,     7,   177,   410,   180,    -1,   171,   179,
     410,   177,   410,   180,    -1,   171,   179,   394,   177,   410,
     180,    -1,   167,   389,    -1,   169,   179,   389,   177,   401,
     180,    -1,   169,   179,   389,   177,   401,   177,    39,   180,
      -1,   170,   179,   389,   177,   401,   177,   401,   180,    -1,
     170,   179,   389,   177,   401,   177,   401,   177,    39,   180,
      -1,   173,   179,   174,   177,   179,   401,   232,   401,   248,
     180,   177,   389,   249,   180,    -1,   172,   409,    -1,   210,
      -1,   179,   410,   180,    -1,   409,    -1,   409,   239,   410,
      -1,   230,    -1,    86,    -1,     4,    -1,     8,    -1,     9,
      -1,     4,    -1,     8,    -1,     9,    -1,     4,    -1,   411,
      -1,    28,    -1,    22,    -1,    29,    -1,    24,    -1,    30,
      -1,   111,    -1,    68,    -1,    72,    -1,   420,    -1,    53,
     179,     4,   177,     4,   177,     4,   177,     4,   180,    -1,
     179,     4,   177,     4,   177,     4,   177,     4,   180,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   283,   283,   284,   287,   288,   291,   314,   319,   341,
     354,   366,   372,   401,   407,   414,   417,   424,   428,   435,
     438,   445,   446,   450,   453,   460,   464,   471,   474,   480,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
     516,   517,   518,   519,   520,   521,   522,   523,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   538,   539,   540,   541,   544,   545,   546,   547,
     548,   549,   550,   551,   552,   555,   556,   557,   558,   559,
     560,   561,   562,   563,   566,   567,   568,   571,   572,   575,
     591,   597,   603,   609,   615,   621,   627,   633,   639,   649,
     659,   669,   679,   689,   699,   711,   716,   723,   728,   735,
     740,   747,   751,   757,   762,   769,   773,   779,   783,   790,
     812,   789,   826,   881,   888,   891,   897,   904,   908,   917,
     921,   916,   984,   985,   989,   988,  1002,  1001,  1016,  1026,
    1027,  1030,  1068,  1067,  1102,  1101,  1132,  1131,  1164,  1163,
    1189,  1200,  1199,  1227,  1233,  1238,  1243,  1250,  1257,  1266,
    1274,  1286,  1285,  1309,  1308,  1330,  1333,  1339,  1349,  1355,
    1364,  1370,  1375,  1381,  1386,  1392,  1403,  1409,  1410,  1413,
    1414,  1417,  1421,  1427,  1428,  1432,  1438,  1446,  1451,  1456,
    1464,  1471,  1479,  1487,  1488,  1491,  1492,  1495,  1500,  1499,
    1513,  1520,  1527,  1535,  1540,  1546,  1552,  1558,  1564,  1569,
    1574,  1579,  1584,  1589,  1594,  1599,  1604,  1609,  1614,  1620,
    1627,  1636,  1640,  1653,  1662,  1661,  1679,  1689,  1695,  1703,
    1709,  1714,  1719,  1724,  1729,  1734,  1739,  1744,  1749,  1760,
    1766,  1771,  1776,  1781,  1786,  1791,  1796,  1801,  1806,  1811,
    1816,  1821,  1828,  1834,  1863,  1868,  1876,  1882,  1888,  1895,
    1902,  1912,  1922,  1937,  1948,  1951,  1957,  1963,  1969,  1975,
    1982,  1988,  1996,  2002,  2007,  2012,  2017,  2022,  2028,  2034,
    2039,  2044,  2049,  2054,  2059,  2066,  2066,  2066,  2066,  2069,
    2075,  2081,  2086,  2093,  2100,  2106,  2112,  2118,  2122,  2128,
    2134,  2145,  2144,  2176,  2179,  2185,  2192,  2198,  2204,  2210,
    2216,  2220,  2225,  2232,  2238,  2245,  2249,  2256,  2264,  2267,
    2277,  2281,  2284,  2290,  2294,  2301,  2305,  2309,  2316,  2319,
    2325,  2332,  2335,  2341,  2347,  2348,  2351,  2352,  2355,  2356,
    2357,  2363,  2364,  2365,  2371,  2372,  2375,  2384,  2389,  2396,
    2407,  2413,  2417,  2421,  2428,  2438,  2445,  2449,  2455,  2459,
    2467,  2471,  2478,  2488,  2501,  2505,  2512,  2522,  2531,  2542,
    2546,  2553,  2563,  2574,  2583,  2593,  2599,  2603,  2610,  2620,
    2631,  2640,  2650,  2654,  2661,  2662,  2668,  2672,  2676,  2680,
    2688,  2697,  2701,  2705,  2709,  2713,  2717,  2720,  2727,  2736,
    2769,  2770,  2773,  2774,  2777,  2781,  2788,  2795,  2806,  2809,
    2817,  2821,  2825,  2829,  2833,  2838,  2842,  2846,  2851,  2856,
    2861,  2865,  2870,  2875,  2879,  2883,  2888,  2892,  2899,  2905,
    2909,  2915,  2922,  2923,  2926,  2927,  2928,  2931,  2935,  2939,
    2943,  2949,  2950,  2953,  2954,  2957,  2958,  2961,  2962,  2965,
    2969,  2995
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
  "BOUNDARY_TYPE_ID", "OBJECT_ID", "COBJECT_ID", "MONSTER_ID", "TRAP_ID",
  "DOOR_ID", "DRAWBRIDGE_ID", "object_ID", "monster_ID", "terrain_ID",
  "MAZEWALK_ID", "WALLIFY_ID", "REGION_ID", "SPECIAL_REGION_ID",
  "SPECIAL_LEVREGION_ID", "SPECIAL_REGION_TYPE", "NAMING_ID",
  "NAMING_TYPE", "FILLING", "IRREGULAR", "JOINED", "ALTAR_ID", "ANVIL_ID",
  "NPC_ID", "LADDER_ID", "STAIR_ID", "NON_DIGGABLE_ID", "NON_PASSWALL_ID",
  "ROOM_ID", "PORTAL_ID", "TELEPRT_ID", "BRANCH_ID", "LEV",
  "MINERALIZE_ID", "CORRIDOR_ID", "GOLD_ID", "ENGRAVING_ID", "FOUNTAIN_ID",
  "THRONE_ID", "MODRON_PORTAL_ID", "POOL_ID", "SINK_ID", "NONE",
  "RAND_CORRIDOR_ID", "DOOR_STATE", "LIGHT_STATE", "CURSE_TYPE",
  "ENGRAVING_TYPE", "KEYTYPE_ID", "LEVER_ID", "DIRECTION", "RANDOM_TYPE",
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
  "RECHARGED_ID", "INVIS_ID", "GREASED_ID", "INDESTRUCTIBLE_ID",
  "FEMALE_ID", "WAITFORU_ID", "CANCELLED_ID", "REVIVED_ID", "AVENGE_ID",
  "FLEEING_ID", "BLINDED_ID", "PARALYZED_ID", "STUNNED_ID", "CONFUSED_ID",
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
  "tileset_detail", "boundary_type_detail", "opt_limited",
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
  "secret", "door_wall", "dir_list", "door_pos", "door_infos", "door_info",
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
     425,   426,   427,   428,   429,   430,   431,    44,    58,    40,
      41,    91,    93,   123,   125,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,    43,    45,    42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   240,   241,   241,   242,   242,   243,   244,   244,   245,
     245,   245,   245,   246,   247,   248,   248,   249,   249,   250,
     250,   251,   251,   252,   252,   253,   253,   254,   254,   255,
     256,   256,   256,   256,   256,   256,   256,   256,   256,   256,
     256,   256,   256,   256,   256,   256,   256,   256,   256,   256,
     256,   256,   256,   256,   256,   256,   256,   256,   256,   256,
     256,   256,   256,   256,   256,   256,   256,   256,   256,   256,
     256,   256,   256,   256,   256,   256,   256,   256,   256,   256,
     256,   256,   256,   256,   256,   256,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   258,   258,   258,   258,   258,
     258,   258,   258,   258,   259,   259,   259,   260,   260,   261,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   263,   263,   264,   264,   265,
     265,   266,   266,   267,   267,   268,   268,   269,   269,   271,
     272,   270,   273,   274,   275,   275,   276,   276,   276,   278,
     279,   277,   280,   280,   282,   281,   283,   281,   284,   285,
     285,   286,   288,   287,   290,   289,   292,   291,   294,   293,
     295,   296,   295,   297,   298,   298,   298,   299,   299,   300,
     301,   303,   302,   305,   304,   306,   306,   307,   307,   308,
     308,   309,   309,   310,   310,   311,   311,   312,   312,   313,
     313,   314,   314,   315,   315,   316,   316,   317,   317,   317,
     318,   318,   318,   319,   319,   320,   320,   321,   322,   321,
     323,   324,   324,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     326,   326,   326,   327,   328,   327,   329,   330,   330,   331,
     331,   331,   331,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   332,   333,   334,   334,   335,   335,   336,   337,
     338,   339,   340,   341,   342,   342,   343,   344,   345,   346,
     347,   347,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   349,   349,   349,   349,   350,
     351,   352,   352,   353,   354,   355,   356,   357,   357,   358,
     359,   361,   360,   362,   362,   363,   364,   365,   366,   367,
     368,   368,   368,   369,   370,   371,   371,   372,   372,   373,
     373,   374,   374,   375,   375,   376,   376,   376,   377,   377,
     378,   379,   379,   380,   381,   381,   382,   382,   383,   383,
     383,   384,   384,   384,   385,   385,   386,   387,   387,   387,
     388,   389,   389,   389,   389,   390,   390,   390,   391,   391,
     392,   392,   392,   393,   394,   394,   394,   395,   395,   396,
     396,   396,   397,   397,   397,   397,   398,   398,   398,   399,
     399,   399,   399,   399,   400,   400,   401,   401,   401,   401,
     401,   401,   401,   401,   401,   401,   401,   402,   402,   403,
     404,   404,   405,   405,   406,   406,   407,   407,   408,   408,
     409,   409,   409,   409,   409,   409,   409,   409,   409,   409,
     409,   409,   409,   409,   409,   409,   409,   409,   409,   410,
     410,   411,   412,   412,   413,   413,   413,   414,   414,   414,
     414,   415,   415,   416,   416,   417,   417,   418,   418,   419,
     419,   420
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     3,     3,     5,     5,
       5,     3,    16,     3,     3,     0,     2,     0,     2,     0,
       2,     1,     1,     0,     3,     3,     1,     0,     2,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     5,     3,     5,     5,     5,     3,     3,     5,     5,
       5,     7,     7,     7,     5,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     0,
       0,     8,     4,     1,     0,     1,     1,     5,     3,     0,
       0,     9,     0,     2,     0,     5,     0,     4,     1,     2,
       1,     6,     0,     3,     0,     6,     0,     4,     0,     4,
       1,     0,     4,     3,     1,     3,     3,     5,     5,     7,
       4,     0,    12,     0,    14,     0,     2,     5,     1,     5,
       1,     5,     1,     5,     1,    10,     6,     1,     1,     1,
       1,     1,     3,     1,     1,     0,     3,     3,     3,     3,
       1,     7,     5,     1,     1,     1,     1,     3,     0,     5,
       4,     0,     3,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     3,     3,     3,     1,     1,     3,     1,
       1,     1,     3,     3,     0,     5,     2,     0,     3,     1,
       3,     1,     3,     3,     1,     1,     3,     1,     1,     1,
       3,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     1,     5,     7,     5,     8,     1,     3,     5,     5,
       7,     7,     6,     5,     0,     2,     5,     3,    11,     6,
       0,     3,     1,     3,     3,     3,     3,     1,     1,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     3,
       3,     1,     5,     9,     5,     3,     3,     5,     9,     5,
       5,     0,    12,     0,     1,     7,     3,     7,     5,     5,
       5,     5,     3,     5,     7,     9,     1,     1,     1,     1,
       1,     0,     2,     1,     3,     1,     1,     1,     0,     3,
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
       2,     0,     0,     0,     3,     4,    23,     0,     0,     1,
       5,     0,    27,     0,     7,     0,   146,     0,     0,     0,
       0,   210,     0,     0,     0,     0,     0,     0,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   336,     0,     0,     0,
       0,     0,     0,     0,     0,   174,     0,     0,     0,     0,
       0,     0,     0,     0,   143,     0,     0,     0,   149,   158,
       0,     0,     0,     0,   106,    95,    86,    96,    87,    97,
      88,    98,    89,    99,    90,   100,    91,   101,    92,   102,
      93,   103,    94,    31,    32,    33,     6,    27,   104,   105,
       0,    43,    42,    60,    61,    58,     0,    53,    59,   162,
      54,    55,    57,    56,    30,    71,    41,    77,    76,    45,
      64,    66,    67,    84,    46,    65,    85,    62,    81,    82,
      70,    83,    40,    49,    50,    51,    63,    78,    69,    80,
      79,    44,    68,    72,    73,    74,    75,    36,    37,    35,
      34,    38,    39,    52,    47,    48,     0,    26,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   168,     0,     0,     0,     0,   107,   108,     0,
       0,     0,     0,   406,     0,   409,     0,   451,     0,   407,
     428,    28,     0,   166,     0,     8,     0,   367,   368,     0,
     404,   173,     0,     0,     0,    11,   453,   452,    13,   376,
       0,   213,   214,     0,     0,   373,     0,     0,   185,   371,
      14,   400,   402,   403,     0,   399,   397,     0,   243,   247,
     396,   244,   393,   395,     0,   392,   390,     0,   217,     0,
     389,   338,   337,     0,   354,   355,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   447,   430,   449,   277,     0,   381,     0,     0,   380,
       0,     0,     0,     0,   469,     0,     0,   326,     0,     0,
       0,     0,     0,   315,   316,   340,   339,     0,   144,     0,
       0,     0,     0,   370,     0,     0,     0,     0,     0,   287,
       0,   310,   309,   456,   454,   455,   176,   175,     0,     0,
       0,     0,   197,   198,     0,     0,     0,     0,   109,     0,
       0,     0,   332,   139,     0,     0,     0,     0,   148,     0,
       0,     0,     0,     0,   425,   424,   426,   429,     0,   462,
     464,   461,   463,   465,   466,     0,     0,     0,   116,   117,
     112,   110,     0,     0,     0,     0,    27,   163,    25,     0,
       0,     0,     0,     0,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   246,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   431,   432,     0,     0,     0,   440,
       0,     0,     0,   446,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   170,
     169,     0,     0,   164,     0,     0,     0,   422,   408,   416,
       0,     0,   411,   412,   413,   414,   415,     0,   142,     0,
     406,     0,     0,     0,     0,   133,   131,   137,   135,     0,
       0,     0,   167,     0,     0,   405,    10,   311,     0,     9,
       0,     0,   377,     0,     0,     0,   216,   215,   185,   186,
     212,     0,     0,     0,   245,     0,     0,   219,   221,   272,
     205,     0,   274,     0,     0,   201,     0,     0,     0,     0,
     387,     0,     0,   385,     0,     0,   384,     0,     0,   448,
     450,     0,     0,   356,   357,     0,   319,     0,     0,   320,
     317,   360,     0,   358,     0,   359,   329,   278,     0,   279,
       0,   188,     0,     0,     0,     0,   284,   283,     0,     0,
     177,   178,   333,   467,   468,     0,   286,     0,   290,     0,
     190,     0,     0,   328,     0,     0,     0,   314,     0,     0,
     160,     0,     0,   150,   331,   330,     0,   420,   423,     0,
     410,   147,   427,   111,     0,     0,   120,     0,   119,     0,
     118,     0,   124,     0,   115,     0,   114,     0,   113,    29,
     369,     0,     0,   379,   372,     0,   374,     0,     0,   398,
     459,   457,   458,   258,   255,   249,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   254,     0,   259,     0,   261,
     262,   271,     0,   257,   248,   263,   460,   251,     0,   391,
     220,   196,     0,     0,   433,     0,     0,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   382,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   180,
       0,     0,   282,     0,     0,     0,     0,   289,     0,     0,
       0,     0,     0,     0,   165,   159,   161,     0,     0,     0,
     140,     0,   132,   134,   136,   138,     0,   125,     0,   127,
       0,   129,     0,     0,   375,   211,   401,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   394,
       0,     0,   273,    19,     0,   202,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   341,     0,     0,     0,
       0,   365,   364,   325,   280,     0,   192,     0,     0,   281,
     285,     0,     0,   334,     0,     0,   327,     0,   194,     0,
     341,   200,     0,   199,   172,     0,   152,   417,   418,   419,
     421,     0,     0,   123,     0,   122,     0,   121,     0,     0,
     270,   265,   264,   266,   267,   268,   269,   252,   253,   256,
     260,   250,     0,   361,   224,   225,     0,   229,   228,   239,
     230,   231,   232,     0,     0,     0,   236,   237,     0,   222,
     226,   362,   223,     0,     0,     0,   206,     0,   275,   434,
     436,     0,   441,     0,   437,     0,   386,   439,   438,     0,
       0,     0,   348,     0,     0,     0,   366,     0,     0,     0,
       0,   203,   204,     0,     0,   292,     0,     0,     0,     0,
       0,     0,     0,   297,   298,     0,     0,     0,   291,     0,
       0,   348,     0,     0,     0,     0,     0,   152,   141,   126,
     128,   130,   312,     0,     0,   227,     0,     0,     0,     0,
       0,     0,     0,    20,     0,     0,   388,     0,     0,   345,
     346,   347,   342,   343,     0,   351,     0,     0,   318,   187,
       0,   341,   335,   179,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   189,     0,   351,   205,   313,
       0,   156,   151,   153,     0,   363,   233,   234,   235,   241,
     240,   238,   208,   209,   207,   442,     0,   443,   412,     0,
       0,     0,     0,   321,     0,     0,     0,   348,     0,   353,
     302,   304,   305,   306,   308,   307,   301,   293,   294,   295,
     296,   299,   300,   303,     0,   181,   195,   154,    27,     0,
       0,     0,     0,     0,   383,   344,   350,   349,     0,   323,
       0,   471,   191,   351,   288,   193,     0,    27,   157,     0,
     242,   444,    16,     0,   352,   324,   322,   470,   183,   182,
     155,     0,     0,     0,     0,    17,   184,    21,    22,    19,
       0,     0,    12,    18,   445
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    93,    94,    95,   993,  1031,
     838,  1029,    12,   158,    96,   387,    97,    98,    99,   100,
     209,   101,   102,   716,   718,   720,   481,   482,   483,   484,
     103,   467,   791,   104,   105,   443,   106,   107,   210,   707,
     886,   887,  1007,   988,   108,   592,   109,   110,   224,   111,
     589,   112,   385,   113,   345,   460,   585,   114,   115,   116,
     325,   317,   117,  1006,   118,  1023,   401,   563,   582,   768,
     780,   119,   344,   782,   526,   863,   661,   836,   120,   247,
     508,   121,   408,   268,   660,   829,   951,   122,   405,   258,
     404,   654,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   692,   133,   134,   135,   136,   697,   878,   976,
     137,   138,   499,   139,   140,   141,   142,   143,   144,   145,
     146,   999,  1016,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   273,   318,   852,   912,   913,   915,   997,   963,
     970,   276,   545,   554,   830,   763,   555,   230,   322,   292,
     249,   395,   298,   299,   535,   536,   269,   270,   259,   260,
     364,   323,   789,   597,   598,   599,   366,   367,   368,   293,
     425,   219,   238,   337,   657,   382,   383,   384,   575,   303,
     304
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -857
static const yytype_int16 yypact[] =
{
     320,   -55,   -10,   129,  -857,   320,    -2,    14,    27,  -857,
    -857,    43,   776,    46,  -857,   126,  -857,    61,    89,   108,
     121,  -857,   127,   144,   169,   171,   176,   181,   190,   213,
     218,   220,   229,   233,   235,   236,   237,   239,   241,   242,
     243,   249,   251,   252,   253,   263,   265,   269,   270,   274,
     275,   276,   277,   279,   281,   289,   307,   309,   311,   313,
     314,    50,   325,   329,  -857,   332,    67,   480,  -857,  -857,
     334,   132,    80,   247,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,   776,  -857,  -857,
     232,  -857,  -857,  -857,  -857,  -857,   335,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,   483,   327,  -857,   -78,
     321,   101,   -12,   424,   142,   142,   100,   -15,    62,     1,
       1,   744,   -77,   -77,   -22,   331,     1,     1,   434,     1,
      -3,   -77,   -77,     0,   -22,   -22,   -22,    80,   341,    80,
       1,   744,   744,     1,   744,   744,   107,     1,   744,     0,
     744,    87,  -857,   744,   -77,   562,    80,  -857,  -857,   286,
     342,     1,   345,  -857,    51,  -857,   348,  -857,   -81,  -857,
      41,  -857,   436,  -857,   351,  -857,   126,  -857,  -857,   356,
    -857,   312,   369,   371,   379,  -857,  -857,  -857,  -857,  -857,
     399,  -857,  -857,   397,   573,  -857,   400,   401,   403,  -857,
    -857,  -857,  -857,  -857,   581,  -857,  -857,   405,  -857,  -857,
    -857,  -857,  -857,  -857,   584,  -857,  -857,   407,   406,   416,
    -857,  -857,  -857,   419,  -857,  -857,   421,   422,   426,   -77,
     -77,     1,     1,   427,     1,   430,   433,   437,   744,   439,
     579,  -857,  -857,   343,  -857,   597,  -857,   432,   428,  -857,
     440,   443,   619,   447,  -857,   451,   452,  -857,   457,   458,
     636,   476,   477,  -857,  -857,  -857,  -857,   479,   652,   482,
     485,   486,   488,   333,   663,   498,   262,   500,   506,  -857,
     510,  -857,  -857,  -857,  -857,  -857,  -857,  -857,   512,   517,
     524,   557,  -857,  -857,   560,   351,   561,   576,  -857,   538,
      80,    80,   583,  -857,   559,   310,    80,    80,  -857,    80,
      80,    80,    80,    80,   312,   333,  -857,   585,   586,  -857,
    -857,  -857,  -857,  -857,  -857,   578,    78,    21,  -857,  -857,
     312,   333,   590,   592,   603,   776,   776,  -857,  -857,    80,
     -78,   780,    29,   782,   609,   605,   744,   611,    80,   264,
     786,   620,   628,    80,   635,   351,   637,    80,   351,     1,
       1,   744,   758,   768,  -857,  -857,   638,   670,   554,  -857,
       1,     1,   390,  -857,   675,   671,   744,   673,    80,    42,
     816,   854,   682,   824,   823,   221,     1,   783,   687,   787,
     -22,    -1,  -857,   689,   -22,   -22,   -22,    80,   691,    45,
       1,    70,   785,   719,   723,   784,     7,   869,    62,   765,
    -857,   133,   133,  -857,   155,   695,   -14,   817,  -857,  -857,
     339,   410,   123,   123,  -857,  -857,  -857,    41,  -857,   744,
     701,   -71,   -49,   -45,   -36,  -857,  -857,   312,   333,    24,
     109,   141,  -857,   696,   415,  -857,  -857,  -857,   876,  -857,
     704,   399,  -857,   703,   882,   438,  -857,  -857,   403,  -857,
    -857,   705,   478,   305,  -857,   709,   497,  -857,  -857,  -857,
    -857,   711,   718,     1,     1,   660,   725,   720,   726,   727,
    -857,   741,   535,  -857,   738,   743,  -857,   745,   747,  -857,
    -857,   917,   610,  -857,  -857,   748,  -857,   749,   923,  -857,
     751,  -857,   753,  -857,   752,  -857,  -857,  -857,   926,  -857,
     755,  -857,   933,   761,    42,   762,   763,  -857,   764,   871,
    -857,  -857,  -857,  -857,  -857,   766,  -857,   769,  -857,   771,
    -857,   942,   772,  -857,   773,   841,   949,  -857,   778,   351,
    -857,   716,    80,  -857,  -857,   312,   781,  -857,   779,   789,
    -857,  -857,  -857,  -857,   954,   788,  -857,    15,  -857,    80,
    -857,   -78,  -857,    25,  -857,    26,  -857,    49,  -857,  -857,
    -857,   811,   957,  -857,  -857,   809,  -857,   775,   810,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,   814,   815,   828,   829,
     830,   831,   832,   851,   852,  -857,   853,  -857,   855,  -857,
    -857,  -857,   856,  -857,  -857,  -857,  -857,  -857,   857,  -857,
     858,   859,    62,  1027,  -857,   862,   891,   744,  -857,    80,
      80,   744,   863,    80,   744,   744,   864,   865,  -857,     0,
    1034,   867,   896,  1041,    94,   -40,   967,   872,    11,  -857,
     866,   968,  -857,    80,   873,   -78,     1,   875,   966,   877,
      17,   290,   351,   133,  -857,  -857,   333,   878,   185,   817,
    -857,   149,  -857,  -857,   333,   312,    57,  -857,    81,  -857,
     103,  -857,    42,   879,  -857,  -857,  -857,   142,   962,   970,
      80,    80,    80,    80,   -78,    80,    80,    80,   100,  -857,
     861,    35,  -857,   883,    80,  -857,   884,   148,   267,   885,
      42,   622,   886,   887,    80,  1055,   892,   893,  1059,   890,
     889,  -857,  -857,  -857,  -857,  1068,  -857,   308,   897,  -857,
    -857,   898,    71,   312,   899,   324,  -857,  1069,  -857,  1073,
     892,  -857,   901,  -857,  -857,   902,    69,  -857,  -857,  -857,
    -857,   351,    25,  -857,    26,  -857,    49,  -857,   900,  1076,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,   312,  -857,  -857,
    -857,  -857,   162,  -857,  -857,  -857,   -78,  -857,  -857,  -857,
    -857,  -857,  -857,   904,   905,   906,  -857,  -857,   907,  -857,
    -857,  -857,   312,   908,   909,   910,  -857,  1086,  -857,   333,
    -857,  1051,  -857,    80,  -857,   911,  -857,  -857,  -857,   543,
     915,   326,  1002,  1090,   918,   100,  -857,   916,   920,    17,
      80,  -857,  -857,   919,  1016,  -857,   922,   924,   925,   927,
     928,   929,   930,  -857,  -857,   931,   932,   934,  -857,   921,
     936,  1002,    71,  1097,   260,   937,   935,    69,  -857,  -857,
    -857,  -857,  -857,   939,  1039,   312,    80,    80,    80,   -54,
     142,  1110,  1003,  -857,   940,   248,  -857,    80,  1113,  -857,
    -857,  -857,  -857,   944,   945,  1036,   948,  1118,  -857,  -857,
     264,   892,  -857,  -857,   950,  1042,  1122,    76,   100,   142,
      29,    29,     1,   -15,  1124,  -857,  1126,  1036,  -857,  -857,
     953,  -857,  -857,  -857,  1127,  -857,  -857,  -857,  -857,  -857,
     895,  -857,  -857,  -857,  -857,  -857,  1095,  -857,   175,   955,
     326,  1046,   959,  -857,  1134,   960,   961,  1002,  1058,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,   963,  -857,   859,  -857,   776,   965,
     -54,   964,   971,   969,  -857,  -857,  -857,  -857,  1042,   351,
     972,  -857,  -857,  1036,  -857,  -857,   351,   776,  -857,    42,
    -857,  -857,  -857,   973,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,   974,     1,   351,    90,   976,  -857,  -857,  -857,   883,
       1,   975,  -857,  -857,  -857
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -857,  -857,  1140,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
     118,  -857,  -857,   938,   -97,  -342,   774,   943,  1087,  -446,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  1096,  -857,  -857,  -857,
     271,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
     707,   977,  -857,  -857,  -857,  -857,   653,  -857,  -857,  -857,
     301,  -857,  -857,  -857,  -581,   280,   225,  -857,  -857,   398,
     246,  -857,  -857,  -857,  -857,  -857,   177,  -857,  -857,  1004,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -536,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,   238,   489,  -737,   210,  -857,  -790,  -857,  -856,
     174,  -410,  -558,  -857,  -857,  -857,   435,   790,  -205,  -160,
    -363,   672,  -105,  -351,  -450,  -583,  -482,  -575,  -484,  -567,
    -155,   -59,  -857,   465,  -857,  -857,   700,  -857,  -857,   894,
    -156,   665,  -857,  -442,  -857,  -857,  -857,  -857,  -857,  -162,
    -857
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -219
static const yytype_int16 yytable[] =
{
     221,   349,   248,   459,   231,   614,   689,   571,   616,   277,
     278,   587,   588,   218,   485,   294,   306,   307,   312,   309,
     311,   596,   319,   320,   321,   213,   486,   251,   251,   262,
     327,   301,   497,   330,   721,   328,   329,   338,   331,   332,
     719,   618,   339,   881,   341,   213,   717,   346,   584,   333,
     301,   352,   530,   334,   335,   213,    16,   271,   594,   354,
     239,   240,   378,   514,   241,   242,   517,   380,   300,   239,
     240,   561,   315,   239,   240,   861,   313,   314,   357,   580,
     972,   985,   480,   766,   213,   745,   354,   239,   240,   778,
     949,   937,   342,   239,   240,  1027,   252,   252,   263,   347,
      11,   358,   295,   262,   833,   236,   605,   227,   543,   253,
     253,   333,   262,   606,   544,   334,   335,   379,   228,   229,
     783,   416,   417,     7,   419,   296,   297,   274,   607,     9,
     326,   950,   609,   834,   275,   608,   530,   758,   573,   610,
     724,   611,   574,   862,   530,   251,   465,  1018,   612,   835,
     359,   360,   361,   362,   363,   355,   243,   302,   973,   343,
     974,   365,  1028,   381,   798,   243,   761,   244,     8,   243,
     272,   227,   263,   762,   414,   415,   310,  1003,   562,   336,
     244,   263,   228,   229,   967,   316,   581,   237,   245,   246,
     767,   975,   845,   884,   244,   885,   779,   245,   246,    13,
     376,   245,   246,   254,   254,   264,   227,   613,   498,   255,
     255,   265,    14,   891,   252,   215,   216,   228,   229,   890,
     214,    15,   487,   156,   324,   889,   227,   253,   586,   157,
     214,    72,   256,   257,   792,   215,   216,   228,   229,   159,
     503,   793,   568,   800,   713,   215,   216,   704,   206,   518,
     519,   217,   742,   785,   712,   520,   811,   214,   794,   214,
     528,   529,   527,   596,   333,   795,   537,   160,   334,   335,
     540,   217,   215,   216,   215,   216,   556,   590,   560,   264,
     796,   217,   565,   566,   567,   265,   161,   797,   264,   493,
     572,   464,   615,   551,   265,   552,   553,   470,   471,   162,
     472,   473,   474,   475,   476,   163,   266,   267,   217,   630,
     217,   595,   586,   631,   632,   266,   267,   355,   488,   212,
     586,   254,   164,   603,   617,   841,   755,   255,   842,   724,
     494,   232,   233,   234,   235,     1,     2,   533,   534,   505,
     894,   506,   507,   683,   512,   533,   534,   165,   516,   166,
     256,   257,   992,   655,   167,   -15,   361,   362,   363,   168,
     784,   525,   781,   664,   665,   909,   910,   911,   169,   542,
     633,   634,   635,   918,   636,   787,   788,   239,   240,   359,
     360,   361,   362,   363,   241,   242,   359,   360,   361,   362,
     363,   170,   591,   530,   979,   980,   171,   531,   172,   637,
     638,   639,   640,   641,   642,   865,   643,   173,   361,   362,
     363,   174,   866,   175,   176,   177,   952,   178,   365,   179,
     180,   181,   867,   644,   645,   956,   220,   182,   957,   183,
     184,   185,   646,   647,   648,   649,   650,   651,   868,   450,
     213,   186,   940,   187,   843,   978,   977,   188,   189,   888,
     652,  1021,   190,   191,   192,   193,   715,   194,   369,   195,
     370,   653,   239,   240,   371,   372,   373,   196,   222,   869,
     870,   871,   872,   243,   873,   874,   875,   876,   877,   359,
     360,   361,   362,   363,   244,   197,   225,   198,   771,   199,
     469,   200,   201,   359,   360,   361,   362,   363,   359,   360,
     361,   362,   363,   203,   226,   245,   246,   204,   239,   240,
     205,   746,   211,   223,   250,   749,   305,   308,   752,   753,
     324,   600,   350,   351,   353,   803,   804,   805,   806,   356,
     808,   809,   810,   706,   386,   217,   774,   389,   672,   397,
     773,   359,   360,   361,   362,   363,   391,   374,   392,   390,
     714,   279,   280,   281,   282,   283,   393,   284,   243,   285,
     286,   287,   288,   289,   359,   360,   361,   362,   363,   532,
     359,   360,   361,   362,   363,   394,   396,   397,   399,   807,
     400,   398,   426,   397,   402,   832,   403,   406,   407,  -218,
     245,   246,   601,   409,   533,   534,   410,   620,   411,   412,
     291,   427,   375,   413,   243,   429,   418,   239,   240,   420,
     747,   748,   421,   428,   751,   376,   422,   430,   424,   377,
     626,   227,   431,   432,   433,   525,   239,   240,   434,   435,
     215,   216,   228,   229,   436,   437,   245,   246,   296,   297,
     438,   359,   360,   361,   362,   363,   359,   360,   361,   362,
     363,   239,   240,   439,   440,   922,   441,  1015,   442,   444,
     629,   895,   445,   446,  1019,   447,   217,   448,   207,   359,
     360,   361,   362,   363,    75,   449,    77,   451,    79,   659,
      81,  1026,    83,   452,    85,   839,    87,   453,    89,   454,
      91,   946,   947,   948,   455,   849,   279,   280,   281,   282,
     283,   456,   284,   243,   285,   286,   287,   288,   289,   359,
     360,   361,   362,   363,   290,   279,   280,   281,   282,   283,
     463,   284,   243,   285,   286,   287,   288,   289,   359,   360,
     361,   362,   363,   290,   457,   245,   246,   458,   461,   468,
     279,   280,   281,   282,   283,   291,   284,   243,   285,   286,
     287,   288,   289,   462,   245,   246,   479,    76,   290,    78,
     466,    80,   477,    82,   291,    84,   478,    86,   489,    88,
     490,    90,   981,    92,   359,   907,   361,   362,   363,   245,
     246,   491,    16,   496,   905,   500,   501,   502,   504,   291,
      17,   509,   678,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,   846,   511,   510,    29,    30,    31,
      32,    33,   513,    34,   515,   523,   239,   240,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,   521,
      46,    47,    48,    49,    50,    51,    52,    53,    54,   522,
      55,   359,   360,   361,   362,   363,    56,   524,   958,   538,
     541,   539,   546,   359,   360,   361,   362,   363,   547,   548,
     549,   550,  1025,   557,   558,    57,   564,   559,   569,   576,
    1033,   577,   578,   583,   579,  -171,    58,   593,   604,   621,
     619,   622,    59,   624,    60,    61,   625,    62,   662,    63,
     628,  1008,    64,    65,   658,   663,    66,    67,   666,    68,
     668,    69,   667,   669,   670,   279,   280,   281,   282,   283,
    1020,   284,   243,   285,   286,   287,   288,   289,   671,   673,
     674,   677,   675,   290,   676,   679,   680,   681,   682,   684,
     685,    70,   686,    71,   683,   812,   813,   687,   688,   690,
     691,   693,   694,   695,   245,   246,   699,   696,   698,   700,
     701,   702,   672,   705,   291,   703,   709,    72,   711,   708,
     723,   725,   525,    73,    74,   814,   815,   295,   816,   710,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,   722,   724,
     726,   817,   727,   728,   818,   819,   820,   821,   822,   823,
     824,   825,   826,   827,   828,    74,   729,   730,   731,   732,
     733,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,   734,
     735,   736,   743,   737,   738,   740,   741,   739,   757,   744,
     750,   759,   755,   754,   758,   760,   227,   764,   770,   765,
     772,   769,   775,   776,   777,   801,   799,   228,   229,   850,
     837,   786,   802,   854,   840,   844,   847,   848,   855,   851,
     853,   856,   857,   879,   859,   860,   864,   880,   882,   883,
     892,   893,   896,   897,   898,   899,   900,   901,   902,   903,
     904,   906,   908,   914,   916,   917,   919,   920,   924,   923,
     925,   935,   926,   927,   939,   928,   929,   930,   931,   932,
     933,   945,   934,   936,   953,   941,   944,   959,   954,   942,
     955,   960,   965,   961,   962,   964,   971,   968,   983,   969,
     984,   987,   989,   990,   991,   994,   996,   998,  1000,  1004,
    1001,  1002,  1009,  1005,  1011,    10,  1012,  1032,   348,  1013,
    1022,  1024,  1017,  1030,   208,  1034,   570,   202,   943,   492,
     921,   627,   938,   986,   388,   858,   966,  1010,   756,   261,
     995,   982,  1014,   623,   790,   831,   340,   602,   656,     0,
     495,     0,   423
};

static const yytype_int16 yycheck[] =
{
      97,   206,   162,   345,   159,   489,   564,   449,   490,   169,
     170,   461,   462,    72,   377,   171,   176,   177,   180,   179,
     180,   467,   184,   185,   186,     4,   377,     3,     3,     3,
     190,    53,     3,   193,   617,   191,   192,   197,   194,   195,
     615,   491,   198,   780,   200,     4,   613,   203,   458,     4,
      53,   211,     3,     8,     9,     4,     6,    72,    72,     8,
      72,    73,   222,   405,    76,    77,   408,   222,   173,    72,
      73,    72,    72,    72,    73,     4,   181,   182,   159,    72,
       4,   937,     4,    72,     4,   666,     8,    72,    73,    72,
     144,   881,     5,    72,    73,     5,    72,    72,    72,   204,
     102,   182,   179,     3,    69,     4,   177,   185,    66,    85,
      85,     4,     3,   184,    72,     8,     9,   222,   196,   197,
     701,   281,   282,   178,   284,   202,   203,    65,   177,     0,
     189,   185,   177,    98,    72,   184,     3,   177,    68,   184,
     180,   177,    72,    72,     3,     3,   351,  1003,   184,   114,
     231,   232,   233,   234,   235,   214,   168,   179,    82,    72,
      84,   220,    72,   222,   722,   168,    72,   179,   178,   168,
     185,   185,    72,    79,   279,   280,   179,   967,   179,    72,
     179,    72,   196,   197,   921,   185,   179,    86,   200,   201,
     179,   115,   750,   124,   179,   126,   179,   200,   201,   185,
     179,   200,   201,   179,   179,   179,   185,   183,   179,   185,
     185,   185,   185,   796,    72,   194,   195,   196,   197,   794,
     179,   178,   377,   177,   179,   792,   185,    85,   179,   103,
     179,   181,   208,   209,   177,   194,   195,   196,   197,   178,
     396,   184,   447,   727,   607,   194,   195,   589,   181,   409,
     410,   230,   662,   703,   605,   411,   738,   179,   177,   179,
     420,   421,   418,   709,     4,   184,   422,   178,     8,     9,
     426,   230,   194,   195,   194,   195,   436,   122,   440,   179,
     177,   230,   444,   445,   446,   185,   178,   184,   179,   386,
     450,   350,   183,    72,   185,    74,    75,   356,   357,   178,
     359,   360,   361,   362,   363,   178,   206,   207,   230,     4,
     230,   466,   179,     8,     9,   206,   207,   376,   377,   187,
     179,   179,   178,   479,   183,   177,   177,   185,   180,   180,
     389,    10,    11,    12,    13,    15,    16,   204,   205,   398,
     178,    77,    78,   181,   403,   204,   205,   178,   407,   178,
     208,   209,   177,   513,   178,   180,   233,   234,   235,   178,
     702,    71,    72,   523,   524,    39,    40,    41,   178,   428,
      65,    66,    67,   855,    69,   190,   191,    72,    73,   231,
     232,   233,   234,   235,    76,    77,   231,   232,   233,   234,
     235,   178,   237,     3,   930,   931,   178,     7,   178,    94,
      95,    96,    97,    98,    99,    81,   101,   178,   233,   234,
     235,   178,    88,   178,   178,   178,   900,   178,   477,   178,
     178,   178,    98,   118,   119,   177,   179,   178,   180,   178,
     178,   178,   127,   128,   129,   130,   131,   132,   114,   177,
       4,   178,   884,   178,   177,   929,   928,   178,   178,   791,
     145,  1009,   178,   178,   178,   178,   611,   178,    22,   178,
      24,   156,    72,    73,    28,    29,    30,   178,   236,   145,
     146,   147,   148,   168,   150,   151,   152,   153,   154,   231,
     232,   233,   234,   235,   179,   178,     3,   178,   693,   178,
     180,   178,   178,   231,   232,   233,   234,   235,   231,   232,
     233,   234,   235,   178,   177,   200,   201,   178,    72,    73,
     178,   667,   178,   178,    90,   671,   185,    83,   674,   675,
     179,   182,   236,   181,   179,   730,   731,   732,   733,   181,
     735,   736,   737,   592,   183,   230,   696,   181,     3,     4,
     695,   231,   232,   233,   234,   235,   177,   111,   177,   237,
     609,   161,   162,   163,   164,   165,   177,   167,   168,   169,
     170,   171,   172,   173,   231,   232,   233,   234,   235,   179,
     231,   232,   233,   234,   235,   176,   179,     4,   177,   734,
     177,   181,   239,     4,     3,   740,   181,     3,   181,   183,
     200,   201,   182,   177,   204,   205,   177,   182,   177,   177,
     210,     4,   166,   177,   168,   177,   179,    72,    73,   179,
     669,   670,   179,   181,   673,   179,   179,   177,   179,   183,
     182,   185,   179,     4,   177,    71,    72,    73,   177,   177,
     194,   195,   196,   197,   177,   177,   200,   201,   202,   203,
       4,   231,   232,   233,   234,   235,   231,   232,   233,   234,
     235,    72,    73,   177,   177,   860,   177,   999,     6,   177,
     182,   816,   177,   177,  1006,   177,   230,     4,   188,   231,
     232,   233,   234,   235,   194,   177,   196,   177,   198,   182,
     200,  1023,   202,   177,   204,   744,   206,   177,   208,   177,
     210,   896,   897,   898,   177,   754,   161,   162,   163,   164,
     165,   177,   167,   168,   169,   170,   171,   172,   173,   231,
     232,   233,   234,   235,   179,   161,   162,   163,   164,   165,
     182,   167,   168,   169,   170,   171,   172,   173,   231,   232,
     233,   234,   235,   179,   177,   200,   201,   177,   177,   180,
     161,   162,   163,   164,   165,   210,   167,   168,   169,   170,
     171,   172,   173,   177,   200,   201,   178,   195,   179,   197,
     177,   199,   177,   201,   210,   203,   180,   205,   178,   207,
     178,   209,   932,   211,   231,   232,   233,   234,   235,   200,
     201,   178,     6,     3,   843,     3,   177,   182,   177,   210,
      14,     5,   182,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,   182,   177,   186,    31,    32,    33,
      34,    35,   177,    37,   177,   177,    72,    73,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    71,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    71,
      64,   231,   232,   233,   234,   235,    70,   177,   907,   174,
     177,   180,    36,   231,   232,   233,   234,   235,     4,   177,
      36,    38,  1022,    80,   177,    89,   177,    80,   177,    84,
    1030,   152,   149,     4,    90,   110,   100,   182,   177,     3,
     184,   177,   106,   180,   108,   109,     4,   111,   177,   113,
     185,   988,   116,   117,   185,   177,   120,   121,   238,   123,
     180,   125,   177,   177,   177,   161,   162,   163,   164,   165,
    1007,   167,   168,   169,   170,   171,   172,   173,   177,   181,
     177,     4,   177,   179,   177,   177,   177,     4,   177,   177,
       4,   155,   177,   157,   181,    74,    75,     4,   177,   177,
     177,   177,    71,   177,   200,   201,     4,   178,   177,   177,
     177,   110,     3,   237,   210,   177,   177,   181,     4,   178,
       3,   186,    71,   187,   188,   104,   105,   179,   107,   180,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   177,   180,
     180,   130,   178,   178,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   188,   178,   178,   178,   178,
     178,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   178,
     178,   178,     5,   178,   178,   177,   177,   180,     4,   177,
     177,   145,   177,   179,   177,     4,   185,    80,    80,   177,
     177,   185,   177,    87,   177,    93,   177,   196,   197,     4,
     177,   183,    92,     4,   180,   180,   180,   180,   178,   177,
     177,   182,     4,     4,   177,   177,   177,     4,   177,   177,
     180,     5,   178,   178,   178,   178,   178,   178,   178,     3,
      39,   180,   177,    91,     4,   177,   180,   177,    82,   180,
     178,   180,   178,   178,     7,   178,   178,   178,   178,   178,
     178,    72,   178,   177,     4,   178,   177,     4,   115,   184,
     180,   177,     4,   178,    88,   177,     4,   177,     4,    87,
       4,   178,     5,   238,    39,   180,    90,   178,     4,    81,
     180,   180,   177,   180,   180,     5,   175,  1029,   205,   180,
     177,   177,   180,   177,    67,   180,   449,    61,   887,   385,
     859,   508,   882,   938,   226,   767,   920,   990,   679,   165,
     960,   933,   998,   501,   709,   740,   199,   477,   513,    -1,
     390,    -1,   288
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   241,   242,   243,   244,   178,   178,     0,
     242,   102,   252,   185,   185,   178,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    31,
      32,    33,    34,    35,    37,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    64,    70,    89,   100,   106,
     108,   109,   111,   113,   116,   117,   120,   121,   123,   125,
     155,   157,   181,   187,   188,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   245,   246,   247,   254,   256,   257,   258,
     259,   261,   262,   270,   273,   274,   276,   277,   284,   286,
     287,   289,   291,   293,   297,   298,   299,   302,   304,   311,
     318,   321,   327,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   343,   344,   345,   346,   350,   351,   353,
     354,   355,   356,   357,   358,   359,   360,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   177,   103,   253,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   276,   178,   178,   178,   181,   188,   258,   260,
     278,   178,   187,     4,   179,   194,   195,   230,   401,   411,
     179,   254,   236,   178,   288,     3,   177,   185,   196,   197,
     387,   400,    10,    11,    12,    13,     4,    86,   412,    72,
      73,    76,    77,   168,   179,   200,   201,   319,   389,   390,
      90,     3,    72,    85,   179,   185,   208,   209,   329,   398,
     399,   329,     3,    72,   179,   185,   206,   207,   323,   396,
     397,    72,   185,   372,    65,    72,   381,   389,   389,   161,
     162,   163,   164,   165,   167,   169,   170,   171,   172,   173,
     179,   210,   389,   409,   410,   179,   202,   203,   392,   393,
     392,    53,   179,   419,   420,   185,   389,   389,    83,   389,
     179,   389,   419,   392,   392,    72,   185,   301,   373,   419,
     419,   419,   388,   401,   179,   300,   401,   389,   410,   410,
     389,   410,   410,     4,     8,     9,    72,   413,   389,   410,
     301,   410,     5,    72,   312,   294,   410,   392,   257,   388,
     236,   181,   389,   179,     8,   401,   181,   159,   182,   231,
     232,   233,   234,   235,   400,   401,   406,   407,   408,    22,
      24,    28,    29,    30,   111,   166,   179,   183,   389,   392,
     400,   401,   415,   416,   417,   292,   183,   255,   253,   181,
     237,   177,   177,   177,   176,   391,   179,     4,   181,   177,
     177,   306,     3,   181,   330,   328,     3,   181,   322,   177,
     177,   177,   177,   177,   392,   392,   389,   389,   179,   389,
     179,   179,   179,   409,   179,   410,   239,     4,   181,   177,
     177,   179,     4,   177,   177,   177,   177,   177,     4,   177,
     177,   177,     6,   275,   177,   177,   177,   177,     4,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   255,
     295,   177,   177,   182,   401,   388,   177,   271,   180,   180,
     401,   401,   401,   401,   401,   401,   401,   177,   180,   178,
       4,   266,   267,   268,   269,   390,   393,   400,   401,   178,
     178,   178,   256,   254,   401,   387,     3,     3,   179,   352,
       3,   177,   182,   410,   177,   401,    77,    78,   320,     5,
     186,   177,   401,   177,   255,   177,   401,   255,   389,   389,
     410,    71,    71,   177,   177,    71,   314,   410,   389,   389,
       3,     7,   179,   204,   205,   394,   395,   410,   174,   180,
     410,   177,   401,    66,    72,   382,    36,     4,   177,    36,
      38,    72,    74,    75,   383,   386,   389,    80,   177,    80,
     419,    72,   179,   307,   177,   419,   419,   419,   388,   177,
     300,   413,   389,    68,    72,   418,    84,   152,   149,    90,
      72,   179,   308,     4,   381,   296,   179,   394,   394,   290,
     122,   237,   285,   182,    72,   400,   259,   403,   404,   405,
     182,   182,   406,   410,   177,   177,   184,   177,   184,   177,
     184,   177,   184,   183,   398,   183,   396,   183,   394,   184,
     182,     3,   177,   391,   180,     4,   182,   306,   185,   182,
       4,     8,     9,    65,    66,    67,    69,    94,    95,    96,
      97,    98,    99,   101,   118,   119,   127,   128,   129,   130,
     131,   132,   145,   156,   331,   389,   411,   414,   185,   182,
     324,   316,   177,   177,   389,   389,   238,   177,   180,   177,
     177,   177,     3,   181,   177,   177,   177,     4,   182,   177,
     177,     4,   177,   181,   177,     4,   177,     4,   177,   382,
     177,   177,   342,   177,    71,   177,   178,   347,   177,     4,
     177,   177,   110,   177,   255,   237,   401,   279,   178,   177,
     180,     4,   393,   390,   401,   400,   263,   399,   264,   397,
     265,   395,   177,     3,   180,   186,   180,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   180,
     177,   177,   381,     5,   177,   314,   410,   401,   401,   410,
     177,   401,   410,   410,   179,   177,   373,     4,   177,   145,
       4,    72,    79,   385,    80,   177,    72,   179,   309,   185,
      80,   388,   177,   400,   389,   177,    87,   177,    72,   179,
     310,    72,   313,   314,   255,   394,   183,   190,   191,   402,
     403,   272,   177,   184,   177,   184,   177,   184,   382,   177,
     398,    93,    92,   388,   388,   388,   388,   400,   388,   388,
     388,   396,    74,    75,   104,   105,   107,   130,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   325,
     384,   386,   400,    69,    98,   114,   317,   177,   250,   401,
     180,   177,   180,   177,   180,   382,   182,   180,   180,   401,
       4,   177,   374,   177,     4,   178,   182,     4,   319,   177,
     177,     4,    72,   315,   177,    81,    88,    98,   114,   145,
     146,   147,   148,   150,   151,   152,   153,   154,   348,     4,
       4,   374,   177,   177,   124,   126,   280,   281,   255,   399,
     397,   395,   180,     5,   178,   400,   178,   178,   178,   178,
     178,   178,   178,     3,    39,   401,   180,   232,   177,    39,
      40,    41,   375,   376,    91,   377,     4,   177,   396,   180,
     177,   310,   388,   180,    82,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   180,   177,   377,   315,     7,
     413,   178,   184,   280,   177,    72,   388,   388,   388,   144,
     185,   326,   398,     4,   115,   180,   177,   180,   401,     4,
     177,   178,    88,   379,   177,     4,   320,   374,   177,    87,
     380,     4,     4,    82,    84,   115,   349,   396,   398,   352,
     352,   389,   372,     4,     4,   379,   316,   178,   283,     5,
     238,    39,   177,   248,   180,   375,    90,   378,   178,   361,
       4,   180,   180,   377,    81,   180,   303,   282,   254,   177,
     326,   180,   175,   180,   380,   255,   362,   180,   379,   255,
     254,   382,   177,   305,   177,   389,   255,     5,    72,   251,
     177,   249,   250,   389,   180
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
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_BOUNDARY_TYPE));
		  ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 414 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 418 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 424 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_COPY));
		      (yyval.i) = 0;
		  ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 429 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 435 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 439 "lev_comp.y"
    {
		      (yyval.i) = what_map_char((char) (yyvsp[(2) - (2)].i));
		  ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 450 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 454 "lev_comp.y"
    {
		      add_opvars(splev, "io",
                                 VA_PASS2((int) (yyvsp[(3) - (3)].i), SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 461 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 465 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 471 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 475 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 481 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (3)].i);
		  ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 576 "lev_comp.y"
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

  case 110:

/* Line 1455 of yacc.c  */
#line 592 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 598 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 604 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 610 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 616 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 622 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 628 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 634 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 640 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 650 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 660 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 670 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 680 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 690 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 700 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 712 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 717 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 724 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 729 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 736 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 741 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 748 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 752 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 758 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 763 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 770 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 774 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 780 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 784 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 790 "lev_comp.y"
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

  case 140:

/* Line 1455 of yacc.c  */
#line 812 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 816 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 827 "lev_comp.y"
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

  case 143:

/* Line 1455 of yacc.c  */
#line 882 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 888 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 892 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 898 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 905 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 909 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 917 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 921 "lev_comp.y"
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

  case 151:

/* Line 1455 of yacc.c  */
#line 944 "lev_comp.y"
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

  case 154:

/* Line 1455 of yacc.c  */
#line 989 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 999 "lev_comp.y"
    {
		  ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1002 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1012 "lev_comp.y"
    {
		  ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1017 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1031 "lev_comp.y"
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

  case 162:

/* Line 1455 of yacc.c  */
#line 1068 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1073 "lev_comp.y"
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

  case 164:

/* Line 1455 of yacc.c  */
#line 1102 "lev_comp.y"
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

  case 165:

/* Line 1455 of yacc.c  */
#line 1116 "lev_comp.y"
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

  case 166:

/* Line 1455 of yacc.c  */
#line 1132 "lev_comp.y"
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

  case 167:

/* Line 1455 of yacc.c  */
#line 1152 "lev_comp.y"
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
#line 1164 "lev_comp.y"
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

  case 169:

/* Line 1455 of yacc.c  */
#line 1184 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1190 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1200 "lev_comp.y"
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

  case 172:

/* Line 1455 of yacc.c  */
#line 1218 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1228 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1234 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1239 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1244 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1251 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1258 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1267 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1275 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1286 "lev_comp.y"
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

  case 182:

/* Line 1455 of yacc.c  */
#line 1302 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1309 "lev_comp.y"
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

  case 184:

/* Line 1455 of yacc.c  */
#line 1323 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1330 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1334 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1340 "lev_comp.y"
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
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1365 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1371 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1376 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1382 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1387 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1393 "lev_comp.y"
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

  case 196:

/* Line 1455 of yacc.c  */
#line 1404 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
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
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1439 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1447 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1452 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1457 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1465 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1472 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1480 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1496 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1500 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1506 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1514 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1520 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1528 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1536 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1541 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1547 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1553 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1559 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1565 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1570 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1575 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1580 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1585 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1590 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1595 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1600 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1605 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1610 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1615 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1621 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1628 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1637 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1641 "lev_comp.y"
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

  case 243:

/* Line 1455 of yacc.c  */
#line 1654 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1662 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1672 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1680 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1689 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1696 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1704 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1710 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1715 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1720 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1725 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1730 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1735 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1740 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1745 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1750 "lev_comp.y"
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

  case 259:

/* Line 1455 of yacc.c  */
#line 1761 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1767 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1772 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1777 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1782 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1787 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1792 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1797 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1802 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1807 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1812 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1817 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1822 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x400000;
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1829 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1835 "lev_comp.y"
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

  case 274:

/* Line 1455 of yacc.c  */
#line 1864 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1869 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1877 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1883 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1889 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1896 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1903 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1913 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1923 "lev_comp.y"
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

  case 283:

/* Line 1455 of yacc.c  */
#line 1938 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1948 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1952 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1958 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1964 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1970 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1976 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1982 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1989 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1997 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 2003 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2008 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2013 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2018 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2023 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2029 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2035 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2040 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2045 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2050 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_CATEGORY));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2055 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2060 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2070 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2076 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2082 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2087 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2094 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2101 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2107 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2113 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2119 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2123 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2129 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2135 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2145 "lev_comp.y"
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

  case 322:

/* Line 1455 of yacc.c  */
#line 2164 "lev_comp.y"
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

  case 323:

/* Line 1455 of yacc.c  */
#line 2176 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2180 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2186 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2193 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2199 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2205 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2211 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2217 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2221 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2226 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2233 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2239 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2246 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2250 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2257 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2268 "lev_comp.y"
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

  case 341:

/* Line 1455 of yacc.c  */
#line 2281 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2285 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2291 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2295 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2302 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2306 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2310 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2316 "lev_comp.y"
    {
			(yyval.i) = ROOM;
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2320 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2326 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2332 "lev_comp.y"
    {
			(yyval.i) = 0;
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2336 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2342 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2358 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2366 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2376 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2385 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2390 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2397 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2408 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2414 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2418 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2422 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2429 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2439 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2446 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2450 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2456 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2460 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2468 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2472 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2479 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2489 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2502 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2506 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2513 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2523 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2532 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2543 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2547 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2554 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2564 "lev_comp.y"
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

  case 393:

/* Line 1455 of yacc.c  */
#line 2575 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2584 "lev_comp.y"
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

  case 395:

/* Line 1455 of yacc.c  */
#line 2594 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2600 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2604 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2611 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2621 "lev_comp.y"
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

  case 400:

/* Line 1455 of yacc.c  */
#line 2632 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2641 "lev_comp.y"
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

  case 402:

/* Line 1455 of yacc.c  */
#line 2651 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2655 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2661 "lev_comp.y"
    { ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2663 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2669 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2673 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2677 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2681 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2689 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2698 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2702 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2706 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2710 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2714 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2717 "lev_comp.y"
    { ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2721 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2728 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2737 "lev_comp.y"
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

  case 424:

/* Line 1455 of yacc.c  */
#line 2778 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2782 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2789 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2796 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2806 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2810 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2818 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2822 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2826 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2830 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2834 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2839 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2843 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2847 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2852 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2857 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2862 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2866 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2871 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2876 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2880 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2884 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2889 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2893 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2900 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2906 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2910 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2916 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2932 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2936 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2940 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2944 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 2966 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2970 "lev_comp.y"
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

  case 471:

/* Line 1455 of yacc.c  */
#line 2996 "lev_comp.y"
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
#line 6746 "lev_comp.tab.c"
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
#line 3024 "lev_comp.y"


/*lev_comp.y*/

