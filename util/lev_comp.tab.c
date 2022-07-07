
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
     PARALYZED_ID = 424,
     STUNNED_ID = 425,
     CONFUSED_ID = 426,
     SEENTRAPS_ID = 427,
     ALL_ID = 428,
     MONTYPE_ID = 429,
     OBJTYPE_ID = 430,
     TERTYPE_ID = 431,
     TERTYPE2_ID = 432,
     LEVER_EFFECT_TYPE = 433,
     SWITCHABLE_ID = 434,
     CONTINUOUSLY_USABLE_ID = 435,
     TARGET_ID = 436,
     TRAPTYPE_ID = 437,
     EFFECT_FLAG_ID = 438,
     GRAVE_ID = 439,
     BRAZIER_ID = 440,
     SIGNPOST_ID = 441,
     TREE_ID = 442,
     ERODEPROOF_ID = 443,
     FUNCTION_ID = 444,
     MSG_OUTPUT_TYPE = 445,
     COMPARE_TYPE = 446,
     UNKNOWN_TYPE = 447,
     rect_ID = 448,
     fillrect_ID = 449,
     line_ID = 450,
     randline_ID = 451,
     grow_ID = 452,
     selection_ID = 453,
     flood_ID = 454,
     rndcoord_ID = 455,
     circle_ID = 456,
     ellipse_ID = 457,
     filter_ID = 458,
     complement_ID = 459,
     gradient_ID = 460,
     GRADIENT_TYPE = 461,
     LIMITED = 462,
     HUMIDITY_TYPE = 463,
     STRING = 464,
     MAP_ID = 465,
     NQSTRING = 466,
     VARSTRING = 467,
     CFUNC = 468,
     CFUNC_INT = 469,
     CFUNC_STR = 470,
     CFUNC_COORD = 471,
     CFUNC_REGION = 472,
     VARSTRING_INT = 473,
     VARSTRING_INT_ARRAY = 474,
     VARSTRING_STRING = 475,
     VARSTRING_STRING_ARRAY = 476,
     VARSTRING_VAR = 477,
     VARSTRING_VAR_ARRAY = 478,
     VARSTRING_COORD = 479,
     VARSTRING_COORD_ARRAY = 480,
     VARSTRING_REGION = 481,
     VARSTRING_REGION_ARRAY = 482,
     VARSTRING_MAPCHAR = 483,
     VARSTRING_MAPCHAR_ARRAY = 484,
     VARSTRING_MONST = 485,
     VARSTRING_MONST_ARRAY = 486,
     VARSTRING_OBJ = 487,
     VARSTRING_OBJ_ARRAY = 488,
     VARSTRING_SEL = 489,
     VARSTRING_SEL_ARRAY = 490,
     METHOD_INT = 491,
     METHOD_INT_ARRAY = 492,
     METHOD_STRING = 493,
     METHOD_STRING_ARRAY = 494,
     METHOD_VAR = 495,
     METHOD_VAR_ARRAY = 496,
     METHOD_COORD = 497,
     METHOD_COORD_ARRAY = 498,
     METHOD_REGION = 499,
     METHOD_REGION_ARRAY = 500,
     METHOD_MAPCHAR = 501,
     METHOD_MAPCHAR_ARRAY = 502,
     METHOD_MONST = 503,
     METHOD_MONST_ARRAY = 504,
     METHOD_OBJ = 505,
     METHOD_OBJ_ARRAY = 506,
     METHOD_SEL = 507,
     METHOD_SEL_ARRAY = 508,
     DICE = 509
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
#line 554 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 566 "lev_comp.tab.c"

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
#define YYNTOKENS  272
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  188
/* YYNRULES -- Number of rules.  */
#define YYNRULES  515
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1140

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   509

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   267,   271,     2,
     211,   212,   265,   263,   209,   264,   269,   266,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   210,     2,
       2,   268,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   213,     2,   214,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   215,   270,   216,     2,     2,     2,     2,
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
     205,   206,   207,   208,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262
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
     804,   808,   809,   815,   818,   819,   823,   825,   829,   831,
     835,   839,   841,   843,   847,   849,   851,   853,   857,   859,
     861,   863,   867,   871,   875,   879,   883,   887,   891,   895,
     897,   899,   901,   903,   907,   911,   915,   921,   929,   935,
     944,   946,   950,   954,   960,   966,   974,   982,   989,   995,
     996,   999,  1005,  1009,  1021,  1035,  1042,  1043,  1047,  1049,
    1053,  1057,  1061,  1065,  1067,  1069,  1073,  1077,  1081,  1085,
    1089,  1093,  1095,  1097,  1099,  1101,  1105,  1109,  1111,  1117,
    1127,  1133,  1141,  1145,  1149,  1155,  1165,  1171,  1177,  1183,
    1184,  1198,  1199,  1201,  1209,  1221,  1231,  1245,  1249,  1257,
    1265,  1271,  1279,  1285,  1295,  1301,  1307,  1311,  1319,  1323,
    1331,  1339,  1343,  1349,  1355,  1359,  1365,  1373,  1383,  1385,
    1387,  1389,  1391,  1393,  1394,  1397,  1399,  1403,  1405,  1407,
    1409,  1410,  1414,  1416,  1417,  1421,  1423,  1424,  1428,  1430,
    1432,  1434,  1436,  1438,  1440,  1442,  1444,  1446,  1450,  1452,
    1454,  1459,  1461,  1463,  1468,  1470,  1472,  1477,  1479,  1484,
    1490,  1492,  1496,  1498,  1502,  1504,  1506,  1511,  1521,  1523,
    1525,  1530,  1532,  1538,  1540,  1542,  1547,  1549,  1551,  1557,
    1559,  1561,  1563,  1568,  1570,  1572,  1578,  1580,  1582,  1584,
    1588,  1590,  1592,  1596,  1598,  1603,  1607,  1611,  1615,  1619,
    1623,  1627,  1629,  1631,  1635,  1637,  1641,  1642,  1644,  1646,
    1648,  1650,  1654,  1655,  1657,  1659,  1662,  1665,  1670,  1677,
    1682,  1689,  1696,  1703,  1710,  1713,  1720,  1729,  1738,  1749,
    1764,  1767,  1769,  1773,  1775,  1779,  1781,  1783,  1785,  1787,
    1789,  1791,  1793,  1795,  1797,  1799,  1801,  1803,  1805,  1807,
    1809,  1811,  1813,  1815,  1817,  1828
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     273,     0,    -1,    -1,   274,    -1,   275,    -1,   275,   274,
      -1,   276,   286,   288,    -1,    16,   210,   217,    -1,    15,
     210,   217,   209,     3,    -1,    17,   210,    11,   209,   386,
      -1,    17,   210,    10,   209,     3,    -1,    17,   210,    13,
      -1,    17,   210,    12,   209,     3,   209,     3,   209,     5,
     209,     5,   209,   421,   209,   285,   284,    -1,    18,   210,
     451,    -1,   136,   210,   137,   209,   138,    -1,   136,   210,
     137,    -1,    29,   210,     4,   209,   298,    -1,    21,   210,
     100,    -1,    -1,   209,   207,    -1,    -1,   209,   428,    -1,
      -1,   209,     3,    -1,     5,    -1,    81,    -1,    -1,   118,
     210,   287,    -1,   119,   209,   287,    -1,   119,    -1,    -1,
     290,   288,    -1,   215,   288,   216,    -1,   331,    -1,   277,
      -1,   278,    -1,   394,    -1,   281,    -1,   279,    -1,   401,
      -1,   280,    -1,   400,    -1,   398,    -1,   399,    -1,   402,
      -1,   403,    -1,   404,    -1,   405,    -1,   406,    -1,   375,
      -1,   333,    -1,   296,    -1,   295,    -1,   389,    -1,   345,
      -1,   367,    -1,   408,    -1,   409,    -1,   377,    -1,   378,
      -1,   379,    -1,   407,    -1,   311,    -1,   321,    -1,   323,
      -1,   327,    -1,   325,    -1,   308,    -1,   318,    -1,   304,
      -1,   307,    -1,   370,    -1,   380,    -1,   352,    -1,   368,
      -1,   355,    -1,   361,    -1,   390,    -1,   385,    -1,   373,
      -1,   332,    -1,   391,    -1,   392,    -1,   393,    -1,   395,
      -1,   338,    -1,   336,    -1,   384,    -1,   388,    -1,   387,
      -1,   371,    -1,   372,    -1,   374,    -1,   366,    -1,   369,
      -1,   227,    -1,   229,    -1,   231,    -1,   233,    -1,   235,
      -1,   237,    -1,   239,    -1,   241,    -1,   243,    -1,   226,
      -1,   228,    -1,   230,    -1,   232,    -1,   234,    -1,   236,
      -1,   238,    -1,   240,    -1,   242,    -1,   291,    -1,   292,
      -1,   220,    -1,   220,    -1,   292,    -1,   140,   210,   291,
      -1,   293,   268,   440,    -1,   293,   268,   198,   210,   449,
      -1,   293,   268,   439,    -1,   293,   268,   456,   210,   433,
      -1,   293,   268,   455,   210,   435,    -1,   293,   268,   454,
     210,   437,    -1,   293,   268,   428,    -1,   293,   268,   431,
      -1,   293,   268,   215,   302,   216,    -1,   293,   268,   215,
     301,   216,    -1,   293,   268,   215,   300,   216,    -1,   293,
     268,   456,   210,   215,   299,   216,    -1,   293,   268,   455,
     210,   215,   298,   216,    -1,   293,   268,   454,   210,   215,
     297,   216,    -1,   293,   268,   215,   303,   216,    -1,   438,
      -1,   297,   209,   438,    -1,   436,    -1,   298,   209,   436,
      -1,   434,    -1,   299,   209,   434,    -1,   432,    -1,   300,
     209,   432,    -1,   429,    -1,   301,   209,   429,    -1,   440,
      -1,   302,   209,   440,    -1,   439,    -1,   303,   209,   439,
      -1,    -1,    -1,   189,   219,   211,   305,   444,   212,   306,
     289,    -1,   219,   211,   447,   212,    -1,   139,    -1,    -1,
       6,    -1,     6,    -1,   213,   440,   191,   440,   214,    -1,
     213,   440,   214,    -1,    -1,    -1,   148,   312,   213,   427,
     214,   313,   215,   314,   216,    -1,    -1,   315,   314,    -1,
      -1,   149,   452,   210,   316,   288,    -1,    -1,   151,   210,
     317,   288,    -1,   150,    -1,   269,   269,    -1,   147,    -1,
     146,   294,   268,   440,   319,   440,    -1,    -1,   320,   322,
     289,    -1,    -1,   145,   213,   427,   214,   324,   289,    -1,
      -1,   310,   210,   326,   290,    -1,    -1,   126,   310,   328,
     329,    -1,   289,    -1,    -1,   289,   330,   127,   289,    -1,
      14,   210,   439,    -1,    71,    -1,    71,   210,   452,    -1,
      71,   210,    81,    -1,    59,   210,   334,   209,   334,    -1,
      59,   210,   334,   209,   452,    -1,   211,     4,   209,    80,
     209,   349,   212,    -1,   411,   309,   209,   421,    -1,    -1,
     116,   210,   335,   209,   342,   209,   344,   412,   415,   417,
     419,   337,   289,    -1,    -1,    51,   210,   335,   209,   341,
     209,   343,   209,   344,   412,   415,   417,   419,   339,   289,
      -1,    -1,   209,     5,    -1,   211,     4,   209,     4,   212,
      -1,    81,    -1,   211,     4,   209,     4,   212,    -1,    81,
      -1,   211,   353,   209,   354,   212,    -1,    81,    -1,   211,
       4,   209,     4,   212,    -1,    81,    -1,   125,   210,   346,
     209,   420,   209,   347,   209,   349,   350,    -1,    27,   210,
     420,   209,   449,   350,    -1,     5,    -1,    81,    -1,   348,
      -1,    81,    -1,    80,    -1,    80,   270,   348,    -1,     4,
      -1,    81,    -1,    -1,   350,   209,   351,    -1,   131,   210,
     132,    -1,    77,   210,   437,    -1,   114,   210,     4,    -1,
     157,    -1,   107,    -1,   108,    -1,   123,    -1,    20,    -1,
      19,   210,   353,   209,   354,   340,   218,    -1,    19,   210,
     428,   340,   218,    -1,    85,    -1,    86,    -1,    87,    -1,
      86,    -1,    25,   210,   357,    -1,    -1,    25,   210,   357,
     356,   289,    -1,   435,   209,   428,   358,    -1,    -1,   358,
     209,   359,    -1,   439,    -1,   120,    -1,   121,    -1,   423,
      -1,   124,   439,    -1,   158,    -1,   159,    -1,   155,    -1,
     162,    -1,   163,    -1,   164,    -1,   165,   210,   427,    -1,
     166,   210,   427,    -1,   169,   210,   427,    -1,   170,    -1,
     171,    -1,   172,   210,   360,    -1,   160,    -1,   161,    -1,
     167,    -1,   168,   210,   427,    -1,   217,    -1,   173,    -1,
     217,   270,   360,    -1,    23,   210,   363,    -1,    -1,    24,
     210,   363,   362,   289,    -1,   437,   364,    -1,    -1,   364,
     209,   365,    -1,    74,    -1,   174,   210,   435,    -1,   453,
      -1,   117,   210,   439,    -1,   143,   210,   427,    -1,   144,
      -1,    73,    -1,   152,   210,   427,    -1,   188,    -1,    72,
      -1,   153,    -1,   154,   210,   427,    -1,   155,    -1,   156,
      -1,   428,    -1,   105,   210,   102,    -1,   104,   210,   103,
      -1,   106,   210,   427,    -1,   113,   210,   427,    -1,   114,
     210,   427,    -1,   141,   210,   142,    -1,   115,   210,   427,
      -1,    77,   210,   437,    -1,   157,    -1,   108,    -1,    79,
      -1,    75,    -1,   111,   210,   109,    -1,   112,   210,   110,
      -1,    58,   210,   427,    -1,    26,   210,   410,   209,   428,
      -1,    28,   210,   428,   209,    80,   209,   420,    -1,    33,
     210,   428,   209,    80,    -1,    33,   210,   428,   209,    80,
     209,     5,   284,    -1,    34,    -1,    34,   210,   449,    -1,
      34,   210,   173,    -1,    47,   210,   428,   209,    90,    -1,
      48,   210,   428,   209,    90,    -1,    48,   210,   458,   209,
     458,   209,    90,    -1,    53,   210,   458,   209,   458,   209,
     217,    -1,    54,   210,   458,   209,   458,   376,    -1,    55,
     210,   458,   209,   458,    -1,    -1,   209,    90,    -1,    62,
     210,   449,   209,    94,    -1,    63,   210,   449,    -1,    64,
     210,   428,   209,   181,   210,   428,   209,    92,   209,    91,
      -1,    64,   210,   428,   209,    65,   209,    66,   209,    67,
     209,    92,   209,    91,    -1,    78,   210,   428,   209,   178,
     381,    -1,    -1,   381,   209,   382,    -1,    91,    -1,   174,
     210,   435,    -1,   175,   210,   437,    -1,   176,   210,   386,
      -1,   177,   210,   386,    -1,   179,    -1,   180,    -1,   181,
     210,   428,    -1,   182,   210,   410,    -1,   131,   210,   383,
      -1,    98,   210,   418,    -1,   183,   210,     4,    -1,   114,
     210,     4,    -1,     4,    -1,    92,    -1,   132,    -1,    94,
      -1,    69,   210,   449,    -1,    68,   210,   449,    -1,     3,
      -1,   211,     3,   209,   421,   212,    -1,   130,   210,   431,
     209,   433,   209,   433,   209,     7,    -1,   128,   210,   449,
     209,   433,    -1,   128,   210,   449,   209,   433,   209,    97,
      -1,    49,   210,   431,    -1,    50,   210,   431,    -1,    39,
     210,   217,   209,    40,    -1,    39,   210,   217,   209,    40,
     209,   174,   210,   435,    -1,    36,   210,   431,   209,    38,
      -1,    37,   210,   458,   209,    38,    -1,    22,   210,   449,
     209,    96,    -1,    -1,    35,   210,   431,   209,   421,   209,
     411,   412,   415,   417,   419,   396,   397,    -1,    -1,   289,
      -1,    44,   210,   428,   209,   422,   209,   424,    -1,    44,
     210,   428,   209,   422,   209,   424,   209,   174,   210,   435,
      -1,    44,   210,   428,   209,   422,   209,   424,   209,    89,
      -1,    44,   210,   428,   209,   422,   209,   424,   209,    89,
     209,   174,   210,   435,    -1,    45,   210,   428,    -1,    45,
     210,   428,   209,   174,   210,   435,    -1,    99,   210,   449,
     209,   100,   209,    97,    -1,   122,   210,   449,   209,     4,
      -1,   122,   210,   449,   209,   100,   209,    97,    -1,    46,
     210,    93,   209,   428,    -1,    46,   210,    93,   209,   428,
     209,   174,   210,   435,    -1,   184,   210,   428,   209,   439,
      -1,   184,   210,   428,   209,    81,    -1,   184,   210,   428,
      -1,   185,   210,   428,   209,   133,   209,   421,    -1,   185,
     210,   428,    -1,   186,   210,   428,   209,   134,   209,   439,
      -1,   186,   210,   428,   209,   134,   209,    81,    -1,   186,
     210,   428,    -1,   187,   210,   428,   209,   135,    -1,   187,
     210,   428,   209,   137,    -1,   187,   210,   428,    -1,    60,
     210,   440,   209,   428,    -1,    61,   210,   428,   209,   457,
     209,   439,    -1,    57,   210,   427,   209,   427,   209,   427,
     209,   427,    -1,    57,    -1,   217,    -1,    81,    -1,   217,
      -1,    81,    -1,    -1,   209,   413,    -1,   414,    -1,   414,
     209,   413,    -1,    41,    -1,    42,    -1,    43,    -1,    -1,
     101,   210,   416,    -1,   100,    -1,    -1,    98,   210,   418,
      -1,    97,    -1,    -1,   174,   210,   435,    -1,    72,    -1,
      81,    -1,    73,    -1,    81,    -1,    84,    -1,   425,    -1,
      81,    -1,    84,    -1,   425,    -1,    83,   210,    81,    -1,
      88,    -1,    81,    -1,    83,   213,     4,   214,    -1,   217,
      -1,   228,    -1,   229,   213,   440,   214,    -1,   440,    -1,
     429,    -1,   200,   211,   449,   212,    -1,   232,    -1,   233,
     213,   440,   214,    -1,   211,     4,   209,     4,   212,    -1,
      81,    -1,    82,   430,   214,    -1,   208,    -1,   208,   209,
     430,    -1,   432,    -1,   234,    -1,   235,   213,   440,   214,
      -1,   211,     4,   209,     4,   209,     4,   209,     4,   212,
      -1,   434,    -1,   236,    -1,   237,   213,   440,   214,    -1,
       3,    -1,   211,     3,   209,   421,   212,    -1,   436,    -1,
     238,    -1,   239,   213,   440,   214,    -1,   217,    -1,     3,
      -1,   211,     3,   209,   217,   212,    -1,    81,    -1,   438,
      -1,   240,    -1,   241,   213,   440,   214,    -1,   217,    -1,
       3,    -1,   211,     3,   209,   217,   212,    -1,    81,    -1,
      95,    -1,   426,    -1,   439,   269,   426,    -1,     4,    -1,
     450,    -1,   211,     8,   212,    -1,   226,    -1,   227,   213,
     440,   214,    -1,   440,   263,   440,    -1,   440,   264,   440,
      -1,   440,   265,   440,    -1,   440,   266,   440,    -1,   440,
     267,   440,    -1,   211,   440,   212,    -1,   222,    -1,   223,
      -1,   293,   210,   441,    -1,   442,    -1,   443,   209,   442,
      -1,    -1,   443,    -1,   440,    -1,   439,    -1,   445,    -1,
     446,   209,   445,    -1,    -1,   446,    -1,   428,    -1,   193,
     431,    -1,   194,   431,    -1,   195,   428,   209,   428,    -1,
     196,   428,   209,   428,   209,   440,    -1,   197,   211,   449,
     212,    -1,   197,   211,   348,   209,   449,   212,    -1,   203,
     211,     7,   209,   449,   212,    -1,   203,   211,   449,   209,
     449,   212,    -1,   203,   211,   433,   209,   449,   212,    -1,
     199,   428,    -1,   201,   211,   428,   209,   440,   212,    -1,
     201,   211,   428,   209,   440,   209,    41,   212,    -1,   202,
     211,   428,   209,   440,   209,   440,   212,    -1,   202,   211,
     428,   209,   440,   209,   440,   209,    41,   212,    -1,   205,
     211,   206,   209,   211,   440,   264,   440,   282,   212,   209,
     428,   283,   212,    -1,   204,   448,    -1,   242,    -1,   211,
     449,   212,    -1,   448,    -1,   448,   271,   449,    -1,   262,
      -1,    96,    -1,     4,    -1,     8,    -1,     9,    -1,     4,
      -1,     8,    -1,     9,    -1,     4,    -1,   450,    -1,    30,
      -1,    23,    -1,    31,    -1,    25,    -1,    32,    -1,   128,
      -1,    76,    -1,    81,    -1,   459,    -1,    56,   211,     4,
     209,     4,   209,     4,   209,     4,   212,    -1,   211,     4,
     209,     4,   209,     4,   209,     4,   212,    -1
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
    1656,  1661,  1666,  1672,  1677,  1682,  1687,  1694,  1703,  1707,
    1720,  1729,  1728,  1746,  1756,  1762,  1770,  1776,  1781,  1786,
    1791,  1796,  1801,  1806,  1811,  1816,  1830,  1836,  1841,  1846,
    1851,  1856,  1861,  1866,  1871,  1876,  1881,  1886,  1891,  1896,
    1901,  1906,  1911,  1917,  1922,  1927,  1934,  1940,  1969,  1974,
    1982,  1988,  1992,  2000,  2007,  2014,  2024,  2034,  2050,  2061,
    2064,  2070,  2076,  2082,  2086,  2092,  2099,  2105,  2113,  2119,
    2124,  2129,  2134,  2139,  2145,  2151,  2156,  2161,  2166,  2171,
    2176,  2183,  2183,  2183,  2183,  2186,  2192,  2198,  2203,  2210,
    2217,  2221,  2227,  2233,  2239,  2244,  2251,  2257,  2267,  2274,
    2273,  2305,  2308,  2314,  2319,  2324,  2329,  2335,  2339,  2345,
    2351,  2355,  2361,  2365,  2371,  2375,  2380,  2387,  2391,  2398,
    2402,  2407,  2414,  2418,  2423,  2431,  2437,  2444,  2448,  2455,
    2463,  2466,  2476,  2480,  2483,  2489,  2493,  2500,  2504,  2508,
    2515,  2518,  2524,  2531,  2534,  2540,  2547,  2551,  2558,  2559,
    2562,  2563,  2566,  2567,  2568,  2574,  2575,  2576,  2582,  2583,
    2586,  2595,  2600,  2607,  2618,  2624,  2628,  2632,  2639,  2649,
    2656,  2660,  2666,  2670,  2678,  2682,  2689,  2699,  2712,  2716,
    2723,  2733,  2742,  2753,  2757,  2764,  2774,  2785,  2794,  2804,
    2810,  2814,  2821,  2831,  2842,  2851,  2861,  2865,  2872,  2873,
    2879,  2883,  2887,  2891,  2899,  2908,  2912,  2916,  2920,  2924,
    2928,  2931,  2938,  2947,  2980,  2981,  2984,  2985,  2988,  2992,
    2999,  3006,  3017,  3020,  3028,  3032,  3036,  3040,  3044,  3049,
    3053,  3057,  3062,  3067,  3072,  3076,  3081,  3086,  3090,  3094,
    3099,  3103,  3110,  3116,  3120,  3126,  3133,  3134,  3137,  3138,
    3139,  3142,  3146,  3150,  3154,  3160,  3161,  3164,  3165,  3168,
    3169,  3172,  3173,  3176,  3180,  3206
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
  "BLINDED_ID", "MAXHP_ID", "LEVEL_ADJUSTMENT_ID", "PARALYZED_ID",
  "STUNNED_ID", "CONFUSED_ID", "SEENTRAPS_ID", "ALL_ID", "MONTYPE_ID",
  "OBJTYPE_ID", "TERTYPE_ID", "TERTYPE2_ID", "LEVER_EFFECT_TYPE",
  "SWITCHABLE_ID", "CONTINUOUSLY_USABLE_ID", "TARGET_ID", "TRAPTYPE_ID",
  "EFFECT_FLAG_ID", "GRAVE_ID", "BRAZIER_ID", "SIGNPOST_ID", "TREE_ID",
  "ERODEPROOF_ID", "FUNCTION_ID", "MSG_OUTPUT_TYPE", "COMPARE_TYPE",
  "UNKNOWN_TYPE", "rect_ID", "fillrect_ID", "line_ID", "randline_ID",
  "grow_ID", "selection_ID", "flood_ID", "rndcoord_ID", "circle_ID",
  "ellipse_ID", "filter_ID", "complement_ID", "gradient_ID",
  "GRADIENT_TYPE", "LIMITED", "HUMIDITY_TYPE", "','", "':'", "'('", "')'",
  "'['", "']'", "'{'", "'}'", "STRING", "MAP_ID", "NQSTRING", "VARSTRING",
  "CFUNC", "CFUNC_INT", "CFUNC_STR", "CFUNC_COORD", "CFUNC_REGION",
  "VARSTRING_INT", "VARSTRING_INT_ARRAY", "VARSTRING_STRING",
  "VARSTRING_STRING_ARRAY", "VARSTRING_VAR", "VARSTRING_VAR_ARRAY",
  "VARSTRING_COORD", "VARSTRING_COORD_ARRAY", "VARSTRING_REGION",
  "VARSTRING_REGION_ARRAY", "VARSTRING_MAPCHAR", "VARSTRING_MAPCHAR_ARRAY",
  "VARSTRING_MONST", "VARSTRING_MONST_ARRAY", "VARSTRING_OBJ",
  "VARSTRING_OBJ_ARRAY", "VARSTRING_SEL", "VARSTRING_SEL_ARRAY",
  "METHOD_INT", "METHOD_INT_ARRAY", "METHOD_STRING", "METHOD_STRING_ARRAY",
  "METHOD_VAR", "METHOD_VAR_ARRAY", "METHOD_COORD", "METHOD_COORD_ARRAY",
  "METHOD_REGION", "METHOD_REGION_ARRAY", "METHOD_MAPCHAR",
  "METHOD_MAPCHAR_ARRAY", "METHOD_MONST", "METHOD_MONST_ARRAY",
  "METHOD_OBJ", "METHOD_OBJ_ARRAY", "METHOD_SEL", "METHOD_SEL_ARRAY",
  "DICE", "'+'", "'-'", "'*'", "'/'", "'%'", "'='", "'.'", "'|'", "'&'",
  "$accept", "file", "levels", "level", "level_def", "lev_init",
  "tileset_detail", "forest_detail", "monster_generation_detail",
  "boundary_type_detail", "opt_limited", "opt_coord_or_var",
  "opt_fillchar", "walled", "flags", "flag_list", "levstatements",
  "stmt_block", "levstatement", "any_var_array", "any_var",
  "any_var_or_arr", "any_var_or_unk", "shuffle_detail", "variable_define",
  "encodeobj_list", "encodemonster_list", "mapchar_list",
  "encoderegion_list", "encodecoord_list", "integer_list", "string_list",
  "function_define", "$@1", "$@2", "function_call", "exitstatement",
  "opt_percent", "comparestmt", "switchstatement", "$@3", "$@4",
  "switchcases", "switchcase", "$@5", "$@6", "breakstatement",
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
     455,   456,   457,   458,   459,   460,   461,   462,   463,    44,
      58,    40,    41,    91,    93,   123,   125,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,    43,    45,    42,    47,    37,    61,    46,
     124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   272,   273,   273,   274,   274,   275,   276,   276,   277,
     277,   277,   277,   278,   279,   279,   280,   281,   282,   282,
     283,   283,   284,   284,   285,   285,   286,   286,   287,   287,
     288,   288,   289,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   290,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   293,   293,   293,   294,   294,   295,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   297,   297,   298,   298,   299,   299,
     300,   300,   301,   301,   302,   302,   303,   303,   305,   306,
     304,   307,   308,   309,   309,   310,   310,   310,   312,   313,
     311,   314,   314,   316,   315,   317,   315,   318,   319,   319,
     320,   322,   321,   324,   323,   326,   325,   328,   327,   329,
     330,   329,   331,   332,   332,   332,   333,   333,   334,   335,
     337,   336,   339,   338,   340,   340,   341,   341,   342,   342,
     343,   343,   344,   344,   345,   345,   346,   346,   347,   347,
     348,   348,   349,   349,   350,   350,   351,   351,   351,   351,
     351,   351,   351,   352,   352,   352,   353,   353,   354,   354,
     355,   356,   355,   357,   358,   358,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   360,   360,   360,
     361,   362,   361,   363,   364,   364,   365,   365,   365,   365,
     365,   365,   365,   365,   365,   365,   365,   365,   365,   365,
     365,   365,   365,   365,   365,   365,   365,   365,   365,   365,
     365,   365,   365,   365,   365,   365,   366,   367,   368,   368,
     369,   369,   369,   370,   371,   372,   373,   374,   375,   376,
     376,   377,   378,   379,   379,   380,   381,   381,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   383,   383,   383,   383,   384,   385,   386,   386,   387,
     388,   388,   389,   390,   391,   391,   392,   393,   394,   396,
     395,   397,   397,   398,   398,   398,   398,   399,   399,   400,
     401,   401,   402,   402,   403,   403,   403,   404,   404,   405,
     405,   405,   406,   406,   406,   407,   408,   409,   409,   410,
     410,   411,   411,   412,   412,   413,   413,   414,   414,   414,
     415,   415,   416,   417,   417,   418,   419,   419,   420,   420,
     421,   421,   422,   422,   422,   423,   423,   423,   424,   424,
     425,   426,   426,   426,   427,   428,   428,   428,   428,   429,
     429,   429,   430,   430,   431,   431,   431,   432,   433,   433,
     433,   434,   434,   435,   435,   435,   436,   436,   436,   436,
     437,   437,   437,   438,   438,   438,   438,   438,   439,   439,
     440,   440,   440,   440,   440,   440,   440,   440,   440,   440,
     440,   441,   441,   442,   443,   443,   444,   444,   445,   445,
     446,   446,   447,   447,   448,   448,   448,   448,   448,   448,
     448,   448,   448,   448,   448,   448,   448,   448,   448,   448,
     448,   448,   448,   449,   449,   450,   451,   451,   452,   452,
     452,   453,   453,   453,   453,   454,   454,   455,   455,   456,
     456,   457,   457,   458,   458,   459
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
       1,     1,     3,     1,     1,     1,     3,     1,     1,     3,
       3,     0,     5,     2,     0,     3,     1,     3,     1,     3,
       3,     1,     1,     3,     1,     1,     1,     3,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     3,     3,     3,     5,     7,     5,     8,
       1,     3,     3,     5,     5,     7,     7,     6,     5,     0,
       2,     5,     3,    11,    13,     6,     0,     3,     1,     3,
       3,     3,     3,     1,     1,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     3,     3,     1,     5,     9,
       5,     7,     3,     3,     5,     9,     5,     5,     5,     0,
      13,     0,     1,     7,    11,     9,    13,     3,     7,     7,
       5,     7,     5,     9,     5,     5,     3,     7,     3,     7,
       7,     3,     5,     5,     3,     5,     7,     9,     1,     1,
       1,     1,     1,     0,     2,     1,     3,     1,     1,     1,
       0,     3,     1,     0,     3,     1,     0,     3,     1,     1,
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
       5,     0,    30,     0,     7,     0,   155,     0,     0,     0,
       0,   223,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   300,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,     0,
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
       0,   450,     0,   453,     0,   495,     0,   451,   472,    31,
       0,   175,     0,     8,     0,   411,   412,     0,   448,   182,
       0,     0,     0,    11,   497,   496,    13,   420,     0,   226,
     227,     0,     0,   417,     0,     0,   194,   415,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   491,   474,   493,     0,   444,   446,   447,     0,   443,
     441,     0,   260,   264,   440,   261,   437,   439,     0,   436,
     434,     0,   230,     0,   433,   380,   379,     0,   398,   399,
       0,     0,     0,     0,   302,   301,     0,   425,     0,     0,
     424,     0,     0,     0,     0,   513,     0,     0,   357,     0,
       0,     0,     0,     0,   342,   343,   382,   381,     0,   153,
       0,     0,     0,     0,   414,     0,     0,     0,     0,     0,
     312,     0,   336,   335,   500,   498,   499,   185,   184,     0,
       0,     0,     0,   206,   207,     0,     0,     0,     0,    15,
     118,     0,     0,     0,   366,   368,   371,   374,   148,     0,
       0,     0,     0,   157,     0,     0,     0,     0,     0,   469,
     468,   470,   473,     0,   506,   508,   505,   507,   509,   510,
       0,     0,     0,   125,   126,   121,   119,     0,     0,     0,
       0,    30,   172,    28,     0,     0,     0,     0,     0,   422,
       0,     0,     0,     0,     0,     0,     0,   475,   476,     0,
       0,     0,   484,     0,     0,     0,   490,     0,     0,     0,
       0,     0,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     154,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   179,   178,     0,
       0,     0,   173,     0,     0,     0,     0,     0,     0,   466,
     452,   460,     0,     0,   455,   456,   457,   458,   459,     0,
     151,     0,   450,     0,     0,     0,     0,   142,   140,   146,
     144,     0,     0,     0,   176,     0,     0,   449,    10,   337,
       0,     9,     0,     0,   421,     0,     0,     0,   229,   228,
     194,   195,   225,     0,     0,   210,     0,     0,     0,     0,
     431,     0,     0,   429,     0,     0,   428,     0,     0,   492,
     494,   348,     0,     0,     0,   262,     0,     0,   232,   234,
     296,   214,     0,    16,   136,   298,     0,     0,   400,   401,
       0,   346,     0,     0,   347,   344,   404,     0,   402,     0,
     403,     0,   362,   303,     0,   304,     0,   197,     0,     0,
       0,     0,   309,   308,     0,     0,   186,   187,   375,   511,
     512,     0,   311,     0,     0,   316,     0,   199,     0,     0,
     360,     0,     0,     0,     0,   340,     0,    14,     0,   169,
       0,     0,   159,   365,   364,     0,     0,   372,   373,     0,
     464,   467,     0,   454,   156,   471,   120,     0,     0,   129,
       0,   128,     0,   127,     0,   133,     0,   124,     0,   123,
       0,   122,    32,   413,     0,     0,   423,   416,     0,   418,
       0,   477,     0,     0,     0,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   442,   503,   501,   502,     0,
     275,   272,   266,   292,     0,   291,     0,     0,     0,   290,
       0,     0,     0,     0,     0,     0,     0,     0,   271,     0,
     276,     0,   278,   279,   289,     0,   274,   265,   280,   504,
     268,     0,   435,   233,   205,     0,     0,     0,     0,   426,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   189,     0,     0,   307,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
     174,   168,   170,     0,     0,     0,     0,     0,   149,     0,
     141,   143,   145,   147,     0,   134,     0,     0,   138,     0,
       0,   419,   224,     0,   211,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   445,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,     0,     0,   297,   137,    22,     0,   383,     0,
       0,     0,     0,   409,   408,   353,   358,     0,   305,     0,
     201,     0,     0,   306,   310,     0,     0,   376,     0,     0,
       0,   359,     0,   203,     0,   383,   361,   209,     0,   208,
     181,   341,     0,   161,   367,   370,   369,   461,   462,   463,
     465,     0,     0,   132,   131,     0,   130,     0,     0,   478,
     480,     0,   485,     0,   481,     0,   430,   483,   482,     0,
     295,   288,   282,   281,   283,   293,   294,   284,   285,   287,
     269,   286,   270,   273,   277,   267,     0,   405,   237,   238,
       0,   243,   241,   242,   253,   254,   244,   245,   246,     0,
       0,   255,     0,     0,   250,   251,     0,   235,   239,   406,
     236,     0,   220,   221,     0,   222,     0,   219,   215,     0,
     299,     0,     0,   390,     0,     0,     0,   410,     0,     0,
       0,     0,     0,     0,   212,   213,     0,     0,     0,   318,
       0,     0,     0,     0,     0,     0,     0,   323,   324,     0,
       0,     0,   317,     0,     0,   390,     0,     0,     0,     0,
       0,   161,   150,   135,   139,   338,     0,     0,     0,   432,
       0,     0,   240,     0,     0,     0,     0,     0,     0,     0,
       0,    23,     0,   387,   388,   389,   384,   385,     0,   393,
       0,     0,   345,   355,     0,   363,   196,     0,   383,   377,
     188,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   198,     0,   393,   214,   339,     0,   165,
     160,   162,     0,   486,     0,   487,   456,   407,   247,   248,
     256,   249,   258,   257,   252,   217,   218,   216,     0,     0,
       0,     0,   396,     0,     0,     0,     0,     0,   390,     0,
       0,   395,   328,   330,   331,   332,   334,   333,   327,   319,
     320,   321,   322,   325,   326,   329,     0,   396,   204,   163,
      30,     0,     0,     0,     0,     0,   427,   386,   392,   391,
       0,     0,   349,     0,   515,     0,   354,   200,   393,     0,
     313,   202,   190,    30,   166,     0,   488,    19,     0,   259,
     394,     0,   351,   514,     0,   396,     0,     0,   164,     0,
       0,   397,   352,   350,   356,   192,   314,   191,     0,    20,
       0,    24,    25,    22,     0,     0,   193,    12,    21,   489
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    99,   100,   101,   102,   103,
    1084,  1135,   930,  1133,    12,   170,   104,   412,   105,   106,
     107,   108,   224,   109,   110,   774,   573,   777,   513,   514,
     515,   516,   111,   499,   861,   112,   113,   471,   114,   115,
     225,   763,   970,   971,  1103,  1080,   116,   631,   117,   118,
     242,   119,   628,   120,   410,   121,   366,   488,   623,   122,
     123,   124,   346,   338,   125,  1117,   126,  1130,   426,   599,
     619,   832,   845,   127,   365,   848,   546,   946,   724,   928,
     128,   265,   540,   129,   447,   302,   723,   917,  1044,   130,
     444,   292,   443,   717,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   745,   141,   142,   143,   144,   751,
     962,  1068,   145,   146,   531,   147,   148,   149,   150,   151,
     152,   153,   154,   155,  1112,  1123,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   307,   339,
     933,   996,   997,   999,  1089,  1052,  1062,  1092,   310,   580,
     589,   918,   825,   590,   248,   343,   282,   267,   420,   319,
     320,   555,   556,   303,   304,   293,   294,   389,   344,   859,
     640,   641,   642,   391,   392,   393,   283,   438,   237,   256,
     358,   720,   407,   408,   409,   611,   324,   325
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -988
static const yytype_int16 yypact[] =
{
     155,  -123,   -84,   130,  -988,   155,    16,   -55,   -32,  -988,
    -988,   -19,   795,   -33,  -988,    89,  -988,    24,    52,    60,
      67,  -988,    94,   111,   137,   147,   157,   159,   163,   182,
     188,   196,   201,   203,   208,   210,   212,   216,   218,   219,
     223,   224,   226,   227,   234,   235,   241,   243,   244,   245,
     254,   264,   275,   276,   285,   286,   288,   289,   292,   293,
     296,   301,   302,    37,   303,   310,   311,  -988,   312,     9,
     763,  -988,  -988,   314,   315,   316,   324,    49,    12,   115,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,   795,  -988,  -988,   -38,  -988,
    -988,  -988,  -988,  -988,   332,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,   362,   169,
    -988,  -124,   448,    65,   381,   297,   886,    92,    92,   107,
     -34,    19,   -22,   411,   -22,   867,   -97,   -97,   -12,   267,
     -22,   -22,   390,   -22,    -4,   -97,   -97,   -16,   -12,   -12,
     -12,    12,   294,    12,   -22,   886,   886,   -22,   886,   886,
      72,   -22,   886,   -16,   886,    87,  -988,   886,   -97,   406,
     553,    12,  -988,  -988,   279,   306,   -22,   -22,   -22,   -22,
     337,  -988,    32,  -988,   336,  -988,   251,  -988,    20,  -988,
     400,  -988,   338,  -988,    89,  -988,  -988,   341,  -988,   281,
     346,   347,   349,  -988,  -988,  -988,  -988,  -988,   351,  -988,
    -988,   358,   556,  -988,   353,   361,   364,  -988,  -988,   -97,
     -97,   -22,   -22,   363,   -22,   368,   375,   384,   886,   399,
     490,  -988,  -988,   304,   403,  -988,  -988,  -988,   590,  -988,
    -988,   405,  -988,  -988,  -988,  -988,  -988,  -988,   613,  -988,
    -988,   407,   404,   412,  -988,  -988,  -988,   415,  -988,  -988,
     416,   421,   422,   432,  -988,  -988,   639,  -988,   437,   443,
    -988,   444,   457,   650,   446,  -988,   452,   468,   469,   471,
     473,   675,   479,   495,  -988,  -988,  -988,  -988,   496,   704,
     502,   509,   511,   512,   273,   726,   522,   373,   524,   538,
    -988,   540,  -988,  -988,  -988,  -988,  -988,  -988,  -988,   545,
     546,   554,   559,  -988,  -988,   561,   338,   562,   563,   566,
    -988,   534,    12,    12,   570,   572,   576,   578,  -988,   550,
     123,    12,    12,  -988,    12,    12,    12,    12,    12,   281,
     273,  -988,   580,   579,  -988,  -988,  -988,  -988,  -988,  -988,
     583,    53,    25,  -988,  -988,   281,   273,   585,   587,   588,
     795,   795,  -988,  -988,    12,  -124,   747,    22,   797,   598,
     594,   886,   601,    12,   274,   806,   608,  -988,  -988,   618,
     624,   696,  -988,   -22,   -22,   470,  -988,   629,   625,   886,
     740,   642,    12,   644,   338,   651,    12,   338,   -22,   -22,
     886,   758,    46,   781,   653,    12,    38,   827,   863,   659,
     831,   830,    62,   697,   -22,   782,   670,   790,   -12,   -18,
    -988,   672,   -12,   -12,   -12,    12,   673,    33,   -22,     8,
     789,     3,   706,   785,   -11,    47,    19,   759,  -988,   119,
     119,   749,  -988,   -50,   674,   -48,   769,   770,    14,   925,
    -988,  -988,   136,   205,   -41,   -41,  -988,  -988,  -988,    20,
    -988,   886,   694,  -107,   -96,   -93,   -30,  -988,  -988,   281,
     273,    82,   114,   105,  -988,   689,   266,  -988,  -988,  -988,
     905,  -988,   700,   351,  -988,   698,   908,   393,  -988,  -988,
     364,  -988,  -988,   -22,   -22,   643,   705,   703,   707,   709,
    -988,   710,   541,  -988,   711,   713,  -988,   717,   718,  -988,
    -988,  -988,   715,   462,   335,  -988,   716,   494,  -988,  -988,
    -988,  -988,   721,   727,  -988,   728,   935,   539,  -988,  -988,
     733,  -988,   735,   942,  -988,   741,  -988,   738,  -988,   743,
    -988,   744,   746,  -988,   949,  -988,   748,  -988,   952,   750,
      38,   751,   754,  -988,   755,   878,  -988,  -988,  -988,  -988,
    -988,   756,  -988,   757,   760,  -988,   762,  -988,   968,   764,
    -988,   765,   767,   850,   975,   777,   778,  -988,   338,  -988,
     719,    12,  -988,  -988,   281,   783,   787,  -988,  -988,   780,
    -988,   791,   792,  -988,  -988,  -988,  -988,   990,   796,  -988,
     -36,  -988,    12,  -988,  -124,  -988,    61,  -988,    46,  -988,
      29,  -988,  -988,  -988,   800,  1003,  -988,  -988,   798,  -988,
     793,  -988,   803,   933,   886,  -988,    12,    12,   886,   807,
      12,   886,   886,   808,   805,  -988,  -988,  -988,  -988,   810,
    -988,  -988,  -988,  -988,   829,  -988,   832,   833,   834,  -988,
     839,   847,   848,   849,   855,   864,   865,   866,  -988,   874,
    -988,   884,  -988,  -988,  -988,   885,  -988,  -988,  -988,  -988,
    -988,   861,  -988,   809,   868,    19,    46,  1036,   889,  -988,
     -16,  1097,   893,   929,  1100,   116,   107,   931,   -91,  1016,
     898,    -9,  -988,   891,  1020,  -988,    12,   902,  -124,  1046,
     -22,   904,  1017,   906,    -8,  1019,   299,   338,  1023,   119,
    -988,  -988,   273,   907,    38,   -23,   153,   925,  -988,     0,
    -988,  -988,   273,   281,    11,  -988,    57,    76,  -988,    38,
     912,  -988,  -988,    12,  -988,   911,   298,   433,   913,    38,
     611,   914,   917,    12,  -988,    12,    92,  1014,  1022,    12,
    1037,  1038,    12,    12,    12,  -124,  1005,    12,    12,    12,
     107,  -988,   972,    17,  -988,  -988,   940,  1146,   960,   961,
    1167,   962,   959,  -988,  -988,   965,  -988,   966,  -988,  1171,
    -988,   309,   969,  -988,  -988,   970,    84,   281,   971,   973,
     871,  -988,  1173,  -988,  1177,   960,  -988,  -988,   974,  -988,
    -988,  -988,   976,   168,  -988,  -988,   281,  -988,  -988,  -988,
    -988,   338,    61,  -988,  -988,    29,  -988,   978,  1179,   273,
    -988,  1145,  -988,    12,  -988,   979,  -988,  -988,  -988,   450,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
     281,  -988,  -988,  -988,  -988,  -988,   141,  -988,  -988,  -988,
    -124,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,   977,
     982,  -988,   983,   984,  -988,  -988,   985,  -988,  -988,  -988,
     281,   986,  -988,  -988,   987,  -988,   988,  -988,  -988,  1185,
    -988,   993,   295,  1098,  1199,   995,   107,  -988,   -14,   107,
     994,   996,    -8,    12,  -988,  -988,   997,  1140,  1116,  -988,
    1000,  1001,  1002,  1004,  1006,  1007,  1008,  -988,  -988,  1009,
    1010,  1011,  -988,  1012,  1013,  1098,    84,  1206,   355,  1015,
     999,   168,  -988,  -988,  -988,  -988,  1018,  1021,   382,  -988,
      12,  1142,   281,    12,    12,    12,    12,  -102,    92,  1222,
    1096,  -988,  1225,  -988,  -988,  -988,  -988,  1025,  1026,  1132,
    1028,  1227,  -988,  1029,  1030,  -988,  -988,   274,   960,  -988,
    -988,  1032,  1033,  1135,  1231,    58,   107,    92,    22,    22,
     -22,   -34,  1235,  -988,  1239,  1132,  -988,  -988,  1034,  -988,
    -988,  -988,  1240,  -988,  1205,  -988,    23,  -988,  -988,  -988,
    -988,  -988,  -988,   980,  -988,  -988,  -988,  -988,  1035,   295,
    1148,  1039,  1077,  1248,  1041,  1080,   107,  1043,  1098,  1164,
    1166,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  1047,  1077,   868,  -988,
     795,  1049,  1048,  1054,  1050,  -102,  -988,  -988,  -988,  -988,
    1135,  1053,  -988,  1052,  -988,  1055,  -988,  -988,  1132,  1057,
    -988,  -988,  -988,   795,  -988,    38,  -988,  -988,  1058,  -988,
    -988,   107,   338,  -988,   107,  1077,  1178,   338,  -988,  1059,
     -22,  -988,  -988,  -988,  -988,  -988,  -988,  -988,    91,  1061,
     338,  -988,  -988,   940,   -22,  1060,  -988,  -988,  -988,  -988
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -988,  -988,  1266,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,   140,  -988,  -988,  1031,  -105,  -361,   869,  1056,
    1204,  -486,  -988,  -988,  -988,  -988,   619,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  1215,  -988,
    -988,  -988,   313,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,   804,  1067,  -988,  -988,  -988,  -988,   742,  -988,
    -988,  -988,   343,  -988,  -988,  -988,  -599,   317,   260,  -988,
    -988,   456,   282,  -988,  -988,  -988,  -988,  -988,   206,  -988,
    -988,  1110,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -634,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,   269,   564,
    -810,   246,  -988,  -899,  -988,  -943,   202,  -987,  -456,  -597,
    -988,  -988,  -988,   481,   881,  -219,  -173,  -379,   766,   187,
    -383,  -475,  -610,  -515,  -440,  -513,  -617,  -161,   -74,  -988,
     530,  -988,  -988,   794,  -988,  -988,  1024,   -73,   734,  -988,
    -471,  -988,  -988,  -988,  -988,  -988,  -172,  -988
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -232
static const yytype_int16 yytable[] =
{
     239,   266,   371,   742,   236,   487,   607,   659,   657,   311,
     249,   313,   574,   639,   625,   626,   231,   327,   328,   518,
     330,   332,   333,   517,   231,   529,   340,   341,   342,   231,
     622,   348,   550,   633,   351,   965,   231,   354,   359,   775,
     379,   355,   356,    16,   322,   257,   258,   305,   661,   296,
     778,   620,   322,   374,   375,   376,   377,   512,   855,   257,
     258,   379,  1064,   597,   285,   336,  1025,   403,   613,   254,
     617,  1042,   830,   843,   784,  1003,   354,   257,   258,   405,
     355,   356,  1077,   565,   609,   285,   568,     7,   944,   610,
    1102,   308,   363,   245,   921,   285,  1131,   629,   429,   430,
     309,   432,   648,   284,   246,   247,   257,   258,   550,   649,
     296,   578,   315,   650,   316,  1043,   652,   296,   820,   579,
     651,   781,   550,   653,   922,   923,     8,   297,  1125,   347,
       9,   924,   349,   350,    11,   352,   353,   317,   318,   360,
     925,   362,   286,   586,   367,   587,   588,   621,   926,   637,
    1065,   638,  1066,   357,   494,  1115,   287,   849,   380,  1098,
    1004,   255,    13,   286,   390,   945,   406,   854,   364,   245,
       1,     2,  1132,   286,   927,   262,   168,   287,   261,   654,
     246,   247,   867,   306,   614,    14,   655,   287,   297,   262,
    1067,    15,   875,   598,   245,   297,   261,   823,  1058,   323,
     618,   337,   831,   844,   824,   246,   247,   331,   169,   817,
     263,   264,   781,   384,   385,   386,   387,   388,   574,   630,
     862,   826,   221,   232,   386,   387,   388,   863,   263,   264,
     240,   232,  1083,   530,   171,   -18,   401,   245,   233,   234,
     624,   519,   245,   232,   345,   973,   233,   234,   246,   247,
      78,   233,   234,   246,   247,   974,   604,   298,   233,   234,
     548,   549,   172,   299,   232,   770,   726,   760,   230,   814,
     173,   771,   288,   864,   235,   569,   570,   174,   289,   233,
     234,   639,   235,   881,   852,   865,   815,   235,   386,   387,
     388,   592,   866,   288,   235,   895,   596,   656,   493,   289,
     601,   602,   603,   288,   175,   608,   525,   502,   503,   289,
     504,   505,   506,   507,   508,   235,   624,   968,   298,   969,
     660,   176,   290,   291,   299,   298,   238,   380,   520,   658,
     624,   299,   290,   291,   634,   501,   993,   994,   995,   686,
     526,   553,   554,   687,   688,   300,   301,   177,   535,   537,
     643,   981,   300,   301,   734,   553,   554,   178,   547,   354,
     538,   539,   557,   355,   356,   243,   560,   179,   563,   180,
     671,   672,   567,   181,   321,   857,   858,   571,   244,   545,
     847,   577,   334,   335,  1071,  1072,   384,   385,   386,   387,
     388,   718,   182,   689,   259,   260,   850,   268,   183,   384,
     385,   386,   387,   388,   231,   368,   184,   690,   691,   692,
     693,   185,   694,   186,   695,   312,   257,   258,   187,   644,
     188,  1002,   189,   394,  1005,   395,   190,   404,   191,   192,
     396,   397,   398,   193,   194,   390,   195,   196,   646,   696,
     697,   698,   382,   699,   197,   198,   700,   701,   702,   703,
     704,   199,   705,   200,   201,   202,   427,   428,   250,   251,
     252,   253,   257,   258,   203,   383,   259,   260,   384,   385,
     386,   387,   388,   550,   204,  1045,   706,   551,   707,   708,
     663,   257,   258,   329,   326,   205,   206,   709,   710,   711,
     712,   713,   714,   773,   422,   207,   208,  1028,   209,   210,
     972,  1069,   211,   212,  1070,   345,   213,   871,  1119,   715,
     872,   214,   215,   217,   384,   385,   386,   387,   388,   373,
     218,   219,   220,   716,   226,   227,   228,   835,   399,   384,
     385,   386,   387,   388,   229,   261,   384,   385,   386,   387,
     388,  1096,   241,   369,   679,   422,   262,   372,   378,   381,
     415,   257,   258,   411,   414,   416,   417,   762,   418,   419,
     422,   384,   385,   386,   387,   388,   423,   263,   264,   421,
     424,   257,   258,   425,   431,   439,   880,   839,   772,   433,
     884,   261,   478,   887,   888,   889,   434,   837,   892,   893,
     894,  1034,   262,   441,  1035,   435,  1121,   235,   400,  1124,
     261,   785,   786,   787,   856,   788,   790,   669,   791,   792,
     437,   401,   440,   263,   264,   402,   445,   245,   442,  -231,
     446,   448,   257,   258,   449,   450,   233,   234,   246,   247,
     451,   452,   263,   264,   317,   318,   384,   385,   386,   387,
     388,   453,   873,   454,   890,   384,   385,   386,   387,   388,
     455,   920,   456,   457,   459,   460,   384,   385,   386,   387,
     388,   461,   235,   269,   270,   271,   272,   273,   458,   274,
     261,   275,   276,   277,   278,   279,   685,   462,   463,   466,
     464,   552,   465,   269,   270,   271,   272,   273,   467,   274,
     261,   275,   276,   277,   278,   279,   384,   385,   386,   387,
     388,   280,   263,   264,   468,   469,   553,   554,   722,   869,
     470,   472,   281,   384,   980,   386,   387,   388,   473,   879,
     474,   475,   263,   264,  1009,   384,   385,   386,   387,   388,
     476,   477,   281,   479,   269,   270,   271,   272,   273,   982,
     274,   261,   275,   276,   277,   278,   279,   480,   492,   481,
     528,  1122,   280,   729,   482,   483,  1127,   384,   385,   386,
     387,   388,   500,   484,  1038,  1039,  1040,  1041,   485,  1136,
     486,   489,   490,   263,   264,   491,   545,   257,   258,   495,
      82,   496,    84,   281,    86,   497,    88,   498,    90,   509,
      92,   510,    94,   511,    96,   521,    98,   522,   523,   978,
     532,    16,   384,   385,   386,   387,   388,   533,   534,    17,
     536,   541,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,   876,   542,   543,    31,    32,
      33,    34,    35,   544,    36,   558,   561,   559,   572,    37,
      38,    39,    40,    41,    42,    43,    44,  1073,    45,    46,
      47,   562,    48,   564,    49,    50,    51,    52,    53,    54,
     566,   575,   576,    55,    56,   581,    57,   582,   583,   584,
     585,   591,   593,    58,   384,   385,   386,   387,   388,   594,
     595,   600,   605,   612,   615,   616,  -180,   627,   632,   269,
     270,   271,   272,   273,    59,   274,   261,   275,   276,   277,
     278,   279,   635,   647,   636,   662,  1036,   280,   664,   665,
     667,    60,   668,   673,   674,   675,   676,    61,   677,   678,
      62,    63,   681,    64,   680,    65,   682,   683,   263,   264,
     725,    66,   684,   721,    67,    68,   726,   727,   281,   728,
      69,    70,   730,    71,   731,    72,   732,  1129,   257,   258,
     733,   734,   735,   738,   736,   737,   740,   739,   747,   741,
     743,  1138,   949,   744,   746,   748,   749,   257,   258,   950,
     750,   752,   753,   754,   755,  1104,   756,   757,   679,    73,
      74,    75,    76,   222,    77,   951,   758,   759,   761,    81,
     766,    83,   764,    85,   769,    87,   765,    89,  1118,    91,
     767,    93,   952,    95,   768,    97,   780,   316,    78,   779,
     781,   782,   783,   545,    79,    80,   789,   794,   812,   793,
     795,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,   796,
     314,   816,   797,   798,   799,   953,   954,   955,   956,   800,
     957,   958,   959,   960,   961,   896,   897,   801,   802,   803,
     269,   270,   271,   272,   273,   804,   274,   261,   275,   276,
     277,   278,   279,   811,   805,   806,   807,   813,   280,   269,
     270,   271,   272,   273,   808,   274,   261,   275,   276,   277,
     278,   279,   898,   899,   809,   810,   900,   280,   817,   263,
     264,   819,   820,   821,   822,   827,   828,   829,   833,   281,
     834,   836,   838,   840,   841,   842,   846,   882,   263,   264,
     851,   868,   853,   870,   883,   874,   877,   901,   281,   878,
     902,   903,   904,   905,   906,   907,   908,   909,   910,   911,
     912,   913,   914,   915,   916,    80,   885,   891,   886,   929,
     931,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,   932,
     934,   935,   936,   937,   938,   940,   939,   963,   942,   943,
     947,   964,   948,   966,   976,   967,   977,   983,   991,   245,
     975,   979,   984,   985,   986,   987,   988,   989,   990,   998,
     246,   247,   992,  1000,  1001,  1007,  1006,  1011,  1012,  1010,
    1013,  1014,  1015,  1027,  1016,  1030,  1017,  1018,  1019,  1020,
    1021,  1022,  1024,  1037,  1023,  1029,  1046,  1032,  1047,  1048,
    1051,  1054,  1061,  1033,  1049,  1063,  1050,  1053,  1055,  1075,
    1056,  1059,  1060,  1076,  1079,  1081,  1082,  1086,  1088,  1090,
    1085,  1091,  1093,  1094,  1095,  1097,  1099,  1100,  1105,  1101,
    1106,  1107,  1108,  1111,  1113,  1114,  1116,  1120,  1128,  1126,
    1134,    10,  1139,  1137,   223,   413,   370,   776,   216,   524,
     361,   606,   670,  1026,  1031,  1008,  1078,   941,   295,  1057,
    1074,  1109,  1110,   919,   818,  1087,   527,   860,   719,   666,
       0,     0,   436,   645
};

static const yytype_int16 yycheck[] =
{
     105,   174,   221,   600,    78,   366,   477,   522,   521,   182,
     171,   184,   452,   499,   489,   490,     4,   190,   191,   402,
     193,   194,   194,   402,     4,     3,   198,   199,   200,     4,
     486,   204,     3,    81,   207,   845,     4,     4,   211,   656,
       8,     8,     9,     6,    56,    81,    82,    81,   523,     3,
     660,     4,    56,   226,   227,   228,   229,     4,    81,    81,
      82,     8,     4,    81,     3,    81,   965,   240,    65,     4,
      81,   173,    81,    81,   673,    89,     4,    81,    82,   240,
       8,     9,  1025,   444,    76,     3,   447,   210,     4,    81,
    1077,    72,     5,   217,    77,     3,     5,   147,   271,   272,
      81,   274,   209,   176,   228,   229,    81,    82,     3,   216,
       3,    73,   185,   209,   211,   217,   209,     3,   209,    81,
     216,   212,     3,   216,   107,   108,   210,    81,  1115,   203,
       0,   114,   205,   206,   118,   208,   209,   234,   235,   212,
     123,   214,    81,    81,   217,    83,    84,   100,   131,   135,
      92,   137,    94,    81,   373,  1098,    95,   756,   232,  1058,
     174,    96,   217,    81,   238,    81,   240,   764,    81,   217,
      15,    16,    81,    81,   157,   211,   209,    95,   200,   209,
     228,   229,   779,   217,   181,   217,   216,    95,    81,   211,
     132,   210,   789,   211,   217,    81,   200,    81,  1008,   211,
     211,   217,   211,   211,    88,   228,   229,   211,   119,   209,
     232,   233,   212,   263,   264,   265,   266,   267,   658,   269,
     209,   736,   213,   211,   265,   266,   267,   216,   232,   233,
     268,   211,   209,   211,   210,   212,   211,   217,   226,   227,
     211,   402,   217,   211,   211,   862,   226,   227,   228,   229,
     213,   226,   227,   228,   229,   865,   475,   211,   226,   227,
     433,   434,   210,   217,   211,   648,   209,   628,   219,   725,
     210,   650,   211,   216,   262,   448,   449,   210,   217,   226,
     227,   767,   262,   796,   759,   209,   726,   262,   265,   266,
     267,   464,   216,   211,   262,   810,   468,   215,   372,   217,
     472,   473,   474,   211,   210,   478,   411,   381,   382,   217,
     384,   385,   386,   387,   388,   262,   211,   149,   211,   151,
     215,   210,   240,   241,   217,   211,   211,   401,   402,   215,
     211,   217,   240,   241,   495,   212,    41,    42,    43,     4,
     414,   236,   237,     8,     9,   238,   239,   210,   421,   423,
     214,   210,   238,   239,   213,   236,   237,   210,   431,     4,
      86,    87,   435,     8,     9,     3,   439,   210,   442,   210,
     543,   544,   446,   210,   187,   222,   223,   450,   209,    80,
      81,   455,   195,   196,  1018,  1019,   263,   264,   265,   266,
     267,   564,   210,    58,    85,    86,   757,   100,   210,   263,
     264,   265,   266,   267,     4,   218,   210,    72,    73,    74,
      75,   210,    77,   210,    79,     4,    81,    82,   210,   214,
     210,   936,   210,    23,   939,    25,   210,   240,   210,   210,
      30,    31,    32,   210,   210,   509,   210,   210,   511,   104,
     105,   106,   191,   108,   210,   210,   111,   112,   113,   114,
     115,   210,   117,   210,   210,   210,   269,   270,    10,    11,
      12,    13,    81,    82,   210,   214,    85,    86,   263,   264,
     265,   266,   267,     3,   210,   988,   141,     7,   143,   144,
     214,    81,    82,    93,   217,   210,   210,   152,   153,   154,
     155,   156,   157,   654,     4,   210,   210,   968,   210,   210,
     861,  1016,   210,   210,  1017,   211,   210,   209,  1105,   174,
     212,   210,   210,   210,   263,   264,   265,   266,   267,   213,
     210,   210,   210,   188,   210,   210,   210,   746,   128,   263,
     264,   265,   266,   267,   210,   200,   263,   264,   265,   266,
     267,  1056,   210,   137,     3,     4,   211,   268,   211,   213,
     269,    81,    82,   215,   213,   209,   209,   631,   209,   208,
       4,   263,   264,   265,   266,   267,   213,   232,   233,   211,
     209,    81,    82,   209,   211,   271,   795,   750,   652,   211,
     799,   200,   209,   802,   803,   804,   211,   748,   807,   808,
     809,   209,   211,     3,   212,   211,  1111,   262,   198,  1114,
     200,   674,   676,   677,   765,   678,   680,   214,   681,   682,
     211,   211,   209,   232,   233,   215,     3,   217,   213,   215,
     213,   209,    81,    82,   209,   209,   226,   227,   228,   229,
     209,   209,   232,   233,   234,   235,   263,   264,   265,   266,
     267,   209,   209,     4,   805,   263,   264,   265,   266,   267,
     213,   812,   209,   209,     4,   209,   263,   264,   265,   266,
     267,   209,   262,   193,   194,   195,   196,   197,   211,   199,
     200,   201,   202,   203,   204,   205,   214,   209,   209,     4,
     209,   211,   209,   193,   194,   195,   196,   197,   209,   199,
     200,   201,   202,   203,   204,   205,   263,   264,   265,   266,
     267,   211,   232,   233,   209,   209,   236,   237,   214,   783,
       6,   209,   242,   263,   264,   265,   266,   267,   209,   793,
     209,   209,   232,   233,   943,   263,   264,   265,   266,   267,
       4,   209,   242,   209,   193,   194,   195,   196,   197,   900,
     199,   200,   201,   202,   203,   204,   205,   209,   214,   209,
       3,  1112,   211,   214,   209,   209,  1117,   263,   264,   265,
     266,   267,   212,   209,   983,   984,   985,   986,   209,  1130,
     209,   209,   209,   232,   233,   209,    80,    81,    82,   209,
     227,   209,   229,   242,   231,   209,   233,   209,   235,   209,
     237,   212,   239,   210,   241,   210,   243,   210,   210,   873,
       3,     6,   263,   264,   265,   266,   267,   209,   214,    14,
     209,     5,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,   214,   218,   209,    33,    34,
      35,    36,    37,   209,    39,   206,    96,   212,    80,    44,
      45,    46,    47,    48,    49,    50,    51,  1020,    53,    54,
      55,   209,    57,   209,    59,    60,    61,    62,    63,    64,
     209,    80,   209,    68,    69,    38,    71,     4,   209,    38,
      40,   174,    90,    78,   263,   264,   265,   266,   267,   209,
      90,   209,   209,    94,   178,   100,   127,   138,   214,   193,
     194,   195,   196,   197,    99,   199,   200,   201,   202,   203,
     204,   205,   133,   209,   134,   216,   980,   211,     3,   209,
     212,   116,     4,   270,   209,   212,   209,   122,   209,   209,
     125,   126,   209,   128,   213,   130,   209,   209,   232,   233,
     209,   136,   217,   217,   139,   140,   209,   209,   242,     4,
     145,   146,   209,   148,   209,   150,     4,  1120,    81,    82,
     209,   213,   209,     4,   210,   209,     4,   209,    80,   209,
     209,  1134,    91,   209,   209,   209,   209,    81,    82,    98,
     210,   209,     4,   209,   209,  1080,   209,   127,     3,   184,
     185,   186,   187,   220,   189,   114,   209,   209,   269,   226,
     210,   228,   209,   230,     4,   232,   209,   234,  1103,   236,
     209,   238,   131,   240,   212,   242,     3,   211,   213,   209,
     212,   218,   209,    80,   219,   220,   209,   212,   209,   211,
     210,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   210,
     173,     5,   210,   210,   210,   174,   175,   176,   177,   210,
     179,   180,   181,   182,   183,    83,    84,   210,   210,   210,
     193,   194,   195,   196,   197,   210,   199,   200,   201,   202,
     203,   204,   205,   212,   210,   210,   210,   209,   211,   193,
     194,   195,   196,   197,   210,   199,   200,   201,   202,   203,
     204,   205,   120,   121,   210,   210,   124,   211,   209,   232,
     233,     4,   209,   174,     4,   174,    90,   209,   217,   242,
      90,   209,    66,   209,    97,   209,    97,   103,   232,   233,
      97,   209,   215,   212,   102,   212,   212,   155,   242,   212,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   220,   109,   142,   110,   209,
       4,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   209,
     209,     4,   210,   214,   209,     4,   210,     4,   209,   209,
     209,     4,   209,   209,     5,   209,    41,   210,     3,   217,
     212,   212,   210,   210,   210,   210,   210,   210,   210,   101,
     228,   229,   209,     4,   209,   209,   212,    67,    92,   212,
     210,   210,   210,     7,   210,   216,   210,   210,   210,   210,
     210,   210,   209,    81,   212,   210,     4,   209,   132,     4,
      98,     4,    97,   212,   209,     4,   210,   209,   209,     4,
     210,   209,   209,     4,   210,     5,    41,   212,   100,   210,
     270,   174,     4,   212,   174,   212,    92,    91,   209,   212,
     212,   207,   212,   210,   212,   210,   209,   209,   209,    91,
     209,     5,   212,  1133,    70,   244,   220,   658,    63,   410,
     213,   477,   540,   966,   971,   942,  1026,   831,   178,  1007,
    1021,  1085,  1090,   812,   730,  1049,   415,   767,   564,   533,
      -1,    -1,   278,   509
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   273,   274,   275,   276,   210,   210,     0,
     274,   118,   286,   217,   217,   210,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    33,    34,    35,    36,    37,    39,    44,    45,    46,
      47,    48,    49,    50,    51,    53,    54,    55,    57,    59,
      60,    61,    62,    63,    64,    68,    69,    71,    78,    99,
     116,   122,   125,   126,   128,   130,   136,   139,   140,   145,
     146,   148,   150,   184,   185,   186,   187,   189,   213,   219,
     220,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   277,
     278,   279,   280,   281,   288,   290,   291,   292,   293,   295,
     296,   304,   307,   308,   310,   311,   318,   320,   321,   323,
     325,   327,   331,   332,   333,   336,   338,   345,   352,   355,
     361,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   377,   378,   379,   380,   384,   385,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   209,   119,
     287,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   310,   210,   210,   210,
     210,   213,   220,   292,   294,   312,   210,   210,   210,   210,
     219,     4,   211,   226,   227,   262,   440,   450,   211,   288,
     268,   210,   322,     3,   209,   217,   228,   229,   426,   439,
      10,    11,    12,    13,     4,    96,   451,    81,    82,    85,
      86,   200,   211,   232,   233,   353,   428,   429,   100,   193,
     194,   195,   196,   197,   199,   201,   202,   203,   204,   205,
     211,   242,   428,   448,   449,     3,    81,    95,   211,   217,
     240,   241,   363,   437,   438,   363,     3,    81,   211,   217,
     238,   239,   357,   435,   436,    81,   217,   410,    72,    81,
     420,   428,     4,   428,   173,   449,   211,   234,   235,   431,
     432,   431,    56,   211,   458,   459,   217,   428,   428,    93,
     428,   211,   428,   458,   431,   431,    81,   217,   335,   411,
     458,   458,   458,   427,   440,   211,   334,   440,   428,   449,
     449,   428,   449,   449,     4,     8,     9,    81,   452,   428,
     449,   335,   449,     5,    81,   346,   328,   449,   431,   137,
     291,   427,   268,   213,   428,   428,   428,   428,   211,     8,
     440,   213,   191,   214,   263,   264,   265,   266,   267,   439,
     440,   445,   446,   447,    23,    25,    30,    31,    32,   128,
     198,   211,   215,   428,   431,   439,   440,   454,   455,   456,
     326,   215,   289,   287,   213,   269,   209,   209,   209,   208,
     430,   211,     4,   213,   209,   209,   340,   431,   431,   428,
     428,   211,   428,   211,   211,   211,   448,   211,   449,   271,
     209,     3,   213,   364,   362,     3,   213,   356,   209,   209,
     209,   209,   209,   209,     4,   213,   209,   209,   211,     4,
     209,   209,   209,   209,   209,   209,     4,   209,   209,   209,
       6,   309,   209,   209,   209,   209,     4,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   289,   329,   209,
     209,   209,   214,   440,   427,   209,   209,   209,   209,   305,
     212,   212,   440,   440,   440,   440,   440,   440,   440,   209,
     212,   210,     4,   300,   301,   302,   303,   429,   432,   439,
     440,   210,   210,   210,   290,   288,   440,   426,     3,     3,
     211,   386,     3,   209,   214,   449,   209,   440,    86,    87,
     354,     5,   218,   209,   209,    80,   348,   449,   428,   428,
       3,     7,   211,   236,   237,   433,   434,   449,   206,   212,
     449,    96,   209,   440,   209,   289,   209,   440,   289,   428,
     428,   449,    80,   298,   436,    80,   209,   440,    73,    81,
     421,    38,     4,   209,    38,    40,    81,    83,    84,   422,
     425,   174,   428,    90,   209,    90,   458,    81,   211,   341,
     209,   458,   458,   458,   427,   209,   334,   452,   428,    76,
      81,   457,    94,    65,   181,   178,   100,    81,   211,   342,
       4,   100,   420,   330,   211,   433,   433,   138,   324,   147,
     269,   319,   214,    81,   439,   133,   134,   135,   137,   293,
     442,   443,   444,   214,   214,   445,   449,   209,   209,   216,
     209,   216,   209,   216,   209,   216,   215,   437,   215,   435,
     215,   433,   216,   214,     3,   209,   430,   212,     4,   214,
     340,   428,   428,   270,   209,   212,   209,   209,   209,     3,
     213,   209,   209,   209,   217,   214,     4,     8,     9,    58,
      72,    73,    74,    75,    77,    79,   104,   105,   106,   108,
     111,   112,   113,   114,   115,   117,   141,   143,   144,   152,
     153,   154,   155,   156,   157,   174,   188,   365,   428,   450,
     453,   217,   214,   358,   350,   209,   209,   209,     4,   214,
     209,   209,     4,   209,   213,   209,   210,   209,     4,   209,
       4,   209,   421,   209,   209,   376,   209,    80,   209,   209,
     210,   381,   209,     4,   209,   209,   209,   127,   209,   209,
     289,   269,   440,   313,   209,   209,   210,   209,   212,     4,
     432,   429,   440,   439,   297,   438,   298,   299,   434,   209,
       3,   212,   218,   209,   348,   449,   440,   440,   449,   209,
     440,   449,   449,   211,   212,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   212,   209,   209,   420,   436,     5,   209,   411,     4,
     209,   174,     4,    81,    88,   424,   435,   174,    90,   209,
      81,   211,   343,   217,    90,   427,   209,   439,    66,   428,
     209,    97,   209,    81,   211,   344,    97,    81,   347,   348,
     289,    97,   433,   215,   421,    81,   439,   222,   223,   441,
     442,   306,   209,   216,   216,   209,   216,   421,   209,   440,
     212,   209,   212,   209,   212,   421,   214,   212,   212,   440,
     427,   437,   103,   102,   427,   109,   110,   427,   427,   427,
     439,   142,   427,   427,   427,   435,    83,    84,   120,   121,
     124,   155,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   359,   423,   425,
     439,    77,   107,   108,   114,   123,   131,   157,   351,   209,
     284,     4,   209,   412,   209,     4,   210,   214,   209,   210,
       4,   353,   209,   209,     4,    81,   349,   209,   209,    91,
      98,   114,   131,   174,   175,   176,   177,   179,   180,   181,
     182,   183,   382,     4,     4,   412,   209,   209,   149,   151,
     314,   315,   289,   438,   434,   212,     5,    41,   440,   212,
     264,   210,   439,   210,   210,   210,   210,   210,   210,   210,
     210,     3,   209,    41,    42,    43,   413,   414,   101,   415,
       4,   209,   435,    89,   174,   435,   212,   209,   344,   427,
     212,    67,    92,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   212,   209,   415,   349,     7,   452,   210,
     216,   314,   209,   212,   209,   212,   440,    81,   427,   427,
     427,   427,   173,   217,   360,   437,     4,   132,     4,   209,
     210,    98,   417,   209,     4,   209,   210,   354,   412,   209,
     209,    97,   418,     4,     4,    92,    94,   132,   383,   435,
     437,   386,   386,   428,   410,     4,     4,   417,   350,   210,
     317,     5,    41,   209,   282,   270,   212,   413,   100,   416,
     210,   174,   419,     4,   212,   174,   435,   212,   415,    92,
      91,   212,   419,   316,   288,   209,   212,   207,   212,   360,
     418,   210,   396,   212,   210,   417,   209,   337,   288,   421,
     209,   435,   289,   397,   435,   419,    91,   289,   209,   428,
     339,     5,    81,   285,   209,   283,   289,   284,   428,   212
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
#line 1695 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1704 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1708 "lev_comp.y"
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

  case 260:

/* Line 1455 of yacc.c  */
#line 1721 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1729 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1739 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1747 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1756 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1763 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1771 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1777 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1782 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1787 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1792 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1797 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1802 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1807 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1812 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1817 "lev_comp.y"
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

  case 276:

/* Line 1455 of yacc.c  */
#line 1831 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1837 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1842 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1847 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1852 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1857 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1862 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1867 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1872 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1877 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1882 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1887 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1892 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1897 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x400000;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1902 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x800000;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1907 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x1000000;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1912 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_MYTHIC_TYPE));
		      (yyval.i) = 0x2000000;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1918 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x4000000;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1923 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x8000000;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1928 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x10000000;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1935 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1941 "lev_comp.y"
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

  case 298:

/* Line 1455 of yacc.c  */
#line 1970 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1975 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1983 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1989 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 1993 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2001 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2008 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2015 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2025 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2035 "lev_comp.y"
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

  case 308:

/* Line 1455 of yacc.c  */
#line 2051 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2061 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2065 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2071 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2077 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2083 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2087 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2093 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2099 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2106 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2114 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2120 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2125 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2130 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2135 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2140 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2146 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2152 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2157 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2162 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2167 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2172 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2177 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2187 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2193 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2199 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2204 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2211 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2218 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2222 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2228 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2234 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2240 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2245 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2252 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2258 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2268 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2274 "lev_comp.y"
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

  case 350:

/* Line 1455 of yacc.c  */
#line 2293 "lev_comp.y"
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

  case 351:

/* Line 1455 of yacc.c  */
#line 2305 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2309 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2315 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2320 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2325 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (long)(yyvsp[(9) - (9)].i), (long)(yyvsp[(7) - (9)].i), (long)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2330 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((long)(yyvsp[(9) - (13)].i), (long)(yyvsp[(7) - (13)].i), (long)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2336 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2340 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2346 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2352 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2356 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2362 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2366 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2372 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2376 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2381 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2388 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2392 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2399 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2403 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2408 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2415 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2419 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2424 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2432 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2438 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2445 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2449 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2456 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2467 "lev_comp.y"
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

  case 383:

/* Line 1455 of yacc.c  */
#line 2480 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2484 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2490 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2494 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2501 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2505 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2509 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2515 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2519 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2525 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2531 "lev_comp.y"
    {
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

  case 395:

/* Line 1455 of yacc.c  */
#line 2541 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2547 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2552 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2569 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2577 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2587 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2596 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2601 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2608 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2619 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2625 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2629 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2633 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2640 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2650 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2657 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2661 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2667 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2671 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2679 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2683 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2690 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2700 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2713 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2717 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2724 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2734 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2743 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2754 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2758 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2765 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2775 "lev_comp.y"
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

  case 437:

/* Line 1455 of yacc.c  */
#line 2786 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2795 "lev_comp.y"
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

  case 439:

/* Line 1455 of yacc.c  */
#line 2805 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2811 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2815 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2822 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2832 "lev_comp.y"
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

  case 444:

/* Line 1455 of yacc.c  */
#line 2843 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2852 "lev_comp.y"
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

  case 446:

/* Line 1455 of yacc.c  */
#line 2862 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2866 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2872 "lev_comp.y"
    { ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2874 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2880 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2884 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2888 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2892 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2900 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2909 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2913 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2917 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2921 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2925 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2928 "lev_comp.y"
    { ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2932 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2939 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2948 "lev_comp.y"
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

  case 468:

/* Line 1455 of yacc.c  */
#line 2989 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 2993 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 3000 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 3007 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 3017 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3021 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3029 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3033 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3037 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3041 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3045 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3050 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3054 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3058 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3063 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3068 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3073 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3077 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3082 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3087 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3091 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3095 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3100 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3104 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3111 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3117 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3121 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3127 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3143 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3147 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3151 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3155 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3177 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3181 "lev_comp.y"
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

  case 515:

/* Line 1455 of yacc.c  */
#line 3207 "lev_comp.y"
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
#line 7269 "lev_comp.tab.c"
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
#line 3235 "lev_comp.y"


/*lev_comp.y*/

