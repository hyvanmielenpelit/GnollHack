
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
     ELEMENTAL_ENCHANTMENT_TYPE = 366,
     EXCEPTIONALITY_TYPE = 367,
     EXCEPTIONALITY_ID = 368,
     ELEMENTAL_ENCHANTMENT_ID = 369,
     ENCHANTMENT_ID = 370,
     SECRET_DOOR_ID = 371,
     USES_UP_KEY_ID = 372,
     MYTHIC_PREFIX_TYPE = 373,
     MYTHIC_SUFFIX_TYPE = 374,
     MYTHIC_PREFIX_ID = 375,
     MYTHIC_SUFFIX_ID = 376,
     MATERIAL_ID = 377,
     MATERIAL_TYPE = 378,
     CHARGES_ID = 379,
     SPECIAL_QUALITY_ID = 380,
     SPEFLAGS_ID = 381,
     SUBROOM_ID = 382,
     NAME_ID = 383,
     FLAGS_ID = 384,
     FLAG_TYPE = 385,
     MON_ATTITUDE = 386,
     MON_ALERTNESS = 387,
     SUBTYPE_ID = 388,
     NON_PASSDOOR_ID = 389,
     CARPET_ID = 390,
     CARPET_PIECE_ID = 391,
     CARPET_TYPE = 392,
     MON_APPEARANCE = 393,
     ROOMDOOR_ID = 394,
     IF_ID = 395,
     ELSE_ID = 396,
     TERRAIN_ID = 397,
     HORIZ_OR_VERT = 398,
     REPLACE_TERRAIN_ID = 399,
     LOCATION_SUBTYPE_ID = 400,
     DOOR_SUBTYPE = 401,
     BRAZIER_SUBTYPE = 402,
     SIGNPOST_SUBTYPE = 403,
     TREE_SUBTYPE = 404,
     FOREST_ID = 405,
     FOREST_TYPE = 406,
     INITIALIZE_TYPE = 407,
     EXIT_ID = 408,
     SHUFFLE_ID = 409,
     MANUAL_TYPE_ID = 410,
     MANUAL_TYPE = 411,
     QUANTITY_ID = 412,
     BURIED_ID = 413,
     LOOP_ID = 414,
     FOR_ID = 415,
     TO_ID = 416,
     SWITCH_ID = 417,
     CASE_ID = 418,
     BREAK_ID = 419,
     DEFAULT_ID = 420,
     ERODED_ID = 421,
     TRAPPED_STATE = 422,
     RECHARGED_ID = 423,
     INVIS_ID = 424,
     GREASED_ID = 425,
     INDESTRUCTIBLE_ID = 426,
     FEMALE_ID = 427,
     MALE_ID = 428,
     WAITFORU_ID = 429,
     PROTECTOR_ID = 430,
     CANCELLED_ID = 431,
     REVIVED_ID = 432,
     AVENGE_ID = 433,
     FLEEING_ID = 434,
     BLINDED_ID = 435,
     MAXHP_ID = 436,
     LEVEL_ADJUSTMENT_ID = 437,
     KEEP_ORIGINAL_INVENTORY_ID = 438,
     PARALYZED_ID = 439,
     STUNNED_ID = 440,
     CONFUSED_ID = 441,
     SEENTRAPS_ID = 442,
     ALL_ID = 443,
     MONTYPE_ID = 444,
     OBJTYPE_ID = 445,
     TERTYPE_ID = 446,
     TERTYPE2_ID = 447,
     LEVER_EFFECT_TYPE = 448,
     SWITCHABLE_ID = 449,
     CONTINUOUSLY_USABLE_ID = 450,
     TARGET_ID = 451,
     TRAPTYPE_ID = 452,
     EFFECT_FLAG_ID = 453,
     GRAVE_ID = 454,
     BRAZIER_ID = 455,
     SIGNPOST_ID = 456,
     TREE_ID = 457,
     ERODEPROOF_ID = 458,
     FUNCTION_ID = 459,
     MSG_OUTPUT_TYPE = 460,
     COMPARE_TYPE = 461,
     UNKNOWN_TYPE = 462,
     rect_ID = 463,
     fillrect_ID = 464,
     line_ID = 465,
     randline_ID = 466,
     grow_ID = 467,
     selection_ID = 468,
     flood_ID = 469,
     rndcoord_ID = 470,
     circle_ID = 471,
     ellipse_ID = 472,
     filter_ID = 473,
     complement_ID = 474,
     gradient_ID = 475,
     GRADIENT_TYPE = 476,
     LIMITED = 477,
     HUMIDITY_TYPE = 478,
     STRING = 479,
     MAP_ID = 480,
     NQSTRING = 481,
     VARSTRING = 482,
     CFUNC = 483,
     CFUNC_INT = 484,
     CFUNC_STR = 485,
     CFUNC_COORD = 486,
     CFUNC_REGION = 487,
     VARSTRING_INT = 488,
     VARSTRING_INT_ARRAY = 489,
     VARSTRING_STRING = 490,
     VARSTRING_STRING_ARRAY = 491,
     VARSTRING_VAR = 492,
     VARSTRING_VAR_ARRAY = 493,
     VARSTRING_COORD = 494,
     VARSTRING_COORD_ARRAY = 495,
     VARSTRING_REGION = 496,
     VARSTRING_REGION_ARRAY = 497,
     VARSTRING_MAPCHAR = 498,
     VARSTRING_MAPCHAR_ARRAY = 499,
     VARSTRING_MONST = 500,
     VARSTRING_MONST_ARRAY = 501,
     VARSTRING_OBJ = 502,
     VARSTRING_OBJ_ARRAY = 503,
     VARSTRING_SEL = 504,
     VARSTRING_SEL_ARRAY = 505,
     METHOD_INT = 506,
     METHOD_INT_ARRAY = 507,
     METHOD_STRING = 508,
     METHOD_STRING_ARRAY = 509,
     METHOD_VAR = 510,
     METHOD_VAR_ARRAY = 511,
     METHOD_COORD = 512,
     METHOD_COORD_ARRAY = 513,
     METHOD_REGION = 514,
     METHOD_REGION_ARRAY = 515,
     METHOD_MAPCHAR = 516,
     METHOD_MAPCHAR_ARRAY = 517,
     METHOD_MONST = 518,
     METHOD_MONST_ARRAY = 519,
     METHOD_OBJ = 520,
     METHOD_OBJ_ARRAY = 521,
     METHOD_SEL = 522,
     METHOD_SEL_ARRAY = 523,
     DICE = 524
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
#line 569 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 581 "lev_comp.tab.c"

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
#define YYNTOKENS  287
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  193
/* YYNRULES -- Number of rules.  */
#define YYNRULES  534
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1197

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   524

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   282,   286,     2,
     226,   227,   280,   278,   224,   279,   284,   281,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   225,     2,
       2,   283,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   228,     2,   229,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   230,   285,   231,     2,     2,     2,     2,
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
     215,   216,   217,   218,   219,   220,   221,   222,   223,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277
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
    1331,  1339,  1347,  1353,  1361,  1367,  1377,  1385,  1391,  1401,
    1407,  1413,  1417,  1425,  1429,  1437,  1445,  1449,  1455,  1461,
    1465,  1471,  1479,  1489,  1491,  1493,  1495,  1497,  1499,  1500,
    1503,  1505,  1509,  1511,  1513,  1515,  1516,  1520,  1522,  1523,
    1527,  1529,  1530,  1534,  1535,  1539,  1540,  1544,  1546,  1548,
    1550,  1552,  1554,  1556,  1558,  1560,  1562,  1566,  1568,  1570,
    1575,  1577,  1579,  1584,  1586,  1588,  1593,  1595,  1600,  1606,
    1608,  1612,  1614,  1618,  1620,  1622,  1627,  1637,  1639,  1641,
    1646,  1648,  1654,  1656,  1658,  1663,  1665,  1667,  1673,  1675,
    1677,  1679,  1684,  1686,  1688,  1694,  1696,  1698,  1700,  1704,
    1706,  1708,  1712,  1714,  1719,  1723,  1727,  1731,  1735,  1739,
    1743,  1745,  1747,  1751,  1753,  1757,  1758,  1760,  1762,  1764,
    1766,  1770,  1771,  1773,  1775,  1778,  1781,  1786,  1793,  1798,
    1805,  1812,  1819,  1826,  1829,  1836,  1845,  1854,  1865,  1880,
    1883,  1885,  1889,  1891,  1895,  1897,  1899,  1901,  1903,  1905,
    1907,  1909,  1911,  1913,  1915,  1917,  1919,  1921,  1923,  1925,
    1927,  1929,  1931,  1933,  1944
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     288,     0,    -1,    -1,   289,    -1,   290,    -1,   290,   289,
      -1,   291,   301,   303,    -1,    17,   225,   232,    -1,    16,
     225,   232,   224,     3,    -1,    18,   225,    11,   224,   401,
      -1,    18,   225,    10,   224,     3,    -1,    18,   225,    13,
      -1,    18,   225,    12,   224,     3,   224,     3,   224,     5,
     224,     5,   224,   441,   224,   300,   299,    -1,    19,   225,
     471,    -1,   150,   225,   151,   224,   152,    -1,   150,   225,
     151,    -1,    32,   225,     4,   224,   313,    -1,    22,   225,
     103,    -1,    -1,   224,   222,    -1,    -1,   224,   448,    -1,
      -1,   224,     3,    -1,     5,    -1,    84,    -1,    -1,   129,
     225,   302,    -1,   130,   224,   302,    -1,   130,    -1,    -1,
     305,   303,    -1,   230,   303,   231,    -1,   346,    -1,   292,
      -1,   293,    -1,   409,    -1,   296,    -1,   294,    -1,   417,
      -1,   418,    -1,   419,    -1,   295,    -1,   416,    -1,   415,
      -1,   413,    -1,   414,    -1,   420,    -1,   421,    -1,   422,
      -1,   423,    -1,   424,    -1,   390,    -1,   348,    -1,   311,
      -1,   310,    -1,   404,    -1,   360,    -1,   382,    -1,   426,
      -1,   427,    -1,   392,    -1,   393,    -1,   394,    -1,   425,
      -1,   326,    -1,   336,    -1,   338,    -1,   342,    -1,   340,
      -1,   323,    -1,   333,    -1,   319,    -1,   322,    -1,   385,
      -1,   395,    -1,   367,    -1,   383,    -1,   370,    -1,   376,
      -1,   405,    -1,   400,    -1,   388,    -1,   347,    -1,   406,
      -1,   407,    -1,   408,    -1,   410,    -1,   353,    -1,   351,
      -1,   399,    -1,   403,    -1,   402,    -1,   386,    -1,   387,
      -1,   389,    -1,   381,    -1,   384,    -1,   242,    -1,   244,
      -1,   246,    -1,   248,    -1,   250,    -1,   252,    -1,   254,
      -1,   256,    -1,   258,    -1,   241,    -1,   243,    -1,   245,
      -1,   247,    -1,   249,    -1,   251,    -1,   253,    -1,   255,
      -1,   257,    -1,   306,    -1,   307,    -1,   235,    -1,   235,
      -1,   307,    -1,   154,   225,   306,    -1,   308,   283,   460,
      -1,   308,   283,   213,   225,   469,    -1,   308,   283,   459,
      -1,   308,   283,   476,   225,   453,    -1,   308,   283,   475,
     225,   455,    -1,   308,   283,   474,   225,   457,    -1,   308,
     283,   448,    -1,   308,   283,   451,    -1,   308,   283,   230,
     317,   231,    -1,   308,   283,   230,   316,   231,    -1,   308,
     283,   230,   315,   231,    -1,   308,   283,   476,   225,   230,
     314,   231,    -1,   308,   283,   475,   225,   230,   313,   231,
      -1,   308,   283,   474,   225,   230,   312,   231,    -1,   308,
     283,   230,   318,   231,    -1,   458,    -1,   312,   224,   458,
      -1,   456,    -1,   313,   224,   456,    -1,   454,    -1,   314,
     224,   454,    -1,   452,    -1,   315,   224,   452,    -1,   449,
      -1,   316,   224,   449,    -1,   460,    -1,   317,   224,   460,
      -1,   459,    -1,   318,   224,   459,    -1,    -1,    -1,   204,
     234,   226,   320,   464,   227,   321,   304,    -1,   234,   226,
     467,   227,    -1,   153,    -1,    -1,     6,    -1,     6,    -1,
     228,   460,   206,   460,   229,    -1,   228,   460,   229,    -1,
      -1,    -1,   162,   327,   228,   447,   229,   328,   230,   329,
     231,    -1,    -1,   330,   329,    -1,    -1,   163,   472,   225,
     331,   303,    -1,    -1,   165,   225,   332,   303,    -1,   164,
      -1,   284,   284,    -1,   161,    -1,   160,   309,   283,   460,
     334,   460,    -1,    -1,   335,   337,   304,    -1,    -1,   159,
     228,   447,   229,   339,   304,    -1,    -1,   325,   225,   341,
     305,    -1,    -1,   140,   325,   343,   344,    -1,   304,    -1,
      -1,   304,   345,   141,   304,    -1,    14,   225,   459,    -1,
      14,   225,   459,   224,    15,    -1,    74,    -1,    74,   225,
     472,    -1,    74,   225,    84,    -1,    62,   225,   349,   224,
     349,    -1,    62,   225,   349,   224,   472,    -1,   226,     4,
     224,    83,   224,   364,   227,    -1,   429,   324,   224,   441,
      -1,    -1,   127,   225,   350,   224,   357,   224,   359,   430,
     433,   435,   438,   439,   437,   352,   304,    -1,    -1,    54,
     225,   350,   224,   356,   224,   358,   224,   359,   430,   433,
     435,   438,   439,   437,   354,   304,    -1,    -1,   224,     5,
      -1,   226,     4,   224,     4,   227,    -1,    84,    -1,   226,
       4,   224,     4,   227,    -1,    84,    -1,   226,   368,   224,
     369,   227,    -1,    84,    -1,   226,     4,   224,     4,   227,
      -1,    84,    -1,   139,   225,   361,   224,   440,   224,   362,
     224,   364,   365,    -1,    30,   225,   440,   224,   469,   365,
      -1,     5,    -1,    84,    -1,   363,    -1,    84,    -1,    83,
      -1,    83,   285,   363,    -1,     4,    -1,    84,    -1,    -1,
     365,   224,   366,    -1,   145,   225,   146,    -1,    80,   225,
     457,    -1,   125,   225,     4,    -1,   171,    -1,   116,    -1,
     117,    -1,   134,    -1,    21,    -1,    20,   225,   368,   224,
     369,   355,   233,    -1,    20,   225,   448,   355,   233,    -1,
      88,    -1,    89,    -1,    90,    -1,    89,    -1,    28,   225,
     372,    -1,    -1,    28,   225,   372,   371,   304,    -1,   455,
     224,   448,   373,    -1,    -1,   373,   224,   374,    -1,   459,
      -1,   131,    -1,   132,    -1,   443,    -1,   138,   459,    -1,
     172,    -1,   173,    -1,   169,    -1,   176,    -1,   177,    -1,
     177,   225,   447,    -1,   178,    -1,   179,   225,   447,    -1,
     180,   225,   447,    -1,   184,   225,   447,    -1,   185,    -1,
     186,    -1,   187,   225,   375,    -1,   174,    -1,   175,    -1,
     181,    -1,   182,   225,   447,    -1,   183,    -1,   232,    -1,
     188,    -1,   232,   285,   375,    -1,    26,   225,   378,    -1,
      -1,    27,   225,   378,   377,   304,    -1,   457,   379,    -1,
      -1,   379,   224,   380,    -1,    77,    -1,   189,   225,   455,
      -1,   473,    -1,   128,   225,   459,    -1,   157,   225,   447,
      -1,   158,    -1,    76,    -1,   166,   225,   447,    -1,   203,
      -1,    75,    -1,   167,    -1,   168,   225,   447,    -1,   169,
      -1,   170,    -1,   448,    -1,   114,   225,   111,    -1,   113,
     225,   112,    -1,   115,   225,   447,    -1,   124,   225,   447,
      -1,   125,   225,   447,    -1,   155,   225,   156,    -1,   126,
     225,   447,    -1,    80,   225,   457,    -1,   171,    -1,   117,
      -1,    82,    -1,    78,    -1,   120,   225,   118,    -1,   121,
     225,   119,    -1,    61,   225,   447,    -1,   122,   225,   123,
      -1,    29,   225,   428,   224,   448,    -1,    31,   225,   448,
     224,    83,   224,   440,    -1,    36,   225,   448,   224,    83,
      -1,    36,   225,   448,   224,    83,   224,     5,   299,    -1,
      37,    -1,    37,   225,   469,    -1,    37,   225,   188,    -1,
      50,   225,   448,   224,    93,    -1,    51,   225,   448,   224,
      93,    -1,    51,   225,   478,   224,   478,   224,    93,    -1,
      56,   225,   478,   224,   478,   224,   232,    -1,    57,   225,
     478,   224,   478,   391,    -1,    58,   225,   478,   224,   478,
      -1,    -1,   224,    93,    -1,    65,   225,   469,   224,    97,
      -1,    66,   225,   469,    -1,    67,   225,   448,   224,   196,
     225,   448,   224,    95,   224,    94,    -1,    67,   225,   448,
     224,    68,   224,    69,   224,    70,   224,    95,   224,    94,
      -1,    81,   225,   448,   224,   193,   396,    -1,    -1,   396,
     224,   397,    -1,    94,    -1,   189,   225,   455,    -1,   190,
     225,   457,    -1,   191,   225,   401,    -1,   192,   225,   401,
      -1,   194,    -1,   195,    -1,   196,   225,   448,    -1,   197,
     225,   428,    -1,   145,   225,   398,    -1,   101,   225,   436,
      -1,   198,   225,     4,    -1,   125,   225,     4,    -1,     4,
      -1,    95,    -1,   146,    -1,    97,    -1,    72,   225,   469,
      -1,    71,   225,   469,    -1,     3,    -1,   226,     3,   224,
     441,   227,    -1,   144,   225,   451,   224,   453,   224,   453,
     224,     7,    -1,   142,   225,   469,   224,   453,    -1,   142,
     225,   469,   224,   453,   224,   100,    -1,    52,   225,   451,
      -1,    53,   225,   451,    -1,    42,   225,   232,   224,    43,
      -1,    42,   225,   232,   224,    43,   224,   189,   225,   455,
      -1,    39,   225,   451,   224,    41,    -1,    40,   225,   478,
     224,    41,    -1,    23,   225,   469,   224,    99,    -1,    -1,
      38,   225,   451,   224,   441,   224,   429,   430,   433,   435,
     438,   439,   437,   411,   412,    -1,    -1,   304,    -1,    47,
     225,   448,   224,   442,   224,   444,    -1,    47,   225,   448,
     224,   442,   224,   444,   224,   189,   225,   455,    -1,    47,
     225,   448,   224,   442,   224,   444,   224,    92,    -1,    47,
     225,   448,   224,   442,   224,   444,   224,    92,   224,   189,
     225,   455,    -1,    48,   225,   448,    -1,    48,   225,   448,
     224,   189,   225,   455,    -1,   105,   225,   448,   224,   106,
     224,     4,   224,   107,   224,   108,    -1,   105,   225,   448,
     224,   106,   224,     4,   224,   107,   224,   108,   224,   441,
      -1,   105,   225,   448,   224,   106,   224,   109,   224,   107,
      -1,   105,   225,   448,   224,   106,   224,   110,   224,   107,
      -1,   105,   225,   448,   224,   106,   224,   107,    -1,   102,
     225,   469,   224,   103,   224,   100,    -1,   133,   225,   469,
     224,     4,    -1,   133,   225,   469,   224,   103,   224,   100,
      -1,   135,   225,   451,   224,   137,    -1,   136,   225,   469,
     224,   137,   224,     4,   224,     4,    -1,   136,   225,   469,
     224,   137,   224,     4,    -1,    49,   225,    96,   224,   448,
      -1,    49,   225,    96,   224,   448,   224,   189,   225,   455,
      -1,   199,   225,   448,   224,   459,    -1,   199,   225,   448,
     224,    84,    -1,   199,   225,   448,    -1,   200,   225,   448,
     224,   147,   224,   441,    -1,   200,   225,   448,    -1,   201,
     225,   448,   224,   148,   224,   459,    -1,   201,   225,   448,
     224,   148,   224,    84,    -1,   201,   225,   448,    -1,   202,
     225,   448,   224,   149,    -1,   202,   225,   448,   224,   151,
      -1,   202,   225,   448,    -1,    63,   225,   460,   224,   448,
      -1,    64,   225,   448,   224,   477,   224,   459,    -1,    60,
     225,   447,   224,   447,   224,   447,   224,   447,    -1,    60,
      -1,   232,    -1,    84,    -1,   232,    -1,    84,    -1,    -1,
     224,   431,    -1,   432,    -1,   432,   224,   431,    -1,    44,
      -1,    45,    -1,    46,    -1,    -1,   104,   225,   434,    -1,
     103,    -1,    -1,   101,   225,   436,    -1,   100,    -1,    -1,
     189,   225,   455,    -1,    -1,    24,   225,    99,    -1,    -1,
      25,   225,     4,    -1,    75,    -1,    84,    -1,    76,    -1,
      84,    -1,    87,    -1,   445,    -1,    84,    -1,    87,    -1,
     445,    -1,    86,   225,    84,    -1,    91,    -1,    84,    -1,
      86,   228,     4,   229,    -1,   232,    -1,   243,    -1,   244,
     228,   460,   229,    -1,   460,    -1,   449,    -1,   215,   226,
     469,   227,    -1,   247,    -1,   248,   228,   460,   229,    -1,
     226,     4,   224,     4,   227,    -1,    84,    -1,    85,   450,
     229,    -1,   223,    -1,   223,   224,   450,    -1,   452,    -1,
     249,    -1,   250,   228,   460,   229,    -1,   226,     4,   224,
       4,   224,     4,   224,     4,   227,    -1,   454,    -1,   251,
      -1,   252,   228,   460,   229,    -1,     3,    -1,   226,     3,
     224,   441,   227,    -1,   456,    -1,   253,    -1,   254,   228,
     460,   229,    -1,   232,    -1,     3,    -1,   226,     3,   224,
     232,   227,    -1,    84,    -1,   458,    -1,   255,    -1,   256,
     228,   460,   229,    -1,   232,    -1,     3,    -1,   226,     3,
     224,   232,   227,    -1,    84,    -1,    98,    -1,   446,    -1,
     459,   284,   446,    -1,     4,    -1,   470,    -1,   226,     8,
     227,    -1,   241,    -1,   242,   228,   460,   229,    -1,   460,
     278,   460,    -1,   460,   279,   460,    -1,   460,   280,   460,
      -1,   460,   281,   460,    -1,   460,   282,   460,    -1,   226,
     460,   227,    -1,   237,    -1,   238,    -1,   308,   225,   461,
      -1,   462,    -1,   463,   224,   462,    -1,    -1,   463,    -1,
     460,    -1,   459,    -1,   465,    -1,   466,   224,   465,    -1,
      -1,   466,    -1,   448,    -1,   208,   451,    -1,   209,   451,
      -1,   210,   448,   224,   448,    -1,   211,   448,   224,   448,
     224,   460,    -1,   212,   226,   469,   227,    -1,   212,   226,
     363,   224,   469,   227,    -1,   218,   226,     7,   224,   469,
     227,    -1,   218,   226,   469,   224,   469,   227,    -1,   218,
     226,   453,   224,   469,   227,    -1,   214,   448,    -1,   216,
     226,   448,   224,   460,   227,    -1,   216,   226,   448,   224,
     460,   224,    44,   227,    -1,   217,   226,   448,   224,   460,
     224,   460,   227,    -1,   217,   226,   448,   224,   460,   224,
     460,   224,    44,   227,    -1,   220,   226,   221,   224,   226,
     460,   279,   460,   297,   227,   224,   448,   298,   227,    -1,
     219,   468,    -1,   257,    -1,   226,   469,   227,    -1,   468,
      -1,   468,   286,   469,    -1,   277,    -1,    99,    -1,     4,
      -1,     8,    -1,     9,    -1,     4,    -1,     8,    -1,     9,
      -1,     4,    -1,   470,    -1,    33,    -1,    26,    -1,    34,
      -1,    28,    -1,    35,    -1,   142,    -1,    79,    -1,    84,
      -1,   479,    -1,    59,   226,     4,   224,     4,   224,     4,
     224,     4,   227,    -1,   226,     4,   224,     4,   224,     4,
     224,     4,   227,    -1
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
    2389,  2395,  2401,  2405,  2411,  2417,  2421,  2427,  2431,  2437,
    2441,  2446,  2453,  2457,  2464,  2468,  2473,  2480,  2484,  2489,
    2497,  2503,  2510,  2514,  2521,  2529,  2532,  2542,  2546,  2549,
    2555,  2559,  2566,  2570,  2574,  2581,  2584,  2590,  2597,  2600,
    2606,  2613,  2617,  2624,  2627,  2634,  2637,  2643,  2644,  2647,
    2648,  2651,  2652,  2653,  2659,  2660,  2661,  2667,  2668,  2671,
    2680,  2685,  2692,  2703,  2709,  2713,  2717,  2724,  2734,  2741,
    2745,  2751,  2755,  2763,  2767,  2774,  2784,  2797,  2801,  2808,
    2818,  2827,  2838,  2842,  2849,  2859,  2870,  2879,  2889,  2895,
    2899,  2906,  2916,  2927,  2936,  2946,  2950,  2957,  2958,  2964,
    2968,  2972,  2976,  2984,  2993,  2997,  3001,  3005,  3009,  3013,
    3016,  3023,  3032,  3065,  3066,  3069,  3070,  3073,  3077,  3084,
    3091,  3102,  3105,  3113,  3117,  3121,  3125,  3129,  3134,  3138,
    3142,  3147,  3152,  3157,  3161,  3166,  3171,  3175,  3179,  3184,
    3188,  3195,  3201,  3205,  3211,  3218,  3219,  3222,  3223,  3224,
    3227,  3231,  3235,  3239,  3245,  3246,  3249,  3250,  3253,  3254,
    3257,  3258,  3261,  3265,  3291
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
  "BANNER_TYPE", "ELEMENTAL_ENCHANTMENT_TYPE", "EXCEPTIONALITY_TYPE",
  "EXCEPTIONALITY_ID", "ELEMENTAL_ENCHANTMENT_ID", "ENCHANTMENT_ID",
  "SECRET_DOOR_ID", "USES_UP_KEY_ID", "MYTHIC_PREFIX_TYPE",
  "MYTHIC_SUFFIX_TYPE", "MYTHIC_PREFIX_ID", "MYTHIC_SUFFIX_ID",
  "MATERIAL_ID", "MATERIAL_TYPE", "CHARGES_ID", "SPECIAL_QUALITY_ID",
  "SPEFLAGS_ID", "SUBROOM_ID", "NAME_ID", "FLAGS_ID", "FLAG_TYPE",
  "MON_ATTITUDE", "MON_ALERTNESS", "SUBTYPE_ID", "NON_PASSDOOR_ID",
  "CARPET_ID", "CARPET_PIECE_ID", "CARPET_TYPE", "MON_APPEARANCE",
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
     475,   476,   477,   478,    44,    58,    40,    41,    91,    93,
     123,   125,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,   523,   524,    43,    45,
      42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   287,   288,   288,   289,   289,   290,   291,   291,   292,
     292,   292,   292,   293,   294,   294,   295,   296,   297,   297,
     298,   298,   299,   299,   300,   300,   301,   301,   302,   302,
     303,   303,   304,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   308,   308,   308,   309,
     309,   310,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   312,   312,   313,
     313,   314,   314,   315,   315,   316,   316,   317,   317,   318,
     318,   320,   321,   319,   322,   323,   324,   324,   325,   325,
     325,   327,   328,   326,   329,   329,   331,   330,   332,   330,
     333,   334,   334,   335,   337,   336,   339,   338,   341,   340,
     343,   342,   344,   345,   344,   346,   346,   347,   347,   347,
     348,   348,   349,   350,   352,   351,   354,   353,   355,   355,
     356,   356,   357,   357,   358,   358,   359,   359,   360,   360,
     361,   361,   362,   362,   363,   363,   364,   364,   365,   365,
     366,   366,   366,   366,   366,   366,   366,   367,   367,   367,
     368,   368,   369,   369,   370,   371,   370,   372,   373,   373,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   375,   375,   375,   376,   377,   376,   378,
     379,   379,   380,   380,   380,   380,   380,   380,   380,   380,
     380,   380,   380,   380,   380,   380,   380,   380,   380,   380,
     380,   380,   380,   380,   380,   380,   380,   380,   380,   380,
     380,   380,   380,   381,   382,   383,   383,   384,   384,   384,
     385,   386,   387,   388,   389,   390,   391,   391,   392,   393,
     394,   394,   395,   396,   396,   397,   397,   397,   397,   397,
     397,   397,   397,   397,   397,   397,   397,   397,   398,   398,
     398,   398,   399,   400,   401,   401,   402,   403,   403,   404,
     405,   406,   406,   407,   408,   409,   411,   410,   412,   412,
     413,   413,   413,   413,   414,   414,   415,   415,   415,   415,
     415,   416,   417,   417,   418,   419,   419,   420,   420,   421,
     421,   421,   422,   422,   423,   423,   423,   424,   424,   424,
     425,   426,   427,   427,   428,   428,   429,   429,   430,   430,
     431,   431,   432,   432,   432,   433,   433,   434,   435,   435,
     436,   437,   437,   438,   438,   439,   439,   440,   440,   441,
     441,   442,   442,   442,   443,   443,   443,   444,   444,   445,
     446,   446,   446,   447,   448,   448,   448,   448,   449,   449,
     449,   450,   450,   451,   451,   451,   452,   453,   453,   453,
     454,   454,   455,   455,   455,   456,   456,   456,   456,   457,
     457,   457,   458,   458,   458,   458,   458,   459,   459,   460,
     460,   460,   460,   460,   460,   460,   460,   460,   460,   460,
     461,   461,   462,   463,   463,   464,   464,   465,   465,   466,
     466,   467,   467,   468,   468,   468,   468,   468,   468,   468,
     468,   468,   468,   468,   468,   468,   468,   468,   468,   468,
     468,   468,   469,   469,   470,   471,   471,   472,   472,   472,
     473,   473,   473,   473,   474,   474,   475,   475,   476,   476,
     477,   477,   478,   478,   479
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
       7,     7,     5,     7,     5,     9,     7,     5,     9,     5,
       5,     3,     7,     3,     7,     7,     3,     5,     5,     3,
       5,     7,     9,     1,     1,     1,     1,     1,     0,     2,
       1,     3,     1,     1,     1,     0,     3,     1,     0,     3,
       1,     0,     3,     0,     3,     0,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     4,
       1,     1,     4,     1,     1,     4,     1,     4,     5,     1,
       3,     1,     3,     1,     1,     4,     9,     1,     1,     4,
       1,     5,     1,     1,     4,     1,     1,     5,     1,     1,
       1,     4,     1,     1,     5,     1,     1,     1,     3,     1,
       1,     3,     1,     4,     3,     3,     3,     3,     3,     3,
       1,     1,     3,     1,     3,     0,     1,     1,     1,     1,
       3,     0,     1,     1,     2,     2,     4,     6,     4,     6,
       6,     6,     6,     2,     6,     8,     8,    10,    14,     2,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,    10,     9
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
       0,     0,     0,     0,     0,     0,     0,     0,   393,     0,
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
     469,     0,   472,     0,   514,     0,   470,   491,    31,     0,
     178,     0,     8,     0,   430,   431,     0,   467,   185,     0,
       0,     0,    11,   516,   515,    13,   439,     0,   230,   231,
       0,     0,   436,     0,     0,   198,   434,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     510,   493,   512,     0,   463,   465,   466,     0,   462,   460,
       0,   266,   270,   459,   267,   456,   458,     0,   455,   453,
       0,   234,     0,   452,   395,   394,     0,   417,   418,     0,
       0,     0,     0,   309,   308,     0,   444,     0,     0,   443,
       0,     0,     0,     0,   532,     0,     0,   364,     0,     0,
       0,     0,     0,   349,   350,   397,   396,     0,   156,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,   319,
       0,   343,   342,   519,   517,   518,   189,   188,     0,     0,
       0,     0,     0,     0,     0,   210,   211,     0,     0,     0,
       0,    15,   121,     0,     0,     0,   381,   383,   386,   389,
     151,     0,     0,     0,     0,   160,     0,     0,     0,     0,
       0,   488,   487,   489,   492,     0,   525,   527,   524,   526,
     528,   529,     0,     0,     0,   128,   129,   124,   122,     0,
       0,     0,     0,    30,   175,    28,     0,     0,     0,     0,
       0,     0,   441,     0,     0,     0,     0,     0,     0,     0,
     494,   495,     0,     0,     0,   503,     0,     0,     0,   509,
       0,     0,     0,     0,     0,     0,   269,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   157,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   182,   181,     0,     0,     0,   176,     0,
       0,     0,     0,     0,     0,   485,   471,   479,     0,     0,
     474,   475,   476,   477,   478,     0,   154,     0,   469,     0,
       0,     0,     0,   145,   143,   149,   147,     0,     0,     0,
     179,     0,     0,   186,   468,    10,   344,     0,     9,     0,
       0,   440,     0,     0,     0,   233,   232,   198,   199,   229,
       0,     0,   214,     0,     0,     0,     0,   450,     0,     0,
     448,     0,     0,   447,     0,     0,   511,   513,   355,     0,
       0,     0,   268,     0,     0,   236,   238,   303,   218,     0,
      16,   139,   305,     0,     0,   419,   420,     0,   353,     0,
       0,   354,   351,   423,     0,   421,     0,   422,     0,   377,
     310,     0,   311,     0,   201,     0,     0,     0,     0,   316,
     315,     0,     0,   190,   191,   390,   530,   531,     0,   318,
       0,     0,   323,     0,     0,   203,     0,     0,   372,     0,
     374,     0,     0,     0,     0,   347,     0,    14,     0,   172,
       0,     0,   162,   380,   379,     0,     0,   387,   388,     0,
     483,   486,     0,   473,   159,   490,   123,     0,     0,   132,
       0,   131,     0,   130,     0,   136,     0,   127,     0,   126,
       0,   125,    32,   432,     0,     0,   442,   435,     0,   437,
       0,   496,     0,     0,     0,   498,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   461,   522,   520,   521,     0,
     281,   278,   272,   298,     0,   297,     0,     0,     0,   296,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,   282,     0,   284,   285,   295,     0,   280,   271,   286,
     523,   274,     0,   454,   237,   209,     0,     0,     0,     0,
     445,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   193,     0,     0,   314,     0,     0,     0,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   177,   171,   173,     0,     0,     0,     0,
       0,   152,     0,   144,   146,   148,   150,     0,   137,     0,
       0,   141,     0,     0,   438,   228,     0,   215,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   464,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   457,     0,     0,   304,   140,
      22,     0,   398,     0,     0,     0,     0,   428,   427,   360,
     365,     0,   312,     0,   205,     0,     0,   313,   317,     0,
       0,   391,     0,     0,     0,   371,     0,   370,     0,     0,
       0,   207,     0,   398,   373,   376,   213,     0,   212,   184,
     348,     0,   164,   382,   385,   384,   480,   481,   482,   484,
       0,     0,   135,   134,     0,   133,     0,     0,   497,   499,
       0,   504,     0,   500,     0,   449,   502,   501,     0,   301,
     294,   288,   287,   289,   299,   300,   302,   290,   291,   293,
     275,   292,   276,   279,   283,   273,     0,   424,   241,   242,
       0,   247,   245,   246,   258,   259,   248,   249,   251,     0,
       0,   260,     0,   262,     0,   255,   256,     0,   239,   243,
     425,   240,     0,   224,   225,     0,   226,     0,   223,   219,
       0,   306,     0,     0,   405,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,   216,   217,     0,     0,     0,
     325,     0,     0,     0,     0,     0,     0,     0,   330,   331,
       0,     0,     0,   324,     0,     0,     0,     0,     0,   405,
       0,     0,     0,     0,     0,     0,   164,   153,   138,   142,
     345,     0,     0,     0,   451,     0,     0,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    23,     0,   402,
     403,   404,   399,   400,     0,   408,     0,     0,   352,   362,
       0,   378,   200,     0,   398,   392,   192,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     368,   369,   202,     0,   408,   375,   218,   346,     0,   168,
     163,   165,     0,   505,     0,   506,   475,   426,   250,   252,
     253,   261,   254,   264,   263,   257,   221,   222,   220,     0,
       0,     0,     0,   413,     0,     0,     0,     0,     0,   405,
       0,     0,   410,   335,   337,   338,   339,   341,   340,   334,
     326,   327,   328,   329,   332,   333,   336,     0,     0,   413,
     208,   166,    30,     0,     0,     0,     0,     0,   446,   401,
     407,   406,     0,     0,   415,     0,   534,     0,   361,   204,
     408,     0,   320,   366,   206,   415,    30,   169,     0,   507,
      19,     0,   265,   409,     0,     0,   411,   533,     0,   413,
       0,     0,   411,   167,     0,     0,   414,     0,     0,   356,
     363,   415,   321,   367,   194,     0,    20,   416,     0,   358,
     411,     0,    24,    25,    22,     0,     0,   412,   359,   357,
     196,   195,    12,    21,   508,     0,   197
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,   102,   103,   104,   105,   106,
    1126,  1186,   961,  1184,    12,   176,   107,   424,   108,   109,
     110,   111,   233,   112,   113,   797,   590,   800,   529,   530,
     531,   532,   114,   515,   890,   115,   116,   484,   117,   118,
     234,   786,  1005,  1006,  1146,  1122,   119,   651,   120,   121,
     251,   122,   648,   123,   422,   124,   378,   504,   643,   125,
     126,   127,   355,   347,   128,  1181,   129,  1195,   439,   616,
     637,   856,   873,   130,   377,   877,   563,   977,   745,   959,
     131,   274,   557,   132,   460,   311,   744,   948,  1085,   133,
     457,   301,   456,   738,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   766,   144,   145,   146,   147,   772,
     993,  1109,   148,   149,   548,   150,   151,   152,   153,   154,
     155,   156,   157,   158,  1179,  1189,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   316,   348,   964,  1032,  1033,  1035,  1131,  1093,  1103,
    1169,  1134,  1156,   319,   597,   606,   949,   849,   607,   257,
     352,   291,   276,   433,   328,   329,   572,   573,   312,   313,
     302,   303,   401,   353,   888,   660,   661,   662,   403,   404,
     405,   292,   451,   246,   265,   367,   741,   419,   420,   421,
     628,   333,   334
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1057
static const yytype_int16 yypact[] =
{
     111,  -111,   -27,   142, -1057,   111,    47,   -28,    18, -1057,
   -1057,    35,   818,    -9, -1057,   135, -1057,    71,   104,   108,
     133, -1057,   148,   169,   176,   191,   201,   218,   225,   229,
     231,   241,   247,   261,   264,   279,   280,   284,   287,   290,
     292,   293,   294,   297,   300,   302,   303,   307,   308,   309,
     313,   321,   324,   325,   331,   335,   347,   348,   349,   356,
     364,   365,   377,   378,   387,   388,    24,   393,   394,   398,
   -1057,   401,    91,   756, -1057, -1057,   407,   411,   414,   416,
     107,    51,   160, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,   818, -1057,
   -1057,   253, -1057, -1057, -1057, -1057, -1057,   417, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057,   590,   421, -1057,  -122,   597,    49,
     311,   543,   -24,    25,    25,   161,   -55,    36,   305,   655,
     305,   699,  -113,  -113,   -16,   428,   305,   305,   565,   305,
     396,  -113,  -113,   -51,   -16,   -16,   -16,    51,   436,    51,
     305,   -24,   -24,   305,   -24,   -24,    99,   305,   -24,   305,
     -51,   -24,  -113,   -24,    77, -1057,   -24,  -113,   513,   879,
      51, -1057, -1057,   386,   443,   305,   305,   305,   305,   446,
   -1057,    58, -1057,   448, -1057,   434, -1057,    32, -1057,   121,
   -1057,   450, -1057,   135, -1057, -1057,   465, -1057,  -139,   478,
     479,   481, -1057, -1057, -1057, -1057, -1057,   484, -1057, -1057,
     483,   700, -1057,   489,   494,   502, -1057, -1057,  -113,  -113,
     305,   305,   503,   305,   512,   515,   517,   -24,   518,   368,
   -1057, -1057,   459,   522, -1057, -1057, -1057,   736, -1057, -1057,
     520, -1057, -1057, -1057, -1057, -1057, -1057,   746, -1057, -1057,
     527,   530,   534, -1057, -1057, -1057,   537, -1057, -1057,   539,
     540,   541,   542, -1057, -1057,   763, -1057,   544,   545, -1057,
     546,   547,   767,   556, -1057,   557,   558,   566,   572,   573,
     794,   575,   578, -1057, -1057, -1057, -1057,   580,   810,   593,
     594,   596,   598,   150,   817,   599,   263,   601,   603, -1057,
     604, -1057, -1057, -1057, -1057, -1057, -1057, -1057,   605,   607,
     609,   610,   611,   618,   619, -1057, -1057,   628,   450,   629,
     635,   637, -1057,   633,    51,    51,   639,   640,   649,   662,
   -1057,   652,   370,    51,    51, -1057,    51,    51,    51,    51,
      51,   612,   150, -1057,   664,   666, -1057, -1057, -1057, -1057,
   -1057, -1057,   669,    69,    12, -1057, -1057,   612,   150,   670,
     672,   673,   818,   818, -1057, -1057,    51,   876,  -122,   902,
      21,   903,   688,   692,   -24,   698,    51,   330,   919,   693,
   -1057, -1057,   703,   705,   480, -1057,   305,   305,   259, -1057,
     709,   704,   -24,   833,   710,    51,   712,   450,   713,    51,
     450,   305,   305,   -24,   850,   116,   855,   716,    51,    54,
     900,   938,   719,   907,   901,   351,   760,   305,   857,   728,
     868,   -16,    -4, -1057,   731,   -16,   -16,   -16,    51,   739,
      33,   305,   128,   867,   -29,   772,   863,   861,     9,    48,
     832,   836,    36,   829, -1057,   123,   123,   822, -1057,   143,
     747,   -15,   828,   835,    90,   862, -1057, -1057,   405,   453,
     183,   183, -1057, -1057, -1057,    32, -1057,   -24,   757,   -65,
     -61,   -49,   -23, -1057, -1057,   612,   150,   129,    16,   126,
   -1057,   753,   472, -1057, -1057, -1057, -1057,   982, -1057,   764,
     484, -1057,   762,   983,   496, -1057, -1057,   502, -1057, -1057,
     305,   305,   707,   766,   771,   769,   770, -1057,   776,   282,
   -1057,   774,   780, -1057,   784,   786, -1057, -1057, -1057,   783,
     507,   553, -1057,   789,   528, -1057, -1057, -1057, -1057,   788,
     792, -1057,   803,  1029,   533, -1057, -1057,   811, -1057,   812,
    1030, -1057,   813, -1057,   814, -1057,   815, -1057,   819,   821,
   -1057,  1039, -1057,   824, -1057,  1045,   826,    54,   827,   830,
   -1057,   831,   973, -1057, -1057, -1057, -1057, -1057,   853, -1057,
     856,   854, -1057,   874,   875, -1057,  1053,   877, -1057,   878,
   -1057,   898,   904,   917,  1097,   906,   908, -1057,   450, -1057,
     840,    51, -1057, -1057,   612,   910,   912, -1057, -1057,   913,
   -1057,   915,   899, -1057, -1057, -1057, -1057,  1136,   916, -1057,
       4, -1057,    51, -1057,  -122, -1057,   113, -1057,   116, -1057,
      23, -1057, -1057, -1057,   920,  1140, -1057, -1057,   918, -1057,
     914, -1057,   922,  1065,   -24, -1057,    51,    51,   -24,   925,
      51,   -24,   -24,   924,   927, -1057, -1057, -1057, -1057,   926,
   -1057, -1057, -1057, -1057,   930, -1057,   931,   932,   933, -1057,
     934,   935,   936,   937,   939,   940,   941,   942,   943, -1057,
     944, -1057,   945, -1057, -1057, -1057,   946, -1057, -1057, -1057,
   -1057, -1057,   947, -1057,   948,   949,    36,   116,  1158,   951,
   -1057,   -51,  1172,   953,   989,  1175,   125,   161,   991,  -109,
    1088,   958,    10, -1057,   952,  1090, -1057,    51,   961,  -122,
    1117,   305,   963,  1089,    43,   964,    14,  1091,  1186,    96,
     450,  1092,   123, -1057, -1057,   150,   965,    54,   -12,   -19,
     862, -1057,   -83, -1057, -1057,   150,   612,   -10, -1057,    82,
      83, -1057,    54,   969, -1057, -1057,    51, -1057,   967,   130,
     289,   970,    54,   622,   971,   972,    51, -1057,    51,    25,
    1084,  1093,    51,  1082,  1083,  1078,    51,    51,    51,  -122,
    1047,    51,    51,    51,   161, -1057,   909,   317, -1057, -1057,
     981,  1202,   984,   985,  1203,   986,   987, -1057, -1057,   988,
   -1057,   990, -1057,  1206, -1057,   352,   993, -1057, -1057,   994,
      59,   612,   995,   996,   834, -1057,   997, -1057,   998,   999,
    1209, -1057,  1210,   984, -1057,  1000, -1057,  1001, -1057, -1057,
   -1057,  1002,   242, -1057, -1057,   612, -1057, -1057, -1057, -1057,
     450,   113, -1057, -1057,    23, -1057,  1003,  1222,   150, -1057,
    1184, -1057,    51, -1057,  1004, -1057, -1057, -1057,   375, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
     612, -1057, -1057, -1057, -1057, -1057,   -68, -1057, -1057, -1057,
    -122, -1057, -1057, -1057, -1057, -1057, -1057,  1007, -1057,  1008,
    1009, -1057,  1010, -1057,  1011, -1057, -1057,  1012, -1057, -1057,
   -1057,   612,  1013, -1057, -1057,  1014, -1057,  1015, -1057, -1057,
    1226, -1057,  1017,   438,  1138,  1239,  1020,   161, -1057,   -17,
     161,  1018,  1022,    14,    51, -1057, -1057,  1021,  1177,  1154,
   -1057,  1025,  1026,  1027,  1028,  1031,  1032,  1033, -1057, -1057,
    1034,  1035,  1036, -1057,  1147,  1148,  1155,  1037,  1041,  1138,
    1259,    59,  1260,   409,  1043,  1038,   242, -1057, -1057, -1057,
   -1057,  1042,  1044,   179, -1057,    51,  1188,   612,    51,    51,
      51,    51,    51,   -98,    25,  1266,  1127, -1057,  1270, -1057,
   -1057, -1057, -1057,  1051,  1052,  1178,  1054,  1272, -1057,  1056,
    1057, -1057, -1057,   330,   984, -1057, -1057,  1059,  1060,  1181,
    1281,   230,   161,    25,    21,    21,   305,   -55,  1282,  1063,
   -1057, -1057, -1057,  1284,  1178, -1057, -1057, -1057,  1064, -1057,
   -1057, -1057,  1285, -1057,  1247, -1057,   164, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057,  1016, -1057, -1057, -1057, -1057,  1066,
     438,  1189,  1069,  1271,  1292,  1070,  1109,   161,  1072,  1138,
    1205,  1208, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057,  1195,  1077,  1271,
     949, -1057,   818,  1081,  1079,  1085,  1086,   -98, -1057, -1057,
   -1057, -1057,  1181,  1087,  1283,  1094, -1057,  1095, -1057, -1057,
    1178,  1098, -1057,  1099, -1057,  1283,   818, -1057,    54, -1057,
   -1057,  1100, -1057, -1057,  1211,  1101,  1120, -1057,   161,  1271,
    1217,    54,  1120, -1057,  1103,   305, -1057,  1310,  1104, -1057,
   -1057,  1283, -1057, -1057, -1057,    94,  1106, -1057,   161,   450,
    1120,   450, -1057, -1057,   981,   305,  1105, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057,   450, -1057
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1057, -1057,  1311, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057,   131, -1057, -1057,  1075,  -108,  -376,   895,  1096,
    1245,  -502, -1057, -1057, -1057, -1057,   641, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,  1265, -1057,
   -1057, -1057,   327, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057,   844,  1115, -1057, -1057, -1057, -1057,   779, -1057,
   -1057, -1057,   366, -1057, -1057, -1057,  -639,   336,   272, -1057,
   -1057,   485,   298, -1057, -1057, -1057, -1057, -1057,   215, -1057,
   -1057,  1159, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057,  -607, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057, -1057,
   -1057,   288,   595,  -841,   254, -1057,  -953, -1057,  -978,   216,
   -1056, -1024, -1040,  -475,  -613, -1057, -1057, -1057,   511,   921,
    -227,  -179,  -392,   800,  -110,  -389,  -491,  -657,  -531,  -453,
    -529,  -589,  -171,   -76, -1057,   561, -1057, -1057,   837, -1057,
   -1057,  1067,  -147,   775, -1057,  -480, -1057, -1057, -1057, -1057,
   -1057,  -155, -1057
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -236
static const yytype_int16 yytable[] =
{
     248,   275,   503,   383,   763,   245,   258,   679,   677,   320,
     624,   322,   591,   659,   645,   646,   240,   336,   337,   305,
     339,   341,   533,   801,   546,   534,   567,   642,   294,   314,
      16,   357,   999,   345,   360,   293,   240,   363,   368,   630,
     370,   364,   365,   331,   324,   342,  1064,   866,   681,   349,
     350,   351,   638,   263,   807,   240,   386,   387,   388,   389,
     266,   267,   240,   975,   358,   359,   391,   361,   362,   653,
     415,   369,   884,   528,   372,  1039,   374,   391,   417,   379,
     614,   582,   375,   330,   585,   427,  1119,   798,   266,   267,
    1083,   343,   344,   635,   854,  1145,   266,   267,   871,  1182,
     306,   442,   443,   363,   445,  1162,  1174,   364,   365,   295,
     254,   317,   373,   325,     7,   844,   294,   380,   804,   305,
     318,   255,   256,   296,  1190,   240,   567,     1,     2,   567,
     595,  1180,   294,   356,  1084,  1171,   326,   327,   596,   416,
     878,   841,     9,   976,   804,   428,  1140,   406,   264,   407,
     867,   639,   868,   869,   408,   409,   410,  1016,   510,   668,
     755,   376,  1159,   670,   305,   392,   669,   631,   440,   441,
     671,   402,  1040,   418,   883,   672,    11,   315,  1183,   562,
     876,   346,   673,   366,   278,   279,   280,   281,   282,   896,
     283,   270,   284,   285,   286,   287,   288,   295,     8,   904,
     306,   674,   289,  1099,    13,   266,   267,   626,   675,   847,
     332,   296,   627,   295,   891,   174,   848,   254,   886,   887,
     254,   892,   615,   272,   273,   591,   850,   296,   255,   256,
     271,   255,   256,   290,  1105,   636,   855,  1009,   413,   657,
     872,   658,   307,   535,   254,   306,   678,   547,   308,   644,
      14,   297,    81,   242,   243,   255,   256,   298,   241,   354,
      15,   621,   567,   411,   254,   175,   568,   565,   566,   309,
     310,   838,   783,   242,   243,   255,   256,   241,   794,   793,
     299,   300,   586,   587,   241,   699,   435,   552,   659,   244,
     910,   881,   242,   243,   839,   241,   177,   564,   609,   242,
     243,   574,  1008,   925,   649,   577,   747,   894,   509,   244,
     242,   243,   625,   893,   895,   541,   588,   518,   519,   230,
     520,   521,   522,   523,   524,  1106,   613,  1107,   244,   178,
     618,   619,   620,   179,   412,   244,   270,   392,   536,   297,
     654,   239,   307,   266,   267,   298,   244,   413,   308,   644,
     542,   414,   644,   254,   900,   297,   680,   901,   180,   676,
     554,   298,   242,   243,   255,   256,   266,   267,   272,   273,
     326,   327,   435,   181,   570,   571,  1108,   570,   571,   580,
     666,   691,   692,   584,   299,   300,   247,   307,  1125,   266,
     267,   -18,   594,   308,   182,   266,   267,   952,   244,   268,
     269,   183,   739,  1074,   879,  1003,  1075,  1004,   396,   397,
     398,   399,   400,   363,   309,   310,   184,   364,   365,   555,
     556,   396,   397,   398,   399,   400,   185,   650,   396,   397,
     398,   399,   400,   953,   954,   603,  1038,   604,   605,  1041,
     268,   269,   955,   186,   398,   399,   400,  1112,  1113,   402,
     187,   956,   266,   267,   188,   331,   189,   396,   397,   398,
     399,   400,   957,   398,   399,   400,   190,   278,   279,   280,
     281,   282,   191,   283,   270,   284,   285,   286,   287,   288,
     266,   267,  1029,  1030,  1031,   569,   192,   491,   958,   193,
     278,   279,   280,   281,   282,  1086,   283,   270,   284,   285,
     286,   287,   288,   796,   194,   195,   272,   273,   289,   196,
     570,   571,   197,   902,  1007,   198,   290,   199,   200,   201,
     270,  1110,   202,  1068,  1111,   203,   270,   204,   205,   272,
     273,   271,   206,   207,   208,  1164,   249,   271,   209,   290,
     859,   396,   397,   398,   399,   400,   210,   808,  1173,   211,
     212,   811,   272,   273,   814,   815,   213,   706,   272,   273,
     214,   707,   708,   562,   266,   267,  1138,   396,   397,   398,
     399,   400,   215,   216,   217,   785,   278,   279,   280,   281,
     282,   218,   283,   270,   284,   285,   286,   287,   288,   219,
     220,   909,   863,   252,   289,   913,   795,   517,   861,   917,
     918,   919,   221,   222,   922,   923,   924,   259,   260,   261,
     262,   270,   223,   224,   709,   272,   273,   885,   226,   227,
     809,   810,   340,   228,   813,   290,   229,  1170,   710,   711,
     712,   713,   235,   714,   663,   715,   236,   266,   267,   237,
     394,   238,   250,   272,   273,   253,   277,  1187,   396,   397,
     398,   399,   400,   396,  1015,   398,   399,   400,   920,   321,
     335,   338,   354,   395,   381,   951,   716,   717,   718,   384,
     719,   385,   390,   720,   721,   722,   393,   723,   724,   725,
     423,   726,   664,   396,   397,   398,   399,   400,   278,   279,
     280,   281,   282,   426,   283,   270,   284,   285,   286,   287,
     288,   683,   429,   430,   435,   431,   289,   432,   727,   434,
     728,   729,   396,   397,   398,   399,   400,   436,   437,   730,
     731,   732,   733,   734,   735,   689,   438,   272,   273,   444,
     898,   396,   397,   398,   399,   400,   705,   290,   446,   454,
     908,   447,   736,   448,   450,   452,   453,  1045,   455,   458,
     396,   397,   398,   399,   400,   459,   737,   743,   461,  1017,
    -235,   462,   750,   463,   464,   465,   466,   467,   270,   469,
     470,   472,   468,   471,   396,   397,   398,   399,   400,   271,
     473,   474,   475,   266,   267,   396,   397,   398,   399,   400,
     476,  1078,  1079,  1080,  1081,  1082,   477,   478,   479,   480,
     272,   273,   481,  1188,   482,  1191,   396,   397,   398,   399,
     400,   396,   397,   398,   399,   400,   483,   485,   486,  1196,
     487,   489,   488,   490,    16,   492,  1013,   493,   494,   495,
     244,   496,    17,   497,   498,   499,    18,    19,    20,    21,
      22,    23,   500,   501,    24,    25,    26,    27,    28,    29,
      30,   905,   502,   505,    31,    32,    33,    34,    35,   506,
      36,   507,   508,   511,   512,    37,    38,    39,    40,    41,
      42,    43,    44,   513,    45,    46,    47,  1114,    48,   516,
      49,    50,    51,    52,    53,    54,   514,   323,   525,    55,
      56,   543,    57,   526,   527,   537,   428,   538,   539,    58,
     396,   397,   398,   399,   400,   545,   549,   278,   279,   280,
     281,   282,   550,   283,   270,   284,   285,   286,   287,   288,
      59,   551,   553,    60,   558,   289,   559,   560,   980,   561,
     575,   576,   578,   589,   579,   981,   581,   583,   592,  1076,
     593,   598,   599,   600,   602,    61,   272,   273,   601,   608,
     610,    62,   611,    63,    64,   617,   290,    65,    66,   982,
      67,   612,    68,   622,   629,   632,   633,   634,    69,   640,
    -183,    70,    71,   641,   647,   655,   652,    72,    73,   983,
      74,   667,    75,   656,   682,   684,  1176,   688,   685,   687,
     694,   231,   693,   696,   697,   926,   927,    84,   695,    86,
     698,    88,   700,    90,   701,    92,  1193,    94,   702,    96,
     703,    98,   746,   100,  1147,   704,   747,    76,    77,    78,
      79,   742,    80,   984,   985,   986,   987,   748,   988,   989,
     990,   991,   992,   749,   753,   751,   752,   754,  1163,   756,
     928,   929,   755,   759,   757,   758,    81,   930,   760,   761,
     762,   764,    82,    83,   765,   767,   768,   775,   780,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   769,   931,   771,
     770,   932,   933,   934,   935,   936,   937,   938,   939,   940,
     941,   942,   943,   944,   945,   946,   947,    83,   773,   774,
     699,   776,   777,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,    85,   778,    87,   784,    89,   791,    91,   779,    93,
     781,    95,   782,    97,   787,    99,   788,   101,   789,   790,
     792,   254,   325,   803,   802,   804,   806,   805,   562,   812,
     816,   818,   255,   256,   817,   819,   820,   821,   822,   823,
     824,   825,   826,   840,   827,   828,   829,   830,   831,   832,
     833,   834,   836,   837,   835,   841,   843,   844,   845,   846,
     851,   852,   853,   858,   857,   860,   862,   864,   870,   865,
     875,   874,   880,   897,   899,   882,   911,   903,   906,   907,
     914,   916,   915,   921,   912,   960,   962,   966,   963,   965,
     971,   967,   969,   997,   998,   970,   968,   973,   974,   978,
     979,   994,   995,   996,  1000,  1001,  1002,  1011,  1012,  1027,
    1010,  1014,  1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,
    1026,  1028,  1034,  1036,  1037,  1042,  1043,  1047,  1046,  1048,
    1049,  1050,  1051,  1052,  1059,  1060,  1053,  1054,  1055,  1056,
    1057,  1058,  1061,  1065,  1062,  1063,  1072,  1067,  1069,  1070,
    1087,  1073,  1077,  1088,  1089,  1090,  1095,  1091,  1094,  1092,
    1096,  1102,  1097,  1100,  1101,  1104,  1116,  1117,  1118,  1121,
    1123,  1124,  1130,  1128,  1132,  1133,  1135,  1136,  1137,  1139,
    1141,  1127,  1142,  1143,  1144,  1148,  1149,  1150,  1155,  1168,
    1166,  1172,  1154,  1151,  1177,  1192,    10,   540,   232,   799,
    1158,  1157,  1160,  1161,  1165,   382,  1167,  1175,   425,  1178,
    1185,   225,  1194,  1071,   623,   371,   690,  1066,  1120,  1044,
     972,  1098,  1152,   304,  1129,  1115,   842,   950,  1153,   544,
     686,   889,     0,     0,   449,     0,   740,     0,     0,     0,
       0,     0,   665
};

static const yytype_int16 yycheck[] =
{
     108,   180,   378,   230,   617,    81,   177,   538,   537,   188,
     490,   190,   465,   515,   505,   506,     4,   196,   197,     3,
     199,   200,   414,   680,     3,   414,     3,   502,     3,    84,
       6,   210,   873,    84,   213,   182,     4,     4,   217,    68,
     219,     8,     9,    59,   191,   200,   999,     4,   539,   204,
     205,   206,     4,     4,   693,     4,   235,   236,   237,   238,
      84,    85,     4,     4,   211,   212,     8,   214,   215,    84,
     249,   218,    84,     4,   221,    92,   223,     8,   249,   226,
      84,   457,     5,   193,   460,   224,  1064,   676,    84,    85,
     188,   201,   202,    84,    84,  1119,    84,    85,    84,     5,
      84,   280,   281,     4,   283,  1145,  1162,     8,     9,    84,
     232,    75,   222,   226,   225,   224,     3,   227,   227,     3,
      84,   243,   244,    98,  1180,     4,     3,    16,    17,     3,
      76,  1171,     3,   209,   232,  1159,   249,   250,    84,   249,
     779,   224,     0,    84,   227,   284,  1099,    26,    99,    28,
     107,   103,   109,   110,    33,    34,    35,   225,   385,   224,
     228,    84,  1140,   224,     3,   241,   231,   196,   278,   279,
     231,   247,   189,   249,   787,   224,   129,   232,    84,    83,
      84,   232,   231,    84,   208,   209,   210,   211,   212,   802,
     214,   215,   216,   217,   218,   219,   220,    84,   225,   812,
      84,   224,   226,  1044,   232,    84,    85,    79,   231,    84,
     226,    98,    84,    84,   224,   224,    91,   232,   237,   238,
     232,   231,   226,   247,   248,   678,   757,    98,   243,   244,
     226,   243,   244,   257,     4,   226,   226,   894,   226,   149,
     226,   151,   226,   414,   232,    84,   230,   226,   232,   226,
     232,   226,   228,   241,   242,   243,   244,   232,   226,   226,
     225,   488,     3,   142,   232,   130,     7,   446,   447,   253,
     254,   746,   648,   241,   242,   243,   244,   226,   670,   668,
     255,   256,   461,   462,   226,     3,     4,   434,   790,   277,
     819,   782,   241,   242,   747,   226,   225,   444,   477,   241,
     242,   448,   891,   834,   161,   452,   224,   224,   384,   277,
     241,   242,   491,   231,   231,   423,   463,   393,   394,   228,
     396,   397,   398,   399,   400,    95,   481,    97,   277,   225,
     485,   486,   487,   225,   213,   277,   215,   413,   414,   226,
     511,   234,   226,    84,    85,   232,   277,   226,   232,   226,
     426,   230,   226,   232,   224,   226,   230,   227,   225,   230,
     436,   232,   241,   242,   243,   244,    84,    85,   247,   248,
     249,   250,     4,   225,   251,   252,   146,   251,   252,   455,
     527,   560,   561,   459,   255,   256,   226,   226,   224,    84,
      85,   227,   468,   232,   225,    84,    85,    80,   277,    88,
      89,   225,   581,   224,   780,   163,   227,   165,   278,   279,
     280,   281,   282,     4,   253,   254,   225,     8,     9,    89,
      90,   278,   279,   280,   281,   282,   225,   284,   278,   279,
     280,   281,   282,   116,   117,    84,   967,    86,    87,   970,
      88,    89,   125,   225,   280,   281,   282,  1054,  1055,   525,
     225,   134,    84,    85,   225,    59,   225,   278,   279,   280,
     281,   282,   145,   280,   281,   282,   225,   208,   209,   210,
     211,   212,   225,   214,   215,   216,   217,   218,   219,   220,
      84,    85,    44,    45,    46,   226,   225,   224,   171,   225,
     208,   209,   210,   211,   212,  1024,   214,   215,   216,   217,
     218,   219,   220,   674,   225,   225,   247,   248,   226,   225,
     251,   252,   225,   224,   890,   225,   257,   225,   225,   225,
     215,  1052,   225,  1003,  1053,   225,   215,   225,   225,   247,
     248,   226,   225,   225,   225,  1148,   283,   226,   225,   257,
     767,   278,   279,   280,   281,   282,   225,   694,  1161,   225,
     225,   698,   247,   248,   701,   702,   225,     4,   247,   248,
     225,     8,     9,    83,    84,    85,  1097,   278,   279,   280,
     281,   282,   225,   225,   225,   651,   208,   209,   210,   211,
     212,   225,   214,   215,   216,   217,   218,   219,   220,   225,
     225,   818,   771,     3,   226,   822,   672,   227,   769,   826,
     827,   828,   225,   225,   831,   832,   833,    10,    11,    12,
      13,   215,   225,   225,    61,   247,   248,   788,   225,   225,
     696,   697,   226,   225,   700,   257,   225,  1158,    75,    76,
      77,    78,   225,    80,   229,    82,   225,    84,    85,   225,
     206,   225,   225,   247,   248,   224,   103,  1178,   278,   279,
     280,   281,   282,   278,   279,   280,   281,   282,   829,     4,
     232,    96,   226,   229,   151,   836,   113,   114,   115,   283,
     117,   228,   226,   120,   121,   122,   228,   124,   125,   126,
     230,   128,   229,   278,   279,   280,   281,   282,   208,   209,
     210,   211,   212,   228,   214,   215,   216,   217,   218,   219,
     220,   229,   224,   224,     4,   224,   226,   223,   155,   226,
     157,   158,   278,   279,   280,   281,   282,   228,   224,   166,
     167,   168,   169,   170,   171,   229,   224,   247,   248,   226,
     806,   278,   279,   280,   281,   282,   229,   257,   226,     3,
     816,   226,   189,   226,   226,   286,   224,   974,   228,     3,
     278,   279,   280,   281,   282,   228,   203,   229,   224,   930,
     230,   224,   229,   224,   224,   224,   224,     4,   215,   224,
     224,     4,   228,   226,   278,   279,   280,   281,   282,   226,
     224,   224,   224,    84,    85,   278,   279,   280,   281,   282,
     224,  1018,  1019,  1020,  1021,  1022,   224,   224,     4,   224,
     247,   248,   224,  1179,   224,  1181,   278,   279,   280,   281,
     282,   278,   279,   280,   281,   282,     6,   224,   224,  1195,
     224,     4,   224,   224,     6,   224,   902,   224,   224,   224,
     277,   224,    14,   224,   224,   224,    18,    19,    20,    21,
      22,    23,   224,   224,    26,    27,    28,    29,    30,    31,
      32,   229,   224,   224,    36,    37,    38,    39,    40,   224,
      42,   224,   229,   224,   224,    47,    48,    49,    50,    51,
      52,    53,    54,   224,    56,    57,    58,  1056,    60,   227,
      62,    63,    64,    65,    66,    67,   224,   188,   224,    71,
      72,    15,    74,   227,   225,   225,   284,   225,   225,    81,
     278,   279,   280,   281,   282,     3,     3,   208,   209,   210,
     211,   212,   224,   214,   215,   216,   217,   218,   219,   220,
     102,   229,   224,   105,     5,   226,   233,   224,    94,   224,
     221,   227,    99,    83,   224,   101,   224,   224,    83,  1015,
     224,    41,     4,   224,    43,   127,   247,   248,    41,   189,
      93,   133,   224,   135,   136,   224,   257,   139,   140,   125,
     142,    93,   144,   224,    97,   193,   103,   106,   150,   137,
     141,   153,   154,   137,   152,   147,   229,   159,   160,   145,
     162,   224,   164,   148,   231,     3,  1165,     4,   224,   227,
     224,   235,   285,   224,   224,    86,    87,   241,   227,   243,
     224,   245,   228,   247,   224,   249,  1185,   251,   224,   253,
     224,   255,   224,   257,  1122,   232,   224,   199,   200,   201,
     202,   232,   204,   189,   190,   191,   192,   224,   194,   195,
     196,   197,   198,     4,     4,   224,   224,   224,  1146,   224,
     131,   132,   228,     4,   225,   224,   228,   138,   224,     4,
     224,   224,   234,   235,   224,   224,    83,     4,   141,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   224,   169,   225,
     224,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   235,   224,   224,
       3,   224,   224,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   242,   224,   244,   284,   246,   227,   248,   224,   250,
     224,   252,   224,   254,   224,   256,   224,   258,   225,   224,
       4,   232,   226,     3,   224,   227,   224,   233,    83,   224,
     226,   225,   243,   244,   227,   225,   225,   225,   225,   225,
     225,   225,   225,     5,   225,   225,   225,   225,   225,   225,
     225,   225,   224,   224,   227,   224,     4,   224,   189,     4,
     189,    93,   224,    93,   232,   224,    69,   224,   224,   100,
       4,   100,   100,   224,   227,   230,   112,   227,   227,   227,
     118,   123,   119,   156,   111,   224,     4,     4,   224,   224,
       4,   225,   224,     4,     4,   225,   229,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,     5,    44,     3,
     227,   227,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   224,   104,     4,   224,   227,   224,    70,   227,    95,
     225,   225,   225,   225,   107,   107,   225,   225,   225,   225,
     225,   225,   107,     4,   227,   224,   224,     7,   225,   231,
       4,   227,    84,   146,     4,   224,     4,   225,   224,   101,
     224,   100,   225,   224,   224,     4,     4,   224,     4,   225,
       5,    44,   103,   227,   225,    24,     4,   227,   189,   227,
      95,   285,    94,   108,   227,   224,   227,   222,    25,   189,
      99,    94,   225,   227,     4,  1184,     5,   422,    73,   678,
     225,   227,   224,   224,   224,   229,   225,   224,   253,   225,
     224,    66,   227,  1006,   490,   220,   557,  1001,  1066,   973,
     855,  1043,  1127,   184,  1090,  1057,   751,   836,  1132,   428,
     550,   790,    -1,    -1,   287,    -1,   581,    -1,    -1,    -1,
      -1,    -1,   525
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    16,    17,   288,   289,   290,   291,   225,   225,     0,
     289,   129,   301,   232,   232,   225,     6,    14,    18,    19,
      20,    21,    22,    23,    26,    27,    28,    29,    30,    31,
      32,    36,    37,    38,    39,    40,    42,    47,    48,    49,
      50,    51,    52,    53,    54,    56,    57,    58,    60,    62,
      63,    64,    65,    66,    67,    71,    72,    74,    81,   102,
     105,   127,   133,   135,   136,   139,   140,   142,   144,   150,
     153,   154,   159,   160,   162,   164,   199,   200,   201,   202,
     204,   228,   234,   235,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   292,   293,   294,   295,   296,   303,   305,   306,
     307,   308,   310,   311,   319,   322,   323,   325,   326,   333,
     335,   336,   338,   340,   342,   346,   347,   348,   351,   353,
     360,   367,   370,   376,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   392,   393,   394,   395,   399,   400,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   422,   423,
     424,   425,   426,   427,   224,   130,   302,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   325,   225,   225,   225,   225,
     228,   235,   307,   309,   327,   225,   225,   225,   225,   234,
       4,   226,   241,   242,   277,   460,   470,   226,   303,   283,
     225,   337,     3,   224,   232,   243,   244,   446,   459,    10,
      11,    12,    13,     4,    99,   471,    84,    85,    88,    89,
     215,   226,   247,   248,   368,   448,   449,   103,   208,   209,
     210,   211,   212,   214,   216,   217,   218,   219,   220,   226,
     257,   448,   468,   469,     3,    84,    98,   226,   232,   255,
     256,   378,   457,   458,   378,     3,    84,   226,   232,   253,
     254,   372,   455,   456,    84,   232,   428,    75,    84,   440,
     448,     4,   448,   188,   469,   226,   249,   250,   451,   452,
     451,    59,   226,   478,   479,   232,   448,   448,    96,   448,
     226,   448,   478,   451,   451,    84,   232,   350,   429,   478,
     478,   478,   447,   460,   226,   349,   460,   448,   469,   469,
     448,   469,   469,     4,     8,     9,    84,   472,   448,   469,
     448,   350,   469,   451,   469,     5,    84,   361,   343,   469,
     451,   151,   306,   447,   283,   228,   448,   448,   448,   448,
     226,     8,   460,   228,   206,   229,   278,   279,   280,   281,
     282,   459,   460,   465,   466,   467,    26,    28,    33,    34,
      35,   142,   213,   226,   230,   448,   451,   459,   460,   474,
     475,   476,   341,   230,   304,   302,   228,   224,   284,   224,
     224,   224,   223,   450,   226,     4,   228,   224,   224,   355,
     451,   451,   448,   448,   226,   448,   226,   226,   226,   468,
     226,   469,   286,   224,     3,   228,   379,   377,     3,   228,
     371,   224,   224,   224,   224,   224,   224,     4,   228,   224,
     224,   226,     4,   224,   224,   224,   224,   224,   224,     4,
     224,   224,   224,     6,   324,   224,   224,   224,   224,     4,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   304,   344,   224,   224,   224,   229,   460,
     447,   224,   224,   224,   224,   320,   227,   227,   460,   460,
     460,   460,   460,   460,   460,   224,   227,   225,     4,   315,
     316,   317,   318,   449,   452,   459,   460,   225,   225,   225,
     305,   303,   460,    15,   446,     3,     3,   226,   401,     3,
     224,   229,   469,   224,   460,    89,    90,   369,     5,   233,
     224,   224,    83,   363,   469,   448,   448,     3,     7,   226,
     251,   252,   453,   454,   469,   221,   227,   469,    99,   224,
     460,   224,   304,   224,   460,   304,   448,   448,   469,    83,
     313,   456,    83,   224,   460,    76,    84,   441,    41,     4,
     224,    41,    43,    84,    86,    87,   442,   445,   189,   448,
      93,   224,    93,   478,    84,   226,   356,   224,   478,   478,
     478,   447,   224,   349,   472,   448,    79,    84,   477,    97,
      68,   196,   193,   103,   106,    84,   226,   357,     4,   103,
     137,   137,   440,   345,   226,   453,   453,   152,   339,   161,
     284,   334,   229,    84,   459,   147,   148,   149,   151,   308,
     462,   463,   464,   229,   229,   465,   469,   224,   224,   231,
     224,   231,   224,   231,   224,   231,   230,   457,   230,   455,
     230,   453,   231,   229,     3,   224,   450,   227,     4,   229,
     355,   448,   448,   285,   224,   227,   224,   224,   224,     3,
     228,   224,   224,   224,   232,   229,     4,     8,     9,    61,
      75,    76,    77,    78,    80,    82,   113,   114,   115,   117,
     120,   121,   122,   124,   125,   126,   128,   155,   157,   158,
     166,   167,   168,   169,   170,   171,   189,   203,   380,   448,
     470,   473,   232,   229,   373,   365,   224,   224,   224,     4,
     229,   224,   224,     4,   224,   228,   224,   225,   224,     4,
     224,     4,   224,   441,   224,   224,   391,   224,    83,   224,
     224,   225,   396,   224,   224,     4,   224,   224,   224,   224,
     141,   224,   224,   304,   284,   460,   328,   224,   224,   225,
     224,   227,     4,   452,   449,   460,   459,   312,   458,   313,
     314,   454,   224,     3,   227,   233,   224,   363,   469,   460,
     460,   469,   224,   460,   469,   469,   226,   227,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   227,   224,   224,   440,   456,
       5,   224,   429,     4,   224,   189,     4,    84,    91,   444,
     455,   189,    93,   224,    84,   226,   358,   232,    93,   447,
     224,   459,    69,   448,   224,   100,     4,   107,   109,   110,
     224,    84,   226,   359,   100,     4,    84,   362,   363,   304,
     100,   453,   230,   441,    84,   459,   237,   238,   461,   462,
     321,   224,   231,   231,   224,   231,   441,   224,   460,   227,
     224,   227,   224,   227,   441,   229,   227,   227,   460,   447,
     457,   112,   111,   447,   118,   119,   123,   447,   447,   447,
     459,   156,   447,   447,   447,   455,    86,    87,   131,   132,
     138,   169,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   374,   443,
     445,   459,    80,   116,   117,   125,   134,   145,   171,   366,
     224,   299,     4,   224,   430,   224,     4,   225,   229,   224,
     225,     4,   368,   224,   224,     4,    84,   364,   224,   224,
      94,   101,   125,   145,   189,   190,   191,   192,   194,   195,
     196,   197,   198,   397,   224,   224,   224,     4,     4,   430,
     224,   224,   224,   163,   165,   329,   330,   304,   458,   454,
     227,     5,    44,   460,   227,   279,   225,   459,   225,   225,
     225,   225,   225,   225,   225,   225,   225,     3,   224,    44,
      45,    46,   431,   432,   104,   433,     4,   224,   455,    92,
     189,   455,   227,   224,   359,   447,   227,    70,    95,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   107,
     107,   107,   227,   224,   433,     4,   364,     7,   472,   225,
     231,   329,   224,   227,   224,   227,   460,    84,   447,   447,
     447,   447,   447,   188,   232,   375,   457,     4,   146,     4,
     224,   225,   101,   435,   224,     4,   224,   225,   369,   430,
     224,   224,   100,   436,     4,     4,    95,    97,   146,   398,
     455,   457,   401,   401,   448,   428,     4,   224,     4,   435,
     365,   225,   332,     5,    44,   224,   297,   285,   227,   431,
     103,   434,   225,    24,   438,     4,   227,   189,   455,   227,
     433,    95,    94,   108,   227,   438,   331,   303,   224,   227,
     222,   227,   375,   436,   225,    25,   439,   227,   225,   435,
     224,   224,   439,   303,   441,   224,    99,   225,   189,   437,
     455,   438,    94,   441,   437,   224,   448,     4,   225,   411,
     439,   352,     5,    84,   300,   224,   298,   455,   304,   412,
     437,   304,   299,   448,   227,   354,   304
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
		      add_opvars(splev, "iiiiio", VA_PASS6(1, 1, (int)(yyvsp[(7) - (7)].i), 0, (int)(yyvsp[(5) - (7)].i), SPO_DECORATION));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2396 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2402 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2406 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2412 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_CARPET));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2418 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2422 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4(0, (int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2428 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2432 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2438 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2442 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2447 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2454 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2458 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2465 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2469 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2474 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2481 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2485 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2490 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2498 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2504 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2511 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2515 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2522 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2533 "lev_comp.y"
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

  case 398:

/* Line 1455 of yacc.c  */
#line 2546 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2550 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2556 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2560 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2567 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2571 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2575 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2581 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2585 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2591 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2597 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2601 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2607 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2613 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2618 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2624 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2628 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2634 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2638 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2654 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2662 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2672 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2681 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2686 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2693 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2704 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2710 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2714 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2718 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2725 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2735 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2742 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2746 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2752 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2756 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2764 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2768 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2775 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2785 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2798 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2802 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2809 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2819 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2828 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2839 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2843 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2850 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2860 "lev_comp.y"
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

  case 456:

/* Line 1455 of yacc.c  */
#line 2871 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2880 "lev_comp.y"
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

  case 458:

/* Line 1455 of yacc.c  */
#line 2890 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2896 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2900 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2907 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2917 "lev_comp.y"
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

  case 463:

/* Line 1455 of yacc.c  */
#line 2928 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2937 "lev_comp.y"
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

  case 465:

/* Line 1455 of yacc.c  */
#line 2947 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2951 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2957 "lev_comp.y"
    { ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2959 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 2965 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2969 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 2973 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 2977 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 2985 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 2994 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 2998 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3002 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3006 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3010 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3013 "lev_comp.y"
    { ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3017 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3024 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3033 "lev_comp.y"
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

  case 487:

/* Line 1455 of yacc.c  */
#line 3074 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3078 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3085 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3092 "lev_comp.y"
    {
			      size_t len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3102 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3106 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3114 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3118 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3122 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3126 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3130 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3135 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3139 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3143 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3148 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3153 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3158 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3162 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3167 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3172 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3176 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3180 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3185 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3189 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3196 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3202 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3206 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3212 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 520:

/* Line 1455 of yacc.c  */
#line 3228 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
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
		      /* nothing */
		  ;}
    break;

  case 532:

/* Line 1455 of yacc.c  */
#line 3262 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 533:

/* Line 1455 of yacc.c  */
#line 3266 "lev_comp.y"
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

  case 534:

/* Line 1455 of yacc.c  */
#line 3292 "lev_comp.y"
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
#line 7496 "lev_comp.tab.c"
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
#line 3320 "lev_comp.y"


/*lev_comp.y*/

