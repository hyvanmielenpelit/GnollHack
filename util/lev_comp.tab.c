
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

/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2021-09-14 */

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
     ENGRAVING_TYPE = 330,
     KEYTYPE_ID = 331,
     LEVER_ID = 332,
     NO_PICKUP_ID = 333,
     DIRECTION = 334,
     RANDOM_TYPE = 335,
     RANDOM_TYPE_BRACKET = 336,
     A_REGISTER = 337,
     ALIGNMENT = 338,
     LEFT_OR_RIGHT = 339,
     CENTER = 340,
     TOP_OR_BOT = 341,
     ALTAR_TYPE = 342,
     ALTAR_SUBTYPE = 343,
     UP_OR_DOWN = 344,
     ACTIVE_OR_INACTIVE = 345,
     MODRON_PORTAL_TYPE = 346,
     NPC_TYPE = 347,
     FOUNTAIN_TYPE = 348,
     SPECIAL_OBJECT_TYPE = 349,
     CMAP_TYPE = 350,
     FLOOR_SUBTYPE = 351,
     FLOOR_SUBTYPE_ID = 352,
     FLOOR_ID = 353,
     FLOOR_TYPE = 354,
     FLOOR_TYPE_ID = 355,
     ELEMENTAL_ENCHANTMENT_TYPE = 356,
     EXCEPTIONALITY_TYPE = 357,
     EXCEPTIONALITY_ID = 358,
     ELEMENTAL_ENCHANTMENT_ID = 359,
     ENCHANTMENT_ID = 360,
     SECRET_DOOR_ID = 361,
     USES_UP_KEY_ID = 362,
     MYTHIC_PREFIX_TYPE = 363,
     MYTHIC_SUFFIX_TYPE = 364,
     MYTHIC_PREFIX_ID = 365,
     MYTHIC_SUFFIX_ID = 366,
     CHARGES_ID = 367,
     SPECIAL_QUALITY_ID = 368,
     SPEFLAGS_ID = 369,
     SUBROOM_ID = 370,
     NAME_ID = 371,
     FLAGS_ID = 372,
     FLAG_TYPE = 373,
     MON_ATTITUDE = 374,
     MON_ALERTNESS = 375,
     SUBTYPE_ID = 376,
     NON_PASSDOOR_ID = 377,
     MON_APPEARANCE = 378,
     ROOMDOOR_ID = 379,
     IF_ID = 380,
     ELSE_ID = 381,
     TERRAIN_ID = 382,
     HORIZ_OR_VERT = 383,
     REPLACE_TERRAIN_ID = 384,
     LOCATION_SUBTYPE_ID = 385,
     DOOR_SUBTYPE = 386,
     BRAZIER_SUBTYPE = 387,
     SIGNPOST_SUBTYPE = 388,
     TREE_SUBTYPE = 389,
     FOREST_ID = 390,
     FOREST_TYPE = 391,
     INITIALIZE_TYPE = 392,
     EXIT_ID = 393,
     SHUFFLE_ID = 394,
     MANUAL_TYPE_ID = 395,
     MANUAL_TYPE = 396,
     QUANTITY_ID = 397,
     BURIED_ID = 398,
     LOOP_ID = 399,
     FOR_ID = 400,
     TO_ID = 401,
     SWITCH_ID = 402,
     CASE_ID = 403,
     BREAK_ID = 404,
     DEFAULT_ID = 405,
     ERODED_ID = 406,
     TRAPPED_STATE = 407,
     RECHARGED_ID = 408,
     INVIS_ID = 409,
     GREASED_ID = 410,
     INDESTRUCTIBLE_ID = 411,
     FEMALE_ID = 412,
     MALE_ID = 413,
     WAITFORU_ID = 414,
     PROTECTOR_ID = 415,
     CANCELLED_ID = 416,
     REVIVED_ID = 417,
     AVENGE_ID = 418,
     FLEEING_ID = 419,
     BLINDED_ID = 420,
     PARALYZED_ID = 421,
     STUNNED_ID = 422,
     CONFUSED_ID = 423,
     SEENTRAPS_ID = 424,
     ALL_ID = 425,
     MONTYPE_ID = 426,
     OBJTYPE_ID = 427,
     TERTYPE_ID = 428,
     TERTYPE2_ID = 429,
     LEVER_EFFECT_TYPE = 430,
     SWITCHABLE_ID = 431,
     CONTINUOUSLY_USABLE_ID = 432,
     TARGET_ID = 433,
     TRAPTYPE_ID = 434,
     EFFECT_FLAG_ID = 435,
     GRAVE_ID = 436,
     BRAZIER_ID = 437,
     SIGNPOST_ID = 438,
     TREE_ID = 439,
     ERODEPROOF_ID = 440,
     FUNCTION_ID = 441,
     MSG_OUTPUT_TYPE = 442,
     COMPARE_TYPE = 443,
     UNKNOWN_TYPE = 444,
     rect_ID = 445,
     fillrect_ID = 446,
     line_ID = 447,
     randline_ID = 448,
     grow_ID = 449,
     selection_ID = 450,
     flood_ID = 451,
     rndcoord_ID = 452,
     circle_ID = 453,
     ellipse_ID = 454,
     filter_ID = 455,
     complement_ID = 456,
     gradient_ID = 457,
     GRADIENT_TYPE = 458,
     LIMITED = 459,
     HUMIDITY_TYPE = 460,
     STRING = 461,
     MAP_ID = 462,
     NQSTRING = 463,
     VARSTRING = 464,
     CFUNC = 465,
     CFUNC_INT = 466,
     CFUNC_STR = 467,
     CFUNC_COORD = 468,
     CFUNC_REGION = 469,
     VARSTRING_INT = 470,
     VARSTRING_INT_ARRAY = 471,
     VARSTRING_STRING = 472,
     VARSTRING_STRING_ARRAY = 473,
     VARSTRING_VAR = 474,
     VARSTRING_VAR_ARRAY = 475,
     VARSTRING_COORD = 476,
     VARSTRING_COORD_ARRAY = 477,
     VARSTRING_REGION = 478,
     VARSTRING_REGION_ARRAY = 479,
     VARSTRING_MAPCHAR = 480,
     VARSTRING_MAPCHAR_ARRAY = 481,
     VARSTRING_MONST = 482,
     VARSTRING_MONST_ARRAY = 483,
     VARSTRING_OBJ = 484,
     VARSTRING_OBJ_ARRAY = 485,
     VARSTRING_SEL = 486,
     VARSTRING_SEL_ARRAY = 487,
     METHOD_INT = 488,
     METHOD_INT_ARRAY = 489,
     METHOD_STRING = 490,
     METHOD_STRING_ARRAY = 491,
     METHOD_VAR = 492,
     METHOD_VAR_ARRAY = 493,
     METHOD_COORD = 494,
     METHOD_COORD_ARRAY = 495,
     METHOD_REGION = 496,
     METHOD_REGION_ARRAY = 497,
     METHOD_MAPCHAR = 498,
     METHOD_MAPCHAR_ARRAY = 499,
     METHOD_MONST = 500,
     METHOD_MONST_ARRAY = 501,
     METHOD_OBJ = 502,
     METHOD_OBJ_ARRAY = 503,
     METHOD_SEL = 504,
     METHOD_SEL_ARRAY = 505,
     DICE = 506
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
#line 551 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 563 "lev_comp.tab.c"

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
#define YYLAST   1297

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  269
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  188
/* YYNRULES -- Number of rules.  */
#define YYNRULES  512
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1135

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   506

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   264,   268,     2,
     208,   209,   262,   260,   206,   261,   266,   263,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   207,     2,
       2,   265,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   210,     2,   211,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   212,   267,   213,     2,     2,     2,     2,
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
     205,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259
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
     778,   780,   782,   786,   788,   790,   792,   794,   798,   802,
     803,   809,   812,   813,   817,   819,   823,   825,   829,   833,
     835,   837,   841,   843,   845,   847,   851,   853,   855,   857,
     861,   865,   869,   873,   877,   881,   885,   889,   891,   893,
     895,   899,   903,   907,   913,   921,   927,   936,   938,   942,
     946,   952,   958,   966,   974,   981,   987,   988,   991,   997,
    1001,  1013,  1027,  1034,  1035,  1039,  1041,  1045,  1049,  1053,
    1057,  1059,  1061,  1065,  1069,  1073,  1077,  1081,  1085,  1087,
    1089,  1091,  1093,  1097,  1101,  1103,  1109,  1119,  1125,  1133,
    1137,  1141,  1147,  1157,  1163,  1169,  1175,  1176,  1190,  1191,
    1193,  1201,  1213,  1223,  1237,  1241,  1249,  1257,  1263,  1271,
    1277,  1287,  1293,  1299,  1303,  1311,  1315,  1323,  1331,  1335,
    1341,  1347,  1351,  1357,  1365,  1375,  1377,  1379,  1381,  1383,
    1385,  1386,  1389,  1391,  1395,  1397,  1399,  1401,  1402,  1406,
    1408,  1409,  1413,  1415,  1416,  1420,  1422,  1424,  1426,  1428,
    1430,  1432,  1434,  1436,  1438,  1442,  1444,  1446,  1451,  1453,
    1455,  1460,  1462,  1464,  1469,  1471,  1476,  1482,  1484,  1488,
    1490,  1494,  1496,  1498,  1503,  1513,  1515,  1517,  1522,  1524,
    1530,  1532,  1534,  1539,  1541,  1543,  1549,  1551,  1553,  1555,
    1560,  1562,  1564,  1570,  1572,  1574,  1576,  1580,  1582,  1584,
    1588,  1590,  1595,  1599,  1603,  1607,  1611,  1615,  1619,  1621,
    1623,  1627,  1629,  1633,  1634,  1636,  1638,  1640,  1642,  1646,
    1647,  1649,  1651,  1654,  1657,  1662,  1669,  1674,  1681,  1688,
    1695,  1702,  1705,  1712,  1721,  1730,  1741,  1756,  1759,  1761,
    1765,  1767,  1771,  1773,  1775,  1777,  1779,  1781,  1783,  1785,
    1787,  1789,  1791,  1793,  1795,  1797,  1799,  1801,  1803,  1805,
    1807,  1809,  1820
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     270,     0,    -1,    -1,   271,    -1,   272,    -1,   272,   271,
      -1,   273,   283,   285,    -1,    16,   207,   214,    -1,    15,
     207,   214,   206,     3,    -1,    17,   207,    11,   206,   383,
      -1,    17,   207,    10,   206,     3,    -1,    17,   207,    13,
      -1,    17,   207,    12,   206,     3,   206,     3,   206,     5,
     206,     5,   206,   418,   206,   282,   281,    -1,    18,   207,
     448,    -1,   135,   207,   136,   206,   137,    -1,   135,   207,
     136,    -1,    29,   207,     4,   206,   295,    -1,    21,   207,
      99,    -1,    -1,   206,   204,    -1,    -1,   206,   425,    -1,
      -1,   206,     3,    -1,     5,    -1,    80,    -1,    -1,   117,
     207,   284,    -1,   118,   206,   284,    -1,   118,    -1,    -1,
     287,   285,    -1,   212,   285,   213,    -1,   328,    -1,   274,
      -1,   275,    -1,   391,    -1,   278,    -1,   276,    -1,   398,
      -1,   277,    -1,   397,    -1,   395,    -1,   396,    -1,   399,
      -1,   400,    -1,   401,    -1,   402,    -1,   403,    -1,   372,
      -1,   330,    -1,   293,    -1,   292,    -1,   386,    -1,   342,
      -1,   364,    -1,   405,    -1,   406,    -1,   374,    -1,   375,
      -1,   376,    -1,   404,    -1,   308,    -1,   318,    -1,   320,
      -1,   324,    -1,   322,    -1,   305,    -1,   315,    -1,   301,
      -1,   304,    -1,   367,    -1,   377,    -1,   349,    -1,   365,
      -1,   352,    -1,   358,    -1,   387,    -1,   382,    -1,   370,
      -1,   329,    -1,   388,    -1,   389,    -1,   390,    -1,   392,
      -1,   335,    -1,   333,    -1,   381,    -1,   385,    -1,   384,
      -1,   368,    -1,   369,    -1,   371,    -1,   363,    -1,   366,
      -1,   224,    -1,   226,    -1,   228,    -1,   230,    -1,   232,
      -1,   234,    -1,   236,    -1,   238,    -1,   240,    -1,   223,
      -1,   225,    -1,   227,    -1,   229,    -1,   231,    -1,   233,
      -1,   235,    -1,   237,    -1,   239,    -1,   288,    -1,   289,
      -1,   217,    -1,   217,    -1,   289,    -1,   139,   207,   288,
      -1,   290,   265,   437,    -1,   290,   265,   195,   207,   446,
      -1,   290,   265,   436,    -1,   290,   265,   453,   207,   430,
      -1,   290,   265,   452,   207,   432,    -1,   290,   265,   451,
     207,   434,    -1,   290,   265,   425,    -1,   290,   265,   428,
      -1,   290,   265,   212,   299,   213,    -1,   290,   265,   212,
     298,   213,    -1,   290,   265,   212,   297,   213,    -1,   290,
     265,   453,   207,   212,   296,   213,    -1,   290,   265,   452,
     207,   212,   295,   213,    -1,   290,   265,   451,   207,   212,
     294,   213,    -1,   290,   265,   212,   300,   213,    -1,   435,
      -1,   294,   206,   435,    -1,   433,    -1,   295,   206,   433,
      -1,   431,    -1,   296,   206,   431,    -1,   429,    -1,   297,
     206,   429,    -1,   426,    -1,   298,   206,   426,    -1,   437,
      -1,   299,   206,   437,    -1,   436,    -1,   300,   206,   436,
      -1,    -1,    -1,   186,   216,   208,   302,   441,   209,   303,
     286,    -1,   216,   208,   444,   209,    -1,   138,    -1,    -1,
       6,    -1,     6,    -1,   210,   437,   188,   437,   211,    -1,
     210,   437,   211,    -1,    -1,    -1,   147,   309,   210,   424,
     211,   310,   212,   311,   213,    -1,    -1,   312,   311,    -1,
      -1,   148,   449,   207,   313,   285,    -1,    -1,   150,   207,
     314,   285,    -1,   149,    -1,   266,   266,    -1,   146,    -1,
     145,   291,   265,   437,   316,   437,    -1,    -1,   317,   319,
     286,    -1,    -1,   144,   210,   424,   211,   321,   286,    -1,
      -1,   307,   207,   323,   287,    -1,    -1,   125,   307,   325,
     326,    -1,   286,    -1,    -1,   286,   327,   126,   286,    -1,
      14,   207,   436,    -1,    71,    -1,    71,   207,   449,    -1,
      71,   207,    80,    -1,    59,   207,   331,   206,   331,    -1,
      59,   207,   331,   206,   449,    -1,   208,     4,   206,    79,
     206,   346,   209,    -1,   408,   306,   206,   418,    -1,    -1,
     115,   207,   332,   206,   339,   206,   341,   409,   412,   414,
     416,   334,   286,    -1,    -1,    51,   207,   332,   206,   338,
     206,   340,   206,   341,   409,   412,   414,   416,   336,   286,
      -1,    -1,   206,     5,    -1,   208,     4,   206,     4,   209,
      -1,    80,    -1,   208,     4,   206,     4,   209,    -1,    80,
      -1,   208,   350,   206,   351,   209,    -1,    80,    -1,   208,
       4,   206,     4,   209,    -1,    80,    -1,   124,   207,   343,
     206,   417,   206,   344,   206,   346,   347,    -1,    27,   207,
     417,   206,   446,   347,    -1,     5,    -1,    80,    -1,   345,
      -1,    80,    -1,    79,    -1,    79,   267,   345,    -1,     4,
      -1,    80,    -1,    -1,   347,   206,   348,    -1,   130,   207,
     131,    -1,    76,   207,   434,    -1,   113,   207,     4,    -1,
     156,    -1,   106,    -1,   107,    -1,   122,    -1,    20,    -1,
      19,   207,   350,   206,   351,   337,   215,    -1,    19,   207,
     425,   337,   215,    -1,    84,    -1,    85,    -1,    86,    -1,
      85,    -1,    25,   207,   354,    -1,    -1,    25,   207,   354,
     353,   286,    -1,   432,   206,   425,   355,    -1,    -1,   355,
     206,   356,    -1,   436,    -1,   119,    -1,   120,    -1,   420,
      -1,   123,   436,    -1,   157,    -1,   158,    -1,   154,    -1,
     161,    -1,   162,    -1,   163,    -1,   164,   207,   424,    -1,
     165,   207,   424,    -1,   166,   207,   424,    -1,   167,    -1,
     168,    -1,   169,   207,   357,    -1,   159,    -1,   160,    -1,
     214,    -1,   170,    -1,   214,   267,   357,    -1,    23,   207,
     360,    -1,    -1,    24,   207,   360,   359,   286,    -1,   434,
     361,    -1,    -1,   361,   206,   362,    -1,    74,    -1,   171,
     207,   432,    -1,   450,    -1,   116,   207,   436,    -1,   142,
     207,   424,    -1,   143,    -1,    73,    -1,   151,   207,   424,
      -1,   185,    -1,    72,    -1,   152,    -1,   153,   207,   424,
      -1,   154,    -1,   155,    -1,   425,    -1,   104,   207,   101,
      -1,   103,   207,   102,    -1,   105,   207,   424,    -1,   112,
     207,   424,    -1,   113,   207,   424,    -1,   140,   207,   141,
      -1,   114,   207,   424,    -1,    76,   207,   434,    -1,   156,
      -1,   107,    -1,    78,    -1,   110,   207,   108,    -1,   111,
     207,   109,    -1,    58,   207,   424,    -1,    26,   207,   407,
     206,   425,    -1,    28,   207,   425,   206,    79,   206,   417,
      -1,    33,   207,   425,   206,    79,    -1,    33,   207,   425,
     206,    79,   206,     5,   281,    -1,    34,    -1,    34,   207,
     446,    -1,    34,   207,   170,    -1,    47,   207,   425,   206,
      89,    -1,    48,   207,   425,   206,    89,    -1,    48,   207,
     455,   206,   455,   206,    89,    -1,    53,   207,   455,   206,
     455,   206,   214,    -1,    54,   207,   455,   206,   455,   373,
      -1,    55,   207,   455,   206,   455,    -1,    -1,   206,    89,
      -1,    62,   207,   446,   206,    93,    -1,    63,   207,   446,
      -1,    64,   207,   425,   206,   178,   207,   425,   206,    91,
     206,    90,    -1,    64,   207,   425,   206,    65,   206,    66,
     206,    67,   206,    91,   206,    90,    -1,    77,   207,   425,
     206,   175,   378,    -1,    -1,   378,   206,   379,    -1,    90,
      -1,   171,   207,   432,    -1,   172,   207,   434,    -1,   173,
     207,   383,    -1,   174,   207,   383,    -1,   176,    -1,   177,
      -1,   178,   207,   425,    -1,   179,   207,   407,    -1,   130,
     207,   380,    -1,    97,   207,   415,    -1,   180,   207,     4,
      -1,   113,   207,     4,    -1,     4,    -1,    91,    -1,   131,
      -1,    93,    -1,    69,   207,   446,    -1,    68,   207,   446,
      -1,     3,    -1,   208,     3,   206,   418,   209,    -1,   129,
     207,   428,   206,   430,   206,   430,   206,     7,    -1,   127,
     207,   446,   206,   430,    -1,   127,   207,   446,   206,   430,
     206,    96,    -1,    49,   207,   428,    -1,    50,   207,   428,
      -1,    39,   207,   214,   206,    40,    -1,    39,   207,   214,
     206,    40,   206,   171,   207,   432,    -1,    36,   207,   428,
     206,    38,    -1,    37,   207,   455,   206,    38,    -1,    22,
     207,   446,   206,    95,    -1,    -1,    35,   207,   428,   206,
     418,   206,   408,   409,   412,   414,   416,   393,   394,    -1,
      -1,   286,    -1,    44,   207,   425,   206,   419,   206,   421,
      -1,    44,   207,   425,   206,   419,   206,   421,   206,   171,
     207,   432,    -1,    44,   207,   425,   206,   419,   206,   421,
     206,    88,    -1,    44,   207,   425,   206,   419,   206,   421,
     206,    88,   206,   171,   207,   432,    -1,    45,   207,   425,
      -1,    45,   207,   425,   206,   171,   207,   432,    -1,    98,
     207,   446,   206,    99,   206,    96,    -1,   121,   207,   446,
     206,     4,    -1,   121,   207,   446,   206,    99,   206,    96,
      -1,    46,   207,    92,   206,   425,    -1,    46,   207,    92,
     206,   425,   206,   171,   207,   432,    -1,   181,   207,   425,
     206,   436,    -1,   181,   207,   425,   206,    80,    -1,   181,
     207,   425,    -1,   182,   207,   425,   206,   132,   206,   418,
      -1,   182,   207,   425,    -1,   183,   207,   425,   206,   133,
     206,   436,    -1,   183,   207,   425,   206,   133,   206,    80,
      -1,   183,   207,   425,    -1,   184,   207,   425,   206,   134,
      -1,   184,   207,   425,   206,   136,    -1,   184,   207,   425,
      -1,    60,   207,   437,   206,   425,    -1,    61,   207,   425,
     206,   454,   206,   436,    -1,    57,   207,   424,   206,   424,
     206,   424,   206,   424,    -1,    57,    -1,   214,    -1,    80,
      -1,   214,    -1,    80,    -1,    -1,   206,   410,    -1,   411,
      -1,   411,   206,   410,    -1,    41,    -1,    42,    -1,    43,
      -1,    -1,   100,   207,   413,    -1,    99,    -1,    -1,    97,
     207,   415,    -1,    96,    -1,    -1,   171,   207,   432,    -1,
      72,    -1,    80,    -1,    73,    -1,    80,    -1,    83,    -1,
     422,    -1,    80,    -1,    83,    -1,   422,    -1,    82,   207,
      80,    -1,    87,    -1,    80,    -1,    82,   210,     4,   211,
      -1,   214,    -1,   225,    -1,   226,   210,   437,   211,    -1,
     437,    -1,   426,    -1,   197,   208,   446,   209,    -1,   229,
      -1,   230,   210,   437,   211,    -1,   208,     4,   206,     4,
     209,    -1,    80,    -1,    81,   427,   211,    -1,   205,    -1,
     205,   206,   427,    -1,   429,    -1,   231,    -1,   232,   210,
     437,   211,    -1,   208,     4,   206,     4,   206,     4,   206,
       4,   209,    -1,   431,    -1,   233,    -1,   234,   210,   437,
     211,    -1,     3,    -1,   208,     3,   206,   418,   209,    -1,
     433,    -1,   235,    -1,   236,   210,   437,   211,    -1,   214,
      -1,     3,    -1,   208,     3,   206,   214,   209,    -1,    80,
      -1,   435,    -1,   237,    -1,   238,   210,   437,   211,    -1,
     214,    -1,     3,    -1,   208,     3,   206,   214,   209,    -1,
      80,    -1,    94,    -1,   423,    -1,   436,   266,   423,    -1,
       4,    -1,   447,    -1,   208,     8,   209,    -1,   223,    -1,
     224,   210,   437,   211,    -1,   437,   260,   437,    -1,   437,
     261,   437,    -1,   437,   262,   437,    -1,   437,   263,   437,
      -1,   437,   264,   437,    -1,   208,   437,   209,    -1,   219,
      -1,   220,    -1,   290,   207,   438,    -1,   439,    -1,   440,
     206,   439,    -1,    -1,   440,    -1,   437,    -1,   436,    -1,
     442,    -1,   443,   206,   442,    -1,    -1,   443,    -1,   425,
      -1,   190,   428,    -1,   191,   428,    -1,   192,   425,   206,
     425,    -1,   193,   425,   206,   425,   206,   437,    -1,   194,
     208,   446,   209,    -1,   194,   208,   345,   206,   446,   209,
      -1,   200,   208,     7,   206,   446,   209,    -1,   200,   208,
     446,   206,   446,   209,    -1,   200,   208,   430,   206,   446,
     209,    -1,   196,   425,    -1,   198,   208,   425,   206,   437,
     209,    -1,   198,   208,   425,   206,   437,   206,    41,   209,
      -1,   199,   208,   425,   206,   437,   206,   437,   209,    -1,
     199,   208,   425,   206,   437,   206,   437,   206,    41,   209,
      -1,   202,   208,   203,   206,   208,   437,   261,   437,   279,
     209,   206,   425,   280,   209,    -1,   201,   445,    -1,   239,
      -1,   208,   446,   209,    -1,   445,    -1,   445,   268,   446,
      -1,   259,    -1,    95,    -1,     4,    -1,     8,    -1,     9,
      -1,     4,    -1,     8,    -1,     9,    -1,     4,    -1,   447,
      -1,    30,    -1,    23,    -1,    31,    -1,    25,    -1,    32,
      -1,   127,    -1,    75,    -1,    80,    -1,   456,    -1,    56,
     208,     4,   206,     4,   206,     4,   206,     4,   209,    -1,
     208,     4,   206,     4,   206,     4,   206,     4,   209,    -1
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
    1656,  1661,  1666,  1672,  1677,  1684,  1693,  1697,  1710,  1719,
    1718,  1736,  1746,  1752,  1760,  1766,  1771,  1776,  1781,  1786,
    1791,  1796,  1801,  1806,  1820,  1826,  1831,  1836,  1841,  1846,
    1851,  1856,  1861,  1866,  1871,  1876,  1881,  1886,  1891,  1896,
    1901,  1906,  1911,  1918,  1924,  1953,  1958,  1966,  1972,  1976,
    1984,  1991,  1998,  2008,  2018,  2033,  2044,  2047,  2053,  2059,
    2065,  2069,  2075,  2082,  2088,  2096,  2102,  2107,  2112,  2117,
    2122,  2128,  2134,  2139,  2144,  2149,  2154,  2159,  2166,  2166,
    2166,  2166,  2169,  2175,  2181,  2186,  2193,  2200,  2204,  2210,
    2216,  2222,  2227,  2234,  2240,  2250,  2257,  2256,  2288,  2291,
    2297,  2302,  2307,  2312,  2318,  2322,  2328,  2334,  2338,  2344,
    2348,  2354,  2358,  2363,  2370,  2374,  2381,  2385,  2390,  2397,
    2401,  2406,  2414,  2420,  2427,  2431,  2438,  2446,  2449,  2459,
    2463,  2466,  2472,  2476,  2483,  2487,  2491,  2498,  2501,  2507,
    2514,  2517,  2523,  2530,  2534,  2541,  2542,  2545,  2546,  2549,
    2550,  2551,  2557,  2558,  2559,  2565,  2566,  2569,  2578,  2583,
    2590,  2601,  2607,  2611,  2615,  2622,  2632,  2639,  2643,  2649,
    2653,  2661,  2665,  2672,  2682,  2695,  2699,  2706,  2716,  2725,
    2736,  2740,  2747,  2757,  2768,  2777,  2787,  2793,  2797,  2804,
    2814,  2825,  2834,  2844,  2848,  2855,  2856,  2862,  2866,  2870,
    2874,  2882,  2891,  2895,  2899,  2903,  2907,  2911,  2914,  2921,
    2930,  2963,  2964,  2967,  2968,  2971,  2975,  2982,  2989,  3000,
    3003,  3011,  3015,  3019,  3023,  3027,  3032,  3036,  3040,  3045,
    3050,  3055,  3059,  3064,  3069,  3073,  3077,  3082,  3086,  3093,
    3099,  3103,  3109,  3116,  3117,  3120,  3121,  3122,  3125,  3129,
    3133,  3137,  3143,  3144,  3147,  3148,  3151,  3152,  3155,  3156,
    3159,  3163,  3189
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
  "ENGRAVING_TYPE", "KEYTYPE_ID", "LEVER_ID", "NO_PICKUP_ID", "DIRECTION",
  "RANDOM_TYPE", "RANDOM_TYPE_BRACKET", "A_REGISTER", "ALIGNMENT",
  "LEFT_OR_RIGHT", "CENTER", "TOP_OR_BOT", "ALTAR_TYPE", "ALTAR_SUBTYPE",
  "UP_OR_DOWN", "ACTIVE_OR_INACTIVE", "MODRON_PORTAL_TYPE", "NPC_TYPE",
  "FOUNTAIN_TYPE", "SPECIAL_OBJECT_TYPE", "CMAP_TYPE", "FLOOR_SUBTYPE",
  "FLOOR_SUBTYPE_ID", "FLOOR_ID", "FLOOR_TYPE", "FLOOR_TYPE_ID",
  "ELEMENTAL_ENCHANTMENT_TYPE", "EXCEPTIONALITY_TYPE", "EXCEPTIONALITY_ID",
  "ELEMENTAL_ENCHANTMENT_ID", "ENCHANTMENT_ID", "SECRET_DOOR_ID",
  "USES_UP_KEY_ID", "MYTHIC_PREFIX_TYPE", "MYTHIC_SUFFIX_TYPE",
  "MYTHIC_PREFIX_ID", "MYTHIC_SUFFIX_ID", "CHARGES_ID",
  "SPECIAL_QUALITY_ID", "SPEFLAGS_ID", "SUBROOM_ID", "NAME_ID", "FLAGS_ID",
  "FLAG_TYPE", "MON_ATTITUDE", "MON_ALERTNESS", "SUBTYPE_ID",
  "NON_PASSDOOR_ID", "MON_APPEARANCE", "ROOMDOOR_ID", "IF_ID", "ELSE_ID",
  "TERRAIN_ID", "HORIZ_OR_VERT", "REPLACE_TERRAIN_ID",
  "LOCATION_SUBTYPE_ID", "DOOR_SUBTYPE", "BRAZIER_SUBTYPE",
  "SIGNPOST_SUBTYPE", "TREE_SUBTYPE", "FOREST_ID", "FOREST_TYPE",
  "INITIALIZE_TYPE", "EXIT_ID", "SHUFFLE_ID", "MANUAL_TYPE_ID",
  "MANUAL_TYPE", "QUANTITY_ID", "BURIED_ID", "LOOP_ID", "FOR_ID", "TO_ID",
  "SWITCH_ID", "CASE_ID", "BREAK_ID", "DEFAULT_ID", "ERODED_ID",
  "TRAPPED_STATE", "RECHARGED_ID", "INVIS_ID", "GREASED_ID",
  "INDESTRUCTIBLE_ID", "FEMALE_ID", "MALE_ID", "WAITFORU_ID",
  "PROTECTOR_ID", "CANCELLED_ID", "REVIVED_ID", "AVENGE_ID", "FLEEING_ID",
  "BLINDED_ID", "PARALYZED_ID", "STUNNED_ID", "CONFUSED_ID",
  "SEENTRAPS_ID", "ALL_ID", "MONTYPE_ID", "OBJTYPE_ID", "TERTYPE_ID",
  "TERTYPE2_ID", "LEVER_EFFECT_TYPE", "SWITCHABLE_ID",
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
     455,   456,   457,   458,   459,   460,    44,    58,    40,    41,
      91,    93,   123,   125,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
      43,    45,    42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   269,   270,   270,   271,   271,   272,   273,   273,   274,
     274,   274,   274,   275,   276,   276,   277,   278,   279,   279,
     280,   280,   281,   281,   282,   282,   283,   283,   284,   284,
     285,   285,   286,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   290,   290,   290,   291,   291,   292,   293,
     293,   293,   293,   293,   293,   293,   293,   293,   293,   293,
     293,   293,   293,   293,   294,   294,   295,   295,   296,   296,
     297,   297,   298,   298,   299,   299,   300,   300,   302,   303,
     301,   304,   305,   306,   306,   307,   307,   307,   309,   310,
     308,   311,   311,   313,   312,   314,   312,   315,   316,   316,
     317,   319,   318,   321,   320,   323,   322,   325,   324,   326,
     327,   326,   328,   329,   329,   329,   330,   330,   331,   332,
     334,   333,   336,   335,   337,   337,   338,   338,   339,   339,
     340,   340,   341,   341,   342,   342,   343,   343,   344,   344,
     345,   345,   346,   346,   347,   347,   348,   348,   348,   348,
     348,   348,   348,   349,   349,   349,   350,   350,   351,   351,
     352,   353,   352,   354,   355,   355,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   357,   357,   357,   358,   359,
     358,   360,   361,   361,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   363,   364,   365,   365,   366,   366,   366,
     367,   368,   369,   370,   371,   372,   373,   373,   374,   375,
     376,   376,   377,   378,   378,   379,   379,   379,   379,   379,
     379,   379,   379,   379,   379,   379,   379,   379,   380,   380,
     380,   380,   381,   382,   383,   383,   384,   385,   385,   386,
     387,   388,   388,   389,   390,   391,   393,   392,   394,   394,
     395,   395,   395,   395,   396,   396,   397,   398,   398,   399,
     399,   400,   400,   400,   401,   401,   402,   402,   402,   403,
     403,   403,   404,   405,   406,   406,   407,   407,   408,   408,
     409,   409,   410,   410,   411,   411,   411,   412,   412,   413,
     414,   414,   415,   416,   416,   417,   417,   418,   418,   419,
     419,   419,   420,   420,   420,   421,   421,   422,   423,   423,
     423,   424,   425,   425,   425,   425,   426,   426,   426,   427,
     427,   428,   428,   428,   429,   430,   430,   430,   431,   431,
     432,   432,   432,   433,   433,   433,   433,   434,   434,   434,
     435,   435,   435,   435,   435,   436,   436,   437,   437,   437,
     437,   437,   437,   437,   437,   437,   437,   437,   438,   438,
     439,   440,   440,   441,   441,   442,   442,   443,   443,   444,
     444,   445,   445,   445,   445,   445,   445,   445,   445,   445,
     445,   445,   445,   445,   445,   445,   445,   445,   445,   445,
     446,   446,   447,   448,   448,   449,   449,   449,   450,   450,
     450,   450,   451,   451,   452,   452,   453,   453,   454,   454,
     455,   455,   456
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
       1,     1,     3,     1,     1,     1,     1,     3,     3,     0,
       5,     2,     0,     3,     1,     3,     1,     3,     3,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       3,     3,     3,     5,     7,     5,     8,     1,     3,     3,
       5,     5,     7,     7,     6,     5,     0,     2,     5,     3,
      11,    13,     6,     0,     3,     1,     3,     3,     3,     3,
       1,     1,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     3,     3,     1,     5,     9,     5,     7,     3,
       3,     5,     9,     5,     5,     5,     0,    13,     0,     1,
       7,    11,     9,    13,     3,     7,     7,     5,     7,     5,
       9,     5,     5,     3,     7,     3,     7,     7,     3,     5,
       5,     3,     5,     7,     9,     1,     1,     1,     1,     1,
       0,     2,     1,     3,     1,     1,     1,     0,     3,     1,
       0,     3,     1,     0,     3,     1,     1,     1,     1,     1,
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
       5,     0,    30,     0,     7,     0,   155,     0,     0,     0,
       0,   223,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   375,     0,
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
       0,   447,     0,   450,     0,   492,     0,   448,   469,    31,
       0,   175,     0,     8,     0,   408,   409,     0,   445,   182,
       0,     0,     0,    11,   494,   493,    13,   417,     0,   226,
     227,     0,     0,   414,     0,     0,   194,   412,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   488,   471,   490,     0,   441,   443,   444,     0,   440,
     438,     0,   258,   262,   437,   259,   434,   436,     0,   433,
     431,     0,   230,     0,   430,   377,   376,     0,   395,   396,
       0,     0,     0,     0,   299,   298,     0,   422,     0,     0,
     421,     0,     0,     0,     0,   510,     0,     0,   354,     0,
       0,     0,     0,     0,   339,   340,   379,   378,     0,   153,
       0,     0,     0,     0,   411,     0,     0,     0,     0,     0,
     309,     0,   333,   332,   497,   495,   496,   185,   184,     0,
       0,     0,     0,   206,   207,     0,     0,     0,     0,    15,
     118,     0,     0,     0,   363,   365,   368,   371,   148,     0,
       0,     0,     0,   157,     0,     0,     0,     0,     0,   466,
     465,   467,   470,     0,   503,   505,   502,   504,   506,   507,
       0,     0,     0,   125,   126,   121,   119,     0,     0,     0,
       0,    30,   172,    28,     0,     0,     0,     0,     0,   419,
       0,     0,     0,     0,     0,     0,     0,   472,   473,     0,
       0,     0,   481,     0,     0,     0,   487,     0,     0,     0,
       0,     0,     0,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     154,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   179,   178,     0,
       0,     0,   173,     0,     0,     0,     0,     0,     0,   463,
     449,   457,     0,     0,   452,   453,   454,   455,   456,     0,
     151,     0,   447,     0,     0,     0,     0,   142,   140,   146,
     144,     0,     0,     0,   176,     0,     0,   446,    10,   334,
       0,     9,     0,     0,   418,     0,     0,     0,   229,   228,
     194,   195,   225,     0,     0,   210,     0,     0,     0,     0,
     428,     0,     0,   426,     0,     0,   425,     0,     0,   489,
     491,   345,     0,     0,     0,   260,     0,     0,   232,   234,
     293,   214,     0,    16,   136,   295,     0,     0,   397,   398,
       0,   343,     0,     0,   344,   341,   401,     0,   399,     0,
     400,     0,   359,   300,     0,   301,     0,   197,     0,     0,
       0,     0,   306,   305,     0,     0,   186,   187,   372,   508,
     509,     0,   308,     0,     0,   313,     0,   199,     0,     0,
     357,     0,     0,     0,     0,   337,     0,    14,     0,   169,
       0,     0,   159,   362,   361,     0,     0,   369,   370,     0,
     461,   464,     0,   451,   156,   468,   120,     0,     0,   129,
       0,   128,     0,   127,     0,   133,     0,   124,     0,   123,
       0,   122,    32,   410,     0,     0,   420,   413,     0,   415,
       0,   474,     0,     0,     0,   476,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   439,   500,   498,   499,     0,
     273,   270,   264,     0,   289,     0,     0,     0,   288,     0,
       0,     0,     0,     0,     0,     0,     0,   269,     0,   274,
       0,   276,   277,   287,     0,   272,   263,   278,   501,   266,
       0,   432,   233,   205,     0,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   189,     0,     0,   304,     0,     0,     0,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,   174,
     168,   170,     0,     0,     0,     0,     0,   149,     0,   141,
     143,   145,   147,     0,   134,     0,     0,   138,     0,     0,
     416,   224,     0,   211,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     435,     0,     0,   294,   137,    22,     0,   380,     0,     0,
       0,     0,   406,   405,   350,   355,     0,   302,     0,   201,
       0,     0,   303,   307,     0,     0,   373,     0,     0,     0,
     356,     0,   203,     0,   380,   358,   209,     0,   208,   181,
     338,     0,   161,   364,   367,   366,   458,   459,   460,   462,
       0,     0,   132,   131,     0,   130,     0,     0,   475,   477,
       0,   482,     0,   478,     0,   427,   480,   479,     0,   292,
     286,   280,   279,   281,   290,   291,   282,   283,   285,   267,
     284,   268,   271,   275,   265,     0,   402,   237,   238,     0,
     243,   241,   242,   253,   254,   244,   245,   246,     0,     0,
       0,   250,   251,     0,   235,   239,   403,   236,     0,   220,
     221,     0,   222,     0,   219,   215,     0,   296,     0,     0,
     387,     0,     0,     0,   407,     0,     0,     0,     0,     0,
       0,   212,   213,     0,     0,     0,   315,     0,     0,     0,
       0,     0,     0,     0,   320,   321,     0,     0,     0,   314,
       0,     0,   387,     0,     0,     0,     0,     0,   161,   150,
     135,   139,   335,     0,     0,     0,   429,     0,     0,   240,
       0,     0,     0,     0,     0,     0,     0,    23,     0,   384,
     385,   386,   381,   382,     0,   390,     0,     0,   342,   352,
       0,   360,   196,     0,   380,   374,   188,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   198,
       0,   390,   214,   336,     0,   165,   160,   162,     0,   483,
       0,   484,   453,   404,   247,   248,   249,   256,   255,   252,
     217,   218,   216,     0,     0,     0,     0,   393,     0,     0,
       0,     0,     0,   387,     0,     0,   392,   325,   327,   328,
     329,   331,   330,   324,   316,   317,   318,   319,   322,   323,
     326,     0,   393,   204,   163,    30,     0,     0,     0,     0,
       0,   424,   383,   389,   388,     0,     0,   346,     0,   512,
       0,   351,   200,   390,     0,   310,   202,   190,    30,   166,
       0,   485,    19,     0,   257,   391,     0,   348,   511,     0,
     393,     0,     0,   164,     0,     0,   394,   349,   347,   353,
     192,   311,   191,     0,    20,     0,    24,    25,    22,     0,
       0,   193,    12,    21,   486
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    99,   100,   101,   102,   103,
    1079,  1130,   927,  1128,    12,   170,   104,   412,   105,   106,
     107,   108,   224,   109,   110,   773,   573,   776,   513,   514,
     515,   516,   111,   499,   860,   112,   113,   471,   114,   115,
     225,   762,   967,   968,  1098,  1075,   116,   631,   117,   118,
     242,   119,   628,   120,   410,   121,   366,   488,   623,   122,
     123,   124,   346,   338,   125,  1112,   126,  1125,   426,   599,
     619,   831,   844,   127,   365,   847,   546,   943,   723,   925,
     128,   265,   540,   129,   447,   302,   722,   914,  1039,   130,
     444,   292,   443,   716,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   744,   141,   142,   143,   144,   750,
     959,  1063,   145,   146,   531,   147,   148,   149,   150,   151,
     152,   153,   154,   155,  1107,  1118,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   307,   339,
     930,   992,   993,   995,  1084,  1047,  1057,  1087,   310,   580,
     589,   915,   824,   590,   248,   343,   282,   267,   420,   319,
     320,   555,   556,   303,   304,   293,   294,   389,   344,   858,
     640,   641,   642,   391,   392,   393,   283,   438,   237,   256,
     358,   719,   407,   408,   409,   611,   324,   325
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -998
static const yytype_int16 yypact[] =
{
      92,  -117,  -113,   122,  -998,    92,     9,   -80,   -62,  -998,
    -998,     4,   785,   -24,  -998,   100,  -998,    29,    50,    62,
      67,  -998,    72,    78,    90,    97,   111,   119,   127,   131,
     152,   157,   162,   166,   189,   194,   196,   201,   202,   209,
     215,   220,   222,   229,   231,   234,   237,   243,   270,   279,
     290,   294,   295,   299,   300,   303,   312,   315,   317,   318,
     320,   321,   322,    18,   339,   345,   351,  -998,   362,    15,
     855,  -998,  -998,   363,   364,   365,   370,   128,    40,   139,
    -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,
    -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,
    -998,  -998,  -998,  -998,  -998,   785,  -998,  -998,   246,  -998,
    -998,  -998,  -998,  -998,   373,  -998,  -998,  -998,  -998,  -998,
    -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,
    -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,
    -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,
    -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,
    -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,   415,   325,
    -998,  -138,   211,    55,   326,   482,   587,   137,   137,   223,
     -39,    20,   333,   529,   333,   493,  -126,  -126,   -31,   371,
     333,   333,   494,   333,   324,  -126,  -126,   -10,   -31,   -31,
     -31,    40,   384,    40,   333,   587,   587,   333,   587,   587,
     385,   333,   587,   -10,   587,    69,  -998,   587,  -126,   458,
     501,    40,  -998,  -998,   330,   391,   333,   333,   333,   333,
     396,  -998,    65,  -998,   395,  -998,   276,  -998,   129,  -998,
     153,  -998,   397,  -998,   100,  -998,  -998,   400,  -998,   341,
     405,   406,   412,  -998,  -998,  -998,  -998,  -998,   437,  -998,
    -998,   431,   637,  -998,   435,   440,   441,  -998,  -998,  -126,
    -126,   333,   333,   446,   333,   450,   451,   452,   587,   461,
     255,  -998,  -998,   381,   444,  -998,  -998,  -998,   658,  -998,
    -998,   489,  -998,  -998,  -998,  -998,  -998,  -998,   697,  -998,
    -998,   502,   499,   518,  -998,  -998,  -998,   520,  -998,  -998,
     522,   524,   528,   530,  -998,  -998,   734,  -998,   532,   534,
    -998,   538,   537,   743,   542,  -998,   543,   544,   546,   547,
     548,   751,   559,   560,  -998,  -998,  -998,  -998,   561,   762,
     568,   569,   570,   576,   609,   786,   586,   228,   588,   590,
    -998,   591,  -998,  -998,  -998,  -998,  -998,  -998,  -998,   594,
     595,   617,   619,  -998,  -998,   621,   397,   622,   635,   644,
    -998,   582,    40,    40,   645,   646,   649,   652,  -998,   606,
     442,    40,    40,  -998,    40,    40,    40,    40,    40,   341,
     609,  -998,   653,   651,  -998,  -998,  -998,  -998,  -998,  -998,
     654,   133,   107,  -998,  -998,   341,   609,   656,   660,   667,
     785,   785,  -998,  -998,    40,  -138,   862,    43,   872,   672,
     668,   587,   675,    40,   159,   879,   670,  -998,  -998,   676,
     690,   480,  -998,   333,   333,   423,  -998,   695,   692,   587,
     807,   698,    40,   699,   397,   701,    40,   397,   333,   333,
     587,   829,    39,   832,   707,    40,    24,   884,   922,   721,
     890,   893,   280,   764,   333,   847,   731,   859,   -31,    -3,
    -998,   746,   -31,   -31,   -31,    40,   747,    57,   333,   140,
     856,    -8,   779,   858,    -2,    54,    20,   835,  -998,    86,
      86,   818,  -998,   212,   752,   -45,   826,   831,   156,   830,
    -998,  -998,   304,   453,   198,   198,  -998,  -998,  -998,   129,
    -998,   587,   756,  -104,  -103,   -92,   -86,  -998,  -998,   341,
     609,   306,   207,   190,  -998,   759,   496,  -998,  -998,  -998,
     962,  -998,   773,   437,  -998,   776,   969,   509,  -998,  -998,
     441,  -998,  -998,   333,   333,   719,   781,   780,   782,   784,
    -998,   788,   436,  -998,   789,   792,  -998,   797,   798,  -998,
    -998,  -998,   777,   626,    87,  -998,   778,   655,  -998,  -998,
    -998,  -998,   799,   800,  -998,   801,  1022,   714,  -998,  -998,
     821,  -998,   822,  1025,  -998,   824,  -998,   823,  -998,   842,
    -998,   825,   843,  -998,  1046,  -998,   845,  -998,  1048,   865,
      24,   867,   868,  -998,   869,   997,  -998,  -998,  -998,  -998,
    -998,   871,  -998,   873,   874,  -998,   881,  -998,  1079,   883,
    -998,   887,   889,   959,  1093,   891,   892,  -998,   397,  -998,
     833,    40,  -998,  -998,   341,   894,   895,  -998,  -998,   897,
    -998,   899,   898,  -998,  -998,  -998,  -998,  1102,   900,  -998,
     -29,  -998,    40,  -998,  -138,  -998,   115,  -998,    39,  -998,
      46,  -998,  -998,  -998,   903,  1107,  -998,  -998,   902,  -998,
     901,  -998,   906,  1034,   587,  -998,    40,    40,   587,   908,
      40,   587,   587,   907,   909,  -998,  -998,  -998,  -998,   910,
    -998,  -998,  -998,   912,  -998,   913,   914,   915,  -998,   916,
     917,   918,   919,   920,   921,   923,   924,  -998,   925,  -998,
     926,  -998,  -998,  -998,   927,  -998,  -998,  -998,  -998,  -998,
     928,  -998,   929,   930,    20,    39,  1124,   932,  -998,   -10,
    1135,   934,   970,  1138,    45,   223,   972,    26,  1055,   939,
      -1,  -998,   933,  1057,  -998,    40,   942,  -138,  1083,   333,
     944,  1056,   945,     0,  1058,    63,   397,  1059,    86,  -998,
    -998,   609,   941,    24,   -12,    30,   830,  -998,   116,  -998,
    -998,   609,   341,   -78,  -998,   -75,   -51,  -998,    24,   950,
    -998,  -998,    40,  -998,   948,   219,   287,   949,    24,   720,
     951,   952,    40,  -998,    40,   137,  1060,  1062,    40,  1051,
    1061,    40,    40,    40,  -138,  1023,    40,    40,    40,   223,
    -998,   877,    17,  -998,  -998,   960,  1161,   961,   963,  1164,
     964,   965,  -998,  -998,   966,  -998,   967,  -998,  1169,  -998,
     348,   971,  -998,  -998,   973,    68,   341,   974,   975,   767,
    -998,  1171,  -998,  1174,   961,  -998,  -998,   976,  -998,  -998,
    -998,   977,   249,  -998,  -998,   341,  -998,  -998,  -998,  -998,
     397,   115,  -998,  -998,    46,  -998,   978,  1179,   609,  -998,
    1144,  -998,    40,  -998,   979,  -998,  -998,  -998,   631,  -998,
    -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,   341,
    -998,  -998,  -998,  -998,  -998,   180,  -998,  -998,  -998,  -138,
    -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,   982,   983,
     984,  -998,  -998,   985,  -998,  -998,  -998,   341,   986,  -998,
    -998,   987,  -998,   988,  -998,  -998,  1183,  -998,   990,   426,
    1097,  1194,   993,   223,  -998,   -25,   223,   991,   995,     0,
      40,  -998,  -998,   994,  1137,  1111,  -998,   998,   999,  1000,
    1001,  1002,  1003,  1004,  -998,  -998,  1005,  1006,  1007,  -998,
    1008,  1009,  1097,    68,  1209,   108,  1011,  1010,   249,  -998,
    -998,  -998,  -998,  1013,  1012,   336,  -998,    40,  1140,   341,
      40,    40,    40,   -99,   137,  1218,  1094,  -998,  1220,  -998,
    -998,  -998,  -998,  1020,  1021,  1130,  1024,  1225,  -998,  1026,
    1027,  -998,  -998,   159,   961,  -998,  -998,  1029,  1030,  1141,
    1227,    81,   223,   137,    43,    43,   333,   -39,  1229,  -998,
    1234,  1130,  -998,  -998,  1032,  -998,  -998,  -998,  1235,  -998,
    1200,  -998,   434,  -998,  -998,  -998,  -998,  -998,   980,  -998,
    -998,  -998,  -998,  1033,   426,  1145,  1036,  1074,  1242,  1039,
    1078,   223,  1041,  1097,  1160,  1162,  -998,  -998,  -998,  -998,
    -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,
    -998,  1044,  1074,   930,  -998,   785,  1049,  1045,  1052,  1050,
     -99,  -998,  -998,  -998,  -998,  1141,  1053,  -998,  1054,  -998,
    1063,  -998,  -998,  1130,  1065,  -998,  -998,  -998,   785,  -998,
      24,  -998,  -998,  1066,  -998,  -998,   223,   397,  -998,   223,
    1074,  1167,   397,  -998,  1067,   333,  -998,  -998,  -998,  -998,
    -998,  -998,  -998,    76,  1068,   397,  -998,  -998,   960,   333,
    1069,  -998,  -998,  -998,  -998
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -998,  -998,  1253,  -998,  -998,  -998,  -998,  -998,  -998,  -998,
    -998,  -998,   134,  -998,  -998,  1017,  -105,  -361,   854,  1047,
    1195,  -480,  -998,  -998,  -998,  -998,   608,  -998,  -998,  -998,
    -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  1205,  -998,
    -998,  -998,   301,  -998,  -998,  -998,  -998,  -998,  -998,  -998,
    -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,
    -998,  -998,   802,  1064,  -998,  -998,  -998,  -998,   735,  -998,
    -998,  -998,   337,  -998,  -998,  -998,  -611,   319,   258,  -998,
    -998,   454,   278,  -998,  -998,  -998,  -998,  -998,   203,  -998,
    -998,  1108,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,
    -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,
    -998,  -998,  -998,  -998,  -544,  -998,  -998,  -998,  -998,  -998,
    -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,
    -998,  -998,  -998,  -998,  -998,  -998,  -998,  -998,   268,   558,
    -815,   244,  -998,  -902,  -998,  -957,   204,  -997,  -456,  -592,
    -998,  -998,  -998,   479,   876,  -219,  -173,  -379,   760,   681,
    -370,  -476,  -627,  -516,  -442,  -518,  -606,  -156,   -74,  -998,
     526,  -998,  -998,   787,  -998,  -998,  1016,  -169,   733,  -998,
    -465,  -998,  -998,  -998,  -998,  -998,  -172,  -998
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -232
static const yytype_int16 yytable[] =
{
     239,   266,   371,   657,   236,   487,   659,   284,   741,   311,
     574,   313,   607,   625,   626,   249,   315,   327,   328,   639,
     330,   332,   333,   517,    16,   322,   340,   341,   342,   962,
     622,   348,   518,   777,   351,   633,   349,   350,   359,   352,
     353,   305,   296,   360,   231,   362,   529,   661,   367,   550,
     774,   257,   258,   374,   375,   376,   377,   613,   620,   254,
    1021,   354,   783,   999,  1072,   355,   356,   403,   854,   231,
     336,  1037,   941,   379,   363,  1097,   245,   597,   617,   829,
     842,  1126,   316,   565,   405,  1059,   568,   246,   247,   550,
       7,   686,   308,   918,     8,   687,   688,   578,   429,   430,
     309,   432,   648,   650,   579,   317,   318,     1,     2,   649,
     651,   231,   354,  1120,   652,  1038,   355,   356,   285,   297,
     654,   653,     9,   919,   920,   822,    11,   655,   861,   347,
     921,   725,   823,   231,    13,   862,  1110,   512,   863,   922,
     285,   379,   545,   846,   848,   689,  1000,   923,   942,   364,
     255,  1093,    14,   621,   494,   864,  1127,   231,   380,   690,
     691,   692,   865,   693,   390,   694,   406,   257,   258,   245,
     614,   853,  1060,   924,  1061,   306,   394,   323,   395,   262,
     246,   247,   168,   396,   397,   398,   866,   257,   258,  1053,
     695,   696,   697,   550,   698,   286,   874,   699,   700,   701,
     702,   703,   245,   704,   337,   598,   618,   830,   843,   287,
     296,    15,  1062,   246,   247,   609,   574,   286,   169,   825,
     610,   250,   251,   252,   253,   221,   296,   705,    78,   706,
     707,   287,   819,   257,   258,   780,   171,   971,   708,   709,
     710,   711,   712,   713,   538,   539,   519,   298,   232,   856,
     857,   530,   535,   299,   624,   970,   604,   172,   714,   422,
     548,   549,   547,   233,   234,   345,   557,   759,   813,   173,
     560,   770,   715,   232,   174,   569,   570,   880,   769,   175,
     399,   571,   851,   814,   261,   176,   639,   297,   233,   234,
     637,   592,   638,   894,   624,   262,   596,   177,   493,   235,
     601,   602,   603,   297,   178,   608,   525,   502,   503,   285,
     504,   505,   506,   507,   508,   401,   263,   264,   179,   553,
     554,   245,   816,   288,   235,   780,   180,   380,   520,   289,
     233,   234,   246,   247,   181,   257,   258,   232,   182,   634,
     526,   232,   646,   245,   230,   288,   235,   238,   400,   537,
     261,   289,   233,   234,   246,   247,   233,   234,   629,   183,
     586,   401,   587,   588,   184,   402,   235,   245,   563,   185,
     671,   672,   567,   186,   290,   291,   233,   234,   246,   247,
     322,   577,   263,   264,   317,   318,   286,   978,   235,   354,
     733,   717,   235,   355,   356,   849,   187,   965,   624,   966,
     287,   188,   660,   189,   257,   258,   257,   258,   190,   191,
     259,   260,   235,   257,   258,   298,   192,   998,   243,   658,
    1001,   299,   193,   553,   554,   870,   550,   194,   871,   195,
     551,   298,   259,   260,   478,   390,   196,   299,   197,   679,
     422,   198,   300,   301,   199,   269,   270,   271,   272,   273,
     200,   274,   261,   275,   276,   277,   278,   279,   300,   301,
     386,   387,   388,   280,   382,   357,  1040,   989,   990,   991,
    1066,  1067,   384,   385,   386,   387,   388,   201,   630,   384,
     385,   386,   387,   388,   263,   264,   202,   383,   384,   385,
     386,   387,   388,   872,   281,  1065,  1064,   203,   772,   969,
    1024,   204,   205,   257,   258,   784,   206,   207,  1114,   787,
     208,   240,   790,   791,   288,   643,   257,   258,   656,   209,
     289,   261,   210,   261,   211,   212,   834,   213,   214,   215,
     261,   244,   331,   312,   262,  1091,   384,   385,   386,   387,
     388,   262,  1030,   290,   291,  1031,   217,   384,   385,   386,
     387,   388,   218,   263,   264,   263,   264,   761,   219,   545,
     257,   258,   263,   264,   384,   385,   386,   387,   388,   220,
     226,   227,   228,   257,   258,   879,   838,   229,   771,   883,
     241,   268,   886,   887,   888,   326,   329,   891,   892,   893,
    1116,   836,   345,  1119,   369,   372,   384,   385,   386,   387,
     388,   373,   785,   786,   378,   381,   789,   415,   855,   411,
     414,   416,   417,   269,   270,   271,   272,   273,   418,   274,
     261,   275,   276,   277,   278,   279,   269,   270,   271,   272,
     273,   552,   274,   261,   275,   276,   277,   278,   279,   421,
    1078,   422,   419,   -18,   280,   423,   424,   425,   889,   439,
     440,   501,   263,   264,   431,   917,   553,   554,   433,   434,
     435,   441,   281,   314,   644,   263,   264,   257,   258,   437,
     269,   270,   271,   272,   273,   281,   274,   261,   275,   276,
     277,   278,   279,   269,   270,   271,   272,   273,   280,   274,
     261,   275,   276,   277,   278,   279,   386,   387,   388,   442,
     445,   280,   384,   385,   386,   387,   388,   663,   868,   263,
     264,  -231,   446,   384,   385,   386,   387,   388,   878,   281,
     669,  1005,   263,   264,   448,    82,   449,    84,   450,    86,
     451,    88,   281,    90,   452,    92,   453,    94,   454,    96,
     456,    98,   455,   979,   457,   458,  1117,   459,   460,   461,
     462,  1122,   463,   464,   465,   466,   384,   385,   386,   387,
     388,  1034,  1035,  1036,  1131,   467,   468,   469,   470,   384,
     385,   386,   387,   388,   472,   473,   474,   269,   270,   271,
     272,   273,   475,   274,   261,   275,   276,   277,   278,   279,
     476,    16,   477,   492,   479,   280,   480,   481,   975,    17,
     482,   483,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,   500,   263,   264,    31,    32,
      33,    34,    35,   484,    36,   485,   281,   486,   489,    37,
      38,    39,    40,    41,    42,    43,    44,   685,    45,    46,
      47,   490,    48,  1068,    49,    50,    51,    52,    53,    54,
     491,   495,   496,    55,    56,   497,    57,   946,   498,   509,
     510,   511,    58,   521,   947,   528,   721,   522,   321,   384,
     385,   386,   387,   388,   523,   532,   334,   335,   533,   534,
     948,   536,   543,    59,   541,   542,   384,   385,   386,   387,
     388,   384,   977,   386,   387,   388,   544,   949,   558,   368,
      60,   559,   561,  1032,   562,   564,    61,   566,   572,    62,
      63,   575,    64,   576,    65,   384,   385,   386,   387,   388,
      66,   404,   581,    67,    68,   728,   582,   583,   584,    69,
      70,   875,    71,   585,    72,   591,   593,   594,   950,   951,
     952,   953,  1124,   954,   955,   956,   957,   958,   595,   612,
     427,   428,   600,   605,   615,   627,  1133,   616,   635,   895,
     896,  -180,   647,   632,   636,   664,    73,    74,    75,    76,
    1099,    77,   662,   668,   384,   385,   386,   387,   388,   665,
     384,   385,   386,   387,   388,   667,   673,   674,   676,   675,
     677,   684,   720,  1113,   678,    78,   897,   898,   681,   680,
     899,    79,    80,   682,   683,   724,   725,   726,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,   727,   729,   730,   731,
     732,   900,   735,   733,   901,   902,   903,   904,   905,   906,
     907,   908,   909,   910,   911,   912,   913,    80,   734,   736,
     737,   738,   739,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,   740,   222,   742,   743,   745,   746,   747,    81,   748,
      83,   749,    85,   752,    87,   756,    89,   751,    91,   753,
      93,   245,    95,   754,    97,   755,   679,   757,   758,   760,
     763,   764,   246,   247,   765,   766,   768,   767,   316,   778,
     779,   780,   782,   545,   788,   792,   781,   794,   793,   795,
     796,   797,   798,   799,   800,   801,   802,   803,   804,   815,
     805,   806,   807,   808,   809,   811,   812,   810,   816,   818,
     819,   820,   821,   826,   827,   828,   833,   832,   835,   837,
     839,   841,   840,   852,   845,   850,   867,   869,   873,   884,
     876,   877,   881,   882,   890,   928,   926,   929,   932,   931,
     885,   933,   935,   937,   936,   960,   934,   939,   961,   940,
     944,   945,   963,   964,   973,   974,   987,   972,   976,   980,
     981,   982,   983,   984,   985,   986,   988,   994,   996,   997,
    1002,  1003,  1008,  1006,  1007,  1009,  1010,  1011,  1012,  1013,
    1014,  1015,  1016,  1017,  1018,  1020,  1023,  1019,  1025,  1028,
    1033,  1029,  1041,  1026,  1043,  1042,  1044,  1046,  1045,  1049,
    1048,  1058,  1050,  1070,  1051,  1054,  1055,  1056,  1071,  1074,
    1076,  1077,  1081,  1085,  1083,  1086,  1088,  1080,  1089,  1090,
    1092,  1094,  1095,  1096,  1101,  1100,  1102,  1121,    10,  1103,
    1106,   413,  1132,  1108,   524,   223,   775,   370,   216,  1027,
    1109,  1111,  1115,  1123,  1129,   670,  1004,   361,  1134,   606,
    1073,  1052,  1022,  1104,   938,  1069,   295,   817,  1082,  1105,
     916,   527,   859,   666,   436,     0,   645,   718
};

static const yytype_int16 yycheck[] =
{
     105,   174,   221,   521,    78,   366,   522,   176,   600,   182,
     452,   184,   477,   489,   490,   171,   185,   190,   191,   499,
     193,   194,   194,   402,     6,    56,   198,   199,   200,   844,
     486,   204,   402,   660,   207,    80,   205,   206,   211,   208,
     209,    80,     3,   212,     4,   214,     3,   523,   217,     3,
     656,    80,    81,   226,   227,   228,   229,    65,     4,     4,
     962,     4,   673,    88,  1021,     8,     9,   240,    80,     4,
      80,   170,     4,     8,     5,  1072,   214,    80,    80,    80,
      80,     5,   208,   444,   240,     4,   447,   225,   226,     3,
     207,     4,    72,    76,   207,     8,     9,    73,   271,   272,
      80,   274,   206,   206,    80,   231,   232,    15,    16,   213,
     213,     4,     4,  1110,   206,   214,     8,     9,     3,    80,
     206,   213,     0,   106,   107,    80,   117,   213,   206,   203,
     113,   206,    87,     4,   214,   213,  1093,     4,   213,   122,
       3,     8,    79,    80,   755,    58,   171,   130,    80,    80,
      95,  1053,   214,    99,   373,   206,    80,     4,   232,    72,
      73,    74,   213,    76,   238,    78,   240,    80,    81,   214,
     178,   763,    91,   156,    93,   214,    23,   208,    25,   208,
     225,   226,   206,    30,    31,    32,   778,    80,    81,  1004,
     103,   104,   105,     3,   107,    80,   788,   110,   111,   112,
     113,   114,   214,   116,   214,   208,   208,   208,   208,    94,
       3,   207,   131,   225,   226,    75,   658,    80,   118,   735,
      80,    10,    11,    12,    13,   210,     3,   140,   210,   142,
     143,    94,   206,    80,    81,   209,   207,   864,   151,   152,
     153,   154,   155,   156,    85,    86,   402,   208,   208,   219,
     220,   208,   421,   214,   208,   861,   475,   207,   171,     4,
     433,   434,   431,   223,   224,   208,   435,   628,   724,   207,
     439,   650,   185,   208,   207,   448,   449,   795,   648,   207,
     127,   450,   758,   725,   197,   207,   766,    80,   223,   224,
     134,   464,   136,   809,   208,   208,   468,   207,   372,   259,
     472,   473,   474,    80,   207,   478,   411,   381,   382,     3,
     384,   385,   386,   387,   388,   208,   229,   230,   207,   233,
     234,   214,   206,   208,   259,   209,   207,   401,   402,   214,
     223,   224,   225,   226,   207,    80,    81,   208,   207,   495,
     414,   208,   511,   214,   216,   208,   259,   208,   195,   423,
     197,   214,   223,   224,   225,   226,   223,   224,   146,   207,
      80,   208,    82,    83,   207,   212,   259,   214,   442,   207,
     543,   544,   446,   207,   237,   238,   223,   224,   225,   226,
      56,   455,   229,   230,   231,   232,    80,   207,   259,     4,
     210,   564,   259,     8,     9,   756,   207,   148,   208,   150,
      94,   207,   212,   207,    80,    81,    80,    81,   207,   207,
      84,    85,   259,    80,    81,   208,   207,   933,     3,   212,
     936,   214,   207,   233,   234,   206,     3,   207,   209,   207,
       7,   208,    84,    85,   206,   509,   207,   214,   207,     3,
       4,   207,   235,   236,   207,   190,   191,   192,   193,   194,
     207,   196,   197,   198,   199,   200,   201,   202,   235,   236,
     262,   263,   264,   208,   188,    80,   984,    41,    42,    43,
    1014,  1015,   260,   261,   262,   263,   264,   207,   266,   260,
     261,   262,   263,   264,   229,   230,   207,   211,   260,   261,
     262,   263,   264,   206,   239,  1013,  1012,   207,   654,   860,
     965,   207,   207,    80,    81,   674,   207,   207,  1100,   678,
     207,   265,   681,   682,   208,   211,    80,    81,   212,   207,
     214,   197,   207,   197,   207,   207,   745,   207,   207,   207,
     197,   206,   208,     4,   208,  1051,   260,   261,   262,   263,
     264,   208,   206,   237,   238,   209,   207,   260,   261,   262,
     263,   264,   207,   229,   230,   229,   230,   631,   207,    79,
      80,    81,   229,   230,   260,   261,   262,   263,   264,   207,
     207,   207,   207,    80,    81,   794,   749,   207,   652,   798,
     207,    99,   801,   802,   803,   214,    92,   806,   807,   808,
    1106,   747,   208,  1109,   136,   265,   260,   261,   262,   263,
     264,   210,   676,   677,   208,   210,   680,   266,   764,   212,
     210,   206,   206,   190,   191,   192,   193,   194,   206,   196,
     197,   198,   199,   200,   201,   202,   190,   191,   192,   193,
     194,   208,   196,   197,   198,   199,   200,   201,   202,   208,
     206,     4,   205,   209,   208,   210,   206,   206,   804,   268,
     206,   209,   229,   230,   208,   811,   233,   234,   208,   208,
     208,     3,   239,   170,   211,   229,   230,    80,    81,   208,
     190,   191,   192,   193,   194,   239,   196,   197,   198,   199,
     200,   201,   202,   190,   191,   192,   193,   194,   208,   196,
     197,   198,   199,   200,   201,   202,   262,   263,   264,   210,
       3,   208,   260,   261,   262,   263,   264,   211,   782,   229,
     230,   212,   210,   260,   261,   262,   263,   264,   792,   239,
     211,   940,   229,   230,   206,   224,   206,   226,   206,   228,
     206,   230,   239,   232,   206,   234,   206,   236,     4,   238,
     206,   240,   210,   899,   206,   208,  1107,     4,   206,   206,
     206,  1112,   206,   206,   206,     4,   260,   261,   262,   263,
     264,   980,   981,   982,  1125,   206,   206,   206,     6,   260,
     261,   262,   263,   264,   206,   206,   206,   190,   191,   192,
     193,   194,   206,   196,   197,   198,   199,   200,   201,   202,
       4,     6,   206,   211,   206,   208,   206,   206,   872,    14,
     206,   206,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,   209,   229,   230,    33,    34,
      35,    36,    37,   206,    39,   206,   239,   206,   206,    44,
      45,    46,    47,    48,    49,    50,    51,   211,    53,    54,
      55,   206,    57,  1016,    59,    60,    61,    62,    63,    64,
     206,   206,   206,    68,    69,   206,    71,    90,   206,   206,
     209,   207,    77,   207,    97,     3,   211,   207,   187,   260,
     261,   262,   263,   264,   207,     3,   195,   196,   206,   211,
     113,   206,   206,    98,     5,   215,   260,   261,   262,   263,
     264,   260,   261,   262,   263,   264,   206,   130,   203,   218,
     115,   209,    95,   977,   206,   206,   121,   206,    79,   124,
     125,    79,   127,   206,   129,   260,   261,   262,   263,   264,
     135,   240,    38,   138,   139,   211,     4,   206,    38,   144,
     145,   211,   147,    40,   149,   171,    89,   206,   171,   172,
     173,   174,  1115,   176,   177,   178,   179,   180,    89,    93,
     269,   270,   206,   206,   175,   137,  1129,    99,   132,    82,
      83,   126,   206,   211,   133,     3,   181,   182,   183,   184,
    1075,   186,   213,     4,   260,   261,   262,   263,   264,   206,
     260,   261,   262,   263,   264,   209,   267,   206,   206,   209,
     206,   214,   214,  1098,   206,   210,   119,   120,   206,   210,
     123,   216,   217,   206,   206,   206,   206,   206,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,     4,   206,   206,     4,
     206,   154,   207,   210,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   217,   206,   206,
       4,   206,     4,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   206,   217,   206,   206,   206,    79,   206,   223,   206,
     225,   207,   227,     4,   229,   126,   231,   206,   233,   206,
     235,   214,   237,   206,   239,   206,     3,   206,   206,   266,
     206,   206,   225,   226,   207,   206,     4,   209,   208,   206,
       3,   209,   206,    79,   206,   208,   215,   207,   209,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,     5,
     207,   207,   207,   207,   207,   206,   206,   209,   206,     4,
     206,   171,     4,   171,    89,   206,    89,   214,   206,    66,
     206,   206,    96,   212,    96,    96,   206,   209,   209,   108,
     209,   209,   102,   101,   141,     4,   206,   206,     4,   206,
     109,   207,   206,     4,   207,     4,   211,   206,     4,   206,
     206,   206,   206,   206,     5,    41,     3,   209,   209,   207,
     207,   207,   207,   207,   207,   207,   206,   100,     4,   206,
     209,   206,    91,   209,    67,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   206,     7,   209,   207,   206,
      80,   209,     4,   213,     4,   131,   206,    97,   207,     4,
     206,     4,   206,     4,   207,   206,   206,    96,     4,   207,
       5,    41,   209,   207,    99,   171,     4,   267,   209,   171,
     209,    91,    90,   209,   209,   206,   204,    90,     5,   209,
     207,   244,  1128,   209,   410,    70,   658,   220,    63,   968,
     207,   206,   206,   206,   206,   540,   939,   213,   209,   477,
    1022,  1003,   963,  1080,   830,  1017,   178,   729,  1044,  1085,
     811,   415,   766,   533,   278,    -1,   509,   564
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   270,   271,   272,   273,   207,   207,     0,
     271,   117,   283,   214,   214,   207,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    33,    34,    35,    36,    37,    39,    44,    45,    46,
      47,    48,    49,    50,    51,    53,    54,    55,    57,    59,
      60,    61,    62,    63,    64,    68,    69,    71,    77,    98,
     115,   121,   124,   125,   127,   129,   135,   138,   139,   144,
     145,   147,   149,   181,   182,   183,   184,   186,   210,   216,
     217,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   274,
     275,   276,   277,   278,   285,   287,   288,   289,   290,   292,
     293,   301,   304,   305,   307,   308,   315,   317,   318,   320,
     322,   324,   328,   329,   330,   333,   335,   342,   349,   352,
     358,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   374,   375,   376,   377,   381,   382,   384,   385,   386,
     387,   388,   389,   390,   391,   392,   395,   396,   397,   398,
     399,   400,   401,   402,   403,   404,   405,   406,   206,   118,
     284,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   307,   207,   207,   207,
     207,   210,   217,   289,   291,   309,   207,   207,   207,   207,
     216,     4,   208,   223,   224,   259,   437,   447,   208,   285,
     265,   207,   319,     3,   206,   214,   225,   226,   423,   436,
      10,    11,    12,    13,     4,    95,   448,    80,    81,    84,
      85,   197,   208,   229,   230,   350,   425,   426,    99,   190,
     191,   192,   193,   194,   196,   198,   199,   200,   201,   202,
     208,   239,   425,   445,   446,     3,    80,    94,   208,   214,
     237,   238,   360,   434,   435,   360,     3,    80,   208,   214,
     235,   236,   354,   432,   433,    80,   214,   407,    72,    80,
     417,   425,     4,   425,   170,   446,   208,   231,   232,   428,
     429,   428,    56,   208,   455,   456,   214,   425,   425,    92,
     425,   208,   425,   455,   428,   428,    80,   214,   332,   408,
     455,   455,   455,   424,   437,   208,   331,   437,   425,   446,
     446,   425,   446,   446,     4,     8,     9,    80,   449,   425,
     446,   332,   446,     5,    80,   343,   325,   446,   428,   136,
     288,   424,   265,   210,   425,   425,   425,   425,   208,     8,
     437,   210,   188,   211,   260,   261,   262,   263,   264,   436,
     437,   442,   443,   444,    23,    25,    30,    31,    32,   127,
     195,   208,   212,   425,   428,   436,   437,   451,   452,   453,
     323,   212,   286,   284,   210,   266,   206,   206,   206,   205,
     427,   208,     4,   210,   206,   206,   337,   428,   428,   425,
     425,   208,   425,   208,   208,   208,   445,   208,   446,   268,
     206,     3,   210,   361,   359,     3,   210,   353,   206,   206,
     206,   206,   206,   206,     4,   210,   206,   206,   208,     4,
     206,   206,   206,   206,   206,   206,     4,   206,   206,   206,
       6,   306,   206,   206,   206,   206,     4,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   286,   326,   206,
     206,   206,   211,   437,   424,   206,   206,   206,   206,   302,
     209,   209,   437,   437,   437,   437,   437,   437,   437,   206,
     209,   207,     4,   297,   298,   299,   300,   426,   429,   436,
     437,   207,   207,   207,   287,   285,   437,   423,     3,     3,
     208,   383,     3,   206,   211,   446,   206,   437,    85,    86,
     351,     5,   215,   206,   206,    79,   345,   446,   425,   425,
       3,     7,   208,   233,   234,   430,   431,   446,   203,   209,
     446,    95,   206,   437,   206,   286,   206,   437,   286,   425,
     425,   446,    79,   295,   433,    79,   206,   437,    73,    80,
     418,    38,     4,   206,    38,    40,    80,    82,    83,   419,
     422,   171,   425,    89,   206,    89,   455,    80,   208,   338,
     206,   455,   455,   455,   424,   206,   331,   449,   425,    75,
      80,   454,    93,    65,   178,   175,    99,    80,   208,   339,
       4,    99,   417,   327,   208,   430,   430,   137,   321,   146,
     266,   316,   211,    80,   436,   132,   133,   134,   136,   290,
     439,   440,   441,   211,   211,   442,   446,   206,   206,   213,
     206,   213,   206,   213,   206,   213,   212,   434,   212,   432,
     212,   430,   213,   211,     3,   206,   427,   209,     4,   211,
     337,   425,   425,   267,   206,   209,   206,   206,   206,     3,
     210,   206,   206,   206,   214,   211,     4,     8,     9,    58,
      72,    73,    74,    76,    78,   103,   104,   105,   107,   110,
     111,   112,   113,   114,   116,   140,   142,   143,   151,   152,
     153,   154,   155,   156,   171,   185,   362,   425,   447,   450,
     214,   211,   355,   347,   206,   206,   206,     4,   211,   206,
     206,     4,   206,   210,   206,   207,   206,     4,   206,     4,
     206,   418,   206,   206,   373,   206,    79,   206,   206,   207,
     378,   206,     4,   206,   206,   206,   126,   206,   206,   286,
     266,   437,   310,   206,   206,   207,   206,   209,     4,   429,
     426,   437,   436,   294,   435,   295,   296,   431,   206,     3,
     209,   215,   206,   345,   446,   437,   437,   446,   206,   437,
     446,   446,   208,   209,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     209,   206,   206,   417,   433,     5,   206,   408,     4,   206,
     171,     4,    80,    87,   421,   432,   171,    89,   206,    80,
     208,   340,   214,    89,   424,   206,   436,    66,   425,   206,
      96,   206,    80,   208,   341,    96,    80,   344,   345,   286,
      96,   430,   212,   418,    80,   436,   219,   220,   438,   439,
     303,   206,   213,   213,   206,   213,   418,   206,   437,   209,
     206,   209,   206,   209,   418,   211,   209,   209,   437,   424,
     434,   102,   101,   424,   108,   109,   424,   424,   424,   436,
     141,   424,   424,   424,   432,    82,    83,   119,   120,   123,
     154,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   356,   420,   422,   436,    76,   106,
     107,   113,   122,   130,   156,   348,   206,   281,     4,   206,
     409,   206,     4,   207,   211,   206,   207,     4,   350,   206,
     206,     4,    80,   346,   206,   206,    90,    97,   113,   130,
     171,   172,   173,   174,   176,   177,   178,   179,   180,   379,
       4,     4,   409,   206,   206,   148,   150,   311,   312,   286,
     435,   431,   209,     5,    41,   437,   209,   261,   207,   436,
     207,   207,   207,   207,   207,   207,   207,     3,   206,    41,
      42,    43,   410,   411,   100,   412,     4,   206,   432,    88,
     171,   432,   209,   206,   341,   424,   209,    67,    91,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   209,
     206,   412,   346,     7,   449,   207,   213,   311,   206,   209,
     206,   209,   437,    80,   424,   424,   424,   170,   214,   357,
     434,     4,   131,     4,   206,   207,    97,   414,   206,     4,
     206,   207,   351,   409,   206,   206,    96,   415,     4,     4,
      91,    93,   131,   380,   432,   434,   383,   383,   425,   407,
       4,     4,   414,   347,   207,   314,     5,    41,   206,   279,
     267,   209,   410,    99,   413,   207,   171,   416,     4,   209,
     171,   432,   209,   412,    91,    90,   209,   416,   313,   285,
     206,   209,   204,   209,   357,   415,   207,   393,   209,   207,
     414,   206,   334,   285,   418,   206,   432,   286,   394,   432,
     416,    90,   286,   206,   425,   336,     5,    80,   282,   206,
     280,   286,   281,   425,   209
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
#line 1685 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1694 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1698 "lev_comp.y"
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

  case 258:

/* Line 1455 of yacc.c  */
#line 1711 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1719 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1729 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1737 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1746 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1753 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1761 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1767 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1772 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1777 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1782 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1787 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1792 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1797 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1802 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1807 "lev_comp.y"
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

  case 274:

/* Line 1455 of yacc.c  */
#line 1821 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1827 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1832 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1837 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1842 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1847 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1852 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1857 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1862 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1867 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1872 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1877 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1882 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1887 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x400000;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1892 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x800000;
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1897 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x1000000;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1902 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x2000000;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1907 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x4000000;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1912 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x8000000;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1919 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1925 "lev_comp.y"
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

  case 295:

/* Line 1455 of yacc.c  */
#line 1954 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1959 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1967 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1973 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1977 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1985 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1992 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 1999 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2009 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2019 "lev_comp.y"
    {
		      long rtyp = 0;
		      switch((yyvsp[(6) - (6)].i)) {
		      case -1: rtyp = LR_TELE; break;
		      case  0: rtyp = LR_DOWNTELE; break;
		      case  1: rtyp = LR_UPTELE; break;
		      }
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (6)].lregn).x1, (yyvsp[(3) - (6)].lregn).y1, (yyvsp[(3) - (6)].lregn).x2, (yyvsp[(3) - (6)].lregn).y2, (yyvsp[(3) - (6)].lregn).area,
					   (yyvsp[(5) - (6)].lregn).x1, (yyvsp[(5) - (6)].lregn).y1, (yyvsp[(5) - (6)].lregn).x2, (yyvsp[(5) - (6)].lregn).y2, (yyvsp[(5) - (6)].lregn).area,
					   rtyp, 0, (char *)0, SPO_LEVREGION));
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2034 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2044 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2048 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2054 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2060 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2066 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2070 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2076 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2082 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2089 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2097 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2103 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2108 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2113 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2118 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2123 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2129 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2135 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2140 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2145 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2150 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2155 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2160 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2170 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2176 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2182 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2187 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2194 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2201 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2205 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2211 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2217 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2223 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2228 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2235 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2241 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2251 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2257 "lev_comp.y"
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

  case 347:

/* Line 1455 of yacc.c  */
#line 2276 "lev_comp.y"
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

  case 348:

/* Line 1455 of yacc.c  */
#line 2288 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2292 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2298 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2303 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2308 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (long)(yyvsp[(9) - (9)].i), (long)(yyvsp[(7) - (9)].i), (long)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2313 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((long)(yyvsp[(9) - (13)].i), (long)(yyvsp[(7) - (13)].i), (long)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2319 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2323 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2329 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2335 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2339 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2345 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2349 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2355 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2359 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2364 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2371 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2375 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2382 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2386 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2391 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2398 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2402 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2407 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2415 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2421 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2428 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2432 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2439 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2450 "lev_comp.y"
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

  case 380:

/* Line 1455 of yacc.c  */
#line 2463 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2467 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2473 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2477 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2484 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2488 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2492 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2498 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2502 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2508 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2514 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2518 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2524 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2530 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2535 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2552 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2560 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2570 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2579 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2584 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2591 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2602 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2608 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2612 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2616 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2623 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2633 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2640 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2644 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2650 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2654 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2662 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2666 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2673 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2683 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2696 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2700 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2707 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2717 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2726 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2737 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2741 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2748 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2758 "lev_comp.y"
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

  case 434:

/* Line 1455 of yacc.c  */
#line 2769 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2778 "lev_comp.y"
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

  case 436:

/* Line 1455 of yacc.c  */
#line 2788 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2794 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2798 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2805 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2815 "lev_comp.y"
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

  case 441:

/* Line 1455 of yacc.c  */
#line 2826 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2835 "lev_comp.y"
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

  case 443:

/* Line 1455 of yacc.c  */
#line 2845 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2849 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2855 "lev_comp.y"
    { ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2857 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2863 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2867 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2871 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2875 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2883 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2892 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2896 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2900 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2904 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2908 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2911 "lev_comp.y"
    { ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2915 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2922 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2931 "lev_comp.y"
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

  case 465:

/* Line 1455 of yacc.c  */
#line 2972 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2976 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2983 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2990 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 3000 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 3004 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 3012 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 3016 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3020 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3024 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3028 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3033 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3037 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3041 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3046 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3051 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3056 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3060 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3065 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3070 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3074 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3078 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3083 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3087 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3094 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3100 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3104 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3110 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3126 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3130 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3134 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3138 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3160 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3164 "lev_comp.y"
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

  case 512:

/* Line 1455 of yacc.c  */
#line 3190 "lev_comp.y"
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
#line 7229 "lev_comp.tab.c"
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
#line 3218 "lev_comp.y"


/*lev_comp.y*/

