
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
     ELEMENTAL_ENCHANTMENT_TYPE = 365,
     EXCEPTIONALITY_TYPE = 366,
     EXCEPTIONALITY_ID = 367,
     ELEMENTAL_ENCHANTMENT_ID = 368,
     ENCHANTMENT_ID = 369,
     SECRET_DOOR_ID = 370,
     USES_UP_KEY_ID = 371,
     MYTHIC_PREFIX_TYPE = 372,
     MYTHIC_SUFFIX_TYPE = 373,
     MYTHIC_PREFIX_ID = 374,
     MYTHIC_SUFFIX_ID = 375,
     MATERIAL_ID = 376,
     MATERIAL_TYPE = 377,
     CHARGES_ID = 378,
     SPECIAL_QUALITY_ID = 379,
     SPEFLAGS_ID = 380,
     SUBROOM_ID = 381,
     NAME_ID = 382,
     FLAGS_ID = 383,
     FLAG_TYPE = 384,
     MON_ATTITUDE = 385,
     MON_ALERTNESS = 386,
     SUBTYPE_ID = 387,
     NON_PASSDOOR_ID = 388,
     CARPET_ID = 389,
     CARPET_PIECE_ID = 390,
     CARPET_TYPE = 391,
     MON_APPEARANCE = 392,
     ROOMDOOR_ID = 393,
     IF_ID = 394,
     ELSE_ID = 395,
     TERRAIN_ID = 396,
     HORIZ_OR_VERT = 397,
     REPLACE_TERRAIN_ID = 398,
     LOCATION_SUBTYPE_ID = 399,
     DOOR_SUBTYPE = 400,
     BRAZIER_SUBTYPE = 401,
     SIGNPOST_SUBTYPE = 402,
     TREE_SUBTYPE = 403,
     FOREST_ID = 404,
     FOREST_TYPE = 405,
     INITIALIZE_TYPE = 406,
     EXIT_ID = 407,
     SHUFFLE_ID = 408,
     MANUAL_TYPE_ID = 409,
     MANUAL_TYPE = 410,
     QUANTITY_ID = 411,
     BURIED_ID = 412,
     LOOP_ID = 413,
     FOR_ID = 414,
     TO_ID = 415,
     SWITCH_ID = 416,
     CASE_ID = 417,
     BREAK_ID = 418,
     DEFAULT_ID = 419,
     ERODED_ID = 420,
     TRAPPED_STATE = 421,
     RECHARGED_ID = 422,
     INVIS_ID = 423,
     GREASED_ID = 424,
     INDESTRUCTIBLE_ID = 425,
     FEMALE_ID = 426,
     MALE_ID = 427,
     WAITFORU_ID = 428,
     PROTECTOR_ID = 429,
     CANCELLED_ID = 430,
     REVIVED_ID = 431,
     AVENGE_ID = 432,
     FLEEING_ID = 433,
     BLINDED_ID = 434,
     MAXHP_ID = 435,
     LEVEL_ADJUSTMENT_ID = 436,
     KEEP_ORIGINAL_INVENTORY_ID = 437,
     PARALYZED_ID = 438,
     STUNNED_ID = 439,
     CONFUSED_ID = 440,
     SEENTRAPS_ID = 441,
     ALL_ID = 442,
     MONTYPE_ID = 443,
     OBJTYPE_ID = 444,
     TERTYPE_ID = 445,
     TERTYPE2_ID = 446,
     LEVER_EFFECT_TYPE = 447,
     SWITCHABLE_ID = 448,
     CONTINUOUSLY_USABLE_ID = 449,
     TARGET_ID = 450,
     TRAPTYPE_ID = 451,
     EFFECT_FLAG_ID = 452,
     GRAVE_ID = 453,
     BRAZIER_ID = 454,
     SIGNPOST_ID = 455,
     TREE_ID = 456,
     ERODEPROOF_ID = 457,
     FUNCTION_ID = 458,
     MSG_OUTPUT_TYPE = 459,
     COMPARE_TYPE = 460,
     UNKNOWN_TYPE = 461,
     rect_ID = 462,
     fillrect_ID = 463,
     line_ID = 464,
     randline_ID = 465,
     grow_ID = 466,
     selection_ID = 467,
     flood_ID = 468,
     rndcoord_ID = 469,
     circle_ID = 470,
     ellipse_ID = 471,
     filter_ID = 472,
     complement_ID = 473,
     gradient_ID = 474,
     GRADIENT_TYPE = 475,
     LIMITED = 476,
     HUMIDITY_TYPE = 477,
     STRING = 478,
     MAP_ID = 479,
     NQSTRING = 480,
     VARSTRING = 481,
     CFUNC = 482,
     CFUNC_INT = 483,
     CFUNC_STR = 484,
     CFUNC_COORD = 485,
     CFUNC_REGION = 486,
     VARSTRING_INT = 487,
     VARSTRING_INT_ARRAY = 488,
     VARSTRING_STRING = 489,
     VARSTRING_STRING_ARRAY = 490,
     VARSTRING_VAR = 491,
     VARSTRING_VAR_ARRAY = 492,
     VARSTRING_COORD = 493,
     VARSTRING_COORD_ARRAY = 494,
     VARSTRING_REGION = 495,
     VARSTRING_REGION_ARRAY = 496,
     VARSTRING_MAPCHAR = 497,
     VARSTRING_MAPCHAR_ARRAY = 498,
     VARSTRING_MONST = 499,
     VARSTRING_MONST_ARRAY = 500,
     VARSTRING_OBJ = 501,
     VARSTRING_OBJ_ARRAY = 502,
     VARSTRING_SEL = 503,
     VARSTRING_SEL_ARRAY = 504,
     METHOD_INT = 505,
     METHOD_INT_ARRAY = 506,
     METHOD_STRING = 507,
     METHOD_STRING_ARRAY = 508,
     METHOD_VAR = 509,
     METHOD_VAR_ARRAY = 510,
     METHOD_COORD = 511,
     METHOD_COORD_ARRAY = 512,
     METHOD_REGION = 513,
     METHOD_REGION_ARRAY = 514,
     METHOD_MAPCHAR = 515,
     METHOD_MAPCHAR_ARRAY = 516,
     METHOD_MONST = 517,
     METHOD_MONST_ARRAY = 518,
     METHOD_OBJ = 519,
     METHOD_OBJ_ARRAY = 520,
     METHOD_SEL = 521,
     METHOD_SEL_ARRAY = 522,
     DICE = 523
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
#line 568 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 580 "lev_comp.tab.c"

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
#define YYNTOKENS  286
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  193
/* YYNRULES -- Number of rules.  */
#define YYNRULES  533
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1194

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   523

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   281,   285,     2,
     225,   226,   279,   277,   223,   278,   283,   280,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   224,     2,
       2,   282,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   227,     2,   228,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   229,   284,   230,     2,     2,     2,     2,
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
     215,   216,   217,   218,   219,   220,   221,   222,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276
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
    1329,  1337,  1343,  1351,  1357,  1367,  1375,  1381,  1391,  1397,
    1403,  1407,  1415,  1419,  1427,  1435,  1439,  1445,  1451,  1455,
    1461,  1469,  1479,  1481,  1483,  1485,  1487,  1489,  1490,  1493,
    1495,  1499,  1501,  1503,  1505,  1506,  1510,  1512,  1513,  1517,
    1519,  1520,  1524,  1525,  1529,  1530,  1534,  1536,  1538,  1540,
    1542,  1544,  1546,  1548,  1550,  1552,  1556,  1558,  1560,  1565,
    1567,  1569,  1574,  1576,  1578,  1583,  1585,  1590,  1596,  1598,
    1602,  1604,  1608,  1610,  1612,  1617,  1627,  1629,  1631,  1636,
    1638,  1644,  1646,  1648,  1653,  1655,  1657,  1663,  1665,  1667,
    1669,  1674,  1676,  1678,  1684,  1686,  1688,  1690,  1694,  1696,
    1698,  1702,  1704,  1709,  1713,  1717,  1721,  1725,  1729,  1733,
    1735,  1737,  1741,  1743,  1747,  1748,  1750,  1752,  1754,  1756,
    1760,  1761,  1763,  1765,  1768,  1771,  1776,  1783,  1788,  1795,
    1802,  1809,  1816,  1819,  1826,  1835,  1844,  1855,  1870,  1873,
    1875,  1879,  1881,  1885,  1887,  1889,  1891,  1893,  1895,  1897,
    1899,  1901,  1903,  1905,  1907,  1909,  1911,  1913,  1915,  1917,
    1919,  1921,  1923,  1934
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     287,     0,    -1,    -1,   288,    -1,   289,    -1,   289,   288,
      -1,   290,   300,   302,    -1,    17,   224,   231,    -1,    16,
     224,   231,   223,     3,    -1,    18,   224,    11,   223,   400,
      -1,    18,   224,    10,   223,     3,    -1,    18,   224,    13,
      -1,    18,   224,    12,   223,     3,   223,     3,   223,     5,
     223,     5,   223,   440,   223,   299,   298,    -1,    19,   224,
     470,    -1,   149,   224,   150,   223,   151,    -1,   149,   224,
     150,    -1,    32,   224,     4,   223,   312,    -1,    22,   224,
     103,    -1,    -1,   223,   221,    -1,    -1,   223,   447,    -1,
      -1,   223,     3,    -1,     5,    -1,    84,    -1,    -1,   128,
     224,   301,    -1,   129,   223,   301,    -1,   129,    -1,    -1,
     304,   302,    -1,   229,   302,   230,    -1,   345,    -1,   291,
      -1,   292,    -1,   408,    -1,   295,    -1,   293,    -1,   416,
      -1,   417,    -1,   418,    -1,   294,    -1,   415,    -1,   414,
      -1,   412,    -1,   413,    -1,   419,    -1,   420,    -1,   421,
      -1,   422,    -1,   423,    -1,   389,    -1,   347,    -1,   310,
      -1,   309,    -1,   403,    -1,   359,    -1,   381,    -1,   425,
      -1,   426,    -1,   391,    -1,   392,    -1,   393,    -1,   424,
      -1,   325,    -1,   335,    -1,   337,    -1,   341,    -1,   339,
      -1,   322,    -1,   332,    -1,   318,    -1,   321,    -1,   384,
      -1,   394,    -1,   366,    -1,   382,    -1,   369,    -1,   375,
      -1,   404,    -1,   399,    -1,   387,    -1,   346,    -1,   405,
      -1,   406,    -1,   407,    -1,   409,    -1,   352,    -1,   350,
      -1,   398,    -1,   402,    -1,   401,    -1,   385,    -1,   386,
      -1,   388,    -1,   380,    -1,   383,    -1,   241,    -1,   243,
      -1,   245,    -1,   247,    -1,   249,    -1,   251,    -1,   253,
      -1,   255,    -1,   257,    -1,   240,    -1,   242,    -1,   244,
      -1,   246,    -1,   248,    -1,   250,    -1,   252,    -1,   254,
      -1,   256,    -1,   305,    -1,   306,    -1,   234,    -1,   234,
      -1,   306,    -1,   153,   224,   305,    -1,   307,   282,   459,
      -1,   307,   282,   212,   224,   468,    -1,   307,   282,   458,
      -1,   307,   282,   475,   224,   452,    -1,   307,   282,   474,
     224,   454,    -1,   307,   282,   473,   224,   456,    -1,   307,
     282,   447,    -1,   307,   282,   450,    -1,   307,   282,   229,
     316,   230,    -1,   307,   282,   229,   315,   230,    -1,   307,
     282,   229,   314,   230,    -1,   307,   282,   475,   224,   229,
     313,   230,    -1,   307,   282,   474,   224,   229,   312,   230,
      -1,   307,   282,   473,   224,   229,   311,   230,    -1,   307,
     282,   229,   317,   230,    -1,   457,    -1,   311,   223,   457,
      -1,   455,    -1,   312,   223,   455,    -1,   453,    -1,   313,
     223,   453,    -1,   451,    -1,   314,   223,   451,    -1,   448,
      -1,   315,   223,   448,    -1,   459,    -1,   316,   223,   459,
      -1,   458,    -1,   317,   223,   458,    -1,    -1,    -1,   203,
     233,   225,   319,   463,   226,   320,   303,    -1,   233,   225,
     466,   226,    -1,   152,    -1,    -1,     6,    -1,     6,    -1,
     227,   459,   205,   459,   228,    -1,   227,   459,   228,    -1,
      -1,    -1,   161,   326,   227,   446,   228,   327,   229,   328,
     230,    -1,    -1,   329,   328,    -1,    -1,   162,   471,   224,
     330,   302,    -1,    -1,   164,   224,   331,   302,    -1,   163,
      -1,   283,   283,    -1,   160,    -1,   159,   308,   282,   459,
     333,   459,    -1,    -1,   334,   336,   303,    -1,    -1,   158,
     227,   446,   228,   338,   303,    -1,    -1,   324,   224,   340,
     304,    -1,    -1,   139,   324,   342,   343,    -1,   303,    -1,
      -1,   303,   344,   140,   303,    -1,    14,   224,   458,    -1,
      14,   224,   458,   223,    15,    -1,    74,    -1,    74,   224,
     471,    -1,    74,   224,    84,    -1,    62,   224,   348,   223,
     348,    -1,    62,   224,   348,   223,   471,    -1,   225,     4,
     223,    83,   223,   363,   226,    -1,   428,   323,   223,   440,
      -1,    -1,   126,   224,   349,   223,   356,   223,   358,   429,
     432,   434,   437,   438,   436,   351,   303,    -1,    -1,    54,
     224,   349,   223,   355,   223,   357,   223,   358,   429,   432,
     434,   437,   438,   436,   353,   303,    -1,    -1,   223,     5,
      -1,   225,     4,   223,     4,   226,    -1,    84,    -1,   225,
       4,   223,     4,   226,    -1,    84,    -1,   225,   367,   223,
     368,   226,    -1,    84,    -1,   225,     4,   223,     4,   226,
      -1,    84,    -1,   138,   224,   360,   223,   439,   223,   361,
     223,   363,   364,    -1,    30,   224,   439,   223,   468,   364,
      -1,     5,    -1,    84,    -1,   362,    -1,    84,    -1,    83,
      -1,    83,   284,   362,    -1,     4,    -1,    84,    -1,    -1,
     364,   223,   365,    -1,   144,   224,   145,    -1,    80,   224,
     456,    -1,   124,   224,     4,    -1,   170,    -1,   115,    -1,
     116,    -1,   133,    -1,    21,    -1,    20,   224,   367,   223,
     368,   354,   232,    -1,    20,   224,   447,   354,   232,    -1,
      88,    -1,    89,    -1,    90,    -1,    89,    -1,    28,   224,
     371,    -1,    -1,    28,   224,   371,   370,   303,    -1,   454,
     223,   447,   372,    -1,    -1,   372,   223,   373,    -1,   458,
      -1,   130,    -1,   131,    -1,   442,    -1,   137,   458,    -1,
     171,    -1,   172,    -1,   168,    -1,   175,    -1,   176,    -1,
     176,   224,   446,    -1,   177,    -1,   178,   224,   446,    -1,
     179,   224,   446,    -1,   183,   224,   446,    -1,   184,    -1,
     185,    -1,   186,   224,   374,    -1,   173,    -1,   174,    -1,
     180,    -1,   181,   224,   446,    -1,   182,    -1,   231,    -1,
     187,    -1,   231,   284,   374,    -1,    26,   224,   377,    -1,
      -1,    27,   224,   377,   376,   303,    -1,   456,   378,    -1,
      -1,   378,   223,   379,    -1,    77,    -1,   188,   224,   454,
      -1,   472,    -1,   127,   224,   458,    -1,   156,   224,   446,
      -1,   157,    -1,    76,    -1,   165,   224,   446,    -1,   202,
      -1,    75,    -1,   166,    -1,   167,   224,   446,    -1,   168,
      -1,   169,    -1,   447,    -1,   113,   224,   110,    -1,   112,
     224,   111,    -1,   114,   224,   446,    -1,   123,   224,   446,
      -1,   124,   224,   446,    -1,   154,   224,   155,    -1,   125,
     224,   446,    -1,    80,   224,   456,    -1,   170,    -1,   116,
      -1,    82,    -1,    78,    -1,   119,   224,   117,    -1,   120,
     224,   118,    -1,    61,   224,   446,    -1,   121,   224,   122,
      -1,    29,   224,   427,   223,   447,    -1,    31,   224,   447,
     223,    83,   223,   439,    -1,    36,   224,   447,   223,    83,
      -1,    36,   224,   447,   223,    83,   223,     5,   298,    -1,
      37,    -1,    37,   224,   468,    -1,    37,   224,   187,    -1,
      50,   224,   447,   223,    93,    -1,    51,   224,   447,   223,
      93,    -1,    51,   224,   477,   223,   477,   223,    93,    -1,
      56,   224,   477,   223,   477,   223,   231,    -1,    57,   224,
     477,   223,   477,   390,    -1,    58,   224,   477,   223,   477,
      -1,    -1,   223,    93,    -1,    65,   224,   468,   223,    97,
      -1,    66,   224,   468,    -1,    67,   224,   447,   223,   195,
     224,   447,   223,    95,   223,    94,    -1,    67,   224,   447,
     223,    68,   223,    69,   223,    70,   223,    95,   223,    94,
      -1,    81,   224,   447,   223,   192,   395,    -1,    -1,   395,
     223,   396,    -1,    94,    -1,   188,   224,   454,    -1,   189,
     224,   456,    -1,   190,   224,   400,    -1,   191,   224,   400,
      -1,   193,    -1,   194,    -1,   195,   224,   447,    -1,   196,
     224,   427,    -1,   144,   224,   397,    -1,   101,   224,   435,
      -1,   197,   224,     4,    -1,   124,   224,     4,    -1,     4,
      -1,    95,    -1,   145,    -1,    97,    -1,    72,   224,   468,
      -1,    71,   224,   468,    -1,     3,    -1,   225,     3,   223,
     440,   226,    -1,   143,   224,   450,   223,   452,   223,   452,
     223,     7,    -1,   141,   224,   468,   223,   452,    -1,   141,
     224,   468,   223,   452,   223,   100,    -1,    52,   224,   450,
      -1,    53,   224,   450,    -1,    42,   224,   231,   223,    43,
      -1,    42,   224,   231,   223,    43,   223,   188,   224,   454,
      -1,    39,   224,   450,   223,    41,    -1,    40,   224,   477,
     223,    41,    -1,    23,   224,   468,   223,    99,    -1,    -1,
      38,   224,   450,   223,   440,   223,   428,   429,   432,   434,
     437,   438,   436,   410,   411,    -1,    -1,   303,    -1,    47,
     224,   447,   223,   441,   223,   443,    -1,    47,   224,   447,
     223,   441,   223,   443,   223,   188,   224,   454,    -1,    47,
     224,   447,   223,   441,   223,   443,   223,    92,    -1,    47,
     224,   447,   223,   441,   223,   443,   223,    92,   223,   188,
     224,   454,    -1,    48,   224,   447,    -1,    48,   224,   447,
     223,   188,   224,   454,    -1,   105,   224,   447,   223,   106,
     223,     4,   223,   107,   223,   108,    -1,   105,   224,   447,
     223,   106,   223,     4,   223,   107,   223,   108,   223,   440,
      -1,   105,   224,   447,   223,   106,   223,   109,   223,   107,
      -1,   105,   224,   447,   223,   106,   223,   107,    -1,   102,
     224,   468,   223,   103,   223,   100,    -1,   132,   224,   468,
     223,     4,    -1,   132,   224,   468,   223,   103,   223,   100,
      -1,   134,   224,   450,   223,   136,    -1,   135,   224,   468,
     223,   136,   223,     4,   223,     4,    -1,   135,   224,   468,
     223,   136,   223,     4,    -1,    49,   224,    96,   223,   447,
      -1,    49,   224,    96,   223,   447,   223,   188,   224,   454,
      -1,   198,   224,   447,   223,   458,    -1,   198,   224,   447,
     223,    84,    -1,   198,   224,   447,    -1,   199,   224,   447,
     223,   146,   223,   440,    -1,   199,   224,   447,    -1,   200,
     224,   447,   223,   147,   223,   458,    -1,   200,   224,   447,
     223,   147,   223,    84,    -1,   200,   224,   447,    -1,   201,
     224,   447,   223,   148,    -1,   201,   224,   447,   223,   150,
      -1,   201,   224,   447,    -1,    63,   224,   459,   223,   447,
      -1,    64,   224,   447,   223,   476,   223,   458,    -1,    60,
     224,   446,   223,   446,   223,   446,   223,   446,    -1,    60,
      -1,   231,    -1,    84,    -1,   231,    -1,    84,    -1,    -1,
     223,   430,    -1,   431,    -1,   431,   223,   430,    -1,    44,
      -1,    45,    -1,    46,    -1,    -1,   104,   224,   433,    -1,
     103,    -1,    -1,   101,   224,   435,    -1,   100,    -1,    -1,
     188,   224,   454,    -1,    -1,    24,   224,    99,    -1,    -1,
      25,   224,     4,    -1,    75,    -1,    84,    -1,    76,    -1,
      84,    -1,    87,    -1,   444,    -1,    84,    -1,    87,    -1,
     444,    -1,    86,   224,    84,    -1,    91,    -1,    84,    -1,
      86,   227,     4,   228,    -1,   231,    -1,   242,    -1,   243,
     227,   459,   228,    -1,   459,    -1,   448,    -1,   214,   225,
     468,   226,    -1,   246,    -1,   247,   227,   459,   228,    -1,
     225,     4,   223,     4,   226,    -1,    84,    -1,    85,   449,
     228,    -1,   222,    -1,   222,   223,   449,    -1,   451,    -1,
     248,    -1,   249,   227,   459,   228,    -1,   225,     4,   223,
       4,   223,     4,   223,     4,   226,    -1,   453,    -1,   250,
      -1,   251,   227,   459,   228,    -1,     3,    -1,   225,     3,
     223,   440,   226,    -1,   455,    -1,   252,    -1,   253,   227,
     459,   228,    -1,   231,    -1,     3,    -1,   225,     3,   223,
     231,   226,    -1,    84,    -1,   457,    -1,   254,    -1,   255,
     227,   459,   228,    -1,   231,    -1,     3,    -1,   225,     3,
     223,   231,   226,    -1,    84,    -1,    98,    -1,   445,    -1,
     458,   283,   445,    -1,     4,    -1,   469,    -1,   225,     8,
     226,    -1,   240,    -1,   241,   227,   459,   228,    -1,   459,
     277,   459,    -1,   459,   278,   459,    -1,   459,   279,   459,
      -1,   459,   280,   459,    -1,   459,   281,   459,    -1,   225,
     459,   226,    -1,   236,    -1,   237,    -1,   307,   224,   460,
      -1,   461,    -1,   462,   223,   461,    -1,    -1,   462,    -1,
     459,    -1,   458,    -1,   464,    -1,   465,   223,   464,    -1,
      -1,   465,    -1,   447,    -1,   207,   450,    -1,   208,   450,
      -1,   209,   447,   223,   447,    -1,   210,   447,   223,   447,
     223,   459,    -1,   211,   225,   468,   226,    -1,   211,   225,
     362,   223,   468,   226,    -1,   217,   225,     7,   223,   468,
     226,    -1,   217,   225,   468,   223,   468,   226,    -1,   217,
     225,   452,   223,   468,   226,    -1,   213,   447,    -1,   215,
     225,   447,   223,   459,   226,    -1,   215,   225,   447,   223,
     459,   223,    44,   226,    -1,   216,   225,   447,   223,   459,
     223,   459,   226,    -1,   216,   225,   447,   223,   459,   223,
     459,   223,    44,   226,    -1,   219,   225,   220,   223,   225,
     459,   278,   459,   296,   226,   223,   447,   297,   226,    -1,
     218,   467,    -1,   256,    -1,   225,   468,   226,    -1,   467,
      -1,   467,   285,   468,    -1,   276,    -1,    99,    -1,     4,
      -1,     8,    -1,     9,    -1,     4,    -1,     8,    -1,     9,
      -1,     4,    -1,   469,    -1,    33,    -1,    26,    -1,    34,
      -1,    28,    -1,    35,    -1,   141,    -1,    79,    -1,    84,
      -1,   478,    -1,    59,   225,     4,   223,     4,   223,     4,
     223,     4,   226,    -1,   225,     4,   223,     4,   223,     4,
     223,     4,   226,    -1
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
    2391,  2397,  2401,  2407,  2413,  2417,  2423,  2427,  2433,  2437,
    2442,  2449,  2453,  2460,  2464,  2469,  2476,  2480,  2485,  2493,
    2499,  2506,  2510,  2517,  2525,  2528,  2538,  2542,  2545,  2551,
    2555,  2562,  2566,  2570,  2577,  2580,  2586,  2593,  2596,  2602,
    2609,  2613,  2620,  2623,  2630,  2633,  2639,  2640,  2643,  2644,
    2647,  2648,  2649,  2655,  2656,  2657,  2663,  2664,  2667,  2676,
    2681,  2688,  2699,  2705,  2709,  2713,  2720,  2730,  2737,  2741,
    2747,  2751,  2759,  2763,  2770,  2780,  2793,  2797,  2804,  2814,
    2823,  2834,  2838,  2845,  2855,  2866,  2875,  2885,  2891,  2895,
    2902,  2912,  2923,  2932,  2942,  2946,  2953,  2954,  2960,  2964,
    2968,  2972,  2980,  2989,  2993,  2997,  3001,  3005,  3009,  3012,
    3019,  3028,  3061,  3062,  3065,  3066,  3069,  3073,  3080,  3087,
    3098,  3101,  3109,  3113,  3117,  3121,  3125,  3130,  3134,  3138,
    3143,  3148,  3153,  3157,  3162,  3167,  3171,  3175,  3180,  3184,
    3191,  3197,  3201,  3207,  3214,  3215,  3218,  3219,  3220,  3223,
    3227,  3231,  3235,  3241,  3242,  3245,  3246,  3249,  3250,  3253,
    3254,  3257,  3261,  3287
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
  "ELEMENTAL_ENCHANTMENT_TYPE", "EXCEPTIONALITY_TYPE", "EXCEPTIONALITY_ID",
  "ELEMENTAL_ENCHANTMENT_ID", "ENCHANTMENT_ID", "SECRET_DOOR_ID",
  "USES_UP_KEY_ID", "MYTHIC_PREFIX_TYPE", "MYTHIC_SUFFIX_TYPE",
  "MYTHIC_PREFIX_ID", "MYTHIC_SUFFIX_ID", "MATERIAL_ID", "MATERIAL_TYPE",
  "CHARGES_ID", "SPECIAL_QUALITY_ID", "SPEFLAGS_ID", "SUBROOM_ID",
  "NAME_ID", "FLAGS_ID", "FLAG_TYPE", "MON_ATTITUDE", "MON_ALERTNESS",
  "SUBTYPE_ID", "NON_PASSDOOR_ID", "CARPET_ID", "CARPET_PIECE_ID",
  "CARPET_TYPE", "MON_APPEARANCE", "ROOMDOOR_ID", "IF_ID", "ELSE_ID",
  "TERRAIN_ID", "HORIZ_OR_VERT", "REPLACE_TERRAIN_ID",
  "LOCATION_SUBTYPE_ID", "DOOR_SUBTYPE", "BRAZIER_SUBTYPE",
  "SIGNPOST_SUBTYPE", "TREE_SUBTYPE", "FOREST_ID", "FOREST_TYPE",
  "INITIALIZE_TYPE", "EXIT_ID", "SHUFFLE_ID", "MANUAL_TYPE_ID",
  "MANUAL_TYPE", "QUANTITY_ID", "BURIED_ID", "LOOP_ID", "FOR_ID", "TO_ID",
  "SWITCH_ID", "CASE_ID", "BREAK_ID", "DEFAULT_ID", "ERODED_ID",
  "TRAPPED_STATE", "RECHARGED_ID", "INVIS_ID", "GREASED_ID",
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
     475,   476,   477,    44,    58,    40,    41,    91,    93,   123,
     125,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,   523,    43,    45,    42,
      47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   286,   287,   287,   288,   288,   289,   290,   290,   291,
     291,   291,   291,   292,   293,   293,   294,   295,   296,   296,
     297,   297,   298,   298,   299,   299,   300,   300,   301,   301,
     302,   302,   303,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   307,   307,   307,   308,
     308,   309,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   311,   311,   312,
     312,   313,   313,   314,   314,   315,   315,   316,   316,   317,
     317,   319,   320,   318,   321,   322,   323,   323,   324,   324,
     324,   326,   327,   325,   328,   328,   330,   329,   331,   329,
     332,   333,   333,   334,   336,   335,   338,   337,   340,   339,
     342,   341,   343,   344,   343,   345,   345,   346,   346,   346,
     347,   347,   348,   349,   351,   350,   353,   352,   354,   354,
     355,   355,   356,   356,   357,   357,   358,   358,   359,   359,
     360,   360,   361,   361,   362,   362,   363,   363,   364,   364,
     365,   365,   365,   365,   365,   365,   365,   366,   366,   366,
     367,   367,   368,   368,   369,   370,   369,   371,   372,   372,
     373,   373,   373,   373,   373,   373,   373,   373,   373,   373,
     373,   373,   373,   373,   373,   373,   373,   373,   373,   373,
     373,   373,   373,   374,   374,   374,   375,   376,   375,   377,
     378,   378,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   379,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   379,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   379,   379,   380,   381,   382,   382,   383,   383,   383,
     384,   385,   386,   387,   388,   389,   390,   390,   391,   392,
     393,   393,   394,   395,   395,   396,   396,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   396,   396,   397,   397,
     397,   397,   398,   399,   400,   400,   401,   402,   402,   403,
     404,   405,   405,   406,   407,   408,   410,   409,   411,   411,
     412,   412,   412,   412,   413,   413,   414,   414,   414,   414,
     415,   416,   416,   417,   418,   418,   419,   419,   420,   420,
     420,   421,   421,   422,   422,   422,   423,   423,   423,   424,
     425,   426,   426,   427,   427,   428,   428,   429,   429,   430,
     430,   431,   431,   431,   432,   432,   433,   434,   434,   435,
     436,   436,   437,   437,   438,   438,   439,   439,   440,   440,
     441,   441,   441,   442,   442,   442,   443,   443,   444,   445,
     445,   445,   446,   447,   447,   447,   447,   448,   448,   448,
     449,   449,   450,   450,   450,   451,   452,   452,   452,   453,
     453,   454,   454,   454,   455,   455,   455,   455,   456,   456,
     456,   457,   457,   457,   457,   457,   458,   458,   459,   459,
     459,   459,   459,   459,   459,   459,   459,   459,   459,   460,
     460,   461,   462,   462,   463,   463,   464,   464,   465,   465,
     466,   466,   467,   467,   467,   467,   467,   467,   467,   467,
     467,   467,   467,   467,   467,   467,   467,   467,   467,   467,
     467,   468,   468,   469,   470,   470,   471,   471,   471,   472,
     472,   472,   472,   473,   473,   474,   474,   475,   475,   476,
     476,   477,   477,   478
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
       7,    11,     9,    13,     3,     7,    11,    13,     9,     7,
       7,     5,     7,     5,     9,     7,     5,     9,     5,     5,
       3,     7,     3,     7,     7,     3,     5,     5,     3,     5,
       7,     9,     1,     1,     1,     1,     1,     0,     2,     1,
       3,     1,     1,     1,     0,     3,     1,     0,     3,     1,
       0,     3,     0,     3,     0,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     4,     1,
       1,     4,     1,     1,     4,     1,     4,     5,     1,     3,
       1,     3,     1,     1,     4,     9,     1,     1,     4,     1,
       5,     1,     1,     4,     1,     1,     5,     1,     1,     1,
       4,     1,     1,     5,     1,     1,     1,     3,     1,     1,
       3,     1,     4,     3,     3,     3,     3,     3,     3,     1,
       1,     3,     1,     3,     0,     1,     1,     1,     1,     3,
       0,     1,     1,     2,     2,     4,     6,     4,     6,     6,
       6,     6,     2,     6,     8,     8,    10,    14,     2,     1,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,    10,     9
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
       0,     0,     0,     0,     0,     0,     0,     0,   392,     0,
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
     468,     0,   471,     0,   513,     0,   469,   490,    31,     0,
     178,     0,     8,     0,   429,   430,     0,   466,   185,     0,
       0,     0,    11,   515,   514,    13,   438,     0,   230,   231,
       0,     0,   435,     0,     0,   198,   433,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     509,   492,   511,     0,   462,   464,   465,     0,   461,   459,
       0,   266,   270,   458,   267,   455,   457,     0,   454,   452,
       0,   234,     0,   451,   394,   393,     0,   416,   417,     0,
       0,     0,     0,   309,   308,     0,   443,     0,     0,   442,
       0,     0,     0,     0,   531,     0,     0,   364,     0,     0,
       0,     0,     0,   349,   350,   396,   395,     0,   156,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,   319,
       0,   343,   342,   518,   516,   517,   189,   188,     0,     0,
       0,     0,     0,     0,     0,   210,   211,     0,     0,     0,
       0,    15,   121,     0,     0,     0,   380,   382,   385,   388,
     151,     0,     0,     0,     0,   160,     0,     0,     0,     0,
       0,   487,   486,   488,   491,     0,   524,   526,   523,   525,
     527,   528,     0,     0,     0,   128,   129,   124,   122,     0,
       0,     0,     0,    30,   175,    28,     0,     0,     0,     0,
       0,     0,   440,     0,     0,     0,     0,     0,     0,     0,
     493,   494,     0,     0,     0,   502,     0,     0,     0,   508,
       0,     0,     0,     0,     0,     0,   269,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   157,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   182,   181,     0,     0,     0,   176,     0,
       0,     0,     0,     0,     0,   484,   470,   478,     0,     0,
     473,   474,   475,   476,   477,     0,   154,     0,   468,     0,
       0,     0,     0,   145,   143,   149,   147,     0,     0,     0,
     179,     0,     0,   186,   467,    10,   344,     0,     9,     0,
       0,   439,     0,     0,     0,   233,   232,   198,   199,   229,
       0,     0,   214,     0,     0,     0,     0,   449,     0,     0,
     447,     0,     0,   446,     0,     0,   510,   512,   355,     0,
       0,     0,   268,     0,     0,   236,   238,   303,   218,     0,
      16,   139,   305,     0,     0,   418,   419,     0,   353,     0,
       0,   354,   351,   422,     0,   420,     0,   421,     0,   376,
     310,     0,   311,     0,   201,     0,     0,     0,     0,   316,
     315,     0,     0,   190,   191,   389,   529,   530,     0,   318,
       0,     0,   323,     0,     0,   203,     0,     0,   371,     0,
     373,     0,     0,     0,     0,   347,     0,    14,     0,   172,
       0,     0,   162,   379,   378,     0,     0,   386,   387,     0,
     482,   485,     0,   472,   159,   489,   123,     0,     0,   132,
       0,   131,     0,   130,     0,   136,     0,   127,     0,   126,
       0,   125,    32,   431,     0,     0,   441,   434,     0,   436,
       0,   495,     0,     0,     0,   497,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   460,   521,   519,   520,     0,
     281,   278,   272,   298,     0,   297,     0,     0,     0,   296,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,   282,     0,   284,   285,   295,     0,   280,   271,   286,
     522,   274,     0,   453,   237,   209,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   193,     0,     0,   314,     0,     0,     0,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   177,   171,   173,     0,     0,     0,     0,
       0,   152,     0,   144,   146,   148,   150,     0,   137,     0,
       0,   141,     0,     0,   437,   228,     0,   215,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   456,     0,     0,   304,   140,
      22,     0,   397,     0,     0,     0,     0,   427,   426,   360,
     365,     0,   312,     0,   205,     0,     0,   313,   317,     0,
       0,   390,     0,     0,     0,   370,     0,   369,     0,     0,
     207,     0,   397,   372,   375,   213,     0,   212,   184,   348,
       0,   164,   381,   384,   383,   479,   480,   481,   483,     0,
       0,   135,   134,     0,   133,     0,     0,   496,   498,     0,
     503,     0,   499,     0,   448,   501,   500,     0,   301,   294,
     288,   287,   289,   299,   300,   302,   290,   291,   293,   275,
     292,   276,   279,   283,   273,     0,   423,   241,   242,     0,
     247,   245,   246,   258,   259,   248,   249,   251,     0,     0,
     260,     0,   262,     0,   255,   256,     0,   239,   243,   424,
     240,     0,   224,   225,     0,   226,     0,   223,   219,     0,
     306,     0,     0,   404,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,   216,   217,     0,     0,     0,   325,
       0,     0,     0,     0,     0,     0,     0,   330,   331,     0,
       0,     0,   324,     0,     0,     0,     0,   404,     0,     0,
       0,     0,     0,     0,   164,   153,   138,   142,   345,     0,
       0,     0,   450,     0,     0,   244,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,     0,   401,   402,   403,
     398,   399,     0,   407,     0,     0,   352,   362,     0,   377,
     200,     0,   397,   391,   192,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   368,   202,
       0,   407,   374,   218,   346,     0,   168,   163,   165,     0,
     504,     0,   505,   474,   425,   250,   252,   253,   261,   254,
     264,   263,   257,   221,   222,   220,     0,     0,     0,     0,
     412,     0,     0,     0,     0,     0,   404,     0,     0,   409,
     335,   337,   338,   339,   341,   340,   334,   326,   327,   328,
     329,   332,   333,   336,     0,     0,   412,   208,   166,    30,
       0,     0,     0,     0,     0,   445,   400,   406,   405,     0,
       0,   414,     0,   533,     0,   361,   204,   407,     0,   320,
     366,   206,   414,    30,   169,     0,   506,    19,     0,   265,
     408,     0,     0,   410,   532,     0,   412,     0,     0,   410,
     167,     0,     0,   413,     0,     0,   356,   363,   414,   321,
     367,   194,     0,    20,   415,     0,   358,   410,     0,    24,
      25,    22,     0,     0,   411,   359,   357,   196,   195,    12,
      21,   507,     0,   197
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,   102,   103,   104,   105,   106,
    1123,  1183,   960,  1181,    12,   176,   107,   424,   108,   109,
     110,   111,   233,   112,   113,   797,   590,   800,   529,   530,
     531,   532,   114,   515,   889,   115,   116,   484,   117,   118,
     234,   786,  1003,  1004,  1143,  1119,   119,   651,   120,   121,
     251,   122,   648,   123,   422,   124,   378,   504,   643,   125,
     126,   127,   355,   347,   128,  1178,   129,  1192,   439,   616,
     637,   856,   872,   130,   377,   876,   563,   976,   745,   958,
     131,   274,   557,   132,   460,   311,   744,   947,  1082,   133,
     457,   301,   456,   738,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   766,   144,   145,   146,   147,   772,
     992,  1106,   148,   149,   548,   150,   151,   152,   153,   154,
     155,   156,   157,   158,  1176,  1186,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   316,   348,   963,  1030,  1031,  1033,  1128,  1090,  1100,
    1166,  1131,  1153,   319,   597,   606,   948,   849,   607,   257,
     352,   291,   276,   433,   328,   329,   572,   573,   312,   313,
     302,   303,   401,   353,   887,   660,   661,   662,   403,   404,
     405,   292,   451,   246,   265,   367,   741,   419,   420,   421,
     628,   333,   334
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1051
static const yytype_int16 yypact[] =
{
     165,  -116,  -111,   145, -1051,   165,    -5,   -65,   -59, -1051,
   -1051,    46,   814,    14, -1051,   136, -1051,    79,    90,   111,
     115, -1051,   117,   128,   137,   161,   170,   195,   196,   197,
     202,   204,   205,   209,   211,   213,   214,   221,   224,   230,
     237,   239,   240,   241,   243,   244,   245,   246,   264,   269,
     273,   276,   277,   278,   280,   281,   287,   288,   290,   291,
     292,   293,   295,   297,   299,   301,    66,   305,   307,   312,
   -1051,   318,    57,   561, -1051, -1051,   322,   324,   325,   326,
      20,    37,    75, -1051, -1051, -1051, -1051, -1051, -1051, -1051,
   -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051,
   -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051,   814, -1051,
   -1051,    64, -1051, -1051, -1051, -1051, -1051,   328, -1051, -1051,
   -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051,
   -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051,
   -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051,
   -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051,
   -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051,
   -1051, -1051, -1051, -1051,   393,   105, -1051,  -112,   402,    62,
     362,   327,   885,    94,    94,   172,   -51,    41,   -30,   449,
     -30,   866,  -110,  -110,   -20,   335,   -30,   -30,   426,   -30,
     347,  -110,  -110,   -38,   -20,   -20,   -20,    37,   333,    37,
     -30,   885,   885,   -30,   885,   885,    85,   -30,   885,   -30,
     -38,   885,  -110,   885,    70, -1051,   885,  -110,   403,   743,
      37, -1051, -1051,   285,   332,   -30,   -30,   -30,   -30,   340,
   -1051,    19, -1051,   342, -1051,   206, -1051,   102, -1051,   122,
   -1051,   344, -1051,   136, -1051, -1051,   351, -1051,  -124,   365,
     366,   367, -1051, -1051, -1051, -1051, -1051,   352, -1051, -1051,
     360,   582, -1051,   376,   384,   387, -1051, -1051,  -110,  -110,
     -30,   -30,   394,   -30,   397,   398,   401,   885,   404,   552,
   -1051, -1051,   345,   415, -1051, -1051, -1051,   624, -1051, -1051,
     424, -1051, -1051, -1051, -1051, -1051, -1051,   649, -1051, -1051,
     428,   427,   434, -1051, -1051, -1051,   436, -1051, -1051,   440,
     441,   456,   457, -1051, -1051,   677, -1051,   472,   460, -1051,
     477,   482,   697,   479, -1051,   488,   489,   490,   491,   498,
     718,   500,   503, -1051, -1051, -1051, -1051,   504,   722,   508,
     509,   510,   513,   363,   733,   515,   163,   516,   518, -1051,
     519, -1051, -1051, -1051, -1051, -1051, -1051, -1051,   520,   521,
     522,   529,   541,   553,   560, -1051, -1051,   571,   344,   573,
     574,   581, -1051,   547,    37,    37,   583,   587,   589,   591,
   -1051,   592,   179,    37,    37, -1051,    37,    37,    37,    37,
      37,   536,   363, -1051,   598,   596, -1051, -1051, -1051, -1051,
   -1051, -1051,   599,   132,    33, -1051, -1051,   536,   363,   600,
     602,   603,   814,   814, -1051, -1051,    37,   815,  -112,   826,
      25,   828,   615,   611,   885,   625,    37,   123,   842,   617,
   -1051, -1051,   634,   635,   689, -1051,   -30,   -30,   459, -1051,
     639,   629,   885,   770,   659,    37,   660,   344,   661,    37,
     344,   -30,   -30,   885,   804,    21,   806,   667,    37,    58,
     850,   888,   670,   853,   858,   272,   727,   -30,   824,   695,
     827,   -20,   -42, -1051,   698,   -20,   -20,   -20,    37,   704,
      44,   -30,   107,   831,    -8,   738,   829,   825,   -22,    76,
     797,   798,    41,   799, -1051,   104,   104,   787, -1051,   -49,
     713,   -41,   796,   800,    -1,   902, -1051, -1051,   369,   470,
      95,    95, -1051, -1051, -1051,   102, -1051,   885,   720,   -95,
     -86,   -53,   -19, -1051, -1051,   536,   363,    26,   164,    60,
   -1051,   714,   476, -1051, -1051, -1051, -1051,   951, -1051,   735,
     352, -1051,   730,   955,   501, -1051, -1051,   387, -1051, -1051,
     -30,   -30,   676,   739,   742,   741,   748, -1051,   751,   478,
   -1051,   734,   753, -1051,   762,   764, -1051, -1051, -1051,   758,
     507,   414, -1051,   760,   632, -1051, -1051, -1051, -1051,   772,
     774, -1051,   776,   961,   645, -1051, -1051,   778, -1051,   779,
     989, -1051,   781, -1051,   780, -1051,   782, -1051,   784,   783,
   -1051,  1005, -1051,   793, -1051,  1006,   795,    58,   801,   802,
   -1051,   803,   936, -1051, -1051, -1051, -1051, -1051,   805, -1051,
     807,   808, -1051,   810,   811, -1051,  1016,   813, -1051,   816,
   -1051,   817,   819,   881,  1019,   820,   821, -1051,   344, -1051,
     740,    37, -1051, -1051,   536,   822,   823, -1051, -1051,   848,
   -1051,   855,   812, -1051, -1051, -1051, -1051,  1023,   861, -1051,
      -2, -1051,    37, -1051,  -112, -1051,    16, -1051,    21, -1051,
      29, -1051, -1051, -1051,   864,  1026, -1051, -1051,   862, -1051,
     818, -1051,   874,   948,   885, -1051,    37,    37,   885,   882,
      37,   885,   885,   883,   880, -1051, -1051, -1051, -1051,   887,
   -1051, -1051, -1051, -1051,   890, -1051,   891,   892,   893, -1051,
     894,   895,   896,   897,   899,   900,   901,   903,   904, -1051,
     905, -1051,   906, -1051, -1051, -1051,   911, -1051, -1051, -1051,
   -1051, -1051,   912, -1051,   884,   886,    41,    21,  1032,   914,
   -1051,   -38,  1045,   916,   863,  1048,   131,   172,   938,    13,
    1067,   939,   -11, -1051,   930,  1070, -1051,    37,   941,  -112,
    1096,   -30,   943,  1068,    87,   944,    -7,  1069,  1166,   304,
     344,  1072,   104, -1051, -1051,   363,   962,    58,   -23,   130,
     902, -1051,    73, -1051, -1051,   363,   536,     3, -1051,    12,
      15, -1051,    58,   950, -1051, -1051,    37, -1051,   964,   229,
     354,   966,    58,   701,   967,   968,    37, -1051,    37,    94,
    1084,  1086,    37,  1080,  1081,  1076,    37,    37,    37,  -112,
    1046,    37,    37,    37,   172, -1051,  1003,   538, -1051, -1051,
     977,  1198,   980,   981,  1201,   982,   979, -1051, -1051,   985,
   -1051,   986, -1051,  1205, -1051,   302,   988, -1051, -1051,   990,
     125,   536,   991,   992,   283, -1051,   993, -1051,   994,  1208,
   -1051,  1214,   980, -1051,   996, -1051,   997, -1051, -1051, -1051,
     998,   124, -1051, -1051,   536, -1051, -1051, -1051, -1051,   344,
      16, -1051, -1051,    29, -1051,   999,  1217,   363, -1051,  1179,
   -1051,    37, -1051,  1000, -1051, -1051, -1051,   439, -1051, -1051,
   -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051,   536,
   -1051, -1051, -1051, -1051, -1051,    -3, -1051, -1051, -1051,  -112,
   -1051, -1051, -1051, -1051, -1051, -1051,  1004, -1051,  1007,  1008,
   -1051,  1009, -1051,  1011, -1051, -1051,  1012, -1051, -1051, -1051,
     536,  1013, -1051, -1051,  1014, -1051,  1015, -1051, -1051,  1221,
   -1051,  1017,   355,  1123,  1225,  1018,   172, -1051,     0,   172,
    1021,  1020,    -7,    37, -1051, -1051,  1022,  1160,  1147, -1051,
    1025,  1027,  1028,  1029,  1030,  1031,  1033, -1051, -1051,  1034,
    1035,  1036, -1051,  1137,  1143,  1037,  1038,  1123,  1252,   125,
    1255,   298,  1040,  1039,   124, -1051, -1051, -1051, -1051,  1042,
    1041,   334, -1051,    37,  1182,   536,    37,    37,    37,    37,
      37,   -99,    94,  1264,  1125, -1051,  1267, -1051, -1051, -1051,
   -1051,  1049,  1050,  1172,  1052,  1272, -1051,  1054,  1055, -1051,
   -1051,   123,   980, -1051, -1051,  1057,  1058,  1178,  1278,    65,
     172,    94,    25,    25,   -30,   -51,  1279,  1061, -1051, -1051,
    1281,  1172, -1051, -1051, -1051,  1062, -1051, -1051, -1051,  1282,
   -1051,  1244, -1051,  -128, -1051, -1051, -1051, -1051, -1051, -1051,
   -1051,  1010, -1051, -1051, -1051, -1051,  1063,   355,  1187,  1071,
    1268,  1287,  1073,  1105,   172,  1074,  1123,  1202,  1204, -1051,
   -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051,
   -1051, -1051, -1051, -1051,  1188,  1075,  1268,   886, -1051,   814,
    1079,  1077,  1083,  1082,   -99, -1051, -1051, -1051, -1051,  1178,
    1085,  1280,  1087, -1051,  1088, -1051, -1051,  1172,  1091, -1051,
    1092, -1051,  1280,   814, -1051,    58, -1051, -1051,  1093, -1051,
   -1051,  1207,  1094,  1119, -1051,   172,  1268,  1216,    58,  1119,
   -1051,  1097,   -30, -1051,  1307,  1095, -1051, -1051,  1280, -1051,
   -1051, -1051,    80,  1098, -1051,   172,   344,  1119,   344, -1051,
   -1051,   977,   -30,  1099, -1051, -1051, -1051, -1051, -1051, -1051,
   -1051, -1051,   344, -1051
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1051, -1051,  1312, -1051, -1051, -1051, -1051, -1051, -1051, -1051,
   -1051, -1051,   141, -1051, -1051,  1078,  -108,  -376,   907,  1101,
    1250,  -499, -1051, -1051, -1051, -1051,   646, -1051, -1051, -1051,
   -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051,  1260, -1051,
   -1051, -1051,   323, -1051, -1051, -1051, -1051, -1051, -1051, -1051,
   -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051,
   -1051, -1051,   838,  1112, -1051, -1051, -1051, -1051,   777, -1051,
   -1051, -1051,   361, -1051, -1051, -1051,  -603,   336,   274, -1051,
   -1051,   481,   300, -1051, -1051, -1051, -1051, -1051,   215, -1051,
   -1051,  1154, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051,
   -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051,
   -1051, -1051, -1051, -1051,  -643, -1051, -1051, -1051, -1051, -1051,
   -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051,
   -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051, -1051,
   -1051,   289,   594,  -842,   253, -1051,  -911, -1051,  -974,   217,
   -1050, -1013, -1046,  -480,  -613, -1051, -1051, -1051,   506,   915,
    -227,  -179,  -399,   809,   -81,  -389,  -492,  -644,  -530,  -453,
    -527,  -650,  -171,   -76, -1051,   557, -1051, -1051,   830, -1051,
   -1051,  1064,  -147,   767, -1051,  -483, -1051, -1051, -1051, -1051,
   -1051,  -155, -1051
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -236
static const yytype_int16 yytable[] =
{
     248,   275,   503,   383,   763,   245,   258,   624,   679,   320,
     677,   322,   591,   645,   646,   533,   659,   336,   337,   294,
     339,   341,   642,   240,   305,   534,   798,   391,   546,   294,
     997,   357,   567,   314,   360,   293,   801,   240,   368,   331,
     370,   240,   614,   653,   324,   342,   345,   681,   363,   349,
     350,   351,   364,   365,   266,   267,   386,   387,   388,   389,
     630,   883,   635,   567,   358,   359,   263,   361,   362,  1102,
     415,   369,    16,   854,   372,   375,   374,   870,   417,   379,
     638,   582,   266,   267,   585,  1179,  1061,  1116,  1080,   363,
     807,   866,  1037,   364,   365,  1122,  1159,   294,   -18,   427,
     295,   442,   443,  1142,   445,   306,   240,   567,     7,  1171,
     295,   649,   330,     8,   296,   325,   317,   266,   267,   254,
     343,   344,  1177,    11,   296,   318,   240,  1187,   668,   974,
     255,   256,  1081,   356,   595,   669,   528,   670,   326,   327,
     391,   373,   596,  1168,   671,     9,   380,   657,   406,   658,
     407,   398,   399,   400,   376,   408,   409,   410,   510,   428,
    1103,   264,  1104,  1156,  1180,   392,    13,   305,   416,   366,
     672,   402,    14,   418,   882,   305,   877,   673,   295,   639,
     315,     1,     2,   615,   270,  1137,   626,   631,  1038,   895,
     254,   627,   296,   346,   867,   271,   868,   440,   441,   903,
    1096,   255,   256,   636,   674,   332,   266,   267,   254,   975,
    1105,   675,   555,   556,   855,   847,   272,   273,   871,   255,
     256,  1014,   848,   271,   755,   591,   890,   850,   396,   397,
     398,   399,   400,   891,   650,   747,   844,   174,   893,   804,
    1006,   297,   892,   535,   241,   894,   307,   298,   306,  1007,
     547,   297,   308,   239,   644,   676,   306,   298,   413,   242,
     243,   621,   241,   411,   254,   175,   838,   565,   566,   354,
      15,   794,   783,   242,   243,   255,   256,   242,   243,   793,
     299,   300,   586,   587,   230,   644,  1001,   552,  1002,   680,
     880,   659,   909,    81,   839,   244,   841,   564,   609,   804,
     247,   574,   363,   177,   924,   577,   364,   365,   509,   244,
     570,   571,   625,   244,   178,   541,   588,   518,   519,   297,
     520,   521,   522,   523,   524,   298,   613,   241,   253,   644,
     618,   619,   620,   254,   412,   179,   270,   392,   536,   180,
     654,   181,   242,   243,   255,   256,   249,   413,   299,   300,
     542,   414,   182,   254,   570,   571,   603,   241,   604,   605,
     554,   183,   242,   243,   255,   256,   885,   886,   272,   273,
     326,   327,   242,   243,   398,   399,   400,   979,   244,   580,
     666,   691,   692,   584,   980,   184,   491,   562,   875,   307,
     268,   269,   594,   678,   185,   308,   252,   307,   244,  1027,
    1028,  1029,   739,   308,   878,   517,   331,   981,   244,  1109,
    1110,   394,   259,   260,   261,   262,   309,   310,   706,   186,
     187,   188,   707,   708,   309,   310,   189,   982,   190,   191,
     277,   266,   267,   192,   395,   193,  1036,   194,   195,  1039,
     396,   397,   398,   399,   400,   196,   266,   267,   197,   402,
     268,   269,   899,   321,   198,   900,   396,   397,   398,   399,
     400,   199,   567,   200,   201,   202,   568,   203,   204,   205,
     206,   983,   984,   985,   986,   709,   987,   988,   989,   990,
     991,   699,   435,   396,   397,   398,   399,   400,   207,   710,
     711,   712,   713,   208,   714,  1083,   715,   209,   266,   267,
     210,   211,   212,   796,   213,   214,   396,   397,   398,   399,
     400,   215,   216,  1005,   217,   218,   219,   220,  1065,   221,
    1107,   222,   338,   223,  1108,   224,   716,   717,   718,   226,
     719,   227,  1161,   720,   721,   722,   228,   723,   724,   725,
     859,   726,   229,   266,   267,  1170,   235,   808,   236,   237,
     238,   811,   250,   381,   814,   815,   435,  1071,   354,   385,
    1072,   270,   266,   267,  1135,   390,   335,   384,   727,   393,
     728,   729,   340,   423,   432,   785,   270,   901,   426,   730,
     731,   732,   733,   734,   735,   434,   435,   271,   429,   430,
     431,   908,   863,   272,   273,   912,   795,   663,   861,   916,
     917,   918,   736,   436,   921,   922,   923,   437,   272,   273,
     438,   396,   397,   398,   399,   400,   737,   884,   951,   444,
     809,   810,   446,   447,   813,  1167,   448,   454,   270,   450,
     452,   396,   397,   398,   399,   400,   266,   267,   453,   271,
     396,   397,   398,   399,   400,  1184,   396,   397,   398,   399,
     400,   455,   458,   952,   953,   459,  -235,   461,   919,   462,
     272,   273,   954,   463,   464,   950,   278,   279,   280,   281,
     282,   955,   283,   270,   284,   285,   286,   287,   288,   465,
     466,   467,   956,   469,   569,   278,   279,   280,   281,   282,
     244,   283,   270,   284,   285,   286,   287,   288,   664,   468,
     470,   472,   473,   289,   683,   272,   273,   471,   957,   570,
     571,   474,   475,   476,   477,   290,   396,  1013,   398,   399,
     400,   478,   479,   480,   272,   273,   481,   482,   483,   689,
     897,   485,   486,   487,   290,   705,   488,   489,   490,   492,
     907,   493,   494,   495,   496,   497,  1043,   396,   397,   398,
     399,   400,   498,   396,   397,   398,   399,   400,  1015,   278,
     279,   280,   281,   282,   499,   283,   270,   284,   285,   286,
     287,   288,   562,   266,   267,   508,   500,   289,   396,   397,
     398,   399,   400,   501,   396,   397,   398,   399,   400,  1075,
    1076,  1077,  1078,  1079,   502,   231,   505,   506,   272,   273,
    1185,    84,  1188,    86,   507,    88,   511,    90,   290,    92,
     512,    94,   513,    96,   514,    98,  1193,   100,   516,   428,
      16,   525,   526,   527,   537,  1011,   538,   539,    17,   545,
     543,   549,    18,    19,    20,    21,    22,    23,   550,   551,
      24,    25,    26,    27,    28,    29,    30,   558,   553,   559,
      31,    32,    33,    34,    35,   576,    36,   560,   561,   575,
     743,    37,    38,    39,    40,    41,    42,    43,    44,   578,
      45,    46,    47,   750,    48,  1111,    49,    50,    51,    52,
      53,    54,   579,   581,   583,    55,    56,   589,    57,   592,
     593,   598,   599,   600,   601,    58,   278,   279,   280,   281,
     282,   602,   283,   270,   284,   285,   286,   287,   288,   396,
     397,   398,   399,   400,   289,   608,    59,   610,   611,    60,
     612,   617,   396,   397,   398,   399,   400,   622,   629,   904,
     632,   634,   633,   640,   641,   272,   273,  1073,   647,  -183,
      61,   652,   655,   667,   682,   290,    62,   656,    63,    64,
     266,   267,    65,    66,   684,    67,   687,    68,   685,   688,
     693,   700,   694,    69,   696,   749,    70,    71,   695,   266,
     267,   697,    72,    73,   698,    74,   701,    75,   396,   397,
     398,   399,   400,  1173,    85,   702,    87,   703,    89,   704,
      91,   742,    93,   753,    95,   746,    97,   747,    99,   748,
     101,   751,   752,  1190,   754,   756,   758,   755,   757,   759,
     761,  1144,    76,    77,    78,    79,   760,    80,   762,   768,
     775,   780,   699,   784,   764,   765,   767,   792,   769,   803,
     770,   562,   771,   773,   774,  1160,   776,   840,   791,   777,
     778,    81,   779,   781,   782,   787,   788,    82,    83,   843,
     805,   845,   846,   323,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   789,   278,   279,   280,   281,   282,   790,   283,
     270,   284,   285,   286,   287,   288,   325,   802,   804,   925,
     926,   289,   278,   279,   280,   281,   282,   806,   283,   270,
     284,   285,   286,   287,   288,   812,   817,   836,   816,   837,
     289,   818,   272,   273,   819,   820,   821,   822,   823,   824,
     825,   826,   290,   827,   828,   829,   851,   830,   831,   832,
     833,   272,   273,   927,   928,   834,    83,   841,   835,   844,
     929,   290,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     852,   857,   853,   858,   860,   862,   864,   869,   865,   873,
     874,   930,   879,   896,   931,   932,   933,   934,   935,   936,
     937,   938,   939,   940,   941,   942,   943,   944,   945,   946,
     898,   881,   902,   905,   906,   910,   911,   913,   915,   914,
     959,   920,   961,   962,   964,   965,   966,   967,   968,   970,
     969,   972,   995,   973,   977,   978,   993,   994,   996,   998,
     999,  1000,  1009,  1010,  1025,  1008,  1012,  1032,  1016,  1034,
    1045,  1017,  1018,  1019,   254,  1020,  1021,  1022,  1023,  1024,
    1026,  1035,  1046,  1041,  1057,   255,   256,  1040,  1044,  1047,
    1058,  1048,  1049,  1050,  1051,  1052,  1062,  1053,  1054,  1055,
    1056,  1060,  1064,  1059,  1066,  1069,  1074,  1070,  1084,  1067,
    1085,  1086,  1087,  1089,  1088,  1091,  1092,  1093,  1099,  1094,
    1097,  1098,  1101,  1113,  1114,  1115,  1118,  1120,  1121,  1125,
    1127,  1132,  1130,  1134,  1124,  1129,  1140,  1138,  1139,  1133,
    1136,  1141,  1145,  1146,  1147,  1152,  1163,  1165,  1148,  1151,
    1169,  1174,  1155,  1154,  1157,  1158,  1162,    10,  1164,  1175,
    1172,  1182,  1189,   232,   799,  1191,   225,  1068,   623,   540,
     382,   425,   371,  1042,   690,  1063,   971,  1117,   304,  1149,
    1126,  1095,   949,   544,  1112,   842,  1150,   888,   740,     0,
       0,   449,     0,     0,     0,   665,     0,     0,     0,   686
};

static const yytype_int16 yycheck[] =
{
     108,   180,   378,   230,   617,    81,   177,   490,   538,   188,
     537,   190,   465,   505,   506,   414,   515,   196,   197,     3,
     199,   200,   502,     4,     3,   414,   676,     8,     3,     3,
     872,   210,     3,    84,   213,   182,   680,     4,   217,    59,
     219,     4,    84,    84,   191,   200,    84,   539,     4,   204,
     205,   206,     8,     9,    84,    85,   235,   236,   237,   238,
      68,    84,    84,     3,   211,   212,     4,   214,   215,     4,
     249,   218,     6,    84,   221,     5,   223,    84,   249,   226,
       4,   457,    84,    85,   460,     5,   997,  1061,   187,     4,
     693,     4,    92,     8,     9,   223,  1142,     3,   226,   223,
      84,   280,   281,  1116,   283,    84,     4,     3,   224,  1159,
      84,   160,   193,   224,    98,   225,    75,    84,    85,   231,
     201,   202,  1168,   128,    98,    84,     4,  1177,   223,     4,
     242,   243,   231,   209,    76,   230,     4,   223,   248,   249,
       8,   222,    84,  1156,   230,     0,   227,   148,    26,   150,
      28,   279,   280,   281,    84,    33,    34,    35,   385,   283,
      95,    99,    97,  1137,    84,   241,   231,     3,   249,    84,
     223,   247,   231,   249,   787,     3,   779,   230,    84,   103,
     231,    16,    17,   225,   214,  1096,    79,   195,   188,   802,
     231,    84,    98,   231,   107,   225,   109,   278,   279,   812,
    1042,   242,   243,   225,   223,   225,    84,    85,   231,    84,
     145,   230,    89,    90,   225,    84,   246,   247,   225,   242,
     243,   224,    91,   225,   227,   678,   223,   757,   277,   278,
     279,   280,   281,   230,   283,   223,   223,   223,   223,   226,
     890,   225,   230,   414,   225,   230,   225,   231,    84,   893,
     225,   225,   231,   233,   225,   229,    84,   231,   225,   240,
     241,   488,   225,   141,   231,   129,   746,   446,   447,   225,
     224,   670,   648,   240,   241,   242,   243,   240,   241,   668,
     254,   255,   461,   462,   227,   225,   162,   434,   164,   229,
     782,   790,   819,   227,   747,   276,   223,   444,   477,   226,
     225,   448,     4,   224,   834,   452,     8,     9,   384,   276,
     250,   251,   491,   276,   224,   423,   463,   393,   394,   225,
     396,   397,   398,   399,   400,   231,   481,   225,   223,   225,
     485,   486,   487,   231,   212,   224,   214,   413,   414,   224,
     511,   224,   240,   241,   242,   243,   282,   225,   254,   255,
     426,   229,   224,   231,   250,   251,    84,   225,    86,    87,
     436,   224,   240,   241,   242,   243,   236,   237,   246,   247,
     248,   249,   240,   241,   279,   280,   281,    94,   276,   455,
     527,   560,   561,   459,   101,   224,   223,    83,    84,   225,
      88,    89,   468,   229,   224,   231,     3,   225,   276,    44,
      45,    46,   581,   231,   780,   226,    59,   124,   276,  1052,
    1053,   205,    10,    11,    12,    13,   252,   253,     4,   224,
     224,   224,     8,     9,   252,   253,   224,   144,   224,   224,
     103,    84,    85,   224,   228,   224,   966,   224,   224,   969,
     277,   278,   279,   280,   281,   224,    84,    85,   224,   525,
      88,    89,   223,     4,   224,   226,   277,   278,   279,   280,
     281,   224,     3,   224,   224,   224,     7,   224,   224,   224,
     224,   188,   189,   190,   191,    61,   193,   194,   195,   196,
     197,     3,     4,   277,   278,   279,   280,   281,   224,    75,
      76,    77,    78,   224,    80,  1022,    82,   224,    84,    85,
     224,   224,   224,   674,   224,   224,   277,   278,   279,   280,
     281,   224,   224,   889,   224,   224,   224,   224,  1001,   224,
    1050,   224,    96,   224,  1051,   224,   112,   113,   114,   224,
     116,   224,  1145,   119,   120,   121,   224,   123,   124,   125,
     767,   127,   224,    84,    85,  1158,   224,   694,   224,   224,
     224,   698,   224,   150,   701,   702,     4,   223,   225,   227,
     226,   214,    84,    85,  1094,   225,   231,   282,   154,   227,
     156,   157,   225,   229,   222,   651,   214,   223,   227,   165,
     166,   167,   168,   169,   170,   225,     4,   225,   223,   223,
     223,   818,   771,   246,   247,   822,   672,   228,   769,   826,
     827,   828,   188,   227,   831,   832,   833,   223,   246,   247,
     223,   277,   278,   279,   280,   281,   202,   788,    80,   225,
     696,   697,   225,   225,   700,  1155,   225,     3,   214,   225,
     285,   277,   278,   279,   280,   281,    84,    85,   223,   225,
     277,   278,   279,   280,   281,  1175,   277,   278,   279,   280,
     281,   227,     3,   115,   116,   227,   229,   223,   829,   223,
     246,   247,   124,   223,   223,   836,   207,   208,   209,   210,
     211,   133,   213,   214,   215,   216,   217,   218,   219,   223,
     223,     4,   144,   223,   225,   207,   208,   209,   210,   211,
     276,   213,   214,   215,   216,   217,   218,   219,   228,   227,
     223,     4,   223,   225,   228,   246,   247,   225,   170,   250,
     251,   223,   223,   223,   223,   256,   277,   278,   279,   280,
     281,   223,     4,   223,   246,   247,   223,   223,     6,   228,
     806,   223,   223,   223,   256,   228,   223,     4,   223,   223,
     816,   223,   223,   223,   223,   223,   973,   277,   278,   279,
     280,   281,   223,   277,   278,   279,   280,   281,   929,   207,
     208,   209,   210,   211,   223,   213,   214,   215,   216,   217,
     218,   219,    83,    84,    85,   228,   223,   225,   277,   278,
     279,   280,   281,   223,   277,   278,   279,   280,   281,  1016,
    1017,  1018,  1019,  1020,   223,   234,   223,   223,   246,   247,
    1176,   240,  1178,   242,   223,   244,   223,   246,   256,   248,
     223,   250,   223,   252,   223,   254,  1192,   256,   226,   283,
       6,   223,   226,   224,   224,   901,   224,   224,    14,     3,
      15,     3,    18,    19,    20,    21,    22,    23,   223,   228,
      26,    27,    28,    29,    30,    31,    32,     5,   223,   232,
      36,    37,    38,    39,    40,   226,    42,   223,   223,   220,
     228,    47,    48,    49,    50,    51,    52,    53,    54,    99,
      56,    57,    58,   228,    60,  1054,    62,    63,    64,    65,
      66,    67,   223,   223,   223,    71,    72,    83,    74,    83,
     223,    41,     4,   223,    41,    81,   207,   208,   209,   210,
     211,    43,   213,   214,   215,   216,   217,   218,   219,   277,
     278,   279,   280,   281,   225,   188,   102,    93,   223,   105,
      93,   223,   277,   278,   279,   280,   281,   223,    97,   228,
     192,   106,   103,   136,   136,   246,   247,  1013,   151,   140,
     126,   228,   146,   223,   230,   256,   132,   147,   134,   135,
      84,    85,   138,   139,     3,   141,   226,   143,   223,     4,
     284,   227,   223,   149,   223,     4,   152,   153,   226,    84,
      85,   223,   158,   159,   223,   161,   223,   163,   277,   278,
     279,   280,   281,  1162,   241,   223,   243,   223,   245,   231,
     247,   231,   249,     4,   251,   223,   253,   223,   255,   223,
     257,   223,   223,  1182,   223,   223,   223,   227,   224,     4,
       4,  1119,   198,   199,   200,   201,   223,   203,   223,    83,
       4,   140,     3,   283,   223,   223,   223,     4,   223,     3,
     223,    83,   224,   223,   223,  1143,   223,     5,   226,   223,
     223,   227,   223,   223,   223,   223,   223,   233,   234,     4,
     232,   188,     4,   187,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   224,   207,   208,   209,   210,   211,   223,   213,
     214,   215,   216,   217,   218,   219,   225,   223,   226,    86,
      87,   225,   207,   208,   209,   210,   211,   223,   213,   214,
     215,   216,   217,   218,   219,   223,   226,   223,   225,   223,
     225,   224,   246,   247,   224,   224,   224,   224,   224,   224,
     224,   224,   256,   224,   224,   224,   188,   224,   224,   224,
     224,   246,   247,   130,   131,   224,   234,   223,   226,   223,
     137,   256,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
      93,   231,   223,    93,   223,    69,   223,   223,   100,   100,
       4,   168,   100,   223,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     226,   229,   226,   226,   226,   111,   110,   117,   122,   118,
     223,   155,     4,   223,   223,     4,   224,   228,   223,     4,
     224,   223,     4,   223,   223,   223,   223,   223,     4,   223,
     223,   223,     5,    44,     3,   226,   226,   104,   224,     4,
      70,   224,   224,   224,   231,   224,   224,   224,   224,   224,
     223,   223,    95,   223,   107,   242,   243,   226,   226,   224,
     107,   224,   224,   224,   224,   224,     4,   224,   224,   224,
     224,   223,     7,   226,   224,   223,    84,   226,     4,   230,
     145,     4,   223,   101,   224,   223,     4,   223,   100,   224,
     223,   223,     4,     4,   223,     4,   224,     5,    44,   226,
     103,     4,    24,   188,   284,   224,   108,    95,    94,   226,
     226,   226,   223,   226,   221,    25,    99,   188,   226,   224,
      94,     4,   224,   226,   223,   223,   223,     5,   224,   224,
     223,   223,  1181,    73,   678,   226,    66,  1004,   490,   422,
     229,   253,   220,   972,   557,   999,   855,  1063,   184,  1124,
    1087,  1041,   836,   428,  1055,   751,  1129,   790,   581,    -1,
      -1,   287,    -1,    -1,    -1,   525,    -1,    -1,    -1,   550
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    16,    17,   287,   288,   289,   290,   224,   224,     0,
     288,   128,   300,   231,   231,   224,     6,    14,    18,    19,
      20,    21,    22,    23,    26,    27,    28,    29,    30,    31,
      32,    36,    37,    38,    39,    40,    42,    47,    48,    49,
      50,    51,    52,    53,    54,    56,    57,    58,    60,    62,
      63,    64,    65,    66,    67,    71,    72,    74,    81,   102,
     105,   126,   132,   134,   135,   138,   139,   141,   143,   149,
     152,   153,   158,   159,   161,   163,   198,   199,   200,   201,
     203,   227,   233,   234,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   291,   292,   293,   294,   295,   302,   304,   305,
     306,   307,   309,   310,   318,   321,   322,   324,   325,   332,
     334,   335,   337,   339,   341,   345,   346,   347,   350,   352,
     359,   366,   369,   375,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   391,   392,   393,   394,   398,   399,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   424,   425,   426,   223,   129,   301,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   324,   224,   224,   224,   224,
     227,   234,   306,   308,   326,   224,   224,   224,   224,   233,
       4,   225,   240,   241,   276,   459,   469,   225,   302,   282,
     224,   336,     3,   223,   231,   242,   243,   445,   458,    10,
      11,    12,    13,     4,    99,   470,    84,    85,    88,    89,
     214,   225,   246,   247,   367,   447,   448,   103,   207,   208,
     209,   210,   211,   213,   215,   216,   217,   218,   219,   225,
     256,   447,   467,   468,     3,    84,    98,   225,   231,   254,
     255,   377,   456,   457,   377,     3,    84,   225,   231,   252,
     253,   371,   454,   455,    84,   231,   427,    75,    84,   439,
     447,     4,   447,   187,   468,   225,   248,   249,   450,   451,
     450,    59,   225,   477,   478,   231,   447,   447,    96,   447,
     225,   447,   477,   450,   450,    84,   231,   349,   428,   477,
     477,   477,   446,   459,   225,   348,   459,   447,   468,   468,
     447,   468,   468,     4,     8,     9,    84,   471,   447,   468,
     447,   349,   468,   450,   468,     5,    84,   360,   342,   468,
     450,   150,   305,   446,   282,   227,   447,   447,   447,   447,
     225,     8,   459,   227,   205,   228,   277,   278,   279,   280,
     281,   458,   459,   464,   465,   466,    26,    28,    33,    34,
      35,   141,   212,   225,   229,   447,   450,   458,   459,   473,
     474,   475,   340,   229,   303,   301,   227,   223,   283,   223,
     223,   223,   222,   449,   225,     4,   227,   223,   223,   354,
     450,   450,   447,   447,   225,   447,   225,   225,   225,   467,
     225,   468,   285,   223,     3,   227,   378,   376,     3,   227,
     370,   223,   223,   223,   223,   223,   223,     4,   227,   223,
     223,   225,     4,   223,   223,   223,   223,   223,   223,     4,
     223,   223,   223,     6,   323,   223,   223,   223,   223,     4,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   303,   343,   223,   223,   223,   228,   459,
     446,   223,   223,   223,   223,   319,   226,   226,   459,   459,
     459,   459,   459,   459,   459,   223,   226,   224,     4,   314,
     315,   316,   317,   448,   451,   458,   459,   224,   224,   224,
     304,   302,   459,    15,   445,     3,     3,   225,   400,     3,
     223,   228,   468,   223,   459,    89,    90,   368,     5,   232,
     223,   223,    83,   362,   468,   447,   447,     3,     7,   225,
     250,   251,   452,   453,   468,   220,   226,   468,    99,   223,
     459,   223,   303,   223,   459,   303,   447,   447,   468,    83,
     312,   455,    83,   223,   459,    76,    84,   440,    41,     4,
     223,    41,    43,    84,    86,    87,   441,   444,   188,   447,
      93,   223,    93,   477,    84,   225,   355,   223,   477,   477,
     477,   446,   223,   348,   471,   447,    79,    84,   476,    97,
      68,   195,   192,   103,   106,    84,   225,   356,     4,   103,
     136,   136,   439,   344,   225,   452,   452,   151,   338,   160,
     283,   333,   228,    84,   458,   146,   147,   148,   150,   307,
     461,   462,   463,   228,   228,   464,   468,   223,   223,   230,
     223,   230,   223,   230,   223,   230,   229,   456,   229,   454,
     229,   452,   230,   228,     3,   223,   449,   226,     4,   228,
     354,   447,   447,   284,   223,   226,   223,   223,   223,     3,
     227,   223,   223,   223,   231,   228,     4,     8,     9,    61,
      75,    76,    77,    78,    80,    82,   112,   113,   114,   116,
     119,   120,   121,   123,   124,   125,   127,   154,   156,   157,
     165,   166,   167,   168,   169,   170,   188,   202,   379,   447,
     469,   472,   231,   228,   372,   364,   223,   223,   223,     4,
     228,   223,   223,     4,   223,   227,   223,   224,   223,     4,
     223,     4,   223,   440,   223,   223,   390,   223,    83,   223,
     223,   224,   395,   223,   223,     4,   223,   223,   223,   223,
     140,   223,   223,   303,   283,   459,   327,   223,   223,   224,
     223,   226,     4,   451,   448,   459,   458,   311,   457,   312,
     313,   453,   223,     3,   226,   232,   223,   362,   468,   459,
     459,   468,   223,   459,   468,   468,   225,   226,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   226,   223,   223,   439,   455,
       5,   223,   428,     4,   223,   188,     4,    84,    91,   443,
     454,   188,    93,   223,    84,   225,   357,   231,    93,   446,
     223,   458,    69,   447,   223,   100,     4,   107,   109,   223,
      84,   225,   358,   100,     4,    84,   361,   362,   303,   100,
     452,   229,   440,    84,   458,   236,   237,   460,   461,   320,
     223,   230,   230,   223,   230,   440,   223,   459,   226,   223,
     226,   223,   226,   440,   228,   226,   226,   459,   446,   456,
     111,   110,   446,   117,   118,   122,   446,   446,   446,   458,
     155,   446,   446,   446,   454,    86,    87,   130,   131,   137,
     168,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   373,   442,   444,
     458,    80,   115,   116,   124,   133,   144,   170,   365,   223,
     298,     4,   223,   429,   223,     4,   224,   228,   223,   224,
       4,   367,   223,   223,     4,    84,   363,   223,   223,    94,
     101,   124,   144,   188,   189,   190,   191,   193,   194,   195,
     196,   197,   396,   223,   223,     4,     4,   429,   223,   223,
     223,   162,   164,   328,   329,   303,   457,   453,   226,     5,
      44,   459,   226,   278,   224,   458,   224,   224,   224,   224,
     224,   224,   224,   224,   224,     3,   223,    44,    45,    46,
     430,   431,   104,   432,     4,   223,   454,    92,   188,   454,
     226,   223,   358,   446,   226,    70,    95,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   107,   107,   226,
     223,   432,     4,   363,     7,   471,   224,   230,   328,   223,
     226,   223,   226,   459,    84,   446,   446,   446,   446,   446,
     187,   231,   374,   456,     4,   145,     4,   223,   224,   101,
     434,   223,     4,   223,   224,   368,   429,   223,   223,   100,
     435,     4,     4,    95,    97,   145,   397,   454,   456,   400,
     400,   447,   427,     4,   223,     4,   434,   364,   224,   331,
       5,    44,   223,   296,   284,   226,   430,   103,   433,   224,
      24,   437,     4,   226,   188,   454,   226,   432,    95,    94,
     108,   226,   437,   330,   302,   223,   226,   221,   226,   374,
     435,   224,    25,   438,   226,   224,   434,   223,   223,   438,
     302,   440,   223,    99,   224,   188,   436,   454,   437,    94,
     440,   436,   223,   447,     4,   224,   410,   438,   351,     5,
      84,   299,   223,   297,   454,   303,   411,   436,   303,   298,
     447,   226,   353,   303
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
		      add_opvars(splev, "iiiiio", VA_PASS6(1, 1, (int)(yyvsp[(7) - (7)].i), 0, (int)(yyvsp[(5) - (7)].i), SPO_DECORATION));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2392 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2398 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2402 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2408 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_CARPET));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2414 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2418 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4(0, (int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2424 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2428 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2434 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2438 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2443 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2450 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2454 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2461 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2465 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2470 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2477 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2481 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2486 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2494 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2500 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2507 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2511 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2518 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2529 "lev_comp.y"
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

  case 397:

/* Line 1455 of yacc.c  */
#line 2542 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2546 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2552 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2556 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2563 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2567 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2571 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2577 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2581 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2587 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2593 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2597 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2603 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2609 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2614 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2620 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2624 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2630 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2634 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2650 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2658 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2668 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2677 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2682 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2689 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2700 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2706 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2710 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2714 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2721 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2731 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2738 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2742 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2748 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2752 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2760 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2764 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2771 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2781 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2794 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2798 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2805 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2815 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2824 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2835 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2839 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2846 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2856 "lev_comp.y"
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

  case 455:

/* Line 1455 of yacc.c  */
#line 2867 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2876 "lev_comp.y"
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

  case 457:

/* Line 1455 of yacc.c  */
#line 2886 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2892 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2896 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2903 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2913 "lev_comp.y"
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

  case 462:

/* Line 1455 of yacc.c  */
#line 2924 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2933 "lev_comp.y"
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

  case 464:

/* Line 1455 of yacc.c  */
#line 2943 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2947 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2953 "lev_comp.y"
    { ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2955 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2961 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 2965 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2969 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 2973 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 2981 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 2990 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 2994 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 2998 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3002 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3006 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3009 "lev_comp.y"
    { ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3013 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3020 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3029 "lev_comp.y"
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

  case 486:

/* Line 1455 of yacc.c  */
#line 3070 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3074 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3081 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3088 "lev_comp.y"
    {
			      size_t len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3098 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3102 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3110 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3114 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3118 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3122 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3126 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3131 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3135 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3139 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3144 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3149 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3154 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3158 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3163 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3168 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3172 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3176 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3181 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3185 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3192 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3198 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3202 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3208 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
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
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 522:

/* Line 1455 of yacc.c  */
#line 3236 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 531:

/* Line 1455 of yacc.c  */
#line 3258 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 532:

/* Line 1455 of yacc.c  */
#line 3262 "lev_comp.y"
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

  case 533:

/* Line 1455 of yacc.c  */
#line 3288 "lev_comp.y"
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
#line 7483 "lev_comp.tab.c"
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
#line 3316 "lev_comp.y"


/*lev_comp.y*/

