
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
     MESSAGE_TYPE = 270,
     MAZE_ID = 271,
     LEVEL_ID = 272,
     LEV_INIT_ID = 273,
     TILESET_ID = 274,
     GEOMETRY_ID = 275,
     NOMAP_ID = 276,
     BOUNDARY_TYPE_ID = 277,
     SPECIAL_TILESET_ID = 278,
     TILESET_PARAM_ID = 279,
     OBJECT_ID = 280,
     COBJECT_ID = 281,
     MONSTER_ID = 282,
     TRAP_ID = 283,
     DOOR_ID = 284,
     DRAWBRIDGE_ID = 285,
     MONSTER_GENERATION_ID = 286,
     object_ID = 287,
     monster_ID = 288,
     terrain_ID = 289,
     MAZEWALK_ID = 290,
     WALLIFY_ID = 291,
     REGION_ID = 292,
     SPECIAL_REGION_ID = 293,
     SPECIAL_LEVREGION_ID = 294,
     SPECIAL_REGION_TYPE = 295,
     NAMING_ID = 296,
     NAMING_TYPE = 297,
     FILLING = 298,
     IRREGULAR = 299,
     JOINED = 300,
     ALTAR_ID = 301,
     ANVIL_ID = 302,
     NPC_ID = 303,
     LADDER_ID = 304,
     STAIR_ID = 305,
     NON_DIGGABLE_ID = 306,
     NON_PASSWALL_ID = 307,
     ROOM_ID = 308,
     ARTIFACT_NAME_ID = 309,
     PORTAL_ID = 310,
     TELEPRT_ID = 311,
     BRANCH_ID = 312,
     LEV = 313,
     MINERALIZE_ID = 314,
     AGE_ID = 315,
     CORRIDOR_ID = 316,
     GOLD_ID = 317,
     ENGRAVING_ID = 318,
     FOUNTAIN_ID = 319,
     THRONE_ID = 320,
     MODRON_PORTAL_ID = 321,
     LEVEL_TELEPORTER_ID = 322,
     LEVEL_TELEPORT_DIRECTION_TYPE = 323,
     LEVEL_TELEPORT_END_TYPE = 324,
     POOL_ID = 325,
     SINK_ID = 326,
     NONE = 327,
     RAND_CORRIDOR_ID = 328,
     DOOR_STATE = 329,
     LIGHT_STATE = 330,
     CURSE_TYPE = 331,
     MYTHIC_TYPE = 332,
     ENGRAVING_TYPE = 333,
     KEYTYPE_ID = 334,
     LEVER_ID = 335,
     NO_PICKUP_ID = 336,
     DIRECTION = 337,
     RANDOM_TYPE = 338,
     RANDOM_TYPE_BRACKET = 339,
     A_REGISTER = 340,
     ALIGNMENT = 341,
     LEFT_OR_RIGHT = 342,
     CENTER = 343,
     TOP_OR_BOT = 344,
     ALTAR_TYPE = 345,
     ALTAR_SUBTYPE = 346,
     UP_OR_DOWN = 347,
     ACTIVE_OR_INACTIVE = 348,
     MODRON_PORTAL_TYPE = 349,
     NPC_TYPE = 350,
     FOUNTAIN_TYPE = 351,
     SPECIAL_OBJECT_TYPE = 352,
     CMAP_TYPE = 353,
     FLOOR_SUBTYPE = 354,
     FLOOR_SUBTYPE_ID = 355,
     FLOOR_ID = 356,
     FLOOR_TYPE = 357,
     FLOOR_TYPE_ID = 358,
     DECORATION_ID = 359,
     DECORATION_TYPE = 360,
     DECORATION_DIR = 361,
     DECORATION_ITEM_STATE = 362,
     ELEMENTAL_ENCHANTMENT_TYPE = 363,
     EXCEPTIONALITY_TYPE = 364,
     EXCEPTIONALITY_ID = 365,
     ELEMENTAL_ENCHANTMENT_ID = 366,
     ENCHANTMENT_ID = 367,
     SECRET_DOOR_ID = 368,
     USES_UP_KEY_ID = 369,
     MYTHIC_PREFIX_TYPE = 370,
     MYTHIC_SUFFIX_TYPE = 371,
     MYTHIC_PREFIX_ID = 372,
     MYTHIC_SUFFIX_ID = 373,
     MATERIAL_ID = 374,
     MATERIAL_TYPE = 375,
     CHARGES_ID = 376,
     SPECIAL_QUALITY_ID = 377,
     SPEFLAGS_ID = 378,
     SUBROOM_ID = 379,
     NAME_ID = 380,
     FLAGS_ID = 381,
     FLAG_TYPE = 382,
     MON_ATTITUDE = 383,
     MON_ALERTNESS = 384,
     SUBTYPE_ID = 385,
     NON_PASSDOOR_ID = 386,
     CARPET_ID = 387,
     CARPET_PIECE_ID = 388,
     CARPET_TYPE = 389,
     MON_APPEARANCE = 390,
     ROOMDOOR_ID = 391,
     IF_ID = 392,
     ELSE_ID = 393,
     TERRAIN_ID = 394,
     HORIZ_OR_VERT = 395,
     REPLACE_TERRAIN_ID = 396,
     LOCATION_SUBTYPE_ID = 397,
     DOOR_SUBTYPE = 398,
     BRAZIER_SUBTYPE = 399,
     SIGNPOST_SUBTYPE = 400,
     TREE_SUBTYPE = 401,
     FOREST_ID = 402,
     FOREST_TYPE = 403,
     INITIALIZE_TYPE = 404,
     EXIT_ID = 405,
     SHUFFLE_ID = 406,
     MANUAL_TYPE_ID = 407,
     MANUAL_TYPE = 408,
     QUANTITY_ID = 409,
     BURIED_ID = 410,
     LOOP_ID = 411,
     FOR_ID = 412,
     TO_ID = 413,
     SWITCH_ID = 414,
     CASE_ID = 415,
     BREAK_ID = 416,
     DEFAULT_ID = 417,
     ERODED_ID = 418,
     TRAPPED_STATE = 419,
     RECHARGED_ID = 420,
     INVIS_ID = 421,
     GREASED_ID = 422,
     INDESTRUCTIBLE_ID = 423,
     FEMALE_ID = 424,
     MALE_ID = 425,
     WAITFORU_ID = 426,
     PROTECTOR_ID = 427,
     CANCELLED_ID = 428,
     REVIVED_ID = 429,
     AVENGE_ID = 430,
     FLEEING_ID = 431,
     BLINDED_ID = 432,
     MAXHP_ID = 433,
     LEVEL_ADJUSTMENT_ID = 434,
     KEEP_ORIGINAL_INVENTORY_ID = 435,
     PARALYZED_ID = 436,
     STUNNED_ID = 437,
     CONFUSED_ID = 438,
     SEENTRAPS_ID = 439,
     ALL_ID = 440,
     MONTYPE_ID = 441,
     OBJTYPE_ID = 442,
     TERTYPE_ID = 443,
     TERTYPE2_ID = 444,
     LEVER_EFFECT_TYPE = 445,
     SWITCHABLE_ID = 446,
     CONTINUOUSLY_USABLE_ID = 447,
     TARGET_ID = 448,
     TRAPTYPE_ID = 449,
     EFFECT_FLAG_ID = 450,
     GRAVE_ID = 451,
     BRAZIER_ID = 452,
     SIGNPOST_ID = 453,
     TREE_ID = 454,
     ERODEPROOF_ID = 455,
     FUNCTION_ID = 456,
     MSG_OUTPUT_TYPE = 457,
     COMPARE_TYPE = 458,
     UNKNOWN_TYPE = 459,
     rect_ID = 460,
     fillrect_ID = 461,
     line_ID = 462,
     randline_ID = 463,
     grow_ID = 464,
     selection_ID = 465,
     flood_ID = 466,
     rndcoord_ID = 467,
     circle_ID = 468,
     ellipse_ID = 469,
     filter_ID = 470,
     complement_ID = 471,
     gradient_ID = 472,
     GRADIENT_TYPE = 473,
     LIMITED = 474,
     HUMIDITY_TYPE = 475,
     STRING = 476,
     MAP_ID = 477,
     NQSTRING = 478,
     VARSTRING = 479,
     CFUNC = 480,
     CFUNC_INT = 481,
     CFUNC_STR = 482,
     CFUNC_COORD = 483,
     CFUNC_REGION = 484,
     VARSTRING_INT = 485,
     VARSTRING_INT_ARRAY = 486,
     VARSTRING_STRING = 487,
     VARSTRING_STRING_ARRAY = 488,
     VARSTRING_VAR = 489,
     VARSTRING_VAR_ARRAY = 490,
     VARSTRING_COORD = 491,
     VARSTRING_COORD_ARRAY = 492,
     VARSTRING_REGION = 493,
     VARSTRING_REGION_ARRAY = 494,
     VARSTRING_MAPCHAR = 495,
     VARSTRING_MAPCHAR_ARRAY = 496,
     VARSTRING_MONST = 497,
     VARSTRING_MONST_ARRAY = 498,
     VARSTRING_OBJ = 499,
     VARSTRING_OBJ_ARRAY = 500,
     VARSTRING_SEL = 501,
     VARSTRING_SEL_ARRAY = 502,
     METHOD_INT = 503,
     METHOD_INT_ARRAY = 504,
     METHOD_STRING = 505,
     METHOD_STRING_ARRAY = 506,
     METHOD_VAR = 507,
     METHOD_VAR_ARRAY = 508,
     METHOD_COORD = 509,
     METHOD_COORD_ARRAY = 510,
     METHOD_REGION = 511,
     METHOD_REGION_ARRAY = 512,
     METHOD_MAPCHAR = 513,
     METHOD_MAPCHAR_ARRAY = 514,
     METHOD_MONST = 515,
     METHOD_MONST_ARRAY = 516,
     METHOD_OBJ = 517,
     METHOD_OBJ_ARRAY = 518,
     METHOD_SEL = 519,
     METHOD_SEL_ARRAY = 520,
     DICE = 521
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
#line 566 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 578 "lev_comp.tab.c"

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
#define YYLAST   1348

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  284
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  192
/* YYNRULES -- Number of rules.  */
#define YYNRULES  530
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1185

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   521

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   279,   283,     2,
     223,   224,   277,   275,   221,   276,   281,   278,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   222,     2,
       2,   280,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   225,     2,   226,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   227,   282,   228,     2,     2,     2,     2,
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
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274
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
     291,   293,   297,   301,   307,   311,   317,   323,   329,   333,
     337,   343,   349,   355,   363,   371,   379,   385,   387,   391,
     393,   397,   399,   403,   405,   409,   411,   415,   417,   421,
     423,   427,   428,   429,   438,   443,   445,   446,   448,   450,
     456,   460,   461,   462,   472,   473,   476,   477,   483,   484,
     489,   491,   494,   496,   503,   504,   508,   509,   516,   517,
     522,   523,   528,   530,   531,   536,   540,   546,   548,   552,
     556,   562,   568,   576,   581,   582,   597,   598,   615,   616,
     619,   625,   627,   633,   635,   641,   643,   649,   651,   662,
     669,   671,   673,   675,   677,   679,   683,   685,   687,   688,
     692,   696,   700,   704,   706,   708,   710,   712,   714,   722,
     728,   730,   732,   734,   736,   740,   741,   747,   752,   753,
     757,   759,   761,   763,   765,   768,   770,   772,   774,   776,
     778,   782,   784,   788,   792,   796,   798,   800,   804,   806,
     808,   810,   814,   816,   818,   820,   824,   828,   829,   835,
     838,   839,   843,   845,   849,   851,   855,   859,   861,   863,
     867,   869,   871,   873,   877,   879,   881,   883,   887,   891,
     895,   899,   903,   907,   911,   915,   917,   919,   921,   923,
     927,   931,   935,   939,   945,   953,   959,   968,   970,   974,
     978,   984,   990,   998,  1006,  1013,  1019,  1020,  1023,  1029,
    1033,  1045,  1059,  1066,  1067,  1071,  1073,  1077,  1081,  1085,
    1089,  1091,  1093,  1097,  1101,  1105,  1109,  1113,  1117,  1119,
    1121,  1123,  1125,  1129,  1133,  1135,  1141,  1151,  1157,  1165,
    1169,  1173,  1179,  1189,  1195,  1201,  1207,  1208,  1223,  1224,
    1226,  1234,  1246,  1256,  1270,  1274,  1282,  1294,  1308,  1316,
    1324,  1330,  1338,  1344,  1354,  1362,  1368,  1378,  1384,  1390,
    1394,  1402,  1406,  1414,  1422,  1426,  1432,  1438,  1442,  1448,
    1456,  1466,  1468,  1470,  1472,  1474,  1476,  1477,  1480,  1482,
    1486,  1488,  1490,  1492,  1493,  1497,  1499,  1500,  1504,  1506,
    1507,  1511,  1512,  1516,  1518,  1520,  1522,  1524,  1526,  1528,
    1530,  1532,  1534,  1538,  1540,  1542,  1547,  1549,  1551,  1556,
    1558,  1560,  1565,  1567,  1572,  1578,  1580,  1584,  1586,  1590,
    1592,  1594,  1599,  1609,  1611,  1613,  1618,  1620,  1626,  1628,
    1630,  1635,  1637,  1639,  1645,  1647,  1649,  1651,  1656,  1658,
    1660,  1666,  1668,  1670,  1672,  1676,  1678,  1680,  1684,  1686,
    1691,  1695,  1699,  1703,  1707,  1711,  1715,  1717,  1719,  1723,
    1725,  1729,  1730,  1732,  1734,  1736,  1738,  1742,  1743,  1745,
    1747,  1750,  1753,  1758,  1765,  1770,  1777,  1784,  1791,  1798,
    1801,  1808,  1817,  1826,  1837,  1852,  1855,  1857,  1861,  1863,
    1867,  1869,  1871,  1873,  1875,  1877,  1879,  1881,  1883,  1885,
    1887,  1889,  1891,  1893,  1895,  1897,  1899,  1901,  1903,  1905,
    1916
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     285,     0,    -1,    -1,   286,    -1,   287,    -1,   287,   286,
      -1,   288,   298,   300,    -1,    17,   222,   229,    -1,    16,
     222,   229,   221,     3,    -1,    18,   222,    11,   221,   398,
      -1,    18,   222,    10,   221,     3,    -1,    18,   222,    13,
      -1,    18,   222,    12,   221,     3,   221,     3,   221,     5,
     221,     5,   221,   437,   221,   297,   296,    -1,    19,   222,
     467,    -1,   147,   222,   148,   221,   149,    -1,   147,   222,
     148,    -1,    31,   222,     4,   221,   310,    -1,    22,   222,
     102,    -1,    -1,   221,   219,    -1,    -1,   221,   444,    -1,
      -1,   221,     3,    -1,     5,    -1,    83,    -1,    -1,   126,
     222,   299,    -1,   127,   221,   299,    -1,   127,    -1,    -1,
     302,   300,    -1,   227,   300,   228,    -1,   343,    -1,   289,
      -1,   290,    -1,   406,    -1,   293,    -1,   291,    -1,   414,
      -1,   415,    -1,   416,    -1,   292,    -1,   413,    -1,   412,
      -1,   410,    -1,   411,    -1,   417,    -1,   418,    -1,   419,
      -1,   420,    -1,   421,    -1,   387,    -1,   345,    -1,   308,
      -1,   307,    -1,   401,    -1,   357,    -1,   379,    -1,   423,
      -1,   424,    -1,   389,    -1,   390,    -1,   391,    -1,   422,
      -1,   323,    -1,   333,    -1,   335,    -1,   339,    -1,   337,
      -1,   320,    -1,   330,    -1,   316,    -1,   319,    -1,   382,
      -1,   392,    -1,   364,    -1,   380,    -1,   367,    -1,   373,
      -1,   402,    -1,   397,    -1,   385,    -1,   344,    -1,   403,
      -1,   404,    -1,   405,    -1,   407,    -1,   350,    -1,   348,
      -1,   396,    -1,   400,    -1,   399,    -1,   383,    -1,   384,
      -1,   386,    -1,   378,    -1,   381,    -1,   239,    -1,   241,
      -1,   243,    -1,   245,    -1,   247,    -1,   249,    -1,   251,
      -1,   253,    -1,   255,    -1,   238,    -1,   240,    -1,   242,
      -1,   244,    -1,   246,    -1,   248,    -1,   250,    -1,   252,
      -1,   254,    -1,   303,    -1,   304,    -1,   232,    -1,   232,
      -1,   304,    -1,   151,   222,   303,    -1,   305,   280,   456,
      -1,   305,   280,   210,   222,   465,    -1,   305,   280,   455,
      -1,   305,   280,   472,   222,   449,    -1,   305,   280,   471,
     222,   451,    -1,   305,   280,   470,   222,   453,    -1,   305,
     280,   444,    -1,   305,   280,   447,    -1,   305,   280,   227,
     314,   228,    -1,   305,   280,   227,   313,   228,    -1,   305,
     280,   227,   312,   228,    -1,   305,   280,   472,   222,   227,
     311,   228,    -1,   305,   280,   471,   222,   227,   310,   228,
      -1,   305,   280,   470,   222,   227,   309,   228,    -1,   305,
     280,   227,   315,   228,    -1,   454,    -1,   309,   221,   454,
      -1,   452,    -1,   310,   221,   452,    -1,   450,    -1,   311,
     221,   450,    -1,   448,    -1,   312,   221,   448,    -1,   445,
      -1,   313,   221,   445,    -1,   456,    -1,   314,   221,   456,
      -1,   455,    -1,   315,   221,   455,    -1,    -1,    -1,   201,
     231,   223,   317,   460,   224,   318,   301,    -1,   231,   223,
     463,   224,    -1,   150,    -1,    -1,     6,    -1,     6,    -1,
     225,   456,   203,   456,   226,    -1,   225,   456,   226,    -1,
      -1,    -1,   159,   324,   225,   443,   226,   325,   227,   326,
     228,    -1,    -1,   327,   326,    -1,    -1,   160,   468,   222,
     328,   300,    -1,    -1,   162,   222,   329,   300,    -1,   161,
      -1,   281,   281,    -1,   158,    -1,   157,   306,   280,   456,
     331,   456,    -1,    -1,   332,   334,   301,    -1,    -1,   156,
     225,   443,   226,   336,   301,    -1,    -1,   322,   222,   338,
     302,    -1,    -1,   137,   322,   340,   341,    -1,   301,    -1,
      -1,   301,   342,   138,   301,    -1,    14,   222,   455,    -1,
      14,   222,   455,   221,    15,    -1,    73,    -1,    73,   222,
     468,    -1,    73,   222,    83,    -1,    61,   222,   346,   221,
     346,    -1,    61,   222,   346,   221,   468,    -1,   223,     4,
     221,    82,   221,   361,   224,    -1,   426,   321,   221,   437,
      -1,    -1,   124,   222,   347,   221,   354,   221,   356,   427,
     430,   432,   435,   434,   349,   301,    -1,    -1,    53,   222,
     347,   221,   353,   221,   355,   221,   356,   427,   430,   432,
     435,   434,   351,   301,    -1,    -1,   221,     5,    -1,   223,
       4,   221,     4,   224,    -1,    83,    -1,   223,     4,   221,
       4,   224,    -1,    83,    -1,   223,   365,   221,   366,   224,
      -1,    83,    -1,   223,     4,   221,     4,   224,    -1,    83,
      -1,   136,   222,   358,   221,   436,   221,   359,   221,   361,
     362,    -1,    29,   222,   436,   221,   465,   362,    -1,     5,
      -1,    83,    -1,   360,    -1,    83,    -1,    82,    -1,    82,
     282,   360,    -1,     4,    -1,    83,    -1,    -1,   362,   221,
     363,    -1,   142,   222,   143,    -1,    79,   222,   453,    -1,
     122,   222,     4,    -1,   168,    -1,   113,    -1,   114,    -1,
     131,    -1,    21,    -1,    20,   222,   365,   221,   366,   352,
     230,    -1,    20,   222,   444,   352,   230,    -1,    87,    -1,
      88,    -1,    89,    -1,    88,    -1,    27,   222,   369,    -1,
      -1,    27,   222,   369,   368,   301,    -1,   451,   221,   444,
     370,    -1,    -1,   370,   221,   371,    -1,   455,    -1,   128,
      -1,   129,    -1,   439,    -1,   135,   455,    -1,   169,    -1,
     170,    -1,   166,    -1,   173,    -1,   174,    -1,   174,   222,
     443,    -1,   175,    -1,   176,   222,   443,    -1,   177,   222,
     443,    -1,   181,   222,   443,    -1,   182,    -1,   183,    -1,
     184,   222,   372,    -1,   171,    -1,   172,    -1,   178,    -1,
     179,   222,   443,    -1,   180,    -1,   229,    -1,   185,    -1,
     229,   282,   372,    -1,    25,   222,   375,    -1,    -1,    26,
     222,   375,   374,   301,    -1,   453,   376,    -1,    -1,   376,
     221,   377,    -1,    76,    -1,   186,   222,   451,    -1,   469,
      -1,   125,   222,   455,    -1,   154,   222,   443,    -1,   155,
      -1,    75,    -1,   163,   222,   443,    -1,   200,    -1,    74,
      -1,   164,    -1,   165,   222,   443,    -1,   166,    -1,   167,
      -1,   444,    -1,   111,   222,   108,    -1,   110,   222,   109,
      -1,   112,   222,   443,    -1,   121,   222,   443,    -1,   122,
     222,   443,    -1,   152,   222,   153,    -1,   123,   222,   443,
      -1,    79,   222,   453,    -1,   168,    -1,   114,    -1,    81,
      -1,    77,    -1,   117,   222,   115,    -1,   118,   222,   116,
      -1,    60,   222,   443,    -1,   119,   222,   120,    -1,    28,
     222,   425,   221,   444,    -1,    30,   222,   444,   221,    82,
     221,   436,    -1,    35,   222,   444,   221,    82,    -1,    35,
     222,   444,   221,    82,   221,     5,   296,    -1,    36,    -1,
      36,   222,   465,    -1,    36,   222,   185,    -1,    49,   222,
     444,   221,    92,    -1,    50,   222,   444,   221,    92,    -1,
      50,   222,   474,   221,   474,   221,    92,    -1,    55,   222,
     474,   221,   474,   221,   229,    -1,    56,   222,   474,   221,
     474,   388,    -1,    57,   222,   474,   221,   474,    -1,    -1,
     221,    92,    -1,    64,   222,   465,   221,    96,    -1,    65,
     222,   465,    -1,    66,   222,   444,   221,   193,   222,   444,
     221,    94,   221,    93,    -1,    66,   222,   444,   221,    67,
     221,    68,   221,    69,   221,    94,   221,    93,    -1,    80,
     222,   444,   221,   190,   393,    -1,    -1,   393,   221,   394,
      -1,    93,    -1,   186,   222,   451,    -1,   187,   222,   453,
      -1,   188,   222,   398,    -1,   189,   222,   398,    -1,   191,
      -1,   192,    -1,   193,   222,   444,    -1,   194,   222,   425,
      -1,   142,   222,   395,    -1,   100,   222,   433,    -1,   195,
     222,     4,    -1,   122,   222,     4,    -1,     4,    -1,    94,
      -1,   143,    -1,    96,    -1,    71,   222,   465,    -1,    70,
     222,   465,    -1,     3,    -1,   223,     3,   221,   437,   224,
      -1,   141,   222,   447,   221,   449,   221,   449,   221,     7,
      -1,   139,   222,   465,   221,   449,    -1,   139,   222,   465,
     221,   449,   221,    99,    -1,    51,   222,   447,    -1,    52,
     222,   447,    -1,    41,   222,   229,   221,    42,    -1,    41,
     222,   229,   221,    42,   221,   186,   222,   451,    -1,    38,
     222,   447,   221,    40,    -1,    39,   222,   474,   221,    40,
      -1,    23,   222,   465,   221,    98,    -1,    -1,    37,   222,
     447,   221,   437,   221,   426,   427,   430,   432,   435,   434,
     408,   409,    -1,    -1,   301,    -1,    46,   222,   444,   221,
     438,   221,   440,    -1,    46,   222,   444,   221,   438,   221,
     440,   221,   186,   222,   451,    -1,    46,   222,   444,   221,
     438,   221,   440,   221,    91,    -1,    46,   222,   444,   221,
     438,   221,   440,   221,    91,   221,   186,   222,   451,    -1,
      47,   222,   444,    -1,    47,   222,   444,   221,   186,   222,
     451,    -1,   104,   222,   444,   221,   105,   221,     4,   221,
     106,   221,   107,    -1,   104,   222,   444,   221,   105,   221,
       4,   221,   106,   221,   107,   221,   437,    -1,   104,   222,
     444,   221,   105,   221,   106,    -1,   101,   222,   465,   221,
     102,   221,    99,    -1,   130,   222,   465,   221,     4,    -1,
     130,   222,   465,   221,   102,   221,    99,    -1,   132,   222,
     447,   221,   134,    -1,   133,   222,   465,   221,   134,   221,
       4,   221,     4,    -1,   133,   222,   465,   221,   134,   221,
       4,    -1,    48,   222,    95,   221,   444,    -1,    48,   222,
      95,   221,   444,   221,   186,   222,   451,    -1,   196,   222,
     444,   221,   455,    -1,   196,   222,   444,   221,    83,    -1,
     196,   222,   444,    -1,   197,   222,   444,   221,   144,   221,
     437,    -1,   197,   222,   444,    -1,   198,   222,   444,   221,
     145,   221,   455,    -1,   198,   222,   444,   221,   145,   221,
      83,    -1,   198,   222,   444,    -1,   199,   222,   444,   221,
     146,    -1,   199,   222,   444,   221,   148,    -1,   199,   222,
     444,    -1,    62,   222,   456,   221,   444,    -1,    63,   222,
     444,   221,   473,   221,   455,    -1,    59,   222,   443,   221,
     443,   221,   443,   221,   443,    -1,    59,    -1,   229,    -1,
      83,    -1,   229,    -1,    83,    -1,    -1,   221,   428,    -1,
     429,    -1,   429,   221,   428,    -1,    43,    -1,    44,    -1,
      45,    -1,    -1,   103,   222,   431,    -1,   102,    -1,    -1,
     100,   222,   433,    -1,    99,    -1,    -1,   186,   222,   451,
      -1,    -1,    24,   222,    98,    -1,    74,    -1,    83,    -1,
      75,    -1,    83,    -1,    86,    -1,   441,    -1,    83,    -1,
      86,    -1,   441,    -1,    85,   222,    83,    -1,    90,    -1,
      83,    -1,    85,   225,     4,   226,    -1,   229,    -1,   240,
      -1,   241,   225,   456,   226,    -1,   456,    -1,   445,    -1,
     212,   223,   465,   224,    -1,   244,    -1,   245,   225,   456,
     226,    -1,   223,     4,   221,     4,   224,    -1,    83,    -1,
      84,   446,   226,    -1,   220,    -1,   220,   221,   446,    -1,
     448,    -1,   246,    -1,   247,   225,   456,   226,    -1,   223,
       4,   221,     4,   221,     4,   221,     4,   224,    -1,   450,
      -1,   248,    -1,   249,   225,   456,   226,    -1,     3,    -1,
     223,     3,   221,   437,   224,    -1,   452,    -1,   250,    -1,
     251,   225,   456,   226,    -1,   229,    -1,     3,    -1,   223,
       3,   221,   229,   224,    -1,    83,    -1,   454,    -1,   252,
      -1,   253,   225,   456,   226,    -1,   229,    -1,     3,    -1,
     223,     3,   221,   229,   224,    -1,    83,    -1,    97,    -1,
     442,    -1,   455,   281,   442,    -1,     4,    -1,   466,    -1,
     223,     8,   224,    -1,   238,    -1,   239,   225,   456,   226,
      -1,   456,   275,   456,    -1,   456,   276,   456,    -1,   456,
     277,   456,    -1,   456,   278,   456,    -1,   456,   279,   456,
      -1,   223,   456,   224,    -1,   234,    -1,   235,    -1,   305,
     222,   457,    -1,   458,    -1,   459,   221,   458,    -1,    -1,
     459,    -1,   456,    -1,   455,    -1,   461,    -1,   462,   221,
     461,    -1,    -1,   462,    -1,   444,    -1,   205,   447,    -1,
     206,   447,    -1,   207,   444,   221,   444,    -1,   208,   444,
     221,   444,   221,   456,    -1,   209,   223,   465,   224,    -1,
     209,   223,   360,   221,   465,   224,    -1,   215,   223,     7,
     221,   465,   224,    -1,   215,   223,   465,   221,   465,   224,
      -1,   215,   223,   449,   221,   465,   224,    -1,   211,   444,
      -1,   213,   223,   444,   221,   456,   224,    -1,   213,   223,
     444,   221,   456,   221,    43,   224,    -1,   214,   223,   444,
     221,   456,   221,   456,   224,    -1,   214,   223,   444,   221,
     456,   221,   456,   221,    43,   224,    -1,   217,   223,   218,
     221,   223,   456,   276,   456,   294,   224,   221,   444,   295,
     224,    -1,   216,   464,    -1,   254,    -1,   223,   465,   224,
      -1,   464,    -1,   464,   283,   465,    -1,   274,    -1,    98,
      -1,     4,    -1,     8,    -1,     9,    -1,     4,    -1,     8,
      -1,     9,    -1,     4,    -1,   466,    -1,    32,    -1,    25,
      -1,    33,    -1,    27,    -1,    34,    -1,   139,    -1,    78,
      -1,    83,    -1,   475,    -1,    58,   223,     4,   221,     4,
     221,     4,   221,     4,   224,    -1,   223,     4,   221,     4,
     221,     4,   221,     4,   224,    -1
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
     563,   564,   565,   566,   567,   568,   569,   570,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   584,   585,   586,
     587,   588,   589,   590,   591,   592,   595,   596,   597,   600,
     601,   604,   620,   626,   632,   638,   644,   650,   656,   662,
     668,   678,   688,   698,   708,   718,   728,   740,   745,   752,
     757,   764,   769,   776,   780,   786,   791,   798,   802,   808,
     812,   819,   841,   818,   855,   910,   917,   920,   926,   933,
     937,   946,   950,   945,  1013,  1014,  1018,  1017,  1031,  1030,
    1045,  1055,  1056,  1059,  1097,  1096,  1131,  1130,  1161,  1160,
    1193,  1192,  1218,  1229,  1228,  1256,  1260,  1265,  1270,  1275,
    1282,  1289,  1298,  1306,  1318,  1317,  1342,  1341,  1366,  1369,
    1375,  1385,  1391,  1400,  1406,  1411,  1417,  1422,  1428,  1439,
    1445,  1446,  1449,  1450,  1453,  1457,  1463,  1464,  1468,  1474,
    1482,  1487,  1492,  1497,  1502,  1507,  1512,  1520,  1527,  1535,
    1543,  1544,  1547,  1548,  1551,  1556,  1555,  1569,  1576,  1583,
    1591,  1596,  1602,  1608,  1614,  1620,  1625,  1630,  1635,  1640,
    1645,  1650,  1655,  1660,  1665,  1670,  1675,  1680,  1686,  1691,
    1696,  1701,  1706,  1713,  1722,  1726,  1739,  1748,  1747,  1765,
    1775,  1781,  1789,  1795,  1800,  1805,  1810,  1815,  1820,  1825,
    1830,  1835,  1849,  1855,  1860,  1865,  1870,  1875,  1880,  1885,
    1890,  1895,  1900,  1905,  1910,  1915,  1920,  1925,  1930,  1936,
    1941,  1946,  1951,  1958,  1964,  1993,  1998,  2006,  2012,  2016,
    2024,  2031,  2038,  2048,  2058,  2074,  2085,  2088,  2094,  2100,
    2106,  2110,  2116,  2123,  2129,  2137,  2143,  2148,  2153,  2158,
    2163,  2169,  2175,  2180,  2185,  2190,  2195,  2200,  2207,  2207,
    2207,  2207,  2210,  2216,  2222,  2227,  2234,  2241,  2245,  2251,
    2257,  2263,  2268,  2275,  2281,  2291,  2298,  2297,  2330,  2333,
    2339,  2344,  2349,  2354,  2360,  2364,  2370,  2374,  2378,  2384,
    2390,  2394,  2400,  2406,  2410,  2416,  2420,  2426,  2430,  2435,
    2442,  2446,  2453,  2457,  2462,  2469,  2473,  2478,  2486,  2492,
    2499,  2503,  2510,  2518,  2521,  2531,  2535,  2538,  2544,  2548,
    2555,  2559,  2563,  2570,  2573,  2579,  2586,  2589,  2595,  2602,
    2606,  2613,  2616,  2623,  2624,  2627,  2628,  2631,  2632,  2633,
    2639,  2640,  2641,  2647,  2648,  2651,  2660,  2665,  2672,  2683,
    2689,  2693,  2697,  2704,  2714,  2721,  2725,  2731,  2735,  2743,
    2747,  2754,  2764,  2777,  2781,  2788,  2798,  2807,  2818,  2822,
    2829,  2839,  2850,  2859,  2869,  2875,  2879,  2886,  2896,  2907,
    2916,  2926,  2930,  2937,  2938,  2944,  2948,  2952,  2956,  2964,
    2973,  2977,  2981,  2985,  2989,  2993,  2996,  3003,  3012,  3045,
    3046,  3049,  3050,  3053,  3057,  3064,  3071,  3082,  3085,  3093,
    3097,  3101,  3105,  3109,  3114,  3118,  3122,  3127,  3132,  3137,
    3141,  3146,  3151,  3155,  3159,  3164,  3168,  3175,  3181,  3185,
    3191,  3198,  3199,  3202,  3203,  3204,  3207,  3211,  3215,  3219,
    3225,  3226,  3229,  3230,  3233,  3234,  3237,  3238,  3241,  3245,
    3271
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CHAR", "INTEGER", "BOOLEAN", "PERCENT",
  "SPERCENT", "MINUS_INTEGER", "PLUS_INTEGER", "MAZE_GRID_ID",
  "SOLID_FILL_ID", "MINES_ID", "ROGUELEV_ID", "MESSAGE_ID", "MESSAGE_TYPE",
  "MAZE_ID", "LEVEL_ID", "LEV_INIT_ID", "TILESET_ID", "GEOMETRY_ID",
  "NOMAP_ID", "BOUNDARY_TYPE_ID", "SPECIAL_TILESET_ID", "TILESET_PARAM_ID",
  "OBJECT_ID", "COBJECT_ID", "MONSTER_ID", "TRAP_ID", "DOOR_ID",
  "DRAWBRIDGE_ID", "MONSTER_GENERATION_ID", "object_ID", "monster_ID",
  "terrain_ID", "MAZEWALK_ID", "WALLIFY_ID", "REGION_ID",
  "SPECIAL_REGION_ID", "SPECIAL_LEVREGION_ID", "SPECIAL_REGION_TYPE",
  "NAMING_ID", "NAMING_TYPE", "FILLING", "IRREGULAR", "JOINED", "ALTAR_ID",
  "ANVIL_ID", "NPC_ID", "LADDER_ID", "STAIR_ID", "NON_DIGGABLE_ID",
  "NON_PASSWALL_ID", "ROOM_ID", "ARTIFACT_NAME_ID", "PORTAL_ID",
  "TELEPRT_ID", "BRANCH_ID", "LEV", "MINERALIZE_ID", "AGE_ID",
  "CORRIDOR_ID", "GOLD_ID", "ENGRAVING_ID", "FOUNTAIN_ID", "THRONE_ID",
  "MODRON_PORTAL_ID", "LEVEL_TELEPORTER_ID",
  "LEVEL_TELEPORT_DIRECTION_TYPE", "LEVEL_TELEPORT_END_TYPE", "POOL_ID",
  "SINK_ID", "NONE", "RAND_CORRIDOR_ID", "DOOR_STATE", "LIGHT_STATE",
  "CURSE_TYPE", "MYTHIC_TYPE", "ENGRAVING_TYPE", "KEYTYPE_ID", "LEVER_ID",
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
  "MYTHIC_SUFFIX_ID", "MATERIAL_ID", "MATERIAL_TYPE", "CHARGES_ID",
  "SPECIAL_QUALITY_ID", "SPEFLAGS_ID", "SUBROOM_ID", "NAME_ID", "FLAGS_ID",
  "FLAG_TYPE", "MON_ATTITUDE", "MON_ALERTNESS", "SUBTYPE_ID",
  "NON_PASSDOOR_ID", "CARPET_ID", "CARPET_PIECE_ID", "CARPET_TYPE",
  "MON_APPEARANCE", "ROOMDOOR_ID", "IF_ID", "ELSE_ID", "TERRAIN_ID",
  "HORIZ_OR_VERT", "REPLACE_TERRAIN_ID", "LOCATION_SUBTYPE_ID",
  "DOOR_SUBTYPE", "BRAZIER_SUBTYPE", "SIGNPOST_SUBTYPE", "TREE_SUBTYPE",
  "FOREST_ID", "FOREST_TYPE", "INITIALIZE_TYPE", "EXIT_ID", "SHUFFLE_ID",
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
  "subtype_detail", "carpet_detail", "carpet_piece_detail", "npc_detail",
  "grave_detail", "brazier_detail", "signpost_detail", "tree_detail",
  "gold_detail", "engraving_detail", "mineralize", "trap_name",
  "room_type", "optroomregionflags", "roomregionflags", "roomregionflag",
  "optfloortype", "floortype", "optfloorsubtype", "floorsubtype",
  "optmontype", "opttileset", "door_state", "light_state", "alignment",
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
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,    44,    58,    40,    41,    91,    93,   123,   125,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,    43,    45,    42,    47,    37,
      61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   284,   285,   285,   286,   286,   287,   288,   288,   289,
     289,   289,   289,   290,   291,   291,   292,   293,   294,   294,
     295,   295,   296,   296,   297,   297,   298,   298,   299,   299,
     300,   300,   301,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   305,   305,   305,   306,
     306,   307,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   309,   309,   310,
     310,   311,   311,   312,   312,   313,   313,   314,   314,   315,
     315,   317,   318,   316,   319,   320,   321,   321,   322,   322,
     322,   324,   325,   323,   326,   326,   328,   327,   329,   327,
     330,   331,   331,   332,   334,   333,   336,   335,   338,   337,
     340,   339,   341,   342,   341,   343,   343,   344,   344,   344,
     345,   345,   346,   347,   349,   348,   351,   350,   352,   352,
     353,   353,   354,   354,   355,   355,   356,   356,   357,   357,
     358,   358,   359,   359,   360,   360,   361,   361,   362,   362,
     363,   363,   363,   363,   363,   363,   363,   364,   364,   364,
     365,   365,   366,   366,   367,   368,   367,   369,   370,   370,
     371,   371,   371,   371,   371,   371,   371,   371,   371,   371,
     371,   371,   371,   371,   371,   371,   371,   371,   371,   371,
     371,   371,   371,   372,   372,   372,   373,   374,   373,   375,
     376,   376,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   378,   379,   380,   380,   381,   381,   381,
     382,   383,   384,   385,   386,   387,   388,   388,   389,   390,
     391,   391,   392,   393,   393,   394,   394,   394,   394,   394,
     394,   394,   394,   394,   394,   394,   394,   394,   395,   395,
     395,   395,   396,   397,   398,   398,   399,   400,   400,   401,
     402,   403,   403,   404,   405,   406,   408,   407,   409,   409,
     410,   410,   410,   410,   411,   411,   412,   412,   412,   413,
     414,   414,   415,   416,   416,   417,   417,   418,   418,   418,
     419,   419,   420,   420,   420,   421,   421,   421,   422,   423,
     424,   424,   425,   425,   426,   426,   427,   427,   428,   428,
     429,   429,   429,   430,   430,   431,   432,   432,   433,   434,
     434,   435,   435,   436,   436,   437,   437,   438,   438,   438,
     439,   439,   439,   440,   440,   441,   442,   442,   442,   443,
     444,   444,   444,   444,   445,   445,   445,   446,   446,   447,
     447,   447,   448,   449,   449,   449,   450,   450,   451,   451,
     451,   452,   452,   452,   452,   453,   453,   453,   454,   454,
     454,   454,   454,   455,   455,   456,   456,   456,   456,   456,
     456,   456,   456,   456,   456,   456,   457,   457,   458,   459,
     459,   460,   460,   461,   461,   462,   462,   463,   463,   464,
     464,   464,   464,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   464,   464,   464,   465,   465,
     466,   467,   467,   468,   468,   468,   469,   469,   469,   469,
     470,   470,   471,   471,   472,   472,   473,   473,   474,   474,
     475
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     5,     3,     5,     5,     5,     3,     3,
       5,     5,     5,     7,     7,     7,     5,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     0,     0,     8,     4,     1,     0,     1,     1,     5,
       3,     0,     0,     9,     0,     2,     0,     5,     0,     4,
       1,     2,     1,     6,     0,     3,     0,     6,     0,     4,
       0,     4,     1,     0,     4,     3,     5,     1,     3,     3,
       5,     5,     7,     4,     0,    14,     0,    16,     0,     2,
       5,     1,     5,     1,     5,     1,     5,     1,    10,     6,
       1,     1,     1,     1,     1,     3,     1,     1,     0,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     7,     5,
       1,     1,     1,     1,     3,     0,     5,     4,     0,     3,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       3,     1,     3,     3,     3,     1,     1,     3,     1,     1,
       1,     3,     1,     1,     1,     3,     3,     0,     5,     2,
       0,     3,     1,     3,     1,     3,     3,     1,     1,     3,
       1,     1,     1,     3,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     3,
       3,     3,     3,     5,     7,     5,     8,     1,     3,     3,
       5,     5,     7,     7,     6,     5,     0,     2,     5,     3,
      11,    13,     6,     0,     3,     1,     3,     3,     3,     3,
       1,     1,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     3,     3,     1,     5,     9,     5,     7,     3,
       3,     5,     9,     5,     5,     5,     0,    14,     0,     1,
       7,    11,     9,    13,     3,     7,    11,    13,     7,     7,
       5,     7,     5,     9,     7,     5,     9,     5,     5,     3,
       7,     3,     7,     7,     3,     5,     5,     3,     5,     7,
       9,     1,     1,     1,     1,     1,     0,     2,     1,     3,
       1,     1,     1,     0,     3,     1,     0,     3,     1,     0,
       3,     0,     3,     1,     1,     1,     1,     1,     1,     1,
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
       5,     0,    30,     0,     7,     0,   158,     0,     0,     0,
       0,   227,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   307,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   391,     0,
       0,     0,     0,     0,     0,     0,     0,   187,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     155,     0,     0,     0,   161,   170,     0,     0,     0,     0,
       0,     0,     0,   118,   107,    98,   108,    99,   109,   100,
     110,   101,   111,   102,   112,   103,   113,   104,   114,   105,
     115,   106,    34,    35,    38,    42,    37,     6,    30,   116,
     117,     0,    55,    54,    72,    73,    70,     0,    65,    71,
     174,    66,    67,    69,    68,    33,    83,    53,    89,    88,
      57,    76,    78,    79,    96,    58,    77,    97,    74,    93,
      94,    82,    95,    52,    61,    62,    63,    75,    90,    81,
      92,    91,    56,    80,    84,    85,    86,    36,    87,    45,
      46,    44,    43,    39,    40,    41,    47,    48,    49,    50,
      51,    64,    59,    60,     0,    29,    27,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   180,     0,     0,     0,     0,
       0,   119,   120,     0,     0,     0,     0,     0,     0,     0,
     465,     0,   468,     0,   510,     0,   466,   487,    31,     0,
     178,     0,     8,     0,   426,   427,     0,   463,   185,     0,
       0,     0,    11,   512,   511,    13,   435,     0,   230,   231,
       0,     0,   432,     0,     0,   198,   430,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     506,   489,   508,     0,   459,   461,   462,     0,   458,   456,
       0,   266,   270,   455,   267,   452,   454,     0,   451,   449,
       0,   234,     0,   448,   393,   392,     0,   413,   414,     0,
       0,     0,     0,   309,   308,     0,   440,     0,     0,   439,
       0,     0,     0,     0,   528,     0,     0,   364,     0,     0,
       0,     0,     0,   349,   350,   395,   394,     0,   156,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,   319,
       0,   343,   342,   515,   513,   514,   189,   188,     0,     0,
       0,     0,     0,     0,     0,   210,   211,     0,     0,     0,
       0,    15,   121,     0,     0,     0,   379,   381,   384,   387,
     151,     0,     0,     0,     0,   160,     0,     0,     0,     0,
       0,   484,   483,   485,   488,     0,   521,   523,   520,   522,
     524,   525,     0,     0,     0,   128,   129,   124,   122,     0,
       0,     0,     0,    30,   175,    28,     0,     0,     0,     0,
       0,     0,   437,     0,     0,     0,     0,     0,     0,     0,
     490,   491,     0,     0,     0,   499,     0,     0,     0,   505,
       0,     0,     0,     0,     0,     0,   269,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   157,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   182,   181,     0,     0,     0,   176,     0,
       0,     0,     0,     0,     0,   481,   467,   475,     0,     0,
     470,   471,   472,   473,   474,     0,   154,     0,   465,     0,
       0,     0,     0,   145,   143,   149,   147,     0,     0,     0,
     179,     0,     0,   186,   464,    10,   344,     0,     9,     0,
       0,   436,     0,     0,     0,   233,   232,   198,   199,   229,
       0,     0,   214,     0,     0,     0,     0,   446,     0,     0,
     444,     0,     0,   443,     0,     0,   507,   509,   355,     0,
       0,     0,   268,     0,     0,   236,   238,   303,   218,     0,
      16,   139,   305,     0,     0,   415,   416,     0,   353,     0,
       0,   354,   351,   419,     0,   417,     0,   418,     0,   375,
     310,     0,   311,     0,   201,     0,     0,     0,     0,   316,
     315,     0,     0,   190,   191,   388,   526,   527,     0,   318,
       0,     0,   323,     0,     0,   203,     0,     0,   370,     0,
     372,     0,     0,     0,     0,   347,     0,    14,     0,   172,
       0,     0,   162,   378,   377,     0,     0,   385,   386,     0,
     479,   482,     0,   469,   159,   486,   123,     0,     0,   132,
       0,   131,     0,   130,     0,   136,     0,   127,     0,   126,
       0,   125,    32,   428,     0,     0,   438,   431,     0,   433,
       0,   492,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   457,   518,   516,   517,     0,
     281,   278,   272,   298,     0,   297,     0,     0,     0,   296,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,   282,     0,   284,   285,   295,     0,   280,   271,   286,
     519,   274,     0,   450,   237,   209,     0,     0,     0,     0,
     441,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   193,     0,     0,   314,     0,     0,     0,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   177,   171,   173,     0,     0,     0,     0,
       0,   152,     0,   144,   146,   148,   150,     0,   137,     0,
       0,   141,     0,     0,   434,   228,     0,   215,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   460,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   453,     0,     0,   304,   140,
      22,     0,   396,     0,     0,     0,     0,   424,   423,   360,
     365,     0,   312,     0,   205,     0,     0,   313,   317,     0,
       0,   389,     0,     0,     0,   369,     0,   368,     0,   207,
       0,   396,   371,   374,   213,     0,   212,   184,   348,     0,
     164,   380,   383,   382,   476,   477,   478,   480,     0,     0,
     135,   134,     0,   133,     0,     0,   493,   495,     0,   500,
       0,   496,     0,   445,   498,   497,     0,   301,   294,   288,
     287,   289,   299,   300,   302,   290,   291,   293,   275,   292,
     276,   279,   283,   273,     0,   420,   241,   242,     0,   247,
     245,   246,   258,   259,   248,   249,   251,     0,     0,   260,
       0,   262,     0,   255,   256,     0,   239,   243,   421,   240,
       0,   224,   225,     0,   226,     0,   223,   219,     0,   306,
       0,     0,   403,     0,     0,     0,   425,     0,     0,     0,
       0,     0,     0,   216,   217,     0,     0,     0,   325,     0,
       0,     0,     0,     0,     0,     0,   330,   331,     0,     0,
       0,   324,     0,     0,     0,   403,     0,     0,     0,     0,
       0,     0,   164,   153,   138,   142,   345,     0,     0,     0,
     447,     0,     0,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,     0,   400,   401,   402,   397,   398,
       0,   406,     0,     0,   352,   362,     0,   376,   200,     0,
     396,   390,   192,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   202,     0,   406,   373,
     218,   346,     0,   168,   163,   165,     0,   501,     0,   502,
     471,   422,   250,   252,   253,   261,   254,   264,   263,   257,
     221,   222,   220,     0,     0,     0,     0,   411,     0,     0,
       0,     0,     0,   403,     0,     0,   408,   335,   337,   338,
     339,   341,   340,   334,   326,   327,   328,   329,   332,   333,
     336,     0,     0,   411,   208,   166,    30,     0,     0,     0,
       0,     0,   442,   399,   405,   404,     0,     0,   409,     0,
     530,     0,   361,   204,   406,     0,   320,   366,   206,   409,
      30,   169,     0,   503,    19,     0,   265,   407,     0,     0,
     356,   529,     0,   411,     0,     0,   194,   167,     0,     0,
     412,     0,   358,   363,   409,   321,   367,     0,     0,    20,
     410,   359,   357,   196,   195,    24,    25,    22,     0,     0,
       0,    12,    21,   504,   197
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,   102,   103,   104,   105,   106,
    1120,  1179,   959,  1177,    12,   176,   107,   424,   108,   109,
     110,   111,   233,   112,   113,   797,   590,   800,   529,   530,
     531,   532,   114,   515,   888,   115,   116,   484,   117,   118,
     234,   786,  1001,  1002,  1140,  1116,   119,   651,   120,   121,
     251,   122,   648,   123,   422,   124,   378,   504,   643,   125,
     126,   127,   355,   347,   128,  1167,   129,  1180,   439,   616,
     637,   856,   871,   130,   377,   875,   563,   975,   745,   957,
     131,   274,   557,   132,   460,   311,   744,   946,  1079,   133,
     457,   301,   456,   738,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   766,   144,   145,   146,   147,   772,
     991,  1103,   148,   149,   548,   150,   151,   152,   153,   154,
     155,   156,   157,   158,  1162,  1172,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   316,   348,   962,  1028,  1029,  1031,  1125,  1087,  1097,
    1150,  1128,   319,   597,   606,   947,   849,   607,   257,   352,
     291,   276,   433,   328,   329,   572,   573,   312,   313,   302,
     303,   401,   353,   886,   660,   661,   662,   403,   404,   405,
     292,   451,   246,   265,   367,   741,   419,   420,   421,   628,
     333,   334
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1043
static const yytype_int16 yypact[] =
{
     174,  -145,  -122,   105, -1043,   174,    10,   -67,   -43, -1043,
   -1043,   -80,   813,   -16, -1043,    84, -1043,    -8,    31,    74,
      92, -1043,   130,   149,   156,   167,   169,   171,   175,   181,
     194,   205,   207,   216,   224,   233,   275,   277,   278,   280,
     282,   283,   288,   289,   291,   299,   300,   306,   307,   324,
     326,   328,   330,   331,   334,   338,   339,   345,   359,   364,
     371,   377,   381,   383,   384,   385,    22,   394,   397,   403,
   -1043,   404,     1,   750, -1043, -1043,   411,   421,   425,   438,
      -7,    12,     9, -1043, -1043, -1043, -1043, -1043, -1043, -1043,
   -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043,
   -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043,   813, -1043,
   -1043,   -44, -1043, -1043, -1043, -1043, -1043,   439, -1043, -1043,
   -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043,
   -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043,
   -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043,
   -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043,
   -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043,
   -1043, -1043, -1043, -1043,   231,    17, -1043,   146,   482,    62,
     400,   155,   864,    40,    40,   112,   -21,   106,   -51,   363,
     -51,   558,   201,   201,   -11,    98,   -51,   -51,   239,   -51,
     286,   201,   201,   -20,   -11,   -11,   -11,    12,   119,    12,
     -51,   864,   864,   -51,   864,   864,   121,   -51,   864,   -51,
     -20,   864,   201,   864,    49, -1043,   864,   201,   295,   742,
      12, -1043, -1043,   190,   260,   -51,   -51,   -51,   -51,   285,
   -1043,    90, -1043,   261, -1043,   182, -1043,   116, -1043,   227,
   -1043,   317, -1043,    84, -1043, -1043,   354, -1043,  -146,   374,
     401,   441, -1043, -1043, -1043, -1043, -1043,   444, -1043, -1043,
     436,   662, -1043,   445,   448,   452, -1043, -1043,   201,   201,
     -51,   -51,   451,   -51,   453,   455,   456,   864,   462,   357,
   -1043, -1043,   392,   473, -1043, -1043, -1043,   690, -1043, -1043,
     470, -1043, -1043, -1043, -1043, -1043, -1043,   693, -1043, -1043,
     479,   491,   476, -1043, -1043, -1043,   500, -1043, -1043,   501,
     502,   504,   505, -1043, -1043,   723, -1043,   503,   510, -1043,
     513,   512,   732,   516, -1043,   520,   521,   523,   524,   525,
     743,   528,   529, -1043, -1043, -1043, -1043,   530,   746,   534,
     535,   539,   540,   264,   764,   555,   144,   556,   557, -1043,
     561, -1043, -1043, -1043, -1043, -1043, -1043, -1043,   562,   563,
     564,   566,   567,   568,   569, -1043, -1043,   571,   317,   572,
     573,   574, -1043,   553,    12,    12,   580,   584,   585,   586,
   -1043,   587,   258,    12,    12, -1043,    12,    12,    12,    12,
      12,   527,   264, -1043,   597,   596, -1043, -1043, -1043, -1043,
   -1043, -1043,   599,   120,    35, -1043, -1043,   527,   264,   600,
     601,   603,   813,   813, -1043, -1043,    12,   811,   146,   825,
      16,   826,   609,   611,   864,   625,    12,   177,   842,   626,
   -1043, -1043,   634,   636,   494, -1043,   -51,   -51,   423, -1043,
     649,   657,   864,   784,   664,    12,   666,   317,   667,    12,
     317,   -51,   -51,   864,   808,    96,   809,   671,    12,    20,
     859,   896,   680,   872,   871,   263,   729,   -51,   824,   697,
     827,   -11,   -10, -1043,   704,   -11,   -11,   -11,    12,   706,
      21,   -51,    43,   832,   -15,   744,   834,   839,    -3,    57,
     817,   819,   106,   818, -1043,   150,   150,   806, -1043,   136,
     731,   -23,   814,   816,   -35,   852, -1043, -1043,   437,   619,
     198,   198, -1043, -1043, -1043,   116, -1043,   864,   738,   -90,
     -87,   -46,   -18, -1043, -1043,   527,   264,   124,   145,   161,
   -1043,   734,   627, -1043, -1043, -1043, -1043,   962, -1043,   745,
     444, -1043,   747,   963,   632, -1043, -1043,   452, -1043, -1043,
     -51,   -51,   686,   752,   760,   765,   780, -1043,   782,   475,
   -1043,   781,   786, -1043,   792,   794, -1043, -1043, -1043,   776,
     645,    33, -1043,   787,   654, -1043, -1043, -1043, -1043,   796,
     797, -1043,   798,  1016,   663, -1043, -1043,   800, -1043,   801,
    1019, -1043,   803, -1043,   802, -1043,   804, -1043,   807,   805,
   -1043,  1024, -1043,   810, -1043,  1026,   812,    20,   815,   820,
   -1043,   821,   952, -1043, -1043, -1043, -1043, -1043,   822, -1043,
     828,   863, -1043,   829,   853, -1043,  1031,   865, -1043,   867,
   -1043,   889,   890,   899,  1043,   891,   892, -1043,   317, -1043,
     766,    12, -1043, -1043,   527,   893,   894, -1043, -1043,   895,
   -1043,   898,   897, -1043, -1043, -1043, -1043,  1044,   916, -1043,
       2, -1043,    12, -1043,   146, -1043,    66, -1043,    96, -1043,
      19, -1043, -1043, -1043,   901,  1113, -1043, -1043,   917, -1043,
     910, -1043,   921,  1061,   864, -1043,    12,    12,   864,   923,
      12,   864,   864,   922,   924, -1043, -1043, -1043, -1043,   925,
   -1043, -1043, -1043, -1043,   927, -1043,   928,   929,   930, -1043,
     931,   932,   933,   934,   935,   936,   937,   938,   939, -1043,
     940, -1043,   941, -1043, -1043, -1043,   942, -1043, -1043, -1043,
   -1043, -1043,   943, -1043,   944,   945,   106,    96,  1141,   947,
   -1043,   -20,  1165,   949,   985,  1168,   172,   112,   987,    60,
    1082,   955,    -1, -1043,   946,  1085, -1043,    12,   957,   146,
    1111,   -51,   959,  1083,    51,   960,     4,  1086,  1180,   323,
     317,  1087,   150, -1043, -1043,   264,   961,    20,     0,   217,
     852, -1043,   221, -1043, -1043,   264,   527,    78, -1043,   179,
     197, -1043,    20,   966, -1043, -1043,    12, -1043,   965,   373,
     248,   967,    20,   700,   968,   969,    12, -1043,    12,    40,
    1081,  1088,    12,  1084,  1089,  1077,    12,    12,    12,   146,
    1045,    12,    12,    12,   112, -1043,   954,   322, -1043, -1043,
     979,  1197,   981,   982,  1200,   984,   983, -1043, -1043,   986,
   -1043,   988, -1043,  1204, -1043,   375,   990, -1043, -1043,   991,
      86,   527,   992,   993,   396, -1043,   994, -1043,  1212, -1043,
    1213,   981, -1043,   997, -1043,   998, -1043, -1043, -1043,   999,
      14, -1043, -1043,   527, -1043, -1043, -1043, -1043,   317,    66,
   -1043, -1043,    19, -1043,  1000,  1216,   264, -1043,  1179, -1043,
      12, -1043,  1001, -1043, -1043, -1043,   378, -1043, -1043, -1043,
   -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043,   527, -1043,
   -1043, -1043, -1043, -1043,   111, -1043, -1043, -1043,   146, -1043,
   -1043, -1043, -1043, -1043, -1043,  1004, -1043,  1005,  1006, -1043,
    1007, -1043,  1008, -1043, -1043,  1009, -1043, -1043, -1043,   527,
    1010, -1043, -1043,  1011, -1043,  1012, -1043, -1043,  1220, -1043,
    1014,   472,  1133,  1233,  1017,   112, -1043,    -2,   112,  1015,
    1020,     4,    12, -1043, -1043,  1018,  1171,  1149, -1043,  1022,
    1023,  1025,  1027,  1028,  1029,  1030, -1043, -1043,  1032,  1033,
    1034, -1043,  1140,  1035,  1036,  1133,  1244,    86,  1246,   424,
    1038,  1037,    14, -1043, -1043, -1043, -1043,  1040,  1039,   538,
   -1043,    12,  1175,   527,    12,    12,    12,    12,    12,   -89,
      40,  1258,  1121, -1043,  1262, -1043, -1043, -1043, -1043,  1046,
    1047,  1170,  1050,  1264, -1043,  1051,  1052, -1043, -1043,   177,
     981, -1043, -1043,  1054,  1055,  1174,  1273,    87,   112,    40,
      16,    16,   -51,   -21,  1274,  1058, -1043,  1276,  1170, -1043,
   -1043, -1043,  1059, -1043, -1043, -1043,  1277, -1043,  1240, -1043,
      25, -1043, -1043, -1043, -1043, -1043, -1043, -1043,  1002, -1043,
   -1043, -1043, -1043,  1062,   472,  1183,  1065,  1265,  1284,  1066,
    1105,   112,  1068,  1133,  1199,  1201, -1043, -1043, -1043, -1043,
   -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043,
   -1043,  1188,  1072,  1265,   945, -1043,   813,  1076,  1074,  1080,
    1078,   -89, -1043, -1043, -1043, -1043,  1174,  1079,  1114,  1090,
   -1043,  1091, -1043, -1043,  1170,  1094, -1043,  1095, -1043,  1114,
     813, -1043,    20, -1043, -1043,  1096, -1043, -1043,  1205,  1097,
   -1043, -1043,   112,  1265,  1211,    20, -1043, -1043,  1099,   -51,
   -1043,   112,   317, -1043,  1114, -1043, -1043,   317,    83,  1100,
   -1043, -1043, -1043, -1043, -1043, -1043, -1043,   979,   -51,  1098,
     317, -1043, -1043, -1043, -1043
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1043, -1043,  1300, -1043, -1043, -1043, -1043, -1043, -1043, -1043,
   -1043, -1043,   129, -1043, -1043,  1056,  -108,  -376,   885,  1101,
    1235,  -502, -1043, -1043, -1043, -1043,   633, -1043, -1043, -1043,
   -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043,  1252, -1043,
   -1043, -1043,   308, -1043, -1043, -1043, -1043, -1043, -1043, -1043,
   -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043,
   -1043, -1043,   833,  1092, -1043, -1043, -1043, -1043,   767, -1043,
   -1043, -1043,   355, -1043, -1043, -1043,  -640,   332,   265, -1043,
   -1043,   477,   292, -1043, -1043, -1043, -1043, -1043,   206, -1043,
   -1043,  1144, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043,
   -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043,
   -1043, -1043, -1043, -1043,  -570, -1043, -1043, -1043, -1043, -1043,
   -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043,
   -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043, -1043,
   -1043,   281,   582,  -825,   251, -1043,  -923, -1043,  -966,   210,
   -1042, -1007,  -475,  -610, -1043, -1043, -1043,   506,   909,  -218,
    -179,  -391,   788,   531,  -388,  -491,  -644,  -534,  -462,  -529,
    -652,  -171,   -76, -1043,   549, -1043, -1043,   823, -1043, -1043,
    1053,  -147,   762, -1043,  -480, -1043, -1043, -1043, -1043, -1043,
    -155, -1043
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -236
static const yytype_int16 yytable[] =
{
     248,   275,   503,   591,   679,   245,   258,   763,   677,   320,
     624,   322,   383,   659,   645,   646,   240,   336,   337,   546,
     339,   341,   567,   533,   798,   363,   534,   642,    16,   364,
     365,   357,   266,   267,   360,   293,   801,   706,   368,   240,
     370,   707,   708,   294,   324,   342,   995,   331,   681,   349,
     350,   351,   630,   807,   375,   866,   386,   387,   388,   389,
     653,   638,   314,   345,   358,   359,   263,   361,   362,   294,
     415,   369,  1058,   614,   372,   427,   374,     7,   417,   379,
     635,   582,   854,   882,   585,   266,   267,   869,  1175,  1035,
     973,  1099,  1113,   709,   240,   595,  1077,  1156,   391,   305,
       8,   442,   443,   596,   445,     9,  1139,   710,   711,   712,
     713,   657,   714,   658,   715,   305,   266,   267,   266,   267,
     240,   626,  1173,   295,   528,   363,   627,   294,   391,   364,
     365,   668,   376,   356,   670,   428,    11,   296,   669,   876,
    1078,   671,    15,   716,   717,   718,  1164,   719,   305,   295,
     720,   721,   722,   567,   723,   724,   725,   867,   726,   639,
     264,   270,    13,   296,   567,   392,  1176,   510,  1153,   974,
    1134,   402,   271,   418,   999,   672,  1000,   881,   631,   306,
     317,  1100,   673,  1101,  1036,   727,    14,   728,   729,   318,
       1,     2,   894,   272,   273,   306,   730,   731,   732,   733,
     734,   735,   902,   674,   366,   174,   254,   295,   315,   346,
     675,   175,   332,   615,   177,  1093,   591,   255,   256,   736,
     636,   296,   855,   850,   239,   271,   230,   870,   306,   254,
    1102,   240,   247,   737,   252,   241,   249,  1004,   253,   547,
     255,   256,   644,   535,   354,   270,  1119,    81,  1005,   -18,
     242,   243,   406,   178,   407,   847,   271,   277,   413,   408,
     409,   410,   848,   297,   254,   555,   556,   565,   566,   298,
     621,   838,   783,   242,   243,   255,   256,   272,   273,   794,
     793,   844,   586,   587,   804,   839,   244,   552,   659,   297,
     908,   879,   299,   300,   649,   298,   179,   564,   609,   889,
     923,   574,   398,   399,   400,   577,   890,   244,   509,   244,
     266,   267,   625,   241,   180,   541,   588,   518,   519,   307,
     520,   521,   522,   523,   524,   308,   613,   335,   242,   243,
     618,   619,   620,  1012,   338,   307,   755,   392,   536,   241,
     654,   308,   354,   241,   331,   254,   603,   297,   604,   605,
     542,   676,   181,   298,   242,   243,   255,   256,   242,   243,
     554,   435,   309,   310,   244,   491,   411,   321,   307,   266,
     267,   182,   678,   644,   308,   254,   299,   300,   183,   580,
     666,   691,   692,   584,   644,   394,   255,   256,   680,   184,
     244,   185,   594,   186,   244,   309,   310,   187,   570,   571,
     747,   950,   739,   188,   877,   562,   874,   891,   395,   570,
     571,   396,   397,   398,   399,   400,   189,   650,   892,   396,
     397,   398,   399,   400,   325,   893,   567,   190,   363,   191,
     568,  1034,   364,   365,  1037,   951,   952,   412,   192,   270,
     266,   267,   841,   381,   953,   804,   193,   326,   327,   402,
     413,   884,   885,   954,   414,   194,   254,   396,   397,   398,
     399,   400,   268,   269,   955,   242,   243,   255,   256,   900,
     384,   272,   273,   326,   327,   398,   399,   400,   699,   435,
    1106,  1107,   517,   266,   267,   385,   393,   268,   269,   978,
     956,  1080,   259,   260,   261,   262,   979,   195,   270,   196,
     197,   244,   198,   796,   199,   200,   266,   267,   390,   340,
     201,   202,  1003,   203,  1104,  1025,  1026,  1027,   980,  1062,
    1105,   204,   205,   396,   397,   398,   399,   400,   206,   207,
     272,   273,  1158,   396,   397,   398,   399,   400,   981,   396,
     397,   398,   399,   400,   423,  1166,   208,   808,   209,   859,
     210,   811,   211,   212,   814,   815,   213,  1132,   266,   267,
     214,   215,   278,   279,   280,   281,   282,   216,   283,   270,
     284,   285,   286,   287,   288,   785,   562,   266,   267,   426,
     289,   217,   982,   983,   984,   985,   218,   986,   987,   988,
     989,   990,   863,   219,   898,   429,   795,   899,   861,   220,
     907,   272,   273,   221,   911,   222,   223,   224,   915,   916,
     917,   290,   270,   920,   921,   922,   226,   883,  1163,   227,
     809,   810,   430,   271,   813,   228,   229,  1170,   278,   279,
     280,   281,   282,   235,   283,   270,   284,   285,   286,   287,
     288,   266,   267,   236,   272,   273,   569,   237,   396,   397,
     398,   399,   400,   396,  1011,   398,   399,   400,   918,   434,
     238,   250,   431,   663,   432,   949,   435,   272,   273,   437,
     436,   570,   571,   438,   444,   452,   446,   290,   447,   448,
     278,   279,   280,   281,   282,   450,   283,   270,   284,   285,
     286,   287,   288,   454,   453,   455,   458,   461,   289,   278,
     279,   280,   281,   282,   459,   283,   270,   284,   285,   286,
     287,   288,   396,   397,   398,   399,   400,   289,  -235,   272,
     273,   462,   463,   464,   330,   465,   466,   467,   468,   290,
     896,   469,   343,   344,   470,   471,   472,   473,   272,   273,
     906,   474,   475,   323,   476,   477,   478,   479,   290,   480,
     481,   482,   483,   373,  1041,   485,   486,  1013,   380,  1068,
     487,   488,  1069,   278,   279,   280,   281,   282,   489,   283,
     270,   284,   285,   286,   287,   288,   490,   492,   493,   508,
     416,   289,   494,   495,   496,   497,  1171,   498,   499,   500,
     501,  1174,   502,   505,   506,   507,  1072,  1073,  1074,  1075,
    1076,   511,   272,   273,  1184,   512,   513,   514,   428,   440,
     441,   516,   290,   396,   397,   398,   399,   400,   525,    16,
     526,   527,   537,   538,  1009,   539,   543,    17,   545,   549,
     550,    18,    19,    20,    21,    22,    23,   551,    24,    25,
      26,    27,    28,    29,    30,   664,   553,   558,    31,    32,
      33,    34,    35,   683,    36,   560,   559,   561,   689,    37,
      38,    39,    40,    41,    42,    43,    44,   575,    45,    46,
      47,   705,    48,  1108,    49,    50,    51,    52,    53,    54,
     743,   576,   578,    55,    56,   579,    57,   581,   583,   750,
     589,   592,   593,    58,   396,   397,   398,   399,   400,   598,
     599,   600,   396,   397,   398,   399,   400,   396,   397,   398,
     399,   400,   601,   602,    59,   608,   610,    60,   611,   612,
     396,   397,   398,   399,   400,   617,   903,   622,   629,   396,
     397,   398,   399,   400,   632,  1070,   633,    61,   396,   397,
     398,   399,   400,    62,   634,    63,    64,   266,   267,    65,
      66,   640,    67,   641,    68,   647,  -183,   652,   655,   667,
      69,   656,   682,    70,    71,   684,   685,   688,   693,    72,
      73,   687,    74,   694,    75,   396,   397,   398,   399,   400,
    1169,    85,   231,    87,   695,    89,   696,    91,    84,    93,
      86,    95,    88,    97,    90,    99,    92,   101,    94,  1182,
      96,   697,    98,   698,   100,   704,   700,   701,  1141,    76,
      77,    78,    79,   702,    80,   703,   742,   746,   747,   748,
     749,   751,   752,   753,   754,   756,   758,   755,   759,   757,
     761,   760,  1157,   762,   768,   775,   764,   780,    81,   924,
     925,   765,   767,   769,    82,    83,   699,   784,   792,   770,
     773,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   278,
     279,   280,   281,   282,   774,   283,   270,   284,   285,   286,
     287,   288,   926,   927,    83,   771,   776,   289,   777,   928,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   272,   273,
     778,   779,   781,   782,   787,   788,   803,   789,   290,   790,
     929,   791,   802,   930,   931,   932,   933,   934,   935,   936,
     937,   938,   939,   940,   941,   942,   943,   944,   945,   325,
     805,   804,   806,   562,   812,   816,   840,   818,   817,   819,
     820,   821,   822,   823,   824,   825,   826,   827,   828,   829,
     830,   831,   832,   833,   834,   836,   837,   835,   841,   843,
     844,   845,   846,   851,   852,   857,   853,   858,   860,   862,
     864,   868,   865,   254,   873,   872,   878,   895,   880,   897,
     909,   901,   904,   905,   255,   256,   910,   914,   919,   912,
     958,   960,   961,   963,   964,   913,   965,   967,   969,   966,
     968,   971,   972,   976,   977,   992,   993,   994,   996,   997,
     998,  1007,  1008,  1023,  1006,  1010,  1014,  1015,  1016,  1017,
    1018,  1019,  1020,  1021,  1022,  1024,  1030,  1032,  1033,  1038,
    1043,  1039,  1042,  1044,  1045,  1046,  1055,  1047,  1059,  1048,
    1049,  1050,  1051,  1061,  1052,  1053,  1054,  1057,  1071,  1056,
    1063,  1066,  1081,  1067,  1082,  1064,  1083,  1084,  1089,  1085,
    1086,  1088,  1090,  1096,  1091,  1094,  1095,  1098,  1110,  1111,
    1112,  1115,  1117,  1118,  1121,  1124,  1122,  1126,  1129,  1127,
    1130,  1131,  1133,  1135,  1136,  1137,  1138,  1142,  1143,  1144,
    1149,  1148,  1145,  1160,  1165,    10,  1181,   540,   232,   425,
    1065,   799,   371,  1152,  1151,  1154,  1155,  1159,   225,  1161,
    1168,  1178,  1183,   623,   690,  1114,  1040,  1146,   304,  1060,
     382,  1092,   970,   842,  1109,  1123,  1147,   544,   686,   887,
     449,     0,   948,   740,     0,     0,     0,     0,   665
};

static const yytype_int16 yycheck[] =
{
     108,   180,   378,   465,   538,    81,   177,   617,   537,   188,
     490,   190,   230,   515,   505,   506,     4,   196,   197,     3,
     199,   200,     3,   414,   676,     4,   414,   502,     6,     8,
       9,   210,    83,    84,   213,   182,   680,     4,   217,     4,
     219,     8,     9,     3,   191,   200,   871,    58,   539,   204,
     205,   206,    67,   693,     5,     4,   235,   236,   237,   238,
      83,     4,    83,    83,   211,   212,     4,   214,   215,     3,
     249,   218,   995,    83,   221,   221,   223,   222,   249,   226,
      83,   457,    83,    83,   460,    83,    84,    83,     5,    91,
       4,     4,  1058,    60,     4,    75,   185,  1139,     8,     3,
     222,   280,   281,    83,   283,     0,  1113,    74,    75,    76,
      77,   146,    79,   148,    81,     3,    83,    84,    83,    84,
       4,    78,  1164,    83,     4,     4,    83,     3,     8,     8,
       9,   221,    83,   209,   221,   281,   126,    97,   228,   779,
     229,   228,   222,   110,   111,   112,  1153,   114,     3,    83,
     117,   118,   119,     3,   121,   122,   123,   106,   125,   102,
      98,   212,   229,    97,     3,   241,    83,   385,  1134,    83,
    1093,   247,   223,   249,   160,   221,   162,   787,   193,    83,
      74,    94,   228,    96,   186,   152,   229,   154,   155,    83,
      16,    17,   802,   244,   245,    83,   163,   164,   165,   166,
     167,   168,   812,   221,    83,   221,   229,    83,   229,   229,
     228,   127,   223,   223,   222,  1040,   678,   240,   241,   186,
     223,    97,   223,   757,   231,   223,   225,   223,    83,   229,
     143,     4,   223,   200,     3,   223,   280,   889,   221,   223,
     240,   241,   223,   414,   223,   212,   221,   225,   892,   224,
     238,   239,    25,   222,    27,    83,   223,   102,   223,    32,
      33,    34,    90,   223,   229,    88,    89,   446,   447,   229,
     488,   746,   648,   238,   239,   240,   241,   244,   245,   670,
     668,   221,   461,   462,   224,   747,   274,   434,   790,   223,
     819,   782,   252,   253,   158,   229,   222,   444,   477,   221,
     834,   448,   277,   278,   279,   452,   228,   274,   384,   274,
      83,    84,   491,   223,   222,   423,   463,   393,   394,   223,
     396,   397,   398,   399,   400,   229,   481,   229,   238,   239,
     485,   486,   487,   222,    95,   223,   225,   413,   414,   223,
     511,   229,   223,   223,    58,   229,    83,   223,    85,    86,
     426,   227,   222,   229,   238,   239,   240,   241,   238,   239,
     436,     4,   250,   251,   274,   221,   139,     4,   223,    83,
      84,   222,   227,   223,   229,   229,   252,   253,   222,   455,
     527,   560,   561,   459,   223,   203,   240,   241,   227,   222,
     274,   222,   468,   222,   274,   250,   251,   222,   248,   249,
     221,    79,   581,   222,   780,    82,    83,   228,   226,   248,
     249,   275,   276,   277,   278,   279,   222,   281,   221,   275,
     276,   277,   278,   279,   223,   228,     3,   222,     4,   222,
       7,   965,     8,     9,   968,   113,   114,   210,   222,   212,
      83,    84,   221,   148,   122,   224,   222,   246,   247,   525,
     223,   234,   235,   131,   227,   222,   229,   275,   276,   277,
     278,   279,    87,    88,   142,   238,   239,   240,   241,   221,
     280,   244,   245,   246,   247,   277,   278,   279,     3,     4,
    1050,  1051,   224,    83,    84,   225,   225,    87,    88,    93,
     168,  1020,    10,    11,    12,    13,   100,   222,   212,   222,
     222,   274,   222,   674,   222,   222,    83,    84,   223,   223,
     222,   222,   888,   222,  1048,    43,    44,    45,   122,   999,
    1049,   222,   222,   275,   276,   277,   278,   279,   222,   222,
     244,   245,  1142,   275,   276,   277,   278,   279,   142,   275,
     276,   277,   278,   279,   227,  1155,   222,   694,   222,   767,
     222,   698,   222,   222,   701,   702,   222,  1091,    83,    84,
     222,   222,   205,   206,   207,   208,   209,   222,   211,   212,
     213,   214,   215,   216,   217,   651,    82,    83,    84,   225,
     223,   222,   186,   187,   188,   189,   222,   191,   192,   193,
     194,   195,   771,   222,   221,   221,   672,   224,   769,   222,
     818,   244,   245,   222,   822,   222,   222,   222,   826,   827,
     828,   254,   212,   831,   832,   833,   222,   788,  1152,   222,
     696,   697,   221,   223,   700,   222,   222,  1161,   205,   206,
     207,   208,   209,   222,   211,   212,   213,   214,   215,   216,
     217,    83,    84,   222,   244,   245,   223,   222,   275,   276,
     277,   278,   279,   275,   276,   277,   278,   279,   829,   223,
     222,   222,   221,   226,   220,   836,     4,   244,   245,   221,
     225,   248,   249,   221,   223,   283,   223,   254,   223,   223,
     205,   206,   207,   208,   209,   223,   211,   212,   213,   214,
     215,   216,   217,     3,   221,   225,     3,   221,   223,   205,
     206,   207,   208,   209,   225,   211,   212,   213,   214,   215,
     216,   217,   275,   276,   277,   278,   279,   223,   227,   244,
     245,   221,   221,   221,   193,   221,   221,     4,   225,   254,
     806,   221,   201,   202,   221,   223,     4,   221,   244,   245,
     816,   221,   221,   185,   221,   221,   221,     4,   254,   221,
     221,   221,     6,   222,   972,   221,   221,   928,   227,   221,
     221,   221,   224,   205,   206,   207,   208,   209,     4,   211,
     212,   213,   214,   215,   216,   217,   221,   221,   221,   226,
     249,   223,   221,   221,   221,   221,  1162,   221,   221,   221,
     221,  1167,   221,   221,   221,   221,  1014,  1015,  1016,  1017,
    1018,   221,   244,   245,  1180,   221,   221,   221,   281,   278,
     279,   224,   254,   275,   276,   277,   278,   279,   221,     6,
     224,   222,   222,   222,   900,   222,    15,    14,     3,     3,
     221,    18,    19,    20,    21,    22,    23,   226,    25,    26,
      27,    28,    29,    30,    31,   226,   221,     5,    35,    36,
      37,    38,    39,   226,    41,   221,   230,   221,   226,    46,
      47,    48,    49,    50,    51,    52,    53,   218,    55,    56,
      57,   226,    59,  1052,    61,    62,    63,    64,    65,    66,
     226,   224,    98,    70,    71,   221,    73,   221,   221,   226,
      82,    82,   221,    80,   275,   276,   277,   278,   279,    40,
       4,   221,   275,   276,   277,   278,   279,   275,   276,   277,
     278,   279,    40,    42,   101,   186,    92,   104,   221,    92,
     275,   276,   277,   278,   279,   221,   226,   221,    96,   275,
     276,   277,   278,   279,   190,  1011,   102,   124,   275,   276,
     277,   278,   279,   130,   105,   132,   133,    83,    84,   136,
     137,   134,   139,   134,   141,   149,   138,   226,   144,   221,
     147,   145,   228,   150,   151,     3,   221,     4,   282,   156,
     157,   224,   159,   221,   161,   275,   276,   277,   278,   279,
    1159,   239,   232,   241,   224,   243,   221,   245,   238,   247,
     240,   249,   242,   251,   244,   253,   246,   255,   248,  1178,
     250,   221,   252,   221,   254,   229,   225,   221,  1116,   196,
     197,   198,   199,   221,   201,   221,   229,   221,   221,   221,
       4,   221,   221,     4,   221,   221,   221,   225,     4,   222,
       4,   221,  1140,   221,    82,     4,   221,   138,   225,    85,
      86,   221,   221,   221,   231,   232,     3,   281,     4,   221,
     221,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   205,
     206,   207,   208,   209,   221,   211,   212,   213,   214,   215,
     216,   217,   128,   129,   232,   222,   221,   223,   221,   135,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   244,   245,
     221,   221,   221,   221,   221,   221,     3,   222,   254,   221,
     166,   224,   221,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   223,
     230,   224,   221,    82,   221,   223,     5,   222,   224,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   221,   221,   224,   221,     4,
     221,   186,     4,   186,    92,   229,   221,    92,   221,    68,
     221,   221,    99,   229,     4,    99,    99,   221,   227,   224,
     109,   224,   224,   224,   240,   241,   108,   120,   153,   115,
     221,     4,   221,   221,     4,   116,   222,   221,     4,   226,
     222,   221,   221,   221,   221,   221,     4,     4,   221,   221,
     221,     5,    43,     3,   224,   224,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   221,   103,     4,   221,   224,
      69,   221,   224,    94,   222,   222,   106,   222,     4,   222,
     222,   222,   222,     7,   222,   222,   222,   221,    83,   224,
     222,   221,     4,   224,   143,   228,     4,   221,     4,   222,
     100,   221,   221,    99,   222,   221,   221,     4,     4,   221,
       4,   222,     5,    43,   282,   102,   224,   222,     4,    24,
     224,   186,   224,    94,    93,   107,   224,   221,   224,   219,
     186,   222,   224,    98,    93,     5,  1177,   422,    73,   253,
    1002,   678,   220,   222,   224,   221,   221,   221,    66,   222,
     221,   221,   224,   490,   557,  1060,   971,  1121,   184,   997,
     229,  1039,   855,   751,  1053,  1084,  1126,   428,   550,   790,
     287,    -1,   836,   581,    -1,    -1,    -1,    -1,   525
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    16,    17,   285,   286,   287,   288,   222,   222,     0,
     286,   126,   298,   229,   229,   222,     6,    14,    18,    19,
      20,    21,    22,    23,    25,    26,    27,    28,    29,    30,
      31,    35,    36,    37,    38,    39,    41,    46,    47,    48,
      49,    50,    51,    52,    53,    55,    56,    57,    59,    61,
      62,    63,    64,    65,    66,    70,    71,    73,    80,   101,
     104,   124,   130,   132,   133,   136,   137,   139,   141,   147,
     150,   151,   156,   157,   159,   161,   196,   197,   198,   199,
     201,   225,   231,   232,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   289,   290,   291,   292,   293,   300,   302,   303,
     304,   305,   307,   308,   316,   319,   320,   322,   323,   330,
     332,   333,   335,   337,   339,   343,   344,   345,   348,   350,
     357,   364,   367,   373,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   389,   390,   391,   392,   396,   397,
     399,   400,   401,   402,   403,   404,   405,   406,   407,   410,
     411,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   423,   424,   221,   127,   299,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   322,   222,   222,   222,   222,
     225,   232,   304,   306,   324,   222,   222,   222,   222,   231,
       4,   223,   238,   239,   274,   456,   466,   223,   300,   280,
     222,   334,     3,   221,   229,   240,   241,   442,   455,    10,
      11,    12,    13,     4,    98,   467,    83,    84,    87,    88,
     212,   223,   244,   245,   365,   444,   445,   102,   205,   206,
     207,   208,   209,   211,   213,   214,   215,   216,   217,   223,
     254,   444,   464,   465,     3,    83,    97,   223,   229,   252,
     253,   375,   453,   454,   375,     3,    83,   223,   229,   250,
     251,   369,   451,   452,    83,   229,   425,    74,    83,   436,
     444,     4,   444,   185,   465,   223,   246,   247,   447,   448,
     447,    58,   223,   474,   475,   229,   444,   444,    95,   444,
     223,   444,   474,   447,   447,    83,   229,   347,   426,   474,
     474,   474,   443,   456,   223,   346,   456,   444,   465,   465,
     444,   465,   465,     4,     8,     9,    83,   468,   444,   465,
     444,   347,   465,   447,   465,     5,    83,   358,   340,   465,
     447,   148,   303,   443,   280,   225,   444,   444,   444,   444,
     223,     8,   456,   225,   203,   226,   275,   276,   277,   278,
     279,   455,   456,   461,   462,   463,    25,    27,    32,    33,
      34,   139,   210,   223,   227,   444,   447,   455,   456,   470,
     471,   472,   338,   227,   301,   299,   225,   221,   281,   221,
     221,   221,   220,   446,   223,     4,   225,   221,   221,   352,
     447,   447,   444,   444,   223,   444,   223,   223,   223,   464,
     223,   465,   283,   221,     3,   225,   376,   374,     3,   225,
     368,   221,   221,   221,   221,   221,   221,     4,   225,   221,
     221,   223,     4,   221,   221,   221,   221,   221,   221,     4,
     221,   221,   221,     6,   321,   221,   221,   221,   221,     4,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   301,   341,   221,   221,   221,   226,   456,
     443,   221,   221,   221,   221,   317,   224,   224,   456,   456,
     456,   456,   456,   456,   456,   221,   224,   222,     4,   312,
     313,   314,   315,   445,   448,   455,   456,   222,   222,   222,
     302,   300,   456,    15,   442,     3,     3,   223,   398,     3,
     221,   226,   465,   221,   456,    88,    89,   366,     5,   230,
     221,   221,    82,   360,   465,   444,   444,     3,     7,   223,
     248,   249,   449,   450,   465,   218,   224,   465,    98,   221,
     456,   221,   301,   221,   456,   301,   444,   444,   465,    82,
     310,   452,    82,   221,   456,    75,    83,   437,    40,     4,
     221,    40,    42,    83,    85,    86,   438,   441,   186,   444,
      92,   221,    92,   474,    83,   223,   353,   221,   474,   474,
     474,   443,   221,   346,   468,   444,    78,    83,   473,    96,
      67,   193,   190,   102,   105,    83,   223,   354,     4,   102,
     134,   134,   436,   342,   223,   449,   449,   149,   336,   158,
     281,   331,   226,    83,   455,   144,   145,   146,   148,   305,
     458,   459,   460,   226,   226,   461,   465,   221,   221,   228,
     221,   228,   221,   228,   221,   228,   227,   453,   227,   451,
     227,   449,   228,   226,     3,   221,   446,   224,     4,   226,
     352,   444,   444,   282,   221,   224,   221,   221,   221,     3,
     225,   221,   221,   221,   229,   226,     4,     8,     9,    60,
      74,    75,    76,    77,    79,    81,   110,   111,   112,   114,
     117,   118,   119,   121,   122,   123,   125,   152,   154,   155,
     163,   164,   165,   166,   167,   168,   186,   200,   377,   444,
     466,   469,   229,   226,   370,   362,   221,   221,   221,     4,
     226,   221,   221,     4,   221,   225,   221,   222,   221,     4,
     221,     4,   221,   437,   221,   221,   388,   221,    82,   221,
     221,   222,   393,   221,   221,     4,   221,   221,   221,   221,
     138,   221,   221,   301,   281,   456,   325,   221,   221,   222,
     221,   224,     4,   448,   445,   456,   455,   309,   454,   310,
     311,   450,   221,     3,   224,   230,   221,   360,   465,   456,
     456,   465,   221,   456,   465,   465,   223,   224,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   224,   221,   221,   436,   452,
       5,   221,   426,     4,   221,   186,     4,    83,    90,   440,
     451,   186,    92,   221,    83,   223,   355,   229,    92,   443,
     221,   455,    68,   444,   221,    99,     4,   106,   221,    83,
     223,   356,    99,     4,    83,   359,   360,   301,    99,   449,
     227,   437,    83,   455,   234,   235,   457,   458,   318,   221,
     228,   228,   221,   228,   437,   221,   456,   224,   221,   224,
     221,   224,   437,   226,   224,   224,   456,   443,   453,   109,
     108,   443,   115,   116,   120,   443,   443,   443,   455,   153,
     443,   443,   443,   451,    85,    86,   128,   129,   135,   166,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   371,   439,   441,   455,
      79,   113,   114,   122,   131,   142,   168,   363,   221,   296,
       4,   221,   427,   221,     4,   222,   226,   221,   222,     4,
     365,   221,   221,     4,    83,   361,   221,   221,    93,   100,
     122,   142,   186,   187,   188,   189,   191,   192,   193,   194,
     195,   394,   221,     4,     4,   427,   221,   221,   221,   160,
     162,   326,   327,   301,   454,   450,   224,     5,    43,   456,
     224,   276,   222,   455,   222,   222,   222,   222,   222,   222,
     222,   222,   222,     3,   221,    43,    44,    45,   428,   429,
     103,   430,     4,   221,   451,    91,   186,   451,   224,   221,
     356,   443,   224,    69,    94,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   106,   224,   221,   430,     4,
     361,     7,   468,   222,   228,   326,   221,   224,   221,   224,
     456,    83,   443,   443,   443,   443,   443,   185,   229,   372,
     453,     4,   143,     4,   221,   222,   100,   432,   221,     4,
     221,   222,   366,   427,   221,   221,    99,   433,     4,     4,
      94,    96,   143,   395,   451,   453,   398,   398,   444,   425,
       4,   221,     4,   432,   362,   222,   329,     5,    43,   221,
     294,   282,   224,   428,   102,   431,   222,    24,   435,     4,
     224,   186,   451,   224,   430,    94,    93,   107,   224,   435,
     328,   300,   221,   224,   219,   224,   372,   433,   222,   186,
     434,   224,   222,   432,   221,   221,   434,   300,   437,   221,
      98,   222,   408,   451,   435,    93,   437,   349,   221,   444,
     451,   301,   409,   434,   301,     5,    83,   297,   221,   295,
     351,   296,   444,   224,   301
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

  case 121:

/* Line 1455 of yacc.c  */
#line 605 "lev_comp.y"
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

  case 122:

/* Line 1455 of yacc.c  */
#line 621 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 627 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 633 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 639 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 645 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 651 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 657 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 663 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 669 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 679 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 689 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 699 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 709 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 719 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 729 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 741 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 746 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 753 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 758 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 765 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 770 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 777 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 781 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 787 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 792 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 799 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 803 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 809 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 813 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 819 "lev_comp.y"
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

  case 152:

/* Line 1455 of yacc.c  */
#line 841 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 845 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 856 "lev_comp.y"
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

  case 155:

/* Line 1455 of yacc.c  */
#line 911 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 917 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 921 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 927 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 934 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 938 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 946 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 950 "lev_comp.y"
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

  case 163:

/* Line 1455 of yacc.c  */
#line 973 "lev_comp.y"
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

  case 166:

/* Line 1455 of yacc.c  */
#line 1018 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1028 "lev_comp.y"
    {
		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1031 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1041 "lev_comp.y"
    {
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1046 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1060 "lev_comp.y"
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

  case 174:

/* Line 1455 of yacc.c  */
#line 1097 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1102 "lev_comp.y"
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

  case 176:

/* Line 1455 of yacc.c  */
#line 1131 "lev_comp.y"
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

  case 177:

/* Line 1455 of yacc.c  */
#line 1145 "lev_comp.y"
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

  case 178:

/* Line 1455 of yacc.c  */
#line 1161 "lev_comp.y"
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

  case 179:

/* Line 1455 of yacc.c  */
#line 1181 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1193 "lev_comp.y"
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

  case 181:

/* Line 1455 of yacc.c  */
#line 1213 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1219 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1229 "lev_comp.y"
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

  case 184:

/* Line 1455 of yacc.c  */
#line 1247 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1257 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(NO_COLOR, SPO_MESSAGE));
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1261 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_MESSAGE));
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1266 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1271 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1276 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1283 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1290 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1299 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1307 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1318 "lev_comp.y"
    {
		      long rflags = (yyvsp[(8) - (12)].i);
		      long flmt = (long)(yyvsp[(9) - (12)].i);
		      long flt = (long)(yyvsp[(10) - (12)].i);
		      long tlset = (long)(yyvsp[(11) - (12)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      //if (flmt == -1) flmt = ROOM;
		      //if (flt == -1) flt = 0;

		      add_opvars(splev, "iiiiiiiiiio",
				 VA_PASS11(tlset, flt, flmt, rflags, ERR, ERR,
					  (yyvsp[(5) - (12)].crd).x, (yyvsp[(5) - (12)].crd).y, (yyvsp[(7) - (12)].sze).width, (yyvsp[(7) - (12)].sze).height,
					  SPO_SUBROOM));
		      break_stmt_start();
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1335 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1342 "lev_comp.y"
    {
		      long rflags = (yyvsp[(10) - (14)].i);
		      long flmt = (long)(yyvsp[(11) - (14)].i);
		      long flt = (long)(yyvsp[(12) - (14)].i);
		      long tlset = (long)(yyvsp[(13) - (14)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      //if (flmt == -1) flmt = ROOM;
		      //if (flt == -1) flt = 0;

		      add_opvars(splev, "iiiiiiiiiio",
				 VA_PASS11(tlset, flt, flmt, rflags,
					  (yyvsp[(7) - (14)].crd).x, (yyvsp[(7) - (14)].crd).y, (yyvsp[(5) - (14)].crd).x, (yyvsp[(5) - (14)].crd).y,
					  (yyvsp[(9) - (14)].sze).width, (yyvsp[(9) - (14)].sze).height, SPO_ROOM));
		      break_stmt_start();
		  ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1359 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1366 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1370 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1376 "lev_comp.y"
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

  case 201:

/* Line 1455 of yacc.c  */
#line 1386 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1392 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1401 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1407 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1412 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1418 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1423 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1429 "lev_comp.y"
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

  case 209:

/* Line 1455 of yacc.c  */
#line 1440 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1454 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1458 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1468 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1475 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1483 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1488 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1493 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1498 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1503 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1508 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1513 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1521 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1528 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1536 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1552 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1556 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1562 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1570 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1576 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1584 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1592 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1597 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1603 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1609 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1615 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1621 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1626 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(0, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1631 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1636 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1641 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1646 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1651 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1656 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1661 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1666 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1671 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1676 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1681 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1687 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1692 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_PROTECTOR));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1697 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_MAXHP));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1702 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_LEVEL_ADJUSTMENT));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1707 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_KEEP_ORIGINAL_INVENTORY));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1714 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1723 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1727 "lev_comp.y"
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

  case 266:

/* Line 1455 of yacc.c  */
#line 1740 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1748 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1758 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1766 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1775 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1782 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1790 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1796 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1801 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1806 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1811 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1816 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1821 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1826 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1831 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1836 "lev_comp.y"
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

  case 282:

/* Line 1455 of yacc.c  */
#line 1850 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1856 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1861 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1866 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1871 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1876 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1881 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1886 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1891 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1896 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1901 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1906 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1911 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x00200000;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1916 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x00400000;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1921 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x00800000;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1926 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x01000000;
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1931 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_MYTHIC_TYPE));
		      (yyval.i) = 0x02000000;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1937 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x04000000;
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1942 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x08000000;
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1947 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x10000000;
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 1952 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MATERIAL));
		      (yyval.i) = 0x20000000;
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 1959 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 1965 "lev_comp.y"
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

  case 305:

/* Line 1455 of yacc.c  */
#line 1994 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 1999 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2007 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2013 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2017 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2025 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2032 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2039 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2049 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2059 "lev_comp.y"
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

  case 315:

/* Line 1455 of yacc.c  */
#line 2075 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2085 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2089 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2095 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2101 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2107 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2111 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2117 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2123 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2130 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2138 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2144 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2149 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2154 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2159 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2164 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2170 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2176 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2181 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2186 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2191 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2196 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2201 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2211 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2217 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2223 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2228 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2235 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2242 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2246 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2252 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2258 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2264 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2269 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2276 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2282 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2292 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2298 "lev_comp.y"
    {
		      long irr;
		      long rt = (yyvsp[(7) - (12)].i);
		      long rflags = (yyvsp[(8) - (12)].i);
		      long flmt = (long)(yyvsp[(9) - (12)].i);
		      long flt = (long)(yyvsp[(10) - (12)].i);
		      long tlset = (long)(yyvsp[(11) - (12)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      //if (flmt == -1) flmt = 0;
		      //if (flt == -1) flt = 0;

		      if (!(rflags & 1)) rt += MAXRTYPE+1;
		      irr = ((rflags & 2) != 0);
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((long)(yyvsp[(5) - (12)].i), rt, rflags, flmt, flt, tlset, SPO_REGION));
		      (yyval.i) = (irr || (rflags & 1) || rt != OROOM);
		      break_stmt_start();
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2318 "lev_comp.y"
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

  case 358:

/* Line 1455 of yacc.c  */
#line 2330 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2334 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2340 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2345 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2350 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (long)(yyvsp[(9) - (9)].i), (long)(yyvsp[(7) - (9)].i), (long)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2355 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((long)(yyvsp[(9) - (13)].i), (long)(yyvsp[(7) - (13)].i), (long)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2361 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2365 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2371 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, (int)(yyvsp[(11) - (11)].i), (int)(yyvsp[(9) - (11)].i), (int)(yyvsp[(7) - (11)].i), (int)(yyvsp[(5) - (11)].i), SPO_DECORATION));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2375 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6((int)(yyvsp[(13) - (13)].i), (int)(yyvsp[(11) - (13)].i), (int)(yyvsp[(9) - (13)].i), (int)(yyvsp[(7) - (13)].i), (int)(yyvsp[(5) - (13)].i), SPO_DECORATION));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2379 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, 1, (int)(yyvsp[(7) - (7)].i), 0, (int)(yyvsp[(5) - (7)].i), SPO_DECORATION));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2385 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2391 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2395 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2401 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_CARPET));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2407 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2411 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4(0, (int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2417 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2421 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2427 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2431 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2436 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2443 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2447 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2454 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2458 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2463 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2470 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2474 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2479 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2487 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2493 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2500 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2504 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2511 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2522 "lev_comp.y"
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

  case 396:

/* Line 1455 of yacc.c  */
#line 2535 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2539 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2545 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2549 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2556 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2560 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2564 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2570 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2574 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2580 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2586 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2590 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2596 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2602 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2607 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2613 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2617 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2634 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2642 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2652 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2661 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2666 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2673 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2684 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2690 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2694 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2698 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2705 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2715 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2722 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2726 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2732 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2736 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2744 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2748 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2755 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2765 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2778 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2782 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2789 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2799 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2808 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2819 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2823 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2830 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2840 "lev_comp.y"
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

  case 452:

/* Line 1455 of yacc.c  */
#line 2851 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2860 "lev_comp.y"
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

  case 454:

/* Line 1455 of yacc.c  */
#line 2870 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2876 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2880 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2887 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2897 "lev_comp.y"
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

  case 459:

/* Line 1455 of yacc.c  */
#line 2908 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2917 "lev_comp.y"
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

  case 461:

/* Line 1455 of yacc.c  */
#line 2927 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2931 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2937 "lev_comp.y"
    { ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2939 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2945 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2949 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2953 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2957 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 2965 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2974 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 2978 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 2982 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 2986 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 2990 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 2993 "lev_comp.y"
    { ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 2997 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3004 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3013 "lev_comp.y"
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

  case 483:

/* Line 1455 of yacc.c  */
#line 3054 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3058 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3065 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3072 "lev_comp.y"
    {
			      size_t len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3082 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3086 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3094 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3098 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3102 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3106 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3110 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3115 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3119 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3123 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3128 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3133 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3138 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3142 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3147 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3152 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3156 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3160 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3165 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3169 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3176 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3182 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3186 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3192 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 516:

/* Line 1455 of yacc.c  */
#line 3208 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 517:

/* Line 1455 of yacc.c  */
#line 3212 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 518:

/* Line 1455 of yacc.c  */
#line 3216 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 519:

/* Line 1455 of yacc.c  */
#line 3220 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 528:

/* Line 1455 of yacc.c  */
#line 3242 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 529:

/* Line 1455 of yacc.c  */
#line 3246 "lev_comp.y"
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

  case 530:

/* Line 1455 of yacc.c  */
#line 3272 "lev_comp.y"
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
#line 7443 "lev_comp.tab.c"
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
#line 3300 "lev_comp.y"


/*lev_comp.y*/

