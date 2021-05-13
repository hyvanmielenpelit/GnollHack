
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
     UP_OR_DOWN = 342,
     ACTIVE_OR_INACTIVE = 343,
     MODRON_PORTAL_TYPE = 344,
     NPC_TYPE = 345,
     FOUNTAIN_TYPE = 346,
     SPECIAL_OBJECT_TYPE = 347,
     CMAP_TYPE = 348,
     FLOOR_TYPE = 349,
     FLOOR_TYPE_ID = 350,
     FLOOR_ID = 351,
     FLOOR_MAIN_TYPE = 352,
     FLOOR_MAIN_TYPE_ID = 353,
     ELEMENTAL_ENCHANTMENT_TYPE = 354,
     EXCEPTIONALITY_TYPE = 355,
     EXCEPTIONALITY_ID = 356,
     ELEMENTAL_ENCHANTMENT_ID = 357,
     ENCHANTMENT_ID = 358,
     SECRET_DOOR_ID = 359,
     USES_UP_KEY_ID = 360,
     MYTHIC_PREFIX_TYPE = 361,
     MYTHIC_SUFFIX_TYPE = 362,
     MYTHIC_PREFIX_ID = 363,
     MYTHIC_SUFFIX_ID = 364,
     CHARGES_ID = 365,
     SPECIAL_QUALITY_ID = 366,
     SPEFLAGS_ID = 367,
     SUBROOM_ID = 368,
     NAME_ID = 369,
     FLAGS_ID = 370,
     FLAG_TYPE = 371,
     MON_ATTITUDE = 372,
     MON_ALERTNESS = 373,
     SUBTYPE_ID = 374,
     NON_PASSDOOR_ID = 375,
     MON_APPEARANCE = 376,
     ROOMDOOR_ID = 377,
     IF_ID = 378,
     ELSE_ID = 379,
     TERRAIN_ID = 380,
     HORIZ_OR_VERT = 381,
     REPLACE_TERRAIN_ID = 382,
     LOCATION_SUBTYPE_ID = 383,
     DOOR_SUBTYPE = 384,
     BRAZIER_SUBTYPE = 385,
     SIGNPOST_SUBTYPE = 386,
     TREE_SUBTYPE = 387,
     FOREST_ID = 388,
     FOREST_TYPE = 389,
     INITIALIZE_TYPE = 390,
     EXIT_ID = 391,
     SHUFFLE_ID = 392,
     QUANTITY_ID = 393,
     BURIED_ID = 394,
     LOOP_ID = 395,
     FOR_ID = 396,
     TO_ID = 397,
     SWITCH_ID = 398,
     CASE_ID = 399,
     BREAK_ID = 400,
     DEFAULT_ID = 401,
     ERODED_ID = 402,
     TRAPPED_STATE = 403,
     RECHARGED_ID = 404,
     INVIS_ID = 405,
     GREASED_ID = 406,
     INDESTRUCTIBLE_ID = 407,
     FEMALE_ID = 408,
     WAITFORU_ID = 409,
     CANCELLED_ID = 410,
     REVIVED_ID = 411,
     AVENGE_ID = 412,
     FLEEING_ID = 413,
     BLINDED_ID = 414,
     PARALYZED_ID = 415,
     STUNNED_ID = 416,
     CONFUSED_ID = 417,
     SEENTRAPS_ID = 418,
     ALL_ID = 419,
     MONTYPE_ID = 420,
     OBJTYPE_ID = 421,
     TERTYPE_ID = 422,
     TERTYPE2_ID = 423,
     LEVER_EFFECT_TYPE = 424,
     SWITCHABLE_ID = 425,
     CONTINUOUSLY_USABLE_ID = 426,
     TARGET_ID = 427,
     TRAPTYPE_ID = 428,
     EFFECT_FLAG_ID = 429,
     GRAVE_ID = 430,
     BRAZIER_ID = 431,
     SIGNPOST_ID = 432,
     TREE_ID = 433,
     ERODEPROOF_ID = 434,
     FUNCTION_ID = 435,
     MSG_OUTPUT_TYPE = 436,
     COMPARE_TYPE = 437,
     UNKNOWN_TYPE = 438,
     rect_ID = 439,
     fillrect_ID = 440,
     line_ID = 441,
     randline_ID = 442,
     grow_ID = 443,
     selection_ID = 444,
     flood_ID = 445,
     rndcoord_ID = 446,
     circle_ID = 447,
     ellipse_ID = 448,
     filter_ID = 449,
     complement_ID = 450,
     gradient_ID = 451,
     GRADIENT_TYPE = 452,
     LIMITED = 453,
     HUMIDITY_TYPE = 454,
     STRING = 455,
     MAP_ID = 456,
     NQSTRING = 457,
     VARSTRING = 458,
     CFUNC = 459,
     CFUNC_INT = 460,
     CFUNC_STR = 461,
     CFUNC_COORD = 462,
     CFUNC_REGION = 463,
     VARSTRING_INT = 464,
     VARSTRING_INT_ARRAY = 465,
     VARSTRING_STRING = 466,
     VARSTRING_STRING_ARRAY = 467,
     VARSTRING_VAR = 468,
     VARSTRING_VAR_ARRAY = 469,
     VARSTRING_COORD = 470,
     VARSTRING_COORD_ARRAY = 471,
     VARSTRING_REGION = 472,
     VARSTRING_REGION_ARRAY = 473,
     VARSTRING_MAPCHAR = 474,
     VARSTRING_MAPCHAR_ARRAY = 475,
     VARSTRING_MONST = 476,
     VARSTRING_MONST_ARRAY = 477,
     VARSTRING_OBJ = 478,
     VARSTRING_OBJ_ARRAY = 479,
     VARSTRING_SEL = 480,
     VARSTRING_SEL_ARRAY = 481,
     METHOD_INT = 482,
     METHOD_INT_ARRAY = 483,
     METHOD_STRING = 484,
     METHOD_STRING_ARRAY = 485,
     METHOD_VAR = 486,
     METHOD_VAR_ARRAY = 487,
     METHOD_COORD = 488,
     METHOD_COORD_ARRAY = 489,
     METHOD_REGION = 490,
     METHOD_REGION_ARRAY = 491,
     METHOD_MAPCHAR = 492,
     METHOD_MAPCHAR_ARRAY = 493,
     METHOD_MONST = 494,
     METHOD_MONST_ARRAY = 495,
     METHOD_OBJ = 496,
     METHOD_OBJ_ARRAY = 497,
     METHOD_SEL = 498,
     METHOD_SEL_ARRAY = 499,
     DICE = 500
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
#line 543 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 555 "lev_comp.tab.c"

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
#define YYLAST   1278

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  263
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  187
/* YYNRULES -- Number of rules.  */
#define YYNRULES  503
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1114

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   500

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   258,   262,     2,
     202,   203,   256,   254,   200,   255,   260,   257,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   201,     2,
       2,   259,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   204,     2,   205,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   206,   261,   207,     2,     2,     2,     2,
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
     195,   196,   197,   198,   199,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253
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
    1087,  1093,  1103,  1109,  1113,  1117,  1123,  1133,  1139,  1145,
    1151,  1152,  1166,  1167,  1169,  1177,  1189,  1193,  1201,  1209,
    1215,  1221,  1231,  1237,  1243,  1247,  1255,  1259,  1267,  1275,
    1279,  1285,  1291,  1295,  1301,  1309,  1319,  1321,  1323,  1325,
    1327,  1329,  1330,  1333,  1335,  1339,  1341,  1343,  1345,  1346,
    1350,  1352,  1353,  1357,  1359,  1360,  1364,  1366,  1368,  1370,
    1372,  1374,  1376,  1378,  1380,  1382,  1386,  1388,  1390,  1395,
    1397,  1399,  1404,  1406,  1408,  1413,  1415,  1420,  1426,  1428,
    1432,  1434,  1438,  1440,  1442,  1447,  1457,  1459,  1461,  1466,
    1468,  1474,  1476,  1478,  1483,  1485,  1487,  1493,  1495,  1497,
    1499,  1504,  1506,  1508,  1514,  1516,  1518,  1520,  1524,  1526,
    1528,  1532,  1534,  1539,  1543,  1547,  1551,  1555,  1559,  1563,
    1565,  1567,  1571,  1573,  1577,  1578,  1580,  1582,  1584,  1586,
    1590,  1591,  1593,  1595,  1598,  1601,  1606,  1613,  1618,  1625,
    1632,  1639,  1646,  1649,  1656,  1665,  1674,  1685,  1700,  1703,
    1705,  1709,  1711,  1715,  1717,  1719,  1721,  1723,  1725,  1727,
    1729,  1731,  1733,  1735,  1737,  1739,  1741,  1743,  1745,  1747,
    1749,  1751,  1753,  1764
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     264,     0,    -1,    -1,   265,    -1,   266,    -1,   266,   265,
      -1,   267,   276,   278,    -1,    16,   201,   208,    -1,    15,
     201,   208,   200,     3,    -1,    17,   201,    11,   200,   376,
      -1,    17,   201,    10,   200,     3,    -1,    17,   201,    13,
      -1,    17,   201,    12,   200,     3,   200,     3,   200,     5,
     200,     5,   200,   411,   200,   275,   274,    -1,    18,   201,
     441,    -1,   133,   201,   134,   200,   135,    -1,   133,   201,
     134,    -1,    21,   201,    97,    -1,    -1,   200,   198,    -1,
      -1,   200,   418,    -1,    -1,   200,     3,    -1,     5,    -1,
      79,    -1,    -1,   115,   201,   277,    -1,   116,   200,   277,
      -1,   116,    -1,    -1,   280,   278,    -1,   206,   278,   207,
      -1,   321,    -1,   268,    -1,   269,    -1,   384,    -1,   271,
      -1,   270,    -1,   391,    -1,   390,    -1,   388,    -1,   389,
      -1,   392,    -1,   393,    -1,   394,    -1,   395,    -1,   396,
      -1,   365,    -1,   323,    -1,   286,    -1,   285,    -1,   379,
      -1,   335,    -1,   357,    -1,   398,    -1,   399,    -1,   367,
      -1,   368,    -1,   369,    -1,   397,    -1,   301,    -1,   311,
      -1,   313,    -1,   317,    -1,   315,    -1,   298,    -1,   308,
      -1,   294,    -1,   297,    -1,   360,    -1,   370,    -1,   342,
      -1,   358,    -1,   345,    -1,   351,    -1,   380,    -1,   375,
      -1,   363,    -1,   322,    -1,   381,    -1,   382,    -1,   383,
      -1,   385,    -1,   328,    -1,   326,    -1,   374,    -1,   378,
      -1,   377,    -1,   361,    -1,   362,    -1,   364,    -1,   356,
      -1,   359,    -1,   218,    -1,   220,    -1,   222,    -1,   224,
      -1,   226,    -1,   228,    -1,   230,    -1,   232,    -1,   234,
      -1,   217,    -1,   219,    -1,   221,    -1,   223,    -1,   225,
      -1,   227,    -1,   229,    -1,   231,    -1,   233,    -1,   281,
      -1,   282,    -1,   211,    -1,   211,    -1,   282,    -1,   137,
     201,   281,    -1,   283,   259,   430,    -1,   283,   259,   189,
     201,   439,    -1,   283,   259,   429,    -1,   283,   259,   446,
     201,   423,    -1,   283,   259,   445,   201,   425,    -1,   283,
     259,   444,   201,   427,    -1,   283,   259,   418,    -1,   283,
     259,   421,    -1,   283,   259,   206,   292,   207,    -1,   283,
     259,   206,   291,   207,    -1,   283,   259,   206,   290,   207,
      -1,   283,   259,   446,   201,   206,   289,   207,    -1,   283,
     259,   445,   201,   206,   288,   207,    -1,   283,   259,   444,
     201,   206,   287,   207,    -1,   283,   259,   206,   293,   207,
      -1,   428,    -1,   287,   200,   428,    -1,   426,    -1,   288,
     200,   426,    -1,   424,    -1,   289,   200,   424,    -1,   422,
      -1,   290,   200,   422,    -1,   419,    -1,   291,   200,   419,
      -1,   430,    -1,   292,   200,   430,    -1,   429,    -1,   293,
     200,   429,    -1,    -1,    -1,   180,   210,   202,   295,   434,
     203,   296,   279,    -1,   210,   202,   437,   203,    -1,   136,
      -1,    -1,     6,    -1,     6,    -1,   204,   430,   182,   430,
     205,    -1,   204,   430,   205,    -1,    -1,    -1,   143,   302,
     204,   417,   205,   303,   206,   304,   207,    -1,    -1,   305,
     304,    -1,    -1,   144,   442,   201,   306,   278,    -1,    -1,
     146,   201,   307,   278,    -1,   145,    -1,   260,   260,    -1,
     142,    -1,   141,   284,   259,   430,   309,   430,    -1,    -1,
     310,   312,   279,    -1,    -1,   140,   204,   417,   205,   314,
     279,    -1,    -1,   300,   201,   316,   280,    -1,    -1,   123,
     300,   318,   319,    -1,   279,    -1,    -1,   279,   320,   124,
     279,    -1,    14,   201,   429,    -1,    70,    -1,    70,   201,
     442,    -1,    70,   201,    79,    -1,    58,   201,   324,   200,
     324,    -1,    58,   201,   324,   200,   442,    -1,   202,     4,
     200,    78,   200,   339,   203,    -1,   401,   299,   200,   411,
      -1,    -1,   113,   201,   325,   200,   332,   200,   334,   402,
     405,   407,   409,   327,   279,    -1,    -1,    50,   201,   325,
     200,   331,   200,   333,   200,   334,   402,   405,   407,   409,
     329,   279,    -1,    -1,   200,     5,    -1,   202,     4,   200,
       4,   203,    -1,    79,    -1,   202,     4,   200,     4,   203,
      -1,    79,    -1,   202,   343,   200,   344,   203,    -1,    79,
      -1,   202,     4,   200,     4,   203,    -1,    79,    -1,   122,
     201,   336,   200,   410,   200,   337,   200,   339,   340,    -1,
      27,   201,   410,   200,   439,   340,    -1,     5,    -1,    79,
      -1,   338,    -1,    79,    -1,    78,    -1,    78,   261,   338,
      -1,     4,    -1,    79,    -1,    -1,   340,   200,   341,    -1,
     128,   201,   129,    -1,    75,   201,   427,    -1,   111,   201,
       4,    -1,   152,    -1,   104,    -1,   105,    -1,   120,    -1,
      20,    -1,    19,   201,   343,   200,   344,   330,   209,    -1,
      19,   201,   418,   330,   209,    -1,    83,    -1,    84,    -1,
      85,    -1,    84,    -1,    25,   201,   347,    -1,    -1,    25,
     201,   347,   346,   279,    -1,   425,   200,   418,   348,    -1,
      -1,   348,   200,   349,    -1,   429,    -1,   117,    -1,   118,
      -1,   413,    -1,   121,   429,    -1,   153,    -1,   150,    -1,
     155,    -1,   156,    -1,   157,    -1,   158,   201,   417,    -1,
     159,   201,   417,    -1,   160,   201,   417,    -1,   161,    -1,
     162,    -1,   163,   201,   350,    -1,   154,    -1,   208,    -1,
     164,    -1,   208,   261,   350,    -1,    23,   201,   353,    -1,
      -1,    24,   201,   353,   352,   279,    -1,   427,   354,    -1,
      -1,   354,   200,   355,    -1,    73,    -1,   165,   201,   425,
      -1,   443,    -1,   114,   201,   429,    -1,   138,   201,   417,
      -1,   139,    -1,    72,    -1,   147,   201,   417,    -1,   179,
      -1,    71,    -1,   148,    -1,   149,   201,   417,    -1,   150,
      -1,   151,    -1,   418,    -1,   102,   201,    99,    -1,   101,
     201,   100,    -1,   103,   201,   417,    -1,   110,   201,   417,
      -1,   111,   201,   417,    -1,   112,   201,   417,    -1,    75,
     201,   427,    -1,   152,    -1,   105,    -1,    77,    -1,   108,
     201,   106,    -1,   109,   201,   107,    -1,    57,   201,   417,
      -1,    26,   201,   400,   200,   418,    -1,    28,   201,   418,
     200,    78,   200,   410,    -1,    32,   201,   418,   200,    78,
      -1,    32,   201,   418,   200,    78,   200,     5,   274,    -1,
      33,    -1,    33,   201,   439,    -1,    33,   201,   164,    -1,
      46,   201,   418,   200,    87,    -1,    47,   201,   418,   200,
      87,    -1,    47,   201,   448,   200,   448,   200,    87,    -1,
      52,   201,   448,   200,   448,   200,   208,    -1,    53,   201,
     448,   200,   448,   366,    -1,    54,   201,   448,   200,   448,
      -1,    -1,   200,    87,    -1,    61,   201,   439,   200,    91,
      -1,    62,   201,   439,    -1,    63,   201,   418,   200,   172,
     201,   418,   200,    89,   200,    88,    -1,    63,   201,   418,
     200,    64,   200,    65,   200,    66,   200,    89,   200,    88,
      -1,    76,   201,   418,   200,   169,   371,    -1,    -1,   371,
     200,   372,    -1,    88,    -1,   165,   201,   425,    -1,   166,
     201,   427,    -1,   167,   201,   376,    -1,   168,   201,   376,
      -1,   170,    -1,   171,    -1,   172,   201,   418,    -1,   173,
     201,   400,    -1,   128,   201,   373,    -1,    95,   201,   408,
      -1,   174,   201,     4,    -1,   111,   201,     4,    -1,     4,
      -1,    89,    -1,   129,    -1,    91,    -1,    68,   201,   439,
      -1,    67,   201,   439,    -1,     3,    -1,   202,     3,   200,
     411,   203,    -1,   127,   201,   421,   200,   423,   200,   423,
     200,     7,    -1,   125,   201,   439,   200,   423,    -1,    48,
     201,   421,    -1,    49,   201,   421,    -1,    38,   201,   208,
     200,    39,    -1,    38,   201,   208,   200,    39,   200,   165,
     201,   425,    -1,    35,   201,   421,   200,    37,    -1,    36,
     201,   448,   200,    37,    -1,    22,   201,   439,   200,    93,
      -1,    -1,    34,   201,   421,   200,   411,   200,   401,   402,
     405,   407,   409,   386,   387,    -1,    -1,   279,    -1,    43,
     201,   418,   200,   412,   200,   414,    -1,    43,   201,   418,
     200,   412,   200,   414,   200,   165,   201,   425,    -1,    44,
     201,   418,    -1,    44,   201,   418,   200,   165,   201,   425,
      -1,    96,   201,   439,   200,    97,   200,    94,    -1,   119,
     201,   439,   200,     4,    -1,    45,   201,    90,   200,   418,
      -1,    45,   201,    90,   200,   418,   200,   165,   201,   425,
      -1,   175,   201,   418,   200,   429,    -1,   175,   201,   418,
     200,    79,    -1,   175,   201,   418,    -1,   176,   201,   418,
     200,   130,   200,   411,    -1,   176,   201,   418,    -1,   177,
     201,   418,   200,   131,   200,   429,    -1,   177,   201,   418,
     200,   131,   200,    79,    -1,   177,   201,   418,    -1,   178,
     201,   418,   200,   132,    -1,   178,   201,   418,   200,   134,
      -1,   178,   201,   418,    -1,    59,   201,   430,   200,   418,
      -1,    60,   201,   418,   200,   447,   200,   429,    -1,    56,
     201,   417,   200,   417,   200,   417,   200,   417,    -1,    56,
      -1,   208,    -1,    79,    -1,   208,    -1,    79,    -1,    -1,
     200,   403,    -1,   404,    -1,   404,   200,   403,    -1,    40,
      -1,    41,    -1,    42,    -1,    -1,    98,   201,   406,    -1,
      97,    -1,    -1,    95,   201,   408,    -1,    94,    -1,    -1,
     165,   201,   425,    -1,    71,    -1,    79,    -1,    72,    -1,
      79,    -1,    82,    -1,   415,    -1,    79,    -1,    82,    -1,
     415,    -1,    81,   201,    79,    -1,    86,    -1,    79,    -1,
      81,   204,     4,   205,    -1,   208,    -1,   219,    -1,   220,
     204,   430,   205,    -1,   430,    -1,   419,    -1,   191,   202,
     439,   203,    -1,   223,    -1,   224,   204,   430,   205,    -1,
     202,     4,   200,     4,   203,    -1,    79,    -1,    80,   420,
     205,    -1,   199,    -1,   199,   200,   420,    -1,   422,    -1,
     225,    -1,   226,   204,   430,   205,    -1,   202,     4,   200,
       4,   200,     4,   200,     4,   203,    -1,   424,    -1,   227,
      -1,   228,   204,   430,   205,    -1,     3,    -1,   202,     3,
     200,   411,   203,    -1,   426,    -1,   229,    -1,   230,   204,
     430,   205,    -1,   208,    -1,     3,    -1,   202,     3,   200,
     208,   203,    -1,    79,    -1,   428,    -1,   231,    -1,   232,
     204,   430,   205,    -1,   208,    -1,     3,    -1,   202,     3,
     200,   208,   203,    -1,    79,    -1,    92,    -1,   416,    -1,
     429,   260,   416,    -1,     4,    -1,   440,    -1,   202,     8,
     203,    -1,   217,    -1,   218,   204,   430,   205,    -1,   430,
     254,   430,    -1,   430,   255,   430,    -1,   430,   256,   430,
      -1,   430,   257,   430,    -1,   430,   258,   430,    -1,   202,
     430,   203,    -1,   213,    -1,   214,    -1,   283,   201,   431,
      -1,   432,    -1,   433,   200,   432,    -1,    -1,   433,    -1,
     430,    -1,   429,    -1,   435,    -1,   436,   200,   435,    -1,
      -1,   436,    -1,   418,    -1,   184,   421,    -1,   185,   421,
      -1,   186,   418,   200,   418,    -1,   187,   418,   200,   418,
     200,   430,    -1,   188,   202,   439,   203,    -1,   188,   202,
     338,   200,   439,   203,    -1,   194,   202,     7,   200,   439,
     203,    -1,   194,   202,   439,   200,   439,   203,    -1,   194,
     202,   423,   200,   439,   203,    -1,   190,   418,    -1,   192,
     202,   418,   200,   430,   203,    -1,   192,   202,   418,   200,
     430,   200,    40,   203,    -1,   193,   202,   418,   200,   430,
     200,   430,   203,    -1,   193,   202,   418,   200,   430,   200,
     430,   200,    40,   203,    -1,   196,   202,   197,   200,   202,
     430,   255,   430,   272,   203,   200,   418,   273,   203,    -1,
     195,   438,    -1,   233,    -1,   202,   439,   203,    -1,   438,
      -1,   438,   262,   439,    -1,   253,    -1,    93,    -1,     4,
      -1,     8,    -1,     9,    -1,     4,    -1,     8,    -1,     9,
      -1,     4,    -1,   440,    -1,    29,    -1,    23,    -1,    30,
      -1,    25,    -1,    31,    -1,   125,    -1,    74,    -1,    79,
      -1,   449,    -1,    55,   202,     4,   200,     4,   200,     4,
     200,     4,   203,    -1,   202,     4,   200,     4,   200,     4,
     200,     4,   203,    -1
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
    2162,  2169,  2176,  2182,  2188,  2194,  2199,  2206,  2212,  2222,
    2229,  2228,  2260,  2263,  2269,  2274,  2281,  2285,  2291,  2297,
    2303,  2307,  2313,  2317,  2322,  2329,  2333,  2340,  2344,  2349,
    2356,  2360,  2365,  2373,  2379,  2386,  2390,  2397,  2405,  2408,
    2418,  2422,  2425,  2431,  2435,  2442,  2446,  2450,  2457,  2460,
    2466,  2473,  2476,  2482,  2489,  2493,  2500,  2501,  2504,  2505,
    2508,  2509,  2510,  2516,  2517,  2518,  2524,  2525,  2528,  2537,
    2542,  2549,  2560,  2566,  2570,  2574,  2581,  2591,  2598,  2602,
    2608,  2612,  2620,  2624,  2631,  2641,  2654,  2658,  2665,  2675,
    2684,  2695,  2699,  2706,  2716,  2727,  2736,  2746,  2752,  2756,
    2763,  2773,  2784,  2793,  2803,  2807,  2814,  2815,  2821,  2825,
    2829,  2833,  2841,  2850,  2854,  2858,  2862,  2866,  2870,  2873,
    2880,  2889,  2922,  2923,  2926,  2927,  2930,  2934,  2941,  2948,
    2959,  2962,  2970,  2974,  2978,  2982,  2986,  2991,  2995,  2999,
    3004,  3009,  3014,  3018,  3023,  3028,  3032,  3036,  3041,  3045,
    3052,  3058,  3062,  3068,  3075,  3076,  3079,  3080,  3081,  3084,
    3088,  3092,  3096,  3102,  3103,  3106,  3107,  3110,  3111,  3114,
    3115,  3118,  3122,  3148
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
  "UP_OR_DOWN", "ACTIVE_OR_INACTIVE", "MODRON_PORTAL_TYPE", "NPC_TYPE",
  "FOUNTAIN_TYPE", "SPECIAL_OBJECT_TYPE", "CMAP_TYPE", "FLOOR_TYPE",
  "FLOOR_TYPE_ID", "FLOOR_ID", "FLOOR_MAIN_TYPE", "FLOOR_MAIN_TYPE_ID",
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
  "GREASED_ID", "INDESTRUCTIBLE_ID", "FEMALE_ID", "WAITFORU_ID",
  "CANCELLED_ID", "REVIVED_ID", "AVENGE_ID", "FLEEING_ID", "BLINDED_ID",
  "PARALYZED_ID", "STUNNED_ID", "CONFUSED_ID", "SEENTRAPS_ID", "ALL_ID",
  "MONTYPE_ID", "OBJTYPE_ID", "TERTYPE_ID", "TERTYPE2_ID",
  "LEVER_EFFECT_TYPE", "SWITCHABLE_ID", "CONTINUOUSLY_USABLE_ID",
  "TARGET_ID", "TRAPTYPE_ID", "EFFECT_FLAG_ID", "GRAVE_ID", "BRAZIER_ID",
  "SIGNPOST_ID", "TREE_ID", "ERODEPROOF_ID", "FUNCTION_ID",
  "MSG_OUTPUT_TYPE", "COMPARE_TYPE", "UNKNOWN_TYPE", "rect_ID",
  "fillrect_ID", "line_ID", "randline_ID", "grow_ID", "selection_ID",
  "flood_ID", "rndcoord_ID", "circle_ID", "ellipse_ID", "filter_ID",
  "complement_ID", "gradient_ID", "GRADIENT_TYPE", "LIMITED",
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
  "roomregionflag", "optfloormaintype", "floormaintype", "optfloortype",
  "floortype", "optmontype", "door_state", "light_state", "alignment",
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
      44,    58,    40,    41,    91,    93,   123,   125,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,    43,    45,    42,    47,    37,    61,
      46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   263,   264,   264,   265,   265,   266,   267,   267,   268,
     268,   268,   268,   269,   270,   270,   271,   272,   272,   273,
     273,   274,   274,   275,   275,   276,   276,   277,   277,   278,
     278,   279,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   283,   283,   283,   284,   284,   285,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   287,   287,   288,   288,   289,   289,   290,   290,
     291,   291,   292,   292,   293,   293,   295,   296,   294,   297,
     298,   299,   299,   300,   300,   300,   302,   303,   301,   304,
     304,   306,   305,   307,   305,   308,   309,   309,   310,   312,
     311,   314,   313,   316,   315,   318,   317,   319,   320,   319,
     321,   322,   322,   322,   323,   323,   324,   325,   327,   326,
     329,   328,   330,   330,   331,   331,   332,   332,   333,   333,
     334,   334,   335,   335,   336,   336,   337,   337,   338,   338,
     339,   339,   340,   340,   341,   341,   341,   341,   341,   341,
     341,   342,   342,   342,   343,   343,   344,   344,   345,   346,
     345,   347,   348,   348,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   350,   350,   350,   351,   352,   351,   353,   354,   354,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   356,   357,
     358,   358,   359,   359,   359,   360,   361,   362,   363,   364,
     365,   366,   366,   367,   368,   369,   369,   370,   371,   371,
     372,   372,   372,   372,   372,   372,   372,   372,   372,   372,
     372,   372,   372,   373,   373,   373,   373,   374,   375,   376,
     376,   377,   378,   379,   380,   381,   381,   382,   383,   384,
     386,   385,   387,   387,   388,   388,   389,   389,   390,   391,
     392,   392,   393,   393,   393,   394,   394,   395,   395,   395,
     396,   396,   396,   397,   398,   399,   399,   400,   400,   401,
     401,   402,   402,   403,   403,   404,   404,   404,   405,   405,
     406,   407,   407,   408,   409,   409,   410,   410,   411,   411,
     412,   412,   412,   413,   413,   413,   414,   414,   415,   416,
     416,   416,   417,   418,   418,   418,   418,   419,   419,   419,
     420,   420,   421,   421,   421,   422,   423,   423,   423,   424,
     424,   425,   425,   425,   426,   426,   426,   426,   427,   427,
     427,   428,   428,   428,   428,   428,   429,   429,   430,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   430,   431,
     431,   432,   433,   433,   434,   434,   435,   435,   436,   436,
     437,   437,   438,   438,   438,   438,   438,   438,   438,   438,
     438,   438,   438,   438,   438,   438,   438,   438,   438,   438,
     438,   439,   439,   440,   441,   441,   442,   442,   442,   443,
     443,   443,   443,   444,   444,   445,   445,   446,   446,   447,
     447,   448,   448,   449
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
       5,     9,     5,     3,     3,     5,     9,     5,     5,     5,
       0,    13,     0,     1,     7,    11,     3,     7,     7,     5,
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
       2,     0,     0,     0,     3,     4,    25,     0,     0,     1,
       5,     0,    29,     0,     7,     0,   153,     0,     0,     0,
       0,   221,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   292,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,     0,     0,
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
     115,     0,     0,     0,     0,     0,     0,     0,   438,     0,
     441,     0,   483,     0,   439,   460,    30,     0,   173,     0,
       8,     0,   399,   400,     0,   436,   180,     0,     0,     0,
      11,   485,   484,    13,   408,     0,   224,   225,     0,     0,
     405,     0,     0,   192,   403,    16,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   479,   462,
     481,     0,   432,   434,   435,     0,   431,   429,     0,   254,
     258,   428,   255,   425,   427,     0,   424,   422,     0,   228,
       0,   421,   368,   367,     0,   386,   387,     0,     0,     0,
     294,   293,     0,   413,     0,     0,   412,     0,     0,     0,
       0,   501,     0,     0,   346,     0,     0,     0,     0,     0,
     333,   334,   370,   369,     0,   151,     0,     0,     0,     0,
     402,     0,     0,     0,     0,     0,   304,     0,   328,   327,
     488,   486,   487,   183,   182,     0,     0,     0,     0,   204,
     205,     0,     0,     0,     0,    15,   116,     0,     0,     0,
     354,   356,   359,   362,   146,     0,     0,     0,     0,   155,
       0,     0,     0,     0,     0,   457,   456,   458,   461,     0,
     494,   496,   493,   495,   497,   498,     0,     0,     0,   123,
     124,   119,   117,     0,     0,     0,     0,    29,   170,    27,
       0,     0,     0,     0,     0,   410,     0,     0,     0,     0,
       0,     0,     0,   463,   464,     0,     0,     0,   472,     0,
       0,     0,   478,     0,     0,     0,     0,     0,     0,   257,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   152,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   177,   176,     0,     0,     0,   171,     0,     0,
       0,     0,     0,     0,   454,   440,   448,     0,     0,   443,
     444,   445,   446,   447,     0,   149,     0,   438,     0,     0,
       0,     0,   140,   138,   144,   142,     0,     0,     0,   174,
       0,     0,   437,    10,   329,     0,     9,     0,     0,   409,
       0,     0,     0,   227,   226,   192,   193,   223,     0,     0,
     208,     0,     0,     0,     0,   419,     0,     0,   417,     0,
       0,   416,     0,     0,   480,   482,   339,     0,     0,     0,
     256,     0,     0,   230,   232,   288,   212,     0,   290,     0,
       0,   388,   389,     0,   337,     0,     0,   338,   335,   392,
       0,   390,     0,   391,     0,   350,   295,     0,   296,     0,
     195,     0,     0,     0,     0,   301,   300,     0,     0,   184,
     185,   363,   499,   500,     0,   303,     0,     0,   308,     0,
     197,     0,     0,   349,     0,     0,     0,   332,     0,    14,
       0,   167,     0,     0,   157,   353,   352,     0,     0,   360,
     361,     0,   452,   455,     0,   442,   154,   459,   118,     0,
       0,   127,     0,   126,     0,   125,     0,   131,     0,   122,
       0,   121,     0,   120,    31,   401,     0,     0,   411,   404,
       0,   406,     0,   465,     0,     0,     0,   467,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,   491,   489,
     490,     0,   269,   266,   260,     0,   284,     0,     0,     0,
     283,     0,     0,     0,     0,     0,     0,     0,   265,     0,
     270,     0,   272,   273,   282,     0,   268,   259,   274,   492,
     262,     0,   423,   231,   203,     0,     0,     0,   414,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   187,     0,     0,   299,     0,     0,     0,     0,     0,
     307,     0,     0,     0,     0,     0,     0,   172,   166,   168,
       0,     0,     0,     0,     0,   147,     0,   139,   141,   143,
     145,     0,   132,     0,   134,     0,   136,     0,     0,   407,
     222,     0,   209,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   426,     0,
       0,   289,    21,     0,   371,     0,     0,     0,     0,   397,
     396,   344,   347,     0,   297,     0,   199,     0,     0,   298,
     302,     0,     0,   364,     0,     0,     0,   348,     0,   201,
       0,   371,   207,     0,   206,   179,     0,   159,   355,   358,
     357,   449,   450,   451,   453,     0,     0,   130,     0,   129,
       0,   128,     0,     0,   466,   468,     0,   473,     0,   469,
       0,   418,   471,   470,     0,   287,   281,   276,   275,   277,
     285,   286,   278,   279,   280,   263,   264,   267,   271,   261,
       0,   393,   235,   236,     0,   240,   239,   250,   241,   242,
     243,     0,     0,     0,   247,   248,     0,   233,   237,   394,
     234,     0,   218,   219,     0,   220,     0,   217,   213,     0,
     291,     0,     0,   378,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,   210,   211,     0,     0,     0,   310,
       0,     0,     0,     0,     0,     0,     0,   315,   316,     0,
       0,     0,   309,     0,     0,   378,     0,     0,     0,     0,
       0,   159,   148,   133,   135,   137,   330,     0,     0,     0,
     420,     0,     0,   238,     0,     0,     0,     0,     0,     0,
       0,    22,     0,   375,   376,   377,   372,   373,     0,   381,
       0,     0,   336,     0,   351,   194,     0,   371,   365,   186,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   196,     0,   381,   212,   331,     0,   163,   158,
     160,     0,   474,     0,   475,   444,   395,   244,   245,   246,
     252,   251,   249,   215,   216,   214,     0,     0,     0,     0,
     384,     0,     0,     0,     0,   378,     0,     0,   383,   320,
     322,   323,   324,   326,   325,   319,   311,   312,   313,   314,
     317,   318,   321,     0,   384,   202,   161,    29,     0,     0,
       0,     0,     0,   415,   374,   380,   379,     0,     0,   340,
       0,   503,   345,   198,   381,     0,   305,   200,   188,    29,
     164,     0,   476,    18,     0,   253,   382,     0,   342,   502,
     384,     0,     0,   162,     0,     0,   385,   343,   341,   190,
     306,   189,     0,    19,     0,    23,    24,    21,     0,     0,
     191,    12,    20,   477
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    98,    99,   100,   101,  1061,
    1109,   910,  1107,    12,   168,   102,   408,   103,   104,   105,
     106,   221,   107,   108,   761,   763,   765,   508,   509,   510,
     511,   109,   494,   845,   110,   111,   466,   112,   113,   222,
     750,   950,   951,  1079,  1057,   114,   623,   115,   116,   239,
     117,   620,   118,   406,   119,   362,   483,   615,   120,   121,
     122,   342,   334,   123,  1092,   124,  1104,   422,   592,   612,
     818,   831,   125,   361,   833,   541,   926,   714,   908,   126,
     262,   535,   127,   443,   299,   713,   897,  1022,   128,   440,
     289,   439,   707,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   734,   139,   140,   141,   142,   740,   942,
    1045,   143,   144,   526,   145,   146,   147,   148,   149,   150,
     151,   152,   153,  1088,  1098,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   304,   335,   913,
     976,   977,   979,  1066,  1030,  1039,  1069,   307,   573,   582,
     898,   811,   583,   245,   339,   279,   264,   416,   315,   316,
     550,   551,   300,   301,   290,   291,   385,   340,   843,   632,
     633,   634,   387,   388,   389,   280,   434,   234,   253,   354,
     710,   403,   404,   405,   604,   320,   321
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -973
static const yytype_int16 yypact[] =
{
     139,   -97,   -84,    86,  -973,   139,     8,   -41,   -26,  -973,
    -973,   -10,   771,    29,  -973,   119,  -973,    52,    83,   102,
     126,  -973,   131,   148,   152,   157,   178,   193,   198,   216,
     220,   224,   227,   234,   236,   237,   243,   253,   254,   259,
     266,   267,   269,   270,   281,   292,   294,   295,   315,   316,
     324,   325,   328,   329,   333,   337,   338,   342,   346,   349,
     351,   353,    32,   354,   360,   361,  -973,   364,   110,   711,
    -973,  -973,   369,   372,   380,   384,   -13,   122,   115,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,   771,  -973,  -973,   123,  -973,  -973,  -973,
    -973,  -973,   390,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,   454,   196,  -973,   -31,
     478,    58,    -6,   436,   822,   113,   113,   133,   -47,    17,
     -29,   -29,   658,  -115,  -115,   -21,   341,   -29,   -29,   458,
     -29,   340,  -115,  -115,   -39,   -21,   -21,   -21,   122,   391,
     122,   -29,   822,   822,   -29,   822,   822,    81,   -29,   822,
     -39,   822,   104,  -973,   822,  -115,   460,   808,   122,  -973,
    -973,   355,   388,   -29,   -29,   -29,   -29,   413,  -973,    38,
    -973,   421,  -973,   195,  -973,   111,  -973,   393,  -973,   435,
    -973,   119,  -973,  -973,   438,  -973,   383,   440,   447,   448,
    -973,  -973,  -973,  -973,  -973,   455,  -973,  -973,   461,   660,
    -973,   469,   462,   474,  -973,  -973,  -115,  -115,   -29,   -29,
     475,   -29,   476,   477,   483,   822,   484,   465,  -973,  -973,
     428,   491,  -973,  -973,  -973,   673,  -973,  -973,   488,  -973,
    -973,  -973,  -973,  -973,  -973,   690,  -973,  -973,   492,   489,
     497,  -973,  -973,  -973,   501,  -973,  -973,   502,   503,   504,
    -973,  -973,   696,  -973,   507,   508,  -973,   512,   516,   722,
     528,  -973,   532,   534,   539,   540,   541,   740,   543,   546,
    -973,  -973,  -973,  -973,   547,   754,   561,   563,   568,   574,
     147,   772,   575,   208,   578,   579,  -973,   580,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,   581,   582,   584,   587,  -973,
    -973,   600,   435,   601,   602,   608,  -973,   605,   122,   122,
     611,   613,   635,   636,  -973,   634,  -124,   122,   122,  -973,
     122,   122,   122,   122,   122,   383,   147,  -973,   640,   652,
    -973,  -973,  -973,  -973,  -973,  -973,   655,   116,    14,  -973,
    -973,   383,   147,   656,   657,   665,   771,   771,  -973,  -973,
     122,   -31,   856,    21,   866,   670,   666,   822,   672,   122,
     136,   868,   671,  -973,  -973,   674,   683,   529,  -973,   -29,
     -29,   313,  -973,   688,   684,   822,   793,   692,   122,   695,
     435,   697,   122,   435,   -29,   -29,   822,   810,   821,   700,
     122,    20,   869,   899,   705,   872,   871,   213,   748,   -29,
     828,   723,   838,   -21,   -44,  -973,   726,   -21,   -21,   -21,
     122,   727,    40,   -29,    27,   840,     3,   760,   836,   -36,
     931,    17,   815,  -973,    55,    55,   806,  -973,   223,   738,
     207,   823,   814,    -7,   739,  -973,  -973,   185,   256,   -32,
     -32,  -973,  -973,  -973,   111,  -973,   822,   752,  -105,   -95,
     -86,    45,  -973,  -973,   383,   147,   149,   181,   144,  -973,
     767,   348,  -973,  -973,  -973,   952,  -973,   777,   455,  -973,
     775,   975,   426,  -973,  -973,   474,  -973,  -973,   -29,   -29,
     719,   783,   781,   787,   811,  -973,   819,   442,  -973,   816,
     827,  -973,   829,   831,  -973,  -973,  -973,   825,   500,    99,
    -973,   833,   607,  -973,  -973,  -973,  -973,   835,   837,  1019,
     621,  -973,  -973,   839,  -973,   843,  1040,  -973,   847,  -973,
     844,  -973,   849,  -973,   850,   852,  -973,  1046,  -973,   853,
    -973,  1052,   870,    20,   873,   874,  -973,   875,   990,  -973,
    -973,  -973,  -973,  -973,   876,  -973,   877,   878,  -973,   880,
    -973,  1065,   881,  -973,   882,   947,  1069,  -973,   883,  -973,
     435,  -973,   818,   122,  -973,  -973,   383,   884,   885,  -973,
    -973,   886,  -973,   888,   887,  -973,  -973,  -973,  -973,  1082,
     889,  -973,    -9,  -973,   122,  -973,   -31,  -973,   135,  -973,
     140,  -973,    28,  -973,  -973,  -973,   892,  1086,  -973,  -973,
     890,  -973,   891,  -973,   894,  1017,   822,  -973,   122,   122,
     822,   896,   122,   822,   822,   895,   898,  -973,  -973,  -973,
    -973,   897,  -973,  -973,  -973,   901,  -973,   902,   903,   905,
    -973,   906,   907,   908,   909,   910,   912,   913,  -973,   914,
    -973,   915,  -973,  -973,  -973,   916,  -973,  -973,  -973,  -973,
    -973,   917,  -973,   918,   919,    17,  1094,   921,  -973,   -39,
    1101,   922,   960,  1122,   190,   133,   962,   -81,  1041,   929,
      -4,  -973,   923,  1043,  -973,   122,   932,   -31,  1068,   -29,
     934,  1042,   935,    -3,   201,   435,    55,  -973,  -973,   147,
     933,    20,   228,   160,   739,  -973,   -63,  -973,  -973,   147,
     383,    70,  -973,   154,  -973,   191,  -973,    20,   937,  -973,
    -973,   122,  -973,   938,   332,   302,   939,    20,   663,   940,
     941,   122,  -973,   122,   113,  1038,  1047,   122,  1034,  1044,
     122,   122,   122,   -31,   122,   122,   122,   133,  -973,   904,
     631,  -973,   945,  1143,   948,   949,  1146,   951,   950,  -973,
    -973,   953,  -973,   955,  -973,  1150,  -973,   347,   957,  -973,
    -973,   958,    65,   383,   959,   961,   599,  -973,  1156,  -973,
    1158,   948,  -973,   963,  -973,  -973,   964,    -5,  -973,  -973,
     383,  -973,  -973,  -973,  -973,   435,   135,  -973,   140,  -973,
      28,  -973,   965,  1160,   147,  -973,  1126,  -973,   122,  -973,
     966,  -973,  -973,  -973,   414,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,   383,  -973,  -973,  -973,  -973,
       2,  -973,  -973,  -973,   -31,  -973,  -973,  -973,  -973,  -973,
    -973,   969,   970,   971,  -973,  -973,   972,  -973,  -973,  -973,
     383,   973,  -973,  -973,   974,  -973,   976,  -973,  -973,  1164,
    -973,   978,   434,  1078,  1175,   980,   133,  -973,  1016,   133,
     979,   983,    -3,   122,  -973,  -973,   981,  1119,  1097,  -973,
     986,   987,   988,   989,   991,   992,   993,  -973,  -973,   994,
     995,   996,  -973,   997,   998,  1078,    65,  1184,   141,  1000,
     999,    -5,  -973,  -973,  -973,  -973,  -973,  1002,  1001,   366,
    -973,   122,  1120,   383,   122,   122,   122,   -73,   113,  1199,
    1076,  -973,  1203,  -973,  -973,  -973,  -973,  1008,  1009,  1114,
    1011,  1208,  -973,  1012,  -973,  -973,   136,   948,  -973,  -973,
    1014,  1015,  1123,  1212,    57,   133,   113,    21,    21,   -29,
     -47,  1214,  -973,  1215,  1114,  -973,  -973,  1020,  -973,  -973,
    -973,  1217,  -973,  1180,  -973,   156,  -973,  -973,  -973,  -973,
    -973,   967,  -973,  -973,  -973,  -973,  1021,   434,  1128,  1022,
    1061,  1223,  1026,   133,  1027,  1078,  1142,  1144,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  1030,  1061,   919,  -973,   771,  1035,  1031,
    1039,  1033,   -73,  -973,  -973,  -973,  -973,  1123,  1037,  -973,
    1036,  -973,  -973,  -973,  1114,  1045,  -973,  -973,  -973,   771,
    -973,    20,  -973,  -973,  1048,  -973,  -973,   133,   435,  -973,
    1061,  1152,   435,  -973,  1049,   -29,  -973,  -973,  -973,  -973,
    -973,  -973,   108,  1050,   435,  -973,  -973,   945,   -29,  1051,
    -973,  -973,  -973,  -973
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -973,  -973,  1236,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,   137,  -973,  -973,  1005,  -103,  -359,   841,  1025,  1174,
    -486,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  1189,  -973,  -973,
    -973,   301,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,   784,  1053,  -973,  -973,  -973,  -973,   718,  -973,  -973,
    -973,   335,  -973,  -973,  -973,  -585,   309,   255,  -973,  -973,
     441,   273,  -973,  -973,  -973,  -973,  -973,   199,  -973,  -973,
    1088,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -564,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,   262,   548,  -819,
     238,  -973,  -906,  -973,  -932,   202,  -972,  -452,  -587,  -973,
    -973,  -973,   467,   857,  -216,  -171,  -376,   742,   192,  -375,
    -471,  -611,  -510,  -605,  -512,  -584,  -154,   -72,  -973,   517,
    -973,  -973,   768,  -973,  -973,  1003,  -146,   714,  -973,  -461,
    -973,  -973,  -973,  -973,  -973,  -170,  -973
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -230
static const yytype_int16 yytable[] =
{
     236,   263,   367,   482,   649,   233,   731,   651,   631,   308,
     309,   600,   945,   617,   618,   246,   323,   324,   228,   326,
     328,   329,   512,   513,   524,   336,   337,   338,   281,   614,
     344,   545,   302,   347,   318,   590,   311,   355,    16,  1004,
     332,   766,   228,   610,   350,   764,   375,   653,   351,   352,
     254,   255,   370,   371,   372,   373,   345,   346,   545,   348,
     349,  1041,   251,   356,   762,   358,   399,   606,   363,   924,
     254,   255,  1054,   254,   255,   816,   829,   256,   257,   496,
     772,   560,  1078,   401,   563,   350,     9,   312,   305,   351,
     352,  1020,   571,   254,   255,   640,   306,   425,   426,   572,
     428,   602,   641,   678,     7,   642,   603,   679,   680,   359,
     313,   314,   643,  1105,   644,   228,   282,     8,  1099,   806,
     507,   645,   769,    11,   375,   629,   228,   630,   343,  1074,
     380,   381,   382,   383,   384,  1021,   293,   803,   282,   948,
     769,   949,  1090,   293,   925,   350,  1042,   545,  1043,   351,
     352,   252,   282,   489,     1,     2,   681,   376,   591,   834,
     353,   303,   258,   386,   838,   402,   611,    13,  1035,   333,
     682,   683,   684,   259,   685,   607,   686,   242,   254,   255,
     852,   319,    14,   360,   293,   258,  1044,  1106,   243,   244,
     860,    15,   283,   259,   260,   261,   259,   227,   817,   830,
     687,   688,   689,   962,   690,   284,   723,   691,   692,   693,
     694,   695,   294,   696,   283,   812,   397,   260,   261,   294,
     533,   534,   242,   525,   382,   383,   384,   284,   283,   166,
     616,   230,   231,   243,   244,   167,    77,   697,   698,   955,
     229,   284,   341,   954,   514,   646,   699,   700,   701,   702,
     703,   704,   647,   169,   597,   230,   231,   616,   543,   544,
     294,   747,   953,   801,   705,   757,   758,   232,   631,   809,
     846,   530,   866,   564,   565,   836,   810,   847,   706,   540,
     832,   542,   548,   549,   170,   552,   625,   879,   585,   555,
     258,   232,   579,   589,   580,   581,   488,   594,   595,   596,
     566,   259,   601,   171,   520,   497,   498,   839,   499,   500,
     501,   502,   503,   229,   218,   285,   545,   235,   229,   242,
     546,   286,   260,   261,   229,   376,   515,   172,   230,   231,
     243,   244,   173,   230,   231,   295,   626,   285,   521,   230,
     231,   296,   295,   286,   287,   288,   616,   532,   296,   174,
     652,   285,   232,   175,   848,   648,  1060,   286,   176,   -17,
     638,   849,   297,   298,   232,   621,   558,   663,   664,   232,
     562,   548,   549,   841,   842,   232,   317,   378,   570,   177,
     287,   288,   237,   295,   330,   331,   835,   650,   708,   296,
     635,   850,   254,   255,   178,   318,   241,   228,   851,   179,
     379,   380,   381,   382,   383,   384,   982,   364,   473,   984,
     297,   298,   382,   383,   384,   242,   390,   180,   391,   254,
     255,   181,   392,   393,   394,   182,   243,   244,   183,   400,
     256,   257,   386,  1048,  1049,   184,   242,   185,   186,   380,
     381,   382,   383,   384,   187,   671,   418,   243,   244,   380,
     381,   382,   383,   384,   188,   189,  1023,   240,   423,   424,
     190,   636,   380,   381,   382,   383,   384,   191,   192,   418,
     193,   194,   254,   255,   973,   974,   975,   380,   381,   382,
     383,   384,   195,   622,  1047,  1046,   952,  1007,   247,   248,
     249,   250,   760,   196,  1094,   197,   198,   266,   267,   268,
     269,   270,   858,   271,   258,   272,   273,   274,   275,   276,
     380,   381,   382,   383,   384,   547,   199,   200,   395,   821,
     773,   254,   255,  1072,   776,   201,   202,   779,   780,   203,
     204,   258,   856,   265,   205,   857,   260,   261,   206,   207,
     548,   549,   327,   208,   254,   255,   278,   209,   325,   322,
     210,   749,   211,   655,   212,   214,   380,   381,   382,   383,
     384,   215,   216,   260,   261,   217,  1013,   865,   825,  1014,
     223,   869,   759,   224,   872,   873,   874,  1096,   876,   877,
     878,   225,   396,   823,   258,   226,   380,   381,   382,   383,
     384,   238,   369,   341,   365,   397,   774,   775,   840,   398,
     778,   242,   380,   381,   382,   383,   384,   540,   254,   255,
     230,   231,   243,   244,   368,   374,   260,   261,   313,   314,
     380,   381,   382,   383,   384,   377,   266,   267,   268,   269,
     270,   661,   271,   258,   272,   273,   274,   275,   276,   875,
     412,   407,   410,   411,   277,   900,   232,   413,   414,   266,
     267,   268,   269,   270,   415,   271,   258,   272,   273,   274,
     275,   276,   420,   417,   418,   260,   261,   277,   380,   961,
     382,   383,   384,   419,   421,   278,   437,   427,   429,   430,
     380,   381,   382,   383,   384,   431,   433,   929,   260,   261,
     435,   436,   438,   441,   930,  -229,   442,   444,   278,   854,
     449,   445,   446,   447,   448,   677,   901,   988,   451,   864,
     931,   450,   452,   266,   267,   268,   269,   270,   453,   271,
     258,   272,   273,   274,   275,   276,   454,   932,   455,  1097,
     963,   277,   456,  1101,   457,   902,   903,   254,   255,   458,
     459,   460,   904,   462,   461,  1110,   463,   464,  1017,  1018,
    1019,   905,   260,   261,   380,   381,   382,   383,   384,   906,
     465,   467,   278,   468,   933,   934,   935,   936,   469,   937,
     938,   939,   940,   941,   470,   472,   471,    16,   474,   475,
     476,   477,   478,   907,   479,    17,   959,   480,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
     481,   484,   485,    30,    31,    32,    33,    34,   486,    35,
     487,   490,   712,   491,    36,    37,    38,    39,    40,    41,
      42,    43,   310,    44,    45,    46,   718,    47,  1050,    48,
      49,    50,    51,    52,    53,   492,   493,   495,    54,    55,
     504,    56,   266,   267,   268,   269,   270,    57,   271,   258,
     272,   273,   274,   275,   276,   505,   506,   516,   517,   523,
     277,   380,   381,   382,   383,   384,   518,    58,   861,   527,
     528,   529,   531,   536,   538,   380,   381,   382,   383,   384,
     537,   260,   261,   539,    59,   553,   556,   554,   567,  1015,
      60,   278,   557,    61,    62,   559,    63,   561,    64,   568,
     569,   254,   255,   575,    65,   576,   574,    66,    67,   577,
     578,    68,    69,   584,    70,   586,    71,   380,   381,   382,
     383,   384,   219,   587,  1103,   588,   593,   598,    80,   608,
      82,   605,    84,   609,    86,   613,    88,  1112,    90,  -178,
      92,   619,    94,   624,    96,   628,    72,    73,    74,    75,
      79,    76,   639,   627,  1080,   656,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,   654,    77,  1093,   657,   659,   660,
     665,    78,    79,   666,   667,   880,   881,   668,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,   266,   267,   268,   269,
     270,   669,   271,   258,   272,   273,   274,   275,   276,   670,
     672,   882,   883,   717,   277,   884,    81,   673,    83,   674,
      85,   675,    87,   676,    89,   715,    91,   716,    93,   719,
      95,   711,    97,   720,   721,   260,   261,   722,   723,   724,
     727,   725,   726,   728,   885,   278,   729,   886,   887,   888,
     889,   890,   891,   892,   893,   894,   895,   896,   736,   742,
     730,   745,   671,   732,   733,   735,   737,   738,   748,   739,
     741,   743,   744,   746,   751,   752,   756,   753,   754,   768,
     755,   312,   767,   769,   771,   540,   777,   781,   783,   802,
     770,   782,   784,   785,   786,   805,   787,   788,   789,   790,
     791,   792,   242,   793,   794,   795,   796,   797,   799,   800,
     798,   803,   806,   243,   244,   807,   808,   813,   814,   815,
     820,   819,   822,   824,   826,   828,   827,   853,   867,   837,
     870,   855,   859,   862,   863,   909,   868,   911,   912,   914,
     915,   871,   916,   918,   920,   917,   919,   922,   923,   927,
     943,   928,   944,   946,   947,   957,   958,   971,   956,   960,
     964,   965,   966,   967,   968,   969,   978,   970,   972,   980,
     981,   983,   985,   986,   989,   990,   991,   992,   993,   994,
     995,  1006,   996,   997,   998,   999,  1000,  1001,  1003,  1016,
    1002,  1008,  1011,  1024,  1012,  1025,  1009,  1026,  1027,  1029,
    1028,  1031,  1032,  1033,  1036,  1037,  1040,  1038,  1052,  1053,
    1059,  1056,  1058,  1067,  1063,  1065,  1068,  1070,  1062,  1071,
    1073,  1075,  1076,  1077,  1082,  1081,  1084,  1083,  1087,  1089,
    1100,    10,   366,   220,  1111,  1091,   409,   519,  1095,  1102,
    1108,   213,  1010,   662,  1113,  1005,   599,   987,   921,  1034,
    1055,  1085,  1051,   357,   292,  1064,   899,   804,   522,  1086,
     658,   844,   637,   709,     0,     0,     0,     0,   432
};

static const yytype_int16 yycheck[] =
{
     103,   172,   218,   362,   516,    77,   593,   517,   494,   180,
     181,   472,   831,   484,   485,   169,   187,   188,     4,   190,
     191,   191,   398,   398,     3,   195,   196,   197,   174,   481,
     201,     3,    79,   204,    55,    79,   182,   208,     6,   945,
      79,   652,     4,    79,     4,   650,     8,   518,     8,     9,
      79,    80,   223,   224,   225,   226,   202,   203,     3,   205,
     206,     4,     4,   209,   648,   211,   237,    64,   214,     4,
      79,    80,  1004,    79,    80,    79,    79,    83,    84,   203,
     665,   440,  1054,   237,   443,     4,     0,   202,    71,     8,
       9,   164,    72,    79,    80,   200,    79,   268,   269,    79,
     271,    74,   207,     4,   201,   200,    79,     8,     9,     5,
     225,   226,   207,     5,   200,     4,     3,   201,  1090,   200,
       4,   207,   203,   115,     8,   132,     4,   134,   200,  1035,
     254,   255,   256,   257,   258,   208,     3,   200,     3,   144,
     203,   146,  1074,     3,    79,     4,    89,     3,    91,     8,
       9,    93,     3,   369,    15,    16,    57,   229,   202,   744,
      79,   208,   191,   235,   751,   237,   202,   208,   987,   208,
      71,    72,    73,   202,    75,   172,    77,   208,    79,    80,
     767,   202,   208,    79,     3,   191,   129,    79,   219,   220,
     777,   201,    79,   202,   223,   224,   202,   210,   202,   202,
     101,   102,   103,   201,   105,    92,   204,   108,   109,   110,
     111,   112,    79,   114,    79,   725,   202,   223,   224,    79,
      84,    85,   208,   202,   256,   257,   258,    92,    79,   200,
     202,   217,   218,   219,   220,   116,   204,   138,   139,   850,
     202,    92,   202,   848,   398,   200,   147,   148,   149,   150,
     151,   152,   207,   201,   470,   217,   218,   202,   429,   430,
      79,   620,   846,   715,   165,   640,   642,   253,   754,    79,
     200,   417,   784,   444,   445,   746,    86,   207,   179,    78,
      79,   427,   227,   228,   201,   431,    79,   797,   459,   435,
     191,   253,    79,   463,    81,    82,   368,   467,   468,   469,
     446,   202,   473,   201,   407,   377,   378,    79,   380,   381,
     382,   383,   384,   202,   204,   202,     3,   202,   202,   208,
       7,   208,   223,   224,   202,   397,   398,   201,   217,   218,
     219,   220,   201,   217,   218,   202,   490,   202,   410,   217,
     218,   208,   202,   208,   231,   232,   202,   419,   208,   201,
     206,   202,   253,   201,   200,   206,   200,   208,   201,   203,
     506,   207,   229,   230,   253,   142,   438,   538,   539,   253,
     442,   227,   228,   213,   214,   253,   184,   182,   450,   201,
     231,   232,   259,   202,   192,   193,   745,   206,   559,   208,
     205,   200,    79,    80,   201,    55,   200,     4,   207,   201,
     205,   254,   255,   256,   257,   258,   916,   215,   200,   919,
     229,   230,   256,   257,   258,   208,    23,   201,    25,    79,
      80,   201,    29,    30,    31,   201,   219,   220,   201,   237,
      83,    84,   504,   997,   998,   201,   208,   201,   201,   254,
     255,   256,   257,   258,   201,     3,     4,   219,   220,   254,
     255,   256,   257,   258,   201,   201,   968,     3,   266,   267,
     201,   205,   254,   255,   256,   257,   258,   201,   201,     4,
     201,   201,    79,    80,    40,    41,    42,   254,   255,   256,
     257,   258,   201,   260,   996,   995,   845,   948,    10,    11,
      12,    13,   646,   201,  1081,   201,   201,   184,   185,   186,
     187,   188,   200,   190,   191,   192,   193,   194,   195,   196,
     254,   255,   256,   257,   258,   202,   201,   201,   125,   735,
     666,    79,    80,  1033,   670,   201,   201,   673,   674,   201,
     201,   191,   200,    97,   201,   203,   223,   224,   201,   201,
     227,   228,   202,   201,    79,    80,   233,   201,    90,   208,
     201,   623,   201,   205,   201,   201,   254,   255,   256,   257,
     258,   201,   201,   223,   224,   201,   200,   783,   739,   203,
     201,   787,   644,   201,   790,   791,   792,  1087,   794,   795,
     796,   201,   189,   737,   191,   201,   254,   255,   256,   257,
     258,   201,   204,   202,   134,   202,   668,   669,   752,   206,
     672,   208,   254,   255,   256,   257,   258,    78,    79,    80,
     217,   218,   219,   220,   259,   202,   223,   224,   225,   226,
     254,   255,   256,   257,   258,   204,   184,   185,   186,   187,
     188,   205,   190,   191,   192,   193,   194,   195,   196,   793,
     200,   206,   204,   260,   202,   799,   253,   200,   200,   184,
     185,   186,   187,   188,   199,   190,   191,   192,   193,   194,
     195,   196,   200,   202,     4,   223,   224,   202,   254,   255,
     256,   257,   258,   204,   200,   233,     3,   202,   202,   202,
     254,   255,   256,   257,   258,   202,   202,    88,   223,   224,
     262,   200,   204,     3,    95,   206,   204,   200,   233,   771,
       4,   200,   200,   200,   200,   205,    75,   923,   200,   781,
     111,   204,   200,   184,   185,   186,   187,   188,   202,   190,
     191,   192,   193,   194,   195,   196,     4,   128,   200,  1088,
     884,   202,   200,  1092,   200,   104,   105,    79,    80,   200,
     200,   200,   111,   200,     4,  1104,   200,   200,   964,   965,
     966,   120,   223,   224,   254,   255,   256,   257,   258,   128,
       6,   200,   233,   200,   165,   166,   167,   168,   200,   170,
     171,   172,   173,   174,   200,   200,     4,     6,   200,   200,
     200,   200,   200,   152,   200,    14,   858,   200,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
     200,   200,   200,    32,    33,    34,    35,    36,   200,    38,
     205,   200,   205,   200,    43,    44,    45,    46,    47,    48,
      49,    50,   164,    52,    53,    54,   205,    56,   999,    58,
      59,    60,    61,    62,    63,   200,   200,   203,    67,    68,
     200,    70,   184,   185,   186,   187,   188,    76,   190,   191,
     192,   193,   194,   195,   196,   203,   201,   201,   201,     3,
     202,   254,   255,   256,   257,   258,   201,    96,   205,     3,
     200,   205,   200,     5,   200,   254,   255,   256,   257,   258,
     209,   223,   224,   200,   113,   197,    93,   203,    78,   961,
     119,   233,   200,   122,   123,   200,   125,   200,   127,    78,
     200,    79,    80,     4,   133,   200,    37,   136,   137,    37,
      39,   140,   141,   165,   143,    87,   145,   254,   255,   256,
     257,   258,   211,   200,  1095,    87,   200,   200,   217,   169,
     219,    91,   221,    97,   223,     4,   225,  1108,   227,   124,
     229,   135,   231,   205,   233,   131,   175,   176,   177,   178,
     211,   180,   200,   130,  1057,     3,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   207,   204,  1079,   200,   203,     4,
     261,   210,   211,   200,   203,    81,    82,   200,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   184,   185,   186,   187,
     188,   200,   190,   191,   192,   193,   194,   195,   196,   200,
     204,   117,   118,     4,   202,   121,   218,   200,   220,   200,
     222,   200,   224,   208,   226,   200,   228,   200,   230,   200,
     232,   208,   234,   200,     4,   223,   224,   200,   204,   200,
       4,   201,   200,   200,   150,   233,     4,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    78,     4,
     200,   124,     3,   200,   200,   200,   200,   200,   260,   201,
     200,   200,   200,   200,   200,   200,     4,   201,   200,     3,
     203,   202,   200,   203,   200,    78,   200,   202,   201,     5,
     209,   203,   201,   201,   201,     4,   201,   201,   201,   201,
     201,   201,   208,   201,   201,   201,   201,   201,   200,   200,
     203,   200,   200,   219,   220,   165,     4,   165,    87,   200,
      87,   208,   200,    65,   200,   200,    94,   200,   100,   206,
     106,   203,   203,   203,   203,   200,    99,     4,   200,   200,
       4,   107,   201,   200,     4,   205,   201,   200,   200,   200,
       4,   200,     4,   200,   200,     5,    40,     3,   203,   203,
     201,   201,   201,   201,   201,   201,    98,   201,   200,     4,
     200,   165,   203,   200,   203,    66,    89,   201,   201,   201,
     201,     7,   201,   201,   201,   201,   201,   201,   200,    79,
     203,   201,   200,     4,   203,   129,   207,     4,   200,    95,
     201,   200,     4,   201,   200,   200,     4,    94,     4,     4,
      40,   201,     5,   201,   203,    97,   165,     4,   261,   203,
     203,    89,    88,   203,   203,   200,   203,   198,   201,   203,
      88,     5,   217,    69,  1107,   200,   241,   406,   200,   200,
     200,    62,   951,   535,   203,   946,   472,   922,   817,   986,
    1005,  1062,  1000,   210,   176,  1027,   799,   719,   411,  1067,
     528,   754,   504,   559,    -1,    -1,    -1,    -1,   275
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   264,   265,   266,   267,   201,   201,     0,
     265,   115,   276,   208,   208,   201,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      32,    33,    34,    35,    36,    38,    43,    44,    45,    46,
      47,    48,    49,    50,    52,    53,    54,    56,    58,    59,
      60,    61,    62,    63,    67,    68,    70,    76,    96,   113,
     119,   122,   123,   125,   127,   133,   136,   137,   140,   141,
     143,   145,   175,   176,   177,   178,   180,   204,   210,   211,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   268,   269,
     270,   271,   278,   280,   281,   282,   283,   285,   286,   294,
     297,   298,   300,   301,   308,   310,   311,   313,   315,   317,
     321,   322,   323,   326,   328,   335,   342,   345,   351,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   367,
     368,   369,   370,   374,   375,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   200,   116,   277,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   300,   201,   201,   201,   201,   204,   211,
     282,   284,   302,   201,   201,   201,   201,   210,     4,   202,
     217,   218,   253,   430,   440,   202,   278,   259,   201,   312,
       3,   200,   208,   219,   220,   416,   429,    10,    11,    12,
      13,     4,    93,   441,    79,    80,    83,    84,   191,   202,
     223,   224,   343,   418,   419,    97,   184,   185,   186,   187,
     188,   190,   192,   193,   194,   195,   196,   202,   233,   418,
     438,   439,     3,    79,    92,   202,   208,   231,   232,   353,
     427,   428,   353,     3,    79,   202,   208,   229,   230,   347,
     425,   426,    79,   208,   400,    71,    79,   410,   418,   418,
     164,   439,   202,   225,   226,   421,   422,   421,    55,   202,
     448,   449,   208,   418,   418,    90,   418,   202,   418,   448,
     421,   421,    79,   208,   325,   401,   448,   448,   448,   417,
     430,   202,   324,   430,   418,   439,   439,   418,   439,   439,
       4,     8,     9,    79,   442,   418,   439,   325,   439,     5,
      79,   336,   318,   439,   421,   134,   281,   417,   259,   204,
     418,   418,   418,   418,   202,     8,   430,   204,   182,   205,
     254,   255,   256,   257,   258,   429,   430,   435,   436,   437,
      23,    25,    29,    30,    31,   125,   189,   202,   206,   418,
     421,   429,   430,   444,   445,   446,   316,   206,   279,   277,
     204,   260,   200,   200,   200,   199,   420,   202,     4,   204,
     200,   200,   330,   421,   421,   418,   418,   202,   418,   202,
     202,   202,   438,   202,   439,   262,   200,     3,   204,   354,
     352,     3,   204,   346,   200,   200,   200,   200,   200,     4,
     204,   200,   200,   202,     4,   200,   200,   200,   200,   200,
     200,     4,   200,   200,   200,     6,   299,   200,   200,   200,
     200,     4,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   279,   319,   200,   200,   200,   205,   430,   417,
     200,   200,   200,   200,   295,   203,   203,   430,   430,   430,
     430,   430,   430,   430,   200,   203,   201,     4,   290,   291,
     292,   293,   419,   422,   429,   430,   201,   201,   201,   280,
     278,   430,   416,     3,     3,   202,   376,     3,   200,   205,
     439,   200,   430,    84,    85,   344,     5,   209,   200,   200,
      78,   338,   439,   418,   418,     3,     7,   202,   227,   228,
     423,   424,   439,   197,   203,   439,    93,   200,   430,   200,
     279,   200,   430,   279,   418,   418,   439,    78,    78,   200,
     430,    72,    79,   411,    37,     4,   200,    37,    39,    79,
      81,    82,   412,   415,   165,   418,    87,   200,    87,   448,
      79,   202,   331,   200,   448,   448,   448,   417,   200,   324,
     442,   418,    74,    79,   447,    91,    64,   172,   169,    97,
      79,   202,   332,     4,   410,   320,   202,   423,   423,   135,
     314,   142,   260,   309,   205,    79,   429,   130,   131,   132,
     134,   283,   432,   433,   434,   205,   205,   435,   439,   200,
     200,   207,   200,   207,   200,   207,   200,   207,   206,   427,
     206,   425,   206,   423,   207,   205,     3,   200,   420,   203,
       4,   205,   330,   418,   418,   261,   200,   203,   200,   200,
     200,     3,   204,   200,   200,   200,   208,   205,     4,     8,
       9,    57,    71,    72,    73,    75,    77,   101,   102,   103,
     105,   108,   109,   110,   111,   112,   114,   138,   139,   147,
     148,   149,   150,   151,   152,   165,   179,   355,   418,   440,
     443,   208,   205,   348,   340,   200,   200,     4,   205,   200,
     200,     4,   200,   204,   200,   201,   200,     4,   200,     4,
     200,   411,   200,   200,   366,   200,    78,   200,   200,   201,
     371,   200,     4,   200,   200,   124,   200,   279,   260,   430,
     303,   200,   200,   201,   200,   203,     4,   422,   419,   430,
     429,   287,   428,   288,   426,   289,   424,   200,     3,   203,
     209,   200,   338,   439,   430,   430,   439,   200,   430,   439,
     439,   202,   203,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   203,   200,
     200,   410,     5,   200,   401,     4,   200,   165,     4,    79,
      86,   414,   425,   165,    87,   200,    79,   202,   333,   208,
      87,   417,   200,   429,    65,   418,   200,    94,   200,    79,
     202,   334,    79,   337,   338,   279,   423,   206,   411,    79,
     429,   213,   214,   431,   432,   296,   200,   207,   200,   207,
     200,   207,   411,   200,   430,   203,   200,   203,   200,   203,
     411,   205,   203,   203,   430,   417,   427,   100,    99,   417,
     106,   107,   417,   417,   417,   429,   417,   417,   417,   425,
      81,    82,   117,   118,   121,   150,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   349,   413,   415,
     429,    75,   104,   105,   111,   120,   128,   152,   341,   200,
     274,     4,   200,   402,   200,     4,   201,   205,   200,   201,
       4,   343,   200,   200,     4,    79,   339,   200,   200,    88,
      95,   111,   128,   165,   166,   167,   168,   170,   171,   172,
     173,   174,   372,     4,     4,   402,   200,   200,   144,   146,
     304,   305,   279,   428,   426,   424,   203,     5,    40,   430,
     203,   255,   201,   429,   201,   201,   201,   201,   201,   201,
     201,     3,   200,    40,    41,    42,   403,   404,    98,   405,
       4,   200,   425,   165,   425,   203,   200,   334,   417,   203,
      66,    89,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   203,   200,   405,   339,     7,   442,   201,   207,
     304,   200,   203,   200,   203,   430,    79,   417,   417,   417,
     164,   208,   350,   427,     4,   129,     4,   200,   201,    95,
     407,   200,     4,   201,   344,   402,   200,   200,    94,   408,
       4,     4,    89,    91,   129,   373,   425,   427,   376,   376,
     418,   400,     4,     4,   407,   340,   201,   307,     5,    40,
     200,   272,   261,   203,   403,    97,   406,   201,   165,   409,
       4,   203,   425,   203,   405,    89,    88,   203,   409,   306,
     278,   200,   203,   198,   203,   350,   408,   201,   386,   203,
     407,   200,   327,   278,   411,   200,   425,   279,   387,   409,
      88,   279,   200,   418,   329,     5,    79,   275,   200,   273,
     279,   274,   418,   203
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
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2183 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2189 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2195 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2200 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2207 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2213 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2223 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2229 "lev_comp.y"
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

  case 341:

/* Line 1455 of yacc.c  */
#line 2248 "lev_comp.y"
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

  case 342:

/* Line 1455 of yacc.c  */
#line 2260 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2264 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2270 "lev_comp.y"
    {
		      add_opvars(splev, "Miio",
				 VA_PASS4(-1, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2275 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2282 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2286 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2292 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2298 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2304 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2308 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2314 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2318 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2323 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2330 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2334 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2341 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2345 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2350 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2357 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2361 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2366 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2374 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2380 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2387 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2391 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2398 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2409 "lev_comp.y"
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

  case 371:

/* Line 1455 of yacc.c  */
#line 2422 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2426 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2432 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2436 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2443 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2447 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2451 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2457 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2461 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2467 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2473 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2477 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2483 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2489 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2494 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2511 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2519 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2529 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2538 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2543 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2550 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2561 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2567 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2571 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2575 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2582 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2592 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2599 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2603 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2609 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2613 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2621 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2625 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2632 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2642 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2655 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2659 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2666 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2676 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2685 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2696 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2700 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2707 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2717 "lev_comp.y"
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

  case 425:

/* Line 1455 of yacc.c  */
#line 2728 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2737 "lev_comp.y"
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

  case 427:

/* Line 1455 of yacc.c  */
#line 2747 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2753 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2757 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2764 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2774 "lev_comp.y"
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

  case 432:

/* Line 1455 of yacc.c  */
#line 2785 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2794 "lev_comp.y"
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

  case 434:

/* Line 1455 of yacc.c  */
#line 2804 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2808 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2814 "lev_comp.y"
    { ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2816 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2822 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2826 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2830 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2834 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2842 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2851 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2855 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2859 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2863 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2867 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2870 "lev_comp.y"
    { ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2874 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2881 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2890 "lev_comp.y"
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

  case 456:

/* Line 1455 of yacc.c  */
#line 2931 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2935 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2942 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2949 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2959 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2963 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2971 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2975 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2979 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2983 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2987 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2992 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2996 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 3000 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 3005 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 3010 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 3015 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3019 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3024 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3029 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3033 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3037 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3042 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3046 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3053 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3059 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3063 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3069 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3085 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3089 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3093 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3097 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3119 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3123 "lev_comp.y"
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

  case 503:

/* Line 1455 of yacc.c  */
#line 3149 "lev_comp.y"
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
#line 7123 "lev_comp.tab.c"
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
#line 3177 "lev_comp.y"


/*lev_comp.y*/

