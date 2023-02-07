
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
     MATERIAL_ID = 372,
     MATERIAL_TYPE = 373,
     CHARGES_ID = 374,
     SPECIAL_QUALITY_ID = 375,
     SPEFLAGS_ID = 376,
     SUBROOM_ID = 377,
     NAME_ID = 378,
     FLAGS_ID = 379,
     FLAG_TYPE = 380,
     MON_ATTITUDE = 381,
     MON_ALERTNESS = 382,
     SUBTYPE_ID = 383,
     NON_PASSDOOR_ID = 384,
     CARPET_ID = 385,
     CARPET_PIECE_ID = 386,
     CARPET_TYPE = 387,
     MON_APPEARANCE = 388,
     ROOMDOOR_ID = 389,
     IF_ID = 390,
     ELSE_ID = 391,
     TERRAIN_ID = 392,
     HORIZ_OR_VERT = 393,
     REPLACE_TERRAIN_ID = 394,
     LOCATION_SUBTYPE_ID = 395,
     DOOR_SUBTYPE = 396,
     BRAZIER_SUBTYPE = 397,
     SIGNPOST_SUBTYPE = 398,
     TREE_SUBTYPE = 399,
     FOREST_ID = 400,
     FOREST_TYPE = 401,
     INITIALIZE_TYPE = 402,
     EXIT_ID = 403,
     SHUFFLE_ID = 404,
     MANUAL_TYPE_ID = 405,
     MANUAL_TYPE = 406,
     QUANTITY_ID = 407,
     BURIED_ID = 408,
     LOOP_ID = 409,
     FOR_ID = 410,
     TO_ID = 411,
     SWITCH_ID = 412,
     CASE_ID = 413,
     BREAK_ID = 414,
     DEFAULT_ID = 415,
     ERODED_ID = 416,
     TRAPPED_STATE = 417,
     RECHARGED_ID = 418,
     INVIS_ID = 419,
     GREASED_ID = 420,
     INDESTRUCTIBLE_ID = 421,
     FEMALE_ID = 422,
     MALE_ID = 423,
     WAITFORU_ID = 424,
     PROTECTOR_ID = 425,
     CANCELLED_ID = 426,
     REVIVED_ID = 427,
     AVENGE_ID = 428,
     FLEEING_ID = 429,
     BLINDED_ID = 430,
     MAXHP_ID = 431,
     LEVEL_ADJUSTMENT_ID = 432,
     KEEP_ORIGINAL_INVENTORY_ID = 433,
     PARALYZED_ID = 434,
     STUNNED_ID = 435,
     CONFUSED_ID = 436,
     SEENTRAPS_ID = 437,
     ALL_ID = 438,
     MONTYPE_ID = 439,
     OBJTYPE_ID = 440,
     TERTYPE_ID = 441,
     TERTYPE2_ID = 442,
     LEVER_EFFECT_TYPE = 443,
     SWITCHABLE_ID = 444,
     CONTINUOUSLY_USABLE_ID = 445,
     TARGET_ID = 446,
     TRAPTYPE_ID = 447,
     EFFECT_FLAG_ID = 448,
     GRAVE_ID = 449,
     BRAZIER_ID = 450,
     SIGNPOST_ID = 451,
     TREE_ID = 452,
     ERODEPROOF_ID = 453,
     FUNCTION_ID = 454,
     MSG_OUTPUT_TYPE = 455,
     COMPARE_TYPE = 456,
     UNKNOWN_TYPE = 457,
     rect_ID = 458,
     fillrect_ID = 459,
     line_ID = 460,
     randline_ID = 461,
     grow_ID = 462,
     selection_ID = 463,
     flood_ID = 464,
     rndcoord_ID = 465,
     circle_ID = 466,
     ellipse_ID = 467,
     filter_ID = 468,
     complement_ID = 469,
     gradient_ID = 470,
     GRADIENT_TYPE = 471,
     LIMITED = 472,
     HUMIDITY_TYPE = 473,
     STRING = 474,
     MAP_ID = 475,
     NQSTRING = 476,
     VARSTRING = 477,
     CFUNC = 478,
     CFUNC_INT = 479,
     CFUNC_STR = 480,
     CFUNC_COORD = 481,
     CFUNC_REGION = 482,
     VARSTRING_INT = 483,
     VARSTRING_INT_ARRAY = 484,
     VARSTRING_STRING = 485,
     VARSTRING_STRING_ARRAY = 486,
     VARSTRING_VAR = 487,
     VARSTRING_VAR_ARRAY = 488,
     VARSTRING_COORD = 489,
     VARSTRING_COORD_ARRAY = 490,
     VARSTRING_REGION = 491,
     VARSTRING_REGION_ARRAY = 492,
     VARSTRING_MAPCHAR = 493,
     VARSTRING_MAPCHAR_ARRAY = 494,
     VARSTRING_MONST = 495,
     VARSTRING_MONST_ARRAY = 496,
     VARSTRING_OBJ = 497,
     VARSTRING_OBJ_ARRAY = 498,
     VARSTRING_SEL = 499,
     VARSTRING_SEL_ARRAY = 500,
     METHOD_INT = 501,
     METHOD_INT_ARRAY = 502,
     METHOD_STRING = 503,
     METHOD_STRING_ARRAY = 504,
     METHOD_VAR = 505,
     METHOD_VAR_ARRAY = 506,
     METHOD_COORD = 507,
     METHOD_COORD_ARRAY = 508,
     METHOD_REGION = 509,
     METHOD_REGION_ARRAY = 510,
     METHOD_MAPCHAR = 511,
     METHOD_MAPCHAR_ARRAY = 512,
     METHOD_MONST = 513,
     METHOD_MONST_ARRAY = 514,
     METHOD_OBJ = 515,
     METHOD_OBJ_ARRAY = 516,
     METHOD_SEL = 517,
     METHOD_SEL_ARRAY = 518,
     DICE = 519
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
#line 564 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 576 "lev_comp.tab.c"

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
#define YYLAST   1336

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  282
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  191
/* YYNRULES -- Number of rules.  */
#define YYNRULES  526
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1175

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   519

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   277,   281,     2,
     221,   222,   275,   273,   219,   274,   279,   276,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   220,     2,
       2,   278,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   223,     2,   224,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   225,   280,   226,     2,     2,     2,     2,
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
     215,   216,   217,   218,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272
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
     522,   523,   528,   530,   531,   536,   540,   542,   546,   550,
     556,   562,   570,   575,   576,   590,   591,   607,   608,   611,
     617,   619,   625,   627,   633,   635,   641,   643,   654,   661,
     663,   665,   667,   669,   671,   675,   677,   679,   680,   684,
     688,   692,   696,   698,   700,   702,   704,   706,   714,   720,
     722,   724,   726,   728,   732,   733,   739,   744,   745,   749,
     751,   753,   755,   757,   760,   762,   764,   766,   768,   770,
     772,   776,   780,   784,   786,   788,   792,   794,   796,   798,
     802,   804,   806,   808,   812,   816,   817,   823,   826,   827,
     831,   833,   837,   839,   843,   847,   849,   851,   855,   857,
     859,   861,   865,   867,   869,   871,   875,   879,   883,   887,
     891,   895,   899,   903,   905,   907,   909,   911,   915,   919,
     923,   927,   933,   941,   947,   956,   958,   962,   966,   972,
     978,   986,   994,  1001,  1007,  1008,  1011,  1017,  1021,  1033,
    1047,  1054,  1055,  1059,  1061,  1065,  1069,  1073,  1077,  1079,
    1081,  1085,  1089,  1093,  1097,  1101,  1105,  1107,  1109,  1111,
    1113,  1117,  1121,  1123,  1129,  1139,  1145,  1153,  1157,  1161,
    1167,  1177,  1183,  1189,  1195,  1196,  1210,  1211,  1213,  1221,
    1233,  1243,  1257,  1261,  1269,  1281,  1295,  1303,  1311,  1317,
    1325,  1331,  1341,  1349,  1355,  1365,  1371,  1377,  1381,  1389,
    1393,  1401,  1409,  1413,  1419,  1425,  1429,  1435,  1443,  1453,
    1455,  1457,  1459,  1461,  1463,  1464,  1467,  1469,  1473,  1475,
    1477,  1479,  1480,  1484,  1486,  1487,  1491,  1493,  1494,  1498,
    1500,  1502,  1504,  1506,  1508,  1510,  1512,  1514,  1516,  1520,
    1522,  1524,  1529,  1531,  1533,  1538,  1540,  1542,  1547,  1549,
    1554,  1560,  1562,  1566,  1568,  1572,  1574,  1576,  1581,  1591,
    1593,  1595,  1600,  1602,  1608,  1610,  1612,  1617,  1619,  1621,
    1627,  1629,  1631,  1633,  1638,  1640,  1642,  1648,  1650,  1652,
    1654,  1658,  1660,  1662,  1666,  1668,  1673,  1677,  1681,  1685,
    1689,  1693,  1697,  1699,  1701,  1705,  1707,  1711,  1712,  1714,
    1716,  1718,  1720,  1724,  1725,  1727,  1729,  1732,  1735,  1740,
    1747,  1752,  1759,  1766,  1773,  1780,  1783,  1790,  1799,  1808,
    1819,  1834,  1837,  1839,  1843,  1845,  1849,  1851,  1853,  1855,
    1857,  1859,  1861,  1863,  1865,  1867,  1869,  1871,  1873,  1875,
    1877,  1879,  1881,  1883,  1885,  1887,  1898
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     283,     0,    -1,    -1,   284,    -1,   285,    -1,   285,   284,
      -1,   286,   296,   298,    -1,    16,   220,   227,    -1,    15,
     220,   227,   219,     3,    -1,    17,   220,    11,   219,   396,
      -1,    17,   220,    10,   219,     3,    -1,    17,   220,    13,
      -1,    17,   220,    12,   219,     3,   219,     3,   219,     5,
     219,     5,   219,   434,   219,   295,   294,    -1,    18,   220,
     464,    -1,   145,   220,   146,   219,   147,    -1,   145,   220,
     146,    -1,    29,   220,     4,   219,   308,    -1,    21,   220,
     100,    -1,    -1,   219,   217,    -1,    -1,   219,   441,    -1,
      -1,   219,     3,    -1,     5,    -1,    81,    -1,    -1,   124,
     220,   297,    -1,   125,   219,   297,    -1,   125,    -1,    -1,
     300,   298,    -1,   225,   298,   226,    -1,   341,    -1,   287,
      -1,   288,    -1,   404,    -1,   291,    -1,   289,    -1,   412,
      -1,   413,    -1,   414,    -1,   290,    -1,   411,    -1,   410,
      -1,   408,    -1,   409,    -1,   415,    -1,   416,    -1,   417,
      -1,   418,    -1,   419,    -1,   385,    -1,   343,    -1,   306,
      -1,   305,    -1,   399,    -1,   355,    -1,   377,    -1,   421,
      -1,   422,    -1,   387,    -1,   388,    -1,   389,    -1,   420,
      -1,   321,    -1,   331,    -1,   333,    -1,   337,    -1,   335,
      -1,   318,    -1,   328,    -1,   314,    -1,   317,    -1,   380,
      -1,   390,    -1,   362,    -1,   378,    -1,   365,    -1,   371,
      -1,   400,    -1,   395,    -1,   383,    -1,   342,    -1,   401,
      -1,   402,    -1,   403,    -1,   405,    -1,   348,    -1,   346,
      -1,   394,    -1,   398,    -1,   397,    -1,   381,    -1,   382,
      -1,   384,    -1,   376,    -1,   379,    -1,   237,    -1,   239,
      -1,   241,    -1,   243,    -1,   245,    -1,   247,    -1,   249,
      -1,   251,    -1,   253,    -1,   236,    -1,   238,    -1,   240,
      -1,   242,    -1,   244,    -1,   246,    -1,   248,    -1,   250,
      -1,   252,    -1,   301,    -1,   302,    -1,   230,    -1,   230,
      -1,   302,    -1,   149,   220,   301,    -1,   303,   278,   453,
      -1,   303,   278,   208,   220,   462,    -1,   303,   278,   452,
      -1,   303,   278,   469,   220,   446,    -1,   303,   278,   468,
     220,   448,    -1,   303,   278,   467,   220,   450,    -1,   303,
     278,   441,    -1,   303,   278,   444,    -1,   303,   278,   225,
     312,   226,    -1,   303,   278,   225,   311,   226,    -1,   303,
     278,   225,   310,   226,    -1,   303,   278,   469,   220,   225,
     309,   226,    -1,   303,   278,   468,   220,   225,   308,   226,
      -1,   303,   278,   467,   220,   225,   307,   226,    -1,   303,
     278,   225,   313,   226,    -1,   451,    -1,   307,   219,   451,
      -1,   449,    -1,   308,   219,   449,    -1,   447,    -1,   309,
     219,   447,    -1,   445,    -1,   310,   219,   445,    -1,   442,
      -1,   311,   219,   442,    -1,   453,    -1,   312,   219,   453,
      -1,   452,    -1,   313,   219,   452,    -1,    -1,    -1,   199,
     229,   221,   315,   457,   222,   316,   299,    -1,   229,   221,
     460,   222,    -1,   148,    -1,    -1,     6,    -1,     6,    -1,
     223,   453,   201,   453,   224,    -1,   223,   453,   224,    -1,
      -1,    -1,   157,   322,   223,   440,   224,   323,   225,   324,
     226,    -1,    -1,   325,   324,    -1,    -1,   158,   465,   220,
     326,   298,    -1,    -1,   160,   220,   327,   298,    -1,   159,
      -1,   279,   279,    -1,   156,    -1,   155,   304,   278,   453,
     329,   453,    -1,    -1,   330,   332,   299,    -1,    -1,   154,
     223,   440,   224,   334,   299,    -1,    -1,   320,   220,   336,
     300,    -1,    -1,   135,   320,   338,   339,    -1,   299,    -1,
      -1,   299,   340,   136,   299,    -1,    14,   220,   452,    -1,
      71,    -1,    71,   220,   465,    -1,    71,   220,    81,    -1,
      59,   220,   344,   219,   344,    -1,    59,   220,   344,   219,
     465,    -1,   221,     4,   219,    80,   219,   359,   222,    -1,
     424,   319,   219,   434,    -1,    -1,   122,   220,   345,   219,
     352,   219,   354,   425,   428,   430,   432,   347,   299,    -1,
      -1,    51,   220,   345,   219,   351,   219,   353,   219,   354,
     425,   428,   430,   432,   349,   299,    -1,    -1,   219,     5,
      -1,   221,     4,   219,     4,   222,    -1,    81,    -1,   221,
       4,   219,     4,   222,    -1,    81,    -1,   221,   363,   219,
     364,   222,    -1,    81,    -1,   221,     4,   219,     4,   222,
      -1,    81,    -1,   134,   220,   356,   219,   433,   219,   357,
     219,   359,   360,    -1,    27,   220,   433,   219,   462,   360,
      -1,     5,    -1,    81,    -1,   358,    -1,    81,    -1,    80,
      -1,    80,   280,   358,    -1,     4,    -1,    81,    -1,    -1,
     360,   219,   361,    -1,   140,   220,   141,    -1,    77,   220,
     450,    -1,   120,   220,     4,    -1,   166,    -1,   111,    -1,
     112,    -1,   129,    -1,    20,    -1,    19,   220,   363,   219,
     364,   350,   228,    -1,    19,   220,   441,   350,   228,    -1,
      85,    -1,    86,    -1,    87,    -1,    86,    -1,    25,   220,
     367,    -1,    -1,    25,   220,   367,   366,   299,    -1,   448,
     219,   441,   368,    -1,    -1,   368,   219,   369,    -1,   452,
      -1,   126,    -1,   127,    -1,   436,    -1,   133,   452,    -1,
     167,    -1,   168,    -1,   164,    -1,   171,    -1,   172,    -1,
     173,    -1,   174,   220,   440,    -1,   175,   220,   440,    -1,
     179,   220,   440,    -1,   180,    -1,   181,    -1,   182,   220,
     370,    -1,   169,    -1,   170,    -1,   176,    -1,   177,   220,
     440,    -1,   178,    -1,   227,    -1,   183,    -1,   227,   280,
     370,    -1,    23,   220,   373,    -1,    -1,    24,   220,   373,
     372,   299,    -1,   450,   374,    -1,    -1,   374,   219,   375,
      -1,    74,    -1,   184,   220,   448,    -1,   466,    -1,   123,
     220,   452,    -1,   152,   220,   440,    -1,   153,    -1,    73,
      -1,   161,   220,   440,    -1,   198,    -1,    72,    -1,   162,
      -1,   163,   220,   440,    -1,   164,    -1,   165,    -1,   441,
      -1,   109,   220,   106,    -1,   108,   220,   107,    -1,   110,
     220,   440,    -1,   119,   220,   440,    -1,   120,   220,   440,
      -1,   150,   220,   151,    -1,   121,   220,   440,    -1,    77,
     220,   450,    -1,   166,    -1,   112,    -1,    79,    -1,    75,
      -1,   115,   220,   113,    -1,   116,   220,   114,    -1,    58,
     220,   440,    -1,   117,   220,   118,    -1,    26,   220,   423,
     219,   441,    -1,    28,   220,   441,   219,    80,   219,   433,
      -1,    33,   220,   441,   219,    80,    -1,    33,   220,   441,
     219,    80,   219,     5,   294,    -1,    34,    -1,    34,   220,
     462,    -1,    34,   220,   183,    -1,    47,   220,   441,   219,
      90,    -1,    48,   220,   441,   219,    90,    -1,    48,   220,
     471,   219,   471,   219,    90,    -1,    53,   220,   471,   219,
     471,   219,   227,    -1,    54,   220,   471,   219,   471,   386,
      -1,    55,   220,   471,   219,   471,    -1,    -1,   219,    90,
      -1,    62,   220,   462,   219,    94,    -1,    63,   220,   462,
      -1,    64,   220,   441,   219,   191,   220,   441,   219,    92,
     219,    91,    -1,    64,   220,   441,   219,    65,   219,    66,
     219,    67,   219,    92,   219,    91,    -1,    78,   220,   441,
     219,   188,   391,    -1,    -1,   391,   219,   392,    -1,    91,
      -1,   184,   220,   448,    -1,   185,   220,   450,    -1,   186,
     220,   396,    -1,   187,   220,   396,    -1,   189,    -1,   190,
      -1,   191,   220,   441,    -1,   192,   220,   423,    -1,   140,
     220,   393,    -1,    98,   220,   431,    -1,   193,   220,     4,
      -1,   120,   220,     4,    -1,     4,    -1,    92,    -1,   141,
      -1,    94,    -1,    69,   220,   462,    -1,    68,   220,   462,
      -1,     3,    -1,   221,     3,   219,   434,   222,    -1,   139,
     220,   444,   219,   446,   219,   446,   219,     7,    -1,   137,
     220,   462,   219,   446,    -1,   137,   220,   462,   219,   446,
     219,    97,    -1,    49,   220,   444,    -1,    50,   220,   444,
      -1,    39,   220,   227,   219,    40,    -1,    39,   220,   227,
     219,    40,   219,   184,   220,   448,    -1,    36,   220,   444,
     219,    38,    -1,    37,   220,   471,   219,    38,    -1,    22,
     220,   462,   219,    96,    -1,    -1,    35,   220,   444,   219,
     434,   219,   424,   425,   428,   430,   432,   406,   407,    -1,
      -1,   299,    -1,    44,   220,   441,   219,   435,   219,   437,
      -1,    44,   220,   441,   219,   435,   219,   437,   219,   184,
     220,   448,    -1,    44,   220,   441,   219,   435,   219,   437,
     219,    89,    -1,    44,   220,   441,   219,   435,   219,   437,
     219,    89,   219,   184,   220,   448,    -1,    45,   220,   441,
      -1,    45,   220,   441,   219,   184,   220,   448,    -1,   102,
     220,   441,   219,   103,   219,     4,   219,   104,   219,   105,
      -1,   102,   220,   441,   219,   103,   219,     4,   219,   104,
     219,   105,   219,   434,    -1,   102,   220,   441,   219,   103,
     219,   104,    -1,    99,   220,   462,   219,   100,   219,    97,
      -1,   128,   220,   462,   219,     4,    -1,   128,   220,   462,
     219,   100,   219,    97,    -1,   130,   220,   444,   219,   132,
      -1,   131,   220,   462,   219,   132,   219,     4,   219,     4,
      -1,   131,   220,   462,   219,   132,   219,     4,    -1,    46,
     220,    93,   219,   441,    -1,    46,   220,    93,   219,   441,
     219,   184,   220,   448,    -1,   194,   220,   441,   219,   452,
      -1,   194,   220,   441,   219,    81,    -1,   194,   220,   441,
      -1,   195,   220,   441,   219,   142,   219,   434,    -1,   195,
     220,   441,    -1,   196,   220,   441,   219,   143,   219,   452,
      -1,   196,   220,   441,   219,   143,   219,    81,    -1,   196,
     220,   441,    -1,   197,   220,   441,   219,   144,    -1,   197,
     220,   441,   219,   146,    -1,   197,   220,   441,    -1,    60,
     220,   453,   219,   441,    -1,    61,   220,   441,   219,   470,
     219,   452,    -1,    57,   220,   440,   219,   440,   219,   440,
     219,   440,    -1,    57,    -1,   227,    -1,    81,    -1,   227,
      -1,    81,    -1,    -1,   219,   426,    -1,   427,    -1,   427,
     219,   426,    -1,    41,    -1,    42,    -1,    43,    -1,    -1,
     101,   220,   429,    -1,   100,    -1,    -1,    98,   220,   431,
      -1,    97,    -1,    -1,   184,   220,   448,    -1,    72,    -1,
      81,    -1,    73,    -1,    81,    -1,    84,    -1,   438,    -1,
      81,    -1,    84,    -1,   438,    -1,    83,   220,    81,    -1,
      88,    -1,    81,    -1,    83,   223,     4,   224,    -1,   227,
      -1,   238,    -1,   239,   223,   453,   224,    -1,   453,    -1,
     442,    -1,   210,   221,   462,   222,    -1,   242,    -1,   243,
     223,   453,   224,    -1,   221,     4,   219,     4,   222,    -1,
      81,    -1,    82,   443,   224,    -1,   218,    -1,   218,   219,
     443,    -1,   445,    -1,   244,    -1,   245,   223,   453,   224,
      -1,   221,     4,   219,     4,   219,     4,   219,     4,   222,
      -1,   447,    -1,   246,    -1,   247,   223,   453,   224,    -1,
       3,    -1,   221,     3,   219,   434,   222,    -1,   449,    -1,
     248,    -1,   249,   223,   453,   224,    -1,   227,    -1,     3,
      -1,   221,     3,   219,   227,   222,    -1,    81,    -1,   451,
      -1,   250,    -1,   251,   223,   453,   224,    -1,   227,    -1,
       3,    -1,   221,     3,   219,   227,   222,    -1,    81,    -1,
      95,    -1,   439,    -1,   452,   279,   439,    -1,     4,    -1,
     463,    -1,   221,     8,   222,    -1,   236,    -1,   237,   223,
     453,   224,    -1,   453,   273,   453,    -1,   453,   274,   453,
      -1,   453,   275,   453,    -1,   453,   276,   453,    -1,   453,
     277,   453,    -1,   221,   453,   222,    -1,   232,    -1,   233,
      -1,   303,   220,   454,    -1,   455,    -1,   456,   219,   455,
      -1,    -1,   456,    -1,   453,    -1,   452,    -1,   458,    -1,
     459,   219,   458,    -1,    -1,   459,    -1,   441,    -1,   203,
     444,    -1,   204,   444,    -1,   205,   441,   219,   441,    -1,
     206,   441,   219,   441,   219,   453,    -1,   207,   221,   462,
     222,    -1,   207,   221,   358,   219,   462,   222,    -1,   213,
     221,     7,   219,   462,   222,    -1,   213,   221,   462,   219,
     462,   222,    -1,   213,   221,   446,   219,   462,   222,    -1,
     209,   441,    -1,   211,   221,   441,   219,   453,   222,    -1,
     211,   221,   441,   219,   453,   219,    41,   222,    -1,   212,
     221,   441,   219,   453,   219,   453,   222,    -1,   212,   221,
     441,   219,   453,   219,   453,   219,    41,   222,    -1,   215,
     221,   216,   219,   221,   453,   274,   453,   292,   222,   219,
     441,   293,   222,    -1,   214,   461,    -1,   252,    -1,   221,
     462,   222,    -1,   461,    -1,   461,   281,   462,    -1,   272,
      -1,    96,    -1,     4,    -1,     8,    -1,     9,    -1,     4,
      -1,     8,    -1,     9,    -1,     4,    -1,   463,    -1,    30,
      -1,    23,    -1,    31,    -1,    25,    -1,    32,    -1,   137,
      -1,    76,    -1,    81,    -1,   472,    -1,    56,   221,     4,
     219,     4,   219,     4,   219,     4,   222,    -1,   221,     4,
     219,     4,   219,     4,   219,     4,   222,    -1
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
    1193,  1192,  1218,  1229,  1228,  1256,  1262,  1267,  1272,  1279,
    1286,  1295,  1303,  1315,  1314,  1338,  1337,  1361,  1364,  1370,
    1380,  1386,  1395,  1401,  1406,  1412,  1417,  1423,  1434,  1440,
    1441,  1444,  1445,  1448,  1452,  1458,  1459,  1463,  1469,  1477,
    1482,  1487,  1492,  1497,  1502,  1507,  1515,  1522,  1530,  1538,
    1539,  1542,  1543,  1546,  1551,  1550,  1564,  1571,  1578,  1586,
    1591,  1597,  1603,  1609,  1615,  1620,  1625,  1630,  1635,  1640,
    1645,  1650,  1655,  1660,  1665,  1670,  1676,  1681,  1686,  1691,
    1696,  1703,  1712,  1716,  1729,  1738,  1737,  1755,  1765,  1771,
    1779,  1785,  1790,  1795,  1800,  1805,  1810,  1815,  1820,  1825,
    1839,  1845,  1850,  1855,  1860,  1865,  1870,  1875,  1880,  1885,
    1890,  1895,  1900,  1905,  1910,  1915,  1920,  1926,  1931,  1936,
    1941,  1948,  1954,  1983,  1988,  1996,  2002,  2006,  2014,  2021,
    2028,  2038,  2048,  2064,  2075,  2078,  2084,  2090,  2096,  2100,
    2106,  2113,  2119,  2127,  2133,  2138,  2143,  2148,  2153,  2159,
    2165,  2170,  2175,  2180,  2185,  2190,  2197,  2197,  2197,  2197,
    2200,  2206,  2212,  2217,  2224,  2231,  2235,  2241,  2247,  2253,
    2258,  2265,  2271,  2281,  2288,  2287,  2319,  2322,  2328,  2333,
    2338,  2343,  2349,  2353,  2359,  2363,  2367,  2373,  2379,  2383,
    2389,  2395,  2399,  2405,  2409,  2415,  2419,  2424,  2431,  2435,
    2442,  2446,  2451,  2458,  2462,  2467,  2475,  2481,  2488,  2492,
    2499,  2507,  2510,  2520,  2524,  2527,  2533,  2537,  2544,  2548,
    2552,  2559,  2562,  2568,  2575,  2578,  2584,  2591,  2595,  2602,
    2603,  2606,  2607,  2610,  2611,  2612,  2618,  2619,  2620,  2626,
    2627,  2630,  2639,  2644,  2651,  2662,  2668,  2672,  2676,  2683,
    2693,  2700,  2704,  2710,  2714,  2722,  2726,  2733,  2743,  2756,
    2760,  2767,  2777,  2786,  2797,  2801,  2808,  2818,  2829,  2838,
    2848,  2854,  2858,  2865,  2875,  2886,  2895,  2905,  2909,  2916,
    2917,  2923,  2927,  2931,  2935,  2943,  2952,  2956,  2960,  2964,
    2968,  2972,  2975,  2982,  2991,  3024,  3025,  3028,  3029,  3032,
    3036,  3043,  3050,  3061,  3064,  3072,  3076,  3080,  3084,  3088,
    3093,  3097,  3101,  3106,  3111,  3116,  3120,  3125,  3130,  3134,
    3138,  3143,  3147,  3154,  3160,  3164,  3170,  3177,  3178,  3181,
    3182,  3183,  3186,  3190,  3194,  3198,  3204,  3205,  3208,  3209,
    3212,  3213,  3216,  3217,  3220,  3224,  3250
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
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,    44,
      58,    40,    41,    91,    93,   123,   125,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,    43,    45,    42,    47,    37,    61,    46,
     124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   282,   283,   283,   284,   284,   285,   286,   286,   287,
     287,   287,   287,   288,   289,   289,   290,   291,   292,   292,
     293,   293,   294,   294,   295,   295,   296,   296,   297,   297,
     298,   298,   299,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   303,   303,   303,   304,
     304,   305,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   307,   307,   308,
     308,   309,   309,   310,   310,   311,   311,   312,   312,   313,
     313,   315,   316,   314,   317,   318,   319,   319,   320,   320,
     320,   322,   323,   321,   324,   324,   326,   325,   327,   325,
     328,   329,   329,   330,   332,   331,   334,   333,   336,   335,
     338,   337,   339,   340,   339,   341,   342,   342,   342,   343,
     343,   344,   345,   347,   346,   349,   348,   350,   350,   351,
     351,   352,   352,   353,   353,   354,   354,   355,   355,   356,
     356,   357,   357,   358,   358,   359,   359,   360,   360,   361,
     361,   361,   361,   361,   361,   361,   362,   362,   362,   363,
     363,   364,   364,   365,   366,   365,   367,   368,   368,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   370,   370,   370,   371,   372,   371,   373,   374,   374,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   376,   377,   378,   378,   379,   379,   379,   380,   381,
     382,   383,   384,   385,   386,   386,   387,   388,   389,   389,
     390,   391,   391,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   393,   393,   393,   393,
     394,   395,   396,   396,   397,   398,   398,   399,   400,   401,
     401,   402,   403,   404,   406,   405,   407,   407,   408,   408,
     408,   408,   409,   409,   410,   410,   410,   411,   412,   412,
     413,   414,   414,   415,   415,   416,   416,   416,   417,   417,
     418,   418,   418,   419,   419,   419,   420,   421,   422,   422,
     423,   423,   424,   424,   425,   425,   426,   426,   427,   427,
     427,   428,   428,   429,   430,   430,   431,   432,   432,   433,
     433,   434,   434,   435,   435,   435,   436,   436,   436,   437,
     437,   438,   439,   439,   439,   440,   441,   441,   441,   441,
     442,   442,   442,   443,   443,   444,   444,   444,   445,   446,
     446,   446,   447,   447,   448,   448,   448,   449,   449,   449,
     449,   450,   450,   450,   451,   451,   451,   451,   451,   452,
     452,   453,   453,   453,   453,   453,   453,   453,   453,   453,
     453,   453,   454,   454,   455,   456,   456,   457,   457,   458,
     458,   459,   459,   460,   460,   461,   461,   461,   461,   461,
     461,   461,   461,   461,   461,   461,   461,   461,   461,   461,
     461,   461,   461,   461,   462,   462,   463,   464,   464,   465,
     465,   465,   466,   466,   466,   466,   467,   467,   468,   468,
     469,   469,   470,   470,   471,   471,   472
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
       0,     4,     1,     0,     4,     3,     1,     3,     3,     5,
       5,     7,     4,     0,    13,     0,    15,     0,     2,     5,
       1,     5,     1,     5,     1,     5,     1,    10,     6,     1,
       1,     1,     1,     1,     3,     1,     1,     0,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     7,     5,     1,
       1,     1,     1,     3,     0,     5,     4,     0,     3,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     1,     1,     3,     1,     1,     1,     3,
       1,     1,     1,     3,     3,     0,     5,     2,     0,     3,
       1,     3,     1,     3,     3,     1,     1,     3,     1,     1,
       1,     3,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     3,     3,     3,
       3,     5,     7,     5,     8,     1,     3,     3,     5,     5,
       7,     7,     6,     5,     0,     2,     5,     3,    11,    13,
       6,     0,     3,     1,     3,     3,     3,     3,     1,     1,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       3,     3,     1,     5,     9,     5,     7,     3,     3,     5,
       9,     5,     5,     5,     0,    13,     0,     1,     7,    11,
       9,    13,     3,     7,    11,    13,     7,     7,     5,     7,
       5,     9,     7,     5,     9,     5,     5,     3,     7,     3,
       7,     7,     3,     5,     5,     3,     5,     7,     9,     1,
       1,     1,     1,     1,     0,     2,     1,     3,     1,     1,
       1,     0,     3,     1,     0,     3,     1,     0,     3,     1,
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
       2,     0,     0,     0,     3,     4,    26,     0,     0,     1,
       5,     0,    30,     0,     7,     0,   158,     0,     0,     0,
       0,   226,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   389,     0,
       0,     0,     0,     0,     0,     0,     0,   186,     0,     0,
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
     461,     0,   464,     0,   506,     0,   462,   483,    31,     0,
     178,     0,     8,     0,   422,   423,     0,   459,   185,     0,
       0,     0,    11,   508,   507,    13,   431,     0,   229,   230,
       0,     0,   428,     0,     0,   197,   426,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     502,   485,   504,     0,   455,   457,   458,     0,   454,   452,
       0,   264,   268,   451,   265,   448,   450,     0,   447,   445,
       0,   233,     0,   444,   391,   390,     0,   409,   410,     0,
       0,     0,     0,   307,   306,     0,   436,     0,     0,   435,
       0,     0,     0,     0,   524,     0,     0,   362,     0,     0,
       0,     0,     0,   347,   348,   393,   392,     0,   156,     0,
       0,     0,     0,   425,     0,     0,     0,     0,     0,   317,
       0,   341,   340,   511,   509,   510,   188,   187,     0,     0,
       0,     0,     0,     0,     0,   209,   210,     0,     0,     0,
       0,    15,   121,     0,     0,     0,   377,   379,   382,   385,
     151,     0,     0,     0,     0,   160,     0,     0,     0,     0,
       0,   480,   479,   481,   484,     0,   517,   519,   516,   518,
     520,   521,     0,     0,     0,   128,   129,   124,   122,     0,
       0,     0,     0,    30,   175,    28,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,   486,
     487,     0,     0,     0,   495,     0,     0,     0,   501,     0,
       0,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   157,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   182,   181,     0,     0,     0,   176,     0,     0,
       0,     0,     0,     0,   477,   463,   471,     0,     0,   466,
     467,   468,   469,   470,     0,   154,     0,   461,     0,     0,
       0,     0,   145,   143,   149,   147,     0,     0,     0,   179,
       0,     0,   460,    10,   342,     0,     9,     0,     0,   432,
       0,     0,     0,   232,   231,   197,   198,   228,     0,     0,
     213,     0,     0,     0,     0,   442,     0,     0,   440,     0,
       0,   439,     0,     0,   503,   505,   353,     0,     0,     0,
     266,     0,     0,   235,   237,   301,   217,     0,    16,   139,
     303,     0,     0,   411,   412,     0,   351,     0,     0,   352,
     349,   415,     0,   413,     0,   414,     0,   373,   308,     0,
     309,     0,   200,     0,     0,     0,     0,   314,   313,     0,
       0,   189,   190,   386,   522,   523,     0,   316,     0,     0,
     321,     0,     0,   202,     0,     0,   368,     0,   370,     0,
       0,     0,     0,   345,     0,    14,     0,   172,     0,     0,
     162,   376,   375,     0,     0,   383,   384,     0,   475,   478,
       0,   465,   159,   482,   123,     0,     0,   132,     0,   131,
       0,   130,     0,   136,     0,   127,     0,   126,     0,   125,
      32,   424,     0,     0,   434,   427,     0,   429,     0,   488,
       0,     0,     0,   490,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   514,   512,   513,     0,   279,   276,
     270,   296,     0,   295,     0,     0,     0,   294,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   275,     0,   280,
       0,   282,   283,   293,     0,   278,   269,   284,   515,   272,
       0,   446,   236,   208,     0,     0,     0,     0,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   192,     0,     0,   312,     0,     0,     0,     0,     0,
     320,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   177,   171,   173,     0,     0,     0,     0,     0,   152,
       0,   144,   146,   148,   150,     0,   137,     0,     0,   141,
       0,     0,   430,   227,     0,   214,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   456,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   449,     0,     0,   302,   140,    22,     0,
     394,     0,     0,     0,     0,   420,   419,   358,   363,     0,
     310,     0,   204,     0,     0,   311,   315,     0,     0,   387,
       0,     0,     0,   367,     0,   366,     0,   206,     0,   394,
     369,   372,   212,     0,   211,   184,   346,     0,   164,   378,
     381,   380,   472,   473,   474,   476,     0,     0,   135,   134,
       0,   133,     0,     0,   489,   491,     0,   496,     0,   492,
       0,   441,   494,   493,     0,   299,   292,   286,   285,   287,
     297,   298,   300,   288,   289,   291,   273,   290,   274,   277,
     281,   271,     0,   416,   240,   241,     0,   246,   244,   245,
     256,   257,   247,   248,   249,     0,     0,   258,     0,   260,
       0,   253,   254,     0,   238,   242,   417,   239,     0,   223,
     224,     0,   225,     0,   222,   218,     0,   304,     0,     0,
     401,     0,     0,     0,   421,     0,     0,     0,     0,     0,
       0,   215,   216,     0,     0,     0,   323,     0,     0,     0,
       0,     0,     0,     0,   328,   329,     0,     0,     0,   322,
       0,     0,     0,   401,     0,     0,     0,     0,     0,     0,
     164,   153,   138,   142,   343,     0,     0,     0,   443,     0,
       0,   243,     0,     0,     0,     0,     0,     0,     0,     0,
      23,     0,   398,   399,   400,   395,   396,     0,   404,     0,
       0,   350,   360,     0,   374,   199,     0,   394,   388,   191,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   201,     0,   404,   371,   217,   344,     0,
     168,   163,   165,     0,   497,     0,   498,   467,   418,   250,
     251,   259,   252,   262,   261,   255,   220,   221,   219,     0,
       0,     0,     0,   407,     0,     0,     0,     0,     0,   401,
       0,     0,   406,   333,   335,   336,   337,   339,   338,   332,
     324,   325,   326,   327,   330,   331,   334,     0,     0,   407,
     207,   166,    30,     0,     0,     0,     0,     0,   438,   397,
     403,   402,     0,     0,   354,     0,   526,     0,   359,   203,
     404,     0,   318,   364,   205,   193,    30,   169,     0,   499,
      19,     0,   263,   405,     0,   356,   525,     0,   407,     0,
       0,     0,   167,     0,     0,   408,   357,   355,   361,   195,
     319,   365,   194,     0,    20,     0,    24,    25,    22,     0,
       0,   196,    12,    21,   500
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,   102,   103,   104,   105,   106,
    1116,  1170,   957,  1168,    12,   176,   107,   424,   108,   109,
     110,   111,   233,   112,   113,   795,   588,   798,   528,   529,
     530,   531,   114,   514,   886,   115,   116,   483,   117,   118,
     234,   784,   999,  1000,  1136,  1112,   119,   649,   120,   121,
     251,   122,   646,   123,   422,   124,   378,   503,   641,   125,
     126,   127,   355,   347,   128,  1151,   129,  1165,   438,   614,
     635,   854,   869,   130,   377,   873,   561,   973,   743,   955,
     131,   274,   555,   132,   459,   311,   742,   944,  1075,   133,
     456,   301,   455,   736,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   764,   144,   145,   146,   147,   770,
     989,  1099,   148,   149,   546,   150,   151,   152,   153,   154,
     155,   156,   157,   158,  1145,  1157,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   316,   348,   960,  1025,  1026,  1028,  1121,  1083,  1093,
    1124,   319,   595,   604,   945,   847,   605,   257,   352,   291,
     276,   432,   328,   329,   570,   571,   312,   313,   302,   303,
     401,   353,   884,   658,   659,   660,   403,   404,   405,   292,
     450,   246,   265,   367,   739,   419,   420,   421,   626,   333,
     334
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1015
static const yytype_int16 yypact[] =
{
     207,   -95,   -79,   146, -1015,   207,    50,   -50,   -45, -1015,
   -1015,   -15,   814,     9, -1015,    87, -1015,    17,    35,    40,
      49, -1015,    64,    72,    88,    98,   109,   111,   128,   134,
     137,   145,   158,   166,   169,   179,   181,   183,   185,   193,
     202,   205,   208,   210,   211,   221,   223,   224,   225,   228,
     233,   234,   239,   240,   246,   261,   264,   266,   267,   269,
     288,   289,   306,   310,   311,   312,    45,   313,   322,   325,
   -1015,   327,     8,   908, -1015, -1015,   329,   337,   339,   363,
      10,    11,    93, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,   814, -1015,
   -1015,    74, -1015, -1015, -1015, -1015, -1015,   365, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015,   324,   214, -1015,   -99,   462,    71,
     -17,   383,   865,   132,   132,   139,   -35,    27,   -27,   433,
     -27,   774,  -125,  -125,   -12,   285,   -27,   -27,   421,   -27,
     125,  -125,  -125,   -31,   -12,   -12,   -12,    11,   330,    11,
     -27,   865,   865,   -27,   865,   865,    77,   -27,   865,   -27,
     -31,   865,  -125,   865,    79, -1015,   865,  -125,   369,   309,
      11, -1015, -1015,   247,   364,   -27,   -27,   -27,   -27,   367,
   -1015,    41, -1015,   368, -1015,   203, -1015,    37, -1015,   213,
   -1015,   361, -1015,    87, -1015, -1015,   374, -1015,   333,   382,
     397,   400, -1015, -1015, -1015, -1015, -1015,   402, -1015, -1015,
     406,   624, -1015,   410,   416,   418, -1015, -1015,  -125,  -125,
     -27,   -27,   435,   -27,   436,   437,   438,   865,   442,   292,
   -1015, -1015,   380,   449, -1015, -1015, -1015,   667, -1015, -1015,
     448, -1015, -1015, -1015, -1015, -1015, -1015,   672, -1015, -1015,
     464,   463,   470, -1015, -1015, -1015,   471, -1015, -1015,   473,
     475,   476,   487, -1015, -1015,   707, -1015,   489,   505, -1015,
     506,   513,   722,   516, -1015,   517,   520,   521,   528,   530,
     734,   531,   532, -1015, -1015, -1015, -1015,   533,   747,   537,
     543,   544,   545,   349,   761,   547,   263,   549,   551, -1015,
     555, -1015, -1015, -1015, -1015, -1015, -1015, -1015,   558,   560,
     561,   562,   568,   569,   575, -1015, -1015,   576,   361,   577,
     578,   579, -1015,   552,    11,    11,   582,   583,   584,   585,
   -1015,   588,   294,    11,    11, -1015,    11,    11,    11,    11,
      11,   333,   349, -1015,   599,   597, -1015, -1015, -1015, -1015,
   -1015, -1015,   604,    89,   103, -1015, -1015,   333,   349,   605,
     606,   607,   814,   814, -1015, -1015,    11,   -99,   819,    19,
     827,   627,   628,   865,   638,    11,   277,   861,   651, -1015,
   -1015,   661,   662,   691, -1015,   -27,   -27,   439, -1015,   668,
     664,   865,   791,   669,    11,   670,   361,   671,    11,   361,
     -27,   -27,   865,   811,    32,   813,   680,    11,    43,   869,
     904,   690,   872,   871,   106,   730,   -27,   825,   698,   828,
     -12,   -19, -1015,   700,   -12,   -12,   -12,    11,   701,    28,
     -27,    57,   832,   -13,   739,   829,   834,   -18,    62,   796,
     798,    27,   795, -1015,   124,   124,   788, -1015,   135,   714,
     -28,   797,   807,    51,   853, -1015, -1015,   120,   423,   -42,
     -42, -1015, -1015, -1015,    37, -1015,   865,   721,  -121,   -97,
     -89,   -72, -1015, -1015,   333,   349,    36,   107,    25, -1015,
     715,   440, -1015, -1015, -1015,   949, -1015,   735,   402, -1015,
     733,   952,   455, -1015, -1015,   418, -1015, -1015,   -27,   -27,
     678,   741,   742,   746,   748, -1015,   751,   602, -1015,   738,
     753, -1015,   755,   757, -1015, -1015, -1015,   764,   469,   557,
   -1015,   765,   484, -1015, -1015, -1015, -1015,   763,   775, -1015,
     777,   962,   509, -1015, -1015,   778, -1015,   779,   989, -1015,
     780, -1015,   782, -1015,   781, -1015,   783,   787, -1015,   997,
   -1015,   793, -1015,   998,   799,    43,   800,   801, -1015,   802,
     927, -1015, -1015, -1015, -1015, -1015,   803, -1015,   804,   794,
   -1015,   805,   806, -1015,  1011,   808, -1015,   810, -1015,   812,
     815,   894,  1029,   816,   817, -1015,   361, -1015,   754,    11,
   -1015, -1015,   333,   821,   822, -1015, -1015,   826, -1015,   823,
     851, -1015, -1015, -1015, -1015,  1041,   863, -1015,   -21, -1015,
      11, -1015,   -99, -1015,    85, -1015,    32, -1015,    30, -1015,
   -1015, -1015,   830,  1044, -1015, -1015,   887, -1015,   820, -1015,
     866,  1007,   865, -1015,    11,    11,   865,   891,    11,   865,
     865,   890,   892, -1015, -1015, -1015, -1015,   893, -1015, -1015,
   -1015, -1015,   895, -1015,   896,   898,   900, -1015,   901,   919,
     920,   921,   922,   923,   925,   929,   931, -1015,   933, -1015,
     935, -1015, -1015, -1015,   937, -1015, -1015, -1015, -1015, -1015,
     939, -1015,   928,   940,    27,    32,  1107,   943, -1015,   -31,
    1159,   945,   981,  1162,    68,   139,   983,  -119,  1078,   950,
      -7, -1015,   944,  1080, -1015,    11,   953,   -99,  1108,   -27,
     954,  1079,    44,   956,    -2,  1081,  1173,   304,   361,  1082,
     124, -1015, -1015,   349,   958,    43,    -9,   159,   853, -1015,
    -113, -1015, -1015,   349,   333,   -69, -1015,   -51,   -40, -1015,
      43,   961, -1015, -1015,    11, -1015,   959,   217,   305,   963,
      43,   648,   964,   965,    11, -1015,    11,   132,  1077,  1083,
      11,  1075,  1076,  1073,    11,    11,    11,   -99,  1045,    11,
      11,    11,   139, -1015,   955,   295, -1015, -1015,   973,  1191,
     978,   979,  1195,   980,   977, -1015, -1015,   984, -1015,   982,
   -1015,  1200, -1015,   331,   986, -1015, -1015,   987,    90,   333,
     988,   990,   278, -1015,   991, -1015,  1204, -1015,  1207,   978,
   -1015,   993, -1015,   994, -1015, -1015, -1015,   995,    94, -1015,
   -1015,   333, -1015, -1015, -1015, -1015,   361,    85, -1015, -1015,
      30, -1015,   996,  1210,   349, -1015,  1175, -1015,    11, -1015,
     999, -1015, -1015, -1015,   428, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015,   333, -1015, -1015, -1015,
   -1015, -1015,   -68, -1015, -1015, -1015,   -99, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015,  1000,  1002, -1015,  1003, -1015,
    1004, -1015, -1015,  1005, -1015, -1015, -1015,   333,  1006, -1015,
   -1015,  1008, -1015,  1009, -1015, -1015,  1214, -1015,  1012,   259,
    1118,  1223,  1013,   139, -1015,   -11,   139,  1014,  1015,    -2,
      11, -1015, -1015,  1016,  1163,  1141, -1015,  1017,  1019,  1020,
    1021,  1022,  1023,  1024, -1015, -1015,  1025,  1026,  1027, -1015,
    1131,  1028,  1030,  1118,  1244,    90,  1245,   519,  1031,  1032,
      94, -1015, -1015, -1015, -1015,  1034,  1033,   300, -1015,    11,
    1176,   333,    11,    11,    11,    11,   -91,   132,  1250,  1115,
   -1015,  1255, -1015, -1015, -1015, -1015,  1042,  1040,  1164,  1046,
    1259, -1015,  1047,  1048, -1015, -1015,   277,   978, -1015, -1015,
    1050,  1051,  1167,  1263,    67,   139,   132,    19,    19,   -27,
     -35,  1267,  1053, -1015,  1269,  1164, -1015, -1015, -1015,  1054,
   -1015, -1015, -1015,  1270, -1015,  1235, -1015,  -132, -1015, -1015,
   -1015, -1015, -1015, -1015,  1001, -1015, -1015, -1015, -1015,  1055,
     259,  1178,  1059,  1096,  1278,  1061,  1100,   139,  1063,  1118,
    1194,  1196, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015,  1183,  1067,  1096,
     940, -1015,   814,  1071,  1069,  1084,  1070,   -91, -1015, -1015,
   -1015, -1015,  1167,  1074, -1015,  1085, -1015,  1086, -1015, -1015,
    1164,  1089, -1015,  1090, -1015, -1015,   814, -1015,    43, -1015,
   -1015,  1091, -1015, -1015,   139,   361, -1015,   139,  1096,  1202,
      43,   361, -1015,  1092,   -27, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015,    84,  1093,   361, -1015, -1015,   973,   -27,
    1094, -1015, -1015, -1015, -1015
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1015, -1015,  1290, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015,   129, -1015, -1015,  1043,  -108,  -376,   876,  1088,
    1226,  -498, -1015, -1015, -1015, -1015,   626, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,  1234, -1015,
   -1015, -1015,   303, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015,   824,  1095, -1015, -1015, -1015, -1015,   749, -1015,
   -1015, -1015,   336, -1015, -1015, -1015,  -624,   319,   262, -1015,
   -1015,   465,   284, -1015, -1015, -1015, -1015, -1015,   204, -1015,
   -1015,  1138, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015,  -628, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015,   273,   580,  -839,   244, -1015,  -920, -1015,  -979,   206,
   -1014,  -459,  -609, -1015, -1015, -1015,   491,   899,  -222,  -179,
    -388,   784,   362,  -387,  -491,  -649,  -534,  -452,  -529,  -631,
    -172,   -77, -1015,   539, -1015, -1015,   809, -1015, -1015,  1049,
    -100,   752, -1015,  -479, -1015, -1015, -1015, -1015, -1015,  -181,
   -1015
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -235
static const yytype_int16 yytable[] =
{
     248,   275,   502,   677,   245,   258,   761,   675,   383,   320,
     622,   322,   589,   643,   644,   240,   657,   336,   337,   342,
     339,   341,   544,   349,   350,   351,   532,   533,   565,   799,
     993,   357,   363,   565,   360,   305,   364,   365,   368,   294,
     370,   240,   640,   796,   331,   240,   314,   679,   864,   391,
     345,    16,   628,   651,   266,   267,   386,   387,   388,   389,
     266,   267,   612,   633,   266,   267,   636,   805,   268,   269,
     415,  1095,   880,  1055,   852,   263,  1109,   417,  1032,   867,
     580,   363,   293,   583,   375,   364,   365,  1115,   294,  1166,
     -18,   324,  1073,   527,   971,  1135,   325,   391,   666,   317,
     842,   441,   442,   802,   444,   667,   839,   240,   318,   802,
     305,   358,   359,   306,   361,   362,   593,   295,   369,   326,
     327,   372,   668,   374,   594,     7,   379,   565,   254,   669,
     670,   296,   356,   624,  1159,   294,  1074,   671,   625,   255,
     256,     8,   305,   398,   399,   400,     9,   672,   865,   845,
     887,  1148,  1010,   874,   673,   753,   846,   888,   366,  1096,
     376,  1097,   637,   509,   392,  1167,   295,   264,   745,  1130,
     402,   972,   418,  1033,    11,   889,   879,    13,   629,   890,
     296,   331,    14,   270,   266,   267,   891,   601,   306,   602,
     603,   892,   315,   270,   271,   655,   346,   656,  1089,   254,
     271,   900,   613,   634,   271,    15,   266,   267,  1098,   332,
     255,   256,   175,   295,   853,   272,   273,   240,   254,   868,
     306,   848,     1,     2,   589,   272,   273,   296,   174,   255,
     256,   230,   241,   398,   399,   400,   406,   177,   407,   239,
     545,  1003,   534,   408,   409,   410,   642,   242,   243,   354,
     678,   642,   997,   307,   998,   178,  1002,   297,   241,   308,
     179,   674,   241,   298,   254,   619,   563,   564,    81,   180,
     781,   568,   569,   242,   243,   255,   256,   242,   243,   791,
     792,   584,   585,   244,   181,   836,   299,   300,   906,   877,
     657,   647,   182,   837,   266,   267,   434,   607,   921,   611,
    1022,  1023,  1024,   616,   617,   618,   297,   508,   183,   244,
     241,   623,   298,   244,   247,   540,   517,   518,   184,   519,
     520,   521,   522,   523,   413,   242,   243,   252,   307,   185,
     254,   186,   676,   550,   308,   270,   392,   535,   652,   242,
     243,   255,   256,   562,   661,   642,   340,   572,   187,   541,
     411,   575,   249,   297,   188,   309,   310,   189,   552,   298,
     307,   244,   586,   553,   554,   190,   308,   272,   273,   976,
     568,   569,   948,   266,   267,   244,   977,   578,   191,   689,
     690,   582,   299,   300,   560,   872,   192,   309,   310,   193,
     592,   882,   883,   396,   397,   398,   399,   400,   978,   194,
     737,   195,   875,   196,   394,   197,   949,   950,   396,   397,
     398,   399,   400,   198,   648,   951,   268,   269,   979,  1102,
    1103,   412,   199,   270,   952,   200,   664,   395,   201,  1031,
     202,   203,  1034,   253,   413,   953,   896,   321,   414,   897,
     254,   204,   565,   205,   206,   207,   566,   402,   208,   242,
     243,   255,   256,   209,   210,   272,   273,   326,   327,   211,
     212,   954,   980,   981,   982,   983,   213,   984,   985,   986,
     987,   988,   259,   260,   261,   262,   396,   397,   398,   399,
     400,   214,   490,   277,   215,   244,   216,   217,  1076,   218,
     396,   397,   398,   399,   400,   278,   279,   280,   281,   282,
     794,   283,   270,   284,   285,   286,   287,   288,   219,   220,
    1001,  1100,   335,   289,   338,   381,   516,  1101,  1059,  1065,
     266,   267,  1066,   363,   898,   384,   221,   364,   365,  1153,
     222,   223,   224,   226,   272,   273,   396,   397,   398,   399,
     400,  1161,   227,   857,   290,   228,    85,   229,    87,   235,
      89,   354,    91,  1128,    93,   330,    95,   236,    97,   237,
      99,   704,   101,   343,   344,   705,   706,   396,   397,   398,
     399,   400,   783,   396,   397,   398,   399,   400,   396,   397,
     398,   399,   400,   238,   373,   250,   423,   385,   390,   380,
     861,   393,   806,   793,   905,   859,   809,   426,   909,   812,
     813,   428,   913,   914,   915,   697,   434,   918,   919,   920,
    1155,   416,   427,  1158,   881,   707,   429,   807,   808,   430,
     431,   811,   396,   397,   398,   399,   400,   433,   434,   708,
     709,   710,   711,   435,   712,   436,   713,   437,   266,   267,
     439,   440,   278,   279,   280,   281,   282,   662,   283,   270,
     284,   285,   286,   287,   288,   916,   443,   445,   446,   447,
     567,   451,   947,   449,   681,   714,   715,   716,   452,   717,
     453,   454,   718,   719,   720,   457,   721,   722,   723,   687,
     724,   272,   273,   266,   267,   568,   569,   458,  -234,   460,
     461,   290,   462,   703,   463,   464,   396,   397,   398,   399,
     400,   396,  1009,   398,   399,   400,   465,   725,   741,   726,
     727,   466,   467,   396,   397,   398,   399,   400,   728,   729,
     730,   731,   732,   733,   468,   469,   471,   894,   396,   397,
     398,   399,   400,   748,   470,   472,   473,   904,   478,   474,
     475,   734,   396,   397,   398,   399,   400,   476,  1038,   477,
     479,   480,   481,   482,  1011,   735,   484,   396,   397,   398,
     399,   400,   485,   486,   487,   488,   489,   270,   491,  1156,
     492,   560,   266,   267,   493,  1162,   507,   494,   271,   495,
     496,   497,   396,   397,   398,   399,   400,   498,   499,  1171,
    1069,  1070,  1071,  1072,   500,   501,   504,   505,   506,   272,
     273,   510,   511,   512,   513,   278,   279,   280,   281,   282,
     515,   283,   270,   284,   285,   286,   287,   288,   524,   525,
      16,  1007,   543,   289,   526,   536,   537,   538,    17,   244,
     547,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,   272,   273,   548,    31,    32,    33,
      34,    35,   549,    36,   290,   266,   267,   551,    37,    38,
      39,    40,    41,    42,    43,    44,   556,    45,    46,    47,
    1104,    48,   901,    49,    50,    51,    52,    53,    54,   557,
     558,   559,    55,    56,   573,    57,   574,   576,   577,   579,
     581,   587,    58,   590,   278,   279,   280,   281,   282,   591,
     283,   270,   284,   285,   286,   287,   288,   596,   597,   598,
     599,   600,   289,    59,   606,   608,    60,   609,   610,   615,
     620,   396,   397,   398,   399,   400,   627,   630,   638,   631,
     639,  -183,  1067,   272,   273,   645,    61,   632,   650,   653,
     665,   680,    62,   290,    63,    64,   266,   267,    65,    66,
     654,    67,   682,    68,   683,   685,   686,   323,   691,    69,
     692,   698,    70,    71,   693,   694,   747,   695,    72,    73,
     696,    74,   699,    75,   700,  1164,   701,   278,   279,   280,
     281,   282,   744,   283,   270,   284,   285,   286,   287,   288,
    1173,   702,   740,   751,   745,   289,   746,   749,   750,   752,
     754,   757,   759,   755,  1137,   753,   756,   766,    76,    77,
      78,    79,   758,    80,   769,   773,   272,   273,   760,   762,
     763,   765,   767,   768,   771,   772,   290,   774,  1152,   775,
     778,   776,   697,   782,   777,   779,   780,    81,   922,   923,
     785,   786,   788,    82,    83,   790,   787,   801,   803,   800,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   278,   279,
     280,   281,   282,   789,   283,   270,   284,   285,   286,   287,
     288,   924,   925,    83,   325,   804,   289,   560,   926,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   272,   273,   802,
     810,   814,   838,   816,   815,   817,   818,   290,   819,   927,
     820,   821,   928,   929,   930,   931,   932,   933,   934,   935,
     936,   937,   938,   939,   940,   941,   942,   943,   231,   822,
     823,   824,   825,   826,    84,   827,    86,   834,    88,   828,
      90,   829,    92,   830,    94,   831,    96,   832,    98,   835,
     100,   833,   839,   841,   842,   843,   844,   849,   850,   851,
     856,   855,   858,   862,   860,   866,   863,   871,   870,   876,
     893,   895,   254,   878,   907,   899,   902,   903,   910,   908,
     911,   912,   956,   255,   256,   958,   917,   959,   961,   962,
     963,   964,   966,   965,   967,   969,   970,   974,   991,   975,
     990,   992,   994,   995,   996,  1005,  1006,  1020,  1004,  1027,
    1012,  1008,  1013,  1014,  1015,  1016,  1017,  1029,  1018,  1019,
    1040,  1021,  1030,  1041,  1036,  1052,  1035,  1042,  1039,  1043,
    1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,  1056,  1054,
    1053,  1060,  1058,  1063,  1077,  1064,  1078,  1068,  1061,  1079,
    1081,  1080,  1082,  1085,  1092,  1084,  1086,  1094,  1087,  1090,
    1091,  1106,  1107,  1108,  1111,  1113,  1114,  1118,  1120,  1122,
    1123,  1117,  1125,  1126,  1127,  1129,  1131,  1132,  1133,  1134,
    1138,  1139,  1141,  1160,  1144,    10,   425,  1172,   539,   232,
     225,  1140,   797,  1062,   688,  1037,  1147,  1146,  1149,  1150,
    1154,  1163,  1169,   621,  1057,   371,  1174,   382,   968,  1110,
    1088,  1142,   304,  1105,  1119,   946,   542,   885,  1143,   840,
       0,   738,   684,   663,     0,     0,   448
};

static const yytype_int16 yycheck[] =
{
     108,   180,   378,   537,    81,   177,   615,   536,   230,   188,
     489,   190,   464,   504,   505,     4,   514,   196,   197,   200,
     199,   200,     3,   204,   205,   206,   414,   414,     3,   678,
     869,   210,     4,     3,   213,     3,     8,     9,   217,     3,
     219,     4,   501,   674,    56,     4,    81,   538,     4,     8,
      81,     6,    65,    81,    81,    82,   235,   236,   237,   238,
      81,    82,    81,    81,    81,    82,     4,   691,    85,    86,
     249,     4,    81,   993,    81,     4,  1055,   249,    89,    81,
     456,     4,   182,   459,     5,     8,     9,   219,     3,     5,
     222,   191,   183,     4,     4,  1109,   221,     8,   219,    72,
     219,   280,   281,   222,   283,   226,   219,     4,    81,   222,
       3,   211,   212,    81,   214,   215,    73,    81,   218,   244,
     245,   221,   219,   223,    81,   220,   226,     3,   227,   226,
     219,    95,   209,    76,  1148,     3,   227,   226,    81,   238,
     239,   220,     3,   275,   276,   277,     0,   219,   104,    81,
     219,  1130,   220,   777,   226,   223,    88,   226,    81,    92,
      81,    94,   100,   385,   241,    81,    81,    96,   219,  1089,
     247,    81,   249,   184,   124,   226,   785,   227,   191,   219,
      95,    56,   227,   210,    81,    82,   226,    81,    81,    83,
      84,   800,   227,   210,   221,   144,   227,   146,  1037,   227,
     221,   810,   221,   221,   221,   220,    81,    82,   141,   221,
     238,   239,   125,    81,   221,   242,   243,     4,   227,   221,
      81,   755,    15,    16,   676,   242,   243,    95,   219,   238,
     239,   223,   221,   275,   276,   277,    23,   220,    25,   229,
     221,   890,   414,    30,    31,    32,   221,   236,   237,   221,
     225,   221,   158,   221,   160,   220,   887,   221,   221,   227,
     220,   225,   221,   227,   227,   487,   445,   446,   223,   220,
     646,   246,   247,   236,   237,   238,   239,   236,   237,   666,
     668,   460,   461,   272,   220,   744,   250,   251,   817,   780,
     788,   156,   220,   745,    81,    82,     4,   476,   832,   480,
      41,    42,    43,   484,   485,   486,   221,   384,   220,   272,
     221,   490,   227,   272,   221,   423,   393,   394,   220,   396,
     397,   398,   399,   400,   221,   236,   237,     3,   221,   220,
     227,   220,   225,   433,   227,   210,   413,   414,   510,   236,
     237,   238,   239,   443,   224,   221,   221,   447,   220,   426,
     137,   451,   278,   221,   220,   248,   249,   220,   435,   227,
     221,   272,   462,    86,    87,   220,   227,   242,   243,    91,
     246,   247,    77,    81,    82,   272,    98,   454,   220,   558,
     559,   458,   250,   251,    80,    81,   220,   248,   249,   220,
     467,   232,   233,   273,   274,   275,   276,   277,   120,   220,
     579,   220,   778,   220,   201,   220,   111,   112,   273,   274,
     275,   276,   277,   220,   279,   120,    85,    86,   140,  1047,
    1048,   208,   220,   210,   129,   220,   526,   224,   220,   963,
     220,   220,   966,   219,   221,   140,   219,     4,   225,   222,
     227,   220,     3,   220,   220,   220,     7,   524,   220,   236,
     237,   238,   239,   220,   220,   242,   243,   244,   245,   220,
     220,   166,   184,   185,   186,   187,   220,   189,   190,   191,
     192,   193,    10,    11,    12,    13,   273,   274,   275,   276,
     277,   220,   219,   100,   220,   272,   220,   220,  1017,   220,
     273,   274,   275,   276,   277,   203,   204,   205,   206,   207,
     672,   209,   210,   211,   212,   213,   214,   215,   220,   220,
     886,  1045,   227,   221,    93,   146,   222,  1046,   997,   219,
      81,    82,   222,     4,   219,   278,   220,     8,     9,  1138,
     220,   220,   220,   220,   242,   243,   273,   274,   275,   276,
     277,  1150,   220,   765,   252,   220,   237,   220,   239,   220,
     241,   221,   243,  1087,   245,   193,   247,   220,   249,   220,
     251,     4,   253,   201,   202,     8,     9,   273,   274,   275,
     276,   277,   649,   273,   274,   275,   276,   277,   273,   274,
     275,   276,   277,   220,   222,   220,   225,   223,   221,   227,
     769,   223,   692,   670,   816,   767,   696,   223,   820,   699,
     700,   219,   824,   825,   826,     3,     4,   829,   830,   831,
    1144,   249,   279,  1147,   786,    58,   219,   694,   695,   219,
     218,   698,   273,   274,   275,   276,   277,   221,     4,    72,
      73,    74,    75,   223,    77,   219,    79,   219,    81,    82,
     278,   279,   203,   204,   205,   206,   207,   224,   209,   210,
     211,   212,   213,   214,   215,   827,   221,   221,   221,   221,
     221,   281,   834,   221,   224,   108,   109,   110,   219,   112,
       3,   223,   115,   116,   117,     3,   119,   120,   121,   224,
     123,   242,   243,    81,    82,   246,   247,   223,   225,   219,
     219,   252,   219,   224,   219,   219,   273,   274,   275,   276,
     277,   273,   274,   275,   276,   277,   219,   150,   224,   152,
     153,     4,   223,   273,   274,   275,   276,   277,   161,   162,
     163,   164,   165,   166,   219,   219,     4,   804,   273,   274,
     275,   276,   277,   224,   221,   219,   219,   814,     4,   219,
     219,   184,   273,   274,   275,   276,   277,   219,   970,   219,
     219,   219,   219,     6,   926,   198,   219,   273,   274,   275,
     276,   277,   219,   219,   219,     4,   219,   210,   219,  1145,
     219,    80,    81,    82,   219,  1151,   224,   219,   221,   219,
     219,   219,   273,   274,   275,   276,   277,   219,   219,  1165,
    1012,  1013,  1014,  1015,   219,   219,   219,   219,   219,   242,
     243,   219,   219,   219,   219,   203,   204,   205,   206,   207,
     222,   209,   210,   211,   212,   213,   214,   215,   219,   222,
       6,   898,     3,   221,   220,   220,   220,   220,    14,   272,
       3,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,   242,   243,   219,    33,    34,    35,
      36,    37,   224,    39,   252,    81,    82,   219,    44,    45,
      46,    47,    48,    49,    50,    51,     5,    53,    54,    55,
    1049,    57,   224,    59,    60,    61,    62,    63,    64,   228,
     219,   219,    68,    69,   216,    71,   222,    96,   219,   219,
     219,    80,    78,    80,   203,   204,   205,   206,   207,   219,
     209,   210,   211,   212,   213,   214,   215,    38,     4,   219,
      38,    40,   221,    99,   184,    90,   102,   219,    90,   219,
     219,   273,   274,   275,   276,   277,    94,   188,   132,   100,
     132,   136,  1009,   242,   243,   147,   122,   103,   224,   142,
     219,   226,   128,   252,   130,   131,    81,    82,   134,   135,
     143,   137,     3,   139,   219,   222,     4,   183,   280,   145,
     219,   223,   148,   149,   222,   219,     4,   219,   154,   155,
     219,   157,   219,   159,   219,  1154,   219,   203,   204,   205,
     206,   207,   219,   209,   210,   211,   212,   213,   214,   215,
    1169,   227,   227,     4,   219,   221,   219,   219,   219,   219,
     219,     4,     4,   220,  1112,   223,   219,    80,   194,   195,
     196,   197,   219,   199,   220,     4,   242,   243,   219,   219,
     219,   219,   219,   219,   219,   219,   252,   219,  1136,   219,
     136,   219,     3,   279,   219,   219,   219,   223,    83,    84,
     219,   219,   219,   229,   230,     4,   220,     3,   228,   219,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   203,   204,
     205,   206,   207,   222,   209,   210,   211,   212,   213,   214,
     215,   126,   127,   230,   221,   219,   221,    80,   133,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   242,   243,   222,
     219,   221,     5,   220,   222,   220,   220,   252,   220,   164,
     220,   220,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   230,   220,
     220,   220,   220,   220,   236,   220,   238,   219,   240,   220,
     242,   220,   244,   220,   246,   220,   248,   220,   250,   219,
     252,   222,   219,     4,   219,   184,     4,   184,    90,   219,
      90,   227,   219,   219,    66,   219,    97,     4,    97,    97,
     219,   222,   227,   225,   107,   222,   222,   222,   113,   106,
     114,   118,   219,   238,   239,     4,   151,   219,   219,     4,
     220,   224,   220,   219,     4,   219,   219,   219,     4,   219,
     219,     4,   219,   219,   219,     5,    41,     3,   222,   101,
     220,   222,   220,   220,   220,   220,   220,     4,   220,   220,
      67,   219,   219,    92,   219,   104,   222,   220,   222,   220,
     220,   220,   220,   220,   220,   220,   220,   220,     4,   219,
     222,   220,     7,   219,     4,   222,   141,    81,   226,     4,
     220,   219,    98,     4,    97,   219,   219,     4,   220,   219,
     219,     4,   219,     4,   220,     5,    41,   222,   100,   220,
     184,   280,     4,   222,   184,   222,    92,    91,   105,   222,
     219,   222,   222,    91,   220,     5,   253,  1168,   422,    73,
      66,   217,   676,  1000,   555,   969,   220,   222,   219,   219,
     219,   219,   219,   489,   995,   220,   222,   229,   853,  1057,
    1036,  1117,   184,  1050,  1080,   834,   427,   788,  1122,   749,
      -1,   579,   548,   524,    -1,    -1,   287
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   283,   284,   285,   286,   220,   220,     0,
     284,   124,   296,   227,   227,   220,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    33,    34,    35,    36,    37,    39,    44,    45,    46,
      47,    48,    49,    50,    51,    53,    54,    55,    57,    59,
      60,    61,    62,    63,    64,    68,    69,    71,    78,    99,
     102,   122,   128,   130,   131,   134,   135,   137,   139,   145,
     148,   149,   154,   155,   157,   159,   194,   195,   196,   197,
     199,   223,   229,   230,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   287,   288,   289,   290,   291,   298,   300,   301,
     302,   303,   305,   306,   314,   317,   318,   320,   321,   328,
     330,   331,   333,   335,   337,   341,   342,   343,   346,   348,
     355,   362,   365,   371,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   387,   388,   389,   390,   394,   395,
     397,   398,   399,   400,   401,   402,   403,   404,   405,   408,
     409,   410,   411,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   219,   125,   297,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   320,   220,   220,   220,   220,
     223,   230,   302,   304,   322,   220,   220,   220,   220,   229,
       4,   221,   236,   237,   272,   453,   463,   221,   298,   278,
     220,   332,     3,   219,   227,   238,   239,   439,   452,    10,
      11,    12,    13,     4,    96,   464,    81,    82,    85,    86,
     210,   221,   242,   243,   363,   441,   442,   100,   203,   204,
     205,   206,   207,   209,   211,   212,   213,   214,   215,   221,
     252,   441,   461,   462,     3,    81,    95,   221,   227,   250,
     251,   373,   450,   451,   373,     3,    81,   221,   227,   248,
     249,   367,   448,   449,    81,   227,   423,    72,    81,   433,
     441,     4,   441,   183,   462,   221,   244,   245,   444,   445,
     444,    56,   221,   471,   472,   227,   441,   441,    93,   441,
     221,   441,   471,   444,   444,    81,   227,   345,   424,   471,
     471,   471,   440,   453,   221,   344,   453,   441,   462,   462,
     441,   462,   462,     4,     8,     9,    81,   465,   441,   462,
     441,   345,   462,   444,   462,     5,    81,   356,   338,   462,
     444,   146,   301,   440,   278,   223,   441,   441,   441,   441,
     221,     8,   453,   223,   201,   224,   273,   274,   275,   276,
     277,   452,   453,   458,   459,   460,    23,    25,    30,    31,
      32,   137,   208,   221,   225,   441,   444,   452,   453,   467,
     468,   469,   336,   225,   299,   297,   223,   279,   219,   219,
     219,   218,   443,   221,     4,   223,   219,   219,   350,   444,
     444,   441,   441,   221,   441,   221,   221,   221,   461,   221,
     462,   281,   219,     3,   223,   374,   372,     3,   223,   366,
     219,   219,   219,   219,   219,   219,     4,   223,   219,   219,
     221,     4,   219,   219,   219,   219,   219,   219,     4,   219,
     219,   219,     6,   319,   219,   219,   219,   219,     4,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   299,   339,   219,   219,   219,   224,   453,   440,
     219,   219,   219,   219,   315,   222,   222,   453,   453,   453,
     453,   453,   453,   453,   219,   222,   220,     4,   310,   311,
     312,   313,   442,   445,   452,   453,   220,   220,   220,   300,
     298,   453,   439,     3,     3,   221,   396,     3,   219,   224,
     462,   219,   453,    86,    87,   364,     5,   228,   219,   219,
      80,   358,   462,   441,   441,     3,     7,   221,   246,   247,
     446,   447,   462,   216,   222,   462,    96,   219,   453,   219,
     299,   219,   453,   299,   441,   441,   462,    80,   308,   449,
      80,   219,   453,    73,    81,   434,    38,     4,   219,    38,
      40,    81,    83,    84,   435,   438,   184,   441,    90,   219,
      90,   471,    81,   221,   351,   219,   471,   471,   471,   440,
     219,   344,   465,   441,    76,    81,   470,    94,    65,   191,
     188,   100,   103,    81,   221,   352,     4,   100,   132,   132,
     433,   340,   221,   446,   446,   147,   334,   156,   279,   329,
     224,    81,   452,   142,   143,   144,   146,   303,   455,   456,
     457,   224,   224,   458,   462,   219,   219,   226,   219,   226,
     219,   226,   219,   226,   225,   450,   225,   448,   225,   446,
     226,   224,     3,   219,   443,   222,     4,   224,   350,   441,
     441,   280,   219,   222,   219,   219,   219,     3,   223,   219,
     219,   219,   227,   224,     4,     8,     9,    58,    72,    73,
      74,    75,    77,    79,   108,   109,   110,   112,   115,   116,
     117,   119,   120,   121,   123,   150,   152,   153,   161,   162,
     163,   164,   165,   166,   184,   198,   375,   441,   463,   466,
     227,   224,   368,   360,   219,   219,   219,     4,   224,   219,
     219,     4,   219,   223,   219,   220,   219,     4,   219,     4,
     219,   434,   219,   219,   386,   219,    80,   219,   219,   220,
     391,   219,   219,     4,   219,   219,   219,   219,   136,   219,
     219,   299,   279,   453,   323,   219,   219,   220,   219,   222,
       4,   445,   442,   453,   452,   307,   451,   308,   309,   447,
     219,     3,   222,   228,   219,   358,   462,   453,   453,   462,
     219,   453,   462,   462,   221,   222,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   222,   219,   219,   433,   449,     5,   219,
     424,     4,   219,   184,     4,    81,    88,   437,   448,   184,
      90,   219,    81,   221,   353,   227,    90,   440,   219,   452,
      66,   441,   219,    97,     4,   104,   219,    81,   221,   354,
      97,     4,    81,   357,   358,   299,    97,   446,   225,   434,
      81,   452,   232,   233,   454,   455,   316,   219,   226,   226,
     219,   226,   434,   219,   453,   222,   219,   222,   219,   222,
     434,   224,   222,   222,   453,   440,   450,   107,   106,   440,
     113,   114,   118,   440,   440,   440,   452,   151,   440,   440,
     440,   448,    83,    84,   126,   127,   133,   164,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   369,   436,   438,   452,    77,   111,
     112,   120,   129,   140,   166,   361,   219,   294,     4,   219,
     425,   219,     4,   220,   224,   219,   220,     4,   363,   219,
     219,     4,    81,   359,   219,   219,    91,    98,   120,   140,
     184,   185,   186,   187,   189,   190,   191,   192,   193,   392,
     219,     4,     4,   425,   219,   219,   219,   158,   160,   324,
     325,   299,   451,   447,   222,     5,    41,   453,   222,   274,
     220,   452,   220,   220,   220,   220,   220,   220,   220,   220,
       3,   219,    41,    42,    43,   426,   427,   101,   428,     4,
     219,   448,    89,   184,   448,   222,   219,   354,   440,   222,
      67,    92,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   104,   222,   219,   428,     4,   359,     7,   465,
     220,   226,   324,   219,   222,   219,   222,   453,    81,   440,
     440,   440,   440,   183,   227,   370,   450,     4,   141,     4,
     219,   220,    98,   430,   219,     4,   219,   220,   364,   425,
     219,   219,    97,   431,     4,     4,    92,    94,   141,   393,
     448,   450,   396,   396,   441,   423,     4,   219,     4,   430,
     360,   220,   327,     5,    41,   219,   292,   280,   222,   426,
     100,   429,   220,   184,   432,     4,   222,   184,   448,   222,
     428,    92,    91,   105,   222,   432,   326,   298,   219,   222,
     217,   222,   370,   431,   220,   406,   222,   220,   430,   219,
     219,   347,   298,   434,   219,   448,   299,   407,   448,   432,
      91,   434,   299,   219,   441,   349,     5,    81,   295,   219,
     293,   299,   294,   441,   222
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
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1263 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1268 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1273 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1280 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1287 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1296 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1304 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1315 "lev_comp.y"
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

  case 194:

/* Line 1455 of yacc.c  */
#line 1331 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1338 "lev_comp.y"
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

  case 196:

/* Line 1455 of yacc.c  */
#line 1354 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1361 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1365 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1371 "lev_comp.y"
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

  case 200:

/* Line 1455 of yacc.c  */
#line 1381 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1387 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1396 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1402 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1407 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1413 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1418 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1424 "lev_comp.y"
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

  case 208:

/* Line 1455 of yacc.c  */
#line 1435 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1449 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1453 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1463 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1470 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1478 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1483 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1488 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1493 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1498 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1503 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1508 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1516 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1523 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1531 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1547 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1551 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1557 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1565 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1571 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1579 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1587 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1592 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1598 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1604 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1610 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1616 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1621 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(0, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1626 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1631 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1636 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1641 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1646 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1651 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1656 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1661 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1666 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1671 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1677 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1682 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_PROTECTOR));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1687 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_MAXHP));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1692 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_LEVEL_ADJUSTMENT));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1697 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_KEEP_ORIGINAL_INVENTORY));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1704 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1713 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1717 "lev_comp.y"
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

  case 264:

/* Line 1455 of yacc.c  */
#line 1730 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1738 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1748 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1756 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1765 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1772 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1780 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1786 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1791 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1796 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1801 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1806 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1811 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1816 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1821 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1826 "lev_comp.y"
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

  case 280:

/* Line 1455 of yacc.c  */
#line 1840 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1846 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1851 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1856 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1861 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1866 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1871 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1876 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1881 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1886 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1891 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1896 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1901 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x00200000;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1906 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x00400000;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1911 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x00800000;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1916 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x01000000;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1921 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_MYTHIC_TYPE));
		      (yyval.i) = 0x02000000;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1927 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x04000000;
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1932 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x08000000;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1937 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x10000000;
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1942 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MATERIAL));
		      (yyval.i) = 0x20000000;
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1949 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 1955 "lev_comp.y"
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

  case 303:

/* Line 1455 of yacc.c  */
#line 1984 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 1989 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 1997 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2003 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2007 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2015 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2022 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2029 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2039 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2049 "lev_comp.y"
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

  case 313:

/* Line 1455 of yacc.c  */
#line 2065 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2075 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2079 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2085 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2091 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2097 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2101 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2107 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2113 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2120 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2128 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2134 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2139 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2144 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2149 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2154 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2160 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2166 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2171 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2176 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2181 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2186 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2191 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2201 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2207 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2213 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2218 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2225 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2232 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2236 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2242 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2248 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2254 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2259 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2266 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2272 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2282 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2288 "lev_comp.y"
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

  case 355:

/* Line 1455 of yacc.c  */
#line 2307 "lev_comp.y"
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

  case 356:

/* Line 1455 of yacc.c  */
#line 2319 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2323 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2329 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2334 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2339 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (long)(yyvsp[(9) - (9)].i), (long)(yyvsp[(7) - (9)].i), (long)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2344 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((long)(yyvsp[(9) - (13)].i), (long)(yyvsp[(7) - (13)].i), (long)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2350 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2354 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2360 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, (int)(yyvsp[(11) - (11)].i), (int)(yyvsp[(9) - (11)].i), (int)(yyvsp[(7) - (11)].i), (int)(yyvsp[(5) - (11)].i), SPO_DECORATION));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2364 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6((int)(yyvsp[(13) - (13)].i), (int)(yyvsp[(11) - (13)].i), (int)(yyvsp[(9) - (13)].i), (int)(yyvsp[(7) - (13)].i), (int)(yyvsp[(5) - (13)].i), SPO_DECORATION));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2368 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, 1, (int)(yyvsp[(7) - (7)].i), 0, (int)(yyvsp[(5) - (7)].i), SPO_DECORATION));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2374 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2380 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2384 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2390 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_CARPET));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2396 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2400 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4(0, (int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2406 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2410 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2416 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2420 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2425 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2432 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2436 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2443 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2447 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2452 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2459 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2463 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2468 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2476 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2482 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2489 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2493 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2500 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2511 "lev_comp.y"
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

  case 394:

/* Line 1455 of yacc.c  */
#line 2524 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2528 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2534 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2538 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2545 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2549 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2553 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2559 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2563 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2569 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2575 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2579 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2585 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2591 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2596 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2613 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2621 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2631 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2640 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2645 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2652 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2663 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2669 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2673 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2677 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2684 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2694 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2701 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2705 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2711 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2715 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2723 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2727 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2734 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2744 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2757 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2761 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2768 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2778 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2787 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2798 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2802 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2809 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2819 "lev_comp.y"
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

  case 448:

/* Line 1455 of yacc.c  */
#line 2830 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2839 "lev_comp.y"
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

  case 450:

/* Line 1455 of yacc.c  */
#line 2849 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2855 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2859 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2866 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2876 "lev_comp.y"
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

  case 455:

/* Line 1455 of yacc.c  */
#line 2887 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2896 "lev_comp.y"
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

  case 457:

/* Line 1455 of yacc.c  */
#line 2906 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2910 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2916 "lev_comp.y"
    { ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2918 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2924 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2928 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2932 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2936 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2944 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2953 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2957 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2961 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 2965 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2969 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 2972 "lev_comp.y"
    { ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 2976 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 2983 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 2992 "lev_comp.y"
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

  case 479:

/* Line 1455 of yacc.c  */
#line 3033 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3037 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3044 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3051 "lev_comp.y"
    {
			      size_t len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3061 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3065 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3073 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3077 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3081 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3085 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3089 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3094 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3098 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3102 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3107 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3112 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3117 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3121 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3126 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3131 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3135 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3139 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3144 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3148 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3155 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3161 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3165 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3171 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3187 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3191 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3195 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 3199 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 524:

/* Line 1455 of yacc.c  */
#line 3221 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 525:

/* Line 1455 of yacc.c  */
#line 3225 "lev_comp.y"
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

  case 526:

/* Line 1455 of yacc.c  */
#line 3251 "lev_comp.y"
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
#line 7388 "lev_comp.tab.c"
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
#line 3279 "lev_comp.y"


/*lev_comp.y*/

