
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
     object_ID = 284,
     monster_ID = 285,
     terrain_ID = 286,
     MAZEWALK_ID = 287,
     WALLIFY_ID = 288,
     REGION_ID = 289,
     SPECIAL_REGION_ID = 290,
     SPECIAL_LEVREGION_ID = 291,
     SPECIAL_REGION_TYPE = 292,
     NAMING_ID = 293,
     NAMING_TYPE = 294,
     FILLING = 295,
     IRREGULAR = 296,
     JOINED = 297,
     ALTAR_ID = 298,
     ANVIL_ID = 299,
     NPC_ID = 300,
     LADDER_ID = 301,
     STAIR_ID = 302,
     NON_DIGGABLE_ID = 303,
     NON_PASSWALL_ID = 304,
     ROOM_ID = 305,
     ARTIFACT_NAME_ID = 306,
     PORTAL_ID = 307,
     TELEPRT_ID = 308,
     BRANCH_ID = 309,
     LEV = 310,
     MINERALIZE_ID = 311,
     AGE_ID = 312,
     CORRIDOR_ID = 313,
     GOLD_ID = 314,
     ENGRAVING_ID = 315,
     FOUNTAIN_ID = 316,
     THRONE_ID = 317,
     MODRON_PORTAL_ID = 318,
     LEVEL_TELEPORTER_ID = 319,
     LEVEL_TELEPORT_DIRECTION_TYPE = 320,
     LEVEL_TELEPORT_END_TYPE = 321,
     POOL_ID = 322,
     SINK_ID = 323,
     NONE = 324,
     RAND_CORRIDOR_ID = 325,
     DOOR_STATE = 326,
     LIGHT_STATE = 327,
     CURSE_TYPE = 328,
     ENGRAVING_TYPE = 329,
     KEYTYPE_ID = 330,
     LEVER_ID = 331,
     NO_PICKUP_ID = 332,
     DIRECTION = 333,
     RANDOM_TYPE = 334,
     RANDOM_TYPE_BRACKET = 335,
     A_REGISTER = 336,
     ALIGNMENT = 337,
     LEFT_OR_RIGHT = 338,
     CENTER = 339,
     TOP_OR_BOT = 340,
     ALTAR_TYPE = 341,
     ALTAR_SUBTYPE = 342,
     UP_OR_DOWN = 343,
     ACTIVE_OR_INACTIVE = 344,
     MODRON_PORTAL_TYPE = 345,
     NPC_TYPE = 346,
     FOUNTAIN_TYPE = 347,
     SPECIAL_OBJECT_TYPE = 348,
     CMAP_TYPE = 349,
     FLOOR_SUBTYPE = 350,
     FLOOR_SUBTYPE_ID = 351,
     FLOOR_ID = 352,
     FLOOR_TYPE = 353,
     FLOOR_TYPE_ID = 354,
     ELEMENTAL_ENCHANTMENT_TYPE = 355,
     EXCEPTIONALITY_TYPE = 356,
     EXCEPTIONALITY_ID = 357,
     ELEMENTAL_ENCHANTMENT_ID = 358,
     ENCHANTMENT_ID = 359,
     SECRET_DOOR_ID = 360,
     USES_UP_KEY_ID = 361,
     MYTHIC_PREFIX_TYPE = 362,
     MYTHIC_SUFFIX_TYPE = 363,
     MYTHIC_PREFIX_ID = 364,
     MYTHIC_SUFFIX_ID = 365,
     CHARGES_ID = 366,
     SPECIAL_QUALITY_ID = 367,
     SPEFLAGS_ID = 368,
     SUBROOM_ID = 369,
     NAME_ID = 370,
     FLAGS_ID = 371,
     FLAG_TYPE = 372,
     MON_ATTITUDE = 373,
     MON_ALERTNESS = 374,
     SUBTYPE_ID = 375,
     NON_PASSDOOR_ID = 376,
     MON_APPEARANCE = 377,
     ROOMDOOR_ID = 378,
     IF_ID = 379,
     ELSE_ID = 380,
     TERRAIN_ID = 381,
     HORIZ_OR_VERT = 382,
     REPLACE_TERRAIN_ID = 383,
     LOCATION_SUBTYPE_ID = 384,
     DOOR_SUBTYPE = 385,
     BRAZIER_SUBTYPE = 386,
     SIGNPOST_SUBTYPE = 387,
     TREE_SUBTYPE = 388,
     FOREST_ID = 389,
     FOREST_TYPE = 390,
     INITIALIZE_TYPE = 391,
     EXIT_ID = 392,
     SHUFFLE_ID = 393,
     QUANTITY_ID = 394,
     BURIED_ID = 395,
     LOOP_ID = 396,
     FOR_ID = 397,
     TO_ID = 398,
     SWITCH_ID = 399,
     CASE_ID = 400,
     BREAK_ID = 401,
     DEFAULT_ID = 402,
     ERODED_ID = 403,
     TRAPPED_STATE = 404,
     RECHARGED_ID = 405,
     INVIS_ID = 406,
     GREASED_ID = 407,
     INDESTRUCTIBLE_ID = 408,
     FEMALE_ID = 409,
     MALE_ID = 410,
     WAITFORU_ID = 411,
     PROTECTOR_ID = 412,
     CANCELLED_ID = 413,
     REVIVED_ID = 414,
     AVENGE_ID = 415,
     FLEEING_ID = 416,
     BLINDED_ID = 417,
     PARALYZED_ID = 418,
     STUNNED_ID = 419,
     CONFUSED_ID = 420,
     SEENTRAPS_ID = 421,
     ALL_ID = 422,
     MONTYPE_ID = 423,
     OBJTYPE_ID = 424,
     TERTYPE_ID = 425,
     TERTYPE2_ID = 426,
     LEVER_EFFECT_TYPE = 427,
     SWITCHABLE_ID = 428,
     CONTINUOUSLY_USABLE_ID = 429,
     TARGET_ID = 430,
     TRAPTYPE_ID = 431,
     EFFECT_FLAG_ID = 432,
     GRAVE_ID = 433,
     BRAZIER_ID = 434,
     SIGNPOST_ID = 435,
     TREE_ID = 436,
     ERODEPROOF_ID = 437,
     FUNCTION_ID = 438,
     MSG_OUTPUT_TYPE = 439,
     COMPARE_TYPE = 440,
     UNKNOWN_TYPE = 441,
     rect_ID = 442,
     fillrect_ID = 443,
     line_ID = 444,
     randline_ID = 445,
     grow_ID = 446,
     selection_ID = 447,
     flood_ID = 448,
     rndcoord_ID = 449,
     circle_ID = 450,
     ellipse_ID = 451,
     filter_ID = 452,
     complement_ID = 453,
     gradient_ID = 454,
     GRADIENT_TYPE = 455,
     LIMITED = 456,
     HUMIDITY_TYPE = 457,
     STRING = 458,
     MAP_ID = 459,
     NQSTRING = 460,
     VARSTRING = 461,
     CFUNC = 462,
     CFUNC_INT = 463,
     CFUNC_STR = 464,
     CFUNC_COORD = 465,
     CFUNC_REGION = 466,
     VARSTRING_INT = 467,
     VARSTRING_INT_ARRAY = 468,
     VARSTRING_STRING = 469,
     VARSTRING_STRING_ARRAY = 470,
     VARSTRING_VAR = 471,
     VARSTRING_VAR_ARRAY = 472,
     VARSTRING_COORD = 473,
     VARSTRING_COORD_ARRAY = 474,
     VARSTRING_REGION = 475,
     VARSTRING_REGION_ARRAY = 476,
     VARSTRING_MAPCHAR = 477,
     VARSTRING_MAPCHAR_ARRAY = 478,
     VARSTRING_MONST = 479,
     VARSTRING_MONST_ARRAY = 480,
     VARSTRING_OBJ = 481,
     VARSTRING_OBJ_ARRAY = 482,
     VARSTRING_SEL = 483,
     VARSTRING_SEL_ARRAY = 484,
     METHOD_INT = 485,
     METHOD_INT_ARRAY = 486,
     METHOD_STRING = 487,
     METHOD_STRING_ARRAY = 488,
     METHOD_VAR = 489,
     METHOD_VAR_ARRAY = 490,
     METHOD_COORD = 491,
     METHOD_COORD_ARRAY = 492,
     METHOD_REGION = 493,
     METHOD_REGION_ARRAY = 494,
     METHOD_MAPCHAR = 495,
     METHOD_MAPCHAR_ARRAY = 496,
     METHOD_MONST = 497,
     METHOD_MONST_ARRAY = 498,
     METHOD_OBJ = 499,
     METHOD_OBJ_ARRAY = 500,
     METHOD_SEL = 501,
     METHOD_SEL_ARRAY = 502,
     DICE = 503
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
#line 546 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 558 "lev_comp.tab.c"

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
#define YYLAST   1286

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  266
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  187
/* YYNRULES -- Number of rules.  */
#define YYNRULES  509
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1126

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   503

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   261,   265,     2,
     205,   206,   259,   257,   203,   258,   263,   260,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   204,     2,
       2,   262,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   207,     2,   208,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   209,   264,   210,     2,     2,     2,     2,
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
     195,   196,   197,   198,   199,   200,   201,   202,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    15,    19,    25,
      31,    37,    41,    58,    62,    68,    72,    76,    77,    80,
      81,    84,    85,    88,    90,    92,    93,    97,   101,   103,
     104,   107,   111,   113,   115,   117,   119,   121,   123,   125,
     127,   129,   131,   133,   135,   137,   139,   141,   143,   145,
     147,   149,   151,   153,   155,   157,   159,   161,   163,   165,
     167,   169,   171,   173,   175,   177,   179,   181,   183,   185,
     187,   189,   191,   193,   195,   197,   199,   201,   203,   205,
     207,   209,   211,   213,   215,   217,   219,   221,   223,   225,
     227,   229,   231,   233,   235,   237,   239,   241,   243,   245,
     247,   249,   251,   253,   255,   257,   259,   261,   263,   265,
     267,   269,   271,   273,   275,   277,   279,   283,   287,   293,
     297,   303,   309,   315,   319,   323,   329,   335,   341,   349,
     357,   365,   371,   373,   377,   379,   383,   385,   389,   391,
     395,   397,   401,   403,   407,   409,   413,   414,   415,   424,
     429,   431,   432,   434,   436,   442,   446,   447,   448,   458,
     459,   462,   463,   469,   470,   475,   477,   480,   482,   489,
     490,   494,   495,   502,   503,   508,   509,   514,   516,   517,
     522,   526,   528,   532,   536,   542,   548,   556,   561,   562,
     576,   577,   593,   594,   597,   603,   605,   611,   613,   619,
     621,   627,   629,   640,   647,   649,   651,   653,   655,   657,
     661,   663,   665,   666,   670,   674,   678,   682,   684,   686,
     688,   690,   692,   700,   706,   708,   710,   712,   714,   718,
     719,   725,   730,   731,   735,   737,   739,   741,   743,   746,
     748,   750,   752,   754,   756,   758,   762,   766,   770,   772,
     774,   778,   780,   782,   784,   786,   790,   794,   795,   801,
     804,   805,   809,   811,   815,   817,   821,   825,   827,   829,
     833,   835,   837,   839,   843,   845,   847,   849,   853,   857,
     861,   865,   869,   873,   877,   879,   881,   883,   887,   891,
     895,   901,   909,   915,   924,   926,   930,   934,   940,   946,
     954,   962,   969,   975,   976,   979,   985,   989,  1001,  1015,
    1022,  1023,  1027,  1029,  1033,  1037,  1041,  1045,  1047,  1049,
    1053,  1057,  1061,  1065,  1069,  1073,  1075,  1077,  1079,  1081,
    1085,  1089,  1091,  1097,  1107,  1113,  1121,  1125,  1129,  1135,
    1145,  1151,  1157,  1163,  1164,  1178,  1179,  1181,  1189,  1201,
    1211,  1225,  1229,  1237,  1245,  1251,  1259,  1265,  1275,  1281,
    1287,  1291,  1299,  1303,  1311,  1319,  1323,  1329,  1335,  1339,
    1345,  1353,  1363,  1365,  1367,  1369,  1371,  1373,  1374,  1377,
    1379,  1383,  1385,  1387,  1389,  1390,  1394,  1396,  1397,  1401,
    1403,  1404,  1408,  1410,  1412,  1414,  1416,  1418,  1420,  1422,
    1424,  1426,  1430,  1432,  1434,  1439,  1441,  1443,  1448,  1450,
    1452,  1457,  1459,  1464,  1470,  1472,  1476,  1478,  1482,  1484,
    1486,  1491,  1501,  1503,  1505,  1510,  1512,  1518,  1520,  1522,
    1527,  1529,  1531,  1537,  1539,  1541,  1543,  1548,  1550,  1552,
    1558,  1560,  1562,  1564,  1568,  1570,  1572,  1576,  1578,  1583,
    1587,  1591,  1595,  1599,  1603,  1607,  1609,  1611,  1615,  1617,
    1621,  1622,  1624,  1626,  1628,  1630,  1634,  1635,  1637,  1639,
    1642,  1645,  1650,  1657,  1662,  1669,  1676,  1683,  1690,  1693,
    1700,  1709,  1718,  1729,  1744,  1747,  1749,  1753,  1755,  1759,
    1761,  1763,  1765,  1767,  1769,  1771,  1773,  1775,  1777,  1779,
    1781,  1783,  1785,  1787,  1789,  1791,  1793,  1795,  1797,  1808
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     267,     0,    -1,    -1,   268,    -1,   269,    -1,   269,   268,
      -1,   270,   279,   281,    -1,    16,   204,   211,    -1,    15,
     204,   211,   203,     3,    -1,    17,   204,    11,   203,   379,
      -1,    17,   204,    10,   203,     3,    -1,    17,   204,    13,
      -1,    17,   204,    12,   203,     3,   203,     3,   203,     5,
     203,     5,   203,   414,   203,   278,   277,    -1,    18,   204,
     444,    -1,   134,   204,   135,   203,   136,    -1,   134,   204,
     135,    -1,    21,   204,    98,    -1,    -1,   203,   201,    -1,
      -1,   203,   421,    -1,    -1,   203,     3,    -1,     5,    -1,
      79,    -1,    -1,   116,   204,   280,    -1,   117,   203,   280,
      -1,   117,    -1,    -1,   283,   281,    -1,   209,   281,   210,
      -1,   324,    -1,   271,    -1,   272,    -1,   387,    -1,   274,
      -1,   273,    -1,   394,    -1,   393,    -1,   391,    -1,   392,
      -1,   395,    -1,   396,    -1,   397,    -1,   398,    -1,   399,
      -1,   368,    -1,   326,    -1,   289,    -1,   288,    -1,   382,
      -1,   338,    -1,   360,    -1,   401,    -1,   402,    -1,   370,
      -1,   371,    -1,   372,    -1,   400,    -1,   304,    -1,   314,
      -1,   316,    -1,   320,    -1,   318,    -1,   301,    -1,   311,
      -1,   297,    -1,   300,    -1,   363,    -1,   373,    -1,   345,
      -1,   361,    -1,   348,    -1,   354,    -1,   383,    -1,   378,
      -1,   366,    -1,   325,    -1,   384,    -1,   385,    -1,   386,
      -1,   388,    -1,   331,    -1,   329,    -1,   377,    -1,   381,
      -1,   380,    -1,   364,    -1,   365,    -1,   367,    -1,   359,
      -1,   362,    -1,   221,    -1,   223,    -1,   225,    -1,   227,
      -1,   229,    -1,   231,    -1,   233,    -1,   235,    -1,   237,
      -1,   220,    -1,   222,    -1,   224,    -1,   226,    -1,   228,
      -1,   230,    -1,   232,    -1,   234,    -1,   236,    -1,   284,
      -1,   285,    -1,   214,    -1,   214,    -1,   285,    -1,   138,
     204,   284,    -1,   286,   262,   433,    -1,   286,   262,   192,
     204,   442,    -1,   286,   262,   432,    -1,   286,   262,   449,
     204,   426,    -1,   286,   262,   448,   204,   428,    -1,   286,
     262,   447,   204,   430,    -1,   286,   262,   421,    -1,   286,
     262,   424,    -1,   286,   262,   209,   295,   210,    -1,   286,
     262,   209,   294,   210,    -1,   286,   262,   209,   293,   210,
      -1,   286,   262,   449,   204,   209,   292,   210,    -1,   286,
     262,   448,   204,   209,   291,   210,    -1,   286,   262,   447,
     204,   209,   290,   210,    -1,   286,   262,   209,   296,   210,
      -1,   431,    -1,   290,   203,   431,    -1,   429,    -1,   291,
     203,   429,    -1,   427,    -1,   292,   203,   427,    -1,   425,
      -1,   293,   203,   425,    -1,   422,    -1,   294,   203,   422,
      -1,   433,    -1,   295,   203,   433,    -1,   432,    -1,   296,
     203,   432,    -1,    -1,    -1,   183,   213,   205,   298,   437,
     206,   299,   282,    -1,   213,   205,   440,   206,    -1,   137,
      -1,    -1,     6,    -1,     6,    -1,   207,   433,   185,   433,
     208,    -1,   207,   433,   208,    -1,    -1,    -1,   144,   305,
     207,   420,   208,   306,   209,   307,   210,    -1,    -1,   308,
     307,    -1,    -1,   145,   445,   204,   309,   281,    -1,    -1,
     147,   204,   310,   281,    -1,   146,    -1,   263,   263,    -1,
     143,    -1,   142,   287,   262,   433,   312,   433,    -1,    -1,
     313,   315,   282,    -1,    -1,   141,   207,   420,   208,   317,
     282,    -1,    -1,   303,   204,   319,   283,    -1,    -1,   124,
     303,   321,   322,    -1,   282,    -1,    -1,   282,   323,   125,
     282,    -1,    14,   204,   432,    -1,    70,    -1,    70,   204,
     445,    -1,    70,   204,    79,    -1,    58,   204,   327,   203,
     327,    -1,    58,   204,   327,   203,   445,    -1,   205,     4,
     203,    78,   203,   342,   206,    -1,   404,   302,   203,   414,
      -1,    -1,   114,   204,   328,   203,   335,   203,   337,   405,
     408,   410,   412,   330,   282,    -1,    -1,    50,   204,   328,
     203,   334,   203,   336,   203,   337,   405,   408,   410,   412,
     332,   282,    -1,    -1,   203,     5,    -1,   205,     4,   203,
       4,   206,    -1,    79,    -1,   205,     4,   203,     4,   206,
      -1,    79,    -1,   205,   346,   203,   347,   206,    -1,    79,
      -1,   205,     4,   203,     4,   206,    -1,    79,    -1,   123,
     204,   339,   203,   413,   203,   340,   203,   342,   343,    -1,
      27,   204,   413,   203,   442,   343,    -1,     5,    -1,    79,
      -1,   341,    -1,    79,    -1,    78,    -1,    78,   264,   341,
      -1,     4,    -1,    79,    -1,    -1,   343,   203,   344,    -1,
     129,   204,   130,    -1,    75,   204,   430,    -1,   112,   204,
       4,    -1,   153,    -1,   105,    -1,   106,    -1,   121,    -1,
      20,    -1,    19,   204,   346,   203,   347,   333,   212,    -1,
      19,   204,   421,   333,   212,    -1,    83,    -1,    84,    -1,
      85,    -1,    84,    -1,    25,   204,   350,    -1,    -1,    25,
     204,   350,   349,   282,    -1,   428,   203,   421,   351,    -1,
      -1,   351,   203,   352,    -1,   432,    -1,   118,    -1,   119,
      -1,   416,    -1,   122,   432,    -1,   154,    -1,   155,    -1,
     151,    -1,   158,    -1,   159,    -1,   160,    -1,   161,   204,
     420,    -1,   162,   204,   420,    -1,   163,   204,   420,    -1,
     164,    -1,   165,    -1,   166,   204,   353,    -1,   156,    -1,
     157,    -1,   211,    -1,   167,    -1,   211,   264,   353,    -1,
      23,   204,   356,    -1,    -1,    24,   204,   356,   355,   282,
      -1,   430,   357,    -1,    -1,   357,   203,   358,    -1,    73,
      -1,   168,   204,   428,    -1,   446,    -1,   115,   204,   432,
      -1,   139,   204,   420,    -1,   140,    -1,    72,    -1,   148,
     204,   420,    -1,   182,    -1,    71,    -1,   149,    -1,   150,
     204,   420,    -1,   151,    -1,   152,    -1,   421,    -1,   103,
     204,   100,    -1,   102,   204,   101,    -1,   104,   204,   420,
      -1,   111,   204,   420,    -1,   112,   204,   420,    -1,   113,
     204,   420,    -1,    75,   204,   430,    -1,   153,    -1,   106,
      -1,    77,    -1,   109,   204,   107,    -1,   110,   204,   108,
      -1,    57,   204,   420,    -1,    26,   204,   403,   203,   421,
      -1,    28,   204,   421,   203,    78,   203,   413,    -1,    32,
     204,   421,   203,    78,    -1,    32,   204,   421,   203,    78,
     203,     5,   277,    -1,    33,    -1,    33,   204,   442,    -1,
      33,   204,   167,    -1,    46,   204,   421,   203,    88,    -1,
      47,   204,   421,   203,    88,    -1,    47,   204,   451,   203,
     451,   203,    88,    -1,    52,   204,   451,   203,   451,   203,
     211,    -1,    53,   204,   451,   203,   451,   369,    -1,    54,
     204,   451,   203,   451,    -1,    -1,   203,    88,    -1,    61,
     204,   442,   203,    92,    -1,    62,   204,   442,    -1,    63,
     204,   421,   203,   175,   204,   421,   203,    90,   203,    89,
      -1,    63,   204,   421,   203,    64,   203,    65,   203,    66,
     203,    90,   203,    89,    -1,    76,   204,   421,   203,   172,
     374,    -1,    -1,   374,   203,   375,    -1,    89,    -1,   168,
     204,   428,    -1,   169,   204,   430,    -1,   170,   204,   379,
      -1,   171,   204,   379,    -1,   173,    -1,   174,    -1,   175,
     204,   421,    -1,   176,   204,   403,    -1,   129,   204,   376,
      -1,    96,   204,   411,    -1,   177,   204,     4,    -1,   112,
     204,     4,    -1,     4,    -1,    90,    -1,   130,    -1,    92,
      -1,    68,   204,   442,    -1,    67,   204,   442,    -1,     3,
      -1,   205,     3,   203,   414,   206,    -1,   128,   204,   424,
     203,   426,   203,   426,   203,     7,    -1,   126,   204,   442,
     203,   426,    -1,   126,   204,   442,   203,   426,   203,    95,
      -1,    48,   204,   424,    -1,    49,   204,   424,    -1,    38,
     204,   211,   203,    39,    -1,    38,   204,   211,   203,    39,
     203,   168,   204,   428,    -1,    35,   204,   424,   203,    37,
      -1,    36,   204,   451,   203,    37,    -1,    22,   204,   442,
     203,    94,    -1,    -1,    34,   204,   424,   203,   414,   203,
     404,   405,   408,   410,   412,   389,   390,    -1,    -1,   282,
      -1,    43,   204,   421,   203,   415,   203,   417,    -1,    43,
     204,   421,   203,   415,   203,   417,   203,   168,   204,   428,
      -1,    43,   204,   421,   203,   415,   203,   417,   203,    87,
      -1,    43,   204,   421,   203,   415,   203,   417,   203,    87,
     203,   168,   204,   428,    -1,    44,   204,   421,    -1,    44,
     204,   421,   203,   168,   204,   428,    -1,    97,   204,   442,
     203,    98,   203,    95,    -1,   120,   204,   442,   203,     4,
      -1,   120,   204,   442,   203,    98,   203,    95,    -1,    45,
     204,    91,   203,   421,    -1,    45,   204,    91,   203,   421,
     203,   168,   204,   428,    -1,   178,   204,   421,   203,   432,
      -1,   178,   204,   421,   203,    79,    -1,   178,   204,   421,
      -1,   179,   204,   421,   203,   131,   203,   414,    -1,   179,
     204,   421,    -1,   180,   204,   421,   203,   132,   203,   432,
      -1,   180,   204,   421,   203,   132,   203,    79,    -1,   180,
     204,   421,    -1,   181,   204,   421,   203,   133,    -1,   181,
     204,   421,   203,   135,    -1,   181,   204,   421,    -1,    59,
     204,   433,   203,   421,    -1,    60,   204,   421,   203,   450,
     203,   432,    -1,    56,   204,   420,   203,   420,   203,   420,
     203,   420,    -1,    56,    -1,   211,    -1,    79,    -1,   211,
      -1,    79,    -1,    -1,   203,   406,    -1,   407,    -1,   407,
     203,   406,    -1,    40,    -1,    41,    -1,    42,    -1,    -1,
      99,   204,   409,    -1,    98,    -1,    -1,    96,   204,   411,
      -1,    95,    -1,    -1,   168,   204,   428,    -1,    71,    -1,
      79,    -1,    72,    -1,    79,    -1,    82,    -1,   418,    -1,
      79,    -1,    82,    -1,   418,    -1,    81,   204,    79,    -1,
      86,    -1,    79,    -1,    81,   207,     4,   208,    -1,   211,
      -1,   222,    -1,   223,   207,   433,   208,    -1,   433,    -1,
     422,    -1,   194,   205,   442,   206,    -1,   226,    -1,   227,
     207,   433,   208,    -1,   205,     4,   203,     4,   206,    -1,
      79,    -1,    80,   423,   208,    -1,   202,    -1,   202,   203,
     423,    -1,   425,    -1,   228,    -1,   229,   207,   433,   208,
      -1,   205,     4,   203,     4,   203,     4,   203,     4,   206,
      -1,   427,    -1,   230,    -1,   231,   207,   433,   208,    -1,
       3,    -1,   205,     3,   203,   414,   206,    -1,   429,    -1,
     232,    -1,   233,   207,   433,   208,    -1,   211,    -1,     3,
      -1,   205,     3,   203,   211,   206,    -1,    79,    -1,   431,
      -1,   234,    -1,   235,   207,   433,   208,    -1,   211,    -1,
       3,    -1,   205,     3,   203,   211,   206,    -1,    79,    -1,
      93,    -1,   419,    -1,   432,   263,   419,    -1,     4,    -1,
     443,    -1,   205,     8,   206,    -1,   220,    -1,   221,   207,
     433,   208,    -1,   433,   257,   433,    -1,   433,   258,   433,
      -1,   433,   259,   433,    -1,   433,   260,   433,    -1,   433,
     261,   433,    -1,   205,   433,   206,    -1,   216,    -1,   217,
      -1,   286,   204,   434,    -1,   435,    -1,   436,   203,   435,
      -1,    -1,   436,    -1,   433,    -1,   432,    -1,   438,    -1,
     439,   203,   438,    -1,    -1,   439,    -1,   421,    -1,   187,
     424,    -1,   188,   424,    -1,   189,   421,   203,   421,    -1,
     190,   421,   203,   421,   203,   433,    -1,   191,   205,   442,
     206,    -1,   191,   205,   341,   203,   442,   206,    -1,   197,
     205,     7,   203,   442,   206,    -1,   197,   205,   442,   203,
     442,   206,    -1,   197,   205,   426,   203,   442,   206,    -1,
     193,   421,    -1,   195,   205,   421,   203,   433,   206,    -1,
     195,   205,   421,   203,   433,   203,    40,   206,    -1,   196,
     205,   421,   203,   433,   203,   433,   206,    -1,   196,   205,
     421,   203,   433,   203,   433,   203,    40,   206,    -1,   199,
     205,   200,   203,   205,   433,   258,   433,   275,   206,   203,
     421,   276,   206,    -1,   198,   441,    -1,   236,    -1,   205,
     442,   206,    -1,   441,    -1,   441,   265,   442,    -1,   256,
      -1,    94,    -1,     4,    -1,     8,    -1,     9,    -1,     4,
      -1,     8,    -1,     9,    -1,     4,    -1,   443,    -1,    29,
      -1,    23,    -1,    30,    -1,    25,    -1,    31,    -1,   126,
      -1,    74,    -1,    79,    -1,   452,    -1,    55,   205,     4,
     203,     4,   203,     4,   203,     4,   206,    -1,   205,     4,
     203,     4,   203,     4,   203,     4,   206,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   284,   284,   285,   288,   289,   292,   315,   320,   342,
     355,   367,   373,   402,   408,   412,   418,   425,   428,   435,
     439,   446,   449,   456,   457,   461,   464,   471,   475,   482,
     485,   491,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   560,   561,   562,   563,   564,   565,   566,
     567,   568,   571,   572,   573,   574,   575,   576,   577,   578,
     579,   582,   583,   584,   587,   588,   591,   607,   613,   619,
     625,   631,   637,   643,   649,   655,   665,   675,   685,   695,
     705,   715,   727,   732,   739,   744,   751,   756,   763,   767,
     773,   778,   785,   789,   795,   799,   806,   828,   805,   842,
     897,   904,   907,   913,   920,   924,   933,   937,   932,  1000,
    1001,  1005,  1004,  1018,  1017,  1032,  1042,  1043,  1046,  1084,
    1083,  1118,  1117,  1148,  1147,  1180,  1179,  1205,  1216,  1215,
    1243,  1249,  1254,  1259,  1266,  1273,  1282,  1290,  1302,  1301,
    1325,  1324,  1348,  1351,  1357,  1367,  1373,  1382,  1388,  1393,
    1399,  1404,  1410,  1421,  1427,  1428,  1431,  1432,  1435,  1439,
    1445,  1446,  1450,  1456,  1464,  1469,  1474,  1479,  1484,  1489,
    1494,  1502,  1509,  1517,  1525,  1526,  1529,  1530,  1533,  1538,
    1537,  1551,  1558,  1565,  1573,  1578,  1584,  1590,  1596,  1602,
    1607,  1612,  1617,  1622,  1627,  1632,  1637,  1642,  1647,  1652,
    1657,  1663,  1668,  1675,  1684,  1688,  1701,  1710,  1709,  1727,
    1737,  1743,  1751,  1757,  1762,  1767,  1772,  1777,  1782,  1787,
    1792,  1797,  1811,  1817,  1822,  1827,  1832,  1837,  1842,  1847,
    1852,  1857,  1862,  1867,  1872,  1877,  1882,  1887,  1892,  1897,
    1904,  1910,  1939,  1944,  1952,  1958,  1962,  1970,  1977,  1984,
    1994,  2004,  2019,  2030,  2033,  2039,  2045,  2051,  2055,  2061,
    2068,  2074,  2082,  2088,  2093,  2098,  2103,  2108,  2114,  2120,
    2125,  2130,  2135,  2140,  2145,  2152,  2152,  2152,  2152,  2155,
    2161,  2167,  2172,  2179,  2186,  2190,  2196,  2202,  2208,  2213,
    2220,  2226,  2236,  2243,  2242,  2274,  2277,  2283,  2288,  2293,
    2298,  2304,  2308,  2314,  2320,  2324,  2330,  2334,  2340,  2344,
    2349,  2356,  2360,  2367,  2371,  2376,  2383,  2387,  2392,  2400,
    2406,  2413,  2417,  2424,  2432,  2435,  2445,  2449,  2452,  2458,
    2462,  2469,  2473,  2477,  2484,  2487,  2493,  2500,  2503,  2509,
    2516,  2520,  2527,  2528,  2531,  2532,  2535,  2536,  2537,  2543,
    2544,  2545,  2551,  2552,  2555,  2564,  2569,  2576,  2587,  2593,
    2597,  2601,  2608,  2618,  2625,  2629,  2635,  2639,  2647,  2651,
    2658,  2668,  2681,  2685,  2692,  2702,  2711,  2722,  2726,  2733,
    2743,  2754,  2763,  2773,  2779,  2783,  2790,  2800,  2811,  2820,
    2830,  2834,  2841,  2842,  2848,  2852,  2856,  2860,  2868,  2877,
    2881,  2885,  2889,  2893,  2897,  2900,  2907,  2916,  2949,  2950,
    2953,  2954,  2957,  2961,  2968,  2975,  2986,  2989,  2997,  3001,
    3005,  3009,  3013,  3018,  3022,  3026,  3031,  3036,  3041,  3045,
    3050,  3055,  3059,  3063,  3068,  3072,  3079,  3085,  3089,  3095,
    3102,  3103,  3106,  3107,  3108,  3111,  3115,  3119,  3123,  3129,
    3130,  3133,  3134,  3137,  3138,  3141,  3142,  3145,  3149,  3175
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
  "MONSTER_ID", "TRAP_ID", "DOOR_ID", "DRAWBRIDGE_ID", "object_ID",
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
  "CURSE_TYPE", "ENGRAVING_TYPE", "KEYTYPE_ID", "LEVER_ID", "NO_PICKUP_ID",
  "DIRECTION", "RANDOM_TYPE", "RANDOM_TYPE_BRACKET", "A_REGISTER",
  "ALIGNMENT", "LEFT_OR_RIGHT", "CENTER", "TOP_OR_BOT", "ALTAR_TYPE",
  "ALTAR_SUBTYPE", "UP_OR_DOWN", "ACTIVE_OR_INACTIVE",
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
  "FOREST_TYPE", "INITIALIZE_TYPE", "EXIT_ID", "SHUFFLE_ID", "QUANTITY_ID",
  "BURIED_ID", "LOOP_ID", "FOR_ID", "TO_ID", "SWITCH_ID", "CASE_ID",
  "BREAK_ID", "DEFAULT_ID", "ERODED_ID", "TRAPPED_STATE", "RECHARGED_ID",
  "INVIS_ID", "GREASED_ID", "INDESTRUCTIBLE_ID", "FEMALE_ID", "MALE_ID",
  "WAITFORU_ID", "PROTECTOR_ID", "CANCELLED_ID", "REVIVED_ID", "AVENGE_ID",
  "FLEEING_ID", "BLINDED_ID", "PARALYZED_ID", "STUNNED_ID", "CONFUSED_ID",
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
     455,   456,   457,    44,    58,    40,    41,    91,    93,   123,
     125,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,    43,    45,    42,
      47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   266,   267,   267,   268,   268,   269,   270,   270,   271,
     271,   271,   271,   272,   273,   273,   274,   275,   275,   276,
     276,   277,   277,   278,   278,   279,   279,   280,   280,   281,
     281,   282,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   286,   286,   286,   287,   287,   288,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   290,   290,   291,   291,   292,   292,   293,   293,
     294,   294,   295,   295,   296,   296,   298,   299,   297,   300,
     301,   302,   302,   303,   303,   303,   305,   306,   304,   307,
     307,   309,   308,   310,   308,   311,   312,   312,   313,   315,
     314,   317,   316,   319,   318,   321,   320,   322,   323,   322,
     324,   325,   325,   325,   326,   326,   327,   328,   330,   329,
     332,   331,   333,   333,   334,   334,   335,   335,   336,   336,
     337,   337,   338,   338,   339,   339,   340,   340,   341,   341,
     342,   342,   343,   343,   344,   344,   344,   344,   344,   344,
     344,   345,   345,   345,   346,   346,   347,   347,   348,   349,
     348,   350,   351,   351,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   353,   353,   353,   354,   355,   354,   356,
     357,   357,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     359,   360,   361,   361,   362,   362,   362,   363,   364,   365,
     366,   367,   368,   369,   369,   370,   371,   372,   372,   373,
     374,   374,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   376,   376,   376,   376,   377,
     378,   379,   379,   380,   381,   381,   382,   383,   384,   384,
     385,   386,   387,   389,   388,   390,   390,   391,   391,   391,
     391,   392,   392,   393,   394,   394,   395,   395,   396,   396,
     396,   397,   397,   398,   398,   398,   399,   399,   399,   400,
     401,   402,   402,   403,   403,   404,   404,   405,   405,   406,
     406,   407,   407,   407,   408,   408,   409,   410,   410,   411,
     412,   412,   413,   413,   414,   414,   415,   415,   415,   416,
     416,   416,   417,   417,   418,   419,   419,   419,   420,   421,
     421,   421,   421,   422,   422,   422,   423,   423,   424,   424,
     424,   425,   426,   426,   426,   427,   427,   428,   428,   428,
     429,   429,   429,   429,   430,   430,   430,   431,   431,   431,
     431,   431,   432,   432,   433,   433,   433,   433,   433,   433,
     433,   433,   433,   433,   433,   434,   434,   435,   436,   436,
     437,   437,   438,   438,   439,   439,   440,   440,   441,   441,
     441,   441,   441,   441,   441,   441,   441,   441,   441,   441,
     441,   441,   441,   441,   441,   441,   441,   442,   442,   443,
     444,   444,   445,   445,   445,   446,   446,   446,   446,   447,
     447,   448,   448,   449,   449,   450,   450,   451,   451,   452
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     3,     3,     5,     5,
       5,     3,    16,     3,     5,     3,     3,     0,     2,     0,
       2,     0,     2,     1,     1,     0,     3,     3,     1,     0,
       2,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     5,     3,
       5,     5,     5,     3,     3,     5,     5,     5,     7,     7,
       7,     5,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     0,     0,     8,     4,
       1,     0,     1,     1,     5,     3,     0,     0,     9,     0,
       2,     0,     5,     0,     4,     1,     2,     1,     6,     0,
       3,     0,     6,     0,     4,     0,     4,     1,     0,     4,
       3,     1,     3,     3,     5,     5,     7,     4,     0,    13,
       0,    15,     0,     2,     5,     1,     5,     1,     5,     1,
       5,     1,    10,     6,     1,     1,     1,     1,     1,     3,
       1,     1,     0,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     7,     5,     1,     1,     1,     1,     3,     0,
       5,     4,     0,     3,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     1,     1,
       3,     1,     1,     1,     1,     3,     3,     0,     5,     2,
       0,     3,     1,     3,     1,     3,     3,     1,     1,     3,
       1,     1,     1,     3,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     3,     3,     3,
       5,     7,     5,     8,     1,     3,     3,     5,     5,     7,
       7,     6,     5,     0,     2,     5,     3,    11,    13,     6,
       0,     3,     1,     3,     3,     3,     3,     1,     1,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     3,
       3,     1,     5,     9,     5,     7,     3,     3,     5,     9,
       5,     5,     5,     0,    13,     0,     1,     7,    11,     9,
      13,     3,     7,     7,     5,     7,     5,     9,     5,     5,
       3,     7,     3,     7,     7,     3,     5,     5,     3,     5,
       7,     9,     1,     1,     1,     1,     1,     0,     2,     1,
       3,     1,     1,     1,     0,     3,     1,     0,     3,     1,
       0,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     4,     1,     1,     4,     1,     1,
       4,     1,     4,     5,     1,     3,     1,     3,     1,     1,
       4,     9,     1,     1,     4,     1,     5,     1,     1,     4,
       1,     1,     5,     1,     1,     1,     4,     1,     1,     5,
       1,     1,     1,     3,     1,     1,     3,     1,     4,     3,
       3,     3,     3,     3,     3,     1,     1,     3,     1,     3,
       0,     1,     1,     1,     1,     3,     0,     1,     1,     2,
       2,     4,     6,     4,     6,     6,     6,     6,     2,     6,
       8,     8,    10,    14,     2,     1,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,    10,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    25,     0,     0,     1,
       5,     0,    29,     0,     7,     0,   153,     0,     0,     0,
       0,   221,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   294,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   372,     0,     0,
       0,     0,     0,     0,     0,     0,   181,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   150,     0,     0,     0,
     156,   165,     0,     0,     0,     0,     0,     0,     0,   113,
     102,    93,   103,    94,   104,    95,   105,    96,   106,    97,
     107,    98,   108,    99,   109,   100,   110,   101,    33,    34,
      37,    36,     6,    29,   111,   112,     0,    50,    49,    67,
      68,    65,     0,    60,    66,   169,    61,    62,    64,    63,
      32,    78,    48,    84,    83,    52,    71,    73,    74,    91,
      53,    72,    92,    69,    88,    89,    77,    90,    47,    56,
      57,    58,    70,    85,    76,    87,    86,    51,    75,    79,
      80,    81,    35,    82,    40,    41,    39,    38,    42,    43,
      44,    45,    46,    59,    54,    55,     0,    28,    26,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   175,     0,     0,     0,     0,     0,   114,
     115,     0,     0,     0,     0,     0,     0,     0,   444,     0,
     447,     0,   489,     0,   445,   466,    30,     0,   173,     0,
       8,     0,   405,   406,     0,   442,   180,     0,     0,     0,
      11,   491,   490,    13,   414,     0,   224,   225,     0,     0,
     411,     0,     0,   192,   409,    16,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   485,   468,
     487,     0,   438,   440,   441,     0,   437,   435,     0,   256,
     260,   434,   257,   431,   433,     0,   430,   428,     0,   228,
       0,   427,   374,   373,     0,   392,   393,     0,     0,     0,
     296,   295,     0,   419,     0,     0,   418,     0,     0,     0,
       0,   507,     0,     0,   351,     0,     0,     0,     0,     0,
     336,   337,   376,   375,     0,   151,     0,     0,     0,     0,
     408,     0,     0,     0,     0,     0,   306,     0,   330,   329,
     494,   492,   493,   183,   182,     0,     0,     0,     0,   204,
     205,     0,     0,     0,     0,    15,   116,     0,     0,     0,
     360,   362,   365,   368,   146,     0,     0,     0,     0,   155,
       0,     0,     0,     0,     0,   463,   462,   464,   467,     0,
     500,   502,   499,   501,   503,   504,     0,     0,     0,   123,
     124,   119,   117,     0,     0,     0,     0,    29,   170,    27,
       0,     0,     0,     0,     0,   416,     0,     0,     0,     0,
       0,     0,     0,   469,   470,     0,     0,     0,   478,     0,
       0,     0,   484,     0,     0,     0,     0,     0,     0,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   152,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   177,   176,     0,     0,     0,   171,     0,     0,
       0,     0,     0,     0,   460,   446,   454,     0,     0,   449,
     450,   451,   452,   453,     0,   149,     0,   444,     0,     0,
       0,     0,   140,   138,   144,   142,     0,     0,     0,   174,
       0,     0,   443,    10,   331,     0,     9,     0,     0,   415,
       0,     0,     0,   227,   226,   192,   193,   223,     0,     0,
     208,     0,     0,     0,     0,   425,     0,     0,   423,     0,
       0,   422,     0,     0,   486,   488,   342,     0,     0,     0,
     258,     0,     0,   230,   232,   290,   212,     0,   292,     0,
       0,   394,   395,     0,   340,     0,     0,   341,   338,   398,
       0,   396,     0,   397,     0,   356,   297,     0,   298,     0,
     195,     0,     0,     0,     0,   303,   302,     0,     0,   184,
     185,   369,   505,   506,     0,   305,     0,     0,   310,     0,
     197,     0,     0,   354,     0,     0,     0,     0,   334,     0,
      14,     0,   167,     0,     0,   157,   359,   358,     0,     0,
     366,   367,     0,   458,   461,     0,   448,   154,   465,   118,
       0,     0,   127,     0,   126,     0,   125,     0,   131,     0,
     122,     0,   121,     0,   120,    31,   407,     0,     0,   417,
     410,     0,   412,     0,   471,     0,     0,     0,   473,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   436,   497,
     495,   496,     0,   271,   268,   262,     0,   286,     0,     0,
       0,   285,     0,     0,     0,     0,     0,     0,     0,   267,
       0,   272,     0,   274,   275,   284,     0,   270,   261,   276,
     498,   264,     0,   429,   231,   203,     0,     0,     0,   420,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   187,     0,     0,   301,     0,     0,     0,     0,
       0,   309,     0,     0,     0,     0,     0,     0,     0,     0,
     172,   166,   168,     0,     0,     0,     0,     0,   147,     0,
     139,   141,   143,   145,     0,   132,     0,   134,     0,   136,
       0,     0,   413,   222,     0,   209,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   439,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,   291,    21,     0,   377,     0,     0,
       0,     0,   403,   402,   347,   352,     0,   299,     0,   199,
       0,     0,   300,   304,     0,     0,   370,     0,     0,     0,
     353,     0,   201,     0,   377,   355,   207,     0,   206,   179,
     335,     0,   159,   361,   364,   363,   455,   456,   457,   459,
       0,     0,   130,     0,   129,     0,   128,     0,     0,   472,
     474,     0,   479,     0,   475,     0,   424,   477,   476,     0,
     289,   283,   278,   277,   279,   287,   288,   280,   281,   282,
     265,   266,   269,   273,   263,     0,   399,   235,   236,     0,
     241,   239,   240,   251,   252,   242,   243,   244,     0,     0,
       0,   248,   249,     0,   233,   237,   400,   234,     0,   218,
     219,     0,   220,     0,   217,   213,     0,   293,     0,     0,
     384,     0,     0,     0,   404,     0,     0,     0,     0,     0,
       0,   210,   211,     0,     0,     0,   312,     0,     0,     0,
       0,     0,     0,     0,   317,   318,     0,     0,     0,   311,
       0,     0,   384,     0,     0,     0,     0,     0,   159,   148,
     133,   135,   137,   332,     0,     0,     0,   426,     0,     0,
     238,     0,     0,     0,     0,     0,     0,     0,    22,     0,
     381,   382,   383,   378,   379,     0,   387,     0,     0,   339,
     349,     0,   357,   194,     0,   377,   371,   186,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     196,     0,   387,   212,   333,     0,   163,   158,   160,     0,
     480,     0,   481,   450,   401,   245,   246,   247,   254,   253,
     250,   215,   216,   214,     0,     0,     0,     0,   390,     0,
       0,     0,     0,     0,   384,     0,     0,   389,   322,   324,
     325,   326,   328,   327,   321,   313,   314,   315,   316,   319,
     320,   323,     0,   390,   202,   161,    29,     0,     0,     0,
       0,     0,   421,   380,   386,   385,     0,     0,   343,     0,
     509,     0,   348,   198,   387,     0,   307,   200,   188,    29,
     164,     0,   482,    18,     0,   255,   388,     0,   345,   508,
       0,   390,     0,     0,   162,     0,     0,   391,   346,   344,
     350,   190,   308,   189,     0,    19,     0,    23,    24,    21,
       0,     0,   191,    12,    20,   483
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    98,    99,   100,   101,  1070,
    1121,   917,  1119,    12,   168,   102,   408,   103,   104,   105,
     106,   221,   107,   108,   764,   766,   768,   508,   509,   510,
     511,   109,   494,   850,   110,   111,   466,   112,   113,   222,
     753,   957,   958,  1089,  1066,   114,   624,   115,   116,   239,
     117,   621,   118,   406,   119,   362,   483,   616,   120,   121,
     122,   342,   334,   123,  1103,   124,  1116,   422,   592,   612,
     821,   834,   125,   361,   837,   541,   933,   715,   915,   126,
     262,   535,   127,   443,   299,   714,   904,  1030,   128,   440,
     289,   439,   708,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   735,   139,   140,   141,   142,   741,   949,
    1054,   143,   144,   526,   145,   146,   147,   148,   149,   150,
     151,   152,   153,  1098,  1109,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   304,   335,   920,
     983,   984,   986,  1075,  1038,  1048,  1078,   307,   573,   582,
     905,   814,   583,   245,   339,   279,   264,   416,   315,   316,
     550,   551,   300,   301,   290,   291,   385,   340,   848,   633,
     634,   635,   387,   388,   389,   280,   434,   234,   253,   354,
     711,   403,   404,   405,   604,   320,   321
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -995
static const yytype_int16 yypact[] =
{
     213,  -127,  -109,   105,  -995,   213,     1,  -102,   -82,  -995,
    -995,   -58,   781,   -48,  -995,    69,  -995,   -32,   -27,    11,
      16,  -995,    78,    97,   113,   116,   142,   148,   165,   167,
     187,   195,   210,   222,   230,   234,   240,   251,   262,   271,
     273,   282,   283,   285,   286,   287,   291,   295,   299,   320,
     323,   327,   329,   330,   331,   339,   345,   346,   349,   351,
     352,   353,    55,   358,   360,   362,  -995,   363,    -8,   846,
    -995,  -995,   364,   367,   371,   381,    32,   109,   132,  -995,
    -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,
    -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,
    -995,  -995,  -995,   781,  -995,  -995,   194,  -995,  -995,  -995,
    -995,  -995,   382,  -995,  -995,  -995,  -995,  -995,  -995,  -995,
    -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,
    -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,
    -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,
    -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,
    -995,  -995,  -995,  -995,  -995,  -995,   519,   333,  -995,   -96,
     372,    46,   516,   432,   481,   129,   129,   269,   -41,    51,
     332,   332,   539,  -104,  -104,   -24,   365,   332,   332,   451,
     332,    89,  -104,  -104,   -40,   -24,   -24,   -24,   109,   379,
     109,   332,   481,   481,   332,   481,   481,    95,   332,   481,
     -40,   481,    57,  -995,   481,  -104,   416,   840,   109,  -995,
    -995,   342,   384,   332,   332,   332,   332,   389,  -995,    71,
    -995,   395,  -995,   380,  -995,    19,  -995,   181,  -995,   411,
    -995,    69,  -995,  -995,   414,  -995,   359,   421,   422,   424,
    -995,  -995,  -995,  -995,  -995,   426,  -995,  -995,   437,   625,
    -995,   439,   444,   447,  -995,  -995,  -104,  -104,   332,   332,
     446,   332,   448,   455,   476,   481,   487,   318,  -995,  -995,
     387,   500,  -995,  -995,  -995,   706,  -995,  -995,   504,  -995,
    -995,  -995,  -995,  -995,  -995,   712,  -995,  -995,   509,   511,
     515,  -995,  -995,  -995,   521,  -995,  -995,   522,   536,   538,
    -995,  -995,   715,  -995,   545,   543,  -995,   550,   517,   750,
     561,  -995,   569,   570,   571,   573,   574,   774,   576,   582,
    -995,  -995,  -995,  -995,   583,   782,   586,   587,   589,   590,
     204,   790,   593,   502,   594,   608,  -995,   609,  -995,  -995,
    -995,  -995,  -995,  -995,  -995,   617,   618,   619,   642,  -995,
    -995,   643,   411,   644,   647,   649,  -995,   645,   109,   109,
     651,   652,   653,   655,  -995,   658,   374,   109,   109,  -995,
     109,   109,   109,   109,   109,   359,   204,  -995,   662,   660,
    -995,  -995,  -995,  -995,  -995,  -995,   673,    98,    14,  -995,
    -995,   359,   204,   676,   682,   689,   781,   781,  -995,  -995,
     109,   -96,   891,    21,   894,   695,   694,   481,   700,   109,
     202,   901,   696,  -995,  -995,   707,   708,   468,  -995,   332,
     332,   418,  -995,   713,   710,   481,   820,   709,   109,   714,
     411,   717,   109,   411,   332,   332,   481,   843,   848,   721,
     109,   101,   896,   930,   735,   902,   907,   338,   779,   332,
     860,   747,   863,   -24,   -39,  -995,   749,   -24,   -24,   -24,
     109,   751,    79,   332,   123,   861,   -19,   783,   858,   -21,
      60,    51,   832,  -995,   128,   128,   829,  -995,   170,   758,
     -47,   827,   835,    70,   822,  -995,  -995,   523,   602,   -11,
     -11,  -995,  -995,  -995,    19,  -995,   481,   765,    28,   133,
     141,   169,  -995,  -995,   359,   204,   249,   144,    38,  -995,
     760,   610,  -995,  -995,  -995,   968,  -995,   784,   426,  -995,
     785,   981,   615,  -995,  -995,   447,  -995,  -995,   332,   332,
     725,   787,   786,   793,   794,  -995,   795,   252,  -995,   792,
     797,  -995,   816,   817,  -995,  -995,  -995,   810,   624,    39,
    -995,   811,   630,  -995,  -995,  -995,  -995,   821,   823,   989,
     671,  -995,  -995,   824,  -995,   825,  1019,  -995,   828,  -995,
     826,  -995,   831,  -995,   833,   836,  -995,  1021,  -995,   859,
    -995,  1026,   876,   101,   878,   880,  -995,   881,   954,  -995,
    -995,  -995,  -995,  -995,   882,  -995,   883,   834,  -995,   884,
    -995,  1031,   885,  -995,   886,   887,   939,  1088,   889,   890,
    -995,   411,  -995,   837,   109,  -995,  -995,   359,   892,   893,
    -995,  -995,   895,  -995,   898,   888,  -995,  -995,  -995,  -995,
    1093,   897,  -995,    -7,  -995,   109,  -995,   -96,  -995,   134,
    -995,   130,  -995,    41,  -995,  -995,  -995,   900,  1095,  -995,
    -995,   899,  -995,   903,  -995,   904,  1028,   481,  -995,   109,
     109,   481,   905,   109,   481,   481,   906,   908,  -995,  -995,
    -995,  -995,   909,  -995,  -995,  -995,   912,  -995,   913,   914,
     915,  -995,   916,   917,   918,   919,   920,   921,   922,  -995,
     923,  -995,   924,  -995,  -995,  -995,   925,  -995,  -995,  -995,
    -995,  -995,   926,  -995,   927,   928,    51,  1099,   931,  -995,
     -40,  1105,   932,   942,  1108,   115,   269,   965,  -114,  1048,
     934,    -9,  -995,   929,  1050,  -995,   109,   936,   -96,  1076,
     332,   940,  1047,   941,    -5,  1051,   278,   411,  1052,   128,
    -995,  -995,   204,   943,   101,   139,   189,   822,  -995,   -68,
    -995,  -995,   204,   359,   171,  -995,   177,  -995,   190,  -995,
     101,   945,  -995,  -995,   109,  -995,   944,   212,   510,   947,
     101,   684,   948,   949,   109,  -995,   109,   129,  1044,  1049,
     109,  1053,  1043,   109,   109,   109,   -96,   109,   109,   109,
     269,  -995,   818,   347,  -995,   953,  1153,   955,   956,  1157,
     958,   957,  -995,  -995,   960,  -995,   962,  -995,  1160,  -995,
     340,   964,  -995,  -995,   966,    80,   359,   967,   969,   514,
    -995,  1164,  -995,  1167,   955,  -995,  -995,   970,  -995,  -995,
    -995,   971,   176,  -995,  -995,   359,  -995,  -995,  -995,  -995,
     411,   134,  -995,   130,  -995,    41,  -995,   972,  1170,   204,
    -995,  1136,  -995,   109,  -995,   973,  -995,  -995,  -995,   440,
    -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,
     359,  -995,  -995,  -995,  -995,    10,  -995,  -995,  -995,   -96,
    -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,   976,   977,
     978,  -995,  -995,   979,  -995,  -995,  -995,   359,   980,  -995,
    -995,   982,  -995,   983,  -995,  -995,  1174,  -995,   985,   354,
    1086,  1185,   987,   269,  -995,    -6,   269,   986,   988,    -5,
     109,  -995,  -995,   990,  1127,  1104,  -995,   991,   993,   994,
     995,   996,   997,   998,  -995,  -995,   999,  1000,  1001,  -995,
    1002,  1003,  1086,    80,  1200,    82,  1005,  1004,   176,  -995,
    -995,  -995,  -995,  -995,  1007,  1006,   490,  -995,   109,  1132,
     359,   109,   109,   109,   -91,   129,  1209,  1085,  -995,  1212,
    -995,  -995,  -995,  -995,  1014,  1015,  1122,  1017,  1217,  -995,
    1020,  1018,  -995,  -995,   202,   955,  -995,  -995,  1022,  1023,
    1129,  1223,    31,   269,   129,    21,    21,   332,   -41,  1224,
    -995,  1225,  1122,  -995,  -995,  1027,  -995,  -995,  -995,  1227,
    -995,  1190,  -995,   279,  -995,  -995,  -995,  -995,  -995,   974,
    -995,  -995,  -995,  -995,  1029,   354,  1135,  1030,  1068,  1233,
    1033,  1072,   269,  1035,  1086,  1152,  1154,  -995,  -995,  -995,
    -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,
    -995,  -995,  1038,  1068,   928,  -995,   781,  1042,  1040,  1046,
    1045,   -91,  -995,  -995,  -995,  -995,  1129,  1054,  -995,  1055,
    -995,  1056,  -995,  -995,  1122,  1059,  -995,  -995,  -995,   781,
    -995,   101,  -995,  -995,  1060,  -995,  -995,   269,   411,  -995,
     269,  1068,  1159,   411,  -995,  1061,   332,  -995,  -995,  -995,
    -995,  -995,  -995,  -995,   103,  1062,   411,  -995,  -995,   953,
     332,  1063,  -995,  -995,  -995,  -995
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -995,  -995,  1244,  -995,  -995,  -995,  -995,  -995,  -995,  -995,
    -995,   131,  -995,  -995,  1011,  -103,  -358,   847,  1037,  1186,
    -479,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,
    -995,  -995,  -995,  -995,  -995,  -995,  -995,  1194,  -995,  -995,
    -995,   301,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,
    -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,
    -995,   796,  1057,  -995,  -995,  -995,  -995,   722,  -995,  -995,
    -995,   337,  -995,  -995,  -995,  -586,   317,   258,  -995,  -995,
     452,   280,  -995,  -995,  -995,  -995,  -995,   205,  -995,  -995,
    1097,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,
    -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,
    -995,  -995,  -995,  -570,  -995,  -995,  -995,  -995,  -995,  -995,
    -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,  -995,
    -995,  -995,  -995,  -995,  -995,  -995,  -995,   267,   557,  -800,
     243,  -995,  -910,  -995,  -945,   203,  -994,  -452,  -587,  -995,
    -995,  -995,   478,   870,  -216,  -171,  -376,   754,   326,  -384,
    -472,  -604,  -510,  -600,  -508,  -571,  -166,   -72,  -995,   526,
    -995,  -995,   780,  -995,  -995,  1010,  -146,   727,  -995,  -461,
    -995,  -995,  -995,  -995,  -995,  -170,  -995
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -230
static const yytype_int16 yytable[] =
{
     236,   263,   367,   246,   482,   233,   732,   652,   650,   308,
     309,   600,   618,   619,   513,   632,   323,   324,   228,   326,
     328,   329,   512,   228,   524,   336,   337,   338,   281,   615,
     344,   318,   626,   347,   952,  1050,   311,   355,   302,   332,
     590,   545,  1012,   679,   545,   606,   654,   680,   681,   769,
     251,   767,   370,   371,   372,   373,   345,   346,   610,   348,
     349,    16,   359,   356,   613,   358,   399,  1063,   363,  1088,
     819,   401,   254,   255,   832,   228,  1028,     7,   765,   375,
     775,   990,   560,   350,   931,   563,   350,   351,   352,   809,
     351,   352,   772,   254,   255,     8,   682,   425,   426,   350,
     428,   312,   507,   351,   352,     9,   375,  1111,  1117,    13,
     683,   684,   685,   228,   686,   242,   687,    11,   254,   255,
    1029,  1051,   305,  1052,   313,   314,   243,   244,   343,    14,
     306,   545,   282,   293,  1084,   806,   360,   282,   772,  1101,
     252,   688,   689,   690,   318,   691,    15,   293,   692,   693,
     694,   695,   696,   489,   697,   166,   607,   376,   614,   932,
     838,  1053,   991,   386,   242,   402,   591,   843,   254,   255,
     303,   333,   169,   571,   353,   243,   244,   170,   698,   699,
     572,   319,  1118,   857,   611,   228,   167,   700,   701,   702,
     703,   704,   705,   865,   812,  1044,   820,   602,   259,   218,
     833,   813,   603,   630,   390,   631,   391,   706,   283,   294,
     392,   393,   394,   283,   969,   171,   815,   724,   844,   397,
     172,   707,   284,   294,   229,   242,   525,   284,     1,     2,
     242,   641,   514,   258,   230,   231,   243,   244,   642,   230,
     231,   243,   244,   617,   259,   227,   617,   653,   382,   383,
     384,   962,   282,   961,   597,   672,   418,   760,   543,   544,
     254,   255,    77,   750,   804,   260,   261,   761,   548,   549,
     232,   530,   293,   564,   565,   232,   229,   841,   632,   871,
     960,   542,   173,   258,   341,   552,   533,   534,   585,   555,
     884,   230,   231,   589,   327,   232,   488,   594,   595,   596,
     566,   174,   601,   229,   520,   497,   498,   395,   499,   500,
     501,   502,   503,   622,   229,   260,   261,   175,   230,   231,
     176,   955,   418,   956,   627,   376,   515,   232,   283,   230,
     231,   254,   255,   617,   285,   295,   643,   235,   521,   285,
     286,   296,   284,   644,   645,   286,   177,   532,   294,   295,
     242,   646,   178,   651,   232,   296,   540,   836,   548,   549,
     639,   243,   244,   287,   288,   232,   558,   664,   665,   179,
     562,   180,   647,   396,   851,   258,   297,   298,   570,   648,
     853,   852,   247,   248,   249,   250,   397,   854,   709,   839,
     398,   181,   242,   855,   980,   981,   982,   254,   255,   182,
     856,   230,   231,   243,   244,   846,   847,   260,   261,   313,
     314,   254,   255,   989,   183,   861,   992,   579,   862,   580,
     581,   545,   908,   256,   257,   546,   184,   380,   381,   382,
     383,   384,   386,   623,   185,  1057,  1058,   232,   186,   266,
     267,   268,   269,   270,   187,   271,   258,   272,   273,   274,
     275,   276,   909,   910,   285,   188,   237,   277,   649,   911,
     286,   380,   381,   382,   383,   384,   189,  1031,   912,   380,
     381,   382,   383,   384,   295,   190,   913,   191,   260,   261,
     296,   763,  1069,   287,   288,   -17,   192,   193,   278,   194,
     195,   196,   959,  1055,  1015,   197,  1056,   254,   255,   198,
     914,   297,   298,   199,  1105,   266,   267,   268,   269,   270,
     317,   271,   258,   272,   273,   274,   275,   276,   330,   331,
     824,   776,   240,   277,   200,   779,   258,   201,   782,   783,
     265,   202,  1082,   203,   204,   205,   241,   259,   382,   383,
     384,   364,   325,   206,   260,   261,   540,   254,   255,   207,
     208,   365,   752,   209,   278,   210,   211,   212,   260,   261,
     254,   255,   214,   400,   215,   378,   216,   217,   223,   828,
     870,   224,   826,   762,   874,   225,   322,   877,   878,   879,
     496,   881,   882,   883,   341,   226,   238,  1107,   379,   845,
    1110,   369,   423,   424,   374,   254,   255,   777,   778,   256,
     257,   781,   377,   936,   368,   266,   267,   268,   269,   270,
     937,   271,   258,   272,   273,   274,   275,   276,   254,   255,
     407,   410,   411,   547,   412,   413,   938,   414,   415,   418,
     880,   380,   381,   382,   383,   384,   907,   380,   381,   382,
     383,   384,   417,   939,   260,   261,   419,   420,   548,   549,
     421,   427,   435,   429,   278,   266,   267,   268,   269,   270,
     430,   271,   258,   272,   273,   274,   275,   276,   266,   267,
     268,   269,   270,   277,   271,   258,   272,   273,   274,   275,
     276,   431,   940,   941,   942,   943,   277,   944,   945,   946,
     947,   948,   433,  1021,   260,   261,  1022,   380,   968,   382,
     383,   384,   859,   436,   278,   473,   310,   260,   261,   437,
     258,   438,   869,   863,   996,   441,   442,   278,   444,   449,
    -229,   259,   453,   970,   445,   446,   266,   267,   268,   269,
     270,   636,   271,   258,   272,   273,   274,   275,   276,   447,
    1108,   448,   260,   261,   277,  1113,   451,   380,   381,   382,
     383,   384,   450,   452,   454,  1025,  1026,  1027,  1122,   380,
     381,   382,   383,   384,   455,   260,   261,   380,   381,   382,
     383,   384,   456,   457,   458,   278,   459,   460,   461,   462,
     380,   381,   382,   383,   384,   463,   464,    16,   465,   467,
     468,   966,   469,   470,   471,    17,   472,   474,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
     637,   475,   476,    30,    31,    32,    33,    34,   656,    35,
     477,   478,   479,   662,    36,    37,    38,    39,    40,    41,
      42,    43,   678,    44,    45,    46,  1059,    47,   713,    48,
      49,    50,    51,    52,    53,   480,   481,   484,    54,    55,
     485,    56,   486,   487,   490,   491,   492,    57,   493,   380,
     381,   382,   383,   384,   495,   504,   505,   380,   381,   382,
     383,   384,   380,   381,   382,   383,   384,   506,    58,   719,
     516,   380,   381,   382,   383,   384,   517,   380,   381,   382,
     383,   384,   866,   518,   523,    59,  1023,   527,   528,   885,
     886,    60,   529,   531,    61,    62,   536,    63,   537,    64,
     538,   539,   557,   553,   556,    65,   554,   559,    66,    67,
     561,   567,    68,    69,   569,    70,   568,    71,   380,   381,
     382,   383,   384,   574,   575,  1115,   887,   888,   576,   577,
     889,   380,   381,   382,   383,   384,   578,   584,   586,  1124,
     587,   588,   593,   605,   598,   608,   609,  -178,   628,    72,
      73,    74,    75,  1090,    76,   620,   625,   629,   640,   890,
     655,   657,   891,   892,   893,   894,   895,   896,   897,   898,
     899,   900,   901,   902,   903,   661,  1104,   658,    77,   666,
     667,   660,   668,   718,    78,    79,   669,   670,   671,   673,
     674,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,   675,
     676,   677,   712,   722,   716,   728,   717,   720,   721,   242,
     730,   723,   737,   724,   725,   743,    79,   726,   740,   727,
     243,   244,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
     219,    81,   729,    83,   747,    85,    80,    87,    82,    89,
      84,    91,    86,    93,    88,    95,    90,    97,    92,   731,
      94,   733,    96,   734,   736,   738,   739,   742,   744,   745,
     746,   672,   748,   749,   758,   754,   755,   759,   771,   756,
     751,   757,   312,   770,   805,   772,   540,   774,   780,   808,
     810,   784,   811,   786,   785,   773,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,   797,   798,   799,   800,
     802,   803,   801,   816,   806,   809,   817,   818,   823,   825,
     822,   827,   830,   829,   831,   872,   835,   840,   858,   873,
     860,   876,   842,   864,   867,   868,   916,   918,   919,   921,
     875,   922,   923,   925,   927,   924,   926,   929,   950,   930,
     934,   951,   935,   953,   954,   964,   965,   978,   963,   967,
     971,   972,   973,   974,   975,   985,   976,   977,   979,   987,
     988,   994,   993,   998,   999,  1000,   997,  1001,  1002,  1003,
    1004,  1005,  1006,  1007,  1008,  1009,  1011,  1014,  1010,  1016,
    1019,  1024,  1020,  1032,  1017,  1033,  1034,  1035,  1037,  1036,
    1039,  1040,  1042,  1041,  1047,  1045,  1046,  1049,  1061,  1062,
    1068,  1065,  1067,  1074,  1076,  1072,  1077,  1079,  1071,  1080,
    1081,  1083,  1085,  1086,  1087,  1091,  1092,  1093,  1112,    10,
    1123,  1094,   409,   519,   366,   220,   213,   663,  1097,  1018,
    1100,  1099,  1102,  1106,  1114,  1120,   995,   357,   599,  1125,
    1013,  1064,   928,   292,  1043,  1060,  1095,   807,  1073,  1096,
     906,   522,   659,   849,   638,   432,   710
};

static const yytype_uint16 yycheck[] =
{
     103,   172,   218,   169,   362,    77,   593,   517,   516,   180,
     181,   472,   484,   485,   398,   494,   187,   188,     4,   190,
     191,   191,   398,     4,     3,   195,   196,   197,   174,   481,
     201,    55,    79,   204,   834,     4,   182,   208,    79,    79,
      79,     3,   952,     4,     3,    64,   518,     8,     9,   653,
       4,   651,   223,   224,   225,   226,   202,   203,    79,   205,
     206,     6,     5,   209,     4,   211,   237,  1012,   214,  1063,
      79,   237,    79,    80,    79,     4,   167,   204,   649,     8,
     666,    87,   440,     4,     4,   443,     4,     8,     9,   203,
       8,     9,   206,    79,    80,   204,    57,   268,   269,     4,
     271,   205,     4,     8,     9,     0,     8,  1101,     5,   211,
      71,    72,    73,     4,    75,   211,    77,   116,    79,    80,
     211,    90,    71,    92,   228,   229,   222,   223,   200,   211,
      79,     3,     3,     3,  1044,   203,    79,     3,   206,  1084,
      94,   102,   103,   104,    55,   106,   204,     3,   109,   110,
     111,   112,   113,   369,   115,   203,   175,   229,    98,    79,
     746,   130,   168,   235,   211,   237,   205,   754,    79,    80,
     211,   211,   204,    72,    79,   222,   223,   204,   139,   140,
      79,   205,    79,   770,   205,     4,   117,   148,   149,   150,
     151,   152,   153,   780,    79,   995,   205,    74,   205,   207,
     205,    86,    79,   133,    23,   135,    25,   168,    79,    79,
      29,    30,    31,    79,   204,   204,   726,   207,    79,   205,
     204,   182,    93,    79,   205,   211,   205,    93,    15,    16,
     211,   203,   398,   194,   220,   221,   222,   223,   210,   220,
     221,   222,   223,   205,   205,   213,   205,   209,   259,   260,
     261,   855,     3,   853,   470,     3,     4,   641,   429,   430,
      79,    80,   207,   621,   716,   226,   227,   643,   230,   231,
     256,   417,     3,   444,   445,   256,   205,   749,   757,   787,
     851,   427,   204,   194,   205,   431,    84,    85,   459,   435,
     800,   220,   221,   463,   205,   256,   368,   467,   468,   469,
     446,   204,   473,   205,   407,   377,   378,   126,   380,   381,
     382,   383,   384,   143,   205,   226,   227,   204,   220,   221,
     204,   145,     4,   147,   490,   397,   398,   256,    79,   220,
     221,    79,    80,   205,   205,   205,   203,   205,   410,   205,
     211,   211,    93,   210,   203,   211,   204,   419,    79,   205,
     211,   210,   204,   209,   256,   211,    78,    79,   230,   231,
     506,   222,   223,   234,   235,   256,   438,   538,   539,   204,
     442,   204,   203,   192,   203,   194,   232,   233,   450,   210,
     203,   210,    10,    11,    12,    13,   205,   210,   559,   747,
     209,   204,   211,   203,    40,    41,    42,    79,    80,   204,
     210,   220,   221,   222,   223,   216,   217,   226,   227,   228,
     229,    79,    80,   923,   204,   203,   926,    79,   206,    81,
      82,     3,    75,    83,    84,     7,   204,   257,   258,   259,
     260,   261,   504,   263,   204,  1005,  1006,   256,   204,   187,
     188,   189,   190,   191,   204,   193,   194,   195,   196,   197,
     198,   199,   105,   106,   205,   204,   262,   205,   209,   112,
     211,   257,   258,   259,   260,   261,   204,   975,   121,   257,
     258,   259,   260,   261,   205,   204,   129,   204,   226,   227,
     211,   647,   203,   234,   235,   206,   204,   204,   236,   204,
     204,   204,   850,  1003,   955,   204,  1004,    79,    80,   204,
     153,   232,   233,   204,  1091,   187,   188,   189,   190,   191,
     184,   193,   194,   195,   196,   197,   198,   199,   192,   193,
     736,   667,     3,   205,   204,   671,   194,   204,   674,   675,
      98,   204,  1042,   204,   204,   204,   203,   205,   259,   260,
     261,   215,    91,   204,   226,   227,    78,    79,    80,   204,
     204,   135,   624,   204,   236,   204,   204,   204,   226,   227,
      79,    80,   204,   237,   204,   185,   204,   204,   204,   740,
     786,   204,   738,   645,   790,   204,   211,   793,   794,   795,
     206,   797,   798,   799,   205,   204,   204,  1097,   208,   755,
    1100,   207,   266,   267,   205,    79,    80,   669,   670,    83,
      84,   673,   207,    89,   262,   187,   188,   189,   190,   191,
      96,   193,   194,   195,   196,   197,   198,   199,    79,    80,
     209,   207,   263,   205,   203,   203,   112,   203,   202,     4,
     796,   257,   258,   259,   260,   261,   802,   257,   258,   259,
     260,   261,   205,   129,   226,   227,   207,   203,   230,   231,
     203,   205,   265,   205,   236,   187,   188,   189,   190,   191,
     205,   193,   194,   195,   196,   197,   198,   199,   187,   188,
     189,   190,   191,   205,   193,   194,   195,   196,   197,   198,
     199,   205,   168,   169,   170,   171,   205,   173,   174,   175,
     176,   177,   205,   203,   226,   227,   206,   257,   258,   259,
     260,   261,   774,   203,   236,   203,   167,   226,   227,     3,
     194,   207,   784,   203,   930,     3,   207,   236,   203,     4,
     209,   205,   205,   889,   203,   203,   187,   188,   189,   190,
     191,   208,   193,   194,   195,   196,   197,   198,   199,   203,
    1098,   203,   226,   227,   205,  1103,   203,   257,   258,   259,
     260,   261,   207,   203,     4,   971,   972,   973,  1116,   257,
     258,   259,   260,   261,   203,   226,   227,   257,   258,   259,
     260,   261,   203,   203,   203,   236,   203,   203,     4,   203,
     257,   258,   259,   260,   261,   203,   203,     6,     6,   203,
     203,   863,   203,   203,     4,    14,   203,   203,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
     208,   203,   203,    32,    33,    34,    35,    36,   208,    38,
     203,   203,   203,   208,    43,    44,    45,    46,    47,    48,
      49,    50,   208,    52,    53,    54,  1007,    56,   208,    58,
      59,    60,    61,    62,    63,   203,   203,   203,    67,    68,
     203,    70,   203,   208,   203,   203,   203,    76,   203,   257,
     258,   259,   260,   261,   206,   203,   206,   257,   258,   259,
     260,   261,   257,   258,   259,   260,   261,   204,    97,   208,
     204,   257,   258,   259,   260,   261,   204,   257,   258,   259,
     260,   261,   208,   204,     3,   114,   968,     3,   203,    81,
      82,   120,   208,   203,   123,   124,     5,   126,   212,   128,
     203,   203,   203,   200,    94,   134,   206,   203,   137,   138,
     203,    78,   141,   142,   203,   144,    78,   146,   257,   258,
     259,   260,   261,    37,     4,  1106,   118,   119,   203,    37,
     122,   257,   258,   259,   260,   261,    39,   168,    88,  1120,
     203,    88,   203,    92,   203,   172,    98,   125,   131,   178,
     179,   180,   181,  1066,   183,   136,   208,   132,   203,   151,
     210,     3,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,     4,  1089,   203,   207,   264,
     203,   206,   206,     4,   213,   214,   203,   203,   203,   207,
     203,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   203,
     203,   211,   211,     4,   203,     4,   203,   203,   203,   211,
       4,   203,    78,   207,   203,     4,   214,   204,   204,   203,
     222,   223,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     214,   221,   203,   223,   125,   225,   220,   227,   222,   229,
     224,   231,   226,   233,   228,   235,   230,   237,   232,   203,
     234,   203,   236,   203,   203,   203,   203,   203,   203,   203,
     203,     3,   203,   203,   206,   203,   203,     4,     3,   204,
     263,   203,   205,   203,     5,   206,    78,   203,   203,     4,
     168,   205,     4,   204,   206,   212,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     203,   203,   206,   168,   203,   203,    88,   203,    88,   203,
     211,    65,    95,   203,   203,   101,    95,    95,   203,   100,
     206,   108,   209,   206,   206,   206,   203,     4,   203,   203,
     107,     4,   204,   203,     4,   208,   204,   203,     4,   203,
     203,     4,   203,   203,   203,     5,    40,     3,   206,   206,
     204,   204,   204,   204,   204,    99,   204,   204,   203,     4,
     203,   203,   206,    66,    90,   204,   206,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   203,     7,   206,   204,
     203,    79,   206,     4,   210,   130,     4,   203,    96,   204,
     203,     4,   204,   203,    95,   203,   203,     4,     4,     4,
      40,   204,     5,    98,   204,   206,   168,     4,   264,   206,
     168,   206,    90,    89,   206,   203,   206,   201,    89,     5,
    1119,   206,   241,   406,   217,    69,    62,   535,   204,   958,
     204,   206,   203,   203,   203,   203,   929,   210,   472,   206,
     953,  1013,   820,   176,   994,  1008,  1071,   720,  1035,  1076,
     802,   411,   528,   757,   504,   275,   559
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   267,   268,   269,   270,   204,   204,     0,
     268,   116,   279,   211,   211,   204,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      32,    33,    34,    35,    36,    38,    43,    44,    45,    46,
      47,    48,    49,    50,    52,    53,    54,    56,    58,    59,
      60,    61,    62,    63,    67,    68,    70,    76,    97,   114,
     120,   123,   124,   126,   128,   134,   137,   138,   141,   142,
     144,   146,   178,   179,   180,   181,   183,   207,   213,   214,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   271,   272,
     273,   274,   281,   283,   284,   285,   286,   288,   289,   297,
     300,   301,   303,   304,   311,   313,   314,   316,   318,   320,
     324,   325,   326,   329,   331,   338,   345,   348,   354,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   370,
     371,   372,   373,   377,   378,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   391,   392,   393,   394,   395,   396,
     397,   398,   399,   400,   401,   402,   203,   117,   280,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   303,   204,   204,   204,   204,   207,   214,
     285,   287,   305,   204,   204,   204,   204,   213,     4,   205,
     220,   221,   256,   433,   443,   205,   281,   262,   204,   315,
       3,   203,   211,   222,   223,   419,   432,    10,    11,    12,
      13,     4,    94,   444,    79,    80,    83,    84,   194,   205,
     226,   227,   346,   421,   422,    98,   187,   188,   189,   190,
     191,   193,   195,   196,   197,   198,   199,   205,   236,   421,
     441,   442,     3,    79,    93,   205,   211,   234,   235,   356,
     430,   431,   356,     3,    79,   205,   211,   232,   233,   350,
     428,   429,    79,   211,   403,    71,    79,   413,   421,   421,
     167,   442,   205,   228,   229,   424,   425,   424,    55,   205,
     451,   452,   211,   421,   421,    91,   421,   205,   421,   451,
     424,   424,    79,   211,   328,   404,   451,   451,   451,   420,
     433,   205,   327,   433,   421,   442,   442,   421,   442,   442,
       4,     8,     9,    79,   445,   421,   442,   328,   442,     5,
      79,   339,   321,   442,   424,   135,   284,   420,   262,   207,
     421,   421,   421,   421,   205,     8,   433,   207,   185,   208,
     257,   258,   259,   260,   261,   432,   433,   438,   439,   440,
      23,    25,    29,    30,    31,   126,   192,   205,   209,   421,
     424,   432,   433,   447,   448,   449,   319,   209,   282,   280,
     207,   263,   203,   203,   203,   202,   423,   205,     4,   207,
     203,   203,   333,   424,   424,   421,   421,   205,   421,   205,
     205,   205,   441,   205,   442,   265,   203,     3,   207,   357,
     355,     3,   207,   349,   203,   203,   203,   203,   203,     4,
     207,   203,   203,   205,     4,   203,   203,   203,   203,   203,
     203,     4,   203,   203,   203,     6,   302,   203,   203,   203,
     203,     4,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   203,   282,   322,   203,   203,   203,   208,   433,   420,
     203,   203,   203,   203,   298,   206,   206,   433,   433,   433,
     433,   433,   433,   433,   203,   206,   204,     4,   293,   294,
     295,   296,   422,   425,   432,   433,   204,   204,   204,   283,
     281,   433,   419,     3,     3,   205,   379,     3,   203,   208,
     442,   203,   433,    84,    85,   347,     5,   212,   203,   203,
      78,   341,   442,   421,   421,     3,     7,   205,   230,   231,
     426,   427,   442,   200,   206,   442,    94,   203,   433,   203,
     282,   203,   433,   282,   421,   421,   442,    78,    78,   203,
     433,    72,    79,   414,    37,     4,   203,    37,    39,    79,
      81,    82,   415,   418,   168,   421,    88,   203,    88,   451,
      79,   205,   334,   203,   451,   451,   451,   420,   203,   327,
     445,   421,    74,    79,   450,    92,    64,   175,   172,    98,
      79,   205,   335,     4,    98,   413,   323,   205,   426,   426,
     136,   317,   143,   263,   312,   208,    79,   432,   131,   132,
     133,   135,   286,   435,   436,   437,   208,   208,   438,   442,
     203,   203,   210,   203,   210,   203,   210,   203,   210,   209,
     430,   209,   428,   209,   426,   210,   208,     3,   203,   423,
     206,     4,   208,   333,   421,   421,   264,   203,   206,   203,
     203,   203,     3,   207,   203,   203,   203,   211,   208,     4,
       8,     9,    57,    71,    72,    73,    75,    77,   102,   103,
     104,   106,   109,   110,   111,   112,   113,   115,   139,   140,
     148,   149,   150,   151,   152,   153,   168,   182,   358,   421,
     443,   446,   211,   208,   351,   343,   203,   203,     4,   208,
     203,   203,     4,   203,   207,   203,   204,   203,     4,   203,
       4,   203,   414,   203,   203,   369,   203,    78,   203,   203,
     204,   374,   203,     4,   203,   203,   203,   125,   203,   203,
     282,   263,   433,   306,   203,   203,   204,   203,   206,     4,
     425,   422,   433,   432,   290,   431,   291,   429,   292,   427,
     203,     3,   206,   212,   203,   341,   442,   433,   433,   442,
     203,   433,   442,   442,   205,   206,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   206,   203,   203,   413,     5,   203,   404,     4,   203,
     168,     4,    79,    86,   417,   428,   168,    88,   203,    79,
     205,   336,   211,    88,   420,   203,   432,    65,   421,   203,
      95,   203,    79,   205,   337,    95,    79,   340,   341,   282,
      95,   426,   209,   414,    79,   432,   216,   217,   434,   435,
     299,   203,   210,   203,   210,   203,   210,   414,   203,   433,
     206,   203,   206,   203,   206,   414,   208,   206,   206,   433,
     420,   430,   101,   100,   420,   107,   108,   420,   420,   420,
     432,   420,   420,   420,   428,    81,    82,   118,   119,   122,
     151,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   352,   416,   418,   432,    75,   105,
     106,   112,   121,   129,   153,   344,   203,   277,     4,   203,
     405,   203,     4,   204,   208,   203,   204,     4,   346,   203,
     203,     4,    79,   342,   203,   203,    89,    96,   112,   129,
     168,   169,   170,   171,   173,   174,   175,   176,   177,   375,
       4,     4,   405,   203,   203,   145,   147,   307,   308,   282,
     431,   429,   427,   206,     5,    40,   433,   206,   258,   204,
     432,   204,   204,   204,   204,   204,   204,   204,     3,   203,
      40,    41,    42,   406,   407,    99,   408,     4,   203,   428,
      87,   168,   428,   206,   203,   337,   420,   206,    66,    90,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     206,   203,   408,   342,     7,   445,   204,   210,   307,   203,
     206,   203,   206,   433,    79,   420,   420,   420,   167,   211,
     353,   430,     4,   130,     4,   203,   204,    96,   410,   203,
       4,   203,   204,   347,   405,   203,   203,    95,   411,     4,
       4,    90,    92,   130,   376,   428,   430,   379,   379,   421,
     403,     4,     4,   410,   343,   204,   310,     5,    40,   203,
     275,   264,   206,   406,    98,   409,   204,   168,   412,     4,
     206,   168,   428,   206,   408,    90,    89,   206,   412,   309,
     281,   203,   206,   201,   206,   353,   411,   204,   389,   206,
     204,   410,   203,   330,   281,   414,   203,   428,   282,   390,
     428,   412,    89,   282,   203,   421,   332,     5,    79,   278,
     203,   276,   282,   277,   421,   206
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
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_BOUNDARY_TYPE));
		  ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 425 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 429 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 435 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_COPY));
		      (yyval.i) = 0;
		  ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 440 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 446 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 450 "lev_comp.y"
    {
		      (yyval.i) = what_map_char((char) (yyvsp[(2) - (2)].i));
		  ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 461 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 465 "lev_comp.y"
    {
		      add_opvars(splev, "io",
                                 VA_PASS2((int) (yyvsp[(3) - (3)].i), SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 472 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 476 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 482 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 486 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 492 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (3)].i);
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 592 "lev_comp.y"
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

  case 117:

/* Line 1455 of yacc.c  */
#line 608 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 614 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 620 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 626 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 632 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 638 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 644 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 650 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 656 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 666 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 676 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 686 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 696 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 706 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 716 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 728 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 733 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 740 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 745 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 752 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 757 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 764 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 768 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 774 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 779 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 786 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 790 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 796 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 800 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 806 "lev_comp.y"
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

  case 147:

/* Line 1455 of yacc.c  */
#line 828 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 832 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 843 "lev_comp.y"
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

  case 150:

/* Line 1455 of yacc.c  */
#line 898 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 904 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 908 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 914 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 921 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 925 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 933 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 937 "lev_comp.y"
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

  case 158:

/* Line 1455 of yacc.c  */
#line 960 "lev_comp.y"
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

  case 161:

/* Line 1455 of yacc.c  */
#line 1005 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1015 "lev_comp.y"
    {
		  ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1018 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1028 "lev_comp.y"
    {
		  ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1033 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1047 "lev_comp.y"
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

  case 169:

/* Line 1455 of yacc.c  */
#line 1084 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1089 "lev_comp.y"
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

  case 171:

/* Line 1455 of yacc.c  */
#line 1118 "lev_comp.y"
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

  case 172:

/* Line 1455 of yacc.c  */
#line 1132 "lev_comp.y"
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

  case 173:

/* Line 1455 of yacc.c  */
#line 1148 "lev_comp.y"
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

  case 174:

/* Line 1455 of yacc.c  */
#line 1168 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1180 "lev_comp.y"
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

  case 176:

/* Line 1455 of yacc.c  */
#line 1200 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1206 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1216 "lev_comp.y"
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

  case 179:

/* Line 1455 of yacc.c  */
#line 1234 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1244 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1250 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1255 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1260 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1267 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1274 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1283 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1291 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1302 "lev_comp.y"
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

  case 189:

/* Line 1455 of yacc.c  */
#line 1318 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1325 "lev_comp.y"
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

  case 191:

/* Line 1455 of yacc.c  */
#line 1341 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1348 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1352 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1358 "lev_comp.y"
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

  case 195:

/* Line 1455 of yacc.c  */
#line 1368 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1374 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1383 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1389 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1394 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1400 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1405 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1411 "lev_comp.y"
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

  case 203:

/* Line 1455 of yacc.c  */
#line 1422 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1436 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1440 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1450 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1457 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1465 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1470 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1475 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1480 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1485 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1490 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1495 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1503 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1510 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1518 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1534 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1538 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1544 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1552 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1558 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1566 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1574 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1579 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1585 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1591 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1597 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1603 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1608 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(0, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1613 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1618 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1623 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1628 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1633 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1638 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1643 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1648 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1653 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1658 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1664 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1669 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_PROTECTOR));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1676 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1685 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1689 "lev_comp.y"
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

  case 256:

/* Line 1455 of yacc.c  */
#line 1702 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1710 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1720 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1728 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1737 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1744 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1752 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1758 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1763 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1768 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1773 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1778 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1783 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1788 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1793 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1798 "lev_comp.y"
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

  case 272:

/* Line 1455 of yacc.c  */
#line 1812 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1818 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1823 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1828 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1833 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1838 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1843 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1848 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1853 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1858 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1863 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1868 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1873 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x400000;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1878 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x800000;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1883 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x1000000;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1888 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x2000000;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1893 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x4000000;
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1898 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x8000000;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1905 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1911 "lev_comp.y"
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

  case 292:

/* Line 1455 of yacc.c  */
#line 1940 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1945 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1953 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1959 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1963 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1971 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1978 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1985 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1995 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2005 "lev_comp.y"
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

  case 302:

/* Line 1455 of yacc.c  */
#line 2020 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2030 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2034 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2040 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2046 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2052 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2056 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2062 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2068 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2075 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2083 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2089 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2094 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2099 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2104 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2109 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2115 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2121 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2126 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2131 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2136 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2141 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2146 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2156 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2162 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2168 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2173 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2180 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2187 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2191 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2197 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2203 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2209 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2214 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2221 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2227 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2237 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2243 "lev_comp.y"
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

  case 344:

/* Line 1455 of yacc.c  */
#line 2262 "lev_comp.y"
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

  case 345:

/* Line 1455 of yacc.c  */
#line 2274 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2278 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2284 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2289 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2294 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (long)(yyvsp[(9) - (9)].i), (long)(yyvsp[(7) - (9)].i), (long)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2299 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((long)(yyvsp[(9) - (13)].i), (long)(yyvsp[(7) - (13)].i), (long)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2305 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2309 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2315 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2321 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2325 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2331 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2335 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2341 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2345 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2350 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2357 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2361 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2368 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2372 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2377 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2384 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2388 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2393 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2401 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2407 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2414 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2418 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2425 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2436 "lev_comp.y"
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

  case 377:

/* Line 1455 of yacc.c  */
#line 2449 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2453 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2459 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2463 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2470 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2474 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2478 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2484 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2488 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2494 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2500 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2504 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2510 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2516 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2521 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2538 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2546 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2556 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2565 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2570 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2577 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2588 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2594 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2598 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2602 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2609 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2619 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2626 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2630 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2636 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2640 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2648 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2652 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2659 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2669 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2682 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2686 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2693 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2703 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2712 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2723 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2727 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2734 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2744 "lev_comp.y"
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

  case 431:

/* Line 1455 of yacc.c  */
#line 2755 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2764 "lev_comp.y"
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

  case 433:

/* Line 1455 of yacc.c  */
#line 2774 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2780 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2784 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2791 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2801 "lev_comp.y"
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

  case 438:

/* Line 1455 of yacc.c  */
#line 2812 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2821 "lev_comp.y"
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

  case 440:

/* Line 1455 of yacc.c  */
#line 2831 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2835 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2841 "lev_comp.y"
    { ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2843 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2849 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2853 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2857 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2861 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2869 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2878 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2882 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2886 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2890 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2894 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2897 "lev_comp.y"
    { ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2901 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2908 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2917 "lev_comp.y"
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

  case 462:

/* Line 1455 of yacc.c  */
#line 2958 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2962 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2969 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2976 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2986 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2990 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2998 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 3002 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 3006 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 3010 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 3014 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3019 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3023 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3027 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3032 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3037 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3042 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3046 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3051 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3056 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3060 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3064 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3069 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3073 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3080 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3086 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3090 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3096 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3112 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3116 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3120 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3124 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3146 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3150 "lev_comp.y"
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

  case 509:

/* Line 1455 of yacc.c  */
#line 3176 "lev_comp.y"
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
#line 7193 "lev_comp.tab.c"
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
#line 3204 "lev_comp.y"


/*lev_comp.y*/

