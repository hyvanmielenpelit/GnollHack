
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

/* GnollHack 4.0  lev_comp.y	$NHDT-Date: 1543371691 2018/11/28 02:21:31 $  $NHDT-Branch: NetHack-3.6.2-beta01 $:$NHDT-Revision: 1.22 $ */
/*      Copyright (c) 1989 by Jean-Christophe Collet */
/* GnollHack may be freely redistributed.  See license for details. */

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
     SPECIAL_TILESET_ID = 277,
     OBJECT_ID = 278,
     COBJECT_ID = 279,
     MONSTER_ID = 280,
     TRAP_ID = 281,
     DOOR_ID = 282,
     DRAWBRIDGE_ID = 283,
     object_ID = 284,
     monster_ID = 285,
     terrain_ID = 286,
     MAZEWALK_ID = 287,
     WALLIFY_ID = 288,
     REGION_ID = 289,
     SPECIAL_REGION_ID = 290,
     SPECIAL_LEVREGION_ID = 291,
     SPECIAL_REGION_TYPE = 292,
     NAMING_ID = 293,
     NAMING_TYPE = 294,
     FILLING = 295,
     IRREGULAR = 296,
     JOINED = 297,
     ALTAR_ID = 298,
     ANVIL_ID = 299,
     NPC_ID = 300,
     LADDER_ID = 301,
     STAIR_ID = 302,
     NON_DIGGABLE_ID = 303,
     NON_PASSWALL_ID = 304,
     ROOM_ID = 305,
     ARTIFACT_NAME_ID = 306,
     PORTAL_ID = 307,
     TELEPRT_ID = 308,
     BRANCH_ID = 309,
     LEV = 310,
     MINERALIZE_ID = 311,
     CORRIDOR_ID = 312,
     GOLD_ID = 313,
     ENGRAVING_ID = 314,
     FOUNTAIN_ID = 315,
     THRONE_ID = 316,
     MODRON_PORTAL_ID = 317,
     LEVEL_TELEPORTER_ID = 318,
     LEVEL_TELEPORT_DIRECTION_TYPE = 319,
     LEVEL_TELEPORT_END_TYPE = 320,
     POOL_ID = 321,
     SINK_ID = 322,
     NONE = 323,
     RAND_CORRIDOR_ID = 324,
     DOOR_STATE = 325,
     LIGHT_STATE = 326,
     CURSE_TYPE = 327,
     ENGRAVING_TYPE = 328,
     KEYTYPE_ID = 329,
     LEVER_ID = 330,
     NO_PICKUP_ID = 331,
     DIRECTION = 332,
     RANDOM_TYPE = 333,
     RANDOM_TYPE_BRACKET = 334,
     A_REGISTER = 335,
     ALIGNMENT = 336,
     LEFT_OR_RIGHT = 337,
     CENTER = 338,
     TOP_OR_BOT = 339,
     ALTAR_TYPE = 340,
     UP_OR_DOWN = 341,
     ACTIVE_OR_INACTIVE = 342,
     MODRON_PORTAL_TYPE = 343,
     NPC_TYPE = 344,
     FOUNTAIN_TYPE = 345,
     SPECIAL_OBJECT_TYPE = 346,
     CMAP_TYPE = 347,
     FLOOR_TYPE = 348,
     FLOOR_TYPE_ID = 349,
     FLOOR_ID = 350,
     FLOOR_MAIN_TYPE = 351,
     FLOOR_MAIN_TYPE_ID = 352,
     ELEMENTAL_ENCHANTMENT_TYPE = 353,
     EXCEPTIONALITY_TYPE = 354,
     EXCEPTIONALITY_ID = 355,
     ELEMENTAL_ENCHANTMENT_ID = 356,
     ENCHANTMENT_ID = 357,
     SECRET_DOOR_ID = 358,
     USES_UP_KEY_ID = 359,
     MYTHIC_PREFIX_TYPE = 360,
     MYTHIC_SUFFIX_TYPE = 361,
     MYTHIC_PREFIX_ID = 362,
     MYTHIC_SUFFIX_ID = 363,
     CHARGES_ID = 364,
     SPECIAL_QUALITY_ID = 365,
     SPEFLAGS_ID = 366,
     SUBROOM_ID = 367,
     NAME_ID = 368,
     FLAGS_ID = 369,
     FLAG_TYPE = 370,
     MON_ATTITUDE = 371,
     MON_ALERTNESS = 372,
     SUBTYPE_ID = 373,
     NON_PASSDOOR_ID = 374,
     MON_APPEARANCE = 375,
     ROOMDOOR_ID = 376,
     IF_ID = 377,
     ELSE_ID = 378,
     TERRAIN_ID = 379,
     HORIZ_OR_VERT = 380,
     REPLACE_TERRAIN_ID = 381,
     LOCATION_SUBTYPE_ID = 382,
     DOOR_SUBTYPE = 383,
     BRAZIER_SUBTYPE = 384,
     SIGNPOST_SUBTYPE = 385,
     EXIT_ID = 386,
     SHUFFLE_ID = 387,
     QUANTITY_ID = 388,
     BURIED_ID = 389,
     LOOP_ID = 390,
     FOR_ID = 391,
     TO_ID = 392,
     SWITCH_ID = 393,
     CASE_ID = 394,
     BREAK_ID = 395,
     DEFAULT_ID = 396,
     ERODED_ID = 397,
     TRAPPED_STATE = 398,
     RECHARGED_ID = 399,
     INVIS_ID = 400,
     GREASED_ID = 401,
     INDESTRUCTIBLE_ID = 402,
     FEMALE_ID = 403,
     WAITFORU_ID = 404,
     CANCELLED_ID = 405,
     REVIVED_ID = 406,
     AVENGE_ID = 407,
     FLEEING_ID = 408,
     BLINDED_ID = 409,
     PARALYZED_ID = 410,
     STUNNED_ID = 411,
     CONFUSED_ID = 412,
     SEENTRAPS_ID = 413,
     ALL_ID = 414,
     MONTYPE_ID = 415,
     OBJTYPE_ID = 416,
     TERTYPE_ID = 417,
     TERTYPE2_ID = 418,
     LEVER_EFFECT_TYPE = 419,
     SWITCHABLE_ID = 420,
     CONTINUOUSLY_USABLE_ID = 421,
     TARGET_ID = 422,
     TRAPTYPE_ID = 423,
     EFFECT_FLAG_ID = 424,
     GRAVE_ID = 425,
     BRAZIER_ID = 426,
     SIGNPOST_ID = 427,
     ERODEPROOF_ID = 428,
     FUNCTION_ID = 429,
     MSG_OUTPUT_TYPE = 430,
     COMPARE_TYPE = 431,
     UNKNOWN_TYPE = 432,
     rect_ID = 433,
     fillrect_ID = 434,
     line_ID = 435,
     randline_ID = 436,
     grow_ID = 437,
     selection_ID = 438,
     flood_ID = 439,
     rndcoord_ID = 440,
     circle_ID = 441,
     ellipse_ID = 442,
     filter_ID = 443,
     complement_ID = 444,
     gradient_ID = 445,
     GRADIENT_TYPE = 446,
     LIMITED = 447,
     HUMIDITY_TYPE = 448,
     STRING = 449,
     MAP_ID = 450,
     NQSTRING = 451,
     VARSTRING = 452,
     CFUNC = 453,
     CFUNC_INT = 454,
     CFUNC_STR = 455,
     CFUNC_COORD = 456,
     CFUNC_REGION = 457,
     VARSTRING_INT = 458,
     VARSTRING_INT_ARRAY = 459,
     VARSTRING_STRING = 460,
     VARSTRING_STRING_ARRAY = 461,
     VARSTRING_VAR = 462,
     VARSTRING_VAR_ARRAY = 463,
     VARSTRING_COORD = 464,
     VARSTRING_COORD_ARRAY = 465,
     VARSTRING_REGION = 466,
     VARSTRING_REGION_ARRAY = 467,
     VARSTRING_MAPCHAR = 468,
     VARSTRING_MAPCHAR_ARRAY = 469,
     VARSTRING_MONST = 470,
     VARSTRING_MONST_ARRAY = 471,
     VARSTRING_OBJ = 472,
     VARSTRING_OBJ_ARRAY = 473,
     VARSTRING_SEL = 474,
     VARSTRING_SEL_ARRAY = 475,
     METHOD_INT = 476,
     METHOD_INT_ARRAY = 477,
     METHOD_STRING = 478,
     METHOD_STRING_ARRAY = 479,
     METHOD_VAR = 480,
     METHOD_VAR_ARRAY = 481,
     METHOD_COORD = 482,
     METHOD_COORD_ARRAY = 483,
     METHOD_REGION = 484,
     METHOD_REGION_ARRAY = 485,
     METHOD_MAPCHAR = 486,
     METHOD_MAPCHAR_ARRAY = 487,
     METHOD_MONST = 488,
     METHOD_MONST_ARRAY = 489,
     METHOD_OBJ = 490,
     METHOD_OBJ_ARRAY = 491,
     METHOD_SEL = 492,
     METHOD_SEL_ARRAY = 493,
     DICE = 494
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
#line 537 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 549 "lev_comp.tab.c"

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
#define YYLAST   1263

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  257
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  185
/* YYNRULES -- Number of rules.  */
#define YYNRULES  495
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1098

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   494

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   252,   256,     2,
     196,   197,   250,   248,   194,   249,   254,   251,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   195,     2,
       2,   253,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   198,     2,   199,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   200,   255,   201,     2,     2,     2,     2,
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
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247
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
     261,   263,   265,   269,   273,   279,   283,   289,   295,   301,
     305,   309,   315,   321,   327,   335,   343,   351,   357,   359,
     363,   365,   369,   371,   375,   377,   381,   383,   387,   389,
     393,   395,   399,   400,   401,   410,   415,   417,   418,   420,
     422,   428,   432,   433,   434,   444,   445,   448,   449,   455,
     456,   461,   463,   466,   468,   475,   476,   480,   481,   488,
     489,   494,   495,   500,   502,   503,   508,   512,   514,   518,
     522,   528,   534,   542,   547,   548,   562,   563,   579,   580,
     583,   589,   591,   597,   599,   605,   607,   613,   615,   626,
     633,   635,   637,   639,   641,   643,   647,   649,   651,   652,
     656,   660,   664,   668,   670,   672,   674,   676,   678,   686,
     692,   694,   696,   698,   700,   704,   705,   711,   716,   717,
     721,   723,   725,   727,   729,   732,   734,   736,   738,   740,
     742,   746,   750,   754,   756,   758,   762,   764,   766,   768,
     772,   776,   777,   783,   786,   787,   791,   793,   797,   799,
     803,   807,   809,   811,   815,   817,   819,   821,   825,   827,
     829,   831,   835,   839,   843,   847,   851,   855,   859,   863,
     867,   869,   871,   873,   879,   887,   893,   902,   904,   908,
     912,   918,   924,   932,   940,   947,   953,   954,   957,   963,
     967,   979,   993,  1000,  1001,  1005,  1007,  1011,  1015,  1019,
    1023,  1025,  1027,  1031,  1035,  1039,  1043,  1047,  1051,  1053,
    1055,  1057,  1059,  1063,  1067,  1069,  1075,  1085,  1091,  1095,
    1099,  1105,  1115,  1121,  1127,  1133,  1134,  1148,  1149,  1151,
    1159,  1171,  1175,  1183,  1191,  1197,  1203,  1213,  1219,  1225,
    1229,  1237,  1241,  1249,  1257,  1261,  1267,  1275,  1285,  1287,
    1289,  1291,  1293,  1295,  1296,  1299,  1301,  1305,  1307,  1309,
    1311,  1312,  1316,  1318,  1319,  1323,  1325,  1326,  1330,  1332,
    1334,  1336,  1338,  1340,  1342,  1344,  1346,  1348,  1352,  1354,
    1356,  1361,  1363,  1365,  1370,  1372,  1374,  1379,  1381,  1386,
    1392,  1394,  1398,  1400,  1404,  1406,  1408,  1413,  1423,  1425,
    1427,  1432,  1434,  1440,  1442,  1444,  1449,  1451,  1453,  1459,
    1461,  1463,  1465,  1470,  1472,  1474,  1480,  1482,  1484,  1486,
    1490,  1492,  1494,  1498,  1500,  1505,  1509,  1513,  1517,  1521,
    1525,  1529,  1531,  1533,  1537,  1539,  1543,  1544,  1546,  1548,
    1550,  1552,  1556,  1557,  1559,  1561,  1564,  1567,  1572,  1579,
    1584,  1591,  1598,  1605,  1612,  1615,  1622,  1631,  1640,  1651,
    1666,  1669,  1671,  1675,  1677,  1681,  1683,  1685,  1687,  1689,
    1691,  1693,  1695,  1697,  1699,  1701,  1703,  1705,  1707,  1709,
    1711,  1713,  1715,  1717,  1719,  1730
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     258,     0,    -1,    -1,   259,    -1,   260,    -1,   260,   259,
      -1,   261,   269,   271,    -1,    16,   195,   202,    -1,    15,
     195,   202,   194,     3,    -1,    17,   195,    11,   194,   369,
      -1,    17,   195,    10,   194,     3,    -1,    17,   195,    13,
      -1,    17,   195,    12,   194,     3,   194,     3,   194,     5,
     194,     5,   194,   403,   194,   268,   267,    -1,    18,   195,
     433,    -1,    21,   195,    96,    -1,    -1,   194,   192,    -1,
      -1,   194,   410,    -1,    -1,   194,     3,    -1,     5,    -1,
      78,    -1,    -1,   114,   195,   270,    -1,   115,   194,   270,
      -1,   115,    -1,    -1,   273,   271,    -1,   200,   271,   201,
      -1,   314,    -1,   262,    -1,   263,    -1,   377,    -1,   264,
      -1,   384,    -1,   383,    -1,   381,    -1,   382,    -1,   385,
      -1,   386,    -1,   387,    -1,   388,    -1,   358,    -1,   316,
      -1,   279,    -1,   278,    -1,   372,    -1,   328,    -1,   350,
      -1,   390,    -1,   391,    -1,   360,    -1,   361,    -1,   362,
      -1,   389,    -1,   294,    -1,   304,    -1,   306,    -1,   310,
      -1,   308,    -1,   291,    -1,   301,    -1,   287,    -1,   290,
      -1,   353,    -1,   363,    -1,   335,    -1,   351,    -1,   338,
      -1,   344,    -1,   373,    -1,   368,    -1,   356,    -1,   315,
      -1,   374,    -1,   375,    -1,   376,    -1,   378,    -1,   321,
      -1,   319,    -1,   367,    -1,   371,    -1,   370,    -1,   354,
      -1,   355,    -1,   357,    -1,   349,    -1,   352,    -1,   212,
      -1,   214,    -1,   216,    -1,   218,    -1,   220,    -1,   222,
      -1,   224,    -1,   226,    -1,   228,    -1,   211,    -1,   213,
      -1,   215,    -1,   217,    -1,   219,    -1,   221,    -1,   223,
      -1,   225,    -1,   227,    -1,   274,    -1,   275,    -1,   205,
      -1,   205,    -1,   275,    -1,   132,   195,   274,    -1,   276,
     253,   422,    -1,   276,   253,   183,   195,   431,    -1,   276,
     253,   421,    -1,   276,   253,   438,   195,   415,    -1,   276,
     253,   437,   195,   417,    -1,   276,   253,   436,   195,   419,
      -1,   276,   253,   410,    -1,   276,   253,   413,    -1,   276,
     253,   200,   285,   201,    -1,   276,   253,   200,   284,   201,
      -1,   276,   253,   200,   283,   201,    -1,   276,   253,   438,
     195,   200,   282,   201,    -1,   276,   253,   437,   195,   200,
     281,   201,    -1,   276,   253,   436,   195,   200,   280,   201,
      -1,   276,   253,   200,   286,   201,    -1,   420,    -1,   280,
     194,   420,    -1,   418,    -1,   281,   194,   418,    -1,   416,
      -1,   282,   194,   416,    -1,   414,    -1,   283,   194,   414,
      -1,   411,    -1,   284,   194,   411,    -1,   422,    -1,   285,
     194,   422,    -1,   421,    -1,   286,   194,   421,    -1,    -1,
      -1,   174,   204,   196,   288,   426,   197,   289,   272,    -1,
     204,   196,   429,   197,    -1,   131,    -1,    -1,     6,    -1,
       6,    -1,   198,   422,   176,   422,   199,    -1,   198,   422,
     199,    -1,    -1,    -1,   138,   295,   198,   409,   199,   296,
     200,   297,   201,    -1,    -1,   298,   297,    -1,    -1,   139,
     434,   195,   299,   271,    -1,    -1,   141,   195,   300,   271,
      -1,   140,    -1,   254,   254,    -1,   137,    -1,   136,   277,
     253,   422,   302,   422,    -1,    -1,   303,   305,   272,    -1,
      -1,   135,   198,   409,   199,   307,   272,    -1,    -1,   293,
     195,   309,   273,    -1,    -1,   122,   293,   311,   312,    -1,
     272,    -1,    -1,   272,   313,   123,   272,    -1,    14,   195,
     421,    -1,    69,    -1,    69,   195,   434,    -1,    69,   195,
      78,    -1,    57,   195,   317,   194,   317,    -1,    57,   195,
     317,   194,   434,    -1,   196,     4,   194,    77,   194,   332,
     197,    -1,   393,   292,   194,   403,    -1,    -1,   112,   195,
     318,   194,   325,   194,   327,   394,   397,   399,   401,   320,
     272,    -1,    -1,    50,   195,   318,   194,   324,   194,   326,
     194,   327,   394,   397,   399,   401,   322,   272,    -1,    -1,
     194,     5,    -1,   196,     4,   194,     4,   197,    -1,    78,
      -1,   196,     4,   194,     4,   197,    -1,    78,    -1,   196,
     336,   194,   337,   197,    -1,    78,    -1,   196,     4,   194,
       4,   197,    -1,    78,    -1,   121,   195,   329,   194,   402,
     194,   330,   194,   332,   333,    -1,    27,   195,   402,   194,
     431,   333,    -1,     5,    -1,    78,    -1,   331,    -1,    78,
      -1,    77,    -1,    77,   255,   331,    -1,     4,    -1,    78,
      -1,    -1,   333,   194,   334,    -1,   127,   195,   128,    -1,
      74,   195,   419,    -1,   110,   195,     4,    -1,   147,    -1,
     103,    -1,   104,    -1,   119,    -1,    20,    -1,    19,   195,
     336,   194,   337,   323,   203,    -1,    19,   195,   410,   323,
     203,    -1,    82,    -1,    83,    -1,    84,    -1,    83,    -1,
      25,   195,   340,    -1,    -1,    25,   195,   340,   339,   272,
      -1,   417,   194,   410,   341,    -1,    -1,   341,   194,   342,
      -1,   421,    -1,   116,    -1,   117,    -1,   405,    -1,   120,
     421,    -1,   148,    -1,   145,    -1,   150,    -1,   151,    -1,
     152,    -1,   153,   195,   409,    -1,   154,   195,   409,    -1,
     155,   195,   409,    -1,   156,    -1,   157,    -1,   158,   195,
     343,    -1,   149,    -1,   202,    -1,   159,    -1,   202,   255,
     343,    -1,    23,   195,   346,    -1,    -1,    24,   195,   346,
     345,   272,    -1,   419,   347,    -1,    -1,   347,   194,   348,
      -1,    72,    -1,   160,   195,   417,    -1,   435,    -1,   113,
     195,   421,    -1,   133,   195,   409,    -1,   134,    -1,    71,
      -1,   142,   195,   409,    -1,   173,    -1,    70,    -1,   143,
      -1,   144,   195,   409,    -1,   145,    -1,   146,    -1,   410,
      -1,   101,   195,    98,    -1,   100,   195,    99,    -1,   107,
     195,   105,    -1,   108,   195,   106,    -1,   102,   195,   409,
      -1,   109,   195,   409,    -1,   110,   195,   409,    -1,   111,
     195,   409,    -1,    74,   195,   419,    -1,   147,    -1,   104,
      -1,    76,    -1,    26,   195,   392,   194,   410,    -1,    28,
     195,   410,   194,    77,   194,   402,    -1,    32,   195,   410,
     194,    77,    -1,    32,   195,   410,   194,    77,   194,     5,
     267,    -1,    33,    -1,    33,   195,   431,    -1,    33,   195,
     159,    -1,    46,   195,   410,   194,    86,    -1,    47,   195,
     410,   194,    86,    -1,    47,   195,   440,   194,   440,   194,
      86,    -1,    52,   195,   440,   194,   440,   194,   202,    -1,
      53,   195,   440,   194,   440,   359,    -1,    54,   195,   440,
     194,   440,    -1,    -1,   194,    86,    -1,    60,   195,   431,
     194,    90,    -1,    61,   195,   431,    -1,    62,   195,   410,
     194,   167,   195,   410,   194,    88,   194,    87,    -1,    62,
     195,   410,   194,    63,   194,    64,   194,    65,   194,    88,
     194,    87,    -1,    75,   195,   410,   194,   164,   364,    -1,
      -1,   364,   194,   365,    -1,    87,    -1,   160,   195,   417,
      -1,   161,   195,   419,    -1,   162,   195,   369,    -1,   163,
     195,   369,    -1,   165,    -1,   166,    -1,   167,   195,   410,
      -1,   168,   195,   392,    -1,   127,   195,   366,    -1,    94,
     195,   400,    -1,   169,   195,     4,    -1,   110,   195,     4,
      -1,     4,    -1,    88,    -1,   128,    -1,    90,    -1,    67,
     195,   431,    -1,    66,   195,   431,    -1,     3,    -1,   196,
       3,   194,   403,   197,    -1,   126,   195,   413,   194,   415,
     194,   415,   194,     7,    -1,   124,   195,   431,   194,   415,
      -1,    48,   195,   413,    -1,    49,   195,   413,    -1,    38,
     195,   202,   194,    39,    -1,    38,   195,   202,   194,    39,
     194,   160,   195,   417,    -1,    35,   195,   413,   194,    37,
      -1,    36,   195,   440,   194,    37,    -1,    22,   195,   431,
     194,    92,    -1,    -1,    34,   195,   413,   194,   403,   194,
     393,   394,   397,   399,   401,   379,   380,    -1,    -1,   272,
      -1,    43,   195,   410,   194,   404,   194,   406,    -1,    43,
     195,   410,   194,   404,   194,   406,   194,   160,   195,   417,
      -1,    44,   195,   410,    -1,    44,   195,   410,   194,   160,
     195,   417,    -1,    95,   195,   431,   194,    96,   194,    93,
      -1,   118,   195,   431,   194,     4,    -1,    45,   195,    89,
     194,   410,    -1,    45,   195,    89,   194,   410,   194,   160,
     195,   417,    -1,   170,   195,   410,   194,   421,    -1,   170,
     195,   410,   194,    78,    -1,   170,   195,   410,    -1,   171,
     195,   410,   194,   129,   194,   403,    -1,   171,   195,   410,
      -1,   172,   195,   410,   194,   130,   194,   421,    -1,   172,
     195,   410,   194,   130,   194,    78,    -1,   172,   195,   410,
      -1,    58,   195,   422,   194,   410,    -1,    59,   195,   410,
     194,   439,   194,   421,    -1,    56,   195,   409,   194,   409,
     194,   409,   194,   409,    -1,    56,    -1,   202,    -1,    78,
      -1,   202,    -1,    78,    -1,    -1,   194,   395,    -1,   396,
      -1,   396,   194,   395,    -1,    40,    -1,    41,    -1,    42,
      -1,    -1,    97,   195,   398,    -1,    96,    -1,    -1,    94,
     195,   400,    -1,    93,    -1,    -1,   160,   195,   417,    -1,
      70,    -1,    78,    -1,    71,    -1,    78,    -1,    81,    -1,
     407,    -1,    78,    -1,    81,    -1,   407,    -1,    80,   195,
      78,    -1,    85,    -1,    78,    -1,    80,   198,     4,   199,
      -1,   202,    -1,   213,    -1,   214,   198,   422,   199,    -1,
     422,    -1,   411,    -1,   185,   196,   431,   197,    -1,   217,
      -1,   218,   198,   422,   199,    -1,   196,     4,   194,     4,
     197,    -1,    78,    -1,    79,   412,   199,    -1,   193,    -1,
     193,   194,   412,    -1,   414,    -1,   219,    -1,   220,   198,
     422,   199,    -1,   196,     4,   194,     4,   194,     4,   194,
       4,   197,    -1,   416,    -1,   221,    -1,   222,   198,   422,
     199,    -1,     3,    -1,   196,     3,   194,   403,   197,    -1,
     418,    -1,   223,    -1,   224,   198,   422,   199,    -1,   202,
      -1,     3,    -1,   196,     3,   194,   202,   197,    -1,    78,
      -1,   420,    -1,   225,    -1,   226,   198,   422,   199,    -1,
     202,    -1,     3,    -1,   196,     3,   194,   202,   197,    -1,
      78,    -1,    91,    -1,   408,    -1,   421,   254,   408,    -1,
       4,    -1,   432,    -1,   196,     8,   197,    -1,   211,    -1,
     212,   198,   422,   199,    -1,   422,   248,   422,    -1,   422,
     249,   422,    -1,   422,   250,   422,    -1,   422,   251,   422,
      -1,   422,   252,   422,    -1,   196,   422,   197,    -1,   207,
      -1,   208,    -1,   276,   195,   423,    -1,   424,    -1,   425,
     194,   424,    -1,    -1,   425,    -1,   422,    -1,   421,    -1,
     427,    -1,   428,   194,   427,    -1,    -1,   428,    -1,   410,
      -1,   178,   413,    -1,   179,   413,    -1,   180,   410,   194,
     410,    -1,   181,   410,   194,   410,   194,   422,    -1,   182,
     196,   431,   197,    -1,   182,   196,   331,   194,   431,   197,
      -1,   188,   196,     7,   194,   431,   197,    -1,   188,   196,
     431,   194,   431,   197,    -1,   188,   196,   415,   194,   431,
     197,    -1,   184,   410,    -1,   186,   196,   410,   194,   422,
     197,    -1,   186,   196,   410,   194,   422,   194,    40,   197,
      -1,   187,   196,   410,   194,   422,   194,   422,   197,    -1,
     187,   196,   410,   194,   422,   194,   422,   194,    40,   197,
      -1,   190,   196,   191,   194,   196,   422,   249,   422,   265,
     197,   194,   410,   266,   197,    -1,   189,   430,    -1,   227,
      -1,   196,   431,   197,    -1,   430,    -1,   430,   256,   431,
      -1,   247,    -1,    92,    -1,     4,    -1,     8,    -1,     9,
      -1,     4,    -1,     8,    -1,     9,    -1,     4,    -1,   432,
      -1,    29,    -1,    23,    -1,    30,    -1,    25,    -1,    31,
      -1,   124,    -1,    73,    -1,    78,    -1,   441,    -1,    55,
     196,     4,   194,     4,   194,     4,   194,     4,   197,    -1,
     196,     4,   194,     4,   194,     4,   194,     4,   197,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   284,   284,   285,   288,   289,   292,   315,   320,   342,
     355,   367,   373,   402,   408,   415,   418,   425,   429,   436,
     439,   446,   447,   451,   454,   461,   465,   472,   475,   481,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   539,   540,   541,   542,   543,   544,   545,   548,
     549,   550,   551,   552,   553,   554,   555,   556,   559,   560,
     561,   562,   563,   564,   565,   566,   567,   570,   571,   572,
     575,   576,   579,   595,   601,   607,   613,   619,   625,   631,
     637,   643,   653,   663,   673,   683,   693,   703,   715,   720,
     727,   732,   739,   744,   751,   755,   761,   766,   773,   777,
     783,   787,   794,   816,   793,   830,   885,   892,   895,   901,
     908,   912,   921,   925,   920,   988,   989,   993,   992,  1006,
    1005,  1020,  1030,  1031,  1034,  1072,  1071,  1106,  1105,  1136,
    1135,  1168,  1167,  1193,  1204,  1203,  1231,  1237,  1242,  1247,
    1254,  1261,  1270,  1278,  1290,  1289,  1313,  1312,  1334,  1337,
    1343,  1353,  1359,  1368,  1374,  1379,  1385,  1390,  1396,  1407,
    1413,  1414,  1417,  1418,  1421,  1425,  1431,  1432,  1436,  1442,
    1450,  1455,  1460,  1465,  1470,  1475,  1480,  1488,  1495,  1503,
    1511,  1512,  1515,  1516,  1519,  1524,  1523,  1537,  1544,  1551,
    1559,  1564,  1570,  1576,  1582,  1588,  1593,  1598,  1603,  1608,
    1613,  1618,  1623,  1628,  1633,  1638,  1644,  1651,  1660,  1664,
    1677,  1686,  1685,  1703,  1713,  1719,  1727,  1733,  1738,  1743,
    1748,  1753,  1758,  1763,  1768,  1773,  1787,  1793,  1798,  1803,
    1808,  1813,  1818,  1823,  1828,  1833,  1838,  1843,  1848,  1853,
    1858,  1863,  1868,  1875,  1881,  1910,  1915,  1923,  1929,  1933,
    1941,  1948,  1955,  1965,  1975,  1990,  2001,  2004,  2010,  2016,
    2022,  2026,  2032,  2039,  2045,  2053,  2059,  2064,  2069,  2074,
    2079,  2085,  2091,  2096,  2101,  2106,  2111,  2116,  2123,  2123,
    2123,  2123,  2126,  2132,  2138,  2143,  2150,  2157,  2163,  2169,
    2175,  2180,  2187,  2193,  2203,  2210,  2209,  2241,  2244,  2250,
    2255,  2262,  2266,  2272,  2278,  2284,  2288,  2294,  2298,  2303,
    2310,  2314,  2321,  2325,  2330,  2337,  2343,  2350,  2354,  2361,
    2369,  2372,  2382,  2386,  2389,  2395,  2399,  2406,  2410,  2414,
    2421,  2424,  2430,  2437,  2440,  2446,  2453,  2457,  2464,  2465,
    2468,  2469,  2472,  2473,  2474,  2480,  2481,  2482,  2488,  2489,
    2492,  2501,  2506,  2513,  2524,  2530,  2534,  2538,  2545,  2555,
    2562,  2566,  2572,  2576,  2584,  2588,  2595,  2605,  2618,  2622,
    2629,  2639,  2648,  2659,  2663,  2670,  2680,  2691,  2700,  2710,
    2716,  2720,  2727,  2737,  2748,  2757,  2767,  2771,  2778,  2779,
    2785,  2789,  2793,  2797,  2805,  2814,  2818,  2822,  2826,  2830,
    2834,  2837,  2844,  2853,  2886,  2887,  2890,  2891,  2894,  2898,
    2905,  2912,  2923,  2926,  2934,  2938,  2942,  2946,  2950,  2955,
    2959,  2963,  2968,  2973,  2978,  2982,  2987,  2992,  2996,  3000,
    3005,  3009,  3016,  3022,  3026,  3032,  3039,  3040,  3043,  3044,
    3045,  3048,  3052,  3056,  3060,  3066,  3067,  3070,  3071,  3074,
    3075,  3078,  3079,  3082,  3086,  3112
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
  "BOUNDARY_TYPE_ID", "SPECIAL_TILESET_ID", "OBJECT_ID", "COBJECT_ID",
  "MONSTER_ID", "TRAP_ID", "DOOR_ID", "DRAWBRIDGE_ID", "object_ID",
  "monster_ID", "terrain_ID", "MAZEWALK_ID", "WALLIFY_ID", "REGION_ID",
  "SPECIAL_REGION_ID", "SPECIAL_LEVREGION_ID", "SPECIAL_REGION_TYPE",
  "NAMING_ID", "NAMING_TYPE", "FILLING", "IRREGULAR", "JOINED", "ALTAR_ID",
  "ANVIL_ID", "NPC_ID", "LADDER_ID", "STAIR_ID", "NON_DIGGABLE_ID",
  "NON_PASSWALL_ID", "ROOM_ID", "ARTIFACT_NAME_ID", "PORTAL_ID",
  "TELEPRT_ID", "BRANCH_ID", "LEV", "MINERALIZE_ID", "CORRIDOR_ID",
  "GOLD_ID", "ENGRAVING_ID", "FOUNTAIN_ID", "THRONE_ID",
  "MODRON_PORTAL_ID", "LEVEL_TELEPORTER_ID",
  "LEVEL_TELEPORT_DIRECTION_TYPE", "LEVEL_TELEPORT_END_TYPE", "POOL_ID",
  "SINK_ID", "NONE", "RAND_CORRIDOR_ID", "DOOR_STATE", "LIGHT_STATE",
  "CURSE_TYPE", "ENGRAVING_TYPE", "KEYTYPE_ID", "LEVER_ID", "NO_PICKUP_ID",
  "DIRECTION", "RANDOM_TYPE", "RANDOM_TYPE_BRACKET", "A_REGISTER",
  "ALIGNMENT", "LEFT_OR_RIGHT", "CENTER", "TOP_OR_BOT", "ALTAR_TYPE",
  "UP_OR_DOWN", "ACTIVE_OR_INACTIVE", "MODRON_PORTAL_TYPE", "NPC_TYPE",
  "FOUNTAIN_TYPE", "SPECIAL_OBJECT_TYPE", "CMAP_TYPE", "FLOOR_TYPE",
  "FLOOR_TYPE_ID", "FLOOR_ID", "FLOOR_MAIN_TYPE", "FLOOR_MAIN_TYPE_ID",
  "ELEMENTAL_ENCHANTMENT_TYPE", "EXCEPTIONALITY_TYPE", "EXCEPTIONALITY_ID",
  "ELEMENTAL_ENCHANTMENT_ID", "ENCHANTMENT_ID", "SECRET_DOOR_ID",
  "USES_UP_KEY_ID", "MYTHIC_PREFIX_TYPE", "MYTHIC_SUFFIX_TYPE",
  "MYTHIC_PREFIX_ID", "MYTHIC_SUFFIX_ID", "CHARGES_ID",
  "SPECIAL_QUALITY_ID", "SPEFLAGS_ID", "SUBROOM_ID", "NAME_ID", "FLAGS_ID",
  "FLAG_TYPE", "MON_ATTITUDE", "MON_ALERTNESS", "SUBTYPE_ID",
  "NON_PASSDOOR_ID", "MON_APPEARANCE", "ROOMDOOR_ID", "IF_ID", "ELSE_ID",
  "TERRAIN_ID", "HORIZ_OR_VERT", "REPLACE_TERRAIN_ID",
  "LOCATION_SUBTYPE_ID", "DOOR_SUBTYPE", "BRAZIER_SUBTYPE",
  "SIGNPOST_SUBTYPE", "EXIT_ID", "SHUFFLE_ID", "QUANTITY_ID", "BURIED_ID",
  "LOOP_ID", "FOR_ID", "TO_ID", "SWITCH_ID", "CASE_ID", "BREAK_ID",
  "DEFAULT_ID", "ERODED_ID", "TRAPPED_STATE", "RECHARGED_ID", "INVIS_ID",
  "GREASED_ID", "INDESTRUCTIBLE_ID", "FEMALE_ID", "WAITFORU_ID",
  "CANCELLED_ID", "REVIVED_ID", "AVENGE_ID", "FLEEING_ID", "BLINDED_ID",
  "PARALYZED_ID", "STUNNED_ID", "CONFUSED_ID", "SEENTRAPS_ID", "ALL_ID",
  "MONTYPE_ID", "OBJTYPE_ID", "TERTYPE_ID", "TERTYPE2_ID",
  "LEVER_EFFECT_TYPE", "SWITCHABLE_ID", "CONTINUOUSLY_USABLE_ID",
  "TARGET_ID", "TRAPTYPE_ID", "EFFECT_FLAG_ID", "GRAVE_ID", "BRAZIER_ID",
  "SIGNPOST_ID", "ERODEPROOF_ID", "FUNCTION_ID", "MSG_OUTPUT_TYPE",
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
  "special_region_detail", "special_levregion_detail",
  "special_tileset_detail", "region_detail", "@16", "region_detail_end",
  "altar_detail", "anvil_detail", "floor_detail", "subtype_detail",
  "npc_detail", "grave_detail", "brazier_detail", "signpost_detail",
  "gold_detail", "engraving_detail", "mineralize", "trap_name",
  "room_type", "optroomregionflags", "roomregionflags", "roomregionflag",
  "optfloormaintype", "floormaintype", "optfloortype", "floortype",
  "optmontype", "door_state", "light_state", "alignment", "alignment_prfx",
  "altar_type", "a_register", "string_or_var", "integer_or_var",
  "coord_or_var", "encodecoord", "humidity_flags", "region_or_var",
  "encoderegion", "mapchar_or_var", "mapchar", "monster_or_var",
  "encodemonster", "object_or_var", "encodeobj", "string_expr",
  "math_expr_var", "func_param_type", "func_param_part", "func_param_list",
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
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,    44,    58,    40,    41,    91,    93,
     123,   125,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,    43,    45,
      42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   257,   258,   258,   259,   259,   260,   261,   261,   262,
     262,   262,   262,   263,   264,   265,   265,   266,   266,   267,
     267,   268,   268,   269,   269,   270,   270,   271,   271,   272,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   276,   276,   276,
     277,   277,   278,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   280,   280,
     281,   281,   282,   282,   283,   283,   284,   284,   285,   285,
     286,   286,   288,   289,   287,   290,   291,   292,   292,   293,
     293,   293,   295,   296,   294,   297,   297,   299,   298,   300,
     298,   301,   302,   302,   303,   305,   304,   307,   306,   309,
     308,   311,   310,   312,   313,   312,   314,   315,   315,   315,
     316,   316,   317,   318,   320,   319,   322,   321,   323,   323,
     324,   324,   325,   325,   326,   326,   327,   327,   328,   328,
     329,   329,   330,   330,   331,   331,   332,   332,   333,   333,
     334,   334,   334,   334,   334,   334,   334,   335,   335,   335,
     336,   336,   337,   337,   338,   339,   338,   340,   341,   341,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   343,   343,   343,
     344,   345,   344,   346,   347,   347,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   349,   350,   351,   351,   352,   352,   352,
     353,   354,   355,   356,   357,   358,   359,   359,   360,   361,
     362,   362,   363,   364,   364,   365,   365,   365,   365,   365,
     365,   365,   365,   365,   365,   365,   365,   365,   366,   366,
     366,   366,   367,   368,   369,   369,   370,   371,   372,   373,
     374,   374,   375,   376,   377,   379,   378,   380,   380,   381,
     381,   382,   382,   383,   384,   385,   385,   386,   386,   386,
     387,   387,   388,   388,   388,   389,   390,   391,   391,   392,
     392,   393,   393,   394,   394,   395,   395,   396,   396,   396,
     397,   397,   398,   399,   399,   400,   401,   401,   402,   402,
     403,   403,   404,   404,   404,   405,   405,   405,   406,   406,
     407,   408,   408,   408,   409,   410,   410,   410,   410,   411,
     411,   411,   412,   412,   413,   413,   413,   414,   415,   415,
     415,   416,   416,   417,   417,   417,   418,   418,   418,   418,
     419,   419,   419,   420,   420,   420,   420,   420,   421,   421,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   423,   423,   424,   425,   425,   426,   426,   427,   427,
     428,   428,   429,   429,   430,   430,   430,   430,   430,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   430,   430,
     430,   430,   430,   431,   431,   432,   433,   433,   434,   434,
     434,   435,   435,   435,   435,   436,   436,   437,   437,   438,
     438,   439,   439,   440,   440,   441
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     5,     3,     5,     5,     5,     3,
       3,     5,     5,     5,     7,     7,     7,     5,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     0,     0,     8,     4,     1,     0,     1,     1,
       5,     3,     0,     0,     9,     0,     2,     0,     5,     0,
       4,     1,     2,     1,     6,     0,     3,     0,     6,     0,
       4,     0,     4,     1,     0,     4,     3,     1,     3,     3,
       5,     5,     7,     4,     0,    13,     0,    15,     0,     2,
       5,     1,     5,     1,     5,     1,     5,     1,    10,     6,
       1,     1,     1,     1,     1,     3,     1,     1,     0,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     7,     5,
       1,     1,     1,     1,     3,     0,     5,     4,     0,     3,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       3,     3,     3,     1,     1,     3,     1,     1,     1,     3,
       3,     0,     5,     2,     0,     3,     1,     3,     1,     3,
       3,     1,     1,     3,     1,     1,     1,     3,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     5,     7,     5,     8,     1,     3,     3,
       5,     5,     7,     7,     6,     5,     0,     2,     5,     3,
      11,    13,     6,     0,     3,     1,     3,     3,     3,     3,
       1,     1,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     3,     3,     1,     5,     9,     5,     3,     3,
       5,     9,     5,     5,     5,     0,    13,     0,     1,     7,
      11,     3,     7,     7,     5,     5,     9,     5,     5,     3,
       7,     3,     7,     7,     3,     5,     7,     9,     1,     1,
       1,     1,     1,     0,     2,     1,     3,     1,     1,     1,
       0,     3,     1,     0,     3,     1,     0,     3,     1,     1,
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
       2,     0,     0,     0,     3,     4,    23,     0,     0,     1,
       5,     0,    27,     0,     7,     0,   149,     0,     0,     0,
       0,   217,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   287,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   358,     0,     0,
       0,     0,     0,     0,     0,     0,   177,     0,     0,     0,
       0,     0,     0,     0,     0,   146,     0,     0,     0,   152,
     161,     0,     0,     0,     0,     0,     0,   109,    98,    89,
      99,    90,   100,    91,   101,    92,   102,    93,   103,    94,
     104,    95,   105,    96,   106,    97,    31,    32,    34,     6,
      27,   107,   108,     0,    46,    45,    63,    64,    61,     0,
      56,    62,   165,    57,    58,    60,    59,    30,    74,    44,
      80,    79,    48,    67,    69,    70,    87,    49,    68,    88,
      65,    84,    85,    73,    86,    43,    52,    53,    54,    66,
      81,    72,    83,    82,    47,    71,    75,    76,    77,    33,
      78,    37,    38,    36,    35,    39,    40,    41,    42,    55,
      50,    51,     0,    26,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
       0,     0,     0,     0,   110,   111,     0,     0,     0,     0,
       0,     0,   430,     0,   433,     0,   475,     0,   431,   452,
      28,     0,   169,     0,     8,     0,   391,   392,     0,   428,
     176,     0,     0,     0,    11,   477,   476,    13,   400,     0,
     220,   221,     0,     0,   397,     0,     0,   188,   395,    14,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   471,   454,   473,     0,   424,   426,   427,     0,
     423,   421,     0,   250,   254,   420,   251,   417,   419,     0,
     416,   414,     0,   224,     0,   413,   360,   359,     0,   378,
     379,     0,     0,     0,   289,   288,     0,   405,     0,     0,
     404,     0,     0,     0,     0,   493,     0,     0,   341,     0,
       0,     0,     0,     0,   328,   329,   362,   361,     0,   147,
       0,     0,     0,     0,   394,     0,     0,     0,     0,     0,
     299,     0,   323,   322,   480,   478,   479,   179,   178,     0,
       0,     0,     0,   200,   201,     0,     0,     0,     0,   112,
       0,     0,     0,   349,   351,   354,   142,     0,     0,     0,
       0,   151,     0,     0,     0,     0,     0,   449,   448,   450,
     453,     0,   486,   488,   485,   487,   489,   490,     0,     0,
       0,   119,   120,   115,   113,     0,     0,     0,     0,    27,
     166,    25,     0,     0,     0,     0,     0,   402,     0,     0,
       0,     0,     0,     0,     0,   455,   456,     0,     0,     0,
     464,     0,     0,     0,   470,     0,     0,     0,     0,     0,
       0,   253,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   148,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   173,   172,     0,     0,   167,     0,
       0,     0,     0,     0,   446,   432,   440,     0,     0,   435,
     436,   437,   438,   439,     0,   145,     0,   430,     0,     0,
       0,     0,   136,   134,   140,   138,     0,     0,     0,   170,
       0,     0,   429,    10,   324,     0,     9,     0,     0,   401,
       0,     0,     0,   223,   222,   188,   189,   219,     0,     0,
     204,     0,     0,     0,     0,   411,     0,     0,   409,     0,
       0,   408,     0,     0,   472,   474,   334,     0,     0,     0,
     252,     0,     0,   226,   228,   283,   208,     0,   285,     0,
       0,   380,   381,     0,   332,     0,     0,   333,   330,   384,
       0,   382,     0,   383,     0,   345,   290,     0,   291,     0,
     191,     0,     0,     0,     0,   296,   295,     0,     0,   180,
     181,   355,   491,   492,     0,   298,     0,     0,   303,     0,
     193,     0,     0,   344,     0,     0,     0,   327,     0,     0,
     163,     0,     0,   153,   348,   347,     0,     0,     0,   444,
     447,     0,   434,   150,   451,   114,     0,     0,   123,     0,
     122,     0,   121,     0,   127,     0,   118,     0,   117,     0,
     116,    29,   393,     0,     0,   403,   396,     0,   398,     0,
     457,     0,     0,     0,   459,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   422,   483,   481,   482,   265,   262,
     256,     0,   282,     0,     0,     0,   281,     0,     0,     0,
       0,     0,     0,     0,   261,     0,   266,     0,   268,   269,
     280,     0,   264,   255,   270,   484,   258,     0,   415,   227,
     199,     0,     0,     0,   406,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   183,     0,     0,
     294,     0,     0,     0,     0,     0,   302,     0,     0,     0,
       0,     0,     0,   168,   162,   164,     0,     0,     0,     0,
       0,   143,     0,   135,   137,   139,   141,     0,   128,     0,
     130,     0,   132,     0,     0,   399,   218,     0,   205,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   425,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   418,     0,     0,   284,    19,     0,   363,
       0,     0,     0,     0,   389,   388,   339,   342,     0,   292,
       0,   195,     0,     0,   293,   297,     0,     0,   356,     0,
       0,     0,   343,     0,   197,     0,   363,   203,     0,   202,
     175,     0,   155,   350,   353,   352,   441,   442,   443,   445,
       0,     0,   126,     0,   125,     0,   124,     0,     0,   458,
     460,     0,   465,     0,   461,     0,   410,   463,   462,     0,
     279,   272,   271,   275,   273,   274,   276,   277,   278,   259,
     260,   263,   267,   257,     0,   385,   231,   232,     0,   236,
     235,   246,   237,   238,   239,     0,     0,     0,   243,   244,
       0,   229,   233,   386,   230,     0,   214,   215,     0,   216,
       0,   213,   209,     0,   286,     0,     0,   370,     0,     0,
       0,   390,     0,     0,     0,     0,     0,     0,   206,   207,
       0,     0,     0,   305,     0,     0,     0,     0,     0,     0,
       0,   310,   311,     0,     0,     0,   304,     0,     0,   370,
       0,     0,     0,     0,     0,   155,   144,   129,   131,   133,
     325,     0,     0,     0,   412,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,    20,     0,   367,   368,   369,
     364,   365,     0,   373,     0,     0,   331,     0,   346,   190,
       0,   363,   357,   182,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   192,     0,   373,   208,
     326,     0,   159,   154,   156,     0,   466,     0,   467,   436,
     387,   240,   241,   242,   248,   247,   245,   211,   212,   210,
       0,     0,     0,     0,   376,     0,     0,     0,     0,   370,
       0,     0,   375,   315,   317,   318,   319,   321,   320,   314,
     306,   307,   308,   309,   312,   313,   316,     0,   376,   198,
     157,    27,     0,     0,     0,     0,     0,   407,   366,   372,
     371,     0,     0,   335,     0,   495,   340,   194,   373,     0,
     300,   196,   184,    27,   160,     0,   468,    16,     0,   249,
     374,     0,   337,   494,   376,     0,     0,   158,     0,     0,
     377,   338,   336,   186,   301,   185,     0,    17,     0,    21,
      22,    19,     0,     0,   187,    12,    18,   469
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    96,    97,    98,  1045,  1093,
     894,  1091,    12,   164,    99,   400,   100,   101,   102,   103,
     216,   104,   105,   747,   749,   751,   498,   499,   500,   501,
     106,   484,   830,   107,   108,   458,   109,   110,   217,   736,
     934,   935,  1063,  1041,   111,   612,   112,   113,   233,   114,
     609,   115,   398,   116,   356,   475,   605,   117,   118,   119,
     336,   328,   120,  1076,   121,  1088,   414,   582,   602,   803,
     816,   122,   355,   818,   531,   910,   700,   892,   123,   256,
     525,   124,   435,   293,   699,   881,  1006,   125,   432,   283,
     431,   693,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   720,   136,   137,   138,   139,   726,   926,  1029,
     140,   141,   516,   142,   143,   144,   145,   146,   147,   148,
     149,   150,  1072,  1082,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   298,   329,   897,   960,   961,
     963,  1050,  1014,  1023,  1053,   301,   563,   572,   882,   796,
     573,   239,   333,   273,   258,   408,   309,   310,   540,   541,
     294,   295,   284,   285,   377,   334,   828,   619,   620,   621,
     379,   380,   381,   274,   426,   228,   247,   348,   696,   395,
     396,   397,   594,   314,   315
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -969
static const yytype_int16 yypact[] =
{
     337,  -103,   -88,   120,  -969,   337,    14,   -45,   -39,  -969,
    -969,   -23,   775,    -3,  -969,    74,  -969,    -2,    17,    58,
     132,  -969,   136,   143,   165,   183,   199,   207,   209,   227,
     239,   242,   245,   246,   247,   249,   251,   253,   254,   255,
     276,   281,   282,   284,   285,   286,   290,   291,   306,   307,
     308,   315,   326,   330,   332,   348,   349,   351,   352,   353,
     356,   359,    12,   360,   362,  -969,   364,    84,   654,  -969,
    -969,   366,   368,   373,    60,   121,   140,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
     775,  -969,  -969,   153,  -969,  -969,  -969,  -969,  -969,   378,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,  -969,   313,   194,  -969,  -114,   193,    81,   335,   340,
     839,   124,   124,   123,   -33,    44,   -47,   -47,   826,   -86,
     -86,   -28,   270,   -47,   -47,   393,   -47,   -30,   -86,   -86,
     -19,   -28,   -28,   -28,   121,   328,   121,   -47,   839,   839,
     -47,   839,   839,   108,   -47,   839,   -19,   839,    30,  -969,
     839,   -86,   708,   121,  -969,  -969,   279,   331,   -47,   -47,
     -47,   346,  -969,    63,  -969,   376,  -969,   248,  -969,    54,
    -969,    11,  -969,   387,  -969,    74,  -969,  -969,   392,  -969,
     339,   397,   400,   406,  -969,  -969,  -969,  -969,  -969,   411,
    -969,  -969,   410,   604,  -969,   413,   418,   419,  -969,  -969,
     -86,   -86,   -47,   -47,   421,   -47,   422,   424,   426,   839,
     432,   471,  -969,  -969,   358,   435,  -969,  -969,  -969,   613,
    -969,  -969,   436,  -969,  -969,  -969,  -969,  -969,  -969,   627,
    -969,  -969,   438,   437,   451,  -969,  -969,  -969,   452,  -969,
    -969,   453,   454,   460,  -969,  -969,   659,  -969,   468,   487,
    -969,   488,   489,   679,   490,  -969,   493,   505,   507,   509,
     512,   703,   514,   515,  -969,  -969,  -969,  -969,   516,   712,
     527,   529,   532,   534,   135,   721,   541,   241,   543,   547,
    -969,   548,  -969,  -969,  -969,  -969,  -969,  -969,  -969,   549,
     553,   556,   557,  -969,  -969,   562,   387,   568,   569,  -969,
     565,   121,   121,   571,   583,   585,  -969,   591,   375,   121,
     121,  -969,   121,   121,   121,   121,   121,   339,   135,  -969,
     586,   593,  -969,  -969,  -969,  -969,  -969,  -969,   587,   101,
      24,  -969,  -969,   339,   135,   596,   609,   610,   775,   775,
    -969,  -969,   121,  -114,   803,    36,   809,   620,   616,   839,
     623,   121,   287,   821,   635,  -969,  -969,   636,   645,   667,
    -969,   -47,   -47,   211,  -969,   649,   646,   839,   766,   666,
     121,   668,   387,   670,   121,   387,   -47,   -47,   839,   784,
     789,   674,   121,    23,   835,   870,   682,   843,   844,   296,
     722,   -47,   800,   694,   804,   -28,   -12,  -969,   695,   -28,
     -28,   -28,   121,   697,   115,   -47,    70,   802,   -17,   731,
     806,    -6,   894,    44,   777,  -969,   118,   118,  -969,   -72,
     704,    -5,   779,   786,   864,  -969,  -969,   334,   420,    41,
      41,  -969,  -969,  -969,    54,  -969,   839,   715,   -97,   -83,
     -55,   -34,  -969,  -969,   339,   135,   243,   141,    21,  -969,
     713,   444,  -969,  -969,  -969,   916,  -969,   727,   411,  -969,
     726,   923,   463,  -969,  -969,   419,  -969,  -969,   -47,   -47,
     676,   735,   736,   741,   743,  -969,   744,   277,  -969,   742,
     745,  -969,   748,   749,  -969,  -969,  -969,   746,   481,   531,
    -969,   750,   518,  -969,  -969,  -969,  -969,   756,   757,   940,
     523,  -969,  -969,   759,  -969,   760,   952,  -969,   761,  -969,
     762,  -969,   763,  -969,   764,   767,  -969,   954,  -969,   768,
    -969,   960,   771,    23,   772,   773,  -969,   774,   892,  -969,
    -969,  -969,  -969,  -969,   776,  -969,   778,   780,  -969,   782,
    -969,   967,   783,  -969,   787,   851,   975,  -969,   788,   387,
    -969,   729,   121,  -969,  -969,   339,   790,   815,   838,  -969,
     836,   837,  -969,  -969,  -969,  -969,  1032,   841,  -969,    53,
    -969,   121,  -969,  -114,  -969,    83,  -969,   155,  -969,    66,
    -969,  -969,  -969,   845,  1035,  -969,  -969,   848,  -969,   846,
    -969,   847,   963,   839,  -969,   121,   121,   839,   852,   121,
     839,   839,   854,   850,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,   853,  -969,   856,   857,   859,  -969,   860,   863,   865,
     866,   867,   868,   869,  -969,   875,  -969,   877,  -969,  -969,
    -969,   878,  -969,  -969,  -969,  -969,  -969,   862,  -969,   871,
     880,    44,  1037,   899,  -969,   -19,  1090,   901,   937,  1094,
      69,   123,   950,   148,  1025,   918,    -4,  -969,   911,  1028,
    -969,   121,   921,  -114,  1052,   -47,   924,  1024,   925,    -1,
     333,   387,   118,  -969,  -969,   135,   920,    23,    59,   208,
     864,  -969,   172,  -969,  -969,   135,   339,    50,  -969,    82,
    -969,    94,  -969,    23,   927,  -969,  -969,   121,  -969,   926,
     257,   347,   928,    23,   535,   929,   930,   121,  -969,   124,
    1023,  1026,   121,  1027,  1022,   121,   121,   121,  -114,   121,
     121,   121,   123,  -969,   951,   409,  -969,   935,  1126,   939,
     941,  1127,   942,   943,  -969,  -969,   944,  -969,   945,  -969,
    1130,  -969,   343,   947,  -969,  -969,   949,    87,   339,   953,
     955,   592,  -969,  1132,  -969,  1135,   939,  -969,   956,  -969,
    -969,   957,   106,  -969,  -969,   339,  -969,  -969,  -969,  -969,
     387,    83,  -969,   155,  -969,    66,  -969,   948,  1139,   135,
    -969,  1106,  -969,   121,  -969,   958,  -969,  -969,  -969,   266,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,   339,
    -969,  -969,  -969,  -969,   214,  -969,  -969,  -969,  -114,  -969,
    -969,  -969,  -969,  -969,  -969,   959,   961,   962,  -969,  -969,
     964,  -969,  -969,  -969,   339,   965,  -969,  -969,   966,  -969,
     968,  -969,  -969,  1145,  -969,   972,   379,  1055,  1154,   973,
     123,  -969,  1002,   123,   971,   976,    -1,   121,  -969,  -969,
     974,  1104,  1084,  -969,   978,   979,   980,   981,   982,   983,
     984,  -969,  -969,   985,   986,   987,  -969,   988,   989,  1055,
      87,  1177,   350,   991,   990,   106,  -969,  -969,  -969,  -969,
    -969,   993,   992,   329,  -969,   121,  1110,   339,   121,   121,
     121,   -66,   124,  1186,  1064,  -969,  1189,  -969,  -969,  -969,
    -969,  1000,  1001,  1101,  1003,  1194,  -969,  1004,  -969,  -969,
     287,   939,  -969,  -969,  1006,  1007,  1109,  1199,    25,   123,
     124,    36,    36,   -47,   -33,  1200,  -969,  1201,  1101,  -969,
    -969,  1011,  -969,  -969,  -969,  1202,  -969,  1168,  -969,   -52,
    -969,  -969,  -969,  -969,  -969,   969,  -969,  -969,  -969,  -969,
    1012,   379,  1114,  1016,  1053,  1208,  1017,   123,  1018,  1055,
    1128,  1131,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  1020,  1053,   880,
    -969,   775,  1029,  1030,  1033,  1031,   -66,  -969,  -969,  -969,
    -969,  1109,  1034,  -969,  1036,  -969,  -969,  -969,  1101,  1038,
    -969,  -969,  -969,   775,  -969,    23,  -969,  -969,  1040,  -969,
    -969,   123,   387,  -969,  1053,  1133,   387,  -969,  1041,   -47,
    -969,  -969,  -969,  -969,  -969,  -969,    52,  1042,   387,  -969,
    -969,   935,   -47,  1043,  -969,  -969,  -969,  -969
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -969,  -969,  1214,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
     130,  -969,  -969,   995,  -100,  -352,   824,  1014,  1163,  -477,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  1175,  -969,  -969,  -969,
     303,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
     781,  1044,  -969,  -969,  -969,  -969,   714,  -969,  -969,  -969,
     336,  -969,  -969,  -969,  -589,   311,   258,  -969,  -969,   441,
     274,  -969,  -969,  -969,  -969,  -969,   200,  -969,  -969,  1076,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,  -969,  -551,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,   265,   546,  -790,   244,  -969,
    -879,  -969,  -907,   201,  -968,  -460,  -578,  -969,  -969,  -969,
     469,   855,  -210,  -167,  -369,   738,   192,  -379,  -454,  -601,
    -495,  -594,  -492,  -591,  -163,   -67,  -969,   517,  -969,  -969,
     765,  -969,  -969,   994,  -123,   705,  -969,  -458,  -969,  -969,
    -969,  -969,  -969,  -131,  -969
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -226
static const yytype_int16 yytable[] =
{
     230,   257,   240,   360,   474,   717,   590,   618,   227,   302,
     303,   503,   638,   604,   636,   222,   317,   318,    16,   320,
     322,   502,   607,   608,   535,   312,   929,   312,   222,  1025,
     338,   248,   249,   341,   382,   353,   383,   349,   752,   514,
     384,   385,   386,   750,   748,   296,   596,   275,   248,   249,
     988,   363,   364,   365,   640,   305,   323,  1089,   222,   326,
     330,   331,   332,   758,   391,   610,   580,   222,   393,   535,
    1062,   367,   600,   614,   801,   339,   340,   814,   342,   343,
     550,  1038,   350,   553,   352,   245,   276,   357,   236,   248,
     249,   908,     7,  1004,   561,   417,   418,   627,   420,   237,
     238,   562,   248,   249,   628,   497,  1083,     8,   354,   367,
     306,   629,   344,  1026,   299,  1027,   345,   346,   630,   344,
       9,   535,   300,   345,   346,   222,   287,   276,    11,   337,
    1090,   248,   249,   307,   308,   387,  1005,   824,   252,   631,
    1058,   819,  1044,   592,   287,   -15,   632,   794,   593,   253,
     597,  1074,   480,  1028,   795,   252,   368,    13,   287,   823,
     633,   277,   378,    14,   394,   909,   321,   634,   313,   297,
     254,   255,    15,   246,   278,   837,   372,   373,   374,   375,
     376,  1019,   611,   327,   581,   845,   347,   254,   255,   163,
     601,   162,   802,   165,   388,   815,   252,   236,   374,   375,
     376,   288,   277,   241,   242,   243,   244,   389,   237,   238,
      75,   390,   166,   236,   535,   278,   797,   606,   536,   288,
     389,   639,   224,   225,   237,   238,   236,   504,   254,   255,
     307,   308,   515,   288,   939,   224,   225,   237,   238,   938,
     937,   786,   538,   539,   831,   932,   276,   933,   743,   253,
     223,   832,   587,   167,   533,   534,   236,   733,   226,   223,
     744,   236,   606,   618,   221,   224,   225,   237,   238,   554,
     555,   226,   237,   238,   224,   225,   833,   850,   821,   279,
     658,   410,   213,   834,   575,   280,   520,   863,   835,   248,
     249,   374,   375,   376,   479,   836,   532,   223,   591,   510,
     542,   226,   487,   488,   545,   489,   490,   491,   492,   493,
     226,   335,   224,   225,   606,   556,   234,   223,   615,   289,
     279,   277,   368,   505,   579,   290,   280,   168,   584,   585,
     586,   169,   224,   225,   278,   511,   229,   289,   170,   538,
     539,   637,   791,   290,   522,   755,   291,   292,   226,   281,
     282,   289,     1,     2,   344,   248,   249,   290,   345,   346,
     171,   650,   651,   548,   291,   292,   788,   552,   226,   755,
     523,   524,   311,   625,   569,   560,   570,   571,   172,   820,
     324,   325,   694,   372,   373,   374,   375,   376,   235,   260,
     261,   262,   263,   264,   173,   265,   252,   266,   267,   268,
     269,   270,   174,   358,   175,   966,   231,   537,   968,   946,
     530,   817,   709,   248,   249,   826,   827,   250,   251,   957,
     958,   959,   176,   392,   370,   250,   251,   378,   254,   255,
    1032,  1033,   538,   539,   177,   465,   259,   178,   272,   279,
     179,   180,   181,   635,   182,   280,   183,   371,   184,   185,
     186,   841,   415,   416,   842,   260,   261,   262,   263,   264,
    1007,   265,   252,   266,   267,   268,   269,   270,   281,   282,
     746,   187,   316,   271,   991,   410,   188,   189,   936,   190,
     191,   192,   319,   885,  1030,   193,   194,  1078,  1031,   372,
     373,   374,   375,   376,   254,   255,   372,   373,   374,   375,
     376,   195,   196,   197,   272,   372,   373,   374,   375,   376,
     198,   806,   886,   887,   372,   945,   374,   375,   376,   888,
     252,   199,  1056,   997,   335,   200,   998,   201,   889,   362,
     759,   253,   361,   622,   762,   665,   890,   765,   766,   666,
     667,   843,   366,   202,   203,   735,   204,   205,   206,   248,
     249,   207,   254,   255,   208,   210,   891,   211,   810,   212,
     808,   218,   853,   219,   745,   856,   857,   858,   220,   860,
     861,   862,   486,   232,   369,   825,  1080,   372,   373,   374,
     375,   376,   372,   373,   374,   375,   376,   399,   760,   761,
     402,   404,   764,   403,   405,   372,   373,   374,   375,   376,
     406,   668,   669,   670,   407,   671,   409,   672,   410,   248,
     249,   411,   412,   413,   427,   859,   429,   419,   421,   623,
     422,   884,   423,   372,   373,   374,   375,   376,   425,   428,
     433,   673,   674,   675,   430,   676,   434,  -225,   677,   678,
     679,   680,   681,   642,   682,   436,   437,   438,   439,   260,
     261,   262,   263,   264,   440,   265,   252,   266,   267,   268,
     269,   270,   648,   441,   683,   684,   442,   271,   372,   373,
     374,   375,   376,   685,   686,   687,   688,   689,   690,   913,
     664,   443,   444,   446,   447,   445,   914,   448,   254,   255,
     839,   691,   372,   373,   374,   375,   376,   972,   272,   449,
     849,   450,   915,   451,   692,   947,   452,   453,   454,   455,
     456,   372,   373,   374,   375,   376,   252,   698,   457,   916,
    1081,   459,   704,   460,  1085,   463,   461,   253,   462,   372,
     373,   374,   375,   376,   846,   464,  1094,   466,  1001,  1002,
    1003,   467,   468,   469,   530,   248,   249,   470,   254,   255,
     471,   472,   917,   918,   919,   920,   473,   921,   922,   923,
     924,   925,   476,   477,   478,   481,   372,   373,   374,   375,
     376,   372,   373,   374,   375,   376,   943,   482,   226,   483,
     494,    16,   496,   372,   373,   374,   375,   376,   485,    17,
     495,   506,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,   507,   508,   513,    30,    31,    32,
      33,    34,   517,    35,   518,   519,  1034,   521,    36,    37,
      38,    39,    40,    41,    42,    43,   526,    44,    45,    46,
     528,    47,    48,    49,    50,    51,    52,    53,   527,   529,
     543,    54,    55,   544,    56,   260,   261,   262,   263,   264,
      57,   265,   252,   266,   267,   268,   269,   270,   546,   214,
     547,   557,   549,   271,   551,    78,   558,    80,   559,    82,
      58,    84,   564,    86,   565,    88,   566,    90,   999,    92,
     567,    94,   574,   568,   254,   255,   576,    59,   577,   583,
     578,   588,   595,    60,   272,   598,    61,    62,   603,    63,
    -174,    64,   599,   613,   248,   249,    65,    66,   616,   626,
      67,    68,  1087,    69,   641,    70,   617,   248,   249,   643,
      79,   644,    81,   646,    83,  1096,    85,   647,    87,   653,
      89,   652,    91,   654,    93,   655,    95,   656,   657,   660,
     659,  1064,   661,   662,   703,    71,    72,    73,   663,    74,
     701,   702,   697,   705,   706,   708,   707,   710,   713,   711,
     709,   712,   714,  1077,   715,   716,   718,   719,   721,   722,
     723,   728,   724,    75,   731,   725,   727,   729,   658,    76,
      77,   730,   732,   734,   737,   304,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,   260,   261,   262,   263,   264,   738,
     265,   252,   266,   267,   268,   269,   270,   260,   261,   262,
     263,   264,   271,   265,   252,   266,   267,   268,   269,   270,
     740,   864,   865,   739,   741,   271,   742,   306,   754,   753,
     530,   757,   787,   254,   255,   755,   763,   768,   769,   756,
     767,   770,   771,   272,   772,   773,   254,   255,   774,   783,
     775,   776,   777,   778,   779,   784,   272,   866,   867,    77,
     780,   868,   781,   782,   785,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,   788,   790,   791,   869,   792,   793,   870,
     871,   872,   873,   874,   875,   876,   877,   878,   879,   880,
     798,   799,   800,   804,   805,   807,   809,   812,   811,   813,
     822,   838,   851,   840,   852,   844,   847,   848,   855,   893,
     895,   899,   854,   896,   904,   898,   927,   900,   902,   928,
     903,   906,   901,   907,   941,   940,   942,   911,   955,   912,
     930,   931,   962,   236,   948,   944,   949,   950,   964,   951,
     952,   953,   967,   954,   237,   238,   956,   965,   969,   974,
     970,   973,   975,   976,   977,   978,   979,   980,   981,   982,
     983,   984,   985,   987,   990,   986,   992,   995,  1000,   996,
    1008,   993,  1009,  1010,  1011,  1013,  1012,  1015,  1016,  1017,
    1020,  1021,  1022,  1024,  1036,  1037,  1040,  1042,  1043,  1047,
    1049,  1051,  1054,  1052,  1055,  1057,  1059,  1061,  1060,    10,
    1084,  1095,   509,  1065,  1046,  1067,   359,  1066,  1068,  1071,
     401,   215,  1075,  1073,  1079,  1086,  1092,   209,   994,   649,
    1097,   989,   971,   905,  1018,   589,  1069,  1039,   286,  1035,
     351,   789,  1070,   883,   695,  1048,   645,   829,   512,   624,
       0,     0,     0,   424
};

static const yytype_int16 yycheck[] =
{
     100,   168,   165,   213,   356,   583,   464,   484,    75,   176,
     177,   390,   507,   473,   506,     4,   183,   184,     6,   186,
     187,   390,   476,   477,     3,    55,   816,    55,     4,     4,
     197,    78,    79,   200,    23,     5,    25,   204,   639,     3,
      29,    30,    31,   637,   635,    78,    63,   170,    78,    79,
     929,   218,   219,   220,   508,   178,   187,     5,     4,    78,
     191,   192,   193,   652,   231,   137,    78,     4,   231,     3,
    1038,     8,    78,    78,    78,   198,   199,    78,   201,   202,
     432,   988,   205,   435,   207,     4,     3,   210,   202,    78,
      79,     4,   195,   159,    71,   262,   263,   194,   265,   213,
     214,    78,    78,    79,   201,     4,  1074,   195,    78,     8,
     196,   194,     4,    88,    70,    90,     8,     9,   201,     4,
       0,     3,    78,     8,     9,     4,     3,     3,   114,   196,
      78,    78,    79,   219,   220,   124,   202,    78,   185,   194,
    1019,   730,   194,    73,     3,   197,   201,    78,    78,   196,
     167,  1058,   362,   128,    85,   185,   223,   202,     3,   737,
     194,    78,   229,   202,   231,    78,   196,   201,   196,   202,
     217,   218,   195,    92,    91,   753,   248,   249,   250,   251,
     252,   971,   254,   202,   196,   763,    78,   217,   218,   115,
     196,   194,   196,   195,   183,   196,   185,   202,   250,   251,
     252,    78,    78,    10,    11,    12,    13,   196,   213,   214,
     198,   200,   195,   202,     3,    91,   711,   196,     7,    78,
     196,   200,   211,   212,   213,   214,   202,   390,   217,   218,
     219,   220,   196,    78,   835,   211,   212,   213,   214,   833,
     831,   701,   221,   222,   194,   139,     3,   141,   627,   196,
     196,   201,   462,   195,   421,   422,   202,   609,   247,   196,
     629,   202,   196,   740,   204,   211,   212,   213,   214,   436,
     437,   247,   213,   214,   211,   212,   194,   769,   732,   196,
       3,     4,   198,   201,   451,   202,   409,   782,   194,    78,
      79,   250,   251,   252,   361,   201,   419,   196,   465,   399,
     423,   247,   369,   370,   427,   372,   373,   374,   375,   376,
     247,   196,   211,   212,   196,   438,     3,   196,   481,   196,
     196,    78,   389,   390,   455,   202,   202,   195,   459,   460,
     461,   195,   211,   212,    91,   402,   196,   196,   195,   221,
     222,   200,   194,   202,   411,   197,   223,   224,   247,   225,
     226,   196,    15,    16,     4,    78,    79,   202,     8,     9,
     195,   528,   529,   430,   223,   224,   194,   434,   247,   197,
      83,    84,   180,   496,    78,   442,    80,    81,   195,   731,
     188,   189,   549,   248,   249,   250,   251,   252,   194,   178,
     179,   180,   181,   182,   195,   184,   185,   186,   187,   188,
     189,   190,   195,   211,   195,   900,   253,   196,   903,   195,
      77,    78,   198,    78,    79,   207,   208,    82,    83,    40,
      41,    42,   195,   231,   176,    82,    83,   494,   217,   218,
     981,   982,   221,   222,   195,   194,    96,   195,   227,   196,
     195,   195,   195,   200,   195,   202,   195,   199,   195,   195,
     195,   194,   260,   261,   197,   178,   179,   180,   181,   182,
     952,   184,   185,   186,   187,   188,   189,   190,   225,   226,
     633,   195,   202,   196,   932,     4,   195,   195,   830,   195,
     195,   195,    89,    74,   979,   195,   195,  1065,   980,   248,
     249,   250,   251,   252,   217,   218,   248,   249,   250,   251,
     252,   195,   195,   195,   227,   248,   249,   250,   251,   252,
     195,   721,   103,   104,   248,   249,   250,   251,   252,   110,
     185,   195,  1017,   194,   196,   195,   197,   195,   119,   198,
     653,   196,   253,   199,   657,     4,   127,   660,   661,     8,
       9,   194,   196,   195,   195,   612,   195,   195,   195,    78,
      79,   195,   217,   218,   195,   195,   147,   195,   725,   195,
     723,   195,   772,   195,   631,   775,   776,   777,   195,   779,
     780,   781,   197,   195,   198,   738,  1071,   248,   249,   250,
     251,   252,   248,   249,   250,   251,   252,   200,   655,   656,
     198,   194,   659,   254,   194,   248,   249,   250,   251,   252,
     194,    70,    71,    72,   193,    74,   196,    76,     4,    78,
      79,   198,   194,   194,   256,   778,     3,   196,   196,   199,
     196,   784,   196,   248,   249,   250,   251,   252,   196,   194,
       3,   100,   101,   102,   198,   104,   198,   200,   107,   108,
     109,   110,   111,   199,   113,   194,   194,   194,   194,   178,
     179,   180,   181,   182,   194,   184,   185,   186,   187,   188,
     189,   190,   199,     4,   133,   134,   198,   196,   248,   249,
     250,   251,   252,   142,   143,   144,   145,   146,   147,    87,
     199,   194,   194,     4,   194,   196,    94,   194,   217,   218,
     757,   160,   248,   249,   250,   251,   252,   907,   227,   194,
     767,   194,   110,   194,   173,   868,   194,     4,   194,   194,
     194,   248,   249,   250,   251,   252,   185,   199,     6,   127,
    1072,   194,   199,   194,  1076,     4,   194,   196,   194,   248,
     249,   250,   251,   252,   199,   194,  1088,   194,   948,   949,
     950,   194,   194,   194,    77,    78,    79,   194,   217,   218,
     194,   194,   160,   161,   162,   163,   194,   165,   166,   167,
     168,   169,   194,   194,   199,   194,   248,   249,   250,   251,
     252,   248,   249,   250,   251,   252,   843,   194,   247,   194,
     194,     6,   195,   248,   249,   250,   251,   252,   197,    14,
     197,   195,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,   195,   195,     3,    32,    33,    34,
      35,    36,     3,    38,   194,   199,   983,   194,    43,    44,
      45,    46,    47,    48,    49,    50,     5,    52,    53,    54,
     194,    56,    57,    58,    59,    60,    61,    62,   203,   194,
     191,    66,    67,   197,    69,   178,   179,   180,   181,   182,
      75,   184,   185,   186,   187,   188,   189,   190,    92,   205,
     194,    77,   194,   196,   194,   211,    77,   213,   194,   215,
      95,   217,    37,   219,     4,   221,   194,   223,   945,   225,
      37,   227,   160,    39,   217,   218,    86,   112,   194,   194,
      86,   194,    90,   118,   227,   164,   121,   122,     4,   124,
     123,   126,    96,   199,    78,    79,   131,   132,   129,   194,
     135,   136,  1079,   138,   201,   140,   130,    78,    79,     3,
     212,   194,   214,   197,   216,  1092,   218,     4,   220,   194,
     222,   255,   224,   197,   226,   194,   228,   194,   194,   194,
     198,  1041,   194,   194,     4,   170,   171,   172,   202,   174,
     194,   194,   202,   194,   194,   194,     4,   194,     4,   195,
     198,   194,   194,  1063,     4,   194,   194,   194,   194,    77,
     194,     4,   194,   198,   123,   195,   194,   194,     3,   204,
     205,   194,   194,   254,   194,   159,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   178,   179,   180,   181,   182,   194,
     184,   185,   186,   187,   188,   189,   190,   178,   179,   180,
     181,   182,   196,   184,   185,   186,   187,   188,   189,   190,
     194,    80,    81,   195,   197,   196,     4,   196,     3,   194,
      77,   194,     5,   217,   218,   197,   194,   197,   195,   203,
     196,   195,   195,   227,   195,   195,   217,   218,   195,   197,
     195,   195,   195,   195,   195,   194,   227,   116,   117,   205,
     195,   120,   195,   195,   194,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   194,     4,   194,   145,   160,     4,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     160,    86,   194,   202,    86,   194,    64,    93,   194,   194,
     200,   194,    99,   197,    98,   197,   197,   197,   106,   194,
       4,     4,   105,   194,     4,   194,     4,   195,   194,     4,
     195,   194,   199,   194,     5,   197,    40,   194,     3,   194,
     194,   194,    97,   202,   195,   197,   195,   195,     4,   195,
     195,   195,   160,   195,   213,   214,   194,   194,   197,    65,
     194,   197,    88,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   194,     7,   197,   195,   194,    78,   197,
       4,   201,   128,     4,   194,    94,   195,   194,     4,   195,
     194,   194,    93,     4,     4,     4,   195,     5,    40,   197,
      96,   195,     4,   160,   197,   197,    88,   197,    87,     5,
      87,  1091,   398,   194,   255,   192,   212,   197,   197,   195,
     235,    68,   194,   197,   194,   194,   194,    62,   935,   525,
     197,   930,   906,   802,   970,   464,  1046,   989,   172,   984,
     206,   705,  1051,   784,   549,  1011,   518,   740,   403,   494,
      -1,    -1,    -1,   269
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   258,   259,   260,   261,   195,   195,     0,
     259,   114,   269,   202,   202,   195,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      32,    33,    34,    35,    36,    38,    43,    44,    45,    46,
      47,    48,    49,    50,    52,    53,    54,    56,    57,    58,
      59,    60,    61,    62,    66,    67,    69,    75,    95,   112,
     118,   121,   122,   124,   126,   131,   132,   135,   136,   138,
     140,   170,   171,   172,   174,   198,   204,   205,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   262,   263,   264,   271,
     273,   274,   275,   276,   278,   279,   287,   290,   291,   293,
     294,   301,   303,   304,   306,   308,   310,   314,   315,   316,
     319,   321,   328,   335,   338,   344,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   360,   361,   362,   363,
     367,   368,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   194,   115,   270,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   293,
     195,   195,   195,   198,   205,   275,   277,   295,   195,   195,
     195,   204,     4,   196,   211,   212,   247,   422,   432,   196,
     271,   253,   195,   305,     3,   194,   202,   213,   214,   408,
     421,    10,    11,    12,    13,     4,    92,   433,    78,    79,
      82,    83,   185,   196,   217,   218,   336,   410,   411,    96,
     178,   179,   180,   181,   182,   184,   186,   187,   188,   189,
     190,   196,   227,   410,   430,   431,     3,    78,    91,   196,
     202,   225,   226,   346,   419,   420,   346,     3,    78,   196,
     202,   223,   224,   340,   417,   418,    78,   202,   392,    70,
      78,   402,   410,   410,   159,   431,   196,   219,   220,   413,
     414,   413,    55,   196,   440,   441,   202,   410,   410,    89,
     410,   196,   410,   440,   413,   413,    78,   202,   318,   393,
     440,   440,   440,   409,   422,   196,   317,   422,   410,   431,
     431,   410,   431,   431,     4,     8,     9,    78,   434,   410,
     431,   318,   431,     5,    78,   329,   311,   431,   413,   274,
     409,   253,   198,   410,   410,   410,   196,     8,   422,   198,
     176,   199,   248,   249,   250,   251,   252,   421,   422,   427,
     428,   429,    23,    25,    29,    30,    31,   124,   183,   196,
     200,   410,   413,   421,   422,   436,   437,   438,   309,   200,
     272,   270,   198,   254,   194,   194,   194,   193,   412,   196,
       4,   198,   194,   194,   323,   413,   413,   410,   410,   196,
     410,   196,   196,   196,   430,   196,   431,   256,   194,     3,
     198,   347,   345,     3,   198,   339,   194,   194,   194,   194,
     194,     4,   198,   194,   194,   196,     4,   194,   194,   194,
     194,   194,   194,     4,   194,   194,   194,     6,   292,   194,
     194,   194,   194,     4,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   272,   312,   194,   194,   199,   422,
     409,   194,   194,   194,   288,   197,   197,   422,   422,   422,
     422,   422,   422,   422,   194,   197,   195,     4,   283,   284,
     285,   286,   411,   414,   421,   422,   195,   195,   195,   273,
     271,   422,   408,     3,     3,   196,   369,     3,   194,   199,
     431,   194,   422,    83,    84,   337,     5,   203,   194,   194,
      77,   331,   431,   410,   410,     3,     7,   196,   221,   222,
     415,   416,   431,   191,   197,   431,    92,   194,   422,   194,
     272,   194,   422,   272,   410,   410,   431,    77,    77,   194,
     422,    71,    78,   403,    37,     4,   194,    37,    39,    78,
      80,    81,   404,   407,   160,   410,    86,   194,    86,   440,
      78,   196,   324,   194,   440,   440,   440,   409,   194,   317,
     434,   410,    73,    78,   439,    90,    63,   167,   164,    96,
      78,   196,   325,     4,   402,   313,   196,   415,   415,   307,
     137,   254,   302,   199,    78,   421,   129,   130,   276,   424,
     425,   426,   199,   199,   427,   431,   194,   194,   201,   194,
     201,   194,   201,   194,   201,   200,   419,   200,   417,   200,
     415,   201,   199,     3,   194,   412,   197,     4,   199,   323,
     410,   410,   255,   194,   197,   194,   194,   194,     3,   198,
     194,   194,   194,   202,   199,     4,     8,     9,    70,    71,
      72,    74,    76,   100,   101,   102,   104,   107,   108,   109,
     110,   111,   113,   133,   134,   142,   143,   144,   145,   146,
     147,   160,   173,   348,   410,   432,   435,   202,   199,   341,
     333,   194,   194,     4,   199,   194,   194,     4,   194,   198,
     194,   195,   194,     4,   194,     4,   194,   403,   194,   194,
     359,   194,    77,   194,   194,   195,   364,   194,     4,   194,
     194,   123,   194,   272,   254,   422,   296,   194,   194,   195,
     194,   197,     4,   414,   411,   422,   421,   280,   420,   281,
     418,   282,   416,   194,     3,   197,   203,   194,   331,   431,
     422,   422,   431,   194,   422,   431,   431,   196,   197,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   197,   194,   194,   402,     5,   194,   393,
       4,   194,   160,     4,    78,    85,   406,   417,   160,    86,
     194,    78,   196,   326,   202,    86,   409,   194,   421,    64,
     410,   194,    93,   194,    78,   196,   327,    78,   330,   331,
     272,   415,   200,   403,    78,   421,   207,   208,   423,   424,
     289,   194,   201,   194,   201,   194,   201,   403,   194,   422,
     197,   194,   197,   194,   197,   403,   199,   197,   197,   422,
     419,    99,    98,   409,   105,   106,   409,   409,   409,   421,
     409,   409,   409,   417,    80,    81,   116,   117,   120,   145,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   342,   405,   407,   421,    74,   103,   104,   110,   119,
     127,   147,   334,   194,   267,     4,   194,   394,   194,     4,
     195,   199,   194,   195,     4,   336,   194,   194,     4,    78,
     332,   194,   194,    87,    94,   110,   127,   160,   161,   162,
     163,   165,   166,   167,   168,   169,   365,     4,     4,   394,
     194,   194,   139,   141,   297,   298,   272,   420,   418,   416,
     197,     5,    40,   422,   197,   249,   195,   421,   195,   195,
     195,   195,   195,   195,   195,     3,   194,    40,    41,    42,
     395,   396,    97,   397,     4,   194,   417,   160,   417,   197,
     194,   327,   409,   197,    65,    88,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   197,   194,   397,   332,
       7,   434,   195,   201,   297,   194,   197,   194,   197,   422,
      78,   409,   409,   409,   159,   202,   343,   419,     4,   128,
       4,   194,   195,    94,   399,   194,     4,   195,   337,   394,
     194,   194,    93,   400,     4,     4,    88,    90,   128,   366,
     417,   419,   369,   369,   410,   392,     4,     4,   399,   333,
     195,   300,     5,    40,   194,   265,   255,   197,   395,    96,
     398,   195,   160,   401,     4,   197,   417,   197,   397,    88,
      87,   197,   401,   299,   271,   194,   197,   192,   197,   343,
     400,   195,   379,   197,   399,   194,   320,   271,   403,   194,
     417,   272,   380,   401,    87,   272,   194,   410,   322,     5,
      78,   268,   194,   266,   272,   267,   410,   197
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
#line 293 "lev_comp.y"
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
#line 316 "lev_comp.y"
    {
		      start_level_def(&splev, (yyvsp[(3) - (3)].map));
		      (yyval.map) = (yyvsp[(3) - (3)].map);
		  ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 321 "lev_comp.y"
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
#line 343 "lev_comp.y"
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
#line 356 "lev_comp.y"
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
#line 368 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_ROGUE,0,0,0,
					  0,0,0,0, SPO_INITLEVEL));
		  ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 374 "lev_comp.y"
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
		      if (joined && fg != CORR && fg != ROOM && fg != GRASS && fg != GROUND && fg != AIR && fg != CLOUD)
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
#line 403 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_TILESET));
		  ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 409 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_BOUNDARY_TYPE));
		  ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 415 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 419 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 425 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_COPY));
		      (yyval.i) = 0;
		  ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 430 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 436 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 440 "lev_comp.y"
    {
		      (yyval.i) = what_map_char((char) (yyvsp[(2) - (2)].i));
		  ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 451 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 455 "lev_comp.y"
    {
		      add_opvars(splev, "io",
                                 VA_PASS2((int) (yyvsp[(3) - (3)].i), SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 462 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 466 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 472 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 476 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 482 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (3)].i);
		  ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 580 "lev_comp.y"
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

  case 113:

/* Line 1455 of yacc.c  */
#line 596 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 602 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 608 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 614 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 620 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 626 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 632 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 638 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 644 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 654 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 664 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 674 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 684 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 694 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 704 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 716 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 721 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 728 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 733 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 740 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 745 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 752 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 756 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 762 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 767 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 774 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 778 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 784 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 788 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 794 "lev_comp.y"
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

  case 143:

/* Line 1455 of yacc.c  */
#line 816 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 820 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 831 "lev_comp.y"
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

  case 146:

/* Line 1455 of yacc.c  */
#line 886 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 892 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 896 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 902 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 909 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 913 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 921 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 925 "lev_comp.y"
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

  case 154:

/* Line 1455 of yacc.c  */
#line 948 "lev_comp.y"
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

  case 157:

/* Line 1455 of yacc.c  */
#line 993 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1003 "lev_comp.y"
    {
		  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1006 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1016 "lev_comp.y"
    {
		  ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1021 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1035 "lev_comp.y"
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

  case 165:

/* Line 1455 of yacc.c  */
#line 1072 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1077 "lev_comp.y"
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

  case 167:

/* Line 1455 of yacc.c  */
#line 1106 "lev_comp.y"
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

  case 168:

/* Line 1455 of yacc.c  */
#line 1120 "lev_comp.y"
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

  case 169:

/* Line 1455 of yacc.c  */
#line 1136 "lev_comp.y"
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

  case 170:

/* Line 1455 of yacc.c  */
#line 1156 "lev_comp.y"
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
#line 1168 "lev_comp.y"
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

  case 172:

/* Line 1455 of yacc.c  */
#line 1188 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1194 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1204 "lev_comp.y"
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

  case 175:

/* Line 1455 of yacc.c  */
#line 1222 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1232 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1238 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1243 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1248 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1255 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1262 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1271 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1279 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1290 "lev_comp.y"
    {
		      long rflags = (yyvsp[(8) - (11)].i);
		      long flmt = (long)(yyvsp[(9) - (11)].i);
		      long flt = (long)(yyvsp[(10) - (11)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      if (flmt == -1) flmt = ROOM;
		      if (flt == -1) flt = 0;

		      add_opvars(splev, "iiiiiiiiio",
				 VA_PASS10(flt, flmt, rflags, ERR, ERR,
					  (yyvsp[(5) - (11)].crd).x, (yyvsp[(5) - (11)].crd).y, (yyvsp[(7) - (11)].sze).width, (yyvsp[(7) - (11)].sze).height,
					  SPO_SUBROOM));
		      break_stmt_start();
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1306 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1313 "lev_comp.y"
    {
		      long rflags = (yyvsp[(10) - (13)].i);
		      long flmt = (long)(yyvsp[(11) - (13)].i);
		      long flt = (long)(yyvsp[(12) - (13)].i);

		      if (rflags == -1) rflags = (1 << 0);

		      add_opvars(splev, "iiiiiiiiio",
				 VA_PASS10(flt, flmt, rflags,
					  (yyvsp[(7) - (13)].crd).x, (yyvsp[(7) - (13)].crd).y, (yyvsp[(5) - (13)].crd).x, (yyvsp[(5) - (13)].crd).y,
					  (yyvsp[(9) - (13)].sze).width, (yyvsp[(9) - (13)].sze).height, SPO_ROOM));
		      break_stmt_start();
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1327 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1334 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1338 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1344 "lev_comp.y"
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

  case 191:

/* Line 1455 of yacc.c  */
#line 1354 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1360 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1369 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1375 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1380 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1386 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1391 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1397 "lev_comp.y"
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

  case 199:

/* Line 1455 of yacc.c  */
#line 1408 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1422 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1426 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1436 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1443 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1451 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1456 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1461 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1466 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1471 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1476 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1481 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1489 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1496 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1504 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1520 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1524 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1530 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1538 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1544 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1552 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1560 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1565 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1571 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1577 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1583 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1589 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1594 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1599 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1604 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1609 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1614 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1619 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1624 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1629 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1634 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1639 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1645 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1652 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1661 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1665 "lev_comp.y"
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

  case 250:

/* Line 1455 of yacc.c  */
#line 1678 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1686 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1696 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1704 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1713 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1720 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1728 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1734 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1739 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1744 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1749 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1754 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1759 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1764 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1769 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1774 "lev_comp.y"
    {
		      if ((yyvsp[(1) - (1)].i) == D_LOCKED) {
			  add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_LOCKED));
			  (yyval.i) = 0x0100;
		      } else if ((yyvsp[(1) - (1)].i) == D_BROKEN) {
			  add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BROKEN));
			  (yyval.i) = 0x0200;
		      } else if ((yyvsp[(1) - (1)].i) == D_ISOPEN) {
			  add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_OPEN));
			  (yyval.i) = 0x2000000;
		      } else
			  lc_error("DOOR state can only be locked or broken.");
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1788 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1794 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1799 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1804 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1809 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1814 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1819 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1824 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x2000000;
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1829 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x4000000;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1834 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1839 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1844 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1849 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1854 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1859 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x400000;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1864 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x800000;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1869 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x1000000;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1876 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1882 "lev_comp.y"
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

  case 285:

/* Line 1455 of yacc.c  */
#line 1911 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1916 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1924 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1930 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1934 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1942 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1949 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1956 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1966 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1976 "lev_comp.y"
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

  case 295:

/* Line 1455 of yacc.c  */
#line 1991 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2001 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2005 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2011 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2017 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2023 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2027 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2033 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2039 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2046 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2054 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2060 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2065 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2070 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2075 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2080 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2086 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2092 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2097 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2102 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2107 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2112 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2117 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2127 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2133 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2139 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2144 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2151 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2158 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2164 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2170 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2176 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2181 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2188 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2194 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2204 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2210 "lev_comp.y"
    {
		      long irr;
		      long rt = (yyvsp[(7) - (11)].i);
		      long rflags = (yyvsp[(8) - (11)].i);
		      long flmt = (long)(yyvsp[(9) - (11)].i);
		      long flt = (long)(yyvsp[(10) - (11)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      if (flmt == -1) flmt = ROOM;
		      if (flt == -1) flt = 0;

		      if (!(rflags & 1)) rt += MAXRTYPE+1;
		      irr = ((rflags & 2) != 0);
		      add_opvars(splev, "iiiiio",
				 VA_PASS6((long)(yyvsp[(5) - (11)].i), rt, rflags, flmt, flt, SPO_REGION));
		      (yyval.i) = (irr || (rflags & 1) || rt != OROOM);
		      break_stmt_start();
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2229 "lev_comp.y"
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

  case 337:

/* Line 1455 of yacc.c  */
#line 2241 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2245 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2251 "lev_comp.y"
    {
		      add_opvars(splev, "Miio",
				 VA_PASS4(-1, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2256 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2263 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2267 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2273 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2279 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2285 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2289 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2295 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2299 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2304 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2311 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2315 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2322 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2326 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2331 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2338 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2344 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2351 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2355 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2362 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2373 "lev_comp.y"
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

  case 363:

/* Line 1455 of yacc.c  */
#line 2386 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2390 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2396 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2400 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2407 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2411 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2415 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2421 "lev_comp.y"
    {
			(yyval.i) = ROOM;
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2425 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2431 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2437 "lev_comp.y"
    {
			(yyval.i) = 0;
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2441 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2447 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2453 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2458 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2475 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2483 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2493 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2502 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2507 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2514 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2525 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2531 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2535 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2539 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2546 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2556 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2563 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2567 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2573 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2577 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2585 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2589 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2596 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2606 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2619 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2623 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2630 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2640 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2649 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2660 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2664 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2671 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2681 "lev_comp.y"
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

  case 417:

/* Line 1455 of yacc.c  */
#line 2692 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2701 "lev_comp.y"
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

  case 419:

/* Line 1455 of yacc.c  */
#line 2711 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2717 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2721 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2728 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2738 "lev_comp.y"
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

  case 424:

/* Line 1455 of yacc.c  */
#line 2749 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2758 "lev_comp.y"
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

  case 426:

/* Line 1455 of yacc.c  */
#line 2768 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2772 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2778 "lev_comp.y"
    { ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2780 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2786 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2790 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2794 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2798 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2806 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2815 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2819 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2823 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2827 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2831 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2834 "lev_comp.y"
    { ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2838 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2845 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2854 "lev_comp.y"
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

  case 448:

/* Line 1455 of yacc.c  */
#line 2895 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2899 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2906 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2913 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2923 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2927 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2935 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2939 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2943 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2947 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2951 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2956 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2960 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2964 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2969 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2974 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2979 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2983 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2988 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2993 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2997 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 3001 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 3006 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 3010 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 3017 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3023 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3027 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3033 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3049 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3053 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3057 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3061 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3083 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3087 "lev_comp.y"
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

  case 495:

/* Line 1455 of yacc.c  */
#line 3113 "lev_comp.y"
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
#line 7037 "lev_comp.tab.c"
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
#line 3141 "lev_comp.y"


/*lev_comp.y*/

