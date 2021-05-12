
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
     AGE_ID = 312,
     CORRIDOR_ID = 313,
     GOLD_ID = 314,
     ENGRAVING_ID = 315,
     FOUNTAIN_ID = 316,
     THRONE_ID = 317,
     MODRON_PORTAL_ID = 318,
     LEVEL_TELEPORTER_ID = 319,
     LEVEL_TELEPORT_DIRECTION_TYPE = 320,
     LEVEL_TELEPORT_END_TYPE = 321,
     POOL_ID = 322,
     SINK_ID = 323,
     NONE = 324,
     RAND_CORRIDOR_ID = 325,
     DOOR_STATE = 326,
     LIGHT_STATE = 327,
     CURSE_TYPE = 328,
     ENGRAVING_TYPE = 329,
     KEYTYPE_ID = 330,
     LEVER_ID = 331,
     NO_PICKUP_ID = 332,
     DIRECTION = 333,
     RANDOM_TYPE = 334,
     RANDOM_TYPE_BRACKET = 335,
     A_REGISTER = 336,
     ALIGNMENT = 337,
     LEFT_OR_RIGHT = 338,
     CENTER = 339,
     TOP_OR_BOT = 340,
     ALTAR_TYPE = 341,
     UP_OR_DOWN = 342,
     ACTIVE_OR_INACTIVE = 343,
     MODRON_PORTAL_TYPE = 344,
     NPC_TYPE = 345,
     FOUNTAIN_TYPE = 346,
     SPECIAL_OBJECT_TYPE = 347,
     CMAP_TYPE = 348,
     FLOOR_TYPE = 349,
     FLOOR_TYPE_ID = 350,
     FLOOR_ID = 351,
     FLOOR_MAIN_TYPE = 352,
     FLOOR_MAIN_TYPE_ID = 353,
     ELEMENTAL_ENCHANTMENT_TYPE = 354,
     EXCEPTIONALITY_TYPE = 355,
     EXCEPTIONALITY_ID = 356,
     ELEMENTAL_ENCHANTMENT_ID = 357,
     ENCHANTMENT_ID = 358,
     SECRET_DOOR_ID = 359,
     USES_UP_KEY_ID = 360,
     MYTHIC_PREFIX_TYPE = 361,
     MYTHIC_SUFFIX_TYPE = 362,
     MYTHIC_PREFIX_ID = 363,
     MYTHIC_SUFFIX_ID = 364,
     CHARGES_ID = 365,
     SPECIAL_QUALITY_ID = 366,
     SPEFLAGS_ID = 367,
     SUBROOM_ID = 368,
     NAME_ID = 369,
     FLAGS_ID = 370,
     FLAG_TYPE = 371,
     MON_ATTITUDE = 372,
     MON_ALERTNESS = 373,
     SUBTYPE_ID = 374,
     NON_PASSDOOR_ID = 375,
     MON_APPEARANCE = 376,
     ROOMDOOR_ID = 377,
     IF_ID = 378,
     ELSE_ID = 379,
     TERRAIN_ID = 380,
     HORIZ_OR_VERT = 381,
     REPLACE_TERRAIN_ID = 382,
     LOCATION_SUBTYPE_ID = 383,
     DOOR_SUBTYPE = 384,
     BRAZIER_SUBTYPE = 385,
     SIGNPOST_SUBTYPE = 386,
     TREE_SUBTYPE = 387,
     EXIT_ID = 388,
     SHUFFLE_ID = 389,
     QUANTITY_ID = 390,
     BURIED_ID = 391,
     LOOP_ID = 392,
     FOR_ID = 393,
     TO_ID = 394,
     SWITCH_ID = 395,
     CASE_ID = 396,
     BREAK_ID = 397,
     DEFAULT_ID = 398,
     ERODED_ID = 399,
     TRAPPED_STATE = 400,
     RECHARGED_ID = 401,
     INVIS_ID = 402,
     GREASED_ID = 403,
     INDESTRUCTIBLE_ID = 404,
     FEMALE_ID = 405,
     WAITFORU_ID = 406,
     CANCELLED_ID = 407,
     REVIVED_ID = 408,
     AVENGE_ID = 409,
     FLEEING_ID = 410,
     BLINDED_ID = 411,
     PARALYZED_ID = 412,
     STUNNED_ID = 413,
     CONFUSED_ID = 414,
     SEENTRAPS_ID = 415,
     ALL_ID = 416,
     MONTYPE_ID = 417,
     OBJTYPE_ID = 418,
     TERTYPE_ID = 419,
     TERTYPE2_ID = 420,
     LEVER_EFFECT_TYPE = 421,
     SWITCHABLE_ID = 422,
     CONTINUOUSLY_USABLE_ID = 423,
     TARGET_ID = 424,
     TRAPTYPE_ID = 425,
     EFFECT_FLAG_ID = 426,
     GRAVE_ID = 427,
     BRAZIER_ID = 428,
     SIGNPOST_ID = 429,
     TREE_ID = 430,
     ERODEPROOF_ID = 431,
     FUNCTION_ID = 432,
     MSG_OUTPUT_TYPE = 433,
     COMPARE_TYPE = 434,
     UNKNOWN_TYPE = 435,
     rect_ID = 436,
     fillrect_ID = 437,
     line_ID = 438,
     randline_ID = 439,
     grow_ID = 440,
     selection_ID = 441,
     flood_ID = 442,
     rndcoord_ID = 443,
     circle_ID = 444,
     ellipse_ID = 445,
     filter_ID = 446,
     complement_ID = 447,
     gradient_ID = 448,
     GRADIENT_TYPE = 449,
     LIMITED = 450,
     HUMIDITY_TYPE = 451,
     STRING = 452,
     MAP_ID = 453,
     NQSTRING = 454,
     VARSTRING = 455,
     CFUNC = 456,
     CFUNC_INT = 457,
     CFUNC_STR = 458,
     CFUNC_COORD = 459,
     CFUNC_REGION = 460,
     VARSTRING_INT = 461,
     VARSTRING_INT_ARRAY = 462,
     VARSTRING_STRING = 463,
     VARSTRING_STRING_ARRAY = 464,
     VARSTRING_VAR = 465,
     VARSTRING_VAR_ARRAY = 466,
     VARSTRING_COORD = 467,
     VARSTRING_COORD_ARRAY = 468,
     VARSTRING_REGION = 469,
     VARSTRING_REGION_ARRAY = 470,
     VARSTRING_MAPCHAR = 471,
     VARSTRING_MAPCHAR_ARRAY = 472,
     VARSTRING_MONST = 473,
     VARSTRING_MONST_ARRAY = 474,
     VARSTRING_OBJ = 475,
     VARSTRING_OBJ_ARRAY = 476,
     VARSTRING_SEL = 477,
     VARSTRING_SEL_ARRAY = 478,
     METHOD_INT = 479,
     METHOD_INT_ARRAY = 480,
     METHOD_STRING = 481,
     METHOD_STRING_ARRAY = 482,
     METHOD_VAR = 483,
     METHOD_VAR_ARRAY = 484,
     METHOD_COORD = 485,
     METHOD_COORD_ARRAY = 486,
     METHOD_REGION = 487,
     METHOD_REGION_ARRAY = 488,
     METHOD_MAPCHAR = 489,
     METHOD_MAPCHAR_ARRAY = 490,
     METHOD_MONST = 491,
     METHOD_MONST_ARRAY = 492,
     METHOD_OBJ = 493,
     METHOD_OBJ_ARRAY = 494,
     METHOD_SEL = 495,
     METHOD_SEL_ARRAY = 496,
     DICE = 497
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
#line 540 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 552 "lev_comp.tab.c"

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
#define YYLAST   1272

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  260
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  186
/* YYNRULES -- Number of rules.  */
#define YYNRULES  499
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1107

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   497

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   255,   259,     2,
     199,   200,   253,   251,   197,   252,   257,   254,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   198,     2,
       2,   256,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   201,     2,   202,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   203,   258,   204,     2,     2,     2,     2,
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
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250
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
     261,   263,   265,   267,   271,   275,   281,   285,   291,   297,
     303,   307,   311,   317,   323,   329,   337,   345,   353,   359,
     361,   365,   367,   371,   373,   377,   379,   383,   385,   389,
     391,   395,   397,   401,   402,   403,   412,   417,   419,   420,
     422,   424,   430,   434,   435,   436,   446,   447,   450,   451,
     457,   458,   463,   465,   468,   470,   477,   478,   482,   483,
     490,   491,   496,   497,   502,   504,   505,   510,   514,   516,
     520,   524,   530,   536,   544,   549,   550,   564,   565,   581,
     582,   585,   591,   593,   599,   601,   607,   609,   615,   617,
     628,   635,   637,   639,   641,   643,   645,   649,   651,   653,
     654,   658,   662,   666,   670,   672,   674,   676,   678,   680,
     688,   694,   696,   698,   700,   702,   706,   707,   713,   718,
     719,   723,   725,   727,   729,   731,   734,   736,   738,   740,
     742,   744,   748,   752,   756,   758,   760,   764,   766,   768,
     770,   774,   778,   779,   785,   788,   789,   793,   795,   799,
     801,   805,   809,   811,   813,   817,   819,   821,   823,   827,
     829,   831,   833,   837,   841,   845,   849,   853,   857,   861,
     863,   865,   867,   871,   875,   879,   885,   893,   899,   908,
     910,   914,   918,   924,   930,   938,   946,   953,   959,   960,
     963,   969,   973,   985,   999,  1006,  1007,  1011,  1013,  1017,
    1021,  1025,  1029,  1031,  1033,  1037,  1041,  1045,  1049,  1053,
    1057,  1059,  1061,  1063,  1065,  1069,  1073,  1075,  1081,  1091,
    1097,  1101,  1105,  1111,  1121,  1127,  1133,  1139,  1140,  1154,
    1155,  1157,  1165,  1177,  1181,  1189,  1197,  1203,  1209,  1219,
    1225,  1231,  1235,  1243,  1247,  1255,  1263,  1267,  1273,  1277,
    1283,  1291,  1301,  1303,  1305,  1307,  1309,  1311,  1312,  1315,
    1317,  1321,  1323,  1325,  1327,  1328,  1332,  1334,  1335,  1339,
    1341,  1342,  1346,  1348,  1350,  1352,  1354,  1356,  1358,  1360,
    1362,  1364,  1368,  1370,  1372,  1377,  1379,  1381,  1386,  1388,
    1390,  1395,  1397,  1402,  1408,  1410,  1414,  1416,  1420,  1422,
    1424,  1429,  1439,  1441,  1443,  1448,  1450,  1456,  1458,  1460,
    1465,  1467,  1469,  1475,  1477,  1479,  1481,  1486,  1488,  1490,
    1496,  1498,  1500,  1502,  1506,  1508,  1510,  1514,  1516,  1521,
    1525,  1529,  1533,  1537,  1541,  1545,  1547,  1549,  1553,  1555,
    1559,  1560,  1562,  1564,  1566,  1568,  1572,  1573,  1575,  1577,
    1580,  1583,  1588,  1595,  1600,  1607,  1614,  1621,  1628,  1631,
    1638,  1647,  1656,  1667,  1682,  1685,  1687,  1691,  1693,  1697,
    1699,  1701,  1703,  1705,  1707,  1709,  1711,  1713,  1715,  1717,
    1719,  1721,  1723,  1725,  1727,  1729,  1731,  1733,  1735,  1746
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     261,     0,    -1,    -1,   262,    -1,   263,    -1,   263,   262,
      -1,   264,   272,   274,    -1,    16,   198,   205,    -1,    15,
     198,   205,   197,     3,    -1,    17,   198,    11,   197,   372,
      -1,    17,   198,    10,   197,     3,    -1,    17,   198,    13,
      -1,    17,   198,    12,   197,     3,   197,     3,   197,     5,
     197,     5,   197,   407,   197,   271,   270,    -1,    18,   198,
     437,    -1,    21,   198,    97,    -1,    -1,   197,   195,    -1,
      -1,   197,   414,    -1,    -1,   197,     3,    -1,     5,    -1,
      79,    -1,    -1,   115,   198,   273,    -1,   116,   197,   273,
      -1,   116,    -1,    -1,   276,   274,    -1,   203,   274,   204,
      -1,   317,    -1,   265,    -1,   266,    -1,   380,    -1,   267,
      -1,   387,    -1,   386,    -1,   384,    -1,   385,    -1,   388,
      -1,   389,    -1,   390,    -1,   391,    -1,   392,    -1,   361,
      -1,   319,    -1,   282,    -1,   281,    -1,   375,    -1,   331,
      -1,   353,    -1,   394,    -1,   395,    -1,   363,    -1,   364,
      -1,   365,    -1,   393,    -1,   297,    -1,   307,    -1,   309,
      -1,   313,    -1,   311,    -1,   294,    -1,   304,    -1,   290,
      -1,   293,    -1,   356,    -1,   366,    -1,   338,    -1,   354,
      -1,   341,    -1,   347,    -1,   376,    -1,   371,    -1,   359,
      -1,   318,    -1,   377,    -1,   378,    -1,   379,    -1,   381,
      -1,   324,    -1,   322,    -1,   370,    -1,   374,    -1,   373,
      -1,   357,    -1,   358,    -1,   360,    -1,   352,    -1,   355,
      -1,   215,    -1,   217,    -1,   219,    -1,   221,    -1,   223,
      -1,   225,    -1,   227,    -1,   229,    -1,   231,    -1,   214,
      -1,   216,    -1,   218,    -1,   220,    -1,   222,    -1,   224,
      -1,   226,    -1,   228,    -1,   230,    -1,   277,    -1,   278,
      -1,   208,    -1,   208,    -1,   278,    -1,   134,   198,   277,
      -1,   279,   256,   426,    -1,   279,   256,   186,   198,   435,
      -1,   279,   256,   425,    -1,   279,   256,   442,   198,   419,
      -1,   279,   256,   441,   198,   421,    -1,   279,   256,   440,
     198,   423,    -1,   279,   256,   414,    -1,   279,   256,   417,
      -1,   279,   256,   203,   288,   204,    -1,   279,   256,   203,
     287,   204,    -1,   279,   256,   203,   286,   204,    -1,   279,
     256,   442,   198,   203,   285,   204,    -1,   279,   256,   441,
     198,   203,   284,   204,    -1,   279,   256,   440,   198,   203,
     283,   204,    -1,   279,   256,   203,   289,   204,    -1,   424,
      -1,   283,   197,   424,    -1,   422,    -1,   284,   197,   422,
      -1,   420,    -1,   285,   197,   420,    -1,   418,    -1,   286,
     197,   418,    -1,   415,    -1,   287,   197,   415,    -1,   426,
      -1,   288,   197,   426,    -1,   425,    -1,   289,   197,   425,
      -1,    -1,    -1,   177,   207,   199,   291,   430,   200,   292,
     275,    -1,   207,   199,   433,   200,    -1,   133,    -1,    -1,
       6,    -1,     6,    -1,   201,   426,   179,   426,   202,    -1,
     201,   426,   202,    -1,    -1,    -1,   140,   298,   201,   413,
     202,   299,   203,   300,   204,    -1,    -1,   301,   300,    -1,
      -1,   141,   438,   198,   302,   274,    -1,    -1,   143,   198,
     303,   274,    -1,   142,    -1,   257,   257,    -1,   139,    -1,
     138,   280,   256,   426,   305,   426,    -1,    -1,   306,   308,
     275,    -1,    -1,   137,   201,   413,   202,   310,   275,    -1,
      -1,   296,   198,   312,   276,    -1,    -1,   123,   296,   314,
     315,    -1,   275,    -1,    -1,   275,   316,   124,   275,    -1,
      14,   198,   425,    -1,    70,    -1,    70,   198,   438,    -1,
      70,   198,    79,    -1,    58,   198,   320,   197,   320,    -1,
      58,   198,   320,   197,   438,    -1,   199,     4,   197,    78,
     197,   335,   200,    -1,   397,   295,   197,   407,    -1,    -1,
     113,   198,   321,   197,   328,   197,   330,   398,   401,   403,
     405,   323,   275,    -1,    -1,    50,   198,   321,   197,   327,
     197,   329,   197,   330,   398,   401,   403,   405,   325,   275,
      -1,    -1,   197,     5,    -1,   199,     4,   197,     4,   200,
      -1,    79,    -1,   199,     4,   197,     4,   200,    -1,    79,
      -1,   199,   339,   197,   340,   200,    -1,    79,    -1,   199,
       4,   197,     4,   200,    -1,    79,    -1,   122,   198,   332,
     197,   406,   197,   333,   197,   335,   336,    -1,    27,   198,
     406,   197,   435,   336,    -1,     5,    -1,    79,    -1,   334,
      -1,    79,    -1,    78,    -1,    78,   258,   334,    -1,     4,
      -1,    79,    -1,    -1,   336,   197,   337,    -1,   128,   198,
     129,    -1,    75,   198,   423,    -1,   111,   198,     4,    -1,
     149,    -1,   104,    -1,   105,    -1,   120,    -1,    20,    -1,
      19,   198,   339,   197,   340,   326,   206,    -1,    19,   198,
     414,   326,   206,    -1,    83,    -1,    84,    -1,    85,    -1,
      84,    -1,    25,   198,   343,    -1,    -1,    25,   198,   343,
     342,   275,    -1,   421,   197,   414,   344,    -1,    -1,   344,
     197,   345,    -1,   425,    -1,   117,    -1,   118,    -1,   409,
      -1,   121,   425,    -1,   150,    -1,   147,    -1,   152,    -1,
     153,    -1,   154,    -1,   155,   198,   413,    -1,   156,   198,
     413,    -1,   157,   198,   413,    -1,   158,    -1,   159,    -1,
     160,   198,   346,    -1,   151,    -1,   205,    -1,   161,    -1,
     205,   258,   346,    -1,    23,   198,   349,    -1,    -1,    24,
     198,   349,   348,   275,    -1,   423,   350,    -1,    -1,   350,
     197,   351,    -1,    73,    -1,   162,   198,   421,    -1,   439,
      -1,   114,   198,   425,    -1,   135,   198,   413,    -1,   136,
      -1,    72,    -1,   144,   198,   413,    -1,   176,    -1,    71,
      -1,   145,    -1,   146,   198,   413,    -1,   147,    -1,   148,
      -1,   414,    -1,   102,   198,    99,    -1,   101,   198,   100,
      -1,   103,   198,   413,    -1,   110,   198,   413,    -1,   111,
     198,   413,    -1,   112,   198,   413,    -1,    75,   198,   423,
      -1,   149,    -1,   105,    -1,    77,    -1,   108,   198,   106,
      -1,   109,   198,   107,    -1,    57,   198,   413,    -1,    26,
     198,   396,   197,   414,    -1,    28,   198,   414,   197,    78,
     197,   406,    -1,    32,   198,   414,   197,    78,    -1,    32,
     198,   414,   197,    78,   197,     5,   270,    -1,    33,    -1,
      33,   198,   435,    -1,    33,   198,   161,    -1,    46,   198,
     414,   197,    87,    -1,    47,   198,   414,   197,    87,    -1,
      47,   198,   444,   197,   444,   197,    87,    -1,    52,   198,
     444,   197,   444,   197,   205,    -1,    53,   198,   444,   197,
     444,   362,    -1,    54,   198,   444,   197,   444,    -1,    -1,
     197,    87,    -1,    61,   198,   435,   197,    91,    -1,    62,
     198,   435,    -1,    63,   198,   414,   197,   169,   198,   414,
     197,    89,   197,    88,    -1,    63,   198,   414,   197,    64,
     197,    65,   197,    66,   197,    89,   197,    88,    -1,    76,
     198,   414,   197,   166,   367,    -1,    -1,   367,   197,   368,
      -1,    88,    -1,   162,   198,   421,    -1,   163,   198,   423,
      -1,   164,   198,   372,    -1,   165,   198,   372,    -1,   167,
      -1,   168,    -1,   169,   198,   414,    -1,   170,   198,   396,
      -1,   128,   198,   369,    -1,    95,   198,   404,    -1,   171,
     198,     4,    -1,   111,   198,     4,    -1,     4,    -1,    89,
      -1,   129,    -1,    91,    -1,    68,   198,   435,    -1,    67,
     198,   435,    -1,     3,    -1,   199,     3,   197,   407,   200,
      -1,   127,   198,   417,   197,   419,   197,   419,   197,     7,
      -1,   125,   198,   435,   197,   419,    -1,    48,   198,   417,
      -1,    49,   198,   417,    -1,    38,   198,   205,   197,    39,
      -1,    38,   198,   205,   197,    39,   197,   162,   198,   421,
      -1,    35,   198,   417,   197,    37,    -1,    36,   198,   444,
     197,    37,    -1,    22,   198,   435,   197,    93,    -1,    -1,
      34,   198,   417,   197,   407,   197,   397,   398,   401,   403,
     405,   382,   383,    -1,    -1,   275,    -1,    43,   198,   414,
     197,   408,   197,   410,    -1,    43,   198,   414,   197,   408,
     197,   410,   197,   162,   198,   421,    -1,    44,   198,   414,
      -1,    44,   198,   414,   197,   162,   198,   421,    -1,    96,
     198,   435,   197,    97,   197,    94,    -1,   119,   198,   435,
     197,     4,    -1,    45,   198,    90,   197,   414,    -1,    45,
     198,    90,   197,   414,   197,   162,   198,   421,    -1,   172,
     198,   414,   197,   425,    -1,   172,   198,   414,   197,    79,
      -1,   172,   198,   414,    -1,   173,   198,   414,   197,   130,
     197,   407,    -1,   173,   198,   414,    -1,   174,   198,   414,
     197,   131,   197,   425,    -1,   174,   198,   414,   197,   131,
     197,    79,    -1,   174,   198,   414,    -1,   175,   198,   414,
     197,   132,    -1,   175,   198,   414,    -1,    59,   198,   426,
     197,   414,    -1,    60,   198,   414,   197,   443,   197,   425,
      -1,    56,   198,   413,   197,   413,   197,   413,   197,   413,
      -1,    56,    -1,   205,    -1,    79,    -1,   205,    -1,    79,
      -1,    -1,   197,   399,    -1,   400,    -1,   400,   197,   399,
      -1,    40,    -1,    41,    -1,    42,    -1,    -1,    98,   198,
     402,    -1,    97,    -1,    -1,    95,   198,   404,    -1,    94,
      -1,    -1,   162,   198,   421,    -1,    71,    -1,    79,    -1,
      72,    -1,    79,    -1,    82,    -1,   411,    -1,    79,    -1,
      82,    -1,   411,    -1,    81,   198,    79,    -1,    86,    -1,
      79,    -1,    81,   201,     4,   202,    -1,   205,    -1,   216,
      -1,   217,   201,   426,   202,    -1,   426,    -1,   415,    -1,
     188,   199,   435,   200,    -1,   220,    -1,   221,   201,   426,
     202,    -1,   199,     4,   197,     4,   200,    -1,    79,    -1,
      80,   416,   202,    -1,   196,    -1,   196,   197,   416,    -1,
     418,    -1,   222,    -1,   223,   201,   426,   202,    -1,   199,
       4,   197,     4,   197,     4,   197,     4,   200,    -1,   420,
      -1,   224,    -1,   225,   201,   426,   202,    -1,     3,    -1,
     199,     3,   197,   407,   200,    -1,   422,    -1,   226,    -1,
     227,   201,   426,   202,    -1,   205,    -1,     3,    -1,   199,
       3,   197,   205,   200,    -1,    79,    -1,   424,    -1,   228,
      -1,   229,   201,   426,   202,    -1,   205,    -1,     3,    -1,
     199,     3,   197,   205,   200,    -1,    79,    -1,    92,    -1,
     412,    -1,   425,   257,   412,    -1,     4,    -1,   436,    -1,
     199,     8,   200,    -1,   214,    -1,   215,   201,   426,   202,
      -1,   426,   251,   426,    -1,   426,   252,   426,    -1,   426,
     253,   426,    -1,   426,   254,   426,    -1,   426,   255,   426,
      -1,   199,   426,   200,    -1,   210,    -1,   211,    -1,   279,
     198,   427,    -1,   428,    -1,   429,   197,   428,    -1,    -1,
     429,    -1,   426,    -1,   425,    -1,   431,    -1,   432,   197,
     431,    -1,    -1,   432,    -1,   414,    -1,   181,   417,    -1,
     182,   417,    -1,   183,   414,   197,   414,    -1,   184,   414,
     197,   414,   197,   426,    -1,   185,   199,   435,   200,    -1,
     185,   199,   334,   197,   435,   200,    -1,   191,   199,     7,
     197,   435,   200,    -1,   191,   199,   435,   197,   435,   200,
      -1,   191,   199,   419,   197,   435,   200,    -1,   187,   414,
      -1,   189,   199,   414,   197,   426,   200,    -1,   189,   199,
     414,   197,   426,   197,    40,   200,    -1,   190,   199,   414,
     197,   426,   197,   426,   200,    -1,   190,   199,   414,   197,
     426,   197,   426,   197,    40,   200,    -1,   193,   199,   194,
     197,   199,   426,   252,   426,   268,   200,   197,   414,   269,
     200,    -1,   192,   434,    -1,   230,    -1,   199,   435,   200,
      -1,   434,    -1,   434,   259,   435,    -1,   250,    -1,    93,
      -1,     4,    -1,     8,    -1,     9,    -1,     4,    -1,     8,
      -1,     9,    -1,     4,    -1,   436,    -1,    29,    -1,    23,
      -1,    30,    -1,    25,    -1,    31,    -1,   125,    -1,    74,
      -1,    79,    -1,   445,    -1,    55,   199,     4,   197,     4,
     197,     4,   197,     4,   200,    -1,   199,     4,   197,     4,
     197,     4,   197,     4,   200,    -1
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
     537,   538,   539,   540,   541,   542,   543,   544,   545,   546,
     549,   550,   551,   552,   553,   554,   555,   556,   557,   560,
     561,   562,   563,   564,   565,   566,   567,   568,   571,   572,
     573,   576,   577,   580,   596,   602,   608,   614,   620,   626,
     632,   638,   644,   654,   664,   674,   684,   694,   704,   716,
     721,   728,   733,   740,   745,   752,   756,   762,   767,   774,
     778,   784,   788,   795,   817,   794,   831,   886,   893,   896,
     902,   909,   913,   922,   926,   921,   989,   990,   994,   993,
    1007,  1006,  1021,  1031,  1032,  1035,  1073,  1072,  1107,  1106,
    1137,  1136,  1169,  1168,  1194,  1205,  1204,  1232,  1238,  1243,
    1248,  1255,  1262,  1271,  1279,  1291,  1290,  1314,  1313,  1337,
    1340,  1346,  1356,  1362,  1371,  1377,  1382,  1388,  1393,  1399,
    1410,  1416,  1417,  1420,  1421,  1424,  1428,  1434,  1435,  1439,
    1445,  1453,  1458,  1463,  1468,  1473,  1478,  1483,  1491,  1498,
    1506,  1514,  1515,  1518,  1519,  1522,  1527,  1526,  1540,  1547,
    1554,  1562,  1567,  1573,  1579,  1585,  1591,  1596,  1601,  1606,
    1611,  1616,  1621,  1626,  1631,  1636,  1641,  1647,  1654,  1663,
    1667,  1680,  1689,  1688,  1706,  1716,  1722,  1730,  1736,  1741,
    1746,  1751,  1756,  1761,  1766,  1771,  1776,  1790,  1796,  1801,
    1806,  1811,  1816,  1821,  1826,  1831,  1836,  1841,  1846,  1851,
    1856,  1861,  1866,  1871,  1876,  1883,  1889,  1918,  1923,  1931,
    1937,  1941,  1949,  1956,  1963,  1973,  1983,  1998,  2009,  2012,
    2018,  2024,  2030,  2034,  2040,  2047,  2053,  2061,  2067,  2072,
    2077,  2082,  2087,  2093,  2099,  2104,  2109,  2114,  2119,  2124,
    2131,  2131,  2131,  2131,  2134,  2140,  2146,  2151,  2158,  2165,
    2171,  2177,  2183,  2188,  2195,  2201,  2211,  2218,  2217,  2249,
    2252,  2258,  2263,  2270,  2274,  2280,  2286,  2292,  2296,  2302,
    2306,  2311,  2318,  2322,  2329,  2333,  2338,  2345,  2349,  2357,
    2363,  2370,  2374,  2381,  2389,  2392,  2402,  2406,  2409,  2415,
    2419,  2426,  2430,  2434,  2441,  2444,  2450,  2457,  2460,  2466,
    2473,  2477,  2484,  2485,  2488,  2489,  2492,  2493,  2494,  2500,
    2501,  2502,  2508,  2509,  2512,  2521,  2526,  2533,  2544,  2550,
    2554,  2558,  2565,  2575,  2582,  2586,  2592,  2596,  2604,  2608,
    2615,  2625,  2638,  2642,  2649,  2659,  2668,  2679,  2683,  2690,
    2700,  2711,  2720,  2730,  2736,  2740,  2747,  2757,  2768,  2777,
    2787,  2791,  2798,  2799,  2805,  2809,  2813,  2817,  2825,  2834,
    2838,  2842,  2846,  2850,  2854,  2857,  2864,  2873,  2906,  2907,
    2910,  2911,  2914,  2918,  2925,  2932,  2943,  2946,  2954,  2958,
    2962,  2966,  2970,  2975,  2979,  2983,  2988,  2993,  2998,  3002,
    3007,  3012,  3016,  3020,  3025,  3029,  3036,  3042,  3046,  3052,
    3059,  3060,  3063,  3064,  3065,  3068,  3072,  3076,  3080,  3086,
    3087,  3090,  3091,  3094,  3095,  3098,  3099,  3102,  3106,  3132
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
  "TELEPRT_ID", "BRANCH_ID", "LEV", "MINERALIZE_ID", "AGE_ID",
  "CORRIDOR_ID", "GOLD_ID", "ENGRAVING_ID", "FOUNTAIN_ID", "THRONE_ID",
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
  "SIGNPOST_SUBTYPE", "TREE_SUBTYPE", "EXIT_ID", "SHUFFLE_ID",
  "QUANTITY_ID", "BURIED_ID", "LOOP_ID", "FOR_ID", "TO_ID", "SWITCH_ID",
  "CASE_ID", "BREAK_ID", "DEFAULT_ID", "ERODED_ID", "TRAPPED_STATE",
  "RECHARGED_ID", "INVIS_ID", "GREASED_ID", "INDESTRUCTIBLE_ID",
  "FEMALE_ID", "WAITFORU_ID", "CANCELLED_ID", "REVIVED_ID", "AVENGE_ID",
  "FLEEING_ID", "BLINDED_ID", "PARALYZED_ID", "STUNNED_ID", "CONFUSED_ID",
  "SEENTRAPS_ID", "ALL_ID", "MONTYPE_ID", "OBJTYPE_ID", "TERTYPE_ID",
  "TERTYPE2_ID", "LEVER_EFFECT_TYPE", "SWITCHABLE_ID",
  "CONTINUOUSLY_USABLE_ID", "TARGET_ID", "TRAPTYPE_ID", "EFFECT_FLAG_ID",
  "GRAVE_ID", "BRAZIER_ID", "SIGNPOST_ID", "TREE_ID", "ERODEPROOF_ID",
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
  "level_def", "lev_init", "tileset_detail", "boundary_type_detail",
  "opt_limited", "opt_coord_or_var", "opt_fillchar", "walled", "flags",
  "flag_list", "levstatements", "stmt_block", "levstatement",
  "any_var_array", "any_var", "any_var_or_arr", "any_var_or_unk",
  "shuffle_detail", "variable_define", "encodeobj_list",
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
  "special_region_detail", "special_levregion_detail",
  "special_tileset_detail", "region_detail", "@16", "region_detail_end",
  "altar_detail", "anvil_detail", "floor_detail", "subtype_detail",
  "npc_detail", "grave_detail", "brazier_detail", "signpost_detail",
  "tree_detail", "gold_detail", "engraving_detail", "mineralize",
  "trap_name", "room_type", "optroomregionflags", "roomregionflags",
  "roomregionflag", "optfloormaintype", "floormaintype", "optfloortype",
  "floortype", "optmontype", "door_state", "light_state", "alignment",
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
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,    44,    58,    40,
      41,    91,    93,   123,   125,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,    43,    45,    42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   260,   261,   261,   262,   262,   263,   264,   264,   265,
     265,   265,   265,   266,   267,   268,   268,   269,   269,   270,
     270,   271,   271,   272,   272,   273,   273,   274,   274,   275,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   279,   279,
     279,   280,   280,   281,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   283,
     283,   284,   284,   285,   285,   286,   286,   287,   287,   288,
     288,   289,   289,   291,   292,   290,   293,   294,   295,   295,
     296,   296,   296,   298,   299,   297,   300,   300,   302,   301,
     303,   301,   304,   305,   305,   306,   308,   307,   310,   309,
     312,   311,   314,   313,   315,   316,   315,   317,   318,   318,
     318,   319,   319,   320,   321,   323,   322,   325,   324,   326,
     326,   327,   327,   328,   328,   329,   329,   330,   330,   331,
     331,   332,   332,   333,   333,   334,   334,   335,   335,   336,
     336,   337,   337,   337,   337,   337,   337,   337,   338,   338,
     338,   339,   339,   340,   340,   341,   342,   341,   343,   344,
     344,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   346,   346,
     346,   347,   348,   347,   349,   350,   350,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   352,   353,   354,   354,   355,
     355,   355,   356,   357,   358,   359,   360,   361,   362,   362,
     363,   364,   365,   365,   366,   367,   367,   368,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     369,   369,   369,   369,   370,   371,   372,   372,   373,   374,
     375,   376,   377,   377,   378,   379,   380,   382,   381,   383,
     383,   384,   384,   385,   385,   386,   387,   388,   388,   389,
     389,   389,   390,   390,   391,   391,   391,   392,   392,   393,
     394,   395,   395,   396,   396,   397,   397,   398,   398,   399,
     399,   400,   400,   400,   401,   401,   402,   403,   403,   404,
     405,   405,   406,   406,   407,   407,   408,   408,   408,   409,
     409,   409,   410,   410,   411,   412,   412,   412,   413,   414,
     414,   414,   414,   415,   415,   415,   416,   416,   417,   417,
     417,   418,   419,   419,   419,   420,   420,   421,   421,   421,
     422,   422,   422,   422,   423,   423,   423,   424,   424,   424,
     424,   424,   425,   425,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   427,   427,   428,   429,   429,
     430,   430,   431,   431,   432,   432,   433,   433,   434,   434,
     434,   434,   434,   434,   434,   434,   434,   434,   434,   434,
     434,   434,   434,   434,   434,   434,   434,   435,   435,   436,
     437,   437,   438,   438,   438,   439,   439,   439,   439,   440,
     440,   441,   441,   442,   442,   443,   443,   444,   444,   445
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
       1,     1,     1,     3,     3,     5,     3,     5,     5,     5,
       3,     3,     5,     5,     5,     7,     7,     7,     5,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     0,     0,     8,     4,     1,     0,     1,
       1,     5,     3,     0,     0,     9,     0,     2,     0,     5,
       0,     4,     1,     2,     1,     6,     0,     3,     0,     6,
       0,     4,     0,     4,     1,     0,     4,     3,     1,     3,
       3,     5,     5,     7,     4,     0,    13,     0,    15,     0,
       2,     5,     1,     5,     1,     5,     1,     5,     1,    10,
       6,     1,     1,     1,     1,     1,     3,     1,     1,     0,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     7,
       5,     1,     1,     1,     1,     3,     0,     5,     4,     0,
       3,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     3,     3,     3,     1,     1,     3,     1,     1,     1,
       3,     3,     0,     5,     2,     0,     3,     1,     3,     1,
       3,     3,     1,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     3,     3,     3,     5,     7,     5,     8,     1,
       3,     3,     5,     5,     7,     7,     6,     5,     0,     2,
       5,     3,    11,    13,     6,     0,     3,     1,     3,     3,
       3,     3,     1,     1,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     3,     3,     1,     5,     9,     5,
       3,     3,     5,     9,     5,     5,     5,     0,    13,     0,
       1,     7,    11,     3,     7,     7,     5,     5,     9,     5,
       5,     3,     7,     3,     7,     7,     3,     5,     3,     5,
       7,     9,     1,     1,     1,     1,     1,     0,     2,     1,
       3,     1,     1,     1,     0,     3,     1,     0,     3,     1,
       0,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     4,     1,     1,     4,     1,     1,
       4,     1,     4,     5,     1,     3,     1,     3,     1,     1,
       4,     9,     1,     1,     4,     1,     5,     1,     1,     4,
       1,     1,     5,     1,     1,     1,     4,     1,     1,     5,
       1,     1,     1,     3,     1,     1,     3,     1,     4,     3,
       3,     3,     3,     3,     3,     1,     1,     3,     1,     3,
       0,     1,     1,     1,     1,     3,     0,     1,     1,     2,
       2,     4,     6,     4,     6,     6,     6,     6,     2,     6,
       8,     8,    10,    14,     2,     1,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,    10,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    23,     0,     0,     1,
       5,     0,    27,     0,     7,     0,   150,     0,     0,     0,
       0,   218,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   289,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,   178,     0,     0,     0,
       0,     0,     0,     0,     0,   147,     0,     0,     0,   153,
     162,     0,     0,     0,     0,     0,     0,     0,   110,    99,
      90,   100,    91,   101,    92,   102,    93,   103,    94,   104,
      95,   105,    96,   106,    97,   107,    98,    31,    32,    34,
       6,    27,   108,   109,     0,    47,    46,    64,    65,    62,
       0,    57,    63,   166,    58,    59,    61,    60,    30,    75,
      45,    81,    80,    49,    68,    70,    71,    88,    50,    69,
      89,    66,    85,    86,    74,    87,    44,    53,    54,    55,
      67,    82,    73,    84,    83,    48,    72,    76,    77,    78,
      33,    79,    37,    38,    36,    35,    39,    40,    41,    42,
      43,    56,    51,    52,     0,    26,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   172,     0,     0,     0,     0,   111,   112,     0,     0,
       0,     0,     0,     0,     0,   434,     0,   437,     0,   479,
       0,   435,   456,    28,     0,   170,     0,     8,     0,   395,
     396,     0,   432,   177,     0,     0,     0,    11,   481,   480,
      13,   404,     0,   221,   222,     0,     0,   401,     0,     0,
     189,   399,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   475,   458,   477,     0,   428,
     430,   431,     0,   427,   425,     0,   251,   255,   424,   252,
     421,   423,     0,   420,   418,     0,   225,     0,   417,   364,
     363,     0,   382,   383,     0,     0,     0,   291,   290,     0,
     409,     0,     0,   408,     0,     0,     0,     0,   497,     0,
       0,   343,     0,     0,     0,     0,     0,   330,   331,   366,
     365,     0,   148,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,   301,     0,   325,   324,   484,   482,   483,
     180,   179,     0,     0,     0,     0,   201,   202,     0,     0,
       0,     0,   113,     0,     0,     0,   351,   353,   356,   358,
     143,     0,     0,     0,     0,   152,     0,     0,     0,     0,
       0,   453,   452,   454,   457,     0,   490,   492,   489,   491,
     493,   494,     0,     0,     0,   120,   121,   116,   114,     0,
       0,     0,     0,    27,   167,    25,     0,     0,     0,     0,
       0,   406,     0,     0,     0,     0,     0,     0,     0,   459,
     460,     0,     0,     0,   468,     0,     0,     0,   474,     0,
       0,     0,     0,     0,     0,   254,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   149,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   174,   173,
       0,     0,   168,     0,     0,     0,     0,     0,     0,   450,
     436,   444,     0,     0,   439,   440,   441,   442,   443,     0,
     146,     0,   434,     0,     0,     0,     0,   137,   135,   141,
     139,     0,     0,     0,   171,     0,     0,   433,    10,   326,
       0,     9,     0,     0,   405,     0,     0,     0,   224,   223,
     189,   190,   220,     0,     0,   205,     0,     0,     0,     0,
     415,     0,     0,   413,     0,     0,   412,     0,     0,   476,
     478,   336,     0,     0,     0,   253,     0,     0,   227,   229,
     285,   209,     0,   287,     0,     0,   384,   385,     0,   334,
       0,     0,   335,   332,   388,     0,   386,     0,   387,     0,
     347,   292,     0,   293,     0,   192,     0,     0,     0,     0,
     298,   297,     0,     0,   181,   182,   359,   495,   496,     0,
     300,     0,     0,   305,     0,   194,     0,     0,   346,     0,
       0,     0,   329,     0,     0,   164,     0,     0,   154,   350,
     349,     0,     0,   357,     0,   448,   451,     0,   438,   151,
     455,   115,     0,     0,   124,     0,   123,     0,   122,     0,
     128,     0,   119,     0,   118,     0,   117,    29,   397,     0,
       0,   407,   400,     0,   402,     0,   461,     0,     0,     0,
     463,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     426,   487,   485,   486,     0,   266,   263,   257,     0,   281,
       0,     0,     0,   280,     0,     0,     0,     0,     0,     0,
       0,   262,     0,   267,     0,   269,   270,   279,     0,   265,
     256,   271,   488,   259,     0,   419,   228,   200,     0,     0,
       0,   410,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   184,     0,     0,   296,     0,     0,
       0,     0,     0,   304,     0,     0,     0,     0,     0,     0,
     169,   163,   165,     0,     0,     0,     0,     0,   144,     0,
     136,   138,   140,   142,     0,   129,     0,   131,     0,   133,
       0,     0,   403,   219,     0,   206,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   422,     0,     0,   286,    19,     0,   367,     0,     0,
       0,     0,   393,   392,   341,   344,     0,   294,     0,   196,
       0,     0,   295,   299,     0,     0,   360,     0,     0,     0,
     345,     0,   198,     0,   367,   204,     0,   203,   176,     0,
     156,   352,   355,   354,   445,   446,   447,   449,     0,     0,
     127,     0,   126,     0,   125,     0,     0,   462,   464,     0,
     469,     0,   465,     0,   414,   467,   466,     0,   284,   278,
     273,   272,   274,   282,   283,   275,   276,   277,   260,   261,
     264,   268,   258,     0,   389,   232,   233,     0,   237,   236,
     247,   238,   239,   240,     0,     0,     0,   244,   245,     0,
     230,   234,   390,   231,     0,   215,   216,     0,   217,     0,
     214,   210,     0,   288,     0,     0,   374,     0,     0,     0,
     394,     0,     0,     0,     0,     0,     0,   207,   208,     0,
       0,     0,   307,     0,     0,     0,     0,     0,     0,     0,
     312,   313,     0,     0,     0,   306,     0,     0,   374,     0,
       0,     0,     0,     0,   156,   145,   130,   132,   134,   327,
       0,     0,     0,   416,     0,     0,   235,     0,     0,     0,
       0,     0,     0,     0,    20,     0,   371,   372,   373,   368,
     369,     0,   377,     0,     0,   333,     0,   348,   191,     0,
     367,   361,   183,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   193,     0,   377,   209,   328,
       0,   160,   155,   157,     0,   470,     0,   471,   440,   391,
     241,   242,   243,   249,   248,   246,   212,   213,   211,     0,
       0,     0,     0,   380,     0,     0,     0,     0,   374,     0,
       0,   379,   317,   319,   320,   321,   323,   322,   316,   308,
     309,   310,   311,   314,   315,   318,     0,   380,   199,   158,
      27,     0,     0,     0,     0,     0,   411,   370,   376,   375,
       0,     0,   337,     0,   499,   342,   195,   377,     0,   302,
     197,   185,    27,   161,     0,   472,    16,     0,   250,   378,
       0,   339,   498,   380,     0,     0,   159,     0,     0,   381,
     340,   338,   187,   303,   186,     0,    17,     0,    21,    22,
      19,     0,     0,   188,    12,    18,   473
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    97,    98,    99,  1054,  1102,
     903,  1100,    12,   166,   100,   404,   101,   102,   103,   104,
     218,   105,   106,   754,   756,   758,   503,   504,   505,   506,
     107,   489,   838,   108,   109,   462,   110,   111,   219,   743,
     943,   944,  1072,  1050,   112,   617,   113,   114,   236,   115,
     614,   116,   402,   117,   359,   479,   610,   118,   119,   120,
     339,   331,   121,  1085,   122,  1097,   418,   587,   607,   811,
     824,   123,   358,   826,   536,   919,   707,   901,   124,   259,
     530,   125,   439,   296,   706,   890,  1015,   126,   436,   286,
     435,   700,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   727,   137,   138,   139,   140,   733,   935,  1038,
     141,   142,   521,   143,   144,   145,   146,   147,   148,   149,
     150,   151,  1081,  1091,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   301,   332,   906,   969,
     970,   972,  1059,  1023,  1032,  1062,   304,   568,   577,   891,
     804,   578,   242,   336,   276,   261,   412,   312,   313,   545,
     546,   297,   298,   287,   288,   381,   337,   836,   625,   626,
     627,   383,   384,   385,   277,   430,   231,   250,   351,   703,
     399,   400,   401,   599,   317,   318
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -953
static const yytype_int16 yypact[] =
{
     360,  -106,   -91,   113,  -953,   360,     4,   -76,   -72,  -953,
    -953,   -61,   768,   -43,  -953,    40,  -953,    -5,     5,    60,
      62,  -953,    71,    76,    85,    89,    97,   103,   150,   157,
     200,   203,   232,   241,   248,   264,   267,   268,   270,   271,
     274,   275,   288,   289,   296,   297,   299,   300,   302,   303,
     306,   310,   316,   319,   320,   323,   328,   334,   336,   342,
     343,   350,    49,   352,   355,  -953,   364,   -39,   843,  -953,
    -953,   368,   381,   389,   397,    -2,   122,    10,  -953,  -953,
    -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,
    -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,
    -953,   768,  -953,  -953,   -21,  -953,  -953,  -953,  -953,  -953,
     398,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,
    -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,
    -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,
    -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,
    -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,
    -953,  -953,  -953,  -953,   340,   198,  -953,     6,   579,    42,
     331,   283,   832,   294,   294,   262,   -34,    27,   -19,   -19,
     819,    68,    68,   -24,   253,   -19,   -19,   370,   -19,   308,
      68,    68,   -15,   -24,   -24,   -24,   122,   293,   122,   -19,
     832,   832,   -19,   832,   832,   106,   -19,   832,   -15,   832,
      88,  -953,   832,    68,   849,   122,  -953,  -953,   247,   402,
     -19,   -19,   -19,   -19,   332,  -953,    67,  -953,   404,  -953,
     167,  -953,    63,  -953,    11,  -953,   321,  -953,    40,  -953,
    -953,   406,  -953,   249,   401,   413,   415,  -953,  -953,  -953,
    -953,  -953,   418,  -953,  -953,   412,   611,  -953,   419,   422,
     431,  -953,  -953,    68,    68,   -19,   -19,   417,   -19,   434,
     436,   437,   832,   443,   260,  -953,  -953,   385,   448,  -953,
    -953,  -953,   643,  -953,  -953,   446,  -953,  -953,  -953,  -953,
    -953,  -953,   650,  -953,  -953,   460,   459,   466,  -953,  -953,
    -953,   488,  -953,  -953,   492,   493,   495,  -953,  -953,   689,
    -953,   494,   497,  -953,   501,   500,   696,   504,  -953,   506,
     510,   511,   513,   514,   705,   516,   519,  -953,  -953,  -953,
    -953,   521,   709,   530,   532,   534,   535,   224,   729,   537,
     330,   538,   542,  -953,   543,  -953,  -953,  -953,  -953,  -953,
    -953,  -953,   544,   546,   547,   551,  -953,  -953,   554,   321,
     555,   560,  -953,   566,   122,   122,   578,   580,   581,   583,
    -953,   576,   429,   122,   122,  -953,   122,   122,   122,   122,
     122,   249,   224,  -953,   584,   597,  -953,  -953,  -953,  -953,
    -953,  -953,   586,   117,    25,  -953,  -953,   249,   224,   600,
     601,   609,   768,   768,  -953,  -953,   122,     6,   802,    44,
     805,   613,   623,   832,   635,   122,   307,   828,   628,  -953,
    -953,   640,   655,   658,  -953,   -19,   -19,   169,  -953,   659,
     654,   832,   762,   666,   122,   669,   321,   670,   122,   321,
     -19,   -19,   832,   778,   796,   678,   122,    45,   839,   873,
     683,   845,   844,   136,   723,   -19,   807,   692,   809,   -24,
     -36,  -953,   695,   -24,   -24,   -24,   122,   700,    54,   -19,
      37,   812,   -14,   734,   810,   -13,   900,    27,   785,  -953,
      22,    22,  -953,   258,   711,   -35,   790,   797,   789,   736,
    -953,  -953,   470,   512,   154,   154,  -953,  -953,  -953,    63,
    -953,   832,   730,   -70,   -65,   -45,    16,  -953,  -953,   249,
     224,   235,   197,   120,  -953,   725,   518,  -953,  -953,  -953,
     927,  -953,   741,   418,  -953,   731,   935,   607,  -953,  -953,
     431,  -953,  -953,   -19,   -19,   688,   750,   748,   771,   773,
    -953,   775,   467,  -953,   772,   777,  -953,   780,   781,  -953,
    -953,  -953,   774,   617,   529,  -953,   776,   663,  -953,  -953,
    -953,  -953,   808,   829,  1023,   671,  -953,  -953,   831,  -953,
     833,  1025,  -953,   835,  -953,   834,  -953,   836,  -953,   838,
     837,  -953,  1033,  -953,   841,  -953,  1037,   846,    45,   847,
     848,  -953,   850,   964,  -953,  -953,  -953,  -953,  -953,   851,
    -953,   853,   856,  -953,   858,  -953,  1042,   859,  -953,   861,
     936,  1072,  -953,   880,   321,  -953,   822,   122,  -953,  -953,
     249,   884,   885,  -953,   886,  -953,   888,   883,  -953,  -953,
    -953,  -953,  1082,   889,  -953,     7,  -953,   122,  -953,     6,
    -953,    99,  -953,   125,  -953,    46,  -953,  -953,  -953,   890,
    1086,  -953,  -953,   891,  -953,   887,  -953,   893,  1014,   832,
    -953,   122,   122,   832,   897,   122,   832,   832,   896,   898,
    -953,  -953,  -953,  -953,   899,  -953,  -953,  -953,   901,  -953,
     902,   903,   904,  -953,   905,   906,   907,   908,   909,   910,
     911,  -953,   912,  -953,   913,  -953,  -953,  -953,   914,  -953,
    -953,  -953,  -953,  -953,   915,  -953,   916,   917,    27,  1091,
     919,  -953,   -15,  1113,   921,   957,  1116,   165,   262,   959,
     -32,  1035,   926,   -11,  -953,   920,  1039,  -953,   122,   930,
       6,  1059,   -19,   931,  1036,   932,    -1,   326,   321,    22,
    -953,  -953,   224,   928,    45,   -22,   202,   736,  -953,    -8,
    -953,  -953,   224,   249,   118,  -953,   131,  -953,   145,  -953,
      45,   937,  -953,  -953,   122,  -953,   933,   174,   416,   938,
      45,   682,   939,   940,   122,  -953,   122,   294,  1032,  1038,
     122,  1029,  1034,   122,   122,   122,     6,   122,   122,   122,
     262,  -953,    -9,   450,  -953,   945,  1132,   946,   947,  1141,
     948,   949,  -953,  -953,   950,  -953,   951,  -953,  1144,  -953,
     333,   953,  -953,  -953,   955,    78,   249,   956,   958,   591,
    -953,  1150,  -953,  1152,   946,  -953,   960,  -953,  -953,   961,
     -40,  -953,  -953,   249,  -953,  -953,  -953,  -953,   321,    99,
    -953,   125,  -953,    46,  -953,   962,  1154,   224,  -953,  1120,
    -953,   122,  -953,   963,  -953,  -953,  -953,   371,  -953,  -953,
    -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,   249,  -953,
    -953,  -953,  -953,   183,  -953,  -953,  -953,     6,  -953,  -953,
    -953,  -953,  -953,  -953,   966,   967,   968,  -953,  -953,   969,
    -953,  -953,  -953,   249,   970,  -953,  -953,   971,  -953,   972,
    -953,  -953,  1158,  -953,   974,   414,  1074,  1169,   977,   262,
    -953,  1013,   262,   976,   980,    -1,   122,  -953,  -953,   978,
    1114,  1090,  -953,   983,   984,   985,   986,   987,   988,   989,
    -953,  -953,   990,   991,   992,  -953,   993,   994,  1074,    78,
    1185,   325,   996,   995,   -40,  -953,  -953,  -953,  -953,  -953,
     998,   997,   305,  -953,   122,  1117,   249,   122,   122,   122,
     -87,   294,  1194,  1071,  -953,  1197,  -953,  -953,  -953,  -953,
    1005,  1006,  1108,  1008,  1202,  -953,  1009,  -953,  -953,   307,
     946,  -953,  -953,  1011,  1012,  1118,  1206,    31,   262,   294,
      44,    44,   -19,   -34,  1207,  -953,  1209,  1108,  -953,  -953,
    1016,  -953,  -953,  -953,  1210,  -953,  1176,  -953,   182,  -953,
    -953,  -953,  -953,  -953,   965,  -953,  -953,  -953,  -953,  1017,
     414,  1121,  1021,  1058,  1217,  1022,   262,  1024,  1074,  1136,
    1138,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,
    -953,  -953,  -953,  -953,  -953,  -953,  1027,  1058,   917,  -953,
     768,  1031,  1030,  1040,  1041,   -87,  -953,  -953,  -953,  -953,
    1118,  1044,  -953,  1043,  -953,  -953,  -953,  1108,  1047,  -953,
    -953,  -953,   768,  -953,    45,  -953,  -953,  1048,  -953,  -953,
     262,   321,  -953,  1058,  1143,   321,  -953,  1049,   -19,  -953,
    -953,  -953,  -953,  -953,  -953,    95,  1050,   321,  -953,  -953,
     945,   -19,  1051,  -953,  -953,  -953,  -953
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -953,  -953,  1224,  -953,  -953,  -953,  -953,  -953,  -953,  -953,
     132,  -953,  -953,   999,  -101,  -355,   852,  1019,  1166,  -477,
    -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,
    -953,  -953,  -953,  -953,  -953,  -953,  1174,  -953,  -953,  -953,
     295,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,
    -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,
     770,  1045,  -953,  -953,  -953,  -953,   710,  -953,  -953,  -953,
     335,  -953,  -953,  -953,  -564,   309,   251,  -953,  -953,   442,
     276,  -953,  -953,  -953,  -953,  -953,   201,  -953,  -953,  1083,
    -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,
    -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,
    -953,  -953,  -558,  -953,  -953,  -953,  -953,  -953,  -953,  -953,
    -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,  -953,
    -953,  -953,  -953,  -953,  -953,  -953,   265,   548,  -796,   239,
    -953,  -849,  -953,  -927,   204,  -952,  -445,  -583,  -953,  -953,
    -953,   469,   855,  -212,  -169,  -380,   740,   354,  -381,  -454,
    -624,  -506,  -605,  -504,  -602,  -165,   -68,  -953,   520,  -953,
    -953,   766,  -953,  -953,  1000,  -124,   712,  -953,  -457,  -953,
    -953,  -953,  -953,  -953,  -171,  -953
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -227
static const yytype_int16 yytable[] =
{
     233,   260,   243,   363,   478,   724,   644,   642,   230,   305,
     306,   595,   624,   508,   507,   225,   320,   321,   326,   323,
     325,   759,   333,   334,   335,   540,   612,   613,   938,   225,
     341,   315,   609,   344,   386,  1034,   387,   352,   757,   755,
     388,   389,   390,   585,   619,   299,   248,   519,   278,   540,
     601,   366,   367,   368,   369,    16,   308,   832,   347,   646,
     251,   252,   348,   349,   329,   395,   605,   225,   809,   397,
    1047,   225,   873,   874,  1013,   371,   342,   343,   822,   345,
     346,   555,   917,   353,   558,   355,   251,   252,   360,   997,
     251,   252,     7,   356,   765,  1071,   421,   422,   302,   424,
    1098,   941,   279,   942,   251,   252,   303,     8,   875,   876,
     347,   597,   877,     9,   348,   349,   598,   566,  1014,    11,
    1035,   502,  1036,   540,   567,   371,   225,   633,   290,    13,
     340,  1092,   635,    14,   634,   249,   391,    15,   878,   636,
    1083,   879,   880,   881,   882,   883,   884,   885,   886,   887,
     888,   889,   637,   484,   164,   602,   165,   918,   372,   638,
    1037,   831,   215,   586,   382,   799,   398,   357,   762,   255,
     239,   300,   540,   827,  1099,   316,   541,   845,   280,  1067,
     256,   240,   241,   239,  1028,   350,   606,   853,   810,   796,
     330,   281,   762,   167,   240,   241,   239,   392,   823,   255,
     290,   257,   258,   168,   291,   224,   256,   240,   241,   232,
     393,   239,   805,   639,   394,   574,   239,   575,   576,   948,
     640,   611,   240,   241,   393,   227,   228,   240,   241,   509,
     239,   257,   258,   310,   311,   234,   947,   946,   279,   227,
     228,   240,   241,   520,   802,   611,   543,   544,   251,   252,
      76,   803,   750,   338,   592,   751,   538,   539,   169,   740,
     170,   229,   226,   794,   414,   290,   226,   309,   239,   171,
     624,   559,   560,   859,   172,   229,   291,   227,   228,   240,
     241,   227,   228,   173,   872,   829,   580,   174,   584,   525,
     310,   311,   589,   590,   591,   175,   483,   279,   282,   537,
     596,   176,   515,   547,   283,   492,   493,   550,   494,   495,
     496,   497,   498,   229,   280,   839,   226,   229,   561,   611,
     620,   226,   840,   645,   292,   372,   510,   281,   841,   347,
     293,   227,   228,   348,   349,   842,   227,   228,   516,   251,
     252,   291,   843,   237,   543,   544,   374,   527,   177,   844,
     263,   264,   265,   266,   267,   178,   268,   255,   269,   270,
     271,   272,   273,   315,   656,   657,   553,   229,   542,   375,
     557,   849,   229,   280,   850,     1,     2,   631,   565,  1053,
     262,   955,   -15,   828,   716,   701,   281,   251,   252,   257,
     258,   528,   529,   543,   544,   238,   292,   615,   179,   275,
     643,   180,   293,   975,   535,   825,   977,   378,   379,   380,
     251,   252,   834,   835,   253,   254,   253,   254,   376,   377,
     378,   379,   380,   294,   295,   376,   377,   378,   379,   380,
     181,   382,  1041,  1042,   282,   378,   379,   380,   641,   182,
     283,   263,   264,   265,   266,   267,   183,   268,   255,   269,
     270,   271,   272,   273,   966,   967,   968,  1016,   319,   274,
     322,   292,   184,   284,   285,   185,   186,   293,   187,   188,
     664,   414,   189,   190,   753,   376,   377,   378,   379,   380,
     257,   258,  1039,   945,  1000,  1040,   191,   192,   294,   295,
     275,  1087,   338,   282,   193,   194,   255,   195,   196,   283,
     197,   198,  1006,   364,   199,  1007,   407,   324,   200,   376,
     377,   378,   379,   380,   201,   616,   814,   202,   203,   255,
    1065,   204,   284,   285,   403,   894,   205,   469,   257,   258,
     256,   370,   206,   671,   207,   766,   314,   672,   673,   769,
     208,   209,   772,   773,   327,   328,   251,   252,   210,   742,
     212,   257,   258,   213,   895,   896,   376,   377,   378,   379,
     380,   897,   214,   818,   858,   816,   220,   361,   862,   752,
     898,   865,   866,   867,  1089,   869,   870,   871,   899,   221,
     833,   376,   377,   378,   379,   380,   674,   222,   396,   244,
     245,   246,   247,   767,   768,   223,   235,   771,   408,   900,
     675,   676,   677,   365,   678,   373,   679,   406,   251,   252,
     409,   413,   410,   851,   411,   414,   423,   419,   420,   416,
     415,   868,   376,   954,   378,   379,   380,   893,   417,   491,
     680,   681,   682,   425,   683,   426,   427,   684,   685,   686,
     687,   688,   429,   689,   431,   432,   433,   434,   263,   264,
     265,   266,   267,   437,   268,   255,   269,   270,   271,   272,
     273,   438,  -226,   440,   690,   691,   274,   376,   377,   378,
     379,   380,   628,   692,   693,   694,   695,   696,   697,   922,
     376,   377,   378,   379,   380,   441,   923,   257,   258,   442,
     443,   698,   444,   445,   447,   446,   847,   275,   448,   449,
     450,   451,   924,   452,   981,   699,   857,   453,   454,   457,
     455,   456,   956,   458,   629,   461,   459,   255,   460,   925,
     648,   376,   377,   378,   379,   380,  1090,   463,   256,   464,
    1094,   465,   466,   467,   468,   470,   535,   251,   252,   471,
     472,   473,  1103,   474,   475,  1010,  1011,  1012,   476,   257,
     258,   477,   480,   926,   927,   928,   929,   481,   930,   931,
     932,   933,   934,   376,   377,   378,   379,   380,   482,   376,
     377,   378,   379,   380,    16,   485,   490,   486,   487,   229,
     488,   499,    17,   952,   501,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,   500,   511,   512,
      30,    31,    32,    33,    34,   518,    35,   513,   522,   654,
     523,    36,    37,    38,    39,    40,    41,    42,    43,   670,
      44,    45,    46,  1043,    47,   524,    48,    49,    50,    51,
      52,    53,   526,   531,   532,    54,    55,   533,    56,   263,
     264,   265,   266,   267,    57,   268,   255,   269,   270,   271,
     272,   273,   534,   548,   549,   551,   562,   274,   376,   377,
     378,   379,   380,   552,    58,   705,   554,   556,   376,   377,
     378,   379,   380,   711,   563,   564,   569,   570,   257,   258,
     571,    59,   572,   573,   854,   579,  1008,    60,   275,   582,
      61,    62,   588,    63,   581,    64,   583,   593,   251,   252,
     603,    65,    66,   600,   608,    67,    68,   604,    69,  -175,
      70,   251,   252,   618,   376,   377,   378,   379,   380,  1096,
     621,   623,   376,   377,   378,   379,   380,   632,   622,   647,
     649,   652,  1105,   376,   377,   378,   379,   380,   650,   653,
      71,    72,    73,    74,    78,    75,   658,   659,   660,  1073,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,   661,    76,
     662,  1086,   663,   665,   666,    77,    78,   667,   668,   669,
     307,   704,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
     263,   264,   265,   266,   267,   708,   268,   255,   269,   270,
     271,   272,   273,   263,   264,   265,   266,   267,   274,   268,
     255,   269,   270,   271,   272,   273,   709,   710,   712,   714,
     713,   274,   715,   717,   719,   716,   718,   720,   721,   257,
     258,   722,   729,   723,   725,   726,   735,   728,   730,   275,
     731,   216,   257,   258,   732,   734,   736,    79,   737,    81,
     738,    83,   275,    85,    80,    87,    82,    89,    84,    91,
      86,    93,    88,    95,    90,   664,    92,   739,    94,   741,
      96,   744,   745,   748,   746,   747,   749,   760,   309,   761,
     764,   762,   535,   763,   770,   774,   795,   776,   775,   777,
     778,   779,   780,   781,   782,   783,   784,   785,   786,   787,
     788,   789,   790,   792,   793,   791,   796,   798,   799,   800,
     801,   806,   807,   808,   817,   812,   813,   815,   819,   821,
     820,   830,   860,   848,   846,   863,   904,   861,   852,   855,
     856,   864,   902,   905,   907,   908,   909,   911,   913,   912,
     915,   910,   916,   920,   936,   921,   937,   939,   940,   950,
     951,   964,   949,   953,   957,   958,   959,   960,   961,   962,
     963,   965,   971,   973,   974,   976,   978,   979,   982,   984,
     983,   985,   986,   987,   988,   989,   990,   991,   992,   993,
     994,   996,   999,   995,  1001,  1004,  1009,  1005,  1017,  1002,
    1018,  1019,  1020,  1022,  1021,  1024,  1025,  1026,  1029,  1030,
    1033,  1045,  1031,  1046,  1049,  1051,  1052,  1056,  1058,  1060,
    1061,  1063,  1064,  1055,  1066,  1068,  1069,  1070,  1074,    10,
    1075,  1093,  1104,   362,   217,  1076,   211,   405,   594,  1003,
     655,  1077,  1080,  1082,  1084,  1088,  1095,  1101,   998,  1048,
     980,  1106,   914,   354,   514,  1027,  1078,   289,  1044,  1057,
     797,   892,   517,   651,  1079,   630,   702,   837,     0,     0,
       0,     0,   428
};

static const yytype_int16 yycheck[] =
{
     101,   170,   167,   215,   359,   588,   512,   511,    76,   178,
     179,   468,   489,   394,   394,     4,   185,   186,   189,   188,
     189,   645,   193,   194,   195,     3,   480,   481,   824,     4,
     199,    55,   477,   202,    23,     4,    25,   206,   643,   641,
      29,    30,    31,    79,    79,    79,     4,     3,   172,     3,
      64,   220,   221,   222,   223,     6,   180,    79,     4,   513,
      79,    80,     8,     9,    79,   234,    79,     4,    79,   234,
     997,     4,    81,    82,   161,     8,   200,   201,    79,   203,
     204,   436,     4,   207,   439,   209,    79,    80,   212,   938,
      79,    80,   198,     5,   658,  1047,   265,   266,    71,   268,
       5,   141,     3,   143,    79,    80,    79,   198,   117,   118,
       4,    74,   121,     0,     8,     9,    79,    72,   205,   115,
      89,     4,    91,     3,    79,     8,     4,   197,     3,   205,
     198,  1083,   197,   205,   204,    93,   125,   198,   147,   204,
    1067,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   197,   365,   197,   169,   116,    79,   226,   204,
     129,   744,   201,   199,   232,   197,   234,    79,   200,   188,
     205,   205,     3,   737,    79,   199,     7,   760,    79,  1028,
     199,   216,   217,   205,   980,    79,   199,   770,   199,   197,
     205,    92,   200,   198,   216,   217,   205,   186,   199,   188,
       3,   220,   221,   198,    79,   207,   199,   216,   217,   199,
     199,   205,   718,   197,   203,    79,   205,    81,    82,   843,
     204,   199,   216,   217,   199,   214,   215,   216,   217,   394,
     205,   220,   221,   222,   223,   256,   841,   839,     3,   214,
     215,   216,   217,   199,    79,   199,   224,   225,    79,    80,
     201,    86,   633,   199,   466,   635,   425,   426,   198,   614,
     198,   250,   199,   708,     4,     3,   199,   199,   205,   198,
     747,   440,   441,   777,   198,   250,    79,   214,   215,   216,
     217,   214,   215,   198,   790,   739,   455,   198,   459,   413,
     222,   223,   463,   464,   465,   198,   364,     3,   199,   423,
     469,   198,   403,   427,   205,   373,   374,   431,   376,   377,
     378,   379,   380,   250,    79,   197,   199,   250,   442,   199,
     485,   199,   204,   203,   199,   393,   394,    92,   197,     4,
     205,   214,   215,     8,     9,   204,   214,   215,   406,    79,
      80,    79,   197,     3,   224,   225,   179,   415,   198,   204,
     181,   182,   183,   184,   185,   198,   187,   188,   189,   190,
     191,   192,   193,    55,   533,   534,   434,   250,   199,   202,
     438,   197,   250,    79,   200,    15,    16,   501,   446,   197,
      97,   198,   200,   738,   201,   554,    92,    79,    80,   220,
     221,    84,    85,   224,   225,   197,   199,   139,   198,   230,
     203,   198,   205,   909,    78,    79,   912,   253,   254,   255,
      79,    80,   210,   211,    83,    84,    83,    84,   251,   252,
     253,   254,   255,   226,   227,   251,   252,   253,   254,   255,
     198,   499,   990,   991,   199,   253,   254,   255,   203,   198,
     205,   181,   182,   183,   184,   185,   198,   187,   188,   189,
     190,   191,   192,   193,    40,    41,    42,   961,   205,   199,
      90,   199,   198,   228,   229,   198,   198,   205,   198,   198,
       3,     4,   198,   198,   639,   251,   252,   253,   254,   255,
     220,   221,   988,   838,   941,   989,   198,   198,   226,   227,
     230,  1074,   199,   199,   198,   198,   188,   198,   198,   205,
     198,   198,   197,   256,   198,   200,   257,   199,   198,   251,
     252,   253,   254,   255,   198,   257,   728,   198,   198,   188,
    1026,   198,   228,   229,   203,    75,   198,   197,   220,   221,
     199,   199,   198,     4,   198,   659,   182,     8,     9,   663,
     198,   198,   666,   667,   190,   191,    79,    80,   198,   617,
     198,   220,   221,   198,   104,   105,   251,   252,   253,   254,
     255,   111,   198,   732,   776,   730,   198,   213,   780,   637,
     120,   783,   784,   785,  1080,   787,   788,   789,   128,   198,
     745,   251,   252,   253,   254,   255,    57,   198,   234,    10,
      11,    12,    13,   661,   662,   198,   198,   665,   197,   149,
      71,    72,    73,   201,    75,   201,    77,   201,    79,    80,
     197,   199,   197,   197,   196,     4,   199,   263,   264,   197,
     201,   786,   251,   252,   253,   254,   255,   792,   197,   200,
     101,   102,   103,   199,   105,   199,   199,   108,   109,   110,
     111,   112,   199,   114,   259,   197,     3,   201,   181,   182,
     183,   184,   185,     3,   187,   188,   189,   190,   191,   192,
     193,   201,   203,   197,   135,   136,   199,   251,   252,   253,
     254,   255,   202,   144,   145,   146,   147,   148,   149,    88,
     251,   252,   253,   254,   255,   197,    95,   220,   221,   197,
     197,   162,   197,     4,   197,   201,   764,   230,   197,   199,
       4,   197,   111,   197,   916,   176,   774,   197,   197,     4,
     197,   197,   877,   197,   202,     6,   197,   188,   197,   128,
     202,   251,   252,   253,   254,   255,  1081,   197,   199,   197,
    1085,   197,   197,     4,   197,   197,    78,    79,    80,   197,
     197,   197,  1097,   197,   197,   957,   958,   959,   197,   220,
     221,   197,   197,   162,   163,   164,   165,   197,   167,   168,
     169,   170,   171,   251,   252,   253,   254,   255,   202,   251,
     252,   253,   254,   255,     6,   197,   200,   197,   197,   250,
     197,   197,    14,   851,   198,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,   200,   198,   198,
      32,    33,    34,    35,    36,     3,    38,   198,     3,   202,
     197,    43,    44,    45,    46,    47,    48,    49,    50,   202,
      52,    53,    54,   992,    56,   202,    58,    59,    60,    61,
      62,    63,   197,     5,   206,    67,    68,   197,    70,   181,
     182,   183,   184,   185,    76,   187,   188,   189,   190,   191,
     192,   193,   197,   194,   200,    93,    78,   199,   251,   252,
     253,   254,   255,   197,    96,   202,   197,   197,   251,   252,
     253,   254,   255,   202,    78,   197,    37,     4,   220,   221,
     197,   113,    37,    39,   202,   162,   954,   119,   230,   197,
     122,   123,   197,   125,    87,   127,    87,   197,    79,    80,
     166,   133,   134,    91,     4,   137,   138,    97,   140,   124,
     142,    79,    80,   202,   251,   252,   253,   254,   255,  1088,
     130,   132,   251,   252,   253,   254,   255,   197,   131,   204,
       3,   200,  1101,   251,   252,   253,   254,   255,   197,     4,
     172,   173,   174,   175,   208,   177,   258,   197,   200,  1050,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   197,   201,
     197,  1072,   197,   201,   197,   207,   208,   197,   197,   205,
     161,   205,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     181,   182,   183,   184,   185,   197,   187,   188,   189,   190,
     191,   192,   193,   181,   182,   183,   184,   185,   199,   187,
     188,   189,   190,   191,   192,   193,   197,     4,   197,     4,
     197,   199,   197,   197,   197,   201,   198,     4,   197,   220,
     221,     4,    78,   197,   197,   197,     4,   197,   197,   230,
     197,   208,   220,   221,   198,   197,   197,   214,   197,   216,
     124,   218,   230,   220,   215,   222,   217,   224,   219,   226,
     221,   228,   223,   230,   225,     3,   227,   197,   229,   257,
     231,   197,   197,   200,   198,   197,     4,   197,   199,     3,
     197,   200,    78,   206,   197,   199,     5,   198,   200,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   197,   197,   200,   197,     4,   197,   162,
       4,   162,    87,   197,    65,   205,    87,   197,   197,   197,
      94,   203,   100,   200,   197,   106,     4,    99,   200,   200,
     200,   107,   197,   197,   197,     4,   198,   197,     4,   198,
     197,   202,   197,   197,     4,   197,     4,   197,   197,     5,
      40,     3,   200,   200,   198,   198,   198,   198,   198,   198,
     198,   197,    98,     4,   197,   162,   200,   197,   200,    89,
      66,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   197,     7,   200,   198,   197,    79,   200,     4,   204,
     129,     4,   197,    95,   198,   197,     4,   198,   197,   197,
       4,     4,    94,     4,   198,     5,    40,   200,    97,   198,
     162,     4,   200,   258,   200,    89,    88,   200,   197,     5,
     200,    88,  1100,   214,    68,   195,    62,   238,   468,   944,
     530,   200,   198,   200,   197,   197,   197,   197,   939,   998,
     915,   200,   810,   208,   402,   979,  1055,   174,   993,  1020,
     712,   792,   407,   523,  1060,   499,   554,   747,    -1,    -1,
      -1,    -1,   272
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   261,   262,   263,   264,   198,   198,     0,
     262,   115,   272,   205,   205,   198,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      32,    33,    34,    35,    36,    38,    43,    44,    45,    46,
      47,    48,    49,    50,    52,    53,    54,    56,    58,    59,
      60,    61,    62,    63,    67,    68,    70,    76,    96,   113,
     119,   122,   123,   125,   127,   133,   134,   137,   138,   140,
     142,   172,   173,   174,   175,   177,   201,   207,   208,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   265,   266,   267,
     274,   276,   277,   278,   279,   281,   282,   290,   293,   294,
     296,   297,   304,   306,   307,   309,   311,   313,   317,   318,
     319,   322,   324,   331,   338,   341,   347,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   363,   364,   365,
     366,   370,   371,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   197,   116,   273,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   296,   198,   198,   198,   201,   208,   278,   280,   298,
     198,   198,   198,   198,   207,     4,   199,   214,   215,   250,
     426,   436,   199,   274,   256,   198,   308,     3,   197,   205,
     216,   217,   412,   425,    10,    11,    12,    13,     4,    93,
     437,    79,    80,    83,    84,   188,   199,   220,   221,   339,
     414,   415,    97,   181,   182,   183,   184,   185,   187,   189,
     190,   191,   192,   193,   199,   230,   414,   434,   435,     3,
      79,    92,   199,   205,   228,   229,   349,   423,   424,   349,
       3,    79,   199,   205,   226,   227,   343,   421,   422,    79,
     205,   396,    71,    79,   406,   414,   414,   161,   435,   199,
     222,   223,   417,   418,   417,    55,   199,   444,   445,   205,
     414,   414,    90,   414,   199,   414,   444,   417,   417,    79,
     205,   321,   397,   444,   444,   444,   413,   426,   199,   320,
     426,   414,   435,   435,   414,   435,   435,     4,     8,     9,
      79,   438,   414,   435,   321,   435,     5,    79,   332,   314,
     435,   417,   277,   413,   256,   201,   414,   414,   414,   414,
     199,     8,   426,   201,   179,   202,   251,   252,   253,   254,
     255,   425,   426,   431,   432,   433,    23,    25,    29,    30,
      31,   125,   186,   199,   203,   414,   417,   425,   426,   440,
     441,   442,   312,   203,   275,   273,   201,   257,   197,   197,
     197,   196,   416,   199,     4,   201,   197,   197,   326,   417,
     417,   414,   414,   199,   414,   199,   199,   199,   434,   199,
     435,   259,   197,     3,   201,   350,   348,     3,   201,   342,
     197,   197,   197,   197,   197,     4,   201,   197,   197,   199,
       4,   197,   197,   197,   197,   197,   197,     4,   197,   197,
     197,     6,   295,   197,   197,   197,   197,     4,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   275,   315,
     197,   197,   202,   426,   413,   197,   197,   197,   197,   291,
     200,   200,   426,   426,   426,   426,   426,   426,   426,   197,
     200,   198,     4,   286,   287,   288,   289,   415,   418,   425,
     426,   198,   198,   198,   276,   274,   426,   412,     3,     3,
     199,   372,     3,   197,   202,   435,   197,   426,    84,    85,
     340,     5,   206,   197,   197,    78,   334,   435,   414,   414,
       3,     7,   199,   224,   225,   419,   420,   435,   194,   200,
     435,    93,   197,   426,   197,   275,   197,   426,   275,   414,
     414,   435,    78,    78,   197,   426,    72,    79,   407,    37,
       4,   197,    37,    39,    79,    81,    82,   408,   411,   162,
     414,    87,   197,    87,   444,    79,   199,   327,   197,   444,
     444,   444,   413,   197,   320,   438,   414,    74,    79,   443,
      91,    64,   169,   166,    97,    79,   199,   328,     4,   406,
     316,   199,   419,   419,   310,   139,   257,   305,   202,    79,
     425,   130,   131,   132,   279,   428,   429,   430,   202,   202,
     431,   435,   197,   197,   204,   197,   204,   197,   204,   197,
     204,   203,   423,   203,   421,   203,   419,   204,   202,     3,
     197,   416,   200,     4,   202,   326,   414,   414,   258,   197,
     200,   197,   197,   197,     3,   201,   197,   197,   197,   205,
     202,     4,     8,     9,    57,    71,    72,    73,    75,    77,
     101,   102,   103,   105,   108,   109,   110,   111,   112,   114,
     135,   136,   144,   145,   146,   147,   148,   149,   162,   176,
     351,   414,   436,   439,   205,   202,   344,   336,   197,   197,
       4,   202,   197,   197,     4,   197,   201,   197,   198,   197,
       4,   197,     4,   197,   407,   197,   197,   362,   197,    78,
     197,   197,   198,   367,   197,     4,   197,   197,   124,   197,
     275,   257,   426,   299,   197,   197,   198,   197,   200,     4,
     418,   415,   426,   425,   283,   424,   284,   422,   285,   420,
     197,     3,   200,   206,   197,   334,   435,   426,   426,   435,
     197,   426,   435,   435,   199,   200,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   200,   197,   197,   406,     5,   197,   397,     4,   197,
     162,     4,    79,    86,   410,   421,   162,    87,   197,    79,
     199,   329,   205,    87,   413,   197,   425,    65,   414,   197,
      94,   197,    79,   199,   330,    79,   333,   334,   275,   419,
     203,   407,    79,   425,   210,   211,   427,   428,   292,   197,
     204,   197,   204,   197,   204,   407,   197,   426,   200,   197,
     200,   197,   200,   407,   202,   200,   200,   426,   413,   423,
     100,    99,   413,   106,   107,   413,   413,   413,   425,   413,
     413,   413,   421,    81,    82,   117,   118,   121,   147,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     345,   409,   411,   425,    75,   104,   105,   111,   120,   128,
     149,   337,   197,   270,     4,   197,   398,   197,     4,   198,
     202,   197,   198,     4,   339,   197,   197,     4,    79,   335,
     197,   197,    88,    95,   111,   128,   162,   163,   164,   165,
     167,   168,   169,   170,   171,   368,     4,     4,   398,   197,
     197,   141,   143,   300,   301,   275,   424,   422,   420,   200,
       5,    40,   426,   200,   252,   198,   425,   198,   198,   198,
     198,   198,   198,   198,     3,   197,    40,    41,    42,   399,
     400,    98,   401,     4,   197,   421,   162,   421,   200,   197,
     330,   413,   200,    66,    89,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   200,   197,   401,   335,     7,
     438,   198,   204,   300,   197,   200,   197,   200,   426,    79,
     413,   413,   413,   161,   205,   346,   423,     4,   129,     4,
     197,   198,    95,   403,   197,     4,   198,   340,   398,   197,
     197,    94,   404,     4,     4,    89,    91,   129,   369,   421,
     423,   372,   372,   414,   396,     4,     4,   403,   336,   198,
     303,     5,    40,   197,   268,   258,   200,   399,    97,   402,
     198,   162,   405,     4,   200,   421,   200,   401,    89,    88,
     200,   405,   302,   274,   197,   200,   195,   200,   346,   404,
     198,   382,   200,   403,   197,   323,   274,   407,   197,   421,
     275,   383,   405,    88,   275,   197,   414,   325,     5,    79,
     271,   197,   269,   275,   270,   414,   200
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

  case 113:

/* Line 1455 of yacc.c  */
#line 581 "lev_comp.y"
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

  case 114:

/* Line 1455 of yacc.c  */
#line 597 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 603 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 609 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 615 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 621 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 627 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 633 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 639 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 645 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 655 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 665 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 675 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 685 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 695 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 705 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 717 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 722 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 729 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 734 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 741 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 746 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 753 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 757 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 763 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 768 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 775 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 779 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 785 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 789 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 795 "lev_comp.y"
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

  case 144:

/* Line 1455 of yacc.c  */
#line 817 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 821 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 832 "lev_comp.y"
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

  case 147:

/* Line 1455 of yacc.c  */
#line 887 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 893 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 897 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 903 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 910 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 914 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 922 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 926 "lev_comp.y"
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

  case 155:

/* Line 1455 of yacc.c  */
#line 949 "lev_comp.y"
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

  case 158:

/* Line 1455 of yacc.c  */
#line 994 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1004 "lev_comp.y"
    {
		  ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1007 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1017 "lev_comp.y"
    {
		  ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1022 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1036 "lev_comp.y"
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

  case 166:

/* Line 1455 of yacc.c  */
#line 1073 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1078 "lev_comp.y"
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

  case 168:

/* Line 1455 of yacc.c  */
#line 1107 "lev_comp.y"
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

  case 169:

/* Line 1455 of yacc.c  */
#line 1121 "lev_comp.y"
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

  case 170:

/* Line 1455 of yacc.c  */
#line 1137 "lev_comp.y"
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

  case 171:

/* Line 1455 of yacc.c  */
#line 1157 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1169 "lev_comp.y"
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

  case 173:

/* Line 1455 of yacc.c  */
#line 1189 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1195 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1205 "lev_comp.y"
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

  case 176:

/* Line 1455 of yacc.c  */
#line 1223 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1233 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1239 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1244 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1249 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1256 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1263 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1272 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1280 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1291 "lev_comp.y"
    {
		      long rflags = (yyvsp[(8) - (11)].i);
		      long flmt = (long)(yyvsp[(9) - (11)].i);
		      long flt = (long)(yyvsp[(10) - (11)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      //if (flmt == -1) flmt = ROOM;
		      //if (flt == -1) flt = 0;

		      add_opvars(splev, "iiiiiiiiio",
				 VA_PASS10(flt, flmt, rflags, ERR, ERR,
					  (yyvsp[(5) - (11)].crd).x, (yyvsp[(5) - (11)].crd).y, (yyvsp[(7) - (11)].sze).width, (yyvsp[(7) - (11)].sze).height,
					  SPO_SUBROOM));
		      break_stmt_start();
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1307 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1314 "lev_comp.y"
    {
		      long rflags = (yyvsp[(10) - (13)].i);
		      long flmt = (long)(yyvsp[(11) - (13)].i);
		      long flt = (long)(yyvsp[(12) - (13)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      //if (flmt == -1) flmt = ROOM;
		      //if (flt == -1) flt = 0;

		      add_opvars(splev, "iiiiiiiiio",
				 VA_PASS10(flt, flmt, rflags,
					  (yyvsp[(7) - (13)].crd).x, (yyvsp[(7) - (13)].crd).y, (yyvsp[(5) - (13)].crd).x, (yyvsp[(5) - (13)].crd).y,
					  (yyvsp[(9) - (13)].sze).width, (yyvsp[(9) - (13)].sze).height, SPO_ROOM));
		      break_stmt_start();
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1330 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1337 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1341 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1347 "lev_comp.y"
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

  case 192:

/* Line 1455 of yacc.c  */
#line 1357 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1363 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1372 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1378 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1383 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1389 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1394 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1400 "lev_comp.y"
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

  case 200:

/* Line 1455 of yacc.c  */
#line 1411 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1425 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1429 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1439 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1446 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1454 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1459 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1464 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1469 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1474 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1479 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1484 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1492 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1499 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1507 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1523 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1527 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1533 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1541 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1547 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1555 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1563 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1568 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1574 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1580 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1586 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1592 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1597 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1602 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1607 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1612 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1617 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1622 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1627 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1632 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1637 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1642 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1648 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1655 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1664 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1668 "lev_comp.y"
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

  case 251:

/* Line 1455 of yacc.c  */
#line 1681 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1689 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1699 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1707 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1716 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1723 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1731 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1737 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1742 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1747 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1752 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1757 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1762 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1767 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1772 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1777 "lev_comp.y"
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

  case 267:

/* Line 1455 of yacc.c  */
#line 1791 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1797 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1802 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1807 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1812 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1817 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1822 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1827 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1832 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1837 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1842 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1847 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1852 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x400000;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1857 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x800000;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1862 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x1000000;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1867 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x2000000;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1872 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x4000000;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1877 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x8000000;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1884 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1890 "lev_comp.y"
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

  case 287:

/* Line 1455 of yacc.c  */
#line 1919 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1924 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1932 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1938 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1942 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1950 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1957 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1964 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1974 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1984 "lev_comp.y"
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

  case 297:

/* Line 1455 of yacc.c  */
#line 1999 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2009 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2013 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2019 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2025 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2031 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2035 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2041 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2047 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2054 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2062 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2068 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2073 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2078 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2083 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2088 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2094 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2100 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2105 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2110 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2115 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2120 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2125 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2135 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2141 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2147 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2152 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2159 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2166 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2172 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2178 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2184 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2189 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2196 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2202 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2212 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2218 "lev_comp.y"
    {
		      long irr;
		      long rt = (yyvsp[(7) - (11)].i);
		      long rflags = (yyvsp[(8) - (11)].i);
		      long flmt = (long)(yyvsp[(9) - (11)].i);
		      long flt = (long)(yyvsp[(10) - (11)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      //if (flmt == -1) flmt = 0;
		      //if (flt == -1) flt = 0;

		      if (!(rflags & 1)) rt += MAXRTYPE+1;
		      irr = ((rflags & 2) != 0);
		      add_opvars(splev, "iiiiio",
				 VA_PASS6((long)(yyvsp[(5) - (11)].i), rt, rflags, flmt, flt, SPO_REGION));
		      (yyval.i) = (irr || (rflags & 1) || rt != OROOM);
		      break_stmt_start();
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2237 "lev_comp.y"
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

  case 339:

/* Line 1455 of yacc.c  */
#line 2249 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2253 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2259 "lev_comp.y"
    {
		      add_opvars(splev, "Miio",
				 VA_PASS4(-1, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2264 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2271 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2275 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2281 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2287 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2293 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2297 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2303 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2307 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2312 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2319 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2323 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2330 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2334 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2339 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2346 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2350 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(0, SPO_TREE));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2358 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2364 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2371 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2375 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2382 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2393 "lev_comp.y"
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

  case 367:

/* Line 1455 of yacc.c  */
#line 2406 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2410 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2416 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2420 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2427 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2431 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2435 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2441 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2445 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2451 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2457 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2461 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2467 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2473 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2478 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2495 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2503 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2513 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2522 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2527 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2534 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2545 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2551 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2555 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2559 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2566 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2576 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2583 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2587 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2593 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2597 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2605 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2609 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2616 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2626 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2639 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2643 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2650 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2660 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2669 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2680 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2684 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2691 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2701 "lev_comp.y"
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

  case 421:

/* Line 1455 of yacc.c  */
#line 2712 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2721 "lev_comp.y"
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

  case 423:

/* Line 1455 of yacc.c  */
#line 2731 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2737 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2741 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2748 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2758 "lev_comp.y"
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

  case 428:

/* Line 1455 of yacc.c  */
#line 2769 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2778 "lev_comp.y"
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

  case 430:

/* Line 1455 of yacc.c  */
#line 2788 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2792 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2798 "lev_comp.y"
    { ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2800 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2806 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2810 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2814 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2818 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2826 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2835 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2839 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2843 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2847 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2851 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2854 "lev_comp.y"
    { ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2858 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2865 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2874 "lev_comp.y"
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

  case 452:

/* Line 1455 of yacc.c  */
#line 2915 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2919 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2926 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2933 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2943 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2947 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2955 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2959 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2963 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2967 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2971 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2976 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2980 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2984 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2989 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2994 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2999 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 3003 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 3008 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 3013 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 3017 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3021 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3026 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3030 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3037 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3043 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3047 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3053 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3069 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3073 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3077 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3081 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3103 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3107 "lev_comp.y"
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

  case 499:

/* Line 1455 of yacc.c  */
#line 3133 "lev_comp.y"
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
#line 7080 "lev_comp.tab.c"
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
#line 3161 "lev_comp.y"


/*lev_comp.y*/

