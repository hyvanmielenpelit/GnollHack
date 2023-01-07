
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

/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-28 */

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

extern int FDECL(reverse_jmp_opcode, (long));

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
#line 225 "lev_comp.tab.c"

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
     MONSTER_GENERATION_ID = 284,
     object_ID = 285,
     monster_ID = 286,
     terrain_ID = 287,
     MAZEWALK_ID = 288,
     WALLIFY_ID = 289,
     REGION_ID = 290,
     SPECIAL_REGION_ID = 291,
     SPECIAL_LEVREGION_ID = 292,
     SPECIAL_REGION_TYPE = 293,
     NAMING_ID = 294,
     NAMING_TYPE = 295,
     FILLING = 296,
     IRREGULAR = 297,
     JOINED = 298,
     ALTAR_ID = 299,
     ANVIL_ID = 300,
     NPC_ID = 301,
     LADDER_ID = 302,
     STAIR_ID = 303,
     NON_DIGGABLE_ID = 304,
     NON_PASSWALL_ID = 305,
     ROOM_ID = 306,
     ARTIFACT_NAME_ID = 307,
     PORTAL_ID = 308,
     TELEPRT_ID = 309,
     BRANCH_ID = 310,
     LEV = 311,
     MINERALIZE_ID = 312,
     AGE_ID = 313,
     CORRIDOR_ID = 314,
     GOLD_ID = 315,
     ENGRAVING_ID = 316,
     FOUNTAIN_ID = 317,
     THRONE_ID = 318,
     MODRON_PORTAL_ID = 319,
     LEVEL_TELEPORTER_ID = 320,
     LEVEL_TELEPORT_DIRECTION_TYPE = 321,
     LEVEL_TELEPORT_END_TYPE = 322,
     POOL_ID = 323,
     SINK_ID = 324,
     NONE = 325,
     RAND_CORRIDOR_ID = 326,
     DOOR_STATE = 327,
     LIGHT_STATE = 328,
     CURSE_TYPE = 329,
     MYTHIC_TYPE = 330,
     ENGRAVING_TYPE = 331,
     KEYTYPE_ID = 332,
     LEVER_ID = 333,
     NO_PICKUP_ID = 334,
     DIRECTION = 335,
     RANDOM_TYPE = 336,
     RANDOM_TYPE_BRACKET = 337,
     A_REGISTER = 338,
     ALIGNMENT = 339,
     LEFT_OR_RIGHT = 340,
     CENTER = 341,
     TOP_OR_BOT = 342,
     ALTAR_TYPE = 343,
     ALTAR_SUBTYPE = 344,
     UP_OR_DOWN = 345,
     ACTIVE_OR_INACTIVE = 346,
     MODRON_PORTAL_TYPE = 347,
     NPC_TYPE = 348,
     FOUNTAIN_TYPE = 349,
     SPECIAL_OBJECT_TYPE = 350,
     CMAP_TYPE = 351,
     FLOOR_SUBTYPE = 352,
     FLOOR_SUBTYPE_ID = 353,
     FLOOR_ID = 354,
     FLOOR_TYPE = 355,
     FLOOR_TYPE_ID = 356,
     DECORATION_ID = 357,
     DECORATION_TYPE = 358,
     DECORATION_DIR = 359,
     DECORATION_ITEM_STATE = 360,
     ELEMENTAL_ENCHANTMENT_TYPE = 361,
     EXCEPTIONALITY_TYPE = 362,
     EXCEPTIONALITY_ID = 363,
     ELEMENTAL_ENCHANTMENT_ID = 364,
     ENCHANTMENT_ID = 365,
     SECRET_DOOR_ID = 366,
     USES_UP_KEY_ID = 367,
     MYTHIC_PREFIX_TYPE = 368,
     MYTHIC_SUFFIX_TYPE = 369,
     MYTHIC_PREFIX_ID = 370,
     MYTHIC_SUFFIX_ID = 371,
     CHARGES_ID = 372,
     SPECIAL_QUALITY_ID = 373,
     SPEFLAGS_ID = 374,
     SUBROOM_ID = 375,
     NAME_ID = 376,
     FLAGS_ID = 377,
     FLAG_TYPE = 378,
     MON_ATTITUDE = 379,
     MON_ALERTNESS = 380,
     SUBTYPE_ID = 381,
     NON_PASSDOOR_ID = 382,
     MON_APPEARANCE = 383,
     ROOMDOOR_ID = 384,
     IF_ID = 385,
     ELSE_ID = 386,
     TERRAIN_ID = 387,
     HORIZ_OR_VERT = 388,
     REPLACE_TERRAIN_ID = 389,
     LOCATION_SUBTYPE_ID = 390,
     DOOR_SUBTYPE = 391,
     BRAZIER_SUBTYPE = 392,
     SIGNPOST_SUBTYPE = 393,
     TREE_SUBTYPE = 394,
     FOREST_ID = 395,
     FOREST_TYPE = 396,
     INITIALIZE_TYPE = 397,
     EXIT_ID = 398,
     SHUFFLE_ID = 399,
     MANUAL_TYPE_ID = 400,
     MANUAL_TYPE = 401,
     QUANTITY_ID = 402,
     BURIED_ID = 403,
     LOOP_ID = 404,
     FOR_ID = 405,
     TO_ID = 406,
     SWITCH_ID = 407,
     CASE_ID = 408,
     BREAK_ID = 409,
     DEFAULT_ID = 410,
     ERODED_ID = 411,
     TRAPPED_STATE = 412,
     RECHARGED_ID = 413,
     INVIS_ID = 414,
     GREASED_ID = 415,
     INDESTRUCTIBLE_ID = 416,
     FEMALE_ID = 417,
     MALE_ID = 418,
     WAITFORU_ID = 419,
     PROTECTOR_ID = 420,
     CANCELLED_ID = 421,
     REVIVED_ID = 422,
     AVENGE_ID = 423,
     FLEEING_ID = 424,
     BLINDED_ID = 425,
     MAXHP_ID = 426,
     LEVEL_ADJUSTMENT_ID = 427,
     KEEP_ORIGINAL_INVENTORY_ID = 428,
     PARALYZED_ID = 429,
     STUNNED_ID = 430,
     CONFUSED_ID = 431,
     SEENTRAPS_ID = 432,
     ALL_ID = 433,
     MONTYPE_ID = 434,
     OBJTYPE_ID = 435,
     TERTYPE_ID = 436,
     TERTYPE2_ID = 437,
     LEVER_EFFECT_TYPE = 438,
     SWITCHABLE_ID = 439,
     CONTINUOUSLY_USABLE_ID = 440,
     TARGET_ID = 441,
     TRAPTYPE_ID = 442,
     EFFECT_FLAG_ID = 443,
     GRAVE_ID = 444,
     BRAZIER_ID = 445,
     SIGNPOST_ID = 446,
     TREE_ID = 447,
     ERODEPROOF_ID = 448,
     FUNCTION_ID = 449,
     MSG_OUTPUT_TYPE = 450,
     COMPARE_TYPE = 451,
     UNKNOWN_TYPE = 452,
     rect_ID = 453,
     fillrect_ID = 454,
     line_ID = 455,
     randline_ID = 456,
     grow_ID = 457,
     selection_ID = 458,
     flood_ID = 459,
     rndcoord_ID = 460,
     circle_ID = 461,
     ellipse_ID = 462,
     filter_ID = 463,
     complement_ID = 464,
     gradient_ID = 465,
     GRADIENT_TYPE = 466,
     LIMITED = 467,
     HUMIDITY_TYPE = 468,
     STRING = 469,
     MAP_ID = 470,
     NQSTRING = 471,
     VARSTRING = 472,
     CFUNC = 473,
     CFUNC_INT = 474,
     CFUNC_STR = 475,
     CFUNC_COORD = 476,
     CFUNC_REGION = 477,
     VARSTRING_INT = 478,
     VARSTRING_INT_ARRAY = 479,
     VARSTRING_STRING = 480,
     VARSTRING_STRING_ARRAY = 481,
     VARSTRING_VAR = 482,
     VARSTRING_VAR_ARRAY = 483,
     VARSTRING_COORD = 484,
     VARSTRING_COORD_ARRAY = 485,
     VARSTRING_REGION = 486,
     VARSTRING_REGION_ARRAY = 487,
     VARSTRING_MAPCHAR = 488,
     VARSTRING_MAPCHAR_ARRAY = 489,
     VARSTRING_MONST = 490,
     VARSTRING_MONST_ARRAY = 491,
     VARSTRING_OBJ = 492,
     VARSTRING_OBJ_ARRAY = 493,
     VARSTRING_SEL = 494,
     VARSTRING_SEL_ARRAY = 495,
     METHOD_INT = 496,
     METHOD_INT_ARRAY = 497,
     METHOD_STRING = 498,
     METHOD_STRING_ARRAY = 499,
     METHOD_VAR = 500,
     METHOD_VAR_ARRAY = 501,
     METHOD_COORD = 502,
     METHOD_COORD_ARRAY = 503,
     METHOD_REGION = 504,
     METHOD_REGION_ARRAY = 505,
     METHOD_MAPCHAR = 506,
     METHOD_MAPCHAR_ARRAY = 507,
     METHOD_MONST = 508,
     METHOD_MONST_ARRAY = 509,
     METHOD_OBJ = 510,
     METHOD_OBJ_ARRAY = 511,
     METHOD_SEL = 512,
     METHOD_SEL_ARRAY = 513,
     DICE = 514
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 153 "lev_comp.y"

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
#line 559 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 571 "lev_comp.tab.c"

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
#define YYLAST   1319

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  277
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  189
/* YYNRULES -- Number of rules.  */
#define YYNRULES  520
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1156

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   514

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   272,   276,     2,
     216,   217,   270,   268,   214,   269,   274,   271,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   215,     2,
       2,   273,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   218,     2,   219,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   220,   275,   221,     2,     2,     2,     2,
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
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    15,    19,    25,
      31,    37,    41,    58,    62,    68,    72,    78,    82,    83,
      86,    87,    90,    91,    94,    96,    98,    99,   103,   107,
     109,   110,   113,   117,   119,   121,   123,   125,   127,   129,
     131,   133,   135,   137,   139,   141,   143,   145,   147,   149,
     151,   153,   155,   157,   159,   161,   163,   165,   167,   169,
     171,   173,   175,   177,   179,   181,   183,   185,   187,   189,
     191,   193,   195,   197,   199,   201,   203,   205,   207,   209,
     211,   213,   215,   217,   219,   221,   223,   225,   227,   229,
     231,   233,   235,   237,   239,   241,   243,   245,   247,   249,
     251,   253,   255,   257,   259,   261,   263,   265,   267,   269,
     271,   273,   275,   277,   279,   281,   283,   285,   287,   289,
     293,   297,   303,   307,   313,   319,   325,   329,   333,   339,
     345,   351,   359,   367,   375,   381,   383,   387,   389,   393,
     395,   399,   401,   405,   407,   411,   413,   417,   419,   423,
     424,   425,   434,   439,   441,   442,   444,   446,   452,   456,
     457,   458,   468,   469,   472,   473,   479,   480,   485,   487,
     490,   492,   499,   500,   504,   505,   512,   513,   518,   519,
     524,   526,   527,   532,   536,   538,   542,   546,   552,   558,
     566,   571,   572,   586,   587,   603,   604,   607,   613,   615,
     621,   623,   629,   631,   637,   639,   650,   657,   659,   661,
     663,   665,   667,   671,   673,   675,   676,   680,   684,   688,
     692,   694,   696,   698,   700,   702,   710,   716,   718,   720,
     722,   724,   728,   729,   735,   740,   741,   745,   747,   749,
     751,   753,   756,   758,   760,   762,   764,   766,   768,   772,
     776,   780,   782,   784,   788,   790,   792,   794,   798,   800,
     802,   804,   808,   812,   813,   819,   822,   823,   827,   829,
     833,   835,   839,   843,   845,   847,   851,   853,   855,   857,
     861,   863,   865,   867,   871,   875,   879,   883,   887,   891,
     895,   899,   901,   903,   905,   907,   911,   915,   919,   925,
     933,   939,   948,   950,   954,   958,   964,   970,   978,   986,
     993,   999,  1000,  1003,  1009,  1013,  1025,  1039,  1046,  1047,
    1051,  1053,  1057,  1061,  1065,  1069,  1071,  1073,  1077,  1081,
    1085,  1089,  1093,  1097,  1099,  1101,  1103,  1105,  1109,  1113,
    1115,  1121,  1131,  1137,  1145,  1149,  1153,  1159,  1169,  1175,
    1181,  1187,  1188,  1202,  1203,  1205,  1213,  1225,  1235,  1249,
    1253,  1261,  1273,  1287,  1295,  1303,  1309,  1317,  1323,  1333,
    1339,  1345,  1349,  1357,  1361,  1369,  1377,  1381,  1387,  1393,
    1397,  1403,  1411,  1421,  1423,  1425,  1427,  1429,  1431,  1432,
    1435,  1437,  1441,  1443,  1445,  1447,  1448,  1452,  1454,  1455,
    1459,  1461,  1462,  1466,  1468,  1470,  1472,  1474,  1476,  1478,
    1480,  1482,  1484,  1488,  1490,  1492,  1497,  1499,  1501,  1506,
    1508,  1510,  1515,  1517,  1522,  1528,  1530,  1534,  1536,  1540,
    1542,  1544,  1549,  1559,  1561,  1563,  1568,  1570,  1576,  1578,
    1580,  1585,  1587,  1589,  1595,  1597,  1599,  1601,  1606,  1608,
    1610,  1616,  1618,  1620,  1622,  1626,  1628,  1630,  1634,  1636,
    1641,  1645,  1649,  1653,  1657,  1661,  1665,  1667,  1669,  1673,
    1675,  1679,  1680,  1682,  1684,  1686,  1688,  1692,  1693,  1695,
    1697,  1700,  1703,  1708,  1715,  1720,  1727,  1734,  1741,  1748,
    1751,  1758,  1767,  1776,  1787,  1802,  1805,  1807,  1811,  1813,
    1817,  1819,  1821,  1823,  1825,  1827,  1829,  1831,  1833,  1835,
    1837,  1839,  1841,  1843,  1845,  1847,  1849,  1851,  1853,  1855,
    1866
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     278,     0,    -1,    -1,   279,    -1,   280,    -1,   280,   279,
      -1,   281,   291,   293,    -1,    16,   215,   222,    -1,    15,
     215,   222,   214,     3,    -1,    17,   215,    11,   214,   391,
      -1,    17,   215,    10,   214,     3,    -1,    17,   215,    13,
      -1,    17,   215,    12,   214,     3,   214,     3,   214,     5,
     214,     5,   214,   427,   214,   290,   289,    -1,    18,   215,
     457,    -1,   140,   215,   141,   214,   142,    -1,   140,   215,
     141,    -1,    29,   215,     4,   214,   303,    -1,    21,   215,
     100,    -1,    -1,   214,   212,    -1,    -1,   214,   434,    -1,
      -1,   214,     3,    -1,     5,    -1,    81,    -1,    -1,   122,
     215,   292,    -1,   123,   214,   292,    -1,   123,    -1,    -1,
     295,   293,    -1,   220,   293,   221,    -1,   336,    -1,   282,
      -1,   283,    -1,   399,    -1,   286,    -1,   284,    -1,   407,
      -1,   285,    -1,   406,    -1,   405,    -1,   403,    -1,   404,
      -1,   408,    -1,   409,    -1,   410,    -1,   411,    -1,   412,
      -1,   380,    -1,   338,    -1,   301,    -1,   300,    -1,   394,
      -1,   350,    -1,   372,    -1,   414,    -1,   415,    -1,   382,
      -1,   383,    -1,   384,    -1,   413,    -1,   316,    -1,   326,
      -1,   328,    -1,   332,    -1,   330,    -1,   313,    -1,   323,
      -1,   309,    -1,   312,    -1,   375,    -1,   385,    -1,   357,
      -1,   373,    -1,   360,    -1,   366,    -1,   395,    -1,   390,
      -1,   378,    -1,   337,    -1,   396,    -1,   397,    -1,   398,
      -1,   400,    -1,   343,    -1,   341,    -1,   389,    -1,   393,
      -1,   392,    -1,   376,    -1,   377,    -1,   379,    -1,   371,
      -1,   374,    -1,   232,    -1,   234,    -1,   236,    -1,   238,
      -1,   240,    -1,   242,    -1,   244,    -1,   246,    -1,   248,
      -1,   231,    -1,   233,    -1,   235,    -1,   237,    -1,   239,
      -1,   241,    -1,   243,    -1,   245,    -1,   247,    -1,   296,
      -1,   297,    -1,   225,    -1,   225,    -1,   297,    -1,   144,
     215,   296,    -1,   298,   273,   446,    -1,   298,   273,   203,
     215,   455,    -1,   298,   273,   445,    -1,   298,   273,   462,
     215,   439,    -1,   298,   273,   461,   215,   441,    -1,   298,
     273,   460,   215,   443,    -1,   298,   273,   434,    -1,   298,
     273,   437,    -1,   298,   273,   220,   307,   221,    -1,   298,
     273,   220,   306,   221,    -1,   298,   273,   220,   305,   221,
      -1,   298,   273,   462,   215,   220,   304,   221,    -1,   298,
     273,   461,   215,   220,   303,   221,    -1,   298,   273,   460,
     215,   220,   302,   221,    -1,   298,   273,   220,   308,   221,
      -1,   444,    -1,   302,   214,   444,    -1,   442,    -1,   303,
     214,   442,    -1,   440,    -1,   304,   214,   440,    -1,   438,
      -1,   305,   214,   438,    -1,   435,    -1,   306,   214,   435,
      -1,   446,    -1,   307,   214,   446,    -1,   445,    -1,   308,
     214,   445,    -1,    -1,    -1,   194,   224,   216,   310,   450,
     217,   311,   294,    -1,   224,   216,   453,   217,    -1,   143,
      -1,    -1,     6,    -1,     6,    -1,   218,   446,   196,   446,
     219,    -1,   218,   446,   219,    -1,    -1,    -1,   152,   317,
     218,   433,   219,   318,   220,   319,   221,    -1,    -1,   320,
     319,    -1,    -1,   153,   458,   215,   321,   293,    -1,    -1,
     155,   215,   322,   293,    -1,   154,    -1,   274,   274,    -1,
     151,    -1,   150,   299,   273,   446,   324,   446,    -1,    -1,
     325,   327,   294,    -1,    -1,   149,   218,   433,   219,   329,
     294,    -1,    -1,   315,   215,   331,   295,    -1,    -1,   130,
     315,   333,   334,    -1,   294,    -1,    -1,   294,   335,   131,
     294,    -1,    14,   215,   445,    -1,    71,    -1,    71,   215,
     458,    -1,    71,   215,    81,    -1,    59,   215,   339,   214,
     339,    -1,    59,   215,   339,   214,   458,    -1,   216,     4,
     214,    80,   214,   354,   217,    -1,   417,   314,   214,   427,
      -1,    -1,   120,   215,   340,   214,   347,   214,   349,   418,
     421,   423,   425,   342,   294,    -1,    -1,    51,   215,   340,
     214,   346,   214,   348,   214,   349,   418,   421,   423,   425,
     344,   294,    -1,    -1,   214,     5,    -1,   216,     4,   214,
       4,   217,    -1,    81,    -1,   216,     4,   214,     4,   217,
      -1,    81,    -1,   216,   358,   214,   359,   217,    -1,    81,
      -1,   216,     4,   214,     4,   217,    -1,    81,    -1,   129,
     215,   351,   214,   426,   214,   352,   214,   354,   355,    -1,
      27,   215,   426,   214,   455,   355,    -1,     5,    -1,    81,
      -1,   353,    -1,    81,    -1,    80,    -1,    80,   275,   353,
      -1,     4,    -1,    81,    -1,    -1,   355,   214,   356,    -1,
     135,   215,   136,    -1,    77,   215,   443,    -1,   118,   215,
       4,    -1,   161,    -1,   111,    -1,   112,    -1,   127,    -1,
      20,    -1,    19,   215,   358,   214,   359,   345,   223,    -1,
      19,   215,   434,   345,   223,    -1,    85,    -1,    86,    -1,
      87,    -1,    86,    -1,    25,   215,   362,    -1,    -1,    25,
     215,   362,   361,   294,    -1,   441,   214,   434,   363,    -1,
      -1,   363,   214,   364,    -1,   445,    -1,   124,    -1,   125,
      -1,   429,    -1,   128,   445,    -1,   162,    -1,   163,    -1,
     159,    -1,   166,    -1,   167,    -1,   168,    -1,   169,   215,
     433,    -1,   170,   215,   433,    -1,   174,   215,   433,    -1,
     175,    -1,   176,    -1,   177,   215,   365,    -1,   164,    -1,
     165,    -1,   171,    -1,   172,   215,   433,    -1,   173,    -1,
     222,    -1,   178,    -1,   222,   275,   365,    -1,    23,   215,
     368,    -1,    -1,    24,   215,   368,   367,   294,    -1,   443,
     369,    -1,    -1,   369,   214,   370,    -1,    74,    -1,   179,
     215,   441,    -1,   459,    -1,   121,   215,   445,    -1,   147,
     215,   433,    -1,   148,    -1,    73,    -1,   156,   215,   433,
      -1,   193,    -1,    72,    -1,   157,    -1,   158,   215,   433,
      -1,   159,    -1,   160,    -1,   434,    -1,   109,   215,   106,
      -1,   108,   215,   107,    -1,   110,   215,   433,    -1,   117,
     215,   433,    -1,   118,   215,   433,    -1,   145,   215,   146,
      -1,   119,   215,   433,    -1,    77,   215,   443,    -1,   161,
      -1,   112,    -1,    79,    -1,    75,    -1,   115,   215,   113,
      -1,   116,   215,   114,    -1,    58,   215,   433,    -1,    26,
     215,   416,   214,   434,    -1,    28,   215,   434,   214,    80,
     214,   426,    -1,    33,   215,   434,   214,    80,    -1,    33,
     215,   434,   214,    80,   214,     5,   289,    -1,    34,    -1,
      34,   215,   455,    -1,    34,   215,   178,    -1,    47,   215,
     434,   214,    90,    -1,    48,   215,   434,   214,    90,    -1,
      48,   215,   464,   214,   464,   214,    90,    -1,    53,   215,
     464,   214,   464,   214,   222,    -1,    54,   215,   464,   214,
     464,   381,    -1,    55,   215,   464,   214,   464,    -1,    -1,
     214,    90,    -1,    62,   215,   455,   214,    94,    -1,    63,
     215,   455,    -1,    64,   215,   434,   214,   186,   215,   434,
     214,    92,   214,    91,    -1,    64,   215,   434,   214,    65,
     214,    66,   214,    67,   214,    92,   214,    91,    -1,    78,
     215,   434,   214,   183,   386,    -1,    -1,   386,   214,   387,
      -1,    91,    -1,   179,   215,   441,    -1,   180,   215,   443,
      -1,   181,   215,   391,    -1,   182,   215,   391,    -1,   184,
      -1,   185,    -1,   186,   215,   434,    -1,   187,   215,   416,
      -1,   135,   215,   388,    -1,    98,   215,   424,    -1,   188,
     215,     4,    -1,   118,   215,     4,    -1,     4,    -1,    92,
      -1,   136,    -1,    94,    -1,    69,   215,   455,    -1,    68,
     215,   455,    -1,     3,    -1,   216,     3,   214,   427,   217,
      -1,   134,   215,   437,   214,   439,   214,   439,   214,     7,
      -1,   132,   215,   455,   214,   439,    -1,   132,   215,   455,
     214,   439,   214,    97,    -1,    49,   215,   437,    -1,    50,
     215,   437,    -1,    39,   215,   222,   214,    40,    -1,    39,
     215,   222,   214,    40,   214,   179,   215,   441,    -1,    36,
     215,   437,   214,    38,    -1,    37,   215,   464,   214,    38,
      -1,    22,   215,   455,   214,    96,    -1,    -1,    35,   215,
     437,   214,   427,   214,   417,   418,   421,   423,   425,   401,
     402,    -1,    -1,   294,    -1,    44,   215,   434,   214,   428,
     214,   430,    -1,    44,   215,   434,   214,   428,   214,   430,
     214,   179,   215,   441,    -1,    44,   215,   434,   214,   428,
     214,   430,   214,    89,    -1,    44,   215,   434,   214,   428,
     214,   430,   214,    89,   214,   179,   215,   441,    -1,    45,
     215,   434,    -1,    45,   215,   434,   214,   179,   215,   441,
      -1,   102,   215,   434,   214,   103,   214,     4,   214,   104,
     214,   105,    -1,   102,   215,   434,   214,   103,   214,     4,
     214,   104,   214,   105,   214,   427,    -1,   102,   215,   434,
     214,   103,   214,   104,    -1,    99,   215,   455,   214,   100,
     214,    97,    -1,   126,   215,   455,   214,     4,    -1,   126,
     215,   455,   214,   100,   214,    97,    -1,    46,   215,    93,
     214,   434,    -1,    46,   215,    93,   214,   434,   214,   179,
     215,   441,    -1,   189,   215,   434,   214,   445,    -1,   189,
     215,   434,   214,    81,    -1,   189,   215,   434,    -1,   190,
     215,   434,   214,   137,   214,   427,    -1,   190,   215,   434,
      -1,   191,   215,   434,   214,   138,   214,   445,    -1,   191,
     215,   434,   214,   138,   214,    81,    -1,   191,   215,   434,
      -1,   192,   215,   434,   214,   139,    -1,   192,   215,   434,
     214,   141,    -1,   192,   215,   434,    -1,    60,   215,   446,
     214,   434,    -1,    61,   215,   434,   214,   463,   214,   445,
      -1,    57,   215,   433,   214,   433,   214,   433,   214,   433,
      -1,    57,    -1,   222,    -1,    81,    -1,   222,    -1,    81,
      -1,    -1,   214,   419,    -1,   420,    -1,   420,   214,   419,
      -1,    41,    -1,    42,    -1,    43,    -1,    -1,   101,   215,
     422,    -1,   100,    -1,    -1,    98,   215,   424,    -1,    97,
      -1,    -1,   179,   215,   441,    -1,    72,    -1,    81,    -1,
      73,    -1,    81,    -1,    84,    -1,   431,    -1,    81,    -1,
      84,    -1,   431,    -1,    83,   215,    81,    -1,    88,    -1,
      81,    -1,    83,   218,     4,   219,    -1,   222,    -1,   233,
      -1,   234,   218,   446,   219,    -1,   446,    -1,   435,    -1,
     205,   216,   455,   217,    -1,   237,    -1,   238,   218,   446,
     219,    -1,   216,     4,   214,     4,   217,    -1,    81,    -1,
      82,   436,   219,    -1,   213,    -1,   213,   214,   436,    -1,
     438,    -1,   239,    -1,   240,   218,   446,   219,    -1,   216,
       4,   214,     4,   214,     4,   214,     4,   217,    -1,   440,
      -1,   241,    -1,   242,   218,   446,   219,    -1,     3,    -1,
     216,     3,   214,   427,   217,    -1,   442,    -1,   243,    -1,
     244,   218,   446,   219,    -1,   222,    -1,     3,    -1,   216,
       3,   214,   222,   217,    -1,    81,    -1,   444,    -1,   245,
      -1,   246,   218,   446,   219,    -1,   222,    -1,     3,    -1,
     216,     3,   214,   222,   217,    -1,    81,    -1,    95,    -1,
     432,    -1,   445,   274,   432,    -1,     4,    -1,   456,    -1,
     216,     8,   217,    -1,   231,    -1,   232,   218,   446,   219,
      -1,   446,   268,   446,    -1,   446,   269,   446,    -1,   446,
     270,   446,    -1,   446,   271,   446,    -1,   446,   272,   446,
      -1,   216,   446,   217,    -1,   227,    -1,   228,    -1,   298,
     215,   447,    -1,   448,    -1,   449,   214,   448,    -1,    -1,
     449,    -1,   446,    -1,   445,    -1,   451,    -1,   452,   214,
     451,    -1,    -1,   452,    -1,   434,    -1,   198,   437,    -1,
     199,   437,    -1,   200,   434,   214,   434,    -1,   201,   434,
     214,   434,   214,   446,    -1,   202,   216,   455,   217,    -1,
     202,   216,   353,   214,   455,   217,    -1,   208,   216,     7,
     214,   455,   217,    -1,   208,   216,   455,   214,   455,   217,
      -1,   208,   216,   439,   214,   455,   217,    -1,   204,   434,
      -1,   206,   216,   434,   214,   446,   217,    -1,   206,   216,
     434,   214,   446,   214,    41,   217,    -1,   207,   216,   434,
     214,   446,   214,   446,   217,    -1,   207,   216,   434,   214,
     446,   214,   446,   214,    41,   217,    -1,   210,   216,   211,
     214,   216,   446,   269,   446,   287,   217,   214,   434,   288,
     217,    -1,   209,   454,    -1,   247,    -1,   216,   455,   217,
      -1,   454,    -1,   454,   276,   455,    -1,   267,    -1,    96,
      -1,     4,    -1,     8,    -1,     9,    -1,     4,    -1,     8,
      -1,     9,    -1,     4,    -1,   456,    -1,    30,    -1,    23,
      -1,    31,    -1,    25,    -1,    32,    -1,   132,    -1,    76,
      -1,    81,    -1,   465,    -1,    56,   216,     4,   214,     4,
     214,     4,   214,     4,   217,    -1,   216,     4,   214,     4,
     214,     4,   214,     4,   217,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   287,   287,   288,   291,   292,   295,   318,   323,   345,
     358,   370,   376,   405,   411,   415,   421,   427,   434,   437,
     444,   448,   455,   458,   465,   466,   470,   473,   480,   484,
     491,   494,   500,   506,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   516,   517,   518,   519,   520,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   582,   583,   584,   585,   586,
     587,   588,   589,   590,   593,   594,   595,   598,   599,   602,
     618,   624,   630,   636,   642,   648,   654,   660,   666,   676,
     686,   696,   706,   716,   726,   738,   743,   750,   755,   762,
     767,   774,   778,   784,   789,   796,   800,   806,   810,   817,
     839,   816,   853,   908,   915,   918,   924,   931,   935,   944,
     948,   943,  1011,  1012,  1016,  1015,  1029,  1028,  1043,  1053,
    1054,  1057,  1095,  1094,  1129,  1128,  1159,  1158,  1191,  1190,
    1216,  1227,  1226,  1254,  1260,  1265,  1270,  1277,  1284,  1293,
    1301,  1313,  1312,  1336,  1335,  1359,  1362,  1368,  1378,  1384,
    1393,  1399,  1404,  1410,  1415,  1421,  1432,  1438,  1439,  1442,
    1443,  1446,  1450,  1456,  1457,  1461,  1467,  1475,  1480,  1485,
    1490,  1495,  1500,  1505,  1513,  1520,  1528,  1536,  1537,  1540,
    1541,  1544,  1549,  1548,  1562,  1569,  1576,  1584,  1589,  1595,
    1601,  1607,  1613,  1618,  1623,  1628,  1633,  1638,  1643,  1648,
    1653,  1658,  1663,  1668,  1674,  1679,  1684,  1689,  1694,  1701,
    1710,  1714,  1727,  1736,  1735,  1753,  1763,  1769,  1777,  1783,
    1788,  1793,  1798,  1803,  1808,  1813,  1818,  1823,  1837,  1843,
    1848,  1853,  1858,  1863,  1868,  1873,  1878,  1883,  1888,  1893,
    1898,  1903,  1908,  1913,  1918,  1924,  1929,  1934,  1941,  1947,
    1976,  1981,  1989,  1995,  1999,  2007,  2014,  2021,  2031,  2041,
    2057,  2068,  2071,  2077,  2083,  2089,  2093,  2099,  2106,  2112,
    2120,  2126,  2131,  2136,  2141,  2146,  2152,  2158,  2163,  2168,
    2173,  2178,  2183,  2190,  2190,  2190,  2190,  2193,  2199,  2205,
    2210,  2217,  2224,  2228,  2234,  2240,  2246,  2251,  2258,  2264,
    2274,  2281,  2280,  2312,  2315,  2321,  2326,  2331,  2336,  2342,
    2346,  2352,  2356,  2360,  2366,  2372,  2376,  2382,  2386,  2392,
    2396,  2401,  2408,  2412,  2419,  2423,  2428,  2435,  2439,  2444,
    2452,  2458,  2465,  2469,  2476,  2484,  2487,  2497,  2501,  2504,
    2510,  2514,  2521,  2525,  2529,  2536,  2539,  2545,  2552,  2555,
    2561,  2568,  2572,  2579,  2580,  2583,  2584,  2587,  2588,  2589,
    2595,  2596,  2597,  2603,  2604,  2607,  2616,  2621,  2628,  2639,
    2645,  2649,  2653,  2660,  2670,  2677,  2681,  2687,  2691,  2699,
    2703,  2710,  2720,  2733,  2737,  2744,  2754,  2763,  2774,  2778,
    2785,  2795,  2806,  2815,  2825,  2831,  2835,  2842,  2852,  2863,
    2872,  2882,  2886,  2893,  2894,  2900,  2904,  2908,  2912,  2920,
    2929,  2933,  2937,  2941,  2945,  2949,  2952,  2959,  2968,  3001,
    3002,  3005,  3006,  3009,  3013,  3020,  3027,  3038,  3041,  3049,
    3053,  3057,  3061,  3065,  3070,  3074,  3078,  3083,  3088,  3093,
    3097,  3102,  3107,  3111,  3115,  3120,  3124,  3131,  3137,  3141,
    3147,  3154,  3155,  3158,  3159,  3160,  3163,  3167,  3171,  3175,
    3181,  3182,  3185,  3186,  3189,  3190,  3193,  3194,  3197,  3201,
    3227
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
  "MONSTER_ID", "TRAP_ID", "DOOR_ID", "DRAWBRIDGE_ID",
  "MONSTER_GENERATION_ID", "object_ID", "monster_ID", "terrain_ID",
  "MAZEWALK_ID", "WALLIFY_ID", "REGION_ID", "SPECIAL_REGION_ID",
  "SPECIAL_LEVREGION_ID", "SPECIAL_REGION_TYPE", "NAMING_ID",
  "NAMING_TYPE", "FILLING", "IRREGULAR", "JOINED", "ALTAR_ID", "ANVIL_ID",
  "NPC_ID", "LADDER_ID", "STAIR_ID", "NON_DIGGABLE_ID", "NON_PASSWALL_ID",
  "ROOM_ID", "ARTIFACT_NAME_ID", "PORTAL_ID", "TELEPRT_ID", "BRANCH_ID",
  "LEV", "MINERALIZE_ID", "AGE_ID", "CORRIDOR_ID", "GOLD_ID",
  "ENGRAVING_ID", "FOUNTAIN_ID", "THRONE_ID", "MODRON_PORTAL_ID",
  "LEVEL_TELEPORTER_ID", "LEVEL_TELEPORT_DIRECTION_TYPE",
  "LEVEL_TELEPORT_END_TYPE", "POOL_ID", "SINK_ID", "NONE",
  "RAND_CORRIDOR_ID", "DOOR_STATE", "LIGHT_STATE", "CURSE_TYPE",
  "MYTHIC_TYPE", "ENGRAVING_TYPE", "KEYTYPE_ID", "LEVER_ID",
  "NO_PICKUP_ID", "DIRECTION", "RANDOM_TYPE", "RANDOM_TYPE_BRACKET",
  "A_REGISTER", "ALIGNMENT", "LEFT_OR_RIGHT", "CENTER", "TOP_OR_BOT",
  "ALTAR_TYPE", "ALTAR_SUBTYPE", "UP_OR_DOWN", "ACTIVE_OR_INACTIVE",
  "MODRON_PORTAL_TYPE", "NPC_TYPE", "FOUNTAIN_TYPE", "SPECIAL_OBJECT_TYPE",
  "CMAP_TYPE", "FLOOR_SUBTYPE", "FLOOR_SUBTYPE_ID", "FLOOR_ID",
  "FLOOR_TYPE", "FLOOR_TYPE_ID", "DECORATION_ID", "DECORATION_TYPE",
  "DECORATION_DIR", "DECORATION_ITEM_STATE", "ELEMENTAL_ENCHANTMENT_TYPE",
  "EXCEPTIONALITY_TYPE", "EXCEPTIONALITY_ID", "ELEMENTAL_ENCHANTMENT_ID",
  "ENCHANTMENT_ID", "SECRET_DOOR_ID", "USES_UP_KEY_ID",
  "MYTHIC_PREFIX_TYPE", "MYTHIC_SUFFIX_TYPE", "MYTHIC_PREFIX_ID",
  "MYTHIC_SUFFIX_ID", "CHARGES_ID", "SPECIAL_QUALITY_ID", "SPEFLAGS_ID",
  "SUBROOM_ID", "NAME_ID", "FLAGS_ID", "FLAG_TYPE", "MON_ATTITUDE",
  "MON_ALERTNESS", "SUBTYPE_ID", "NON_PASSDOOR_ID", "MON_APPEARANCE",
  "ROOMDOOR_ID", "IF_ID", "ELSE_ID", "TERRAIN_ID", "HORIZ_OR_VERT",
  "REPLACE_TERRAIN_ID", "LOCATION_SUBTYPE_ID", "DOOR_SUBTYPE",
  "BRAZIER_SUBTYPE", "SIGNPOST_SUBTYPE", "TREE_SUBTYPE", "FOREST_ID",
  "FOREST_TYPE", "INITIALIZE_TYPE", "EXIT_ID", "SHUFFLE_ID",
  "MANUAL_TYPE_ID", "MANUAL_TYPE", "QUANTITY_ID", "BURIED_ID", "LOOP_ID",
  "FOR_ID", "TO_ID", "SWITCH_ID", "CASE_ID", "BREAK_ID", "DEFAULT_ID",
  "ERODED_ID", "TRAPPED_STATE", "RECHARGED_ID", "INVIS_ID", "GREASED_ID",
  "INDESTRUCTIBLE_ID", "FEMALE_ID", "MALE_ID", "WAITFORU_ID",
  "PROTECTOR_ID", "CANCELLED_ID", "REVIVED_ID", "AVENGE_ID", "FLEEING_ID",
  "BLINDED_ID", "MAXHP_ID", "LEVEL_ADJUSTMENT_ID",
  "KEEP_ORIGINAL_INVENTORY_ID", "PARALYZED_ID", "STUNNED_ID",
  "CONFUSED_ID", "SEENTRAPS_ID", "ALL_ID", "MONTYPE_ID", "OBJTYPE_ID",
  "TERTYPE_ID", "TERTYPE2_ID", "LEVER_EFFECT_TYPE", "SWITCHABLE_ID",
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
  "level_def", "lev_init", "tileset_detail", "forest_detail",
  "monster_generation_detail", "boundary_type_detail", "opt_limited",
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
  "altar_detail", "anvil_detail", "decoration_detail", "floor_detail",
  "subtype_detail", "npc_detail", "grave_detail", "brazier_detail",
  "signpost_detail", "tree_detail", "gold_detail", "engraving_detail",
  "mineralize", "trap_name", "room_type", "optroomregionflags",
  "roomregionflags", "roomregionflag", "optfloortype", "floortype",
  "optfloorsubtype", "floorsubtype", "optmontype", "door_state",
  "light_state", "alignment", "alignment_prfx", "altar_type", "a_register",
  "string_or_var", "integer_or_var", "coord_or_var", "encodecoord",
  "humidity_flags", "region_or_var", "encoderegion", "mapchar_or_var",
  "mapchar", "monster_or_var", "encodemonster", "object_or_var",
  "encodeobj", "string_expr", "math_expr_var", "func_param_type",
  "func_param_part", "func_param_list", "func_params_list",
  "func_call_param_part", "func_call_param_list", "func_call_params_list",
  "ter_selection_x", "ter_selection", "dice", "tileset_number",
  "all_integers", "all_ints_push", "objectid", "monsterid", "terrainid",
  "engraving_type", "lev_region", "region", 0
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
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,    44,    58,    40,    41,    91,    93,
     123,   125,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,    43,    45,
      42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   277,   278,   278,   279,   279,   280,   281,   281,   282,
     282,   282,   282,   283,   284,   284,   285,   286,   287,   287,
     288,   288,   289,   289,   290,   290,   291,   291,   292,   292,
     293,   293,   294,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   298,   298,   298,   299,   299,   300,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   302,   302,   303,   303,   304,
     304,   305,   305,   306,   306,   307,   307,   308,   308,   310,
     311,   309,   312,   313,   314,   314,   315,   315,   315,   317,
     318,   316,   319,   319,   321,   320,   322,   320,   323,   324,
     324,   325,   327,   326,   329,   328,   331,   330,   333,   332,
     334,   335,   334,   336,   337,   337,   337,   338,   338,   339,
     340,   342,   341,   344,   343,   345,   345,   346,   346,   347,
     347,   348,   348,   349,   349,   350,   350,   351,   351,   352,
     352,   353,   353,   354,   354,   355,   355,   356,   356,   356,
     356,   356,   356,   356,   357,   357,   357,   358,   358,   359,
     359,   360,   361,   360,   362,   363,   363,   364,   364,   364,
     364,   364,   364,   364,   364,   364,   364,   364,   364,   364,
     364,   364,   364,   364,   364,   364,   364,   364,   364,   365,
     365,   365,   366,   367,   366,   368,   369,   369,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   371,   372,
     373,   373,   374,   374,   374,   375,   376,   377,   378,   379,
     380,   381,   381,   382,   383,   384,   384,   385,   386,   386,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   388,   388,   388,   388,   389,   390,   391,
     391,   392,   393,   393,   394,   395,   396,   396,   397,   398,
     399,   401,   400,   402,   402,   403,   403,   403,   403,   404,
     404,   405,   405,   405,   406,   407,   407,   408,   408,   409,
     409,   409,   410,   410,   411,   411,   411,   412,   412,   412,
     413,   414,   415,   415,   416,   416,   417,   417,   418,   418,
     419,   419,   420,   420,   420,   421,   421,   422,   423,   423,
     424,   425,   425,   426,   426,   427,   427,   428,   428,   428,
     429,   429,   429,   430,   430,   431,   432,   432,   432,   433,
     434,   434,   434,   434,   435,   435,   435,   436,   436,   437,
     437,   437,   438,   439,   439,   439,   440,   440,   441,   441,
     441,   442,   442,   442,   442,   443,   443,   443,   444,   444,
     444,   444,   444,   445,   445,   446,   446,   446,   446,   446,
     446,   446,   446,   446,   446,   446,   447,   447,   448,   449,
     449,   450,   450,   451,   451,   452,   452,   453,   453,   454,
     454,   454,   454,   454,   454,   454,   454,   454,   454,   454,
     454,   454,   454,   454,   454,   454,   454,   454,   455,   455,
     456,   457,   457,   458,   458,   458,   459,   459,   459,   459,
     460,   460,   461,   461,   462,   462,   463,   463,   464,   464,
     465
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     3,     3,     5,     5,
       5,     3,    16,     3,     5,     3,     5,     3,     0,     2,
       0,     2,     0,     2,     1,     1,     0,     3,     3,     1,
       0,     2,     3,     1,     1,     1,     1,     1,     1,     1,
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
       4,     0,    13,     0,    15,     0,     2,     5,     1,     5,
       1,     5,     1,     5,     1,    10,     6,     1,     1,     1,
       1,     1,     3,     1,     1,     0,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     7,     5,     1,     1,     1,
       1,     3,     0,     5,     4,     0,     3,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     1,     1,     3,     1,     1,     1,     3,     1,     1,
       1,     3,     3,     0,     5,     2,     0,     3,     1,     3,
       1,     3,     3,     1,     1,     3,     1,     1,     1,     3,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     3,     3,     3,     5,     7,
       5,     8,     1,     3,     3,     5,     5,     7,     7,     6,
       5,     0,     2,     5,     3,    11,    13,     6,     0,     3,
       1,     3,     3,     3,     3,     1,     1,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     3,     3,     1,
       5,     9,     5,     7,     3,     3,     5,     9,     5,     5,
       5,     0,    13,     0,     1,     7,    11,     9,    13,     3,
       7,    11,    13,     7,     7,     5,     7,     5,     9,     5,
       5,     3,     7,     3,     7,     7,     3,     5,     5,     3,
       5,     7,     9,     1,     1,     1,     1,     1,     0,     2,
       1,     3,     1,     1,     1,     0,     3,     1,     0,     3,
       1,     0,     3,     1,     1,     1,     1,     1,     1,     1,
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
       2,     0,     0,     0,     3,     4,    26,     0,     0,     1,
       5,     0,    30,     0,     7,     0,   156,     0,     0,     0,
       0,   224,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   302,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   383,     0,
       0,     0,     0,     0,     0,     0,     0,   184,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   153,     0,
       0,     0,   159,   168,     0,     0,     0,     0,     0,     0,
       0,   116,   105,    96,   106,    97,   107,    98,   108,    99,
     109,   100,   110,   101,   111,   102,   112,   103,   113,   104,
      34,    35,    38,    40,    37,     6,    30,   114,   115,     0,
      53,    52,    70,    71,    68,     0,    63,    69,   172,    64,
      65,    67,    66,    33,    81,    51,    87,    86,    55,    74,
      76,    77,    94,    56,    75,    95,    72,    91,    92,    80,
      93,    50,    59,    60,    61,    73,    88,    79,    90,    89,
      54,    78,    82,    83,    84,    36,    85,    43,    44,    42,
      41,    39,    45,    46,    47,    48,    49,    62,    57,    58,
       0,    29,    27,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   178,
       0,     0,     0,     0,     0,   117,   118,     0,     0,     0,
       0,     0,     0,     0,   455,     0,   458,     0,   500,     0,
     456,   477,    31,     0,   176,     0,     8,     0,   416,   417,
       0,   453,   183,     0,     0,     0,    11,   502,   501,    13,
     425,     0,   227,   228,     0,     0,   422,     0,     0,   195,
     420,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   496,   479,   498,     0,   449,   451,
     452,     0,   448,   446,     0,   262,   266,   445,   263,   442,
     444,     0,   441,   439,     0,   231,     0,   438,   385,   384,
       0,   403,   404,     0,     0,     0,     0,   304,   303,     0,
     430,     0,     0,   429,     0,     0,     0,     0,   518,     0,
       0,   359,     0,     0,     0,     0,     0,   344,   345,   387,
     386,     0,   154,     0,     0,     0,     0,   419,     0,     0,
       0,     0,     0,   314,     0,   338,   337,   505,   503,   504,
     186,   185,     0,     0,     0,     0,     0,   207,   208,     0,
       0,     0,     0,    15,   119,     0,     0,     0,   371,   373,
     376,   379,   149,     0,     0,     0,     0,   158,     0,     0,
       0,     0,     0,   474,   473,   475,   478,     0,   511,   513,
     510,   512,   514,   515,     0,     0,     0,   126,   127,   122,
     120,     0,     0,     0,     0,    30,   173,    28,     0,     0,
       0,     0,     0,   427,     0,     0,     0,     0,     0,     0,
       0,   480,   481,     0,     0,     0,   489,     0,     0,     0,
     495,     0,     0,     0,     0,     0,     0,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   155,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   180,   179,     0,     0,     0,   174,     0,     0,
       0,     0,     0,     0,   471,   457,   465,     0,     0,   460,
     461,   462,   463,   464,     0,   152,     0,   455,     0,     0,
       0,     0,   143,   141,   147,   145,     0,     0,     0,   177,
       0,     0,   454,    10,   339,     0,     9,     0,     0,   426,
       0,     0,     0,   230,   229,   195,   196,   226,     0,     0,
     211,     0,     0,     0,     0,   436,     0,     0,   434,     0,
       0,   433,     0,     0,   497,   499,   350,     0,     0,     0,
     264,     0,     0,   233,   235,   298,   215,     0,    16,   137,
     300,     0,     0,   405,   406,     0,   348,     0,     0,   349,
     346,   409,     0,   407,     0,   408,     0,   367,   305,     0,
     306,     0,   198,     0,     0,     0,     0,   311,   310,     0,
       0,   187,   188,   380,   516,   517,     0,   313,     0,     0,
     318,     0,     0,   200,     0,     0,   365,     0,     0,     0,
       0,   342,     0,    14,     0,   170,     0,     0,   160,   370,
     369,     0,     0,   377,   378,     0,   469,   472,     0,   459,
     157,   476,   121,     0,     0,   130,     0,   129,     0,   128,
       0,   134,     0,   125,     0,   124,     0,   123,    32,   418,
       0,     0,   428,   421,     0,   423,     0,   482,     0,     0,
       0,   484,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   447,   508,   506,   507,     0,   277,   274,   268,   294,
       0,   293,     0,     0,     0,   292,     0,     0,     0,     0,
       0,     0,     0,     0,   273,     0,   278,     0,   280,   281,
     291,     0,   276,   267,   282,   509,   270,     0,   440,   234,
     206,     0,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   190,     0,
       0,   309,     0,     0,     0,     0,     0,   317,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   175,   169,   171,
       0,     0,     0,     0,     0,   150,     0,   142,   144,   146,
     148,     0,   135,     0,     0,   139,     0,     0,   424,   225,
       0,   212,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   450,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   443,     0,
       0,   299,   138,    22,     0,   388,     0,     0,     0,     0,
     414,   413,   355,   360,     0,   307,     0,   202,     0,     0,
     308,   312,     0,     0,   381,     0,     0,     0,   364,     0,
     363,     0,   204,     0,   388,   366,   210,     0,   209,   182,
     343,     0,   162,   372,   375,   374,   466,   467,   468,   470,
       0,     0,   133,   132,     0,   131,     0,     0,   483,   485,
       0,   490,     0,   486,     0,   435,   488,   487,     0,   297,
     290,   284,   283,   285,   295,   296,   286,   287,   289,   271,
     288,   272,   275,   279,   269,     0,   410,   238,   239,     0,
     244,   242,   243,   254,   255,   245,   246,   247,     0,     0,
     256,     0,   258,     0,   251,   252,     0,   236,   240,   411,
     237,     0,   221,   222,     0,   223,     0,   220,   216,     0,
     301,     0,     0,   395,     0,     0,     0,   415,     0,     0,
       0,     0,     0,     0,   213,   214,     0,     0,     0,   320,
       0,     0,     0,     0,     0,     0,     0,   325,   326,     0,
       0,     0,   319,     0,     0,     0,   395,     0,     0,     0,
       0,     0,   162,   151,   136,   140,   340,     0,     0,     0,
     437,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,    23,     0,   392,   393,   394,   389,   390,     0,
     398,     0,     0,   347,   357,     0,   368,   197,     0,   388,
     382,   189,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   199,     0,   398,   215,   341,
       0,   166,   161,   163,     0,   491,     0,   492,   461,   412,
     248,   249,   257,   250,   260,   259,   253,   218,   219,   217,
       0,     0,     0,     0,   401,     0,     0,     0,     0,     0,
     395,     0,     0,   400,   330,   332,   333,   334,   336,   335,
     329,   321,   322,   323,   324,   327,   328,   331,     0,     0,
     401,   205,   164,    30,     0,     0,     0,     0,     0,   432,
     391,   397,   396,     0,     0,   351,     0,   520,     0,   356,
     201,   398,     0,   315,   361,   203,   191,    30,   167,     0,
     493,    19,     0,   261,   399,     0,   353,   519,     0,   401,
       0,     0,     0,   165,     0,     0,   402,   354,   352,   358,
     193,   316,   362,   192,     0,    20,     0,    24,    25,    22,
       0,     0,   194,    12,    21,   494
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,   100,   101,   102,   103,   104,
    1097,  1151,   940,  1149,    12,   172,   105,   416,   106,   107,
     108,   109,   227,   110,   111,   781,   578,   784,   518,   519,
     520,   521,   112,   504,   870,   113,   114,   475,   115,   116,
     228,   770,   981,   982,  1117,  1093,   117,   637,   118,   119,
     245,   120,   634,   121,   414,   122,   370,   493,   629,   123,
     124,   125,   349,   341,   126,  1132,   127,  1146,   430,   604,
     625,   839,   854,   128,   369,   857,   551,   956,   730,   938,
     129,   268,   545,   130,   451,   305,   729,   927,  1056,   131,
     448,   295,   447,   723,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   751,   142,   143,   144,   145,   757,
     972,  1080,   146,   147,   536,   148,   149,   150,   151,   152,
     153,   154,   155,   156,  1126,  1138,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   310,
     342,   943,  1007,  1008,  1010,  1102,  1064,  1074,  1105,   313,
     585,   594,   928,   832,   595,   251,   346,   285,   270,   424,
     322,   323,   560,   561,   306,   307,   296,   297,   393,   347,
     868,   646,   647,   648,   395,   396,   397,   286,   442,   240,
     259,   361,   726,   411,   412,   413,   616,   327,   328
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1006
static const yytype_int16 yypact[] =
{
     324,  -127,  -112,   125, -1006,   324,    24,   -65,   -55, -1006,
   -1006,     7,   851,   -41, -1006,    59, -1006,     9,    30,    54,
      77, -1006,   114,   133,   145,   154,   180,   192,   197,   214,
     232,   242,   248,   267,   269,   273,   279,   280,   285,   286,
     288,   290,   294,   296,   297,   298,   300,   301,   303,   304,
     310,   311,   313,   315,   316,   317,   319,   320,   321,   322,
     330,   331,   332,   333,    58,   340,   342,   343, -1006,   344,
     -13,   604, -1006, -1006,   348,   349,   354,   360,   -17,    39,
      -6, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006,   851, -1006, -1006,   -46,
   -1006, -1006, -1006, -1006, -1006,   369, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
     287,    74, -1006,  -117,   462,    62,   328,   196,   938,    34,
      34,    78,   -33,    29,    -8,   376,    -8,   925,  -121,  -121,
     -14,   177,    -8,    -8,   312,    -8,   -12,  -121,  -121,   -19,
     -14,   -14,   -14,    39,   187,    39,    -8,   938,   938,    -8,
     938,   938,   100,    -8,   938,    -8,   -19,   938,    71, -1006,
     938,  -121,   350,   498,    39, -1006, -1006,   233,   352,    -8,
      -8,    -8,    -8,   371, -1006,    28, -1006,   372, -1006,   166,
   -1006,    20, -1006,   186, -1006,   373, -1006,    59, -1006, -1006,
     387, -1006,   338,   375,   380,   400, -1006, -1006, -1006, -1006,
   -1006,   402, -1006, -1006,   403,   612, -1006,   404,   414,   418,
   -1006, -1006,  -121,  -121,    -8,    -8,   407,    -8,   419,   425,
     426,   938,   427,   260, -1006, -1006,   374,   443, -1006, -1006,
   -1006,   615, -1006, -1006,   440, -1006, -1006, -1006, -1006, -1006,
   -1006,   656, -1006, -1006,   447,   446,   458, -1006, -1006, -1006,
     460, -1006, -1006,   461,   464,   475,   487, -1006, -1006,   700,
   -1006,   488,   493, -1006,   500,   489,   712,   508, -1006,   512,
     515,   521,   523,   529,   761,   553,   555, -1006, -1006, -1006,
   -1006,   556,   760,   557,   558,   559,   560,   271,   771,   562,
     377,   563,   565, -1006,   570, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006,   571,   572,   573,   574,   577, -1006, -1006,   578,
     373,   584,   592,   594, -1006,   590,    39,    39,   596,   601,
     602,   603, -1006,   605,    95,    39,    39, -1006,    39,    39,
      39,    39,    39,   338,   271, -1006,   606,   607, -1006, -1006,
   -1006, -1006, -1006, -1006,   608,    41,    15, -1006, -1006,   338,
     271,   610,   611,   613,   851,   851, -1006, -1006,    39,  -117,
     815,    19,   818,   616,   617,   938,   620,    39,   291,   822,
     621, -1006, -1006,   624,   626,   732, -1006,    -8,    -8,   486,
   -1006,   635,   614,   938,   752,   636,    39,   638,   373,   639,
      39,   373,    -8,    -8,   938,   774,    12,   775,   644,    39,
      40,   821,   856,   647,   824,   823,   270,   685,    -8,   776,
     653,   791,   -14,   -28, -1006,   668,   -14,   -14,   -14,    39,
     669,    42,    -8,    47,   795,    -7,   708,   792,   800,   -20,
      55,    29,   762, -1006,    86,    86,   765, -1006,   174,   675,
     -42,   772,   779,   -27,   790, -1006, -1006,  -137,    65,   161,
     161, -1006, -1006, -1006,    20, -1006,   938,   704,  -123,    18,
     165,   167, -1006, -1006,   338,   271,   104,   139,   103, -1006,
     702,   209, -1006, -1006, -1006,   918, -1006,   710,   402, -1006,
     709,   921,   252, -1006, -1006,   418, -1006, -1006,    -8,    -8,
     652,   714,   718,   729,   730, -1006,   731,   595, -1006,   728,
     733, -1006,   735,   737, -1006, -1006, -1006,   734,   384,   552,
   -1006,   736,   411, -1006, -1006, -1006, -1006,   738,   740, -1006,
     741,   953,   479, -1006, -1006,   745, -1006,   747,   958, -1006,
     749, -1006,   746, -1006,   751, -1006,   753,   758, -1006,   962,
   -1006,   759, -1006,   963,   764,    40,   768,   770, -1006,   778,
     894, -1006, -1006, -1006, -1006, -1006,   782, -1006,   783,   773,
   -1006,   784,   785, -1006,   972,   788, -1006,   794,   796,   855,
     986,   798,   799, -1006,   373, -1006,   716,    39, -1006, -1006,
     338,   802,   803, -1006, -1006,   789, -1006,   804,   797, -1006,
   -1006, -1006, -1006,   989,   793, -1006,   -10, -1006,    39, -1006,
    -117, -1006,    75, -1006,    12, -1006,    27, -1006, -1006, -1006,
     825,  1041, -1006, -1006,   829, -1006,   826, -1006,   833,   968,
     938, -1006,    39,    39,   938,   836,    39,   938,   938,   835,
     837, -1006, -1006, -1006, -1006,   838, -1006, -1006, -1006, -1006,
     840, -1006,   841,   842,   843, -1006,   844,   845,   846,   847,
     848,   849,   850,   852, -1006,   853, -1006,   857, -1006, -1006,
   -1006,   858, -1006, -1006, -1006, -1006, -1006,   854, -1006,   860,
     863,    29,    12,  1047,   864, -1006,   -19,  1062,   865,   891,
    1076,   152,    78,   902,   -78,  1010,   887,   -18, -1006,   880,
    1014, -1006,    39,   892,  -117,  1039,    -8,   893,  1013,    61,
     897,    -2,  1015,   276,   373,  1016,    86, -1006, -1006,   271,
     895,    40,   -21,   188,   790, -1006,   -69, -1006, -1006,   271,
     338,   176, -1006,   179,   190, -1006,    40,   900, -1006, -1006,
      39, -1006,   899,   282,   449,   901,    40,   484,   903,   904,
      39, -1006,    39,    34,  1012,  1011,    39,  1009,  1037,    39,
      39,    39,  -117,   982,    39,    39,    39,    78, -1006,  1025,
      60, -1006, -1006,   941,  1148,   942,   943,  1154,   944,   945,
   -1006, -1006,   946, -1006,   950, -1006,  1157, -1006,   355,   952,
   -1006, -1006,   954,    73,   338,   955,   956,   392, -1006,   957,
   -1006,  1163, -1006,  1169,   942, -1006, -1006,   960, -1006, -1006,
   -1006,   964,    -4, -1006, -1006,   338, -1006, -1006, -1006, -1006,
     373,    75, -1006, -1006,    27, -1006,   965,  1172,   271, -1006,
    1138, -1006,    39, -1006,   966, -1006, -1006, -1006,   491, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,   338,
   -1006, -1006, -1006, -1006, -1006,   -32, -1006, -1006, -1006,  -117,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,   971,   988,
   -1006,   990, -1006,   991, -1006, -1006,   992, -1006, -1006, -1006,
     338,   993, -1006, -1006,   994, -1006,   995, -1006, -1006,  1177,
   -1006,   967,   409,  1103,  1207,   998,    78, -1006,    -5,    78,
     996,  1000,    -2,    39, -1006, -1006,   999,  1150,  1123, -1006,
    1003,  1004,  1005,  1006,  1007,  1008,  1017, -1006, -1006,  1018,
    1019,  1020, -1006,  1120,  1021,  1022,  1103,    73,  1218,   118,
    1024,  1023,    -4, -1006, -1006, -1006, -1006,  1026,  1028,   368,
   -1006,    39,  1145,   338,    39,    39,    39,    39,   -84,    34,
    1223,  1092, -1006,  1225, -1006, -1006, -1006, -1006,  1027,  1031,
    1132,  1029,  1227, -1006,  1034,  1035, -1006, -1006,   291,   942,
   -1006, -1006,  1038,  1040,  1140,  1238,    83,    78,    34,    19,
      19,    -8,   -33,  1245,  1042, -1006,  1247,  1132, -1006, -1006,
    1045, -1006, -1006, -1006,  1248, -1006,  1214, -1006,   184, -1006,
   -1006, -1006, -1006, -1006, -1006,   987, -1006, -1006, -1006, -1006,
    1044,   409,  1164,  1048,  1078,  1261,  1049,  1088,    78,  1051,
    1103,  1178,  1180, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,  1167,  1052,
    1078,   863, -1006,   851,  1059,  1057,  1063,  1060,   -84, -1006,
   -1006, -1006, -1006,  1140,  1061, -1006,  1064, -1006,  1065, -1006,
   -1006,  1132,  1068, -1006,  1069, -1006, -1006,   851, -1006,    40,
   -1006, -1006,  1070, -1006, -1006,    78,   373, -1006,    78,  1078,
    1187,    40,   373, -1006,  1071,    -8, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006,    81,  1072,   373, -1006, -1006,   941,
      -8,  1073, -1006, -1006, -1006, -1006
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1006, -1006,  1274, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006,   138, -1006, -1006,  1046,  -106,  -368,   874,  1066,
    1220,  -488, -1006, -1006, -1006, -1006,   628, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,  1230, -1006,
   -1006, -1006,   314, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006,   814,  1081, -1006, -1006, -1006, -1006,   754, -1006,
   -1006, -1006,   346, -1006, -1006, -1006,  -587,   323,   263, -1006,
   -1006,   465,   284, -1006, -1006, -1006, -1006, -1006,   206, -1006,
   -1006,  1125, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006,  -543, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,   274,
     575,  -819,   246, -1006,  -909, -1006,  -947,   205, -1005,  -450,
    -602, -1006, -1006, -1006,   490,   896,  -214,  -175,  -381,   780,
     149,  -380,  -481,  -633,  -519,  -449,  -514,  -610,  -168,   -75,
   -1006,   536, -1006, -1006,   805, -1006, -1006,  1032,   -67,   743,
   -1006,  -475, -1006, -1006, -1006, -1006, -1006,  -173, -1006
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -233
static const yytype_int16 yytable[] =
{
     242,   269,   492,   748,   239,   252,   612,   579,   665,   314,
     375,   316,   663,   631,   632,   299,   645,   330,   331,   234,
     333,   335,   534,   336,   234,   522,   523,   343,   344,   345,
     555,   351,   234,   785,   354,   976,   383,   288,   362,   639,
     364,   628,   325,   234,   325,   517,   357,   667,   308,   383,
     358,   359,   782,   602,   378,   379,   380,   381,   618,   626,
     864,   623,   339,   837,    16,   849,   257,  1037,   407,   260,
     261,   260,   261,   260,   261,   409,   367,   954,   288,   852,
     570,   299,   649,   573,  1014,  1116,  1147,  1076,     7,   555,
    1090,   654,   791,   300,  1054,   319,   260,   261,   655,   433,
     434,   311,   436,     8,   357,   248,   555,   288,   358,   359,
     312,   287,   643,   583,   644,   289,   249,   250,   320,   321,
     318,   584,   357,   614,  1140,     9,   358,   359,   615,   290,
     350,   388,   389,   390,   391,   392,   827,   931,  1055,   788,
     352,   353,   299,   355,   356,   824,    11,   363,   788,   979,
     366,   980,   368,   371,   955,   627,   289,    13,   258,   300,
     384,  1111,  1148,   499,  1129,   850,   394,    14,   410,   863,
     290,   932,   933,   170,  1015,  1077,   858,  1078,   934,   619,
     248,   360,   171,   992,   876,   289,   740,   935,   603,   309,
     234,   249,   250,   264,   884,   936,   624,   264,   838,   290,
    1070,   248,   326,   340,   334,   224,   265,   233,   265,   398,
     241,   399,   249,   250,   853,   579,   400,   401,   402,  1079,
     300,   937,    15,   833,   173,   266,   267,   243,   301,   266,
     267,   405,   656,   830,   302,   535,   235,   248,   524,   657,
     831,   985,   248,   630,   235,   174,   236,   237,   249,   250,
     291,   236,   237,   249,   250,   235,   292,   235,   348,   236,
     237,   984,   553,   554,   426,   609,   767,   260,   261,   175,
     236,   237,   236,   237,   777,   778,    79,   574,   575,   293,
     294,   821,   238,   822,   650,   861,   645,   238,   247,   890,
     246,   291,   176,   597,   301,   238,   271,   292,   904,   601,
     302,   498,   630,   606,   607,   608,   238,   613,   238,   530,
     507,   508,   506,   509,   510,   511,   512,   513,   403,   630,
     291,   303,   304,   666,   662,   635,   292,   558,   559,   177,
     384,   525,   640,   388,   389,   390,   391,   392,   324,     1,
       2,   260,   261,   531,   558,   559,   337,   338,   178,   293,
     294,   591,   542,   592,   593,   301,   550,   856,   540,   664,
     179,   302,   386,   388,   389,   390,   391,   392,   552,   180,
     372,   568,   562,   677,   678,   572,   565,   543,   544,   658,
     315,   660,   303,   304,   582,   387,   659,   576,   661,   404,
     871,   264,   408,   732,   724,   181,   859,   872,  1096,   329,
     873,   -18,   405,   348,   874,   332,   406,   182,   248,   260,
     261,   875,   183,   262,   263,   866,   867,   236,   237,   249,
     250,   431,   432,   266,   267,   320,   321,  1013,   669,   184,
    1016,   390,   391,   392,   388,   389,   390,   391,   392,   394,
     262,   263,   388,   389,   390,   391,   392,   185,   636,   652,
    1004,  1005,  1006,   238,   390,   391,   392,   186,   272,   273,
     274,   275,   276,   187,   277,   264,   278,   279,   280,   281,
     282,   675,   253,   254,   255,   256,   283,   388,   389,   390,
     391,   392,   188,   959,   189,  1057,  1083,  1084,   190,   555,
     960,   373,   780,   556,   191,   192,   880,   266,   267,   881,
     193,   194,   983,   195,  1040,   196,   376,   284,  1081,   197,
     961,   198,   199,   200,  1082,   201,   202,  1134,   203,   204,
     388,   389,   390,   391,   392,   205,   206,   962,   207,  1142,
     208,   209,   210,   264,   211,   212,   213,   214,   842,   388,
     389,   390,   391,   392,   265,   215,   216,   217,   218,  1109,
     388,   389,   390,   391,   392,   220,   692,   221,   222,   223,
     693,   694,   769,   229,   230,   266,   267,   260,   261,   231,
     377,   963,   964,   965,   966,   232,   967,   968,   969,   970,
     971,   846,  1046,   779,   244,  1047,   844,   382,   889,   420,
     385,   482,   893,   415,   421,   896,   897,   898,   685,   426,
     901,   902,   903,   691,   865,   418,  1136,   793,   794,  1139,
     695,   797,   419,   792,   422,   423,   426,   795,   445,   425,
     798,   799,   427,   435,   696,   697,   698,   699,   428,   700,
     728,   701,   429,   260,   261,   437,   388,   389,   390,   391,
     392,   438,   439,   441,   899,   388,   389,   390,   391,   392,
     443,   930,   388,   389,   390,   391,   392,   444,   446,   449,
     702,   703,   704,   882,   705,   450,  -232,   706,   707,   708,
     709,   710,   452,   711,   453,   454,   260,   261,   455,   388,
     389,   390,   391,   392,   272,   273,   274,   275,   276,   456,
     277,   264,   278,   279,   280,   281,   282,   712,   735,   713,
     714,   457,   557,   885,   458,   462,   459,   460,   715,   716,
     717,   718,   719,   720,   461,   878,   463,   388,   389,   390,
     391,   392,   464,   266,   267,   888,   465,   558,   559,   466,
      83,   721,    85,   284,    87,   467,    89,   468,    91,  1020,
      93,   993,    95,   469,    97,   722,    99,   388,   389,   390,
     391,   392,   388,   389,   390,   391,   392,   264,  1137,   388,
     991,   390,   391,   392,  1143,   470,   474,   471,   265,   472,
     473,   476,   477,   478,   479,   480,   481,   483,  1152,   484,
    1050,  1051,  1052,  1053,   485,   486,   487,   488,   489,   266,
     267,   490,   491,   272,   273,   274,   275,   276,   494,   277,
     264,   278,   279,   280,   281,   282,   495,   989,   496,   497,
     500,   283,   550,   260,   261,   501,   502,   503,   533,   238,
     514,   537,   505,   516,   515,   526,   527,   546,   528,   225,
     538,   564,   266,   267,   541,    82,   539,    84,   548,    86,
     549,    88,   284,    90,   547,    92,   563,    94,   566,    96,
     567,    98,   569,   571,   577,   580,  1085,    16,   581,   586,
     587,   588,   589,   590,   596,    17,   598,   599,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,   600,   605,   610,    31,    32,    33,    34,    35,   617,
      36,   620,   621,  -181,   638,    37,    38,    39,    40,    41,
      42,    43,    44,   622,    45,    46,    47,   633,    48,   641,
      49,    50,    51,    52,    53,    54,  1048,   642,   653,    55,
      56,   670,    57,   668,   671,   674,   673,   679,   680,    58,
     272,   273,   274,   275,   276,   681,   277,   264,   278,   279,
     280,   281,   282,   682,   683,   684,   686,   687,   283,   688,
      59,   689,   731,    60,   732,   733,   690,   734,   727,   736,
    1145,   737,   738,   739,   740,   741,   744,   746,   742,   266,
     267,    61,   743,   745,   753,  1154,   760,    62,   747,   284,
      63,    64,   749,    65,   750,    66,   764,  1118,   756,   685,
     768,    67,   752,   776,    68,    69,   754,   755,   758,   759,
      70,    71,   761,    72,   773,    73,   260,   261,   762,   319,
     763,  1133,   765,   766,   775,    81,   771,   772,   774,   260,
     261,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   786,
      74,    75,    76,    77,   787,    78,   788,   790,   550,   789,
     796,   800,   823,   802,   801,   803,   804,   805,   806,   807,
     808,   809,   810,   811,   812,   813,   826,   814,   815,    79,
     828,   818,   816,   817,   819,    80,    81,   820,   824,   827,
     829,   834,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     835,   836,   840,   317,   841,   845,   843,   847,   905,   906,
     848,   851,   855,   860,   877,   862,   879,   892,   883,   891,
     886,   887,   894,   272,   273,   274,   275,   276,   900,   277,
     264,   278,   279,   280,   281,   282,   272,   273,   274,   275,
     276,   283,   277,   264,   278,   279,   280,   281,   282,   907,
     908,   895,   941,   909,   283,   939,   942,   944,   945,   946,
     948,   950,   266,   267,   947,   949,   952,   974,   953,   957,
     958,   973,   284,   975,   977,   266,   267,   987,   978,   988,
    1002,  1003,   986,   990,   910,   284,   994,   911,   912,   913,
     914,   915,   916,   917,   918,   919,   920,   921,   922,   923,
     924,   925,   926,   995,  1009,   996,   997,   998,   999,  1000,
    1001,  1011,  1012,  1017,  1018,  1023,  1021,  1022,  1024,  1025,
    1026,  1027,  1028,  1029,  1034,  1039,  1049,  1058,  1059,  1060,
    1063,  1066,  1030,  1031,  1032,  1033,  1036,  1073,  1035,  1041,
    1044,  1061,  1075,  1065,  1042,  1045,  1062,   248,  1067,  1087,
    1068,  1089,  1071,  1094,  1072,  1095,  1088,  1104,   249,   250,
    1092,  1099,  1098,  1103,  1101,  1106,  1107,  1108,  1110,  1115,
    1112,  1113,  1114,  1119,  1120,  1121,  1125,  1122,  1141,    10,
    1128,  1127,  1130,  1131,  1135,  1144,  1150,  1153,   529,   374,
    1155,   226,   783,   417,   219,   611,  1043,   365,  1019,   676,
    1038,  1091,  1069,   951,  1123,   298,  1086,  1100,  1124,   929,
     869,   825,   725,   440,     0,   532,     0,     0,   672,   651
};

static const yytype_int16 yycheck[] =
{
     106,   176,   370,   605,    79,   173,   481,   456,   527,   184,
     224,   186,   526,   494,   495,     3,   504,   192,   193,     4,
     195,   196,     3,   196,     4,   406,   406,   200,   201,   202,
       3,   206,     4,   666,   209,   854,     8,     3,   213,    81,
     215,   491,    56,     4,    56,     4,     4,   528,    81,     8,
       8,     9,   662,    81,   229,   230,   231,   232,    65,     4,
      81,    81,    81,    81,     6,     4,     4,   976,   243,    81,
      82,    81,    82,    81,    82,   243,     5,     4,     3,    81,
     448,     3,   219,   451,    89,  1090,     5,     4,   215,     3,
    1037,   214,   679,    81,   178,   216,    81,    82,   221,   274,
     275,    72,   277,   215,     4,   222,     3,     3,     8,     9,
      81,   178,   139,    73,   141,    81,   233,   234,   239,   240,
     187,    81,     4,    76,  1129,     0,     8,     9,    81,    95,
     205,   268,   269,   270,   271,   272,   214,    77,   222,   217,
     207,   208,     3,   210,   211,   214,   122,   214,   217,   153,
     217,   155,    81,   220,    81,   100,    81,   222,    96,    81,
     235,  1070,    81,   377,  1111,   104,   241,   222,   243,   771,
      95,   111,   112,   214,   179,    92,   763,    94,   118,   186,
     222,    81,   123,   215,   786,    81,   218,   127,   216,   222,
       4,   233,   234,   205,   796,   135,   216,   205,   216,    95,
    1019,   222,   216,   222,   216,   218,   216,   224,   216,    23,
     216,    25,   233,   234,   216,   664,    30,    31,    32,   136,
      81,   161,   215,   742,   215,   237,   238,   273,   216,   237,
     238,   216,   214,    81,   222,   216,   216,   222,   406,   221,
      88,   874,   222,   216,   216,   215,   231,   232,   233,   234,
     216,   231,   232,   233,   234,   216,   222,   216,   216,   231,
     232,   871,   437,   438,     4,   479,   634,    81,    82,   215,
     231,   232,   231,   232,   654,   656,   218,   452,   453,   245,
     246,   731,   267,   732,   219,   766,   774,   267,   214,   803,
       3,   216,   215,   468,   216,   267,   100,   222,   817,   472,
     222,   376,   216,   476,   477,   478,   267,   482,   267,   415,
     385,   386,   217,   388,   389,   390,   391,   392,   132,   216,
     216,   243,   244,   220,   220,   151,   222,   241,   242,   215,
     405,   406,   500,   268,   269,   270,   271,   272,   189,    15,
      16,    81,    82,   418,   241,   242,   197,   198,   215,   245,
     246,    81,   427,    83,    84,   216,    80,    81,   425,   220,
     215,   222,   196,   268,   269,   270,   271,   272,   435,   215,
     221,   446,   439,   548,   549,   450,   443,    86,    87,   214,
       4,   214,   243,   244,   459,   219,   221,   454,   221,   203,
     214,   205,   243,   214,   569,   215,   764,   221,   214,   222,
     221,   217,   216,   216,   214,    93,   220,   215,   222,    81,
      82,   221,   215,    85,    86,   227,   228,   231,   232,   233,
     234,   272,   273,   237,   238,   239,   240,   946,   219,   215,
     949,   270,   271,   272,   268,   269,   270,   271,   272,   514,
      85,    86,   268,   269,   270,   271,   272,   215,   274,   516,
      41,    42,    43,   267,   270,   271,   272,   215,   198,   199,
     200,   201,   202,   215,   204,   205,   206,   207,   208,   209,
     210,   219,    10,    11,    12,    13,   216,   268,   269,   270,
     271,   272,   215,    91,   215,   999,  1029,  1030,   215,     3,
      98,   141,   660,     7,   215,   215,   214,   237,   238,   217,
     215,   215,   870,   215,   979,   215,   273,   247,  1027,   215,
     118,   215,   215,   215,  1028,   215,   215,  1119,   215,   215,
     268,   269,   270,   271,   272,   215,   215,   135,   215,  1131,
     215,   215,   215,   205,   215,   215,   215,   215,   752,   268,
     269,   270,   271,   272,   216,   215,   215,   215,   215,  1068,
     268,   269,   270,   271,   272,   215,     4,   215,   215,   215,
       8,     9,   637,   215,   215,   237,   238,    81,    82,   215,
     218,   179,   180,   181,   182,   215,   184,   185,   186,   187,
     188,   756,   214,   658,   215,   217,   754,   216,   802,   214,
     218,   214,   806,   220,   214,   809,   810,   811,     3,     4,
     814,   815,   816,   219,   772,   218,  1125,   682,   683,  1128,
      58,   686,   274,   680,   214,   213,     4,   684,     3,   216,
     687,   688,   218,   216,    72,    73,    74,    75,   214,    77,
     219,    79,   214,    81,    82,   216,   268,   269,   270,   271,
     272,   216,   216,   216,   812,   268,   269,   270,   271,   272,
     276,   819,   268,   269,   270,   271,   272,   214,   218,     3,
     108,   109,   110,   214,   112,   218,   220,   115,   116,   117,
     118,   119,   214,   121,   214,   214,    81,    82,   214,   268,
     269,   270,   271,   272,   198,   199,   200,   201,   202,   214,
     204,   205,   206,   207,   208,   209,   210,   145,   219,   147,
     148,   214,   216,   219,     4,   216,   218,   214,   156,   157,
     158,   159,   160,   161,   214,   790,     4,   268,   269,   270,
     271,   272,   214,   237,   238,   800,   214,   241,   242,   214,
     232,   179,   234,   247,   236,   214,   238,   214,   240,   953,
     242,   909,   244,   214,   246,   193,   248,   268,   269,   270,
     271,   272,   268,   269,   270,   271,   272,   205,  1126,   268,
     269,   270,   271,   272,  1132,     4,     6,   214,   216,   214,
     214,   214,   214,   214,   214,     4,   214,   214,  1146,   214,
     994,   995,   996,   997,   214,   214,   214,   214,   214,   237,
     238,   214,   214,   198,   199,   200,   201,   202,   214,   204,
     205,   206,   207,   208,   209,   210,   214,   882,   214,   219,
     214,   216,    80,    81,    82,   214,   214,   214,     3,   267,
     214,     3,   217,   215,   217,   215,   215,     5,   215,   225,
     214,   217,   237,   238,   214,   231,   219,   233,   214,   235,
     214,   237,   247,   239,   223,   241,   211,   243,    96,   245,
     214,   247,   214,   214,    80,    80,  1031,     6,   214,    38,
       4,   214,    38,    40,   179,    14,    90,   214,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    90,   214,   214,    33,    34,    35,    36,    37,    94,
      39,   183,   100,   131,   219,    44,    45,    46,    47,    48,
      49,    50,    51,   103,    53,    54,    55,   142,    57,   137,
      59,    60,    61,    62,    63,    64,   991,   138,   214,    68,
      69,     3,    71,   221,   214,     4,   217,   275,   214,    78,
     198,   199,   200,   201,   202,   217,   204,   205,   206,   207,
     208,   209,   210,   214,   214,   214,   218,   214,   216,   214,
      99,   214,   214,   102,   214,   214,   222,     4,   222,   214,
    1135,   214,     4,   214,   218,   214,     4,     4,   215,   237,
     238,   120,   214,   214,    80,  1150,     4,   126,   214,   247,
     129,   130,   214,   132,   214,   134,   131,  1093,   215,     3,
     274,   140,   214,     4,   143,   144,   214,   214,   214,   214,
     149,   150,   214,   152,   215,   154,    81,    82,   214,   216,
     214,  1117,   214,   214,   217,   225,   214,   214,   214,    81,
      82,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   214,
     189,   190,   191,   192,     3,   194,   217,   214,    80,   223,
     214,   216,     5,   215,   217,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,     4,   215,   215,   218,
     179,   217,   215,   215,   214,   224,   225,   214,   214,   214,
       4,   179,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
      90,   214,   222,   178,    90,    66,   214,   214,    83,    84,
      97,   214,    97,    97,   214,   220,   217,   106,   217,   107,
     217,   217,   113,   198,   199,   200,   201,   202,   146,   204,
     205,   206,   207,   208,   209,   210,   198,   199,   200,   201,
     202,   216,   204,   205,   206,   207,   208,   209,   210,   124,
     125,   114,     4,   128,   216,   214,   214,   214,     4,   215,
     214,     4,   237,   238,   219,   215,   214,     4,   214,   214,
     214,   214,   247,     4,   214,   237,   238,     5,   214,    41,
       3,   214,   217,   217,   159,   247,   215,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   215,   101,   215,   215,   215,   215,   215,
     215,     4,   214,   217,   214,    92,   217,    67,   215,   215,
     215,   215,   215,   215,   104,     7,    81,     4,   136,     4,
      98,     4,   215,   215,   215,   215,   214,    97,   217,   215,
     214,   214,     4,   214,   221,   217,   215,   222,   214,     4,
     215,     4,   214,     5,   214,    41,   214,   179,   233,   234,
     215,   217,   275,   215,   100,     4,   217,   179,   217,   217,
      92,    91,   105,   214,   217,   212,   215,   217,    91,     5,
     215,   217,   214,   214,   214,   214,   214,  1149,   414,   223,
     217,    71,   664,   247,    64,   481,   982,   216,   952,   545,
     977,  1038,  1018,   838,  1098,   180,  1032,  1061,  1103,   819,
     774,   736,   569,   281,    -1,   419,    -1,    -1,   538,   514
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   278,   279,   280,   281,   215,   215,     0,
     279,   122,   291,   222,   222,   215,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    33,    34,    35,    36,    37,    39,    44,    45,    46,
      47,    48,    49,    50,    51,    53,    54,    55,    57,    59,
      60,    61,    62,    63,    64,    68,    69,    71,    78,    99,
     102,   120,   126,   129,   130,   132,   134,   140,   143,   144,
     149,   150,   152,   154,   189,   190,   191,   192,   194,   218,
     224,   225,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     282,   283,   284,   285,   286,   293,   295,   296,   297,   298,
     300,   301,   309,   312,   313,   315,   316,   323,   325,   326,
     328,   330,   332,   336,   337,   338,   341,   343,   350,   357,
     360,   366,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   382,   383,   384,   385,   389,   390,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   403,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   415,
     214,   123,   292,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   315,
     215,   215,   215,   215,   218,   225,   297,   299,   317,   215,
     215,   215,   215,   224,     4,   216,   231,   232,   267,   446,
     456,   216,   293,   273,   215,   327,     3,   214,   222,   233,
     234,   432,   445,    10,    11,    12,    13,     4,    96,   457,
      81,    82,    85,    86,   205,   216,   237,   238,   358,   434,
     435,   100,   198,   199,   200,   201,   202,   204,   206,   207,
     208,   209,   210,   216,   247,   434,   454,   455,     3,    81,
      95,   216,   222,   245,   246,   368,   443,   444,   368,     3,
      81,   216,   222,   243,   244,   362,   441,   442,    81,   222,
     416,    72,    81,   426,   434,     4,   434,   178,   455,   216,
     239,   240,   437,   438,   437,    56,   216,   464,   465,   222,
     434,   434,    93,   434,   216,   434,   464,   437,   437,    81,
     222,   340,   417,   464,   464,   464,   433,   446,   216,   339,
     446,   434,   455,   455,   434,   455,   455,     4,     8,     9,
      81,   458,   434,   455,   434,   340,   455,     5,    81,   351,
     333,   455,   437,   141,   296,   433,   273,   218,   434,   434,
     434,   434,   216,     8,   446,   218,   196,   219,   268,   269,
     270,   271,   272,   445,   446,   451,   452,   453,    23,    25,
      30,    31,    32,   132,   203,   216,   220,   434,   437,   445,
     446,   460,   461,   462,   331,   220,   294,   292,   218,   274,
     214,   214,   214,   213,   436,   216,     4,   218,   214,   214,
     345,   437,   437,   434,   434,   216,   434,   216,   216,   216,
     454,   216,   455,   276,   214,     3,   218,   369,   367,     3,
     218,   361,   214,   214,   214,   214,   214,   214,     4,   218,
     214,   214,   216,     4,   214,   214,   214,   214,   214,   214,
       4,   214,   214,   214,     6,   314,   214,   214,   214,   214,
       4,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   294,   334,   214,   214,   214,   219,   446,   433,
     214,   214,   214,   214,   310,   217,   217,   446,   446,   446,
     446,   446,   446,   446,   214,   217,   215,     4,   305,   306,
     307,   308,   435,   438,   445,   446,   215,   215,   215,   295,
     293,   446,   432,     3,     3,   216,   391,     3,   214,   219,
     455,   214,   446,    86,    87,   359,     5,   223,   214,   214,
      80,   353,   455,   434,   434,     3,     7,   216,   241,   242,
     439,   440,   455,   211,   217,   455,    96,   214,   446,   214,
     294,   214,   446,   294,   434,   434,   455,    80,   303,   442,
      80,   214,   446,    73,    81,   427,    38,     4,   214,    38,
      40,    81,    83,    84,   428,   431,   179,   434,    90,   214,
      90,   464,    81,   216,   346,   214,   464,   464,   464,   433,
     214,   339,   458,   434,    76,    81,   463,    94,    65,   186,
     183,   100,   103,    81,   216,   347,     4,   100,   426,   335,
     216,   439,   439,   142,   329,   151,   274,   324,   219,    81,
     445,   137,   138,   139,   141,   298,   448,   449,   450,   219,
     219,   451,   455,   214,   214,   221,   214,   221,   214,   221,
     214,   221,   220,   443,   220,   441,   220,   439,   221,   219,
       3,   214,   436,   217,     4,   219,   345,   434,   434,   275,
     214,   217,   214,   214,   214,     3,   218,   214,   214,   214,
     222,   219,     4,     8,     9,    58,    72,    73,    74,    75,
      77,    79,   108,   109,   110,   112,   115,   116,   117,   118,
     119,   121,   145,   147,   148,   156,   157,   158,   159,   160,
     161,   179,   193,   370,   434,   456,   459,   222,   219,   363,
     355,   214,   214,   214,     4,   219,   214,   214,     4,   214,
     218,   214,   215,   214,     4,   214,     4,   214,   427,   214,
     214,   381,   214,    80,   214,   214,   215,   386,   214,   214,
       4,   214,   214,   214,   131,   214,   214,   294,   274,   446,
     318,   214,   214,   215,   214,   217,     4,   438,   435,   446,
     445,   302,   444,   303,   304,   440,   214,     3,   217,   223,
     214,   353,   455,   446,   446,   455,   214,   446,   455,   455,
     216,   217,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   217,   214,
     214,   426,   442,     5,   214,   417,     4,   214,   179,     4,
      81,    88,   430,   441,   179,    90,   214,    81,   216,   348,
     222,    90,   433,   214,   445,    66,   434,   214,    97,     4,
     104,   214,    81,   216,   349,    97,    81,   352,   353,   294,
      97,   439,   220,   427,    81,   445,   227,   228,   447,   448,
     311,   214,   221,   221,   214,   221,   427,   214,   446,   217,
     214,   217,   214,   217,   427,   219,   217,   217,   446,   433,
     443,   107,   106,   433,   113,   114,   433,   433,   433,   445,
     146,   433,   433,   433,   441,    83,    84,   124,   125,   128,
     159,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   364,   429,   431,
     445,    77,   111,   112,   118,   127,   135,   161,   356,   214,
     289,     4,   214,   418,   214,     4,   215,   219,   214,   215,
       4,   358,   214,   214,     4,    81,   354,   214,   214,    91,
      98,   118,   135,   179,   180,   181,   182,   184,   185,   186,
     187,   188,   387,   214,     4,     4,   418,   214,   214,   153,
     155,   319,   320,   294,   444,   440,   217,     5,    41,   446,
     217,   269,   215,   445,   215,   215,   215,   215,   215,   215,
     215,   215,     3,   214,    41,    42,    43,   419,   420,   101,
     421,     4,   214,   441,    89,   179,   441,   217,   214,   349,
     433,   217,    67,    92,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   104,   217,   214,   421,   354,     7,
     458,   215,   221,   319,   214,   217,   214,   217,   446,    81,
     433,   433,   433,   433,   178,   222,   365,   443,     4,   136,
       4,   214,   215,    98,   423,   214,     4,   214,   215,   359,
     418,   214,   214,    97,   424,     4,     4,    92,    94,   136,
     388,   441,   443,   391,   391,   434,   416,     4,   214,     4,
     423,   355,   215,   322,     5,    41,   214,   287,   275,   217,
     419,   100,   422,   215,   179,   425,     4,   217,   179,   441,
     217,   421,    92,    91,   105,   217,   425,   321,   293,   214,
     217,   212,   217,   365,   424,   215,   401,   217,   215,   423,
     214,   214,   342,   293,   427,   214,   441,   294,   402,   441,
     425,    91,   427,   294,   214,   434,   344,     5,    81,   290,
     214,   288,   294,   289,   434,   217
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
#line 296 "lev_comp.y"
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
#line 319 "lev_comp.y"
    {
		      start_level_def(&splev, (yyvsp[(3) - (3)].map));
		      (yyval.map) = (yyvsp[(3) - (3)].map);
		  ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 324 "lev_comp.y"
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
#line 346 "lev_comp.y"
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
#line 359 "lev_comp.y"
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
#line 371 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_ROGUE,0,0,0,
					  0,0,0,0, SPO_INITLEVEL));
		  ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 377 "lev_comp.y"
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
#line 406 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_TILESET));
		  ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 412 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), (int)(yyvsp[(3) - (5)].i), SPO_FOREST));
		  ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 416 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(0, (int)(yyvsp[(3) - (3)].i), SPO_FOREST));
		  ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 422 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), (int)(yyvsp[(3) - (5)].i), SPO_MONSTER_GENERATION));
		  ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 428 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_BOUNDARY_TYPE));
		  ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 434 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 438 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 444 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_COPY));
		      (yyval.i) = 0;
		  ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 449 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 455 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 459 "lev_comp.y"
    {
		      (yyval.i) = what_map_char((char) (yyvsp[(2) - (2)].i));
		  ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 470 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 474 "lev_comp.y"
    {
		      add_opvars(splev, "io",
                                 VA_PASS2((int) (yyvsp[(3) - (3)].i), SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 481 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 485 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 491 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 495 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 501 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (3)].i);
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 603 "lev_comp.y"
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

  case 120:

/* Line 1455 of yacc.c  */
#line 619 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 625 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 631 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 637 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 643 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 649 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 655 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 661 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 667 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 677 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 687 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 697 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 707 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 717 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 727 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 739 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 744 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 751 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 756 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 763 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 768 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 775 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 779 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 785 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 790 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 797 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 801 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 807 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 811 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 817 "lev_comp.y"
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

  case 150:

/* Line 1455 of yacc.c  */
#line 839 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 843 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 854 "lev_comp.y"
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

  case 153:

/* Line 1455 of yacc.c  */
#line 909 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 915 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 919 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 925 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 932 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 936 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 944 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 948 "lev_comp.y"
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

  case 161:

/* Line 1455 of yacc.c  */
#line 971 "lev_comp.y"
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

  case 164:

/* Line 1455 of yacc.c  */
#line 1016 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1026 "lev_comp.y"
    {
		  ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1029 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1039 "lev_comp.y"
    {
		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1044 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1058 "lev_comp.y"
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

  case 172:

/* Line 1455 of yacc.c  */
#line 1095 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1100 "lev_comp.y"
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

  case 174:

/* Line 1455 of yacc.c  */
#line 1129 "lev_comp.y"
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

  case 175:

/* Line 1455 of yacc.c  */
#line 1143 "lev_comp.y"
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

  case 176:

/* Line 1455 of yacc.c  */
#line 1159 "lev_comp.y"
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

  case 177:

/* Line 1455 of yacc.c  */
#line 1179 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1191 "lev_comp.y"
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

  case 179:

/* Line 1455 of yacc.c  */
#line 1211 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1217 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1227 "lev_comp.y"
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

  case 182:

/* Line 1455 of yacc.c  */
#line 1245 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1255 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1261 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1266 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1271 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1278 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1285 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1294 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1302 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1313 "lev_comp.y"
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

  case 192:

/* Line 1455 of yacc.c  */
#line 1329 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1336 "lev_comp.y"
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

  case 194:

/* Line 1455 of yacc.c  */
#line 1352 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1359 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1363 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1369 "lev_comp.y"
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

  case 198:

/* Line 1455 of yacc.c  */
#line 1379 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1385 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1394 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1400 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1405 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1411 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1416 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1422 "lev_comp.y"
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

  case 206:

/* Line 1455 of yacc.c  */
#line 1433 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1447 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1451 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1461 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1468 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1476 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1481 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1486 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1491 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1496 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1501 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1506 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1514 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1521 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1529 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1545 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1549 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1555 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1563 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1569 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1577 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1585 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1590 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1596 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1602 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1608 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1614 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1619 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(0, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1624 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1629 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1634 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1639 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1644 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1649 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1654 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1659 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1664 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1669 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1675 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1680 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_PROTECTOR));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1685 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_MAXHP));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1690 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_LEVEL_ADJUSTMENT));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1695 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_KEEP_ORIGINAL_INVENTORY));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1702 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1711 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1715 "lev_comp.y"
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

  case 262:

/* Line 1455 of yacc.c  */
#line 1728 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1736 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1746 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1754 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1763 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1770 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1778 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1784 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1789 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1794 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1799 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1804 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1809 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1814 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1819 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1824 "lev_comp.y"
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

  case 278:

/* Line 1455 of yacc.c  */
#line 1838 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1844 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1849 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1854 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1859 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1864 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1869 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1874 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1879 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1884 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1889 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1894 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1899 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1904 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x400000;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1909 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x800000;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1914 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x1000000;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1919 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_MYTHIC_TYPE));
		      (yyval.i) = 0x2000000;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1925 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x4000000;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1930 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x8000000;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1935 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x10000000;
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1942 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1948 "lev_comp.y"
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

  case 300:

/* Line 1455 of yacc.c  */
#line 1977 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1982 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 1990 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 1996 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2000 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2008 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2015 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2022 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2032 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2042 "lev_comp.y"
    {
		      long rtyp = 0;
		      switch((yyvsp[(6) - (6)].i)) {
		      case -1: rtyp = LR_TELE; break;
		      case  0: rtyp = LR_DOWNTELE; break;
		      case  1: rtyp = LR_UPTELE; break;
		      case  2: rtyp = LR_NOTELE; break;
		      }
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (6)].lregn).x1, (yyvsp[(3) - (6)].lregn).y1, (yyvsp[(3) - (6)].lregn).x2, (yyvsp[(3) - (6)].lregn).y2, (yyvsp[(3) - (6)].lregn).area,
					   (yyvsp[(5) - (6)].lregn).x1, (yyvsp[(5) - (6)].lregn).y1, (yyvsp[(5) - (6)].lregn).x2, (yyvsp[(5) - (6)].lregn).y2, (yyvsp[(5) - (6)].lregn).area,
					   rtyp, 0, (char *)0, SPO_LEVREGION));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2058 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2068 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2072 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2078 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2084 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2090 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2094 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2100 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2106 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2113 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2121 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2127 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2132 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2137 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2142 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2147 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2153 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2159 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2164 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2169 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2174 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2179 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2184 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2194 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2200 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2206 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2211 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2218 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2225 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2229 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2235 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2241 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2247 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2252 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2259 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2265 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2275 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2281 "lev_comp.y"
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

  case 352:

/* Line 1455 of yacc.c  */
#line 2300 "lev_comp.y"
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

  case 353:

/* Line 1455 of yacc.c  */
#line 2312 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2316 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2322 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2327 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2332 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (long)(yyvsp[(9) - (9)].i), (long)(yyvsp[(7) - (9)].i), (long)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2337 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((long)(yyvsp[(9) - (13)].i), (long)(yyvsp[(7) - (13)].i), (long)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2343 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2347 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2353 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, (int)(yyvsp[(11) - (11)].i), (int)(yyvsp[(9) - (11)].i), (int)(yyvsp[(7) - (11)].i), (int)(yyvsp[(5) - (11)].i), SPO_DECORATION));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2357 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6((int)(yyvsp[(13) - (13)].i), (int)(yyvsp[(11) - (13)].i), (int)(yyvsp[(9) - (13)].i), (int)(yyvsp[(7) - (13)].i), (int)(yyvsp[(5) - (13)].i), SPO_DECORATION));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2361 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, 1, (int)(yyvsp[(7) - (7)].i), 0, (int)(yyvsp[(5) - (7)].i), SPO_DECORATION));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2367 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2373 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2377 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2383 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2387 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2393 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2397 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2402 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2409 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2413 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2420 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2424 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2429 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2436 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2440 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2445 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2453 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2459 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2466 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2470 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2477 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2488 "lev_comp.y"
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

  case 388:

/* Line 1455 of yacc.c  */
#line 2501 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2505 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2511 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2515 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2522 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2526 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2530 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2536 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2540 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2546 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2552 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2556 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2562 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2568 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2573 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2590 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2598 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2608 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2617 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2622 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2629 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2640 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2646 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2650 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2654 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2661 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2671 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2678 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2682 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2688 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2692 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2700 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2704 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2711 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2721 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2734 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2738 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2745 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2755 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2764 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2775 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2779 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2786 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2796 "lev_comp.y"
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

  case 442:

/* Line 1455 of yacc.c  */
#line 2807 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2816 "lev_comp.y"
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

  case 444:

/* Line 1455 of yacc.c  */
#line 2826 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2832 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2836 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2843 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2853 "lev_comp.y"
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

  case 449:

/* Line 1455 of yacc.c  */
#line 2864 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2873 "lev_comp.y"
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

  case 451:

/* Line 1455 of yacc.c  */
#line 2883 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2887 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2893 "lev_comp.y"
    { ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2895 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2901 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2905 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2909 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2913 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2921 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2930 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2934 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2938 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2942 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2946 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2949 "lev_comp.y"
    { ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2953 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2960 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2969 "lev_comp.y"
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

  case 473:

/* Line 1455 of yacc.c  */
#line 3010 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3014 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3021 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3028 "lev_comp.y"
    {
			      size_t len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3038 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3042 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3050 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3054 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3058 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3062 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3066 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3071 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3075 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3079 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3084 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3089 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3094 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3098 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3103 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3108 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3112 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3116 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3121 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3125 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3132 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3138 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3142 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3148 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3164 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3168 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3172 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3176 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 518:

/* Line 1455 of yacc.c  */
#line 3198 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 519:

/* Line 1455 of yacc.c  */
#line 3202 "lev_comp.y"
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

  case 520:

/* Line 1455 of yacc.c  */
#line 3228 "lev_comp.y"
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
#line 7329 "lev_comp.tab.c"
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
#line 3256 "lev_comp.y"


/*lev_comp.y*/

