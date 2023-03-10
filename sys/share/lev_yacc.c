
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
     DECOTYP_ID = 280,
     OBJECT_ID = 281,
     COBJECT_ID = 282,
     MONSTER_ID = 283,
     TRAP_ID = 284,
     DOOR_ID = 285,
     DRAWBRIDGE_ID = 286,
     MONSTER_GENERATION_ID = 287,
     object_ID = 288,
     monster_ID = 289,
     terrain_ID = 290,
     MAZEWALK_ID = 291,
     WALLIFY_ID = 292,
     REGION_ID = 293,
     SPECIAL_REGION_ID = 294,
     SPECIAL_LEVREGION_ID = 295,
     SPECIAL_REGION_TYPE = 296,
     NAMING_ID = 297,
     NAMING_TYPE = 298,
     FILLING = 299,
     IRREGULAR = 300,
     JOINED = 301,
     ALTAR_ID = 302,
     ANVIL_ID = 303,
     NPC_ID = 304,
     LADDER_ID = 305,
     STAIR_ID = 306,
     NON_DIGGABLE_ID = 307,
     NON_PASSWALL_ID = 308,
     ROOM_ID = 309,
     ARTIFACT_NAME_ID = 310,
     PORTAL_ID = 311,
     TELEPRT_ID = 312,
     BRANCH_ID = 313,
     LEV = 314,
     MINERALIZE_ID = 315,
     AGE_ID = 316,
     CORRIDOR_ID = 317,
     GOLD_ID = 318,
     ENGRAVING_ID = 319,
     FOUNTAIN_ID = 320,
     THRONE_ID = 321,
     MODRON_PORTAL_ID = 322,
     LEVEL_TELEPORTER_ID = 323,
     LEVEL_TELEPORT_DIRECTION_TYPE = 324,
     LEVEL_TELEPORT_END_TYPE = 325,
     POOL_ID = 326,
     SINK_ID = 327,
     NONE = 328,
     RAND_CORRIDOR_ID = 329,
     DOOR_STATE = 330,
     LIGHT_STATE = 331,
     CURSE_TYPE = 332,
     MYTHIC_TYPE = 333,
     ENGRAVING_TYPE = 334,
     KEYTYPE_ID = 335,
     LEVER_ID = 336,
     NO_PICKUP_ID = 337,
     DIRECTION = 338,
     RANDOM_TYPE = 339,
     RANDOM_TYPE_BRACKET = 340,
     A_REGISTER = 341,
     ALIGNMENT = 342,
     LEFT_OR_RIGHT = 343,
     CENTER = 344,
     TOP_OR_BOT = 345,
     ALTAR_TYPE = 346,
     ALTAR_SUBTYPE = 347,
     UP_OR_DOWN = 348,
     ACTIVE_OR_INACTIVE = 349,
     MODRON_PORTAL_TYPE = 350,
     NPC_TYPE = 351,
     FOUNTAIN_TYPE = 352,
     SPECIAL_OBJECT_TYPE = 353,
     CMAP_TYPE = 354,
     FLOOR_SUBTYPE = 355,
     FLOOR_SUBTYPE_ID = 356,
     FLOOR_ID = 357,
     FLOOR_TYPE = 358,
     FLOOR_TYPE_ID = 359,
     DECORATION_ID = 360,
     DECORATION_TYPE = 361,
     DECORATION_DIR = 362,
     DECORATION_ITEM_STATE = 363,
     PAINTING_TYPE = 364,
     BANNER_TYPE = 365,
     WALL_SCULPTURE_TYPE = 366,
     ELEMENTAL_ENCHANTMENT_TYPE = 367,
     EXCEPTIONALITY_TYPE = 368,
     EXCEPTIONALITY_ID = 369,
     ELEMENTAL_ENCHANTMENT_ID = 370,
     ENCHANTMENT_ID = 371,
     SECRET_DOOR_ID = 372,
     USES_UP_KEY_ID = 373,
     MYTHIC_PREFIX_TYPE = 374,
     MYTHIC_SUFFIX_TYPE = 375,
     MYTHIC_PREFIX_ID = 376,
     MYTHIC_SUFFIX_ID = 377,
     MATERIAL_ID = 378,
     MATERIAL_TYPE = 379,
     CHARGES_ID = 380,
     SPECIAL_QUALITY_ID = 381,
     SPEFLAGS_ID = 382,
     SUBROOM_ID = 383,
     NAME_ID = 384,
     FLAGS_ID = 385,
     FLAG_TYPE = 386,
     MON_ATTITUDE = 387,
     MON_ALERTNESS = 388,
     SUBTYPE_ID = 389,
     NON_PASSDOOR_ID = 390,
     CARPET_ID = 391,
     CARPET_PIECE_ID = 392,
     CARPET_TYPE = 393,
     MON_APPEARANCE = 394,
     ROOMDOOR_ID = 395,
     IF_ID = 396,
     ELSE_ID = 397,
     TERRAIN_ID = 398,
     HORIZ_OR_VERT = 399,
     REPLACE_TERRAIN_ID = 400,
     LOCATION_SUBTYPE_ID = 401,
     DOOR_SUBTYPE = 402,
     BRAZIER_SUBTYPE = 403,
     SIGNPOST_SUBTYPE = 404,
     TREE_SUBTYPE = 405,
     FOREST_ID = 406,
     FOREST_TYPE = 407,
     INITIALIZE_TYPE = 408,
     EXIT_ID = 409,
     SHUFFLE_ID = 410,
     MANUAL_TYPE_ID = 411,
     MANUAL_TYPE = 412,
     QUANTITY_ID = 413,
     BURIED_ID = 414,
     LOOP_ID = 415,
     FOR_ID = 416,
     TO_ID = 417,
     SWITCH_ID = 418,
     CASE_ID = 419,
     BREAK_ID = 420,
     DEFAULT_ID = 421,
     ERODED_ID = 422,
     TRAPPED_STATE = 423,
     RECHARGED_ID = 424,
     INVIS_ID = 425,
     GREASED_ID = 426,
     INDESTRUCTIBLE_ID = 427,
     FEMALE_ID = 428,
     MALE_ID = 429,
     WAITFORU_ID = 430,
     PROTECTOR_ID = 431,
     CANCELLED_ID = 432,
     REVIVED_ID = 433,
     AVENGE_ID = 434,
     FLEEING_ID = 435,
     BLINDED_ID = 436,
     MAXHP_ID = 437,
     LEVEL_ADJUSTMENT_ID = 438,
     KEEP_ORIGINAL_INVENTORY_ID = 439,
     PARALYZED_ID = 440,
     STUNNED_ID = 441,
     CONFUSED_ID = 442,
     SEENTRAPS_ID = 443,
     ALL_ID = 444,
     MONTYPE_ID = 445,
     OBJTYPE_ID = 446,
     TERTYPE_ID = 447,
     TERTYPE2_ID = 448,
     LEVER_EFFECT_TYPE = 449,
     SWITCHABLE_ID = 450,
     CONTINUOUSLY_USABLE_ID = 451,
     TARGET_ID = 452,
     TRAPTYPE_ID = 453,
     EFFECT_FLAG_ID = 454,
     GRAVE_ID = 455,
     BRAZIER_ID = 456,
     SIGNPOST_ID = 457,
     TREE_ID = 458,
     ERODEPROOF_ID = 459,
     FUNCTION_ID = 460,
     MSG_OUTPUT_TYPE = 461,
     COMPARE_TYPE = 462,
     UNKNOWN_TYPE = 463,
     rect_ID = 464,
     fillrect_ID = 465,
     line_ID = 466,
     randline_ID = 467,
     grow_ID = 468,
     selection_ID = 469,
     flood_ID = 470,
     rndcoord_ID = 471,
     circle_ID = 472,
     ellipse_ID = 473,
     filter_ID = 474,
     complement_ID = 475,
     gradient_ID = 476,
     GRADIENT_TYPE = 477,
     LIMITED = 478,
     HUMIDITY_TYPE = 479,
     STRING = 480,
     MAP_ID = 481,
     NQSTRING = 482,
     VARSTRING = 483,
     CFUNC = 484,
     CFUNC_INT = 485,
     CFUNC_STR = 486,
     CFUNC_COORD = 487,
     CFUNC_REGION = 488,
     VARSTRING_INT = 489,
     VARSTRING_INT_ARRAY = 490,
     VARSTRING_STRING = 491,
     VARSTRING_STRING_ARRAY = 492,
     VARSTRING_VAR = 493,
     VARSTRING_VAR_ARRAY = 494,
     VARSTRING_COORD = 495,
     VARSTRING_COORD_ARRAY = 496,
     VARSTRING_REGION = 497,
     VARSTRING_REGION_ARRAY = 498,
     VARSTRING_MAPCHAR = 499,
     VARSTRING_MAPCHAR_ARRAY = 500,
     VARSTRING_MONST = 501,
     VARSTRING_MONST_ARRAY = 502,
     VARSTRING_OBJ = 503,
     VARSTRING_OBJ_ARRAY = 504,
     VARSTRING_SEL = 505,
     VARSTRING_SEL_ARRAY = 506,
     METHOD_INT = 507,
     METHOD_INT_ARRAY = 508,
     METHOD_STRING = 509,
     METHOD_STRING_ARRAY = 510,
     METHOD_VAR = 511,
     METHOD_VAR_ARRAY = 512,
     METHOD_COORD = 513,
     METHOD_COORD_ARRAY = 514,
     METHOD_REGION = 515,
     METHOD_REGION_ARRAY = 516,
     METHOD_MAPCHAR = 517,
     METHOD_MAPCHAR_ARRAY = 518,
     METHOD_MONST = 519,
     METHOD_MONST_ARRAY = 520,
     METHOD_OBJ = 521,
     METHOD_OBJ_ARRAY = 522,
     METHOD_SEL = 523,
     METHOD_SEL_ARRAY = 524,
     DICE = 525
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
#line 570 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 582 "lev_comp.tab.c"

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
#define YYLAST   1359

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  288
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  193
/* YYNRULES -- Number of rules.  */
#define YYNRULES  535
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1200

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   525

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   283,   287,     2,
     227,   228,   281,   279,   225,   280,   285,   282,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   226,     2,
       2,   284,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   229,     2,   230,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   231,   286,   232,     2,     2,     2,     2,
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
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278
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
     556,   562,   568,   576,   581,   582,   598,   599,   617,   618,
     621,   627,   629,   635,   637,   643,   645,   651,   653,   664,
     671,   673,   675,   677,   679,   681,   685,   687,   689,   690,
     694,   698,   702,   706,   708,   710,   712,   714,   716,   724,
     730,   732,   734,   736,   738,   742,   743,   749,   754,   755,
     759,   761,   763,   765,   767,   770,   772,   774,   776,   778,
     780,   784,   786,   790,   794,   798,   800,   802,   806,   808,
     810,   812,   816,   818,   820,   822,   826,   830,   831,   837,
     840,   841,   845,   847,   851,   853,   857,   861,   863,   865,
     869,   871,   873,   875,   879,   881,   883,   885,   889,   893,
     897,   901,   905,   909,   913,   917,   919,   921,   923,   925,
     929,   933,   937,   941,   947,   955,   961,   970,   972,   976,
     980,   986,   992,  1000,  1008,  1015,  1021,  1022,  1025,  1031,
    1035,  1047,  1061,  1068,  1069,  1073,  1075,  1079,  1083,  1087,
    1091,  1093,  1095,  1099,  1103,  1107,  1111,  1115,  1119,  1121,
    1123,  1125,  1127,  1131,  1135,  1137,  1143,  1153,  1159,  1167,
    1171,  1175,  1181,  1191,  1197,  1203,  1209,  1210,  1226,  1227,
    1229,  1237,  1249,  1259,  1273,  1277,  1285,  1297,  1311,  1321,
    1331,  1341,  1349,  1357,  1363,  1371,  1377,  1387,  1395,  1401,
    1411,  1417,  1423,  1427,  1435,  1439,  1447,  1455,  1459,  1465,
    1471,  1475,  1481,  1489,  1499,  1501,  1503,  1505,  1507,  1509,
    1510,  1513,  1515,  1519,  1521,  1523,  1525,  1526,  1530,  1532,
    1533,  1537,  1539,  1540,  1544,  1545,  1549,  1550,  1554,  1556,
    1558,  1560,  1562,  1564,  1566,  1568,  1570,  1572,  1576,  1578,
    1580,  1585,  1587,  1589,  1594,  1596,  1598,  1603,  1605,  1610,
    1616,  1618,  1622,  1624,  1628,  1630,  1632,  1637,  1647,  1649,
    1651,  1656,  1658,  1664,  1666,  1668,  1673,  1675,  1677,  1683,
    1685,  1687,  1689,  1694,  1696,  1698,  1704,  1706,  1708,  1710,
    1714,  1716,  1718,  1722,  1724,  1729,  1733,  1737,  1741,  1745,
    1749,  1753,  1755,  1757,  1761,  1763,  1767,  1768,  1770,  1772,
    1774,  1776,  1780,  1781,  1783,  1785,  1788,  1791,  1796,  1803,
    1808,  1815,  1822,  1829,  1836,  1839,  1846,  1855,  1864,  1875,
    1890,  1893,  1895,  1899,  1901,  1905,  1907,  1909,  1911,  1913,
    1915,  1917,  1919,  1921,  1923,  1925,  1927,  1929,  1931,  1933,
    1935,  1937,  1939,  1941,  1943,  1954
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     289,     0,    -1,    -1,   290,    -1,   291,    -1,   291,   290,
      -1,   292,   302,   304,    -1,    17,   226,   233,    -1,    16,
     226,   233,   225,     3,    -1,    18,   226,    11,   225,   402,
      -1,    18,   226,    10,   225,     3,    -1,    18,   226,    13,
      -1,    18,   226,    12,   225,     3,   225,     3,   225,     5,
     225,     5,   225,   442,   225,   301,   300,    -1,    19,   226,
     472,    -1,   151,   226,   152,   225,   153,    -1,   151,   226,
     152,    -1,    32,   226,     4,   225,   314,    -1,    22,   226,
     103,    -1,    -1,   225,   223,    -1,    -1,   225,   449,    -1,
      -1,   225,     3,    -1,     5,    -1,    84,    -1,    -1,   130,
     226,   303,    -1,   131,   225,   303,    -1,   131,    -1,    -1,
     306,   304,    -1,   231,   304,   232,    -1,   347,    -1,   293,
      -1,   294,    -1,   410,    -1,   297,    -1,   295,    -1,   418,
      -1,   419,    -1,   420,    -1,   296,    -1,   417,    -1,   416,
      -1,   414,    -1,   415,    -1,   421,    -1,   422,    -1,   423,
      -1,   424,    -1,   425,    -1,   391,    -1,   349,    -1,   312,
      -1,   311,    -1,   405,    -1,   361,    -1,   383,    -1,   427,
      -1,   428,    -1,   393,    -1,   394,    -1,   395,    -1,   426,
      -1,   327,    -1,   337,    -1,   339,    -1,   343,    -1,   341,
      -1,   324,    -1,   334,    -1,   320,    -1,   323,    -1,   386,
      -1,   396,    -1,   368,    -1,   384,    -1,   371,    -1,   377,
      -1,   406,    -1,   401,    -1,   389,    -1,   348,    -1,   407,
      -1,   408,    -1,   409,    -1,   411,    -1,   354,    -1,   352,
      -1,   400,    -1,   404,    -1,   403,    -1,   387,    -1,   388,
      -1,   390,    -1,   382,    -1,   385,    -1,   243,    -1,   245,
      -1,   247,    -1,   249,    -1,   251,    -1,   253,    -1,   255,
      -1,   257,    -1,   259,    -1,   242,    -1,   244,    -1,   246,
      -1,   248,    -1,   250,    -1,   252,    -1,   254,    -1,   256,
      -1,   258,    -1,   307,    -1,   308,    -1,   236,    -1,   236,
      -1,   308,    -1,   155,   226,   307,    -1,   309,   284,   461,
      -1,   309,   284,   214,   226,   470,    -1,   309,   284,   460,
      -1,   309,   284,   477,   226,   454,    -1,   309,   284,   476,
     226,   456,    -1,   309,   284,   475,   226,   458,    -1,   309,
     284,   449,    -1,   309,   284,   452,    -1,   309,   284,   231,
     318,   232,    -1,   309,   284,   231,   317,   232,    -1,   309,
     284,   231,   316,   232,    -1,   309,   284,   477,   226,   231,
     315,   232,    -1,   309,   284,   476,   226,   231,   314,   232,
      -1,   309,   284,   475,   226,   231,   313,   232,    -1,   309,
     284,   231,   319,   232,    -1,   459,    -1,   313,   225,   459,
      -1,   457,    -1,   314,   225,   457,    -1,   455,    -1,   315,
     225,   455,    -1,   453,    -1,   316,   225,   453,    -1,   450,
      -1,   317,   225,   450,    -1,   461,    -1,   318,   225,   461,
      -1,   460,    -1,   319,   225,   460,    -1,    -1,    -1,   205,
     235,   227,   321,   465,   228,   322,   305,    -1,   235,   227,
     468,   228,    -1,   154,    -1,    -1,     6,    -1,     6,    -1,
     229,   461,   207,   461,   230,    -1,   229,   461,   230,    -1,
      -1,    -1,   163,   328,   229,   448,   230,   329,   231,   330,
     232,    -1,    -1,   331,   330,    -1,    -1,   164,   473,   226,
     332,   304,    -1,    -1,   166,   226,   333,   304,    -1,   165,
      -1,   285,   285,    -1,   162,    -1,   161,   310,   284,   461,
     335,   461,    -1,    -1,   336,   338,   305,    -1,    -1,   160,
     229,   448,   230,   340,   305,    -1,    -1,   326,   226,   342,
     306,    -1,    -1,   141,   326,   344,   345,    -1,   305,    -1,
      -1,   305,   346,   142,   305,    -1,    14,   226,   460,    -1,
      14,   226,   460,   225,    15,    -1,    74,    -1,    74,   226,
     473,    -1,    74,   226,    84,    -1,    62,   226,   350,   225,
     350,    -1,    62,   226,   350,   225,   473,    -1,   227,     4,
     225,    83,   225,   365,   228,    -1,   430,   325,   225,   442,
      -1,    -1,   128,   226,   351,   225,   358,   225,   360,   431,
     434,   436,   439,   440,   438,   353,   305,    -1,    -1,    54,
     226,   351,   225,   357,   225,   359,   225,   360,   431,   434,
     436,   439,   440,   438,   355,   305,    -1,    -1,   225,     5,
      -1,   227,     4,   225,     4,   228,    -1,    84,    -1,   227,
       4,   225,     4,   228,    -1,    84,    -1,   227,   369,   225,
     370,   228,    -1,    84,    -1,   227,     4,   225,     4,   228,
      -1,    84,    -1,   140,   226,   362,   225,   441,   225,   363,
     225,   365,   366,    -1,    30,   226,   441,   225,   470,   366,
      -1,     5,    -1,    84,    -1,   364,    -1,    84,    -1,    83,
      -1,    83,   286,   364,    -1,     4,    -1,    84,    -1,    -1,
     366,   225,   367,    -1,   146,   226,   147,    -1,    80,   226,
     458,    -1,   126,   226,     4,    -1,   172,    -1,   117,    -1,
     118,    -1,   135,    -1,    21,    -1,    20,   226,   369,   225,
     370,   356,   234,    -1,    20,   226,   449,   356,   234,    -1,
      88,    -1,    89,    -1,    90,    -1,    89,    -1,    28,   226,
     373,    -1,    -1,    28,   226,   373,   372,   305,    -1,   456,
     225,   449,   374,    -1,    -1,   374,   225,   375,    -1,   460,
      -1,   132,    -1,   133,    -1,   444,    -1,   139,   460,    -1,
     173,    -1,   174,    -1,   170,    -1,   177,    -1,   178,    -1,
     178,   226,   448,    -1,   179,    -1,   180,   226,   448,    -1,
     181,   226,   448,    -1,   185,   226,   448,    -1,   186,    -1,
     187,    -1,   188,   226,   376,    -1,   175,    -1,   176,    -1,
     182,    -1,   183,   226,   448,    -1,   184,    -1,   233,    -1,
     189,    -1,   233,   286,   376,    -1,    26,   226,   379,    -1,
      -1,    27,   226,   379,   378,   305,    -1,   458,   380,    -1,
      -1,   380,   225,   381,    -1,    77,    -1,   190,   226,   456,
      -1,   474,    -1,   129,   226,   460,    -1,   158,   226,   448,
      -1,   159,    -1,    76,    -1,   167,   226,   448,    -1,   204,
      -1,    75,    -1,   168,    -1,   169,   226,   448,    -1,   170,
      -1,   171,    -1,   449,    -1,   115,   226,   112,    -1,   114,
     226,   113,    -1,   116,   226,   448,    -1,   125,   226,   448,
      -1,   126,   226,   448,    -1,   156,   226,   157,    -1,   127,
     226,   448,    -1,    80,   226,   458,    -1,   172,    -1,   118,
      -1,    82,    -1,    78,    -1,   121,   226,   119,    -1,   122,
     226,   120,    -1,    61,   226,   448,    -1,   123,   226,   124,
      -1,    29,   226,   429,   225,   449,    -1,    31,   226,   449,
     225,    83,   225,   441,    -1,    36,   226,   449,   225,    83,
      -1,    36,   226,   449,   225,    83,   225,     5,   300,    -1,
      37,    -1,    37,   226,   470,    -1,    37,   226,   189,    -1,
      50,   226,   449,   225,    93,    -1,    51,   226,   449,   225,
      93,    -1,    51,   226,   479,   225,   479,   225,    93,    -1,
      56,   226,   479,   225,   479,   225,   233,    -1,    57,   226,
     479,   225,   479,   392,    -1,    58,   226,   479,   225,   479,
      -1,    -1,   225,    93,    -1,    65,   226,   470,   225,    97,
      -1,    66,   226,   470,    -1,    67,   226,   449,   225,   197,
     226,   449,   225,    95,   225,    94,    -1,    67,   226,   449,
     225,    68,   225,    69,   225,    70,   225,    95,   225,    94,
      -1,    81,   226,   449,   225,   194,   397,    -1,    -1,   397,
     225,   398,    -1,    94,    -1,   190,   226,   456,    -1,   191,
     226,   458,    -1,   192,   226,   402,    -1,   193,   226,   402,
      -1,   195,    -1,   196,    -1,   197,   226,   449,    -1,   198,
     226,   429,    -1,   146,   226,   399,    -1,   101,   226,   437,
      -1,   199,   226,     4,    -1,   126,   226,     4,    -1,     4,
      -1,    95,    -1,   147,    -1,    97,    -1,    72,   226,   470,
      -1,    71,   226,   470,    -1,     3,    -1,   227,     3,   225,
     442,   228,    -1,   145,   226,   452,   225,   454,   225,   454,
     225,     7,    -1,   143,   226,   470,   225,   454,    -1,   143,
     226,   470,   225,   454,   225,   100,    -1,    52,   226,   452,
      -1,    53,   226,   452,    -1,    42,   226,   233,   225,    43,
      -1,    42,   226,   233,   225,    43,   225,   190,   226,   456,
      -1,    39,   226,   452,   225,    41,    -1,    40,   226,   479,
     225,    41,    -1,    23,   226,   470,   225,    99,    -1,    -1,
      38,   226,   452,   225,   442,   225,   430,   431,   434,   436,
     439,   440,   438,   412,   413,    -1,    -1,   305,    -1,    47,
     226,   449,   225,   443,   225,   445,    -1,    47,   226,   449,
     225,   443,   225,   445,   225,   190,   226,   456,    -1,    47,
     226,   449,   225,   443,   225,   445,   225,    92,    -1,    47,
     226,   449,   225,   443,   225,   445,   225,    92,   225,   190,
     226,   456,    -1,    48,   226,   449,    -1,    48,   226,   449,
     225,   190,   226,   456,    -1,   105,   226,   449,   225,   106,
     225,     4,   225,   107,   225,   108,    -1,   105,   226,   449,
     225,   106,   225,     4,   225,   107,   225,   108,   225,   442,
      -1,   105,   226,   449,   225,   106,   225,   109,   225,   107,
      -1,   105,   226,   449,   225,   106,   225,   110,   225,   107,
      -1,   105,   226,   449,   225,   106,   225,   111,   225,   107,
      -1,   105,   226,   449,   225,   106,   225,   107,    -1,   102,
     226,   470,   225,   103,   225,   100,    -1,   134,   226,   470,
     225,     4,    -1,   134,   226,   470,   225,   103,   225,   100,
      -1,   136,   226,   452,   225,   138,    -1,   137,   226,   470,
     225,   138,   225,     4,   225,     4,    -1,   137,   226,   470,
     225,   138,   225,     4,    -1,    49,   226,    96,   225,   449,
      -1,    49,   226,    96,   225,   449,   225,   190,   226,   456,
      -1,   200,   226,   449,   225,   460,    -1,   200,   226,   449,
     225,    84,    -1,   200,   226,   449,    -1,   201,   226,   449,
     225,   148,   225,   442,    -1,   201,   226,   449,    -1,   202,
     226,   449,   225,   149,   225,   460,    -1,   202,   226,   449,
     225,   149,   225,    84,    -1,   202,   226,   449,    -1,   203,
     226,   449,   225,   150,    -1,   203,   226,   449,   225,   152,
      -1,   203,   226,   449,    -1,    63,   226,   461,   225,   449,
      -1,    64,   226,   449,   225,   478,   225,   460,    -1,    60,
     226,   448,   225,   448,   225,   448,   225,   448,    -1,    60,
      -1,   233,    -1,    84,    -1,   233,    -1,    84,    -1,    -1,
     225,   432,    -1,   433,    -1,   433,   225,   432,    -1,    44,
      -1,    45,    -1,    46,    -1,    -1,   104,   226,   435,    -1,
     103,    -1,    -1,   101,   226,   437,    -1,   100,    -1,    -1,
     190,   226,   456,    -1,    -1,    24,   226,    99,    -1,    -1,
      25,   226,     4,    -1,    75,    -1,    84,    -1,    76,    -1,
      84,    -1,    87,    -1,   446,    -1,    84,    -1,    87,    -1,
     446,    -1,    86,   226,    84,    -1,    91,    -1,    84,    -1,
      86,   229,     4,   230,    -1,   233,    -1,   244,    -1,   245,
     229,   461,   230,    -1,   461,    -1,   450,    -1,   216,   227,
     470,   228,    -1,   248,    -1,   249,   229,   461,   230,    -1,
     227,     4,   225,     4,   228,    -1,    84,    -1,    85,   451,
     230,    -1,   224,    -1,   224,   225,   451,    -1,   453,    -1,
     250,    -1,   251,   229,   461,   230,    -1,   227,     4,   225,
       4,   225,     4,   225,     4,   228,    -1,   455,    -1,   252,
      -1,   253,   229,   461,   230,    -1,     3,    -1,   227,     3,
     225,   442,   228,    -1,   457,    -1,   254,    -1,   255,   229,
     461,   230,    -1,   233,    -1,     3,    -1,   227,     3,   225,
     233,   228,    -1,    84,    -1,   459,    -1,   256,    -1,   257,
     229,   461,   230,    -1,   233,    -1,     3,    -1,   227,     3,
     225,   233,   228,    -1,    84,    -1,    98,    -1,   447,    -1,
     460,   285,   447,    -1,     4,    -1,   471,    -1,   227,     8,
     228,    -1,   242,    -1,   243,   229,   461,   230,    -1,   461,
     279,   461,    -1,   461,   280,   461,    -1,   461,   281,   461,
      -1,   461,   282,   461,    -1,   461,   283,   461,    -1,   227,
     461,   228,    -1,   238,    -1,   239,    -1,   309,   226,   462,
      -1,   463,    -1,   464,   225,   463,    -1,    -1,   464,    -1,
     461,    -1,   460,    -1,   466,    -1,   467,   225,   466,    -1,
      -1,   467,    -1,   449,    -1,   209,   452,    -1,   210,   452,
      -1,   211,   449,   225,   449,    -1,   212,   449,   225,   449,
     225,   461,    -1,   213,   227,   470,   228,    -1,   213,   227,
     364,   225,   470,   228,    -1,   219,   227,     7,   225,   470,
     228,    -1,   219,   227,   470,   225,   470,   228,    -1,   219,
     227,   454,   225,   470,   228,    -1,   215,   449,    -1,   217,
     227,   449,   225,   461,   228,    -1,   217,   227,   449,   225,
     461,   225,    44,   228,    -1,   218,   227,   449,   225,   461,
     225,   461,   228,    -1,   218,   227,   449,   225,   461,   225,
     461,   225,    44,   228,    -1,   221,   227,   222,   225,   227,
     461,   280,   461,   298,   228,   225,   449,   299,   228,    -1,
     220,   469,    -1,   258,    -1,   227,   470,   228,    -1,   469,
      -1,   469,   287,   470,    -1,   278,    -1,    99,    -1,     4,
      -1,     8,    -1,     9,    -1,     4,    -1,     8,    -1,     9,
      -1,     4,    -1,   471,    -1,    33,    -1,    26,    -1,    34,
      -1,    28,    -1,    35,    -1,   143,    -1,    79,    -1,    84,
      -1,   480,    -1,    59,   227,     4,   225,     4,   225,     4,
     225,     4,   228,    -1,   227,     4,   225,     4,   225,     4,
     225,     4,   228,    -1
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
    1282,  1289,  1298,  1306,  1318,  1317,  1343,  1342,  1368,  1371,
    1377,  1387,  1393,  1402,  1408,  1413,  1419,  1424,  1430,  1441,
    1447,  1448,  1451,  1452,  1455,  1459,  1465,  1466,  1470,  1476,
    1484,  1489,  1494,  1499,  1504,  1509,  1514,  1522,  1529,  1537,
    1545,  1546,  1549,  1550,  1553,  1558,  1557,  1571,  1578,  1585,
    1593,  1598,  1604,  1610,  1616,  1622,  1627,  1632,  1637,  1642,
    1647,  1652,  1657,  1662,  1667,  1672,  1677,  1682,  1688,  1693,
    1698,  1703,  1708,  1715,  1724,  1728,  1741,  1750,  1749,  1767,
    1777,  1783,  1791,  1797,  1802,  1807,  1812,  1817,  1822,  1827,
    1832,  1837,  1851,  1857,  1862,  1867,  1872,  1877,  1882,  1887,
    1892,  1897,  1902,  1907,  1912,  1917,  1922,  1927,  1932,  1938,
    1943,  1948,  1953,  1960,  1966,  1995,  2000,  2008,  2014,  2018,
    2026,  2033,  2040,  2050,  2060,  2076,  2087,  2090,  2096,  2102,
    2108,  2112,  2118,  2125,  2131,  2139,  2145,  2150,  2155,  2160,
    2165,  2171,  2177,  2182,  2187,  2192,  2197,  2202,  2209,  2209,
    2209,  2209,  2212,  2218,  2224,  2229,  2236,  2243,  2247,  2253,
    2259,  2265,  2270,  2277,  2283,  2293,  2300,  2299,  2333,  2336,
    2342,  2347,  2352,  2357,  2363,  2367,  2373,  2377,  2381,  2385,
    2389,  2393,  2399,  2405,  2409,  2415,  2421,  2425,  2431,  2435,
    2441,  2445,  2450,  2457,  2461,  2468,  2472,  2477,  2484,  2488,
    2493,  2501,  2507,  2514,  2518,  2525,  2533,  2536,  2546,  2550,
    2553,  2559,  2563,  2570,  2574,  2578,  2585,  2588,  2594,  2601,
    2604,  2610,  2617,  2621,  2628,  2631,  2638,  2641,  2647,  2648,
    2651,  2652,  2655,  2656,  2657,  2663,  2664,  2665,  2671,  2672,
    2675,  2684,  2689,  2696,  2707,  2713,  2717,  2721,  2728,  2738,
    2745,  2749,  2755,  2759,  2767,  2771,  2778,  2788,  2801,  2805,
    2812,  2822,  2831,  2842,  2846,  2853,  2863,  2874,  2883,  2893,
    2899,  2903,  2910,  2920,  2931,  2940,  2950,  2954,  2961,  2962,
    2968,  2972,  2976,  2980,  2988,  2997,  3001,  3005,  3009,  3013,
    3017,  3020,  3027,  3036,  3069,  3070,  3073,  3074,  3077,  3081,
    3088,  3095,  3106,  3109,  3117,  3121,  3125,  3129,  3133,  3138,
    3142,  3146,  3151,  3156,  3161,  3165,  3170,  3175,  3179,  3183,
    3188,  3192,  3199,  3205,  3209,  3215,  3222,  3223,  3226,  3227,
    3228,  3231,  3235,  3239,  3243,  3249,  3250,  3253,  3254,  3257,
    3258,  3261,  3262,  3265,  3269,  3295
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
  "DECOTYP_ID", "OBJECT_ID", "COBJECT_ID", "MONSTER_ID", "TRAP_ID",
  "DOOR_ID", "DRAWBRIDGE_ID", "MONSTER_GENERATION_ID", "object_ID",
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
  "CURSE_TYPE", "MYTHIC_TYPE", "ENGRAVING_TYPE", "KEYTYPE_ID", "LEVER_ID",
  "NO_PICKUP_ID", "DIRECTION", "RANDOM_TYPE", "RANDOM_TYPE_BRACKET",
  "A_REGISTER", "ALIGNMENT", "LEFT_OR_RIGHT", "CENTER", "TOP_OR_BOT",
  "ALTAR_TYPE", "ALTAR_SUBTYPE", "UP_OR_DOWN", "ACTIVE_OR_INACTIVE",
  "MODRON_PORTAL_TYPE", "NPC_TYPE", "FOUNTAIN_TYPE", "SPECIAL_OBJECT_TYPE",
  "CMAP_TYPE", "FLOOR_SUBTYPE", "FLOOR_SUBTYPE_ID", "FLOOR_ID",
  "FLOOR_TYPE", "FLOOR_TYPE_ID", "DECORATION_ID", "DECORATION_TYPE",
  "DECORATION_DIR", "DECORATION_ITEM_STATE", "PAINTING_TYPE",
  "BANNER_TYPE", "WALL_SCULPTURE_TYPE", "ELEMENTAL_ENCHANTMENT_TYPE",
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
  "optmontype", "opttileset", "optdecotyp", "door_state", "light_state",
  "alignment", "alignment_prfx", "altar_type", "a_register",
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
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,    44,    58,    40,    41,    91,
      93,   123,   125,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,   523,   524,   525,    43,
      45,    42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   288,   289,   289,   290,   290,   291,   292,   292,   293,
     293,   293,   293,   294,   295,   295,   296,   297,   298,   298,
     299,   299,   300,   300,   301,   301,   302,   302,   303,   303,
     304,   304,   305,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   309,   309,   309,   310,
     310,   311,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   313,   313,   314,
     314,   315,   315,   316,   316,   317,   317,   318,   318,   319,
     319,   321,   322,   320,   323,   324,   325,   325,   326,   326,
     326,   328,   329,   327,   330,   330,   332,   331,   333,   331,
     334,   335,   335,   336,   338,   337,   340,   339,   342,   341,
     344,   343,   345,   346,   345,   347,   347,   348,   348,   348,
     349,   349,   350,   351,   353,   352,   355,   354,   356,   356,
     357,   357,   358,   358,   359,   359,   360,   360,   361,   361,
     362,   362,   363,   363,   364,   364,   365,   365,   366,   366,
     367,   367,   367,   367,   367,   367,   367,   368,   368,   368,
     369,   369,   370,   370,   371,   372,   371,   373,   374,   374,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   376,   376,   376,   377,   378,   377,   379,
     380,   380,   381,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   382,   383,   384,   384,   385,   385,   385,
     386,   387,   388,   389,   390,   391,   392,   392,   393,   394,
     395,   395,   396,   397,   397,   398,   398,   398,   398,   398,
     398,   398,   398,   398,   398,   398,   398,   398,   399,   399,
     399,   399,   400,   401,   402,   402,   403,   404,   404,   405,
     406,   407,   407,   408,   409,   410,   412,   411,   413,   413,
     414,   414,   414,   414,   415,   415,   416,   416,   416,   416,
     416,   416,   417,   418,   418,   419,   420,   420,   421,   421,
     422,   422,   422,   423,   423,   424,   424,   424,   425,   425,
     425,   426,   427,   428,   428,   429,   429,   430,   430,   431,
     431,   432,   432,   433,   433,   433,   434,   434,   435,   436,
     436,   437,   438,   438,   439,   439,   440,   440,   441,   441,
     442,   442,   443,   443,   443,   444,   444,   444,   445,   445,
     446,   447,   447,   447,   448,   449,   449,   449,   449,   450,
     450,   450,   451,   451,   452,   452,   452,   453,   454,   454,
     454,   455,   455,   456,   456,   456,   457,   457,   457,   457,
     458,   458,   458,   459,   459,   459,   459,   459,   460,   460,
     461,   461,   461,   461,   461,   461,   461,   461,   461,   461,
     461,   462,   462,   463,   464,   464,   465,   465,   466,   466,
     467,   467,   468,   468,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   470,   470,   471,   472,   472,   473,   473,
     473,   474,   474,   474,   474,   475,   475,   476,   476,   477,
     477,   478,   478,   479,   479,   480
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
       5,     5,     7,     4,     0,    15,     0,    17,     0,     2,
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
       3,     5,     9,     5,     5,     5,     0,    15,     0,     1,
       7,    11,     9,    13,     3,     7,    11,    13,     9,     9,
       9,     7,     7,     5,     7,     5,     9,     7,     5,     9,
       5,     5,     3,     7,     3,     7,     7,     3,     5,     5,
       3,     5,     7,     9,     1,     1,     1,     1,     1,     0,
       2,     1,     3,     1,     1,     1,     0,     3,     1,     0,
       3,     1,     0,     3,     0,     3,     0,     3,     1,     1,
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
       2,     0,     0,     0,     3,     4,    26,     0,     0,     1,
       5,     0,    30,     0,     7,     0,   158,     0,     0,     0,
       0,   227,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   307,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   394,     0,
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
     470,     0,   473,     0,   515,     0,   471,   492,    31,     0,
     178,     0,     8,     0,   431,   432,     0,   468,   185,     0,
       0,     0,    11,   517,   516,    13,   440,     0,   230,   231,
       0,     0,   437,     0,     0,   198,   435,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     511,   494,   513,     0,   464,   466,   467,     0,   463,   461,
       0,   266,   270,   460,   267,   457,   459,     0,   456,   454,
       0,   234,     0,   453,   396,   395,     0,   418,   419,     0,
       0,     0,     0,   309,   308,     0,   445,     0,     0,   444,
       0,     0,     0,     0,   533,     0,     0,   364,     0,     0,
       0,     0,     0,   349,   350,   398,   397,     0,   156,     0,
       0,     0,     0,   434,     0,     0,     0,     0,     0,   319,
       0,   343,   342,   520,   518,   519,   189,   188,     0,     0,
       0,     0,     0,     0,     0,   210,   211,     0,     0,     0,
       0,    15,   121,     0,     0,     0,   382,   384,   387,   390,
     151,     0,     0,     0,     0,   160,     0,     0,     0,     0,
       0,   489,   488,   490,   493,     0,   526,   528,   525,   527,
     529,   530,     0,     0,     0,   128,   129,   124,   122,     0,
       0,     0,     0,    30,   175,    28,     0,     0,     0,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     495,   496,     0,     0,     0,   504,     0,     0,     0,   510,
       0,     0,     0,     0,     0,     0,   269,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   157,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   182,   181,     0,     0,     0,   176,     0,
       0,     0,     0,     0,     0,   486,   472,   480,     0,     0,
     475,   476,   477,   478,   479,     0,   154,     0,   470,     0,
       0,     0,     0,   145,   143,   149,   147,     0,     0,     0,
     179,     0,     0,   186,   469,    10,   344,     0,     9,     0,
       0,   441,     0,     0,     0,   233,   232,   198,   199,   229,
       0,     0,   214,     0,     0,     0,     0,   451,     0,     0,
     449,     0,     0,   448,     0,     0,   512,   514,   355,     0,
       0,     0,   268,     0,     0,   236,   238,   303,   218,     0,
      16,   139,   305,     0,     0,   420,   421,     0,   353,     0,
       0,   354,   351,   424,     0,   422,     0,   423,     0,   378,
     310,     0,   311,     0,   201,     0,     0,     0,     0,   316,
     315,     0,     0,   190,   191,   391,   531,   532,     0,   318,
       0,     0,   323,     0,     0,   203,     0,     0,   373,     0,
     375,     0,     0,     0,     0,   347,     0,    14,     0,   172,
       0,     0,   162,   381,   380,     0,     0,   388,   389,     0,
     484,   487,     0,   474,   159,   491,   123,     0,     0,   132,
       0,   131,     0,   130,     0,   136,     0,   127,     0,   126,
       0,   125,    32,   433,     0,     0,   443,   436,     0,   438,
       0,   497,     0,     0,     0,   499,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   462,   523,   521,   522,     0,
     281,   278,   272,   298,     0,   297,     0,     0,     0,   296,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,   282,     0,   284,   285,   295,     0,   280,   271,   286,
     524,   274,     0,   455,   237,   209,     0,     0,     0,     0,
     446,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   193,     0,     0,   314,     0,     0,     0,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   177,   171,   173,     0,     0,     0,     0,
       0,   152,     0,   144,   146,   148,   150,     0,   137,     0,
       0,   141,     0,     0,   439,   228,     0,   215,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   465,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   458,     0,     0,   304,   140,
      22,     0,   399,     0,     0,     0,     0,   429,   428,   360,
     365,     0,   312,     0,   205,     0,     0,   313,   317,     0,
       0,   392,     0,     0,     0,   372,     0,   371,     0,     0,
       0,     0,   207,     0,   399,   374,   377,   213,     0,   212,
     184,   348,     0,   164,   383,   386,   385,   481,   482,   483,
     485,     0,     0,   135,   134,     0,   133,     0,     0,   498,
     500,     0,   505,     0,   501,     0,   450,   503,   502,     0,
     301,   294,   288,   287,   289,   299,   300,   302,   290,   291,
     293,   275,   292,   276,   279,   283,   273,     0,   425,   241,
     242,     0,   247,   245,   246,   258,   259,   248,   249,   251,
       0,     0,   260,     0,   262,     0,   255,   256,     0,   239,
     243,   426,   240,     0,   224,   225,     0,   226,     0,   223,
     219,     0,   306,     0,     0,   406,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,   216,   217,     0,     0,
       0,   325,     0,     0,     0,     0,     0,     0,     0,   330,
     331,     0,     0,     0,   324,     0,     0,     0,     0,     0,
       0,   406,     0,     0,     0,     0,     0,     0,   164,   153,
     138,   142,   345,     0,     0,     0,   452,     0,     0,   244,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
       0,   403,   404,   405,   400,   401,     0,   409,     0,     0,
     352,   362,     0,   379,   200,     0,   399,   393,   192,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   368,   369,   370,   202,     0,   409,   376,   218,
     346,     0,   168,   163,   165,     0,   506,     0,   507,   476,
     427,   250,   252,   253,   261,   254,   264,   263,   257,   221,
     222,   220,     0,     0,     0,     0,   414,     0,     0,     0,
       0,     0,   406,     0,     0,   411,   335,   337,   338,   339,
     341,   340,   334,   326,   327,   328,   329,   332,   333,   336,
       0,     0,   414,   208,   166,    30,     0,     0,     0,     0,
       0,   447,   402,   408,   407,     0,     0,   416,     0,   535,
       0,   361,   204,   409,     0,   320,   366,   206,   416,    30,
     169,     0,   508,    19,     0,   265,   410,     0,     0,   412,
     534,     0,   414,     0,     0,   412,   167,     0,     0,   415,
       0,     0,   356,   363,   416,   321,   367,   194,     0,    20,
     417,     0,   358,   412,     0,    24,    25,    22,     0,     0,
     413,   359,   357,   196,   195,    12,    21,   509,     0,   197
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,   102,   103,   104,   105,   106,
    1129,  1189,   962,  1187,    12,   176,   107,   424,   108,   109,
     110,   111,   233,   112,   113,   797,   590,   800,   529,   530,
     531,   532,   114,   515,   891,   115,   116,   484,   117,   118,
     234,   786,  1007,  1008,  1149,  1125,   119,   651,   120,   121,
     251,   122,   648,   123,   422,   124,   378,   504,   643,   125,
     126,   127,   355,   347,   128,  1184,   129,  1198,   439,   616,
     637,   856,   874,   130,   377,   878,   563,   978,   745,   960,
     131,   274,   557,   132,   460,   311,   744,   949,  1088,   133,
     457,   301,   456,   738,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   766,   144,   145,   146,   147,   772,
     994,  1112,   148,   149,   548,   150,   151,   152,   153,   154,
     155,   156,   157,   158,  1182,  1192,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   316,   348,   965,  1034,  1035,  1037,  1134,  1096,  1106,
    1172,  1137,  1159,   319,   597,   606,   950,   849,   607,   257,
     352,   291,   276,   433,   328,   329,   572,   573,   312,   313,
     302,   303,   401,   353,   889,   660,   661,   662,   403,   404,
     405,   292,   451,   246,   265,   367,   741,   419,   420,   421,
     628,   333,   334
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1057
static const yytype_int16 yypact[] =
{
     198,  -144,  -113,   121, -1057,   198,    14,  -104,   -84, -1057,
   -1057,   -51,   810,    -5, -1057,   108, -1057,     7,    55,   100,
     117, -1057,   140,   170,   177,   183,   195,   197,   209,   215,
     216,   217,   231,   234,   238,   242,   245,   246,   253,   254,
     255,   258,   259,   260,   263,   264,   267,   268,   269,   274,
     276,   278,   287,   290,   293,   295,   296,   297,   301,   302,
     303,   310,   312,   313,   317,   320,    45,   321,   322,   324,
   -1057,   325,    16,   563, -1057, -1057,   331,   338,   339,   345,
       0,    23,    62, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,   810, -1057,
   -1057,   222, -1057, -1057, -1057, -1057, -1057,   346, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057,   299,   105, -1057,  -114,   417,    63,
     243,   375,   895,    19,    19,   119,   -11,    76,   -32,   510,
     -32,   882,  -109,  -109,   -15,   291,   -32,   -32,   446,   -32,
       5,  -109,  -109,    -9,   -15,   -15,   -15,    23,   347,    23,
     -32,   895,   895,   -32,   895,   895,   103,   -32,   895,   -32,
      -9,   895,  -109,   895,    86, -1057,   895,  -109,   421,   680,
      23, -1057, -1057,   294,   361,   -32,   -32,   -32,   -32,   353,
   -1057,    28, -1057,   364, -1057,   165, -1057,   112, -1057,   120,
   -1057,   363, -1057,   108, -1057, -1057,   368, -1057,  -146,   378,
     382,   383, -1057, -1057, -1057, -1057, -1057,   390, -1057, -1057,
     388,   612, -1057,   394,   401,   403, -1057, -1057,  -109,  -109,
     -32,   -32,   392,   -32,   402,   411,   412,   895,   413,   497,
   -1057, -1057,   354,   430, -1057, -1057, -1057,   653, -1057, -1057,
     428, -1057, -1057, -1057, -1057, -1057, -1057,   656, -1057, -1057,
     432,   431,   438, -1057, -1057, -1057,   439, -1057, -1057,   458,
     459,   462,   463, -1057, -1057,   685, -1057,   461,   467, -1057,
     473,   499,   723,   503, -1057,   504,   509,   512,   513,   514,
     731,   516,   518, -1057, -1057, -1057, -1057,   519,   741,   529,
     531,   532,   533,   304,   755,   541,   194,   565,   566, -1057,
     567, -1057, -1057, -1057, -1057, -1057, -1057, -1057,   573,   577,
     578,   579,   587,   589,   593, -1057, -1057,   595,   363,   598,
     600,   601, -1057,   604,    23,    23,   610,   619,   620,   628,
   -1057,   627,   397,    23,    23, -1057,    23,    23,    23,    23,
      23,   569,   304, -1057,   640,   643, -1057, -1057, -1057, -1057,
   -1057, -1057,   652,    35,    15, -1057, -1057,   569,   304,   654,
     657,   659,   810,   810, -1057, -1057,    23,   871,  -114,   884,
      13,   885,   664,   660,   895,   672,    23,   298,   893,   665,
   -1057, -1057,   686,   688,   552, -1057,   -32,   -32,   433, -1057,
     692,   682,   895,   817,   699,    23,   701,   363,   703,    23,
     363,   -32,   -32,   895,   834,   134,   847,   707,    23,    44,
     899,   930,   711,   901,   900,   307,   758,   -32,   852,   724,
     859,   -15,   -29, -1057,   729,   -15,   -15,   -15,    23,   732,
      37,   -32,    31,   861,    30,   762,   856,   854,    -8,    56,
     824,   825,    76,   826, -1057,    89,    89,   816, -1057,   252,
     742,   -37,   828,   829,    33,   748, -1057, -1057,   440,   495,
      94,    94, -1057, -1057, -1057,   112, -1057,   895,   749,  -100,
     -75,   -31,   -19, -1057, -1057,   569,   304,   102,   153,   158,
   -1057,   745,   501, -1057, -1057, -1057, -1057,   978, -1057,   757,
     390, -1057,   759,   979,   506, -1057, -1057,   403, -1057, -1057,
     -32,   -32,   700,   760,   780,   763,   789, -1057,   791,   484,
   -1057,   790,   793, -1057,   795,   796, -1057, -1057, -1057,   792,
     613,   340, -1057,   794,   621, -1057, -1057, -1057, -1057,   797,
     798, -1057,   799,  1022,   626, -1057, -1057,   803, -1057,   804,
    1026, -1057,   806, -1057,   805, -1057,   807, -1057,   809,   808,
   -1057,  1032, -1057,   812, -1057,  1034,   815,    44,   818,   819,
   -1057,   822,   959, -1057, -1057, -1057, -1057, -1057,   823, -1057,
     845,   848, -1057,   850,   851, -1057,  1045,   853, -1057,   855,
   -1057,   857,   858,   908,  1048,   860,   862, -1057,   363, -1057,
     801,    23, -1057, -1057,   569,   863,   864, -1057, -1057,   870,
   -1057,   865,   849, -1057, -1057, -1057, -1057,  1075,   890, -1057,
       1, -1057,    23, -1057,  -114, -1057,    92, -1057,   134, -1057,
      46, -1057, -1057, -1057,   896,  1078, -1057, -1057,   892, -1057,
     889, -1057,   902,  1001,   895, -1057,    23,    23,   895,   903,
      23,   895,   895,   897,   898, -1057, -1057, -1057, -1057,   906,
   -1057, -1057, -1057, -1057,   907, -1057,   909,   910,   911, -1057,
     912,   913,   915,   916,   919,   920,   921,   922,   923, -1057,
     924, -1057,   925, -1057, -1057, -1057,   926, -1057, -1057, -1057,
   -1057, -1057,   927, -1057,   904,   929,    76,   134,  1129,   932,
   -1057,    -9,  1154,   950,   987,  1172,   145,   119,   988,    59,
    1086,   955,    -1, -1057,   948,  1089, -1057,    23,   958,  -114,
    1115,   -32,   960,  1087,    57,   961,     3,  1088,  1185,   316,
     363,  1090,    89, -1057, -1057,   304,   962,    44,   -34,   174,
     748, -1057,    79, -1057, -1057,   304,   569,     6, -1057,     9,
      24, -1057,    44,   966, -1057, -1057,    23, -1057,   964,   330,
     280,   967,    44,   639,   968,   969,    23, -1057,    23,    19,
    1081,  1091,    23,  1079,  1080,  1077,    23,    23,    23,  -114,
    1042,    23,    23,    23,   119, -1057,   986,    17, -1057, -1057,
     977,  1200,   980,   981,  1203,   982,   983, -1057, -1057,   984,
   -1057,   985, -1057,  1206, -1057,   344,   989, -1057, -1057,   990,
     104,   569,   991,   992,   476, -1057,   993, -1057,   995,   996,
     997,  1208, -1057,  1219,   980, -1057,   999, -1057,  1000, -1057,
   -1057, -1057,  1002,    27, -1057, -1057,   569, -1057, -1057, -1057,
   -1057,   363,    92, -1057, -1057,    46, -1057,   998,  1223,   304,
   -1057,  1188, -1057,    23, -1057,  1005, -1057, -1057, -1057,   470,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057,   569, -1057, -1057, -1057, -1057, -1057,    18, -1057, -1057,
   -1057,  -114, -1057, -1057, -1057, -1057, -1057, -1057,  1003, -1057,
    1008,  1009, -1057,  1010, -1057,  1011, -1057, -1057,  1012, -1057,
   -1057, -1057,   569,  1013, -1057, -1057,  1014, -1057,  1015, -1057,
   -1057,  1239, -1057,  1018,   406,  1140,  1241,  1021,   119, -1057,
     -12,   119,  1019,  1023,     3,    23, -1057, -1057,  1024,  1179,
    1155, -1057,  1025,  1027,  1028,  1029,  1030,  1031,  1033, -1057,
   -1057,  1035,  1036,  1037, -1057,  1151,  1153,  1157,  1158,  1038,
    1043,  1140,  1263,   104,  1262,   288,  1044,  1039,    27, -1057,
   -1057, -1057, -1057,  1047,  1046,   351, -1057,    23,  1189,   569,
      23,    23,    23,    23,    23,   -95,    19,  1271,  1130, -1057,
    1272, -1057, -1057, -1057, -1057,  1053,  1054,  1178,  1056,  1278,
   -1057,  1058,  1059, -1057, -1057,   298,   980, -1057, -1057,  1061,
    1062,  1184,  1284,    50,   119,    19,    13,    13,   -32,   -11,
    1285,  1065, -1057, -1057, -1057, -1057,  1287,  1178, -1057, -1057,
   -1057,  1066, -1057, -1057, -1057,  1288, -1057,  1250, -1057,  -102,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057,  1016, -1057, -1057,
   -1057, -1057,  1067,   406,  1193,  1071,  1274,  1295,  1072,  1111,
     119,  1076,  1140,  1210,  1209, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
    1198,  1082,  1274,   929, -1057,   810,  1083,  1084,  1092,  1085,
     -95, -1057, -1057, -1057, -1057,  1184,  1093,  1282,  1094, -1057,
    1095, -1057, -1057,  1178,  1098, -1057,  1099, -1057,  1282,   810,
   -1057,    44, -1057, -1057,  1100, -1057, -1057,  1212,  1101,  1119,
   -1057,   119,  1274,  1220,    44,  1119, -1057,  1103,   -32, -1057,
    1312,  1104, -1057, -1057,  1282, -1057, -1057, -1057,    88,  1106,
   -1057,   119,   363,  1119,   363, -1057, -1057,   977,   -32,  1105,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,   363, -1057
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1057, -1057,  1313, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057,   130, -1057, -1057,  1073,  -108,  -376,   914,  1108,
    1247,  -502, -1057, -1057, -1057, -1057,   651, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,  1266, -1057,
   -1057, -1057,   326, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057,   866,  1118, -1057, -1057, -1057, -1057,   778, -1057,
   -1057, -1057,   365, -1057, -1057, -1057,  -605,   337,   272, -1057,
   -1057,   487,   300, -1057, -1057, -1057, -1057, -1057,   213, -1057,
   -1057,  1160, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057,  -618, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057,   289,   596,  -837,   256, -1057,  -933, -1057,  -961,   211,
   -1056, -1026, -1034,  -477,  -610, -1057, -1057, -1057,   515,   931,
    -227,  -179,  -391,   800,   204,  -388,  -491,  -609,  -534,  -453,
    -529,  -641,  -171,   -76, -1057,   562, -1057, -1057,   830, -1057,
   -1057,  1070,  -149,   772, -1057,  -480, -1057, -1057, -1057, -1057,
   -1057,  -176, -1057
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -236
static const yytype_int16 yytable[] =
{
     248,   275,   503,   383,   679,   245,   258,   763,   677,   320,
     624,   322,   591,   659,   645,   646,   546,   336,   337,   240,
     339,   341,   294,   533,   342,   642,   534,   240,   349,   350,
     351,   357,   240,   293,   360,   798,   391,  1001,   368,   528,
     370,   363,   324,   391,   331,   364,   365,   653,   681,   567,
     885,    16,   266,   267,  1108,   614,   386,   387,   388,   389,
     638,   866,   358,   359,   331,   361,   362,   263,  1067,   369,
     415,   801,   372,   314,   374,   345,   635,   379,   417,   427,
    1041,   582,     7,   854,   585,   266,   267,   872,   807,   266,
     267,   375,   567,  1185,  1086,   294,  1148,   953,   630,   266,
     267,   442,   443,   295,   445,   294,  1122,   363,   976,  1177,
     626,   364,   365,     8,  1165,   627,   240,   296,   325,   254,
     595,     9,   305,  1128,   240,   668,   -18,  1193,   596,    13,
     255,   256,   669,   356,   954,   955,  1174,   305,  1087,   428,
    1183,   326,   327,   956,    11,  1109,   406,  1110,   407,    14,
     670,   317,   957,   408,   409,   410,   305,   671,   510,   639,
     318,   567,   264,   958,   867,   392,   868,   869,   870,  1143,
     376,   402,  1186,   418,   879,    15,   295,   884,  1042,   398,
     399,   400,  1162,   657,   270,   658,   295,   366,   977,   959,
     296,  1005,   897,  1006,   672,   271,   254,  1111,   615,   254,
     296,   673,   905,   306,   266,   267,   674,   255,   256,  1102,
     255,   256,   332,   675,     1,     2,   272,   273,   306,   636,
     174,   270,   315,   850,   346,   591,   855,   631,   271,   847,
     873,   892,   340,   177,   747,   239,   848,   306,   893,   175,
     547,   894,   413,   535,  1018,   230,   297,   755,   254,   895,
     241,  1010,   298,   272,   273,   241,   896,   242,   243,   255,
     256,   621,   241,   411,   354,   242,   243,   565,   566,   838,
     242,   243,   783,   644,    81,   299,   300,   242,   243,   794,
     793,   178,   586,   587,   844,   552,  1011,   804,   659,   247,
     911,   882,   363,   244,   839,   564,   364,   365,   609,   574,
     926,   244,   252,   577,   841,   613,   244,   804,   509,   618,
     619,   620,   625,   244,   588,   541,   644,   518,   519,   297,
     520,   521,   522,   523,   524,   298,   179,   266,   267,   297,
     253,   268,   269,   676,   412,   298,   270,   392,   536,   241,
     654,   570,   571,   180,   706,   254,   307,   413,   707,   708,
     542,   414,   308,   254,   242,   243,   255,   256,   299,   300,
     554,   307,   242,   243,   255,   256,   181,   308,   272,   273,
     326,   327,   394,   309,   310,   398,   399,   400,   666,   580,
     307,   691,   692,   584,   678,   644,   308,   555,   556,   680,
     244,   603,   594,   604,   605,   395,   182,   330,   244,   562,
     877,   709,   739,   183,   880,   343,   344,   309,   310,   184,
     570,   571,   887,   888,   649,   710,   711,   712,   713,   491,
     714,   185,   715,   186,   266,   267,   373,   259,   260,   261,
     262,   380,   268,   269,  1040,   187,   567,  1043,  1115,  1116,
     568,   188,   189,   190,   396,   397,   398,   399,   400,   402,
    1031,  1032,  1033,   416,   716,   717,   718,   191,   719,   270,
     192,   720,   721,   722,   193,   723,   724,   725,   194,   726,
     271,   195,   196,   396,   397,   398,   399,   400,   277,   197,
     198,   199,   440,   441,   200,   201,   202,   699,   435,   203,
     204,   272,   273,   205,   206,   207,   727,  1089,   728,   729,
     208,   435,   209,   796,   210,   903,   249,   730,   731,   732,
     733,   734,   735,   211,   321,  1009,   212,   266,   267,   213,
    1113,   214,   215,   216,   335,  1071,  1114,   217,   218,   219,
     736,   396,   397,   398,   399,   400,   220,   650,   221,   222,
     859,  1167,   338,   223,   737,   808,   224,   226,   227,   811,
     228,   229,   814,   815,  1176,   901,   270,   235,   902,   396,
     397,   398,   399,   400,   236,   237,  1141,   271,   266,   267,
     981,   238,   250,   381,   354,   785,  1077,   982,   384,  1078,
     390,   266,   267,   396,   397,   398,   399,   400,   272,   273,
     385,   910,   863,   393,   423,   914,   795,   426,   861,   918,
     919,   920,   983,   429,   923,   924,   925,   430,   431,   396,
     397,   398,   399,   400,   432,   434,   435,   886,   244,   444,
     809,   810,   984,   436,   813,   517,   437,  1173,   438,   446,
     396,   397,   398,   399,   400,   562,   266,   267,   447,   448,
     450,   452,   278,   279,   280,   281,   282,  1190,   283,   270,
     284,   285,   286,   287,   288,   453,   454,   455,   921,   458,
     569,   459,  -235,   461,   462,   952,   985,   986,   987,   988,
     663,   989,   990,   991,   992,   993,   396,   397,   398,   399,
     400,   272,   273,   463,   464,   570,   571,   465,   466,   467,
     468,   290,   469,   278,   279,   280,   281,   282,   470,   283,
     270,   284,   285,   286,   287,   288,   278,   279,   280,   281,
     282,   289,   283,   270,   284,   285,   286,   287,   288,   396,
     397,   398,   399,   400,   289,   664,   471,   472,   473,   474,
     899,   683,   272,   273,   475,   479,   689,   476,   477,   478,
     909,   480,   290,   481,   482,   272,   273,   483,  1047,   396,
    1017,   398,   399,   400,   485,   290,   486,   487,   488,   489,
    1019,   278,   279,   280,   281,   282,   490,   283,   270,   284,
     285,   286,   287,   288,   396,   397,   398,   399,   400,   289,
     396,   397,   398,   399,   400,   396,   397,   398,   399,   400,
     492,   493,   494,  1081,  1082,  1083,  1084,  1085,   495,   231,
     272,   273,   496,   497,   498,    84,  1191,    86,  1194,    88,
     290,    90,   499,    92,   500,    94,    16,    96,   501,    98,
     502,   100,  1199,   505,    17,   506,   507,  1015,    18,    19,
      20,    21,    22,    23,   508,   511,    24,    25,    26,    27,
      28,    29,    30,   705,   512,   513,    31,    32,    33,    34,
      35,   743,    36,   514,   428,   516,   750,    37,    38,    39,
      40,    41,    42,    43,    44,   525,    45,    46,    47,   906,
      48,   526,    49,    50,    51,    52,    53,    54,   527,  1117,
     537,    55,    56,   538,    57,   539,   543,   545,   549,   550,
     551,    58,   396,   397,   398,   399,   400,   553,   558,   559,
     396,   397,   398,   399,   400,   396,   397,   398,   399,   400,
     576,   560,    59,   561,   575,    60,   578,   589,   396,   397,
     398,   399,   400,    85,   579,    87,   581,    89,   583,    91,
     592,    93,   593,    95,   599,    97,   600,    99,    61,   101,
     598,  1079,   601,   602,    62,   610,    63,    64,   608,   611,
      65,    66,   612,    67,   617,    68,   632,   622,   629,   633,
     634,    69,   640,   641,    70,    71,   266,   267,  -183,   647,
      72,    73,   652,    74,   667,    75,   655,   682,   656,   266,
     267,   684,   685,   688,    83,   694,   693,   687,   696,  1179,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   695,  1196,
      76,    77,    78,    79,   697,    80,   698,  1150,   701,   700,
     702,   703,   746,   747,   748,   704,   749,   742,   751,   752,
     753,   754,   756,   758,   755,   757,   759,   760,   761,    81,
     762,  1166,   768,   764,   765,    82,    83,   767,   769,   775,
     780,   699,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     770,   323,   927,   928,   771,   773,   774,   791,   776,   792,
     777,   803,   778,   779,   562,   781,   784,   782,   787,   788,
     790,   278,   279,   280,   281,   282,   789,   283,   270,   284,
     285,   286,   287,   288,   278,   279,   280,   281,   282,   289,
     283,   270,   284,   285,   286,   287,   288,   325,   929,   930,
     804,   802,   289,   805,   816,   931,   817,   806,   812,   836,
     272,   273,   818,   819,   840,   820,   821,   822,   823,   824,
     290,   825,   826,   272,   273,   827,   828,   829,   830,   831,
     832,   833,   834,   290,   837,   835,   932,   841,   843,   933,
     934,   935,   936,   937,   938,   939,   940,   941,   942,   943,
     944,   945,   946,   947,   948,   844,   846,   845,   851,   852,
     853,   857,   858,   860,   862,   864,   871,   865,   875,   876,
     881,   898,   900,   883,   912,   904,   907,   908,   915,   922,
     916,   917,   961,   913,   963,   964,   966,   967,   968,   970,
     972,   971,   999,   969,   974,   975,   979,   980,   995,   254,
     996,   997,   998,  1000,  1002,  1003,  1012,  1004,  1013,  1020,
     255,   256,  1014,  1016,  1021,  1022,  1023,  1024,  1025,  1026,
    1027,  1028,  1029,  1030,  1036,  1038,  1039,  1044,  1045,  1049,
    1050,  1051,  1048,  1052,  1053,  1054,  1055,  1056,  1061,  1057,
    1062,  1058,  1059,  1060,  1063,  1064,  1065,  1068,  1066,  1070,
    1072,  1073,  1075,  1080,  1076,  1090,  1092,  1091,  1093,  1095,
    1094,  1097,  1098,  1099,  1105,  1100,  1103,  1104,  1107,  1119,
    1120,  1121,  1124,  1126,  1127,  1131,  1133,  1135,  1136,  1138,
    1139,  1140,  1130,  1145,  1142,  1144,  1146,  1158,  1151,  1171,
    1147,  1169,  1152,  1154,  1175,  1153,  1180,  1195,    10,  1157,
     232,  1161,  1160,  1163,  1164,  1168,   425,  1170,  1178,   799,
    1181,  1188,   225,  1197,  1074,   690,   540,   382,   371,  1046,
    1069,  1123,   973,  1155,   304,  1101,  1156,   842,  1118,  1132,
     686,   951,   890,   740,     0,   665,   623,   449,     0,   544
};

static const yytype_int16 yycheck[] =
{
     108,   180,   378,   230,   538,    81,   177,   617,   537,   188,
     490,   190,   465,   515,   505,   506,     3,   196,   197,     4,
     199,   200,     3,   414,   200,   502,   414,     4,   204,   205,
     206,   210,     4,   182,   213,   676,     8,   874,   217,     4,
     219,     4,   191,     8,    59,     8,     9,    84,   539,     3,
      84,     6,    84,    85,     4,    84,   235,   236,   237,   238,
       4,     4,   211,   212,    59,   214,   215,     4,  1001,   218,
     249,   680,   221,    84,   223,    84,    84,   226,   249,   225,
      92,   457,   226,    84,   460,    84,    85,    84,   693,    84,
      85,     5,     3,     5,   189,     3,  1122,    80,    68,    84,
      85,   280,   281,    84,   283,     3,  1067,     4,     4,  1165,
      79,     8,     9,   226,  1148,    84,     4,    98,   227,   233,
      76,     0,     3,   225,     4,   225,   228,  1183,    84,   233,
     244,   245,   232,   209,   117,   118,  1162,     3,   233,   285,
    1174,   250,   251,   126,   130,    95,    26,    97,    28,   233,
     225,    75,   135,    33,    34,    35,     3,   232,   385,   103,
      84,     3,    99,   146,   107,   241,   109,   110,   111,  1102,
      84,   247,    84,   249,   779,   226,    84,   787,   190,   281,
     282,   283,  1143,   150,   216,   152,    84,    84,    84,   172,
      98,   164,   802,   166,   225,   227,   233,   147,   227,   233,
      98,   232,   812,    84,    84,    85,   225,   244,   245,  1046,
     244,   245,   227,   232,    16,    17,   248,   249,    84,   227,
     225,   216,   233,   757,   233,   678,   227,   197,   227,    84,
     227,   225,   227,   226,   225,   235,    91,    84,   232,   131,
     227,   232,   227,   414,   226,   229,   227,   229,   233,   225,
     227,   892,   233,   248,   249,   227,   232,   242,   243,   244,
     245,   488,   227,   143,   227,   242,   243,   446,   447,   746,
     242,   243,   648,   227,   229,   256,   257,   242,   243,   670,
     668,   226,   461,   462,   225,   434,   895,   228,   790,   227,
     819,   782,     4,   278,   747,   444,     8,     9,   477,   448,
     834,   278,     3,   452,   225,   481,   278,   228,   384,   485,
     486,   487,   491,   278,   463,   423,   227,   393,   394,   227,
     396,   397,   398,   399,   400,   233,   226,    84,    85,   227,
     225,    88,    89,   231,   214,   233,   216,   413,   414,   227,
     511,   252,   253,   226,     4,   233,   227,   227,     8,     9,
     426,   231,   233,   233,   242,   243,   244,   245,   256,   257,
     436,   227,   242,   243,   244,   245,   226,   233,   248,   249,
     250,   251,   207,   254,   255,   281,   282,   283,   527,   455,
     227,   560,   561,   459,   231,   227,   233,    89,    90,   231,
     278,    84,   468,    86,    87,   230,   226,   193,   278,    83,
      84,    61,   581,   226,   780,   201,   202,   254,   255,   226,
     252,   253,   238,   239,   162,    75,    76,    77,    78,   225,
      80,   226,    82,   226,    84,    85,   222,    10,    11,    12,
      13,   227,    88,    89,   968,   226,     3,   971,  1056,  1057,
       7,   226,   226,   226,   279,   280,   281,   282,   283,   525,
      44,    45,    46,   249,   114,   115,   116,   226,   118,   216,
     226,   121,   122,   123,   226,   125,   126,   127,   226,   129,
     227,   226,   226,   279,   280,   281,   282,   283,   103,   226,
     226,   226,   278,   279,   226,   226,   226,     3,     4,   226,
     226,   248,   249,   226,   226,   226,   156,  1026,   158,   159,
     226,     4,   226,   674,   226,   225,   284,   167,   168,   169,
     170,   171,   172,   226,     4,   891,   226,    84,    85,   226,
    1054,   226,   226,   226,   233,  1005,  1055,   226,   226,   226,
     190,   279,   280,   281,   282,   283,   226,   285,   226,   226,
     767,  1151,    96,   226,   204,   694,   226,   226,   226,   698,
     226,   226,   701,   702,  1164,   225,   216,   226,   228,   279,
     280,   281,   282,   283,   226,   226,  1100,   227,    84,    85,
      94,   226,   226,   152,   227,   651,   225,   101,   284,   228,
     227,    84,    85,   279,   280,   281,   282,   283,   248,   249,
     229,   818,   771,   229,   231,   822,   672,   229,   769,   826,
     827,   828,   126,   225,   831,   832,   833,   225,   225,   279,
     280,   281,   282,   283,   224,   227,     4,   788,   278,   227,
     696,   697,   146,   229,   700,   228,   225,  1161,   225,   227,
     279,   280,   281,   282,   283,    83,    84,    85,   227,   227,
     227,   287,   209,   210,   211,   212,   213,  1181,   215,   216,
     217,   218,   219,   220,   221,   225,     3,   229,   829,     3,
     227,   229,   231,   225,   225,   836,   190,   191,   192,   193,
     230,   195,   196,   197,   198,   199,   279,   280,   281,   282,
     283,   248,   249,   225,   225,   252,   253,   225,   225,     4,
     229,   258,   225,   209,   210,   211,   212,   213,   225,   215,
     216,   217,   218,   219,   220,   221,   209,   210,   211,   212,
     213,   227,   215,   216,   217,   218,   219,   220,   221,   279,
     280,   281,   282,   283,   227,   230,   227,     4,   225,   225,
     806,   230,   248,   249,   225,     4,   230,   225,   225,   225,
     816,   225,   258,   225,   225,   248,   249,     6,   975,   279,
     280,   281,   282,   283,   225,   258,   225,   225,   225,     4,
     931,   209,   210,   211,   212,   213,   225,   215,   216,   217,
     218,   219,   220,   221,   279,   280,   281,   282,   283,   227,
     279,   280,   281,   282,   283,   279,   280,   281,   282,   283,
     225,   225,   225,  1020,  1021,  1022,  1023,  1024,   225,   236,
     248,   249,   225,   225,   225,   242,  1182,   244,  1184,   246,
     258,   248,   225,   250,   225,   252,     6,   254,   225,   256,
     225,   258,  1198,   225,    14,   225,   225,   903,    18,    19,
      20,    21,    22,    23,   230,   225,    26,    27,    28,    29,
      30,    31,    32,   230,   225,   225,    36,    37,    38,    39,
      40,   230,    42,   225,   285,   228,   230,    47,    48,    49,
      50,    51,    52,    53,    54,   225,    56,    57,    58,   230,
      60,   228,    62,    63,    64,    65,    66,    67,   226,  1058,
     226,    71,    72,   226,    74,   226,    15,     3,     3,   225,
     230,    81,   279,   280,   281,   282,   283,   225,     5,   234,
     279,   280,   281,   282,   283,   279,   280,   281,   282,   283,
     228,   225,   102,   225,   222,   105,    99,    83,   279,   280,
     281,   282,   283,   243,   225,   245,   225,   247,   225,   249,
      83,   251,   225,   253,     4,   255,   225,   257,   128,   259,
      41,  1017,    41,    43,   134,    93,   136,   137,   190,   225,
     140,   141,    93,   143,   225,   145,   194,   225,    97,   103,
     106,   151,   138,   138,   154,   155,    84,    85,   142,   153,
     160,   161,   230,   163,   225,   165,   148,   232,   149,    84,
      85,     3,   225,     4,   236,   225,   286,   228,   225,  1168,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   228,  1188,
     200,   201,   202,   203,   225,   205,   225,  1125,   225,   229,
     225,   225,   225,   225,   225,   233,     4,   233,   225,   225,
       4,   225,   225,   225,   229,   226,     4,   225,     4,   229,
     225,  1149,    83,   225,   225,   235,   236,   225,   225,     4,
     142,     3,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     225,   189,    86,    87,   226,   225,   225,   228,   225,     4,
     225,     3,   225,   225,    83,   225,   285,   225,   225,   225,
     225,   209,   210,   211,   212,   213,   226,   215,   216,   217,
     218,   219,   220,   221,   209,   210,   211,   212,   213,   227,
     215,   216,   217,   218,   219,   220,   221,   227,   132,   133,
     228,   225,   227,   234,   227,   139,   228,   225,   225,   225,
     248,   249,   226,   226,     5,   226,   226,   226,   226,   226,
     258,   226,   226,   248,   249,   226,   226,   226,   226,   226,
     226,   226,   226,   258,   225,   228,   170,   225,     4,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   225,     4,   190,   190,    93,
     225,   233,    93,   225,    69,   225,   225,   100,   100,     4,
     100,   225,   228,   231,   113,   228,   228,   228,   119,   157,
     120,   124,   225,   112,     4,   225,   225,     4,   226,   225,
       4,   226,     4,   230,   225,   225,   225,   225,   225,   233,
     225,   225,   225,     4,   225,   225,   228,   225,     5,   226,
     244,   245,    44,   228,   226,   226,   226,   226,   226,   226,
     226,   226,     3,   225,   104,     4,   225,   228,   225,    70,
      95,   226,   228,   226,   226,   226,   226,   226,   107,   226,
     107,   226,   226,   226,   107,   107,   228,     4,   225,     7,
     226,   232,   225,    84,   228,     4,     4,   147,   225,   101,
     226,   225,     4,   225,   100,   226,   225,   225,     4,     4,
     225,     4,   226,     5,    44,   228,   103,   226,    24,     4,
     228,   190,   286,    94,   228,    95,   108,    25,   225,   190,
     228,    99,   228,   228,    94,   223,     4,  1187,     5,   226,
      73,   226,   228,   225,   225,   225,   253,   226,   225,   678,
     226,   225,    66,   228,  1008,   557,   422,   229,   220,   974,
    1003,  1069,   855,  1130,   184,  1045,  1135,   751,  1059,  1093,
     550,   836,   790,   581,    -1,   525,   490,   287,    -1,   428
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    16,    17,   289,   290,   291,   292,   226,   226,     0,
     290,   130,   302,   233,   233,   226,     6,    14,    18,    19,
      20,    21,    22,    23,    26,    27,    28,    29,    30,    31,
      32,    36,    37,    38,    39,    40,    42,    47,    48,    49,
      50,    51,    52,    53,    54,    56,    57,    58,    60,    62,
      63,    64,    65,    66,    67,    71,    72,    74,    81,   102,
     105,   128,   134,   136,   137,   140,   141,   143,   145,   151,
     154,   155,   160,   161,   163,   165,   200,   201,   202,   203,
     205,   229,   235,   236,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   293,   294,   295,   296,   297,   304,   306,   307,
     308,   309,   311,   312,   320,   323,   324,   326,   327,   334,
     336,   337,   339,   341,   343,   347,   348,   349,   352,   354,
     361,   368,   371,   377,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   393,   394,   395,   396,   400,   401,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   225,   131,   303,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   326,   226,   226,   226,   226,
     229,   236,   308,   310,   328,   226,   226,   226,   226,   235,
       4,   227,   242,   243,   278,   461,   471,   227,   304,   284,
     226,   338,     3,   225,   233,   244,   245,   447,   460,    10,
      11,    12,    13,     4,    99,   472,    84,    85,    88,    89,
     216,   227,   248,   249,   369,   449,   450,   103,   209,   210,
     211,   212,   213,   215,   217,   218,   219,   220,   221,   227,
     258,   449,   469,   470,     3,    84,    98,   227,   233,   256,
     257,   379,   458,   459,   379,     3,    84,   227,   233,   254,
     255,   373,   456,   457,    84,   233,   429,    75,    84,   441,
     449,     4,   449,   189,   470,   227,   250,   251,   452,   453,
     452,    59,   227,   479,   480,   233,   449,   449,    96,   449,
     227,   449,   479,   452,   452,    84,   233,   351,   430,   479,
     479,   479,   448,   461,   227,   350,   461,   449,   470,   470,
     449,   470,   470,     4,     8,     9,    84,   473,   449,   470,
     449,   351,   470,   452,   470,     5,    84,   362,   344,   470,
     452,   152,   307,   448,   284,   229,   449,   449,   449,   449,
     227,     8,   461,   229,   207,   230,   279,   280,   281,   282,
     283,   460,   461,   466,   467,   468,    26,    28,    33,    34,
      35,   143,   214,   227,   231,   449,   452,   460,   461,   475,
     476,   477,   342,   231,   305,   303,   229,   225,   285,   225,
     225,   225,   224,   451,   227,     4,   229,   225,   225,   356,
     452,   452,   449,   449,   227,   449,   227,   227,   227,   469,
     227,   470,   287,   225,     3,   229,   380,   378,     3,   229,
     372,   225,   225,   225,   225,   225,   225,     4,   229,   225,
     225,   227,     4,   225,   225,   225,   225,   225,   225,     4,
     225,   225,   225,     6,   325,   225,   225,   225,   225,     4,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   305,   345,   225,   225,   225,   230,   461,
     448,   225,   225,   225,   225,   321,   228,   228,   461,   461,
     461,   461,   461,   461,   461,   225,   228,   226,     4,   316,
     317,   318,   319,   450,   453,   460,   461,   226,   226,   226,
     306,   304,   461,    15,   447,     3,     3,   227,   402,     3,
     225,   230,   470,   225,   461,    89,    90,   370,     5,   234,
     225,   225,    83,   364,   470,   449,   449,     3,     7,   227,
     252,   253,   454,   455,   470,   222,   228,   470,    99,   225,
     461,   225,   305,   225,   461,   305,   449,   449,   470,    83,
     314,   457,    83,   225,   461,    76,    84,   442,    41,     4,
     225,    41,    43,    84,    86,    87,   443,   446,   190,   449,
      93,   225,    93,   479,    84,   227,   357,   225,   479,   479,
     479,   448,   225,   350,   473,   449,    79,    84,   478,    97,
      68,   197,   194,   103,   106,    84,   227,   358,     4,   103,
     138,   138,   441,   346,   227,   454,   454,   153,   340,   162,
     285,   335,   230,    84,   460,   148,   149,   150,   152,   309,
     463,   464,   465,   230,   230,   466,   470,   225,   225,   232,
     225,   232,   225,   232,   225,   232,   231,   458,   231,   456,
     231,   454,   232,   230,     3,   225,   451,   228,     4,   230,
     356,   449,   449,   286,   225,   228,   225,   225,   225,     3,
     229,   225,   225,   225,   233,   230,     4,     8,     9,    61,
      75,    76,    77,    78,    80,    82,   114,   115,   116,   118,
     121,   122,   123,   125,   126,   127,   129,   156,   158,   159,
     167,   168,   169,   170,   171,   172,   190,   204,   381,   449,
     471,   474,   233,   230,   374,   366,   225,   225,   225,     4,
     230,   225,   225,     4,   225,   229,   225,   226,   225,     4,
     225,     4,   225,   442,   225,   225,   392,   225,    83,   225,
     225,   226,   397,   225,   225,     4,   225,   225,   225,   225,
     142,   225,   225,   305,   285,   461,   329,   225,   225,   226,
     225,   228,     4,   453,   450,   461,   460,   313,   459,   314,
     315,   455,   225,     3,   228,   234,   225,   364,   470,   461,
     461,   470,   225,   461,   470,   470,   227,   228,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   228,   225,   225,   441,   457,
       5,   225,   430,     4,   225,   190,     4,    84,    91,   445,
     456,   190,    93,   225,    84,   227,   359,   233,    93,   448,
     225,   460,    69,   449,   225,   100,     4,   107,   109,   110,
     111,   225,    84,   227,   360,   100,     4,    84,   363,   364,
     305,   100,   454,   231,   442,    84,   460,   238,   239,   462,
     463,   322,   225,   232,   232,   225,   232,   442,   225,   461,
     228,   225,   228,   225,   228,   442,   230,   228,   228,   461,
     448,   458,   113,   112,   448,   119,   120,   124,   448,   448,
     448,   460,   157,   448,   448,   448,   456,    86,    87,   132,
     133,   139,   170,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   375,
     444,   446,   460,    80,   117,   118,   126,   135,   146,   172,
     367,   225,   300,     4,   225,   431,   225,     4,   226,   230,
     225,   226,     4,   369,   225,   225,     4,    84,   365,   225,
     225,    94,   101,   126,   146,   190,   191,   192,   193,   195,
     196,   197,   198,   199,   398,   225,   225,   225,   225,     4,
       4,   431,   225,   225,   225,   164,   166,   330,   331,   305,
     459,   455,   228,     5,    44,   461,   228,   280,   226,   460,
     226,   226,   226,   226,   226,   226,   226,   226,   226,     3,
     225,    44,    45,    46,   432,   433,   104,   434,     4,   225,
     456,    92,   190,   456,   228,   225,   360,   448,   228,    70,
      95,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   107,   107,   107,   107,   228,   225,   434,     4,   365,
       7,   473,   226,   232,   330,   225,   228,   225,   228,   461,
      84,   448,   448,   448,   448,   448,   189,   233,   376,   458,
       4,   147,     4,   225,   226,   101,   436,   225,     4,   225,
     226,   370,   431,   225,   225,   100,   437,     4,     4,    95,
      97,   147,   399,   456,   458,   402,   402,   449,   429,     4,
     225,     4,   436,   366,   226,   333,     5,    44,   225,   298,
     286,   228,   432,   103,   435,   226,    24,   439,     4,   228,
     190,   456,   228,   434,    95,    94,   108,   228,   439,   332,
     304,   225,   228,   223,   228,   376,   437,   226,    25,   440,
     228,   226,   436,   225,   225,   440,   304,   442,   225,    99,
     226,   190,   438,   456,   439,    94,   442,   438,   225,   449,
       4,   226,   412,   440,   353,     5,    84,   301,   225,   299,
     456,   305,   413,   438,   305,   300,   449,   228,   355,   305
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
		      long rflags = (yyvsp[(8) - (13)].i);
		      long flmt = (long)(yyvsp[(9) - (13)].i);
		      long flt = (long)(yyvsp[(10) - (13)].i);
		      long tlset = (long)(yyvsp[(11) - (13)].i);
		      long decotyp = (long)(yyvsp[(12) - (13)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      //if (flmt == -1) flmt = ROOM;
		      //if (flt == -1) flt = 0;

		      add_opvars(splev, "iiiiiiiiiiio",
				 VA_PASS12(decotyp, tlset, flt, flmt, rflags, ERR, ERR,
					  (yyvsp[(5) - (13)].crd).x, (yyvsp[(5) - (13)].crd).y, (yyvsp[(7) - (13)].sze).width, (yyvsp[(7) - (13)].sze).height,
					  SPO_SUBROOM));
		      break_stmt_start();
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1336 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1343 "lev_comp.y"
    {
		      long rflags = (yyvsp[(10) - (15)].i);
		      long flmt = (long)(yyvsp[(11) - (15)].i);
		      long flt = (long)(yyvsp[(12) - (15)].i);
		      long tlset = (long)(yyvsp[(13) - (15)].i);
		      long decotyp = (long)(yyvsp[(14) - (15)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      //if (flmt == -1) flmt = ROOM;
		      //if (flt == -1) flt = 0;

		      add_opvars(splev, "iiiiiiiiiiio",
				 VA_PASS12(decotyp, tlset, flt, flmt, rflags,
					  (yyvsp[(7) - (15)].crd).x, (yyvsp[(7) - (15)].crd).y, (yyvsp[(5) - (15)].crd).x, (yyvsp[(5) - (15)].crd).y,
					  (yyvsp[(9) - (15)].sze).width, (yyvsp[(9) - (15)].sze).height, SPO_ROOM));
		      break_stmt_start();
		  ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1361 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1368 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1372 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1378 "lev_comp.y"
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
#line 1388 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1394 "lev_comp.y"
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
#line 1403 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1409 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1414 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1420 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1425 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1431 "lev_comp.y"
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
#line 1442 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1456 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1460 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1470 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1477 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1485 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1490 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1495 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1500 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1505 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1510 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1515 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1523 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1530 "lev_comp.y"
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
#line 1538 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1554 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1558 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1564 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1572 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1578 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1586 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1594 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1599 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1605 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1611 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1617 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1623 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1628 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(0, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1633 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1638 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1643 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1648 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1653 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1658 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1663 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1668 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1673 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1678 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1683 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1689 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1694 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_PROTECTOR));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1699 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_MAXHP));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1704 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_LEVEL_ADJUSTMENT));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1709 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_KEEP_ORIGINAL_INVENTORY));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1716 "lev_comp.y"
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
#line 1725 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1729 "lev_comp.y"
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
#line 1742 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1750 "lev_comp.y"
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
#line 1760 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1768 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1777 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1784 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1792 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1798 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1803 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1808 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1813 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1818 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1823 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1828 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1833 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1838 "lev_comp.y"
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
#line 1852 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1858 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1863 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1868 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1873 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1878 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1883 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1888 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1893 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1898 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1903 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1908 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1913 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x00200000;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1918 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x00400000;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1923 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x00800000;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1928 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x01000000;
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1933 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_MYTHIC_TYPE));
		      (yyval.i) = 0x02000000;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1939 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x04000000;
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1944 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x08000000;
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1949 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x10000000;
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 1954 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MATERIAL));
		      (yyval.i) = 0x20000000;
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 1961 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 1967 "lev_comp.y"
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
#line 1996 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2001 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2009 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2015 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2019 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2027 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2034 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2041 "lev_comp.y"
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
#line 2051 "lev_comp.y"
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
#line 2061 "lev_comp.y"
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
#line 2077 "lev_comp.y"
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
#line 2087 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2091 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2097 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2103 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2109 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2113 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2119 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2125 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2132 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2140 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2146 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2151 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2156 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2161 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2166 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2172 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2178 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2183 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2188 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2193 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2198 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2203 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2213 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2219 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2225 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2230 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2237 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2244 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2248 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2254 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2260 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2266 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2271 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2278 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2284 "lev_comp.y"
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
#line 2294 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2300 "lev_comp.y"
    {
		      long irr;
		      long rt = (yyvsp[(7) - (13)].i);
		      long rflags = (yyvsp[(8) - (13)].i);
		      long flmt = (long)(yyvsp[(9) - (13)].i);
		      long flt = (long)(yyvsp[(10) - (13)].i);
		      long tlset = (long)(yyvsp[(11) - (13)].i);
		      long decotyp = (long)(yyvsp[(12) - (13)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      //if (flmt == -1) flmt = 0;
		      //if (flt == -1) flt = 0;

		      if (!(rflags & 1)) rt += MAXRTYPE+1;
		      irr = ((rflags & 2) != 0);
		      add_opvars(splev, "iiiiiiio",
				 VA_PASS8((long)(yyvsp[(5) - (13)].i), rt, rflags, flmt, flt, tlset, decotyp, SPO_REGION));
		      (yyval.i) = (irr || (rflags & 1) || rt != OROOM);
		      break_stmt_start();
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2321 "lev_comp.y"
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
#line 2333 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2337 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2343 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2348 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2353 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (long)(yyvsp[(9) - (9)].i), (long)(yyvsp[(7) - (9)].i), (long)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2358 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((long)(yyvsp[(9) - (13)].i), (long)(yyvsp[(7) - (13)].i), (long)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2364 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2368 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2374 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, (int)(yyvsp[(11) - (11)].i), (int)(yyvsp[(9) - (11)].i), (int)(yyvsp[(7) - (11)].i), (int)(yyvsp[(5) - (11)].i), SPO_DECORATION));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2378 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6((int)(yyvsp[(13) - (13)].i), (int)(yyvsp[(11) - (13)].i), (int)(yyvsp[(9) - (13)].i), (int)(yyvsp[(7) - (13)].i), (int)(yyvsp[(5) - (13)].i), SPO_DECORATION));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2382 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2386 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2390 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2394 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, 1, (int)(yyvsp[(7) - (7)].i), 0, (int)(yyvsp[(5) - (7)].i), SPO_DECORATION));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2400 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2406 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2410 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2416 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_CARPET));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2422 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2426 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4(0, (int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2432 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2436 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2442 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2446 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2451 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2458 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2462 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2469 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2473 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2478 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2485 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2489 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2494 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2502 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2508 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2515 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2519 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2526 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2537 "lev_comp.y"
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

  case 399:

/* Line 1455 of yacc.c  */
#line 2550 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2554 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2560 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2564 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2571 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2575 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2579 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2585 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2589 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2595 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2601 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2605 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2611 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2617 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2622 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2628 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2632 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2638 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2642 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2658 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2666 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2676 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2685 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2690 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2697 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2708 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2714 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2718 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2722 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2729 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2739 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2746 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2750 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2756 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2760 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2768 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2772 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2779 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2789 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2802 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2806 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2813 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2823 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2832 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2843 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2847 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2854 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2864 "lev_comp.y"
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

  case 457:

/* Line 1455 of yacc.c  */
#line 2875 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2884 "lev_comp.y"
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

  case 459:

/* Line 1455 of yacc.c  */
#line 2894 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2900 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2904 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2911 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2921 "lev_comp.y"
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

  case 464:

/* Line 1455 of yacc.c  */
#line 2932 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2941 "lev_comp.y"
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

  case 466:

/* Line 1455 of yacc.c  */
#line 2951 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2955 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2961 "lev_comp.y"
    { ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 2963 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2969 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 2973 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 2977 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 2981 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 2989 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 2998 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3002 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3006 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3010 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3014 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3017 "lev_comp.y"
    { ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3021 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3028 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3037 "lev_comp.y"
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

  case 488:

/* Line 1455 of yacc.c  */
#line 3078 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3082 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3089 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3096 "lev_comp.y"
    {
			      size_t len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3106 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3110 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3118 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3122 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3126 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3130 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3134 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3139 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3143 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3147 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3152 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3157 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3162 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3166 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3171 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3176 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3180 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3184 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3189 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3193 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3200 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3206 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3210 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 3216 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 521:

/* Line 1455 of yacc.c  */
#line 3232 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 522:

/* Line 1455 of yacc.c  */
#line 3236 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 523:

/* Line 1455 of yacc.c  */
#line 3240 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 524:

/* Line 1455 of yacc.c  */
#line 3244 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 533:

/* Line 1455 of yacc.c  */
#line 3266 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 534:

/* Line 1455 of yacc.c  */
#line 3270 "lev_comp.y"
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

  case 535:

/* Line 1455 of yacc.c  */
#line 3296 "lev_comp.y"
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
#line 7505 "lev_comp.tab.c"
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
#line 3324 "lev_comp.y"


/*lev_comp.y*/

