
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
     ELEMENTAL_ENCHANTMENT_TYPE = 364,
     EXCEPTIONALITY_TYPE = 365,
     EXCEPTIONALITY_ID = 366,
     ELEMENTAL_ENCHANTMENT_ID = 367,
     ENCHANTMENT_ID = 368,
     SECRET_DOOR_ID = 369,
     USES_UP_KEY_ID = 370,
     MYTHIC_PREFIX_TYPE = 371,
     MYTHIC_SUFFIX_TYPE = 372,
     MYTHIC_PREFIX_ID = 373,
     MYTHIC_SUFFIX_ID = 374,
     MATERIAL_ID = 375,
     MATERIAL_TYPE = 376,
     CHARGES_ID = 377,
     SPECIAL_QUALITY_ID = 378,
     SPEFLAGS_ID = 379,
     SUBROOM_ID = 380,
     NAME_ID = 381,
     FLAGS_ID = 382,
     FLAG_TYPE = 383,
     MON_ATTITUDE = 384,
     MON_ALERTNESS = 385,
     SUBTYPE_ID = 386,
     NON_PASSDOOR_ID = 387,
     CARPET_ID = 388,
     CARPET_PIECE_ID = 389,
     CARPET_TYPE = 390,
     MON_APPEARANCE = 391,
     ROOMDOOR_ID = 392,
     IF_ID = 393,
     ELSE_ID = 394,
     TERRAIN_ID = 395,
     HORIZ_OR_VERT = 396,
     REPLACE_TERRAIN_ID = 397,
     LOCATION_SUBTYPE_ID = 398,
     DOOR_SUBTYPE = 399,
     BRAZIER_SUBTYPE = 400,
     SIGNPOST_SUBTYPE = 401,
     TREE_SUBTYPE = 402,
     FOREST_ID = 403,
     FOREST_TYPE = 404,
     INITIALIZE_TYPE = 405,
     EXIT_ID = 406,
     SHUFFLE_ID = 407,
     MANUAL_TYPE_ID = 408,
     MANUAL_TYPE = 409,
     QUANTITY_ID = 410,
     BURIED_ID = 411,
     LOOP_ID = 412,
     FOR_ID = 413,
     TO_ID = 414,
     SWITCH_ID = 415,
     CASE_ID = 416,
     BREAK_ID = 417,
     DEFAULT_ID = 418,
     ERODED_ID = 419,
     TRAPPED_STATE = 420,
     RECHARGED_ID = 421,
     INVIS_ID = 422,
     GREASED_ID = 423,
     INDESTRUCTIBLE_ID = 424,
     FEMALE_ID = 425,
     MALE_ID = 426,
     WAITFORU_ID = 427,
     PROTECTOR_ID = 428,
     CANCELLED_ID = 429,
     REVIVED_ID = 430,
     AVENGE_ID = 431,
     FLEEING_ID = 432,
     BLINDED_ID = 433,
     MAXHP_ID = 434,
     LEVEL_ADJUSTMENT_ID = 435,
     KEEP_ORIGINAL_INVENTORY_ID = 436,
     PARALYZED_ID = 437,
     STUNNED_ID = 438,
     CONFUSED_ID = 439,
     SEENTRAPS_ID = 440,
     ALL_ID = 441,
     MONTYPE_ID = 442,
     OBJTYPE_ID = 443,
     TERTYPE_ID = 444,
     TERTYPE2_ID = 445,
     LEVER_EFFECT_TYPE = 446,
     SWITCHABLE_ID = 447,
     CONTINUOUSLY_USABLE_ID = 448,
     TARGET_ID = 449,
     TRAPTYPE_ID = 450,
     EFFECT_FLAG_ID = 451,
     GRAVE_ID = 452,
     BRAZIER_ID = 453,
     SIGNPOST_ID = 454,
     TREE_ID = 455,
     ERODEPROOF_ID = 456,
     FUNCTION_ID = 457,
     MSG_OUTPUT_TYPE = 458,
     COMPARE_TYPE = 459,
     UNKNOWN_TYPE = 460,
     rect_ID = 461,
     fillrect_ID = 462,
     line_ID = 463,
     randline_ID = 464,
     grow_ID = 465,
     selection_ID = 466,
     flood_ID = 467,
     rndcoord_ID = 468,
     circle_ID = 469,
     ellipse_ID = 470,
     filter_ID = 471,
     complement_ID = 472,
     gradient_ID = 473,
     GRADIENT_TYPE = 474,
     LIMITED = 475,
     HUMIDITY_TYPE = 476,
     STRING = 477,
     MAP_ID = 478,
     NQSTRING = 479,
     VARSTRING = 480,
     CFUNC = 481,
     CFUNC_INT = 482,
     CFUNC_STR = 483,
     CFUNC_COORD = 484,
     CFUNC_REGION = 485,
     VARSTRING_INT = 486,
     VARSTRING_INT_ARRAY = 487,
     VARSTRING_STRING = 488,
     VARSTRING_STRING_ARRAY = 489,
     VARSTRING_VAR = 490,
     VARSTRING_VAR_ARRAY = 491,
     VARSTRING_COORD = 492,
     VARSTRING_COORD_ARRAY = 493,
     VARSTRING_REGION = 494,
     VARSTRING_REGION_ARRAY = 495,
     VARSTRING_MAPCHAR = 496,
     VARSTRING_MAPCHAR_ARRAY = 497,
     VARSTRING_MONST = 498,
     VARSTRING_MONST_ARRAY = 499,
     VARSTRING_OBJ = 500,
     VARSTRING_OBJ_ARRAY = 501,
     VARSTRING_SEL = 502,
     VARSTRING_SEL_ARRAY = 503,
     METHOD_INT = 504,
     METHOD_INT_ARRAY = 505,
     METHOD_STRING = 506,
     METHOD_STRING_ARRAY = 507,
     METHOD_VAR = 508,
     METHOD_VAR_ARRAY = 509,
     METHOD_COORD = 510,
     METHOD_COORD_ARRAY = 511,
     METHOD_REGION = 512,
     METHOD_REGION_ARRAY = 513,
     METHOD_MAPCHAR = 514,
     METHOD_MAPCHAR_ARRAY = 515,
     METHOD_MONST = 516,
     METHOD_MONST_ARRAY = 517,
     METHOD_OBJ = 518,
     METHOD_OBJ_ARRAY = 519,
     METHOD_SEL = 520,
     METHOD_SEL_ARRAY = 521,
     DICE = 522
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
#line 567 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 579 "lev_comp.tab.c"

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
#define YYLAST   1349

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  285
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  193
/* YYNRULES -- Number of rules.  */
#define YYNRULES  532
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1191

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   522

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   280,   284,     2,
     224,   225,   278,   276,   222,   277,   282,   279,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   223,     2,
       2,   281,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   226,     2,   227,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   228,   283,   229,     2,     2,     2,     2,
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
     215,   216,   217,   218,   219,   220,   221,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275
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
    1229,  1237,  1249,  1259,  1273,  1277,  1285,  1297,  1311,  1319,
    1327,  1333,  1341,  1347,  1357,  1365,  1371,  1381,  1387,  1393,
    1397,  1405,  1409,  1417,  1425,  1429,  1435,  1441,  1445,  1451,
    1459,  1469,  1471,  1473,  1475,  1477,  1479,  1480,  1483,  1485,
    1489,  1491,  1493,  1495,  1496,  1500,  1502,  1503,  1507,  1509,
    1510,  1514,  1515,  1519,  1520,  1524,  1526,  1528,  1530,  1532,
    1534,  1536,  1538,  1540,  1542,  1546,  1548,  1550,  1555,  1557,
    1559,  1564,  1566,  1568,  1573,  1575,  1580,  1586,  1588,  1592,
    1594,  1598,  1600,  1602,  1607,  1617,  1619,  1621,  1626,  1628,
    1634,  1636,  1638,  1643,  1645,  1647,  1653,  1655,  1657,  1659,
    1664,  1666,  1668,  1674,  1676,  1678,  1680,  1684,  1686,  1688,
    1692,  1694,  1699,  1703,  1707,  1711,  1715,  1719,  1723,  1725,
    1727,  1731,  1733,  1737,  1738,  1740,  1742,  1744,  1746,  1750,
    1751,  1753,  1755,  1758,  1761,  1766,  1773,  1778,  1785,  1792,
    1799,  1806,  1809,  1816,  1825,  1834,  1845,  1860,  1863,  1865,
    1869,  1871,  1875,  1877,  1879,  1881,  1883,  1885,  1887,  1889,
    1891,  1893,  1895,  1897,  1899,  1901,  1903,  1905,  1907,  1909,
    1911,  1913,  1924
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     286,     0,    -1,    -1,   287,    -1,   288,    -1,   288,   287,
      -1,   289,   299,   301,    -1,    17,   223,   230,    -1,    16,
     223,   230,   222,     3,    -1,    18,   223,    11,   222,   399,
      -1,    18,   223,    10,   222,     3,    -1,    18,   223,    13,
      -1,    18,   223,    12,   222,     3,   222,     3,   222,     5,
     222,     5,   222,   439,   222,   298,   297,    -1,    19,   223,
     469,    -1,   148,   223,   149,   222,   150,    -1,   148,   223,
     149,    -1,    32,   223,     4,   222,   311,    -1,    22,   223,
     103,    -1,    -1,   222,   220,    -1,    -1,   222,   446,    -1,
      -1,   222,     3,    -1,     5,    -1,    84,    -1,    -1,   127,
     223,   300,    -1,   128,   222,   300,    -1,   128,    -1,    -1,
     303,   301,    -1,   228,   301,   229,    -1,   344,    -1,   290,
      -1,   291,    -1,   407,    -1,   294,    -1,   292,    -1,   415,
      -1,   416,    -1,   417,    -1,   293,    -1,   414,    -1,   413,
      -1,   411,    -1,   412,    -1,   418,    -1,   419,    -1,   420,
      -1,   421,    -1,   422,    -1,   388,    -1,   346,    -1,   309,
      -1,   308,    -1,   402,    -1,   358,    -1,   380,    -1,   424,
      -1,   425,    -1,   390,    -1,   391,    -1,   392,    -1,   423,
      -1,   324,    -1,   334,    -1,   336,    -1,   340,    -1,   338,
      -1,   321,    -1,   331,    -1,   317,    -1,   320,    -1,   383,
      -1,   393,    -1,   365,    -1,   381,    -1,   368,    -1,   374,
      -1,   403,    -1,   398,    -1,   386,    -1,   345,    -1,   404,
      -1,   405,    -1,   406,    -1,   408,    -1,   351,    -1,   349,
      -1,   397,    -1,   401,    -1,   400,    -1,   384,    -1,   385,
      -1,   387,    -1,   379,    -1,   382,    -1,   240,    -1,   242,
      -1,   244,    -1,   246,    -1,   248,    -1,   250,    -1,   252,
      -1,   254,    -1,   256,    -1,   239,    -1,   241,    -1,   243,
      -1,   245,    -1,   247,    -1,   249,    -1,   251,    -1,   253,
      -1,   255,    -1,   304,    -1,   305,    -1,   233,    -1,   233,
      -1,   305,    -1,   152,   223,   304,    -1,   306,   281,   458,
      -1,   306,   281,   211,   223,   467,    -1,   306,   281,   457,
      -1,   306,   281,   474,   223,   451,    -1,   306,   281,   473,
     223,   453,    -1,   306,   281,   472,   223,   455,    -1,   306,
     281,   446,    -1,   306,   281,   449,    -1,   306,   281,   228,
     315,   229,    -1,   306,   281,   228,   314,   229,    -1,   306,
     281,   228,   313,   229,    -1,   306,   281,   474,   223,   228,
     312,   229,    -1,   306,   281,   473,   223,   228,   311,   229,
      -1,   306,   281,   472,   223,   228,   310,   229,    -1,   306,
     281,   228,   316,   229,    -1,   456,    -1,   310,   222,   456,
      -1,   454,    -1,   311,   222,   454,    -1,   452,    -1,   312,
     222,   452,    -1,   450,    -1,   313,   222,   450,    -1,   447,
      -1,   314,   222,   447,    -1,   458,    -1,   315,   222,   458,
      -1,   457,    -1,   316,   222,   457,    -1,    -1,    -1,   202,
     232,   224,   318,   462,   225,   319,   302,    -1,   232,   224,
     465,   225,    -1,   151,    -1,    -1,     6,    -1,     6,    -1,
     226,   458,   204,   458,   227,    -1,   226,   458,   227,    -1,
      -1,    -1,   160,   325,   226,   445,   227,   326,   228,   327,
     229,    -1,    -1,   328,   327,    -1,    -1,   161,   470,   223,
     329,   301,    -1,    -1,   163,   223,   330,   301,    -1,   162,
      -1,   282,   282,    -1,   159,    -1,   158,   307,   281,   458,
     332,   458,    -1,    -1,   333,   335,   302,    -1,    -1,   157,
     226,   445,   227,   337,   302,    -1,    -1,   323,   223,   339,
     303,    -1,    -1,   138,   323,   341,   342,    -1,   302,    -1,
      -1,   302,   343,   139,   302,    -1,    14,   223,   457,    -1,
      14,   223,   457,   222,    15,    -1,    74,    -1,    74,   223,
     470,    -1,    74,   223,    84,    -1,    62,   223,   347,   222,
     347,    -1,    62,   223,   347,   222,   470,    -1,   224,     4,
     222,    83,   222,   362,   225,    -1,   427,   322,   222,   439,
      -1,    -1,   125,   223,   348,   222,   355,   222,   357,   428,
     431,   433,   436,   437,   435,   350,   302,    -1,    -1,    54,
     223,   348,   222,   354,   222,   356,   222,   357,   428,   431,
     433,   436,   437,   435,   352,   302,    -1,    -1,   222,     5,
      -1,   224,     4,   222,     4,   225,    -1,    84,    -1,   224,
       4,   222,     4,   225,    -1,    84,    -1,   224,   366,   222,
     367,   225,    -1,    84,    -1,   224,     4,   222,     4,   225,
      -1,    84,    -1,   137,   223,   359,   222,   438,   222,   360,
     222,   362,   363,    -1,    30,   223,   438,   222,   467,   363,
      -1,     5,    -1,    84,    -1,   361,    -1,    84,    -1,    83,
      -1,    83,   283,   361,    -1,     4,    -1,    84,    -1,    -1,
     363,   222,   364,    -1,   143,   223,   144,    -1,    80,   223,
     455,    -1,   123,   223,     4,    -1,   169,    -1,   114,    -1,
     115,    -1,   132,    -1,    21,    -1,    20,   223,   366,   222,
     367,   353,   231,    -1,    20,   223,   446,   353,   231,    -1,
      88,    -1,    89,    -1,    90,    -1,    89,    -1,    28,   223,
     370,    -1,    -1,    28,   223,   370,   369,   302,    -1,   453,
     222,   446,   371,    -1,    -1,   371,   222,   372,    -1,   457,
      -1,   129,    -1,   130,    -1,   441,    -1,   136,   457,    -1,
     170,    -1,   171,    -1,   167,    -1,   174,    -1,   175,    -1,
     175,   223,   445,    -1,   176,    -1,   177,   223,   445,    -1,
     178,   223,   445,    -1,   182,   223,   445,    -1,   183,    -1,
     184,    -1,   185,   223,   373,    -1,   172,    -1,   173,    -1,
     179,    -1,   180,   223,   445,    -1,   181,    -1,   230,    -1,
     186,    -1,   230,   283,   373,    -1,    26,   223,   376,    -1,
      -1,    27,   223,   376,   375,   302,    -1,   455,   377,    -1,
      -1,   377,   222,   378,    -1,    77,    -1,   187,   223,   453,
      -1,   471,    -1,   126,   223,   457,    -1,   155,   223,   445,
      -1,   156,    -1,    76,    -1,   164,   223,   445,    -1,   201,
      -1,    75,    -1,   165,    -1,   166,   223,   445,    -1,   167,
      -1,   168,    -1,   446,    -1,   112,   223,   109,    -1,   111,
     223,   110,    -1,   113,   223,   445,    -1,   122,   223,   445,
      -1,   123,   223,   445,    -1,   153,   223,   154,    -1,   124,
     223,   445,    -1,    80,   223,   455,    -1,   169,    -1,   115,
      -1,    82,    -1,    78,    -1,   118,   223,   116,    -1,   119,
     223,   117,    -1,    61,   223,   445,    -1,   120,   223,   121,
      -1,    29,   223,   426,   222,   446,    -1,    31,   223,   446,
     222,    83,   222,   438,    -1,    36,   223,   446,   222,    83,
      -1,    36,   223,   446,   222,    83,   222,     5,   297,    -1,
      37,    -1,    37,   223,   467,    -1,    37,   223,   186,    -1,
      50,   223,   446,   222,    93,    -1,    51,   223,   446,   222,
      93,    -1,    51,   223,   476,   222,   476,   222,    93,    -1,
      56,   223,   476,   222,   476,   222,   230,    -1,    57,   223,
     476,   222,   476,   389,    -1,    58,   223,   476,   222,   476,
      -1,    -1,   222,    93,    -1,    65,   223,   467,   222,    97,
      -1,    66,   223,   467,    -1,    67,   223,   446,   222,   194,
     223,   446,   222,    95,   222,    94,    -1,    67,   223,   446,
     222,    68,   222,    69,   222,    70,   222,    95,   222,    94,
      -1,    81,   223,   446,   222,   191,   394,    -1,    -1,   394,
     222,   395,    -1,    94,    -1,   187,   223,   453,    -1,   188,
     223,   455,    -1,   189,   223,   399,    -1,   190,   223,   399,
      -1,   192,    -1,   193,    -1,   194,   223,   446,    -1,   195,
     223,   426,    -1,   143,   223,   396,    -1,   101,   223,   434,
      -1,   196,   223,     4,    -1,   123,   223,     4,    -1,     4,
      -1,    95,    -1,   144,    -1,    97,    -1,    72,   223,   467,
      -1,    71,   223,   467,    -1,     3,    -1,   224,     3,   222,
     439,   225,    -1,   142,   223,   449,   222,   451,   222,   451,
     222,     7,    -1,   140,   223,   467,   222,   451,    -1,   140,
     223,   467,   222,   451,   222,   100,    -1,    52,   223,   449,
      -1,    53,   223,   449,    -1,    42,   223,   230,   222,    43,
      -1,    42,   223,   230,   222,    43,   222,   187,   223,   453,
      -1,    39,   223,   449,   222,    41,    -1,    40,   223,   476,
     222,    41,    -1,    23,   223,   467,   222,    99,    -1,    -1,
      38,   223,   449,   222,   439,   222,   427,   428,   431,   433,
     436,   437,   435,   409,   410,    -1,    -1,   302,    -1,    47,
     223,   446,   222,   440,   222,   442,    -1,    47,   223,   446,
     222,   440,   222,   442,   222,   187,   223,   453,    -1,    47,
     223,   446,   222,   440,   222,   442,   222,    92,    -1,    47,
     223,   446,   222,   440,   222,   442,   222,    92,   222,   187,
     223,   453,    -1,    48,   223,   446,    -1,    48,   223,   446,
     222,   187,   223,   453,    -1,   105,   223,   446,   222,   106,
     222,     4,   222,   107,   222,   108,    -1,   105,   223,   446,
     222,   106,   222,     4,   222,   107,   222,   108,   222,   439,
      -1,   105,   223,   446,   222,   106,   222,   107,    -1,   102,
     223,   467,   222,   103,   222,   100,    -1,   131,   223,   467,
     222,     4,    -1,   131,   223,   467,   222,   103,   222,   100,
      -1,   133,   223,   449,   222,   135,    -1,   134,   223,   467,
     222,   135,   222,     4,   222,     4,    -1,   134,   223,   467,
     222,   135,   222,     4,    -1,    49,   223,    96,   222,   446,
      -1,    49,   223,    96,   222,   446,   222,   187,   223,   453,
      -1,   197,   223,   446,   222,   457,    -1,   197,   223,   446,
     222,    84,    -1,   197,   223,   446,    -1,   198,   223,   446,
     222,   145,   222,   439,    -1,   198,   223,   446,    -1,   199,
     223,   446,   222,   146,   222,   457,    -1,   199,   223,   446,
     222,   146,   222,    84,    -1,   199,   223,   446,    -1,   200,
     223,   446,   222,   147,    -1,   200,   223,   446,   222,   149,
      -1,   200,   223,   446,    -1,    63,   223,   458,   222,   446,
      -1,    64,   223,   446,   222,   475,   222,   457,    -1,    60,
     223,   445,   222,   445,   222,   445,   222,   445,    -1,    60,
      -1,   230,    -1,    84,    -1,   230,    -1,    84,    -1,    -1,
     222,   429,    -1,   430,    -1,   430,   222,   429,    -1,    44,
      -1,    45,    -1,    46,    -1,    -1,   104,   223,   432,    -1,
     103,    -1,    -1,   101,   223,   434,    -1,   100,    -1,    -1,
     187,   223,   453,    -1,    -1,    24,   223,    99,    -1,    -1,
      25,   223,     4,    -1,    75,    -1,    84,    -1,    76,    -1,
      84,    -1,    87,    -1,   443,    -1,    84,    -1,    87,    -1,
     443,    -1,    86,   223,    84,    -1,    91,    -1,    84,    -1,
      86,   226,     4,   227,    -1,   230,    -1,   241,    -1,   242,
     226,   458,   227,    -1,   458,    -1,   447,    -1,   213,   224,
     467,   225,    -1,   245,    -1,   246,   226,   458,   227,    -1,
     224,     4,   222,     4,   225,    -1,    84,    -1,    85,   448,
     227,    -1,   221,    -1,   221,   222,   448,    -1,   450,    -1,
     247,    -1,   248,   226,   458,   227,    -1,   224,     4,   222,
       4,   222,     4,   222,     4,   225,    -1,   452,    -1,   249,
      -1,   250,   226,   458,   227,    -1,     3,    -1,   224,     3,
     222,   439,   225,    -1,   454,    -1,   251,    -1,   252,   226,
     458,   227,    -1,   230,    -1,     3,    -1,   224,     3,   222,
     230,   225,    -1,    84,    -1,   456,    -1,   253,    -1,   254,
     226,   458,   227,    -1,   230,    -1,     3,    -1,   224,     3,
     222,   230,   225,    -1,    84,    -1,    98,    -1,   444,    -1,
     457,   282,   444,    -1,     4,    -1,   468,    -1,   224,     8,
     225,    -1,   239,    -1,   240,   226,   458,   227,    -1,   458,
     276,   458,    -1,   458,   277,   458,    -1,   458,   278,   458,
      -1,   458,   279,   458,    -1,   458,   280,   458,    -1,   224,
     458,   225,    -1,   235,    -1,   236,    -1,   306,   223,   459,
      -1,   460,    -1,   461,   222,   460,    -1,    -1,   461,    -1,
     458,    -1,   457,    -1,   463,    -1,   464,   222,   463,    -1,
      -1,   464,    -1,   446,    -1,   206,   449,    -1,   207,   449,
      -1,   208,   446,   222,   446,    -1,   209,   446,   222,   446,
     222,   458,    -1,   210,   224,   467,   225,    -1,   210,   224,
     361,   222,   467,   225,    -1,   216,   224,     7,   222,   467,
     225,    -1,   216,   224,   467,   222,   467,   225,    -1,   216,
     224,   451,   222,   467,   225,    -1,   212,   446,    -1,   214,
     224,   446,   222,   458,   225,    -1,   214,   224,   446,   222,
     458,   222,    44,   225,    -1,   215,   224,   446,   222,   458,
     222,   458,   225,    -1,   215,   224,   446,   222,   458,   222,
     458,   222,    44,   225,    -1,   218,   224,   219,   222,   224,
     458,   277,   458,   295,   225,   222,   446,   296,   225,    -1,
     217,   466,    -1,   255,    -1,   224,   467,   225,    -1,   466,
      -1,   466,   284,   467,    -1,   275,    -1,    99,    -1,     4,
      -1,     8,    -1,     9,    -1,     4,    -1,     8,    -1,     9,
      -1,     4,    -1,   468,    -1,    33,    -1,    26,    -1,    34,
      -1,    28,    -1,    35,    -1,   140,    -1,    79,    -1,    84,
      -1,   477,    -1,    59,   224,     4,   222,     4,   222,     4,
     222,     4,   225,    -1,   224,     4,   222,     4,   222,     4,
     222,     4,   225,    -1
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
    2342,  2347,  2352,  2357,  2363,  2367,  2373,  2377,  2381,  2387,
    2393,  2397,  2403,  2409,  2413,  2419,  2423,  2429,  2433,  2438,
    2445,  2449,  2456,  2460,  2465,  2472,  2476,  2481,  2489,  2495,
    2502,  2506,  2513,  2521,  2524,  2534,  2538,  2541,  2547,  2551,
    2558,  2562,  2566,  2573,  2576,  2582,  2589,  2592,  2598,  2605,
    2609,  2616,  2619,  2626,  2629,  2635,  2636,  2639,  2640,  2643,
    2644,  2645,  2651,  2652,  2653,  2659,  2660,  2663,  2672,  2677,
    2684,  2695,  2701,  2705,  2709,  2716,  2726,  2733,  2737,  2743,
    2747,  2755,  2759,  2766,  2776,  2789,  2793,  2800,  2810,  2819,
    2830,  2834,  2841,  2851,  2862,  2871,  2881,  2887,  2891,  2898,
    2908,  2919,  2928,  2938,  2942,  2949,  2950,  2956,  2960,  2964,
    2968,  2976,  2985,  2989,  2993,  2997,  3001,  3005,  3008,  3015,
    3024,  3057,  3058,  3061,  3062,  3065,  3069,  3076,  3083,  3094,
    3097,  3105,  3109,  3113,  3117,  3121,  3126,  3130,  3134,  3139,
    3144,  3149,  3153,  3158,  3163,  3167,  3171,  3176,  3180,  3187,
    3193,  3197,  3203,  3210,  3211,  3214,  3215,  3216,  3219,  3223,
    3227,  3231,  3237,  3238,  3241,  3242,  3245,  3246,  3249,  3250,
    3253,  3257,  3283
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
     475,   476,    44,    58,    40,    41,    91,    93,   123,   125,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,    43,    45,    42,    47,
      37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   285,   286,   286,   287,   287,   288,   289,   289,   290,
     290,   290,   290,   291,   292,   292,   293,   294,   295,   295,
     296,   296,   297,   297,   298,   298,   299,   299,   300,   300,
     301,   301,   302,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   306,   306,   306,   307,
     307,   308,   309,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   310,   310,   311,
     311,   312,   312,   313,   313,   314,   314,   315,   315,   316,
     316,   318,   319,   317,   320,   321,   322,   322,   323,   323,
     323,   325,   326,   324,   327,   327,   329,   328,   330,   328,
     331,   332,   332,   333,   335,   334,   337,   336,   339,   338,
     341,   340,   342,   343,   342,   344,   344,   345,   345,   345,
     346,   346,   347,   348,   350,   349,   352,   351,   353,   353,
     354,   354,   355,   355,   356,   356,   357,   357,   358,   358,
     359,   359,   360,   360,   361,   361,   362,   362,   363,   363,
     364,   364,   364,   364,   364,   364,   364,   365,   365,   365,
     366,   366,   367,   367,   368,   369,   368,   370,   371,   371,
     372,   372,   372,   372,   372,   372,   372,   372,   372,   372,
     372,   372,   372,   372,   372,   372,   372,   372,   372,   372,
     372,   372,   372,   373,   373,   373,   374,   375,   374,   376,
     377,   377,   378,   378,   378,   378,   378,   378,   378,   378,
     378,   378,   378,   378,   378,   378,   378,   378,   378,   378,
     378,   378,   378,   378,   378,   378,   378,   378,   378,   378,
     378,   378,   378,   379,   380,   381,   381,   382,   382,   382,
     383,   384,   385,   386,   387,   388,   389,   389,   390,   391,
     392,   392,   393,   394,   394,   395,   395,   395,   395,   395,
     395,   395,   395,   395,   395,   395,   395,   395,   396,   396,
     396,   396,   397,   398,   399,   399,   400,   401,   401,   402,
     403,   404,   404,   405,   406,   407,   409,   408,   410,   410,
     411,   411,   411,   411,   412,   412,   413,   413,   413,   414,
     415,   415,   416,   417,   417,   418,   418,   419,   419,   419,
     420,   420,   421,   421,   421,   422,   422,   422,   423,   424,
     425,   425,   426,   426,   427,   427,   428,   428,   429,   429,
     430,   430,   430,   431,   431,   432,   433,   433,   434,   435,
     435,   436,   436,   437,   437,   438,   438,   439,   439,   440,
     440,   440,   441,   441,   441,   442,   442,   443,   444,   444,
     444,   445,   446,   446,   446,   446,   447,   447,   447,   448,
     448,   449,   449,   449,   450,   451,   451,   451,   452,   452,
     453,   453,   453,   454,   454,   454,   454,   455,   455,   455,
     456,   456,   456,   456,   456,   457,   457,   458,   458,   458,
     458,   458,   458,   458,   458,   458,   458,   458,   459,   459,
     460,   461,   461,   462,   462,   463,   463,   464,   464,   465,
     465,   466,   466,   466,   466,   466,   466,   466,   466,   466,
     466,   466,   466,   466,   466,   466,   466,   466,   466,   466,
     467,   467,   468,   469,   469,   470,   470,   470,   471,   471,
     471,   471,   472,   472,   473,   473,   474,   474,   475,   475,
     476,   476,   477
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
       7,    11,     9,    13,     3,     7,    11,    13,     7,     7,
       5,     7,     5,     9,     7,     5,     9,     5,     5,     3,
       7,     3,     7,     7,     3,     5,     5,     3,     5,     7,
       9,     1,     1,     1,     1,     1,     0,     2,     1,     3,
       1,     1,     1,     0,     3,     1,     0,     3,     1,     0,
       3,     0,     3,     0,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     1,     4,     1,     1,
       4,     1,     1,     4,     1,     4,     5,     1,     3,     1,
       3,     1,     1,     4,     9,     1,     1,     4,     1,     5,
       1,     1,     4,     1,     1,     5,     1,     1,     1,     4,
       1,     1,     5,     1,     1,     1,     3,     1,     1,     3,
       1,     4,     3,     3,     3,     3,     3,     3,     1,     1,
       3,     1,     3,     0,     1,     1,     1,     1,     3,     0,
       1,     1,     2,     2,     4,     6,     4,     6,     6,     6,
       6,     2,     6,     8,     8,    10,    14,     2,     1,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,    10,     9
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
     467,     0,   470,     0,   512,     0,   468,   489,    31,     0,
     178,     0,     8,     0,   428,   429,     0,   465,   185,     0,
       0,     0,    11,   514,   513,    13,   437,     0,   230,   231,
       0,     0,   434,     0,     0,   198,   432,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     508,   491,   510,     0,   461,   463,   464,     0,   460,   458,
       0,   266,   270,   457,   267,   454,   456,     0,   453,   451,
       0,   234,     0,   450,   393,   392,     0,   415,   416,     0,
       0,     0,     0,   309,   308,     0,   442,     0,     0,   441,
       0,     0,     0,     0,   530,     0,     0,   364,     0,     0,
       0,     0,     0,   349,   350,   395,   394,     0,   156,     0,
       0,     0,     0,   431,     0,     0,     0,     0,     0,   319,
       0,   343,   342,   517,   515,   516,   189,   188,     0,     0,
       0,     0,     0,     0,     0,   210,   211,     0,     0,     0,
       0,    15,   121,     0,     0,     0,   379,   381,   384,   387,
     151,     0,     0,     0,     0,   160,     0,     0,     0,     0,
       0,   486,   485,   487,   490,     0,   523,   525,   522,   524,
     526,   527,     0,     0,     0,   128,   129,   124,   122,     0,
       0,     0,     0,    30,   175,    28,     0,     0,     0,     0,
       0,     0,   439,     0,     0,     0,     0,     0,     0,     0,
     492,   493,     0,     0,     0,   501,     0,     0,     0,   507,
       0,     0,     0,     0,     0,     0,   269,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   157,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   182,   181,     0,     0,     0,   176,     0,
       0,     0,     0,     0,     0,   483,   469,   477,     0,     0,
     472,   473,   474,   475,   476,     0,   154,     0,   467,     0,
       0,     0,     0,   145,   143,   149,   147,     0,     0,     0,
     179,     0,     0,   186,   466,    10,   344,     0,     9,     0,
       0,   438,     0,     0,     0,   233,   232,   198,   199,   229,
       0,     0,   214,     0,     0,     0,     0,   448,     0,     0,
     446,     0,     0,   445,     0,     0,   509,   511,   355,     0,
       0,     0,   268,     0,     0,   236,   238,   303,   218,     0,
      16,   139,   305,     0,     0,   417,   418,     0,   353,     0,
       0,   354,   351,   421,     0,   419,     0,   420,     0,   375,
     310,     0,   311,     0,   201,     0,     0,     0,     0,   316,
     315,     0,     0,   190,   191,   388,   528,   529,     0,   318,
       0,     0,   323,     0,     0,   203,     0,     0,   370,     0,
     372,     0,     0,     0,     0,   347,     0,    14,     0,   172,
       0,     0,   162,   378,   377,     0,     0,   385,   386,     0,
     481,   484,     0,   471,   159,   488,   123,     0,     0,   132,
       0,   131,     0,   130,     0,   136,     0,   127,     0,   126,
       0,   125,    32,   430,     0,     0,   440,   433,     0,   435,
       0,   494,     0,     0,     0,   496,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   459,   520,   518,   519,     0,
     281,   278,   272,   298,     0,   297,     0,     0,     0,   296,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,   282,     0,   284,   285,   295,     0,   280,   271,   286,
     521,   274,     0,   452,   237,   209,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   193,     0,     0,   314,     0,     0,     0,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   177,   171,   173,     0,     0,     0,     0,
       0,   152,     0,   144,   146,   148,   150,     0,   137,     0,
       0,   141,     0,     0,   436,   228,     0,   215,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   462,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,     0,     0,   304,   140,
      22,     0,   396,     0,     0,     0,     0,   426,   425,   360,
     365,     0,   312,     0,   205,     0,     0,   313,   317,     0,
       0,   389,     0,     0,     0,   369,     0,   368,     0,   207,
       0,   396,   371,   374,   213,     0,   212,   184,   348,     0,
     164,   380,   383,   382,   478,   479,   480,   482,     0,     0,
     135,   134,     0,   133,     0,     0,   495,   497,     0,   502,
       0,   498,     0,   447,   500,   499,     0,   301,   294,   288,
     287,   289,   299,   300,   302,   290,   291,   293,   275,   292,
     276,   279,   283,   273,     0,   422,   241,   242,     0,   247,
     245,   246,   258,   259,   248,   249,   251,     0,     0,   260,
       0,   262,     0,   255,   256,     0,   239,   243,   423,   240,
       0,   224,   225,     0,   226,     0,   223,   219,     0,   306,
       0,     0,   403,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,   216,   217,     0,     0,     0,   325,     0,
       0,     0,     0,     0,     0,     0,   330,   331,     0,     0,
       0,   324,     0,     0,     0,   403,     0,     0,     0,     0,
       0,     0,   164,   153,   138,   142,   345,     0,     0,     0,
     449,     0,     0,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,     0,   400,   401,   402,   397,   398,
       0,   406,     0,     0,   352,   362,     0,   376,   200,     0,
     396,   390,   192,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   202,     0,   406,   373,
     218,   346,     0,   168,   163,   165,     0,   503,     0,   504,
     473,   424,   250,   252,   253,   261,   254,   264,   263,   257,
     221,   222,   220,     0,     0,     0,     0,   411,     0,     0,
       0,     0,     0,   403,     0,     0,   408,   335,   337,   338,
     339,   341,   340,   334,   326,   327,   328,   329,   332,   333,
     336,     0,     0,   411,   208,   166,    30,     0,     0,     0,
       0,     0,   444,   399,   405,   404,     0,     0,   413,     0,
     532,     0,   361,   204,   406,     0,   320,   366,   206,   413,
      30,   169,     0,   505,    19,     0,   265,   407,     0,     0,
     409,   531,     0,   411,     0,     0,   409,   167,     0,     0,
     412,     0,     0,   356,   363,   413,   321,   367,   194,     0,
      20,   414,     0,   358,   409,     0,    24,    25,    22,     0,
       0,   410,   359,   357,   196,   195,    12,    21,   506,     0,
     197
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,   102,   103,   104,   105,   106,
    1120,  1180,   959,  1178,    12,   176,   107,   424,   108,   109,
     110,   111,   233,   112,   113,   797,   590,   800,   529,   530,
     531,   532,   114,   515,   888,   115,   116,   484,   117,   118,
     234,   786,  1001,  1002,  1140,  1116,   119,   651,   120,   121,
     251,   122,   648,   123,   422,   124,   378,   504,   643,   125,
     126,   127,   355,   347,   128,  1175,   129,  1189,   439,   616,
     637,   856,   871,   130,   377,   875,   563,   975,   745,   957,
     131,   274,   557,   132,   460,   311,   744,   946,  1079,   133,
     457,   301,   456,   738,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   766,   144,   145,   146,   147,   772,
     991,  1103,   148,   149,   548,   150,   151,   152,   153,   154,
     155,   156,   157,   158,  1173,  1183,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   316,   348,   962,  1028,  1029,  1031,  1125,  1087,  1097,
    1163,  1128,  1150,   319,   597,   606,   947,   849,   607,   257,
     352,   291,   276,   433,   328,   329,   572,   573,   312,   313,
     302,   303,   401,   353,   886,   660,   661,   662,   403,   404,
     405,   292,   451,   246,   265,   367,   741,   419,   420,   421,
     628,   333,   334
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1047
static const yytype_int16 yypact[] =
{
     194,   -88,   -71,   115, -1047,   194,    34,   -46,   -14, -1047,
   -1047,     8,   865,    19, -1047,   119, -1047,    32,    48,    50,
      53, -1047,    61,    63,    69,    81,    88,   120,   122,   132,
     165,   168,   180,   182,   188,   196,   198,   217,   219,   222,
     224,   225,   231,   233,   234,   248,   252,   253,   259,   260,
     261,   263,   265,   267,   269,   271,   287,   288,   294,   298,
     299,   302,   305,   306,   307,   308,    30,   310,   313,   314,
   -1047,   315,    23,   421, -1047, -1047,   316,   319,   320,   321,
      12,    38,    28, -1047, -1047, -1047, -1047, -1047, -1047, -1047,
   -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047,
   -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047,   865, -1047,
   -1047,   163, -1047, -1047, -1047, -1047, -1047,   323, -1047, -1047,
   -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047,
   -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047,
   -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047,
   -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047,
   -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047,
   -1047, -1047, -1047, -1047,   277,    92, -1047,  -119,   417,    71,
     289,   199,   949,   103,   103,    95,   -47,    43,   -31,   330,
     -31,   936,  -117,  -117,   -26,   286,   -31,   -31,   452,   -31,
     148,  -117,  -117,   -18,   -26,   -26,   -26,    38,   325,    38,
     -31,   949,   949,   -31,   949,   949,   116,   -31,   949,   -31,
     -18,   949,  -117,   949,    80, -1047,   949,  -117,   401,   804,
      38, -1047, -1047,   280,   326,   -31,   -31,   -31,   -31,   332,
   -1047,    18, -1047,   336, -1047,   159, -1047,   112, -1047,   439,
   -1047,   335, -1047,   119, -1047, -1047,   339, -1047,  -126,   346,
     352,   355, -1047, -1047, -1047, -1047, -1047,   357, -1047, -1047,
     363,   572, -1047,   362,   368,   372, -1047, -1047,  -117,  -117,
     -31,   -31,   365,   -31,   373,   378,   379,   949,   383,   619,
   -1047, -1047,   324,   392, -1047, -1047, -1047,   613, -1047, -1047,
     393, -1047, -1047, -1047, -1047, -1047, -1047,   622, -1047, -1047,
     400,   399,   406, -1047, -1047, -1047,   413, -1047, -1047,   414,
     415,   419,   425, -1047, -1047,   644, -1047,   423,   431, -1047,
     433,   437,   652,   435, -1047,   451,   453,   455,   460,   461,
     689,   480,   483, -1047, -1047, -1047, -1047,   485,   707,   497,
     504,   507,   512,   354,   731,   514,   -42,   517,   519, -1047,
     520, -1047, -1047, -1047, -1047, -1047, -1047, -1047,   522,   524,
     525,   526,   527,   533,   536, -1047, -1047,   537,   335,   544,
     558,   559, -1047,   511,    38,    38,   576,   580,   581,   582,
   -1047,   535,   221,    38,    38, -1047,    38,    38,    38,    38,
      38,   530,   354, -1047,   589,   594, -1047, -1047, -1047, -1047,
   -1047, -1047,   597,    35,    24, -1047, -1047,   530,   354,   598,
     599,   600,   865,   865, -1047, -1047,    38,   815,  -119,   835,
      26,   836,   618,   614,   949,   620,    38,   206,   839,   615,
   -1047, -1047,   623,   625,   769, -1047,   -31,   -31,   482, -1047,
     629,   624,   949,   751,   633,    38,   634,   335,   635,    38,
     335,   -31,   -31,   949,   768,    16,   775,   637,    38,    60,
     819,   857,   640,   822,   823,   283,   680,   -31,   776,   646,
     777,   -26,   -38, -1047,   650,   -26,   -26,   -26,    38,   653,
      85,   -31,    42,   779,   -16,   686,   778,   772,   -21,    51,
     745,   747,    43,   750, -1047,    79,    79,   740, -1047,   130,
     671,   -22,   754,   760,    27,   708, -1047, -1047,   333,   366,
     117,   117, -1047, -1047, -1047,   112, -1047,   949,   678,   -84,
     -82,   -80,   -72, -1047, -1047,   530,   354,   111,   138,    57,
   -1047,   679,   432, -1047, -1047, -1047, -1047,   906, -1047,   688,
     357, -1047,   695,   907,   474, -1047, -1047,   372, -1047, -1047,
     -31,   -31,   641,   704,   709,   711,   716, -1047,   718,   555,
   -1047,   717,   720, -1047,   722,   723, -1047, -1047, -1047,   735,
     498,   340, -1047,   736,   506, -1047, -1047, -1047, -1047,   746,
     749, -1047,   752,   965,   516, -1047, -1047,   766, -1047,   767,
     968, -1047,   770, -1047,   765, -1047,   773, -1047,   771,   782,
   -1047,   969, -1047,   784, -1047,   993,   787,    60,   788,   789,
   -1047,   790,   918, -1047, -1047, -1047, -1047, -1047,   796, -1047,
     797,   803, -1047,   806,   807, -1047,  1026,   809, -1047,   813,
   -1047,   814,   816,   898,  1036,   818,   825, -1047,   335, -1047,
     759,    38, -1047, -1047,   530,   827,   829, -1047, -1047,   830,
   -1047,   833,   820, -1047, -1047, -1047, -1047,  1053,   837, -1047,
       2, -1047,    38, -1047,  -119, -1047,    21, -1047,    16, -1047,
      29, -1047, -1047, -1047,   844,  1056, -1047, -1047,   843, -1047,
     838, -1047,   848,   988,   949, -1047,    38,    38,   949,   850,
      38,   949,   949,   849,   851, -1047, -1047, -1047, -1047,   852,
   -1047, -1047, -1047, -1047,   854, -1047,   855,   856,   858, -1047,
     859,   860,   861,   864,   866,   867,   870,   871,   872, -1047,
     873, -1047,   876, -1047, -1047, -1047,   877, -1047, -1047, -1047,
   -1047, -1047,   863, -1047,   879,   880,    43,    16,  1069,   881,
   -1047,   -18,  1076,   902,   938,  1143,    62,    95,   981,   -62,
    1077,   947,   -11, -1047,   941,  1079, -1047,    38,   952,  -119,
    1106,   -31,   954,  1078,    65,   955,    -7,  1080,  1175,   275,
     335,  1083,    79, -1047, -1047,   354,   957,    60,   -12,   149,
     708, -1047,   -34, -1047, -1047,   354,   530,   -67, -1047,   -32,
      -1, -1047,    60,   962, -1047, -1047,    38, -1047,   963,   201,
     304,   964,    60,   529,   967,   971,    38, -1047,    38,   103,
    1089,  1081,    38,  1071,  1084,  1072,    38,    38,    38,  -119,
    1046,    38,    38,    38,    95, -1047,   956,   318, -1047, -1047,
     980,  1199,   983,   984,  1203,   985,   982, -1047, -1047,   989,
   -1047,   987, -1047,  1208, -1047,   311,   991, -1047, -1047,   992,
      91,   530,   994,   995,   528, -1047,   996, -1047,  1211, -1047,
    1215,   983, -1047,   998, -1047,   999, -1047, -1047, -1047,  1000,
      33, -1047, -1047,   530, -1047, -1047, -1047, -1047,   335,    21,
   -1047, -1047,    29, -1047,  1001,  1218,   354, -1047,  1180, -1047,
      38, -1047,  1002, -1047, -1047, -1047,   538, -1047, -1047, -1047,
   -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047,   530, -1047,
   -1047, -1047, -1047, -1047,     1, -1047, -1047, -1047,  -119, -1047,
   -1047, -1047, -1047, -1047, -1047,  1005, -1047,  1006,  1007, -1047,
    1008, -1047,  1009, -1047, -1047,  1010, -1047, -1047, -1047,   530,
    1011, -1047, -1047,  1012, -1047,  1013, -1047, -1047,  1222, -1047,
    1015,   424,  1134,  1235,  1018,    95, -1047,    -2,    95,  1016,
    1020,    -7,    38, -1047, -1047,  1019,  1173,  1150, -1047,  1023,
    1024,  1025,  1027,  1028,  1029,  1030, -1047, -1047,  1031,  1032,
    1033, -1047,  1142,  1034,  1035,  1134,  1254,    91,  1253,   367,
    1038,  1037,    33, -1047, -1047, -1047, -1047,  1040,  1039,   293,
   -1047,    38,  1179,   530,    38,    38,    38,    38,    38,   -87,
     103,  1261,  1123, -1047,  1264, -1047, -1047, -1047, -1047,  1047,
    1048,  1169,  1050,  1269, -1047,  1052,  1054, -1047, -1047,   206,
     983, -1047, -1047,  1057,  1058,  1176,  1271,    37,    95,   103,
      26,    26,   -31,   -47,  1274,  1059, -1047,  1278,  1169, -1047,
   -1047, -1047,  1060, -1047, -1047, -1047,  1279, -1047,  1241, -1047,
     -74, -1047, -1047, -1047, -1047, -1047, -1047, -1047,  1003, -1047,
   -1047, -1047, -1047,  1062,   424,  1185,  1066,  1266,  1287,  1067,
    1107,    95,  1068,  1134,  1200,  1202, -1047, -1047, -1047, -1047,
   -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047,
   -1047,  1189,  1073,  1266,   880, -1047,   865,  1082,  1074,  1085,
    1075,   -87, -1047, -1047, -1047, -1047,  1176,  1086,  1276,  1087,
   -1047,  1088, -1047, -1047,  1169,  1091, -1047,  1092, -1047,  1276,
     865, -1047,    60, -1047, -1047,  1093, -1047, -1047,  1204,  1094,
    1115, -1047,    95,  1266,  1212,    60,  1115, -1047,  1096,   -31,
   -1047,  1303,  1097, -1047, -1047,  1276, -1047, -1047, -1047,    83,
    1099, -1047,    95,   335,  1115,   335, -1047, -1047,   980,   -31,
    1098, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047,   335,
   -1047
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1047, -1047,  1305, -1047, -1047, -1047, -1047, -1047, -1047, -1047,
   -1047, -1047,   141, -1047, -1047,  1055,  -108,  -376,   894,  1095,
    1249,  -502, -1047, -1047, -1047, -1047,   647, -1047, -1047, -1047,
   -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047,  1260, -1047,
   -1047, -1047,   327, -1047, -1047, -1047, -1047, -1047, -1047, -1047,
   -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047,
   -1047, -1047,   840,  1108, -1047, -1047, -1047, -1047,   774, -1047,
   -1047, -1047,   356, -1047, -1047, -1047,  -613,   337,   272, -1047,
   -1047,   478,   296, -1047, -1047, -1047, -1047, -1047,   215, -1047,
   -1047,  1153, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047,
   -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047,
   -1047, -1047, -1047, -1047,  -637, -1047, -1047, -1047, -1047, -1047,
   -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047,
   -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047, -1047,
   -1047,   285,   588,  -841,   256, -1047,  -934, -1047,  -945,   216,
   -1046, -1016, -1036,  -477,  -610, -1047, -1047, -1047,   505,   915,
    -227,  -179,  -391,   794,  -110,  -398,  -491,  -653,  -534,  -453,
    -529,  -629,  -171,   -76, -1047,   556, -1047, -1047,   824, -1047,
   -1047,  1061,  -147,   764, -1047,  -480, -1047, -1047, -1047, -1047,
   -1047,  -155, -1047
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -236
static const yytype_int16 yytable[] =
{
     248,   275,   503,   383,   679,   245,   258,   763,   677,   320,
     624,   322,   591,   659,   645,   646,   534,   336,   337,   305,
     339,   341,   240,   533,   294,   642,   391,   801,   240,   546,
     995,   357,   567,   331,   360,   293,    16,   314,   368,   528,
     370,  1099,   240,   391,   324,   342,   614,   798,   681,   349,
     350,   351,   630,   266,   267,   638,   386,   387,   388,   389,
     567,  1058,   653,   635,   358,   359,   345,   361,   362,   866,
     415,   369,   882,   854,   372,   263,   374,   869,   417,   379,
     807,   582,   567,   330,   585,   375,   266,   267,  1176,   363,
    1035,   343,   344,   364,   365,   973,   427,  1139,   305,  1077,
     306,   442,   443,  1156,   445,   295,   294,   325,   266,   267,
    1168,   254,   373,  1113,   294,     9,   240,   380,   317,   296,
     363,   626,   255,   256,   364,   365,   627,   318,  1184,  1174,
     326,   327,  1100,   356,  1101,     7,   595,  1165,   668,   416,
     670,   305,   672,  1078,   596,   669,   847,   671,  1119,   673,
     674,   -18,     8,   848,   639,   889,   428,   675,   510,  1134,
     844,    11,   890,   804,   376,   392,   876,  1177,   440,   441,
     264,   402,   867,   418,   657,   974,   658,   881,   631,   306,
     491,  1102,   270,   315,    13,  1036,   615,   295,   841,  1153,
     747,   804,   894,   271,   999,   295,  1000,   891,   332,  1093,
     366,   296,   902,   636,   398,   399,   400,   331,   254,   296,
       1,     2,   346,   855,   272,   273,    14,   870,   254,   255,
     256,   892,   306,   850,  1012,   591,   271,   755,   893,   255,
     256,    15,   266,   267,   396,   397,   398,   399,   400,  1005,
     307,   174,   241,   535,   239,   297,   308,   175,   413,   230,
     547,   298,   247,   644,   254,   177,    81,   242,   243,   241,
    1004,   621,   241,   242,   243,   255,   256,   565,   566,   838,
     793,   178,   783,   179,   242,   243,   180,   242,   243,   794,
     252,   644,   586,   587,   181,   680,   182,   552,   659,   649,
     908,   879,   183,   244,   839,   555,   556,   564,   609,   244,
     923,   574,   277,   644,   184,   577,   570,   571,   509,   354,
     244,   185,   625,   244,   253,   541,   588,   518,   519,   307,
     520,   521,   522,   523,   524,   308,   613,   297,   570,   571,
     618,   619,   620,   298,   321,   297,   241,   392,   536,   676,
     654,   298,   254,   186,   706,   187,   309,   310,   707,   708,
     542,   242,   243,   255,   256,   188,   299,   300,   562,   874,
     554,   270,   307,   394,   299,   300,   678,   603,   308,   604,
     605,   363,   340,   266,   267,   364,   365,   268,   269,   580,
     666,   691,   692,   584,   884,   885,   395,   244,   189,   309,
     310,   190,   594,   272,   273,   398,   399,   400,   950,   268,
     269,   709,   739,   191,   877,   192,   396,   397,   398,   399,
     400,   193,   650,  1106,  1107,   710,   711,   712,   713,   194,
     714,   195,   715,   898,   266,   267,   899,   259,   260,   261,
     262,  1034,   951,   952,  1037,   396,   397,   398,   399,   400,
     196,   953,   197,   240,   249,   198,   517,   199,   200,   402,
     954,   716,   717,   718,   201,   719,   202,   203,   720,   721,
     722,   955,   723,   724,   725,   406,   726,   407,  1025,  1026,
    1027,   204,   408,   409,   410,   205,   206,   396,   397,   398,
     399,   400,   207,   208,   209,   567,   210,   956,   211,   568,
     212,  1080,   213,   727,   214,   728,   729,   396,   397,   398,
     399,   400,   270,   796,   730,   731,   732,   733,   734,   735,
     215,   216,  1003,   271,  1104,  1068,   335,   217,  1069,  1062,
    1105,   218,   219,   266,   267,   220,   900,   736,   221,   222,
     223,   224,  1158,   226,   272,   273,   227,   228,   229,   235,
     859,   737,   236,   237,   238,  1167,   250,   808,   338,   354,
     381,   811,   385,   270,   814,   815,   390,  1132,   699,   435,
     663,   384,   393,   423,   271,   426,   266,   267,   429,   396,
     397,   398,   399,   400,   430,   785,   435,   431,   432,   411,
     396,   397,   398,   399,   400,   272,   273,   434,   436,   444,
     437,   907,   863,   664,   438,   911,   795,   446,   861,   915,
     916,   917,   447,   448,   920,   921,   922,   450,   452,   396,
     397,   398,   399,   400,   453,   244,   454,   883,  1164,   455,
     809,   810,   978,   435,   813,   458,   459,  -235,   461,   979,
     396,   397,   398,   399,   400,   462,   463,   464,  1181,   266,
     267,   465,   396,   397,   398,   399,   400,   466,   467,   468,
     412,   980,   270,   469,   231,   470,   472,   473,   918,   683,
      84,   471,    86,   413,    88,   949,    90,   414,    92,   254,
      94,   981,    96,   474,    98,   475,   100,   476,   242,   243,
     255,   256,   477,   478,   272,   273,   326,   327,   278,   279,
     280,   281,   282,   479,   283,   270,   284,   285,   286,   287,
     288,   689,   480,   266,   267,   481,   569,   482,   396,   397,
     398,   399,   400,   483,   244,   982,   983,   984,   985,   485,
     986,   987,   988,   989,   990,   705,   486,   272,   273,   487,
     896,   570,   571,   743,   488,   489,   490,   290,   508,   492,
     906,   493,   494,   750,   495,  1041,   496,   497,   498,   499,
     396,   397,   398,   399,   400,   500,   903,  1013,   501,   502,
     516,   278,   279,   280,   281,   282,   505,   283,   270,   284,
     285,   286,   287,   288,   396,   397,   398,   399,   400,   289,
     506,   507,   396,   397,   398,   399,   400,  1072,  1073,  1074,
    1075,  1076,   396,   397,   398,   399,   400,  1182,   511,  1185,
     272,   273,   512,   513,   514,   396,   397,   398,   399,   400,
     290,   525,   428,  1190,   396,  1011,   398,   399,   400,   526,
     527,   537,   538,   539,  1009,   278,   279,   280,   281,   282,
     543,   283,   270,   284,   285,   286,   287,   288,   545,   549,
     550,   551,   553,   289,   558,   560,   559,   561,   575,   576,
     578,   589,   562,   266,   267,   579,   581,   583,   592,   593,
     598,   599,   600,   601,   272,   273,   602,   608,   611,   610,
     612,    16,   617,  1108,   290,   622,   629,   632,   634,    17,
     640,   633,   641,    18,    19,    20,    21,    22,    23,  -183,
     647,    24,    25,    26,    27,    28,    29,    30,   652,   655,
     667,    31,    32,    33,    34,    35,   656,    36,   682,   684,
     685,   688,    37,    38,    39,    40,    41,    42,    43,    44,
     687,    45,    46,    47,   693,    48,   694,    49,    50,    51,
      52,    53,    54,   696,   695,  1070,    55,    56,   697,    57,
     698,    83,   701,   700,   702,   703,    58,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   704,   742,    59,   746,   749,
      60,   747,   753,   759,   748,   278,   279,   280,   281,   282,
    1170,   283,   270,   284,   285,   286,   287,   288,   751,   752,
      61,   755,   754,   289,   757,   756,    62,   761,    63,    64,
    1187,   768,    65,    66,   758,    67,   760,    68,  1141,   762,
     764,   765,   767,    69,   272,   273,    70,    71,   769,   770,
     266,   267,    72,    73,   290,    74,   771,    75,   773,   774,
     775,   776,  1157,   266,   267,   777,   778,   780,   779,   699,
     781,   784,   924,   925,    85,   791,    87,   782,    89,   787,
      91,   788,    93,   789,    95,   790,    97,   792,    99,   803,
     101,   325,    76,    77,    78,    79,   802,    80,   804,   805,
     806,   562,   812,   816,   840,   818,   817,   819,   820,   821,
     843,   822,   823,   824,   825,   926,   927,   826,   835,   827,
     828,    81,   928,   829,   830,   831,   832,    82,    83,   833,
     834,   836,   837,   841,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   323,   929,   844,   845,   930,   931,   932,   933,
     934,   935,   936,   937,   938,   939,   940,   941,   942,   943,
     944,   945,   278,   279,   280,   281,   282,   846,   283,   270,
     284,   285,   286,   287,   288,   278,   279,   280,   281,   282,
     289,   283,   270,   284,   285,   286,   287,   288,   851,   853,
     852,   857,   858,   289,   860,   862,   864,   868,   865,   873,
     872,   272,   273,   878,   895,   880,   254,   912,   897,   901,
     910,   290,   904,   914,   272,   273,   905,   255,   256,   909,
     919,   913,   958,   960,   290,   961,   963,   964,   965,   966,
     968,   967,   969,   971,   972,   993,   976,   977,   992,   994,
     996,   997,   998,  1007,  1008,  1023,  1006,  1010,  1014,  1015,
    1016,  1017,  1018,  1019,  1020,  1021,  1022,  1024,  1030,  1032,
    1033,  1038,  1039,  1043,  1042,  1044,  1045,  1046,  1047,  1055,
    1048,  1049,  1050,  1051,  1052,  1053,  1054,  1057,  1059,  1056,
    1061,  1063,  1066,  1071,  1067,  1081,  1064,  1082,  1083,  1084,
    1086,  1085,  1088,  1089,  1090,  1098,  1096,  1091,  1110,  1094,
    1095,  1111,  1112,  1115,  1117,  1118,  1121,  1122,  1124,  1126,
    1127,  1129,  1130,  1133,  1131,  1135,  1136,  1137,  1138,  1143,
    1145,  1149,  1162,  1160,  1142,  1144,  1166,  1171,   425,  1148,
      10,  1152,  1151,  1154,  1155,  1159,   540,  1161,  1169,  1186,
    1172,  1179,   232,  1188,   382,   799,   225,  1040,   371,  1065,
     623,   690,  1114,   970,  1060,  1092,  1146,   304,  1109,   842,
    1123,   948,  1147,   544,   686,   740,   887,     0,   449,   665
};

static const yytype_int16 yycheck[] =
{
     108,   180,   378,   230,   538,    81,   177,   617,   537,   188,
     490,   190,   465,   515,   505,   506,   414,   196,   197,     3,
     199,   200,     4,   414,     3,   502,     8,   680,     4,     3,
     871,   210,     3,    59,   213,   182,     6,    84,   217,     4,
     219,     4,     4,     8,   191,   200,    84,   676,   539,   204,
     205,   206,    68,    84,    85,     4,   235,   236,   237,   238,
       3,   995,    84,    84,   211,   212,    84,   214,   215,     4,
     249,   218,    84,    84,   221,     4,   223,    84,   249,   226,
     693,   457,     3,   193,   460,     5,    84,    85,     5,     4,
      92,   201,   202,     8,     9,     4,   222,  1113,     3,   186,
      84,   280,   281,  1139,   283,    84,     3,   224,    84,    85,
    1156,   230,   222,  1058,     3,     0,     4,   227,    75,    98,
       4,    79,   241,   242,     8,     9,    84,    84,  1174,  1165,
     247,   248,    95,   209,    97,   223,    76,  1153,   222,   249,
     222,     3,   222,   230,    84,   229,    84,   229,   222,   229,
     222,   225,   223,    91,   103,   222,   282,   229,   385,  1093,
     222,   127,   229,   225,    84,   241,   779,    84,   278,   279,
      99,   247,   107,   249,   147,    84,   149,   787,   194,    84,
     222,   144,   213,   230,   230,   187,   224,    84,   222,  1134,
     222,   225,   802,   224,   161,    84,   163,   229,   224,  1040,
      84,    98,   812,   224,   278,   279,   280,    59,   230,    98,
      16,    17,   230,   224,   245,   246,   230,   224,   230,   241,
     242,   222,    84,   757,   223,   678,   224,   226,   229,   241,
     242,   223,    84,    85,   276,   277,   278,   279,   280,   892,
     224,   222,   224,   414,   232,   224,   230,   128,   224,   226,
     224,   230,   224,   224,   230,   223,   226,   239,   240,   224,
     889,   488,   224,   239,   240,   241,   242,   446,   447,   746,
     668,   223,   648,   223,   239,   240,   223,   239,   240,   670,
       3,   224,   461,   462,   223,   228,   223,   434,   790,   159,
     819,   782,   223,   275,   747,    89,    90,   444,   477,   275,
     834,   448,   103,   224,   223,   452,   249,   250,   384,   224,
     275,   223,   491,   275,   222,   423,   463,   393,   394,   224,
     396,   397,   398,   399,   400,   230,   481,   224,   249,   250,
     485,   486,   487,   230,     4,   224,   224,   413,   414,   228,
     511,   230,   230,   223,     4,   223,   251,   252,     8,     9,
     426,   239,   240,   241,   242,   223,   253,   254,    83,    84,
     436,   213,   224,   204,   253,   254,   228,    84,   230,    86,
      87,     4,   224,    84,    85,     8,     9,    88,    89,   455,
     527,   560,   561,   459,   235,   236,   227,   275,   223,   251,
     252,   223,   468,   245,   246,   278,   279,   280,    80,    88,
      89,    61,   581,   223,   780,   223,   276,   277,   278,   279,
     280,   223,   282,  1050,  1051,    75,    76,    77,    78,   223,
      80,   223,    82,   222,    84,    85,   225,    10,    11,    12,
      13,   965,   114,   115,   968,   276,   277,   278,   279,   280,
     223,   123,   223,     4,   281,   223,   225,   223,   223,   525,
     132,   111,   112,   113,   223,   115,   223,   223,   118,   119,
     120,   143,   122,   123,   124,    26,   126,    28,    44,    45,
      46,   223,    33,    34,    35,   223,   223,   276,   277,   278,
     279,   280,   223,   223,   223,     3,   223,   169,   223,     7,
     223,  1020,   223,   153,   223,   155,   156,   276,   277,   278,
     279,   280,   213,   674,   164,   165,   166,   167,   168,   169,
     223,   223,   888,   224,  1048,   222,   230,   223,   225,   999,
    1049,   223,   223,    84,    85,   223,   222,   187,   223,   223,
     223,   223,  1142,   223,   245,   246,   223,   223,   223,   223,
     767,   201,   223,   223,   223,  1155,   223,   694,    96,   224,
     149,   698,   226,   213,   701,   702,   224,  1091,     3,     4,
     227,   281,   226,   228,   224,   226,    84,    85,   222,   276,
     277,   278,   279,   280,   222,   651,     4,   222,   221,   140,
     276,   277,   278,   279,   280,   245,   246,   224,   226,   224,
     222,   818,   771,   227,   222,   822,   672,   224,   769,   826,
     827,   828,   224,   224,   831,   832,   833,   224,   284,   276,
     277,   278,   279,   280,   222,   275,     3,   788,  1152,   226,
     696,   697,    94,     4,   700,     3,   226,   228,   222,   101,
     276,   277,   278,   279,   280,   222,   222,   222,  1172,    84,
      85,   222,   276,   277,   278,   279,   280,   222,     4,   226,
     211,   123,   213,   222,   233,   222,     4,   222,   829,   227,
     239,   224,   241,   224,   243,   836,   245,   228,   247,   230,
     249,   143,   251,   222,   253,   222,   255,   222,   239,   240,
     241,   242,   222,   222,   245,   246,   247,   248,   206,   207,
     208,   209,   210,     4,   212,   213,   214,   215,   216,   217,
     218,   227,   222,    84,    85,   222,   224,   222,   276,   277,
     278,   279,   280,     6,   275,   187,   188,   189,   190,   222,
     192,   193,   194,   195,   196,   227,   222,   245,   246,   222,
     806,   249,   250,   227,   222,     4,   222,   255,   227,   222,
     816,   222,   222,   227,   222,   972,   222,   222,   222,   222,
     276,   277,   278,   279,   280,   222,   227,   928,   222,   222,
     225,   206,   207,   208,   209,   210,   222,   212,   213,   214,
     215,   216,   217,   218,   276,   277,   278,   279,   280,   224,
     222,   222,   276,   277,   278,   279,   280,  1014,  1015,  1016,
    1017,  1018,   276,   277,   278,   279,   280,  1173,   222,  1175,
     245,   246,   222,   222,   222,   276,   277,   278,   279,   280,
     255,   222,   282,  1189,   276,   277,   278,   279,   280,   225,
     223,   223,   223,   223,   900,   206,   207,   208,   209,   210,
      15,   212,   213,   214,   215,   216,   217,   218,     3,     3,
     222,   227,   222,   224,     5,   222,   231,   222,   219,   225,
      99,    83,    83,    84,    85,   222,   222,   222,    83,   222,
      41,     4,   222,    41,   245,   246,    43,   187,   222,    93,
      93,     6,   222,  1052,   255,   222,    97,   191,   106,    14,
     135,   103,   135,    18,    19,    20,    21,    22,    23,   139,
     150,    26,    27,    28,    29,    30,    31,    32,   227,   145,
     222,    36,    37,    38,    39,    40,   146,    42,   229,     3,
     222,     4,    47,    48,    49,    50,    51,    52,    53,    54,
     225,    56,    57,    58,   283,    60,   222,    62,    63,    64,
      65,    66,    67,   222,   225,  1011,    71,    72,   222,    74,
     222,   233,   222,   226,   222,   222,    81,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   230,   230,   102,   222,     4,
     105,   222,     4,     4,   222,   206,   207,   208,   209,   210,
    1159,   212,   213,   214,   215,   216,   217,   218,   222,   222,
     125,   226,   222,   224,   223,   222,   131,     4,   133,   134,
    1179,    83,   137,   138,   222,   140,   222,   142,  1116,   222,
     222,   222,   222,   148,   245,   246,   151,   152,   222,   222,
      84,    85,   157,   158,   255,   160,   223,   162,   222,   222,
       4,   222,  1140,    84,    85,   222,   222,   139,   222,     3,
     222,   282,    86,    87,   240,   225,   242,   222,   244,   222,
     246,   222,   248,   223,   250,   222,   252,     4,   254,     3,
     256,   224,   197,   198,   199,   200,   222,   202,   225,   231,
     222,    83,   222,   224,     5,   223,   225,   223,   223,   223,
       4,   223,   223,   223,   223,   129,   130,   223,   225,   223,
     223,   226,   136,   223,   223,   223,   223,   232,   233,   223,
     223,   222,   222,   222,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   186,   167,   222,   187,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   206,   207,   208,   209,   210,     4,   212,   213,
     214,   215,   216,   217,   218,   206,   207,   208,   209,   210,
     224,   212,   213,   214,   215,   216,   217,   218,   187,   222,
      93,   230,    93,   224,   222,    69,   222,   222,   100,     4,
     100,   245,   246,   100,   222,   228,   230,   116,   225,   225,
     109,   255,   225,   121,   245,   246,   225,   241,   242,   110,
     154,   117,   222,     4,   255,   222,   222,     4,   223,   227,
     223,   222,     4,   222,   222,     4,   222,   222,   222,     4,
     222,   222,   222,     5,    44,     3,   225,   225,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   222,   104,     4,
     222,   225,   222,    70,   225,    95,   223,   223,   223,   107,
     223,   223,   223,   223,   223,   223,   223,   222,     4,   225,
       7,   223,   222,    84,   225,     4,   229,   144,     4,   222,
     101,   223,   222,     4,   222,     4,   100,   223,     4,   222,
     222,   222,     4,   223,     5,    44,   283,   225,   103,   223,
      24,     4,   225,   225,   187,    95,    94,   108,   225,   225,
     225,    25,   187,    99,   222,   220,    94,     4,   253,   223,
       5,   223,   225,   222,   222,   222,   422,   223,   222,  1178,
     223,   222,    73,   225,   229,   678,    66,   971,   220,  1002,
     490,   557,  1060,   855,   997,  1039,  1121,   184,  1053,   751,
    1084,   836,  1126,   428,   550,   581,   790,    -1,   287,   525
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    16,    17,   286,   287,   288,   289,   223,   223,     0,
     287,   127,   299,   230,   230,   223,     6,    14,    18,    19,
      20,    21,    22,    23,    26,    27,    28,    29,    30,    31,
      32,    36,    37,    38,    39,    40,    42,    47,    48,    49,
      50,    51,    52,    53,    54,    56,    57,    58,    60,    62,
      63,    64,    65,    66,    67,    71,    72,    74,    81,   102,
     105,   125,   131,   133,   134,   137,   138,   140,   142,   148,
     151,   152,   157,   158,   160,   162,   197,   198,   199,   200,
     202,   226,   232,   233,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   290,   291,   292,   293,   294,   301,   303,   304,
     305,   306,   308,   309,   317,   320,   321,   323,   324,   331,
     333,   334,   336,   338,   340,   344,   345,   346,   349,   351,
     358,   365,   368,   374,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   390,   391,   392,   393,   397,   398,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   222,   128,   300,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   323,   223,   223,   223,   223,
     226,   233,   305,   307,   325,   223,   223,   223,   223,   232,
       4,   224,   239,   240,   275,   458,   468,   224,   301,   281,
     223,   335,     3,   222,   230,   241,   242,   444,   457,    10,
      11,    12,    13,     4,    99,   469,    84,    85,    88,    89,
     213,   224,   245,   246,   366,   446,   447,   103,   206,   207,
     208,   209,   210,   212,   214,   215,   216,   217,   218,   224,
     255,   446,   466,   467,     3,    84,    98,   224,   230,   253,
     254,   376,   455,   456,   376,     3,    84,   224,   230,   251,
     252,   370,   453,   454,    84,   230,   426,    75,    84,   438,
     446,     4,   446,   186,   467,   224,   247,   248,   449,   450,
     449,    59,   224,   476,   477,   230,   446,   446,    96,   446,
     224,   446,   476,   449,   449,    84,   230,   348,   427,   476,
     476,   476,   445,   458,   224,   347,   458,   446,   467,   467,
     446,   467,   467,     4,     8,     9,    84,   470,   446,   467,
     446,   348,   467,   449,   467,     5,    84,   359,   341,   467,
     449,   149,   304,   445,   281,   226,   446,   446,   446,   446,
     224,     8,   458,   226,   204,   227,   276,   277,   278,   279,
     280,   457,   458,   463,   464,   465,    26,    28,    33,    34,
      35,   140,   211,   224,   228,   446,   449,   457,   458,   472,
     473,   474,   339,   228,   302,   300,   226,   222,   282,   222,
     222,   222,   221,   448,   224,     4,   226,   222,   222,   353,
     449,   449,   446,   446,   224,   446,   224,   224,   224,   466,
     224,   467,   284,   222,     3,   226,   377,   375,     3,   226,
     369,   222,   222,   222,   222,   222,   222,     4,   226,   222,
     222,   224,     4,   222,   222,   222,   222,   222,   222,     4,
     222,   222,   222,     6,   322,   222,   222,   222,   222,     4,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   302,   342,   222,   222,   222,   227,   458,
     445,   222,   222,   222,   222,   318,   225,   225,   458,   458,
     458,   458,   458,   458,   458,   222,   225,   223,     4,   313,
     314,   315,   316,   447,   450,   457,   458,   223,   223,   223,
     303,   301,   458,    15,   444,     3,     3,   224,   399,     3,
     222,   227,   467,   222,   458,    89,    90,   367,     5,   231,
     222,   222,    83,   361,   467,   446,   446,     3,     7,   224,
     249,   250,   451,   452,   467,   219,   225,   467,    99,   222,
     458,   222,   302,   222,   458,   302,   446,   446,   467,    83,
     311,   454,    83,   222,   458,    76,    84,   439,    41,     4,
     222,    41,    43,    84,    86,    87,   440,   443,   187,   446,
      93,   222,    93,   476,    84,   224,   354,   222,   476,   476,
     476,   445,   222,   347,   470,   446,    79,    84,   475,    97,
      68,   194,   191,   103,   106,    84,   224,   355,     4,   103,
     135,   135,   438,   343,   224,   451,   451,   150,   337,   159,
     282,   332,   227,    84,   457,   145,   146,   147,   149,   306,
     460,   461,   462,   227,   227,   463,   467,   222,   222,   229,
     222,   229,   222,   229,   222,   229,   228,   455,   228,   453,
     228,   451,   229,   227,     3,   222,   448,   225,     4,   227,
     353,   446,   446,   283,   222,   225,   222,   222,   222,     3,
     226,   222,   222,   222,   230,   227,     4,     8,     9,    61,
      75,    76,    77,    78,    80,    82,   111,   112,   113,   115,
     118,   119,   120,   122,   123,   124,   126,   153,   155,   156,
     164,   165,   166,   167,   168,   169,   187,   201,   378,   446,
     468,   471,   230,   227,   371,   363,   222,   222,   222,     4,
     227,   222,   222,     4,   222,   226,   222,   223,   222,     4,
     222,     4,   222,   439,   222,   222,   389,   222,    83,   222,
     222,   223,   394,   222,   222,     4,   222,   222,   222,   222,
     139,   222,   222,   302,   282,   458,   326,   222,   222,   223,
     222,   225,     4,   450,   447,   458,   457,   310,   456,   311,
     312,   452,   222,     3,   225,   231,   222,   361,   467,   458,
     458,   467,   222,   458,   467,   467,   224,   225,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   225,   222,   222,   438,   454,
       5,   222,   427,     4,   222,   187,     4,    84,    91,   442,
     453,   187,    93,   222,    84,   224,   356,   230,    93,   445,
     222,   457,    69,   446,   222,   100,     4,   107,   222,    84,
     224,   357,   100,     4,    84,   360,   361,   302,   100,   451,
     228,   439,    84,   457,   235,   236,   459,   460,   319,   222,
     229,   229,   222,   229,   439,   222,   458,   225,   222,   225,
     222,   225,   439,   227,   225,   225,   458,   445,   455,   110,
     109,   445,   116,   117,   121,   445,   445,   445,   457,   154,
     445,   445,   445,   453,    86,    87,   129,   130,   136,   167,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   372,   441,   443,   457,
      80,   114,   115,   123,   132,   143,   169,   364,   222,   297,
       4,   222,   428,   222,     4,   223,   227,   222,   223,     4,
     366,   222,   222,     4,    84,   362,   222,   222,    94,   101,
     123,   143,   187,   188,   189,   190,   192,   193,   194,   195,
     196,   395,   222,     4,     4,   428,   222,   222,   222,   161,
     163,   327,   328,   302,   456,   452,   225,     5,    44,   458,
     225,   277,   223,   457,   223,   223,   223,   223,   223,   223,
     223,   223,   223,     3,   222,    44,    45,    46,   429,   430,
     104,   431,     4,   222,   453,    92,   187,   453,   225,   222,
     357,   445,   225,    70,    95,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   107,   225,   222,   431,     4,
     362,     7,   470,   223,   229,   327,   222,   225,   222,   225,
     458,    84,   445,   445,   445,   445,   445,   186,   230,   373,
     455,     4,   144,     4,   222,   223,   101,   433,   222,     4,
     222,   223,   367,   428,   222,   222,   100,   434,     4,     4,
      95,    97,   144,   396,   453,   455,   399,   399,   446,   426,
       4,   222,     4,   433,   363,   223,   330,     5,    44,   222,
     295,   283,   225,   429,   103,   432,   223,    24,   436,     4,
     225,   187,   453,   225,   431,    95,    94,   108,   225,   436,
     329,   301,   222,   225,   220,   225,   373,   434,   223,    25,
     437,   225,   223,   433,   222,   222,   437,   301,   439,   222,
      99,   223,   187,   435,   453,   436,    94,   439,   435,   222,
     446,     4,   223,   409,   437,   350,     5,    84,   298,   222,
     296,   453,   302,   410,   435,   302,   297,   446,   225,   352,
     302
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
		      add_opvars(splev, "iiiiio", VA_PASS6(1, 1, (int)(yyvsp[(7) - (7)].i), 0, (int)(yyvsp[(5) - (7)].i), SPO_DECORATION));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2388 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2394 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2398 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2404 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_CARPET));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2410 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2414 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4(0, (int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2420 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2424 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2430 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2434 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2439 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2446 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2450 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2457 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2461 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2466 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2473 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2477 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2482 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2490 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2496 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2503 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2507 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2514 "lev_comp.y"
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
#line 2525 "lev_comp.y"
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
#line 2538 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2542 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2548 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2552 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2559 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2563 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2567 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2573 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2577 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2583 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2589 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2593 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2599 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2605 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2610 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2616 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2620 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2626 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2630 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2646 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2654 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2664 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2673 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2678 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2685 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2696 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2702 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2706 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2710 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2717 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2727 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2734 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2738 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2744 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2748 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2756 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2760 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2767 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2777 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2790 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2794 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2801 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2811 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2820 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2831 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2835 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2842 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2852 "lev_comp.y"
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

  case 454:

/* Line 1455 of yacc.c  */
#line 2863 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2872 "lev_comp.y"
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

  case 456:

/* Line 1455 of yacc.c  */
#line 2882 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2888 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2892 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2899 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2909 "lev_comp.y"
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

  case 461:

/* Line 1455 of yacc.c  */
#line 2920 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2929 "lev_comp.y"
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

  case 463:

/* Line 1455 of yacc.c  */
#line 2939 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2943 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2949 "lev_comp.y"
    { ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2951 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2957 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2961 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 2965 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2969 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 2977 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 2986 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 2990 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 2994 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 2998 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3002 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3005 "lev_comp.y"
    { ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3009 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3016 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3025 "lev_comp.y"
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

  case 485:

/* Line 1455 of yacc.c  */
#line 3066 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3070 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3077 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3084 "lev_comp.y"
    {
			      size_t len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3094 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3098 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3106 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3110 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3114 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3118 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3122 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3127 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3131 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3135 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3140 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3145 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3150 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3154 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3159 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3164 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3168 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3172 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3177 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3181 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3188 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3194 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3198 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3204 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 518:

/* Line 1455 of yacc.c  */
#line 3220 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 519:

/* Line 1455 of yacc.c  */
#line 3224 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
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
		      /* nothing */
		  ;}
    break;

  case 530:

/* Line 1455 of yacc.c  */
#line 3254 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 531:

/* Line 1455 of yacc.c  */
#line 3258 "lev_comp.y"
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

  case 532:

/* Line 1455 of yacc.c  */
#line 3284 "lev_comp.y"
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
#line 7469 "lev_comp.tab.c"
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
#line 3312 "lev_comp.y"


/*lev_comp.y*/

