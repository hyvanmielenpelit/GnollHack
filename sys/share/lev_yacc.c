
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
#line 223 "lev_comp.tab.c"

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
     QUANTITY_ID = 395,
     BURIED_ID = 396,
     LOOP_ID = 397,
     FOR_ID = 398,
     TO_ID = 399,
     SWITCH_ID = 400,
     CASE_ID = 401,
     BREAK_ID = 402,
     DEFAULT_ID = 403,
     ERODED_ID = 404,
     TRAPPED_STATE = 405,
     RECHARGED_ID = 406,
     INVIS_ID = 407,
     GREASED_ID = 408,
     INDESTRUCTIBLE_ID = 409,
     FEMALE_ID = 410,
     MALE_ID = 411,
     WAITFORU_ID = 412,
     PROTECTOR_ID = 413,
     CANCELLED_ID = 414,
     REVIVED_ID = 415,
     AVENGE_ID = 416,
     FLEEING_ID = 417,
     BLINDED_ID = 418,
     PARALYZED_ID = 419,
     STUNNED_ID = 420,
     CONFUSED_ID = 421,
     SEENTRAPS_ID = 422,
     ALL_ID = 423,
     MONTYPE_ID = 424,
     OBJTYPE_ID = 425,
     TERTYPE_ID = 426,
     TERTYPE2_ID = 427,
     LEVER_EFFECT_TYPE = 428,
     SWITCHABLE_ID = 429,
     CONTINUOUSLY_USABLE_ID = 430,
     TARGET_ID = 431,
     TRAPTYPE_ID = 432,
     EFFECT_FLAG_ID = 433,
     GRAVE_ID = 434,
     BRAZIER_ID = 435,
     SIGNPOST_ID = 436,
     TREE_ID = 437,
     ERODEPROOF_ID = 438,
     FUNCTION_ID = 439,
     MSG_OUTPUT_TYPE = 440,
     COMPARE_TYPE = 441,
     UNKNOWN_TYPE = 442,
     rect_ID = 443,
     fillrect_ID = 444,
     line_ID = 445,
     randline_ID = 446,
     grow_ID = 447,
     selection_ID = 448,
     flood_ID = 449,
     rndcoord_ID = 450,
     circle_ID = 451,
     ellipse_ID = 452,
     filter_ID = 453,
     complement_ID = 454,
     gradient_ID = 455,
     GRADIENT_TYPE = 456,
     LIMITED = 457,
     HUMIDITY_TYPE = 458,
     STRING = 459,
     MAP_ID = 460,
     NQSTRING = 461,
     VARSTRING = 462,
     CFUNC = 463,
     CFUNC_INT = 464,
     CFUNC_STR = 465,
     CFUNC_COORD = 466,
     CFUNC_REGION = 467,
     VARSTRING_INT = 468,
     VARSTRING_INT_ARRAY = 469,
     VARSTRING_STRING = 470,
     VARSTRING_STRING_ARRAY = 471,
     VARSTRING_VAR = 472,
     VARSTRING_VAR_ARRAY = 473,
     VARSTRING_COORD = 474,
     VARSTRING_COORD_ARRAY = 475,
     VARSTRING_REGION = 476,
     VARSTRING_REGION_ARRAY = 477,
     VARSTRING_MAPCHAR = 478,
     VARSTRING_MAPCHAR_ARRAY = 479,
     VARSTRING_MONST = 480,
     VARSTRING_MONST_ARRAY = 481,
     VARSTRING_OBJ = 482,
     VARSTRING_OBJ_ARRAY = 483,
     VARSTRING_SEL = 484,
     VARSTRING_SEL_ARRAY = 485,
     METHOD_INT = 486,
     METHOD_INT_ARRAY = 487,
     METHOD_STRING = 488,
     METHOD_STRING_ARRAY = 489,
     METHOD_VAR = 490,
     METHOD_VAR_ARRAY = 491,
     METHOD_COORD = 492,
     METHOD_COORD_ARRAY = 493,
     METHOD_REGION = 494,
     METHOD_REGION_ARRAY = 495,
     METHOD_MAPCHAR = 496,
     METHOD_MAPCHAR_ARRAY = 497,
     METHOD_MONST = 498,
     METHOD_MONST_ARRAY = 499,
     METHOD_OBJ = 500,
     METHOD_OBJ_ARRAY = 501,
     METHOD_SEL = 502,
     METHOD_SEL_ARRAY = 503,
     DICE = 504
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 151 "lev_comp.y"

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
#line 547 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 559 "lev_comp.tab.c"

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
#define YYLAST   1306

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  267
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  188
/* YYNRULES -- Number of rules.  */
#define YYNRULES  511
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1132

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   504

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   262,   266,     2,
     206,   207,   260,   258,   204,   259,   264,   261,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   205,     2,
       2,   263,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   208,     2,   209,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   210,   265,   211,     2,     2,     2,     2,
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
     195,   196,   197,   198,   199,   200,   201,   202,   203,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257
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
     861,   865,   869,   873,   877,   881,   885,   887,   889,   891,
     895,   899,   903,   909,   917,   923,   932,   934,   938,   942,
     948,   954,   962,   970,   977,   983,   984,   987,   993,   997,
    1009,  1023,  1030,  1031,  1035,  1037,  1041,  1045,  1049,  1053,
    1055,  1057,  1061,  1065,  1069,  1073,  1077,  1081,  1083,  1085,
    1087,  1089,  1093,  1097,  1099,  1105,  1115,  1121,  1129,  1133,
    1137,  1143,  1153,  1159,  1165,  1171,  1172,  1186,  1187,  1189,
    1197,  1209,  1219,  1233,  1237,  1245,  1253,  1259,  1267,  1273,
    1283,  1289,  1295,  1299,  1307,  1311,  1319,  1327,  1331,  1337,
    1343,  1347,  1353,  1361,  1371,  1373,  1375,  1377,  1379,  1381,
    1382,  1385,  1387,  1391,  1393,  1395,  1397,  1398,  1402,  1404,
    1405,  1409,  1411,  1412,  1416,  1418,  1420,  1422,  1424,  1426,
    1428,  1430,  1432,  1434,  1438,  1440,  1442,  1447,  1449,  1451,
    1456,  1458,  1460,  1465,  1467,  1472,  1478,  1480,  1484,  1486,
    1490,  1492,  1494,  1499,  1509,  1511,  1513,  1518,  1520,  1526,
    1528,  1530,  1535,  1537,  1539,  1545,  1547,  1549,  1551,  1556,
    1558,  1560,  1566,  1568,  1570,  1572,  1576,  1578,  1580,  1584,
    1586,  1591,  1595,  1599,  1603,  1607,  1611,  1615,  1617,  1619,
    1623,  1625,  1629,  1630,  1632,  1634,  1636,  1638,  1642,  1643,
    1645,  1647,  1650,  1653,  1658,  1665,  1670,  1677,  1684,  1691,
    1698,  1701,  1708,  1717,  1726,  1737,  1752,  1755,  1757,  1761,
    1763,  1767,  1769,  1771,  1773,  1775,  1777,  1779,  1781,  1783,
    1785,  1787,  1789,  1791,  1793,  1795,  1797,  1799,  1801,  1803,
    1805,  1816
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     268,     0,    -1,    -1,   269,    -1,   270,    -1,   270,   269,
      -1,   271,   281,   283,    -1,    16,   205,   212,    -1,    15,
     205,   212,   204,     3,    -1,    17,   205,    11,   204,   381,
      -1,    17,   205,    10,   204,     3,    -1,    17,   205,    13,
      -1,    17,   205,    12,   204,     3,   204,     3,   204,     5,
     204,     5,   204,   416,   204,   280,   279,    -1,    18,   205,
     446,    -1,   135,   205,   136,   204,   137,    -1,   135,   205,
     136,    -1,    29,   205,     4,   204,   293,    -1,    21,   205,
      99,    -1,    -1,   204,   202,    -1,    -1,   204,   423,    -1,
      -1,   204,     3,    -1,     5,    -1,    80,    -1,    -1,   117,
     205,   282,    -1,   118,   204,   282,    -1,   118,    -1,    -1,
     285,   283,    -1,   210,   283,   211,    -1,   326,    -1,   272,
      -1,   273,    -1,   389,    -1,   276,    -1,   274,    -1,   396,
      -1,   275,    -1,   395,    -1,   393,    -1,   394,    -1,   397,
      -1,   398,    -1,   399,    -1,   400,    -1,   401,    -1,   370,
      -1,   328,    -1,   291,    -1,   290,    -1,   384,    -1,   340,
      -1,   362,    -1,   403,    -1,   404,    -1,   372,    -1,   373,
      -1,   374,    -1,   402,    -1,   306,    -1,   316,    -1,   318,
      -1,   322,    -1,   320,    -1,   303,    -1,   313,    -1,   299,
      -1,   302,    -1,   365,    -1,   375,    -1,   347,    -1,   363,
      -1,   350,    -1,   356,    -1,   385,    -1,   380,    -1,   368,
      -1,   327,    -1,   386,    -1,   387,    -1,   388,    -1,   390,
      -1,   333,    -1,   331,    -1,   379,    -1,   383,    -1,   382,
      -1,   366,    -1,   367,    -1,   369,    -1,   361,    -1,   364,
      -1,   222,    -1,   224,    -1,   226,    -1,   228,    -1,   230,
      -1,   232,    -1,   234,    -1,   236,    -1,   238,    -1,   221,
      -1,   223,    -1,   225,    -1,   227,    -1,   229,    -1,   231,
      -1,   233,    -1,   235,    -1,   237,    -1,   286,    -1,   287,
      -1,   215,    -1,   215,    -1,   287,    -1,   139,   205,   286,
      -1,   288,   263,   435,    -1,   288,   263,   193,   205,   444,
      -1,   288,   263,   434,    -1,   288,   263,   451,   205,   428,
      -1,   288,   263,   450,   205,   430,    -1,   288,   263,   449,
     205,   432,    -1,   288,   263,   423,    -1,   288,   263,   426,
      -1,   288,   263,   210,   297,   211,    -1,   288,   263,   210,
     296,   211,    -1,   288,   263,   210,   295,   211,    -1,   288,
     263,   451,   205,   210,   294,   211,    -1,   288,   263,   450,
     205,   210,   293,   211,    -1,   288,   263,   449,   205,   210,
     292,   211,    -1,   288,   263,   210,   298,   211,    -1,   433,
      -1,   292,   204,   433,    -1,   431,    -1,   293,   204,   431,
      -1,   429,    -1,   294,   204,   429,    -1,   427,    -1,   295,
     204,   427,    -1,   424,    -1,   296,   204,   424,    -1,   435,
      -1,   297,   204,   435,    -1,   434,    -1,   298,   204,   434,
      -1,    -1,    -1,   184,   214,   206,   300,   439,   207,   301,
     284,    -1,   214,   206,   442,   207,    -1,   138,    -1,    -1,
       6,    -1,     6,    -1,   208,   435,   186,   435,   209,    -1,
     208,   435,   209,    -1,    -1,    -1,   145,   307,   208,   422,
     209,   308,   210,   309,   211,    -1,    -1,   310,   309,    -1,
      -1,   146,   447,   205,   311,   283,    -1,    -1,   148,   205,
     312,   283,    -1,   147,    -1,   264,   264,    -1,   144,    -1,
     143,   289,   263,   435,   314,   435,    -1,    -1,   315,   317,
     284,    -1,    -1,   142,   208,   422,   209,   319,   284,    -1,
      -1,   305,   205,   321,   285,    -1,    -1,   125,   305,   323,
     324,    -1,   284,    -1,    -1,   284,   325,   126,   284,    -1,
      14,   205,   434,    -1,    71,    -1,    71,   205,   447,    -1,
      71,   205,    80,    -1,    59,   205,   329,   204,   329,    -1,
      59,   205,   329,   204,   447,    -1,   206,     4,   204,    79,
     204,   344,   207,    -1,   406,   304,   204,   416,    -1,    -1,
     115,   205,   330,   204,   337,   204,   339,   407,   410,   412,
     414,   332,   284,    -1,    -1,    51,   205,   330,   204,   336,
     204,   338,   204,   339,   407,   410,   412,   414,   334,   284,
      -1,    -1,   204,     5,    -1,   206,     4,   204,     4,   207,
      -1,    80,    -1,   206,     4,   204,     4,   207,    -1,    80,
      -1,   206,   348,   204,   349,   207,    -1,    80,    -1,   206,
       4,   204,     4,   207,    -1,    80,    -1,   124,   205,   341,
     204,   415,   204,   342,   204,   344,   345,    -1,    27,   205,
     415,   204,   444,   345,    -1,     5,    -1,    80,    -1,   343,
      -1,    80,    -1,    79,    -1,    79,   265,   343,    -1,     4,
      -1,    80,    -1,    -1,   345,   204,   346,    -1,   130,   205,
     131,    -1,    76,   205,   432,    -1,   113,   205,     4,    -1,
     154,    -1,   106,    -1,   107,    -1,   122,    -1,    20,    -1,
      19,   205,   348,   204,   349,   335,   213,    -1,    19,   205,
     423,   335,   213,    -1,    84,    -1,    85,    -1,    86,    -1,
      85,    -1,    25,   205,   352,    -1,    -1,    25,   205,   352,
     351,   284,    -1,   430,   204,   423,   353,    -1,    -1,   353,
     204,   354,    -1,   434,    -1,   119,    -1,   120,    -1,   418,
      -1,   123,   434,    -1,   155,    -1,   156,    -1,   152,    -1,
     159,    -1,   160,    -1,   161,    -1,   162,   205,   422,    -1,
     163,   205,   422,    -1,   164,   205,   422,    -1,   165,    -1,
     166,    -1,   167,   205,   355,    -1,   157,    -1,   158,    -1,
     212,    -1,   168,    -1,   212,   265,   355,    -1,    23,   205,
     358,    -1,    -1,    24,   205,   358,   357,   284,    -1,   432,
     359,    -1,    -1,   359,   204,   360,    -1,    74,    -1,   169,
     205,   430,    -1,   448,    -1,   116,   205,   434,    -1,   140,
     205,   422,    -1,   141,    -1,    73,    -1,   149,   205,   422,
      -1,   183,    -1,    72,    -1,   150,    -1,   151,   205,   422,
      -1,   152,    -1,   153,    -1,   423,    -1,   104,   205,   101,
      -1,   103,   205,   102,    -1,   105,   205,   422,    -1,   112,
     205,   422,    -1,   113,   205,   422,    -1,   114,   205,   422,
      -1,    76,   205,   432,    -1,   154,    -1,   107,    -1,    78,
      -1,   110,   205,   108,    -1,   111,   205,   109,    -1,    58,
     205,   422,    -1,    26,   205,   405,   204,   423,    -1,    28,
     205,   423,   204,    79,   204,   415,    -1,    33,   205,   423,
     204,    79,    -1,    33,   205,   423,   204,    79,   204,     5,
     279,    -1,    34,    -1,    34,   205,   444,    -1,    34,   205,
     168,    -1,    47,   205,   423,   204,    89,    -1,    48,   205,
     423,   204,    89,    -1,    48,   205,   453,   204,   453,   204,
      89,    -1,    53,   205,   453,   204,   453,   204,   212,    -1,
      54,   205,   453,   204,   453,   371,    -1,    55,   205,   453,
     204,   453,    -1,    -1,   204,    89,    -1,    62,   205,   444,
     204,    93,    -1,    63,   205,   444,    -1,    64,   205,   423,
     204,   176,   205,   423,   204,    91,   204,    90,    -1,    64,
     205,   423,   204,    65,   204,    66,   204,    67,   204,    91,
     204,    90,    -1,    77,   205,   423,   204,   173,   376,    -1,
      -1,   376,   204,   377,    -1,    90,    -1,   169,   205,   430,
      -1,   170,   205,   432,    -1,   171,   205,   381,    -1,   172,
     205,   381,    -1,   174,    -1,   175,    -1,   176,   205,   423,
      -1,   177,   205,   405,    -1,   130,   205,   378,    -1,    97,
     205,   413,    -1,   178,   205,     4,    -1,   113,   205,     4,
      -1,     4,    -1,    91,    -1,   131,    -1,    93,    -1,    69,
     205,   444,    -1,    68,   205,   444,    -1,     3,    -1,   206,
       3,   204,   416,   207,    -1,   129,   205,   426,   204,   428,
     204,   428,   204,     7,    -1,   127,   205,   444,   204,   428,
      -1,   127,   205,   444,   204,   428,   204,    96,    -1,    49,
     205,   426,    -1,    50,   205,   426,    -1,    39,   205,   212,
     204,    40,    -1,    39,   205,   212,   204,    40,   204,   169,
     205,   430,    -1,    36,   205,   426,   204,    38,    -1,    37,
     205,   453,   204,    38,    -1,    22,   205,   444,   204,    95,
      -1,    -1,    35,   205,   426,   204,   416,   204,   406,   407,
     410,   412,   414,   391,   392,    -1,    -1,   284,    -1,    44,
     205,   423,   204,   417,   204,   419,    -1,    44,   205,   423,
     204,   417,   204,   419,   204,   169,   205,   430,    -1,    44,
     205,   423,   204,   417,   204,   419,   204,    88,    -1,    44,
     205,   423,   204,   417,   204,   419,   204,    88,   204,   169,
     205,   430,    -1,    45,   205,   423,    -1,    45,   205,   423,
     204,   169,   205,   430,    -1,    98,   205,   444,   204,    99,
     204,    96,    -1,   121,   205,   444,   204,     4,    -1,   121,
     205,   444,   204,    99,   204,    96,    -1,    46,   205,    92,
     204,   423,    -1,    46,   205,    92,   204,   423,   204,   169,
     205,   430,    -1,   179,   205,   423,   204,   434,    -1,   179,
     205,   423,   204,    80,    -1,   179,   205,   423,    -1,   180,
     205,   423,   204,   132,   204,   416,    -1,   180,   205,   423,
      -1,   181,   205,   423,   204,   133,   204,   434,    -1,   181,
     205,   423,   204,   133,   204,    80,    -1,   181,   205,   423,
      -1,   182,   205,   423,   204,   134,    -1,   182,   205,   423,
     204,   136,    -1,   182,   205,   423,    -1,    60,   205,   435,
     204,   423,    -1,    61,   205,   423,   204,   452,   204,   434,
      -1,    57,   205,   422,   204,   422,   204,   422,   204,   422,
      -1,    57,    -1,   212,    -1,    80,    -1,   212,    -1,    80,
      -1,    -1,   204,   408,    -1,   409,    -1,   409,   204,   408,
      -1,    41,    -1,    42,    -1,    43,    -1,    -1,   100,   205,
     411,    -1,    99,    -1,    -1,    97,   205,   413,    -1,    96,
      -1,    -1,   169,   205,   430,    -1,    72,    -1,    80,    -1,
      73,    -1,    80,    -1,    83,    -1,   420,    -1,    80,    -1,
      83,    -1,   420,    -1,    82,   205,    80,    -1,    87,    -1,
      80,    -1,    82,   208,     4,   209,    -1,   212,    -1,   223,
      -1,   224,   208,   435,   209,    -1,   435,    -1,   424,    -1,
     195,   206,   444,   207,    -1,   227,    -1,   228,   208,   435,
     209,    -1,   206,     4,   204,     4,   207,    -1,    80,    -1,
      81,   425,   209,    -1,   203,    -1,   203,   204,   425,    -1,
     427,    -1,   229,    -1,   230,   208,   435,   209,    -1,   206,
       4,   204,     4,   204,     4,   204,     4,   207,    -1,   429,
      -1,   231,    -1,   232,   208,   435,   209,    -1,     3,    -1,
     206,     3,   204,   416,   207,    -1,   431,    -1,   233,    -1,
     234,   208,   435,   209,    -1,   212,    -1,     3,    -1,   206,
       3,   204,   212,   207,    -1,    80,    -1,   433,    -1,   235,
      -1,   236,   208,   435,   209,    -1,   212,    -1,     3,    -1,
     206,     3,   204,   212,   207,    -1,    80,    -1,    94,    -1,
     421,    -1,   434,   264,   421,    -1,     4,    -1,   445,    -1,
     206,     8,   207,    -1,   221,    -1,   222,   208,   435,   209,
      -1,   435,   258,   435,    -1,   435,   259,   435,    -1,   435,
     260,   435,    -1,   435,   261,   435,    -1,   435,   262,   435,
      -1,   206,   435,   207,    -1,   217,    -1,   218,    -1,   288,
     205,   436,    -1,   437,    -1,   438,   204,   437,    -1,    -1,
     438,    -1,   435,    -1,   434,    -1,   440,    -1,   441,   204,
     440,    -1,    -1,   441,    -1,   423,    -1,   188,   426,    -1,
     189,   426,    -1,   190,   423,   204,   423,    -1,   191,   423,
     204,   423,   204,   435,    -1,   192,   206,   444,   207,    -1,
     192,   206,   343,   204,   444,   207,    -1,   198,   206,     7,
     204,   444,   207,    -1,   198,   206,   444,   204,   444,   207,
      -1,   198,   206,   428,   204,   444,   207,    -1,   194,   423,
      -1,   196,   206,   423,   204,   435,   207,    -1,   196,   206,
     423,   204,   435,   204,    41,   207,    -1,   197,   206,   423,
     204,   435,   204,   435,   207,    -1,   197,   206,   423,   204,
     435,   204,   435,   204,    41,   207,    -1,   200,   206,   201,
     204,   206,   435,   259,   435,   277,   207,   204,   423,   278,
     207,    -1,   199,   443,    -1,   237,    -1,   206,   444,   207,
      -1,   443,    -1,   443,   266,   444,    -1,   257,    -1,    95,
      -1,     4,    -1,     8,    -1,     9,    -1,     4,    -1,     8,
      -1,     9,    -1,     4,    -1,   445,    -1,    30,    -1,    23,
      -1,    31,    -1,    25,    -1,    32,    -1,   127,    -1,    75,
      -1,    80,    -1,   454,    -1,    56,   206,     4,   204,     4,
     204,     4,   204,     4,   207,    -1,   206,     4,   204,     4,
     204,     4,   204,     4,   207,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   284,   284,   285,   288,   289,   292,   315,   320,   342,
     355,   367,   373,   402,   408,   412,   418,   424,   431,   434,
     441,   445,   452,   455,   462,   463,   467,   470,   477,   481,
     488,   491,   497,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,   517,   518,   519,
     520,   521,   522,   523,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
     540,   541,   542,   543,   544,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   561,   562,   563,   564,   567,   568,   569,   570,   571,
     572,   573,   574,   575,   578,   579,   580,   581,   582,   583,
     584,   585,   586,   589,   590,   591,   594,   595,   598,   614,
     620,   626,   632,   638,   644,   650,   656,   662,   672,   682,
     692,   702,   712,   722,   734,   739,   746,   751,   758,   763,
     770,   774,   780,   785,   792,   796,   802,   806,   813,   835,
     812,   849,   904,   911,   914,   920,   927,   931,   940,   944,
     939,  1007,  1008,  1012,  1011,  1025,  1024,  1039,  1049,  1050,
    1053,  1091,  1090,  1125,  1124,  1155,  1154,  1187,  1186,  1212,
    1223,  1222,  1250,  1256,  1261,  1266,  1273,  1280,  1289,  1297,
    1309,  1308,  1332,  1331,  1355,  1358,  1364,  1374,  1380,  1389,
    1395,  1400,  1406,  1411,  1417,  1428,  1434,  1435,  1438,  1439,
    1442,  1446,  1452,  1453,  1457,  1463,  1471,  1476,  1481,  1486,
    1491,  1496,  1501,  1509,  1516,  1524,  1532,  1533,  1536,  1537,
    1540,  1545,  1544,  1558,  1565,  1572,  1580,  1585,  1591,  1597,
    1603,  1609,  1614,  1619,  1624,  1629,  1634,  1639,  1644,  1649,
    1654,  1659,  1664,  1670,  1675,  1682,  1691,  1695,  1708,  1717,
    1716,  1734,  1744,  1750,  1758,  1764,  1769,  1774,  1779,  1784,
    1789,  1794,  1799,  1804,  1818,  1824,  1829,  1834,  1839,  1844,
    1849,  1854,  1859,  1864,  1869,  1874,  1879,  1884,  1889,  1894,
    1899,  1904,  1911,  1917,  1946,  1951,  1959,  1965,  1969,  1977,
    1984,  1991,  2001,  2011,  2026,  2037,  2040,  2046,  2052,  2058,
    2062,  2068,  2075,  2081,  2089,  2095,  2100,  2105,  2110,  2115,
    2121,  2127,  2132,  2137,  2142,  2147,  2152,  2159,  2159,  2159,
    2159,  2162,  2168,  2174,  2179,  2186,  2193,  2197,  2203,  2209,
    2215,  2220,  2227,  2233,  2243,  2250,  2249,  2281,  2284,  2290,
    2295,  2300,  2305,  2311,  2315,  2321,  2327,  2331,  2337,  2341,
    2347,  2351,  2356,  2363,  2367,  2374,  2378,  2383,  2390,  2394,
    2399,  2407,  2413,  2420,  2424,  2431,  2439,  2442,  2452,  2456,
    2459,  2465,  2469,  2476,  2480,  2484,  2491,  2494,  2500,  2507,
    2510,  2516,  2523,  2527,  2534,  2535,  2538,  2539,  2542,  2543,
    2544,  2550,  2551,  2552,  2558,  2559,  2562,  2571,  2576,  2583,
    2594,  2600,  2604,  2608,  2615,  2625,  2632,  2636,  2642,  2646,
    2654,  2658,  2665,  2675,  2688,  2692,  2699,  2709,  2718,  2729,
    2733,  2740,  2750,  2761,  2770,  2780,  2786,  2790,  2797,  2807,
    2818,  2827,  2837,  2841,  2848,  2849,  2855,  2859,  2863,  2867,
    2875,  2884,  2888,  2892,  2896,  2900,  2904,  2907,  2914,  2923,
    2956,  2957,  2960,  2961,  2964,  2968,  2975,  2982,  2993,  2996,
    3004,  3008,  3012,  3016,  3020,  3025,  3029,  3033,  3038,  3043,
    3048,  3052,  3057,  3062,  3066,  3070,  3075,  3079,  3086,  3092,
    3096,  3102,  3109,  3110,  3113,  3114,  3115,  3118,  3122,  3126,
    3130,  3136,  3137,  3140,  3141,  3144,  3145,  3148,  3149,  3152,
    3156,  3182
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
  "INITIALIZE_TYPE", "EXIT_ID", "SHUFFLE_ID", "QUANTITY_ID", "BURIED_ID",
  "LOOP_ID", "FOR_ID", "TO_ID", "SWITCH_ID", "CASE_ID", "BREAK_ID",
  "DEFAULT_ID", "ERODED_ID", "TRAPPED_STATE", "RECHARGED_ID", "INVIS_ID",
  "GREASED_ID", "INDESTRUCTIBLE_ID", "FEMALE_ID", "MALE_ID", "WAITFORU_ID",
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
     455,   456,   457,   458,    44,    58,    40,    41,    91,    93,
     123,   125,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,    43,    45,
      42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   267,   268,   268,   269,   269,   270,   271,   271,   272,
     272,   272,   272,   273,   274,   274,   275,   276,   277,   277,
     278,   278,   279,   279,   280,   280,   281,   281,   282,   282,
     283,   283,   284,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   288,   288,   288,   289,   289,   290,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   292,   292,   293,   293,   294,   294,
     295,   295,   296,   296,   297,   297,   298,   298,   300,   301,
     299,   302,   303,   304,   304,   305,   305,   305,   307,   308,
     306,   309,   309,   311,   310,   312,   310,   313,   314,   314,
     315,   317,   316,   319,   318,   321,   320,   323,   322,   324,
     325,   324,   326,   327,   327,   327,   328,   328,   329,   330,
     332,   331,   334,   333,   335,   335,   336,   336,   337,   337,
     338,   338,   339,   339,   340,   340,   341,   341,   342,   342,
     343,   343,   344,   344,   345,   345,   346,   346,   346,   346,
     346,   346,   346,   347,   347,   347,   348,   348,   349,   349,
     350,   351,   350,   352,   353,   353,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   355,   355,   355,   356,   357,
     356,   358,   359,   359,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   361,   362,   363,   363,   364,   364,   364,   365,
     366,   367,   368,   369,   370,   371,   371,   372,   373,   374,
     374,   375,   376,   376,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   378,   378,   378,
     378,   379,   380,   381,   381,   382,   383,   383,   384,   385,
     386,   386,   387,   388,   389,   391,   390,   392,   392,   393,
     393,   393,   393,   394,   394,   395,   396,   396,   397,   397,
     398,   398,   398,   399,   399,   400,   400,   400,   401,   401,
     401,   402,   403,   404,   404,   405,   405,   406,   406,   407,
     407,   408,   408,   409,   409,   409,   410,   410,   411,   412,
     412,   413,   414,   414,   415,   415,   416,   416,   417,   417,
     417,   418,   418,   418,   419,   419,   420,   421,   421,   421,
     422,   423,   423,   423,   423,   424,   424,   424,   425,   425,
     426,   426,   426,   427,   428,   428,   428,   429,   429,   430,
     430,   430,   431,   431,   431,   431,   432,   432,   432,   433,
     433,   433,   433,   433,   434,   434,   435,   435,   435,   435,
     435,   435,   435,   435,   435,   435,   435,   436,   436,   437,
     438,   438,   439,   439,   440,   440,   441,   441,   442,   442,
     443,   443,   443,   443,   443,   443,   443,   443,   443,   443,
     443,   443,   443,   443,   443,   443,   443,   443,   443,   444,
     444,   445,   446,   446,   447,   447,   447,   448,   448,   448,
     448,   449,   449,   450,   450,   451,   451,   452,   452,   453,
     453,   454
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
       3,     3,     3,     3,     3,     3,     1,     1,     1,     3,
       3,     3,     5,     7,     5,     8,     1,     3,     3,     5,
       5,     7,     7,     6,     5,     0,     2,     5,     3,    11,
      13,     6,     0,     3,     1,     3,     3,     3,     3,     1,
       1,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     3,     3,     1,     5,     9,     5,     7,     3,     3,
       5,     9,     5,     5,     5,     0,    13,     0,     1,     7,
      11,     9,    13,     3,     7,     7,     5,     7,     5,     9,
       5,     5,     3,     7,     3,     7,     7,     3,     5,     5,
       3,     5,     7,     9,     1,     1,     1,     1,     1,     0,
       2,     1,     3,     1,     1,     1,     0,     3,     1,     0,
       3,     1,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     4,     1,     1,     4,
       1,     1,     4,     1,     4,     5,     1,     3,     1,     3,
       1,     1,     4,     9,     1,     1,     4,     1,     5,     1,
       1,     4,     1,     1,     5,     1,     1,     1,     4,     1,
       1,     5,     1,     1,     1,     3,     1,     1,     3,     1,
       4,     3,     3,     3,     3,     3,     3,     1,     1,     3,
       1,     3,     0,     1,     1,     1,     1,     3,     0,     1,
       1,     2,     2,     4,     6,     4,     6,     6,     6,     6,
       2,     6,     8,     8,    10,    14,     2,     1,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
      10,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    26,     0,     0,     1,
       5,     0,    30,     0,     7,     0,   155,     0,     0,     0,
       0,   223,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   296,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   374,     0,
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
       0,   446,     0,   449,     0,   491,     0,   447,   468,    31,
       0,   175,     0,     8,     0,   407,   408,     0,   444,   182,
       0,     0,     0,    11,   493,   492,    13,   416,     0,   226,
     227,     0,     0,   413,     0,     0,   194,   411,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   487,   470,   489,     0,   440,   442,   443,     0,   439,
     437,     0,   258,   262,   436,   259,   433,   435,     0,   432,
     430,     0,   230,     0,   429,   376,   375,     0,   394,   395,
       0,     0,     0,     0,   298,   297,     0,   421,     0,     0,
     420,     0,     0,     0,     0,   509,     0,     0,   353,     0,
       0,     0,     0,     0,   338,   339,   378,   377,     0,   153,
       0,     0,     0,     0,   410,     0,     0,     0,     0,     0,
     308,     0,   332,   331,   496,   494,   495,   185,   184,     0,
       0,     0,     0,   206,   207,     0,     0,     0,     0,    15,
     118,     0,     0,     0,   362,   364,   367,   370,   148,     0,
       0,     0,     0,   157,     0,     0,     0,     0,     0,   465,
     464,   466,   469,     0,   502,   504,   501,   503,   505,   506,
       0,     0,     0,   125,   126,   121,   119,     0,     0,     0,
       0,    30,   172,    28,     0,     0,     0,     0,     0,   418,
       0,     0,     0,     0,     0,     0,     0,   471,   472,     0,
       0,     0,   480,     0,     0,     0,   486,     0,     0,     0,
       0,     0,     0,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     154,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   179,   178,     0,
       0,     0,   173,     0,     0,     0,     0,     0,     0,   462,
     448,   456,     0,     0,   451,   452,   453,   454,   455,     0,
     151,     0,   446,     0,     0,     0,     0,   142,   140,   146,
     144,     0,     0,     0,   176,     0,     0,   445,    10,   333,
       0,     9,     0,     0,   417,     0,     0,     0,   229,   228,
     194,   195,   225,     0,     0,   210,     0,     0,     0,     0,
     427,     0,     0,   425,     0,     0,   424,     0,     0,   488,
     490,   344,     0,     0,     0,   260,     0,     0,   232,   234,
     292,   214,     0,    16,   136,   294,     0,     0,   396,   397,
       0,   342,     0,     0,   343,   340,   400,     0,   398,     0,
     399,     0,   358,   299,     0,   300,     0,   197,     0,     0,
       0,     0,   305,   304,     0,     0,   186,   187,   371,   507,
     508,     0,   307,     0,     0,   312,     0,   199,     0,     0,
     356,     0,     0,     0,     0,   336,     0,    14,     0,   169,
       0,     0,   159,   361,   360,     0,     0,   368,   369,     0,
     460,   463,     0,   450,   156,   467,   120,     0,     0,   129,
       0,   128,     0,   127,     0,   133,     0,   124,     0,   123,
       0,   122,    32,   409,     0,     0,   419,   412,     0,   414,
       0,   473,     0,     0,     0,   475,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   438,   499,   497,   498,     0,
     273,   270,   264,     0,   288,     0,     0,     0,   287,     0,
       0,     0,     0,     0,     0,     0,   269,     0,   274,     0,
     276,   277,   286,     0,   272,   263,   278,   500,   266,     0,
     431,   233,   205,     0,     0,     0,     0,   422,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     189,     0,     0,   303,     0,     0,     0,     0,     0,   311,
       0,     0,     0,     0,     0,     0,     0,     0,   174,   168,
     170,     0,     0,     0,     0,     0,   149,     0,   141,   143,
     145,   147,     0,   134,     0,     0,   138,     0,     0,   415,
     224,     0,   211,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   441,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,     0,
       0,   293,   137,    22,     0,   379,     0,     0,     0,     0,
     405,   404,   349,   354,     0,   301,     0,   201,     0,     0,
     302,   306,     0,     0,   372,     0,     0,     0,   355,     0,
     203,     0,   379,   357,   209,     0,   208,   181,   337,     0,
     161,   363,   366,   365,   457,   458,   459,   461,     0,     0,
     132,   131,     0,   130,     0,     0,   474,   476,     0,   481,
       0,   477,     0,   426,   479,   478,     0,   291,   285,   280,
     279,   281,   289,   290,   282,   283,   284,   267,   268,   271,
     275,   265,     0,   401,   237,   238,     0,   243,   241,   242,
     253,   254,   244,   245,   246,     0,     0,     0,   250,   251,
       0,   235,   239,   402,   236,     0,   220,   221,     0,   222,
       0,   219,   215,     0,   295,     0,     0,   386,     0,     0,
       0,   406,     0,     0,     0,     0,     0,     0,   212,   213,
       0,     0,     0,   314,     0,     0,     0,     0,     0,     0,
       0,   319,   320,     0,     0,     0,   313,     0,     0,   386,
       0,     0,     0,     0,     0,   161,   150,   135,   139,   334,
       0,     0,     0,   428,     0,     0,   240,     0,     0,     0,
       0,     0,     0,     0,    23,     0,   383,   384,   385,   380,
     381,     0,   389,     0,     0,   341,   351,     0,   359,   196,
       0,   379,   373,   188,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   198,     0,   389,   214,
     335,     0,   165,   160,   162,     0,   482,     0,   483,   452,
     403,   247,   248,   249,   256,   255,   252,   217,   218,   216,
       0,     0,     0,     0,   392,     0,     0,     0,     0,     0,
     386,     0,     0,   391,   324,   326,   327,   328,   330,   329,
     323,   315,   316,   317,   318,   321,   322,   325,     0,   392,
     204,   163,    30,     0,     0,     0,     0,     0,   423,   382,
     388,   387,     0,     0,   345,     0,   511,     0,   350,   200,
     389,     0,   309,   202,   190,    30,   166,     0,   484,    19,
       0,   257,   390,     0,   347,   510,     0,   392,     0,     0,
     164,     0,     0,   393,   348,   346,   352,   192,   310,   191,
       0,    20,     0,    24,    25,    22,     0,     0,   193,    12,
      21,   485
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    99,   100,   101,   102,   103,
    1076,  1127,   924,  1125,    12,   170,   104,   412,   105,   106,
     107,   108,   224,   109,   110,   772,   573,   775,   513,   514,
     515,   516,   111,   499,   858,   112,   113,   471,   114,   115,
     225,   761,   964,   965,  1095,  1072,   116,   631,   117,   118,
     242,   119,   628,   120,   410,   121,   366,   488,   623,   122,
     123,   124,   346,   338,   125,  1109,   126,  1122,   426,   599,
     619,   829,   842,   127,   365,   845,   546,   940,   722,   922,
     128,   265,   540,   129,   447,   302,   721,   911,  1036,   130,
     444,   292,   443,   715,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   743,   141,   142,   143,   144,   749,
     956,  1060,   145,   146,   531,   147,   148,   149,   150,   151,
     152,   153,   154,   155,  1104,  1115,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   307,   339,
     927,   989,   990,   992,  1081,  1044,  1054,  1084,   310,   580,
     589,   912,   822,   590,   248,   343,   282,   267,   420,   319,
     320,   555,   556,   303,   304,   293,   294,   389,   344,   856,
     640,   641,   642,   391,   392,   393,   283,   438,   237,   256,
     358,   718,   407,   408,   409,   611,   324,   325
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -984
static const yytype_int16 yypact[] =
{
     408,  -115,   -93,   103,  -984,   408,    20,   -84,   -57,  -984,
    -984,    -6,   784,   -31,  -984,   101,  -984,    26,    45,    69,
      78,  -984,    82,    94,   104,   120,   141,   164,   174,   180,
     212,   215,   242,   249,   251,   259,   266,   272,   283,   284,
     291,   295,   297,   306,   307,   314,   329,   330,   332,   333,
     336,   337,   339,   340,   343,   347,   351,   358,   366,   369,
     391,   395,   396,    40,   399,   400,   403,  -984,   404,    28,
     709,  -984,  -984,   405,   406,   412,   427,    30,    36,    34,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,   784,  -984,  -984,    98,  -984,
    -984,  -984,  -984,  -984,   428,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,   584,   223,
    -984,  -118,   481,    58,   345,   494,   675,   419,   419,   236,
     -43,    16,   -32,   624,   -32,   499,   148,   148,   -26,   441,
     -32,   -32,   537,   -32,   238,   148,   148,   -21,   -26,   -26,
     -26,    36,   450,    36,   -32,   675,   675,   -32,   675,   675,
     117,   -32,   675,   -21,   675,    52,  -984,   675,   148,   521,
     747,    36,  -984,  -984,   397,   451,   -32,   -32,   -32,   -32,
     486,  -984,   123,  -984,   478,  -984,   248,  -984,    31,  -984,
     129,  -984,   493,  -984,   101,  -984,  -984,   496,  -984,   442,
     504,   505,   506,  -984,  -984,  -984,  -984,  -984,   516,  -984,
    -984,   518,   716,  -984,   517,   525,   526,  -984,  -984,   148,
     148,   -32,   -32,   528,   -32,   529,   531,   532,   675,   534,
     424,  -984,  -984,   456,   527,  -984,  -984,  -984,   736,  -984,
    -984,   535,  -984,  -984,  -984,  -984,  -984,  -984,   741,  -984,
    -984,   539,   538,   541,  -984,  -984,  -984,   550,  -984,  -984,
     553,   554,   556,   560,  -984,  -984,   761,  -984,   558,   583,
    -984,   585,   582,   793,   595,  -984,   596,   610,   611,   612,
     620,   818,   621,   622,  -984,  -984,  -984,  -984,   632,   836,
     645,   646,   647,   650,   382,   858,   664,   423,   679,   680,
    -984,   681,  -984,  -984,  -984,  -984,  -984,  -984,  -984,   682,
     683,   684,   685,  -984,  -984,   686,   493,   687,   688,   689,
    -984,   692,    36,    36,   690,   691,   693,   694,  -984,   697,
     376,    36,    36,  -984,    36,    36,    36,    36,    36,   442,
     382,  -984,   702,   700,  -984,  -984,  -984,  -984,  -984,  -984,
     705,   126,    11,  -984,  -984,   442,   382,   710,   711,   712,
     784,   784,  -984,  -984,    36,  -118,   893,    19,   911,   714,
     719,   675,   717,    36,   346,   915,   720,  -984,  -984,   721,
     731,   474,  -984,   -32,   -32,   209,  -984,   740,   730,   675,
     848,   743,    36,   744,   493,   745,    36,   493,   -32,   -32,
     675,   866,   204,   871,   748,    36,    24,   913,   950,   751,
     920,   919,    99,   791,   -32,   872,   758,   881,   -26,   -44,
    -984,   768,   -26,   -26,   -26,    36,   770,    60,   -32,    43,
     883,    -5,   805,   885,    -8,    57,    16,   854,  -984,   132,
     132,   845,  -984,   222,   777,   -22,   855,   856,   -14,   827,
    -984,  -984,   417,   491,   152,   152,  -984,  -984,  -984,    31,
    -984,   675,   787,  -109,   -68,     9,   151,  -984,  -984,   442,
     382,   243,    39,   111,  -984,   785,   509,  -984,  -984,  -984,
     985,  -984,   796,   516,  -984,   788,   997,   514,  -984,  -984,
     526,  -984,  -984,   -32,   -32,   737,   799,   797,   819,   820,
    -984,   821,   370,  -984,   835,   823,  -984,   824,   840,  -984,
    -984,  -984,   833,   519,    35,  -984,   834,   524,  -984,  -984,
    -984,  -984,   843,   862,  -984,   863,  1064,   533,  -984,  -984,
     865,  -984,   867,  1066,  -984,   868,  -984,   869,  -984,   870,
    -984,   873,   875,  -984,  1069,  -984,   876,  -984,  1071,   877,
      24,   878,   879,  -984,   880,  1006,  -984,  -984,  -984,  -984,
    -984,   884,  -984,   886,   882,  -984,   887,  -984,  1072,   888,
    -984,   889,   890,   963,  1092,   892,   895,  -984,   493,  -984,
     837,    36,  -984,  -984,   442,   896,   898,  -984,  -984,   899,
    -984,   901,   900,  -984,  -984,  -984,  -984,  1099,   902,  -984,
     -29,  -984,    36,  -984,  -118,  -984,   114,  -984,   204,  -984,
      22,  -984,  -984,  -984,   905,  1103,  -984,  -984,   903,  -984,
     904,  -984,   907,  1033,   675,  -984,    36,    36,   675,   909,
      36,   675,   675,   908,   912,  -984,  -984,  -984,  -984,   910,
    -984,  -984,  -984,   916,  -984,   917,   918,   921,  -984,   922,
     923,   924,   925,   926,   927,   928,  -984,   929,  -984,   930,
    -984,  -984,  -984,   931,  -984,  -984,  -984,  -984,  -984,   932,
    -984,   914,   933,    16,   204,  1111,   934,  -984,   -21,  1116,
     936,   955,  1121,   184,   236,   972,    -4,  1053,   939,     0,
    -984,   935,  1055,  -984,    36,   941,  -118,  1080,   -32,   944,
    1054,   945,     5,  1056,   359,   493,  1057,   132,  -984,  -984,
     382,   946,    24,     3,   241,   827,  -984,   186,  -984,  -984,
     382,   442,   156,  -984,   171,   183,  -984,    24,   947,  -984,
    -984,    36,  -984,   948,   256,   454,   951,    24,   618,   952,
     953,    36,  -984,    36,   419,  1052,  1060,    36,  1049,  1058,
      36,    36,    36,  -118,    36,    36,    36,   236,  -984,   874,
     289,  -984,  -984,   958,  1159,   960,   961,  1162,   964,   959,
    -984,  -984,   966,  -984,   967,  -984,  1167,  -984,   377,   969,
    -984,  -984,   970,    61,   442,   971,   973,   355,  -984,  1172,
    -984,  1174,   960,  -984,  -984,   975,  -984,  -984,  -984,   976,
     188,  -984,  -984,   442,  -984,  -984,  -984,  -984,   493,   114,
    -984,  -984,    22,  -984,   974,  1177,   382,  -984,  1142,  -984,
      36,  -984,   977,  -984,  -984,  -984,   598,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,   442,  -984,  -984,
    -984,  -984,   125,  -984,  -984,  -984,  -118,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,   980,   981,   982,  -984,  -984,
     983,  -984,  -984,  -984,   442,   984,  -984,  -984,   986,  -984,
     987,  -984,  -984,  1187,  -984,   989,   480,  1094,  1191,   992,
     236,  -984,   -25,   236,   990,   994,     5,    36,  -984,  -984,
     993,  1132,  1110,  -984,   998,   999,  1000,  1001,  1002,  1003,
    1004,  -984,  -984,  1005,  1007,  1008,  -984,   995,  1010,  1094,
      61,  1204,   380,  1011,  1009,   188,  -984,  -984,  -984,  -984,
    1013,  1012,   388,  -984,    36,  1135,   442,    36,    36,    36,
    -102,   419,  1214,  1090,  -984,  1218,  -984,  -984,  -984,  -984,
    1019,  1020,  1127,  1022,  1223,  -984,  1024,  1025,  -984,  -984,
     346,   960,  -984,  -984,  1027,  1028,  1133,  1229,    74,   236,
     419,    19,    19,   -32,   -43,  1230,  -984,  1231,  1127,  -984,
    -984,  1031,  -984,  -984,  -984,  1232,  -984,  1197,  -984,    17,
    -984,  -984,  -984,  -984,  -984,   978,  -984,  -984,  -984,  -984,
    1032,   480,  1141,  1036,  1073,  1240,  1038,  1077,   236,  1040,
    1094,  1157,  1160,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  1042,  1073,
     933,  -984,   784,  1047,  1045,  1051,  1048,  -102,  -984,  -984,
    -984,  -984,  1133,  1059,  -984,  1050,  -984,  1061,  -984,  -984,
    1127,  1063,  -984,  -984,  -984,   784,  -984,    24,  -984,  -984,
    1065,  -984,  -984,   236,   493,  -984,   236,  1073,  1164,   493,
    -984,  1067,   -32,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
      70,  1068,   493,  -984,  -984,   958,   -32,  1070,  -984,  -984,
    -984,  -984
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -984,  -984,  1251,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,   133,  -984,  -984,  1015,  -105,  -363,   850,  1041,
    1192,  -483,  -984,  -984,  -984,  -984,   605,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  1202,  -984,
    -984,  -984,   303,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,   798,  1074,  -984,  -984,  -984,  -984,   733,  -984,
    -984,  -984,   334,  -984,  -984,  -984,  -584,   316,   255,  -984,
    -984,   452,   278,  -984,  -984,  -984,  -984,  -984,   202,  -984,
    -984,  1104,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -538,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,   267,   555,
    -809,   244,  -984,  -872,  -984,  -918,   206,  -983,  -454,  -594,
    -984,  -984,  -984,   475,   891,  -216,  -173,  -383,   753,   280,
    -378,  -476,  -615,  -512,  -444,  -509,  -630,  -164,   -74,  -984,
     530,  -984,  -984,   780,  -984,  -984,  1014,  -135,   726,  -984,
    -475,  -984,  -984,  -984,  -984,  -984,  -171,  -984
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -232
static const yytype_int16 yytable[] =
{
     239,   266,   607,   487,   236,   371,   740,   249,   574,   311,
     659,   313,   657,   625,   626,   231,   639,   327,   328,   517,
     330,   332,   529,   333,   518,   550,   773,   340,   341,   342,
     322,   348,   622,   959,   351,   231,   597,   305,   359,   686,
     231,   284,   296,   687,   688,   776,    16,   661,   257,   258,
     315,   257,   258,   374,   375,   376,   377,   363,   633,   336,
     613,   620,   254,   996,   354,   938,  1034,   403,   355,   356,
     349,   350,   617,   352,   353,  1123,   405,   360,  1056,   362,
     827,   565,   367,   852,   568,   840,  1094,  1018,   308,   782,
       7,   257,   258,   689,   245,   648,   309,   578,   429,   430,
    1069,   432,   649,     9,   579,   246,   247,   690,   691,   692,
    1035,   693,     8,   694,   550,   257,   258,   285,   609,   297,
     637,   354,   638,   610,  1117,   355,   356,   231,    13,   347,
     512,   379,   364,   231,   379,   550,   650,    11,   695,   696,
     697,   939,   698,   651,   997,   699,   700,   701,   702,   703,
    1124,   704,   394,   255,   395,    14,   621,   494,   380,   396,
     397,   398,   598,   261,   390,  1057,   406,  1058,   851,   306,
     846,   614,  1107,   168,   262,   705,   706,   262,  1090,   586,
     323,   587,   588,   864,   707,   708,   709,   710,   711,   712,
     245,   337,  1050,   872,   286,   263,   264,   357,   618,    15,
     817,   246,   247,   779,   713,  1059,   828,   296,   287,   257,
     258,   841,   550,   652,   574,   245,   551,   401,   714,   169,
     653,  1075,   823,   245,   -18,   530,   246,   247,   624,   967,
     261,   171,   233,   234,   246,   247,   221,   232,   519,   296,
     238,   262,   232,   245,   230,   298,   285,   968,    78,   658,
     172,   299,   233,   234,   246,   247,   399,   233,   234,   604,
     548,   549,   263,   264,   820,   758,   345,   769,   235,   811,
     768,   821,   300,   301,   173,   569,   570,   386,   387,   388,
     812,   849,   639,   174,   297,   878,   535,   175,   235,   257,
     258,   592,   235,   235,   322,   891,   547,   596,   493,   176,
     557,   601,   602,   603,   560,   608,   525,   502,   503,   177,
     504,   505,   506,   507,   508,   571,   297,   624,   257,   258,
     288,   660,   400,   286,   261,   178,   289,   380,   520,   232,
     975,   634,   232,   732,   962,   401,   963,   287,   624,   402,
     526,   245,   553,   554,   233,   234,   179,   233,   234,   537,
     233,   234,   246,   247,   316,   654,   263,   264,   317,   318,
     859,   240,   655,   553,   554,   915,   629,   860,   563,   180,
     671,   672,   567,   679,   422,   724,   646,   317,   318,   181,
     235,   577,   861,   235,   354,   182,   235,   862,   355,   356,
     814,   716,   847,   779,   863,   916,   917,   269,   270,   271,
     272,   273,   918,   274,   261,   275,   276,   277,   278,   279,
     298,   919,   386,   387,   388,   552,   299,   183,   995,   920,
     184,   998,   285,     1,     2,   257,   258,   244,   422,   259,
     260,   538,   539,   261,   382,   390,   263,   264,   545,   844,
     553,   554,   298,   921,   331,   943,   281,   185,   299,   288,
     257,   258,   944,   656,   186,   289,   187,   383,   854,   855,
     868,   259,   260,   869,   188,   263,   264,   321,   945,   300,
     301,   189,  1037,  1063,  1064,   334,   335,   190,   290,   291,
     384,   385,   386,   387,   388,   946,   630,  1021,   191,   192,
     771,   250,   251,   252,   253,   966,   193,  1061,   368,   286,
     194,  1062,   195,  1111,   257,   258,   384,   385,   386,   387,
     388,   196,   197,   287,   384,   385,   386,   387,   388,   198,
     404,   986,   987,   988,   947,   948,   949,   950,   832,   951,
     952,   953,   954,   955,   199,   200,  1088,   201,   202,   783,
     261,   203,   204,   786,   205,   206,   789,   790,   207,   427,
     428,   262,   208,   545,   257,   258,   209,   760,   269,   270,
     271,   272,   273,   210,   274,   261,   275,   276,   277,   278,
     279,   211,   263,   264,   212,   836,   280,   877,   770,   257,
     258,   881,   834,   501,   884,   885,   886,   243,   888,   889,
     890,  1113,  1027,   268,  1116,  1028,   213,   263,   264,   853,
     214,   215,   784,   785,   217,   218,   788,   281,   219,   220,
     226,   227,   269,   270,   271,   272,   273,   228,   274,   261,
     275,   276,   277,   278,   279,   288,   643,   478,   312,   329,
     280,   289,   229,   241,   384,   385,   386,   387,   388,   887,
     384,   385,   386,   387,   388,   914,   384,   385,   386,   387,
     388,   263,   264,   326,   290,   291,   345,   369,   870,   373,
     372,   281,   269,   270,   271,   272,   273,   314,   274,   261,
     275,   276,   277,   278,   279,   384,   385,   386,   387,   388,
     280,   384,   385,   386,   387,   388,   381,   269,   270,   271,
     272,   273,   378,   274,   261,   275,   276,   277,   278,   279,
     644,   263,   264,   411,   414,   280,   415,   866,   416,   417,
     418,   281,   384,   385,   386,   387,   388,   876,   663,   419,
     422,  1002,   439,   669,   421,   423,   263,   264,   685,   424,
     425,   440,   976,   720,   431,   433,   281,   434,   435,   441,
     437,  1114,   727,   442,   445,   448,  1119,   446,  -231,   384,
     385,   386,   387,   388,   449,   257,   258,   450,   451,  1128,
     452,  1031,  1032,  1033,   453,   454,   455,   384,   385,   386,
     387,   388,   384,   385,   386,   387,   388,   384,   385,   386,
     387,   388,   384,   385,   386,   387,   388,   456,   458,   457,
      16,   384,   385,   386,   387,   388,   972,   459,    17,   460,
     461,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,   462,   463,   464,    31,    32,    33,
      34,    35,   466,    36,   465,   467,   468,   873,    37,    38,
      39,    40,    41,    42,    43,    44,   469,    45,    46,    47,
    1065,    48,   470,    49,    50,    51,    52,    53,    54,   472,
     473,   474,    55,    56,   475,    57,   384,   974,   386,   387,
     388,    58,   476,   269,   270,   271,   272,   273,   477,   274,
     261,   275,   276,   277,   278,   279,   384,   385,   386,   387,
     388,   280,    59,   479,   480,   481,   482,   483,   484,   485,
     486,   489,   490,   491,   495,   496,   528,   497,   498,    60,
    1029,   492,   263,   264,   500,    61,   509,   510,    62,    63,
     511,    64,   281,    65,   532,   521,   522,   523,   533,    66,
     541,   536,    67,    68,   222,   543,    69,    70,   534,    71,
      81,    72,    83,   542,    85,   544,    87,   559,    89,  1121,
      91,   558,    93,   561,    95,   572,    97,   562,   564,   566,
     575,   581,   576,  1130,   582,   583,   892,   893,   584,   585,
     591,   593,   594,    73,    74,    75,    76,  1096,    77,    82,
     595,    84,   600,    86,   605,    88,   612,    90,   615,    92,
    -180,    94,   627,    96,   616,    98,   632,   635,   664,   636,
    1110,   647,    78,   894,   895,   667,   662,   896,    79,    80,
     665,   668,   673,   674,   675,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,   676,   677,   678,   897,   681,   682,   898,
     899,   900,   901,   902,   903,   904,   905,   906,   907,   908,
     909,   910,    80,   680,   683,   684,   719,   723,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,   724,   725,   726,   728,
     730,   729,   731,   736,   733,   738,   751,   732,   734,   735,
     737,   739,   741,   742,   744,   745,   245,   748,   746,   755,
     747,   750,   752,   753,   754,   679,   756,   246,   247,   757,
     762,   759,   763,   767,   764,   765,   778,   766,   316,   777,
     779,   781,   545,   787,   791,   793,   813,   780,   809,   792,
     816,   794,   795,   796,   818,   819,   797,   798,   799,   800,
     801,   802,   803,   804,   805,   806,   807,   810,   814,   808,
     817,   824,   825,   826,   831,   833,   835,   830,   837,   839,
     838,   865,   843,   848,   879,   867,   850,   882,   871,   874,
     875,   880,   923,   925,   926,   928,   929,   883,   931,   930,
     932,   934,   933,   936,   937,   941,   957,   942,   958,   960,
     961,   969,   970,   971,   973,   977,   978,   979,   980,   981,
     984,   982,   983,   985,   991,   993,   994,   999,  1000,  1004,
    1003,  1005,  1016,  1006,  1007,  1008,  1009,  1010,  1011,  1012,
    1013,  1020,  1014,  1015,  1017,  1030,  1022,  1025,  1038,  1026,
    1023,  1039,  1040,  1041,  1043,  1042,  1045,  1046,  1047,  1053,
    1048,  1051,  1052,  1055,  1067,  1068,  1071,  1073,  1074,  1078,
    1080,  1082,  1083,  1077,  1085,  1086,  1087,  1089,  1091,  1093,
    1092,  1097,  1098,  1099,  1118,  1100,    10,  1105,  1129,   413,
     524,   370,   223,   774,  1103,   216,  1106,  1108,  1024,  1112,
    1001,  1120,  1126,   670,  1070,   606,  1019,  1131,  1049,  1101,
     935,  1066,   295,   815,   913,  1079,   666,   361,  1102,   645,
     717,     0,   436,     0,     0,   857,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   527
};

static const yytype_int16 yycheck[] =
{
     105,   174,   477,   366,    78,   221,   600,   171,   452,   182,
     522,   184,   521,   489,   490,     4,   499,   190,   191,   402,
     193,   194,     3,   194,   402,     3,   656,   198,   199,   200,
      56,   204,   486,   842,   207,     4,    80,    80,   211,     4,
       4,   176,     3,     8,     9,   660,     6,   523,    80,    81,
     185,    80,    81,   226,   227,   228,   229,     5,    80,    80,
      65,     4,     4,    88,     4,     4,   168,   240,     8,     9,
     205,   206,    80,   208,   209,     5,   240,   212,     4,   214,
      80,   444,   217,    80,   447,    80,  1069,   959,    72,   673,
     205,    80,    81,    58,   212,   204,    80,    73,   271,   272,
    1018,   274,   211,     0,    80,   223,   224,    72,    73,    74,
     212,    76,   205,    78,     3,    80,    81,     3,    75,    80,
     134,     4,   136,    80,  1107,     8,     9,     4,   212,   203,
       4,     8,    80,     4,     8,     3,   204,   117,   103,   104,
     105,    80,   107,   211,   169,   110,   111,   112,   113,   114,
      80,   116,    23,    95,    25,   212,    99,   373,   232,    30,
      31,    32,   206,   195,   238,    91,   240,    93,   762,   212,
     754,   176,  1090,   204,   206,   140,   141,   206,  1050,    80,
     206,    82,    83,   777,   149,   150,   151,   152,   153,   154,
     212,   212,  1001,   787,    80,   227,   228,    80,   206,   205,
     204,   223,   224,   207,   169,   131,   206,     3,    94,    80,
      81,   206,     3,   204,   658,   212,     7,   206,   183,   118,
     211,   204,   734,   212,   207,   206,   223,   224,   206,   859,
     195,   205,   221,   222,   223,   224,   208,   206,   402,     3,
     206,   206,   206,   212,   214,   206,     3,   862,   208,   210,
     205,   212,   221,   222,   223,   224,   127,   221,   222,   475,
     433,   434,   227,   228,    80,   628,   206,   650,   257,   723,
     648,    87,   233,   234,   205,   448,   449,   260,   261,   262,
     724,   757,   765,   205,    80,   794,   421,   205,   257,    80,
      81,   464,   257,   257,    56,   807,   431,   468,   372,   205,
     435,   472,   473,   474,   439,   478,   411,   381,   382,   205,
     384,   385,   386,   387,   388,   450,    80,   206,    80,    81,
     206,   210,   193,    80,   195,   205,   212,   401,   402,   206,
     205,   495,   206,   208,   146,   206,   148,    94,   206,   210,
     414,   212,   231,   232,   221,   222,   205,   221,   222,   423,
     221,   222,   223,   224,   206,   204,   227,   228,   229,   230,
     204,   263,   211,   231,   232,    76,   144,   211,   442,   205,
     543,   544,   446,     3,     4,   204,   511,   229,   230,   205,
     257,   455,   211,   257,     4,   205,   257,   204,     8,     9,
     204,   564,   755,   207,   211,   106,   107,   188,   189,   190,
     191,   192,   113,   194,   195,   196,   197,   198,   199,   200,
     206,   122,   260,   261,   262,   206,   212,   205,   930,   130,
     205,   933,     3,    15,    16,    80,    81,   204,     4,    84,
      85,    85,    86,   195,   186,   509,   227,   228,    79,    80,
     231,   232,   206,   154,   206,    90,   237,   205,   212,   206,
      80,    81,    97,   210,   205,   212,   205,   209,   217,   218,
     204,    84,    85,   207,   205,   227,   228,   187,   113,   233,
     234,   205,   981,  1011,  1012,   195,   196,   205,   235,   236,
     258,   259,   260,   261,   262,   130,   264,   962,   205,   205,
     654,    10,    11,    12,    13,   858,   205,  1009,   218,    80,
     205,  1010,   205,  1097,    80,    81,   258,   259,   260,   261,
     262,   205,   205,    94,   258,   259,   260,   261,   262,   205,
     240,    41,    42,    43,   169,   170,   171,   172,   744,   174,
     175,   176,   177,   178,   205,   205,  1048,   205,   205,   674,
     195,   205,   205,   678,   205,   205,   681,   682,   205,   269,
     270,   206,   205,    79,    80,    81,   205,   631,   188,   189,
     190,   191,   192,   205,   194,   195,   196,   197,   198,   199,
     200,   205,   227,   228,   205,   748,   206,   793,   652,    80,
      81,   797,   746,   207,   800,   801,   802,     3,   804,   805,
     806,  1103,   204,    99,  1106,   207,   205,   227,   228,   763,
     205,   205,   676,   677,   205,   205,   680,   237,   205,   205,
     205,   205,   188,   189,   190,   191,   192,   205,   194,   195,
     196,   197,   198,   199,   200,   206,   209,   204,     4,    92,
     206,   212,   205,   205,   258,   259,   260,   261,   262,   803,
     258,   259,   260,   261,   262,   809,   258,   259,   260,   261,
     262,   227,   228,   212,   235,   236,   206,   136,   204,   208,
     263,   237,   188,   189,   190,   191,   192,   168,   194,   195,
     196,   197,   198,   199,   200,   258,   259,   260,   261,   262,
     206,   258,   259,   260,   261,   262,   208,   188,   189,   190,
     191,   192,   206,   194,   195,   196,   197,   198,   199,   200,
     209,   227,   228,   210,   208,   206,   264,   781,   204,   204,
     204,   237,   258,   259,   260,   261,   262,   791,   209,   203,
       4,   937,   266,   209,   206,   208,   227,   228,   209,   204,
     204,   204,   896,   209,   206,   206,   237,   206,   206,     3,
     206,  1104,   209,   208,     3,   204,  1109,   208,   210,   258,
     259,   260,   261,   262,   204,    80,    81,   204,   204,  1122,
     204,   977,   978,   979,   204,     4,   208,   258,   259,   260,
     261,   262,   258,   259,   260,   261,   262,   258,   259,   260,
     261,   262,   258,   259,   260,   261,   262,   204,   206,   204,
       6,   258,   259,   260,   261,   262,   870,     4,    14,   204,
     204,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,   204,   204,   204,    33,    34,    35,
      36,    37,     4,    39,   204,   204,   204,   209,    44,    45,
      46,    47,    48,    49,    50,    51,   204,    53,    54,    55,
    1013,    57,     6,    59,    60,    61,    62,    63,    64,   204,
     204,   204,    68,    69,   204,    71,   258,   259,   260,   261,
     262,    77,     4,   188,   189,   190,   191,   192,   204,   194,
     195,   196,   197,   198,   199,   200,   258,   259,   260,   261,
     262,   206,    98,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,     3,   204,   204,   115,
     974,   209,   227,   228,   207,   121,   204,   207,   124,   125,
     205,   127,   237,   129,     3,   205,   205,   205,   204,   135,
       5,   204,   138,   139,   215,   204,   142,   143,   209,   145,
     221,   147,   223,   213,   225,   204,   227,   207,   229,  1112,
     231,   201,   233,    95,   235,    79,   237,   204,   204,   204,
      79,    38,   204,  1126,     4,   204,    82,    83,    38,    40,
     169,    89,   204,   179,   180,   181,   182,  1072,   184,   222,
      89,   224,   204,   226,   204,   228,    93,   230,   173,   232,
     126,   234,   137,   236,    99,   238,   209,   132,     3,   133,
    1095,   204,   208,   119,   120,   207,   211,   123,   214,   215,
     204,     4,   265,   204,   207,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   204,   204,   204,   152,   204,   204,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   215,   208,   204,   212,   212,   204,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   204,   204,     4,   204,
       4,   204,   204,     4,   204,     4,     4,   208,   205,   204,
     204,   204,   204,   204,   204,    79,   212,   205,   204,   126,
     204,   204,   204,   204,   204,     3,   204,   223,   224,   204,
     204,   264,   204,     4,   205,   204,     3,   207,   206,   204,
     207,   204,    79,   204,   206,   205,     5,   213,   204,   207,
       4,   205,   205,   205,   169,     4,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   204,   204,   207,
     204,   169,    89,   204,    89,   204,    66,   212,   204,   204,
      96,   204,    96,    96,   102,   207,   210,   108,   207,   207,
     207,   101,   204,     4,   204,   204,     4,   109,   209,   205,
     204,     4,   205,   204,   204,   204,     4,   204,     4,   204,
     204,   207,     5,    41,   207,   205,   205,   205,   205,   205,
       3,   205,   205,   204,   100,     4,   204,   207,   204,    67,
     207,    91,   207,   205,   205,   205,   205,   205,   205,   205,
     205,     7,   205,   205,   204,    80,   205,   204,     4,   207,
     211,   131,     4,   204,    97,   205,   204,     4,   204,    96,
     205,   204,   204,     4,     4,     4,   205,     5,    41,   207,
      99,   205,   169,   265,     4,   207,   169,   207,    91,   207,
      90,   204,   207,   202,    90,   207,     5,   207,  1125,   244,
     410,   220,    70,   658,   205,    63,   205,   204,   965,   204,
     936,   204,   204,   540,  1019,   477,   960,   207,  1000,  1077,
     828,  1014,   178,   728,   809,  1041,   533,   213,  1082,   509,
     564,    -1,   278,    -1,    -1,   765,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   415
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   268,   269,   270,   271,   205,   205,     0,
     269,   117,   281,   212,   212,   205,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    33,    34,    35,    36,    37,    39,    44,    45,    46,
      47,    48,    49,    50,    51,    53,    54,    55,    57,    59,
      60,    61,    62,    63,    64,    68,    69,    71,    77,    98,
     115,   121,   124,   125,   127,   129,   135,   138,   139,   142,
     143,   145,   147,   179,   180,   181,   182,   184,   208,   214,
     215,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   272,
     273,   274,   275,   276,   283,   285,   286,   287,   288,   290,
     291,   299,   302,   303,   305,   306,   313,   315,   316,   318,
     320,   322,   326,   327,   328,   331,   333,   340,   347,   350,
     356,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   372,   373,   374,   375,   379,   380,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   393,   394,   395,   396,
     397,   398,   399,   400,   401,   402,   403,   404,   204,   118,
     282,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   305,   205,   205,   205,
     205,   208,   215,   287,   289,   307,   205,   205,   205,   205,
     214,     4,   206,   221,   222,   257,   435,   445,   206,   283,
     263,   205,   317,     3,   204,   212,   223,   224,   421,   434,
      10,    11,    12,    13,     4,    95,   446,    80,    81,    84,
      85,   195,   206,   227,   228,   348,   423,   424,    99,   188,
     189,   190,   191,   192,   194,   196,   197,   198,   199,   200,
     206,   237,   423,   443,   444,     3,    80,    94,   206,   212,
     235,   236,   358,   432,   433,   358,     3,    80,   206,   212,
     233,   234,   352,   430,   431,    80,   212,   405,    72,    80,
     415,   423,     4,   423,   168,   444,   206,   229,   230,   426,
     427,   426,    56,   206,   453,   454,   212,   423,   423,    92,
     423,   206,   423,   453,   426,   426,    80,   212,   330,   406,
     453,   453,   453,   422,   435,   206,   329,   435,   423,   444,
     444,   423,   444,   444,     4,     8,     9,    80,   447,   423,
     444,   330,   444,     5,    80,   341,   323,   444,   426,   136,
     286,   422,   263,   208,   423,   423,   423,   423,   206,     8,
     435,   208,   186,   209,   258,   259,   260,   261,   262,   434,
     435,   440,   441,   442,    23,    25,    30,    31,    32,   127,
     193,   206,   210,   423,   426,   434,   435,   449,   450,   451,
     321,   210,   284,   282,   208,   264,   204,   204,   204,   203,
     425,   206,     4,   208,   204,   204,   335,   426,   426,   423,
     423,   206,   423,   206,   206,   206,   443,   206,   444,   266,
     204,     3,   208,   359,   357,     3,   208,   351,   204,   204,
     204,   204,   204,   204,     4,   208,   204,   204,   206,     4,
     204,   204,   204,   204,   204,   204,     4,   204,   204,   204,
       6,   304,   204,   204,   204,   204,     4,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   284,   324,   204,
     204,   204,   209,   435,   422,   204,   204,   204,   204,   300,
     207,   207,   435,   435,   435,   435,   435,   435,   435,   204,
     207,   205,     4,   295,   296,   297,   298,   424,   427,   434,
     435,   205,   205,   205,   285,   283,   435,   421,     3,     3,
     206,   381,     3,   204,   209,   444,   204,   435,    85,    86,
     349,     5,   213,   204,   204,    79,   343,   444,   423,   423,
       3,     7,   206,   231,   232,   428,   429,   444,   201,   207,
     444,    95,   204,   435,   204,   284,   204,   435,   284,   423,
     423,   444,    79,   293,   431,    79,   204,   435,    73,    80,
     416,    38,     4,   204,    38,    40,    80,    82,    83,   417,
     420,   169,   423,    89,   204,    89,   453,    80,   206,   336,
     204,   453,   453,   453,   422,   204,   329,   447,   423,    75,
      80,   452,    93,    65,   176,   173,    99,    80,   206,   337,
       4,    99,   415,   325,   206,   428,   428,   137,   319,   144,
     264,   314,   209,    80,   434,   132,   133,   134,   136,   288,
     437,   438,   439,   209,   209,   440,   444,   204,   204,   211,
     204,   211,   204,   211,   204,   211,   210,   432,   210,   430,
     210,   428,   211,   209,     3,   204,   425,   207,     4,   209,
     335,   423,   423,   265,   204,   207,   204,   204,   204,     3,
     208,   204,   204,   204,   212,   209,     4,     8,     9,    58,
      72,    73,    74,    76,    78,   103,   104,   105,   107,   110,
     111,   112,   113,   114,   116,   140,   141,   149,   150,   151,
     152,   153,   154,   169,   183,   360,   423,   445,   448,   212,
     209,   353,   345,   204,   204,   204,     4,   209,   204,   204,
       4,   204,   208,   204,   205,   204,     4,   204,     4,   204,
     416,   204,   204,   371,   204,    79,   204,   204,   205,   376,
     204,     4,   204,   204,   204,   126,   204,   204,   284,   264,
     435,   308,   204,   204,   205,   204,   207,     4,   427,   424,
     435,   434,   292,   433,   293,   294,   429,   204,     3,   207,
     213,   204,   343,   444,   435,   435,   444,   204,   435,   444,
     444,   206,   207,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   207,   204,
     204,   415,   431,     5,   204,   406,     4,   204,   169,     4,
      80,    87,   419,   430,   169,    89,   204,    80,   206,   338,
     212,    89,   422,   204,   434,    66,   423,   204,    96,   204,
      80,   206,   339,    96,    80,   342,   343,   284,    96,   428,
     210,   416,    80,   434,   217,   218,   436,   437,   301,   204,
     211,   211,   204,   211,   416,   204,   435,   207,   204,   207,
     204,   207,   416,   209,   207,   207,   435,   422,   432,   102,
     101,   422,   108,   109,   422,   422,   422,   434,   422,   422,
     422,   430,    82,    83,   119,   120,   123,   152,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   354,   418,   420,   434,    76,   106,   107,   113,   122,
     130,   154,   346,   204,   279,     4,   204,   407,   204,     4,
     205,   209,   204,   205,     4,   348,   204,   204,     4,    80,
     344,   204,   204,    90,    97,   113,   130,   169,   170,   171,
     172,   174,   175,   176,   177,   178,   377,     4,     4,   407,
     204,   204,   146,   148,   309,   310,   284,   433,   429,   207,
       5,    41,   435,   207,   259,   205,   434,   205,   205,   205,
     205,   205,   205,   205,     3,   204,    41,    42,    43,   408,
     409,   100,   410,     4,   204,   430,    88,   169,   430,   207,
     204,   339,   422,   207,    67,    91,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   207,   204,   410,   344,
       7,   447,   205,   211,   309,   204,   207,   204,   207,   435,
      80,   422,   422,   422,   168,   212,   355,   432,     4,   131,
       4,   204,   205,    97,   412,   204,     4,   204,   205,   349,
     407,   204,   204,    96,   413,     4,     4,    91,    93,   131,
     378,   430,   432,   381,   381,   423,   405,     4,     4,   412,
     345,   205,   312,     5,    41,   204,   277,   265,   207,   408,
      99,   411,   205,   169,   414,     4,   207,   169,   430,   207,
     410,    91,    90,   207,   414,   311,   283,   204,   207,   202,
     207,   355,   413,   205,   391,   207,   205,   412,   204,   332,
     283,   416,   204,   430,   284,   392,   430,   414,    90,   284,
     204,   423,   334,     5,    80,   280,   204,   278,   284,   279,
     423,   207
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
#line 293 "lev_comp.y"
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
#line 316 "lev_comp.y"
    {
		      start_level_def(&splev, (yyvsp[(3) - (3)].map));
		      (yyval.map) = (yyvsp[(3) - (3)].map);
		  ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 321 "lev_comp.y"
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
#line 343 "lev_comp.y"
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
#line 356 "lev_comp.y"
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
#line 368 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_ROGUE,0,0,0,
					  0,0,0,0, SPO_INITLEVEL));
		  ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 374 "lev_comp.y"
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
#line 403 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_TILESET));
		  ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 409 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), (int)(yyvsp[(3) - (5)].i), SPO_FOREST));
		  ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 413 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(0, (int)(yyvsp[(3) - (3)].i), SPO_FOREST));
		  ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 419 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), (int)(yyvsp[(3) - (5)].i), SPO_MONSTER_GENERATION));
		  ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 425 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_BOUNDARY_TYPE));
		  ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 431 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 435 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 441 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_COPY));
		      (yyval.i) = 0;
		  ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 446 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 452 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 456 "lev_comp.y"
    {
		      (yyval.i) = what_map_char((char) (yyvsp[(2) - (2)].i));
		  ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 467 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 471 "lev_comp.y"
    {
		      add_opvars(splev, "io",
                                 VA_PASS2((int) (yyvsp[(3) - (3)].i), SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 478 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 482 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 488 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 492 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 498 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (3)].i);
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 599 "lev_comp.y"
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
#line 615 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 621 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 627 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 633 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 639 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 645 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 651 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 657 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 663 "lev_comp.y"
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
#line 673 "lev_comp.y"
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
#line 683 "lev_comp.y"
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
#line 693 "lev_comp.y"
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
#line 703 "lev_comp.y"
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
#line 713 "lev_comp.y"
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
#line 723 "lev_comp.y"
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
#line 735 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 740 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 747 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 752 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 759 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 764 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 771 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 775 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 781 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 786 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 793 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 797 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 803 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 807 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 813 "lev_comp.y"
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
#line 835 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 839 "lev_comp.y"
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
#line 850 "lev_comp.y"
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
#line 905 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 911 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 915 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 921 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 928 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 932 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 940 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 944 "lev_comp.y"
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
#line 967 "lev_comp.y"
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
#line 1012 "lev_comp.y"
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
#line 1022 "lev_comp.y"
    {
		  ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1025 "lev_comp.y"
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
#line 1035 "lev_comp.y"
    {
		  ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1040 "lev_comp.y"
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
#line 1054 "lev_comp.y"
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
#line 1091 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1096 "lev_comp.y"
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
#line 1125 "lev_comp.y"
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
#line 1139 "lev_comp.y"
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
#line 1155 "lev_comp.y"
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
#line 1175 "lev_comp.y"
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
#line 1187 "lev_comp.y"
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
#line 1207 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1213 "lev_comp.y"
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
#line 1223 "lev_comp.y"
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
#line 1241 "lev_comp.y"
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
#line 1251 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1257 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1262 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1267 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1274 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1281 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1290 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1298 "lev_comp.y"
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
#line 1309 "lev_comp.y"
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
#line 1325 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1332 "lev_comp.y"
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
#line 1348 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1355 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1359 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1365 "lev_comp.y"
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
#line 1375 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1381 "lev_comp.y"
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
#line 1390 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1396 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1401 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1407 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1412 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1418 "lev_comp.y"
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
#line 1429 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1443 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1447 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1457 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1464 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1472 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1477 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1482 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1487 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1492 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1497 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1502 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1510 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1517 "lev_comp.y"
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
#line 1525 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1541 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1545 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1551 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1559 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1565 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1573 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1581 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1586 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1592 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1598 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1604 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1610 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1615 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(0, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1620 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1625 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1630 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1635 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1640 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1645 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1650 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1655 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1660 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1665 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1671 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1676 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_PROTECTOR));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1683 "lev_comp.y"
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
#line 1692 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1696 "lev_comp.y"
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
#line 1709 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1717 "lev_comp.y"
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
#line 1727 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1735 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1744 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1751 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1759 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1765 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1770 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1775 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1780 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1785 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1790 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1795 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1800 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1805 "lev_comp.y"
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
#line 1819 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1825 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1830 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1835 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1840 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1845 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1850 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1855 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1860 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1865 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1870 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1875 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1880 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x400000;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1885 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x800000;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1890 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x1000000;
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1895 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x2000000;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1900 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x4000000;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1905 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x8000000;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1912 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1918 "lev_comp.y"
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

  case 294:

/* Line 1455 of yacc.c  */
#line 1947 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1952 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1960 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1966 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1970 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1978 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1985 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1992 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2002 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2012 "lev_comp.y"
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

  case 304:

/* Line 1455 of yacc.c  */
#line 2027 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2037 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2041 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2047 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2053 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2059 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2063 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2069 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2075 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2082 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2090 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2096 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2101 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2106 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2111 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2116 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2122 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2128 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2133 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2138 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2143 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2148 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2153 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2163 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2169 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2175 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2180 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2187 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2194 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2198 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2204 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2210 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2216 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2221 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2228 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2234 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2244 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2250 "lev_comp.y"
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

  case 346:

/* Line 1455 of yacc.c  */
#line 2269 "lev_comp.y"
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

  case 347:

/* Line 1455 of yacc.c  */
#line 2281 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2285 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2291 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2296 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2301 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (long)(yyvsp[(9) - (9)].i), (long)(yyvsp[(7) - (9)].i), (long)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2306 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((long)(yyvsp[(9) - (13)].i), (long)(yyvsp[(7) - (13)].i), (long)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2312 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2316 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2322 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2328 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2332 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2338 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2342 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2348 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2352 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2357 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2364 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2368 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2375 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2379 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2384 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2391 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2395 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2400 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2408 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2414 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2421 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2425 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2432 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2443 "lev_comp.y"
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

  case 379:

/* Line 1455 of yacc.c  */
#line 2456 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2460 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2466 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2470 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2477 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2481 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2485 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2491 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2495 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2501 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2507 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2511 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2517 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2523 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2528 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2545 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2553 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2563 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2572 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2577 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2584 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2595 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2601 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2605 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2609 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2616 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2626 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2633 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2637 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2643 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2647 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2655 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2659 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2666 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2676 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2689 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2693 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2700 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2710 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2719 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2730 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2734 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2741 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2751 "lev_comp.y"
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

  case 433:

/* Line 1455 of yacc.c  */
#line 2762 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2771 "lev_comp.y"
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

  case 435:

/* Line 1455 of yacc.c  */
#line 2781 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2787 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2791 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2798 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2808 "lev_comp.y"
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

  case 440:

/* Line 1455 of yacc.c  */
#line 2819 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2828 "lev_comp.y"
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

  case 442:

/* Line 1455 of yacc.c  */
#line 2838 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2842 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2848 "lev_comp.y"
    { ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2850 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2856 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2860 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2864 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2868 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2876 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2885 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2889 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2893 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2897 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2901 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2904 "lev_comp.y"
    { ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2908 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2915 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2924 "lev_comp.y"
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

  case 464:

/* Line 1455 of yacc.c  */
#line 2965 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2969 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2976 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2983 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2993 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 2997 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 3005 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 3009 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 3013 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3017 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3021 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3026 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3030 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3034 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3039 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3044 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3049 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3053 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3058 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3063 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3067 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3071 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3076 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3080 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3087 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3093 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3097 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3103 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3119 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3123 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3127 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3131 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3153 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3157 "lev_comp.y"
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

  case 511:

/* Line 1455 of yacc.c  */
#line 3183 "lev_comp.y"
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
#line 7216 "lev_comp.tab.c"
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
#line 3211 "lev_comp.y"


/*lev_comp.y*/

