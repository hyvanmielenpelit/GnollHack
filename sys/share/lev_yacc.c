
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
     MESSAGE_ATTR = 270,
     MESSAGE_COLOR = 271,
     MESSAGE_SOUND_TYPE = 272,
     MESSAGE_SOUND_ID = 273,
     MAZE_ID = 274,
     LEVEL_ID = 275,
     LEV_INIT_ID = 276,
     TILESET_ID = 277,
     GEOMETRY_ID = 278,
     NOMAP_ID = 279,
     BOUNDARY_TYPE_ID = 280,
     SPECIAL_TILESET_ID = 281,
     TILESET_PARAM_ID = 282,
     DECOTYP_ID = 283,
     OBJECT_ID = 284,
     COBJECT_ID = 285,
     MONSTER_ID = 286,
     TRAP_ID = 287,
     DOOR_ID = 288,
     DRAWBRIDGE_ID = 289,
     MONSTER_GENERATION_ID = 290,
     object_ID = 291,
     monster_ID = 292,
     terrain_ID = 293,
     MAZEWALK_ID = 294,
     WALLIFY_ID = 295,
     REGION_ID = 296,
     SPECIAL_REGION_ID = 297,
     SPECIAL_LEVREGION_ID = 298,
     SPECIAL_REGION_TYPE = 299,
     NAMING_ID = 300,
     NAMING_TYPE = 301,
     FILLING = 302,
     IRREGULAR = 303,
     JOINED = 304,
     ALTAR_ID = 305,
     ANVIL_ID = 306,
     NPC_ID = 307,
     LADDER_ID = 308,
     STAIR_ID = 309,
     NON_DIGGABLE_ID = 310,
     NON_PASSWALL_ID = 311,
     ROOM_ID = 312,
     ARTIFACT_NAME_ID = 313,
     PORTAL_ID = 314,
     TELEPRT_ID = 315,
     BRANCH_ID = 316,
     LEV = 317,
     MINERALIZE_ID = 318,
     AGE_ID = 319,
     CORRIDOR_ID = 320,
     GOLD_ID = 321,
     ENGRAVING_ID = 322,
     FOUNTAIN_ID = 323,
     THRONE_ID = 324,
     MODRON_PORTAL_ID = 325,
     LEVEL_TELEPORTER_ID = 326,
     LEVEL_TELEPORT_DIRECTION_TYPE = 327,
     LEVEL_TELEPORT_END_TYPE = 328,
     POOL_ID = 329,
     SINK_ID = 330,
     NONE = 331,
     RAND_CORRIDOR_ID = 332,
     DOOR_STATE = 333,
     LIGHT_STATE = 334,
     CURSE_TYPE = 335,
     MYTHIC_TYPE = 336,
     ENGRAVING_TYPE = 337,
     KEYTYPE_ID = 338,
     LEVER_ID = 339,
     NO_PICKUP_ID = 340,
     DIRECTION = 341,
     RANDOM_TYPE = 342,
     RANDOM_TYPE_BRACKET = 343,
     A_REGISTER = 344,
     ALIGNMENT = 345,
     LEFT_OR_RIGHT = 346,
     CENTER = 347,
     TOP_OR_BOT = 348,
     ALTAR_TYPE = 349,
     ALTAR_SUBTYPE = 350,
     UP_OR_DOWN = 351,
     ACTIVE_OR_INACTIVE = 352,
     MODRON_PORTAL_TYPE = 353,
     NPC_TYPE = 354,
     FOUNTAIN_TYPE = 355,
     SPECIAL_OBJECT_TYPE = 356,
     CMAP_TYPE = 357,
     FLOOR_SUBTYPE = 358,
     FLOOR_SUBTYPE_ID = 359,
     FLOOR_ID = 360,
     FLOOR_TYPE = 361,
     FLOOR_TYPE_ID = 362,
     DECORATION_ID = 363,
     DECORATION_TYPE = 364,
     DECORATION_DIR = 365,
     DECORATION_ITEM_STATE = 366,
     PAINTING_TYPE = 367,
     BANNER_TYPE = 368,
     WALL_SCULPTURE_TYPE = 369,
     ELEMENTAL_ENCHANTMENT_TYPE = 370,
     EXCEPTIONALITY_TYPE = 371,
     EXCEPTIONALITY_ID = 372,
     ELEMENTAL_ENCHANTMENT_ID = 373,
     ENCHANTMENT_ID = 374,
     SECRET_DOOR_ID = 375,
     USES_UP_KEY_ID = 376,
     MYTHIC_PREFIX_TYPE = 377,
     MYTHIC_SUFFIX_TYPE = 378,
     MYTHIC_PREFIX_ID = 379,
     MYTHIC_SUFFIX_ID = 380,
     MATERIAL_ID = 381,
     MATERIAL_TYPE = 382,
     CHARGES_ID = 383,
     SPECIAL_QUALITY_ID = 384,
     SPEFLAGS_ID = 385,
     SUBROOM_ID = 386,
     NAME_ID = 387,
     FLAGS_ID = 388,
     FLAG_TYPE = 389,
     MON_ATTITUDE = 390,
     MON_ALERTNESS = 391,
     SUBTYPE_ID = 392,
     NON_PASSDOOR_ID = 393,
     CARPET_ID = 394,
     CARPET_PIECE_ID = 395,
     CARPET_TYPE = 396,
     MON_APPEARANCE = 397,
     ROOMDOOR_ID = 398,
     IF_ID = 399,
     ELSE_ID = 400,
     TERRAIN_ID = 401,
     HORIZ_OR_VERT = 402,
     REPLACE_TERRAIN_ID = 403,
     LOCATION_SUBTYPE_ID = 404,
     DOOR_SUBTYPE = 405,
     BRAZIER_SUBTYPE = 406,
     SIGNPOST_SUBTYPE = 407,
     TREE_SUBTYPE = 408,
     FOREST_ID = 409,
     FOREST_TYPE = 410,
     INITIALIZE_TYPE = 411,
     EXIT_ID = 412,
     SHUFFLE_ID = 413,
     MANUAL_TYPE_ID = 414,
     MANUAL_TYPE = 415,
     QUANTITY_ID = 416,
     BURIED_ID = 417,
     LOOP_ID = 418,
     FOR_ID = 419,
     TO_ID = 420,
     SWITCH_ID = 421,
     CASE_ID = 422,
     BREAK_ID = 423,
     DEFAULT_ID = 424,
     ERODED_ID = 425,
     TRAPPED_STATE = 426,
     RECHARGED_ID = 427,
     INVIS_ID = 428,
     GREASED_ID = 429,
     INDESTRUCTIBLE_ID = 430,
     FEMALE_ID = 431,
     MALE_ID = 432,
     WAITFORU_ID = 433,
     PROTECTOR_ID = 434,
     CANCELLED_ID = 435,
     REVIVED_ID = 436,
     AVENGE_ID = 437,
     FLEEING_ID = 438,
     BLINDED_ID = 439,
     MAXHP_ID = 440,
     LEVEL_ADJUSTMENT_ID = 441,
     KEEP_ORIGINAL_INVENTORY_ID = 442,
     PARALYZED_ID = 443,
     STUNNED_ID = 444,
     CONFUSED_ID = 445,
     SEENTRAPS_ID = 446,
     ALL_ID = 447,
     MONTYPE_ID = 448,
     OBJTYPE_ID = 449,
     TERTYPE_ID = 450,
     TERTYPE2_ID = 451,
     LEVER_EFFECT_TYPE = 452,
     SWITCHABLE_ID = 453,
     CONTINUOUSLY_USABLE_ID = 454,
     TARGET_ID = 455,
     TRAPTYPE_ID = 456,
     EFFECT_FLAG_ID = 457,
     GRAVE_ID = 458,
     BRAZIER_ID = 459,
     SIGNPOST_ID = 460,
     TREE_ID = 461,
     ERODEPROOF_ID = 462,
     FUNCTION_ID = 463,
     MSG_OUTPUT_TYPE = 464,
     COMPARE_TYPE = 465,
     UNKNOWN_TYPE = 466,
     rect_ID = 467,
     fillrect_ID = 468,
     line_ID = 469,
     randline_ID = 470,
     grow_ID = 471,
     selection_ID = 472,
     flood_ID = 473,
     rndcoord_ID = 474,
     circle_ID = 475,
     ellipse_ID = 476,
     filter_ID = 477,
     complement_ID = 478,
     gradient_ID = 479,
     GRADIENT_TYPE = 480,
     LIMITED = 481,
     HUMIDITY_TYPE = 482,
     STRING = 483,
     MAP_ID = 484,
     NQSTRING = 485,
     VARSTRING = 486,
     CFUNC = 487,
     CFUNC_INT = 488,
     CFUNC_STR = 489,
     CFUNC_COORD = 490,
     CFUNC_REGION = 491,
     VARSTRING_INT = 492,
     VARSTRING_INT_ARRAY = 493,
     VARSTRING_STRING = 494,
     VARSTRING_STRING_ARRAY = 495,
     VARSTRING_VAR = 496,
     VARSTRING_VAR_ARRAY = 497,
     VARSTRING_COORD = 498,
     VARSTRING_COORD_ARRAY = 499,
     VARSTRING_REGION = 500,
     VARSTRING_REGION_ARRAY = 501,
     VARSTRING_MAPCHAR = 502,
     VARSTRING_MAPCHAR_ARRAY = 503,
     VARSTRING_MONST = 504,
     VARSTRING_MONST_ARRAY = 505,
     VARSTRING_OBJ = 506,
     VARSTRING_OBJ_ARRAY = 507,
     VARSTRING_SEL = 508,
     VARSTRING_SEL_ARRAY = 509,
     METHOD_INT = 510,
     METHOD_INT_ARRAY = 511,
     METHOD_STRING = 512,
     METHOD_STRING_ARRAY = 513,
     METHOD_VAR = 514,
     METHOD_VAR_ARRAY = 515,
     METHOD_COORD = 516,
     METHOD_COORD_ARRAY = 517,
     METHOD_REGION = 518,
     METHOD_REGION_ARRAY = 519,
     METHOD_MAPCHAR = 520,
     METHOD_MAPCHAR_ARRAY = 521,
     METHOD_MONST = 522,
     METHOD_MONST_ARRAY = 523,
     METHOD_OBJ = 524,
     METHOD_OBJ_ARRAY = 525,
     METHOD_SEL = 526,
     METHOD_SEL_ARRAY = 527,
     DICE = 528
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
#line 573 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 585 "lev_comp.tab.c"

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
#define YYLAST   1372

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  291
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  193
/* YYNRULES -- Number of rules.  */
#define YYNRULES  538
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1213

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   528

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   286,   290,     2,
     230,   231,   284,   282,   228,   283,   288,   285,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   229,     2,
       2,   287,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   232,     2,   233,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   234,   289,   235,     2,     2,     2,     2,
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
     225,   226,   227,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281
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
     525,   530,   531,   536,   538,   539,   544,   548,   554,   570,
     572,   576,   580,   586,   592,   600,   605,   606,   622,   623,
     641,   642,   645,   651,   653,   659,   661,   667,   669,   675,
     677,   688,   695,   697,   699,   701,   703,   705,   709,   711,
     713,   714,   718,   722,   726,   730,   732,   734,   736,   738,
     740,   748,   754,   756,   758,   760,   762,   766,   767,   773,
     778,   779,   783,   785,   787,   789,   791,   794,   796,   798,
     800,   802,   804,   808,   810,   814,   818,   822,   824,   826,
     830,   832,   834,   836,   838,   842,   844,   846,   848,   852,
     856,   857,   863,   866,   867,   871,   873,   877,   879,   883,
     887,   889,   891,   895,   897,   899,   901,   905,   907,   909,
     911,   915,   919,   923,   927,   931,   935,   939,   943,   945,
     947,   949,   951,   955,   959,   963,   967,   973,   981,   987,
     996,   998,  1002,  1006,  1012,  1018,  1026,  1034,  1041,  1047,
    1048,  1051,  1057,  1061,  1073,  1087,  1094,  1095,  1099,  1101,
    1105,  1109,  1113,  1117,  1119,  1121,  1125,  1129,  1133,  1137,
    1141,  1145,  1147,  1149,  1151,  1153,  1157,  1161,  1163,  1169,
    1179,  1185,  1193,  1197,  1201,  1207,  1217,  1223,  1229,  1235,
    1236,  1252,  1253,  1255,  1263,  1275,  1285,  1299,  1303,  1311,
    1323,  1337,  1347,  1357,  1367,  1375,  1383,  1389,  1397,  1403,
    1413,  1421,  1427,  1437,  1443,  1449,  1453,  1461,  1465,  1473,
    1481,  1485,  1491,  1497,  1501,  1507,  1515,  1525,  1527,  1529,
    1531,  1533,  1535,  1536,  1539,  1541,  1545,  1547,  1549,  1551,
    1552,  1556,  1558,  1559,  1563,  1565,  1566,  1570,  1571,  1575,
    1576,  1580,  1582,  1584,  1586,  1588,  1590,  1592,  1594,  1596,
    1598,  1602,  1604,  1606,  1611,  1613,  1615,  1620,  1622,  1624,
    1629,  1631,  1636,  1642,  1644,  1648,  1650,  1654,  1656,  1658,
    1663,  1673,  1675,  1677,  1682,  1684,  1690,  1692,  1694,  1699,
    1701,  1703,  1709,  1711,  1713,  1715,  1720,  1722,  1724,  1730,
    1732,  1734,  1736,  1740,  1742,  1744,  1748,  1750,  1755,  1759,
    1763,  1767,  1771,  1775,  1779,  1781,  1783,  1787,  1789,  1793,
    1794,  1796,  1798,  1800,  1802,  1806,  1807,  1809,  1811,  1814,
    1817,  1822,  1829,  1834,  1841,  1848,  1855,  1862,  1865,  1872,
    1881,  1890,  1901,  1916,  1919,  1921,  1925,  1927,  1931,  1933,
    1935,  1937,  1939,  1941,  1943,  1945,  1947,  1949,  1951,  1953,
    1955,  1957,  1959,  1961,  1963,  1965,  1967,  1969,  1980
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     292,     0,    -1,    -1,   293,    -1,   294,    -1,   294,   293,
      -1,   295,   305,   307,    -1,    20,   229,   236,    -1,    19,
     229,   236,   228,     3,    -1,    19,   229,   236,   228,     3,
     228,   445,    -1,    21,   229,    11,   228,   405,    -1,    21,
     229,    10,   228,     3,    -1,    21,   229,    13,    -1,    21,
     229,    12,   228,     3,   228,     3,   228,     5,   228,     5,
     228,   445,   228,   304,   303,    -1,    22,   229,   475,    -1,
     154,   229,   155,   228,   156,    -1,   154,   229,   155,    -1,
      35,   229,     4,   228,   317,    -1,    25,   229,   106,    -1,
      -1,   228,   226,    -1,    -1,   228,   452,    -1,    -1,   228,
       3,    -1,     5,    -1,    87,    -1,    -1,   133,   229,   306,
      -1,   134,   228,   306,    -1,   134,    -1,    -1,   309,   307,
      -1,   234,   307,   235,    -1,   350,    -1,   296,    -1,   297,
      -1,   413,    -1,   300,    -1,   298,    -1,   421,    -1,   422,
      -1,   423,    -1,   299,    -1,   420,    -1,   419,    -1,   417,
      -1,   418,    -1,   424,    -1,   425,    -1,   426,    -1,   427,
      -1,   428,    -1,   394,    -1,   352,    -1,   315,    -1,   314,
      -1,   408,    -1,   364,    -1,   386,    -1,   430,    -1,   431,
      -1,   396,    -1,   397,    -1,   398,    -1,   429,    -1,   330,
      -1,   340,    -1,   342,    -1,   346,    -1,   344,    -1,   327,
      -1,   337,    -1,   323,    -1,   326,    -1,   389,    -1,   399,
      -1,   371,    -1,   387,    -1,   374,    -1,   380,    -1,   409,
      -1,   404,    -1,   392,    -1,   351,    -1,   410,    -1,   411,
      -1,   412,    -1,   414,    -1,   357,    -1,   355,    -1,   403,
      -1,   407,    -1,   406,    -1,   390,    -1,   391,    -1,   393,
      -1,   385,    -1,   388,    -1,   246,    -1,   248,    -1,   250,
      -1,   252,    -1,   254,    -1,   256,    -1,   258,    -1,   260,
      -1,   262,    -1,   245,    -1,   247,    -1,   249,    -1,   251,
      -1,   253,    -1,   255,    -1,   257,    -1,   259,    -1,   261,
      -1,   310,    -1,   311,    -1,   239,    -1,   239,    -1,   311,
      -1,   158,   229,   310,    -1,   312,   287,   464,    -1,   312,
     287,   217,   229,   473,    -1,   312,   287,   463,    -1,   312,
     287,   480,   229,   457,    -1,   312,   287,   479,   229,   459,
      -1,   312,   287,   478,   229,   461,    -1,   312,   287,   452,
      -1,   312,   287,   455,    -1,   312,   287,   234,   321,   235,
      -1,   312,   287,   234,   320,   235,    -1,   312,   287,   234,
     319,   235,    -1,   312,   287,   480,   229,   234,   318,   235,
      -1,   312,   287,   479,   229,   234,   317,   235,    -1,   312,
     287,   478,   229,   234,   316,   235,    -1,   312,   287,   234,
     322,   235,    -1,   462,    -1,   316,   228,   462,    -1,   460,
      -1,   317,   228,   460,    -1,   458,    -1,   318,   228,   458,
      -1,   456,    -1,   319,   228,   456,    -1,   453,    -1,   320,
     228,   453,    -1,   464,    -1,   321,   228,   464,    -1,   463,
      -1,   322,   228,   463,    -1,    -1,    -1,   208,   238,   230,
     324,   468,   231,   325,   308,    -1,   238,   230,   471,   231,
      -1,   157,    -1,    -1,     6,    -1,     6,    -1,   232,   464,
     210,   464,   233,    -1,   232,   464,   233,    -1,    -1,    -1,
     166,   331,   232,   451,   233,   332,   234,   333,   235,    -1,
      -1,   334,   333,    -1,    -1,   167,   476,   229,   335,   307,
      -1,    -1,   169,   229,   336,   307,    -1,   168,    -1,   288,
     288,    -1,   165,    -1,   164,   313,   287,   464,   338,   464,
      -1,    -1,   339,   341,   308,    -1,    -1,   163,   232,   451,
     233,   343,   308,    -1,    -1,   329,   229,   345,   309,    -1,
      -1,   144,   329,   347,   348,    -1,   308,    -1,    -1,   308,
     349,   145,   308,    -1,    14,   229,   463,    -1,    14,   229,
     463,   228,    16,    -1,    14,   229,   463,   228,    15,   228,
      16,   228,    17,   228,    18,   228,     4,   228,     4,    -1,
      77,    -1,    77,   229,   476,    -1,    77,   229,    87,    -1,
      65,   229,   353,   228,   353,    -1,    65,   229,   353,   228,
     476,    -1,   230,     4,   228,    86,   228,   368,   231,    -1,
     433,   328,   228,   445,    -1,    -1,   131,   229,   354,   228,
     361,   228,   363,   434,   437,   439,   442,   443,   441,   356,
     308,    -1,    -1,    57,   229,   354,   228,   360,   228,   362,
     228,   363,   434,   437,   439,   442,   443,   441,   358,   308,
      -1,    -1,   228,     5,    -1,   230,     4,   228,     4,   231,
      -1,    87,    -1,   230,     4,   228,     4,   231,    -1,    87,
      -1,   230,   372,   228,   373,   231,    -1,    87,    -1,   230,
       4,   228,     4,   231,    -1,    87,    -1,   143,   229,   365,
     228,   444,   228,   366,   228,   368,   369,    -1,    33,   229,
     444,   228,   473,   369,    -1,     5,    -1,    87,    -1,   367,
      -1,    87,    -1,    86,    -1,    86,   289,   367,    -1,     4,
      -1,    87,    -1,    -1,   369,   228,   370,    -1,   149,   229,
     150,    -1,    83,   229,   461,    -1,   129,   229,     4,    -1,
     175,    -1,   120,    -1,   121,    -1,   138,    -1,    24,    -1,
      23,   229,   372,   228,   373,   359,   237,    -1,    23,   229,
     452,   359,   237,    -1,    91,    -1,    92,    -1,    93,    -1,
      92,    -1,    31,   229,   376,    -1,    -1,    31,   229,   376,
     375,   308,    -1,   459,   228,   452,   377,    -1,    -1,   377,
     228,   378,    -1,   463,    -1,   135,    -1,   136,    -1,   447,
      -1,   142,   463,    -1,   176,    -1,   177,    -1,   173,    -1,
     180,    -1,   181,    -1,   181,   229,   451,    -1,   182,    -1,
     183,   229,   451,    -1,   184,   229,   451,    -1,   188,   229,
     451,    -1,   189,    -1,   190,    -1,   191,   229,   379,    -1,
     191,    -1,   178,    -1,   179,    -1,   185,    -1,   186,   229,
     451,    -1,   187,    -1,   236,    -1,   192,    -1,   236,   289,
     379,    -1,    29,   229,   382,    -1,    -1,    30,   229,   382,
     381,   308,    -1,   461,   383,    -1,    -1,   383,   228,   384,
      -1,    80,    -1,   193,   229,   459,    -1,   477,    -1,   132,
     229,   463,    -1,   161,   229,   451,    -1,   162,    -1,    79,
      -1,   170,   229,   451,    -1,   207,    -1,    78,    -1,   171,
      -1,   172,   229,   451,    -1,   173,    -1,   174,    -1,   452,
      -1,   118,   229,   115,    -1,   117,   229,   116,    -1,   119,
     229,   451,    -1,   128,   229,   451,    -1,   129,   229,   451,
      -1,   159,   229,   160,    -1,   130,   229,   451,    -1,    83,
     229,   461,    -1,   175,    -1,   121,    -1,    85,    -1,    81,
      -1,   124,   229,   122,    -1,   125,   229,   123,    -1,    64,
     229,   451,    -1,   126,   229,   127,    -1,    32,   229,   432,
     228,   452,    -1,    34,   229,   452,   228,    86,   228,   444,
      -1,    39,   229,   452,   228,    86,    -1,    39,   229,   452,
     228,    86,   228,     5,   303,    -1,    40,    -1,    40,   229,
     473,    -1,    40,   229,   192,    -1,    53,   229,   452,   228,
      96,    -1,    54,   229,   452,   228,    96,    -1,    54,   229,
     482,   228,   482,   228,    96,    -1,    59,   229,   482,   228,
     482,   228,   236,    -1,    60,   229,   482,   228,   482,   395,
      -1,    61,   229,   482,   228,   482,    -1,    -1,   228,    96,
      -1,    68,   229,   473,   228,   100,    -1,    69,   229,   473,
      -1,    70,   229,   452,   228,   200,   229,   452,   228,    98,
     228,    97,    -1,    70,   229,   452,   228,    71,   228,    72,
     228,    73,   228,    98,   228,    97,    -1,    84,   229,   452,
     228,   197,   400,    -1,    -1,   400,   228,   401,    -1,    97,
      -1,   193,   229,   459,    -1,   194,   229,   461,    -1,   195,
     229,   405,    -1,   196,   229,   405,    -1,   198,    -1,   199,
      -1,   200,   229,   452,    -1,   201,   229,   432,    -1,   149,
     229,   402,    -1,   104,   229,   440,    -1,   202,   229,     4,
      -1,   129,   229,     4,    -1,     4,    -1,    98,    -1,   150,
      -1,   100,    -1,    75,   229,   473,    -1,    74,   229,   473,
      -1,     3,    -1,   230,     3,   228,   445,   231,    -1,   148,
     229,   455,   228,   457,   228,   457,   228,     7,    -1,   146,
     229,   473,   228,   457,    -1,   146,   229,   473,   228,   457,
     228,   103,    -1,    55,   229,   455,    -1,    56,   229,   455,
      -1,    45,   229,   236,   228,    46,    -1,    45,   229,   236,
     228,    46,   228,   193,   229,   459,    -1,    42,   229,   455,
     228,    44,    -1,    43,   229,   482,   228,    44,    -1,    26,
     229,   473,   228,   102,    -1,    -1,    41,   229,   455,   228,
     445,   228,   433,   434,   437,   439,   442,   443,   441,   415,
     416,    -1,    -1,   308,    -1,    50,   229,   452,   228,   446,
     228,   448,    -1,    50,   229,   452,   228,   446,   228,   448,
     228,   193,   229,   459,    -1,    50,   229,   452,   228,   446,
     228,   448,   228,    95,    -1,    50,   229,   452,   228,   446,
     228,   448,   228,    95,   228,   193,   229,   459,    -1,    51,
     229,   452,    -1,    51,   229,   452,   228,   193,   229,   459,
      -1,   108,   229,   452,   228,   109,   228,     4,   228,   110,
     228,   111,    -1,   108,   229,   452,   228,   109,   228,     4,
     228,   110,   228,   111,   228,   445,    -1,   108,   229,   452,
     228,   109,   228,   112,   228,   110,    -1,   108,   229,   452,
     228,   109,   228,   113,   228,   110,    -1,   108,   229,   452,
     228,   109,   228,   114,   228,   110,    -1,   108,   229,   452,
     228,   109,   228,   110,    -1,   105,   229,   473,   228,   106,
     228,   103,    -1,   137,   229,   473,   228,     4,    -1,   137,
     229,   473,   228,   106,   228,   103,    -1,   139,   229,   455,
     228,   141,    -1,   140,   229,   473,   228,   141,   228,     4,
     228,     4,    -1,   140,   229,   473,   228,   141,   228,     4,
      -1,    52,   229,    99,   228,   452,    -1,    52,   229,    99,
     228,   452,   228,   193,   229,   459,    -1,   203,   229,   452,
     228,   463,    -1,   203,   229,   452,   228,    87,    -1,   203,
     229,   452,    -1,   204,   229,   452,   228,   151,   228,   445,
      -1,   204,   229,   452,    -1,   205,   229,   452,   228,   152,
     228,   463,    -1,   205,   229,   452,   228,   152,   228,    87,
      -1,   205,   229,   452,    -1,   206,   229,   452,   228,   153,
      -1,   206,   229,   452,   228,   155,    -1,   206,   229,   452,
      -1,    66,   229,   464,   228,   452,    -1,    67,   229,   452,
     228,   481,   228,   463,    -1,    63,   229,   451,   228,   451,
     228,   451,   228,   451,    -1,    63,    -1,   236,    -1,    87,
      -1,   236,    -1,    87,    -1,    -1,   228,   435,    -1,   436,
      -1,   436,   228,   435,    -1,    47,    -1,    48,    -1,    49,
      -1,    -1,   107,   229,   438,    -1,   106,    -1,    -1,   104,
     229,   440,    -1,   103,    -1,    -1,   193,   229,   459,    -1,
      -1,    27,   229,   102,    -1,    -1,    28,   229,     4,    -1,
      78,    -1,    87,    -1,    79,    -1,    87,    -1,    90,    -1,
     449,    -1,    87,    -1,    90,    -1,   449,    -1,    89,   229,
      87,    -1,    94,    -1,    87,    -1,    89,   232,     4,   233,
      -1,   236,    -1,   247,    -1,   248,   232,   464,   233,    -1,
     464,    -1,   453,    -1,   219,   230,   473,   231,    -1,   251,
      -1,   252,   232,   464,   233,    -1,   230,     4,   228,     4,
     231,    -1,    87,    -1,    88,   454,   233,    -1,   227,    -1,
     227,   228,   454,    -1,   456,    -1,   253,    -1,   254,   232,
     464,   233,    -1,   230,     4,   228,     4,   228,     4,   228,
       4,   231,    -1,   458,    -1,   255,    -1,   256,   232,   464,
     233,    -1,     3,    -1,   230,     3,   228,   445,   231,    -1,
     460,    -1,   257,    -1,   258,   232,   464,   233,    -1,   236,
      -1,     3,    -1,   230,     3,   228,   236,   231,    -1,    87,
      -1,   462,    -1,   259,    -1,   260,   232,   464,   233,    -1,
     236,    -1,     3,    -1,   230,     3,   228,   236,   231,    -1,
      87,    -1,   101,    -1,   450,    -1,   463,   288,   450,    -1,
       4,    -1,   474,    -1,   230,     8,   231,    -1,   245,    -1,
     246,   232,   464,   233,    -1,   464,   282,   464,    -1,   464,
     283,   464,    -1,   464,   284,   464,    -1,   464,   285,   464,
      -1,   464,   286,   464,    -1,   230,   464,   231,    -1,   241,
      -1,   242,    -1,   312,   229,   465,    -1,   466,    -1,   467,
     228,   466,    -1,    -1,   467,    -1,   464,    -1,   463,    -1,
     469,    -1,   470,   228,   469,    -1,    -1,   470,    -1,   452,
      -1,   212,   455,    -1,   213,   455,    -1,   214,   452,   228,
     452,    -1,   215,   452,   228,   452,   228,   464,    -1,   216,
     230,   473,   231,    -1,   216,   230,   367,   228,   473,   231,
      -1,   222,   230,     7,   228,   473,   231,    -1,   222,   230,
     473,   228,   473,   231,    -1,   222,   230,   457,   228,   473,
     231,    -1,   218,   452,    -1,   220,   230,   452,   228,   464,
     231,    -1,   220,   230,   452,   228,   464,   228,    47,   231,
      -1,   221,   230,   452,   228,   464,   228,   464,   231,    -1,
     221,   230,   452,   228,   464,   228,   464,   228,    47,   231,
      -1,   224,   230,   225,   228,   230,   464,   283,   464,   301,
     231,   228,   452,   302,   231,    -1,   223,   472,    -1,   261,
      -1,   230,   473,   231,    -1,   472,    -1,   472,   290,   473,
      -1,   281,    -1,   102,    -1,     4,    -1,     8,    -1,     9,
      -1,     4,    -1,     8,    -1,     9,    -1,     4,    -1,   474,
      -1,    36,    -1,    29,    -1,    37,    -1,    31,    -1,    38,
      -1,   146,    -1,    82,    -1,    87,    -1,   483,    -1,    62,
     230,     4,   228,     4,   228,     4,   228,     4,   231,    -1,
     230,     4,   228,     4,   228,     4,   228,     4,   231,    -1
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
    1714,  1720,  1725,  1730,  1735,  1740,  1747,  1756,  1760,  1773,
    1782,  1781,  1799,  1809,  1815,  1823,  1829,  1834,  1839,  1844,
    1849,  1854,  1859,  1864,  1869,  1883,  1889,  1894,  1899,  1904,
    1909,  1914,  1919,  1924,  1929,  1934,  1939,  1944,  1949,  1954,
    1959,  1964,  1970,  1975,  1980,  1985,  1992,  1998,  2027,  2032,
    2040,  2046,  2050,  2058,  2065,  2072,  2082,  2092,  2108,  2119,
    2122,  2128,  2134,  2140,  2144,  2150,  2157,  2163,  2171,  2177,
    2182,  2187,  2192,  2197,  2203,  2209,  2214,  2219,  2224,  2229,
    2234,  2241,  2241,  2241,  2241,  2244,  2250,  2256,  2261,  2268,
    2275,  2279,  2285,  2291,  2297,  2302,  2309,  2315,  2325,  2332,
    2331,  2365,  2368,  2374,  2379,  2384,  2389,  2395,  2399,  2405,
    2409,  2413,  2417,  2421,  2425,  2431,  2437,  2441,  2447,  2453,
    2457,  2463,  2467,  2473,  2477,  2482,  2489,  2493,  2500,  2504,
    2509,  2516,  2520,  2525,  2533,  2539,  2546,  2550,  2557,  2565,
    2568,  2578,  2582,  2585,  2591,  2595,  2602,  2606,  2610,  2617,
    2620,  2626,  2633,  2636,  2642,  2649,  2653,  2660,  2663,  2670,
    2673,  2679,  2680,  2683,  2684,  2687,  2688,  2689,  2695,  2696,
    2697,  2703,  2704,  2707,  2716,  2721,  2728,  2739,  2745,  2749,
    2753,  2760,  2770,  2777,  2781,  2787,  2791,  2799,  2803,  2810,
    2820,  2833,  2837,  2844,  2854,  2863,  2874,  2878,  2885,  2895,
    2906,  2915,  2925,  2931,  2935,  2942,  2952,  2963,  2972,  2982,
    2986,  2993,  2994,  3000,  3004,  3008,  3012,  3020,  3029,  3033,
    3037,  3041,  3045,  3049,  3052,  3059,  3068,  3101,  3102,  3105,
    3106,  3109,  3113,  3120,  3127,  3138,  3141,  3149,  3153,  3157,
    3161,  3165,  3170,  3174,  3178,  3183,  3188,  3193,  3197,  3202,
    3207,  3211,  3215,  3220,  3224,  3231,  3237,  3241,  3247,  3254,
    3255,  3258,  3259,  3260,  3263,  3267,  3271,  3275,  3281,  3282,
    3285,  3286,  3289,  3290,  3293,  3294,  3297,  3301,  3327
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CHAR", "INTEGER", "BOOLEAN", "PERCENT",
  "SPERCENT", "MINUS_INTEGER", "PLUS_INTEGER", "MAZE_GRID_ID",
  "SOLID_FILL_ID", "MINES_ID", "ROGUELEV_ID", "MESSAGE_ID", "MESSAGE_ATTR",
  "MESSAGE_COLOR", "MESSAGE_SOUND_TYPE", "MESSAGE_SOUND_ID", "MAZE_ID",
  "LEVEL_ID", "LEV_INIT_ID", "TILESET_ID", "GEOMETRY_ID", "NOMAP_ID",
  "BOUNDARY_TYPE_ID", "SPECIAL_TILESET_ID", "TILESET_PARAM_ID",
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
     475,   476,   477,   478,   479,   480,   481,   482,    44,    58,
      40,    41,    91,    93,   123,   125,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,   528,    43,    45,    42,    47,    37,    61,    46,   124,
      38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   291,   292,   292,   293,   293,   294,   295,   295,   295,
     296,   296,   296,   296,   297,   298,   298,   299,   300,   301,
     301,   302,   302,   303,   303,   304,   304,   305,   305,   306,
     306,   307,   307,   308,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   312,   312,   312,
     313,   313,   314,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   316,   316,
     317,   317,   318,   318,   319,   319,   320,   320,   321,   321,
     322,   322,   324,   325,   323,   326,   327,   328,   328,   329,
     329,   329,   331,   332,   330,   333,   333,   335,   334,   336,
     334,   337,   338,   338,   339,   341,   340,   343,   342,   345,
     344,   347,   346,   348,   349,   348,   350,   350,   350,   351,
     351,   351,   352,   352,   353,   354,   356,   355,   358,   357,
     359,   359,   360,   360,   361,   361,   362,   362,   363,   363,
     364,   364,   365,   365,   366,   366,   367,   367,   368,   368,
     369,   369,   370,   370,   370,   370,   370,   370,   370,   371,
     371,   371,   372,   372,   373,   373,   374,   375,   374,   376,
     377,   377,   378,   378,   378,   378,   378,   378,   378,   378,
     378,   378,   378,   378,   378,   378,   378,   378,   378,   378,
     378,   378,   378,   378,   378,   378,   379,   379,   379,   380,
     381,   380,   382,   383,   383,   384,   384,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   385,   386,   387,   387,
     388,   388,   388,   389,   390,   391,   392,   393,   394,   395,
     395,   396,   397,   398,   398,   399,   400,   400,   401,   401,
     401,   401,   401,   401,   401,   401,   401,   401,   401,   401,
     401,   402,   402,   402,   402,   403,   404,   405,   405,   406,
     407,   407,   408,   409,   410,   410,   411,   412,   413,   415,
     414,   416,   416,   417,   417,   417,   417,   418,   418,   419,
     419,   419,   419,   419,   419,   420,   421,   421,   422,   423,
     423,   424,   424,   425,   425,   425,   426,   426,   427,   427,
     427,   428,   428,   428,   429,   430,   431,   431,   432,   432,
     433,   433,   434,   434,   435,   435,   436,   436,   436,   437,
     437,   438,   439,   439,   440,   441,   441,   442,   442,   443,
     443,   444,   444,   445,   445,   446,   446,   446,   447,   447,
     447,   448,   448,   449,   450,   450,   450,   451,   452,   452,
     452,   452,   453,   453,   453,   454,   454,   455,   455,   455,
     456,   457,   457,   457,   458,   458,   459,   459,   459,   460,
     460,   460,   460,   461,   461,   461,   462,   462,   462,   462,
     462,   463,   463,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   465,   465,   466,   467,   467,   468,
     468,   469,   469,   470,   470,   471,   471,   472,   472,   472,
     472,   472,   472,   472,   472,   472,   472,   472,   472,   472,
     472,   472,   472,   472,   472,   472,   473,   473,   474,   475,
     475,   476,   476,   476,   477,   477,   477,   477,   478,   478,
     479,   479,   480,   480,   481,   481,   482,   482,   483
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
       4,     0,     4,     1,     0,     4,     3,     5,    15,     1,
       3,     3,     5,     5,     7,     4,     0,    15,     0,    17,
       0,     2,     5,     1,     5,     1,     5,     1,     5,     1,
      10,     6,     1,     1,     1,     1,     1,     3,     1,     1,
       0,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       7,     5,     1,     1,     1,     1,     3,     0,     5,     4,
       0,     3,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     3,     1,     3,     3,     3,     1,     1,     3,
       1,     1,     1,     1,     3,     1,     1,     1,     3,     3,
       0,     5,     2,     0,     3,     1,     3,     1,     3,     3,
       1,     1,     3,     1,     1,     1,     3,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     3,     3,     3,     3,     5,     7,     5,     8,
       1,     3,     3,     5,     5,     7,     7,     6,     5,     0,
       2,     5,     3,    11,    13,     6,     0,     3,     1,     3,
       3,     3,     3,     1,     1,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     3,     3,     1,     5,     9,
       5,     7,     3,     3,     5,     9,     5,     5,     5,     0,
      15,     0,     1,     7,    11,     9,    13,     3,     7,    11,
      13,     9,     9,     9,     7,     7,     5,     7,     5,     9,
       7,     5,     9,     5,     5,     3,     7,     3,     7,     7,
       3,     5,     5,     3,     5,     7,     9,     1,     1,     1,
       1,     1,     0,     2,     1,     3,     1,     1,     1,     0,
       3,     1,     0,     3,     1,     0,     3,     0,     3,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     4,     1,     1,     4,     1,     1,     4,
       1,     4,     5,     1,     3,     1,     3,     1,     1,     4,
       9,     1,     1,     4,     1,     5,     1,     1,     4,     1,
       1,     5,     1,     1,     1,     4,     1,     1,     5,     1,
       1,     1,     3,     1,     1,     3,     1,     4,     3,     3,
       3,     3,     3,     3,     1,     1,     3,     1,     3,     0,
       1,     1,     1,     1,     3,     0,     1,     1,     2,     2,
       4,     6,     4,     6,     6,     6,     6,     2,     6,     8,
       8,    10,    14,     2,     1,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,    10,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    27,     0,     0,     1,
       5,     0,    31,     0,     7,     0,   159,     0,     0,     0,
       0,   229,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   310,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   397,     0,
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
     473,     0,   476,     0,   518,     0,   474,   495,    32,     0,
     179,     0,     8,     0,   434,   435,     0,   471,   186,     0,
       0,     0,    12,   520,   519,    14,   443,     0,   232,   233,
       0,     0,   440,     0,     0,   200,   438,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   497,   516,     0,   467,   469,   470,     0,   466,   464,
       0,   269,   273,   463,   270,   460,   462,     0,   459,   457,
       0,   236,     0,   456,   399,   398,     0,   421,   422,     0,
       0,     0,     0,   312,   311,     0,   448,     0,     0,   447,
       0,     0,     0,     0,   536,     0,     0,   367,     0,     0,
       0,     0,     0,   352,   353,   401,   400,     0,   157,     0,
       0,     0,     0,   437,     0,     0,     0,     0,     0,   322,
       0,   346,   345,   523,   521,   522,   191,   190,     0,     0,
       0,     0,     0,     0,     0,   212,   213,     0,     0,     0,
       0,    16,   122,     0,     0,     0,   385,   387,   390,   393,
     152,     0,     0,     0,     0,   161,     0,     0,     0,     0,
       0,   492,   491,   493,   496,     0,   529,   531,   528,   530,
     532,   533,     0,     0,     0,   129,   130,   125,   123,     0,
       0,     0,     0,    31,   176,     0,    29,     0,     0,     0,
       0,     0,     0,   445,     0,     0,     0,     0,     0,     0,
       0,   498,   499,     0,     0,     0,   507,     0,     0,     0,
     513,     0,     0,     0,     0,     0,     0,   272,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   158,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   183,   182,     0,     0,     0,   177,
       0,     0,     0,     0,     0,     0,   489,   475,   483,     0,
       0,   478,   479,   480,   481,   482,     0,   155,     0,   473,
       0,     0,     0,     0,   146,   144,   150,   148,     0,     0,
       0,   180,     0,   423,   424,     9,     0,     0,   187,   472,
      11,   347,     0,    10,     0,     0,   444,     0,     0,     0,
     235,   234,   200,   201,   231,     0,     0,   216,     0,     0,
       0,     0,   454,     0,     0,   452,     0,     0,   451,     0,
       0,   515,   517,   358,     0,     0,     0,   271,     0,     0,
     238,   240,   306,   220,     0,    17,   140,   308,     0,     0,
       0,   356,     0,     0,   357,   354,   427,     0,   425,     0,
     426,     0,   381,   313,     0,   314,     0,   203,     0,     0,
       0,     0,   319,   318,     0,     0,   192,   193,   394,   534,
     535,     0,   321,     0,     0,   326,     0,     0,   205,     0,
       0,   376,     0,   378,     0,     0,     0,     0,   350,     0,
      15,     0,   173,     0,     0,   163,   384,   383,     0,     0,
     391,   392,     0,   487,   490,     0,   477,   160,   494,   124,
       0,     0,   133,     0,   132,     0,   131,     0,   137,     0,
     128,     0,   127,     0,   126,    33,   436,     0,     0,     0,
     446,   439,     0,   441,     0,   500,     0,     0,     0,   502,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   465,
     526,   524,   525,     0,   284,   281,   275,   301,     0,   300,
       0,     0,     0,   299,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   280,     0,   285,     0,   287,   288,   298,
       0,   283,   274,   289,   527,   277,     0,   458,   239,   211,
       0,     0,     0,     0,   449,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   195,     0,     0,
     317,     0,     0,     0,     0,     0,   325,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   178,   172,   174,
       0,     0,     0,     0,     0,   153,     0,   145,   147,   149,
     151,     0,   138,     0,     0,   142,     0,     0,     0,   442,
     230,     0,   217,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   468,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     461,     0,     0,   307,   141,    23,     0,   402,     0,     0,
       0,     0,   432,   431,   363,   368,     0,   315,     0,   207,
       0,     0,   316,   320,     0,     0,   395,     0,     0,     0,
     375,     0,   374,     0,     0,     0,     0,   209,     0,   402,
     377,   380,   215,     0,   214,   185,   351,     0,   165,   386,
     389,   388,   484,   485,   486,   488,     0,     0,   136,   135,
       0,   134,     0,     0,     0,   501,   503,     0,   508,     0,
     504,     0,   453,   506,   505,     0,   304,   297,   291,   290,
     292,   302,   303,   305,   293,   294,   296,   278,   295,   279,
     282,   286,   276,     0,   428,   243,   244,     0,   249,   247,
     248,   261,   262,   250,   251,   253,     0,     0,   263,     0,
     265,     0,   257,   258,   260,   241,   245,   429,   242,     0,
     226,   227,     0,   228,     0,   225,   221,     0,   309,     0,
       0,   409,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,   218,   219,     0,     0,     0,   328,     0,     0,
       0,     0,     0,     0,     0,   333,   334,     0,     0,     0,
     327,     0,     0,     0,     0,     0,     0,   409,     0,     0,
       0,     0,     0,     0,   165,   154,   139,   143,     0,   348,
       0,     0,     0,   455,     0,     0,   246,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    24,     0,   406,   407,
     408,   403,   404,     0,   412,     0,     0,   355,   365,     0,
     382,   202,     0,   402,   396,   194,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   371,
     372,   373,   204,     0,   412,   379,   220,   349,     0,   169,
     164,   166,     0,     0,   509,     0,   510,   479,   430,   252,
     254,   255,   264,   256,   267,   266,   259,   223,   224,   222,
       0,     0,     0,     0,   417,     0,     0,     0,     0,     0,
     409,     0,     0,   414,   338,   340,   341,   342,   344,   343,
     337,   329,   330,   331,   332,   335,   336,   339,     0,     0,
     417,   210,   167,    31,     0,     0,     0,     0,     0,     0,
     450,   405,   411,   410,     0,     0,   419,     0,   538,     0,
     364,   206,   412,     0,   323,   369,   208,   419,    31,   170,
       0,     0,   511,    20,     0,   268,   413,     0,     0,   415,
     537,     0,   417,     0,     0,   415,   168,     0,     0,     0,
     418,     0,     0,   359,   366,   419,   324,   370,   196,     0,
       0,    21,   420,     0,   361,   415,     0,   188,    25,    26,
      23,     0,     0,   416,   362,   360,   198,   197,    13,    22,
     512,     0,   199
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,   102,   103,   104,   105,   106,
    1138,  1202,   968,  1200,    12,   176,   107,   424,   108,   109,
     110,   111,   233,   112,   113,   801,   595,   804,   530,   531,
     532,   533,   114,   516,   896,   115,   116,   485,   117,   118,
     234,   790,  1013,  1014,  1158,  1133,   119,   654,   120,   121,
     251,   122,   651,   123,   422,   124,   378,   505,   646,   125,
     126,   127,   355,   347,   128,  1196,   129,  1211,   440,   619,
     640,   861,   879,   130,   377,   883,   568,   984,   749,   966,
     131,   274,   562,   132,   461,   311,   748,   955,  1096,   133,
     458,   301,   457,   742,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   770,   144,   145,   146,   147,   776,
    1000,  1120,   148,   149,   553,   150,   151,   152,   153,   154,
     155,   156,   157,   158,  1194,  1205,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   316,   348,   971,  1041,  1042,  1044,  1143,  1104,  1114,
    1183,  1146,  1169,   319,   545,   609,   956,   854,   610,   257,
     352,   291,   276,   434,   328,   329,   577,   578,   312,   313,
     302,   303,   401,   353,   894,   663,   664,   665,   403,   404,
     405,   292,   452,   246,   265,   367,   745,   419,   420,   421,
     631,   333,   334
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1069
static const yytype_int16 yypact[] =
{
     154,   -85,   -81,   124, -1069,   154,    26,   -68,   -50, -1069,
   -1069,   -38,   889,   -29, -1069,    86, -1069,   -35,   -28,     3,
      17, -1069,    62,    89,    95,   103,   126,   129,   143,   149,
     160,   176,   177,   180,   186,   191,   193,   198,   199,   205,
     211,   215,   229,   231,   240,   248,   250,   252,   254,   259,
     261,   273,   275,   278,   295,   297,   300,   304,   306,   314,
     315,   317,   319,   320,   326,   327,    48,   329,   330,   331,
   -1069,   333,    94,   754, -1069, -1069,   334,   335,   337,   340,
      16,    84,   111, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,   889, -1069,
   -1069,   -64, -1069, -1069, -1069, -1069, -1069,   341, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069,   345,   163, -1069,  -128,   462,    63,
      -8,   318,   766,    75,    75,   146,   -43,    27,   -22,   435,
     -22,   961,  -136,  -136,   -20,   270,   -22,   -22,   468,   -22,
     -12,  -136,  -136,   -36,   -20,   -20,   -20,    84,   343,    84,
     -22,   766,   766,   -22,   766,   766,    82,   -22,   766,   -22,
     -36,   766,  -136,   766,    91, -1069,   766,  -136,   416,   828,
      84, -1069, -1069,   287,   344,   -22,   -22,   -22,   -22,   352,
   -1069,    25, -1069,   359, -1069,   128, -1069,    15, -1069,   449,
   -1069,   358,   347,    86, -1069, -1069,   361, -1069,  -133,   366,
     369,   372, -1069, -1069, -1069, -1069, -1069,   375, -1069, -1069,
     374,   601, -1069,   378,   383,   387, -1069, -1069,  -136,  -136,
     -22,   -22,   376,   -22,   386,   395,   398,   766,   404,   648,
   -1069, -1069,   353,   389, -1069, -1069, -1069,   639, -1069, -1069,
     412, -1069, -1069, -1069, -1069, -1069, -1069,   650, -1069, -1069,
     424,   420,   430, -1069, -1069, -1069,   431, -1069, -1069,   432,
     433,   436,   439, -1069, -1069,   659, -1069,   445,   443, -1069,
     450,   451,   661,   452, -1069,   454,   456,   458,   464,   465,
     706,   484,   485, -1069, -1069, -1069, -1069,   486,   711,   490,
     496,   497,   498,   112,   723,   500,   233,   501,   503, -1069,
     504, -1069, -1069, -1069, -1069, -1069, -1069, -1069,   505,   509,
     510,   511,   513,   514,   515, -1069, -1069,   517,   358,   518,
     519,   525, -1069,   521,    84,    84,   527,   528,   535,   536,
   -1069,   537,   -48,    84,    84, -1069,    84,    84,    84,    84,
      84,   478,   112, -1069,   539,   538, -1069, -1069, -1069, -1069,
   -1069, -1069,   546,    44,    11, -1069, -1069,   478,   112,   554,
     555,   556,   889,   889, -1069,    42, -1069,    84,   234,  -128,
     783,    34,   784,   566,   562,   766,   568,    84,   195,   792,
     561, -1069, -1069,   571,   572,   849, -1069,   -22,   -22,   558,
   -1069,   576,   577,   766,   700,   583,    84,   584,   358,   587,
      84,   358,   -22,   -22,   766,   730,    65,   731,   593,    84,
      42,   778,   819,   596,   781,   780,   281,   634,   -22,   732,
     602,   733,   -20,   -18, -1069,   603,   -20,   -20,   -20,    84,
     605,    37,   -22,    29,   734,   -10,   640,   739,   727,   -15,
      60,   697,   698,    27,   710, -1069,    22,    22,   701, -1069,
     147,   623,   -44,   708,   713,   145,   978, -1069, -1069,   210,
     268,    78,    78, -1069, -1069, -1069,    15, -1069,   766,   645,
    -102,   -94,   -82,   -46, -1069, -1069,   478,   112,    97,   109,
     119, -1069,   641, -1069, -1069, -1069,   355,   646, -1069, -1069,
   -1069, -1069,   872, -1069,   649,   375, -1069,   651,   877,   422,
   -1069, -1069,   387, -1069, -1069,   -22,   -22,   594,   656,   654,
     660,   662, -1069,   663,   628, -1069,   655,   664, -1069,   665,
     666, -1069, -1069, -1069,   668,   437,   338, -1069,   669,   466,
   -1069, -1069, -1069, -1069,   670,   673, -1069,   674,   892,   476,
     678, -1069,   679,   893, -1069,   680, -1069,   684, -1069,   689,
   -1069,   696,   699, -1069,   922, -1069,   705, -1069,   934,   725,
      42,   737,   740, -1069,   741,   865, -1069, -1069, -1069, -1069,
   -1069,   742, -1069,   743,   738, -1069,   744,   746, -1069,   956,
     747, -1069,   748, -1069,   749,   755,   816,   959,   763,   764,
   -1069,   358, -1069,   707,    84, -1069, -1069,   478,   770,   774,
   -1069, -1069,   775, -1069,   782,   777, -1069, -1069, -1069, -1069,
    1002,   786, -1069,   -25, -1069,    84, -1069,  -128, -1069,    36,
   -1069,    65, -1069,    46, -1069, -1069, -1069,   996,   791,  1011,
   -1069, -1069,   790, -1069,   787, -1069,   795,   944,   766, -1069,
      84,    84,   766,   803,    84,   766,   766,   804,   805, -1069,
   -1069, -1069, -1069,   809, -1069, -1069, -1069, -1069,   810, -1069,
     811,   812,   813, -1069,   815,   822,   825,   827,   829,   830,
     831,   837,   846, -1069,   848, -1069,   852, -1069, -1069, -1069,
     854, -1069, -1069, -1069, -1069, -1069,   814, -1069,   857,   859,
      27,    65,  1084,   863, -1069,   -36,  1092,   870,   906,  1098,
     101,   146,   910,  -118,  1008,   878,   -14, -1069,   869,  1015,
   -1069,    84,   879,  -128,  1040,   -22,   885,  1012,    67,   886,
     -13,  1013,  1113,   265,   358,  1016,    22, -1069, -1069,   112,
     884,    42,   -34,   139,   978, -1069,   -88, -1069, -1069,   112,
     478,    -9, -1069,    -7,    -4, -1069,   894,    42,   895, -1069,
   -1069,    84, -1069,   898,   390,   256,   899,    42,   507,   900,
     901,    84, -1069,    84,    75,  1004,  1009,    84,  1003,  1010,
     999,    84,    84,    84,  -128,   992,    84,    84,    84,   146,
   -1069,  1019,   316, -1069, -1069,   928,  1153,   930,   931,  1156,
     933,   932, -1069, -1069,   935, -1069,   937, -1069,  1160, -1069,
     293,   939, -1069, -1069,   940,    88,   478,   941,   942,   385,
   -1069,   943, -1069,   950,   958,   960,  1168, -1069,  1183,   930,
   -1069,   962, -1069,   965, -1069, -1069, -1069,   966,   -39, -1069,
   -1069,   478, -1069, -1069, -1069, -1069,   358,    36, -1069, -1069,
      46, -1069,  1172,   980,  1209,   112, -1069,  1169, -1069,    84,
   -1069,   984, -1069, -1069, -1069,   365, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069,   478, -1069, -1069,
   -1069, -1069, -1069,   -69, -1069, -1069, -1069,  -128, -1069, -1069,
   -1069, -1069, -1069, -1069,   989, -1069,   990,   991, -1069,  1014,
   -1069,  1017, -1069, -1069,  1018, -1069, -1069, -1069,   478,  1020,
   -1069, -1069,  1021, -1069,  1022, -1069, -1069,  1218, -1069,  1024,
     474,  1134,  1238,  1025,   146, -1069,    -6,   146,  1023,  1028,
     -13,    84, -1069, -1069,  1026,  1171,  1147, -1069,  1029,  1030,
    1031,  1032,  1033,  1034,  1035, -1069, -1069,  1036,  1039,  1041,
   -1069,  1138,  1159,  1161,  1162,  1042,  1046,  1134,  1271,    88,
    1269,   277,  1048,  1043,   -39, -1069, -1069, -1069,  1051, -1069,
    1052,  1050,   405, -1069,    84,  1195,   478,    84,    84,    84,
      84,    84,  -105,    75,  1279,  1135, -1069,  1280, -1069, -1069,
   -1069, -1069,  1058,  1059,  1185,  1062,  1283, -1069,  1063,  1064,
   -1069, -1069,   195,   930, -1069, -1069,  1066,  1067,  1189,  1292,
      56,   146,    75,    34,    34,   -22,   -43,  1293,  1070, -1069,
   -1069, -1069, -1069,  1295,  1185, -1069, -1069, -1069,  1071, -1069,
   -1069, -1069,  1284,  1296, -1069,  1256, -1069,   162, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069,  1027, -1069, -1069, -1069, -1069,
    1073,   474,  1199,  1077,  1281,  1303,  1078,  1117,   146,  1080,
    1134,  1214,  1216, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,  1203,  1086,
    1281,   859, -1069,   889,  1087,  1090,  1088,  1094,  1091,  -105,
   -1069, -1069, -1069, -1069,  1189,  1095,  1297,  1096, -1069,  1097,
   -1069, -1069,  1185,  1093, -1069,  1100, -1069,  1297,   889, -1069,
    1319,    42, -1069, -1069,  1101, -1069, -1069,  1228,  1102,  1139,
   -1069,   146,  1281,  1236,    42,  1139, -1069,  1106,  1107,   -22,
   -1069,  1332,  1108, -1069, -1069,  1297, -1069, -1069, -1069,  1334,
      98,  1111, -1069,   146,   358,  1139,   358, -1069, -1069, -1069,
     928,   -22,  1109, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069,   358, -1069
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1069, -1069,  1336, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069,   142, -1069, -1069,  1099,  -108,  -376,   921,  1115,
    1272,  -492, -1069, -1069, -1069, -1069,   667, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,  1285, -1069,
   -1069, -1069,   332, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069,   856,  1129, -1069, -1069, -1069, -1069,   788, -1069,
   -1069, -1069,   373, -1069, -1069, -1069,  -616,   346,   280, -1069,
   -1069,   494,   305, -1069, -1069, -1069, -1069, -1069,   219, -1069,
   -1069,  1175, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069,  -663, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069,   294,   606,  -844,   262, -1069,  -952, -1069,  -981,   218,
   -1068, -1021, -1060,  -471,  -463, -1069, -1069, -1069,   523,   936,
    -224,  -179,  -391,   817,   249,  -398,  -493,  -647,  -536,  -454,
    -530,  -649,  -172,   -77, -1069,   573, -1069, -1069,   840, -1069,
   -1069,  1081,   -76,   785, -1069,  -481, -1069, -1069, -1069, -1069,
   -1069,  -178, -1069
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -238
static const yytype_int16 yytable[] =
{
     248,   275,   504,   682,   245,   258,   383,   600,   680,   320,
     627,   322,   596,   648,   649,   240,   535,   336,   337,   240,
     339,   341,   342,   534,   662,   572,   349,   350,   351,   240,
     802,   357,   645,   391,   360,  1007,   805,   551,   368,   294,
     370,   363,   331,   656,   314,   364,   365,   684,   529,   572,
     331,   345,   391,   890,    16,  1074,   386,   387,   388,   389,
    1116,   633,   266,   267,   641,   266,   267,   263,   305,   617,
     415,   871,   638,   859,   877,   266,   267,   417,   294,   266,
     267,   812,   587,   268,   269,   590,   363,  1094,   240,  1048,
     364,   365,   982,  1130,   325,   428,   375,  1175,   266,   267,
     294,   443,   444,  1198,   446,   317,   293,  1188,   254,  1157,
     849,   629,   305,   809,   318,   324,   630,   326,   327,   255,
     256,   543,   572,   295,     9,  1195,   671,  1206,  1011,   544,
    1012,  1095,   356,   672,   673,   358,   359,   296,   361,   362,
     846,   674,   369,   809,     7,   372,   675,   374,     8,   305,
     379,  1185,   306,   676,  1117,   429,  1118,   767,  1152,    11,
    1025,   511,   295,   759,   392,   264,   642,   884,    13,   366,
     402,  1172,   418,     1,     2,   983,   296,   872,   376,   873,
     874,   875,   677,   518,   295,  1199,    14,  1049,   852,   678,
     634,    15,   254,   315,   177,   853,   306,   270,   296,   174,
     346,   178,   254,   255,   256,   271,  1119,   270,   271,  1110,
     332,   270,   618,   255,   256,   639,   860,   878,   340,   897,
     175,   751,   271,   249,   900,   855,   898,   596,   899,   272,
     273,   901,   179,   306,   396,   397,   398,   399,   400,   272,
     273,   413,   536,   272,   273,   241,   180,   254,  1016,   547,
     548,   254,   647,  1017,   239,   241,   242,   243,   255,   256,
     242,   243,   255,   256,   552,   624,   297,   354,   570,   571,
     242,   243,   298,   797,   241,   787,   647,   575,   576,   843,
      81,   363,   798,   591,   592,   364,   365,   560,   561,   242,
     243,   181,   244,   887,   917,   307,   244,   844,   660,   612,
     661,   308,   662,   932,   616,   297,   244,   510,   621,   622,
     623,   298,   652,   628,   241,   542,   519,   520,   182,   521,
     522,   523,   524,   525,   183,   244,   230,   297,   889,   242,
     243,   679,   184,   298,   299,   300,   392,   537,   394,   307,
     657,   247,   710,   681,   903,   308,   711,   712,   252,   647,
     546,   567,   882,   683,   911,   185,   299,   300,   186,   557,
     559,   395,   398,   399,   400,   244,   309,   310,   606,   569,
     607,   608,   187,   579,   575,   576,   307,   582,   188,   585,
     892,   893,   308,   589,   268,   269,   695,   696,   593,   189,
    1137,   253,   599,   -19,   396,   397,   398,   399,   400,   959,
    1123,  1124,   713,   309,   310,   190,   191,   743,   885,   192,
     396,   397,   398,   399,   400,   193,   714,   715,   716,   717,
     194,   718,   195,   719,   277,   266,   267,   196,   197,   396,
     397,   398,   399,   400,   198,   653,   960,   961,  1047,   321,
     199,  1050,   330,   666,   200,   962,   398,   399,   400,   402,
     343,   344,   669,   240,   963,   720,   721,   722,   201,   723,
     202,   492,   724,   725,   726,   964,   727,   728,   729,   203,
     730,   373,   259,   260,   261,   262,   380,   204,   406,   205,
     407,   206,   987,   207,   909,   408,   409,   410,   208,   988,
     209,   965,   396,   397,   398,   399,   400,   731,   416,   732,
     733,   667,   210,  1097,   211,   800,   335,   212,   734,   735,
     736,   737,   738,   739,   989,   396,   397,   398,   399,   400,
    1015,  1038,  1039,  1040,   213,  1121,   214,   441,   442,   215,
    1078,   740,  1122,   216,   990,   217,   266,   267,   396,   397,
     398,   399,   400,   218,   219,   741,   220,   864,   221,   222,
     396,   397,   398,   399,   400,   223,   224,   270,   226,   227,
     228,   572,   229,   235,   236,   573,   237,   338,   271,   238,
     250,   381,  1150,   354,   384,   425,   385,   789,   991,   992,
     993,   994,   390,   995,   996,   997,   998,   999,   686,   272,
     273,   393,   423,   427,   430,   411,   868,   431,   799,   916,
     432,   866,   433,   920,   435,   436,   445,   924,   925,   926,
     437,   438,   929,   930,   931,   439,   447,   454,   907,   244,
     891,   908,   813,   814,   815,   448,   816,   818,   449,   819,
     820,   703,   436,  1085,   451,  1184,  1086,   396,   397,   398,
     399,   400,   455,   453,   456,   266,   267,   396,  1024,   398,
     399,   400,   436,   459,  -237,   693,   460,  1203,   462,   463,
     464,   465,   927,   468,   466,   473,   412,   467,   270,   958,
     709,   470,   396,   397,   398,   399,   400,   469,   471,   413,
     474,   472,   475,   414,   476,   254,   477,   396,   397,   398,
     399,   400,   478,   479,   242,   243,   255,   256,  1178,   747,
     272,   273,   326,   327,   396,   397,   398,   399,   400,   754,
     480,  1187,   481,   482,   483,   266,   267,   484,   486,   396,
     397,   398,   399,   400,   487,   488,   489,   490,   491,   493,
     244,   494,   495,   496,   905,   266,   267,   497,   498,   499,
     912,   500,   501,   502,   915,   503,   506,   507,   396,   397,
     398,   399,   400,   508,   509,   512,   513,  1054,   396,   397,
     398,   399,   400,   514,   515,  1026,   429,   526,   517,   527,
     278,   279,   280,   281,   282,   528,   283,   270,   284,   285,
     286,   287,   288,   538,   539,   540,   550,   554,   574,   396,
     397,   398,   399,   400,   555,   556,   558,   563,   564,   565,
     566,   580,   583,  1089,  1090,  1091,  1092,  1093,   581,   272,
     273,   584,   586,   575,   576,   588,   594,   597,  1204,   290,
    1207,   598,   601,   602,   603,   604,   605,   611,   613,   615,
     614,   620,  1022,   625,   632,  1212,   637,   635,   643,   644,
     278,   279,   280,   281,   282,   636,   283,   270,   284,   285,
     286,   287,   288,   266,   267,  -184,   655,   650,   289,   658,
     278,   279,   280,   281,   282,   659,   283,   270,   284,   285,
     286,   287,   288,   670,   687,   688,   685,   689,   289,   272,
     273,   692,   691,   697,   698,   699,  1125,   704,   700,   290,
     701,   702,   705,   706,   707,    16,   753,   757,   750,   272,
     273,   751,   752,    17,   708,   746,   755,   756,   758,   290,
      18,    19,    20,    21,    22,    23,   759,   760,    24,    25,
      26,    27,    28,    29,    30,   761,   763,   762,    31,    32,
      33,    34,    35,   764,    36,   567,   266,   267,   765,    37,
      38,    39,    40,    41,    42,    43,    44,  1087,    45,    46,
      47,   772,    48,   766,    49,    50,    51,    52,    53,    54,
     779,   784,   703,    55,    56,   768,    57,   775,   769,   771,
     773,   774,   777,    58,   778,   780,   781,   782,   278,   279,
     280,   281,   282,   783,   283,   270,   284,   285,   286,   287,
     288,   785,   786,   231,    59,   788,   289,    60,   791,    84,
    1191,    86,   792,    88,   793,    90,   796,    92,   795,    94,
     794,    96,   806,    98,   808,   100,   325,   272,   273,   807,
      61,   809,  1209,   811,   810,  1159,    62,   290,    63,    64,
     567,   817,    65,    66,   821,    67,   822,    68,   823,   824,
     825,   826,   827,    69,   828,   840,    70,    71,   266,   267,
    1176,   829,    72,    73,   830,    74,   831,    75,   832,   833,
     834,   278,   279,   280,   281,   282,   835,   283,   270,   284,
     285,   286,   287,   288,    85,   836,    87,   837,    89,   289,
      91,   838,    93,   839,    95,   841,    97,   842,    99,   845,
     101,   846,    76,    77,    78,    79,   848,    80,   849,   850,
     272,   273,   851,   856,   857,   862,   858,   865,   933,   934,
     290,   863,   867,   869,   876,   870,   880,   881,   888,   886,
     918,    81,   902,   904,   919,   921,   923,    82,    83,   906,
     910,   913,   914,   922,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   928,   323,   935,   936,   967,   969,   970,   972,
     973,   937,   974,   976,   978,   975,   977,   980,   981,   985,
     986,  1001,  1005,   278,   279,   280,   281,   282,  1002,   283,
     270,   284,   285,   286,   287,   288,  1003,  1006,  1004,  1018,
    1008,   289,   938,  1009,  1010,   939,   940,   941,   942,   943,
     944,   945,   946,   947,   948,   949,   950,   951,   952,   953,
     954,  1019,   272,   273,  1020,  1023,  1021,    83,  1027,  1028,
    1029,  1036,   290,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,  1043,  1045,  1030,  1056,  1057,  1031,  1032,  1068,  1033,
    1034,  1035,  1037,  1046,  1051,   254,  1052,  1055,  1058,  1059,
    1060,  1061,  1062,  1063,  1064,  1065,   255,   256,  1066,  1069,
    1067,  1070,  1071,  1072,  1073,  1075,  1077,  1079,  1080,  1082,
    1083,  1084,  1088,  1098,  1100,  1099,  1101,  1106,  1102,  1103,
    1105,  1107,  1113,  1108,  1111,  1112,  1115,  1127,  1128,  1129,
    1132,  1135,  1134,  1136,  1140,  1142,  1144,  1147,  1145,  1148,
    1149,  1151,  1153,  1154,  1155,  1160,  1139,  1156,  1161,  1162,
    1163,  1173,  1164,  1177,  1167,  1168,  1171,  1170,  1174,  1179,
    1180,  1181,  1182,  1186,  1189,  1190,  1192,  1193,  1197,  1201,
    1210,    10,  1208,   541,   382,   232,  1081,   626,   803,   371,
     694,   225,   426,  1053,   979,  1076,  1131,  1109,  1165,   304,
    1126,   847,  1166,  1141,   957,   549,   668,   895,   450,     0,
       0,   744,   690
};

static const yytype_int16 yycheck[] =
{
     108,   180,   378,   539,    81,   177,   230,   470,   538,   188,
     491,   190,   466,   506,   507,     4,   414,   196,   197,     4,
     199,   200,   200,   414,   516,     3,   204,   205,   206,     4,
     679,   210,   503,     8,   213,   879,   683,     3,   217,     3,
     219,     4,    62,    87,    87,     8,     9,   540,     4,     3,
      62,    87,     8,    87,     6,  1007,   235,   236,   237,   238,
       4,    71,    87,    88,     4,    87,    88,     4,     3,    87,
     249,     4,    87,    87,    87,    87,    88,   249,     3,    87,
      88,   697,   458,    91,    92,   461,     4,   192,     4,    95,
       8,     9,     4,  1074,   230,   228,     5,  1157,    87,    88,
       3,   280,   281,     5,   283,    78,   182,  1175,   236,  1130,
     228,    82,     3,   231,    87,   191,    87,   253,   254,   247,
     248,    79,     3,    87,     0,  1185,   228,  1195,   167,    87,
     169,   236,   209,   235,   228,   211,   212,   101,   214,   215,
     228,   235,   218,   231,   229,   221,   228,   223,   229,     3,
     226,  1172,    87,   235,    98,   288,   100,   620,  1110,   133,
     229,   385,    87,   232,   241,   102,   106,   783,   236,    87,
     247,  1152,   249,    19,    20,    87,   101,   110,    87,   112,
     113,   114,   228,   231,    87,    87,   236,   193,    87,   235,
     200,   229,   236,   236,   229,    94,    87,   219,   101,   228,
     236,   229,   236,   247,   248,   230,   150,   219,   230,  1053,
     230,   219,   230,   247,   248,   230,   230,   230,   230,   228,
     134,   228,   230,   287,   228,   761,   235,   681,   235,   251,
     252,   235,   229,    87,   282,   283,   284,   285,   286,   251,
     252,   230,   414,   251,   252,   230,   229,   236,   897,    15,
      16,   236,   230,   900,   238,   230,   245,   246,   247,   248,
     245,   246,   247,   248,   230,   489,   230,   230,   447,   448,
     245,   246,   236,   671,   230,   651,   230,   255,   256,   750,
     232,     4,   673,   462,   463,     8,     9,    92,    93,   245,
     246,   229,   281,   786,   824,   230,   281,   751,   153,   478,
     155,   236,   794,   839,   482,   230,   281,   384,   486,   487,
     488,   236,   165,   492,   230,   423,   393,   394,   229,   396,
     397,   398,   399,   400,   229,   281,   232,   230,   791,   245,
     246,   234,   229,   236,   259,   260,   413,   414,   210,   230,
     512,   230,     4,   234,   807,   236,     8,     9,     3,   230,
     427,    86,    87,   234,   817,   229,   259,   260,   229,   435,
     437,   233,   284,   285,   286,   281,   257,   258,    87,   445,
      89,    90,   229,   449,   255,   256,   230,   453,   229,   456,
     241,   242,   236,   460,    91,    92,   565,   566,   464,   229,
     228,   228,   469,   231,   282,   283,   284,   285,   286,    83,
    1063,  1064,    64,   257,   258,   229,   229,   586,   784,   229,
     282,   283,   284,   285,   286,   229,    78,    79,    80,    81,
     229,    83,   229,    85,   106,    87,    88,   229,   229,   282,
     283,   284,   285,   286,   229,   288,   120,   121,   974,     4,
     229,   977,   193,   233,   229,   129,   284,   285,   286,   526,
     201,   202,   528,     4,   138,   117,   118,   119,   229,   121,
     229,   228,   124,   125,   126,   149,   128,   129,   130,   229,
     132,   222,    10,    11,    12,    13,   227,   229,    29,   229,
      31,   229,    97,   229,   228,    36,    37,    38,   229,   104,
     229,   175,   282,   283,   284,   285,   286,   159,   249,   161,
     162,   233,   229,  1033,   229,   677,   236,   229,   170,   171,
     172,   173,   174,   175,   129,   282,   283,   284,   285,   286,
     896,    47,    48,    49,   229,  1061,   229,   278,   279,   229,
    1011,   193,  1062,   229,   149,   229,    87,    88,   282,   283,
     284,   285,   286,   229,   229,   207,   229,   771,   229,   229,
     282,   283,   284,   285,   286,   229,   229,   219,   229,   229,
     229,     3,   229,   229,   229,     7,   229,    99,   230,   229,
     229,   155,  1108,   230,   287,   228,   232,   654,   193,   194,
     195,   196,   230,   198,   199,   200,   201,   202,   233,   251,
     252,   232,   234,   232,   228,   146,   775,   228,   675,   823,
     228,   773,   227,   827,   230,     4,   230,   831,   832,   833,
     232,   228,   836,   837,   838,   228,   230,   228,   228,   281,
     792,   231,   698,   700,   701,   230,   702,   704,   230,   705,
     706,     3,     4,   228,   230,  1171,   231,   282,   283,   284,
     285,   286,     3,   290,   232,    87,    88,   282,   283,   284,
     285,   286,     4,     3,   234,   233,   232,  1193,   228,   228,
     228,   228,   834,     4,   228,     4,   217,   228,   219,   841,
     233,   228,   282,   283,   284,   285,   286,   232,   228,   230,
     228,   230,   228,   234,   228,   236,   228,   282,   283,   284,
     285,   286,   228,   228,   245,   246,   247,   248,  1161,   233,
     251,   252,   253,   254,   282,   283,   284,   285,   286,   233,
       4,  1174,   228,   228,   228,    87,    88,     6,   228,   282,
     283,   284,   285,   286,   228,   228,   228,     4,   228,   228,
     281,   228,   228,   228,   811,    87,    88,   228,   228,   228,
     233,   228,   228,   228,   821,   228,   228,   228,   282,   283,
     284,   285,   286,   228,   233,   228,   228,   981,   282,   283,
     284,   285,   286,   228,   228,   937,   288,   228,   231,   231,
     212,   213,   214,   215,   216,   229,   218,   219,   220,   221,
     222,   223,   224,   229,   229,   229,     3,     3,   230,   282,
     283,   284,   285,   286,   228,   233,   228,     5,   237,   228,
     228,   225,   102,  1027,  1028,  1029,  1030,  1031,   231,   251,
     252,   228,   228,   255,   256,   228,    86,    86,  1194,   261,
    1196,   228,    44,     4,   228,    44,    46,   193,    96,    96,
     228,   228,   909,   228,   100,  1211,   109,   197,   141,   141,
     212,   213,   214,   215,   216,   106,   218,   219,   220,   221,
     222,   223,   224,    87,    88,   145,   233,   156,   230,   151,
     212,   213,   214,   215,   216,   152,   218,   219,   220,   221,
     222,   223,   224,   228,   228,     3,   235,   228,   230,   251,
     252,     4,   231,   289,   228,   231,  1065,   232,   228,   261,
     228,   228,   228,   228,   228,     6,     4,     4,   228,   251,
     252,   228,   228,    14,   236,   236,   228,   228,   228,   261,
      21,    22,    23,    24,    25,    26,   232,   228,    29,    30,
      31,    32,    33,    34,    35,   229,     4,   228,    39,    40,
      41,    42,    43,   228,    45,    86,    87,    88,     4,    50,
      51,    52,    53,    54,    55,    56,    57,  1024,    59,    60,
      61,    86,    63,   228,    65,    66,    67,    68,    69,    70,
       4,   145,     3,    74,    75,   228,    77,   229,   228,   228,
     228,   228,   228,    84,   228,   228,   228,   228,   212,   213,
     214,   215,   216,   228,   218,   219,   220,   221,   222,   223,
     224,   228,   228,   239,   105,   288,   230,   108,   228,   245,
    1179,   247,   228,   249,   229,   251,     4,   253,   231,   255,
     228,   257,    16,   259,     3,   261,   230,   251,   252,   228,
     131,   231,  1201,   228,   237,  1133,   137,   261,   139,   140,
      86,   228,   143,   144,   230,   146,   231,   148,   229,   229,
     229,   229,   229,   154,   229,   231,   157,   158,    87,    88,
    1158,   229,   163,   164,   229,   166,   229,   168,   229,   229,
     229,   212,   213,   214,   215,   216,   229,   218,   219,   220,
     221,   222,   223,   224,   246,   229,   248,   229,   250,   230,
     252,   229,   254,   229,   256,   228,   258,   228,   260,     5,
     262,   228,   203,   204,   205,   206,     4,   208,   228,   193,
     251,   252,     4,   193,    96,   236,   228,   228,    89,    90,
     261,    96,    72,   228,   228,   103,   103,     4,   234,   103,
     116,   232,   228,   228,   115,   122,   127,   238,   239,   231,
     231,   231,   231,   123,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   160,   192,   135,   136,   228,     4,   228,   228,
       4,   142,   229,   228,     4,   233,   229,   228,   228,   228,
     228,   228,     4,   212,   213,   214,   215,   216,   228,   218,
     219,   220,   221,   222,   223,   224,   228,     4,   228,    17,
     228,   230,   173,   228,   228,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   231,   251,   252,     5,   231,    47,   239,   229,   229,
     229,     3,   261,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   107,     4,   229,    73,    98,   229,   229,   110,   229,
     229,   229,   228,   228,   231,   236,   228,   231,   229,   229,
     229,   229,   229,   229,   229,   229,   247,   248,   229,   110,
     229,   110,   110,   231,   228,     4,     7,   229,   235,   228,
     228,   231,    87,     4,     4,   150,   228,     4,   229,   104,
     228,   228,   103,   229,   228,   228,     4,     4,   228,     4,
     229,     5,    18,    47,   231,   106,   229,     4,    27,   231,
     193,   231,    98,    97,   111,   228,   289,   231,   228,   231,
     226,   228,   231,     4,   229,    28,   229,   231,   228,   228,
     102,   229,   193,    97,   228,   228,     4,   229,     4,   228,
     231,     5,  1200,   422,   229,    73,  1014,   491,   681,   220,
     562,    66,   253,   980,   860,  1009,  1076,  1052,  1139,   184,
    1066,   755,  1144,  1101,   841,   429,   526,   794,   287,    -1,
      -1,   586,   555
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    19,    20,   292,   293,   294,   295,   229,   229,     0,
     293,   133,   305,   236,   236,   229,     6,    14,    21,    22,
      23,    24,    25,    26,    29,    30,    31,    32,    33,    34,
      35,    39,    40,    41,    42,    43,    45,    50,    51,    52,
      53,    54,    55,    56,    57,    59,    60,    61,    63,    65,
      66,    67,    68,    69,    70,    74,    75,    77,    84,   105,
     108,   131,   137,   139,   140,   143,   144,   146,   148,   154,
     157,   158,   163,   164,   166,   168,   203,   204,   205,   206,
     208,   232,   238,   239,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   296,   297,   298,   299,   300,   307,   309,   310,
     311,   312,   314,   315,   323,   326,   327,   329,   330,   337,
     339,   340,   342,   344,   346,   350,   351,   352,   355,   357,
     364,   371,   374,   380,   385,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   396,   397,   398,   399,   403,   404,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   417,
     418,   419,   420,   421,   422,   423,   424,   425,   426,   427,
     428,   429,   430,   431,   228,   134,   306,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   329,   229,   229,   229,   229,
     232,   239,   311,   313,   331,   229,   229,   229,   229,   238,
       4,   230,   245,   246,   281,   464,   474,   230,   307,   287,
     229,   341,     3,   228,   236,   247,   248,   450,   463,    10,
      11,    12,    13,     4,   102,   475,    87,    88,    91,    92,
     219,   230,   251,   252,   372,   452,   453,   106,   212,   213,
     214,   215,   216,   218,   220,   221,   222,   223,   224,   230,
     261,   452,   472,   473,     3,    87,   101,   230,   236,   259,
     260,   382,   461,   462,   382,     3,    87,   230,   236,   257,
     258,   376,   459,   460,    87,   236,   432,    78,    87,   444,
     452,     4,   452,   192,   473,   230,   253,   254,   455,   456,
     455,    62,   230,   482,   483,   236,   452,   452,    99,   452,
     230,   452,   482,   455,   455,    87,   236,   354,   433,   482,
     482,   482,   451,   464,   230,   353,   464,   452,   473,   473,
     452,   473,   473,     4,     8,     9,    87,   476,   452,   473,
     452,   354,   473,   455,   473,     5,    87,   365,   347,   473,
     455,   155,   310,   451,   287,   232,   452,   452,   452,   452,
     230,     8,   464,   232,   210,   233,   282,   283,   284,   285,
     286,   463,   464,   469,   470,   471,    29,    31,    36,    37,
      38,   146,   217,   230,   234,   452,   455,   463,   464,   478,
     479,   480,   345,   234,   308,   228,   306,   232,   228,   288,
     228,   228,   228,   227,   454,   230,     4,   232,   228,   228,
     359,   455,   455,   452,   452,   230,   452,   230,   230,   230,
     472,   230,   473,   290,   228,     3,   232,   383,   381,     3,
     232,   375,   228,   228,   228,   228,   228,   228,     4,   232,
     228,   228,   230,     4,   228,   228,   228,   228,   228,   228,
       4,   228,   228,   228,     6,   328,   228,   228,   228,   228,
       4,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   308,   348,   228,   228,   228,   233,
     464,   451,   228,   228,   228,   228,   324,   231,   231,   464,
     464,   464,   464,   464,   464,   464,   228,   231,   229,     4,
     319,   320,   321,   322,   453,   456,   463,   464,   229,   229,
     229,   309,   307,    79,    87,   445,   464,    15,    16,   450,
       3,     3,   230,   405,     3,   228,   233,   473,   228,   464,
      92,    93,   373,     5,   237,   228,   228,    86,   367,   473,
     452,   452,     3,     7,   230,   255,   256,   457,   458,   473,
     225,   231,   473,   102,   228,   464,   228,   308,   228,   464,
     308,   452,   452,   473,    86,   317,   460,    86,   228,   464,
     445,    44,     4,   228,    44,    46,    87,    89,    90,   446,
     449,   193,   452,    96,   228,    96,   482,    87,   230,   360,
     228,   482,   482,   482,   451,   228,   353,   476,   452,    82,
      87,   481,   100,    71,   200,   197,   106,   109,    87,   230,
     361,     4,   106,   141,   141,   444,   349,   230,   457,   457,
     156,   343,   165,   288,   338,   233,    87,   463,   151,   152,
     153,   155,   312,   466,   467,   468,   233,   233,   469,   473,
     228,   228,   235,   228,   235,   228,   235,   228,   235,   234,
     461,   234,   459,   234,   457,   235,   233,   228,     3,   228,
     454,   231,     4,   233,   359,   452,   452,   289,   228,   231,
     228,   228,   228,     3,   232,   228,   228,   228,   236,   233,
       4,     8,     9,    64,    78,    79,    80,    81,    83,    85,
     117,   118,   119,   121,   124,   125,   126,   128,   129,   130,
     132,   159,   161,   162,   170,   171,   172,   173,   174,   175,
     193,   207,   384,   452,   474,   477,   236,   233,   377,   369,
     228,   228,   228,     4,   233,   228,   228,     4,   228,   232,
     228,   229,   228,     4,   228,     4,   228,   445,   228,   228,
     395,   228,    86,   228,   228,   229,   400,   228,   228,     4,
     228,   228,   228,   228,   145,   228,   228,   308,   288,   464,
     332,   228,   228,   229,   228,   231,     4,   456,   453,   464,
     463,   316,   462,   317,   318,   458,    16,   228,     3,   231,
     237,   228,   367,   473,   464,   464,   473,   228,   464,   473,
     473,   230,   231,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     231,   228,   228,   444,   460,     5,   228,   433,     4,   228,
     193,     4,    87,    94,   448,   459,   193,    96,   228,    87,
     230,   362,   236,    96,   451,   228,   463,    72,   452,   228,
     103,     4,   110,   112,   113,   114,   228,    87,   230,   363,
     103,     4,    87,   366,   367,   308,   103,   457,   234,   445,
      87,   463,   241,   242,   465,   466,   325,   228,   235,   235,
     228,   235,   228,   445,   228,   464,   231,   228,   231,   228,
     231,   445,   233,   231,   231,   464,   451,   461,   116,   115,
     451,   122,   123,   127,   451,   451,   451,   463,   160,   451,
     451,   451,   459,    89,    90,   135,   136,   142,   173,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   378,   447,   449,   463,    83,
     120,   121,   129,   138,   149,   175,   370,   228,   303,     4,
     228,   434,   228,     4,   229,   233,   228,   229,     4,   372,
     228,   228,     4,    87,   368,   228,   228,    97,   104,   129,
     149,   193,   194,   195,   196,   198,   199,   200,   201,   202,
     401,   228,   228,   228,   228,     4,     4,   434,   228,   228,
     228,   167,   169,   333,   334,   308,   462,   458,    17,   231,
       5,    47,   464,   231,   283,   229,   463,   229,   229,   229,
     229,   229,   229,   229,   229,   229,     3,   228,    47,    48,
      49,   435,   436,   107,   437,     4,   228,   459,    95,   193,
     459,   231,   228,   363,   451,   231,    73,    98,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   110,   110,
     110,   110,   231,   228,   437,     4,   368,     7,   476,   229,
     235,   333,   228,   228,   231,   228,   231,   464,    87,   451,
     451,   451,   451,   451,   192,   236,   379,   461,     4,   150,
       4,   228,   229,   104,   439,   228,     4,   228,   229,   373,
     434,   228,   228,   103,   440,     4,     4,    98,   100,   150,
     402,   459,   461,   405,   405,   452,   432,     4,   228,     4,
     439,   369,   229,   336,    18,     5,    47,   228,   301,   289,
     231,   435,   106,   438,   229,    27,   442,     4,   231,   193,
     459,   231,   437,    98,    97,   111,   231,   442,   335,   307,
     228,   228,   231,   226,   231,   379,   440,   229,    28,   443,
     231,   229,   439,   228,   228,   443,   307,     4,   445,   228,
     102,   229,   193,   441,   459,   442,    97,   445,   441,   228,
     228,   452,     4,   229,   415,   443,   356,     4,     5,    87,
     304,   228,   302,   459,   308,   416,   441,   308,   303,   452,
     231,   358,   308
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
		      add_opvars(splev, "iiiiiio", VA_PASS7(0, 0, 0, 0, NO_COLOR, ATR_NONE, SPO_MESSAGE));
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1282 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio", VA_PASS7(0, 0, 0, 0, (int)(yyvsp[(5) - (5)].i), ATR_NONE, SPO_MESSAGE));
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1286 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio", VA_PASS7((int)(yyvsp[(15) - (15)].i), (int)(yyvsp[(13) - (15)].i), (int)(yyvsp[(11) - (15)].i), (int)(yyvsp[(9) - (15)].i), (int)(yyvsp[(7) - (15)].i), (int)(yyvsp[(5) - (15)].i), SPO_MESSAGE));
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
#line 1748 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1757 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1761 "lev_comp.y"
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

  case 269:

/* Line 1455 of yacc.c  */
#line 1774 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1782 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1792 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1800 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1809 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1816 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1824 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1830 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1835 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1840 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1845 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1850 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1855 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1860 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1865 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1870 "lev_comp.y"
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

  case 285:

/* Line 1455 of yacc.c  */
#line 1884 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1890 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1895 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1900 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1905 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1910 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1915 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1920 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1925 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1930 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1935 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1940 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1945 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x00200000;
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1950 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x00400000;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1955 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x00800000;
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1960 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x01000000;
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1965 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_MYTHIC_TYPE));
		      (yyval.i) = 0x02000000;
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 1971 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x04000000;
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 1976 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x08000000;
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 1981 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x10000000;
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 1986 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MATERIAL));
		      (yyval.i) = 0x20000000;
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 1993 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 1999 "lev_comp.y"
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

  case 308:

/* Line 1455 of yacc.c  */
#line 2028 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2033 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2041 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2047 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2051 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2059 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2066 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2073 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2083 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2093 "lev_comp.y"
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

  case 318:

/* Line 1455 of yacc.c  */
#line 2109 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2119 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2123 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2129 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2135 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2141 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2145 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2151 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2157 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2164 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2172 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2178 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2183 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2188 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2193 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2198 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2204 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2210 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2215 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2220 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2225 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2230 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2235 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2245 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2251 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2257 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2262 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2269 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2276 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2280 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2286 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2292 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2298 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2303 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2310 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2316 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2326 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2332 "lev_comp.y"
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

  case 360:

/* Line 1455 of yacc.c  */
#line 2353 "lev_comp.y"
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

  case 361:

/* Line 1455 of yacc.c  */
#line 2365 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2369 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2375 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2380 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2385 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (long)(yyvsp[(9) - (9)].i), (long)(yyvsp[(7) - (9)].i), (long)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2390 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((long)(yyvsp[(9) - (13)].i), (long)(yyvsp[(7) - (13)].i), (long)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2396 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2400 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2406 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, (int)(yyvsp[(11) - (11)].i), (int)(yyvsp[(9) - (11)].i), (int)(yyvsp[(7) - (11)].i), (int)(yyvsp[(5) - (11)].i), SPO_DECORATION));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2410 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6((int)(yyvsp[(13) - (13)].i), (int)(yyvsp[(11) - (13)].i), (int)(yyvsp[(9) - (13)].i), (int)(yyvsp[(7) - (13)].i), (int)(yyvsp[(5) - (13)].i), SPO_DECORATION));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2414 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2418 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2422 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2426 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, 1, (int)(yyvsp[(7) - (7)].i), 0, (int)(yyvsp[(5) - (7)].i), SPO_DECORATION));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2432 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2438 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2442 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2448 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_CARPET));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2454 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2458 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4(0, (int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2464 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2468 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2474 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2478 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2483 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2490 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2494 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2501 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2505 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2510 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2517 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2521 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2526 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2534 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2540 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2547 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2551 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2558 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2569 "lev_comp.y"
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

  case 402:

/* Line 1455 of yacc.c  */
#line 2582 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2586 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2592 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2596 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2603 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2607 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2611 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2617 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2621 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2627 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2633 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2637 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2643 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2649 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2654 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2660 "lev_comp.y"
    {
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

  case 427:

/* Line 1455 of yacc.c  */
#line 2690 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2698 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2708 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2717 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2722 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2729 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2740 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2746 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2750 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2754 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2761 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2771 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2778 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2782 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2788 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2792 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2800 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2804 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2811 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2821 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2834 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2838 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2845 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2855 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_LEVTYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2864 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_LEVTYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2875 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2879 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2886 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2896 "lev_comp.y"
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

  case 460:

/* Line 1455 of yacc.c  */
#line 2907 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2916 "lev_comp.y"
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

  case 462:

/* Line 1455 of yacc.c  */
#line 2926 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2932 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2936 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2943 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2953 "lev_comp.y"
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

  case 467:

/* Line 1455 of yacc.c  */
#line 2964 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2973 "lev_comp.y"
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

  case 469:

/* Line 1455 of yacc.c  */
#line 2983 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2987 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 2993 "lev_comp.y"
    { ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 2995 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3001 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3005 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3009 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3013 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3021 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3030 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3034 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3038 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3042 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3046 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3049 "lev_comp.y"
    { ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3053 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3060 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3069 "lev_comp.y"
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

  case 491:

/* Line 1455 of yacc.c  */
#line 3110 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3114 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3121 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3128 "lev_comp.y"
    {
			      size_t len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3138 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3142 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3150 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3154 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3158 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3162 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3166 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3171 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3175 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3179 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3184 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3189 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3194 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3198 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3203 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3208 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3212 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3216 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3221 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3225 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 3232 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 516:

/* Line 1455 of yacc.c  */
#line 3238 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 517:

/* Line 1455 of yacc.c  */
#line 3242 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 518:

/* Line 1455 of yacc.c  */
#line 3248 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 524:

/* Line 1455 of yacc.c  */
#line 3264 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 525:

/* Line 1455 of yacc.c  */
#line 3268 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 526:

/* Line 1455 of yacc.c  */
#line 3272 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 527:

/* Line 1455 of yacc.c  */
#line 3276 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 536:

/* Line 1455 of yacc.c  */
#line 3298 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 537:

/* Line 1455 of yacc.c  */
#line 3302 "lev_comp.y"
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

  case 538:

/* Line 1455 of yacc.c  */
#line 3328 "lev_comp.y"
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
#line 7568 "lev_comp.tab.c"
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
#line 3356 "lev_comp.y"


/*lev_comp.y*/

