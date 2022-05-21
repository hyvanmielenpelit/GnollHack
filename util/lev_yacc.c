
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

/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-04-16 */

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
     PARALYZED_ID = 422,
     STUNNED_ID = 423,
     CONFUSED_ID = 424,
     SEENTRAPS_ID = 425,
     ALL_ID = 426,
     MONTYPE_ID = 427,
     OBJTYPE_ID = 428,
     TERTYPE_ID = 429,
     TERTYPE2_ID = 430,
     LEVER_EFFECT_TYPE = 431,
     SWITCHABLE_ID = 432,
     CONTINUOUSLY_USABLE_ID = 433,
     TARGET_ID = 434,
     TRAPTYPE_ID = 435,
     EFFECT_FLAG_ID = 436,
     GRAVE_ID = 437,
     BRAZIER_ID = 438,
     SIGNPOST_ID = 439,
     TREE_ID = 440,
     ERODEPROOF_ID = 441,
     FUNCTION_ID = 442,
     MSG_OUTPUT_TYPE = 443,
     COMPARE_TYPE = 444,
     UNKNOWN_TYPE = 445,
     rect_ID = 446,
     fillrect_ID = 447,
     line_ID = 448,
     randline_ID = 449,
     grow_ID = 450,
     selection_ID = 451,
     flood_ID = 452,
     rndcoord_ID = 453,
     circle_ID = 454,
     ellipse_ID = 455,
     filter_ID = 456,
     complement_ID = 457,
     gradient_ID = 458,
     GRADIENT_TYPE = 459,
     LIMITED = 460,
     HUMIDITY_TYPE = 461,
     STRING = 462,
     MAP_ID = 463,
     NQSTRING = 464,
     VARSTRING = 465,
     CFUNC = 466,
     CFUNC_INT = 467,
     CFUNC_STR = 468,
     CFUNC_COORD = 469,
     CFUNC_REGION = 470,
     VARSTRING_INT = 471,
     VARSTRING_INT_ARRAY = 472,
     VARSTRING_STRING = 473,
     VARSTRING_STRING_ARRAY = 474,
     VARSTRING_VAR = 475,
     VARSTRING_VAR_ARRAY = 476,
     VARSTRING_COORD = 477,
     VARSTRING_COORD_ARRAY = 478,
     VARSTRING_REGION = 479,
     VARSTRING_REGION_ARRAY = 480,
     VARSTRING_MAPCHAR = 481,
     VARSTRING_MAPCHAR_ARRAY = 482,
     VARSTRING_MONST = 483,
     VARSTRING_MONST_ARRAY = 484,
     VARSTRING_OBJ = 485,
     VARSTRING_OBJ_ARRAY = 486,
     VARSTRING_SEL = 487,
     VARSTRING_SEL_ARRAY = 488,
     METHOD_INT = 489,
     METHOD_INT_ARRAY = 490,
     METHOD_STRING = 491,
     METHOD_STRING_ARRAY = 492,
     METHOD_VAR = 493,
     METHOD_VAR_ARRAY = 494,
     METHOD_COORD = 495,
     METHOD_COORD_ARRAY = 496,
     METHOD_REGION = 497,
     METHOD_REGION_ARRAY = 498,
     METHOD_MAPCHAR = 499,
     METHOD_MAPCHAR_ARRAY = 500,
     METHOD_MONST = 501,
     METHOD_MONST_ARRAY = 502,
     METHOD_OBJ = 503,
     METHOD_OBJ_ARRAY = 504,
     METHOD_SEL = 505,
     METHOD_SEL_ARRAY = 506,
     DICE = 507
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
#line 552 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 564 "lev_comp.tab.c"

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
#define YYLAST   1300

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  270
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  188
/* YYNRULES -- Number of rules.  */
#define YYNRULES  513
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1136

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   507

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   265,   269,     2,
     209,   210,   263,   261,   207,   262,   267,   264,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   208,     2,
       2,   266,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   211,     2,   212,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   213,   268,   214,     2,     2,     2,     2,
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
     205,   206,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260
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
     895,   897,   901,   905,   909,   915,   923,   929,   938,   940,
     944,   948,   954,   960,   968,   976,   983,   989,   990,   993,
     999,  1003,  1015,  1029,  1036,  1037,  1041,  1043,  1047,  1051,
    1055,  1059,  1061,  1063,  1067,  1071,  1075,  1079,  1083,  1087,
    1089,  1091,  1093,  1095,  1099,  1103,  1105,  1111,  1121,  1127,
    1135,  1139,  1143,  1149,  1159,  1165,  1171,  1177,  1178,  1192,
    1193,  1195,  1203,  1215,  1225,  1239,  1243,  1251,  1259,  1265,
    1273,  1279,  1289,  1295,  1301,  1305,  1313,  1317,  1325,  1333,
    1337,  1343,  1349,  1353,  1359,  1367,  1377,  1379,  1381,  1383,
    1385,  1387,  1388,  1391,  1393,  1397,  1399,  1401,  1403,  1404,
    1408,  1410,  1411,  1415,  1417,  1418,  1422,  1424,  1426,  1428,
    1430,  1432,  1434,  1436,  1438,  1440,  1444,  1446,  1448,  1453,
    1455,  1457,  1462,  1464,  1466,  1471,  1473,  1478,  1484,  1486,
    1490,  1492,  1496,  1498,  1500,  1505,  1515,  1517,  1519,  1524,
    1526,  1532,  1534,  1536,  1541,  1543,  1545,  1551,  1553,  1555,
    1557,  1562,  1564,  1566,  1572,  1574,  1576,  1578,  1582,  1584,
    1586,  1590,  1592,  1597,  1601,  1605,  1609,  1613,  1617,  1621,
    1623,  1625,  1629,  1631,  1635,  1636,  1638,  1640,  1642,  1644,
    1648,  1649,  1651,  1653,  1656,  1659,  1664,  1671,  1676,  1683,
    1690,  1697,  1704,  1707,  1714,  1723,  1732,  1743,  1758,  1761,
    1763,  1767,  1769,  1773,  1775,  1777,  1779,  1781,  1783,  1785,
    1787,  1789,  1791,  1793,  1795,  1797,  1799,  1801,  1803,  1805,
    1807,  1809,  1811,  1822
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     271,     0,    -1,    -1,   272,    -1,   273,    -1,   273,   272,
      -1,   274,   284,   286,    -1,    16,   208,   215,    -1,    15,
     208,   215,   207,     3,    -1,    17,   208,    11,   207,   384,
      -1,    17,   208,    10,   207,     3,    -1,    17,   208,    13,
      -1,    17,   208,    12,   207,     3,   207,     3,   207,     5,
     207,     5,   207,   419,   207,   283,   282,    -1,    18,   208,
     449,    -1,   136,   208,   137,   207,   138,    -1,   136,   208,
     137,    -1,    29,   208,     4,   207,   296,    -1,    21,   208,
     100,    -1,    -1,   207,   205,    -1,    -1,   207,   426,    -1,
      -1,   207,     3,    -1,     5,    -1,    81,    -1,    -1,   118,
     208,   285,    -1,   119,   207,   285,    -1,   119,    -1,    -1,
     288,   286,    -1,   213,   286,   214,    -1,   329,    -1,   275,
      -1,   276,    -1,   392,    -1,   279,    -1,   277,    -1,   399,
      -1,   278,    -1,   398,    -1,   396,    -1,   397,    -1,   400,
      -1,   401,    -1,   402,    -1,   403,    -1,   404,    -1,   373,
      -1,   331,    -1,   294,    -1,   293,    -1,   387,    -1,   343,
      -1,   365,    -1,   406,    -1,   407,    -1,   375,    -1,   376,
      -1,   377,    -1,   405,    -1,   309,    -1,   319,    -1,   321,
      -1,   325,    -1,   323,    -1,   306,    -1,   316,    -1,   302,
      -1,   305,    -1,   368,    -1,   378,    -1,   350,    -1,   366,
      -1,   353,    -1,   359,    -1,   388,    -1,   383,    -1,   371,
      -1,   330,    -1,   389,    -1,   390,    -1,   391,    -1,   393,
      -1,   336,    -1,   334,    -1,   382,    -1,   386,    -1,   385,
      -1,   369,    -1,   370,    -1,   372,    -1,   364,    -1,   367,
      -1,   225,    -1,   227,    -1,   229,    -1,   231,    -1,   233,
      -1,   235,    -1,   237,    -1,   239,    -1,   241,    -1,   224,
      -1,   226,    -1,   228,    -1,   230,    -1,   232,    -1,   234,
      -1,   236,    -1,   238,    -1,   240,    -1,   289,    -1,   290,
      -1,   218,    -1,   218,    -1,   290,    -1,   140,   208,   289,
      -1,   291,   266,   438,    -1,   291,   266,   196,   208,   447,
      -1,   291,   266,   437,    -1,   291,   266,   454,   208,   431,
      -1,   291,   266,   453,   208,   433,    -1,   291,   266,   452,
     208,   435,    -1,   291,   266,   426,    -1,   291,   266,   429,
      -1,   291,   266,   213,   300,   214,    -1,   291,   266,   213,
     299,   214,    -1,   291,   266,   213,   298,   214,    -1,   291,
     266,   454,   208,   213,   297,   214,    -1,   291,   266,   453,
     208,   213,   296,   214,    -1,   291,   266,   452,   208,   213,
     295,   214,    -1,   291,   266,   213,   301,   214,    -1,   436,
      -1,   295,   207,   436,    -1,   434,    -1,   296,   207,   434,
      -1,   432,    -1,   297,   207,   432,    -1,   430,    -1,   298,
     207,   430,    -1,   427,    -1,   299,   207,   427,    -1,   438,
      -1,   300,   207,   438,    -1,   437,    -1,   301,   207,   437,
      -1,    -1,    -1,   187,   217,   209,   303,   442,   210,   304,
     287,    -1,   217,   209,   445,   210,    -1,   139,    -1,    -1,
       6,    -1,     6,    -1,   211,   438,   189,   438,   212,    -1,
     211,   438,   212,    -1,    -1,    -1,   148,   310,   211,   425,
     212,   311,   213,   312,   214,    -1,    -1,   313,   312,    -1,
      -1,   149,   450,   208,   314,   286,    -1,    -1,   151,   208,
     315,   286,    -1,   150,    -1,   267,   267,    -1,   147,    -1,
     146,   292,   266,   438,   317,   438,    -1,    -1,   318,   320,
     287,    -1,    -1,   145,   211,   425,   212,   322,   287,    -1,
      -1,   308,   208,   324,   288,    -1,    -1,   126,   308,   326,
     327,    -1,   287,    -1,    -1,   287,   328,   127,   287,    -1,
      14,   208,   437,    -1,    71,    -1,    71,   208,   450,    -1,
      71,   208,    81,    -1,    59,   208,   332,   207,   332,    -1,
      59,   208,   332,   207,   450,    -1,   209,     4,   207,    80,
     207,   347,   210,    -1,   409,   307,   207,   419,    -1,    -1,
     116,   208,   333,   207,   340,   207,   342,   410,   413,   415,
     417,   335,   287,    -1,    -1,    51,   208,   333,   207,   339,
     207,   341,   207,   342,   410,   413,   415,   417,   337,   287,
      -1,    -1,   207,     5,    -1,   209,     4,   207,     4,   210,
      -1,    81,    -1,   209,     4,   207,     4,   210,    -1,    81,
      -1,   209,   351,   207,   352,   210,    -1,    81,    -1,   209,
       4,   207,     4,   210,    -1,    81,    -1,   125,   208,   344,
     207,   418,   207,   345,   207,   347,   348,    -1,    27,   208,
     418,   207,   447,   348,    -1,     5,    -1,    81,    -1,   346,
      -1,    81,    -1,    80,    -1,    80,   268,   346,    -1,     4,
      -1,    81,    -1,    -1,   348,   207,   349,    -1,   131,   208,
     132,    -1,    77,   208,   435,    -1,   114,   208,     4,    -1,
     157,    -1,   107,    -1,   108,    -1,   123,    -1,    20,    -1,
      19,   208,   351,   207,   352,   338,   216,    -1,    19,   208,
     426,   338,   216,    -1,    85,    -1,    86,    -1,    87,    -1,
      86,    -1,    25,   208,   355,    -1,    -1,    25,   208,   355,
     354,   287,    -1,   433,   207,   426,   356,    -1,    -1,   356,
     207,   357,    -1,   437,    -1,   120,    -1,   121,    -1,   421,
      -1,   124,   437,    -1,   158,    -1,   159,    -1,   155,    -1,
     162,    -1,   163,    -1,   164,    -1,   165,   208,   425,    -1,
     166,   208,   425,    -1,   167,   208,   425,    -1,   168,    -1,
     169,    -1,   170,   208,   358,    -1,   160,    -1,   161,    -1,
     215,    -1,   171,    -1,   215,   268,   358,    -1,    23,   208,
     361,    -1,    -1,    24,   208,   361,   360,   287,    -1,   435,
     362,    -1,    -1,   362,   207,   363,    -1,    74,    -1,   172,
     208,   433,    -1,   451,    -1,   117,   208,   437,    -1,   143,
     208,   425,    -1,   144,    -1,    73,    -1,   152,   208,   425,
      -1,   186,    -1,    72,    -1,   153,    -1,   154,   208,   425,
      -1,   155,    -1,   156,    -1,   426,    -1,   105,   208,   102,
      -1,   104,   208,   103,    -1,   106,   208,   425,    -1,   113,
     208,   425,    -1,   114,   208,   425,    -1,   141,   208,   142,
      -1,   115,   208,   425,    -1,    77,   208,   435,    -1,   157,
      -1,   108,    -1,    79,    -1,    75,    -1,   111,   208,   109,
      -1,   112,   208,   110,    -1,    58,   208,   425,    -1,    26,
     208,   408,   207,   426,    -1,    28,   208,   426,   207,    80,
     207,   418,    -1,    33,   208,   426,   207,    80,    -1,    33,
     208,   426,   207,    80,   207,     5,   282,    -1,    34,    -1,
      34,   208,   447,    -1,    34,   208,   171,    -1,    47,   208,
     426,   207,    90,    -1,    48,   208,   426,   207,    90,    -1,
      48,   208,   456,   207,   456,   207,    90,    -1,    53,   208,
     456,   207,   456,   207,   215,    -1,    54,   208,   456,   207,
     456,   374,    -1,    55,   208,   456,   207,   456,    -1,    -1,
     207,    90,    -1,    62,   208,   447,   207,    94,    -1,    63,
     208,   447,    -1,    64,   208,   426,   207,   179,   208,   426,
     207,    92,   207,    91,    -1,    64,   208,   426,   207,    65,
     207,    66,   207,    67,   207,    92,   207,    91,    -1,    78,
     208,   426,   207,   176,   379,    -1,    -1,   379,   207,   380,
      -1,    91,    -1,   172,   208,   433,    -1,   173,   208,   435,
      -1,   174,   208,   384,    -1,   175,   208,   384,    -1,   177,
      -1,   178,    -1,   179,   208,   426,    -1,   180,   208,   408,
      -1,   131,   208,   381,    -1,    98,   208,   416,    -1,   181,
     208,     4,    -1,   114,   208,     4,    -1,     4,    -1,    92,
      -1,   132,    -1,    94,    -1,    69,   208,   447,    -1,    68,
     208,   447,    -1,     3,    -1,   209,     3,   207,   419,   210,
      -1,   130,   208,   429,   207,   431,   207,   431,   207,     7,
      -1,   128,   208,   447,   207,   431,    -1,   128,   208,   447,
     207,   431,   207,    97,    -1,    49,   208,   429,    -1,    50,
     208,   429,    -1,    39,   208,   215,   207,    40,    -1,    39,
     208,   215,   207,    40,   207,   172,   208,   433,    -1,    36,
     208,   429,   207,    38,    -1,    37,   208,   456,   207,    38,
      -1,    22,   208,   447,   207,    96,    -1,    -1,    35,   208,
     429,   207,   419,   207,   409,   410,   413,   415,   417,   394,
     395,    -1,    -1,   287,    -1,    44,   208,   426,   207,   420,
     207,   422,    -1,    44,   208,   426,   207,   420,   207,   422,
     207,   172,   208,   433,    -1,    44,   208,   426,   207,   420,
     207,   422,   207,    89,    -1,    44,   208,   426,   207,   420,
     207,   422,   207,    89,   207,   172,   208,   433,    -1,    45,
     208,   426,    -1,    45,   208,   426,   207,   172,   208,   433,
      -1,    99,   208,   447,   207,   100,   207,    97,    -1,   122,
     208,   447,   207,     4,    -1,   122,   208,   447,   207,   100,
     207,    97,    -1,    46,   208,    93,   207,   426,    -1,    46,
     208,    93,   207,   426,   207,   172,   208,   433,    -1,   182,
     208,   426,   207,   437,    -1,   182,   208,   426,   207,    81,
      -1,   182,   208,   426,    -1,   183,   208,   426,   207,   133,
     207,   419,    -1,   183,   208,   426,    -1,   184,   208,   426,
     207,   134,   207,   437,    -1,   184,   208,   426,   207,   134,
     207,    81,    -1,   184,   208,   426,    -1,   185,   208,   426,
     207,   135,    -1,   185,   208,   426,   207,   137,    -1,   185,
     208,   426,    -1,    60,   208,   438,   207,   426,    -1,    61,
     208,   426,   207,   455,   207,   437,    -1,    57,   208,   425,
     207,   425,   207,   425,   207,   425,    -1,    57,    -1,   215,
      -1,    81,    -1,   215,    -1,    81,    -1,    -1,   207,   411,
      -1,   412,    -1,   412,   207,   411,    -1,    41,    -1,    42,
      -1,    43,    -1,    -1,   101,   208,   414,    -1,   100,    -1,
      -1,    98,   208,   416,    -1,    97,    -1,    -1,   172,   208,
     433,    -1,    72,    -1,    81,    -1,    73,    -1,    81,    -1,
      84,    -1,   423,    -1,    81,    -1,    84,    -1,   423,    -1,
      83,   208,    81,    -1,    88,    -1,    81,    -1,    83,   211,
       4,   212,    -1,   215,    -1,   226,    -1,   227,   211,   438,
     212,    -1,   438,    -1,   427,    -1,   198,   209,   447,   210,
      -1,   230,    -1,   231,   211,   438,   212,    -1,   209,     4,
     207,     4,   210,    -1,    81,    -1,    82,   428,   212,    -1,
     206,    -1,   206,   207,   428,    -1,   430,    -1,   232,    -1,
     233,   211,   438,   212,    -1,   209,     4,   207,     4,   207,
       4,   207,     4,   210,    -1,   432,    -1,   234,    -1,   235,
     211,   438,   212,    -1,     3,    -1,   209,     3,   207,   419,
     210,    -1,   434,    -1,   236,    -1,   237,   211,   438,   212,
      -1,   215,    -1,     3,    -1,   209,     3,   207,   215,   210,
      -1,    81,    -1,   436,    -1,   238,    -1,   239,   211,   438,
     212,    -1,   215,    -1,     3,    -1,   209,     3,   207,   215,
     210,    -1,    81,    -1,    95,    -1,   424,    -1,   437,   267,
     424,    -1,     4,    -1,   448,    -1,   209,     8,   210,    -1,
     224,    -1,   225,   211,   438,   212,    -1,   438,   261,   438,
      -1,   438,   262,   438,    -1,   438,   263,   438,    -1,   438,
     264,   438,    -1,   438,   265,   438,    -1,   209,   438,   210,
      -1,   220,    -1,   221,    -1,   291,   208,   439,    -1,   440,
      -1,   441,   207,   440,    -1,    -1,   441,    -1,   438,    -1,
     437,    -1,   443,    -1,   444,   207,   443,    -1,    -1,   444,
      -1,   426,    -1,   191,   429,    -1,   192,   429,    -1,   193,
     426,   207,   426,    -1,   194,   426,   207,   426,   207,   438,
      -1,   195,   209,   447,   210,    -1,   195,   209,   346,   207,
     447,   210,    -1,   201,   209,     7,   207,   447,   210,    -1,
     201,   209,   447,   207,   447,   210,    -1,   201,   209,   431,
     207,   447,   210,    -1,   197,   426,    -1,   199,   209,   426,
     207,   438,   210,    -1,   199,   209,   426,   207,   438,   207,
      41,   210,    -1,   200,   209,   426,   207,   438,   207,   438,
     210,    -1,   200,   209,   426,   207,   438,   207,   438,   207,
      41,   210,    -1,   203,   209,   204,   207,   209,   438,   262,
     438,   280,   210,   207,   426,   281,   210,    -1,   202,   446,
      -1,   240,    -1,   209,   447,   210,    -1,   446,    -1,   446,
     269,   447,    -1,   260,    -1,    96,    -1,     4,    -1,     8,
      -1,     9,    -1,     4,    -1,     8,    -1,     9,    -1,     4,
      -1,   448,    -1,    30,    -1,    23,    -1,    31,    -1,    25,
      -1,    32,    -1,   128,    -1,    76,    -1,    81,    -1,   457,
      -1,    56,   209,     4,   207,     4,   207,     4,   207,     4,
     210,    -1,   209,     4,   207,     4,   207,     4,   207,     4,
     210,    -1
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
    1901,  1907,  1912,  1917,  1924,  1930,  1959,  1964,  1972,  1978,
    1982,  1990,  1997,  2004,  2014,  2024,  2040,  2051,  2054,  2060,
    2066,  2072,  2076,  2082,  2089,  2095,  2103,  2109,  2114,  2119,
    2124,  2129,  2135,  2141,  2146,  2151,  2156,  2161,  2166,  2173,
    2173,  2173,  2173,  2176,  2182,  2188,  2193,  2200,  2207,  2211,
    2217,  2223,  2229,  2234,  2241,  2247,  2257,  2264,  2263,  2295,
    2298,  2304,  2309,  2314,  2319,  2325,  2329,  2335,  2341,  2345,
    2351,  2355,  2361,  2365,  2370,  2377,  2381,  2388,  2392,  2397,
    2404,  2408,  2413,  2421,  2427,  2434,  2438,  2445,  2453,  2456,
    2466,  2470,  2473,  2479,  2483,  2490,  2494,  2498,  2505,  2508,
    2514,  2521,  2524,  2530,  2537,  2541,  2548,  2549,  2552,  2553,
    2556,  2557,  2558,  2564,  2565,  2566,  2572,  2573,  2576,  2585,
    2590,  2597,  2608,  2614,  2618,  2622,  2629,  2639,  2646,  2650,
    2656,  2660,  2668,  2672,  2679,  2689,  2702,  2706,  2713,  2723,
    2732,  2743,  2747,  2754,  2764,  2775,  2784,  2794,  2800,  2804,
    2811,  2821,  2832,  2841,  2851,  2855,  2862,  2863,  2869,  2873,
    2877,  2881,  2889,  2898,  2902,  2906,  2910,  2914,  2918,  2921,
    2928,  2937,  2970,  2971,  2974,  2975,  2978,  2982,  2989,  2996,
    3007,  3010,  3018,  3022,  3026,  3030,  3034,  3039,  3043,  3047,
    3052,  3057,  3062,  3066,  3071,  3076,  3080,  3084,  3089,  3093,
    3100,  3106,  3110,  3116,  3123,  3124,  3127,  3128,  3129,  3132,
    3136,  3140,  3144,  3150,  3151,  3154,  3155,  3158,  3159,  3162,
    3163,  3166,  3170,  3196
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
     455,   456,   457,   458,   459,   460,   461,    44,    58,    40,
      41,    91,    93,   123,   125,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,    43,    45,    42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   270,   271,   271,   272,   272,   273,   274,   274,   275,
     275,   275,   275,   276,   277,   277,   278,   279,   280,   280,
     281,   281,   282,   282,   283,   283,   284,   284,   285,   285,
     286,   286,   287,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   291,   291,   291,   292,   292,   293,   294,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   295,   295,   296,   296,   297,   297,
     298,   298,   299,   299,   300,   300,   301,   301,   303,   304,
     302,   305,   306,   307,   307,   308,   308,   308,   310,   311,
     309,   312,   312,   314,   313,   315,   313,   316,   317,   317,
     318,   320,   319,   322,   321,   324,   323,   326,   325,   327,
     328,   327,   329,   330,   330,   330,   331,   331,   332,   333,
     335,   334,   337,   336,   338,   338,   339,   339,   340,   340,
     341,   341,   342,   342,   343,   343,   344,   344,   345,   345,
     346,   346,   347,   347,   348,   348,   349,   349,   349,   349,
     349,   349,   349,   350,   350,   350,   351,   351,   352,   352,
     353,   354,   353,   355,   356,   356,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   358,   358,   358,   359,   360,
     359,   361,   362,   362,   363,   363,   363,   363,   363,   363,
     363,   363,   363,   363,   363,   363,   363,   363,   363,   363,
     363,   363,   363,   363,   363,   363,   363,   363,   363,   363,
     363,   363,   363,   363,   364,   365,   366,   366,   367,   367,
     367,   368,   369,   370,   371,   372,   373,   374,   374,   375,
     376,   377,   377,   378,   379,   379,   380,   380,   380,   380,
     380,   380,   380,   380,   380,   380,   380,   380,   380,   381,
     381,   381,   381,   382,   383,   384,   384,   385,   386,   386,
     387,   388,   389,   389,   390,   391,   392,   394,   393,   395,
     395,   396,   396,   396,   396,   397,   397,   398,   399,   399,
     400,   400,   401,   401,   401,   402,   402,   403,   403,   403,
     404,   404,   404,   405,   406,   407,   407,   408,   408,   409,
     409,   410,   410,   411,   411,   412,   412,   412,   413,   413,
     414,   415,   415,   416,   417,   417,   418,   418,   419,   419,
     420,   420,   420,   421,   421,   421,   422,   422,   423,   424,
     424,   424,   425,   426,   426,   426,   426,   427,   427,   427,
     428,   428,   429,   429,   429,   430,   431,   431,   431,   432,
     432,   433,   433,   433,   434,   434,   434,   434,   435,   435,
     435,   436,   436,   436,   436,   436,   437,   437,   438,   438,
     438,   438,   438,   438,   438,   438,   438,   438,   438,   439,
     439,   440,   441,   441,   442,   442,   443,   443,   444,   444,
     445,   445,   446,   446,   446,   446,   446,   446,   446,   446,
     446,   446,   446,   446,   446,   446,   446,   446,   446,   446,
     446,   447,   447,   448,   449,   449,   450,   450,   450,   451,
     451,   451,   451,   452,   452,   453,   453,   454,   454,   455,
     455,   456,   456,   457
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
       1,     3,     3,     3,     5,     7,     5,     8,     1,     3,
       3,     5,     5,     7,     7,     6,     5,     0,     2,     5,
       3,    11,    13,     6,     0,     3,     1,     3,     3,     3,
       3,     1,     1,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     3,     3,     1,     5,     9,     5,     7,
       3,     3,     5,     9,     5,     5,     5,     0,    13,     0,
       1,     7,    11,     9,    13,     3,     7,     7,     5,     7,
       5,     9,     5,     5,     3,     7,     3,     7,     7,     3,
       5,     5,     3,     5,     7,     9,     1,     1,     1,     1,
       1,     0,     2,     1,     3,     1,     1,     1,     0,     3,
       1,     0,     3,     1,     0,     3,     1,     1,     1,     1,
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
       5,     0,    30,     0,     7,     0,   155,     0,     0,     0,
       0,   223,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   298,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   376,     0,
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
       0,   448,     0,   451,     0,   493,     0,   449,   470,    31,
       0,   175,     0,     8,     0,   409,   410,     0,   446,   182,
       0,     0,     0,    11,   495,   494,    13,   418,     0,   226,
     227,     0,     0,   415,     0,     0,   194,   413,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   489,   472,   491,     0,   442,   444,   445,     0,   441,
     439,     0,   258,   262,   438,   259,   435,   437,     0,   434,
     432,     0,   230,     0,   431,   378,   377,     0,   396,   397,
       0,     0,     0,     0,   300,   299,     0,   423,     0,     0,
     422,     0,     0,     0,     0,   511,     0,     0,   355,     0,
       0,     0,     0,     0,   340,   341,   380,   379,     0,   153,
       0,     0,     0,     0,   412,     0,     0,     0,     0,     0,
     310,     0,   334,   333,   498,   496,   497,   185,   184,     0,
       0,     0,     0,   206,   207,     0,     0,     0,     0,    15,
     118,     0,     0,     0,   364,   366,   369,   372,   148,     0,
       0,     0,     0,   157,     0,     0,     0,     0,     0,   467,
     466,   468,   471,     0,   504,   506,   503,   505,   507,   508,
       0,     0,     0,   125,   126,   121,   119,     0,     0,     0,
       0,    30,   172,    28,     0,     0,     0,     0,     0,   420,
       0,     0,     0,     0,     0,     0,     0,   473,   474,     0,
       0,     0,   482,     0,     0,     0,   488,     0,     0,     0,
       0,     0,     0,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     154,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   179,   178,     0,
       0,     0,   173,     0,     0,     0,     0,     0,     0,   464,
     450,   458,     0,     0,   453,   454,   455,   456,   457,     0,
     151,     0,   448,     0,     0,     0,     0,   142,   140,   146,
     144,     0,     0,     0,   176,     0,     0,   447,    10,   335,
       0,     9,     0,     0,   419,     0,     0,     0,   229,   228,
     194,   195,   225,     0,     0,   210,     0,     0,     0,     0,
     429,     0,     0,   427,     0,     0,   426,     0,     0,   490,
     492,   346,     0,     0,     0,   260,     0,     0,   232,   234,
     294,   214,     0,    16,   136,   296,     0,     0,   398,   399,
       0,   344,     0,     0,   345,   342,   402,     0,   400,     0,
     401,     0,   360,   301,     0,   302,     0,   197,     0,     0,
       0,     0,   307,   306,     0,     0,   186,   187,   373,   509,
     510,     0,   309,     0,     0,   314,     0,   199,     0,     0,
     358,     0,     0,     0,     0,   338,     0,    14,     0,   169,
       0,     0,   159,   363,   362,     0,     0,   370,   371,     0,
     462,   465,     0,   452,   156,   469,   120,     0,     0,   129,
       0,   128,     0,   127,     0,   133,     0,   124,     0,   123,
       0,   122,    32,   411,     0,     0,   421,   414,     0,   416,
       0,   475,     0,     0,     0,   477,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   440,   501,   499,   500,     0,
     273,   270,   264,   290,     0,   289,     0,     0,     0,   288,
       0,     0,     0,     0,     0,     0,     0,     0,   269,     0,
     274,     0,   276,   277,   287,     0,   272,   263,   278,   502,
     266,     0,   433,   233,   205,     0,     0,     0,     0,   424,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   189,     0,     0,   305,     0,     0,     0,     0,
       0,   313,     0,     0,     0,     0,     0,     0,     0,     0,
     174,   168,   170,     0,     0,     0,     0,     0,   149,     0,
     141,   143,   145,   147,     0,   134,     0,     0,   138,     0,
       0,   417,   224,     0,   211,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   443,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,   295,   137,    22,     0,   381,     0,
       0,     0,     0,   407,   406,   351,   356,     0,   303,     0,
     201,     0,     0,   304,   308,     0,     0,   374,     0,     0,
       0,   357,     0,   203,     0,   381,   359,   209,     0,   208,
     181,   339,     0,   161,   365,   368,   367,   459,   460,   461,
     463,     0,     0,   132,   131,     0,   130,     0,     0,   476,
     478,     0,   483,     0,   479,     0,   428,   481,   480,     0,
     293,   286,   280,   279,   281,   291,   292,   282,   283,   285,
     267,   284,   268,   271,   275,   265,     0,   403,   237,   238,
       0,   243,   241,   242,   253,   254,   244,   245,   246,     0,
       0,     0,   250,   251,     0,   235,   239,   404,   236,     0,
     220,   221,     0,   222,     0,   219,   215,     0,   297,     0,
       0,   388,     0,     0,     0,   408,     0,     0,     0,     0,
       0,     0,   212,   213,     0,     0,     0,   316,     0,     0,
       0,     0,     0,     0,     0,   321,   322,     0,     0,     0,
     315,     0,     0,   388,     0,     0,     0,     0,     0,   161,
     150,   135,   139,   336,     0,     0,     0,   430,     0,     0,
     240,     0,     0,     0,     0,     0,     0,     0,    23,     0,
     385,   386,   387,   382,   383,     0,   391,     0,     0,   343,
     353,     0,   361,   196,     0,   381,   375,   188,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     198,     0,   391,   214,   337,     0,   165,   160,   162,     0,
     484,     0,   485,   454,   405,   247,   248,   249,   256,   255,
     252,   217,   218,   216,     0,     0,     0,     0,   394,     0,
       0,     0,     0,     0,   388,     0,     0,   393,   326,   328,
     329,   330,   332,   331,   325,   317,   318,   319,   320,   323,
     324,   327,     0,   394,   204,   163,    30,     0,     0,     0,
       0,     0,   425,   384,   390,   389,     0,     0,   347,     0,
     513,     0,   352,   200,   391,     0,   311,   202,   190,    30,
     166,     0,   486,    19,     0,   257,   392,     0,   349,   512,
       0,   394,     0,     0,   164,     0,     0,   395,   350,   348,
     354,   192,   312,   191,     0,    20,     0,    24,    25,    22,
       0,     0,   193,    12,    21,   487
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    99,   100,   101,   102,   103,
    1080,  1131,   928,  1129,    12,   170,   104,   412,   105,   106,
     107,   108,   224,   109,   110,   774,   573,   777,   513,   514,
     515,   516,   111,   499,   861,   112,   113,   471,   114,   115,
     225,   763,   968,   969,  1099,  1076,   116,   631,   117,   118,
     242,   119,   628,   120,   410,   121,   366,   488,   623,   122,
     123,   124,   346,   338,   125,  1113,   126,  1126,   426,   599,
     619,   832,   845,   127,   365,   848,   546,   944,   724,   926,
     128,   265,   540,   129,   447,   302,   723,   915,  1040,   130,
     444,   292,   443,   717,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   745,   141,   142,   143,   144,   751,
     960,  1064,   145,   146,   531,   147,   148,   149,   150,   151,
     152,   153,   154,   155,  1108,  1119,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   307,   339,
     931,   993,   994,   996,  1085,  1048,  1058,  1088,   310,   580,
     589,   916,   825,   590,   248,   343,   282,   267,   420,   319,
     320,   555,   556,   303,   304,   293,   294,   389,   344,   859,
     640,   641,   642,   391,   392,   393,   283,   438,   237,   256,
     358,   720,   407,   408,   409,   611,   324,   325
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -971
static const yytype_int16 yypact[] =
{
     230,   -38,   -29,   147,  -971,   230,    59,   -32,   -30,  -971,
    -971,   -18,   852,   -11,  -971,    81,  -971,     2,    11,    21,
      24,  -971,    60,    66,    79,    82,    87,    89,   101,   112,
     118,   121,   128,   140,   151,   169,   177,   179,   182,   184,
     194,   195,   203,   205,   207,   211,   212,   214,   216,   218,
     219,   220,   221,   225,   226,   228,   229,   234,   236,   237,
     243,   245,   256,    40,   258,   260,   261,  -971,   262,    -9,
     933,  -971,  -971,   265,   266,   267,   272,     4,   113,    26,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,   852,  -971,  -971,   -27,  -971,
    -971,  -971,  -971,  -971,   275,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,   269,    30,
    -971,  -111,   352,    76,   -23,   192,   947,    39,    39,   152,
     -34,    33,     0,   280,     0,   903,  -107,  -107,   -12,   186,
       0,     0,   313,     0,    13,  -107,  -107,   -31,   -12,   -12,
     -12,   113,   287,   113,     0,   947,   947,     0,   947,   947,
      88,     0,   947,   -31,   947,    84,  -971,   947,  -107,   301,
     820,   113,  -971,  -971,   233,   273,     0,     0,     0,     0,
     297,  -971,    25,  -971,   292,  -971,   193,  -971,   126,  -971,
     400,  -971,   298,  -971,    81,  -971,  -971,   299,  -971,   248,
     309,   311,   312,  -971,  -971,  -971,  -971,  -971,   316,  -971,
    -971,   317,   516,  -971,   318,   325,   327,  -971,  -971,  -107,
    -107,     0,     0,   326,     0,   328,   329,   330,   947,   332,
     615,  -971,  -971,   254,   336,  -971,  -971,  -971,   544,  -971,
    -971,   331,  -971,  -971,  -971,  -971,  -971,  -971,   551,  -971,
    -971,   344,   343,   351,  -971,  -971,  -971,   354,  -971,  -971,
     355,   356,   366,   368,  -971,  -971,   575,  -971,   370,   375,
    -971,   379,   390,   601,   401,  -971,   403,   404,   405,   407,
     409,   603,   410,   411,  -971,  -971,  -971,  -971,   416,   622,
     422,   428,   429,   432,   449,   636,   435,   -48,   436,   438,
    -971,   445,  -971,  -971,  -971,  -971,  -971,  -971,  -971,   446,
     447,   461,   486,  -971,  -971,   494,   298,   495,   498,   499,
    -971,   457,   113,   113,   500,   501,   508,   509,  -971,   490,
     493,   113,   113,  -971,   113,   113,   113,   113,   113,   248,
     449,  -971,   511,   510,  -971,  -971,  -971,  -971,  -971,  -971,
     513,    64,   106,  -971,  -971,   248,   449,   520,   521,   522,
     852,   852,  -971,  -971,   113,  -111,   728,    29,   735,   533,
     529,   947,   535,   113,   172,   740,   530,  -971,  -971,   541,
     542,   812,  -971,     0,     0,   464,  -971,   546,   549,   947,
     665,   564,   113,   565,   298,   566,   113,   298,     0,     0,
     947,   694,    38,   695,   570,   113,    50,   741,   774,   578,
     756,   755,   144,   624,     0,   707,   598,   731,   -12,   -20,
    -971,   616,   -12,   -12,   -12,   113,   618,    62,     0,    31,
     732,    20,   646,   727,   -16,    73,    33,   701,  -971,   160,
     160,   692,  -971,   198,   619,   -21,   699,   704,    -2,   889,
    -971,  -971,   385,   462,    53,    53,  -971,  -971,  -971,   126,
    -971,   947,   626,   -96,   -86,   -75,   -71,  -971,  -971,   248,
     449,   145,   110,   165,  -971,   633,   505,  -971,  -971,  -971,
     845,  -971,   642,   316,  -971,   640,   847,   539,  -971,  -971,
     327,  -971,  -971,     0,     0,   584,   647,   643,   649,   650,
    -971,   652,   589,  -971,   654,   660,  -971,   661,   676,  -971,
    -971,  -971,   669,   599,   335,  -971,   675,   670,  -971,  -971,
    -971,  -971,   710,   711,  -971,   712,   904,   683,  -971,  -971,
     715,  -971,   717,   921,  -971,   719,  -971,   716,  -971,   721,
    -971,   729,   722,  -971,   932,  -971,   733,  -971,   934,   734,
      50,   742,   743,  -971,   745,   859,  -971,  -971,  -971,  -971,
    -971,   746,  -971,   747,   748,  -971,   751,  -971,   938,   757,
    -971,   758,   759,   828,   964,   762,   763,  -971,   298,  -971,
     705,   113,  -971,  -971,   248,   766,   768,  -971,  -971,   771,
    -971,   769,   773,  -971,  -971,  -971,  -971,   977,   777,  -971,
      -8,  -971,   113,  -971,  -111,  -971,    27,  -971,    38,  -971,
      46,  -971,  -971,  -971,   780,   986,  -971,  -971,   783,  -971,
     779,  -971,   789,   910,   947,  -971,   113,   113,   947,   792,
     113,   947,   947,   799,   791,  -971,  -971,  -971,  -971,   808,
    -971,  -971,  -971,  -971,   809,  -971,   810,   811,   814,  -971,
     815,   816,   817,   818,   819,   822,   823,   824,  -971,   825,
    -971,   830,  -971,  -971,  -971,   832,  -971,  -971,  -971,  -971,
    -971,   831,  -971,   813,   837,    33,    38,  1041,   841,  -971,
     -31,  1046,   849,   882,  1054,    57,   152,   888,  -101,   972,
     857,    -6,  -971,   850,   976,  -971,   113,   860,  -111,  1002,
       0,   864,   975,   866,    -5,   978,   199,   298,  1011,   160,
    -971,  -971,   449,   886,    50,    -3,   155,   889,  -971,   -83,
    -971,  -971,   449,   248,   -63,  -971,   -61,   -58,  -971,    50,
     902,  -971,  -971,   113,  -971,   900,   288,   427,   901,    50,
     698,   922,   925,   113,  -971,   113,    39,  1028,  1034,   113,
    1043,  1027,   113,   113,   113,  -111,  1012,   113,   113,   113,
     152,  -971,   517,   417,  -971,  -971,   946,  1151,   951,   953,
    1158,   956,   954,  -971,  -971,   961,  -971,   962,  -971,  1168,
    -971,   294,   967,  -971,  -971,   968,    80,   248,   969,   973,
     662,  -971,  1175,  -971,  1177,   951,  -971,  -971,   979,  -971,
    -971,  -971,   981,     1,  -971,  -971,   248,  -971,  -971,  -971,
    -971,   298,    27,  -971,  -971,    46,  -971,   974,  1178,   449,
    -971,  1141,  -971,   113,  -971,   980,  -971,  -971,  -971,   472,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
     248,  -971,  -971,  -971,  -971,  -971,   -51,  -971,  -971,  -971,
    -111,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,   983,
     984,   985,  -971,  -971,   987,  -971,  -971,  -971,   248,   988,
    -971,  -971,   989,  -971,   990,  -971,  -971,  1182,  -971,   982,
     314,  1093,  1195,   993,   152,  -971,    -1,   152,   991,   995,
      -5,   113,  -971,  -971,   994,  1136,  1113,  -971,   998,   999,
    1000,  1001,  1003,  1004,  1005,  -971,  -971,  1006,  1007,  1008,
    -971,  1009,  1010,  1093,    80,  1203,   295,  1013,  1014,     1,
    -971,  -971,  -971,  -971,  1015,  1016,   307,  -971,   113,  1137,
     248,   113,   113,   113,   -78,    39,  1216,  1091,  -971,  1220,
    -971,  -971,  -971,  -971,  1018,  1019,  1131,  1023,  1227,  -971,
    1025,  1026,  -971,  -971,   172,   951,  -971,  -971,  1029,  1030,
    1138,  1229,    48,   152,    39,    29,    29,     0,   -34,  1234,
    -971,  1235,  1131,  -971,  -971,  1032,  -971,  -971,  -971,  1236,
    -971,  1201,  -971,   357,  -971,  -971,  -971,  -971,  -971,   992,
    -971,  -971,  -971,  -971,  1033,   314,  1144,  1037,  1074,  1243,
    1038,  1077,   152,  1040,  1093,  1159,  1161,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  1044,  1074,   837,  -971,   852,  1048,  1047,  1051,
    1049,   -78,  -971,  -971,  -971,  -971,  1138,  1045,  -971,  1052,
    -971,  1050,  -971,  -971,  1131,  1056,  -971,  -971,  -971,   852,
    -971,    50,  -971,  -971,  1057,  -971,  -971,   152,   298,  -971,
     152,  1074,  1170,   298,  -971,  1058,     0,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,    95,  1059,   298,  -971,  -971,   946,
       0,  1060,  -971,  -971,  -971,  -971
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -971,  -971,  1262,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,   139,  -971,  -971,  1031,  -105,  -361,   861,  1053,
    1199,  -485,  -971,  -971,  -971,  -971,   614,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  1211,  -971,
    -971,  -971,   308,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,   801,  1063,  -971,  -971,  -971,  -971,   739,  -971,
    -971,  -971,   340,  -971,  -971,  -971,  -582,   319,   259,  -971,
    -971,   450,   281,  -971,  -971,  -971,  -971,  -971,   206,  -971,
    -971,  1106,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -618,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,   268,   558,
    -810,   244,  -971,  -876,  -971,  -932,   204,  -970,  -461,  -597,
    -971,  -971,  -971,   479,   877,  -219,  -173,  -387,   760,   290,
    -383,  -466,  -596,  -516,  -440,  -513,  -605,  -161,   -74,  -971,
     527,  -971,  -971,   786,  -971,  -971,  1020,  -169,   736,  -971,
    -464,  -971,  -971,  -971,  -971,  -971,  -172,  -971
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -232
static const yytype_int16 yytable[] =
{
     239,   266,   371,   742,   236,   487,   659,   284,   657,   311,
     249,   313,   574,   607,   639,   517,   315,   327,   328,   518,
     330,   332,   333,   625,   626,   622,   340,   341,   342,   231,
     285,   348,   529,   379,   351,   963,   349,   350,   359,   352,
     353,   296,   285,   360,   322,   362,    16,   305,   367,   550,
     336,   775,  1060,   374,   375,   376,   377,   661,   257,   258,
     633,   597,   259,   260,   778,   617,   354,   403,   512,   322,
     355,   356,   379,   257,   258,   830,   843,   620,   855,   405,
     254,   257,   258,   565,   942,   613,   568,  1022,  1000,   363,
    1073,   784,   354,  1038,   257,   258,   355,   356,   429,   430,
    1127,   432,   316,  1098,   245,   308,   820,   609,   286,   781,
     231,   648,   610,   296,   309,   246,   247,   231,   649,   297,
     286,   650,   287,   578,   817,   317,   318,   781,   651,   347,
     231,   579,   652,   637,   287,   638,   654,  1039,   823,   653,
    1061,  1121,  1062,   655,   862,   824,   726,     9,   285,   865,
     966,   863,   967,   864,   494,   296,   866,   979,   380,   478,
     734,   943,  1111,   550,   390,   364,   406,   854,   550,   357,
       7,  1001,   255,   621,   849,   261,  1128,    11,  1094,     8,
    1063,   306,   867,    13,   337,    14,   262,   257,   258,   598,
      15,   297,   875,   618,   245,  1054,   168,   323,   261,   614,
     169,   262,   221,   831,   844,   246,   247,   263,   264,   262,
     171,   261,   245,   384,   385,   386,   387,   388,   574,   172,
     826,   230,   331,   246,   247,   586,   286,   587,   588,   173,
     263,   264,   174,   297,   232,   238,   288,   244,   530,   240,
     287,   519,   289,   263,   264,     1,     2,   298,   288,   233,
     234,    78,   535,   299,   289,   624,   604,   971,   538,   539,
     548,   549,   547,   771,   814,   770,   557,   760,   175,   972,
     560,   345,   243,   232,   176,   569,   570,   290,   291,   545,
     847,   571,   639,   881,   312,   235,   815,   177,   233,   234,
     178,   592,   268,   852,   895,   179,   596,   180,   493,   354,
     601,   602,   603,   355,   356,   608,   525,   502,   503,   181,
     504,   505,   506,   507,   508,   401,   386,   387,   388,   298,
     182,   245,   232,   658,   235,   299,   183,   380,   520,   184,
     233,   234,   246,   247,   634,   232,   185,   233,   234,   686,
     526,   245,   646,   687,   688,   629,   300,   301,   186,   537,
     233,   234,   246,   247,   288,   990,   991,   992,   656,   187,
     289,   298,   250,   251,   252,   253,   235,   299,   563,   624,
     671,   672,   567,   235,   624,   857,   858,   188,   660,   259,
     260,   577,   382,   290,   291,   189,   235,   190,   300,   301,
     191,   718,   192,   689,   553,   554,   850,  1067,  1068,   553,
     554,   326,   193,   194,   231,   383,   329,   690,   691,   692,
     693,   195,   694,   196,   695,   197,   257,   258,   999,   198,
     199,  1002,   200,   394,   201,   395,   202,   203,   204,   205,
     396,   397,   398,   206,   207,   390,   208,   209,   369,   696,
     697,   698,   210,   699,   211,   212,   700,   701,   702,   703,
     704,   213,   705,   214,   384,   385,   386,   387,   388,   384,
     385,   386,   387,   388,   215,   630,   217,   550,   218,   219,
     220,   551,  1041,   226,   227,   228,   706,   321,   707,   708,
     229,   257,   258,   241,   373,   334,   335,   709,   710,   711,
     712,   713,   714,   773,   919,   871,   345,  1065,   872,   372,
     970,  1066,  1025,   381,  1115,   785,   378,   715,   368,   788,
     414,   411,   791,   792,  1031,   415,   416,  1032,   417,   418,
     422,   716,   419,   439,   920,   921,   421,   835,   399,   423,
     404,   922,   424,   261,   425,   431,  1092,   433,   434,   435,
     923,   437,   442,   440,   262,   257,   258,   441,   924,   384,
     385,   386,   387,   388,   445,   446,  -231,   762,   448,   427,
     428,   449,   450,   451,  1079,   263,   264,   -18,   384,   385,
     386,   387,   388,   452,   925,   453,   880,   839,   772,   454,
     884,   455,   456,   887,   888,   889,   457,   837,   892,   893,
     894,  1117,   679,   422,  1120,   235,   400,   643,   261,   458,
     896,   897,   786,   787,   856,   459,   790,   466,   460,   401,
     461,   462,   463,   402,   464,   245,   465,   467,   468,   422,
     386,   387,   388,   469,   233,   234,   246,   247,   470,   472,
     263,   264,   317,   318,   873,   473,   474,   898,   899,   475,
     476,   900,   477,   479,   890,   480,   384,   385,   386,   387,
     388,   918,   481,   482,   483,   269,   270,   271,   272,   273,
     235,   274,   261,   275,   276,   277,   278,   279,   484,   492,
     257,   258,   901,   552,   644,   902,   903,   904,   905,   906,
     907,   908,   909,   910,   911,   912,   913,   914,   384,   385,
     386,   387,   388,   485,   263,   264,   257,   258,   553,   554,
     500,   486,   489,   501,   281,   490,   491,   495,   496,   869,
     384,   385,   386,   387,   388,   497,   498,   663,   509,   879,
     510,   511,  1006,   384,   385,   386,   387,   388,   521,   522,
     523,   528,   245,   384,   978,   386,   387,   388,   532,   980,
     533,   534,   536,   246,   247,   541,   542,  1118,   543,   544,
     558,   669,  1123,   947,   384,   385,   386,   387,   388,   559,
     948,   561,  1035,  1036,  1037,  1132,   384,   385,   386,   387,
     388,   562,   564,   566,   572,   575,   949,   576,   582,   581,
     269,   270,   271,   272,   273,   583,   274,   261,   275,   276,
     277,   278,   279,   950,   584,   585,   591,   593,   280,   976,
     384,   385,   386,   387,   388,   594,   269,   270,   271,   272,
     273,   685,   274,   261,   275,   276,   277,   278,   279,   263,
     264,   595,   615,   600,   280,   605,   612,   616,  -180,   281,
     627,   632,   635,   647,   951,   952,   953,   954,   636,   955,
     956,   957,   958,   959,  1069,   263,   264,   662,   664,   665,
     667,   668,   673,   675,   674,   281,   676,   677,    16,   678,
     384,   385,   386,   387,   388,   680,    17,   681,   682,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,   722,   683,   684,    31,    32,    33,    34,    35,
     721,    36,   545,   257,   258,   729,    37,    38,    39,    40,
      41,    42,    43,    44,  1033,    45,    46,    47,   728,    48,
     876,    49,    50,    51,    52,    53,    54,   725,   726,   727,
      55,    56,   730,    57,   731,   732,   733,   734,   735,   737,
      58,   384,   385,   386,   387,   388,   738,   736,   740,   747,
     739,   741,   753,  1125,   384,   385,   386,   387,   388,   743,
     744,    59,   746,   748,   749,   757,   750,  1134,   752,   384,
     385,   386,   387,   388,   754,   755,   756,   679,    60,   758,
     759,  1100,   761,   764,    61,   765,   767,    62,    63,   766,
      64,   769,    65,   768,   257,   258,   316,   779,    66,   780,
     545,    67,    68,   781,  1114,   782,   783,    69,    70,   789,
      71,   794,    72,   269,   270,   271,   272,   273,   793,   274,
     261,   275,   276,   277,   278,   279,   795,   796,   797,   798,
     812,   280,   799,   800,   801,   802,   803,   804,   257,   258,
     805,   806,   807,   808,    73,    74,    75,    76,   809,    77,
     810,   811,   263,   264,   813,    82,   816,    84,   817,    86,
     819,    88,   281,    90,   821,    92,   820,    94,   822,    96,
     827,    98,   828,    78,   829,   833,   834,   836,   838,    79,
      80,   840,   841,   842,   314,   846,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,   269,   270,   271,   272,   273,   853,
     274,   261,   275,   276,   277,   278,   279,    80,   851,   868,
     870,   874,   280,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,   882,   877,   263,   264,   878,   883,   886,   269,   270,
     271,   272,   273,   281,   274,   261,   275,   276,   277,   278,
     279,   222,   885,   927,   891,   929,   280,    81,   930,    83,
     932,    85,   933,    87,   934,    89,   935,    91,   936,    93,
     937,    95,   938,    97,   940,   941,   945,   263,   264,   961,
     946,   962,   975,   974,   973,   988,   964,   281,   965,   989,
     977,   981,   982,   983,   995,   984,   985,   986,   987,   997,
     998,  1003,  1004,  1008,  1007,  1009,  1010,  1011,  1012,  1013,
    1024,  1014,  1015,  1016,  1017,  1018,  1019,  1021,  1034,  1020,
    1042,  1026,  1029,  1043,  1044,  1045,  1030,  1046,  1027,  1047,
    1049,  1050,  1051,  1059,  1052,  1057,  1055,  1056,  1071,  1072,
    1075,  1077,  1078,  1082,  1084,  1086,  1087,  1089,  1090,  1091,
    1093,  1095,  1096,  1107,  1097,  1101,  1103,  1102,  1110,  1104,
    1081,  1122,  1109,  1112,  1116,  1124,  1130,    10,  1133,   223,
    1135,   524,   776,   370,   216,   413,   361,  1028,   606,   670,
    1005,   939,  1074,  1023,   295,  1053,  1070,  1105,   818,  1083,
    1106,   917,   527,   666,   860,   645,     0,     0,   436,     0,
     719
};

static const yytype_int16 yycheck[] =
{
     105,   174,   221,   600,    78,   366,   522,   176,   521,   182,
     171,   184,   452,   477,   499,   402,   185,   190,   191,   402,
     193,   194,   194,   489,   490,   486,   198,   199,   200,     4,
       3,   204,     3,     8,   207,   845,   205,   206,   211,   208,
     209,     3,     3,   212,    56,   214,     6,    81,   217,     3,
      81,   656,     4,   226,   227,   228,   229,   523,    81,    82,
      81,    81,    85,    86,   660,    81,     4,   240,     4,    56,
       8,     9,     8,    81,    82,    81,    81,     4,    81,   240,
       4,    81,    82,   444,     4,    65,   447,   963,    89,     5,
    1022,   673,     4,   171,    81,    82,     8,     9,   271,   272,
       5,   274,   209,  1073,   215,    72,   207,    76,    81,   210,
       4,   207,    81,     3,    81,   226,   227,     4,   214,    81,
      81,   207,    95,    73,   207,   232,   233,   210,   214,   203,
       4,    81,   207,   135,    95,   137,   207,   215,    81,   214,
      92,  1111,    94,   214,   207,    88,   207,     0,     3,   207,
     149,   214,   151,   214,   373,     3,   214,   208,   232,   207,
     211,    81,  1094,     3,   238,    81,   240,   764,     3,    81,
     208,   172,    96,   100,   756,   198,    81,   118,  1054,   208,
     132,   215,   779,   215,   215,   215,   209,    81,    82,   209,
     208,    81,   789,   209,   215,  1005,   207,   209,   198,   179,
     119,   209,   211,   209,   209,   226,   227,   230,   231,   209,
     208,   198,   215,   261,   262,   263,   264,   265,   658,   208,
     736,   217,   209,   226,   227,    81,    81,    83,    84,   208,
     230,   231,   208,    81,   209,   209,   209,   207,   209,   266,
      95,   402,   215,   230,   231,    15,    16,   209,   209,   224,
     225,   211,   421,   215,   215,   209,   475,   862,    86,    87,
     433,   434,   431,   650,   725,   648,   435,   628,   208,   865,
     439,   209,     3,   209,   208,   448,   449,   238,   239,    80,
      81,   450,   767,   796,     4,   260,   726,   208,   224,   225,
     208,   464,   100,   759,   810,   208,   468,   208,   372,     4,
     472,   473,   474,     8,     9,   478,   411,   381,   382,   208,
     384,   385,   386,   387,   388,   209,   263,   264,   265,   209,
     208,   215,   209,   213,   260,   215,   208,   401,   402,   208,
     224,   225,   226,   227,   495,   209,   208,   224,   225,     4,
     414,   215,   511,     8,     9,   147,   236,   237,   208,   423,
     224,   225,   226,   227,   209,    41,    42,    43,   213,   208,
     215,   209,    10,    11,    12,    13,   260,   215,   442,   209,
     543,   544,   446,   260,   209,   220,   221,   208,   213,    85,
      86,   455,   189,   238,   239,   208,   260,   208,   236,   237,
     208,   564,   208,    58,   234,   235,   757,  1015,  1016,   234,
     235,   215,   208,   208,     4,   212,    93,    72,    73,    74,
      75,   208,    77,   208,    79,   208,    81,    82,   934,   208,
     208,   937,   208,    23,   208,    25,   208,   208,   208,   208,
      30,    31,    32,   208,   208,   509,   208,   208,   137,   104,
     105,   106,   208,   108,   208,   208,   111,   112,   113,   114,
     115,   208,   117,   208,   261,   262,   263,   264,   265,   261,
     262,   263,   264,   265,   208,   267,   208,     3,   208,   208,
     208,     7,   985,   208,   208,   208,   141,   187,   143,   144,
     208,    81,    82,   208,   211,   195,   196,   152,   153,   154,
     155,   156,   157,   654,    77,   207,   209,  1013,   210,   266,
     861,  1014,   966,   211,  1101,   674,   209,   172,   218,   678,
     211,   213,   681,   682,   207,   267,   207,   210,   207,   207,
       4,   186,   206,   269,   107,   108,   209,   746,   128,   211,
     240,   114,   207,   198,   207,   209,  1052,   209,   209,   209,
     123,   209,   211,   207,   209,    81,    82,     3,   131,   261,
     262,   263,   264,   265,     3,   211,   213,   631,   207,   269,
     270,   207,   207,   207,   207,   230,   231,   210,   261,   262,
     263,   264,   265,   207,   157,   207,   795,   750,   652,     4,
     799,   211,   207,   802,   803,   804,   207,   748,   807,   808,
     809,  1107,     3,     4,  1110,   260,   196,   212,   198,   209,
      83,    84,   676,   677,   765,     4,   680,     4,   207,   209,
     207,   207,   207,   213,   207,   215,   207,   207,   207,     4,
     263,   264,   265,   207,   224,   225,   226,   227,     6,   207,
     230,   231,   232,   233,   207,   207,   207,   120,   121,   207,
       4,   124,   207,   207,   805,   207,   261,   262,   263,   264,
     265,   812,   207,   207,   207,   191,   192,   193,   194,   195,
     260,   197,   198,   199,   200,   201,   202,   203,   207,   212,
      81,    82,   155,   209,   212,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   261,   262,
     263,   264,   265,   207,   230,   231,    81,    82,   234,   235,
     210,   207,   207,   210,   240,   207,   207,   207,   207,   783,
     261,   262,   263,   264,   265,   207,   207,   212,   207,   793,
     210,   208,   941,   261,   262,   263,   264,   265,   208,   208,
     208,     3,   215,   261,   262,   263,   264,   265,     3,   900,
     207,   212,   207,   226,   227,     5,   216,  1108,   207,   207,
     204,   212,  1113,    91,   261,   262,   263,   264,   265,   210,
      98,    96,   981,   982,   983,  1126,   261,   262,   263,   264,
     265,   207,   207,   207,    80,    80,   114,   207,     4,    38,
     191,   192,   193,   194,   195,   207,   197,   198,   199,   200,
     201,   202,   203,   131,    38,    40,   172,    90,   209,   873,
     261,   262,   263,   264,   265,   207,   191,   192,   193,   194,
     195,   212,   197,   198,   199,   200,   201,   202,   203,   230,
     231,    90,   176,   207,   209,   207,    94,   100,   127,   240,
     138,   212,   133,   207,   172,   173,   174,   175,   134,   177,
     178,   179,   180,   181,  1017,   230,   231,   214,     3,   207,
     210,     4,   268,   210,   207,   240,   207,   207,     6,   207,
     261,   262,   263,   264,   265,   211,    14,   207,   207,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,   212,   207,   215,    33,    34,    35,    36,    37,
     215,    39,    80,    81,    82,   212,    44,    45,    46,    47,
      48,    49,    50,    51,   978,    53,    54,    55,     4,    57,
     212,    59,    60,    61,    62,    63,    64,   207,   207,   207,
      68,    69,   207,    71,   207,     4,   207,   211,   207,   207,
      78,   261,   262,   263,   264,   265,     4,   208,     4,    80,
     207,   207,     4,  1116,   261,   262,   263,   264,   265,   207,
     207,    99,   207,   207,   207,   127,   208,  1130,   207,   261,
     262,   263,   264,   265,   207,   207,   207,     3,   116,   207,
     207,  1076,   267,   207,   122,   207,   207,   125,   126,   208,
     128,     4,   130,   210,    81,    82,   209,   207,   136,     3,
      80,   139,   140,   210,  1099,   216,   207,   145,   146,   207,
     148,   210,   150,   191,   192,   193,   194,   195,   209,   197,
     198,   199,   200,   201,   202,   203,   208,   208,   208,   208,
     207,   209,   208,   208,   208,   208,   208,   208,    81,    82,
     208,   208,   208,   208,   182,   183,   184,   185,   208,   187,
     208,   210,   230,   231,   207,   225,     5,   227,   207,   229,
       4,   231,   240,   233,   172,   235,   207,   237,     4,   239,
     172,   241,    90,   211,   207,   215,    90,   207,    66,   217,
     218,   207,    97,   207,   171,    97,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   191,   192,   193,   194,   195,   213,
     197,   198,   199,   200,   201,   202,   203,   218,    97,   207,
     210,   210,   209,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   103,   210,   230,   231,   210,   102,   110,   191,   192,
     193,   194,   195,   240,   197,   198,   199,   200,   201,   202,
     203,   218,   109,   207,   142,     4,   209,   224,   207,   226,
     207,   228,     4,   230,   208,   232,   212,   234,   207,   236,
     208,   238,     4,   240,   207,   207,   207,   230,   231,     4,
     207,     4,    41,     5,   210,     3,   207,   240,   207,   207,
     210,   208,   208,   208,   101,   208,   208,   208,   208,     4,
     207,   210,   207,    67,   210,    92,   208,   208,   208,   208,
       7,   208,   208,   208,   208,   208,   208,   207,    81,   210,
       4,   208,   207,   132,     4,   207,   210,   208,   214,    98,
     207,     4,   207,     4,   208,    97,   207,   207,     4,     4,
     208,     5,    41,   210,   100,   208,   172,     4,   210,   172,
     210,    92,    91,   208,   210,   207,   205,   210,   208,   210,
     268,    91,   210,   207,   207,   207,   207,     5,  1129,    70,
     210,   410,   658,   220,    63,   244,   213,   969,   477,   540,
     940,   831,  1023,   964,   178,  1004,  1018,  1081,   730,  1045,
    1086,   812,   415,   533,   767,   509,    -1,    -1,   278,    -1,
     564
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   271,   272,   273,   274,   208,   208,     0,
     272,   118,   284,   215,   215,   208,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    33,    34,    35,    36,    37,    39,    44,    45,    46,
      47,    48,    49,    50,    51,    53,    54,    55,    57,    59,
      60,    61,    62,    63,    64,    68,    69,    71,    78,    99,
     116,   122,   125,   126,   128,   130,   136,   139,   140,   145,
     146,   148,   150,   182,   183,   184,   185,   187,   211,   217,
     218,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   275,
     276,   277,   278,   279,   286,   288,   289,   290,   291,   293,
     294,   302,   305,   306,   308,   309,   316,   318,   319,   321,
     323,   325,   329,   330,   331,   334,   336,   343,   350,   353,
     359,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   375,   376,   377,   378,   382,   383,   385,   386,   387,
     388,   389,   390,   391,   392,   393,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   207,   119,
     285,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   308,   208,   208,   208,
     208,   211,   218,   290,   292,   310,   208,   208,   208,   208,
     217,     4,   209,   224,   225,   260,   438,   448,   209,   286,
     266,   208,   320,     3,   207,   215,   226,   227,   424,   437,
      10,    11,    12,    13,     4,    96,   449,    81,    82,    85,
      86,   198,   209,   230,   231,   351,   426,   427,   100,   191,
     192,   193,   194,   195,   197,   199,   200,   201,   202,   203,
     209,   240,   426,   446,   447,     3,    81,    95,   209,   215,
     238,   239,   361,   435,   436,   361,     3,    81,   209,   215,
     236,   237,   355,   433,   434,    81,   215,   408,    72,    81,
     418,   426,     4,   426,   171,   447,   209,   232,   233,   429,
     430,   429,    56,   209,   456,   457,   215,   426,   426,    93,
     426,   209,   426,   456,   429,   429,    81,   215,   333,   409,
     456,   456,   456,   425,   438,   209,   332,   438,   426,   447,
     447,   426,   447,   447,     4,     8,     9,    81,   450,   426,
     447,   333,   447,     5,    81,   344,   326,   447,   429,   137,
     289,   425,   266,   211,   426,   426,   426,   426,   209,     8,
     438,   211,   189,   212,   261,   262,   263,   264,   265,   437,
     438,   443,   444,   445,    23,    25,    30,    31,    32,   128,
     196,   209,   213,   426,   429,   437,   438,   452,   453,   454,
     324,   213,   287,   285,   211,   267,   207,   207,   207,   206,
     428,   209,     4,   211,   207,   207,   338,   429,   429,   426,
     426,   209,   426,   209,   209,   209,   446,   209,   447,   269,
     207,     3,   211,   362,   360,     3,   211,   354,   207,   207,
     207,   207,   207,   207,     4,   211,   207,   207,   209,     4,
     207,   207,   207,   207,   207,   207,     4,   207,   207,   207,
       6,   307,   207,   207,   207,   207,     4,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   287,   327,   207,
     207,   207,   212,   438,   425,   207,   207,   207,   207,   303,
     210,   210,   438,   438,   438,   438,   438,   438,   438,   207,
     210,   208,     4,   298,   299,   300,   301,   427,   430,   437,
     438,   208,   208,   208,   288,   286,   438,   424,     3,     3,
     209,   384,     3,   207,   212,   447,   207,   438,    86,    87,
     352,     5,   216,   207,   207,    80,   346,   447,   426,   426,
       3,     7,   209,   234,   235,   431,   432,   447,   204,   210,
     447,    96,   207,   438,   207,   287,   207,   438,   287,   426,
     426,   447,    80,   296,   434,    80,   207,   438,    73,    81,
     419,    38,     4,   207,    38,    40,    81,    83,    84,   420,
     423,   172,   426,    90,   207,    90,   456,    81,   209,   339,
     207,   456,   456,   456,   425,   207,   332,   450,   426,    76,
      81,   455,    94,    65,   179,   176,   100,    81,   209,   340,
       4,   100,   418,   328,   209,   431,   431,   138,   322,   147,
     267,   317,   212,    81,   437,   133,   134,   135,   137,   291,
     440,   441,   442,   212,   212,   443,   447,   207,   207,   214,
     207,   214,   207,   214,   207,   214,   213,   435,   213,   433,
     213,   431,   214,   212,     3,   207,   428,   210,     4,   212,
     338,   426,   426,   268,   207,   210,   207,   207,   207,     3,
     211,   207,   207,   207,   215,   212,     4,     8,     9,    58,
      72,    73,    74,    75,    77,    79,   104,   105,   106,   108,
     111,   112,   113,   114,   115,   117,   141,   143,   144,   152,
     153,   154,   155,   156,   157,   172,   186,   363,   426,   448,
     451,   215,   212,   356,   348,   207,   207,   207,     4,   212,
     207,   207,     4,   207,   211,   207,   208,   207,     4,   207,
       4,   207,   419,   207,   207,   374,   207,    80,   207,   207,
     208,   379,   207,     4,   207,   207,   207,   127,   207,   207,
     287,   267,   438,   311,   207,   207,   208,   207,   210,     4,
     430,   427,   438,   437,   295,   436,   296,   297,   432,   207,
       3,   210,   216,   207,   346,   447,   438,   438,   447,   207,
     438,   447,   447,   209,   210,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   210,   207,   207,   418,   434,     5,   207,   409,     4,
     207,   172,     4,    81,    88,   422,   433,   172,    90,   207,
      81,   209,   341,   215,    90,   425,   207,   437,    66,   426,
     207,    97,   207,    81,   209,   342,    97,    81,   345,   346,
     287,    97,   431,   213,   419,    81,   437,   220,   221,   439,
     440,   304,   207,   214,   214,   207,   214,   419,   207,   438,
     210,   207,   210,   207,   210,   419,   212,   210,   210,   438,
     425,   435,   103,   102,   425,   109,   110,   425,   425,   425,
     437,   142,   425,   425,   425,   433,    83,    84,   120,   121,
     124,   155,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   357,   421,   423,   437,    77,
     107,   108,   114,   123,   131,   157,   349,   207,   282,     4,
     207,   410,   207,     4,   208,   212,   207,   208,     4,   351,
     207,   207,     4,    81,   347,   207,   207,    91,    98,   114,
     131,   172,   173,   174,   175,   177,   178,   179,   180,   181,
     380,     4,     4,   410,   207,   207,   149,   151,   312,   313,
     287,   436,   432,   210,     5,    41,   438,   210,   262,   208,
     437,   208,   208,   208,   208,   208,   208,   208,     3,   207,
      41,    42,    43,   411,   412,   101,   413,     4,   207,   433,
      89,   172,   433,   210,   207,   342,   425,   210,    67,    92,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     210,   207,   413,   347,     7,   450,   208,   214,   312,   207,
     210,   207,   210,   438,    81,   425,   425,   425,   171,   215,
     358,   435,     4,   132,     4,   207,   208,    98,   415,   207,
       4,   207,   208,   352,   410,   207,   207,    97,   416,     4,
       4,    92,    94,   132,   381,   433,   435,   384,   384,   426,
     408,     4,     4,   415,   348,   208,   315,     5,    41,   207,
     280,   268,   210,   411,   100,   414,   208,   172,   417,     4,
     210,   172,   433,   210,   413,    92,    91,   210,   417,   314,
     286,   207,   210,   205,   210,   358,   416,   208,   394,   210,
     208,   415,   207,   335,   286,   419,   207,   433,   287,   395,
     433,   417,    91,   287,   207,   426,   337,     5,    81,   283,
     207,   281,   287,   282,   426,   210
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
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_MYTHIC_TYPE));
		      (yyval.i) = 0x2000000;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1908 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x4000000;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1913 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x8000000;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1918 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x10000000;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1925 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1931 "lev_comp.y"
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

  case 296:

/* Line 1455 of yacc.c  */
#line 1960 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1965 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1973 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1979 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1983 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1991 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 1998 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2005 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2015 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2025 "lev_comp.y"
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

  case 306:

/* Line 1455 of yacc.c  */
#line 2041 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2051 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2055 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2061 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2067 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2073 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2077 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2083 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2089 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2096 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2104 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2110 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2115 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2120 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2125 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2130 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2136 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2142 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2147 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2152 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2157 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2162 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2167 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2177 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2183 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2189 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2194 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2201 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2208 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2212 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2218 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2224 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2230 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2235 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2242 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2248 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2258 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2264 "lev_comp.y"
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

  case 348:

/* Line 1455 of yacc.c  */
#line 2283 "lev_comp.y"
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

  case 349:

/* Line 1455 of yacc.c  */
#line 2295 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2299 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2305 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2310 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2315 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (long)(yyvsp[(9) - (9)].i), (long)(yyvsp[(7) - (9)].i), (long)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2320 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((long)(yyvsp[(9) - (13)].i), (long)(yyvsp[(7) - (13)].i), (long)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2326 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2330 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2336 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2342 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2346 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2352 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2356 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2362 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2366 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2371 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2378 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2382 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2389 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2393 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2398 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2405 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2409 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2414 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2422 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2428 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2435 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2439 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2446 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2457 "lev_comp.y"
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

  case 381:

/* Line 1455 of yacc.c  */
#line 2470 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2474 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2480 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2484 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2491 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2495 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2499 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2505 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2509 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2515 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2521 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2525 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2531 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2537 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2542 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2559 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2567 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2577 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2586 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2591 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2598 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2609 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2615 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2619 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2623 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2630 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2640 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2647 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2651 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2657 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2661 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2669 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2673 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2680 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2690 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2703 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2707 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2714 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2724 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2733 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2744 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2748 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2755 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2765 "lev_comp.y"
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

  case 435:

/* Line 1455 of yacc.c  */
#line 2776 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2785 "lev_comp.y"
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

  case 437:

/* Line 1455 of yacc.c  */
#line 2795 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2801 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2805 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2812 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2822 "lev_comp.y"
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

  case 442:

/* Line 1455 of yacc.c  */
#line 2833 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2842 "lev_comp.y"
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

  case 444:

/* Line 1455 of yacc.c  */
#line 2852 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2856 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2862 "lev_comp.y"
    { ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2864 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2870 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2874 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2878 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2882 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2890 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2899 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2903 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2907 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2911 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2915 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2918 "lev_comp.y"
    { ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2922 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2929 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2938 "lev_comp.y"
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

  case 466:

/* Line 1455 of yacc.c  */
#line 2979 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2983 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2990 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 2997 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 3007 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 3011 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 3019 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3023 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3027 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3031 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3035 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3040 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3044 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3048 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3053 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3058 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3063 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3067 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3072 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3077 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3081 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3085 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3090 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3094 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3101 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3107 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3111 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3117 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3133 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3137 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3141 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3145 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3167 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3171 "lev_comp.y"
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

  case 513:

/* Line 1455 of yacc.c  */
#line 3197 "lev_comp.y"
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
#line 7245 "lev_comp.tab.c"
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
#line 3225 "lev_comp.y"


/*lev_comp.y*/

