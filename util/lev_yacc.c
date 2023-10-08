
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

/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-07-16 */

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
     MESSAGE_ATTR = 271,
     MESSAGE_COLOR = 272,
     MESSAGE_SOUND_TYPE = 273,
     MESSAGE_SOUND_ID = 274,
     MAZE_ID = 275,
     LEVEL_ID = 276,
     LEV_INIT_ID = 277,
     TILESET_ID = 278,
     GEOMETRY_ID = 279,
     NOMAP_ID = 280,
     BOUNDARY_TYPE_ID = 281,
     SPECIAL_TILESET_ID = 282,
     TILESET_PARAM_ID = 283,
     DECOTYP_ID = 284,
     OBJECT_ID = 285,
     COBJECT_ID = 286,
     MONSTER_ID = 287,
     TRAP_ID = 288,
     DOOR_ID = 289,
     DRAWBRIDGE_ID = 290,
     MONSTER_GENERATION_ID = 291,
     object_ID = 292,
     monster_ID = 293,
     terrain_ID = 294,
     MAZEWALK_ID = 295,
     WALLIFY_ID = 296,
     REGION_ID = 297,
     SPECIAL_REGION_ID = 298,
     SPECIAL_LEVREGION_ID = 299,
     SPECIAL_REGION_TYPE = 300,
     NAMING_ID = 301,
     NAMING_TYPE = 302,
     FILLING = 303,
     IRREGULAR = 304,
     JOINED = 305,
     ALTAR_ID = 306,
     ANVIL_ID = 307,
     NPC_ID = 308,
     LADDER_ID = 309,
     STAIR_ID = 310,
     NON_DIGGABLE_ID = 311,
     NON_PASSWALL_ID = 312,
     ROOM_ID = 313,
     ARTIFACT_NAME_ID = 314,
     PORTAL_ID = 315,
     TELEPRT_ID = 316,
     BRANCH_ID = 317,
     LEV = 318,
     MINERALIZE_ID = 319,
     AGE_ID = 320,
     CORRIDOR_ID = 321,
     GOLD_ID = 322,
     ENGRAVING_ID = 323,
     FOUNTAIN_ID = 324,
     THRONE_ID = 325,
     MODRON_PORTAL_ID = 326,
     LEVEL_TELEPORTER_ID = 327,
     LEVEL_TELEPORT_DIRECTION_TYPE = 328,
     LEVEL_TELEPORT_END_TYPE = 329,
     POOL_ID = 330,
     SINK_ID = 331,
     NONE = 332,
     RAND_CORRIDOR_ID = 333,
     DOOR_STATE = 334,
     LIGHT_STATE = 335,
     CURSE_TYPE = 336,
     MYTHIC_TYPE = 337,
     ENGRAVING_TYPE = 338,
     KEYTYPE_ID = 339,
     LEVER_ID = 340,
     NO_PICKUP_ID = 341,
     DIRECTION = 342,
     RANDOM_TYPE = 343,
     RANDOM_TYPE_BRACKET = 344,
     A_REGISTER = 345,
     ALIGNMENT = 346,
     LEFT_OR_RIGHT = 347,
     CENTER = 348,
     TOP_OR_BOT = 349,
     ALTAR_TYPE = 350,
     ALTAR_SUBTYPE = 351,
     UP_OR_DOWN = 352,
     ACTIVE_OR_INACTIVE = 353,
     MODRON_PORTAL_TYPE = 354,
     NPC_TYPE = 355,
     FOUNTAIN_TYPE = 356,
     SPECIAL_OBJECT_TYPE = 357,
     CMAP_TYPE = 358,
     FLOOR_SUBTYPE = 359,
     FLOOR_SUBTYPE_ID = 360,
     FLOOR_ID = 361,
     FLOOR_TYPE = 362,
     FLOOR_TYPE_ID = 363,
     DECORATION_ID = 364,
     DECORATION_TYPE = 365,
     DECORATION_DIR = 366,
     DECORATION_ITEM_STATE = 367,
     PAINTING_TYPE = 368,
     BANNER_TYPE = 369,
     WALL_SCULPTURE_TYPE = 370,
     ELEMENTAL_ENCHANTMENT_TYPE = 371,
     EXCEPTIONALITY_TYPE = 372,
     EXCEPTIONALITY_ID = 373,
     ELEMENTAL_ENCHANTMENT_ID = 374,
     ENCHANTMENT_ID = 375,
     SECRET_DOOR_ID = 376,
     USES_UP_KEY_ID = 377,
     MYTHIC_PREFIX_TYPE = 378,
     MYTHIC_SUFFIX_TYPE = 379,
     MYTHIC_PREFIX_ID = 380,
     MYTHIC_SUFFIX_ID = 381,
     MATERIAL_ID = 382,
     MATERIAL_TYPE = 383,
     CHARGES_ID = 384,
     SPECIAL_QUALITY_ID = 385,
     SPEFLAGS_ID = 386,
     LEVEL_BOSS_ID = 387,
     BOSS_HOSTILITY_ID = 388,
     SUBROOM_ID = 389,
     NAME_ID = 390,
     FLAGS_ID = 391,
     FLAG_TYPE = 392,
     MON_ATTITUDE = 393,
     MON_ALERTNESS = 394,
     SUBTYPE_ID = 395,
     NON_PASSDOOR_ID = 396,
     CARPET_ID = 397,
     CARPET_PIECE_ID = 398,
     CARPET_TYPE = 399,
     MON_APPEARANCE = 400,
     ROOMDOOR_ID = 401,
     IF_ID = 402,
     ELSE_ID = 403,
     TERRAIN_ID = 404,
     HORIZ_OR_VERT = 405,
     REPLACE_TERRAIN_ID = 406,
     LOCATION_SUBTYPE_ID = 407,
     DOOR_SUBTYPE = 408,
     BRAZIER_SUBTYPE = 409,
     SIGNPOST_SUBTYPE = 410,
     TREE_SUBTYPE = 411,
     FOREST_ID = 412,
     FOREST_TYPE = 413,
     INITIALIZE_TYPE = 414,
     EXIT_ID = 415,
     SHUFFLE_ID = 416,
     MANUAL_TYPE_ID = 417,
     MANUAL_TYPE = 418,
     QUANTITY_ID = 419,
     BURIED_ID = 420,
     LOOP_ID = 421,
     FOR_ID = 422,
     TO_ID = 423,
     SWITCH_ID = 424,
     CASE_ID = 425,
     BREAK_ID = 426,
     DEFAULT_ID = 427,
     ERODED_ID = 428,
     TRAPPED_STATE = 429,
     RECHARGED_ID = 430,
     INVIS_ID = 431,
     GREASED_ID = 432,
     INDESTRUCTIBLE_ID = 433,
     FEMALE_ID = 434,
     MALE_ID = 435,
     WAITFORU_ID = 436,
     PROTECTOR_ID = 437,
     CANCELLED_ID = 438,
     REVIVED_ID = 439,
     AVENGE_ID = 440,
     FLEEING_ID = 441,
     BLINDED_ID = 442,
     MAXHP_ID = 443,
     LEVEL_ADJUSTMENT_ID = 444,
     KEEP_ORIGINAL_INVENTORY_ID = 445,
     PARALYZED_ID = 446,
     STUNNED_ID = 447,
     CONFUSED_ID = 448,
     SEENTRAPS_ID = 449,
     ALL_ID = 450,
     MONTYPE_ID = 451,
     OBJTYPE_ID = 452,
     TERTYPE_ID = 453,
     TERTYPE2_ID = 454,
     LEVER_EFFECT_TYPE = 455,
     SWITCHABLE_ID = 456,
     CONTINUOUSLY_USABLE_ID = 457,
     TARGET_ID = 458,
     TRAPTYPE_ID = 459,
     EFFECT_FLAG_ID = 460,
     GRAVE_ID = 461,
     BRAZIER_ID = 462,
     SIGNPOST_ID = 463,
     TREE_ID = 464,
     ERODEPROOF_ID = 465,
     FUNCTION_ID = 466,
     MSG_OUTPUT_TYPE = 467,
     COMPARE_TYPE = 468,
     UNKNOWN_TYPE = 469,
     rect_ID = 470,
     fillrect_ID = 471,
     line_ID = 472,
     randline_ID = 473,
     grow_ID = 474,
     selection_ID = 475,
     flood_ID = 476,
     rndcoord_ID = 477,
     circle_ID = 478,
     ellipse_ID = 479,
     filter_ID = 480,
     complement_ID = 481,
     gradient_ID = 482,
     GRADIENT_TYPE = 483,
     LIMITED = 484,
     HUMIDITY_TYPE = 485,
     STRING = 486,
     MAP_ID = 487,
     NQSTRING = 488,
     VARSTRING = 489,
     CFUNC = 490,
     CFUNC_INT = 491,
     CFUNC_STR = 492,
     CFUNC_COORD = 493,
     CFUNC_REGION = 494,
     VARSTRING_INT = 495,
     VARSTRING_INT_ARRAY = 496,
     VARSTRING_STRING = 497,
     VARSTRING_STRING_ARRAY = 498,
     VARSTRING_VAR = 499,
     VARSTRING_VAR_ARRAY = 500,
     VARSTRING_COORD = 501,
     VARSTRING_COORD_ARRAY = 502,
     VARSTRING_REGION = 503,
     VARSTRING_REGION_ARRAY = 504,
     VARSTRING_MAPCHAR = 505,
     VARSTRING_MAPCHAR_ARRAY = 506,
     VARSTRING_MONST = 507,
     VARSTRING_MONST_ARRAY = 508,
     VARSTRING_OBJ = 509,
     VARSTRING_OBJ_ARRAY = 510,
     VARSTRING_SEL = 511,
     VARSTRING_SEL_ARRAY = 512,
     METHOD_INT = 513,
     METHOD_INT_ARRAY = 514,
     METHOD_STRING = 515,
     METHOD_STRING_ARRAY = 516,
     METHOD_VAR = 517,
     METHOD_VAR_ARRAY = 518,
     METHOD_COORD = 519,
     METHOD_COORD_ARRAY = 520,
     METHOD_REGION = 521,
     METHOD_REGION_ARRAY = 522,
     METHOD_MAPCHAR = 523,
     METHOD_MAPCHAR_ARRAY = 524,
     METHOD_MONST = 525,
     METHOD_MONST_ARRAY = 526,
     METHOD_OBJ = 527,
     METHOD_OBJ_ARRAY = 528,
     METHOD_SEL = 529,
     METHOD_SEL_ARRAY = 530,
     DICE = 531
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
#line 576 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 588 "lev_comp.tab.c"

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
#define YYLAST   1378

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  294
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  193
/* YYNRULES -- Number of rules.  */
#define YYNRULES  540
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1217

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   531

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   289,   293,     2,
     233,   234,   287,   285,   231,   286,   291,   288,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   232,     2,
       2,   290,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   235,     2,   236,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   237,   292,   238,     2,     2,     2,     2,
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
     225,   226,   227,   228,   229,   230,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284
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
     525,   530,   531,   536,   538,   539,   544,   548,   560,   578,
     580,   584,   588,   594,   600,   608,   613,   614,   630,   631,
     649,   650,   653,   659,   661,   667,   669,   675,   677,   683,
     685,   696,   703,   705,   707,   709,   711,   713,   717,   719,
     721,   722,   726,   730,   734,   738,   740,   742,   744,   746,
     748,   756,   762,   764,   766,   768,   770,   774,   775,   781,
     786,   787,   791,   793,   795,   797,   799,   802,   804,   806,
     808,   810,   812,   816,   818,   822,   826,   830,   832,   834,
     838,   840,   842,   844,   846,   850,   852,   854,   856,   858,
     860,   864,   868,   869,   875,   878,   879,   883,   885,   889,
     891,   895,   899,   901,   903,   907,   909,   911,   913,   917,
     919,   921,   923,   927,   931,   935,   939,   943,   947,   951,
     955,   957,   959,   961,   963,   967,   971,   975,   979,   985,
     993,   999,  1008,  1010,  1014,  1018,  1024,  1030,  1038,  1046,
    1053,  1059,  1060,  1063,  1069,  1073,  1085,  1099,  1106,  1107,
    1111,  1113,  1117,  1121,  1125,  1129,  1131,  1133,  1137,  1141,
    1145,  1149,  1153,  1157,  1159,  1161,  1163,  1165,  1169,  1173,
    1175,  1181,  1191,  1197,  1205,  1209,  1213,  1219,  1229,  1235,
    1241,  1247,  1248,  1264,  1265,  1267,  1275,  1287,  1297,  1311,
    1315,  1323,  1335,  1349,  1359,  1369,  1379,  1387,  1395,  1401,
    1409,  1415,  1425,  1433,  1439,  1449,  1455,  1461,  1465,  1473,
    1477,  1485,  1493,  1497,  1503,  1509,  1513,  1519,  1527,  1537,
    1539,  1541,  1543,  1545,  1547,  1548,  1551,  1553,  1557,  1559,
    1561,  1563,  1564,  1568,  1570,  1571,  1575,  1577,  1578,  1582,
    1583,  1587,  1588,  1592,  1594,  1596,  1598,  1600,  1602,  1604,
    1606,  1608,  1610,  1614,  1616,  1618,  1623,  1625,  1627,  1632,
    1634,  1636,  1641,  1643,  1648,  1654,  1656,  1660,  1662,  1666,
    1668,  1670,  1675,  1685,  1687,  1689,  1694,  1696,  1702,  1704,
    1706,  1711,  1713,  1715,  1721,  1723,  1725,  1727,  1732,  1734,
    1736,  1742,  1744,  1746,  1748,  1752,  1754,  1756,  1760,  1762,
    1767,  1771,  1775,  1779,  1783,  1787,  1791,  1793,  1795,  1799,
    1801,  1805,  1806,  1808,  1810,  1812,  1814,  1818,  1819,  1821,
    1823,  1826,  1829,  1834,  1841,  1846,  1853,  1860,  1867,  1874,
    1877,  1884,  1893,  1902,  1913,  1928,  1931,  1933,  1937,  1939,
    1943,  1945,  1947,  1949,  1951,  1953,  1955,  1957,  1959,  1961,
    1963,  1965,  1967,  1969,  1971,  1973,  1975,  1977,  1979,  1981,
    1992
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     295,     0,    -1,    -1,   296,    -1,   297,    -1,   297,   296,
      -1,   298,   308,   310,    -1,    21,   232,   239,    -1,    20,
     232,   239,   231,     3,    -1,    20,   232,   239,   231,     3,
     231,   448,    -1,    22,   232,    11,   231,   408,    -1,    22,
     232,    10,   231,     3,    -1,    22,   232,    13,    -1,    22,
     232,    12,   231,     3,   231,     3,   231,     5,   231,     5,
     231,   448,   231,   307,   306,    -1,    23,   232,   478,    -1,
     157,   232,   158,   231,   159,    -1,   157,   232,   158,    -1,
      36,   232,     4,   231,   320,    -1,    26,   232,   107,    -1,
      -1,   231,   229,    -1,    -1,   231,   455,    -1,    -1,   231,
       3,    -1,     5,    -1,    88,    -1,    -1,   136,   232,   309,
      -1,   137,   231,   309,    -1,   137,    -1,    -1,   312,   310,
      -1,   237,   310,   238,    -1,   353,    -1,   299,    -1,   300,
      -1,   416,    -1,   303,    -1,   301,    -1,   424,    -1,   425,
      -1,   426,    -1,   302,    -1,   423,    -1,   422,    -1,   420,
      -1,   421,    -1,   427,    -1,   428,    -1,   429,    -1,   430,
      -1,   431,    -1,   397,    -1,   355,    -1,   318,    -1,   317,
      -1,   411,    -1,   367,    -1,   389,    -1,   433,    -1,   434,
      -1,   399,    -1,   400,    -1,   401,    -1,   432,    -1,   333,
      -1,   343,    -1,   345,    -1,   349,    -1,   347,    -1,   330,
      -1,   340,    -1,   326,    -1,   329,    -1,   392,    -1,   402,
      -1,   374,    -1,   390,    -1,   377,    -1,   383,    -1,   412,
      -1,   407,    -1,   395,    -1,   354,    -1,   413,    -1,   414,
      -1,   415,    -1,   417,    -1,   360,    -1,   358,    -1,   406,
      -1,   410,    -1,   409,    -1,   393,    -1,   394,    -1,   396,
      -1,   388,    -1,   391,    -1,   249,    -1,   251,    -1,   253,
      -1,   255,    -1,   257,    -1,   259,    -1,   261,    -1,   263,
      -1,   265,    -1,   248,    -1,   250,    -1,   252,    -1,   254,
      -1,   256,    -1,   258,    -1,   260,    -1,   262,    -1,   264,
      -1,   313,    -1,   314,    -1,   242,    -1,   242,    -1,   314,
      -1,   161,   232,   313,    -1,   315,   290,   467,    -1,   315,
     290,   220,   232,   476,    -1,   315,   290,   466,    -1,   315,
     290,   483,   232,   460,    -1,   315,   290,   482,   232,   462,
      -1,   315,   290,   481,   232,   464,    -1,   315,   290,   455,
      -1,   315,   290,   458,    -1,   315,   290,   237,   324,   238,
      -1,   315,   290,   237,   323,   238,    -1,   315,   290,   237,
     322,   238,    -1,   315,   290,   483,   232,   237,   321,   238,
      -1,   315,   290,   482,   232,   237,   320,   238,    -1,   315,
     290,   481,   232,   237,   319,   238,    -1,   315,   290,   237,
     325,   238,    -1,   465,    -1,   319,   231,   465,    -1,   463,
      -1,   320,   231,   463,    -1,   461,    -1,   321,   231,   461,
      -1,   459,    -1,   322,   231,   459,    -1,   456,    -1,   323,
     231,   456,    -1,   467,    -1,   324,   231,   467,    -1,   466,
      -1,   325,   231,   466,    -1,    -1,    -1,   211,   241,   233,
     327,   471,   234,   328,   311,    -1,   241,   233,   474,   234,
      -1,   160,    -1,    -1,     6,    -1,     6,    -1,   235,   467,
     213,   467,   236,    -1,   235,   467,   236,    -1,    -1,    -1,
     169,   334,   235,   454,   236,   335,   237,   336,   238,    -1,
      -1,   337,   336,    -1,    -1,   170,   479,   232,   338,   310,
      -1,    -1,   172,   232,   339,   310,    -1,   171,    -1,   291,
     291,    -1,   168,    -1,   167,   316,   290,   467,   341,   467,
      -1,    -1,   342,   344,   311,    -1,    -1,   166,   235,   454,
     236,   346,   311,    -1,    -1,   332,   232,   348,   312,    -1,
      -1,   147,   332,   350,   351,    -1,   311,    -1,    -1,   311,
     352,   148,   311,    -1,    14,   232,   466,    -1,    14,   232,
     466,   231,    15,   231,    16,   231,    17,   231,     4,    -1,
      14,   232,   466,   231,    15,   231,    16,   231,    17,   231,
      18,   231,    19,   231,     4,   231,     4,    -1,    78,    -1,
      78,   232,   479,    -1,    78,   232,    88,    -1,    66,   232,
     356,   231,   356,    -1,    66,   232,   356,   231,   479,    -1,
     233,     4,   231,    87,   231,   371,   234,    -1,   436,   331,
     231,   448,    -1,    -1,   134,   232,   357,   231,   364,   231,
     366,   437,   440,   442,   445,   446,   444,   359,   311,    -1,
      -1,    58,   232,   357,   231,   363,   231,   365,   231,   366,
     437,   440,   442,   445,   446,   444,   361,   311,    -1,    -1,
     231,     5,    -1,   233,     4,   231,     4,   234,    -1,    88,
      -1,   233,     4,   231,     4,   234,    -1,    88,    -1,   233,
     375,   231,   376,   234,    -1,    88,    -1,   233,     4,   231,
       4,   234,    -1,    88,    -1,   146,   232,   368,   231,   447,
     231,   369,   231,   371,   372,    -1,    34,   232,   447,   231,
     476,   372,    -1,     5,    -1,    88,    -1,   370,    -1,    88,
      -1,    87,    -1,    87,   292,   370,    -1,     4,    -1,    88,
      -1,    -1,   372,   231,   373,    -1,   152,   232,   153,    -1,
      84,   232,   464,    -1,   130,   232,     4,    -1,   178,    -1,
     121,    -1,   122,    -1,   141,    -1,    25,    -1,    24,   232,
     375,   231,   376,   362,   240,    -1,    24,   232,   455,   362,
     240,    -1,    92,    -1,    93,    -1,    94,    -1,    93,    -1,
      32,   232,   379,    -1,    -1,    32,   232,   379,   378,   311,
      -1,   462,   231,   455,   380,    -1,    -1,   380,   231,   381,
      -1,   466,    -1,   138,    -1,   139,    -1,   450,    -1,   145,
     466,    -1,   179,    -1,   180,    -1,   176,    -1,   183,    -1,
     184,    -1,   184,   232,   454,    -1,   185,    -1,   186,   232,
     454,    -1,   187,   232,   454,    -1,   191,   232,   454,    -1,
     192,    -1,   193,    -1,   194,   232,   382,    -1,   194,    -1,
     181,    -1,   182,    -1,   188,    -1,   189,   232,   454,    -1,
     190,    -1,   132,    -1,   133,    -1,   239,    -1,   195,    -1,
     239,   292,   382,    -1,    30,   232,   385,    -1,    -1,    31,
     232,   385,   384,   311,    -1,   464,   386,    -1,    -1,   386,
     231,   387,    -1,    81,    -1,   196,   232,   462,    -1,   480,
      -1,   135,   232,   466,    -1,   164,   232,   454,    -1,   165,
      -1,    80,    -1,   173,   232,   454,    -1,   210,    -1,    79,
      -1,   174,    -1,   175,   232,   454,    -1,   176,    -1,   177,
      -1,   455,    -1,   119,   232,   116,    -1,   118,   232,   117,
      -1,   120,   232,   454,    -1,   129,   232,   454,    -1,   130,
     232,   454,    -1,   162,   232,   163,    -1,   131,   232,   454,
      -1,    84,   232,   464,    -1,   178,    -1,   122,    -1,    86,
      -1,    82,    -1,   125,   232,   123,    -1,   126,   232,   124,
      -1,    65,   232,   454,    -1,   127,   232,   128,    -1,    33,
     232,   435,   231,   455,    -1,    35,   232,   455,   231,    87,
     231,   447,    -1,    40,   232,   455,   231,    87,    -1,    40,
     232,   455,   231,    87,   231,     5,   306,    -1,    41,    -1,
      41,   232,   476,    -1,    41,   232,   195,    -1,    54,   232,
     455,   231,    97,    -1,    55,   232,   455,   231,    97,    -1,
      55,   232,   485,   231,   485,   231,    97,    -1,    60,   232,
     485,   231,   485,   231,   239,    -1,    61,   232,   485,   231,
     485,   398,    -1,    62,   232,   485,   231,   485,    -1,    -1,
     231,    97,    -1,    69,   232,   476,   231,   101,    -1,    70,
     232,   476,    -1,    71,   232,   455,   231,   203,   232,   455,
     231,    99,   231,    98,    -1,    71,   232,   455,   231,    72,
     231,    73,   231,    74,   231,    99,   231,    98,    -1,    85,
     232,   455,   231,   200,   403,    -1,    -1,   403,   231,   404,
      -1,    98,    -1,   196,   232,   462,    -1,   197,   232,   464,
      -1,   198,   232,   408,    -1,   199,   232,   408,    -1,   201,
      -1,   202,    -1,   203,   232,   455,    -1,   204,   232,   435,
      -1,   152,   232,   405,    -1,   105,   232,   443,    -1,   205,
     232,     4,    -1,   130,   232,     4,    -1,     4,    -1,    99,
      -1,   153,    -1,   101,    -1,    76,   232,   476,    -1,    75,
     232,   476,    -1,     3,    -1,   233,     3,   231,   448,   234,
      -1,   151,   232,   458,   231,   460,   231,   460,   231,     7,
      -1,   149,   232,   476,   231,   460,    -1,   149,   232,   476,
     231,   460,   231,   104,    -1,    56,   232,   458,    -1,    57,
     232,   458,    -1,    46,   232,   239,   231,    47,    -1,    46,
     232,   239,   231,    47,   231,   196,   232,   462,    -1,    43,
     232,   458,   231,    45,    -1,    44,   232,   485,   231,    45,
      -1,    27,   232,   476,   231,   103,    -1,    -1,    42,   232,
     458,   231,   448,   231,   436,   437,   440,   442,   445,   446,
     444,   418,   419,    -1,    -1,   311,    -1,    51,   232,   455,
     231,   449,   231,   451,    -1,    51,   232,   455,   231,   449,
     231,   451,   231,   196,   232,   462,    -1,    51,   232,   455,
     231,   449,   231,   451,   231,    96,    -1,    51,   232,   455,
     231,   449,   231,   451,   231,    96,   231,   196,   232,   462,
      -1,    52,   232,   455,    -1,    52,   232,   455,   231,   196,
     232,   462,    -1,   109,   232,   455,   231,   110,   231,     4,
     231,   111,   231,   112,    -1,   109,   232,   455,   231,   110,
     231,     4,   231,   111,   231,   112,   231,   448,    -1,   109,
     232,   455,   231,   110,   231,   113,   231,   111,    -1,   109,
     232,   455,   231,   110,   231,   114,   231,   111,    -1,   109,
     232,   455,   231,   110,   231,   115,   231,   111,    -1,   109,
     232,   455,   231,   110,   231,   111,    -1,   106,   232,   476,
     231,   107,   231,   104,    -1,   140,   232,   476,   231,     4,
      -1,   140,   232,   476,   231,   107,   231,   104,    -1,   142,
     232,   458,   231,   144,    -1,   143,   232,   476,   231,   144,
     231,     4,   231,     4,    -1,   143,   232,   476,   231,   144,
     231,     4,    -1,    53,   232,   100,   231,   455,    -1,    53,
     232,   100,   231,   455,   231,   196,   232,   462,    -1,   206,
     232,   455,   231,   466,    -1,   206,   232,   455,   231,    88,
      -1,   206,   232,   455,    -1,   207,   232,   455,   231,   154,
     231,   448,    -1,   207,   232,   455,    -1,   208,   232,   455,
     231,   155,   231,   466,    -1,   208,   232,   455,   231,   155,
     231,    88,    -1,   208,   232,   455,    -1,   209,   232,   455,
     231,   156,    -1,   209,   232,   455,   231,   158,    -1,   209,
     232,   455,    -1,    67,   232,   467,   231,   455,    -1,    68,
     232,   455,   231,   484,   231,   466,    -1,    64,   232,   454,
     231,   454,   231,   454,   231,   454,    -1,    64,    -1,   239,
      -1,    88,    -1,   239,    -1,    88,    -1,    -1,   231,   438,
      -1,   439,    -1,   439,   231,   438,    -1,    48,    -1,    49,
      -1,    50,    -1,    -1,   108,   232,   441,    -1,   107,    -1,
      -1,   105,   232,   443,    -1,   104,    -1,    -1,   196,   232,
     462,    -1,    -1,    28,   232,   103,    -1,    -1,    29,   232,
       4,    -1,    79,    -1,    88,    -1,    80,    -1,    88,    -1,
      91,    -1,   452,    -1,    88,    -1,    91,    -1,   452,    -1,
      90,   232,    88,    -1,    95,    -1,    88,    -1,    90,   235,
       4,   236,    -1,   239,    -1,   250,    -1,   251,   235,   467,
     236,    -1,   467,    -1,   456,    -1,   222,   233,   476,   234,
      -1,   254,    -1,   255,   235,   467,   236,    -1,   233,     4,
     231,     4,   234,    -1,    88,    -1,    89,   457,   236,    -1,
     230,    -1,   230,   231,   457,    -1,   459,    -1,   256,    -1,
     257,   235,   467,   236,    -1,   233,     4,   231,     4,   231,
       4,   231,     4,   234,    -1,   461,    -1,   258,    -1,   259,
     235,   467,   236,    -1,     3,    -1,   233,     3,   231,   448,
     234,    -1,   463,    -1,   260,    -1,   261,   235,   467,   236,
      -1,   239,    -1,     3,    -1,   233,     3,   231,   239,   234,
      -1,    88,    -1,   465,    -1,   262,    -1,   263,   235,   467,
     236,    -1,   239,    -1,     3,    -1,   233,     3,   231,   239,
     234,    -1,    88,    -1,   102,    -1,   453,    -1,   466,   291,
     453,    -1,     4,    -1,   477,    -1,   233,     8,   234,    -1,
     248,    -1,   249,   235,   467,   236,    -1,   467,   285,   467,
      -1,   467,   286,   467,    -1,   467,   287,   467,    -1,   467,
     288,   467,    -1,   467,   289,   467,    -1,   233,   467,   234,
      -1,   244,    -1,   245,    -1,   315,   232,   468,    -1,   469,
      -1,   470,   231,   469,    -1,    -1,   470,    -1,   467,    -1,
     466,    -1,   472,    -1,   473,   231,   472,    -1,    -1,   473,
      -1,   455,    -1,   215,   458,    -1,   216,   458,    -1,   217,
     455,   231,   455,    -1,   218,   455,   231,   455,   231,   467,
      -1,   219,   233,   476,   234,    -1,   219,   233,   370,   231,
     476,   234,    -1,   225,   233,     7,   231,   476,   234,    -1,
     225,   233,   476,   231,   476,   234,    -1,   225,   233,   460,
     231,   476,   234,    -1,   221,   455,    -1,   223,   233,   455,
     231,   467,   234,    -1,   223,   233,   455,   231,   467,   231,
      48,   234,    -1,   224,   233,   455,   231,   467,   231,   467,
     234,    -1,   224,   233,   455,   231,   467,   231,   467,   231,
      48,   234,    -1,   227,   233,   228,   231,   233,   467,   286,
     467,   304,   234,   231,   455,   305,   234,    -1,   226,   475,
      -1,   264,    -1,   233,   476,   234,    -1,   475,    -1,   475,
     293,   476,    -1,   284,    -1,   103,    -1,     4,    -1,     8,
      -1,     9,    -1,     4,    -1,     8,    -1,     9,    -1,     4,
      -1,   477,    -1,    37,    -1,    30,    -1,    38,    -1,    32,
      -1,    39,    -1,   149,    -1,    83,    -1,    88,    -1,   486,
      -1,    63,   233,     4,   231,     4,   231,     4,   231,     4,
     234,    -1,   233,     4,   231,     4,   231,     4,   231,     4,
     234,    -1
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
    1181,  1214,  1213,  1239,  1250,  1249,  1277,  1281,  1285,  1291,
    1296,  1301,  1308,  1315,  1324,  1332,  1344,  1343,  1369,  1368,
    1394,  1397,  1403,  1413,  1419,  1428,  1434,  1439,  1445,  1450,
    1456,  1467,  1473,  1474,  1477,  1478,  1481,  1485,  1491,  1492,
    1496,  1502,  1510,  1515,  1520,  1525,  1530,  1535,  1540,  1548,
    1555,  1563,  1571,  1572,  1575,  1576,  1579,  1584,  1583,  1597,
    1604,  1611,  1619,  1624,  1630,  1636,  1642,  1648,  1653,  1658,
    1663,  1668,  1673,  1678,  1683,  1688,  1693,  1698,  1703,  1708,
    1714,  1720,  1725,  1730,  1735,  1740,  1745,  1750,  1757,  1766,
    1770,  1783,  1792,  1791,  1809,  1819,  1825,  1833,  1839,  1844,
    1849,  1854,  1859,  1864,  1869,  1874,  1879,  1893,  1899,  1904,
    1909,  1914,  1919,  1924,  1929,  1934,  1939,  1944,  1949,  1954,
    1959,  1964,  1969,  1974,  1980,  1985,  1990,  1995,  2002,  2008,
    2037,  2042,  2050,  2056,  2060,  2068,  2075,  2082,  2092,  2102,
    2118,  2129,  2132,  2138,  2144,  2150,  2154,  2160,  2167,  2173,
    2181,  2187,  2192,  2197,  2202,  2207,  2213,  2219,  2224,  2229,
    2234,  2239,  2244,  2251,  2251,  2251,  2251,  2254,  2260,  2266,
    2271,  2278,  2285,  2289,  2295,  2301,  2307,  2312,  2319,  2325,
    2335,  2342,  2341,  2375,  2378,  2384,  2389,  2394,  2399,  2405,
    2409,  2415,  2419,  2423,  2427,  2431,  2435,  2441,  2447,  2451,
    2457,  2463,  2467,  2473,  2477,  2483,  2487,  2492,  2499,  2503,
    2510,  2514,  2519,  2526,  2530,  2535,  2543,  2549,  2556,  2560,
    2567,  2575,  2578,  2588,  2592,  2595,  2601,  2605,  2612,  2616,
    2620,  2627,  2630,  2636,  2643,  2646,  2652,  2659,  2663,  2670,
    2673,  2680,  2683,  2689,  2690,  2693,  2694,  2697,  2698,  2699,
    2705,  2706,  2707,  2713,  2714,  2717,  2726,  2731,  2738,  2749,
    2755,  2759,  2763,  2770,  2780,  2787,  2791,  2797,  2801,  2809,
    2813,  2820,  2830,  2843,  2847,  2854,  2864,  2873,  2884,  2888,
    2895,  2905,  2916,  2925,  2935,  2941,  2945,  2952,  2962,  2973,
    2982,  2992,  2996,  3003,  3004,  3010,  3014,  3018,  3022,  3030,
    3039,  3043,  3047,  3051,  3055,  3059,  3062,  3069,  3078,  3111,
    3112,  3115,  3116,  3119,  3123,  3130,  3137,  3148,  3151,  3159,
    3163,  3167,  3171,  3175,  3180,  3184,  3188,  3193,  3198,  3203,
    3207,  3212,  3217,  3221,  3225,  3230,  3234,  3241,  3247,  3251,
    3257,  3264,  3265,  3268,  3269,  3270,  3273,  3277,  3281,  3285,
    3291,  3292,  3295,  3296,  3299,  3300,  3303,  3304,  3307,  3311,
    3337
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
  "MESSAGE_ATTR", "MESSAGE_COLOR", "MESSAGE_SOUND_TYPE",
  "MESSAGE_SOUND_ID", "MAZE_ID", "LEVEL_ID", "LEV_INIT_ID", "TILESET_ID",
  "GEOMETRY_ID", "NOMAP_ID", "BOUNDARY_TYPE_ID", "SPECIAL_TILESET_ID",
  "TILESET_PARAM_ID", "DECOTYP_ID", "OBJECT_ID", "COBJECT_ID",
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
  "DECORATION_DIR", "DECORATION_ITEM_STATE", "PAINTING_TYPE",
  "BANNER_TYPE", "WALL_SCULPTURE_TYPE", "ELEMENTAL_ENCHANTMENT_TYPE",
  "EXCEPTIONALITY_TYPE", "EXCEPTIONALITY_ID", "ELEMENTAL_ENCHANTMENT_ID",
  "ENCHANTMENT_ID", "SECRET_DOOR_ID", "USES_UP_KEY_ID",
  "MYTHIC_PREFIX_TYPE", "MYTHIC_SUFFIX_TYPE", "MYTHIC_PREFIX_ID",
  "MYTHIC_SUFFIX_ID", "MATERIAL_ID", "MATERIAL_TYPE", "CHARGES_ID",
  "SPECIAL_QUALITY_ID", "SPEFLAGS_ID", "LEVEL_BOSS_ID",
  "BOSS_HOSTILITY_ID", "SUBROOM_ID", "NAME_ID", "FLAGS_ID", "FLAG_TYPE",
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
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,    44,    58,    40,    41,    91,    93,   123,   125,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,   531,    43,    45,    42,    47,    37,
      61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   294,   295,   295,   296,   296,   297,   298,   298,   298,
     299,   299,   299,   299,   300,   301,   301,   302,   303,   304,
     304,   305,   305,   306,   306,   307,   307,   308,   308,   309,
     309,   310,   310,   311,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   315,   315,   315,
     316,   316,   317,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   319,   319,
     320,   320,   321,   321,   322,   322,   323,   323,   324,   324,
     325,   325,   327,   328,   326,   329,   330,   331,   331,   332,
     332,   332,   334,   335,   333,   336,   336,   338,   337,   339,
     337,   340,   341,   341,   342,   344,   343,   346,   345,   348,
     347,   350,   349,   351,   352,   351,   353,   353,   353,   354,
     354,   354,   355,   355,   356,   357,   359,   358,   361,   360,
     362,   362,   363,   363,   364,   364,   365,   365,   366,   366,
     367,   367,   368,   368,   369,   369,   370,   370,   371,   371,
     372,   372,   373,   373,   373,   373,   373,   373,   373,   374,
     374,   374,   375,   375,   376,   376,   377,   378,   377,   379,
     380,   380,   381,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   381,   382,   382,
     382,   383,   384,   383,   385,   386,   386,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   388,   389,
     390,   390,   391,   391,   391,   392,   393,   394,   395,   396,
     397,   398,   398,   399,   400,   401,   401,   402,   403,   403,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   405,   405,   405,   405,   406,   407,   408,
     408,   409,   410,   410,   411,   412,   413,   413,   414,   415,
     416,   418,   417,   419,   419,   420,   420,   420,   420,   421,
     421,   422,   422,   422,   422,   422,   422,   423,   424,   424,
     425,   426,   426,   427,   427,   428,   428,   428,   429,   429,
     430,   430,   430,   431,   431,   431,   432,   433,   434,   434,
     435,   435,   436,   436,   437,   437,   438,   438,   439,   439,
     439,   440,   440,   441,   442,   442,   443,   444,   444,   445,
     445,   446,   446,   447,   447,   448,   448,   449,   449,   449,
     450,   450,   450,   451,   451,   452,   453,   453,   453,   454,
     455,   455,   455,   455,   456,   456,   456,   457,   457,   458,
     458,   458,   459,   460,   460,   460,   461,   461,   462,   462,
     462,   463,   463,   463,   463,   464,   464,   464,   465,   465,
     465,   465,   465,   466,   466,   467,   467,   467,   467,   467,
     467,   467,   467,   467,   467,   467,   468,   468,   469,   470,
     470,   471,   471,   472,   472,   473,   473,   474,   474,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   476,   476,
     477,   478,   478,   479,   479,   479,   480,   480,   480,   480,
     481,   481,   482,   482,   483,   483,   484,   484,   485,   485,
     486
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
       4,     0,     4,     1,     0,     4,     3,    11,    17,     1,
       3,     3,     5,     5,     7,     4,     0,    15,     0,    17,
       0,     2,     5,     1,     5,     1,     5,     1,     5,     1,
      10,     6,     1,     1,     1,     1,     1,     3,     1,     1,
       0,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       7,     5,     1,     1,     1,     1,     3,     0,     5,     4,
       0,     3,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     3,     1,     3,     3,     3,     1,     1,     3,
       1,     1,     1,     1,     3,     1,     1,     1,     1,     1,
       3,     3,     0,     5,     2,     0,     3,     1,     3,     1,
       3,     3,     1,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     3,     3,     3,     3,     5,     7,
       5,     8,     1,     3,     3,     5,     5,     7,     7,     6,
       5,     0,     2,     5,     3,    11,    13,     6,     0,     3,
       1,     3,     3,     3,     3,     1,     1,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     3,     3,     1,
       5,     9,     5,     7,     3,     3,     5,     9,     5,     5,
       5,     0,    15,     0,     1,     7,    11,     9,    13,     3,
       7,    11,    13,     9,     9,     9,     7,     7,     5,     7,
       5,     9,     7,     5,     9,     5,     5,     3,     7,     3,
       7,     7,     3,     5,     5,     3,     5,     7,     9,     1,
       1,     1,     1,     1,     0,     2,     1,     3,     1,     1,
       1,     0,     3,     1,     0,     3,     1,     0,     3,     0,
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
       2,     0,     0,     0,     3,     4,    27,     0,     0,     1,
       5,     0,    31,     0,     7,     0,   159,     0,     0,     0,
       0,   229,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,     0,   189,     0,     0,
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
     475,     0,   478,     0,   520,     0,   476,   497,    32,     0,
     179,     0,     8,     0,   436,   437,     0,   473,   186,     0,
       0,     0,    12,   522,   521,    14,   445,     0,   232,   233,
       0,     0,   442,     0,     0,   200,   440,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     516,   499,   518,     0,   469,   471,   472,     0,   468,   466,
       0,   271,   275,   465,   272,   462,   464,     0,   461,   459,
       0,   236,     0,   458,   401,   400,     0,   423,   424,     0,
       0,     0,     0,   314,   313,     0,   450,     0,     0,   449,
       0,     0,     0,     0,   538,     0,     0,   369,     0,     0,
       0,     0,     0,   354,   355,   403,   402,     0,   157,     0,
       0,     0,     0,   439,     0,     0,     0,     0,     0,   324,
       0,   348,   347,   525,   523,   524,   191,   190,     0,     0,
       0,     0,     0,     0,     0,   212,   213,     0,     0,     0,
       0,    16,   122,     0,     0,     0,   387,   389,   392,   395,
     152,     0,     0,     0,     0,   161,     0,     0,     0,     0,
       0,   494,   493,   495,   498,     0,   531,   533,   530,   532,
     534,   535,     0,     0,     0,   129,   130,   125,   123,     0,
       0,     0,     0,    31,   176,     0,    29,     0,     0,     0,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,   500,   501,     0,     0,     0,   509,     0,     0,     0,
     515,     0,     0,     0,     0,     0,     0,   274,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   158,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   183,   182,     0,     0,     0,   177,
       0,     0,     0,     0,     0,     0,   491,   477,   485,     0,
       0,   480,   481,   482,   483,   484,     0,   155,     0,   475,
       0,     0,     0,     0,   146,   144,   150,   148,     0,     0,
       0,   180,     0,   425,   426,     9,     0,     0,   474,    11,
     349,     0,    10,     0,     0,   446,     0,     0,     0,   235,
     234,   200,   201,   231,     0,     0,   216,     0,     0,     0,
       0,   456,     0,     0,   454,     0,     0,   453,     0,     0,
     517,   519,   360,     0,     0,     0,   273,     0,     0,   238,
     240,   308,   220,     0,    17,   140,   310,     0,     0,     0,
     358,     0,     0,   359,   356,   429,     0,   427,     0,   428,
       0,   383,   315,     0,   316,     0,   203,     0,     0,     0,
       0,   321,   320,     0,     0,   192,   193,   396,   536,   537,
       0,   323,     0,     0,   328,     0,     0,   205,     0,     0,
     378,     0,   380,     0,     0,     0,     0,   352,     0,    15,
       0,   173,     0,     0,   163,   386,   385,     0,     0,   393,
     394,     0,   489,   492,     0,   479,   160,   496,   124,     0,
       0,   133,     0,   132,     0,   131,     0,   137,     0,   128,
       0,   127,     0,   126,    33,   438,     0,     0,     0,   448,
     441,     0,   443,     0,   502,     0,     0,     0,   504,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   467,   528,
     526,   527,     0,   286,   283,   277,   303,     0,   302,     0,
       0,     0,   301,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   282,     0,   287,     0,   289,   290,   300,     0,
     285,   276,   291,   529,   279,     0,   460,   239,   211,     0,
       0,     0,     0,   451,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   195,     0,     0,   319,
       0,     0,     0,     0,     0,   327,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   178,   172,   174,     0,
       0,     0,     0,     0,   153,     0,   145,   147,   149,   151,
       0,   138,     0,     0,   142,     0,     0,     0,   444,   230,
       0,   217,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   470,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   463,
       0,     0,   309,   141,    23,     0,   404,     0,     0,     0,
       0,   434,   433,   365,   370,     0,   317,     0,   207,     0,
       0,   318,   322,     0,     0,   397,     0,     0,     0,   377,
       0,   376,     0,     0,     0,     0,   209,     0,   404,   379,
     382,   215,     0,   214,   185,   353,     0,   165,   388,   391,
     390,   486,   487,   488,   490,     0,     0,   136,   135,     0,
     134,     0,     0,     0,   503,   505,     0,   510,     0,   506,
       0,   455,   508,   507,     0,   306,   299,   293,   292,   294,
     304,   305,   307,   295,   296,   298,   280,   297,   281,   284,
     288,   278,     0,   430,   266,   267,   243,   244,     0,   249,
     247,   248,   261,   262,   250,   251,   253,     0,     0,   263,
       0,   265,     0,   257,   258,   260,   241,   245,   431,   242,
       0,   226,   227,     0,   228,     0,   225,   221,     0,   311,
       0,     0,   411,     0,     0,     0,   435,     0,     0,     0,
       0,     0,     0,   218,   219,     0,     0,     0,   330,     0,
       0,     0,     0,     0,     0,     0,   335,   336,     0,     0,
       0,   329,     0,     0,     0,     0,     0,     0,   411,     0,
       0,     0,     0,     0,     0,   165,   154,   139,   143,     0,
     350,     0,     0,     0,   457,     0,     0,   246,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    24,     0,   408,
     409,   410,   405,   406,     0,   414,     0,     0,   357,   367,
       0,   384,   202,     0,   404,   398,   194,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     373,   374,   375,   204,     0,   414,   381,   220,   351,     0,
     169,   164,   166,     0,     0,   511,     0,   512,   481,   432,
     252,   254,   255,   264,   256,   269,   268,   259,   223,   224,
     222,     0,     0,     0,     0,   419,     0,     0,     0,     0,
       0,   411,     0,     0,   416,   340,   342,   343,   344,   346,
     345,   339,   331,   332,   333,   334,   337,   338,   341,     0,
       0,   419,   210,   167,    31,   187,     0,     0,     0,     0,
       0,     0,   452,   407,   413,   412,     0,     0,   421,     0,
     540,     0,   366,   206,   414,     0,   325,   371,   208,   421,
      31,   170,     0,     0,   513,    20,     0,   270,   415,     0,
       0,   417,   539,     0,   419,     0,     0,   417,   168,     0,
       0,     0,   420,     0,     0,   361,   368,   421,   326,   372,
     196,     0,     0,    21,   422,     0,   363,   417,     0,     0,
      25,    26,    23,     0,     0,   418,   364,   362,   198,   197,
       0,    13,    22,   514,     0,   188,   199
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,   102,   103,   104,   105,   106,
    1140,  1204,   969,  1202,    12,   176,   107,   424,   108,   109,
     110,   111,   233,   112,   113,   800,   594,   803,   530,   531,
     532,   533,   114,   516,   895,   115,   116,   485,   117,   118,
     234,   789,  1014,  1015,  1160,  1134,   119,   653,   120,   121,
     251,   122,   650,   123,   422,   124,   378,   505,   645,   125,
     126,   127,   355,   347,   128,  1198,   129,  1214,   440,   618,
     639,   860,   878,   130,   377,   882,   567,   985,   748,   967,
     131,   274,   561,   132,   461,   311,   747,   956,  1097,   133,
     458,   301,   457,   741,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   769,   144,   145,   146,   147,   775,
    1001,  1121,   148,   149,   552,   150,   151,   152,   153,   154,
     155,   156,   157,   158,  1196,  1207,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   316,   348,   972,  1042,  1043,  1045,  1145,  1105,  1115,
    1185,  1148,  1171,   319,   545,   608,   957,   853,   609,   257,
     352,   291,   276,   434,   328,   329,   576,   577,   312,   313,
     302,   303,   401,   353,   893,   662,   663,   664,   403,   404,
     405,   292,   452,   246,   265,   367,   744,   419,   420,   421,
     630,   333,   334
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1069
static const yytype_int16 yypact[] =
{
     388,  -152,  -136,   112, -1069,   388,     7,   -84,   -81, -1069,
   -1069,   -59,   987,   -52, -1069,    48, -1069,   -43,   -18,    -5,
       3, -1069,    14,    24,    55,    74,    86,    96,   100,   124,
     140,   155,   157,   159,   180,   229,   248,   265,   271,   281,
     285,   286,   288,   289,   290,   291,   293,   295,   296,   299,
     300,   301,   302,   309,   310,   311,   312,   313,   339,   342,
     343,   345,   346,   352,   353,   354,    23,   357,   358,   361,
   -1069,   362,   -31,   732, -1069, -1069,   364,   367,   371,   372,
     121,    49,   142, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,   987, -1069,
   -1069,    57, -1069, -1069, -1069, -1069, -1069,   373, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069,   407,   183, -1069,   107,   420,    63,
      -2,   450,   681,    72,    72,   137,   -41,    46,   -46,   568,
     -46,   499,  -130,  -130,   -22,   370,   -46,   -46,   510,   -46,
     -26,  -130,  -130,   -39,   -22,   -22,   -22,    49,   383,    49,
     -46,   681,   681,   -46,   681,   681,   115,   -46,   681,   -46,
     -39,   681,  -130,   681,    71, -1069,   681,  -130,   459,   629,
      49, -1069, -1069,   330,   399,   -46,   -46,   -46,   -46,   398,
   -1069,    40, -1069,   400, -1069,   294, -1069,    42, -1069,   145,
   -1069,   406,   421,    48, -1069, -1069,   401, -1069,  -126,   424,
     426,   429, -1069, -1069, -1069, -1069, -1069,   432, -1069, -1069,
     418,   650, -1069,   439,   444,   445, -1069, -1069,  -130,  -130,
     -46,   -46,   446,   -46,   455,   456,   457,   681,   458,   337,
   -1069, -1069,   403,   461, -1069, -1069, -1069,   690, -1069, -1069,
     462, -1069, -1069, -1069, -1069, -1069, -1069,   695, -1069, -1069,
     464,   467,   470, -1069, -1069, -1069,   474, -1069, -1069,   475,
     480,   481,   488, -1069, -1069,   724, -1069,   494,   500, -1069,
     503,   497,   731,   505, -1069,   511,   513,   514,   515,   516,
     744,   519,   520, -1069, -1069, -1069, -1069,   521,   749,   525,
     526,   528,   530,   158,   758,   534,   135,   536,   537, -1069,
     540, -1069, -1069, -1069, -1069, -1069, -1069, -1069,   541,   542,
     543,   544,   545,   546,   551, -1069, -1069,   553,   406,   555,
     558,   560, -1069,   556,    49,    49,   583,   584,   592,   596,
   -1069,   598,   452,    49,    49, -1069,    49,    49,    49,    49,
      49,   566,   158, -1069,   602,   601, -1069, -1069, -1069, -1069,
   -1069, -1069,   604,   106,    28, -1069, -1069,   566,   158,   605,
     612,   613,   987,   987, -1069,    19, -1069,    49,   836,   107,
     855,    21,   856,   631,   624,   681,   639,    49,   325,   866,
     641, -1069, -1069,   648,   652,   423, -1069,   -46,   -46,   241,
   -1069,   657,   655,   681,   788,   662,    49,   670,   406,   679,
      49,   406,   -46,   -46,   681,   824,    12,   825,   682,    49,
      19,   870,   912,   686,   873,   872,   360,   726,   -46,   823,
     692,   827,   -22,   -36, -1069,   694,   -22,   -22,   -22,    49,
     696,    70,   -46,    30,   828,   -17,   728,   819,   820,   -27,
      47,   787,   789,    46,   786, -1069,    27,    27,   778, -1069,
     203,   702,   -38,   785,   791,    37,   707, -1069, -1069,   262,
     280,   116,   116, -1069, -1069, -1069,    42, -1069,   681,   709,
     -87,   -85,    -8,     1, -1069, -1069,   566,   158,   178,    10,
     105, -1069,   703, -1069, -1069, -1069,   378,   711, -1069, -1069,
   -1069,   940, -1069,   713,   432, -1069,   716,   943,   524, -1069,
   -1069,   445, -1069, -1069,   -46,   -46,   659,   721,   739,   745,
     746, -1069,   747,   260, -1069,   740,   748, -1069,   750,   754,
   -1069, -1069, -1069,   752,   554,   699, -1069,   756,   561, -1069,
   -1069, -1069, -1069,   766,   767, -1069,   768,   983,   567,   769,
   -1069,   772,   985, -1069,   773, -1069,   770, -1069,   775, -1069,
     776,   784, -1069,  1003, -1069,   794, -1069,  1012,   801,    19,
     803,   804, -1069,   805,   950, -1069, -1069, -1069, -1069, -1069,
     815, -1069,   829,   818, -1069,   830,   833, -1069,  1055,   835,
   -1069,   837, -1069,   838,   839,   919,  1068,   842,   843, -1069,
     406, -1069,   790,    49, -1069, -1069,   566,   844,   845, -1069,
   -1069,   846, -1069,   848,   849, -1069, -1069, -1069, -1069,  1073,
     847, -1069,   -23, -1069,    49, -1069,   107, -1069,    92, -1069,
      12, -1069,    22, -1069, -1069, -1069,  1066,   853,  1082, -1069,
   -1069,   852, -1069,   850, -1069,   857,  1000,   681, -1069,    49,
      49,   681,   858,    49,   681,   681,   859,   860, -1069, -1069,
   -1069, -1069,   863, -1069, -1069, -1069, -1069,   865, -1069,   867,
     868,   869, -1069,   871,   874,   875,   876,   877,   878,   879,
     880,   881, -1069,   882, -1069,   883, -1069, -1069, -1069,   884,
   -1069, -1069, -1069, -1069, -1069,   864, -1069,   888,   889,    46,
      12,  1086,   891, -1069,   -39,  1098,   892,   908,  1101,   256,
     137,   928,  -103,  1028,   895,   -15, -1069,   893,  1031, -1069,
      49,   900,   107,  1062,   -46,   906,  1035,    77,   909,     5,
    1037,  1138,   340,   406,  1039,    27, -1069, -1069,   158,   913,
      19,   -34,   228,   707, -1069,   -32, -1069, -1069,   158,   566,
     114, -1069,   130,   143, -1069,   914,    19,   915, -1069, -1069,
      49, -1069,   917,   384,   149,   918,    19,   580,   921,   923,
      49, -1069,    49,    72,  1032,  1045,    49,  1040,  1038,  1036,
      49,    49,    49,   107,  1004,    49,    49,    49,   137, -1069,
    1027,     0, -1069, -1069,   937,  1165,   939,   942,  1167,   944,
     938, -1069, -1069,   946, -1069,   947, -1069,  1171, -1069,   409,
     949, -1069, -1069,   951,    60,   566,   952,   953,   597, -1069,
     954, -1069,   955,   956,   957,  1174, -1069,  1177,   939, -1069,
     958, -1069,   959, -1069, -1069, -1069,   960,    87, -1069, -1069,
     566, -1069, -1069, -1069, -1069,   406,    92, -1069, -1069,    22,
   -1069,  1175,   963,  1194,   158, -1069,  1152, -1069,    49, -1069,
     967, -1069, -1069, -1069,   251, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069,   566, -1069, -1069, -1069,
   -1069, -1069,   -13, -1069, -1069, -1069, -1069, -1069,   107, -1069,
   -1069, -1069, -1069, -1069, -1069,   970, -1069,   972,   973, -1069,
     991, -1069,   992, -1069, -1069,   993, -1069, -1069, -1069,   566,
     994, -1069, -1069,   995, -1069,   998, -1069, -1069,  1228, -1069,
    1001,   405,  1125,  1230,  1022,   137, -1069,   -28,   137,  1020,
    1024,     5,    49, -1069, -1069,  1023,  1182,  1159, -1069,  1029,
    1030,  1033,  1041,  1042,  1043,  1044, -1069, -1069,  1047,  1048,
    1049, -1069,  1148,  1149,  1153,  1156,  1034,  1051,  1125,  1259,
      60,  1262,   232,  1052,  1050,    87, -1069, -1069, -1069,  1054,
   -1069,  1056,  1057,   396, -1069,    49,  1183,   566,    49,    49,
      49,    49,    49,  -106,    72,  1266,  1119, -1069,  1279, -1069,
   -1069, -1069, -1069,  1058,  1060,  1181,  1059,  1289, -1069,  1063,
    1064, -1069, -1069,   325,   939, -1069, -1069,  1067,  1069,  1191,
    1293,    68,   137,    72,    21,    21,   -46,   -41,  1295,  1070,
   -1069, -1069, -1069, -1069,  1298,  1181, -1069, -1069, -1069,  1071,
   -1069, -1069, -1069,    93,  1299, -1069,  1257, -1069,   182, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069,  1014, -1069, -1069, -1069,
   -1069,  1074,   405,  1200,  1077,  1282,  1307,  1078,  1117,   137,
    1080,  1125,  1216,  1218, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,  1205,
    1084,  1282,   889, -1069,   987, -1069,  1088,  1089,  1087,  1093,
    1090,  -106, -1069, -1069, -1069, -1069,  1191,  1091,  1296,  1092,
   -1069,  1095, -1069, -1069,  1181,  1097, -1069,  1099, -1069,  1296,
     987, -1069,  1310,    19, -1069, -1069,  1100, -1069, -1069,  1229,
    1102,  1137, -1069,   137,  1282,  1237,    19,  1137, -1069,  1105,
    1106,   -46, -1069,  1334,  1107, -1069, -1069,  1296, -1069, -1069,
   -1069,  1336,    83,  1110, -1069,   137,   406,  1137,   406,  1111,
   -1069, -1069,   937,   -46,  1109, -1069, -1069, -1069, -1069, -1069,
    1340, -1069, -1069, -1069,   406, -1069, -1069
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1069, -1069,  1341, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069,   146, -1069, -1069,  1094,  -108,  -376,   927,  1116,
    1277,  -493, -1069, -1069, -1069, -1069,   671, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,  1286, -1069,
   -1069, -1069,   338, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069,   885,  1134, -1069, -1069, -1069, -1069,   795, -1069,
   -1069, -1069,   374, -1069, -1069, -1069,  -625,   347,   282, -1069,
   -1069,   501,   305, -1069, -1069, -1069, -1069, -1069,   220, -1069,
   -1069,  1178, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069,  -556, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069,   297,   609,  -839,   263, -1069,  -948, -1069,  -992,   221,
   -1068, -1037, -1067,  -487,  -463, -1069, -1069, -1069,   529,   941,
    -224,  -179,  -400,   812,   431,  -369,  -471,  -649,  -536,  -454,
    -528,  -659,  -172,   -77, -1069,   575, -1069, -1069,   851, -1069,
   -1069,  1085,   -76,   793, -1069,  -483, -1069, -1069, -1069, -1069,
   -1069,  -178, -1069
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -238
static const yytype_int16 yytable[] =
{
     248,   275,   504,   681,   245,   258,   383,   599,   626,   320,
     679,   322,   595,   305,   534,   305,   644,   336,   337,   801,
     339,   341,   342,   661,   550,   571,   349,   350,   351,    16,
     571,   357,   240,   804,   360,   647,   648,   331,   368,  1008,
     370,   331,   266,   267,   240,   535,   240,   314,   391,   345,
     655,   640,   616,   240,   889,   632,   386,   387,   388,   389,
    1075,   637,   266,   267,   983,   266,   267,   263,  1049,   683,
     415,   811,  1117,   858,   363,   294,   375,   417,   364,   365,
       7,   870,   586,  1131,   960,   589,   266,   267,  1200,  1095,
     268,   269,  1177,   876,  1159,   294,     8,  1135,   306,   543,
     306,   443,   444,   325,   446,   428,   293,   544,   571,  1190,
     529,  1136,     9,   628,   391,   324,   266,   267,   629,   363,
    1197,   961,   962,   364,   365,   317,   326,   327,   848,  1208,
     963,   808,   356,  1096,   318,   358,   359,  1187,   361,   362,
     305,   964,   369,    11,   670,   372,   672,   374,   984,   240,
     379,   671,   965,   673,   641,    13,   766,   883,    14,   376,
     295,   511,  1174,  1154,   392,   429,   264,  1118,  1050,  1119,
     402,  1201,   418,    15,   296,   406,   270,   407,   966,   174,
     295,   294,   408,   409,   410,   175,   633,   271,   871,   177,
     872,   873,   874,   659,   296,   660,   270,   617,   315,   845,
     346,   254,   808,   366,   230,   254,   638,   340,   272,   273,
     271,   332,   255,   256,   178,  1111,   255,   256,   859,  1026,
     270,  1120,   758,   674,   854,   306,   595,   179,   272,   273,
     675,   271,   676,   266,   267,   180,   363,  1017,   877,   677,
     364,   365,   536,   307,   571,   307,   181,   680,   572,   308,
    1018,   308,   272,   273,   551,   646,   182,  1012,    81,  1013,
     646,   413,   842,   702,   436,   623,   295,   254,   569,   570,
     309,   310,   797,   241,   786,   241,   242,   243,   255,   256,
     296,   254,   241,   590,   591,   574,   575,   183,   242,   243,
     242,   243,   255,   256,   411,   916,   843,   242,   243,   611,
     661,   796,   931,   354,   615,   297,   184,   510,   620,   621,
     622,   298,   244,   627,   886,   542,   519,   520,   185,   521,
     522,   523,   524,   525,   244,   297,   244,   888,   186,   266,
     267,   298,   187,   244,   299,   300,   392,   537,   646,   241,
     656,   436,   682,   902,   851,   896,   254,   249,   266,   267,
     546,   852,   897,   910,   242,   243,   188,   255,   256,   556,
     558,   750,   239,   574,   575,   412,   492,   270,   898,   568,
     307,   651,   189,   578,   899,   247,   308,   581,   413,   584,
     908,   900,   414,   588,   254,   694,   695,   190,   592,   191,
     244,   192,   598,   242,   243,   255,   256,   309,   310,   272,
     273,   326,   327,   398,   399,   400,   742,   884,     1,     2,
     252,   297,   193,  1139,   253,   678,   -19,   298,   559,   560,
     396,   397,   398,   399,   400,   266,   267,   566,   881,   244,
     259,   260,   261,   262,   396,   397,   398,   399,   400,  1048,
     299,   300,  1051,   396,   397,   398,   399,   400,   605,   402,
     606,   607,   668,  1039,  1040,  1041,   278,   279,   280,   281,
     282,   194,   283,   270,   284,   285,   286,   287,   288,   398,
     399,   400,   891,   892,   573,   278,   279,   280,   281,   282,
     195,   283,   270,   284,   285,   286,   287,   288,   396,   397,
     398,   399,   400,   289,   652,   272,   273,   196,   665,   574,
     575,   268,   269,   197,   799,   290,  1098,   394,  1124,  1125,
     566,   266,   267,   198,   272,   273,   666,   199,   200,  1016,
     201,   202,   203,   204,   290,   205,  1122,   206,   207,  1079,
     395,   208,   209,   210,   211,  1123,   396,  1025,   398,   399,
     400,   212,   213,   214,   215,   216,   863,   396,   397,   398,
     399,   400,   278,   279,   280,   281,   282,   277,   283,   270,
     284,   285,   286,   287,   288,   396,   397,   398,   399,   400,
     289,   217,   321,  1152,   218,   219,   788,   220,   221,   396,
     397,   398,   399,   400,   222,   223,   224,   266,   267,   226,
     227,   272,   273,   228,   229,   867,   235,   798,   915,   236,
     865,   290,   919,   237,   238,   250,   923,   924,   925,   335,
     338,   928,   929,   930,   685,   906,   354,   381,   907,   890,
     384,   812,   813,   814,   330,   815,   817,  1086,   818,   819,
    1087,   390,   343,   344,   385,   393,   427,  1186,   278,   279,
     280,   281,   282,   423,   283,   270,   284,   285,   286,   287,
     288,   435,   425,   373,   436,   430,   289,   431,   380,  1205,
     432,   926,   433,   396,   397,   398,   399,   400,   959,   396,
     397,   398,   399,   400,   437,   438,   439,   272,   273,   445,
     416,   396,   397,   398,   399,   400,   518,   290,   447,   448,
     449,   451,   454,   455,   323,   988,   453,   456,   459,   460,
    1180,   462,   989,   709,  -237,   463,   464,   710,   711,   441,
     442,   465,   466,  1189,   278,   279,   280,   281,   282,   467,
     283,   270,   284,   285,   286,   287,   288,   990,   468,   469,
     472,   470,   289,   904,   471,   473,   474,   396,   397,   398,
     399,   400,   475,   914,   476,   477,   478,   479,   480,   991,
     481,   482,   483,   272,   273,   484,   486,   487,  1055,   488,
     692,   489,   490,   290,   712,   491,  1027,   493,   494,   266,
     267,   495,   496,   497,   498,   499,   500,   501,   713,   714,
     715,   716,   502,   717,   503,   718,   506,   266,   267,   507,
     708,   508,   509,   992,   993,   994,   995,   746,   996,   997,
     998,   999,  1000,   753,  1090,  1091,  1092,  1093,  1094,   396,
     397,   398,   399,   400,   512,   513,   911,   719,   720,   721,
    1206,   722,  1209,   514,   723,   724,   725,   515,   726,   727,
     728,  1023,   517,   526,   729,   527,   528,   538,  1216,   396,
     397,   398,   399,   400,   539,   540,   396,   397,   398,   399,
     400,   547,   396,   397,   398,   399,   400,   429,   549,   553,
     555,   730,   554,   731,   732,   396,   397,   398,   399,   400,
     557,   562,   733,   734,   735,   736,   737,   738,    85,   564,
      87,   563,    89,   565,    91,   579,    93,  1126,    95,   580,
      97,   582,    99,   583,   101,   739,   278,   279,   280,   281,
     282,   585,   283,   270,   284,   285,   286,   287,   288,   740,
     587,   593,   596,   597,   289,   600,   601,   602,   603,   604,
     612,   270,   610,   613,   614,   619,   635,   624,   634,   631,
     636,   642,   271,   643,  -184,   272,   273,   649,   654,   657,
     669,   684,   686,   687,   688,   290,   658,   691,  1088,    83,
     690,   696,   697,   272,   273,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   698,   231,   703,   699,   700,   701,   704,
      84,   705,    86,   244,    88,   706,    90,   752,    92,   756,
      94,   707,    96,    16,    98,   745,   100,   749,   750,   751,
     754,    17,  1193,   755,   757,   758,   759,   762,   760,    18,
      19,    20,    21,    22,    23,   761,   764,    24,    25,    26,
      27,    28,    29,    30,  1212,   763,  1161,    31,    32,    33,
      34,    35,   765,    36,   767,   768,   770,   771,    37,    38,
      39,    40,    41,    42,    43,    44,   772,    45,    46,    47,
     774,    48,  1178,    49,    50,    51,    52,    53,    54,   778,
     773,   776,    55,    56,   777,    57,   779,   783,   780,   781,
     782,   702,    58,   784,   785,   790,   791,   795,   792,   793,
     325,   787,   805,   794,   806,   807,   808,   566,   810,   816,
     809,   844,   820,    59,   821,   822,    60,   823,   839,   824,
     825,   826,   847,   827,   849,   850,   828,   829,   830,   831,
     832,   833,   834,   835,   836,   837,   838,   932,   933,   840,
     841,    61,   845,   848,   855,   856,   857,    62,   862,    63,
      64,   864,   861,    65,    66,   866,    67,   868,    68,   869,
     875,   879,   880,   885,    69,   901,   903,    70,    71,   917,
     887,   905,   909,    72,    73,   912,    74,   913,    75,   934,
     935,   918,   921,   920,   922,   936,   937,   927,   968,   970,
     971,   974,   938,   973,   976,   979,   975,   977,  1006,   978,
     981,  1007,   982,   986,   987,  1002,  1003,  1004,  1005,  1009,
    1010,  1011,  1019,    76,    77,    78,    79,  1020,    80,  1021,
    1022,  1024,  1028,   939,  1029,  1030,   940,   941,   942,   943,
     944,   945,   946,   947,   948,   949,   950,   951,   952,   953,
     954,   955,    81,  1031,  1032,  1033,  1034,  1035,    82,    83,
    1036,  1037,  1038,  1044,  1046,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,  1047,  1052,  1053,  1057,  1056,  1058,  1069,
    1070,  1059,  1060,  1076,  1071,  1061,   254,  1072,  1073,  1078,
    1099,  1089,  1100,  1062,  1063,  1064,  1065,   255,   256,  1066,
    1067,  1068,  1074,  1101,  1080,  1083,  1104,  1084,  1081,  1102,
    1106,  1085,  1103,  1107,  1108,  1114,  1109,  1116,  1112,  1128,
    1113,  1129,  1130,  1133,  1137,  1138,  1141,  1144,  1142,  1146,
    1147,  1149,  1150,  1151,  1153,  1155,  1156,  1157,  1158,  1162,
    1163,  1164,  1165,  1169,  1166,  1170,  1172,  1173,  1175,  1179,
    1176,  1181,  1182,  1184,  1183,  1188,  1191,  1192,  1194,  1195,
    1199,  1203,  1210,  1213,  1215,   382,    10,   426,  1211,   541,
     232,   802,   225,  1082,   371,  1054,   693,  1077,  1110,  1132,
     980,  1167,   304,   846,  1127,  1143,   689,  1168,   894,   958,
     548,     0,   450,     0,     0,     0,   625,   667,   743
};

static const yytype_int16 yycheck[] =
{
     108,   180,   378,   539,    81,   177,   230,   470,   491,   188,
     538,   190,   466,     3,   414,     3,   503,   196,   197,   678,
     199,   200,   200,   516,     3,     3,   204,   205,   206,     6,
       3,   210,     4,   682,   213,   506,   507,    63,   217,   878,
     219,    63,    88,    89,     4,   414,     4,    88,     8,    88,
      88,     4,    88,     4,    88,    72,   235,   236,   237,   238,
    1008,    88,    88,    89,     4,    88,    89,     4,    96,   540,
     249,   696,     4,    88,     4,     3,     5,   249,     8,     9,
     232,     4,   458,  1075,    84,   461,    88,    89,     5,   195,
      92,    93,  1159,    88,  1131,     3,   232,     4,    88,    80,
      88,   280,   281,   233,   283,   231,   182,    88,     3,  1177,
       4,    18,     0,    83,     8,   191,    88,    89,    88,     4,
    1187,   121,   122,     8,     9,    79,   256,   257,   231,  1197,
     130,   234,   209,   239,    88,   211,   212,  1174,   214,   215,
       3,   141,   218,   136,   231,   221,   231,   223,    88,     4,
     226,   238,   152,   238,   107,   239,   619,   782,   239,    88,
      88,   385,  1154,  1111,   241,   291,   103,    99,   196,   101,
     247,    88,   249,   232,   102,    30,   222,    32,   178,   231,
      88,     3,    37,    38,    39,   137,   203,   233,   111,   232,
     113,   114,   115,   156,   102,   158,   222,   233,   239,   231,
     239,   239,   234,    88,   235,   239,   233,   233,   254,   255,
     233,   233,   250,   251,   232,  1054,   250,   251,   233,   232,
     222,   153,   235,   231,   760,    88,   680,   232,   254,   255,
     238,   233,   231,    88,    89,   232,     4,   896,   233,   238,
       8,     9,   414,   233,     3,   233,   232,   237,     7,   239,
     899,   239,   254,   255,   233,   233,   232,   170,   235,   172,
     233,   233,   749,     3,     4,   489,    88,   239,   447,   448,
     260,   261,   672,   233,   650,   233,   248,   249,   250,   251,
     102,   239,   233,   462,   463,   258,   259,   232,   248,   249,
     248,   249,   250,   251,   149,   823,   750,   248,   249,   478,
     793,   670,   838,   233,   482,   233,   232,   384,   486,   487,
     488,   239,   284,   492,   785,   423,   393,   394,   232,   396,
     397,   398,   399,   400,   284,   233,   284,   790,   232,    88,
      89,   239,   232,   284,   262,   263,   413,   414,   233,   233,
     512,     4,   237,   806,    88,   231,   239,   290,    88,    89,
     427,    95,   238,   816,   248,   249,   232,   250,   251,   435,
     437,   231,   241,   258,   259,   220,   231,   222,   238,   445,
     233,   168,   232,   449,   231,   233,   239,   453,   233,   456,
     231,   238,   237,   460,   239,   564,   565,   232,   464,   232,
     284,   232,   469,   248,   249,   250,   251,   260,   261,   254,
     255,   256,   257,   287,   288,   289,   585,   783,    20,    21,
       3,   233,   232,   231,   231,   237,   234,   239,    93,    94,
     285,   286,   287,   288,   289,    88,    89,    87,    88,   284,
      10,    11,    12,    13,   285,   286,   287,   288,   289,   975,
     262,   263,   978,   285,   286,   287,   288,   289,    88,   526,
      90,    91,   528,    48,    49,    50,   215,   216,   217,   218,
     219,   232,   221,   222,   223,   224,   225,   226,   227,   287,
     288,   289,   244,   245,   233,   215,   216,   217,   218,   219,
     232,   221,   222,   223,   224,   225,   226,   227,   285,   286,
     287,   288,   289,   233,   291,   254,   255,   232,   236,   258,
     259,    92,    93,   232,   676,   264,  1034,   213,  1064,  1065,
      87,    88,    89,   232,   254,   255,   236,   232,   232,   895,
     232,   232,   232,   232,   264,   232,  1062,   232,   232,  1012,
     236,   232,   232,   232,   232,  1063,   285,   286,   287,   288,
     289,   232,   232,   232,   232,   232,   770,   285,   286,   287,
     288,   289,   215,   216,   217,   218,   219,   107,   221,   222,
     223,   224,   225,   226,   227,   285,   286,   287,   288,   289,
     233,   232,     4,  1109,   232,   232,   653,   232,   232,   285,
     286,   287,   288,   289,   232,   232,   232,    88,    89,   232,
     232,   254,   255,   232,   232,   774,   232,   674,   822,   232,
     772,   264,   826,   232,   232,   232,   830,   831,   832,   239,
     100,   835,   836,   837,   236,   231,   233,   158,   234,   791,
     290,   697,   699,   700,   193,   701,   703,   231,   704,   705,
     234,   233,   201,   202,   235,   235,   235,  1173,   215,   216,
     217,   218,   219,   237,   221,   222,   223,   224,   225,   226,
     227,   233,   231,   222,     4,   231,   233,   231,   227,  1195,
     231,   833,   230,   285,   286,   287,   288,   289,   840,   285,
     286,   287,   288,   289,   235,   231,   231,   254,   255,   233,
     249,   285,   286,   287,   288,   289,   234,   264,   233,   233,
     233,   233,   231,     3,   195,    98,   293,   235,     3,   235,
    1163,   231,   105,     4,   237,   231,   231,     8,     9,   278,
     279,   231,   231,  1176,   215,   216,   217,   218,   219,   231,
     221,   222,   223,   224,   225,   226,   227,   130,     4,   235,
     233,   231,   233,   810,   231,     4,   231,   285,   286,   287,
     288,   289,   231,   820,   231,   231,   231,   231,     4,   152,
     231,   231,   231,   254,   255,     6,   231,   231,   982,   231,
     236,   231,     4,   264,    65,   231,   938,   231,   231,    88,
      89,   231,   231,   231,   231,   231,   231,   231,    79,    80,
      81,    82,   231,    84,   231,    86,   231,    88,    89,   231,
     236,   231,   236,   196,   197,   198,   199,   236,   201,   202,
     203,   204,   205,   236,  1028,  1029,  1030,  1031,  1032,   285,
     286,   287,   288,   289,   231,   231,   236,   118,   119,   120,
    1196,   122,  1198,   231,   125,   126,   127,   231,   129,   130,
     131,   908,   234,   231,   135,   234,   232,   232,  1214,   285,
     286,   287,   288,   289,   232,   232,   285,   286,   287,   288,
     289,    15,   285,   286,   287,   288,   289,   291,     3,     3,
     236,   162,   231,   164,   165,   285,   286,   287,   288,   289,
     231,     5,   173,   174,   175,   176,   177,   178,   249,   231,
     251,   240,   253,   231,   255,   228,   257,  1066,   259,   234,
     261,   103,   263,   231,   265,   196,   215,   216,   217,   218,
     219,   231,   221,   222,   223,   224,   225,   226,   227,   210,
     231,    87,    87,   231,   233,    45,     4,   231,    45,    47,
      97,   222,   196,   231,    97,   231,   107,   231,   200,   101,
     110,   144,   233,   144,   148,   254,   255,   159,   236,   154,
     231,   238,   231,     3,   231,   264,   155,     4,  1025,   242,
     234,   292,   231,   254,   255,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   234,   242,   235,   231,   231,   231,   231,
     248,   231,   250,   284,   252,   231,   254,     4,   256,     4,
     258,   239,   260,     6,   262,   239,   264,   231,   231,   231,
     231,    14,  1181,   231,   231,   235,   231,     4,   232,    22,
      23,    24,    25,    26,    27,   231,     4,    30,    31,    32,
      33,    34,    35,    36,  1203,   231,  1134,    40,    41,    42,
      43,    44,   231,    46,   231,   231,   231,    87,    51,    52,
      53,    54,    55,    56,    57,    58,   231,    60,    61,    62,
     232,    64,  1160,    66,    67,    68,    69,    70,    71,     4,
     231,   231,    75,    76,   231,    78,   231,   148,   231,   231,
     231,     3,    85,   231,   231,   231,   231,     4,   232,   231,
     233,   291,    16,   234,   231,     3,   234,    87,   231,   231,
     240,     5,   233,   106,   234,   232,   109,   232,   234,   232,
     232,   232,     4,   232,   196,     4,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,    90,    91,   231,
     231,   134,   231,   231,   196,    97,   231,   140,    97,   142,
     143,   231,   239,   146,   147,    73,   149,   231,   151,   104,
     231,   104,     4,   104,   157,   231,   231,   160,   161,   117,
     237,   234,   234,   166,   167,   234,   169,   234,   171,   132,
     133,   116,   124,   123,   128,   138,   139,   163,   231,     4,
     231,     4,   145,   231,   236,     4,   232,   231,     4,   232,
     231,     4,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,    17,   206,   207,   208,   209,   234,   211,     5,
      48,   234,   232,   176,   232,   232,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   235,   232,   232,   232,   232,   232,   241,   242,
     232,     3,   231,   108,     4,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   231,   234,   231,    74,   234,    99,   111,
     111,   232,   232,     4,   111,   232,   239,   111,   234,     7,
       4,    88,   153,   232,   232,   232,   232,   250,   251,   232,
     232,   232,   231,     4,   232,   231,   105,   231,   238,   231,
     231,   234,   232,     4,   231,   104,   232,     4,   231,     4,
     231,   231,     4,   232,     5,    48,   292,   107,   234,   232,
      28,     4,   234,   196,   234,    99,    98,   112,   234,   231,
     231,   234,   229,   232,   234,    29,   234,   232,   231,    19,
     231,   231,   103,   196,   232,    98,   231,   231,     4,   232,
       4,   231,   231,   234,     4,   229,     5,   253,  1202,   422,
      73,   680,    66,  1015,   220,   981,   561,  1010,  1053,  1077,
     859,  1141,   184,   754,  1067,  1102,   554,  1146,   793,   840,
     429,    -1,   287,    -1,    -1,    -1,   491,   526,   585
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    20,    21,   295,   296,   297,   298,   232,   232,     0,
     296,   136,   308,   239,   239,   232,     6,    14,    22,    23,
      24,    25,    26,    27,    30,    31,    32,    33,    34,    35,
      36,    40,    41,    42,    43,    44,    46,    51,    52,    53,
      54,    55,    56,    57,    58,    60,    61,    62,    64,    66,
      67,    68,    69,    70,    71,    75,    76,    78,    85,   106,
     109,   134,   140,   142,   143,   146,   147,   149,   151,   157,
     160,   161,   166,   167,   169,   171,   206,   207,   208,   209,
     211,   235,   241,   242,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   299,   300,   301,   302,   303,   310,   312,   313,
     314,   315,   317,   318,   326,   329,   330,   332,   333,   340,
     342,   343,   345,   347,   349,   353,   354,   355,   358,   360,
     367,   374,   377,   383,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   399,   400,   401,   402,   406,   407,
     409,   410,   411,   412,   413,   414,   415,   416,   417,   420,
     421,   422,   423,   424,   425,   426,   427,   428,   429,   430,
     431,   432,   433,   434,   231,   137,   309,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   332,   232,   232,   232,   232,
     235,   242,   314,   316,   334,   232,   232,   232,   232,   241,
       4,   233,   248,   249,   284,   467,   477,   233,   310,   290,
     232,   344,     3,   231,   239,   250,   251,   453,   466,    10,
      11,    12,    13,     4,   103,   478,    88,    89,    92,    93,
     222,   233,   254,   255,   375,   455,   456,   107,   215,   216,
     217,   218,   219,   221,   223,   224,   225,   226,   227,   233,
     264,   455,   475,   476,     3,    88,   102,   233,   239,   262,
     263,   385,   464,   465,   385,     3,    88,   233,   239,   260,
     261,   379,   462,   463,    88,   239,   435,    79,    88,   447,
     455,     4,   455,   195,   476,   233,   256,   257,   458,   459,
     458,    63,   233,   485,   486,   239,   455,   455,   100,   455,
     233,   455,   485,   458,   458,    88,   239,   357,   436,   485,
     485,   485,   454,   467,   233,   356,   467,   455,   476,   476,
     455,   476,   476,     4,     8,     9,    88,   479,   455,   476,
     455,   357,   476,   458,   476,     5,    88,   368,   350,   476,
     458,   158,   313,   454,   290,   235,   455,   455,   455,   455,
     233,     8,   467,   235,   213,   236,   285,   286,   287,   288,
     289,   466,   467,   472,   473,   474,    30,    32,    37,    38,
      39,   149,   220,   233,   237,   455,   458,   466,   467,   481,
     482,   483,   348,   237,   311,   231,   309,   235,   231,   291,
     231,   231,   231,   230,   457,   233,     4,   235,   231,   231,
     362,   458,   458,   455,   455,   233,   455,   233,   233,   233,
     475,   233,   476,   293,   231,     3,   235,   386,   384,     3,
     235,   378,   231,   231,   231,   231,   231,   231,     4,   235,
     231,   231,   233,     4,   231,   231,   231,   231,   231,   231,
       4,   231,   231,   231,     6,   331,   231,   231,   231,   231,
       4,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   311,   351,   231,   231,   231,   236,
     467,   454,   231,   231,   231,   231,   327,   234,   234,   467,
     467,   467,   467,   467,   467,   467,   231,   234,   232,     4,
     322,   323,   324,   325,   456,   459,   466,   467,   232,   232,
     232,   312,   310,    80,    88,   448,   467,    15,   453,     3,
       3,   233,   408,     3,   231,   236,   476,   231,   467,    93,
      94,   376,     5,   240,   231,   231,    87,   370,   476,   455,
     455,     3,     7,   233,   258,   259,   460,   461,   476,   228,
     234,   476,   103,   231,   467,   231,   311,   231,   467,   311,
     455,   455,   476,    87,   320,   463,    87,   231,   467,   448,
      45,     4,   231,    45,    47,    88,    90,    91,   449,   452,
     196,   455,    97,   231,    97,   485,    88,   233,   363,   231,
     485,   485,   485,   454,   231,   356,   479,   455,    83,    88,
     484,   101,    72,   203,   200,   107,   110,    88,   233,   364,
       4,   107,   144,   144,   447,   352,   233,   460,   460,   159,
     346,   168,   291,   341,   236,    88,   466,   154,   155,   156,
     158,   315,   469,   470,   471,   236,   236,   472,   476,   231,
     231,   238,   231,   238,   231,   238,   231,   238,   237,   464,
     237,   462,   237,   460,   238,   236,   231,     3,   231,   457,
     234,     4,   236,   362,   455,   455,   292,   231,   234,   231,
     231,   231,     3,   235,   231,   231,   231,   239,   236,     4,
       8,     9,    65,    79,    80,    81,    82,    84,    86,   118,
     119,   120,   122,   125,   126,   127,   129,   130,   131,   135,
     162,   164,   165,   173,   174,   175,   176,   177,   178,   196,
     210,   387,   455,   477,   480,   239,   236,   380,   372,   231,
     231,   231,     4,   236,   231,   231,     4,   231,   235,   231,
     232,   231,     4,   231,     4,   231,   448,   231,   231,   398,
     231,    87,   231,   231,   232,   403,   231,   231,     4,   231,
     231,   231,   231,   148,   231,   231,   311,   291,   467,   335,
     231,   231,   232,   231,   234,     4,   459,   456,   467,   466,
     319,   465,   320,   321,   461,    16,   231,     3,   234,   240,
     231,   370,   476,   467,   467,   476,   231,   467,   476,   476,
     233,   234,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   234,
     231,   231,   447,   463,     5,   231,   436,     4,   231,   196,
       4,    88,    95,   451,   462,   196,    97,   231,    88,   233,
     365,   239,    97,   454,   231,   466,    73,   455,   231,   104,
       4,   111,   113,   114,   115,   231,    88,   233,   366,   104,
       4,    88,   369,   370,   311,   104,   460,   237,   448,    88,
     466,   244,   245,   468,   469,   328,   231,   238,   238,   231,
     238,   231,   448,   231,   467,   234,   231,   234,   231,   234,
     448,   236,   234,   234,   467,   454,   464,   117,   116,   454,
     123,   124,   128,   454,   454,   454,   466,   163,   454,   454,
     454,   462,    90,    91,   132,   133,   138,   139,   145,   176,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   381,   450,   452,   466,
      84,   121,   122,   130,   141,   152,   178,   373,   231,   306,
       4,   231,   437,   231,     4,   232,   236,   231,   232,     4,
     375,   231,   231,     4,    88,   371,   231,   231,    98,   105,
     130,   152,   196,   197,   198,   199,   201,   202,   203,   204,
     205,   404,   231,   231,   231,   231,     4,     4,   437,   231,
     231,   231,   170,   172,   336,   337,   311,   465,   461,    17,
     234,     5,    48,   467,   234,   286,   232,   466,   232,   232,
     232,   232,   232,   232,   232,   232,   232,     3,   231,    48,
      49,    50,   438,   439,   108,   440,     4,   231,   462,    96,
     196,   462,   234,   231,   366,   454,   234,    74,    99,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   111,
     111,   111,   111,   234,   231,   440,     4,   371,     7,   479,
     232,   238,   336,   231,   231,   234,   231,   234,   467,    88,
     454,   454,   454,   454,   454,   195,   239,   382,   464,     4,
     153,     4,   231,   232,   105,   442,   231,     4,   231,   232,
     376,   437,   231,   231,   104,   443,     4,     4,    99,   101,
     153,   405,   462,   464,   408,   408,   455,   435,     4,   231,
       4,   442,   372,   232,   339,     4,    18,     5,    48,   231,
     304,   292,   234,   438,   107,   441,   232,    28,   445,     4,
     234,   196,   462,   234,   440,    99,    98,   112,   234,   445,
     338,   310,   231,   231,   234,   229,   234,   382,   443,   232,
      29,   446,   234,   232,   442,   231,   231,   446,   310,    19,
     448,   231,   103,   232,   196,   444,   462,   445,    98,   448,
     444,   231,   231,   455,     4,   232,   418,   446,   359,     4,
       5,    88,   307,   231,   305,   462,   311,   419,   444,   311,
     231,   306,   455,   234,   361,     4,   311
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
		      add_opvars(splev, "iiiiiiio", VA_PASS8(0, 0, 0, 0, NO_COLOR, ATR_NONE, SPLEV_MESSAGE_TYPE_MESSAGE, SPO_MESSAGE));
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1282 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiio", VA_PASS8((int)(yyvsp[(11) - (11)].i), 0, 0, 0, (int)(yyvsp[(9) - (11)].i), (int)(yyvsp[(7) - (11)].i), (int)(yyvsp[(5) - (11)].i), SPO_MESSAGE));
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1286 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiio", VA_PASS8((int)(yyvsp[(17) - (17)].i), (int)(yyvsp[(15) - (17)].i), (int)(yyvsp[(13) - (17)].i), (int)(yyvsp[(11) - (17)].i), (int)(yyvsp[(9) - (17)].i), (int)(yyvsp[(7) - (17)].i), (int)(yyvsp[(5) - (17)].i), SPO_MESSAGE));
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1292 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1297 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1302 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1309 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1316 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1325 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1333 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1344 "lev_comp.y"
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

  case 197:

/* Line 1455 of yacc.c  */
#line 1362 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1369 "lev_comp.y"
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

  case 199:

/* Line 1455 of yacc.c  */
#line 1387 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1394 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1398 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1404 "lev_comp.y"
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

  case 203:

/* Line 1455 of yacc.c  */
#line 1414 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1420 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1429 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1435 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1440 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1446 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1451 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1457 "lev_comp.y"
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

  case 211:

/* Line 1455 of yacc.c  */
#line 1468 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1482 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1486 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1496 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1503 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1511 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1516 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1521 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1526 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1531 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1536 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1541 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1549 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1556 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1564 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1580 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1584 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1590 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1598 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1604 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1612 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1620 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1625 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1631 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1637 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1643 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1649 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1654 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(0, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1659 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1664 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1669 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1674 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1679 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1684 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1689 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1694 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1699 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1704 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1709 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1715 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2(0xFFFFFFFFL, SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1721 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1726 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_PROTECTOR));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1731 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_MAXHP));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1736 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_LEVEL_ADJUSTMENT));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1741 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_KEEP_ORIGINAL_INVENTORY));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1746 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_LEVEL_BOSS));
		      (yyval.i) = 0x00200000;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1751 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_BOSS_HOSTILITY));
		      (yyval.i) = 0x00400000;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1758 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1767 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1771 "lev_comp.y"
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

  case 271:

/* Line 1455 of yacc.c  */
#line 1784 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1792 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1802 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1810 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1819 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1826 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1834 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1840 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1845 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1850 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1855 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1860 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1865 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1870 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1875 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1880 "lev_comp.y"
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

  case 287:

/* Line 1455 of yacc.c  */
#line 1894 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1900 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1905 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1910 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1915 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1920 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1925 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1930 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1935 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1940 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1945 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1950 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1955 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x00200000;
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1960 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x00400000;
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1965 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x00800000;
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 1970 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x01000000;
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 1975 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_MYTHIC_TYPE));
		      (yyval.i) = 0x02000000;
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 1981 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x04000000;
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 1986 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x08000000;
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 1991 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x10000000;
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 1996 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MATERIAL));
		      (yyval.i) = 0x20000000;
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2003 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2009 "lev_comp.y"
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

  case 310:

/* Line 1455 of yacc.c  */
#line 2038 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2043 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2051 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2057 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2061 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2069 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2076 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2083 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2093 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2103 "lev_comp.y"
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

  case 320:

/* Line 1455 of yacc.c  */
#line 2119 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2129 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2133 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2139 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2145 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2151 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2155 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2161 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2167 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2174 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2182 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2188 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2193 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2198 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2203 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2208 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2214 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2220 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2225 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2230 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2235 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2240 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2245 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2255 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2261 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2267 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2272 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2279 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2286 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2290 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2296 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2302 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2308 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2313 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2320 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2326 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2336 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2342 "lev_comp.y"
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

  case 362:

/* Line 1455 of yacc.c  */
#line 2363 "lev_comp.y"
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

  case 363:

/* Line 1455 of yacc.c  */
#line 2375 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2379 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2385 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2390 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2395 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (long)(yyvsp[(9) - (9)].i), (long)(yyvsp[(7) - (9)].i), (long)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2400 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((long)(yyvsp[(9) - (13)].i), (long)(yyvsp[(7) - (13)].i), (long)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2406 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2410 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2416 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, (int)(yyvsp[(11) - (11)].i), (int)(yyvsp[(9) - (11)].i), (int)(yyvsp[(7) - (11)].i), (int)(yyvsp[(5) - (11)].i), SPO_DECORATION));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2420 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6((int)(yyvsp[(13) - (13)].i), (int)(yyvsp[(11) - (13)].i), (int)(yyvsp[(9) - (13)].i), (int)(yyvsp[(7) - (13)].i), (int)(yyvsp[(5) - (13)].i), SPO_DECORATION));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2424 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2428 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2432 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2436 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, 1, (int)(yyvsp[(7) - (7)].i), 0, (int)(yyvsp[(5) - (7)].i), SPO_DECORATION));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2442 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2448 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2452 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2458 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_CARPET));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2464 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2468 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4(0, (int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2474 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2478 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2484 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2488 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2493 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2500 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2504 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2511 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2515 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2520 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2527 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2531 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2536 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2544 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2550 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2557 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2561 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2568 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2579 "lev_comp.y"
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

  case 404:

/* Line 1455 of yacc.c  */
#line 2592 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2596 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2602 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2606 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2613 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2617 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2621 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2627 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2631 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2637 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2643 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2647 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2653 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2659 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2664 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2670 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2674 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2680 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2684 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2700 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2708 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2718 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2727 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2732 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2739 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2750 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2756 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2760 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2764 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2771 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2781 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2788 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2792 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2798 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2802 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2810 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2814 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2821 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2831 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2844 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2848 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2855 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2865 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_LEVTYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2874 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_LEVTYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2885 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2889 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2896 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2906 "lev_comp.y"
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

  case 462:

/* Line 1455 of yacc.c  */
#line 2917 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2926 "lev_comp.y"
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

  case 464:

/* Line 1455 of yacc.c  */
#line 2936 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2942 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2946 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2953 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2963 "lev_comp.y"
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

  case 469:

/* Line 1455 of yacc.c  */
#line 2974 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2983 "lev_comp.y"
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

  case 471:

/* Line 1455 of yacc.c  */
#line 2993 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 2997 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3003 "lev_comp.y"
    { ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3005 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3011 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3015 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3019 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3023 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3031 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3040 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3044 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3048 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3052 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3056 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3059 "lev_comp.y"
    { ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3063 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3070 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3079 "lev_comp.y"
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

  case 493:

/* Line 1455 of yacc.c  */
#line 3120 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3124 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3131 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3138 "lev_comp.y"
    {
			      size_t len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3148 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3152 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3160 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3164 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3168 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3172 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3176 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3181 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3185 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3189 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3194 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3199 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3204 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3208 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3213 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3218 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3222 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3226 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 3231 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 516:

/* Line 1455 of yacc.c  */
#line 3235 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 517:

/* Line 1455 of yacc.c  */
#line 3242 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 518:

/* Line 1455 of yacc.c  */
#line 3248 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 519:

/* Line 1455 of yacc.c  */
#line 3252 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 520:

/* Line 1455 of yacc.c  */
#line 3258 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 526:

/* Line 1455 of yacc.c  */
#line 3274 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 527:

/* Line 1455 of yacc.c  */
#line 3278 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 528:

/* Line 1455 of yacc.c  */
#line 3282 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 529:

/* Line 1455 of yacc.c  */
#line 3286 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 538:

/* Line 1455 of yacc.c  */
#line 3308 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 539:

/* Line 1455 of yacc.c  */
#line 3312 "lev_comp.y"
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

  case 540:

/* Line 1455 of yacc.c  */
#line 3338 "lev_comp.y"
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
#line 7600 "lev_comp.tab.c"
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
#line 3366 "lev_comp.y"


/*lev_comp.y*/

