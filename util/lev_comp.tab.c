
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
     WAITFORU_ID = 410,
     CANCELLED_ID = 411,
     REVIVED_ID = 412,
     AVENGE_ID = 413,
     FLEEING_ID = 414,
     BLINDED_ID = 415,
     PARALYZED_ID = 416,
     STUNNED_ID = 417,
     CONFUSED_ID = 418,
     SEENTRAPS_ID = 419,
     ALL_ID = 420,
     MONTYPE_ID = 421,
     OBJTYPE_ID = 422,
     TERTYPE_ID = 423,
     TERTYPE2_ID = 424,
     LEVER_EFFECT_TYPE = 425,
     SWITCHABLE_ID = 426,
     CONTINUOUSLY_USABLE_ID = 427,
     TARGET_ID = 428,
     TRAPTYPE_ID = 429,
     EFFECT_FLAG_ID = 430,
     GRAVE_ID = 431,
     BRAZIER_ID = 432,
     SIGNPOST_ID = 433,
     TREE_ID = 434,
     ERODEPROOF_ID = 435,
     FUNCTION_ID = 436,
     MSG_OUTPUT_TYPE = 437,
     COMPARE_TYPE = 438,
     UNKNOWN_TYPE = 439,
     rect_ID = 440,
     fillrect_ID = 441,
     line_ID = 442,
     randline_ID = 443,
     grow_ID = 444,
     selection_ID = 445,
     flood_ID = 446,
     rndcoord_ID = 447,
     circle_ID = 448,
     ellipse_ID = 449,
     filter_ID = 450,
     complement_ID = 451,
     gradient_ID = 452,
     GRADIENT_TYPE = 453,
     LIMITED = 454,
     HUMIDITY_TYPE = 455,
     STRING = 456,
     MAP_ID = 457,
     NQSTRING = 458,
     VARSTRING = 459,
     CFUNC = 460,
     CFUNC_INT = 461,
     CFUNC_STR = 462,
     CFUNC_COORD = 463,
     CFUNC_REGION = 464,
     VARSTRING_INT = 465,
     VARSTRING_INT_ARRAY = 466,
     VARSTRING_STRING = 467,
     VARSTRING_STRING_ARRAY = 468,
     VARSTRING_VAR = 469,
     VARSTRING_VAR_ARRAY = 470,
     VARSTRING_COORD = 471,
     VARSTRING_COORD_ARRAY = 472,
     VARSTRING_REGION = 473,
     VARSTRING_REGION_ARRAY = 474,
     VARSTRING_MAPCHAR = 475,
     VARSTRING_MAPCHAR_ARRAY = 476,
     VARSTRING_MONST = 477,
     VARSTRING_MONST_ARRAY = 478,
     VARSTRING_OBJ = 479,
     VARSTRING_OBJ_ARRAY = 480,
     VARSTRING_SEL = 481,
     VARSTRING_SEL_ARRAY = 482,
     METHOD_INT = 483,
     METHOD_INT_ARRAY = 484,
     METHOD_STRING = 485,
     METHOD_STRING_ARRAY = 486,
     METHOD_VAR = 487,
     METHOD_VAR_ARRAY = 488,
     METHOD_COORD = 489,
     METHOD_COORD_ARRAY = 490,
     METHOD_REGION = 491,
     METHOD_REGION_ARRAY = 492,
     METHOD_MAPCHAR = 493,
     METHOD_MAPCHAR_ARRAY = 494,
     METHOD_MONST = 495,
     METHOD_MONST_ARRAY = 496,
     METHOD_OBJ = 497,
     METHOD_OBJ_ARRAY = 498,
     METHOD_SEL = 499,
     METHOD_SEL_ARRAY = 500,
     DICE = 501
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
#line 544 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 556 "lev_comp.tab.c"

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
#define YYLAST   1287

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  264
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  187
/* YYNRULES -- Number of rules.  */
#define YYNRULES  507
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1124

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   501

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   259,   263,     2,
     203,   204,   257,   255,   201,   256,   261,   258,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   202,     2,
       2,   260,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   205,     2,   206,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   207,   262,   208,     2,     2,     2,     2,
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
     195,   196,   197,   198,   199,   200,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254
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
     748,   750,   752,   754,   756,   760,   764,   768,   770,   772,
     776,   778,   780,   782,   786,   790,   791,   797,   800,   801,
     805,   807,   811,   813,   817,   821,   823,   825,   829,   831,
     833,   835,   839,   841,   843,   845,   849,   853,   857,   861,
     865,   869,   873,   875,   877,   879,   883,   887,   891,   897,
     905,   911,   920,   922,   926,   930,   936,   942,   950,   958,
     965,   971,   972,   975,   981,   985,   997,  1011,  1018,  1019,
    1023,  1025,  1029,  1033,  1037,  1041,  1043,  1045,  1049,  1053,
    1057,  1061,  1065,  1069,  1071,  1073,  1075,  1077,  1081,  1085,
    1087,  1093,  1103,  1109,  1117,  1121,  1125,  1131,  1141,  1147,
    1153,  1159,  1160,  1174,  1175,  1177,  1185,  1197,  1207,  1221,
    1225,  1233,  1241,  1247,  1255,  1261,  1271,  1277,  1283,  1287,
    1295,  1299,  1307,  1315,  1319,  1325,  1331,  1335,  1341,  1349,
    1359,  1361,  1363,  1365,  1367,  1369,  1370,  1373,  1375,  1379,
    1381,  1383,  1385,  1386,  1390,  1392,  1393,  1397,  1399,  1400,
    1404,  1406,  1408,  1410,  1412,  1414,  1416,  1418,  1420,  1422,
    1426,  1428,  1430,  1435,  1437,  1439,  1444,  1446,  1448,  1453,
    1455,  1460,  1466,  1468,  1472,  1474,  1478,  1480,  1482,  1487,
    1497,  1499,  1501,  1506,  1508,  1514,  1516,  1518,  1523,  1525,
    1527,  1533,  1535,  1537,  1539,  1544,  1546,  1548,  1554,  1556,
    1558,  1560,  1564,  1566,  1568,  1572,  1574,  1579,  1583,  1587,
    1591,  1595,  1599,  1603,  1605,  1607,  1611,  1613,  1617,  1618,
    1620,  1622,  1624,  1626,  1630,  1631,  1633,  1635,  1638,  1641,
    1646,  1653,  1658,  1665,  1672,  1679,  1686,  1689,  1696,  1705,
    1714,  1725,  1740,  1743,  1745,  1749,  1751,  1755,  1757,  1759,
    1761,  1763,  1765,  1767,  1769,  1771,  1773,  1775,  1777,  1779,
    1781,  1783,  1785,  1787,  1789,  1791,  1793,  1804
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     265,     0,    -1,    -1,   266,    -1,   267,    -1,   267,   266,
      -1,   268,   277,   279,    -1,    16,   202,   209,    -1,    15,
     202,   209,   201,     3,    -1,    17,   202,    11,   201,   377,
      -1,    17,   202,    10,   201,     3,    -1,    17,   202,    13,
      -1,    17,   202,    12,   201,     3,   201,     3,   201,     5,
     201,     5,   201,   412,   201,   276,   275,    -1,    18,   202,
     442,    -1,   134,   202,   135,   201,   136,    -1,   134,   202,
     135,    -1,    21,   202,    98,    -1,    -1,   201,   199,    -1,
      -1,   201,   419,    -1,    -1,   201,     3,    -1,     5,    -1,
      79,    -1,    -1,   116,   202,   278,    -1,   117,   201,   278,
      -1,   117,    -1,    -1,   281,   279,    -1,   207,   279,   208,
      -1,   322,    -1,   269,    -1,   270,    -1,   385,    -1,   272,
      -1,   271,    -1,   392,    -1,   391,    -1,   389,    -1,   390,
      -1,   393,    -1,   394,    -1,   395,    -1,   396,    -1,   397,
      -1,   366,    -1,   324,    -1,   287,    -1,   286,    -1,   380,
      -1,   336,    -1,   358,    -1,   399,    -1,   400,    -1,   368,
      -1,   369,    -1,   370,    -1,   398,    -1,   302,    -1,   312,
      -1,   314,    -1,   318,    -1,   316,    -1,   299,    -1,   309,
      -1,   295,    -1,   298,    -1,   361,    -1,   371,    -1,   343,
      -1,   359,    -1,   346,    -1,   352,    -1,   381,    -1,   376,
      -1,   364,    -1,   323,    -1,   382,    -1,   383,    -1,   384,
      -1,   386,    -1,   329,    -1,   327,    -1,   375,    -1,   379,
      -1,   378,    -1,   362,    -1,   363,    -1,   365,    -1,   357,
      -1,   360,    -1,   219,    -1,   221,    -1,   223,    -1,   225,
      -1,   227,    -1,   229,    -1,   231,    -1,   233,    -1,   235,
      -1,   218,    -1,   220,    -1,   222,    -1,   224,    -1,   226,
      -1,   228,    -1,   230,    -1,   232,    -1,   234,    -1,   282,
      -1,   283,    -1,   212,    -1,   212,    -1,   283,    -1,   138,
     202,   282,    -1,   284,   260,   431,    -1,   284,   260,   190,
     202,   440,    -1,   284,   260,   430,    -1,   284,   260,   447,
     202,   424,    -1,   284,   260,   446,   202,   426,    -1,   284,
     260,   445,   202,   428,    -1,   284,   260,   419,    -1,   284,
     260,   422,    -1,   284,   260,   207,   293,   208,    -1,   284,
     260,   207,   292,   208,    -1,   284,   260,   207,   291,   208,
      -1,   284,   260,   447,   202,   207,   290,   208,    -1,   284,
     260,   446,   202,   207,   289,   208,    -1,   284,   260,   445,
     202,   207,   288,   208,    -1,   284,   260,   207,   294,   208,
      -1,   429,    -1,   288,   201,   429,    -1,   427,    -1,   289,
     201,   427,    -1,   425,    -1,   290,   201,   425,    -1,   423,
      -1,   291,   201,   423,    -1,   420,    -1,   292,   201,   420,
      -1,   431,    -1,   293,   201,   431,    -1,   430,    -1,   294,
     201,   430,    -1,    -1,    -1,   181,   211,   203,   296,   435,
     204,   297,   280,    -1,   211,   203,   438,   204,    -1,   137,
      -1,    -1,     6,    -1,     6,    -1,   205,   431,   183,   431,
     206,    -1,   205,   431,   206,    -1,    -1,    -1,   144,   303,
     205,   418,   206,   304,   207,   305,   208,    -1,    -1,   306,
     305,    -1,    -1,   145,   443,   202,   307,   279,    -1,    -1,
     147,   202,   308,   279,    -1,   146,    -1,   261,   261,    -1,
     143,    -1,   142,   285,   260,   431,   310,   431,    -1,    -1,
     311,   313,   280,    -1,    -1,   141,   205,   418,   206,   315,
     280,    -1,    -1,   301,   202,   317,   281,    -1,    -1,   124,
     301,   319,   320,    -1,   280,    -1,    -1,   280,   321,   125,
     280,    -1,    14,   202,   430,    -1,    70,    -1,    70,   202,
     443,    -1,    70,   202,    79,    -1,    58,   202,   325,   201,
     325,    -1,    58,   202,   325,   201,   443,    -1,   203,     4,
     201,    78,   201,   340,   204,    -1,   402,   300,   201,   412,
      -1,    -1,   114,   202,   326,   201,   333,   201,   335,   403,
     406,   408,   410,   328,   280,    -1,    -1,    50,   202,   326,
     201,   332,   201,   334,   201,   335,   403,   406,   408,   410,
     330,   280,    -1,    -1,   201,     5,    -1,   203,     4,   201,
       4,   204,    -1,    79,    -1,   203,     4,   201,     4,   204,
      -1,    79,    -1,   203,   344,   201,   345,   204,    -1,    79,
      -1,   203,     4,   201,     4,   204,    -1,    79,    -1,   123,
     202,   337,   201,   411,   201,   338,   201,   340,   341,    -1,
      27,   202,   411,   201,   440,   341,    -1,     5,    -1,    79,
      -1,   339,    -1,    79,    -1,    78,    -1,    78,   262,   339,
      -1,     4,    -1,    79,    -1,    -1,   341,   201,   342,    -1,
     129,   202,   130,    -1,    75,   202,   428,    -1,   112,   202,
       4,    -1,   153,    -1,   105,    -1,   106,    -1,   121,    -1,
      20,    -1,    19,   202,   344,   201,   345,   331,   210,    -1,
      19,   202,   419,   331,   210,    -1,    83,    -1,    84,    -1,
      85,    -1,    84,    -1,    25,   202,   348,    -1,    -1,    25,
     202,   348,   347,   280,    -1,   426,   201,   419,   349,    -1,
      -1,   349,   201,   350,    -1,   430,    -1,   118,    -1,   119,
      -1,   414,    -1,   122,   430,    -1,   154,    -1,   151,    -1,
     156,    -1,   157,    -1,   158,    -1,   159,   202,   418,    -1,
     160,   202,   418,    -1,   161,   202,   418,    -1,   162,    -1,
     163,    -1,   164,   202,   351,    -1,   155,    -1,   209,    -1,
     165,    -1,   209,   262,   351,    -1,    23,   202,   354,    -1,
      -1,    24,   202,   354,   353,   280,    -1,   428,   355,    -1,
      -1,   355,   201,   356,    -1,    73,    -1,   166,   202,   426,
      -1,   444,    -1,   115,   202,   430,    -1,   139,   202,   418,
      -1,   140,    -1,    72,    -1,   148,   202,   418,    -1,   180,
      -1,    71,    -1,   149,    -1,   150,   202,   418,    -1,   151,
      -1,   152,    -1,   419,    -1,   103,   202,   100,    -1,   102,
     202,   101,    -1,   104,   202,   418,    -1,   111,   202,   418,
      -1,   112,   202,   418,    -1,   113,   202,   418,    -1,    75,
     202,   428,    -1,   153,    -1,   106,    -1,    77,    -1,   109,
     202,   107,    -1,   110,   202,   108,    -1,    57,   202,   418,
      -1,    26,   202,   401,   201,   419,    -1,    28,   202,   419,
     201,    78,   201,   411,    -1,    32,   202,   419,   201,    78,
      -1,    32,   202,   419,   201,    78,   201,     5,   275,    -1,
      33,    -1,    33,   202,   440,    -1,    33,   202,   165,    -1,
      46,   202,   419,   201,    88,    -1,    47,   202,   419,   201,
      88,    -1,    47,   202,   449,   201,   449,   201,    88,    -1,
      52,   202,   449,   201,   449,   201,   209,    -1,    53,   202,
     449,   201,   449,   367,    -1,    54,   202,   449,   201,   449,
      -1,    -1,   201,    88,    -1,    61,   202,   440,   201,    92,
      -1,    62,   202,   440,    -1,    63,   202,   419,   201,   173,
     202,   419,   201,    90,   201,    89,    -1,    63,   202,   419,
     201,    64,   201,    65,   201,    66,   201,    90,   201,    89,
      -1,    76,   202,   419,   201,   170,   372,    -1,    -1,   372,
     201,   373,    -1,    89,    -1,   166,   202,   426,    -1,   167,
     202,   428,    -1,   168,   202,   377,    -1,   169,   202,   377,
      -1,   171,    -1,   172,    -1,   173,   202,   419,    -1,   174,
     202,   401,    -1,   129,   202,   374,    -1,    96,   202,   409,
      -1,   175,   202,     4,    -1,   112,   202,     4,    -1,     4,
      -1,    90,    -1,   130,    -1,    92,    -1,    68,   202,   440,
      -1,    67,   202,   440,    -1,     3,    -1,   203,     3,   201,
     412,   204,    -1,   128,   202,   422,   201,   424,   201,   424,
     201,     7,    -1,   126,   202,   440,   201,   424,    -1,   126,
     202,   440,   201,   424,   201,    95,    -1,    48,   202,   422,
      -1,    49,   202,   422,    -1,    38,   202,   209,   201,    39,
      -1,    38,   202,   209,   201,    39,   201,   166,   202,   426,
      -1,    35,   202,   422,   201,    37,    -1,    36,   202,   449,
     201,    37,    -1,    22,   202,   440,   201,    94,    -1,    -1,
      34,   202,   422,   201,   412,   201,   402,   403,   406,   408,
     410,   387,   388,    -1,    -1,   280,    -1,    43,   202,   419,
     201,   413,   201,   415,    -1,    43,   202,   419,   201,   413,
     201,   415,   201,   166,   202,   426,    -1,    43,   202,   419,
     201,   413,   201,   415,   201,    87,    -1,    43,   202,   419,
     201,   413,   201,   415,   201,    87,   201,   166,   202,   426,
      -1,    44,   202,   419,    -1,    44,   202,   419,   201,   166,
     202,   426,    -1,    97,   202,   440,   201,    98,   201,    95,
      -1,   120,   202,   440,   201,     4,    -1,   120,   202,   440,
     201,    98,   201,    95,    -1,    45,   202,    91,   201,   419,
      -1,    45,   202,    91,   201,   419,   201,   166,   202,   426,
      -1,   176,   202,   419,   201,   430,    -1,   176,   202,   419,
     201,    79,    -1,   176,   202,   419,    -1,   177,   202,   419,
     201,   131,   201,   412,    -1,   177,   202,   419,    -1,   178,
     202,   419,   201,   132,   201,   430,    -1,   178,   202,   419,
     201,   132,   201,    79,    -1,   178,   202,   419,    -1,   179,
     202,   419,   201,   133,    -1,   179,   202,   419,   201,   135,
      -1,   179,   202,   419,    -1,    59,   202,   431,   201,   419,
      -1,    60,   202,   419,   201,   448,   201,   430,    -1,    56,
     202,   418,   201,   418,   201,   418,   201,   418,    -1,    56,
      -1,   209,    -1,    79,    -1,   209,    -1,    79,    -1,    -1,
     201,   404,    -1,   405,    -1,   405,   201,   404,    -1,    40,
      -1,    41,    -1,    42,    -1,    -1,    99,   202,   407,    -1,
      98,    -1,    -1,    96,   202,   409,    -1,    95,    -1,    -1,
     166,   202,   426,    -1,    71,    -1,    79,    -1,    72,    -1,
      79,    -1,    82,    -1,   416,    -1,    79,    -1,    82,    -1,
     416,    -1,    81,   202,    79,    -1,    86,    -1,    79,    -1,
      81,   205,     4,   206,    -1,   209,    -1,   220,    -1,   221,
     205,   431,   206,    -1,   431,    -1,   420,    -1,   192,   203,
     440,   204,    -1,   224,    -1,   225,   205,   431,   206,    -1,
     203,     4,   201,     4,   204,    -1,    79,    -1,    80,   421,
     206,    -1,   200,    -1,   200,   201,   421,    -1,   423,    -1,
     226,    -1,   227,   205,   431,   206,    -1,   203,     4,   201,
       4,   201,     4,   201,     4,   204,    -1,   425,    -1,   228,
      -1,   229,   205,   431,   206,    -1,     3,    -1,   203,     3,
     201,   412,   204,    -1,   427,    -1,   230,    -1,   231,   205,
     431,   206,    -1,   209,    -1,     3,    -1,   203,     3,   201,
     209,   204,    -1,    79,    -1,   429,    -1,   232,    -1,   233,
     205,   431,   206,    -1,   209,    -1,     3,    -1,   203,     3,
     201,   209,   204,    -1,    79,    -1,    93,    -1,   417,    -1,
     430,   261,   417,    -1,     4,    -1,   441,    -1,   203,     8,
     204,    -1,   218,    -1,   219,   205,   431,   206,    -1,   431,
     255,   431,    -1,   431,   256,   431,    -1,   431,   257,   431,
      -1,   431,   258,   431,    -1,   431,   259,   431,    -1,   203,
     431,   204,    -1,   214,    -1,   215,    -1,   284,   202,   432,
      -1,   433,    -1,   434,   201,   433,    -1,    -1,   434,    -1,
     431,    -1,   430,    -1,   436,    -1,   437,   201,   436,    -1,
      -1,   437,    -1,   419,    -1,   185,   422,    -1,   186,   422,
      -1,   187,   419,   201,   419,    -1,   188,   419,   201,   419,
     201,   431,    -1,   189,   203,   440,   204,    -1,   189,   203,
     339,   201,   440,   204,    -1,   195,   203,     7,   201,   440,
     204,    -1,   195,   203,   440,   201,   440,   204,    -1,   195,
     203,   424,   201,   440,   204,    -1,   191,   419,    -1,   193,
     203,   419,   201,   431,   204,    -1,   193,   203,   419,   201,
     431,   201,    40,   204,    -1,   194,   203,   419,   201,   431,
     201,   431,   204,    -1,   194,   203,   419,   201,   431,   201,
     431,   201,    40,   204,    -1,   197,   203,   198,   201,   203,
     431,   256,   431,   273,   204,   201,   419,   274,   204,    -1,
     196,   439,    -1,   234,    -1,   203,   440,   204,    -1,   439,
      -1,   439,   263,   440,    -1,   254,    -1,    94,    -1,     4,
      -1,     8,    -1,     9,    -1,     4,    -1,     8,    -1,     9,
      -1,     4,    -1,   441,    -1,    29,    -1,    23,    -1,    30,
      -1,    25,    -1,    31,    -1,   126,    -1,    74,    -1,    79,
      -1,   450,    -1,    55,   203,     4,   201,     4,   201,     4,
     201,     4,   204,    -1,   203,     4,   201,     4,   201,     4,
     201,     4,   204,    -1
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
    1658,  1665,  1674,  1678,  1691,  1700,  1699,  1717,  1727,  1733,
    1741,  1747,  1752,  1757,  1762,  1767,  1772,  1777,  1782,  1787,
    1801,  1807,  1812,  1817,  1822,  1827,  1832,  1837,  1842,  1847,
    1852,  1857,  1862,  1867,  1872,  1877,  1882,  1887,  1894,  1900,
    1929,  1934,  1942,  1948,  1952,  1960,  1967,  1974,  1984,  1994,
    2009,  2020,  2023,  2029,  2035,  2041,  2045,  2051,  2058,  2064,
    2072,  2078,  2083,  2088,  2093,  2098,  2104,  2110,  2115,  2120,
    2125,  2130,  2135,  2142,  2142,  2142,  2142,  2145,  2151,  2157,
    2162,  2169,  2176,  2180,  2186,  2192,  2198,  2203,  2210,  2216,
    2226,  2233,  2232,  2264,  2267,  2273,  2278,  2283,  2288,  2294,
    2298,  2304,  2310,  2314,  2320,  2324,  2330,  2334,  2339,  2346,
    2350,  2357,  2361,  2366,  2373,  2377,  2382,  2390,  2396,  2403,
    2407,  2414,  2422,  2425,  2435,  2439,  2442,  2448,  2452,  2459,
    2463,  2467,  2474,  2477,  2483,  2490,  2493,  2499,  2506,  2510,
    2517,  2518,  2521,  2522,  2525,  2526,  2527,  2533,  2534,  2535,
    2541,  2542,  2545,  2554,  2559,  2566,  2577,  2583,  2587,  2591,
    2598,  2608,  2615,  2619,  2625,  2629,  2637,  2641,  2648,  2658,
    2671,  2675,  2682,  2692,  2701,  2712,  2716,  2723,  2733,  2744,
    2753,  2763,  2769,  2773,  2780,  2790,  2801,  2810,  2820,  2824,
    2831,  2832,  2838,  2842,  2846,  2850,  2858,  2867,  2871,  2875,
    2879,  2883,  2887,  2890,  2897,  2906,  2939,  2940,  2943,  2944,
    2947,  2951,  2958,  2965,  2976,  2979,  2987,  2991,  2995,  2999,
    3003,  3008,  3012,  3016,  3021,  3026,  3031,  3035,  3040,  3045,
    3049,  3053,  3058,  3062,  3069,  3075,  3079,  3085,  3092,  3093,
    3096,  3097,  3098,  3101,  3105,  3109,  3113,  3119,  3120,  3123,
    3124,  3127,  3128,  3131,  3132,  3135,  3139,  3165
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
  "INVIS_ID", "GREASED_ID", "INDESTRUCTIBLE_ID", "FEMALE_ID",
  "WAITFORU_ID", "CANCELLED_ID", "REVIVED_ID", "AVENGE_ID", "FLEEING_ID",
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
     455,    44,    58,    40,    41,    91,    93,   123,   125,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,    43,    45,    42,    47,    37,
      61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   264,   265,   265,   266,   266,   267,   268,   268,   269,
     269,   269,   269,   270,   271,   271,   272,   273,   273,   274,
     274,   275,   275,   276,   276,   277,   277,   278,   278,   279,
     279,   280,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   284,   284,   284,   285,   285,   286,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   288,   288,   289,   289,   290,   290,   291,   291,
     292,   292,   293,   293,   294,   294,   296,   297,   295,   298,
     299,   300,   300,   301,   301,   301,   303,   304,   302,   305,
     305,   307,   306,   308,   306,   309,   310,   310,   311,   313,
     312,   315,   314,   317,   316,   319,   318,   320,   321,   320,
     322,   323,   323,   323,   324,   324,   325,   326,   328,   327,
     330,   329,   331,   331,   332,   332,   333,   333,   334,   334,
     335,   335,   336,   336,   337,   337,   338,   338,   339,   339,
     340,   340,   341,   341,   342,   342,   342,   342,   342,   342,
     342,   343,   343,   343,   344,   344,   345,   345,   346,   347,
     346,   348,   349,   349,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   351,   351,   351,   352,   353,   352,   354,   355,   355,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   357,   358,
     359,   359,   360,   360,   360,   361,   362,   363,   364,   365,
     366,   367,   367,   368,   369,   370,   370,   371,   372,   372,
     373,   373,   373,   373,   373,   373,   373,   373,   373,   373,
     373,   373,   373,   374,   374,   374,   374,   375,   376,   377,
     377,   378,   379,   379,   380,   381,   382,   382,   383,   384,
     385,   387,   386,   388,   388,   389,   389,   389,   389,   390,
     390,   391,   392,   392,   393,   393,   394,   394,   394,   395,
     395,   396,   396,   396,   397,   397,   397,   398,   399,   400,
     400,   401,   401,   402,   402,   403,   403,   404,   404,   405,
     405,   405,   406,   406,   407,   408,   408,   409,   410,   410,
     411,   411,   412,   412,   413,   413,   413,   414,   414,   414,
     415,   415,   416,   417,   417,   417,   418,   419,   419,   419,
     419,   420,   420,   420,   421,   421,   422,   422,   422,   423,
     424,   424,   424,   425,   425,   426,   426,   426,   427,   427,
     427,   427,   428,   428,   428,   429,   429,   429,   429,   429,
     430,   430,   431,   431,   431,   431,   431,   431,   431,   431,
     431,   431,   431,   432,   432,   433,   434,   434,   435,   435,
     436,   436,   437,   437,   438,   438,   439,   439,   439,   439,
     439,   439,   439,   439,   439,   439,   439,   439,   439,   439,
     439,   439,   439,   439,   439,   440,   440,   441,   442,   442,
     443,   443,   443,   444,   444,   444,   444,   445,   445,   446,
     446,   447,   447,   448,   448,   449,   449,   450
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
       1,     1,     1,     1,     3,     3,     3,     1,     1,     3,
       1,     1,     1,     3,     3,     0,     5,     2,     0,     3,
       1,     3,     1,     3,     3,     1,     1,     3,     1,     1,
       1,     3,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     3,     3,     3,     5,     7,
       5,     8,     1,     3,     3,     5,     5,     7,     7,     6,
       5,     0,     2,     5,     3,    11,    13,     6,     0,     3,
       1,     3,     3,     3,     3,     1,     1,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     3,     3,     1,
       5,     9,     5,     7,     3,     3,     5,     9,     5,     5,
       5,     0,    13,     0,     1,     7,    11,     9,    13,     3,
       7,     7,     5,     7,     5,     9,     5,     5,     3,     7,
       3,     7,     7,     3,     5,     5,     3,     5,     7,     9,
       1,     1,     1,     1,     1,     0,     2,     1,     3,     1,
       1,     1,     0,     3,     1,     0,     3,     1,     0,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     1,     4,     1,     1,     4,     1,     1,     4,     1,
       4,     5,     1,     3,     1,     3,     1,     1,     4,     9,
       1,     1,     4,     1,     5,     1,     1,     4,     1,     1,
       5,     1,     1,     1,     4,     1,     1,     5,     1,     1,
       1,     3,     1,     1,     3,     1,     4,     3,     3,     3,
       3,     3,     3,     1,     1,     3,     1,     3,     0,     1,
       1,     1,     1,     3,     0,     1,     1,     2,     2,     4,
       6,     4,     6,     6,     6,     6,     2,     6,     8,     8,
      10,    14,     2,     1,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,    10,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    25,     0,     0,     1,
       5,     0,    29,     0,     7,     0,   153,     0,     0,     0,
       0,   221,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   292,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   370,     0,     0,
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
     115,     0,     0,     0,     0,     0,     0,     0,   442,     0,
     445,     0,   487,     0,   443,   464,    30,     0,   173,     0,
       8,     0,   403,   404,     0,   440,   180,     0,     0,     0,
      11,   489,   488,    13,   412,     0,   224,   225,     0,     0,
     409,     0,     0,   192,   407,    16,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   483,   466,
     485,     0,   436,   438,   439,     0,   435,   433,     0,   254,
     258,   432,   255,   429,   431,     0,   428,   426,     0,   228,
       0,   425,   372,   371,     0,   390,   391,     0,     0,     0,
     294,   293,     0,   417,     0,     0,   416,     0,     0,     0,
       0,   505,     0,     0,   349,     0,     0,     0,     0,     0,
     334,   335,   374,   373,     0,   151,     0,     0,     0,     0,
     406,     0,     0,     0,     0,     0,   304,     0,   328,   327,
     492,   490,   491,   183,   182,     0,     0,     0,     0,   204,
     205,     0,     0,     0,     0,    15,   116,     0,     0,     0,
     358,   360,   363,   366,   146,     0,     0,     0,     0,   155,
       0,     0,     0,     0,     0,   461,   460,   462,   465,     0,
     498,   500,   497,   499,   501,   502,     0,     0,     0,   123,
     124,   119,   117,     0,     0,     0,     0,    29,   170,    27,
       0,     0,     0,     0,     0,   414,     0,     0,     0,     0,
       0,     0,     0,   467,   468,     0,     0,     0,   476,     0,
       0,     0,   482,     0,     0,     0,     0,     0,     0,   257,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   152,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   177,   176,     0,     0,     0,   171,     0,     0,
       0,     0,     0,     0,   458,   444,   452,     0,     0,   447,
     448,   449,   450,   451,     0,   149,     0,   442,     0,     0,
       0,     0,   140,   138,   144,   142,     0,     0,     0,   174,
       0,     0,   441,    10,   329,     0,     9,     0,     0,   413,
       0,     0,     0,   227,   226,   192,   193,   223,     0,     0,
     208,     0,     0,     0,     0,   423,     0,     0,   421,     0,
       0,   420,     0,     0,   484,   486,   340,     0,     0,     0,
     256,     0,     0,   230,   232,   288,   212,     0,   290,     0,
       0,   392,   393,     0,   338,     0,     0,   339,   336,   396,
       0,   394,     0,   395,     0,   354,   295,     0,   296,     0,
     195,     0,     0,     0,     0,   301,   300,     0,     0,   184,
     185,   367,   503,   504,     0,   303,     0,     0,   308,     0,
     197,     0,     0,   352,     0,     0,     0,     0,   332,     0,
      14,     0,   167,     0,     0,   157,   357,   356,     0,     0,
     364,   365,     0,   456,   459,     0,   446,   154,   463,   118,
       0,     0,   127,     0,   126,     0,   125,     0,   131,     0,
     122,     0,   121,     0,   120,    31,   405,     0,     0,   415,
     408,     0,   410,     0,   469,     0,     0,     0,   471,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   495,
     493,   494,     0,   269,   266,   260,     0,   284,     0,     0,
       0,   283,     0,     0,     0,     0,     0,     0,     0,   265,
       0,   270,     0,   272,   273,   282,     0,   268,   259,   274,
     496,   262,     0,   427,   231,   203,     0,     0,     0,   418,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   187,     0,     0,   299,     0,     0,     0,     0,
       0,   307,     0,     0,     0,     0,     0,     0,     0,     0,
     172,   166,   168,     0,     0,     0,     0,     0,   147,     0,
     139,   141,   143,   145,     0,   132,     0,   134,     0,   136,
       0,     0,   411,   222,     0,   209,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   430,     0,     0,   289,    21,     0,   375,     0,     0,
       0,     0,   401,   400,   345,   350,     0,   297,     0,   199,
       0,     0,   298,   302,     0,     0,   368,     0,     0,     0,
     351,     0,   201,     0,   375,   353,   207,     0,   206,   179,
     333,     0,   159,   359,   362,   361,   453,   454,   455,   457,
       0,     0,   130,     0,   129,     0,   128,     0,     0,   470,
     472,     0,   477,     0,   473,     0,   422,   475,   474,     0,
     287,   281,   276,   275,   277,   285,   286,   278,   279,   280,
     263,   264,   267,   271,   261,     0,   397,   235,   236,     0,
     240,   239,   250,   241,   242,   243,     0,     0,     0,   247,
     248,     0,   233,   237,   398,   234,     0,   218,   219,     0,
     220,     0,   217,   213,     0,   291,     0,     0,   382,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,   210,
     211,     0,     0,     0,   310,     0,     0,     0,     0,     0,
       0,     0,   315,   316,     0,     0,     0,   309,     0,     0,
     382,     0,     0,     0,     0,     0,   159,   148,   133,   135,
     137,   330,     0,     0,     0,   424,     0,     0,   238,     0,
       0,     0,     0,     0,     0,     0,    22,     0,   379,   380,
     381,   376,   377,     0,   385,     0,     0,   337,   347,     0,
     355,   194,     0,   375,   369,   186,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   196,     0,
     385,   212,   331,     0,   163,   158,   160,     0,   478,     0,
     479,   448,   399,   244,   245,   246,   252,   251,   249,   215,
     216,   214,     0,     0,     0,     0,   388,     0,     0,     0,
       0,     0,   382,     0,     0,   387,   320,   322,   323,   324,
     326,   325,   319,   311,   312,   313,   314,   317,   318,   321,
       0,   388,   202,   161,    29,     0,     0,     0,     0,     0,
     419,   378,   384,   383,     0,     0,   341,     0,   507,     0,
     346,   198,   385,     0,   305,   200,   188,    29,   164,     0,
     480,    18,     0,   253,   386,     0,   343,   506,     0,   388,
       0,     0,   162,     0,     0,   389,   344,   342,   348,   190,
     306,   189,     0,    19,     0,    23,    24,    21,     0,     0,
     191,    12,    20,   481
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    98,    99,   100,   101,  1068,
    1119,   915,  1117,    12,   168,   102,   408,   103,   104,   105,
     106,   221,   107,   108,   764,   766,   768,   508,   509,   510,
     511,   109,   494,   850,   110,   111,   466,   112,   113,   222,
     753,   955,   956,  1087,  1064,   114,   624,   115,   116,   239,
     117,   621,   118,   406,   119,   362,   483,   616,   120,   121,
     122,   342,   334,   123,  1101,   124,  1114,   422,   592,   612,
     821,   834,   125,   361,   837,   541,   931,   715,   913,   126,
     262,   535,   127,   443,   299,   714,   902,  1028,   128,   440,
     289,   439,   708,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   735,   139,   140,   141,   142,   741,   947,
    1052,   143,   144,   526,   145,   146,   147,   148,   149,   150,
     151,   152,   153,  1096,  1107,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   304,   335,   918,
     981,   982,   984,  1073,  1036,  1046,  1076,   307,   573,   582,
     903,   814,   583,   245,   339,   279,   264,   416,   315,   316,
     550,   551,   300,   301,   290,   291,   385,   340,   848,   633,
     634,   635,   387,   388,   389,   280,   434,   234,   253,   354,
     711,   403,   404,   405,   604,   320,   321
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -982
static const yytype_int16 yypact[] =
{
     379,  -108,   -63,    99,  -982,   379,   150,    -6,    31,  -982,
    -982,    27,   793,    51,  -982,   172,  -982,   122,   151,   161,
     167,  -982,   187,   190,   216,   220,   222,   225,   226,   238,
     250,   260,   266,   268,   279,   280,   282,   284,   289,   291,
     304,   306,   318,   323,   325,   332,   336,   338,   339,   349,
     358,   360,   362,   365,   368,   370,   375,   383,   385,   386,
     389,   397,    20,   398,   406,   415,  -982,   420,   115,   720,
    -982,  -982,   421,   426,   427,   428,   205,    79,   297,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,   793,  -982,  -982,   272,  -982,  -982,  -982,
    -982,  -982,   431,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,   565,   401,  -982,  -117,
     535,    44,   371,   534,   686,    36,    36,   118,   -30,    41,
     264,   264,   844,  -118,  -118,   -17,   451,   264,   264,   578,
     264,   341,  -118,  -118,   -20,   -17,   -17,   -17,    79,   467,
      79,   264,   686,   686,   264,   686,   686,    56,   264,   686,
     -20,   686,    57,  -982,   686,  -118,   536,   754,    79,  -982,
    -982,   412,   471,   264,   264,   264,   264,   478,  -982,    53,
    -982,   472,  -982,   178,  -982,    23,  -982,   400,  -982,   475,
    -982,   172,  -982,  -982,   485,  -982,   422,   490,   492,   495,
    -982,  -982,  -982,  -982,  -982,   498,  -982,  -982,   489,   695,
    -982,   496,   499,   502,  -982,  -982,  -118,  -118,   264,   264,
     503,   264,   505,   506,   508,   686,   511,   214,  -982,  -982,
     442,   514,  -982,  -982,  -982,   704,  -982,  -982,   518,  -982,
    -982,  -982,  -982,  -982,  -982,   713,  -982,  -982,   519,   529,
     524,  -982,  -982,  -982,   525,  -982,  -982,   538,   540,   541,
    -982,  -982,   731,  -982,   532,   543,  -982,   550,   549,   734,
     552,  -982,   556,   563,   566,   567,   568,   766,   575,   581,
    -982,  -982,  -982,  -982,   587,   783,   589,   596,   597,   599,
     124,   797,   601,   393,   602,   603,  -982,   604,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,   605,   607,   608,   621,  -982,
    -982,   622,   475,   623,   629,   631,  -982,   627,    79,    79,
     634,   643,   647,   649,  -982,   653,   254,    79,    79,  -982,
      79,    79,    79,    79,    79,   422,   124,  -982,   657,   655,
    -982,  -982,  -982,  -982,  -982,  -982,   660,    97,   136,  -982,
    -982,   422,   124,   662,   663,   664,   793,   793,  -982,  -982,
      79,  -117,   864,    11,   865,   669,   670,   686,   683,    79,
      29,   880,   676,  -982,  -982,   687,   690,   -44,  -982,   264,
     264,   450,  -982,   689,   688,   686,   799,   694,    79,   696,
     475,   697,    79,   475,   264,   264,   686,   818,   821,   699,
      79,    16,   866,   897,   701,   867,   869,   113,   739,   264,
     824,   705,   826,   -17,   -47,  -982,   708,   -17,   -17,   -17,
      79,   714,    59,   264,   258,   830,   -13,   748,   827,   -39,
      85,    41,   801,  -982,    25,    25,   792,  -982,   188,   723,
     120,   805,   809,    86,   831,  -982,  -982,   462,   474,   218,
     218,  -982,  -982,  -982,    23,  -982,   686,   742,   -91,   -85,
     -77,   -76,  -982,  -982,   422,   124,   127,   155,   143,  -982,
     737,   491,  -982,  -982,  -982,   946,  -982,   750,   498,  -982,
     749,   951,   504,  -982,  -982,   502,  -982,  -982,   264,   264,
     698,   755,   753,   757,   758,  -982,   761,   470,  -982,   759,
     762,  -982,   764,   765,  -982,  -982,  -982,   767,   516,    98,
    -982,   769,   522,  -982,  -982,  -982,  -982,   779,   781,   963,
     528,  -982,  -982,   785,  -982,   787,   964,  -982,   789,  -982,
     786,  -982,   791,  -982,   794,   796,  -982,   989,  -982,   798,
    -982,   990,   800,    16,   802,   833,  -982,   841,   917,  -982,
    -982,  -982,  -982,  -982,   845,  -982,   870,   804,  -982,   871,
    -982,   996,   872,  -982,   873,   874,   877,  1007,   875,   878,
    -982,   475,  -982,   806,    79,  -982,  -982,   422,   890,   891,
    -982,  -982,   868,  -982,   892,   893,  -982,  -982,  -982,  -982,
    1090,   895,  -982,   -36,  -982,    79,  -982,  -117,  -982,    83,
    -982,   184,  -982,    21,  -982,  -982,  -982,   894,  1093,  -982,
    -982,   896,  -982,   889,  -982,   900,  1024,   686,  -982,    79,
      79,   686,   902,    79,   686,   686,   901,   903,  -982,  -982,
    -982,  -982,   904,  -982,  -982,  -982,   906,  -982,   907,   908,
     909,  -982,   910,   911,   912,   913,   914,   915,   916,  -982,
     918,  -982,   919,  -982,  -982,  -982,   920,  -982,  -982,  -982,
    -982,  -982,   921,  -982,   922,   923,    41,  1100,   925,  -982,
     -20,  1115,   927,   961,  1125,    47,   118,   965,  -111,  1042,
     931,   -10,  -982,   924,  1046,  -982,    79,   935,  -117,  1072,
     264,   937,  1044,   939,    -7,  1047,   202,   475,  1048,    25,
    -982,  -982,   124,   934,    16,   156,   183,   831,  -982,    26,
    -982,  -982,   124,   422,   -74,  -982,     4,  -982,   134,  -982,
      16,   943,  -982,  -982,    79,  -982,   941,   259,   430,   944,
      16,   537,   945,   947,    79,  -982,    79,    36,  1049,  1052,
      79,  1050,  1045,    79,    79,    79,  -117,    79,    79,    79,
     118,  -982,   926,   366,  -982,   953,  1151,   955,   957,  1155,
     958,   956,  -982,  -982,   960,  -982,   962,  -982,  1159,  -982,
     381,   966,  -982,  -982,   967,    87,   422,   968,   969,   330,
    -982,  1161,  -982,  1162,   955,  -982,  -982,   970,  -982,  -982,
    -982,   971,   228,  -982,  -982,   422,  -982,  -982,  -982,  -982,
     475,    83,  -982,   184,  -982,    21,  -982,   972,  1168,   124,
    -982,  1134,  -982,    79,  -982,   973,  -982,  -982,  -982,   298,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
     422,  -982,  -982,  -982,  -982,    65,  -982,  -982,  -982,  -117,
    -982,  -982,  -982,  -982,  -982,  -982,   976,   977,   978,  -982,
    -982,   979,  -982,  -982,  -982,   422,   980,  -982,  -982,   981,
    -982,   982,  -982,  -982,  1172,  -982,   984,   564,  1087,  1183,
     987,   118,  -982,   -29,   118,   985,   991,    -7,    79,  -982,
    -982,   986,  1127,  1101,  -982,   992,   993,   994,   995,   997,
     998,   999,  -982,  -982,  1000,  1001,  1002,  -982,  1003,  1004,
    1087,    87,  1191,   275,  1006,  1005,   228,  -982,  -982,  -982,
    -982,  -982,  1008,  1010,   357,  -982,    79,  1131,   422,    79,
      79,    79,   -87,    36,  1202,  1081,  -982,  1208,  -982,  -982,
    -982,  -982,  1014,  1015,  1120,  1017,  1215,  -982,  1019,  1020,
    -982,  -982,    29,   955,  -982,  -982,  1022,  1023,  1126,  1221,
      92,   118,    36,    11,    11,   264,   -30,  1222,  -982,  1223,
    1120,  -982,  -982,  1026,  -982,  -982,  -982,  1224,  -982,  1190,
    -982,   265,  -982,  -982,  -982,  -982,  -982,   974,  -982,  -982,
    -982,  -982,  1027,   564,  1135,  1030,  1068,  1231,  1033,  1073,
     118,  1034,  1087,  1150,  1152,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    1038,  1068,   923,  -982,   793,  1043,  1039,  1051,  1041,   -87,
    -982,  -982,  -982,  -982,  1126,  1053,  -982,  1054,  -982,  1055,
    -982,  -982,  1120,  1058,  -982,  -982,  -982,   793,  -982,    16,
    -982,  -982,  1059,  -982,  -982,   118,   475,  -982,   118,  1068,
    1157,   475,  -982,  1060,   264,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,   106,  1061,   475,  -982,  -982,   953,   264,  1062,
    -982,  -982,  -982,  -982
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -982,  -982,  1242,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,   131,  -982,  -982,  1011,  -103,  -356,   843,  1036,  1182,
    -481,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  1192,  -982,  -982,
    -982,   300,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,   795,  1063,  -982,  -982,  -982,  -982,   728,  -982,  -982,
    -982,   337,  -982,  -982,  -982,  -585,   314,   257,  -982,  -982,
     449,   278,  -982,  -982,  -982,  -982,  -982,   203,  -982,  -982,
    1095,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -590,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,   269,   554,  -819,
     243,  -982,  -874,  -982,  -891,   204,  -981,  -456,  -582,  -982,
    -982,  -982,   477,   876,  -215,  -171,  -386,   752,   344,  -380,
    -462,  -632,  -509,  -622,  -512,  -618,  -162,   -72,  -982,   520,
    -982,  -982,   777,  -982,  -982,  1009,  -132,   724,  -982,  -470,
    -982,  -982,  -982,  -982,  -982,  -150,  -982
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -230
static const yytype_int16 yytable[] =
{
     236,   263,   600,   367,   650,   233,   482,   246,   652,   308,
     309,   732,   512,   632,   524,   950,   323,   324,   513,   326,
     328,   769,   618,   619,   545,   615,    16,   228,   545,   767,
     344,   765,   590,   347,   540,   254,   255,   355,   318,   282,
     610,   329,   281,   254,   255,   336,   337,   338,   251,   302,
     311,   606,   370,   371,   372,   373,   654,   228,   988,   332,
     350,   375,   359,   350,   351,   352,   399,   351,   352,   819,
     345,   346,   832,   348,   349,   401,  1010,   356,  1026,   358,
    1086,   775,   363,   228,   560,   312,   282,   563,   571,   613,
     809,   929,   242,   772,     7,   572,  1048,   425,   426,     9,
     428,   507,   679,   243,   244,   375,   680,   681,   313,   314,
     641,  1115,   305,   533,   534,   283,   643,   642,  1109,  1061,
     306,   293,  1027,   644,   645,   647,   812,   851,   343,   284,
     282,   646,   648,   813,   852,   353,   360,   989,   252,     8,
     228,   266,   267,   268,   269,   270,   545,   271,   258,   272,
     273,   274,   275,   276,   489,   682,   591,   376,   293,   277,
     607,   838,   283,   386,   611,   402,   930,   259,  1082,   683,
     684,   685,   843,   686,  1042,   687,   284,   254,   255,   303,
     260,   261,  1049,   614,  1050,  1116,   319,   293,   857,   333,
     278,  1099,   579,   820,   580,   581,   833,   294,   865,   626,
     688,   689,   690,    13,   691,   853,   283,   692,   693,   694,
     695,   696,   854,   697,   525,   254,   255,   815,   418,   630,
     284,   631,  1051,   960,   617,    77,   229,   806,   617,    15,
     772,   959,   242,   958,   294,   844,   514,   698,   699,   285,
      14,   230,   231,   243,   244,   286,   700,   701,   702,   703,
     704,   705,   166,   548,   549,   597,   229,   761,   543,   544,
     804,   760,   341,   294,   706,   750,    11,   967,   287,   288,
     724,   230,   231,   564,   565,   871,   632,   232,   707,   350,
     540,   836,   229,   351,   352,   530,   285,   841,   585,   167,
     258,   884,   286,   254,   255,   542,   488,   230,   231,   552,
     229,   259,   601,   555,   520,   497,   498,   232,   499,   500,
     501,   502,   503,   589,   566,   230,   231,   594,   595,   596,
     218,   295,   260,   261,   169,   376,   515,   296,   627,   242,
     285,   622,   602,   232,   649,   855,   286,   603,   521,   397,
     243,   244,   856,   254,   255,   242,   617,   532,   297,   298,
     653,   232,   232,   170,   230,   231,   243,   244,   295,   287,
     288,   378,   651,   171,   296,   242,   558,   664,   665,   172,
     562,   548,   549,   953,   639,   954,   243,   244,   570,   380,
     381,   382,   383,   384,   379,   297,   298,   295,   709,   173,
     232,   839,   174,   296,     1,     2,   318,   846,   847,   266,
     267,   268,   269,   270,   228,   271,   258,   272,   273,   274,
     275,   276,   987,  1055,  1056,   990,   227,   277,   175,   934,
     254,   255,   176,   390,   177,   391,   935,   178,   179,   392,
     393,   394,   386,   380,   381,   382,   383,   384,   260,   261,
     180,   906,   936,   380,   381,   382,   383,   384,   278,   623,
     254,   255,   181,   545,   256,   257,   258,   546,   496,   937,
     861,  1029,   182,   862,   256,   257,  1067,   259,   183,   -17,
     184,   907,   908,   672,   418,   382,   383,   384,   909,   254,
     255,   185,   186,  1013,   187,   763,   188,   910,   260,   261,
    1054,   189,  1053,   190,   957,   911,   938,   939,   940,   941,
     235,   942,   943,   944,   945,   946,   191,  1103,   192,   380,
     381,   382,   383,   384,   380,   381,   382,   383,   384,   912,
     193,   824,   382,   383,   384,   194,   395,   195,   317,   254,
     255,  1080,   237,   258,   196,   776,   330,   331,   197,   779,
     198,   199,   782,   783,   327,   247,   248,   249,   250,   254,
     255,   200,   752,   380,   966,   382,   383,   384,  1019,   364,
     201,  1020,   202,   258,   203,   260,   261,   204,   240,   828,
     205,   870,   206,   762,   259,   874,   826,   207,   877,   878,
     879,   400,   881,   882,   883,   208,  1105,   209,   210,  1108,
     396,   211,   258,   845,   473,   260,   261,   777,   778,   212,
     214,   781,   241,   397,   978,   979,   980,   398,   215,   242,
     423,   424,   380,   381,   382,   383,   384,   216,   230,   231,
     243,   244,   217,   223,   260,   261,   313,   314,   224,   225,
     226,   863,   265,   238,   880,   266,   267,   268,   269,   270,
     905,   271,   258,   272,   273,   274,   275,   276,   380,   381,
     382,   383,   384,   547,   232,   266,   267,   268,   269,   270,
     322,   271,   258,   272,   273,   274,   275,   276,   636,   325,
     341,   365,   368,   277,   260,   261,   369,   377,   548,   549,
     637,   374,   407,   411,   278,   380,   381,   382,   383,   384,
     410,   412,   417,   413,   260,   261,   414,   656,   415,   418,
     420,   419,   859,   421,   278,   435,   427,   437,   429,   430,
     662,   431,   869,   994,   433,   436,   441,   380,   381,   382,
     383,   384,   678,   438,   442,   444,   445,   968,   713,   380,
     381,   382,   383,   384,   719,   449,  -229,   450,   454,   446,
    1106,   447,   448,   866,   451,  1111,   380,   381,   382,   383,
     384,   452,   453,   455,  1023,  1024,  1025,   456,  1120,   380,
     381,   382,   383,   384,   457,   254,   255,   458,   459,   460,
     461,   380,   381,   382,   383,   384,   462,   380,   381,   382,
     383,   384,   463,   380,   381,   382,   383,   384,   464,   465,
     467,   964,   380,   381,   382,   383,   384,   468,   469,    16,
     470,   471,   472,   474,   475,   476,   477,    17,   478,   479,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,   480,   481,   484,    30,    31,    32,    33,    34,
     485,    35,   486,   487,  1057,   490,    36,    37,    38,    39,
      40,    41,    42,    43,   491,    44,    45,    46,   492,    47,
     493,    48,    49,    50,    51,    52,    53,   495,   504,   505,
      54,    55,   506,    56,   516,   517,   518,   523,   527,    57,
     528,   266,   267,   268,   269,   270,   529,   271,   258,   272,
     273,   274,   275,   276,   531,   536,   537,   553,   538,   277,
      58,   539,   554,   556,  1021,   557,   567,   559,   561,   568,
     569,   575,   576,   574,   577,   584,   587,    59,   578,   593,
     260,   261,   586,    60,   588,   598,    61,    62,   608,    63,
     278,    64,   605,   254,   255,   609,  -178,    65,   620,   625,
      66,    67,   219,  1113,    68,    69,   628,    70,    80,    71,
      82,   629,    84,   640,    86,   655,    88,  1122,    90,   657,
      92,   658,    94,   660,    96,   661,   667,   668,   669,   670,
     666,  1088,   671,   674,   673,   675,   676,   718,   722,    72,
      73,    74,    75,    81,    76,    83,   677,    85,   712,    87,
     716,    89,   717,    91,  1102,    93,   720,    95,   721,    97,
     723,   724,   725,   728,   730,   737,   726,   727,    77,   729,
     743,   731,   747,   733,    78,    79,   740,   885,   886,   310,
     672,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,   266,
     267,   268,   269,   270,   734,   271,   258,   272,   273,   274,
     275,   276,   736,    79,   887,   888,   738,   277,   889,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,   751,   260,   261,
     756,   739,   742,   744,   745,   746,   748,   890,   278,   749,
     891,   892,   893,   894,   895,   896,   897,   898,   899,   900,
     901,   754,   755,   757,   759,   770,   771,   758,   312,   773,
     772,   774,   540,   780,   784,   805,   786,   785,   787,   788,
     789,   790,   791,   792,   793,   794,   795,   796,   797,   808,
     798,   799,   800,   802,   803,   801,   806,   810,   809,   811,
     817,   816,   818,   822,   823,   242,   825,   827,   829,   830,
     831,   842,   835,   840,   858,   860,   243,   244,   864,   867,
     872,   868,   873,   876,   914,   916,   917,   875,   919,   920,
     921,   923,   922,   925,   924,   948,   949,   927,   928,   932,
     933,   951,   952,   962,   963,   976,   961,   965,   969,   970,
     971,   972,   973,   974,   975,   977,   983,   985,   986,   991,
     995,   997,   992,   996,   998,   999,  1000,  1001,  1012,  1002,
    1003,  1004,  1005,  1006,  1007,  1009,  1030,  1008,  1014,  1017,
    1022,  1031,  1032,  1015,  1018,  1033,  1035,  1034,  1037,  1038,
    1039,  1045,  1040,  1043,  1044,  1047,  1059,  1060,  1063,  1065,
    1066,  1070,  1074,  1072,  1075,  1077,  1069,  1078,  1081,  1079,
    1083,  1084,  1085,  1090,  1089,  1092,  1110,    10,  1121,   519,
    1091,   220,   409,   366,   213,  1095,  1016,  1098,  1097,  1100,
    1104,  1112,  1118,   663,   993,  1011,  1123,   599,  1062,   926,
    1041,   292,  1093,   357,   807,  1058,  1071,   849,  1094,   904,
     659,   638,     0,   710,   432,     0,     0,   522
};

static const yytype_int16 yycheck[] =
{
     103,   172,   472,   218,   516,    77,   362,   169,   517,   180,
     181,   593,   398,   494,     3,   834,   187,   188,   398,   190,
     191,   653,   484,   485,     3,   481,     6,     4,     3,   651,
     201,   649,    79,   204,    78,    79,    80,   208,    55,     3,
      79,   191,   174,    79,    80,   195,   196,   197,     4,    79,
     182,    64,   223,   224,   225,   226,   518,     4,    87,    79,
       4,     8,     5,     4,     8,     9,   237,     8,     9,    79,
     202,   203,    79,   205,   206,   237,   950,   209,   165,   211,
    1061,   666,   214,     4,   440,   203,     3,   443,    72,     4,
     201,     4,   209,   204,   202,    79,     4,   268,   269,     0,
     271,     4,     4,   220,   221,     8,     8,     9,   226,   227,
     201,     5,    71,    84,    85,    79,   201,   208,  1099,  1010,
      79,     3,   209,   208,   201,   201,    79,   201,   200,    93,
       3,   208,   208,    86,   208,    79,    79,   166,    94,   202,
       4,   185,   186,   187,   188,   189,     3,   191,   192,   193,
     194,   195,   196,   197,   369,    57,   203,   229,     3,   203,
     173,   746,    79,   235,   203,   237,    79,   203,  1042,    71,
      72,    73,   754,    75,   993,    77,    93,    79,    80,   209,
     224,   225,    90,    98,    92,    79,   203,     3,   770,   209,
     234,  1082,    79,   203,    81,    82,   203,    79,   780,    79,
     102,   103,   104,   209,   106,   201,    79,   109,   110,   111,
     112,   113,   208,   115,   203,    79,    80,   726,     4,   133,
      93,   135,   130,   855,   203,   205,   203,   201,   203,   202,
     204,   853,   209,   851,    79,    79,   398,   139,   140,   203,
     209,   218,   219,   220,   221,   209,   148,   149,   150,   151,
     152,   153,   201,   228,   229,   470,   203,   643,   429,   430,
     716,   641,   203,    79,   166,   621,   116,   202,   232,   233,
     205,   218,   219,   444,   445,   787,   757,   254,   180,     4,
      78,    79,   203,     8,     9,   417,   203,   749,   459,   117,
     192,   800,   209,    79,    80,   427,   368,   218,   219,   431,
     203,   203,   473,   435,   407,   377,   378,   254,   380,   381,
     382,   383,   384,   463,   446,   218,   219,   467,   468,   469,
     205,   203,   224,   225,   202,   397,   398,   209,   490,   209,
     203,   143,    74,   254,   207,   201,   209,    79,   410,   203,
     220,   221,   208,    79,    80,   209,   203,   419,   230,   231,
     207,   254,   254,   202,   218,   219,   220,   221,   203,   232,
     233,   183,   207,   202,   209,   209,   438,   538,   539,   202,
     442,   228,   229,   145,   506,   147,   220,   221,   450,   255,
     256,   257,   258,   259,   206,   230,   231,   203,   559,   202,
     254,   747,   202,   209,    15,    16,    55,   214,   215,   185,
     186,   187,   188,   189,     4,   191,   192,   193,   194,   195,
     196,   197,   921,  1003,  1004,   924,   211,   203,   202,    89,
      79,    80,   202,    23,   202,    25,    96,   202,   202,    29,
      30,    31,   504,   255,   256,   257,   258,   259,   224,   225,
     202,    75,   112,   255,   256,   257,   258,   259,   234,   261,
      79,    80,   202,     3,    83,    84,   192,     7,   204,   129,
     201,   973,   202,   204,    83,    84,   201,   203,   202,   204,
     202,   105,   106,     3,     4,   257,   258,   259,   112,    79,
      80,   202,   202,   953,   202,   647,   202,   121,   224,   225,
    1002,   202,  1001,   202,   850,   129,   166,   167,   168,   169,
     203,   171,   172,   173,   174,   175,   202,  1089,   202,   255,
     256,   257,   258,   259,   255,   256,   257,   258,   259,   153,
     202,   736,   257,   258,   259,   202,   126,   202,   184,    79,
      80,  1040,   260,   192,   202,   667,   192,   193,   202,   671,
     202,   202,   674,   675,   203,    10,    11,    12,    13,    79,
      80,   202,   624,   255,   256,   257,   258,   259,   201,   215,
     202,   204,   202,   192,   202,   224,   225,   202,     3,   740,
     202,   786,   202,   645,   203,   790,   738,   202,   793,   794,
     795,   237,   797,   798,   799,   202,  1095,   202,   202,  1098,
     190,   202,   192,   755,   201,   224,   225,   669,   670,   202,
     202,   673,   201,   203,    40,    41,    42,   207,   202,   209,
     266,   267,   255,   256,   257,   258,   259,   202,   218,   219,
     220,   221,   202,   202,   224,   225,   226,   227,   202,   202,
     202,   201,    98,   202,   796,   185,   186,   187,   188,   189,
     802,   191,   192,   193,   194,   195,   196,   197,   255,   256,
     257,   258,   259,   203,   254,   185,   186,   187,   188,   189,
     209,   191,   192,   193,   194,   195,   196,   197,   206,    91,
     203,   135,   260,   203,   224,   225,   205,   205,   228,   229,
     206,   203,   207,   261,   234,   255,   256,   257,   258,   259,
     205,   201,   203,   201,   224,   225,   201,   206,   200,     4,
     201,   205,   774,   201,   234,   263,   203,     3,   203,   203,
     206,   203,   784,   928,   203,   201,     3,   255,   256,   257,
     258,   259,   206,   205,   205,   201,   201,   889,   206,   255,
     256,   257,   258,   259,   206,     4,   207,   205,     4,   201,
    1096,   201,   201,   206,   201,  1101,   255,   256,   257,   258,
     259,   201,   203,   201,   969,   970,   971,   201,  1114,   255,
     256,   257,   258,   259,   201,    79,    80,   201,   201,   201,
       4,   255,   256,   257,   258,   259,   201,   255,   256,   257,
     258,   259,   201,   255,   256,   257,   258,   259,   201,     6,
     201,   863,   255,   256,   257,   258,   259,   201,   201,     6,
     201,     4,   201,   201,   201,   201,   201,    14,   201,   201,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,   201,   201,   201,    32,    33,    34,    35,    36,
     201,    38,   201,   206,  1005,   201,    43,    44,    45,    46,
      47,    48,    49,    50,   201,    52,    53,    54,   201,    56,
     201,    58,    59,    60,    61,    62,    63,   204,   201,   204,
      67,    68,   202,    70,   202,   202,   202,     3,     3,    76,
     201,   185,   186,   187,   188,   189,   206,   191,   192,   193,
     194,   195,   196,   197,   201,     5,   210,   198,   201,   203,
      97,   201,   204,    94,   966,   201,    78,   201,   201,    78,
     201,     4,   201,    37,    37,   166,   201,   114,    39,   201,
     224,   225,    88,   120,    88,   201,   123,   124,   170,   126,
     234,   128,    92,    79,    80,    98,   125,   134,   136,   206,
     137,   138,   212,  1104,   141,   142,   131,   144,   218,   146,
     220,   132,   222,   201,   224,   208,   226,  1118,   228,     3,
     230,   201,   232,   204,   234,     4,   201,   204,   201,   201,
     262,  1064,   201,   201,   205,   201,   201,     4,     4,   176,
     177,   178,   179,   219,   181,   221,   209,   223,   209,   225,
     201,   227,   201,   229,  1087,   231,   201,   233,   201,   235,
     201,   205,   201,     4,     4,    78,   202,   201,   205,   201,
       4,   201,   125,   201,   211,   212,   202,    81,    82,   165,
       3,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   185,
     186,   187,   188,   189,   201,   191,   192,   193,   194,   195,
     196,   197,   201,   212,   118,   119,   201,   203,   122,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   261,   224,   225,
     202,   201,   201,   201,   201,   201,   201,   151,   234,   201,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   201,   201,   201,     4,   201,     3,   204,   203,   210,
     204,   201,    78,   201,   203,     5,   202,   204,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,     4,
     202,   202,   202,   201,   201,   204,   201,   166,   201,     4,
      88,   166,   201,   209,    88,   209,   201,    65,   201,    95,
     201,   207,    95,    95,   201,   204,   220,   221,   204,   204,
     101,   204,   100,   108,   201,     4,   201,   107,   201,     4,
     202,   201,   206,     4,   202,     4,     4,   201,   201,   201,
     201,   201,   201,     5,    40,     3,   204,   204,   202,   202,
     202,   202,   202,   202,   202,   201,    99,     4,   201,   204,
     204,    90,   201,    66,   202,   202,   202,   202,     7,   202,
     202,   202,   202,   202,   202,   201,     4,   204,   202,   201,
      79,   130,     4,   208,   204,   201,    96,   202,   201,     4,
     201,    95,   202,   201,   201,     4,     4,     4,   202,     5,
      40,   204,   202,    98,   166,     4,   262,   204,   204,   166,
      90,    89,   204,   204,   201,   204,    89,     5,  1117,   406,
     199,    69,   241,   217,    62,   202,   956,   202,   204,   201,
     201,   201,   201,   535,   927,   951,   204,   472,  1011,   820,
     992,   176,  1069,   210,   720,  1006,  1033,   757,  1074,   802,
     528,   504,    -1,   559,   275,    -1,    -1,   411
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   265,   266,   267,   268,   202,   202,     0,
     266,   116,   277,   209,   209,   202,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      32,    33,    34,    35,    36,    38,    43,    44,    45,    46,
      47,    48,    49,    50,    52,    53,    54,    56,    58,    59,
      60,    61,    62,    63,    67,    68,    70,    76,    97,   114,
     120,   123,   124,   126,   128,   134,   137,   138,   141,   142,
     144,   146,   176,   177,   178,   179,   181,   205,   211,   212,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   269,   270,
     271,   272,   279,   281,   282,   283,   284,   286,   287,   295,
     298,   299,   301,   302,   309,   311,   312,   314,   316,   318,
     322,   323,   324,   327,   329,   336,   343,   346,   352,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,   368,
     369,   370,   371,   375,   376,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   201,   117,   278,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   301,   202,   202,   202,   202,   205,   212,
     283,   285,   303,   202,   202,   202,   202,   211,     4,   203,
     218,   219,   254,   431,   441,   203,   279,   260,   202,   313,
       3,   201,   209,   220,   221,   417,   430,    10,    11,    12,
      13,     4,    94,   442,    79,    80,    83,    84,   192,   203,
     224,   225,   344,   419,   420,    98,   185,   186,   187,   188,
     189,   191,   193,   194,   195,   196,   197,   203,   234,   419,
     439,   440,     3,    79,    93,   203,   209,   232,   233,   354,
     428,   429,   354,     3,    79,   203,   209,   230,   231,   348,
     426,   427,    79,   209,   401,    71,    79,   411,   419,   419,
     165,   440,   203,   226,   227,   422,   423,   422,    55,   203,
     449,   450,   209,   419,   419,    91,   419,   203,   419,   449,
     422,   422,    79,   209,   326,   402,   449,   449,   449,   418,
     431,   203,   325,   431,   419,   440,   440,   419,   440,   440,
       4,     8,     9,    79,   443,   419,   440,   326,   440,     5,
      79,   337,   319,   440,   422,   135,   282,   418,   260,   205,
     419,   419,   419,   419,   203,     8,   431,   205,   183,   206,
     255,   256,   257,   258,   259,   430,   431,   436,   437,   438,
      23,    25,    29,    30,    31,   126,   190,   203,   207,   419,
     422,   430,   431,   445,   446,   447,   317,   207,   280,   278,
     205,   261,   201,   201,   201,   200,   421,   203,     4,   205,
     201,   201,   331,   422,   422,   419,   419,   203,   419,   203,
     203,   203,   439,   203,   440,   263,   201,     3,   205,   355,
     353,     3,   205,   347,   201,   201,   201,   201,   201,     4,
     205,   201,   201,   203,     4,   201,   201,   201,   201,   201,
     201,     4,   201,   201,   201,     6,   300,   201,   201,   201,
     201,     4,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   280,   320,   201,   201,   201,   206,   431,   418,
     201,   201,   201,   201,   296,   204,   204,   431,   431,   431,
     431,   431,   431,   431,   201,   204,   202,     4,   291,   292,
     293,   294,   420,   423,   430,   431,   202,   202,   202,   281,
     279,   431,   417,     3,     3,   203,   377,     3,   201,   206,
     440,   201,   431,    84,    85,   345,     5,   210,   201,   201,
      78,   339,   440,   419,   419,     3,     7,   203,   228,   229,
     424,   425,   440,   198,   204,   440,    94,   201,   431,   201,
     280,   201,   431,   280,   419,   419,   440,    78,    78,   201,
     431,    72,    79,   412,    37,     4,   201,    37,    39,    79,
      81,    82,   413,   416,   166,   419,    88,   201,    88,   449,
      79,   203,   332,   201,   449,   449,   449,   418,   201,   325,
     443,   419,    74,    79,   448,    92,    64,   173,   170,    98,
      79,   203,   333,     4,    98,   411,   321,   203,   424,   424,
     136,   315,   143,   261,   310,   206,    79,   430,   131,   132,
     133,   135,   284,   433,   434,   435,   206,   206,   436,   440,
     201,   201,   208,   201,   208,   201,   208,   201,   208,   207,
     428,   207,   426,   207,   424,   208,   206,     3,   201,   421,
     204,     4,   206,   331,   419,   419,   262,   201,   204,   201,
     201,   201,     3,   205,   201,   201,   201,   209,   206,     4,
       8,     9,    57,    71,    72,    73,    75,    77,   102,   103,
     104,   106,   109,   110,   111,   112,   113,   115,   139,   140,
     148,   149,   150,   151,   152,   153,   166,   180,   356,   419,
     441,   444,   209,   206,   349,   341,   201,   201,     4,   206,
     201,   201,     4,   201,   205,   201,   202,   201,     4,   201,
       4,   201,   412,   201,   201,   367,   201,    78,   201,   201,
     202,   372,   201,     4,   201,   201,   201,   125,   201,   201,
     280,   261,   431,   304,   201,   201,   202,   201,   204,     4,
     423,   420,   431,   430,   288,   429,   289,   427,   290,   425,
     201,     3,   204,   210,   201,   339,   440,   431,   431,   440,
     201,   431,   440,   440,   203,   204,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   204,   201,   201,   411,     5,   201,   402,     4,   201,
     166,     4,    79,    86,   415,   426,   166,    88,   201,    79,
     203,   334,   209,    88,   418,   201,   430,    65,   419,   201,
      95,   201,    79,   203,   335,    95,    79,   338,   339,   280,
      95,   424,   207,   412,    79,   430,   214,   215,   432,   433,
     297,   201,   208,   201,   208,   201,   208,   412,   201,   431,
     204,   201,   204,   201,   204,   412,   206,   204,   204,   431,
     418,   428,   101,   100,   418,   107,   108,   418,   418,   418,
     430,   418,   418,   418,   426,    81,    82,   118,   119,   122,
     151,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   350,   414,   416,   430,    75,   105,   106,   112,
     121,   129,   153,   342,   201,   275,     4,   201,   403,   201,
       4,   202,   206,   201,   202,     4,   344,   201,   201,     4,
      79,   340,   201,   201,    89,    96,   112,   129,   166,   167,
     168,   169,   171,   172,   173,   174,   175,   373,     4,     4,
     403,   201,   201,   145,   147,   305,   306,   280,   429,   427,
     425,   204,     5,    40,   431,   204,   256,   202,   430,   202,
     202,   202,   202,   202,   202,   202,     3,   201,    40,    41,
      42,   404,   405,    99,   406,     4,   201,   426,    87,   166,
     426,   204,   201,   335,   418,   204,    66,    90,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   204,   201,
     406,   340,     7,   443,   202,   208,   305,   201,   204,   201,
     204,   431,    79,   418,   418,   418,   165,   209,   351,   428,
       4,   130,     4,   201,   202,    96,   408,   201,     4,   201,
     202,   345,   403,   201,   201,    95,   409,     4,     4,    90,
      92,   130,   374,   426,   428,   377,   377,   419,   401,     4,
       4,   408,   341,   202,   308,     5,    40,   201,   273,   262,
     204,   404,    98,   407,   202,   166,   410,     4,   204,   166,
     426,   204,   406,    90,    89,   204,   410,   307,   279,   201,
     204,   199,   204,   351,   409,   202,   387,   204,   202,   408,
     201,   328,   279,   412,   201,   426,   280,   388,   426,   410,
      89,   280,   201,   419,   330,     5,    79,   276,   201,   274,
     280,   275,   419,   204
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
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1613 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1618 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1623 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1628 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1633 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1638 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1643 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1648 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1653 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1659 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1666 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1675 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1679 "lev_comp.y"
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

  case 254:

/* Line 1455 of yacc.c  */
#line 1692 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1700 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1710 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1718 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1727 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1734 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1742 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1748 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1753 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1758 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1763 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1768 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1773 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1778 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1783 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1788 "lev_comp.y"
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

  case 270:

/* Line 1455 of yacc.c  */
#line 1802 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1808 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1813 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1818 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1823 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1828 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1833 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1838 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1843 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1848 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1853 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1858 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1863 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x400000;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1868 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x800000;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1873 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x1000000;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1878 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x2000000;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1883 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x4000000;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1888 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x8000000;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1895 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1901 "lev_comp.y"
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

  case 290:

/* Line 1455 of yacc.c  */
#line 1930 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1935 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1943 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1949 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1953 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1961 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1968 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1975 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1985 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1995 "lev_comp.y"
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

  case 300:

/* Line 1455 of yacc.c  */
#line 2010 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2020 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2024 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2030 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2036 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2042 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2046 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2052 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2058 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2065 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2073 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2079 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2084 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2089 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2094 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2099 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2105 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2111 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2116 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2121 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2126 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2131 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2136 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2146 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2152 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2158 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2163 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2170 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2177 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2181 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2187 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2193 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2199 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2204 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2211 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2217 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2227 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2233 "lev_comp.y"
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

  case 342:

/* Line 1455 of yacc.c  */
#line 2252 "lev_comp.y"
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

  case 343:

/* Line 1455 of yacc.c  */
#line 2264 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2268 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2274 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2279 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2284 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (long)(yyvsp[(9) - (9)].i), (long)(yyvsp[(7) - (9)].i), (long)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2289 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((long)(yyvsp[(9) - (13)].i), (long)(yyvsp[(7) - (13)].i), (long)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2295 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2299 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2305 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2311 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2315 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2321 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2325 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2331 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2335 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2340 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2347 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2351 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2358 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2362 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2367 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2374 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2378 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2383 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2391 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2397 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2404 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2408 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2415 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2426 "lev_comp.y"
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

  case 375:

/* Line 1455 of yacc.c  */
#line 2439 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2443 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2449 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2453 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2460 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2464 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2468 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2474 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2478 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2484 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2490 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2494 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2500 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2506 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2511 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2528 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2536 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2546 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2555 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2560 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2567 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2578 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2584 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2588 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2592 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2599 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2609 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2616 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2620 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2626 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2630 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2638 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2642 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2649 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2659 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2672 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2676 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2683 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2693 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2702 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2713 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2717 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2724 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2734 "lev_comp.y"
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

  case 429:

/* Line 1455 of yacc.c  */
#line 2745 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2754 "lev_comp.y"
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

  case 431:

/* Line 1455 of yacc.c  */
#line 2764 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2770 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2774 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2781 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2791 "lev_comp.y"
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

  case 436:

/* Line 1455 of yacc.c  */
#line 2802 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2811 "lev_comp.y"
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

  case 438:

/* Line 1455 of yacc.c  */
#line 2821 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2825 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2831 "lev_comp.y"
    { ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2833 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2839 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2843 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2847 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2851 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2859 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2868 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2872 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2876 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2880 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2884 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2887 "lev_comp.y"
    { ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2891 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2898 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2907 "lev_comp.y"
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

  case 460:

/* Line 1455 of yacc.c  */
#line 2948 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2952 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2959 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2966 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2976 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2980 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2988 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2992 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2996 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 3000 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 3004 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 3009 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 3013 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3017 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3022 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3027 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3032 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3036 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3041 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3046 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3050 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3054 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3059 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3063 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3070 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3076 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3080 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3086 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3102 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3106 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3110 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3114 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3136 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3140 "lev_comp.y"
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

  case 507:

/* Line 1455 of yacc.c  */
#line 3166 "lev_comp.y"
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
#line 7171 "lev_comp.tab.c"
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
#line 3194 "lev_comp.y"


/*lev_comp.y*/

