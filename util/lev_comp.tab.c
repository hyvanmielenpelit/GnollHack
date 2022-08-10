
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

/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-05 */

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

extern int FDECL(reverse_jmp_opcode, (int));

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
     ELEMENTAL_ENCHANTMENT_TYPE = 357,
     EXCEPTIONALITY_TYPE = 358,
     EXCEPTIONALITY_ID = 359,
     ELEMENTAL_ENCHANTMENT_ID = 360,
     ENCHANTMENT_ID = 361,
     SECRET_DOOR_ID = 362,
     USES_UP_KEY_ID = 363,
     MYTHIC_PREFIX_TYPE = 364,
     MYTHIC_SUFFIX_TYPE = 365,
     MYTHIC_PREFIX_ID = 366,
     MYTHIC_SUFFIX_ID = 367,
     CHARGES_ID = 368,
     SPECIAL_QUALITY_ID = 369,
     SPEFLAGS_ID = 370,
     SUBROOM_ID = 371,
     NAME_ID = 372,
     FLAGS_ID = 373,
     FLAG_TYPE = 374,
     MON_ATTITUDE = 375,
     MON_ALERTNESS = 376,
     SUBTYPE_ID = 377,
     NON_PASSDOOR_ID = 378,
     MON_APPEARANCE = 379,
     ROOMDOOR_ID = 380,
     IF_ID = 381,
     ELSE_ID = 382,
     TERRAIN_ID = 383,
     HORIZ_OR_VERT = 384,
     REPLACE_TERRAIN_ID = 385,
     LOCATION_SUBTYPE_ID = 386,
     DOOR_SUBTYPE = 387,
     BRAZIER_SUBTYPE = 388,
     SIGNPOST_SUBTYPE = 389,
     TREE_SUBTYPE = 390,
     FOREST_ID = 391,
     FOREST_TYPE = 392,
     INITIALIZE_TYPE = 393,
     EXIT_ID = 394,
     SHUFFLE_ID = 395,
     MANUAL_TYPE_ID = 396,
     MANUAL_TYPE = 397,
     QUANTITY_ID = 398,
     BURIED_ID = 399,
     LOOP_ID = 400,
     FOR_ID = 401,
     TO_ID = 402,
     SWITCH_ID = 403,
     CASE_ID = 404,
     BREAK_ID = 405,
     DEFAULT_ID = 406,
     ERODED_ID = 407,
     TRAPPED_STATE = 408,
     RECHARGED_ID = 409,
     INVIS_ID = 410,
     GREASED_ID = 411,
     INDESTRUCTIBLE_ID = 412,
     FEMALE_ID = 413,
     MALE_ID = 414,
     WAITFORU_ID = 415,
     PROTECTOR_ID = 416,
     CANCELLED_ID = 417,
     REVIVED_ID = 418,
     AVENGE_ID = 419,
     FLEEING_ID = 420,
     BLINDED_ID = 421,
     MAXHP_ID = 422,
     LEVEL_ADJUSTMENT_ID = 423,
     KEEP_ORIGINAL_INVENTORY_ID = 424,
     PARALYZED_ID = 425,
     STUNNED_ID = 426,
     CONFUSED_ID = 427,
     SEENTRAPS_ID = 428,
     ALL_ID = 429,
     MONTYPE_ID = 430,
     OBJTYPE_ID = 431,
     TERTYPE_ID = 432,
     TERTYPE2_ID = 433,
     LEVER_EFFECT_TYPE = 434,
     SWITCHABLE_ID = 435,
     CONTINUOUSLY_USABLE_ID = 436,
     TARGET_ID = 437,
     TRAPTYPE_ID = 438,
     EFFECT_FLAG_ID = 439,
     GRAVE_ID = 440,
     BRAZIER_ID = 441,
     SIGNPOST_ID = 442,
     TREE_ID = 443,
     ERODEPROOF_ID = 444,
     FUNCTION_ID = 445,
     MSG_OUTPUT_TYPE = 446,
     COMPARE_TYPE = 447,
     UNKNOWN_TYPE = 448,
     rect_ID = 449,
     fillrect_ID = 450,
     line_ID = 451,
     randline_ID = 452,
     grow_ID = 453,
     selection_ID = 454,
     flood_ID = 455,
     rndcoord_ID = 456,
     circle_ID = 457,
     ellipse_ID = 458,
     filter_ID = 459,
     complement_ID = 460,
     gradient_ID = 461,
     GRADIENT_TYPE = 462,
     LIMITED = 463,
     HUMIDITY_TYPE = 464,
     STRING = 465,
     MAP_ID = 466,
     NQSTRING = 467,
     VARSTRING = 468,
     CFUNC = 469,
     CFUNC_INT = 470,
     CFUNC_STR = 471,
     CFUNC_COORD = 472,
     CFUNC_REGION = 473,
     VARSTRING_INT = 474,
     VARSTRING_INT_ARRAY = 475,
     VARSTRING_STRING = 476,
     VARSTRING_STRING_ARRAY = 477,
     VARSTRING_VAR = 478,
     VARSTRING_VAR_ARRAY = 479,
     VARSTRING_COORD = 480,
     VARSTRING_COORD_ARRAY = 481,
     VARSTRING_REGION = 482,
     VARSTRING_REGION_ARRAY = 483,
     VARSTRING_MAPCHAR = 484,
     VARSTRING_MAPCHAR_ARRAY = 485,
     VARSTRING_MONST = 486,
     VARSTRING_MONST_ARRAY = 487,
     VARSTRING_OBJ = 488,
     VARSTRING_OBJ_ARRAY = 489,
     VARSTRING_SEL = 490,
     VARSTRING_SEL_ARRAY = 491,
     METHOD_INT = 492,
     METHOD_INT_ARRAY = 493,
     METHOD_STRING = 494,
     METHOD_STRING_ARRAY = 495,
     METHOD_VAR = 496,
     METHOD_VAR_ARRAY = 497,
     METHOD_COORD = 498,
     METHOD_COORD_ARRAY = 499,
     METHOD_REGION = 500,
     METHOD_REGION_ARRAY = 501,
     METHOD_MAPCHAR = 502,
     METHOD_MAPCHAR_ARRAY = 503,
     METHOD_MONST = 504,
     METHOD_MONST_ARRAY = 505,
     METHOD_OBJ = 506,
     METHOD_OBJ_ARRAY = 507,
     METHOD_SEL = 508,
     METHOD_SEL_ARRAY = 509,
     DICE = 510
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
#line 555 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 567 "lev_comp.tab.c"

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
#define YYLAST   1303

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  273
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  188
/* YYNRULES -- Number of rules.  */
#define YYNRULES  516
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1141

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   510

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   268,   272,     2,
     212,   213,   266,   264,   210,   265,   270,   267,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   211,     2,
       2,   269,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   214,     2,   215,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   216,   271,   217,     2,     2,     2,     2,
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
     205,   206,   207,   208,   209,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263
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
     271,   273,   275,   277,   279,   281,   283,   285,   287,   291,
     295,   301,   305,   311,   317,   323,   327,   331,   337,   343,
     349,   357,   365,   373,   379,   381,   385,   387,   391,   393,
     397,   399,   403,   405,   409,   411,   415,   417,   421,   422,
     423,   432,   437,   439,   440,   442,   444,   450,   454,   455,
     456,   466,   467,   470,   471,   477,   478,   483,   485,   488,
     490,   497,   498,   502,   503,   510,   511,   516,   517,   522,
     524,   525,   530,   534,   536,   540,   544,   550,   556,   564,
     569,   570,   584,   585,   601,   602,   605,   611,   613,   619,
     621,   627,   629,   635,   637,   648,   655,   657,   659,   661,
     663,   665,   669,   671,   673,   674,   678,   682,   686,   690,
     692,   694,   696,   698,   700,   708,   714,   716,   718,   720,
     722,   726,   727,   733,   738,   739,   743,   745,   747,   749,
     751,   754,   756,   758,   760,   762,   764,   766,   770,   774,
     778,   780,   782,   786,   788,   790,   792,   796,   798,   800,
     802,   806,   810,   811,   817,   820,   821,   825,   827,   831,
     833,   837,   841,   843,   845,   849,   851,   853,   855,   859,
     861,   863,   865,   869,   873,   877,   881,   885,   889,   893,
     897,   899,   901,   903,   905,   909,   913,   917,   923,   931,
     937,   946,   948,   952,   956,   962,   968,   976,   984,   991,
     997,   998,  1001,  1007,  1011,  1023,  1037,  1044,  1045,  1049,
    1051,  1055,  1059,  1063,  1067,  1069,  1071,  1075,  1079,  1083,
    1087,  1091,  1095,  1097,  1099,  1101,  1103,  1107,  1111,  1113,
    1119,  1129,  1135,  1143,  1147,  1151,  1157,  1167,  1173,  1179,
    1185,  1186,  1200,  1201,  1203,  1211,  1223,  1233,  1247,  1251,
    1259,  1267,  1273,  1281,  1287,  1297,  1303,  1309,  1313,  1321,
    1325,  1333,  1341,  1345,  1351,  1357,  1361,  1367,  1375,  1385,
    1387,  1389,  1391,  1393,  1395,  1396,  1399,  1401,  1405,  1407,
    1409,  1411,  1412,  1416,  1418,  1419,  1423,  1425,  1426,  1430,
    1432,  1434,  1436,  1438,  1440,  1442,  1444,  1446,  1448,  1452,
    1454,  1456,  1461,  1463,  1465,  1470,  1472,  1474,  1479,  1481,
    1486,  1492,  1494,  1498,  1500,  1504,  1506,  1508,  1513,  1523,
    1525,  1527,  1532,  1534,  1540,  1542,  1544,  1549,  1551,  1553,
    1559,  1561,  1563,  1565,  1570,  1572,  1574,  1580,  1582,  1584,
    1586,  1590,  1592,  1594,  1598,  1600,  1605,  1609,  1613,  1617,
    1621,  1625,  1629,  1631,  1633,  1637,  1639,  1643,  1644,  1646,
    1648,  1650,  1652,  1656,  1657,  1659,  1661,  1664,  1667,  1672,
    1679,  1684,  1691,  1698,  1705,  1712,  1715,  1722,  1731,  1740,
    1751,  1766,  1769,  1771,  1775,  1777,  1781,  1783,  1785,  1787,
    1789,  1791,  1793,  1795,  1797,  1799,  1801,  1803,  1805,  1807,
    1809,  1811,  1813,  1815,  1817,  1819,  1830
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     274,     0,    -1,    -1,   275,    -1,   276,    -1,   276,   275,
      -1,   277,   287,   289,    -1,    16,   211,   218,    -1,    15,
     211,   218,   210,     3,    -1,    17,   211,    11,   210,   387,
      -1,    17,   211,    10,   210,     3,    -1,    17,   211,    13,
      -1,    17,   211,    12,   210,     3,   210,     3,   210,     5,
     210,     5,   210,   422,   210,   286,   285,    -1,    18,   211,
     452,    -1,   136,   211,   137,   210,   138,    -1,   136,   211,
     137,    -1,    29,   211,     4,   210,   299,    -1,    21,   211,
     100,    -1,    -1,   210,   208,    -1,    -1,   210,   429,    -1,
      -1,   210,     3,    -1,     5,    -1,    81,    -1,    -1,   118,
     211,   288,    -1,   119,   210,   288,    -1,   119,    -1,    -1,
     291,   289,    -1,   216,   289,   217,    -1,   332,    -1,   278,
      -1,   279,    -1,   395,    -1,   282,    -1,   280,    -1,   402,
      -1,   281,    -1,   401,    -1,   399,    -1,   400,    -1,   403,
      -1,   404,    -1,   405,    -1,   406,    -1,   407,    -1,   376,
      -1,   334,    -1,   297,    -1,   296,    -1,   390,    -1,   346,
      -1,   368,    -1,   409,    -1,   410,    -1,   378,    -1,   379,
      -1,   380,    -1,   408,    -1,   312,    -1,   322,    -1,   324,
      -1,   328,    -1,   326,    -1,   309,    -1,   319,    -1,   305,
      -1,   308,    -1,   371,    -1,   381,    -1,   353,    -1,   369,
      -1,   356,    -1,   362,    -1,   391,    -1,   386,    -1,   374,
      -1,   333,    -1,   392,    -1,   393,    -1,   394,    -1,   396,
      -1,   339,    -1,   337,    -1,   385,    -1,   389,    -1,   388,
      -1,   372,    -1,   373,    -1,   375,    -1,   367,    -1,   370,
      -1,   228,    -1,   230,    -1,   232,    -1,   234,    -1,   236,
      -1,   238,    -1,   240,    -1,   242,    -1,   244,    -1,   227,
      -1,   229,    -1,   231,    -1,   233,    -1,   235,    -1,   237,
      -1,   239,    -1,   241,    -1,   243,    -1,   292,    -1,   293,
      -1,   221,    -1,   221,    -1,   293,    -1,   140,   211,   292,
      -1,   294,   269,   441,    -1,   294,   269,   199,   211,   450,
      -1,   294,   269,   440,    -1,   294,   269,   457,   211,   434,
      -1,   294,   269,   456,   211,   436,    -1,   294,   269,   455,
     211,   438,    -1,   294,   269,   429,    -1,   294,   269,   432,
      -1,   294,   269,   216,   303,   217,    -1,   294,   269,   216,
     302,   217,    -1,   294,   269,   216,   301,   217,    -1,   294,
     269,   457,   211,   216,   300,   217,    -1,   294,   269,   456,
     211,   216,   299,   217,    -1,   294,   269,   455,   211,   216,
     298,   217,    -1,   294,   269,   216,   304,   217,    -1,   439,
      -1,   298,   210,   439,    -1,   437,    -1,   299,   210,   437,
      -1,   435,    -1,   300,   210,   435,    -1,   433,    -1,   301,
     210,   433,    -1,   430,    -1,   302,   210,   430,    -1,   441,
      -1,   303,   210,   441,    -1,   440,    -1,   304,   210,   440,
      -1,    -1,    -1,   190,   220,   212,   306,   445,   213,   307,
     290,    -1,   220,   212,   448,   213,    -1,   139,    -1,    -1,
       6,    -1,     6,    -1,   214,   441,   192,   441,   215,    -1,
     214,   441,   215,    -1,    -1,    -1,   148,   313,   214,   428,
     215,   314,   216,   315,   217,    -1,    -1,   316,   315,    -1,
      -1,   149,   453,   211,   317,   289,    -1,    -1,   151,   211,
     318,   289,    -1,   150,    -1,   270,   270,    -1,   147,    -1,
     146,   295,   269,   441,   320,   441,    -1,    -1,   321,   323,
     290,    -1,    -1,   145,   214,   428,   215,   325,   290,    -1,
      -1,   311,   211,   327,   291,    -1,    -1,   126,   311,   329,
     330,    -1,   290,    -1,    -1,   290,   331,   127,   290,    -1,
      14,   211,   440,    -1,    71,    -1,    71,   211,   453,    -1,
      71,   211,    81,    -1,    59,   211,   335,   210,   335,    -1,
      59,   211,   335,   210,   453,    -1,   212,     4,   210,    80,
     210,   350,   213,    -1,   412,   310,   210,   422,    -1,    -1,
     116,   211,   336,   210,   343,   210,   345,   413,   416,   418,
     420,   338,   290,    -1,    -1,    51,   211,   336,   210,   342,
     210,   344,   210,   345,   413,   416,   418,   420,   340,   290,
      -1,    -1,   210,     5,    -1,   212,     4,   210,     4,   213,
      -1,    81,    -1,   212,     4,   210,     4,   213,    -1,    81,
      -1,   212,   354,   210,   355,   213,    -1,    81,    -1,   212,
       4,   210,     4,   213,    -1,    81,    -1,   125,   211,   347,
     210,   421,   210,   348,   210,   350,   351,    -1,    27,   211,
     421,   210,   450,   351,    -1,     5,    -1,    81,    -1,   349,
      -1,    81,    -1,    80,    -1,    80,   271,   349,    -1,     4,
      -1,    81,    -1,    -1,   351,   210,   352,    -1,   131,   211,
     132,    -1,    77,   211,   438,    -1,   114,   211,     4,    -1,
     157,    -1,   107,    -1,   108,    -1,   123,    -1,    20,    -1,
      19,   211,   354,   210,   355,   341,   219,    -1,    19,   211,
     429,   341,   219,    -1,    85,    -1,    86,    -1,    87,    -1,
      86,    -1,    25,   211,   358,    -1,    -1,    25,   211,   358,
     357,   290,    -1,   436,   210,   429,   359,    -1,    -1,   359,
     210,   360,    -1,   440,    -1,   120,    -1,   121,    -1,   424,
      -1,   124,   440,    -1,   158,    -1,   159,    -1,   155,    -1,
     162,    -1,   163,    -1,   164,    -1,   165,   211,   428,    -1,
     166,   211,   428,    -1,   170,   211,   428,    -1,   171,    -1,
     172,    -1,   173,   211,   361,    -1,   160,    -1,   161,    -1,
     167,    -1,   168,   211,   428,    -1,   169,    -1,   218,    -1,
     174,    -1,   218,   271,   361,    -1,    23,   211,   364,    -1,
      -1,    24,   211,   364,   363,   290,    -1,   438,   365,    -1,
      -1,   365,   210,   366,    -1,    74,    -1,   175,   211,   436,
      -1,   454,    -1,   117,   211,   440,    -1,   143,   211,   428,
      -1,   144,    -1,    73,    -1,   152,   211,   428,    -1,   189,
      -1,    72,    -1,   153,    -1,   154,   211,   428,    -1,   155,
      -1,   156,    -1,   429,    -1,   105,   211,   102,    -1,   104,
     211,   103,    -1,   106,   211,   428,    -1,   113,   211,   428,
      -1,   114,   211,   428,    -1,   141,   211,   142,    -1,   115,
     211,   428,    -1,    77,   211,   438,    -1,   157,    -1,   108,
      -1,    79,    -1,    75,    -1,   111,   211,   109,    -1,   112,
     211,   110,    -1,    58,   211,   428,    -1,    26,   211,   411,
     210,   429,    -1,    28,   211,   429,   210,    80,   210,   421,
      -1,    33,   211,   429,   210,    80,    -1,    33,   211,   429,
     210,    80,   210,     5,   285,    -1,    34,    -1,    34,   211,
     450,    -1,    34,   211,   174,    -1,    47,   211,   429,   210,
      90,    -1,    48,   211,   429,   210,    90,    -1,    48,   211,
     459,   210,   459,   210,    90,    -1,    53,   211,   459,   210,
     459,   210,   218,    -1,    54,   211,   459,   210,   459,   377,
      -1,    55,   211,   459,   210,   459,    -1,    -1,   210,    90,
      -1,    62,   211,   450,   210,    94,    -1,    63,   211,   450,
      -1,    64,   211,   429,   210,   182,   211,   429,   210,    92,
     210,    91,    -1,    64,   211,   429,   210,    65,   210,    66,
     210,    67,   210,    92,   210,    91,    -1,    78,   211,   429,
     210,   179,   382,    -1,    -1,   382,   210,   383,    -1,    91,
      -1,   175,   211,   436,    -1,   176,   211,   438,    -1,   177,
     211,   387,    -1,   178,   211,   387,    -1,   180,    -1,   181,
      -1,   182,   211,   429,    -1,   183,   211,   411,    -1,   131,
     211,   384,    -1,    98,   211,   419,    -1,   184,   211,     4,
      -1,   114,   211,     4,    -1,     4,    -1,    92,    -1,   132,
      -1,    94,    -1,    69,   211,   450,    -1,    68,   211,   450,
      -1,     3,    -1,   212,     3,   210,   422,   213,    -1,   130,
     211,   432,   210,   434,   210,   434,   210,     7,    -1,   128,
     211,   450,   210,   434,    -1,   128,   211,   450,   210,   434,
     210,    97,    -1,    49,   211,   432,    -1,    50,   211,   432,
      -1,    39,   211,   218,   210,    40,    -1,    39,   211,   218,
     210,    40,   210,   175,   211,   436,    -1,    36,   211,   432,
     210,    38,    -1,    37,   211,   459,   210,    38,    -1,    22,
     211,   450,   210,    96,    -1,    -1,    35,   211,   432,   210,
     422,   210,   412,   413,   416,   418,   420,   397,   398,    -1,
      -1,   290,    -1,    44,   211,   429,   210,   423,   210,   425,
      -1,    44,   211,   429,   210,   423,   210,   425,   210,   175,
     211,   436,    -1,    44,   211,   429,   210,   423,   210,   425,
     210,    89,    -1,    44,   211,   429,   210,   423,   210,   425,
     210,    89,   210,   175,   211,   436,    -1,    45,   211,   429,
      -1,    45,   211,   429,   210,   175,   211,   436,    -1,    99,
     211,   450,   210,   100,   210,    97,    -1,   122,   211,   450,
     210,     4,    -1,   122,   211,   450,   210,   100,   210,    97,
      -1,    46,   211,    93,   210,   429,    -1,    46,   211,    93,
     210,   429,   210,   175,   211,   436,    -1,   185,   211,   429,
     210,   440,    -1,   185,   211,   429,   210,    81,    -1,   185,
     211,   429,    -1,   186,   211,   429,   210,   133,   210,   422,
      -1,   186,   211,   429,    -1,   187,   211,   429,   210,   134,
     210,   440,    -1,   187,   211,   429,   210,   134,   210,    81,
      -1,   187,   211,   429,    -1,   188,   211,   429,   210,   135,
      -1,   188,   211,   429,   210,   137,    -1,   188,   211,   429,
      -1,    60,   211,   441,   210,   429,    -1,    61,   211,   429,
     210,   458,   210,   440,    -1,    57,   211,   428,   210,   428,
     210,   428,   210,   428,    -1,    57,    -1,   218,    -1,    81,
      -1,   218,    -1,    81,    -1,    -1,   210,   414,    -1,   415,
      -1,   415,   210,   414,    -1,    41,    -1,    42,    -1,    43,
      -1,    -1,   101,   211,   417,    -1,   100,    -1,    -1,    98,
     211,   419,    -1,    97,    -1,    -1,   175,   211,   436,    -1,
      72,    -1,    81,    -1,    73,    -1,    81,    -1,    84,    -1,
     426,    -1,    81,    -1,    84,    -1,   426,    -1,    83,   211,
      81,    -1,    88,    -1,    81,    -1,    83,   214,     4,   215,
      -1,   218,    -1,   229,    -1,   230,   214,   441,   215,    -1,
     441,    -1,   430,    -1,   201,   212,   450,   213,    -1,   233,
      -1,   234,   214,   441,   215,    -1,   212,     4,   210,     4,
     213,    -1,    81,    -1,    82,   431,   215,    -1,   209,    -1,
     209,   210,   431,    -1,   433,    -1,   235,    -1,   236,   214,
     441,   215,    -1,   212,     4,   210,     4,   210,     4,   210,
       4,   213,    -1,   435,    -1,   237,    -1,   238,   214,   441,
     215,    -1,     3,    -1,   212,     3,   210,   422,   213,    -1,
     437,    -1,   239,    -1,   240,   214,   441,   215,    -1,   218,
      -1,     3,    -1,   212,     3,   210,   218,   213,    -1,    81,
      -1,   439,    -1,   241,    -1,   242,   214,   441,   215,    -1,
     218,    -1,     3,    -1,   212,     3,   210,   218,   213,    -1,
      81,    -1,    95,    -1,   427,    -1,   440,   270,   427,    -1,
       4,    -1,   451,    -1,   212,     8,   213,    -1,   227,    -1,
     228,   214,   441,   215,    -1,   441,   264,   441,    -1,   441,
     265,   441,    -1,   441,   266,   441,    -1,   441,   267,   441,
      -1,   441,   268,   441,    -1,   212,   441,   213,    -1,   223,
      -1,   224,    -1,   294,   211,   442,    -1,   443,    -1,   444,
     210,   443,    -1,    -1,   444,    -1,   441,    -1,   440,    -1,
     446,    -1,   447,   210,   446,    -1,    -1,   447,    -1,   429,
      -1,   194,   432,    -1,   195,   432,    -1,   196,   429,   210,
     429,    -1,   197,   429,   210,   429,   210,   441,    -1,   198,
     212,   450,   213,    -1,   198,   212,   349,   210,   450,   213,
      -1,   204,   212,     7,   210,   450,   213,    -1,   204,   212,
     450,   210,   450,   213,    -1,   204,   212,   434,   210,   450,
     213,    -1,   200,   429,    -1,   202,   212,   429,   210,   441,
     213,    -1,   202,   212,   429,   210,   441,   210,    41,   213,
      -1,   203,   212,   429,   210,   441,   210,   441,   213,    -1,
     203,   212,   429,   210,   441,   210,   441,   210,    41,   213,
      -1,   206,   212,   207,   210,   212,   441,   265,   441,   283,
     213,   210,   429,   284,   213,    -1,   205,   449,    -1,   243,
      -1,   212,   450,   213,    -1,   449,    -1,   449,   272,   450,
      -1,   263,    -1,    96,    -1,     4,    -1,     8,    -1,     9,
      -1,     4,    -1,     8,    -1,     9,    -1,     4,    -1,   451,
      -1,    30,    -1,    23,    -1,    31,    -1,    25,    -1,    32,
      -1,   128,    -1,    76,    -1,    81,    -1,   460,    -1,    56,
     212,     4,   210,     4,   210,     4,   210,     4,   213,    -1,
     212,     4,   210,     4,   210,     4,   210,     4,   213,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   286,   286,   287,   290,   291,   294,   317,   322,   344,
     357,   369,   375,   404,   410,   414,   420,   426,   433,   436,
     443,   447,   454,   457,   464,   465,   469,   472,   479,   483,
     490,   493,   499,   505,   506,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,   539,   540,   541,
     542,   543,   544,   545,   546,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,   561,
     562,   563,   564,   565,   566,   569,   570,   571,   572,   573,
     574,   575,   576,   577,   580,   581,   582,   583,   584,   585,
     586,   587,   588,   591,   592,   593,   596,   597,   600,   616,
     622,   628,   634,   640,   646,   652,   658,   664,   674,   684,
     694,   704,   714,   724,   736,   741,   748,   753,   760,   765,
     772,   776,   782,   787,   794,   798,   804,   808,   815,   837,
     814,   851,   906,   913,   916,   922,   929,   933,   942,   946,
     941,  1009,  1010,  1014,  1013,  1027,  1026,  1041,  1051,  1052,
    1055,  1093,  1092,  1127,  1126,  1157,  1156,  1189,  1188,  1214,
    1225,  1224,  1252,  1258,  1263,  1268,  1275,  1282,  1291,  1299,
    1311,  1310,  1334,  1333,  1357,  1360,  1366,  1376,  1382,  1391,
    1397,  1402,  1408,  1413,  1419,  1430,  1436,  1437,  1440,  1441,
    1444,  1448,  1454,  1455,  1459,  1465,  1473,  1478,  1483,  1488,
    1493,  1498,  1503,  1511,  1518,  1526,  1534,  1535,  1538,  1539,
    1542,  1547,  1546,  1560,  1567,  1574,  1582,  1587,  1593,  1599,
    1605,  1611,  1616,  1621,  1626,  1631,  1636,  1641,  1646,  1651,
    1656,  1661,  1666,  1672,  1677,  1682,  1687,  1692,  1699,  1708,
    1712,  1725,  1734,  1733,  1751,  1761,  1767,  1775,  1781,  1786,
    1791,  1796,  1801,  1806,  1811,  1816,  1821,  1835,  1841,  1846,
    1851,  1856,  1861,  1866,  1871,  1876,  1881,  1886,  1891,  1896,
    1901,  1906,  1911,  1916,  1922,  1927,  1932,  1939,  1945,  1974,
    1979,  1987,  1993,  1997,  2005,  2012,  2019,  2029,  2039,  2055,
    2066,  2069,  2075,  2081,  2087,  2091,  2097,  2104,  2110,  2118,
    2124,  2129,  2134,  2139,  2144,  2150,  2156,  2161,  2166,  2171,
    2176,  2181,  2188,  2188,  2188,  2188,  2191,  2197,  2203,  2208,
    2215,  2222,  2226,  2232,  2238,  2244,  2249,  2256,  2262,  2272,
    2279,  2278,  2310,  2313,  2319,  2324,  2329,  2334,  2340,  2344,
    2350,  2356,  2360,  2366,  2370,  2376,  2380,  2385,  2392,  2396,
    2403,  2407,  2412,  2419,  2423,  2428,  2436,  2442,  2449,  2453,
    2460,  2468,  2471,  2481,  2485,  2488,  2494,  2498,  2505,  2509,
    2513,  2520,  2523,  2529,  2536,  2539,  2545,  2552,  2556,  2563,
    2564,  2567,  2568,  2571,  2572,  2573,  2579,  2580,  2581,  2587,
    2588,  2591,  2600,  2605,  2612,  2623,  2629,  2633,  2637,  2644,
    2654,  2661,  2665,  2671,  2675,  2683,  2687,  2694,  2704,  2717,
    2721,  2728,  2738,  2747,  2758,  2762,  2769,  2779,  2790,  2799,
    2809,  2815,  2819,  2826,  2836,  2847,  2856,  2866,  2870,  2877,
    2878,  2884,  2888,  2892,  2896,  2904,  2913,  2917,  2921,  2925,
    2929,  2933,  2936,  2943,  2952,  2985,  2986,  2989,  2990,  2993,
    2997,  3004,  3011,  3022,  3025,  3033,  3037,  3041,  3045,  3049,
    3054,  3058,  3062,  3067,  3072,  3077,  3081,  3086,  3091,  3095,
    3099,  3104,  3108,  3115,  3121,  3125,  3131,  3138,  3139,  3142,
    3143,  3144,  3147,  3151,  3155,  3159,  3165,  3166,  3169,  3170,
    3173,  3174,  3177,  3178,  3181,  3185,  3211
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
  "FLOOR_TYPE", "FLOOR_TYPE_ID", "ELEMENTAL_ENCHANTMENT_TYPE",
  "EXCEPTIONALITY_TYPE", "EXCEPTIONALITY_ID", "ELEMENTAL_ENCHANTMENT_ID",
  "ENCHANTMENT_ID", "SECRET_DOOR_ID", "USES_UP_KEY_ID",
  "MYTHIC_PREFIX_TYPE", "MYTHIC_SUFFIX_TYPE", "MYTHIC_PREFIX_ID",
  "MYTHIC_SUFFIX_ID", "CHARGES_ID", "SPECIAL_QUALITY_ID", "SPEFLAGS_ID",
  "SUBROOM_ID", "NAME_ID", "FLAGS_ID", "FLAG_TYPE", "MON_ATTITUDE",
  "MON_ALERTNESS", "SUBTYPE_ID", "NON_PASSDOOR_ID", "MON_APPEARANCE",
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
  "altar_detail", "anvil_detail", "floor_detail", "subtype_detail",
  "npc_detail", "grave_detail", "brazier_detail", "signpost_detail",
  "tree_detail", "gold_detail", "engraving_detail", "mineralize",
  "trap_name", "room_type", "optroomregionflags", "roomregionflags",
  "roomregionflag", "optfloortype", "floortype", "optfloorsubtype",
  "floorsubtype", "optmontype", "door_state", "light_state", "alignment",
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
      44,    58,    40,    41,    91,    93,   123,   125,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,    43,    45,    42,    47,    37,    61,
      46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   273,   274,   274,   275,   275,   276,   277,   277,   278,
     278,   278,   278,   279,   280,   280,   281,   282,   283,   283,
     284,   284,   285,   285,   286,   286,   287,   287,   288,   288,
     289,   289,   290,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   293,   293,   293,   293,   293,   293,
     293,   293,   293,   294,   294,   294,   295,   295,   296,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   298,   298,   299,   299,   300,   300,
     301,   301,   302,   302,   303,   303,   304,   304,   306,   307,
     305,   308,   309,   310,   310,   311,   311,   311,   313,   314,
     312,   315,   315,   317,   316,   318,   316,   319,   320,   320,
     321,   323,   322,   325,   324,   327,   326,   329,   328,   330,
     331,   330,   332,   333,   333,   333,   334,   334,   335,   336,
     338,   337,   340,   339,   341,   341,   342,   342,   343,   343,
     344,   344,   345,   345,   346,   346,   347,   347,   348,   348,
     349,   349,   350,   350,   351,   351,   352,   352,   352,   352,
     352,   352,   352,   353,   353,   353,   354,   354,   355,   355,
     356,   357,   356,   358,   359,   359,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   361,   361,
     361,   362,   363,   362,   364,   365,   365,   366,   366,   366,
     366,   366,   366,   366,   366,   366,   366,   366,   366,   366,
     366,   366,   366,   366,   366,   366,   366,   366,   366,   366,
     366,   366,   366,   366,   366,   366,   366,   367,   368,   369,
     369,   370,   370,   370,   371,   372,   373,   374,   375,   376,
     377,   377,   378,   379,   380,   380,   381,   382,   382,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   384,   384,   384,   384,   385,   386,   387,   387,
     388,   389,   389,   390,   391,   392,   392,   393,   394,   395,
     397,   396,   398,   398,   399,   399,   399,   399,   400,   400,
     401,   402,   402,   403,   403,   404,   404,   404,   405,   405,
     406,   406,   406,   407,   407,   407,   408,   409,   410,   410,
     411,   411,   412,   412,   413,   413,   414,   414,   415,   415,
     415,   416,   416,   417,   418,   418,   419,   420,   420,   421,
     421,   422,   422,   423,   423,   423,   424,   424,   424,   425,
     425,   426,   427,   427,   427,   428,   429,   429,   429,   429,
     430,   430,   430,   431,   431,   432,   432,   432,   433,   434,
     434,   434,   435,   435,   436,   436,   436,   437,   437,   437,
     437,   438,   438,   438,   439,   439,   439,   439,   439,   440,
     440,   441,   441,   441,   441,   441,   441,   441,   441,   441,
     441,   441,   442,   442,   443,   444,   444,   445,   445,   446,
     446,   447,   447,   448,   448,   449,   449,   449,   449,   449,
     449,   449,   449,   449,   449,   449,   449,   449,   449,   449,
     449,   449,   449,   449,   450,   450,   451,   452,   452,   453,
     453,   453,   454,   454,   454,   454,   455,   455,   456,   456,
     457,   457,   458,   458,   459,   459,   460
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
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       5,     3,     5,     5,     5,     3,     3,     5,     5,     5,
       7,     7,     7,     5,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     0,     0,
       8,     4,     1,     0,     1,     1,     5,     3,     0,     0,
       9,     0,     2,     0,     5,     0,     4,     1,     2,     1,
       6,     0,     3,     0,     6,     0,     4,     0,     4,     1,
       0,     4,     3,     1,     3,     3,     5,     5,     7,     4,
       0,    13,     0,    15,     0,     2,     5,     1,     5,     1,
       5,     1,     5,     1,    10,     6,     1,     1,     1,     1,
       1,     3,     1,     1,     0,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     7,     5,     1,     1,     1,     1,
       3,     0,     5,     4,     0,     3,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       1,     1,     3,     1,     1,     1,     3,     1,     1,     1,
       3,     3,     0,     5,     2,     0,     3,     1,     3,     1,
       3,     3,     1,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     3,     3,     3,     5,     7,     5,
       8,     1,     3,     3,     5,     5,     7,     7,     6,     5,
       0,     2,     5,     3,    11,    13,     6,     0,     3,     1,
       3,     3,     3,     3,     1,     1,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     3,     3,     1,     5,
       9,     5,     7,     3,     3,     5,     9,     5,     5,     5,
       0,    13,     0,     1,     7,    11,     9,    13,     3,     7,
       7,     5,     7,     5,     9,     5,     5,     3,     7,     3,
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
       5,     0,    30,     0,     7,     0,   155,     0,     0,     0,
       0,   223,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   301,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   379,     0,
       0,     0,     0,     0,     0,     0,     0,   183,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   152,     0,     0,
       0,   158,   167,     0,     0,     0,     0,     0,     0,     0,
     115,   104,    95,   105,    96,   106,    97,   107,    98,   108,
      99,   109,   100,   110,   101,   111,   102,   112,   103,    34,
      35,    38,    40,    37,     6,    30,   113,   114,     0,    52,
      51,    69,    70,    67,     0,    62,    68,   171,    63,    64,
      66,    65,    33,    80,    50,    86,    85,    54,    73,    75,
      76,    93,    55,    74,    94,    71,    90,    91,    79,    92,
      49,    58,    59,    60,    72,    87,    78,    89,    88,    53,
      77,    81,    82,    83,    36,    84,    42,    43,    41,    39,
      44,    45,    46,    47,    48,    61,    56,    57,     0,    29,
      27,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   177,     0,     0,     0,
       0,     0,   116,   117,     0,     0,     0,     0,     0,     0,
       0,   451,     0,   454,     0,   496,     0,   452,   473,    31,
       0,   175,     0,     8,     0,   412,   413,     0,   449,   182,
       0,     0,     0,    11,   498,   497,    13,   421,     0,   226,
     227,     0,     0,   418,     0,     0,   194,   416,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   492,   475,   494,     0,   445,   447,   448,     0,   444,
     442,     0,   261,   265,   441,   262,   438,   440,     0,   437,
     435,     0,   230,     0,   434,   381,   380,     0,   399,   400,
       0,     0,     0,     0,   303,   302,     0,   426,     0,     0,
     425,     0,     0,     0,     0,   514,     0,     0,   358,     0,
       0,     0,     0,     0,   343,   344,   383,   382,     0,   153,
       0,     0,     0,     0,   415,     0,     0,     0,     0,     0,
     313,     0,   337,   336,   501,   499,   500,   185,   184,     0,
       0,     0,     0,   206,   207,     0,     0,     0,     0,    15,
     118,     0,     0,     0,   367,   369,   372,   375,   148,     0,
       0,     0,     0,   157,     0,     0,     0,     0,     0,   470,
     469,   471,   474,     0,   507,   509,   506,   508,   510,   511,
       0,     0,     0,   125,   126,   121,   119,     0,     0,     0,
       0,    30,   172,    28,     0,     0,     0,     0,     0,   423,
       0,     0,     0,     0,     0,     0,     0,   476,   477,     0,
       0,     0,   485,     0,     0,     0,   491,     0,     0,     0,
       0,     0,     0,   264,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     154,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   179,   178,     0,
       0,     0,   173,     0,     0,     0,     0,     0,     0,   467,
     453,   461,     0,     0,   456,   457,   458,   459,   460,     0,
     151,     0,   451,     0,     0,     0,     0,   142,   140,   146,
     144,     0,     0,     0,   176,     0,     0,   450,    10,   338,
       0,     9,     0,     0,   422,     0,     0,     0,   229,   228,
     194,   195,   225,     0,     0,   210,     0,     0,     0,     0,
     432,     0,     0,   430,     0,     0,   429,     0,     0,   493,
     495,   349,     0,     0,     0,   263,     0,     0,   232,   234,
     297,   214,     0,    16,   136,   299,     0,     0,   401,   402,
       0,   347,     0,     0,   348,   345,   405,     0,   403,     0,
     404,     0,   363,   304,     0,   305,     0,   197,     0,     0,
       0,     0,   310,   309,     0,     0,   186,   187,   376,   512,
     513,     0,   312,     0,     0,   317,     0,   199,     0,     0,
     361,     0,     0,     0,     0,   341,     0,    14,     0,   169,
       0,     0,   159,   366,   365,     0,     0,   373,   374,     0,
     465,   468,     0,   455,   156,   472,   120,     0,     0,   129,
       0,   128,     0,   127,     0,   133,     0,   124,     0,   123,
       0,   122,    32,   414,     0,     0,   424,   417,     0,   419,
       0,   478,     0,     0,     0,   480,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   443,   504,   502,   503,     0,
     276,   273,   267,   293,     0,   292,     0,     0,     0,   291,
       0,     0,     0,     0,     0,     0,     0,     0,   272,     0,
     277,     0,   279,   280,   290,     0,   275,   266,   281,   505,
     269,     0,   436,   233,   205,     0,     0,     0,     0,   427,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   189,     0,     0,   308,     0,     0,     0,     0,
       0,   316,     0,     0,     0,     0,     0,     0,     0,     0,
     174,   168,   170,     0,     0,     0,     0,     0,   149,     0,
     141,   143,   145,   147,     0,   134,     0,     0,   138,     0,
       0,   420,   224,     0,   211,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   446,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   439,     0,     0,   298,   137,    22,     0,   384,     0,
       0,     0,     0,   410,   409,   354,   359,     0,   306,     0,
     201,     0,     0,   307,   311,     0,     0,   377,     0,     0,
       0,   360,     0,   203,     0,   384,   362,   209,     0,   208,
     181,   342,     0,   161,   368,   371,   370,   462,   463,   464,
     466,     0,     0,   132,   131,     0,   130,     0,     0,   479,
     481,     0,   486,     0,   482,     0,   431,   484,   483,     0,
     296,   289,   283,   282,   284,   294,   295,   285,   286,   288,
     270,   287,   271,   274,   278,   268,     0,   406,   237,   238,
       0,   243,   241,   242,   253,   254,   244,   245,   246,     0,
       0,   255,     0,   257,     0,   250,   251,     0,   235,   239,
     407,   236,     0,   220,   221,     0,   222,     0,   219,   215,
       0,   300,     0,     0,   391,     0,     0,     0,   411,     0,
       0,     0,     0,     0,     0,   212,   213,     0,     0,     0,
     319,     0,     0,     0,     0,     0,     0,     0,   324,   325,
       0,     0,     0,   318,     0,     0,   391,     0,     0,     0,
       0,     0,   161,   150,   135,   139,   339,     0,     0,     0,
     433,     0,     0,   240,     0,     0,     0,     0,     0,     0,
       0,     0,    23,     0,   388,   389,   390,   385,   386,     0,
     394,     0,     0,   346,   356,     0,   364,   196,     0,   384,
     378,   188,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   198,     0,   394,   214,   340,     0,
     165,   160,   162,     0,   487,     0,   488,   457,   408,   247,
     248,   256,   249,   259,   258,   252,   217,   218,   216,     0,
       0,     0,     0,   397,     0,     0,     0,     0,     0,   391,
       0,     0,   396,   329,   331,   332,   333,   335,   334,   328,
     320,   321,   322,   323,   326,   327,   330,     0,   397,   204,
     163,    30,     0,     0,     0,     0,     0,   428,   387,   393,
     392,     0,     0,   350,     0,   516,     0,   355,   200,   394,
       0,   314,   202,   190,    30,   166,     0,   489,    19,     0,
     260,   395,     0,   352,   515,     0,   397,     0,     0,   164,
       0,     0,   398,   353,   351,   357,   192,   315,   191,     0,
      20,     0,    24,    25,    22,     0,     0,   193,    12,    21,
     490
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    99,   100,   101,   102,   103,
    1085,  1136,   931,  1134,    12,   170,   104,   412,   105,   106,
     107,   108,   224,   109,   110,   774,   573,   777,   513,   514,
     515,   516,   111,   499,   861,   112,   113,   471,   114,   115,
     225,   763,   971,   972,  1104,  1081,   116,   631,   117,   118,
     242,   119,   628,   120,   410,   121,   366,   488,   623,   122,
     123,   124,   346,   338,   125,  1118,   126,  1131,   426,   599,
     619,   832,   845,   127,   365,   848,   546,   947,   724,   929,
     128,   265,   540,   129,   447,   302,   723,   918,  1045,   130,
     444,   292,   443,   717,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   745,   141,   142,   143,   144,   751,
     963,  1069,   145,   146,   531,   147,   148,   149,   150,   151,
     152,   153,   154,   155,  1113,  1124,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   307,   339,
     934,   997,   998,  1000,  1090,  1053,  1063,  1093,   310,   580,
     589,   919,   825,   590,   248,   343,   282,   267,   420,   319,
     320,   555,   556,   303,   304,   293,   294,   389,   344,   859,
     640,   641,   642,   391,   392,   393,   283,   438,   237,   256,
     358,   720,   407,   408,   409,   611,   324,   325
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -986
static const yytype_int16 yypact[] =
{
      97,  -114,   -38,   157,  -986,    97,    65,   -24,    22,  -986,
    -986,    66,   792,    63,  -986,   152,  -986,    72,    84,    88,
      98,  -986,   110,   115,   126,   128,   130,   148,   154,   158,
     171,   176,   178,   197,   199,   217,   244,   253,   262,   263,
     271,   272,   273,   275,   276,   279,   281,   287,   291,   295,
     296,   297,   300,   310,   311,   314,   315,   317,   322,   324,
     326,   329,   336,    24,   337,   338,   344,  -986,   345,    73,
     885,  -986,  -986,   347,   348,   349,   353,    77,    15,    91,
    -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,
    -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,
    -986,  -986,  -986,  -986,  -986,   792,  -986,  -986,    94,  -986,
    -986,  -986,  -986,  -986,   355,  -986,  -986,  -986,  -986,  -986,
    -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,
    -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,
    -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,
    -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,
    -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,   327,   107,
    -986,  -113,   467,    83,   -23,   285,   843,   136,   136,   174,
      -6,    39,   -30,   459,   -30,   500,  -117,  -117,   -12,   235,
     -30,   -30,   396,   -30,   -10,  -117,  -117,    -5,   -12,   -12,
     -12,    15,   361,    15,   -30,   843,   843,   -30,   843,   843,
     100,   -30,   843,    -5,   843,    86,  -986,   843,  -117,   360,
     731,    15,  -986,  -986,   298,   365,   -30,   -30,   -30,   -30,
     363,  -986,    62,  -986,   383,  -986,   151,  -986,   146,  -986,
     117,  -986,   379,  -986,   152,  -986,  -986,   391,  -986,   351,
     397,   398,   405,  -986,  -986,  -986,  -986,  -986,   421,  -986,
    -986,   419,   632,  -986,   428,   423,   433,  -986,  -986,  -117,
    -117,   -30,   -30,   438,   -30,   440,   441,   442,   843,   445,
     242,  -986,  -986,   389,   435,  -986,  -986,  -986,   659,  -986,
    -986,   452,  -986,  -986,  -986,  -986,  -986,  -986,   660,  -986,
    -986,   453,   456,   458,  -986,  -986,  -986,   463,  -986,  -986,
     465,   466,   473,   479,  -986,  -986,   688,  -986,   485,   483,
    -986,   497,   496,   706,   501,  -986,   508,   511,   513,   514,
     521,   728,   525,   526,  -986,  -986,  -986,  -986,   527,   732,
     530,   531,   532,   534,   166,   741,   536,   192,   537,   538,
    -986,   539,  -986,  -986,  -986,  -986,  -986,  -986,  -986,   540,
     541,   543,   544,  -986,  -986,   545,   379,   546,   551,   552,
    -986,   548,    15,    15,   554,   559,   566,   567,  -986,   565,
     373,    15,    15,  -986,    15,    15,    15,    15,    15,   351,
     166,  -986,   569,   568,  -986,  -986,  -986,  -986,  -986,  -986,
     571,    92,    21,  -986,  -986,   351,   166,   577,   585,   586,
     792,   792,  -986,  -986,    15,  -113,   777,    32,   797,   592,
     588,   843,   594,    15,    48,   800,   605,  -986,  -986,   597,
     598,   678,  -986,   -30,   -30,   422,  -986,   623,   620,   843,
     738,   625,    15,   640,   379,   647,    15,   379,   -30,   -30,
     843,   778,    47,   779,   652,    15,    51,   826,   861,   656,
     829,   828,   339,   694,   -30,   781,   667,   795,   -12,   -20,
    -986,   676,   -12,   -12,   -12,    15,   677,    60,   -30,    49,
     794,     8,   710,   798,    -7,    76,    39,   765,  -986,   119,
     119,   761,  -986,   133,   686,   -34,   769,   770,     9,   830,
    -986,  -986,   414,   420,   -32,   -32,  -986,  -986,  -986,   146,
    -986,   843,   696,   -84,   -72,   -48,    11,  -986,  -986,   351,
     166,   120,   172,   167,  -986,   692,   449,  -986,  -986,  -986,
     910,  -986,   705,   421,  -986,   703,   915,   462,  -986,  -986,
     433,  -986,  -986,   -30,   -30,   658,   713,   714,   720,   723,
    -986,   724,   589,  -986,   721,   726,  -986,   729,   734,  -986,
    -986,  -986,   727,   507,   457,  -986,   742,   629,  -986,  -986,
    -986,  -986,   736,   737,  -986,   754,   937,   685,  -986,  -986,
     756,  -986,   758,   966,  -986,   762,  -986,   760,  -986,   771,
    -986,   772,   774,  -986,   981,  -986,   776,  -986,   987,   787,
      51,   788,   790,  -986,   791,   922,  -986,  -986,  -986,  -986,
    -986,   793,  -986,   799,   796,  -986,   801,  -986,  1000,   804,
    -986,   807,   808,   878,  1005,   832,   840,  -986,   379,  -986,
     740,    15,  -986,  -986,   351,   844,   865,  -986,  -986,   867,
    -986,   869,   868,  -986,  -986,  -986,  -986,  1076,   870,  -986,
       7,  -986,    15,  -986,  -113,  -986,    46,  -986,    47,  -986,
      57,  -986,  -986,  -986,   873,  1081,  -986,  -986,   872,  -986,
     888,  -986,   879,  1008,   843,  -986,    15,    15,   843,   898,
      15,   843,   843,   897,   900,  -986,  -986,  -986,  -986,   899,
    -986,  -986,  -986,  -986,   904,  -986,   906,   908,   912,  -986,
     914,   916,   918,   919,   920,   921,   923,   924,  -986,   925,
    -986,   926,  -986,  -986,  -986,   927,  -986,  -986,  -986,  -986,
    -986,   928,  -986,   901,   911,    39,    47,  1128,   929,  -986,
      -5,  1136,   933,   967,  1140,   141,   174,   970,   -54,  1056,
     938,    -3,  -986,   931,  1057,  -986,    15,   941,  -113,  1086,
     -30,   943,  1058,   944,     4,  1059,   127,   379,  1060,   119,
    -986,  -986,   166,   942,    51,    -4,   183,   830,  -986,   -25,
    -986,  -986,   166,   351,    75,  -986,   105,   150,  -986,    51,
     949,  -986,  -986,    15,  -986,   947,   278,   252,   950,    51,
     690,   951,   952,    15,  -986,    15,   136,  1063,  1065,    15,
    1061,  1062,    15,    15,    15,  -113,  1026,    15,    15,    15,
     174,  -986,   932,    29,  -986,  -986,   959,  1167,   963,   964,
    1171,   965,   962,  -986,  -986,   968,  -986,   969,  -986,  1175,
    -986,   341,   971,  -986,  -986,   972,    80,   351,   973,   974,
     812,  -986,  1181,  -986,  1182,   963,  -986,  -986,   977,  -986,
    -986,  -986,   978,    81,  -986,  -986,   351,  -986,  -986,  -986,
    -986,   379,    46,  -986,  -986,    57,  -986,   976,  1185,   166,
    -986,  1150,  -986,    15,  -986,   979,  -986,  -986,  -986,   203,
    -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,
     351,  -986,  -986,  -986,  -986,  -986,    43,  -986,  -986,  -986,
    -113,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,   982,
     983,  -986,   984,  -986,   985,  -986,  -986,   986,  -986,  -986,
    -986,   351,   988,  -986,  -986,   989,  -986,   990,  -986,  -986,
    1195,  -986,   992,   352,  1102,  1200,   995,   174,  -986,     5,
     174,   993,   997,     4,    15,  -986,  -986,   996,  1141,  1118,
    -986,  1001,  1002,  1003,  1004,  1006,  1007,  1009,  -986,  -986,
    1010,  1011,  1012,  -986,   998,  1014,  1102,    80,  1209,   106,
    1015,  1013,    81,  -986,  -986,  -986,  -986,  1017,  1016,   286,
    -986,    15,  1138,   351,    15,    15,    15,    15,   -67,   136,
    1221,  1096,  -986,  1227,  -986,  -986,  -986,  -986,  1022,  1023,
    1135,  1025,  1232,  -986,  1027,  1028,  -986,  -986,    48,   963,
    -986,  -986,  1030,  1031,  1145,  1234,    61,   174,   136,    32,
      32,   -30,    -6,  1239,  -986,  1240,  1135,  -986,  -986,  1034,
    -986,  -986,  -986,  1241,  -986,  1206,  -986,   381,  -986,  -986,
    -986,  -986,  -986,  -986,   980,  -986,  -986,  -986,  -986,  1035,
     352,  1149,  1039,  1077,  1249,  1041,  1080,   174,  1043,  1102,
    1165,  1168,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,
    -986,  -986,  -986,  -986,  -986,  -986,  -986,  1045,  1077,   911,
    -986,   792,  1050,  1048,  1054,  1051,   -67,  -986,  -986,  -986,
    -986,  1145,  1052,  -986,  1053,  -986,  1064,  -986,  -986,  1135,
    1055,  -986,  -986,  -986,   792,  -986,    51,  -986,  -986,  1066,
    -986,  -986,   174,   379,  -986,   174,  1077,  1176,   379,  -986,
    1067,   -30,  -986,  -986,  -986,  -986,  -986,  -986,  -986,    87,
    1068,   379,  -986,  -986,   959,   -30,  1069,  -986,  -986,  -986,
    -986
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -986,  -986,  1263,  -986,  -986,  -986,  -986,  -986,  -986,  -986,
    -986,  -986,   135,  -986,  -986,  1029,  -105,  -361,   860,  1070,
    1201,  -485,  -986,  -986,  -986,  -986,   614,  -986,  -986,  -986,
    -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  1211,  -986,
    -986,  -986,   307,  -986,  -986,  -986,  -986,  -986,  -986,  -986,
    -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,
    -986,  -986,   803,  1071,  -986,  -986,  -986,  -986,   743,  -986,
    -986,  -986,   342,  -986,  -986,  -986,  -591,   319,   260,  -986,
    -986,   450,   280,  -986,  -986,  -986,  -986,  -986,   205,  -986,
    -986,  1111,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,
    -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,
    -986,  -986,  -986,  -986,  -568,  -986,  -986,  -986,  -986,  -986,
    -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,
    -986,  -986,  -986,  -986,  -986,  -986,  -986,  -986,   270,   563,
    -803,   245,  -986,  -885,  -986,  -936,   206,  -985,  -457,  -592,
    -986,  -986,  -986,   482,   881,  -219,  -173,  -387,   766,   254,
    -369,  -466,  -628,  -516,  -440,  -508,  -615,  -161,   -74,  -986,
     533,  -986,  -986,   789,  -986,  -986,  1024,  -169,   739,  -986,
    -474,  -986,  -986,  -986,  -986,  -986,  -172,  -986
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -232
static const yytype_int16 yytable[] =
{
     239,   266,   371,   607,   236,   487,   659,   284,   742,   311,
     249,   313,   574,   657,   639,   517,   315,   327,   328,   231,
     330,   332,   333,   625,   626,   231,   340,   341,   342,   622,
      16,   348,   778,   518,   351,   529,   349,   350,   359,   352,
     353,   775,   966,   360,   322,   362,   322,   633,   367,   285,
     296,   257,   258,   374,   375,   376,   377,   661,   257,   258,
     550,   597,   259,   260,   354,  1065,   231,   403,   355,   356,
     379,   257,   258,   613,   617,   305,   336,   855,   830,   405,
     620,  1026,   784,   565,   945,   843,   568,   254,   257,   258,
    1078,   363,  1132,  1103,  1004,   316,   512,     7,   429,   430,
     379,   432,   257,   258,   354,   245,   922,  1043,   355,   356,
     354,   308,     1,     2,   355,   356,   246,   247,   317,   318,
     309,   231,   550,   285,   578,   609,   648,   286,   297,   347,
     610,  1126,   579,   649,   538,   539,   923,   924,   650,   285,
     394,   287,   395,   925,   637,   651,   638,   396,   397,   398,
     231,  1044,   926,  1066,   494,  1067,   820,     9,   380,   781,
     927,   946,   652,  1116,   390,   849,   406,   364,  1133,   653,
     550,   261,   854,     8,  1099,   296,   621,   296,   261,   255,
    1005,   357,   262,    11,   245,   817,   928,   867,   781,   262,
     614,   261,   598,  1068,    13,   246,   247,   875,   257,   258,
     323,   286,   331,   263,   264,   618,  1059,   545,   847,   831,
     263,   264,   306,   337,   245,   287,   844,   286,   574,   262,
     826,   654,   823,   263,   264,   246,   247,   232,   655,   824,
     969,   287,   970,   401,   386,   387,   388,   975,    78,   245,
      14,   519,   233,   234,   530,   399,   422,   974,   233,   234,
     246,   247,   535,   297,   982,   297,   604,   734,   288,   298,
     548,   549,   547,   771,   289,   299,   557,   760,   814,   624,
     560,   169,   345,   168,   232,   569,   570,    15,   235,   770,
     629,   571,   639,   171,   235,   862,   815,   221,   881,   233,
     234,   592,   863,   852,   895,   172,   596,   230,   493,   173,
     601,   602,   603,   238,   232,   608,   525,   502,   503,   174,
     504,   505,   506,   507,   508,   726,   400,   244,   261,   233,
     234,   175,   864,   257,   258,   235,   176,   380,   520,   401,
     243,   624,   288,   402,   634,   245,   656,   177,   289,   178,
     526,   179,   646,   382,   233,   234,   246,   247,   288,   537,
     263,   264,   317,   318,   289,   235,   553,   554,   232,   180,
     865,   290,   291,   240,   245,   181,   383,   866,   563,   182,
     671,   672,   567,   233,   234,   246,   247,   290,   291,   624,
     235,   577,   183,   660,   298,   268,   298,   184,   658,   185,
     299,   718,   299,   994,   995,   996,   850,   384,   385,   386,
     387,   388,   478,   630,   553,   554,   857,   858,   186,   235,
     187,   300,   301,   300,   301,   384,   385,   386,   387,   388,
     586,  1003,   587,   588,  1006,   550,   259,   260,   188,   551,
     384,   385,   386,   387,   388,   390,   269,   270,   271,   272,
     273,   321,   274,   261,   275,   276,   277,   278,   279,   334,
     335,  1072,  1073,   326,   280,   189,   384,   385,   386,   387,
     388,   686,   873,   312,   190,   687,   688,   384,   981,   386,
     387,   388,   368,   191,   192,   263,   264,   250,   251,   252,
     253,  1046,   193,   194,   195,   281,   196,   197,   871,   329,
     198,   872,   199,   773,   404,  1029,  1035,   369,   200,  1036,
     973,  1070,   201,   257,   258,   785,   202,   203,   204,   788,
    1071,   205,   791,   792,  1120,   689,   384,   385,   386,   387,
     388,   206,   207,   427,   428,   208,   209,   835,   210,   690,
     691,   692,   693,   211,   694,   212,   695,   213,   257,   258,
     214,  1097,   384,   385,   386,   387,   388,   215,   217,   218,
     384,   385,   386,   387,   388,   219,   220,   762,   226,   227,
     228,   696,   697,   698,   229,   699,   241,   372,   700,   701,
     702,   703,   704,   345,   705,   378,   880,   839,   772,   373,
     884,   257,   258,   887,   888,   889,   501,   837,   892,   893,
     894,  1084,   679,   422,   -18,   411,  1122,   381,   706,  1125,
     707,   708,   786,   787,   856,   414,   790,   416,   417,   709,
     710,   711,   712,   713,   714,   418,   269,   270,   271,   272,
     273,   415,   274,   261,   275,   276,   277,   278,   279,   643,
     419,   421,   715,   424,   552,   644,   422,   384,   385,   386,
     387,   388,   423,   425,   890,   440,   716,   386,   387,   388,
     431,   921,   433,   434,   435,   263,   264,   437,   261,   553,
     554,   439,   441,   445,   663,   281,   442,   446,   448,   262,
     257,   258,  -231,   449,   314,   450,   451,   669,   384,   385,
     386,   387,   388,   452,   384,   385,   386,   387,   388,   453,
     263,   264,   454,   456,   269,   270,   271,   272,   273,   455,
     274,   261,   275,   276,   277,   278,   279,   457,   458,   869,
     459,   460,   280,   384,   385,   386,   387,   388,   461,   879,
     235,   462,   685,   463,   464,  1010,   384,   385,   386,   387,
     388,   465,   466,   263,   264,   467,   468,   469,   470,   983,
     472,   473,   474,   281,   475,   476,   477,   479,   480,   481,
     482,   483,  1123,   484,   485,   486,   489,  1128,   545,   257,
     258,   490,   491,   492,   495,  1039,  1040,  1041,  1042,   496,
    1137,   384,   385,   386,   387,   388,   497,   498,   500,   509,
     528,   510,   511,   269,   270,   271,   272,   273,   521,   274,
     261,   275,   276,   277,   278,   279,   522,   523,    16,   979,
     532,   280,   533,   534,   536,   541,    17,   543,   544,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,   263,   264,   542,    31,    32,    33,    34,    35,
     558,    36,   281,   559,   561,   562,    37,    38,    39,    40,
      41,    42,    43,    44,   722,    45,    46,    47,  1074,    48,
     564,    49,    50,    51,    52,    53,    54,   566,   572,   575,
      55,    56,   576,    57,   581,   582,   583,   584,   585,   591,
      58,   593,   269,   270,   271,   272,   273,   594,   274,   261,
     275,   276,   277,   278,   279,   595,   600,   605,   612,   615,
     280,    59,  -180,   384,   385,   386,   387,   388,   616,   627,
     729,   632,   635,   950,   636,   876,   647,  1037,    60,   662,
     951,   263,   264,   664,    61,   665,   667,    62,    63,   668,
      64,   281,    65,   674,   257,   258,   952,   675,    66,   673,
     676,    67,    68,   677,   678,   680,   681,    69,    70,   682,
      71,   728,    72,   953,   683,   684,   725,   726,  1130,   384,
     385,   386,   387,   388,   384,   385,   386,   387,   388,    82,
     721,    84,  1139,    86,   727,    88,   730,    90,   731,    92,
     732,    94,   733,    96,   734,    98,  1105,    73,    74,    75,
      76,   735,    77,   736,   737,   738,   739,   954,   955,   956,
     957,   740,   958,   959,   960,   961,   962,   741,   743,  1119,
     744,   746,   747,   748,   753,   757,    78,   750,   679,   749,
     761,   752,    79,    80,   754,   896,   897,   755,   756,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,   269,   270,   271,
     272,   273,   758,   274,   261,   275,   276,   277,   278,   279,
     759,    80,   898,   899,   764,   280,   900,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,   765,   263,   264,   766,   767,
     769,   768,   316,   779,   780,   781,   281,   901,   545,   783,
     902,   903,   904,   905,   906,   907,   908,   909,   910,   911,
     912,   913,   914,   915,   916,   917,   222,   782,   789,   793,
     795,   812,    81,   794,    83,   796,    85,   797,    87,   798,
      89,   813,    91,   799,    93,   800,    95,   801,    97,   802,
     803,   804,   805,   816,   806,   807,   808,   809,   810,   817,
     819,   811,   821,   820,   822,   827,   828,   834,   829,   833,
     245,   836,   838,   840,   842,   841,   846,   851,   853,   868,
     870,   246,   247,   874,   877,   878,   882,   883,   891,   930,
     885,   932,   886,   933,   935,   936,   937,   938,   939,   941,
     940,   943,   944,   948,   949,   964,   965,   967,   968,   976,
     977,   978,   980,   984,   985,   986,   987,   988,   992,   989,
     990,   991,   993,   999,  1001,  1002,  1007,  1008,  1012,  1011,
    1013,  1024,  1014,  1015,  1016,  1017,  1028,  1018,  1019,  1038,
    1020,  1021,  1022,  1023,  1025,  1047,  1030,  1033,  1048,  1034,
    1031,  1049,  1050,  1052,  1051,  1054,  1055,  1056,  1064,  1057,
    1060,  1061,  1062,  1076,  1077,  1080,  1082,  1083,  1087,  1089,
    1091,  1086,  1092,  1094,  1095,  1096,  1098,  1100,  1102,  1101,
    1106,  1107,  1108,  1112,  1109,  1117,  1114,  1127,    10,  1138,
     524,   223,   776,   413,   216,  1115,  1121,  1129,  1135,  1032,
     606,   942,  1140,   670,   361,  1009,  1027,  1079,  1058,   295,
     370,  1110,  1075,   818,   920,  1088,   527,  1111,   645,   666,
     860,     0,   436,   719
};

static const yytype_int16 yycheck[] =
{
     105,   174,   221,   477,    78,   366,   522,   176,   600,   182,
     171,   184,   452,   521,   499,   402,   185,   190,   191,     4,
     193,   194,   194,   489,   490,     4,   198,   199,   200,   486,
       6,   204,   660,   402,   207,     3,   205,   206,   211,   208,
     209,   656,   845,   212,    56,   214,    56,    81,   217,     3,
       3,    81,    82,   226,   227,   228,   229,   523,    81,    82,
       3,    81,    85,    86,     4,     4,     4,   240,     8,     9,
       8,    81,    82,    65,    81,    81,    81,    81,    81,   240,
       4,   966,   673,   444,     4,    81,   447,     4,    81,    82,
    1026,     5,     5,  1078,    89,   212,     4,   211,   271,   272,
       8,   274,    81,    82,     4,   218,    77,   174,     8,     9,
       4,    72,    15,    16,     8,     9,   229,   230,   235,   236,
      81,     4,     3,     3,    73,    76,   210,    81,    81,   203,
      81,  1116,    81,   217,    86,    87,   107,   108,   210,     3,
      23,    95,    25,   114,   135,   217,   137,    30,    31,    32,
       4,   218,   123,    92,   373,    94,   210,     0,   232,   213,
     131,    81,   210,  1099,   238,   756,   240,    81,    81,   217,
       3,   201,   764,   211,  1059,     3,   100,     3,   201,    96,
     175,    81,   212,   118,   218,   210,   157,   779,   213,   212,
     182,   201,   212,   132,   218,   229,   230,   789,    81,    82,
     212,    81,   212,   233,   234,   212,  1009,    80,    81,   212,
     233,   234,   218,   218,   218,    95,   212,    81,   658,   212,
     736,   210,    81,   233,   234,   229,   230,   212,   217,    88,
     149,    95,   151,   212,   266,   267,   268,   865,   214,   218,
     218,   402,   227,   228,   212,   128,     4,   862,   227,   228,
     229,   230,   421,    81,   211,    81,   475,   214,   212,   212,
     433,   434,   431,   650,   218,   218,   435,   628,   725,   212,
     439,   119,   212,   210,   212,   448,   449,   211,   263,   648,
     147,   450,   767,   211,   263,   210,   726,   214,   796,   227,
     228,   464,   217,   759,   810,   211,   468,   220,   372,   211,
     472,   473,   474,   212,   212,   478,   411,   381,   382,   211,
     384,   385,   386,   387,   388,   210,   199,   210,   201,   227,
     228,   211,   217,    81,    82,   263,   211,   401,   402,   212,
       3,   212,   212,   216,   495,   218,   216,   211,   218,   211,
     414,   211,   511,   192,   227,   228,   229,   230,   212,   423,
     233,   234,   235,   236,   218,   263,   237,   238,   212,   211,
     210,   241,   242,   269,   218,   211,   215,   217,   442,   211,
     543,   544,   446,   227,   228,   229,   230,   241,   242,   212,
     263,   455,   211,   216,   212,   100,   212,   211,   216,   211,
     218,   564,   218,    41,    42,    43,   757,   264,   265,   266,
     267,   268,   210,   270,   237,   238,   223,   224,   211,   263,
     211,   239,   240,   239,   240,   264,   265,   266,   267,   268,
      81,   937,    83,    84,   940,     3,    85,    86,   211,     7,
     264,   265,   266,   267,   268,   509,   194,   195,   196,   197,
     198,   187,   200,   201,   202,   203,   204,   205,   206,   195,
     196,  1019,  1020,   218,   212,   211,   264,   265,   266,   267,
     268,     4,   210,     4,   211,     8,     9,   264,   265,   266,
     267,   268,   218,   211,   211,   233,   234,    10,    11,    12,
      13,   989,   211,   211,   211,   243,   211,   211,   210,    93,
     211,   213,   211,   654,   240,   969,   210,   137,   211,   213,
     861,  1017,   211,    81,    82,   674,   211,   211,   211,   678,
    1018,   211,   681,   682,  1106,    58,   264,   265,   266,   267,
     268,   211,   211,   269,   270,   211,   211,   746,   211,    72,
      73,    74,    75,   211,    77,   211,    79,   211,    81,    82,
     211,  1057,   264,   265,   266,   267,   268,   211,   211,   211,
     264,   265,   266,   267,   268,   211,   211,   631,   211,   211,
     211,   104,   105,   106,   211,   108,   211,   269,   111,   112,
     113,   114,   115,   212,   117,   212,   795,   750,   652,   214,
     799,    81,    82,   802,   803,   804,   213,   748,   807,   808,
     809,   210,     3,     4,   213,   216,  1112,   214,   141,  1115,
     143,   144,   676,   677,   765,   214,   680,   210,   210,   152,
     153,   154,   155,   156,   157,   210,   194,   195,   196,   197,
     198,   270,   200,   201,   202,   203,   204,   205,   206,   215,
     209,   212,   175,   210,   212,   215,     4,   264,   265,   266,
     267,   268,   214,   210,   805,   210,   189,   266,   267,   268,
     212,   812,   212,   212,   212,   233,   234,   212,   201,   237,
     238,   272,     3,     3,   215,   243,   214,   214,   210,   212,
      81,    82,   216,   210,   174,   210,   210,   215,   264,   265,
     266,   267,   268,   210,   264,   265,   266,   267,   268,   210,
     233,   234,     4,   210,   194,   195,   196,   197,   198,   214,
     200,   201,   202,   203,   204,   205,   206,   210,   212,   783,
       4,   210,   212,   264,   265,   266,   267,   268,   210,   793,
     263,   210,   215,   210,   210,   944,   264,   265,   266,   267,
     268,   210,     4,   233,   234,   210,   210,   210,     6,   900,
     210,   210,   210,   243,   210,     4,   210,   210,   210,   210,
     210,   210,  1113,   210,   210,   210,   210,  1118,    80,    81,
      82,   210,   210,   215,   210,   984,   985,   986,   987,   210,
    1131,   264,   265,   266,   267,   268,   210,   210,   213,   210,
       3,   213,   211,   194,   195,   196,   197,   198,   211,   200,
     201,   202,   203,   204,   205,   206,   211,   211,     6,   873,
       3,   212,   210,   215,   210,     5,    14,   210,   210,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,   233,   234,   219,    33,    34,    35,    36,    37,
     207,    39,   243,   213,    96,   210,    44,    45,    46,    47,
      48,    49,    50,    51,   215,    53,    54,    55,  1021,    57,
     210,    59,    60,    61,    62,    63,    64,   210,    80,    80,
      68,    69,   210,    71,    38,     4,   210,    38,    40,   175,
      78,    90,   194,   195,   196,   197,   198,   210,   200,   201,
     202,   203,   204,   205,   206,    90,   210,   210,    94,   179,
     212,    99,   127,   264,   265,   266,   267,   268,   100,   138,
     215,   215,   133,    91,   134,   215,   210,   981,   116,   217,
      98,   233,   234,     3,   122,   210,   213,   125,   126,     4,
     128,   243,   130,   210,    81,    82,   114,   213,   136,   271,
     210,   139,   140,   210,   210,   214,   210,   145,   146,   210,
     148,     4,   150,   131,   210,   218,   210,   210,  1121,   264,
     265,   266,   267,   268,   264,   265,   266,   267,   268,   228,
     218,   230,  1135,   232,   210,   234,   210,   236,   210,   238,
       4,   240,   210,   242,   214,   244,  1081,   185,   186,   187,
     188,   210,   190,   211,   210,     4,   210,   175,   176,   177,
     178,     4,   180,   181,   182,   183,   184,   210,   210,  1104,
     210,   210,    80,   210,     4,   127,   214,   211,     3,   210,
     270,   210,   220,   221,   210,    83,    84,   210,   210,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   194,   195,   196,
     197,   198,   210,   200,   201,   202,   203,   204,   205,   206,
     210,   221,   120,   121,   210,   212,   124,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   210,   233,   234,   211,   210,
       4,   213,   212,   210,     3,   213,   243,   155,    80,   210,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   221,   219,   210,   212,
     211,   210,   227,   213,   229,   211,   231,   211,   233,   211,
     235,   210,   237,   211,   239,   211,   241,   211,   243,   211,
     211,   211,   211,     5,   211,   211,   211,   211,   211,   210,
       4,   213,   175,   210,     4,   175,    90,    90,   210,   218,
     218,   210,    66,   210,   210,    97,    97,    97,   216,   210,
     213,   229,   230,   213,   213,   213,   103,   102,   142,   210,
     109,     4,   110,   210,   210,     4,   211,   215,   210,     4,
     211,   210,   210,   210,   210,     4,     4,   210,   210,   213,
       5,    41,   213,   211,   211,   211,   211,   211,     3,   211,
     211,   211,   210,   101,     4,   210,   213,   210,    67,   213,
      92,   213,   211,   211,   211,   211,     7,   211,   211,    81,
     211,   211,   211,   211,   210,     4,   211,   210,   132,   213,
     217,     4,   210,    98,   211,   210,     4,   210,     4,   211,
     210,   210,    97,     4,     4,   211,     5,    41,   213,   100,
     211,   271,   175,     4,   213,   175,   213,    92,   213,    91,
     210,   213,   208,   211,   213,   210,   213,    91,     5,  1134,
     410,    70,   658,   244,    63,   211,   210,   210,   210,   972,
     477,   831,   213,   540,   213,   943,   967,  1027,  1008,   178,
     220,  1086,  1022,   730,   812,  1050,   415,  1091,   509,   533,
     767,    -1,   278,   564
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   274,   275,   276,   277,   211,   211,     0,
     275,   118,   287,   218,   218,   211,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    33,    34,    35,    36,    37,    39,    44,    45,    46,
      47,    48,    49,    50,    51,    53,    54,    55,    57,    59,
      60,    61,    62,    63,    64,    68,    69,    71,    78,    99,
     116,   122,   125,   126,   128,   130,   136,   139,   140,   145,
     146,   148,   150,   185,   186,   187,   188,   190,   214,   220,
     221,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   278,
     279,   280,   281,   282,   289,   291,   292,   293,   294,   296,
     297,   305,   308,   309,   311,   312,   319,   321,   322,   324,
     326,   328,   332,   333,   334,   337,   339,   346,   353,   356,
     362,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   378,   379,   380,   381,   385,   386,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   399,   400,   401,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   210,   119,
     288,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   311,   211,   211,   211,
     211,   214,   221,   293,   295,   313,   211,   211,   211,   211,
     220,     4,   212,   227,   228,   263,   441,   451,   212,   289,
     269,   211,   323,     3,   210,   218,   229,   230,   427,   440,
      10,    11,    12,    13,     4,    96,   452,    81,    82,    85,
      86,   201,   212,   233,   234,   354,   429,   430,   100,   194,
     195,   196,   197,   198,   200,   202,   203,   204,   205,   206,
     212,   243,   429,   449,   450,     3,    81,    95,   212,   218,
     241,   242,   364,   438,   439,   364,     3,    81,   212,   218,
     239,   240,   358,   436,   437,    81,   218,   411,    72,    81,
     421,   429,     4,   429,   174,   450,   212,   235,   236,   432,
     433,   432,    56,   212,   459,   460,   218,   429,   429,    93,
     429,   212,   429,   459,   432,   432,    81,   218,   336,   412,
     459,   459,   459,   428,   441,   212,   335,   441,   429,   450,
     450,   429,   450,   450,     4,     8,     9,    81,   453,   429,
     450,   336,   450,     5,    81,   347,   329,   450,   432,   137,
     292,   428,   269,   214,   429,   429,   429,   429,   212,     8,
     441,   214,   192,   215,   264,   265,   266,   267,   268,   440,
     441,   446,   447,   448,    23,    25,    30,    31,    32,   128,
     199,   212,   216,   429,   432,   440,   441,   455,   456,   457,
     327,   216,   290,   288,   214,   270,   210,   210,   210,   209,
     431,   212,     4,   214,   210,   210,   341,   432,   432,   429,
     429,   212,   429,   212,   212,   212,   449,   212,   450,   272,
     210,     3,   214,   365,   363,     3,   214,   357,   210,   210,
     210,   210,   210,   210,     4,   214,   210,   210,   212,     4,
     210,   210,   210,   210,   210,   210,     4,   210,   210,   210,
       6,   310,   210,   210,   210,   210,     4,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   290,   330,   210,
     210,   210,   215,   441,   428,   210,   210,   210,   210,   306,
     213,   213,   441,   441,   441,   441,   441,   441,   441,   210,
     213,   211,     4,   301,   302,   303,   304,   430,   433,   440,
     441,   211,   211,   211,   291,   289,   441,   427,     3,     3,
     212,   387,     3,   210,   215,   450,   210,   441,    86,    87,
     355,     5,   219,   210,   210,    80,   349,   450,   429,   429,
       3,     7,   212,   237,   238,   434,   435,   450,   207,   213,
     450,    96,   210,   441,   210,   290,   210,   441,   290,   429,
     429,   450,    80,   299,   437,    80,   210,   441,    73,    81,
     422,    38,     4,   210,    38,    40,    81,    83,    84,   423,
     426,   175,   429,    90,   210,    90,   459,    81,   212,   342,
     210,   459,   459,   459,   428,   210,   335,   453,   429,    76,
      81,   458,    94,    65,   182,   179,   100,    81,   212,   343,
       4,   100,   421,   331,   212,   434,   434,   138,   325,   147,
     270,   320,   215,    81,   440,   133,   134,   135,   137,   294,
     443,   444,   445,   215,   215,   446,   450,   210,   210,   217,
     210,   217,   210,   217,   210,   217,   216,   438,   216,   436,
     216,   434,   217,   215,     3,   210,   431,   213,     4,   215,
     341,   429,   429,   271,   210,   213,   210,   210,   210,     3,
     214,   210,   210,   210,   218,   215,     4,     8,     9,    58,
      72,    73,    74,    75,    77,    79,   104,   105,   106,   108,
     111,   112,   113,   114,   115,   117,   141,   143,   144,   152,
     153,   154,   155,   156,   157,   175,   189,   366,   429,   451,
     454,   218,   215,   359,   351,   210,   210,   210,     4,   215,
     210,   210,     4,   210,   214,   210,   211,   210,     4,   210,
       4,   210,   422,   210,   210,   377,   210,    80,   210,   210,
     211,   382,   210,     4,   210,   210,   210,   127,   210,   210,
     290,   270,   441,   314,   210,   210,   211,   210,   213,     4,
     433,   430,   441,   440,   298,   439,   299,   300,   435,   210,
       3,   213,   219,   210,   349,   450,   441,   441,   450,   210,
     441,   450,   450,   212,   213,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   213,   210,   210,   421,   437,     5,   210,   412,     4,
     210,   175,     4,    81,    88,   425,   436,   175,    90,   210,
      81,   212,   344,   218,    90,   428,   210,   440,    66,   429,
     210,    97,   210,    81,   212,   345,    97,    81,   348,   349,
     290,    97,   434,   216,   422,    81,   440,   223,   224,   442,
     443,   307,   210,   217,   217,   210,   217,   422,   210,   441,
     213,   210,   213,   210,   213,   422,   215,   213,   213,   441,
     428,   438,   103,   102,   428,   109,   110,   428,   428,   428,
     440,   142,   428,   428,   428,   436,    83,    84,   120,   121,
     124,   155,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   360,   424,
     426,   440,    77,   107,   108,   114,   123,   131,   157,   352,
     210,   285,     4,   210,   413,   210,     4,   211,   215,   210,
     211,     4,   354,   210,   210,     4,    81,   350,   210,   210,
      91,    98,   114,   131,   175,   176,   177,   178,   180,   181,
     182,   183,   184,   383,     4,     4,   413,   210,   210,   149,
     151,   315,   316,   290,   439,   435,   213,     5,    41,   441,
     213,   265,   211,   440,   211,   211,   211,   211,   211,   211,
     211,   211,     3,   210,    41,    42,    43,   414,   415,   101,
     416,     4,   210,   436,    89,   175,   436,   213,   210,   345,
     428,   213,    67,    92,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   213,   210,   416,   350,     7,   453,
     211,   217,   315,   210,   213,   210,   213,   441,    81,   428,
     428,   428,   428,   174,   218,   361,   438,     4,   132,     4,
     210,   211,    98,   418,   210,     4,   210,   211,   355,   413,
     210,   210,    97,   419,     4,     4,    92,    94,   132,   384,
     436,   438,   387,   387,   429,   411,     4,     4,   418,   351,
     211,   318,     5,    41,   210,   283,   271,   213,   414,   100,
     417,   211,   175,   420,     4,   213,   175,   436,   213,   416,
      92,    91,   213,   420,   317,   289,   210,   213,   208,   213,
     361,   419,   211,   397,   213,   211,   418,   210,   338,   289,
     422,   210,   436,   290,   398,   436,   420,    91,   290,   210,
     429,   340,     5,    81,   286,   210,   284,   290,   285,   429,
     213
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
#line 295 "lev_comp.y"
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
#line 318 "lev_comp.y"
    {
		      start_level_def(&splev, (yyvsp[(3) - (3)].map));
		      (yyval.map) = (yyvsp[(3) - (3)].map);
		  ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 323 "lev_comp.y"
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
#line 345 "lev_comp.y"
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
#line 358 "lev_comp.y"
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
#line 370 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_ROGUE,0,0,0,
					  0,0,0,0, SPO_INITLEVEL));
		  ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 376 "lev_comp.y"
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
#line 405 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_TILESET));
		  ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 411 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), (int)(yyvsp[(3) - (5)].i), SPO_FOREST));
		  ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 415 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(0, (int)(yyvsp[(3) - (3)].i), SPO_FOREST));
		  ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 421 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), (int)(yyvsp[(3) - (5)].i), SPO_MONSTER_GENERATION));
		  ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 427 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_BOUNDARY_TYPE));
		  ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 433 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 437 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 443 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_COPY));
		      (yyval.i) = 0;
		  ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 448 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 454 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 458 "lev_comp.y"
    {
		      (yyval.i) = what_map_char((char) (yyvsp[(2) - (2)].i));
		  ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 469 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 473 "lev_comp.y"
    {
		      add_opvars(splev, "io",
                                 VA_PASS2((int) (yyvsp[(3) - (3)].i), SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 480 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 484 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 490 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 494 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 500 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (3)].i);
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 601 "lev_comp.y"
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

  case 119:

/* Line 1455 of yacc.c  */
#line 617 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 623 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 629 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 635 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 641 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 647 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 653 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 659 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 665 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 675 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 685 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 695 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 705 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 715 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 725 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 737 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 742 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 749 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 754 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 761 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 766 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 773 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 777 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 783 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 788 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 795 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 799 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 805 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 809 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 815 "lev_comp.y"
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

  case 149:

/* Line 1455 of yacc.c  */
#line 837 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 841 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 852 "lev_comp.y"
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

  case 152:

/* Line 1455 of yacc.c  */
#line 907 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 913 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 917 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 923 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 930 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 934 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 942 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 946 "lev_comp.y"
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

  case 160:

/* Line 1455 of yacc.c  */
#line 969 "lev_comp.y"
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

  case 163:

/* Line 1455 of yacc.c  */
#line 1014 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1024 "lev_comp.y"
    {
		  ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1027 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1037 "lev_comp.y"
    {
		  ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1042 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1056 "lev_comp.y"
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

  case 171:

/* Line 1455 of yacc.c  */
#line 1093 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1098 "lev_comp.y"
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

  case 173:

/* Line 1455 of yacc.c  */
#line 1127 "lev_comp.y"
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

  case 174:

/* Line 1455 of yacc.c  */
#line 1141 "lev_comp.y"
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

  case 175:

/* Line 1455 of yacc.c  */
#line 1157 "lev_comp.y"
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

  case 176:

/* Line 1455 of yacc.c  */
#line 1177 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1189 "lev_comp.y"
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

  case 178:

/* Line 1455 of yacc.c  */
#line 1209 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1215 "lev_comp.y"
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
#line 1225 "lev_comp.y"
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

  case 181:

/* Line 1455 of yacc.c  */
#line 1243 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1253 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1259 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1264 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1269 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1276 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1283 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1292 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1300 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1311 "lev_comp.y"
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

  case 191:

/* Line 1455 of yacc.c  */
#line 1327 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1334 "lev_comp.y"
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

  case 193:

/* Line 1455 of yacc.c  */
#line 1350 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1357 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1361 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1367 "lev_comp.y"
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

  case 197:

/* Line 1455 of yacc.c  */
#line 1377 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1383 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1392 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1398 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1403 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1409 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1414 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1420 "lev_comp.y"
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

  case 205:

/* Line 1455 of yacc.c  */
#line 1431 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1445 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1449 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1459 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1466 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1474 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1479 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1484 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1489 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1494 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1499 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1504 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1512 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1519 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1527 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1543 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1547 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1553 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1561 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1567 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1575 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1583 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1588 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1594 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1600 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1606 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1612 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1617 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(0, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1622 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1627 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1632 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1637 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1642 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1647 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1652 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1657 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1662 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1667 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1673 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1678 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_PROTECTOR));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1683 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_MAXHP));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1688 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_LEVEL_ADJUSTMENT));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1693 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_KEEP_ORIGINAL_INVENTORY));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1700 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1709 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1713 "lev_comp.y"
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

  case 261:

/* Line 1455 of yacc.c  */
#line 1726 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1734 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1744 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1752 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1761 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1768 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1776 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1782 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1787 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1792 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1797 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1802 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1807 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1812 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1817 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1822 "lev_comp.y"
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

  case 277:

/* Line 1455 of yacc.c  */
#line 1836 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1842 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1847 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1852 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1857 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1862 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1867 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1872 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1877 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1882 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1887 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1892 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1897 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1902 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x400000;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1907 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x800000;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1912 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x1000000;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1917 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_MYTHIC_TYPE));
		      (yyval.i) = 0x2000000;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1923 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x4000000;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1928 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x8000000;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1933 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x10000000;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1940 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1946 "lev_comp.y"
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

  case 299:

/* Line 1455 of yacc.c  */
#line 1975 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1980 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1988 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 1994 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 1998 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2006 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2013 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2020 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2030 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2040 "lev_comp.y"
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

  case 309:

/* Line 1455 of yacc.c  */
#line 2056 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2066 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2070 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2076 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2082 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2088 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2092 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2098 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2104 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2111 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2119 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2125 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2130 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2135 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2140 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2145 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2151 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2157 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2162 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2167 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2172 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2177 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2182 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2192 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2198 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2204 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2209 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2216 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2223 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2227 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2233 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2239 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2245 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2250 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2257 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2263 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2273 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2279 "lev_comp.y"
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

  case 351:

/* Line 1455 of yacc.c  */
#line 2298 "lev_comp.y"
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

  case 352:

/* Line 1455 of yacc.c  */
#line 2310 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2314 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2320 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2325 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2330 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (long)(yyvsp[(9) - (9)].i), (long)(yyvsp[(7) - (9)].i), (long)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2335 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((long)(yyvsp[(9) - (13)].i), (long)(yyvsp[(7) - (13)].i), (long)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2341 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2345 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2351 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2357 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2361 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2367 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2371 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2377 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2381 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2386 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2393 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2397 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2404 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2408 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2413 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2420 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2424 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2429 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2437 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2443 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2450 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2454 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2461 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2472 "lev_comp.y"
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

  case 384:

/* Line 1455 of yacc.c  */
#line 2485 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2489 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2495 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2499 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2506 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2510 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2514 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2520 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2524 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2530 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2536 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2540 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2546 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2552 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2557 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2574 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2582 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2592 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2601 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2606 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2613 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2624 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2630 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2634 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2638 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2645 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2655 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2662 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2666 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2672 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2676 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2684 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2688 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2695 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2705 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2718 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2722 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2729 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2739 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2748 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2759 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2763 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2770 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2780 "lev_comp.y"
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

  case 438:

/* Line 1455 of yacc.c  */
#line 2791 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2800 "lev_comp.y"
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

  case 440:

/* Line 1455 of yacc.c  */
#line 2810 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2816 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2820 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2827 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2837 "lev_comp.y"
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

  case 445:

/* Line 1455 of yacc.c  */
#line 2848 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2857 "lev_comp.y"
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

  case 447:

/* Line 1455 of yacc.c  */
#line 2867 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2871 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2877 "lev_comp.y"
    { ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2879 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2885 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2889 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2893 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2897 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2905 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2914 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2918 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2922 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2926 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2930 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2933 "lev_comp.y"
    { ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2937 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2944 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2953 "lev_comp.y"
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

  case 469:

/* Line 1455 of yacc.c  */
#line 2994 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2998 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 3005 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 3012 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3022 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3026 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3034 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3038 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3042 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3046 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3050 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3055 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3059 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3063 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3068 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3073 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3078 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3082 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3087 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3092 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3096 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3100 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3105 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3109 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3116 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3122 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3126 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3132 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3148 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3152 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3156 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3160 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3182 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 3186 "lev_comp.y"
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

  case 516:

/* Line 1455 of yacc.c  */
#line 3212 "lev_comp.y"
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
#line 7284 "lev_comp.tab.c"
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
#line 3240 "lev_comp.y"


/*lev_comp.y*/

