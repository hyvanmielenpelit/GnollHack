
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
     SUBROOM_ID = 387,
     NAME_ID = 388,
     FLAGS_ID = 389,
     FLAG_TYPE = 390,
     MON_ATTITUDE = 391,
     MON_ALERTNESS = 392,
     SUBTYPE_ID = 393,
     NON_PASSDOOR_ID = 394,
     CARPET_ID = 395,
     CARPET_PIECE_ID = 396,
     CARPET_TYPE = 397,
     MON_APPEARANCE = 398,
     ROOMDOOR_ID = 399,
     IF_ID = 400,
     ELSE_ID = 401,
     TERRAIN_ID = 402,
     HORIZ_OR_VERT = 403,
     REPLACE_TERRAIN_ID = 404,
     LOCATION_SUBTYPE_ID = 405,
     DOOR_SUBTYPE = 406,
     BRAZIER_SUBTYPE = 407,
     SIGNPOST_SUBTYPE = 408,
     TREE_SUBTYPE = 409,
     FOREST_ID = 410,
     FOREST_TYPE = 411,
     INITIALIZE_TYPE = 412,
     EXIT_ID = 413,
     SHUFFLE_ID = 414,
     MANUAL_TYPE_ID = 415,
     MANUAL_TYPE = 416,
     QUANTITY_ID = 417,
     BURIED_ID = 418,
     LOOP_ID = 419,
     FOR_ID = 420,
     TO_ID = 421,
     SWITCH_ID = 422,
     CASE_ID = 423,
     BREAK_ID = 424,
     DEFAULT_ID = 425,
     ERODED_ID = 426,
     TRAPPED_STATE = 427,
     RECHARGED_ID = 428,
     INVIS_ID = 429,
     GREASED_ID = 430,
     INDESTRUCTIBLE_ID = 431,
     FEMALE_ID = 432,
     MALE_ID = 433,
     WAITFORU_ID = 434,
     PROTECTOR_ID = 435,
     CANCELLED_ID = 436,
     REVIVED_ID = 437,
     AVENGE_ID = 438,
     FLEEING_ID = 439,
     BLINDED_ID = 440,
     MAXHP_ID = 441,
     LEVEL_ADJUSTMENT_ID = 442,
     KEEP_ORIGINAL_INVENTORY_ID = 443,
     PARALYZED_ID = 444,
     STUNNED_ID = 445,
     CONFUSED_ID = 446,
     SEENTRAPS_ID = 447,
     ALL_ID = 448,
     MONTYPE_ID = 449,
     OBJTYPE_ID = 450,
     TERTYPE_ID = 451,
     TERTYPE2_ID = 452,
     LEVER_EFFECT_TYPE = 453,
     SWITCHABLE_ID = 454,
     CONTINUOUSLY_USABLE_ID = 455,
     TARGET_ID = 456,
     TRAPTYPE_ID = 457,
     EFFECT_FLAG_ID = 458,
     GRAVE_ID = 459,
     BRAZIER_ID = 460,
     SIGNPOST_ID = 461,
     TREE_ID = 462,
     ERODEPROOF_ID = 463,
     FUNCTION_ID = 464,
     MSG_OUTPUT_TYPE = 465,
     COMPARE_TYPE = 466,
     UNKNOWN_TYPE = 467,
     rect_ID = 468,
     fillrect_ID = 469,
     line_ID = 470,
     randline_ID = 471,
     grow_ID = 472,
     selection_ID = 473,
     flood_ID = 474,
     rndcoord_ID = 475,
     circle_ID = 476,
     ellipse_ID = 477,
     filter_ID = 478,
     complement_ID = 479,
     gradient_ID = 480,
     GRADIENT_TYPE = 481,
     LIMITED = 482,
     HUMIDITY_TYPE = 483,
     STRING = 484,
     MAP_ID = 485,
     NQSTRING = 486,
     VARSTRING = 487,
     CFUNC = 488,
     CFUNC_INT = 489,
     CFUNC_STR = 490,
     CFUNC_COORD = 491,
     CFUNC_REGION = 492,
     VARSTRING_INT = 493,
     VARSTRING_INT_ARRAY = 494,
     VARSTRING_STRING = 495,
     VARSTRING_STRING_ARRAY = 496,
     VARSTRING_VAR = 497,
     VARSTRING_VAR_ARRAY = 498,
     VARSTRING_COORD = 499,
     VARSTRING_COORD_ARRAY = 500,
     VARSTRING_REGION = 501,
     VARSTRING_REGION_ARRAY = 502,
     VARSTRING_MAPCHAR = 503,
     VARSTRING_MAPCHAR_ARRAY = 504,
     VARSTRING_MONST = 505,
     VARSTRING_MONST_ARRAY = 506,
     VARSTRING_OBJ = 507,
     VARSTRING_OBJ_ARRAY = 508,
     VARSTRING_SEL = 509,
     VARSTRING_SEL_ARRAY = 510,
     METHOD_INT = 511,
     METHOD_INT_ARRAY = 512,
     METHOD_STRING = 513,
     METHOD_STRING_ARRAY = 514,
     METHOD_VAR = 515,
     METHOD_VAR_ARRAY = 516,
     METHOD_COORD = 517,
     METHOD_COORD_ARRAY = 518,
     METHOD_REGION = 519,
     METHOD_REGION_ARRAY = 520,
     METHOD_MAPCHAR = 521,
     METHOD_MAPCHAR_ARRAY = 522,
     METHOD_MONST = 523,
     METHOD_MONST_ARRAY = 524,
     METHOD_OBJ = 525,
     METHOD_OBJ_ARRAY = 526,
     METHOD_SEL = 527,
     METHOD_SEL_ARRAY = 528,
     DICE = 529
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
#line 574 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 586 "lev_comp.tab.c"

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
#define YYLAST   1389

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  292
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  193
/* YYNRULES -- Number of rules.  */
#define YYNRULES  538
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1215

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   529

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   287,   291,     2,
     231,   232,   285,   283,   229,   284,   289,   286,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   230,     2,
       2,   288,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   233,     2,   234,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   235,   290,   236,     2,     2,     2,     2,
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
     225,   226,   227,   228,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282
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
     838,   840,   842,   844,   846,   850,   852,   854,   856,   860,
     864,   865,   871,   874,   875,   879,   881,   885,   887,   891,
     895,   897,   899,   903,   905,   907,   909,   913,   915,   917,
     919,   923,   927,   931,   935,   939,   943,   947,   951,   953,
     955,   957,   959,   963,   967,   971,   975,   981,   989,   995,
    1004,  1006,  1010,  1014,  1020,  1026,  1034,  1042,  1049,  1055,
    1056,  1059,  1065,  1069,  1081,  1095,  1102,  1103,  1107,  1109,
    1113,  1117,  1121,  1125,  1127,  1129,  1133,  1137,  1141,  1145,
    1149,  1153,  1155,  1157,  1159,  1161,  1165,  1169,  1171,  1177,
    1187,  1193,  1201,  1205,  1209,  1215,  1225,  1231,  1237,  1243,
    1244,  1260,  1261,  1263,  1271,  1283,  1293,  1307,  1311,  1319,
    1331,  1345,  1355,  1365,  1375,  1383,  1391,  1397,  1405,  1411,
    1421,  1429,  1435,  1445,  1451,  1457,  1461,  1469,  1473,  1481,
    1489,  1493,  1499,  1505,  1509,  1515,  1523,  1533,  1535,  1537,
    1539,  1541,  1543,  1544,  1547,  1549,  1553,  1555,  1557,  1559,
    1560,  1564,  1566,  1567,  1571,  1573,  1574,  1578,  1579,  1583,
    1584,  1588,  1590,  1592,  1594,  1596,  1598,  1600,  1602,  1604,
    1606,  1610,  1612,  1614,  1619,  1621,  1623,  1628,  1630,  1632,
    1637,  1639,  1644,  1650,  1652,  1656,  1658,  1662,  1664,  1666,
    1671,  1681,  1683,  1685,  1690,  1692,  1698,  1700,  1702,  1707,
    1709,  1711,  1717,  1719,  1721,  1723,  1728,  1730,  1732,  1738,
    1740,  1742,  1744,  1748,  1750,  1752,  1756,  1758,  1763,  1767,
    1771,  1775,  1779,  1783,  1787,  1789,  1791,  1795,  1797,  1801,
    1802,  1804,  1806,  1808,  1810,  1814,  1815,  1817,  1819,  1822,
    1825,  1830,  1837,  1842,  1849,  1856,  1863,  1870,  1873,  1880,
    1889,  1898,  1909,  1924,  1927,  1929,  1933,  1935,  1939,  1941,
    1943,  1945,  1947,  1949,  1951,  1953,  1955,  1957,  1959,  1961,
    1963,  1965,  1967,  1969,  1971,  1973,  1975,  1977,  1988
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     293,     0,    -1,    -1,   294,    -1,   295,    -1,   295,   294,
      -1,   296,   306,   308,    -1,    21,   230,   237,    -1,    20,
     230,   237,   229,     3,    -1,    20,   230,   237,   229,     3,
     229,   446,    -1,    22,   230,    11,   229,   406,    -1,    22,
     230,    10,   229,     3,    -1,    22,   230,    13,    -1,    22,
     230,    12,   229,     3,   229,     3,   229,     5,   229,     5,
     229,   446,   229,   305,   304,    -1,    23,   230,   476,    -1,
     155,   230,   156,   229,   157,    -1,   155,   230,   156,    -1,
      36,   230,     4,   229,   318,    -1,    26,   230,   107,    -1,
      -1,   229,   227,    -1,    -1,   229,   453,    -1,    -1,   229,
       3,    -1,     5,    -1,    88,    -1,    -1,   134,   230,   307,
      -1,   135,   229,   307,    -1,   135,    -1,    -1,   310,   308,
      -1,   235,   308,   236,    -1,   351,    -1,   297,    -1,   298,
      -1,   414,    -1,   301,    -1,   299,    -1,   422,    -1,   423,
      -1,   424,    -1,   300,    -1,   421,    -1,   420,    -1,   418,
      -1,   419,    -1,   425,    -1,   426,    -1,   427,    -1,   428,
      -1,   429,    -1,   395,    -1,   353,    -1,   316,    -1,   315,
      -1,   409,    -1,   365,    -1,   387,    -1,   431,    -1,   432,
      -1,   397,    -1,   398,    -1,   399,    -1,   430,    -1,   331,
      -1,   341,    -1,   343,    -1,   347,    -1,   345,    -1,   328,
      -1,   338,    -1,   324,    -1,   327,    -1,   390,    -1,   400,
      -1,   372,    -1,   388,    -1,   375,    -1,   381,    -1,   410,
      -1,   405,    -1,   393,    -1,   352,    -1,   411,    -1,   412,
      -1,   413,    -1,   415,    -1,   358,    -1,   356,    -1,   404,
      -1,   408,    -1,   407,    -1,   391,    -1,   392,    -1,   394,
      -1,   386,    -1,   389,    -1,   247,    -1,   249,    -1,   251,
      -1,   253,    -1,   255,    -1,   257,    -1,   259,    -1,   261,
      -1,   263,    -1,   246,    -1,   248,    -1,   250,    -1,   252,
      -1,   254,    -1,   256,    -1,   258,    -1,   260,    -1,   262,
      -1,   311,    -1,   312,    -1,   240,    -1,   240,    -1,   312,
      -1,   159,   230,   311,    -1,   313,   288,   465,    -1,   313,
     288,   218,   230,   474,    -1,   313,   288,   464,    -1,   313,
     288,   481,   230,   458,    -1,   313,   288,   480,   230,   460,
      -1,   313,   288,   479,   230,   462,    -1,   313,   288,   453,
      -1,   313,   288,   456,    -1,   313,   288,   235,   322,   236,
      -1,   313,   288,   235,   321,   236,    -1,   313,   288,   235,
     320,   236,    -1,   313,   288,   481,   230,   235,   319,   236,
      -1,   313,   288,   480,   230,   235,   318,   236,    -1,   313,
     288,   479,   230,   235,   317,   236,    -1,   313,   288,   235,
     323,   236,    -1,   463,    -1,   317,   229,   463,    -1,   461,
      -1,   318,   229,   461,    -1,   459,    -1,   319,   229,   459,
      -1,   457,    -1,   320,   229,   457,    -1,   454,    -1,   321,
     229,   454,    -1,   465,    -1,   322,   229,   465,    -1,   464,
      -1,   323,   229,   464,    -1,    -1,    -1,   209,   239,   231,
     325,   469,   232,   326,   309,    -1,   239,   231,   472,   232,
      -1,   158,    -1,    -1,     6,    -1,     6,    -1,   233,   465,
     211,   465,   234,    -1,   233,   465,   234,    -1,    -1,    -1,
     167,   332,   233,   452,   234,   333,   235,   334,   236,    -1,
      -1,   335,   334,    -1,    -1,   168,   477,   230,   336,   308,
      -1,    -1,   170,   230,   337,   308,    -1,   169,    -1,   289,
     289,    -1,   166,    -1,   165,   314,   288,   465,   339,   465,
      -1,    -1,   340,   342,   309,    -1,    -1,   164,   233,   452,
     234,   344,   309,    -1,    -1,   330,   230,   346,   310,    -1,
      -1,   145,   330,   348,   349,    -1,   309,    -1,    -1,   309,
     350,   146,   309,    -1,    14,   230,   464,    -1,    14,   230,
     464,   229,    15,   229,    16,   229,    17,   229,     4,    -1,
      14,   230,   464,   229,    15,   229,    16,   229,    17,   229,
      18,   229,    19,   229,     4,   229,     4,    -1,    78,    -1,
      78,   230,   477,    -1,    78,   230,    88,    -1,    66,   230,
     354,   229,   354,    -1,    66,   230,   354,   229,   477,    -1,
     231,     4,   229,    87,   229,   369,   232,    -1,   434,   329,
     229,   446,    -1,    -1,   132,   230,   355,   229,   362,   229,
     364,   435,   438,   440,   443,   444,   442,   357,   309,    -1,
      -1,    58,   230,   355,   229,   361,   229,   363,   229,   364,
     435,   438,   440,   443,   444,   442,   359,   309,    -1,    -1,
     229,     5,    -1,   231,     4,   229,     4,   232,    -1,    88,
      -1,   231,     4,   229,     4,   232,    -1,    88,    -1,   231,
     373,   229,   374,   232,    -1,    88,    -1,   231,     4,   229,
       4,   232,    -1,    88,    -1,   144,   230,   366,   229,   445,
     229,   367,   229,   369,   370,    -1,    34,   230,   445,   229,
     474,   370,    -1,     5,    -1,    88,    -1,   368,    -1,    88,
      -1,    87,    -1,    87,   290,   368,    -1,     4,    -1,    88,
      -1,    -1,   370,   229,   371,    -1,   150,   230,   151,    -1,
      84,   230,   462,    -1,   130,   230,     4,    -1,   176,    -1,
     121,    -1,   122,    -1,   139,    -1,    25,    -1,    24,   230,
     373,   229,   374,   360,   238,    -1,    24,   230,   453,   360,
     238,    -1,    92,    -1,    93,    -1,    94,    -1,    93,    -1,
      32,   230,   377,    -1,    -1,    32,   230,   377,   376,   309,
      -1,   460,   229,   453,   378,    -1,    -1,   378,   229,   379,
      -1,   464,    -1,   136,    -1,   137,    -1,   448,    -1,   143,
     464,    -1,   177,    -1,   178,    -1,   174,    -1,   181,    -1,
     182,    -1,   182,   230,   452,    -1,   183,    -1,   184,   230,
     452,    -1,   185,   230,   452,    -1,   189,   230,   452,    -1,
     190,    -1,   191,    -1,   192,   230,   380,    -1,   192,    -1,
     179,    -1,   180,    -1,   186,    -1,   187,   230,   452,    -1,
     188,    -1,   237,    -1,   193,    -1,   237,   290,   380,    -1,
      30,   230,   383,    -1,    -1,    31,   230,   383,   382,   309,
      -1,   462,   384,    -1,    -1,   384,   229,   385,    -1,    81,
      -1,   194,   230,   460,    -1,   478,    -1,   133,   230,   464,
      -1,   162,   230,   452,    -1,   163,    -1,    80,    -1,   171,
     230,   452,    -1,   208,    -1,    79,    -1,   172,    -1,   173,
     230,   452,    -1,   174,    -1,   175,    -1,   453,    -1,   119,
     230,   116,    -1,   118,   230,   117,    -1,   120,   230,   452,
      -1,   129,   230,   452,    -1,   130,   230,   452,    -1,   160,
     230,   161,    -1,   131,   230,   452,    -1,    84,   230,   462,
      -1,   176,    -1,   122,    -1,    86,    -1,    82,    -1,   125,
     230,   123,    -1,   126,   230,   124,    -1,    65,   230,   452,
      -1,   127,   230,   128,    -1,    33,   230,   433,   229,   453,
      -1,    35,   230,   453,   229,    87,   229,   445,    -1,    40,
     230,   453,   229,    87,    -1,    40,   230,   453,   229,    87,
     229,     5,   304,    -1,    41,    -1,    41,   230,   474,    -1,
      41,   230,   193,    -1,    54,   230,   453,   229,    97,    -1,
      55,   230,   453,   229,    97,    -1,    55,   230,   483,   229,
     483,   229,    97,    -1,    60,   230,   483,   229,   483,   229,
     237,    -1,    61,   230,   483,   229,   483,   396,    -1,    62,
     230,   483,   229,   483,    -1,    -1,   229,    97,    -1,    69,
     230,   474,   229,   101,    -1,    70,   230,   474,    -1,    71,
     230,   453,   229,   201,   230,   453,   229,    99,   229,    98,
      -1,    71,   230,   453,   229,    72,   229,    73,   229,    74,
     229,    99,   229,    98,    -1,    85,   230,   453,   229,   198,
     401,    -1,    -1,   401,   229,   402,    -1,    98,    -1,   194,
     230,   460,    -1,   195,   230,   462,    -1,   196,   230,   406,
      -1,   197,   230,   406,    -1,   199,    -1,   200,    -1,   201,
     230,   453,    -1,   202,   230,   433,    -1,   150,   230,   403,
      -1,   105,   230,   441,    -1,   203,   230,     4,    -1,   130,
     230,     4,    -1,     4,    -1,    99,    -1,   151,    -1,   101,
      -1,    76,   230,   474,    -1,    75,   230,   474,    -1,     3,
      -1,   231,     3,   229,   446,   232,    -1,   149,   230,   456,
     229,   458,   229,   458,   229,     7,    -1,   147,   230,   474,
     229,   458,    -1,   147,   230,   474,   229,   458,   229,   104,
      -1,    56,   230,   456,    -1,    57,   230,   456,    -1,    46,
     230,   237,   229,    47,    -1,    46,   230,   237,   229,    47,
     229,   194,   230,   460,    -1,    43,   230,   456,   229,    45,
      -1,    44,   230,   483,   229,    45,    -1,    27,   230,   474,
     229,   103,    -1,    -1,    42,   230,   456,   229,   446,   229,
     434,   435,   438,   440,   443,   444,   442,   416,   417,    -1,
      -1,   309,    -1,    51,   230,   453,   229,   447,   229,   449,
      -1,    51,   230,   453,   229,   447,   229,   449,   229,   194,
     230,   460,    -1,    51,   230,   453,   229,   447,   229,   449,
     229,    96,    -1,    51,   230,   453,   229,   447,   229,   449,
     229,    96,   229,   194,   230,   460,    -1,    52,   230,   453,
      -1,    52,   230,   453,   229,   194,   230,   460,    -1,   109,
     230,   453,   229,   110,   229,     4,   229,   111,   229,   112,
      -1,   109,   230,   453,   229,   110,   229,     4,   229,   111,
     229,   112,   229,   446,    -1,   109,   230,   453,   229,   110,
     229,   113,   229,   111,    -1,   109,   230,   453,   229,   110,
     229,   114,   229,   111,    -1,   109,   230,   453,   229,   110,
     229,   115,   229,   111,    -1,   109,   230,   453,   229,   110,
     229,   111,    -1,   106,   230,   474,   229,   107,   229,   104,
      -1,   138,   230,   474,   229,     4,    -1,   138,   230,   474,
     229,   107,   229,   104,    -1,   140,   230,   456,   229,   142,
      -1,   141,   230,   474,   229,   142,   229,     4,   229,     4,
      -1,   141,   230,   474,   229,   142,   229,     4,    -1,    53,
     230,   100,   229,   453,    -1,    53,   230,   100,   229,   453,
     229,   194,   230,   460,    -1,   204,   230,   453,   229,   464,
      -1,   204,   230,   453,   229,    88,    -1,   204,   230,   453,
      -1,   205,   230,   453,   229,   152,   229,   446,    -1,   205,
     230,   453,    -1,   206,   230,   453,   229,   153,   229,   464,
      -1,   206,   230,   453,   229,   153,   229,    88,    -1,   206,
     230,   453,    -1,   207,   230,   453,   229,   154,    -1,   207,
     230,   453,   229,   156,    -1,   207,   230,   453,    -1,    67,
     230,   465,   229,   453,    -1,    68,   230,   453,   229,   482,
     229,   464,    -1,    64,   230,   452,   229,   452,   229,   452,
     229,   452,    -1,    64,    -1,   237,    -1,    88,    -1,   237,
      -1,    88,    -1,    -1,   229,   436,    -1,   437,    -1,   437,
     229,   436,    -1,    48,    -1,    49,    -1,    50,    -1,    -1,
     108,   230,   439,    -1,   107,    -1,    -1,   105,   230,   441,
      -1,   104,    -1,    -1,   194,   230,   460,    -1,    -1,    28,
     230,   103,    -1,    -1,    29,   230,     4,    -1,    79,    -1,
      88,    -1,    80,    -1,    88,    -1,    91,    -1,   450,    -1,
      88,    -1,    91,    -1,   450,    -1,    90,   230,    88,    -1,
      95,    -1,    88,    -1,    90,   233,     4,   234,    -1,   237,
      -1,   248,    -1,   249,   233,   465,   234,    -1,   465,    -1,
     454,    -1,   220,   231,   474,   232,    -1,   252,    -1,   253,
     233,   465,   234,    -1,   231,     4,   229,     4,   232,    -1,
      88,    -1,    89,   455,   234,    -1,   228,    -1,   228,   229,
     455,    -1,   457,    -1,   254,    -1,   255,   233,   465,   234,
      -1,   231,     4,   229,     4,   229,     4,   229,     4,   232,
      -1,   459,    -1,   256,    -1,   257,   233,   465,   234,    -1,
       3,    -1,   231,     3,   229,   446,   232,    -1,   461,    -1,
     258,    -1,   259,   233,   465,   234,    -1,   237,    -1,     3,
      -1,   231,     3,   229,   237,   232,    -1,    88,    -1,   463,
      -1,   260,    -1,   261,   233,   465,   234,    -1,   237,    -1,
       3,    -1,   231,     3,   229,   237,   232,    -1,    88,    -1,
     102,    -1,   451,    -1,   464,   289,   451,    -1,     4,    -1,
     475,    -1,   231,     8,   232,    -1,   246,    -1,   247,   233,
     465,   234,    -1,   465,   283,   465,    -1,   465,   284,   465,
      -1,   465,   285,   465,    -1,   465,   286,   465,    -1,   465,
     287,   465,    -1,   231,   465,   232,    -1,   242,    -1,   243,
      -1,   313,   230,   466,    -1,   467,    -1,   468,   229,   467,
      -1,    -1,   468,    -1,   465,    -1,   464,    -1,   470,    -1,
     471,   229,   470,    -1,    -1,   471,    -1,   453,    -1,   213,
     456,    -1,   214,   456,    -1,   215,   453,   229,   453,    -1,
     216,   453,   229,   453,   229,   465,    -1,   217,   231,   474,
     232,    -1,   217,   231,   368,   229,   474,   232,    -1,   223,
     231,     7,   229,   474,   232,    -1,   223,   231,   474,   229,
     474,   232,    -1,   223,   231,   458,   229,   474,   232,    -1,
     219,   453,    -1,   221,   231,   453,   229,   465,   232,    -1,
     221,   231,   453,   229,   465,   229,    48,   232,    -1,   222,
     231,   453,   229,   465,   229,   465,   232,    -1,   222,   231,
     453,   229,   465,   229,   465,   229,    48,   232,    -1,   225,
     231,   226,   229,   231,   465,   284,   465,   302,   232,   229,
     453,   303,   232,    -1,   224,   473,    -1,   262,    -1,   231,
     474,   232,    -1,   473,    -1,   473,   291,   474,    -1,   282,
      -1,   103,    -1,     4,    -1,     8,    -1,     9,    -1,     4,
      -1,     8,    -1,     9,    -1,     4,    -1,   475,    -1,    37,
      -1,    30,    -1,    38,    -1,    32,    -1,    39,    -1,   147,
      -1,    83,    -1,    88,    -1,   484,    -1,    63,   231,     4,
     229,     4,   229,     4,   229,     4,   232,    -1,   231,     4,
     229,     4,   229,     4,   229,     4,   232,    -1
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
     475,   476,   477,   478,   479,   480,   481,   482,   483,    44,
      58,    40,    41,    91,    93,   123,   125,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,    43,    45,    42,    47,    37,    61,    46,
     124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   292,   293,   293,   294,   294,   295,   296,   296,   296,
     297,   297,   297,   297,   298,   299,   299,   300,   301,   302,
     302,   303,   303,   304,   304,   305,   305,   306,   306,   307,
     307,   308,   308,   309,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   313,   313,   313,
     314,   314,   315,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   317,   317,
     318,   318,   319,   319,   320,   320,   321,   321,   322,   322,
     323,   323,   325,   326,   324,   327,   328,   329,   329,   330,
     330,   330,   332,   333,   331,   334,   334,   336,   335,   337,
     335,   338,   339,   339,   340,   342,   341,   344,   343,   346,
     345,   348,   347,   349,   350,   349,   351,   351,   351,   352,
     352,   352,   353,   353,   354,   355,   357,   356,   359,   358,
     360,   360,   361,   361,   362,   362,   363,   363,   364,   364,
     365,   365,   366,   366,   367,   367,   368,   368,   369,   369,
     370,   370,   371,   371,   371,   371,   371,   371,   371,   372,
     372,   372,   373,   373,   374,   374,   375,   376,   375,   377,
     378,   378,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   379,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   379,   379,   379,   379,   379,   380,   380,   380,   381,
     382,   381,   383,   384,   384,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   386,   387,   388,   388,
     389,   389,   389,   390,   391,   392,   393,   394,   395,   396,
     396,   397,   398,   399,   399,   400,   401,   401,   402,   402,
     402,   402,   402,   402,   402,   402,   402,   402,   402,   402,
     402,   403,   403,   403,   403,   404,   405,   406,   406,   407,
     408,   408,   409,   410,   411,   411,   412,   413,   414,   416,
     415,   417,   417,   418,   418,   418,   418,   419,   419,   420,
     420,   420,   420,   420,   420,   421,   422,   422,   423,   424,
     424,   425,   425,   426,   426,   426,   427,   427,   428,   428,
     428,   429,   429,   429,   430,   431,   432,   432,   433,   433,
     434,   434,   435,   435,   436,   436,   437,   437,   437,   438,
     438,   439,   440,   440,   441,   442,   442,   443,   443,   444,
     444,   445,   445,   446,   446,   447,   447,   447,   448,   448,
     448,   449,   449,   450,   451,   451,   451,   452,   453,   453,
     453,   453,   454,   454,   454,   455,   455,   456,   456,   456,
     457,   458,   458,   458,   459,   459,   460,   460,   460,   461,
     461,   461,   461,   462,   462,   462,   463,   463,   463,   463,
     463,   464,   464,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   466,   466,   467,   468,   468,   469,
     469,   470,   470,   471,   471,   472,   472,   473,   473,   473,
     473,   473,   473,   473,   473,   473,   473,   473,   473,   473,
     473,   473,   473,   473,   473,   473,   474,   474,   475,   476,
     476,   477,   477,   477,   478,   478,   478,   478,   479,   479,
     480,   480,   481,   481,   482,   482,   483,   483,   484
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
       0,   180,     0,   423,   424,     9,     0,     0,   472,    11,
     347,     0,    10,     0,     0,   444,     0,     0,     0,   235,
     234,   200,   201,   231,     0,     0,   216,     0,     0,     0,
       0,   454,     0,     0,   452,     0,     0,   451,     0,     0,
     515,   517,   358,     0,     0,     0,   271,     0,     0,   238,
     240,   306,   220,     0,    17,   140,   308,     0,     0,     0,
     356,     0,     0,   357,   354,   427,     0,   425,     0,   426,
       0,   381,   313,     0,   314,     0,   203,     0,     0,     0,
       0,   319,   318,     0,     0,   192,   193,   394,   534,   535,
       0,   321,     0,     0,   326,     0,     0,   205,     0,     0,
     376,     0,   378,     0,     0,     0,     0,   350,     0,    15,
       0,   173,     0,     0,   163,   384,   383,     0,     0,   391,
     392,     0,   487,   490,     0,   477,   160,   494,   124,     0,
       0,   133,     0,   132,     0,   131,     0,   137,     0,   128,
       0,   127,     0,   126,    33,   436,     0,     0,     0,   446,
     439,     0,   441,     0,   500,     0,     0,     0,   502,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   465,   526,
     524,   525,     0,   284,   281,   275,   301,     0,   300,     0,
       0,     0,   299,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   280,     0,   285,     0,   287,   288,   298,     0,
     283,   274,   289,   527,   277,     0,   458,   239,   211,     0,
       0,     0,     0,   449,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   195,     0,     0,   317,
       0,     0,     0,     0,     0,   325,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   178,   172,   174,     0,
       0,     0,     0,     0,   153,     0,   145,   147,   149,   151,
       0,   138,     0,     0,   142,     0,     0,     0,   442,   230,
       0,   217,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   468,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   461,
       0,     0,   307,   141,    23,     0,   402,     0,     0,     0,
       0,   432,   431,   363,   368,     0,   315,     0,   207,     0,
       0,   316,   320,     0,     0,   395,     0,     0,     0,   375,
       0,   374,     0,     0,     0,     0,   209,     0,   402,   377,
     380,   215,     0,   214,   185,   351,     0,   165,   386,   389,
     388,   484,   485,   486,   488,     0,     0,   136,   135,     0,
     134,     0,     0,     0,   501,   503,     0,   508,     0,   504,
       0,   453,   506,   505,     0,   304,   297,   291,   290,   292,
     302,   303,   305,   293,   294,   296,   278,   295,   279,   282,
     286,   276,     0,   428,   243,   244,     0,   249,   247,   248,
     261,   262,   250,   251,   253,     0,     0,   263,     0,   265,
       0,   257,   258,   260,   241,   245,   429,   242,     0,   226,
     227,     0,   228,     0,   225,   221,     0,   309,     0,     0,
     409,     0,     0,     0,   433,     0,     0,     0,     0,     0,
       0,   218,   219,     0,     0,     0,   328,     0,     0,     0,
       0,     0,     0,     0,   333,   334,     0,     0,     0,   327,
       0,     0,     0,     0,     0,     0,   409,     0,     0,     0,
       0,     0,     0,   165,   154,   139,   143,     0,   348,     0,
       0,     0,   455,     0,     0,   246,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    24,     0,   406,   407,   408,
     403,   404,     0,   412,     0,     0,   355,   365,     0,   382,
     202,     0,   402,   396,   194,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   371,   372,
     373,   204,     0,   412,   379,   220,   349,     0,   169,   164,
     166,     0,     0,   509,     0,   510,   479,   430,   252,   254,
     255,   264,   256,   267,   266,   259,   223,   224,   222,     0,
       0,     0,     0,   417,     0,     0,     0,     0,     0,   409,
       0,     0,   414,   338,   340,   341,   342,   344,   343,   337,
     329,   330,   331,   332,   335,   336,   339,     0,     0,   417,
     210,   167,    31,   187,     0,     0,     0,     0,     0,     0,
     450,   405,   411,   410,     0,     0,   419,     0,   538,     0,
     364,   206,   412,     0,   323,   369,   208,   419,    31,   170,
       0,     0,   511,    20,     0,   268,   413,     0,     0,   415,
     537,     0,   417,     0,     0,   415,   168,     0,     0,     0,
     418,     0,     0,   359,   366,   419,   324,   370,   196,     0,
       0,    21,   420,     0,   361,   415,     0,     0,    25,    26,
      23,     0,     0,   416,   362,   360,   198,   197,     0,    13,
      22,   512,     0,   188,   199
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,   102,   103,   104,   105,   106,
    1138,  1202,   967,  1200,    12,   176,   107,   424,   108,   109,
     110,   111,   233,   112,   113,   800,   594,   803,   530,   531,
     532,   533,   114,   516,   895,   115,   116,   485,   117,   118,
     234,   789,  1012,  1013,  1158,  1132,   119,   653,   120,   121,
     251,   122,   650,   123,   422,   124,   378,   505,   645,   125,
     126,   127,   355,   347,   128,  1196,   129,  1212,   440,   618,
     639,   860,   878,   130,   377,   882,   567,   983,   748,   965,
     131,   274,   561,   132,   461,   311,   747,   954,  1095,   133,
     458,   301,   457,   741,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   769,   144,   145,   146,   147,   775,
     999,  1119,   148,   149,   552,   150,   151,   152,   153,   154,
     155,   156,   157,   158,  1194,  1205,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   316,   348,   970,  1040,  1041,  1043,  1143,  1103,  1113,
    1183,  1146,  1169,   319,   545,   608,   955,   853,   609,   257,
     352,   291,   276,   434,   328,   329,   576,   577,   312,   313,
     302,   303,   401,   353,   893,   662,   663,   664,   403,   404,
     405,   292,   452,   246,   265,   367,   744,   419,   420,   421,
     630,   333,   334
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1089
static const yytype_int16 yypact[] =
{
     120,  -109,   -81,   157, -1089,   120,    25,   -62,   -48, -1089,
   -1089,   -33,   887,   -23, -1089,    77, -1089,   -10,    61,    82,
      84, -1089,    98,   109,   114,   117,   119,   150,   168,   181,
     183,   187,   200,   201,   202,   205,   211,   232,   240,   242,
     243,   248,   256,   264,   265,   273,   275,   278,   279,   280,
     286,   288,   292,   295,   312,   314,   315,   322,   337,   338,
     340,   342,   343,   344,   345,   352,    37,   353,   354,   355,
   -1089,   358,    24,   751, -1089, -1089,   364,   369,   371,   373,
       6,    99,    36, -1089, -1089, -1089, -1089, -1089, -1089, -1089,
   -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089,
   -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089,   887, -1089,
   -1089,    -2, -1089, -1089, -1089, -1089, -1089,   374, -1089, -1089,
   -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089,
   -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089,
   -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089,
   -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089,
   -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089,
   -1089, -1089, -1089, -1089,   384,   245, -1089,   -94,   443,    68,
       3,   400,   450,   111,   111,   160,   -46,    11,   -20,   476,
     -20,   846,  -126,  -126,   -24,    81,   -20,   -20,   441,   -20,
     -15,  -126,  -126,   -44,   -24,   -24,   -24,    99,   378,    99,
     -20,   450,   450,   -20,   450,   450,    72,   -20,   450,   -20,
     -44,   450,  -126,   450,    78, -1089,   450,  -126,   449,   635,
      99, -1089, -1089,   332,   377,   -20,   -20,   -20,   -20,   396,
   -1089,    42, -1089,   391, -1089,   306, -1089,    15, -1089,   147,
   -1089,   395,   402,    77, -1089, -1089,   399, -1089,  -136,   404,
     405,   407, -1089, -1089, -1089, -1089, -1089,   414, -1089, -1089,
     420,   648, -1089,   423,   431,   433, -1089, -1089,  -126,  -126,
     -20,   -20,   447,   -20,   448,   451,   452,   450,   453,   334,
   -1089, -1089,   389,   456, -1089, -1089, -1089,   684, -1089, -1089,
     458, -1089, -1089, -1089, -1089, -1089, -1089,   686, -1089, -1089,
     459,   462,   465, -1089, -1089, -1089,   467, -1089, -1089,   470,
     472,   481,   484, -1089, -1089,   710, -1089,   483,   488, -1089,
     489,   490,   715,   493, -1089,   494,   495,   496,   497,   498,
     724,   506,   508, -1089, -1089, -1089, -1089,   512,   736,   525,
     526,   528,   534,   161,   761,   537,   141,   538,   542, -1089,
     544, -1089, -1089, -1089, -1089, -1089, -1089, -1089,   545,   549,
     553,   565,   566,   568,   569, -1089, -1089,   570,   395,   571,
     578,   579, -1089,   575,    99,    99,   585,   586,   588,   599,
   -1089,   587,   294,    99,    99, -1089,    99,    99,    99,    99,
      99,   540,   161, -1089,   601,   600, -1089, -1089, -1089, -1089,
   -1089, -1089,   603,    59,    29, -1089, -1089,   540,   161,   604,
     605,   612,   887,   887, -1089,    28, -1089,    99,   828,   -94,
     852,    13,   853,   628,   625,   450,   642,    99,   186,   867,
     636, -1089, -1089,   644,   646,   424, -1089,   -20,   -20,   244,
   -1089,   657,   655,   450,   786,   662,    99,   666,   395,   668,
      99,   395,   -20,   -20,   450,   812,    12,   813,   675,    99,
      28,   860,   902,   678,   863,   868,   348,   722,   -20,   827,
     696,   829,   -24,   -43, -1089,   707,   -24,   -24,   -24,    99,
     708,    56,   -20,    90,   849,     7,   754,   854,   850,   -39,
      51,   817,   822,    11,   820, -1089,    95,    95,   811, -1089,
     277,   735,   -47,   818,   839,   -29,   727, -1089, -1089,   466,
     475,    89,    89, -1089, -1089, -1089,    15, -1089,   450,   742,
    -117,  -103,   -92,   -55, -1089, -1089,   540,   161,    94,   131,
     177, -1089,   758, -1089, -1089, -1089,   554,   766, -1089, -1089,
   -1089,   995, -1089,   773,   414, -1089,   772,  1002,   562, -1089,
   -1089,   433, -1089, -1089,   -20,   -20,   718,   781,   780,   785,
     787, -1089,   788,   268, -1089,   782,   789, -1089,   791,   792,
   -1089, -1089, -1089,   793,   567,   650, -1089,   796,   577, -1089,
   -1089, -1089, -1089,   794,   797, -1089,   800,  1031,   582,   808,
   -1089,   809,  1036, -1089,   814, -1089,   815, -1089,   824, -1089,
     819,   826, -1089,  1037, -1089,   835, -1089,  1040,   845,    28,
     847,   851, -1089,   855,   960, -1089, -1089, -1089, -1089, -1089,
     856, -1089,   857,   848, -1089,   858,   859, -1089,  1053,   861,
   -1089,   866, -1089,   871,   872,   912,  1072,   873,   874, -1089,
     395, -1089,   790,    99, -1089, -1089,   540,   875,   876, -1089,
   -1089,   877, -1089,   880,   865, -1089, -1089, -1089, -1089,  1077,
     879, -1089,   -36, -1089,    99, -1089,   -94, -1089,    22, -1089,
      12, -1089,    27, -1089, -1089, -1089,  1066,   882,  1080, -1089,
   -1089,   881, -1089,   878, -1089,   883,  1019,   450, -1089,    99,
      99,   450,   885,    99,   450,   450,   884,   889, -1089, -1089,
   -1089, -1089,   892, -1089, -1089, -1089, -1089,   893, -1089,   894,
     898,   899, -1089,   900,   901,   921,   922,   923,   924,   925,
     927,   928, -1089,   945, -1089,   946, -1089, -1089, -1089,   947,
   -1089, -1089, -1089, -1089, -1089,   948, -1089,   888,   903,    11,
      12,  1084,   949, -1089,   -44,  1175,   952,   988,  1179,    88,
     160,   990,    -7,  1088,   957,   -27, -1089,   950,  1091, -1089,
      99,   961,   -94,  1116,   -20,   962,  1089,   116,   963,   -13,
    1090,  1191,   247,   395,  1092,    95, -1089, -1089,   161,   964,
      28,   -34,   121,   727, -1089,    66, -1089, -1089,   161,   540,
     -42, -1089,   -19,    -8, -1089,   968,    28,   969, -1089, -1089,
      99, -1089,   970,   421,   250,   971,    28,   593,   972,   973,
      99, -1089,    99,   111,  1083,  1085,    99,  1086,  1082,  1079,
      99,    99,    99,   -94,  1047,    99,    99,    99,   160, -1089,
     982,   393, -1089, -1089,   981,  1207,   983,   984,  1210,   985,
     986, -1089, -1089,   987, -1089,   991, -1089,  1213, -1089,   311,
     989, -1089, -1089,   993,    74,   540,   994,   996,   590, -1089,
     997, -1089,   998,   999,  1000,  1220, -1089,  1228,   983, -1089,
    1004, -1089,  1005, -1089, -1089, -1089,  1006,   -57, -1089, -1089,
     540, -1089, -1089, -1089, -1089,   395,    22, -1089, -1089,    27,
   -1089,  1219,  1007,  1232,   161, -1089,  1190, -1089,    99, -1089,
    1008, -1089, -1089, -1089,   331, -1089, -1089, -1089, -1089, -1089,
   -1089, -1089, -1089, -1089, -1089, -1089,   540, -1089, -1089, -1089,
   -1089, -1089,    70, -1089, -1089, -1089,   -94, -1089, -1089, -1089,
   -1089, -1089, -1089,  1011, -1089,  1012,  1013, -1089,  1014, -1089,
    1015, -1089, -1089,  1016, -1089, -1089, -1089,   540,  1017, -1089,
   -1089,  1018, -1089,  1020, -1089, -1089,  1246, -1089,  1022,   191,
    1144,  1249,  1025,   160, -1089,   -25,   160,  1023,  1027,   -13,
      99, -1089, -1089,  1026,  1183,  1160, -1089,  1030,  1032,  1033,
    1034,  1035,  1038,  1039, -1089, -1089,  1041,  1042,  1043, -1089,
    1150,  1155,  1156,  1159,  1044,  1045,  1144,  1271,    74,  1270,
     401,  1048,  1046,   -57, -1089, -1089, -1089,  1050, -1089,  1051,
    1049,   461, -1089,    99,  1195,   540,    99,    99,    99,    99,
      99,  -107,   111,  1280,  1134, -1089,  1282, -1089, -1089, -1089,
   -1089,  1058,  1059,  1185,  1062,  1284, -1089,  1063,  1064, -1089,
   -1089,   186,   983, -1089, -1089,  1067,  1068,  1189,  1291,    47,
     160,   111,    13,    13,   -20,   -46,  1294,  1070, -1089, -1089,
   -1089, -1089,  1296,  1185, -1089, -1089, -1089,  1071, -1089, -1089,
   -1089,   105,  1297, -1089,  1255, -1089,   129, -1089, -1089, -1089,
   -1089, -1089, -1089, -1089,  1021, -1089, -1089, -1089, -1089,  1073,
     191,  1197,  1076,  1279,  1304,  1078,  1115,   160,  1081,  1144,
    1215,  1214, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089,
   -1089, -1089, -1089, -1089, -1089, -1089, -1089,  1203,  1087,  1279,
     903, -1089,   887, -1089,  1093,  1094,  1095,  1097,  1096,  -107,
   -1089, -1089, -1089, -1089,  1189,  1099,  1287,  1098, -1089,  1101,
   -1089, -1089,  1185,  1103, -1089,  1104, -1089,  1287,   887, -1089,
    1298,    28, -1089, -1089,  1105, -1089, -1089,  1217,  1106,  1124,
   -1089,   160,  1279,  1223,    28,  1124, -1089,  1108,  1109,   -20,
   -1089,  1321,  1110, -1089, -1089,  1287, -1089, -1089, -1089,  1322,
      79,  1112, -1089,   160,   395,  1124,   395,  1113, -1089, -1089,
     981,   -20,  1107, -1089, -1089, -1089, -1089, -1089,  1331, -1089,
   -1089, -1089,   395, -1089, -1089
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1089, -1089,  1338, -1089, -1089, -1089, -1089, -1089, -1089, -1089,
   -1089, -1089,   144, -1089, -1089,  1100,  -108,  -376,   926,  1117,
    1272,  -492, -1089, -1089, -1089, -1089,   667, -1089, -1089, -1089,
   -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089,  1283, -1089,
   -1089, -1089,   339, -1089, -1089, -1089, -1089, -1089, -1089, -1089,
   -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089,
   -1089, -1089,   864,  1130, -1089, -1089, -1089, -1089,   795, -1089,
   -1089, -1089,   372, -1089, -1089, -1089,  -630,   346,   282, -1089,
   -1089,   499,   308, -1089, -1089, -1089, -1089, -1089,   221, -1089,
   -1089,  1177, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089,
   -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089,
   -1089, -1089, -1089, -1089,  -642, -1089, -1089, -1089, -1089, -1089,
   -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089,
   -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089, -1089,
   -1089,   297,   609,  -843,   266, -1089,  -944, -1089,  -984,   220,
   -1088, -1041, -1063,  -467,  -463, -1089, -1089, -1089,   527,   936,
    -224,  -179,  -391,   816,   249,  -385,  -493,  -645,  -536,  -454,
    -530,  -646,  -172,   -77, -1089,   576, -1089, -1089,   842, -1089,
   -1089,  1102,   -76,   798, -1089,  -481, -1089, -1089, -1089, -1089,
   -1089,  -178, -1089
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -238
static const yytype_int16 yytable[] =
{
     248,   275,   504,   681,   245,   258,   383,   599,   679,   320,
     626,   322,   595,   647,   648,   305,   550,   336,   337,   240,
     339,   341,   342,   534,   661,   294,   349,   350,   351,   535,
     571,   357,   801,   240,   360,  1006,   644,   804,   368,   331,
     370,   655,   314,    16,   345,   616,   240,   683,   331,   637,
     391,  1115,   266,   267,   889,   640,   386,   387,   388,   389,
     363,   858,  1073,   529,   364,   365,   811,   391,   266,   267,
     415,  1047,   263,   266,   267,   876,   363,   417,   981,   632,
     364,   365,   586,   375,  1198,   589,  1093,  1188,  1157,  1129,
     317,   266,   267,   428,  1175,   268,   269,   294,   571,   318,
     306,   443,   444,   240,   446,   325,   293,  1206,   543,  1133,
     295,  1010,   670,  1011,   294,   324,   544,   266,   267,   671,
     870,     7,  1195,  1134,   296,   659,   672,   660,   326,   327,
    1094,  1185,   356,   673,   305,   358,   359,   674,   361,   362,
       1,     2,   369,   254,   675,   372,  1116,   374,  1117,     8,
     379,   240,   883,   429,   255,   256,   766,     9,   641,    11,
     366,   511,   982,   305,   392,  1152,   376,  1199,  1172,  1048,
     402,   264,   418,   628,   676,    13,   851,   406,   629,   407,
     571,   677,   295,   852,   408,   409,   410,   896,   617,    14,
     254,   315,   638,   346,   897,   271,   296,    15,  1118,   295,
     270,   255,   256,   254,   859,   270,   174,   332,   633,  1109,
     750,   271,   175,   296,   255,   256,   340,   898,   877,   306,
     177,   899,   848,   270,   854,   808,   595,   871,   900,   872,
     873,   874,   272,   273,   271,   266,   267,   272,   273,  1037,
    1038,  1039,   536,   307,   551,   239,   241,   571,   306,   308,
    1015,   572,   254,   297,  1016,   272,   273,   230,   646,   298,
     413,   242,   243,   255,   256,   623,   254,   247,   569,   570,
      81,   702,   436,   241,   786,   242,   243,   255,   256,   559,
     560,   797,   842,   590,   591,   796,   249,   354,   242,   243,
     241,   178,   886,   916,   411,   845,   843,   244,   808,   611,
    1024,   661,   931,   758,   615,   242,   243,   510,   620,   621,
     622,   244,   179,   627,   180,   542,   519,   520,   335,   521,
     522,   523,   524,   525,   244,   297,   646,   888,   181,   678,
     241,   298,   266,   267,   566,   881,   392,   537,   436,   182,
     656,   244,   297,   902,   183,   242,   243,   184,   298,   185,
     546,   574,   575,   910,   299,   300,   266,   267,  1137,   556,
     558,   -19,   307,   891,   892,   412,   680,   270,   308,   568,
     492,   299,   300,   578,   398,   399,   400,   581,   413,   584,
     186,   244,   414,   588,   254,   694,   695,   252,   592,   309,
     310,   307,   598,   242,   243,   255,   256,   308,   187,   272,
     273,   326,   327,   268,   269,   363,   742,   884,   646,   364,
     365,   188,   682,   189,   398,   399,   400,   190,   309,   310,
    1122,  1123,   266,   267,   396,   397,   398,   399,   400,   244,
     191,   192,   193,   574,   575,   194,   605,  1046,   606,   607,
    1049,   195,   330,   651,   396,   397,   398,   399,   400,   402,
     343,   344,   668,   259,   260,   261,   262,   278,   279,   280,
     281,   282,   196,   283,   270,   284,   285,   286,   287,   288,
     197,   373,   198,   199,   253,   573,   380,   958,   200,   908,
     321,   278,   279,   280,   281,   282,   201,   283,   270,   284,
     285,   286,   287,   288,   202,   203,   272,   273,   416,   289,
     574,   575,  1096,   204,   799,   205,   290,   277,   206,   207,
     208,   566,   266,   267,   959,   960,   209,   394,   210,  1014,
     272,   273,   211,   961,  1120,   212,   518,   441,   442,  1077,
     290,  1121,   962,   396,   397,   398,   399,   400,   266,   267,
     395,   338,   213,   963,   214,   215,   863,   278,   279,   280,
     281,   282,   216,   283,   270,   284,   285,   286,   287,   288,
     396,   397,   398,   399,   400,   289,   652,   217,   218,   964,
     219,  1150,   220,   221,   222,   223,   788,   396,   397,   398,
     399,   400,   224,   226,   227,   228,   272,   273,   229,   396,
     397,   398,   399,   400,   235,   867,   290,   798,   915,   236,
     865,   237,   919,   238,   250,   381,   923,   924,   925,   354,
     385,   928,   929,   930,   396,  1023,   398,   399,   400,   890,
     384,   812,   813,   814,   393,   815,   817,   390,   818,   819,
     423,   425,   427,   430,   431,  1184,   432,   278,   279,   280,
     281,   282,   433,   283,   270,   284,   285,   286,   287,   288,
     906,   435,   436,   907,   709,   289,   437,  1203,   710,   711,
     438,   926,   439,   278,   279,   280,   281,   282,   957,   283,
     270,   284,   285,   286,   287,   288,   272,   273,   445,   447,
     453,   289,   448,   449,   451,   454,   290,   455,   986,   459,
    1084,   456,   460,  1085,   462,   987,   463,  -237,  1178,   464,
     665,   465,   272,   273,   396,   397,   398,   399,   400,   666,
     466,  1187,   290,   467,   468,   712,   469,   470,   471,   473,
     988,   472,   474,   475,   476,   477,   478,   479,   480,   713,
     714,   715,   716,   904,   717,   481,   718,   482,   266,   267,
     989,   483,   484,   914,   396,   397,   398,   399,   400,   396,
     397,   398,   399,   400,   486,   487,  1053,   488,   396,   397,
     398,   399,   400,   489,  1025,   490,   491,   493,   719,   720,
     721,   494,   722,   495,   496,   723,   724,   725,   497,   726,
     727,   728,   498,   729,   990,   991,   992,   993,   685,   994,
     995,   996,   997,   998,   499,   500,   692,   501,   502,   503,
     506,   708,  1088,  1089,  1090,  1091,  1092,   507,   508,   509,
     730,   746,   731,   732,   512,   513,   753,   514,  1204,   517,
    1207,   733,   734,   735,   736,   737,   738,   911,   515,   429,
     526,  1021,   527,   528,   538,   539,  1214,   396,   397,   398,
     399,   400,   540,   547,   739,   396,   397,   398,   399,   400,
     396,   397,   398,   399,   400,   549,   553,   554,   740,   555,
     396,   397,   398,   399,   400,   396,   397,   398,   399,   400,
     270,   557,   562,   564,   563,   565,   396,   397,   398,   399,
     400,   271,    85,   579,    87,  1124,    89,   580,    91,   582,
      93,   583,    95,    16,    97,   585,    99,   587,   101,   593,
     596,    17,   272,   273,   597,   600,   601,   602,   603,    18,
      19,    20,    21,    22,    23,   604,   610,    24,    25,    26,
      27,    28,    29,    30,   612,   613,   614,    31,    32,    33,
      34,    35,   244,    36,   266,   267,   619,   624,    37,    38,
      39,    40,    41,    42,    43,    44,  1086,    45,    46,    47,
     631,    48,   634,    49,    50,    51,    52,    53,    54,   642,
     636,   635,    55,    56,   643,    57,  -184,    83,   649,   654,
     657,   669,    58,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   231,   658,    59,   684,   686,    60,    84,   687,    86,
    1191,    88,   688,    90,   690,    92,   691,    94,   696,    96,
     697,    98,   698,   100,   699,   703,   700,   701,   704,    61,
     705,   706,  1210,   749,  1159,    62,   750,    63,    64,   751,
     707,    65,    66,   745,    67,   752,    68,   754,   755,   323,
     756,   762,    69,   757,   764,    70,    71,   771,   758,   760,
    1176,    72,    73,   759,    74,   761,    75,   778,   783,   278,
     279,   280,   281,   282,   763,   283,   270,   284,   285,   286,
     287,   288,   932,   933,   765,   702,   767,   289,   774,   787,
     768,   795,   805,   807,   770,   772,   773,   776,   777,   844,
     779,    76,    77,    78,    79,   780,    80,   794,   272,   273,
     781,   782,   784,   785,   790,   791,   566,   792,   290,   793,
     325,   806,   810,   808,   816,   820,   809,   840,   934,   935,
      81,   821,   822,   823,   824,   936,    82,    83,   825,   826,
     827,   828,   841,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   829,   830,   831,   832,   833,   937,   834,   835,   938,
     939,   940,   941,   942,   943,   944,   945,   946,   947,   948,
     949,   950,   951,   952,   953,   836,   837,   838,   845,   847,
     839,   848,   849,   850,   855,   856,   857,   861,   862,   866,
     864,   868,   875,   869,   879,   880,   885,   901,   903,   887,
     917,   918,   905,   909,   912,   913,   921,   922,   927,   920,
     966,   968,   969,   971,   972,   973,   975,   977,   979,   254,
     974,   976,   980,   984,  1004,   985,  1000,  1001,  1002,  1003,
     255,   256,  1005,  1007,  1008,  1009,  1017,  1019,  1020,  1018,
    1022,  1026,  1027,  1028,  1029,  1030,  1031,  1032,  1033,  1035,
    1034,  1036,  1042,  1044,  1045,  1050,  1051,  1055,  1054,  1056,
    1057,  1067,  1058,  1059,  1060,  1061,  1068,  1069,  1062,  1063,
    1070,  1064,  1065,  1066,  1072,  1074,  1071,  1076,  1078,  1081,
    1082,  1083,  1079,  1087,  1097,  1098,  1099,  1100,  1105,  1101,
    1102,  1104,  1106,  1112,  1107,  1114,  1110,  1111,  1126,  1127,
    1128,  1131,  1135,  1136,  1142,  1140,  1144,  1145,  1147,  1149,
    1148,  1139,  1154,  1151,  1153,  1155,  1168,  1177,  1182,  1156,
    1180,  1186,  1160,  1161,  1163,  1192,  1197,  1162,  1164,  1167,
    1170,  1171,  1173,  1174,  1179,  1213,  1181,  1189,  1190,  1211,
    1193,  1201,  1208,    10,  1209,   232,   382,   802,   541,   225,
     371,  1052,  1080,   426,  1075,   625,   693,  1130,   978,  1108,
    1165,   304,  1125,   846,  1166,   548,  1141,   956,   667,   894,
     689,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   743,     0,     0,     0,     0,     0,   450
};

static const yytype_int16 yycheck[] =
{
     108,   180,   378,   539,    81,   177,   230,   470,   538,   188,
     491,   190,   466,   506,   507,     3,     3,   196,   197,     4,
     199,   200,   200,   414,   516,     3,   204,   205,   206,   414,
       3,   210,   678,     4,   213,   878,   503,   682,   217,    63,
     219,    88,    88,     6,    88,    88,     4,   540,    63,    88,
       8,     4,    88,    89,    88,     4,   235,   236,   237,   238,
       4,    88,  1006,     4,     8,     9,   696,     8,    88,    89,
     249,    96,     4,    88,    89,    88,     4,   249,     4,    72,
       8,     9,   458,     5,     5,   461,   193,  1175,  1129,  1073,
      79,    88,    89,   229,  1157,    92,    93,     3,     3,    88,
      88,   280,   281,     4,   283,   231,   182,  1195,    80,     4,
      88,   168,   229,   170,     3,   191,    88,    88,    89,   236,
       4,   230,  1185,    18,   102,   154,   229,   156,   254,   255,
     237,  1172,   209,   236,     3,   211,   212,   229,   214,   215,
      20,    21,   218,   237,   236,   221,    99,   223,   101,   230,
     226,     4,   782,   289,   248,   249,   619,     0,   107,   134,
      88,   385,    88,     3,   241,  1109,    88,    88,  1152,   194,
     247,   103,   249,    83,   229,   237,    88,    30,    88,    32,
       3,   236,    88,    95,    37,    38,    39,   229,   231,   237,
     237,   237,   231,   237,   236,   231,   102,   230,   151,    88,
     220,   248,   249,   237,   231,   220,   229,   231,   201,  1052,
     229,   231,   135,   102,   248,   249,   231,   236,   231,    88,
     230,   229,   229,   220,   760,   232,   680,   111,   236,   113,
     114,   115,   252,   253,   231,    88,    89,   252,   253,    48,
      49,    50,   414,   231,   231,   239,   231,     3,    88,   237,
     896,     7,   237,   231,   899,   252,   253,   233,   231,   237,
     231,   246,   247,   248,   249,   489,   237,   231,   447,   448,
     233,     3,     4,   231,   650,   246,   247,   248,   249,    93,
      94,   672,   749,   462,   463,   670,   288,   231,   246,   247,
     231,   230,   785,   823,   147,   229,   750,   282,   232,   478,
     230,   793,   838,   233,   482,   246,   247,   384,   486,   487,
     488,   282,   230,   492,   230,   423,   393,   394,   237,   396,
     397,   398,   399,   400,   282,   231,   231,   790,   230,   235,
     231,   237,    88,    89,    87,    88,   413,   414,     4,   230,
     512,   282,   231,   806,   230,   246,   247,   230,   237,   230,
     427,   256,   257,   816,   260,   261,    88,    89,   229,   435,
     437,   232,   231,   242,   243,   218,   235,   220,   237,   445,
     229,   260,   261,   449,   285,   286,   287,   453,   231,   456,
     230,   282,   235,   460,   237,   564,   565,     3,   464,   258,
     259,   231,   469,   246,   247,   248,   249,   237,   230,   252,
     253,   254,   255,    92,    93,     4,   585,   783,   231,     8,
       9,   230,   235,   230,   285,   286,   287,   230,   258,   259,
    1062,  1063,    88,    89,   283,   284,   285,   286,   287,   282,
     230,   230,   230,   256,   257,   230,    88,   973,    90,    91,
     976,   230,   193,   166,   283,   284,   285,   286,   287,   526,
     201,   202,   528,    10,    11,    12,    13,   213,   214,   215,
     216,   217,   230,   219,   220,   221,   222,   223,   224,   225,
     230,   222,   230,   230,   229,   231,   227,    84,   230,   229,
       4,   213,   214,   215,   216,   217,   230,   219,   220,   221,
     222,   223,   224,   225,   230,   230,   252,   253,   249,   231,
     256,   257,  1032,   230,   676,   230,   262,   107,   230,   230,
     230,    87,    88,    89,   121,   122,   230,   211,   230,   895,
     252,   253,   230,   130,  1060,   230,   232,   278,   279,  1010,
     262,  1061,   139,   283,   284,   285,   286,   287,    88,    89,
     234,   100,   230,   150,   230,   230,   770,   213,   214,   215,
     216,   217,   230,   219,   220,   221,   222,   223,   224,   225,
     283,   284,   285,   286,   287,   231,   289,   230,   230,   176,
     230,  1107,   230,   230,   230,   230,   653,   283,   284,   285,
     286,   287,   230,   230,   230,   230,   252,   253,   230,   283,
     284,   285,   286,   287,   230,   774,   262,   674,   822,   230,
     772,   230,   826,   230,   230,   156,   830,   831,   832,   231,
     233,   835,   836,   837,   283,   284,   285,   286,   287,   791,
     288,   697,   699,   700,   233,   701,   703,   231,   704,   705,
     235,   229,   233,   229,   229,  1171,   229,   213,   214,   215,
     216,   217,   228,   219,   220,   221,   222,   223,   224,   225,
     229,   231,     4,   232,     4,   231,   233,  1193,     8,     9,
     229,   833,   229,   213,   214,   215,   216,   217,   840,   219,
     220,   221,   222,   223,   224,   225,   252,   253,   231,   231,
     291,   231,   231,   231,   231,   229,   262,     3,    98,     3,
     229,   233,   233,   232,   229,   105,   229,   235,  1161,   229,
     234,   229,   252,   253,   283,   284,   285,   286,   287,   234,
     229,  1174,   262,   229,     4,    65,   233,   229,   229,     4,
     130,   231,   229,   229,   229,   229,   229,   229,     4,    79,
      80,    81,    82,   810,    84,   229,    86,   229,    88,    89,
     150,   229,     6,   820,   283,   284,   285,   286,   287,   283,
     284,   285,   286,   287,   229,   229,   980,   229,   283,   284,
     285,   286,   287,   229,   936,     4,   229,   229,   118,   119,
     120,   229,   122,   229,   229,   125,   126,   127,   229,   129,
     130,   131,   229,   133,   194,   195,   196,   197,   234,   199,
     200,   201,   202,   203,   229,   229,   234,   229,   229,   229,
     229,   234,  1026,  1027,  1028,  1029,  1030,   229,   229,   234,
     160,   234,   162,   163,   229,   229,   234,   229,  1194,   232,
    1196,   171,   172,   173,   174,   175,   176,   234,   229,   289,
     229,   908,   232,   230,   230,   230,  1212,   283,   284,   285,
     286,   287,   230,    15,   194,   283,   284,   285,   286,   287,
     283,   284,   285,   286,   287,     3,     3,   229,   208,   234,
     283,   284,   285,   286,   287,   283,   284,   285,   286,   287,
     220,   229,     5,   229,   238,   229,   283,   284,   285,   286,
     287,   231,   247,   226,   249,  1064,   251,   232,   253,   103,
     255,   229,   257,     6,   259,   229,   261,   229,   263,    87,
      87,    14,   252,   253,   229,    45,     4,   229,    45,    22,
      23,    24,    25,    26,    27,    47,   194,    30,    31,    32,
      33,    34,    35,    36,    97,   229,    97,    40,    41,    42,
      43,    44,   282,    46,    88,    89,   229,   229,    51,    52,
      53,    54,    55,    56,    57,    58,  1023,    60,    61,    62,
     101,    64,   198,    66,    67,    68,    69,    70,    71,   142,
     110,   107,    75,    76,   142,    78,   146,   240,   157,   234,
     152,   229,    85,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   240,   153,   106,   236,   229,   109,   246,     3,   248,
    1179,   250,   229,   252,   232,   254,     4,   256,   290,   258,
     229,   260,   232,   262,   229,   233,   229,   229,   229,   132,
     229,   229,  1201,   229,  1132,   138,   229,   140,   141,   229,
     237,   144,   145,   237,   147,     4,   149,   229,   229,   193,
       4,     4,   155,   229,     4,   158,   159,    87,   233,   230,
    1158,   164,   165,   229,   167,   229,   169,     4,   146,   213,
     214,   215,   216,   217,   229,   219,   220,   221,   222,   223,
     224,   225,    90,    91,   229,     3,   229,   231,   230,   289,
     229,     4,    16,     3,   229,   229,   229,   229,   229,     5,
     229,   204,   205,   206,   207,   229,   209,   232,   252,   253,
     229,   229,   229,   229,   229,   229,    87,   230,   262,   229,
     231,   229,   229,   232,   229,   231,   238,   229,   136,   137,
     233,   232,   230,   230,   230,   143,   239,   240,   230,   230,
     230,   230,   229,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   230,   230,   230,   230,   230,   174,   230,   230,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   230,   230,   230,   229,     4,
     232,   229,   194,     4,   194,    97,   229,   237,    97,    73,
     229,   229,   229,   104,   104,     4,   104,   229,   229,   235,
     117,   116,   232,   232,   232,   232,   124,   128,   161,   123,
     229,     4,   229,   229,     4,   230,   229,     4,   229,   237,
     234,   230,   229,   229,     4,   229,   229,   229,   229,   229,
     248,   249,     4,   229,   229,   229,    17,     5,    48,   232,
     232,   230,   230,   230,   230,   230,   230,   230,   230,     3,
     230,   229,   108,     4,   229,   232,   229,    74,   232,    99,
     230,   111,   230,   230,   230,   230,   111,   111,   230,   230,
     111,   230,   230,   230,   229,     4,   232,     7,   230,   229,
     229,   232,   236,    88,     4,   151,     4,   229,     4,   230,
     105,   229,   229,   104,   230,     4,   229,   229,     4,   229,
       4,   230,     5,    48,   107,   232,   230,    28,     4,   194,
     232,   290,    98,   232,    99,   112,    29,    19,   194,   232,
     103,    98,   229,   229,   227,     4,     4,   232,   232,   230,
     232,   230,   229,   229,   229,     4,   230,   229,   229,   232,
     230,   229,   229,     5,  1200,    73,   229,   680,   422,    66,
     220,   979,  1013,   253,  1008,   491,   561,  1075,   859,  1051,
    1139,   184,  1065,   754,  1144,   429,  1100,   840,   526,   793,
     554,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   585,    -1,    -1,    -1,    -1,    -1,   287
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    20,    21,   293,   294,   295,   296,   230,   230,     0,
     294,   134,   306,   237,   237,   230,     6,    14,    22,    23,
      24,    25,    26,    27,    30,    31,    32,    33,    34,    35,
      36,    40,    41,    42,    43,    44,    46,    51,    52,    53,
      54,    55,    56,    57,    58,    60,    61,    62,    64,    66,
      67,    68,    69,    70,    71,    75,    76,    78,    85,   106,
     109,   132,   138,   140,   141,   144,   145,   147,   149,   155,
     158,   159,   164,   165,   167,   169,   204,   205,   206,   207,
     209,   233,   239,   240,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   297,   298,   299,   300,   301,   308,   310,   311,
     312,   313,   315,   316,   324,   327,   328,   330,   331,   338,
     340,   341,   343,   345,   347,   351,   352,   353,   356,   358,
     365,   372,   375,   381,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   397,   398,   399,   400,   404,   405,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   418,
     419,   420,   421,   422,   423,   424,   425,   426,   427,   428,
     429,   430,   431,   432,   229,   135,   307,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   330,   230,   230,   230,   230,
     233,   240,   312,   314,   332,   230,   230,   230,   230,   239,
       4,   231,   246,   247,   282,   465,   475,   231,   308,   288,
     230,   342,     3,   229,   237,   248,   249,   451,   464,    10,
      11,    12,    13,     4,   103,   476,    88,    89,    92,    93,
     220,   231,   252,   253,   373,   453,   454,   107,   213,   214,
     215,   216,   217,   219,   221,   222,   223,   224,   225,   231,
     262,   453,   473,   474,     3,    88,   102,   231,   237,   260,
     261,   383,   462,   463,   383,     3,    88,   231,   237,   258,
     259,   377,   460,   461,    88,   237,   433,    79,    88,   445,
     453,     4,   453,   193,   474,   231,   254,   255,   456,   457,
     456,    63,   231,   483,   484,   237,   453,   453,   100,   453,
     231,   453,   483,   456,   456,    88,   237,   355,   434,   483,
     483,   483,   452,   465,   231,   354,   465,   453,   474,   474,
     453,   474,   474,     4,     8,     9,    88,   477,   453,   474,
     453,   355,   474,   456,   474,     5,    88,   366,   348,   474,
     456,   156,   311,   452,   288,   233,   453,   453,   453,   453,
     231,     8,   465,   233,   211,   234,   283,   284,   285,   286,
     287,   464,   465,   470,   471,   472,    30,    32,    37,    38,
      39,   147,   218,   231,   235,   453,   456,   464,   465,   479,
     480,   481,   346,   235,   309,   229,   307,   233,   229,   289,
     229,   229,   229,   228,   455,   231,     4,   233,   229,   229,
     360,   456,   456,   453,   453,   231,   453,   231,   231,   231,
     473,   231,   474,   291,   229,     3,   233,   384,   382,     3,
     233,   376,   229,   229,   229,   229,   229,   229,     4,   233,
     229,   229,   231,     4,   229,   229,   229,   229,   229,   229,
       4,   229,   229,   229,     6,   329,   229,   229,   229,   229,
       4,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   309,   349,   229,   229,   229,   234,
     465,   452,   229,   229,   229,   229,   325,   232,   232,   465,
     465,   465,   465,   465,   465,   465,   229,   232,   230,     4,
     320,   321,   322,   323,   454,   457,   464,   465,   230,   230,
     230,   310,   308,    80,    88,   446,   465,    15,   451,     3,
       3,   231,   406,     3,   229,   234,   474,   229,   465,    93,
      94,   374,     5,   238,   229,   229,    87,   368,   474,   453,
     453,     3,     7,   231,   256,   257,   458,   459,   474,   226,
     232,   474,   103,   229,   465,   229,   309,   229,   465,   309,
     453,   453,   474,    87,   318,   461,    87,   229,   465,   446,
      45,     4,   229,    45,    47,    88,    90,    91,   447,   450,
     194,   453,    97,   229,    97,   483,    88,   231,   361,   229,
     483,   483,   483,   452,   229,   354,   477,   453,    83,    88,
     482,   101,    72,   201,   198,   107,   110,    88,   231,   362,
       4,   107,   142,   142,   445,   350,   231,   458,   458,   157,
     344,   166,   289,   339,   234,    88,   464,   152,   153,   154,
     156,   313,   467,   468,   469,   234,   234,   470,   474,   229,
     229,   236,   229,   236,   229,   236,   229,   236,   235,   462,
     235,   460,   235,   458,   236,   234,   229,     3,   229,   455,
     232,     4,   234,   360,   453,   453,   290,   229,   232,   229,
     229,   229,     3,   233,   229,   229,   229,   237,   234,     4,
       8,     9,    65,    79,    80,    81,    82,    84,    86,   118,
     119,   120,   122,   125,   126,   127,   129,   130,   131,   133,
     160,   162,   163,   171,   172,   173,   174,   175,   176,   194,
     208,   385,   453,   475,   478,   237,   234,   378,   370,   229,
     229,   229,     4,   234,   229,   229,     4,   229,   233,   229,
     230,   229,     4,   229,     4,   229,   446,   229,   229,   396,
     229,    87,   229,   229,   230,   401,   229,   229,     4,   229,
     229,   229,   229,   146,   229,   229,   309,   289,   465,   333,
     229,   229,   230,   229,   232,     4,   457,   454,   465,   464,
     317,   463,   318,   319,   459,    16,   229,     3,   232,   238,
     229,   368,   474,   465,   465,   474,   229,   465,   474,   474,
     231,   232,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   232,
     229,   229,   445,   461,     5,   229,   434,     4,   229,   194,
       4,    88,    95,   449,   460,   194,    97,   229,    88,   231,
     363,   237,    97,   452,   229,   464,    73,   453,   229,   104,
       4,   111,   113,   114,   115,   229,    88,   231,   364,   104,
       4,    88,   367,   368,   309,   104,   458,   235,   446,    88,
     464,   242,   243,   466,   467,   326,   229,   236,   236,   229,
     236,   229,   446,   229,   465,   232,   229,   232,   229,   232,
     446,   234,   232,   232,   465,   452,   462,   117,   116,   452,
     123,   124,   128,   452,   452,   452,   464,   161,   452,   452,
     452,   460,    90,    91,   136,   137,   143,   174,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   379,   448,   450,   464,    84,   121,
     122,   130,   139,   150,   176,   371,   229,   304,     4,   229,
     435,   229,     4,   230,   234,   229,   230,     4,   373,   229,
     229,     4,    88,   369,   229,   229,    98,   105,   130,   150,
     194,   195,   196,   197,   199,   200,   201,   202,   203,   402,
     229,   229,   229,   229,     4,     4,   435,   229,   229,   229,
     168,   170,   334,   335,   309,   463,   459,    17,   232,     5,
      48,   465,   232,   284,   230,   464,   230,   230,   230,   230,
     230,   230,   230,   230,   230,     3,   229,    48,    49,    50,
     436,   437,   108,   438,     4,   229,   460,    96,   194,   460,
     232,   229,   364,   452,   232,    74,    99,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   111,   111,   111,
     111,   232,   229,   438,     4,   369,     7,   477,   230,   236,
     334,   229,   229,   232,   229,   232,   465,    88,   452,   452,
     452,   452,   452,   193,   237,   380,   462,     4,   151,     4,
     229,   230,   105,   440,   229,     4,   229,   230,   374,   435,
     229,   229,   104,   441,     4,     4,    99,   101,   151,   403,
     460,   462,   406,   406,   453,   433,     4,   229,     4,   440,
     370,   230,   337,     4,    18,     5,    48,   229,   302,   290,
     232,   436,   107,   439,   230,    28,   443,     4,   232,   194,
     460,   232,   438,    99,    98,   112,   232,   443,   336,   308,
     229,   229,   232,   227,   232,   380,   441,   230,    29,   444,
     232,   230,   440,   229,   229,   444,   308,    19,   446,   229,
     103,   230,   194,   442,   460,   443,    98,   446,   442,   229,
     229,   453,     4,   230,   416,   444,   357,     4,     5,    88,
     305,   229,   303,   460,   309,   417,   442,   309,   229,   304,
     453,   232,   359,     4,   309
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
#line 7573 "lev_comp.tab.c"
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

