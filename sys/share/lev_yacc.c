
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

/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-03-17 */

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
#define YYLAST   1362

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  288
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  193
/* YYNRULES -- Number of rules.  */
#define YYNRULES  537
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1202

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
      33,    39,    45,    49,    66,    70,    76,    80,    86,    90,
      91,    94,    95,    98,    99,   102,   104,   106,   107,   111,
     115,   117,   118,   121,   125,   127,   129,   131,   133,   135,
     137,   139,   141,   143,   145,   147,   149,   151,   153,   155,
     157,   159,   161,   163,   165,   167,   169,   171,   173,   175,
     177,   179,   181,   183,   185,   187,   189,   191,   193,   195,
     197,   199,   201,   203,   205,   207,   209,   211,   213,   215,
     217,   219,   221,   223,   225,   227,   229,   231,   233,   235,
     237,   239,   241,   243,   245,   247,   249,   251,   253,   255,
     257,   259,   261,   263,   265,   267,   269,   271,   273,   275,
     277,   279,   281,   283,   285,   287,   289,   291,   293,   295,
     297,   299,   301,   305,   309,   315,   319,   325,   331,   337,
     341,   345,   351,   357,   363,   371,   379,   387,   393,   395,
     399,   401,   405,   407,   411,   413,   417,   419,   423,   425,
     429,   431,   435,   436,   437,   446,   451,   453,   454,   456,
     458,   464,   468,   469,   470,   480,   481,   484,   485,   491,
     492,   497,   499,   502,   504,   511,   512,   516,   517,   524,
     525,   530,   531,   536,   538,   539,   544,   548,   554,   556,
     560,   564,   570,   576,   584,   589,   590,   606,   607,   625,
     626,   629,   635,   637,   643,   645,   651,   653,   659,   661,
     672,   679,   681,   683,   685,   687,   689,   693,   695,   697,
     698,   702,   706,   710,   714,   716,   718,   720,   722,   724,
     732,   738,   740,   742,   744,   746,   750,   751,   757,   762,
     763,   767,   769,   771,   773,   775,   778,   780,   782,   784,
     786,   788,   792,   794,   798,   802,   806,   808,   810,   814,
     816,   818,   820,   822,   826,   828,   830,   832,   836,   840,
     841,   847,   850,   851,   855,   857,   861,   863,   867,   871,
     873,   875,   879,   881,   883,   885,   889,   891,   893,   895,
     899,   903,   907,   911,   915,   919,   923,   927,   929,   931,
     933,   935,   939,   943,   947,   951,   957,   965,   971,   980,
     982,   986,   990,   996,  1002,  1010,  1018,  1025,  1031,  1032,
    1035,  1041,  1045,  1057,  1071,  1078,  1079,  1083,  1085,  1089,
    1093,  1097,  1101,  1103,  1105,  1109,  1113,  1117,  1121,  1125,
    1129,  1131,  1133,  1135,  1137,  1141,  1145,  1147,  1153,  1163,
    1169,  1177,  1181,  1185,  1191,  1201,  1207,  1213,  1219,  1220,
    1236,  1237,  1239,  1247,  1259,  1269,  1283,  1287,  1295,  1307,
    1321,  1331,  1341,  1351,  1359,  1367,  1373,  1381,  1387,  1397,
    1405,  1411,  1421,  1427,  1433,  1437,  1445,  1449,  1457,  1465,
    1469,  1475,  1481,  1485,  1491,  1499,  1509,  1511,  1513,  1515,
    1517,  1519,  1520,  1523,  1525,  1529,  1531,  1533,  1535,  1536,
    1540,  1542,  1543,  1547,  1549,  1550,  1554,  1555,  1559,  1560,
    1564,  1566,  1568,  1570,  1572,  1574,  1576,  1578,  1580,  1582,
    1586,  1588,  1590,  1595,  1597,  1599,  1604,  1606,  1608,  1613,
    1615,  1620,  1626,  1628,  1632,  1634,  1638,  1640,  1642,  1647,
    1657,  1659,  1661,  1666,  1668,  1674,  1676,  1678,  1683,  1685,
    1687,  1693,  1695,  1697,  1699,  1704,  1706,  1708,  1714,  1716,
    1718,  1720,  1724,  1726,  1728,  1732,  1734,  1739,  1743,  1747,
    1751,  1755,  1759,  1763,  1765,  1767,  1771,  1773,  1777,  1778,
    1780,  1782,  1784,  1786,  1790,  1791,  1793,  1795,  1798,  1801,
    1806,  1813,  1818,  1825,  1832,  1839,  1846,  1849,  1856,  1865,
    1874,  1885,  1900,  1903,  1905,  1909,  1911,  1915,  1917,  1919,
    1921,  1923,  1925,  1927,  1929,  1931,  1933,  1935,  1937,  1939,
    1941,  1943,  1945,  1947,  1949,  1951,  1953,  1964
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     289,     0,    -1,    -1,   290,    -1,   291,    -1,   291,   290,
      -1,   292,   302,   304,    -1,    17,   226,   233,    -1,    16,
     226,   233,   225,     3,    -1,    16,   226,   233,   225,     3,
     225,   442,    -1,    18,   226,    11,   225,   402,    -1,    18,
     226,    10,   225,     3,    -1,    18,   226,    13,    -1,    18,
     226,    12,   225,     3,   225,     3,   225,     5,   225,     5,
     225,   442,   225,   301,   300,    -1,    19,   226,   472,    -1,
     151,   226,   152,   225,   153,    -1,   151,   226,   152,    -1,
      32,   226,     4,   225,   314,    -1,    22,   226,   103,    -1,
      -1,   225,   223,    -1,    -1,   225,   449,    -1,    -1,   225,
       3,    -1,     5,    -1,    84,    -1,    -1,   130,   226,   303,
      -1,   131,   225,   303,    -1,   131,    -1,    -1,   306,   304,
      -1,   231,   304,   232,    -1,   347,    -1,   293,    -1,   294,
      -1,   410,    -1,   297,    -1,   295,    -1,   418,    -1,   419,
      -1,   420,    -1,   296,    -1,   417,    -1,   416,    -1,   414,
      -1,   415,    -1,   421,    -1,   422,    -1,   423,    -1,   424,
      -1,   425,    -1,   391,    -1,   349,    -1,   312,    -1,   311,
      -1,   405,    -1,   361,    -1,   383,    -1,   427,    -1,   428,
      -1,   393,    -1,   394,    -1,   395,    -1,   426,    -1,   327,
      -1,   337,    -1,   339,    -1,   343,    -1,   341,    -1,   324,
      -1,   334,    -1,   320,    -1,   323,    -1,   386,    -1,   396,
      -1,   368,    -1,   384,    -1,   371,    -1,   377,    -1,   406,
      -1,   401,    -1,   389,    -1,   348,    -1,   407,    -1,   408,
      -1,   409,    -1,   411,    -1,   354,    -1,   352,    -1,   400,
      -1,   404,    -1,   403,    -1,   387,    -1,   388,    -1,   390,
      -1,   382,    -1,   385,    -1,   243,    -1,   245,    -1,   247,
      -1,   249,    -1,   251,    -1,   253,    -1,   255,    -1,   257,
      -1,   259,    -1,   242,    -1,   244,    -1,   246,    -1,   248,
      -1,   250,    -1,   252,    -1,   254,    -1,   256,    -1,   258,
      -1,   307,    -1,   308,    -1,   236,    -1,   236,    -1,   308,
      -1,   155,   226,   307,    -1,   309,   284,   461,    -1,   309,
     284,   214,   226,   470,    -1,   309,   284,   460,    -1,   309,
     284,   477,   226,   454,    -1,   309,   284,   476,   226,   456,
      -1,   309,   284,   475,   226,   458,    -1,   309,   284,   449,
      -1,   309,   284,   452,    -1,   309,   284,   231,   318,   232,
      -1,   309,   284,   231,   317,   232,    -1,   309,   284,   231,
     316,   232,    -1,   309,   284,   477,   226,   231,   315,   232,
      -1,   309,   284,   476,   226,   231,   314,   232,    -1,   309,
     284,   475,   226,   231,   313,   232,    -1,   309,   284,   231,
     319,   232,    -1,   459,    -1,   313,   225,   459,    -1,   457,
      -1,   314,   225,   457,    -1,   455,    -1,   315,   225,   455,
      -1,   453,    -1,   316,   225,   453,    -1,   450,    -1,   317,
     225,   450,    -1,   461,    -1,   318,   225,   461,    -1,   460,
      -1,   319,   225,   460,    -1,    -1,    -1,   205,   235,   227,
     321,   465,   228,   322,   305,    -1,   235,   227,   468,   228,
      -1,   154,    -1,    -1,     6,    -1,     6,    -1,   229,   461,
     207,   461,   230,    -1,   229,   461,   230,    -1,    -1,    -1,
     163,   328,   229,   448,   230,   329,   231,   330,   232,    -1,
      -1,   331,   330,    -1,    -1,   164,   473,   226,   332,   304,
      -1,    -1,   166,   226,   333,   304,    -1,   165,    -1,   285,
     285,    -1,   162,    -1,   161,   310,   284,   461,   335,   461,
      -1,    -1,   336,   338,   305,    -1,    -1,   160,   229,   448,
     230,   340,   305,    -1,    -1,   326,   226,   342,   306,    -1,
      -1,   141,   326,   344,   345,    -1,   305,    -1,    -1,   305,
     346,   142,   305,    -1,    14,   226,   460,    -1,    14,   226,
     460,   225,    15,    -1,    74,    -1,    74,   226,   473,    -1,
      74,   226,    84,    -1,    62,   226,   350,   225,   350,    -1,
      62,   226,   350,   225,   473,    -1,   227,     4,   225,    83,
     225,   365,   228,    -1,   430,   325,   225,   442,    -1,    -1,
     128,   226,   351,   225,   358,   225,   360,   431,   434,   436,
     439,   440,   438,   353,   305,    -1,    -1,    54,   226,   351,
     225,   357,   225,   359,   225,   360,   431,   434,   436,   439,
     440,   438,   355,   305,    -1,    -1,   225,     5,    -1,   227,
       4,   225,     4,   228,    -1,    84,    -1,   227,     4,   225,
       4,   228,    -1,    84,    -1,   227,   369,   225,   370,   228,
      -1,    84,    -1,   227,     4,   225,     4,   228,    -1,    84,
      -1,   140,   226,   362,   225,   441,   225,   363,   225,   365,
     366,    -1,    30,   226,   441,   225,   470,   366,    -1,     5,
      -1,    84,    -1,   364,    -1,    84,    -1,    83,    -1,    83,
     286,   364,    -1,     4,    -1,    84,    -1,    -1,   366,   225,
     367,    -1,   146,   226,   147,    -1,    80,   226,   458,    -1,
     126,   226,     4,    -1,   172,    -1,   117,    -1,   118,    -1,
     135,    -1,    21,    -1,    20,   226,   369,   225,   370,   356,
     234,    -1,    20,   226,   449,   356,   234,    -1,    88,    -1,
      89,    -1,    90,    -1,    89,    -1,    28,   226,   373,    -1,
      -1,    28,   226,   373,   372,   305,    -1,   456,   225,   449,
     374,    -1,    -1,   374,   225,   375,    -1,   460,    -1,   132,
      -1,   133,    -1,   444,    -1,   139,   460,    -1,   173,    -1,
     174,    -1,   170,    -1,   177,    -1,   178,    -1,   178,   226,
     448,    -1,   179,    -1,   180,   226,   448,    -1,   181,   226,
     448,    -1,   185,   226,   448,    -1,   186,    -1,   187,    -1,
     188,   226,   376,    -1,   188,    -1,   175,    -1,   176,    -1,
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
       0,   287,   287,   288,   291,   292,   295,   318,   323,   343,
     366,   379,   391,   397,   426,   432,   436,   442,   448,   455,
     458,   465,   469,   476,   479,   486,   487,   491,   494,   501,
     505,   512,   515,   521,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,   581,   582,
     583,   584,   585,   586,   587,   588,   589,   590,   591,   594,
     595,   596,   597,   598,   599,   600,   601,   602,   605,   606,
     607,   608,   609,   610,   611,   612,   613,   616,   617,   618,
     621,   622,   625,   641,   647,   653,   659,   665,   671,   677,
     683,   689,   699,   709,   719,   729,   739,   749,   761,   766,
     773,   778,   785,   790,   797,   801,   807,   812,   819,   823,
     829,   833,   840,   862,   839,   876,   931,   938,   941,   947,
     954,   958,   967,   971,   966,  1034,  1035,  1039,  1038,  1052,
    1051,  1066,  1076,  1077,  1080,  1118,  1117,  1152,  1151,  1182,
    1181,  1214,  1213,  1239,  1250,  1249,  1277,  1281,  1286,  1291,
    1296,  1303,  1310,  1319,  1327,  1339,  1338,  1364,  1363,  1389,
    1392,  1398,  1408,  1414,  1423,  1429,  1434,  1440,  1445,  1451,
    1462,  1468,  1469,  1472,  1473,  1476,  1480,  1486,  1487,  1491,
    1497,  1505,  1510,  1515,  1520,  1525,  1530,  1535,  1543,  1550,
    1558,  1566,  1567,  1570,  1571,  1574,  1579,  1578,  1592,  1599,
    1606,  1614,  1619,  1625,  1631,  1637,  1643,  1648,  1653,  1658,
    1663,  1668,  1673,  1678,  1683,  1688,  1693,  1698,  1703,  1709,
    1715,  1720,  1725,  1730,  1735,  1742,  1751,  1755,  1768,  1777,
    1776,  1794,  1804,  1810,  1818,  1824,  1829,  1834,  1839,  1844,
    1849,  1854,  1859,  1864,  1878,  1884,  1889,  1894,  1899,  1904,
    1909,  1914,  1919,  1924,  1929,  1934,  1939,  1944,  1949,  1954,
    1959,  1965,  1970,  1975,  1980,  1987,  1993,  2022,  2027,  2035,
    2041,  2045,  2053,  2060,  2067,  2077,  2087,  2103,  2114,  2117,
    2123,  2129,  2135,  2139,  2145,  2152,  2158,  2166,  2172,  2177,
    2182,  2187,  2192,  2198,  2204,  2209,  2214,  2219,  2224,  2229,
    2236,  2236,  2236,  2236,  2239,  2245,  2251,  2256,  2263,  2270,
    2274,  2280,  2286,  2292,  2297,  2304,  2310,  2320,  2327,  2326,
    2360,  2363,  2369,  2374,  2379,  2384,  2390,  2394,  2400,  2404,
    2408,  2412,  2416,  2420,  2426,  2432,  2436,  2442,  2448,  2452,
    2458,  2462,  2468,  2472,  2477,  2484,  2488,  2495,  2499,  2504,
    2511,  2515,  2520,  2528,  2534,  2541,  2545,  2552,  2560,  2563,
    2573,  2577,  2580,  2586,  2590,  2597,  2601,  2605,  2612,  2615,
    2621,  2628,  2631,  2637,  2644,  2648,  2655,  2658,  2665,  2668,
    2674,  2675,  2678,  2679,  2682,  2683,  2684,  2690,  2691,  2692,
    2698,  2699,  2702,  2711,  2716,  2723,  2734,  2740,  2744,  2748,
    2755,  2765,  2772,  2776,  2782,  2786,  2794,  2798,  2805,  2815,
    2828,  2832,  2839,  2849,  2858,  2869,  2873,  2880,  2890,  2901,
    2910,  2920,  2926,  2930,  2937,  2947,  2958,  2967,  2977,  2981,
    2988,  2989,  2995,  2999,  3003,  3007,  3015,  3024,  3028,  3032,
    3036,  3040,  3044,  3047,  3054,  3063,  3096,  3097,  3100,  3101,
    3104,  3108,  3115,  3122,  3133,  3136,  3144,  3148,  3152,  3156,
    3160,  3165,  3169,  3173,  3178,  3183,  3188,  3192,  3197,  3202,
    3206,  3210,  3215,  3219,  3226,  3232,  3236,  3242,  3249,  3250,
    3253,  3254,  3255,  3258,  3262,  3266,  3270,  3276,  3277,  3280,
    3281,  3284,  3285,  3288,  3289,  3292,  3296,  3322
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
       0,   288,   289,   289,   290,   290,   291,   292,   292,   292,
     293,   293,   293,   293,   294,   295,   295,   296,   297,   298,
     298,   299,   299,   300,   300,   301,   301,   302,   302,   303,
     303,   304,   304,   305,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   309,   309,   309,
     310,   310,   311,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   313,   313,
     314,   314,   315,   315,   316,   316,   317,   317,   318,   318,
     319,   319,   321,   322,   320,   323,   324,   325,   325,   326,
     326,   326,   328,   329,   327,   330,   330,   332,   331,   333,
     331,   334,   335,   335,   336,   338,   337,   340,   339,   342,
     341,   344,   343,   345,   346,   345,   347,   347,   348,   348,
     348,   349,   349,   350,   351,   353,   352,   355,   354,   356,
     356,   357,   357,   358,   358,   359,   359,   360,   360,   361,
     361,   362,   362,   363,   363,   364,   364,   365,   365,   366,
     366,   367,   367,   367,   367,   367,   367,   367,   368,   368,
     368,   369,   369,   370,   370,   371,   372,   371,   373,   374,
     374,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   376,   376,   376,   377,   378,
     377,   379,   380,   380,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   382,   383,   384,   384,   385,
     385,   385,   386,   387,   388,   389,   390,   391,   392,   392,
     393,   394,   395,   395,   396,   397,   397,   398,   398,   398,
     398,   398,   398,   398,   398,   398,   398,   398,   398,   398,
     399,   399,   399,   399,   400,   401,   402,   402,   403,   404,
     404,   405,   406,   407,   407,   408,   409,   410,   412,   411,
     413,   413,   414,   414,   414,   414,   415,   415,   416,   416,
     416,   416,   416,   416,   417,   418,   418,   419,   420,   420,
     421,   421,   422,   422,   422,   423,   423,   424,   424,   424,
     425,   425,   425,   426,   427,   428,   428,   429,   429,   430,
     430,   431,   431,   432,   432,   433,   433,   433,   434,   434,
     435,   436,   436,   437,   438,   438,   439,   439,   440,   440,
     441,   441,   442,   442,   443,   443,   443,   444,   444,   444,
     445,   445,   446,   447,   447,   447,   448,   449,   449,   449,
     449,   450,   450,   450,   451,   451,   452,   452,   452,   453,
     454,   454,   454,   455,   455,   456,   456,   456,   457,   457,
     457,   457,   458,   458,   458,   459,   459,   459,   459,   459,
     460,   460,   461,   461,   461,   461,   461,   461,   461,   461,
     461,   461,   461,   462,   462,   463,   464,   464,   465,   465,
     466,   466,   467,   467,   468,   468,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   470,   470,   471,   472,   472,
     473,   473,   473,   474,   474,   474,   474,   475,   475,   476,
     476,   477,   477,   478,   478,   479,   479,   480
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     3,     3,     5,     7,
       5,     5,     3,    16,     3,     5,     3,     5,     3,     0,
       2,     0,     2,     0,     2,     1,     1,     0,     3,     3,
       1,     0,     2,     3,     1,     1,     1,     1,     1,     1,
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
       4,     0,     4,     1,     0,     4,     3,     5,     1,     3,
       3,     5,     5,     7,     4,     0,    15,     0,    17,     0,
       2,     5,     1,     5,     1,     5,     1,     5,     1,    10,
       6,     1,     1,     1,     1,     1,     3,     1,     1,     0,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     7,
       5,     1,     1,     1,     1,     3,     0,     5,     4,     0,
       3,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     3,     1,     3,     3,     3,     1,     1,     3,     1,
       1,     1,     1,     3,     1,     1,     1,     3,     3,     0,
       5,     2,     0,     3,     1,     3,     1,     3,     3,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     3,     3,     3,     3,     5,     7,     5,     8,     1,
       3,     3,     5,     5,     7,     7,     6,     5,     0,     2,
       5,     3,    11,    13,     6,     0,     3,     1,     3,     3,
       3,     3,     1,     1,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     3,     3,     1,     5,     9,     5,
       7,     3,     3,     5,     9,     5,     5,     5,     0,    15,
       0,     1,     7,    11,     9,    13,     3,     7,    11,    13,
       9,     9,     9,     7,     7,     5,     7,     5,     9,     7,
       5,     9,     5,     5,     3,     7,     3,     7,     7,     3,
       5,     5,     3,     5,     7,     9,     1,     1,     1,     1,
       1,     0,     2,     1,     3,     1,     1,     1,     0,     3,
       1,     0,     3,     1,     0,     3,     0,     3,     0,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     1,     4,     1,     1,     4,     1,     1,     4,     1,
       4,     5,     1,     3,     1,     3,     1,     1,     4,     9,
       1,     1,     4,     1,     5,     1,     1,     4,     1,     1,
       5,     1,     1,     1,     4,     1,     1,     5,     1,     1,
       1,     3,     1,     1,     3,     1,     4,     3,     3,     3,
       3,     3,     3,     1,     1,     3,     1,     3,     0,     1,
       1,     1,     1,     3,     0,     1,     1,     2,     2,     4,
       6,     4,     6,     6,     6,     6,     2,     6,     8,     8,
      10,    14,     2,     1,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,    10,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    27,     0,     0,     1,
       5,     0,    31,     0,     7,     0,   159,     0,     0,     0,
       0,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   309,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   396,     0,
       0,     0,     0,     0,     0,     0,     0,   188,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     156,     0,     0,     0,   162,   171,     0,     0,     0,     0,
       0,     0,     0,   119,   108,    99,   109,   100,   110,   101,
     111,   102,   112,   103,   113,   104,   114,   105,   115,   106,
     116,   107,    35,    36,    39,    43,    38,     6,    31,   117,
     118,     0,    56,    55,    73,    74,    71,     0,    66,    72,
     175,    67,    68,    70,    69,    34,    84,    54,    90,    89,
      58,    77,    79,    80,    97,    59,    78,    98,    75,    94,
      95,    83,    96,    53,    62,    63,    64,    76,    91,    82,
      93,    92,    57,    81,    85,    86,    87,    37,    88,    46,
      47,    45,    44,    40,    41,    42,    48,    49,    50,    51,
      52,    65,    60,    61,     0,    30,    28,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   181,     0,     0,     0,     0,
       0,   120,   121,     0,     0,     0,     0,     0,     0,     0,
     472,     0,   475,     0,   517,     0,   473,   494,    32,     0,
     179,     0,     8,     0,   433,   434,     0,   470,   186,     0,
       0,     0,    12,   519,   518,    14,   442,     0,   231,   232,
       0,     0,   439,     0,     0,   199,   437,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     513,   496,   515,     0,   466,   468,   469,     0,   465,   463,
       0,   268,   272,   462,   269,   459,   461,     0,   458,   456,
       0,   235,     0,   455,   398,   397,     0,   420,   421,     0,
       0,     0,     0,   311,   310,     0,   447,     0,     0,   446,
       0,     0,     0,     0,   535,     0,     0,   366,     0,     0,
       0,     0,     0,   351,   352,   400,   399,     0,   157,     0,
       0,     0,     0,   436,     0,     0,     0,     0,     0,   321,
       0,   345,   344,   522,   520,   521,   190,   189,     0,     0,
       0,     0,     0,     0,     0,   211,   212,     0,     0,     0,
       0,    16,   122,     0,     0,     0,   384,   386,   389,   392,
     152,     0,     0,     0,     0,   161,     0,     0,     0,     0,
       0,   491,   490,   492,   495,     0,   528,   530,   527,   529,
     531,   532,     0,     0,     0,   129,   130,   125,   123,     0,
       0,     0,     0,    31,   176,     0,    29,     0,     0,     0,
       0,     0,     0,   444,     0,     0,     0,     0,     0,     0,
       0,   497,   498,     0,     0,     0,   506,     0,     0,     0,
     512,     0,     0,     0,     0,     0,     0,   271,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   158,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   183,   182,     0,     0,     0,   177,
       0,     0,     0,     0,     0,     0,   488,   474,   482,     0,
       0,   477,   478,   479,   480,   481,     0,   155,     0,   472,
       0,     0,     0,     0,   146,   144,   150,   148,     0,     0,
       0,   180,     0,   422,   423,     9,     0,   187,   471,    11,
     346,     0,    10,     0,     0,   443,     0,     0,     0,   234,
     233,   199,   200,   230,     0,     0,   215,     0,     0,     0,
       0,   453,     0,     0,   451,     0,     0,   450,     0,     0,
     514,   516,   357,     0,     0,     0,   270,     0,     0,   237,
     239,   305,   219,     0,    17,   140,   307,     0,     0,     0,
     355,     0,     0,   356,   353,   426,     0,   424,     0,   425,
       0,   380,   312,     0,   313,     0,   202,     0,     0,     0,
       0,   318,   317,     0,     0,   191,   192,   393,   533,   534,
       0,   320,     0,     0,   325,     0,     0,   204,     0,     0,
     375,     0,   377,     0,     0,     0,     0,   349,     0,    15,
       0,   173,     0,     0,   163,   383,   382,     0,     0,   390,
     391,     0,   486,   489,     0,   476,   160,   493,   124,     0,
       0,   133,     0,   132,     0,   131,     0,   137,     0,   128,
       0,   127,     0,   126,    33,   435,     0,     0,   445,   438,
       0,   440,     0,   499,     0,     0,     0,   501,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   464,   525,   523,
     524,     0,   283,   280,   274,   300,     0,   299,     0,     0,
       0,   298,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   279,     0,   284,     0,   286,   287,   297,     0,   282,
     273,   288,   526,   276,     0,   457,   238,   210,     0,     0,
       0,     0,   448,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   194,     0,     0,   316,     0,
       0,     0,     0,     0,   324,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   178,   172,   174,     0,     0,
       0,     0,     0,   153,     0,   145,   147,   149,   151,     0,
     138,     0,     0,   142,     0,     0,   441,   229,     0,   216,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   467,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   460,     0,     0,
     306,   141,    23,     0,   401,     0,     0,     0,     0,   431,
     430,   362,   367,     0,   314,     0,   206,     0,     0,   315,
     319,     0,     0,   394,     0,     0,     0,   374,     0,   373,
       0,     0,     0,     0,   208,     0,   401,   376,   379,   214,
       0,   213,   185,   350,     0,   165,   385,   388,   387,   483,
     484,   485,   487,     0,     0,   136,   135,     0,   134,     0,
       0,   500,   502,     0,   507,     0,   503,     0,   452,   505,
     504,     0,   303,   296,   290,   289,   291,   301,   302,   304,
     292,   293,   295,   277,   294,   278,   281,   285,   275,     0,
     427,   242,   243,     0,   248,   246,   247,   260,   261,   249,
     250,   252,     0,     0,   262,     0,   264,     0,   256,   257,
     259,   240,   244,   428,   241,     0,   225,   226,     0,   227,
       0,   224,   220,     0,   308,     0,     0,   408,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,   217,   218,
       0,     0,     0,   327,     0,     0,     0,     0,     0,     0,
       0,   332,   333,     0,     0,     0,   326,     0,     0,     0,
       0,     0,     0,   408,     0,     0,     0,     0,     0,     0,
     165,   154,   139,   143,   347,     0,     0,     0,   454,     0,
       0,   245,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    24,     0,   405,   406,   407,   402,   403,     0,   411,
       0,     0,   354,   364,     0,   381,   201,     0,   401,   395,
     193,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   370,   371,   372,   203,     0,   411,
     378,   219,   348,     0,   169,   164,   166,     0,   508,     0,
     509,   478,   429,   251,   253,   254,   263,   255,   266,   265,
     258,   222,   223,   221,     0,     0,     0,     0,   416,     0,
       0,     0,     0,     0,   408,     0,     0,   413,   337,   339,
     340,   341,   343,   342,   336,   328,   329,   330,   331,   334,
     335,   338,     0,     0,   416,   209,   167,    31,     0,     0,
       0,     0,     0,   449,   404,   410,   409,     0,     0,   418,
       0,   537,     0,   363,   205,   411,     0,   322,   368,   207,
     418,    31,   170,     0,   510,    20,     0,   267,   412,     0,
       0,   414,   536,     0,   416,     0,     0,   414,   168,     0,
       0,   417,     0,     0,   358,   365,   418,   323,   369,   195,
       0,    21,   419,     0,   360,   414,     0,    25,    26,    23,
       0,     0,   415,   361,   359,   197,   196,    13,    22,   511,
       0,   198
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,   102,   103,   104,   105,   106,
    1131,  1191,   964,  1189,    12,   176,   107,   424,   108,   109,
     110,   111,   233,   112,   113,   799,   594,   802,   530,   531,
     532,   533,   114,   516,   893,   115,   116,   485,   117,   118,
     234,   788,  1009,  1010,  1151,  1127,   119,   653,   120,   121,
     251,   122,   650,   123,   422,   124,   378,   505,   645,   125,
     126,   127,   355,   347,   128,  1186,   129,  1200,   440,   618,
     639,   858,   876,   130,   377,   880,   567,   980,   747,   962,
     131,   274,   561,   132,   461,   311,   746,   951,  1090,   133,
     458,   301,   457,   740,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   768,   144,   145,   146,   147,   774,
     996,  1114,   148,   149,   552,   150,   151,   152,   153,   154,
     155,   156,   157,   158,  1184,  1194,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   316,   348,   967,  1036,  1037,  1039,  1136,  1098,  1108,
    1174,  1139,  1161,   319,   545,   608,   952,   851,   609,   257,
     352,   291,   276,   434,   328,   329,   576,   577,   312,   313,
     302,   303,   401,   353,   891,   662,   663,   664,   403,   404,
     405,   292,   452,   246,   265,   367,   743,   419,   420,   421,
     630,   333,   334
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1056
static const yytype_int16 yypact[] =
{
     327,  -134,  -107,   133, -1056,   327,    29,   -70,   -58, -1056,
   -1056,   -45,   812,   -36, -1056,    70, -1056,    -7,    18,    30,
      46, -1056,    56,    66,    86,   105,   109,   123,   138,   142,
     147,   165,   202,   219,   221,   226,   230,   243,   245,   246,
     252,   253,   257,   259,   260,   268,   276,   277,   293,   294,
     295,   304,   305,   306,   313,   323,   329,   332,   333,   334,
     336,   337,   340,   341,   345,   348,    17,   351,   354,   363,
   -1056,   364,   -15,   774, -1056, -1056,   365,   369,   372,   387,
      15,    91,   125, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,   812, -1056,
   -1056,    54, -1056, -1056, -1056, -1056, -1056,   390, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056,   452,   235, -1056,  -118,   616,    77,
     324,   394,   866,    24,    24,   126,   -40,    36,   -24,   496,
     -24,   716,  -110,  -110,    -9,   317,   -24,   -24,   472,   -24,
     -18,  -110,  -110,   -39,    -9,    -9,    -9,    91,   352,    91,
     -24,   866,   866,   -24,   866,   866,    89,   -24,   866,   -24,
     -39,   866,  -110,   866,    98, -1056,   866,  -110,   456,   778,
      91, -1056, -1056,   335,   388,   -24,   -24,   -24,   -24,   393,
   -1056,    20, -1056,   395, -1056,   229, -1056,   112, -1056,   151,
   -1056,   392,   412,    70, -1056, -1056,   409, -1056,  -125,   421,
     422,   423, -1056, -1056, -1056, -1056, -1056,   425, -1056, -1056,
     424,   653, -1056,   429,   437,   438, -1056, -1056,  -110,  -110,
     -24,   -24,   445,   -24,   459,   460,   461,   866,   462,   502,
   -1056, -1056,   377,   440, -1056, -1056, -1056,   688, -1056, -1056,
     463, -1056, -1056, -1056, -1056, -1056, -1056,   690, -1056, -1056,
     470,   469,   476, -1056, -1056, -1056,   477, -1056, -1056,   479,
     480,   483,   484, -1056, -1056,   706, -1056,   495,   500, -1056,
     501,   503,   723,   507, -1056,   508,   509,   510,   528,   530,
     748,   531,   532, -1056, -1056, -1056, -1056,   533,   753,   537,
     538,   541,   542,   234,   761,   543,   194,   544,   546, -1056,
     547, -1056, -1056, -1056, -1056, -1056, -1056, -1056,   548,   549,
     557,   558,   559,   561,   562, -1056, -1056,   563,   392,   564,
     565,   566, -1056,   569,    91,    91,   567,   568,   577,   578,
   -1056,   576,  -132,    91,    91, -1056,    91,    91,    91,    91,
      91,   521,   234, -1056,   582,   580, -1056, -1056, -1056, -1056,
   -1056, -1056,   584,    31,    22, -1056, -1056,   521,   234,   598,
     601,   603,   812,   812, -1056,    59, -1056,    91,   821,  -118,
     843,    16,   844,   628,   625,   866,   631,    91,   287,   853,
     633, -1056, -1056,   648,   655,   527, -1056,   -24,   -24,   205,
   -1056,   660,   657,   866,   788,   663,    91,   664,   392,   665,
      91,   392,   -24,   -24,   866,   808,    68,   809,   674,    91,
      59,   859,   907,   687,   872,   873,   218,   725,   -24,   825,
     694,   845,    -9,   -30, -1056,   714,    -9,    -9,    -9,    91,
     719,    60,   -24,   248,   848,   -17,   747,   851,   841,   -21,
      84,   818,   820,    36,   817, -1056,    33,    33,   807, -1056,
     185,   731,   -29,   814,   819,   -27,   750, -1056, -1056,   373,
     415,   122,   122, -1056, -1056, -1056,   112, -1056,   866,   744,
    -104,   -64,   -63,   -42, -1056, -1056,   521,   234,   115,   274,
     135, -1056,   738, -1056, -1056, -1056,   498, -1056, -1056, -1056,
   -1056,   968, -1056,   751,   425, -1056,   756,   974,   534, -1056,
   -1056,   438, -1056, -1056,   -24,   -24,   699,   762,   760,   764,
     765, -1056,   811,   458, -1056,   805,   813, -1056,   815,   824,
   -1056, -1056, -1056,   806,   540,   366, -1056,   839,   615, -1056,
   -1056, -1056, -1056,   826,   827, -1056,   828,  1038,   641,   849,
   -1056,   855,  1042, -1056,   863, -1056,   860, -1056,   867, -1056,
     847,   869, -1056,  1046, -1056,   870, -1056,  1092,   874,    59,
     875,   876, -1056,   877,  1015, -1056, -1056, -1056, -1056, -1056,
     878, -1056,   879,   880, -1056,   882,   883, -1056,  1101,   884,
   -1056,   885, -1056,   886,   887,   971,  1113,   892,   893, -1056,
     392, -1056,   834,    91, -1056, -1056,   521,   895,   896, -1056,
   -1056,   897, -1056,   900,   894, -1056, -1056, -1056, -1056,  1122,
     902, -1056,     5, -1056,    91, -1056,  -118, -1056,    26, -1056,
      68, -1056,    27, -1056, -1056, -1056,   905,  1124, -1056, -1056,
     919, -1056,   914, -1056,   924,  1067,   866, -1056,    91,    91,
     866,   926,    91,   866,   866,   925,   927, -1056, -1056, -1056,
   -1056,   928, -1056, -1056, -1056, -1056,   930, -1056,   931,   932,
     933, -1056,   934,   935,   936,   937,   938,   939,   940,   941,
     942, -1056,   943, -1056,   944, -1056, -1056, -1056,   945, -1056,
   -1056, -1056, -1056, -1056,   946, -1056,   947,   948,    36,    68,
    1148,   950, -1056,   -39,  1172,   952,   988,  1175,   111,   126,
     990,   -86,  1088,   957,   -10, -1056,   951,  1090, -1056,    91,
     960,  -118,  1117,   -24,   962,  1089,   130,   963,    -6,  1093,
    1186,   314,   392,  1094,    33, -1056, -1056,   234,   961,    59,
     -22,   172,   750, -1056,    51, -1056, -1056,   234,   521,   -25,
   -1056,    -5,    13, -1056,    59,   970, -1056, -1056,    91, -1056,
     969,   353,   265,   972,    59,   700,   973,   976,    91, -1056,
      91,    24,  1083,  1086,    91,  1080,  1085,  1082,    91,    91,
      91,  -118,  1050,    91,    91,    91,   126, -1056,   958,    19,
   -1056, -1056,   983,  1205,   985,   986,  1208,   987,   984, -1056,
   -1056,   991, -1056,   989, -1056,  1213, -1056,   342,   993, -1056,
   -1056,   994,    90,   521,   995,   996,   711, -1056,   997, -1056,
     998,   999,  1000,  1222, -1056,  1223,   985, -1056,  1003, -1056,
    1004, -1056, -1056, -1056,  1005,   197, -1056, -1056,   521, -1056,
   -1056, -1056, -1056,   392,    26, -1056, -1056,    27, -1056,  1006,
    1226,   234, -1056,  1188, -1056,    91, -1056,  1007, -1056, -1056,
   -1056,   401, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056,   521, -1056, -1056, -1056, -1056, -1056,    82,
   -1056, -1056, -1056,  -118, -1056, -1056, -1056, -1056, -1056, -1056,
    1010, -1056,  1011,  1012, -1056,  1013, -1056,  1014, -1056, -1056,
    1016, -1056, -1056, -1056,   521,  1017, -1056, -1056,  1018, -1056,
    1019, -1056, -1056,  1230, -1056,  1021,   389,  1137,  1243,  1023,
     126, -1056,   -12,   126,  1022,  1024,    -6,    91, -1056, -1056,
    1025,  1181,  1157, -1056,  1028,  1029,  1030,  1031,  1032,  1033,
    1034, -1056, -1056,  1035,  1036,  1037, -1056,  1158,  1159,  1160,
    1161,  1041,  1039,  1137,  1266,    90,  1264,   225,  1047,  1040,
     197, -1056, -1056, -1056, -1056,  1049,  1048,   360, -1056,    91,
    1191,   521,    91,    91,    91,    91,    91,   -75,    24,  1273,
    1131, -1056,  1275, -1056, -1056, -1056, -1056,  1055,  1056,  1180,
    1058,  1280, -1056,  1060,  1061, -1056, -1056,   287,   985, -1056,
   -1056,  1063,  1064,  1190,  1282,    49,   126,    24,    16,    16,
     -24,   -40,  1287,  1068, -1056, -1056, -1056, -1056,  1288,  1180,
   -1056, -1056, -1056,  1069, -1056, -1056, -1056,  1289, -1056,  1252,
   -1056,   271, -1056, -1056, -1056, -1056, -1056, -1056, -1056,  1020,
   -1056, -1056, -1056, -1056,  1070,   389,  1194,  1073,  1276,  1297,
    1074,  1114,   126,  1075,  1137,  1210,  1214, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056,  1199,  1081,  1276,   948, -1056,   812,  1087,  1091,
    1095,  1096,   -75, -1056, -1056, -1056, -1056,  1190,  1084,  1286,
    1097, -1056,  1100, -1056, -1056,  1180,  1098, -1056,  1102, -1056,
    1286,   812, -1056,    59, -1056, -1056,  1103, -1056, -1056,  1215,
    1104,  1123, -1056,   126,  1276,  1221,    59,  1123, -1056,  1106,
     -24, -1056,  1312,  1107, -1056, -1056,  1286, -1056, -1056, -1056,
     104,  1109, -1056,   126,   392,  1123,   392, -1056, -1056,   983,
     -24,  1108, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
     392, -1056
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1056, -1056,  1315, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056,   128, -1056, -1056,  1076,  -108,  -375,   899,  1110,
    1249,  -504, -1056, -1056, -1056, -1056,   652, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,  1269, -1056,
   -1056, -1056,   328, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056,   846,  1120, -1056, -1056, -1056, -1056,   780, -1056,
   -1056, -1056,   367, -1056, -1056, -1056,  -610,   339,   275, -1056,
   -1056,   485,   298, -1056, -1056, -1056, -1056, -1056,   215, -1056,
   -1056,  1164, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056,  -620, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056,   288,   597,  -843,   256, -1056,  -938, -1056,  -978,   216,
   -1055, -1011, -1045,  -478,  -463, -1056, -1056, -1056,   514,   929,
    -228,  -179,  -401,   800,   382,  -392,  -491,  -645,  -533,  -452,
    -530,  -646,  -172,   -77, -1056,   570, -1056, -1056,   829, -1056,
   -1056,  1072,  -139,   771, -1056,  -481, -1056, -1056, -1056, -1056,
   -1056,  -158, -1056
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -237
static const yytype_int16 yytable[] =
{
     248,   275,   383,   504,   245,   258,   681,   599,   679,   320,
     626,   322,   661,   534,   595,   647,   648,   336,   337,   550,
     339,   341,   535,    16,   240,   644,   240,   294,   391,   294,
     571,   357,   800,  1003,   360,   529,   571,   803,   368,   391,
     370,   331,   342,   293,   314,   345,   349,   350,   351,   683,
     331,   632,   324,  1110,   616,   655,   386,   387,   388,   389,
     266,   267,   887,   637,   363,  1069,   266,   267,   364,   365,
     415,   305,   358,   359,   856,   361,   362,   417,   874,   369,
    1043,   263,   372,   586,   374,   809,   589,   379,   640,   266,
     267,  1124,     7,   363,   978,   240,   518,   364,   365,   955,
     428,   443,   444,   375,   446,  1167,   266,   267,   295,  1187,
     295,   317,  1179,  1150,  1088,   254,   240,   325,   294,     8,
     318,   670,   296,   659,   296,   660,   255,   256,   671,   305,
    1195,  1185,   356,     9,   868,   543,   956,   957,   571,   846,
     326,   327,   806,   544,  1111,   958,  1112,   396,   397,   398,
     399,   400,   306,  1176,   959,   240,   765,   511,  1089,    11,
     429,   672,   674,    13,   392,   960,  1145,  1164,   673,   675,
     402,   881,   418,   366,   979,    14,   264,   406,  1044,   407,
     633,    15,   376,   676,   408,   409,   410,   641,  1188,   174,
     677,   961,   270,   315,   346,   849,  1113,   617,   270,   295,
     894,   175,   850,   271,   254,  1104,   638,   895,   571,   340,
     306,   254,   572,   296,   230,   255,   256,   857,   332,   177,
     749,   875,   255,   256,   272,   273,   852,   896,   595,   363,
     272,   273,   271,   364,   365,   266,   267,   869,   897,   870,
     871,   872,   536,   551,   178,   898,    81,   241,  1012,   413,
     239,   297,  1013,   297,   646,   254,   179,   298,   241,   298,
     646,   623,   242,   243,   242,   243,   255,   256,   569,   570,
     840,   796,   180,   242,   243,   785,   843,   305,   795,   806,
     299,   300,   181,   590,   591,   574,   575,   354,   661,   266,
     267,   913,   182,   884,   411,   307,   556,   841,   244,   611,
     244,   308,   605,   928,   606,   607,   568,   510,  1020,   244,
     578,   757,   183,   627,   581,   542,   519,   520,   241,   521,
     522,   523,   524,   525,   615,   592,   886,   628,   620,   621,
     622,   184,   629,   242,   243,   185,   392,   537,   249,   241,
     656,   899,   297,     1,     2,   254,   678,   651,   298,   186,
     546,   907,   247,   307,   242,   243,   255,   256,   306,   308,
     558,  1007,   646,  1008,   187,   412,   682,   270,   188,   244,
     708,   299,   300,   189,   709,   710,   559,   560,   413,   584,
     309,   310,   414,   588,   254,   693,   694,   574,   575,   668,
     244,   190,   598,   242,   243,   255,   256,   566,   879,   272,
     273,   326,   327,   398,   399,   400,   741,   882,   266,   267,
     889,   890,   268,   269,   278,   279,   280,   281,   282,   492,
     283,   270,   284,   285,   286,   287,   288,   711,   191,   244,
     268,   269,   573,  1033,  1034,  1035,   394,  1042,  1117,  1118,
    1045,   712,   713,   714,   715,   192,   716,   193,   717,   402,
     266,   267,   194,   272,   273,   252,   195,   574,   575,   395,
     253,   701,   436,   290,   396,   397,   398,   399,   400,   196,
     652,   197,   198,   396,   397,   398,   399,   400,   199,   200,
     718,   719,   720,   201,   721,   202,   203,   722,   723,   724,
     905,   725,   726,   727,   204,   728,  1130,   277,  1091,   -19,
     321,   307,   205,   206,   798,   680,   436,   308,   396,   397,
     398,   399,   400,   396,   397,   398,   399,   400,  1011,   207,
     208,   209,   729,  1115,   730,   731,  1073,  1116,   309,   310,
     210,   211,   212,   732,   733,   734,   735,   736,   737,   213,
     270,   861,   266,   267,   396,   397,   398,   399,   400,   214,
     335,   271,   398,   399,   400,   215,   738,   810,   216,   217,
     218,   813,   219,   220,   816,   817,   221,   222,   338,  1143,
     739,   223,   272,   273,   224,   330,   787,   226,   903,   354,
     227,   904,   270,   343,   344,  1079,   266,   267,  1080,   228,
     229,   235,   912,   271,   865,   236,   916,   797,   237,   863,
     920,   921,   922,   665,   373,   925,   926,   927,   381,   380,
     566,   266,   267,   238,   272,   273,   250,   385,   888,   384,
     390,   811,   812,   423,   393,   815,   259,   260,   261,   262,
    1175,   416,   396,   397,   398,   399,   400,   425,   427,   396,
     397,   398,   399,   400,   244,   666,   430,   431,   432,   433,
    1192,   435,   396,   397,   398,   399,   400,   436,   437,   923,
     441,   442,   438,   439,   453,   454,   954,   278,   279,   280,
     281,   282,   445,   283,   270,   284,   285,   286,   287,   288,
     396,  1019,   398,   399,   400,   289,   447,   448,   449,   451,
    1169,   455,   456,   459,   396,   397,   398,   399,   400,   460,
    -236,   462,   463,  1178,   464,   465,   272,   273,   466,   467,
     468,   278,   279,   280,   281,   282,   290,   283,   270,   284,
     285,   286,   287,   288,   469,   470,   471,   473,   685,   289,
     472,   901,   474,   475,   476,   477,   278,   279,   280,   281,
     282,   911,   283,   270,   284,   285,   286,   287,   288,  1049,
     272,   273,   480,   478,   289,   479,   481,   482,   483,   484,
     290,  1021,   486,   487,   691,   490,   488,   489,   491,   493,
     707,   494,   495,   496,   497,   272,   273,   396,   397,   398,
     399,   400,   498,   499,   500,   290,   501,   502,   503,   506,
     507,   508,   512,   513,  1083,  1084,  1085,  1086,  1087,   509,
     266,   267,   514,   515,   517,   983,   429,   526,   527,  1193,
     528,  1196,   984,   396,   397,   398,   399,   400,    16,   396,
     397,   398,   399,   400,   538,  1201,    17,   539,  1017,   540,
      18,    19,    20,    21,    22,    23,   547,   985,    24,    25,
      26,    27,    28,    29,    30,   745,   549,   553,    31,    32,
      33,    34,    35,   554,    36,   555,   557,   986,   562,    37,
      38,    39,    40,    41,    42,    43,    44,   563,    45,    46,
      47,   752,    48,   564,    49,    50,    51,    52,    53,    54,
     565,  1119,   579,    55,    56,   580,    57,   582,   583,   585,
     587,   593,   596,    58,   396,   397,   398,   399,   400,   597,
     600,   987,   988,   989,   990,   323,   991,   992,   993,   994,
     995,   601,   602,   603,    59,   610,   604,    60,   612,   613,
     396,   397,   398,   399,   400,   278,   279,   280,   281,   282,
     908,   283,   270,   284,   285,   286,   287,   288,   614,   619,
      61,   634,  1081,   289,   624,   631,    62,   636,    63,    64,
     266,   267,    65,    66,   635,    67,   642,    68,   643,  -184,
     649,   654,   657,    69,   272,   273,    70,    71,   658,   669,
     684,   686,    72,    73,   290,    74,   687,    75,   690,   396,
     397,   398,   399,   400,   689,   695,    83,   696,   697,   698,
     699,  1181,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     231,  1198,    76,    77,    78,    79,    84,    80,    86,  1152,
      88,    85,    90,    87,    92,    89,    94,    91,    96,    93,
      98,    95,   100,    97,   702,    99,   700,   101,   703,   706,
     704,    81,   751,  1168,   929,   930,   755,    82,    83,   705,
     761,   748,   749,   750,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   744,   759,   753,   278,   279,   280,   281,   282,
     754,   283,   270,   284,   285,   286,   287,   288,   756,   757,
     931,   932,   758,   289,   760,   762,   763,   933,   770,   764,
     766,   767,   769,   771,   772,   777,   773,   775,   776,   778,
     779,   780,   781,   782,   272,   273,   701,   783,   784,   786,
     789,   790,   793,   791,   290,   792,   794,   805,   934,   325,
     804,   935,   936,   937,   938,   939,   940,   941,   942,   943,
     944,   945,   946,   947,   948,   949,   950,   806,   807,   808,
     566,   814,   818,   842,   820,   819,   821,   822,   823,   824,
     825,   826,   827,   828,   829,   830,   831,   832,   833,   834,
     835,   836,   838,   839,   837,   843,   845,   846,   847,   848,
     853,   854,   855,   860,   859,   862,   864,   866,   873,   867,
     878,   254,   885,   877,   883,   900,   914,   902,   915,   917,
     906,   909,   255,   256,   910,   918,   919,   924,   963,   965,
     966,   968,   969,   970,   971,   973,   972,   974,   976,   977,
     981,   982,   997,   998,   999,  1000,  1001,  1002,  1004,  1005,
    1006,  1015,  1016,  1031,  1014,  1018,  1022,  1023,  1024,  1025,
    1026,  1038,  1027,  1028,  1029,  1030,  1032,  1040,  1041,  1047,
    1046,  1051,  1052,  1050,  1053,  1054,  1055,  1056,  1057,  1058,
    1059,  1060,  1061,  1062,  1068,  1063,  1064,  1065,  1066,  1067,
    1070,  1072,  1075,  1074,  1077,  1082,  1078,  1092,  1093,  1094,
    1095,  1097,  1096,  1099,  1100,  1101,  1109,  1102,  1105,  1106,
    1107,  1121,  1123,  1122,  1128,  1126,  1129,  1135,  1133,  1137,
    1138,  1140,  1141,  1144,  1142,  1146,  1132,  1148,  1147,  1149,
    1159,  1160,  1153,  1173,  1171,  1177,  1182,  1197,  1155,  1154,
      10,   541,   232,  1165,  1156,  1162,  1163,  1166,  1170,   426,
    1172,  1180,   801,  1183,  1190,   225,  1199,   625,  1076,   382,
     371,   692,   975,  1048,  1071,  1103,  1125,  1157,   304,  1120,
     844,  1134,   953,  1158,   688,   667,   742,     0,   548,   450,
       0,     0,   892
};

static const yytype_int16 yycheck[] =
{
     108,   180,   230,   378,    81,   177,   539,   470,   538,   188,
     491,   190,   516,   414,   466,   506,   507,   196,   197,     3,
     199,   200,   414,     6,     4,   503,     4,     3,     8,     3,
       3,   210,   678,   876,   213,     4,     3,   682,   217,     8,
     219,    59,   200,   182,    84,    84,   204,   205,   206,   540,
      59,    68,   191,     4,    84,    84,   235,   236,   237,   238,
      84,    85,    84,    84,     4,  1003,    84,    85,     8,     9,
     249,     3,   211,   212,    84,   214,   215,   249,    84,   218,
      92,     4,   221,   458,   223,   695,   461,   226,     4,    84,
      85,  1069,   226,     4,     4,     4,   228,     8,     9,    80,
     225,   280,   281,     5,   283,  1150,    84,    85,    84,     5,
      84,    75,  1167,  1124,   189,   233,     4,   227,     3,   226,
      84,   225,    98,   150,    98,   152,   244,   245,   232,     3,
    1185,  1176,   209,     0,     4,    76,   117,   118,     3,   225,
     250,   251,   228,    84,    95,   126,    97,   279,   280,   281,
     282,   283,    84,  1164,   135,     4,   619,   385,   233,   130,
     285,   225,   225,   233,   241,   146,  1104,  1145,   232,   232,
     247,   781,   249,    84,    84,   233,    99,    26,   190,    28,
     197,   226,    84,   225,    33,    34,    35,   103,    84,   225,
     232,   172,   216,   233,   233,    84,   147,   227,   216,    84,
     225,   131,    91,   227,   233,  1048,   227,   232,     3,   227,
      84,   233,     7,    98,   229,   244,   245,   227,   227,   226,
     225,   227,   244,   245,   248,   249,   759,   232,   680,     4,
     248,   249,   227,     8,     9,    84,    85,   107,   225,   109,
     110,   111,   414,   227,   226,   232,   229,   227,   894,   227,
     235,   227,   897,   227,   227,   233,   226,   233,   227,   233,
     227,   489,   242,   243,   242,   243,   244,   245,   447,   448,
     748,   672,   226,   242,   243,   650,   225,     3,   670,   228,
     256,   257,   226,   462,   463,   252,   253,   227,   792,    84,
      85,   821,   226,   784,   143,   227,   435,   749,   278,   478,
     278,   233,    84,   836,    86,    87,   445,   384,   226,   278,
     449,   229,   226,   492,   453,   423,   393,   394,   227,   396,
     397,   398,   399,   400,   482,   464,   789,    79,   486,   487,
     488,   226,    84,   242,   243,   226,   413,   414,   284,   227,
     512,   804,   227,    16,    17,   233,   231,   162,   233,   226,
     427,   814,   227,   227,   242,   243,   244,   245,    84,   233,
     437,   164,   227,   166,   226,   214,   231,   216,   226,   278,
       4,   256,   257,   226,     8,     9,    89,    90,   227,   456,
     254,   255,   231,   460,   233,   564,   565,   252,   253,   528,
     278,   226,   469,   242,   243,   244,   245,    83,    84,   248,
     249,   250,   251,   281,   282,   283,   585,   782,    84,    85,
     238,   239,    88,    89,   209,   210,   211,   212,   213,   225,
     215,   216,   217,   218,   219,   220,   221,    61,   226,   278,
      88,    89,   227,    44,    45,    46,   207,   970,  1058,  1059,
     973,    75,    76,    77,    78,   226,    80,   226,    82,   526,
      84,    85,   226,   248,   249,     3,   226,   252,   253,   230,
     225,     3,     4,   258,   279,   280,   281,   282,   283,   226,
     285,   226,   226,   279,   280,   281,   282,   283,   226,   226,
     114,   115,   116,   226,   118,   226,   226,   121,   122,   123,
     225,   125,   126,   127,   226,   129,   225,   103,  1028,   228,
       4,   227,   226,   226,   676,   231,     4,   233,   279,   280,
     281,   282,   283,   279,   280,   281,   282,   283,   893,   226,
     226,   226,   156,  1056,   158,   159,  1007,  1057,   254,   255,
     226,   226,   226,   167,   168,   169,   170,   171,   172,   226,
     216,   769,    84,    85,   279,   280,   281,   282,   283,   226,
     233,   227,   281,   282,   283,   226,   190,   696,   226,   226,
     226,   700,   226,   226,   703,   704,   226,   226,    96,  1102,
     204,   226,   248,   249,   226,   193,   653,   226,   225,   227,
     226,   228,   216,   201,   202,   225,    84,    85,   228,   226,
     226,   226,   820,   227,   773,   226,   824,   674,   226,   771,
     828,   829,   830,   230,   222,   833,   834,   835,   152,   227,
      83,    84,    85,   226,   248,   249,   226,   229,   790,   284,
     227,   698,   699,   231,   229,   702,    10,    11,    12,    13,
    1163,   249,   279,   280,   281,   282,   283,   225,   229,   279,
     280,   281,   282,   283,   278,   230,   225,   225,   225,   224,
    1183,   227,   279,   280,   281,   282,   283,     4,   229,   831,
     278,   279,   225,   225,   287,   225,   838,   209,   210,   211,
     212,   213,   227,   215,   216,   217,   218,   219,   220,   221,
     279,   280,   281,   282,   283,   227,   227,   227,   227,   227,
    1153,     3,   229,     3,   279,   280,   281,   282,   283,   229,
     231,   225,   225,  1166,   225,   225,   248,   249,   225,   225,
       4,   209,   210,   211,   212,   213,   258,   215,   216,   217,
     218,   219,   220,   221,   229,   225,   225,     4,   230,   227,
     227,   808,   225,   225,   225,   225,   209,   210,   211,   212,
     213,   818,   215,   216,   217,   218,   219,   220,   221,   977,
     248,   249,     4,   225,   227,   225,   225,   225,   225,     6,
     258,   933,   225,   225,   230,     4,   225,   225,   225,   225,
     230,   225,   225,   225,   225,   248,   249,   279,   280,   281,
     282,   283,   225,   225,   225,   258,   225,   225,   225,   225,
     225,   225,   225,   225,  1022,  1023,  1024,  1025,  1026,   230,
      84,    85,   225,   225,   228,    94,   285,   225,   228,  1184,
     226,  1186,   101,   279,   280,   281,   282,   283,     6,   279,
     280,   281,   282,   283,   226,  1200,    14,   226,   905,   226,
      18,    19,    20,    21,    22,    23,    15,   126,    26,    27,
      28,    29,    30,    31,    32,   230,     3,     3,    36,    37,
      38,    39,    40,   225,    42,   230,   225,   146,     5,    47,
      48,    49,    50,    51,    52,    53,    54,   234,    56,    57,
      58,   230,    60,   225,    62,    63,    64,    65,    66,    67,
     225,  1060,   222,    71,    72,   228,    74,    99,   225,   225,
     225,    83,    83,    81,   279,   280,   281,   282,   283,   225,
      41,   190,   191,   192,   193,   189,   195,   196,   197,   198,
     199,     4,   225,    41,   102,   190,    43,   105,    93,   225,
     279,   280,   281,   282,   283,   209,   210,   211,   212,   213,
     230,   215,   216,   217,   218,   219,   220,   221,    93,   225,
     128,   194,  1019,   227,   225,    97,   134,   106,   136,   137,
      84,    85,   140,   141,   103,   143,   138,   145,   138,   142,
     153,   230,   148,   151,   248,   249,   154,   155,   149,   225,
     232,     3,   160,   161,   258,   163,   225,   165,     4,   279,
     280,   281,   282,   283,   228,   286,   236,   225,   228,   225,
     225,  1170,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     236,  1190,   200,   201,   202,   203,   242,   205,   244,  1127,
     246,   243,   248,   245,   250,   247,   252,   249,   254,   251,
     256,   253,   258,   255,   229,   257,   225,   259,   225,   233,
     225,   229,     4,  1151,    86,    87,     4,   235,   236,   225,
       4,   225,   225,   225,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   233,   226,   225,   209,   210,   211,   212,   213,
     225,   215,   216,   217,   218,   219,   220,   221,   225,   229,
     132,   133,   225,   227,   225,   225,     4,   139,    83,   225,
     225,   225,   225,   225,   225,     4,   226,   225,   225,   225,
     225,   225,   225,   142,   248,   249,     3,   225,   225,   285,
     225,   225,   228,   226,   258,   225,     4,     3,   170,   227,
     225,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   228,   234,   225,
      83,   225,   227,     5,   226,   228,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   225,   225,   228,   225,     4,   225,   190,     4,
     190,    93,   225,    93,   233,   225,    69,   225,   225,   100,
       4,   233,   231,   100,   100,   225,   113,   228,   112,   119,
     228,   228,   244,   245,   228,   120,   124,   157,   225,     4,
     225,   225,     4,   226,   230,   226,   225,     4,   225,   225,
     225,   225,   225,   225,   225,   225,     4,     4,   225,   225,
     225,     5,    44,     3,   228,   228,   226,   226,   226,   226,
     226,   104,   226,   226,   226,   226,   225,     4,   225,   225,
     228,    70,    95,   228,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   225,   107,   107,   107,   107,   228,
       4,     7,   232,   226,   225,    84,   228,     4,   147,     4,
     225,   101,   226,   225,     4,   225,     4,   226,   225,   225,
     100,     4,     4,   225,     5,   226,    44,   103,   228,   226,
      24,     4,   228,   228,   190,    95,   286,   108,    94,   228,
     226,    25,   225,   190,    99,    94,     4,  1189,   223,   228,
       5,   422,    73,   225,   228,   228,   226,   225,   225,   253,
     226,   225,   680,   226,   225,    66,   228,   491,  1010,   229,
     220,   561,   857,   976,  1005,  1047,  1071,  1132,   184,  1061,
     753,  1095,   838,  1137,   554,   526,   585,    -1,   429,   287,
      -1,    -1,   792
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
     476,   477,   342,   231,   305,   225,   303,   229,   225,   285,
     225,   225,   225,   224,   451,   227,     4,   229,   225,   225,
     356,   452,   452,   449,   449,   227,   449,   227,   227,   227,
     469,   227,   470,   287,   225,     3,   229,   380,   378,     3,
     229,   372,   225,   225,   225,   225,   225,   225,     4,   229,
     225,   225,   227,     4,   225,   225,   225,   225,   225,   225,
       4,   225,   225,   225,     6,   325,   225,   225,   225,   225,
       4,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   305,   345,   225,   225,   225,   230,
     461,   448,   225,   225,   225,   225,   321,   228,   228,   461,
     461,   461,   461,   461,   461,   461,   225,   228,   226,     4,
     316,   317,   318,   319,   450,   453,   460,   461,   226,   226,
     226,   306,   304,    76,    84,   442,   461,    15,   447,     3,
       3,   227,   402,     3,   225,   230,   470,   225,   461,    89,
      90,   370,     5,   234,   225,   225,    83,   364,   470,   449,
     449,     3,     7,   227,   252,   253,   454,   455,   470,   222,
     228,   470,    99,   225,   461,   225,   305,   225,   461,   305,
     449,   449,   470,    83,   314,   457,    83,   225,   461,   442,
      41,     4,   225,    41,    43,    84,    86,    87,   443,   446,
     190,   449,    93,   225,    93,   479,    84,   227,   357,   225,
     479,   479,   479,   448,   225,   350,   473,   449,    79,    84,
     478,    97,    68,   197,   194,   103,   106,    84,   227,   358,
       4,   103,   138,   138,   441,   346,   227,   454,   454,   153,
     340,   162,   285,   335,   230,    84,   460,   148,   149,   150,
     152,   309,   463,   464,   465,   230,   230,   466,   470,   225,
     225,   232,   225,   232,   225,   232,   225,   232,   231,   458,
     231,   456,   231,   454,   232,   230,     3,   225,   451,   228,
       4,   230,   356,   449,   449,   286,   225,   228,   225,   225,
     225,     3,   229,   225,   225,   225,   233,   230,     4,     8,
       9,    61,    75,    76,    77,    78,    80,    82,   114,   115,
     116,   118,   121,   122,   123,   125,   126,   127,   129,   156,
     158,   159,   167,   168,   169,   170,   171,   172,   190,   204,
     381,   449,   471,   474,   233,   230,   374,   366,   225,   225,
     225,     4,   230,   225,   225,     4,   225,   229,   225,   226,
     225,     4,   225,     4,   225,   442,   225,   225,   392,   225,
      83,   225,   225,   226,   397,   225,   225,     4,   225,   225,
     225,   225,   142,   225,   225,   305,   285,   461,   329,   225,
     225,   226,   225,   228,     4,   453,   450,   461,   460,   313,
     459,   314,   315,   455,   225,     3,   228,   234,   225,   364,
     470,   461,   461,   470,   225,   461,   470,   470,   227,   228,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   228,   225,   225,
     441,   457,     5,   225,   430,     4,   225,   190,     4,    84,
      91,   445,   456,   190,    93,   225,    84,   227,   359,   233,
      93,   448,   225,   460,    69,   449,   225,   100,     4,   107,
     109,   110,   111,   225,    84,   227,   360,   100,     4,    84,
     363,   364,   305,   100,   454,   231,   442,    84,   460,   238,
     239,   462,   463,   322,   225,   232,   232,   225,   232,   442,
     225,   461,   228,   225,   228,   225,   228,   442,   230,   228,
     228,   461,   448,   458,   113,   112,   448,   119,   120,   124,
     448,   448,   448,   460,   157,   448,   448,   448,   456,    86,
      87,   132,   133,   139,   170,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   375,   444,   446,   460,    80,   117,   118,   126,   135,
     146,   172,   367,   225,   300,     4,   225,   431,   225,     4,
     226,   230,   225,   226,     4,   369,   225,   225,     4,    84,
     365,   225,   225,    94,   101,   126,   146,   190,   191,   192,
     193,   195,   196,   197,   198,   199,   398,   225,   225,   225,
     225,     4,     4,   431,   225,   225,   225,   164,   166,   330,
     331,   305,   459,   455,   228,     5,    44,   461,   228,   280,
     226,   460,   226,   226,   226,   226,   226,   226,   226,   226,
     226,     3,   225,    44,    45,    46,   432,   433,   104,   434,
       4,   225,   456,    92,   190,   456,   228,   225,   360,   448,
     228,    70,    95,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   107,   107,   107,   107,   228,   225,   434,
       4,   365,     7,   473,   226,   232,   330,   225,   228,   225,
     228,   461,    84,   448,   448,   448,   448,   448,   189,   233,
     376,   458,     4,   147,     4,   225,   226,   101,   436,   225,
       4,   225,   226,   370,   431,   225,   225,   100,   437,     4,
       4,    95,    97,   147,   399,   456,   458,   402,   402,   449,
     429,     4,   225,     4,   436,   366,   226,   333,     5,    44,
     225,   298,   286,   228,   432,   103,   435,   226,    24,   439,
       4,   228,   190,   456,   228,   434,    95,    94,   108,   228,
     439,   332,   304,   225,   228,   223,   228,   376,   437,   226,
      25,   440,   228,   226,   436,   225,   225,   440,   304,   442,
     225,    99,   226,   190,   438,   456,   439,    94,   442,   438,
     225,   449,     4,   226,   412,   440,   353,     5,    84,   301,
     225,   299,   456,   305,   413,   438,   305,   300,   449,   228,
     355,   305
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
#line 344 "lev_comp.y"
    {
              int lit = (int) (yyvsp[(7) - (7)].i);
		      start_level_def(&splev, (yyvsp[(3) - (7)].map));
		      if ((yyvsp[(5) - (7)].i) == -1) {
			  add_opvars(splev, "iiiiiiiio",
				     VA_PASS9(LVLINIT_MAZEGRID, HWALL, 0, lit,
					      0,0,0,0, SPO_INITLEVEL));
		      } else {
			  int bg = (int)what_map_char((char) (yyvsp[(5) - (7)].i));

			  add_opvars(splev, "iiiiiiiio",
				     VA_PASS9(LVLINIT_SOLIDFILL, bg, 0, lit,
					      0,0,0,0, SPO_INITLEVEL));
		      }
		      add_opvars(splev, "io",
				 VA_PASS2(MAZELEVEL, SPO_LEVEL_FLAGS));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		      (yyval.map) = (yyvsp[(3) - (7)].map);
		  ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 367 "lev_comp.y"
    {
		      int filling = (int) (yyvsp[(5) - (5)].terr).ter;

		      if (filling == INVALID_LEVTYPE || filling >= MAX_LEVTYPE)
			  lc_error("INIT_MAP: Invalid fill char type.");
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_SOLIDFILL, filling,
                                          0, (int) (yyvsp[(5) - (5)].terr).lit,
                                          0,0,0,0, SPO_INITLEVEL));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 380 "lev_comp.y"
    {
		      int filling = (int) what_map_char((char) (yyvsp[(5) - (5)].i));

		      if (filling == INVALID_LEVTYPE || filling >= MAX_LEVTYPE)
			  lc_error("INIT_MAP: Invalid fill char type.");
                      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_MAZEGRID, filling, 0,0,
					  0,0,0,0, SPO_INITLEVEL));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 392 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_ROGUE,0,0,0,
					  0,0,0,0, SPO_INITLEVEL));
		  ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 398 "lev_comp.y"
    {
                      int fg = (int) what_map_char((char) (yyvsp[(5) - (16)].i)),
                          bg = (int) what_map_char((char) (yyvsp[(7) - (16)].i));
                      int smoothed = (int) (yyvsp[(9) - (16)].i),
                          joined = (int) (yyvsp[(11) - (16)].i),
                          lit = (int) (yyvsp[(13) - (16)].i),
                          walled = (int) (yyvsp[(15) - (16)].i),
                          filling = (int) (yyvsp[(16) - (16)].i);

		      if (fg == INVALID_LEVTYPE || fg >= MAX_LEVTYPE)
			  lc_error("INIT_MAP: Invalid foreground type.");
		      if (bg == INVALID_LEVTYPE || bg >= MAX_LEVTYPE)
			  lc_error("INIT_MAP: Invalid background type.");
		      if (joined && fg != CORR && fg != ROOM && fg != GRASS && fg != GROUND && fg != AIR && fg != CLOUD)
			  lc_error("INIT_MAP: Invalid foreground type for joined map.");

		      if (filling == INVALID_LEVTYPE)
			  lc_error("INIT_MAP: Invalid fill char type.");

		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_MINES, filling, walled, lit,
					  joined, smoothed, bg, fg,
					  SPO_INITLEVEL));
			max_x_map = COLNO-1;
			max_y_map = ROWNO;
		  ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 427 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_TILESET));
		  ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 433 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), (int)(yyvsp[(3) - (5)].i), SPO_FOREST));
		  ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 437 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(0, (int)(yyvsp[(3) - (3)].i), SPO_FOREST));
		  ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 443 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), (int)(yyvsp[(3) - (5)].i), SPO_MONSTER_GENERATION));
		  ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 449 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_BOUNDARY_TYPE));
		  ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 455 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 459 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 465 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_COPY));
		      (yyval.i) = 0;
		  ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 470 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 476 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 480 "lev_comp.y"
    {
		      (yyval.i) = what_map_char((char) (yyvsp[(2) - (2)].i));
		  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 491 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 495 "lev_comp.y"
    {
		      add_opvars(splev, "io",
                                 VA_PASS2((int) (yyvsp[(3) - (3)].i), SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 502 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 506 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 512 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 516 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 522 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (3)].i);
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 626 "lev_comp.y"
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

  case 123:

/* Line 1455 of yacc.c  */
#line 642 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 648 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 654 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 660 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 666 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 672 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 678 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 684 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 690 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 700 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 710 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 720 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 730 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 740 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 750 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 762 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 767 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 774 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 779 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 786 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 791 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 798 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 802 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 808 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 813 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 820 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 824 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 830 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 834 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 840 "lev_comp.y"
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

  case 153:

/* Line 1455 of yacc.c  */
#line 862 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 866 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 877 "lev_comp.y"
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

  case 156:

/* Line 1455 of yacc.c  */
#line 932 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 938 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 942 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 948 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 955 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 959 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 967 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 971 "lev_comp.y"
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

  case 164:

/* Line 1455 of yacc.c  */
#line 994 "lev_comp.y"
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

  case 167:

/* Line 1455 of yacc.c  */
#line 1039 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1049 "lev_comp.y"
    {
		  ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1052 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1062 "lev_comp.y"
    {
		  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1067 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1081 "lev_comp.y"
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

  case 175:

/* Line 1455 of yacc.c  */
#line 1118 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1123 "lev_comp.y"
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

  case 177:

/* Line 1455 of yacc.c  */
#line 1152 "lev_comp.y"
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

  case 178:

/* Line 1455 of yacc.c  */
#line 1166 "lev_comp.y"
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

  case 179:

/* Line 1455 of yacc.c  */
#line 1182 "lev_comp.y"
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

  case 180:

/* Line 1455 of yacc.c  */
#line 1202 "lev_comp.y"
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
#line 1214 "lev_comp.y"
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

  case 182:

/* Line 1455 of yacc.c  */
#line 1234 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1240 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1250 "lev_comp.y"
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

  case 185:

/* Line 1455 of yacc.c  */
#line 1268 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1278 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(NO_COLOR, SPO_MESSAGE));
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1282 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_MESSAGE));
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1287 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1292 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1297 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1304 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1311 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1320 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1328 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1339 "lev_comp.y"
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

  case 196:

/* Line 1455 of yacc.c  */
#line 1357 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1364 "lev_comp.y"
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

  case 198:

/* Line 1455 of yacc.c  */
#line 1382 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1389 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1393 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1399 "lev_comp.y"
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

  case 202:

/* Line 1455 of yacc.c  */
#line 1409 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1415 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1424 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1430 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1435 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1441 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1446 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1452 "lev_comp.y"
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

  case 210:

/* Line 1455 of yacc.c  */
#line 1463 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1477 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1481 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1491 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1498 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1506 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1511 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1516 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1521 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1526 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1531 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1536 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1544 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1551 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1559 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1575 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1579 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1585 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1593 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1599 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1607 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1615 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1620 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1626 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1632 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1638 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1644 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1649 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(0, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1654 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1659 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1664 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1669 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1674 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1679 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1684 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1689 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1694 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1699 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1704 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1710 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2(0xFFFFFFFFL, SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1716 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1721 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_PROTECTOR));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1726 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_MAXHP));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1731 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_LEVEL_ADJUSTMENT));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1736 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_KEEP_ORIGINAL_INVENTORY));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1743 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1752 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1756 "lev_comp.y"
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

  case 268:

/* Line 1455 of yacc.c  */
#line 1769 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1777 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1787 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1795 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1804 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1811 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1819 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1825 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1830 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1835 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1840 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1845 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1850 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1855 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1860 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1865 "lev_comp.y"
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

  case 284:

/* Line 1455 of yacc.c  */
#line 1879 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1885 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1890 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1895 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1900 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1905 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1910 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1915 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1920 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1925 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1930 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1935 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1940 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x00200000;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1945 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x00400000;
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1950 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x00800000;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1955 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x01000000;
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1960 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_MYTHIC_TYPE));
		      (yyval.i) = 0x02000000;
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1966 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x04000000;
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 1971 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x08000000;
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 1976 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x10000000;
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 1981 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MATERIAL));
		      (yyval.i) = 0x20000000;
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 1988 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 1994 "lev_comp.y"
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

  case 307:

/* Line 1455 of yacc.c  */
#line 2023 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2028 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2036 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2042 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2046 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2054 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2061 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2068 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2078 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2088 "lev_comp.y"
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

  case 317:

/* Line 1455 of yacc.c  */
#line 2104 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2114 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2118 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2124 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2130 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2136 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2140 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2146 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2152 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2159 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2167 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2173 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2178 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2183 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2188 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2193 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2199 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2205 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2210 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2215 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2220 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2225 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2230 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2240 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2246 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2252 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2257 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2264 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2271 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2275 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2281 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2287 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2293 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2298 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2305 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2311 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2321 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2327 "lev_comp.y"
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

  case 359:

/* Line 1455 of yacc.c  */
#line 2348 "lev_comp.y"
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

  case 360:

/* Line 1455 of yacc.c  */
#line 2360 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2364 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2370 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2375 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2380 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (long)(yyvsp[(9) - (9)].i), (long)(yyvsp[(7) - (9)].i), (long)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2385 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((long)(yyvsp[(9) - (13)].i), (long)(yyvsp[(7) - (13)].i), (long)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2391 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2395 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2401 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, (int)(yyvsp[(11) - (11)].i), (int)(yyvsp[(9) - (11)].i), (int)(yyvsp[(7) - (11)].i), (int)(yyvsp[(5) - (11)].i), SPO_DECORATION));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2405 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6((int)(yyvsp[(13) - (13)].i), (int)(yyvsp[(11) - (13)].i), (int)(yyvsp[(9) - (13)].i), (int)(yyvsp[(7) - (13)].i), (int)(yyvsp[(5) - (13)].i), SPO_DECORATION));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2409 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2413 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2417 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2421 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, 1, (int)(yyvsp[(7) - (7)].i), 0, (int)(yyvsp[(5) - (7)].i), SPO_DECORATION));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2427 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2433 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2437 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2443 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_CARPET));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2449 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2453 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4(0, (int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2459 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2463 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2469 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2473 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2478 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2485 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2489 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2496 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2500 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2505 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2512 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2516 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2521 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2529 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2535 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2542 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2546 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2553 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2564 "lev_comp.y"
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

  case 401:

/* Line 1455 of yacc.c  */
#line 2577 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2581 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2587 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2591 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2598 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2602 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2606 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2612 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2616 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2622 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2628 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2632 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2638 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2644 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2649 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2655 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2659 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2665 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2669 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2685 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2693 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2703 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2712 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2717 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2724 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2735 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2741 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2745 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2749 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2756 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2766 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2773 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2777 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2783 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2787 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2795 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2799 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2806 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2816 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2829 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2833 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2840 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2850 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_LEVTYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2859 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_LEVTYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2870 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2874 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2881 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2891 "lev_comp.y"
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

  case 459:

/* Line 1455 of yacc.c  */
#line 2902 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2911 "lev_comp.y"
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

  case 461:

/* Line 1455 of yacc.c  */
#line 2921 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2927 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2931 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2938 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2948 "lev_comp.y"
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

  case 466:

/* Line 1455 of yacc.c  */
#line 2959 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2968 "lev_comp.y"
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

  case 468:

/* Line 1455 of yacc.c  */
#line 2978 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 2982 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2988 "lev_comp.y"
    { ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 2990 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 2996 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3000 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3004 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3008 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3016 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3025 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3029 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3033 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3037 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3041 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3044 "lev_comp.y"
    { ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3048 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3055 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3064 "lev_comp.y"
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

  case 490:

/* Line 1455 of yacc.c  */
#line 3105 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3109 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3116 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3123 "lev_comp.y"
    {
			      size_t len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3133 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3137 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3145 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3149 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3153 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3157 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3161 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3166 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3170 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3174 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3179 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3184 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3189 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3193 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3198 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3203 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3207 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3211 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3216 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3220 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3227 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 3233 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 516:

/* Line 1455 of yacc.c  */
#line 3237 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 517:

/* Line 1455 of yacc.c  */
#line 3243 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 523:

/* Line 1455 of yacc.c  */
#line 3259 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 524:

/* Line 1455 of yacc.c  */
#line 3263 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 525:

/* Line 1455 of yacc.c  */
#line 3267 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 526:

/* Line 1455 of yacc.c  */
#line 3271 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 535:

/* Line 1455 of yacc.c  */
#line 3293 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 536:

/* Line 1455 of yacc.c  */
#line 3297 "lev_comp.y"
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

  case 537:

/* Line 1455 of yacc.c  */
#line 3323 "lev_comp.y"
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
#line 7548 "lev_comp.tab.c"
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
#line 3351 "lev_comp.y"


/*lev_comp.y*/

