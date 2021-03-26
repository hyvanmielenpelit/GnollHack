
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
     OBJECT_ID = 277,
     COBJECT_ID = 278,
     MONSTER_ID = 279,
     TRAP_ID = 280,
     DOOR_ID = 281,
     DRAWBRIDGE_ID = 282,
     object_ID = 283,
     monster_ID = 284,
     terrain_ID = 285,
     MAZEWALK_ID = 286,
     WALLIFY_ID = 287,
     REGION_ID = 288,
     SPECIAL_REGION_ID = 289,
     SPECIAL_LEVREGION_ID = 290,
     SPECIAL_REGION_TYPE = 291,
     NAMING_ID = 292,
     NAMING_TYPE = 293,
     FILLING = 294,
     IRREGULAR = 295,
     JOINED = 296,
     ALTAR_ID = 297,
     ANVIL_ID = 298,
     NPC_ID = 299,
     LADDER_ID = 300,
     STAIR_ID = 301,
     NON_DIGGABLE_ID = 302,
     NON_PASSWALL_ID = 303,
     ROOM_ID = 304,
     ARTIFACT_NAME_ID = 305,
     PORTAL_ID = 306,
     TELEPRT_ID = 307,
     BRANCH_ID = 308,
     LEV = 309,
     MINERALIZE_ID = 310,
     CORRIDOR_ID = 311,
     GOLD_ID = 312,
     ENGRAVING_ID = 313,
     FOUNTAIN_ID = 314,
     THRONE_ID = 315,
     MODRON_PORTAL_ID = 316,
     LEVEL_TELEPORTER_ID = 317,
     LEVEL_TELEPORT_DIRECTION_TYPE = 318,
     LEVEL_TELEPORT_END_TYPE = 319,
     POOL_ID = 320,
     SINK_ID = 321,
     NONE = 322,
     RAND_CORRIDOR_ID = 323,
     DOOR_STATE = 324,
     LIGHT_STATE = 325,
     CURSE_TYPE = 326,
     ENGRAVING_TYPE = 327,
     KEYTYPE_ID = 328,
     LEVER_ID = 329,
     NO_PICKUP_ID = 330,
     DIRECTION = 331,
     RANDOM_TYPE = 332,
     RANDOM_TYPE_BRACKET = 333,
     A_REGISTER = 334,
     ALIGNMENT = 335,
     LEFT_OR_RIGHT = 336,
     CENTER = 337,
     TOP_OR_BOT = 338,
     ALTAR_TYPE = 339,
     UP_OR_DOWN = 340,
     ACTIVE_OR_INACTIVE = 341,
     MODRON_PORTAL_TYPE = 342,
     NPC_TYPE = 343,
     FOUNTAIN_TYPE = 344,
     SPECIAL_OBJECT_TYPE = 345,
     CMAP_TYPE = 346,
     FLOOR_TYPE = 347,
     FLOOR_TYPE_ID = 348,
     FLOOR_ID = 349,
     FLOOR_MAIN_TYPE = 350,
     FLOOR_MAIN_TYPE_ID = 351,
     ELEMENTAL_ENCHANTMENT_TYPE = 352,
     EXCEPTIONALITY_TYPE = 353,
     EXCEPTIONALITY_ID = 354,
     ELEMENTAL_ENCHANTMENT_ID = 355,
     ENCHANTMENT_ID = 356,
     SECRET_DOOR_ID = 357,
     USES_UP_KEY_ID = 358,
     CHARGES_ID = 359,
     SPECIAL_QUALITY_ID = 360,
     SPEFLAGS_ID = 361,
     SUBROOM_ID = 362,
     NAME_ID = 363,
     FLAGS_ID = 364,
     FLAG_TYPE = 365,
     MON_ATTITUDE = 366,
     MON_ALERTNESS = 367,
     SUBTYPE_ID = 368,
     NON_PASSDOOR_ID = 369,
     MON_APPEARANCE = 370,
     ROOMDOOR_ID = 371,
     IF_ID = 372,
     ELSE_ID = 373,
     TERRAIN_ID = 374,
     HORIZ_OR_VERT = 375,
     REPLACE_TERRAIN_ID = 376,
     LOCATION_SUBTYPE_ID = 377,
     DOOR_SUBTYPE = 378,
     BRAZIER_SUBTYPE = 379,
     SIGNPOST_SUBTYPE = 380,
     EXIT_ID = 381,
     SHUFFLE_ID = 382,
     QUANTITY_ID = 383,
     BURIED_ID = 384,
     LOOP_ID = 385,
     FOR_ID = 386,
     TO_ID = 387,
     SWITCH_ID = 388,
     CASE_ID = 389,
     BREAK_ID = 390,
     DEFAULT_ID = 391,
     ERODED_ID = 392,
     TRAPPED_STATE = 393,
     RECHARGED_ID = 394,
     INVIS_ID = 395,
     GREASED_ID = 396,
     INDESTRUCTIBLE_ID = 397,
     FEMALE_ID = 398,
     WAITFORU_ID = 399,
     CANCELLED_ID = 400,
     REVIVED_ID = 401,
     AVENGE_ID = 402,
     FLEEING_ID = 403,
     BLINDED_ID = 404,
     PARALYZED_ID = 405,
     STUNNED_ID = 406,
     CONFUSED_ID = 407,
     SEENTRAPS_ID = 408,
     ALL_ID = 409,
     MONTYPE_ID = 410,
     OBJTYPE_ID = 411,
     TERTYPE_ID = 412,
     TERTYPE2_ID = 413,
     LEVER_EFFECT_TYPE = 414,
     SWITCHABLE_ID = 415,
     CONTINUOUSLY_USABLE_ID = 416,
     TARGET_ID = 417,
     TRAPTYPE_ID = 418,
     EFFECT_FLAG_ID = 419,
     GRAVE_ID = 420,
     BRAZIER_ID = 421,
     SIGNPOST_ID = 422,
     ERODEPROOF_ID = 423,
     FUNCTION_ID = 424,
     MSG_OUTPUT_TYPE = 425,
     COMPARE_TYPE = 426,
     UNKNOWN_TYPE = 427,
     rect_ID = 428,
     fillrect_ID = 429,
     line_ID = 430,
     randline_ID = 431,
     grow_ID = 432,
     selection_ID = 433,
     flood_ID = 434,
     rndcoord_ID = 435,
     circle_ID = 436,
     ellipse_ID = 437,
     filter_ID = 438,
     complement_ID = 439,
     gradient_ID = 440,
     GRADIENT_TYPE = 441,
     LIMITED = 442,
     HUMIDITY_TYPE = 443,
     STRING = 444,
     MAP_ID = 445,
     NQSTRING = 446,
     VARSTRING = 447,
     CFUNC = 448,
     CFUNC_INT = 449,
     CFUNC_STR = 450,
     CFUNC_COORD = 451,
     CFUNC_REGION = 452,
     VARSTRING_INT = 453,
     VARSTRING_INT_ARRAY = 454,
     VARSTRING_STRING = 455,
     VARSTRING_STRING_ARRAY = 456,
     VARSTRING_VAR = 457,
     VARSTRING_VAR_ARRAY = 458,
     VARSTRING_COORD = 459,
     VARSTRING_COORD_ARRAY = 460,
     VARSTRING_REGION = 461,
     VARSTRING_REGION_ARRAY = 462,
     VARSTRING_MAPCHAR = 463,
     VARSTRING_MAPCHAR_ARRAY = 464,
     VARSTRING_MONST = 465,
     VARSTRING_MONST_ARRAY = 466,
     VARSTRING_OBJ = 467,
     VARSTRING_OBJ_ARRAY = 468,
     VARSTRING_SEL = 469,
     VARSTRING_SEL_ARRAY = 470,
     METHOD_INT = 471,
     METHOD_INT_ARRAY = 472,
     METHOD_STRING = 473,
     METHOD_STRING_ARRAY = 474,
     METHOD_VAR = 475,
     METHOD_VAR_ARRAY = 476,
     METHOD_COORD = 477,
     METHOD_COORD_ARRAY = 478,
     METHOD_REGION = 479,
     METHOD_REGION_ARRAY = 480,
     METHOD_MAPCHAR = 481,
     METHOD_MAPCHAR_ARRAY = 482,
     METHOD_MONST = 483,
     METHOD_MONST_ARRAY = 484,
     METHOD_OBJ = 485,
     METHOD_OBJ_ARRAY = 486,
     METHOD_SEL = 487,
     METHOD_SEL_ARRAY = 488,
     DICE = 489
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
#line 532 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 544 "lev_comp.tab.c"

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
#define YYLAST   1262

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  252
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  184
/* YYNRULES -- Number of rules.  */
#define YYNRULES  491
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1086

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   489

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   247,   251,     2,
     191,   192,   245,   243,   189,   244,   249,   246,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   190,     2,
       2,   248,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   193,     2,   194,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   195,   250,   196,     2,     2,     2,     2,
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
     185,   186,   187,   188,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    15,    19,    25,
      31,    37,    41,    58,    62,    66,    67,    70,    71,    74,
      75,    78,    80,    82,    83,    87,    91,    93,    94,    97,
     101,   103,   105,   107,   109,   111,   113,   115,   117,   119,
     121,   123,   125,   127,   129,   131,   133,   135,   137,   139,
     141,   143,   145,   147,   149,   151,   153,   155,   157,   159,
     161,   163,   165,   167,   169,   171,   173,   175,   177,   179,
     181,   183,   185,   187,   189,   191,   193,   195,   197,   199,
     201,   203,   205,   207,   209,   211,   213,   215,   217,   219,
     221,   223,   225,   227,   229,   231,   233,   235,   237,   239,
     241,   243,   245,   247,   249,   251,   253,   255,   257,   259,
     261,   263,   267,   271,   277,   281,   287,   293,   299,   303,
     307,   313,   319,   325,   333,   341,   349,   355,   357,   361,
     363,   367,   369,   373,   375,   379,   381,   385,   387,   391,
     393,   397,   398,   399,   408,   413,   415,   416,   418,   420,
     426,   430,   431,   432,   442,   443,   446,   447,   453,   454,
     459,   461,   464,   466,   473,   474,   478,   479,   486,   487,
     492,   493,   498,   500,   501,   506,   510,   512,   516,   520,
     526,   532,   540,   545,   546,   560,   561,   577,   578,   581,
     587,   589,   595,   597,   603,   605,   611,   613,   624,   631,
     633,   635,   637,   639,   641,   645,   647,   649,   650,   654,
     658,   662,   666,   668,   670,   672,   674,   676,   684,   690,
     692,   694,   696,   698,   702,   703,   709,   714,   715,   719,
     721,   723,   725,   727,   730,   732,   734,   736,   738,   740,
     744,   748,   752,   754,   756,   760,   762,   764,   766,   770,
     774,   775,   781,   784,   785,   789,   791,   795,   797,   801,
     805,   807,   809,   813,   815,   817,   819,   823,   825,   827,
     829,   833,   837,   841,   845,   849,   853,   857,   859,   861,
     863,   869,   877,   883,   892,   894,   898,   902,   908,   914,
     922,   930,   937,   943,   944,   947,   953,   957,   969,   983,
     990,   991,   995,   997,  1001,  1005,  1009,  1013,  1015,  1017,
    1021,  1025,  1029,  1033,  1037,  1041,  1043,  1045,  1047,  1049,
    1053,  1057,  1059,  1065,  1075,  1081,  1085,  1089,  1095,  1105,
    1111,  1117,  1118,  1132,  1133,  1135,  1143,  1155,  1159,  1167,
    1175,  1181,  1187,  1197,  1203,  1209,  1213,  1221,  1225,  1233,
    1241,  1245,  1251,  1259,  1269,  1271,  1273,  1275,  1277,  1279,
    1280,  1283,  1285,  1289,  1291,  1293,  1295,  1296,  1300,  1302,
    1303,  1307,  1309,  1310,  1314,  1316,  1318,  1320,  1322,  1324,
    1326,  1328,  1330,  1332,  1336,  1338,  1340,  1345,  1347,  1349,
    1354,  1356,  1358,  1363,  1365,  1370,  1376,  1378,  1382,  1384,
    1388,  1390,  1392,  1397,  1407,  1409,  1411,  1416,  1418,  1424,
    1426,  1428,  1433,  1435,  1437,  1443,  1445,  1447,  1449,  1454,
    1456,  1458,  1464,  1466,  1468,  1470,  1474,  1476,  1478,  1482,
    1484,  1489,  1493,  1497,  1501,  1505,  1509,  1513,  1515,  1517,
    1521,  1523,  1527,  1528,  1530,  1532,  1534,  1536,  1540,  1541,
    1543,  1545,  1548,  1551,  1556,  1563,  1568,  1575,  1582,  1589,
    1596,  1599,  1606,  1615,  1624,  1635,  1650,  1653,  1655,  1659,
    1661,  1665,  1667,  1669,  1671,  1673,  1675,  1677,  1679,  1681,
    1683,  1685,  1687,  1689,  1691,  1693,  1695,  1697,  1699,  1701,
    1703,  1714
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     253,     0,    -1,    -1,   254,    -1,   255,    -1,   255,   254,
      -1,   256,   264,   266,    -1,    16,   190,   197,    -1,    15,
     190,   197,   189,     3,    -1,    17,   190,    11,   189,   364,
      -1,    17,   190,    10,   189,     3,    -1,    17,   190,    13,
      -1,    17,   190,    12,   189,     3,   189,     3,   189,     5,
     189,     5,   189,   397,   189,   263,   262,    -1,    18,   190,
     427,    -1,    21,   190,    95,    -1,    -1,   189,   187,    -1,
      -1,   189,   404,    -1,    -1,   189,     3,    -1,     5,    -1,
      77,    -1,    -1,   109,   190,   265,    -1,   110,   189,   265,
      -1,   110,    -1,    -1,   268,   266,    -1,   195,   266,   196,
      -1,   309,    -1,   257,    -1,   258,    -1,   259,    -1,   378,
      -1,   377,    -1,   375,    -1,   376,    -1,   379,    -1,   380,
      -1,   381,    -1,   382,    -1,   353,    -1,   311,    -1,   274,
      -1,   273,    -1,   367,    -1,   323,    -1,   345,    -1,   384,
      -1,   385,    -1,   355,    -1,   356,    -1,   357,    -1,   383,
      -1,   289,    -1,   299,    -1,   301,    -1,   305,    -1,   303,
      -1,   286,    -1,   296,    -1,   282,    -1,   285,    -1,   348,
      -1,   358,    -1,   330,    -1,   346,    -1,   333,    -1,   339,
      -1,   368,    -1,   363,    -1,   351,    -1,   310,    -1,   369,
      -1,   370,    -1,   371,    -1,   372,    -1,   316,    -1,   314,
      -1,   362,    -1,   366,    -1,   365,    -1,   349,    -1,   350,
      -1,   352,    -1,   344,    -1,   347,    -1,   207,    -1,   209,
      -1,   211,    -1,   213,    -1,   215,    -1,   217,    -1,   219,
      -1,   221,    -1,   223,    -1,   206,    -1,   208,    -1,   210,
      -1,   212,    -1,   214,    -1,   216,    -1,   218,    -1,   220,
      -1,   222,    -1,   269,    -1,   270,    -1,   200,    -1,   200,
      -1,   270,    -1,   127,   190,   269,    -1,   271,   248,   416,
      -1,   271,   248,   178,   190,   425,    -1,   271,   248,   415,
      -1,   271,   248,   432,   190,   409,    -1,   271,   248,   431,
     190,   411,    -1,   271,   248,   430,   190,   413,    -1,   271,
     248,   404,    -1,   271,   248,   407,    -1,   271,   248,   195,
     280,   196,    -1,   271,   248,   195,   279,   196,    -1,   271,
     248,   195,   278,   196,    -1,   271,   248,   432,   190,   195,
     277,   196,    -1,   271,   248,   431,   190,   195,   276,   196,
      -1,   271,   248,   430,   190,   195,   275,   196,    -1,   271,
     248,   195,   281,   196,    -1,   414,    -1,   275,   189,   414,
      -1,   412,    -1,   276,   189,   412,    -1,   410,    -1,   277,
     189,   410,    -1,   408,    -1,   278,   189,   408,    -1,   405,
      -1,   279,   189,   405,    -1,   416,    -1,   280,   189,   416,
      -1,   415,    -1,   281,   189,   415,    -1,    -1,    -1,   169,
     199,   191,   283,   420,   192,   284,   267,    -1,   199,   191,
     423,   192,    -1,   126,    -1,    -1,     6,    -1,     6,    -1,
     193,   416,   171,   416,   194,    -1,   193,   416,   194,    -1,
      -1,    -1,   133,   290,   193,   403,   194,   291,   195,   292,
     196,    -1,    -1,   293,   292,    -1,    -1,   134,   428,   190,
     294,   266,    -1,    -1,   136,   190,   295,   266,    -1,   135,
      -1,   249,   249,    -1,   132,    -1,   131,   272,   248,   416,
     297,   416,    -1,    -1,   298,   300,   267,    -1,    -1,   130,
     193,   403,   194,   302,   267,    -1,    -1,   288,   190,   304,
     268,    -1,    -1,   117,   288,   306,   307,    -1,   267,    -1,
      -1,   267,   308,   118,   267,    -1,    14,   190,   415,    -1,
      68,    -1,    68,   190,   428,    -1,    68,   190,    77,    -1,
      56,   190,   312,   189,   312,    -1,    56,   190,   312,   189,
     428,    -1,   191,     4,   189,    76,   189,   327,   192,    -1,
     387,   287,   189,   397,    -1,    -1,   107,   190,   313,   189,
     320,   189,   322,   388,   391,   393,   395,   315,   267,    -1,
      -1,    49,   190,   313,   189,   319,   189,   321,   189,   322,
     388,   391,   393,   395,   317,   267,    -1,    -1,   189,     5,
      -1,   191,     4,   189,     4,   192,    -1,    77,    -1,   191,
       4,   189,     4,   192,    -1,    77,    -1,   191,   331,   189,
     332,   192,    -1,    77,    -1,   191,     4,   189,     4,   192,
      -1,    77,    -1,   116,   190,   324,   189,   396,   189,   325,
     189,   327,   328,    -1,    26,   190,   396,   189,   425,   328,
      -1,     5,    -1,    77,    -1,   326,    -1,    77,    -1,    76,
      -1,    76,   250,   326,    -1,     4,    -1,    77,    -1,    -1,
     328,   189,   329,    -1,   122,   190,   123,    -1,    73,   190,
     413,    -1,   105,   190,     4,    -1,   142,    -1,   102,    -1,
     103,    -1,   114,    -1,    20,    -1,    19,   190,   331,   189,
     332,   318,   198,    -1,    19,   190,   404,   318,   198,    -1,
      81,    -1,    82,    -1,    83,    -1,    82,    -1,    24,   190,
     335,    -1,    -1,    24,   190,   335,   334,   267,    -1,   411,
     189,   404,   336,    -1,    -1,   336,   189,   337,    -1,   415,
      -1,   111,    -1,   112,    -1,   399,    -1,   115,   415,    -1,
     143,    -1,   140,    -1,   145,    -1,   146,    -1,   147,    -1,
     148,   190,   403,    -1,   149,   190,   403,    -1,   150,   190,
     403,    -1,   151,    -1,   152,    -1,   153,   190,   338,    -1,
     144,    -1,   197,    -1,   154,    -1,   197,   250,   338,    -1,
      22,   190,   341,    -1,    -1,    23,   190,   341,   340,   267,
      -1,   413,   342,    -1,    -1,   342,   189,   343,    -1,    71,
      -1,   155,   190,   411,    -1,   429,    -1,   108,   190,   415,
      -1,   128,   190,   403,    -1,   129,    -1,    70,    -1,   137,
     190,   403,    -1,   168,    -1,    69,    -1,   138,    -1,   139,
     190,   403,    -1,   140,    -1,   141,    -1,   404,    -1,   100,
     190,    97,    -1,    99,   190,    98,    -1,   101,   190,   403,
      -1,   104,   190,   403,    -1,   105,   190,   403,    -1,   106,
     190,   403,    -1,    73,   190,   413,    -1,   142,    -1,   103,
      -1,    75,    -1,    25,   190,   386,   189,   404,    -1,    27,
     190,   404,   189,    76,   189,   396,    -1,    31,   190,   404,
     189,    76,    -1,    31,   190,   404,   189,    76,   189,     5,
     262,    -1,    32,    -1,    32,   190,   425,    -1,    32,   190,
     154,    -1,    45,   190,   404,   189,    85,    -1,    46,   190,
     404,   189,    85,    -1,    46,   190,   434,   189,   434,   189,
      85,    -1,    51,   190,   434,   189,   434,   189,   197,    -1,
      52,   190,   434,   189,   434,   354,    -1,    53,   190,   434,
     189,   434,    -1,    -1,   189,    85,    -1,    59,   190,   425,
     189,    89,    -1,    60,   190,   425,    -1,    61,   190,   404,
     189,   162,   190,   404,   189,    87,   189,    86,    -1,    61,
     190,   404,   189,    62,   189,    63,   189,    64,   189,    87,
     189,    86,    -1,    74,   190,   404,   189,   159,   359,    -1,
      -1,   359,   189,   360,    -1,    86,    -1,   155,   190,   411,
      -1,   156,   190,   413,    -1,   157,   190,   364,    -1,   158,
     190,   364,    -1,   160,    -1,   161,    -1,   162,   190,   404,
      -1,   163,   190,   386,    -1,   122,   190,   361,    -1,    93,
     190,   394,    -1,   164,   190,     4,    -1,   105,   190,     4,
      -1,     4,    -1,    87,    -1,   123,    -1,    89,    -1,    66,
     190,   425,    -1,    65,   190,   425,    -1,     3,    -1,   191,
       3,   189,   397,   192,    -1,   121,   190,   407,   189,   409,
     189,   409,   189,     7,    -1,   119,   190,   425,   189,   409,
      -1,    47,   190,   407,    -1,    48,   190,   407,    -1,    37,
     190,   197,   189,    38,    -1,    37,   190,   197,   189,    38,
     189,   155,   190,   411,    -1,    34,   190,   407,   189,    36,
      -1,    35,   190,   434,   189,    36,    -1,    -1,    33,   190,
     407,   189,   397,   189,   387,   388,   391,   393,   395,   373,
     374,    -1,    -1,   267,    -1,    42,   190,   404,   189,   398,
     189,   400,    -1,    42,   190,   404,   189,   398,   189,   400,
     189,   155,   190,   411,    -1,    43,   190,   404,    -1,    43,
     190,   404,   189,   155,   190,   411,    -1,    94,   190,   425,
     189,    95,   189,    92,    -1,   113,   190,   425,   189,     4,
      -1,    44,   190,    88,   189,   404,    -1,    44,   190,    88,
     189,   404,   189,   155,   190,   411,    -1,   165,   190,   404,
     189,   415,    -1,   165,   190,   404,   189,    77,    -1,   165,
     190,   404,    -1,   166,   190,   404,   189,   124,   189,   397,
      -1,   166,   190,   404,    -1,   167,   190,   404,   189,   125,
     189,   415,    -1,   167,   190,   404,   189,   125,   189,    77,
      -1,   167,   190,   404,    -1,    57,   190,   416,   189,   404,
      -1,    58,   190,   404,   189,   433,   189,   415,    -1,    55,
     190,   403,   189,   403,   189,   403,   189,   403,    -1,    55,
      -1,   197,    -1,    77,    -1,   197,    -1,    77,    -1,    -1,
     189,   389,    -1,   390,    -1,   390,   189,   389,    -1,    39,
      -1,    40,    -1,    41,    -1,    -1,    96,   190,   392,    -1,
      95,    -1,    -1,    93,   190,   394,    -1,    92,    -1,    -1,
     155,   190,   411,    -1,    69,    -1,    77,    -1,    70,    -1,
      77,    -1,    80,    -1,   401,    -1,    77,    -1,    80,    -1,
     401,    -1,    79,   190,    77,    -1,    84,    -1,    77,    -1,
      79,   193,     4,   194,    -1,   197,    -1,   208,    -1,   209,
     193,   416,   194,    -1,   416,    -1,   405,    -1,   180,   191,
     425,   192,    -1,   212,    -1,   213,   193,   416,   194,    -1,
     191,     4,   189,     4,   192,    -1,    77,    -1,    78,   406,
     194,    -1,   188,    -1,   188,   189,   406,    -1,   408,    -1,
     214,    -1,   215,   193,   416,   194,    -1,   191,     4,   189,
       4,   189,     4,   189,     4,   192,    -1,   410,    -1,   216,
      -1,   217,   193,   416,   194,    -1,     3,    -1,   191,     3,
     189,   397,   192,    -1,   412,    -1,   218,    -1,   219,   193,
     416,   194,    -1,   197,    -1,     3,    -1,   191,     3,   189,
     197,   192,    -1,    77,    -1,   414,    -1,   220,    -1,   221,
     193,   416,   194,    -1,   197,    -1,     3,    -1,   191,     3,
     189,   197,   192,    -1,    77,    -1,    90,    -1,   402,    -1,
     415,   249,   402,    -1,     4,    -1,   426,    -1,   191,     8,
     192,    -1,   206,    -1,   207,   193,   416,   194,    -1,   416,
     243,   416,    -1,   416,   244,   416,    -1,   416,   245,   416,
      -1,   416,   246,   416,    -1,   416,   247,   416,    -1,   191,
     416,   192,    -1,   202,    -1,   203,    -1,   271,   190,   417,
      -1,   418,    -1,   419,   189,   418,    -1,    -1,   419,    -1,
     416,    -1,   415,    -1,   421,    -1,   422,   189,   421,    -1,
      -1,   422,    -1,   404,    -1,   173,   407,    -1,   174,   407,
      -1,   175,   404,   189,   404,    -1,   176,   404,   189,   404,
     189,   416,    -1,   177,   191,   425,   192,    -1,   177,   191,
     326,   189,   425,   192,    -1,   183,   191,     7,   189,   425,
     192,    -1,   183,   191,   425,   189,   425,   192,    -1,   183,
     191,   409,   189,   425,   192,    -1,   179,   404,    -1,   181,
     191,   404,   189,   416,   192,    -1,   181,   191,   404,   189,
     416,   189,    39,   192,    -1,   182,   191,   404,   189,   416,
     189,   416,   192,    -1,   182,   191,   404,   189,   416,   189,
     416,   189,    39,   192,    -1,   185,   191,   186,   189,   191,
     416,   244,   416,   260,   192,   189,   404,   261,   192,    -1,
     184,   424,    -1,   222,    -1,   191,   425,   192,    -1,   424,
      -1,   424,   251,   425,    -1,   242,    -1,    91,    -1,     4,
      -1,     8,    -1,     9,    -1,     4,    -1,     8,    -1,     9,
      -1,     4,    -1,   426,    -1,    28,    -1,    22,    -1,    29,
      -1,    24,    -1,    30,    -1,   119,    -1,    72,    -1,    77,
      -1,   435,    -1,    54,   191,     4,   189,     4,   189,     4,
     189,     4,   192,    -1,   191,     4,   189,     4,   189,     4,
     189,     4,   192,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   283,   283,   284,   287,   288,   291,   314,   319,   341,
     354,   366,   372,   401,   407,   414,   417,   424,   428,   435,
     438,   445,   446,   450,   453,   460,   464,   471,   474,   480,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
     516,   517,   518,   519,   520,   521,   522,   523,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   538,   539,   540,   541,   542,   543,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   557,   558,   559,
     560,   561,   562,   563,   564,   565,   568,   569,   570,   573,
     574,   577,   593,   599,   605,   611,   617,   623,   629,   635,
     641,   651,   661,   671,   681,   691,   701,   713,   718,   725,
     730,   737,   742,   749,   753,   759,   764,   771,   775,   781,
     785,   792,   814,   791,   828,   883,   890,   893,   899,   906,
     910,   919,   923,   918,   986,   987,   991,   990,  1004,  1003,
    1018,  1028,  1029,  1032,  1070,  1069,  1104,  1103,  1134,  1133,
    1166,  1165,  1191,  1202,  1201,  1229,  1235,  1240,  1245,  1252,
    1259,  1268,  1276,  1288,  1287,  1311,  1310,  1332,  1335,  1341,
    1351,  1357,  1366,  1372,  1377,  1383,  1388,  1394,  1405,  1411,
    1412,  1415,  1416,  1419,  1423,  1429,  1430,  1434,  1440,  1448,
    1453,  1458,  1463,  1468,  1473,  1478,  1486,  1493,  1501,  1509,
    1510,  1513,  1514,  1517,  1522,  1521,  1535,  1542,  1549,  1557,
    1562,  1568,  1574,  1580,  1586,  1591,  1596,  1601,  1606,  1611,
    1616,  1621,  1626,  1631,  1636,  1642,  1649,  1658,  1662,  1675,
    1684,  1683,  1701,  1711,  1717,  1725,  1731,  1736,  1741,  1746,
    1751,  1756,  1761,  1766,  1771,  1785,  1791,  1796,  1801,  1806,
    1811,  1816,  1821,  1826,  1831,  1836,  1841,  1846,  1851,  1856,
    1863,  1869,  1898,  1903,  1911,  1917,  1921,  1929,  1936,  1943,
    1953,  1963,  1978,  1989,  1992,  1998,  2004,  2010,  2014,  2020,
    2027,  2033,  2041,  2047,  2052,  2057,  2062,  2067,  2073,  2079,
    2084,  2089,  2094,  2099,  2104,  2111,  2111,  2111,  2111,  2114,
    2120,  2126,  2131,  2138,  2145,  2151,  2157,  2163,  2168,  2175,
    2181,  2192,  2191,  2223,  2226,  2232,  2237,  2244,  2248,  2254,
    2260,  2266,  2270,  2276,  2280,  2285,  2292,  2296,  2303,  2307,
    2312,  2319,  2325,  2332,  2336,  2343,  2351,  2354,  2364,  2368,
    2371,  2377,  2381,  2388,  2392,  2396,  2403,  2406,  2412,  2419,
    2422,  2428,  2435,  2439,  2446,  2447,  2450,  2451,  2454,  2455,
    2456,  2462,  2463,  2464,  2470,  2471,  2474,  2483,  2488,  2495,
    2506,  2512,  2516,  2520,  2527,  2537,  2544,  2548,  2554,  2558,
    2566,  2570,  2577,  2587,  2600,  2604,  2611,  2621,  2630,  2641,
    2645,  2652,  2662,  2673,  2682,  2692,  2698,  2702,  2709,  2719,
    2730,  2739,  2749,  2753,  2760,  2761,  2767,  2771,  2775,  2779,
    2787,  2796,  2800,  2804,  2808,  2812,  2816,  2819,  2826,  2835,
    2868,  2869,  2872,  2873,  2876,  2880,  2887,  2894,  2905,  2908,
    2916,  2920,  2924,  2928,  2932,  2937,  2941,  2945,  2950,  2955,
    2960,  2964,  2969,  2974,  2978,  2982,  2987,  2991,  2998,  3004,
    3008,  3014,  3021,  3022,  3025,  3026,  3027,  3030,  3034,  3038,
    3042,  3048,  3049,  3052,  3053,  3056,  3057,  3060,  3061,  3064,
    3068,  3094
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
  "BOUNDARY_TYPE_ID", "OBJECT_ID", "COBJECT_ID", "MONSTER_ID", "TRAP_ID",
  "DOOR_ID", "DRAWBRIDGE_ID", "object_ID", "monster_ID", "terrain_ID",
  "MAZEWALK_ID", "WALLIFY_ID", "REGION_ID", "SPECIAL_REGION_ID",
  "SPECIAL_LEVREGION_ID", "SPECIAL_REGION_TYPE", "NAMING_ID",
  "NAMING_TYPE", "FILLING", "IRREGULAR", "JOINED", "ALTAR_ID", "ANVIL_ID",
  "NPC_ID", "LADDER_ID", "STAIR_ID", "NON_DIGGABLE_ID", "NON_PASSWALL_ID",
  "ROOM_ID", "ARTIFACT_NAME_ID", "PORTAL_ID", "TELEPRT_ID", "BRANCH_ID",
  "LEV", "MINERALIZE_ID", "CORRIDOR_ID", "GOLD_ID", "ENGRAVING_ID",
  "FOUNTAIN_ID", "THRONE_ID", "MODRON_PORTAL_ID", "LEVEL_TELEPORTER_ID",
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
  "USES_UP_KEY_ID", "CHARGES_ID", "SPECIAL_QUALITY_ID", "SPEFLAGS_ID",
  "SUBROOM_ID", "NAME_ID", "FLAGS_ID", "FLAG_TYPE", "MON_ATTITUDE",
  "MON_ALERTNESS", "SUBTYPE_ID", "NON_PASSDOOR_ID", "MON_APPEARANCE",
  "ROOMDOOR_ID", "IF_ID", "ELSE_ID", "TERRAIN_ID", "HORIZ_OR_VERT",
  "REPLACE_TERRAIN_ID", "LOCATION_SUBTYPE_ID", "DOOR_SUBTYPE",
  "BRAZIER_SUBTYPE", "SIGNPOST_SUBTYPE", "EXIT_ID", "SHUFFLE_ID",
  "QUANTITY_ID", "BURIED_ID", "LOOP_ID", "FOR_ID", "TO_ID", "SWITCH_ID",
  "CASE_ID", "BREAK_ID", "DEFAULT_ID", "ERODED_ID", "TRAPPED_STATE",
  "RECHARGED_ID", "INVIS_ID", "GREASED_ID", "INDESTRUCTIBLE_ID",
  "FEMALE_ID", "WAITFORU_ID", "CANCELLED_ID", "REVIVED_ID", "AVENGE_ID",
  "FLEEING_ID", "BLINDED_ID", "PARALYZED_ID", "STUNNED_ID", "CONFUSED_ID",
  "SEENTRAPS_ID", "ALL_ID", "MONTYPE_ID", "OBJTYPE_ID", "TERTYPE_ID",
  "TERTYPE2_ID", "LEVER_EFFECT_TYPE", "SWITCHABLE_ID",
  "CONTINUOUSLY_USABLE_ID", "TARGET_ID", "TRAPTYPE_ID", "EFFECT_FLAG_ID",
  "GRAVE_ID", "BRAZIER_ID", "SIGNPOST_ID", "ERODEPROOF_ID", "FUNCTION_ID",
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
  "level_def", "lev_init", "tileset_detail", "boundary_type_detail",
  "opt_limited", "opt_coord_or_var", "opt_fillchar", "walled", "flags",
  "flag_list", "levstatements", "stmt_block", "levstatement",
  "any_var_array", "any_var", "any_var_or_arr", "any_var_or_unk",
  "shuffle_detail", "variable_define", "encodeobj_list",
  "encodemonster_list", "mapchar_list", "encoderegion_list",
  "encodecoord_list", "integer_list", "string_list", "function_define",
  "$@1", "$@2", "function_call", "exitstatement", "opt_percent",
  "comparestmt", "switchstatement", "$@3", "$@4", "switchcases",
  "switchcase", "$@5", "$@6", "breakstatement", "for_to_span",
  "forstmt_start", "forstatement", "$@7", "loopstatement", "$@8",
  "chancestatement", "$@9", "ifstatement", "$@10", "if_ending", "$@11",
  "message", "random_corridors", "corridor", "corr_spec", "room_begin",
  "subroom_def", "$@12", "room_def", "$@13", "roomfill", "room_pos",
  "subroom_pos", "room_align", "room_size", "door_detail", "secret",
  "door_wall", "dir_list", "door_pos", "door_infos", "door_info",
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
  "special_region_detail", "special_levregion_detail", "region_detail",
  "@16", "region_detail_end", "altar_detail", "anvil_detail",
  "floor_detail", "subtype_detail", "npc_detail", "grave_detail",
  "brazier_detail", "signpost_detail", "gold_detail", "engraving_detail",
  "mineralize", "trap_name", "room_type", "optroomregionflags",
  "roomregionflags", "roomregionflag", "optfloormaintype", "floormaintype",
  "optfloortype", "floortype", "optmontype", "door_state", "light_state",
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
     435,   436,   437,   438,   439,   440,   441,   442,   443,    44,
      58,    40,    41,    91,    93,   123,   125,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,    43,    45,    42,    47,    37,    61,    46,
     124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   252,   253,   253,   254,   254,   255,   256,   256,   257,
     257,   257,   257,   258,   259,   260,   260,   261,   261,   262,
     262,   263,   263,   264,   264,   265,   265,   266,   266,   267,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   271,   271,   271,   272,
     272,   273,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   275,   275,   276,
     276,   277,   277,   278,   278,   279,   279,   280,   280,   281,
     281,   283,   284,   282,   285,   286,   287,   287,   288,   288,
     288,   290,   291,   289,   292,   292,   294,   293,   295,   293,
     296,   297,   297,   298,   300,   299,   302,   301,   304,   303,
     306,   305,   307,   308,   307,   309,   310,   310,   310,   311,
     311,   312,   313,   315,   314,   317,   316,   318,   318,   319,
     319,   320,   320,   321,   321,   322,   322,   323,   323,   324,
     324,   325,   325,   326,   326,   327,   327,   328,   328,   329,
     329,   329,   329,   329,   329,   329,   330,   330,   330,   331,
     331,   332,   332,   333,   334,   333,   335,   336,   336,   337,
     337,   337,   337,   337,   337,   337,   337,   337,   337,   337,
     337,   337,   337,   337,   337,   337,   338,   338,   338,   339,
     340,   339,   341,   342,   342,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     344,   345,   346,   346,   347,   347,   347,   348,   349,   350,
     351,   352,   353,   354,   354,   355,   356,   357,   357,   358,
     359,   359,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   361,   361,   361,   361,   362,
     363,   364,   364,   365,   366,   367,   368,   369,   369,   370,
     371,   373,   372,   374,   374,   375,   375,   376,   376,   377,
     378,   379,   379,   380,   380,   380,   381,   381,   382,   382,
     382,   383,   384,   385,   385,   386,   386,   387,   387,   388,
     388,   389,   389,   390,   390,   390,   391,   391,   392,   393,
     393,   394,   395,   395,   396,   396,   397,   397,   398,   398,
     398,   399,   399,   399,   400,   400,   401,   402,   402,   402,
     403,   404,   404,   404,   404,   405,   405,   405,   406,   406,
     407,   407,   407,   408,   409,   409,   409,   410,   410,   411,
     411,   411,   412,   412,   412,   412,   413,   413,   413,   414,
     414,   414,   414,   414,   415,   415,   416,   416,   416,   416,
     416,   416,   416,   416,   416,   416,   416,   417,   417,   418,
     419,   419,   420,   420,   421,   421,   422,   422,   423,   423,
     424,   424,   424,   424,   424,   424,   424,   424,   424,   424,
     424,   424,   424,   424,   424,   424,   424,   424,   424,   425,
     425,   426,   427,   427,   428,   428,   428,   429,   429,   429,
     429,   430,   430,   431,   431,   432,   432,   433,   433,   434,
     434,   435
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     3,     3,     5,     5,
       5,     3,    16,     3,     3,     0,     2,     0,     2,     0,
       2,     1,     1,     0,     3,     3,     1,     0,     2,     3,
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
       0,     4,     1,     0,     4,     3,     1,     3,     3,     5,
       5,     7,     4,     0,    13,     0,    15,     0,     2,     5,
       1,     5,     1,     5,     1,     5,     1,    10,     6,     1,
       1,     1,     1,     1,     3,     1,     1,     0,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     7,     5,     1,
       1,     1,     1,     3,     0,     5,     4,     0,     3,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     3,
       3,     3,     1,     1,     3,     1,     1,     1,     3,     3,
       0,     5,     2,     0,     3,     1,     3,     1,     3,     3,
       1,     1,     3,     1,     1,     1,     3,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       5,     7,     5,     8,     1,     3,     3,     5,     5,     7,
       7,     6,     5,     0,     2,     5,     3,    11,    13,     6,
       0,     3,     1,     3,     3,     3,     3,     1,     1,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     3,
       3,     1,     5,     9,     5,     3,     3,     5,     9,     5,
       5,     0,    13,     0,     1,     7,    11,     3,     7,     7,
       5,     5,     9,     5,     5,     3,     7,     3,     7,     7,
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
       2,     0,     0,     0,     3,     4,    23,     0,     0,     1,
       5,     0,    27,     0,     7,     0,   148,     0,     0,     0,
       0,   216,     0,     0,     0,     0,     0,     0,     0,     0,
     284,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   354,     0,     0,     0,
       0,     0,     0,     0,     0,   176,     0,     0,     0,     0,
       0,     0,     0,     0,   145,     0,     0,     0,   151,   160,
       0,     0,     0,     0,     0,     0,   108,    97,    88,    98,
      89,    99,    90,   100,    91,   101,    92,   102,    93,   103,
      94,   104,    95,   105,    96,    31,    32,    33,     6,    27,
     106,   107,     0,    45,    44,    62,    63,    60,     0,    55,
      61,   164,    56,    57,    59,    58,    30,    73,    43,    79,
      78,    47,    66,    68,    69,    86,    48,    67,    87,    64,
      83,    84,    72,    85,    42,    51,    52,    53,    65,    80,
      71,    82,    81,    46,    70,    74,    75,    76,    77,    36,
      37,    35,    34,    38,    39,    40,    41,    54,    49,    50,
       0,    26,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   170,     0,     0,     0,
       0,   109,   110,     0,     0,     0,     0,     0,     0,   426,
       0,   429,     0,   471,     0,   427,   448,    28,     0,   168,
       0,     8,     0,   387,   388,     0,   424,   175,     0,     0,
       0,    11,   473,   472,    13,   396,     0,   219,   220,     0,
       0,   393,     0,     0,   187,   391,    14,   420,   422,   423,
       0,   419,   417,     0,   249,   253,   416,   250,   413,   415,
       0,   412,   410,     0,   223,     0,   409,   356,   355,     0,
     374,   375,     0,     0,     0,   286,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   467,   450,
     469,   285,     0,   401,     0,     0,   400,     0,     0,     0,
       0,   489,     0,     0,   337,     0,     0,     0,     0,     0,
     325,   326,   358,   357,     0,   146,     0,     0,     0,     0,
     390,     0,     0,     0,     0,     0,   296,     0,   320,   319,
     476,   474,   475,   178,   177,     0,     0,     0,     0,   199,
     200,     0,     0,     0,     0,   111,     0,     0,     0,   345,
     347,   350,   141,     0,     0,     0,     0,   150,     0,     0,
       0,     0,     0,   445,   444,   446,   449,     0,   482,   484,
     481,   483,   485,   486,     0,     0,     0,   118,   119,   114,
     112,     0,     0,     0,     0,    27,   165,    25,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   252,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   451,   452,     0,     0,     0,   460,     0,
       0,     0,   466,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   147,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   172,
     171,     0,     0,   166,     0,     0,     0,     0,     0,   442,
     428,   436,     0,     0,   431,   432,   433,   434,   435,     0,
     144,     0,   426,     0,     0,     0,     0,   135,   133,   139,
     137,     0,     0,     0,   169,     0,     0,   425,    10,   321,
       0,     9,     0,     0,   397,     0,     0,     0,   222,   221,
     187,   188,   218,     0,     0,     0,   251,     0,     0,   225,
     227,   280,   207,     0,   282,     0,     0,   203,     0,     0,
       0,     0,   407,     0,     0,   405,     0,     0,   404,     0,
       0,   468,   470,     0,     0,   376,   377,     0,   329,     0,
       0,   330,   327,   380,     0,   378,     0,   379,     0,   341,
     287,     0,   288,     0,   190,     0,     0,     0,     0,   293,
     292,     0,     0,   179,   180,   351,   487,   488,     0,   295,
       0,     0,   300,     0,   192,     0,     0,   340,     0,     0,
       0,   324,     0,     0,   162,     0,     0,   152,   344,   343,
       0,     0,     0,   440,   443,     0,   430,   149,   447,   113,
       0,     0,   122,     0,   121,     0,   120,     0,   126,     0,
     117,     0,   116,     0,   115,    29,   389,     0,     0,   399,
     392,     0,   394,     0,     0,   418,   479,   477,   478,   264,
     261,   255,     0,   279,     0,     0,     0,   278,     0,     0,
       0,     0,     0,   260,     0,   265,     0,   267,   268,   277,
       0,   263,   254,   269,   480,   257,     0,   411,   226,   198,
       0,     0,   453,     0,     0,     0,   455,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   402,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   182,
       0,     0,   291,     0,     0,     0,     0,     0,   299,     0,
       0,     0,     0,     0,     0,   167,   161,   163,     0,     0,
       0,     0,     0,   142,     0,   134,   136,   138,   140,     0,
     127,     0,   129,     0,   131,     0,     0,   395,   217,   421,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   414,     0,     0,   281,    19,     0,   204,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   359,
       0,     0,     0,     0,   385,   384,   335,   338,     0,   289,
       0,   194,     0,     0,   290,   294,     0,     0,   352,     0,
       0,     0,   339,     0,   196,     0,   359,   202,     0,   201,
     174,     0,   154,   346,   349,   348,   437,   438,   439,   441,
       0,     0,   125,     0,   124,     0,   123,     0,     0,   276,
     271,   270,   272,   273,   274,   275,   258,   259,   262,   266,
     256,     0,   381,   230,   231,     0,   235,   234,   245,   236,
     237,   238,     0,     0,     0,   242,   243,     0,   228,   232,
     382,   229,     0,   213,   214,     0,   215,     0,   212,   208,
       0,   283,   454,   456,     0,   461,     0,   457,     0,   406,
     459,   458,     0,     0,     0,   366,     0,     0,     0,   386,
       0,     0,     0,     0,     0,     0,   205,   206,     0,     0,
       0,   302,     0,     0,     0,     0,     0,     0,     0,   307,
     308,     0,     0,     0,   301,     0,     0,   366,     0,     0,
       0,     0,     0,   154,   143,   128,   130,   132,   322,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,    20,
       0,     0,   408,     0,     0,   363,   364,   365,   360,   361,
       0,   369,     0,     0,   328,     0,   342,   189,     0,   359,
     353,   181,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   191,     0,   369,   207,   323,     0,
     158,   153,   155,     0,   383,   239,   240,   241,   247,   246,
     244,   210,   211,   209,   462,     0,   463,   432,     0,     0,
       0,     0,   372,     0,     0,     0,     0,   366,     0,     0,
     371,   312,   314,   315,   316,   318,   317,   311,   303,   304,
     305,   306,   309,   310,   313,     0,   372,   197,   156,    27,
       0,     0,     0,     0,     0,   403,   362,   368,   367,     0,
       0,   331,     0,   491,   336,   193,   369,     0,   297,   195,
     183,    27,   159,     0,   248,   464,    16,     0,   370,     0,
     333,   490,   372,     0,     0,   157,     0,     0,   373,   334,
     332,   185,   298,   184,     0,    17,     0,    21,    22,    19,
       0,     0,   186,    12,    18,   465
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    95,    96,    97,  1034,  1081,
     871,  1079,    12,   162,    98,   396,    99,   100,   101,   102,
     213,   103,   104,   739,   741,   743,   493,   494,   495,   496,
     105,   479,   820,   106,   107,   453,   108,   109,   214,   728,
     922,   923,  1051,  1029,   110,   606,   111,   112,   230,   113,
     603,   114,   394,   115,   352,   470,   599,   116,   117,   118,
     332,   324,   119,  1064,   120,  1076,   410,   576,   596,   793,
     806,   121,   351,   808,   538,   898,   679,   869,   122,   253,
     520,   123,   417,   274,   678,   858,   990,   124,   414,   264,
     413,   672,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   712,   135,   136,   137,   138,   718,   914,  1017,
     139,   140,   511,   141,   142,   143,   144,   145,   146,   147,
     148,  1060,  1070,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   279,   325,   885,   948,   949,   951,
    1038,  1002,  1011,  1041,   282,   557,   566,   859,   786,   567,
     236,   329,   299,   255,   404,   305,   306,   547,   548,   275,
     276,   265,   266,   373,   330,   818,   613,   614,   615,   375,
     376,   377,   300,   434,   225,   244,   344,   675,   391,   392,
     393,   588,   310,   311
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -929
static const yytype_int16 yypact[] =
{
     135,  -100,   -90,   113,  -929,   135,    56,   -14,    -7,  -929,
    -929,     8,   770,   -12,  -929,    12,  -929,    31,    34,    42,
      82,  -929,    92,   103,   118,   126,   155,   157,   166,   178,
     185,   190,   200,   209,   210,   211,   213,   214,   215,   221,
     222,   232,   233,   248,   249,   250,   254,   255,   262,   269,
     271,   272,   276,   280,   282,   303,   304,   309,   317,   324,
     325,    70,   326,   331,  -929,   338,    17,   839,  -929,  -929,
     340,   342,   343,    19,    43,    83,  -929,  -929,  -929,  -929,
    -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,
    -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,   770,
    -929,  -929,   -40,  -929,  -929,  -929,  -929,  -929,   346,  -929,
    -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,
    -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,
    -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,
    -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,
    -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,
     285,   160,  -929,   -61,   407,    54,   376,   334,    25,    25,
     116,   -38,    24,     0,     0,   666,  -111,  -111,   -19,   244,
       0,     0,   381,     0,   305,  -111,  -111,   -34,   -19,   -19,
     -19,    43,   306,    43,     0,   492,   492,     0,   492,   492,
      87,     0,   492,   -34,   492,    55,  -929,   492,  -111,   833,
      43,  -929,  -929,   291,   302,     0,     0,     0,   349,  -929,
      77,  -929,   365,  -929,   266,  -929,   134,  -929,   201,  -929,
     347,  -929,    12,  -929,  -929,   368,  -929,   292,   358,   374,
     377,  -929,  -929,  -929,  -929,  -929,   380,  -929,  -929,   394,
     567,  -929,   403,   398,   413,  -929,  -929,  -929,  -929,  -929,
     600,  -929,  -929,   420,  -929,  -929,  -929,  -929,  -929,  -929,
     611,  -929,  -929,   424,   423,   433,  -929,  -929,  -929,   435,
    -929,  -929,   439,   441,   443,  -929,  -111,  -111,     0,     0,
     454,     0,   462,   463,   472,   492,   479,   251,  -929,  -929,
     427,  -929,   682,  -929,   489,   498,  -929,   502,   510,   699,
     513,  -929,   517,   519,   523,   524,   529,   712,   531,   533,
    -929,  -929,  -929,  -929,   534,   719,   537,   543,   544,   545,
     362,   731,   553,   204,   557,   559,  -929,   560,  -929,  -929,
    -929,  -929,  -929,  -929,  -929,   565,   571,   572,   573,  -929,
    -929,   577,   347,   578,   586,  -929,   551,    43,    43,   588,
     589,   596,  -929,   594,   354,    43,    43,  -929,    43,    43,
      43,    43,    43,   292,   362,  -929,   610,   608,  -929,  -929,
    -929,  -929,  -929,  -929,   620,   108,    29,  -929,  -929,   292,
     362,   621,   634,   642,   770,   770,  -929,  -929,    43,   -61,
     830,    28,   831,   648,   658,   492,   664,    43,   239,   849,
     657,   667,    43,   674,   347,   676,    43,   347,     0,     0,
     492,   790,   792,  -929,  -929,   680,   681,   467,  -929,     0,
       0,   399,  -929,   685,   684,   492,   691,    43,    84,   837,
     870,   692,   846,   847,   247,   729,     0,   805,   705,   808,
     -19,   -21,  -929,   706,   -19,   -19,   -19,    43,   710,    53,
       0,    22,   815,     4,   747,   812,    -9,   904,    24,   791,
    -929,    89,    89,  -929,   257,   716,   -35,   787,   788,   795,
    -929,  -929,   392,   527,   106,   106,  -929,  -929,  -929,   134,
    -929,   492,   723,   -75,   -20,   -10,     5,  -929,  -929,   292,
     362,    20,     9,   141,  -929,   718,   536,  -929,  -929,  -929,
     923,  -929,   738,   380,  -929,   736,   925,   615,  -929,  -929,
     413,  -929,  -929,   734,   673,   556,  -929,   735,   678,  -929,
    -929,  -929,  -929,   744,   745,     0,     0,   688,   751,   754,
     764,   765,  -929,   766,   307,  -929,   763,   768,  -929,   769,
     771,  -929,  -929,   955,   698,  -929,  -929,   772,  -929,   773,
     962,  -929,   778,  -929,   775,  -929,   782,  -929,   783,   785,
    -929,   968,  -929,   786,  -929,   990,   809,    84,   810,   832,
    -929,   834,   943,  -929,  -929,  -929,  -929,  -929,   835,  -929,
     838,   836,  -929,   852,  -929,  1016,   854,  -929,   869,   942,
    1019,  -929,   873,   347,  -929,   814,    43,  -929,  -929,   292,
     875,   876,   877,  -929,   879,   874,  -929,  -929,  -929,  -929,
    1065,   880,  -929,     6,  -929,    43,  -929,   -61,  -929,    51,
    -929,    79,  -929,    52,  -929,  -929,  -929,   881,  1069,  -929,
    -929,   882,  -929,   878,   883,  -929,  -929,  -929,  -929,  -929,
    -929,  -929,   887,  -929,   888,   889,   890,  -929,   891,   893,
     894,   895,   896,  -929,   897,  -929,   898,  -929,  -929,  -929,
     899,  -929,  -929,  -929,  -929,  -929,   900,  -929,   884,   901,
      24,  1086,  -929,   906,  1020,   492,  -929,    43,    43,   492,
     908,    43,   492,   492,   907,   910,  -929,   -34,  1096,   912,
     947,  1099,    91,   116,   949,   -84,  1021,   916,    -6,  -929,
     911,  1022,  -929,    43,   920,   -61,  1047,     0,   922,  1023,
     924,    -3,   261,   347,    89,  -929,  -929,   362,   917,    84,
     -13,   159,   795,  -929,   -11,  -929,  -929,   362,   292,    13,
    -929,    18,  -929,    98,  -929,    84,   927,  -929,  -929,  -929,
      25,  1024,  1017,    43,    43,    43,    43,   -61,    43,    43,
      43,   116,  -929,   885,    15,  -929,   928,    43,  -929,   926,
     120,   348,   929,    84,   704,   931,   932,    43,  1115,   936,
     937,  1116,   938,   933,  -929,  -929,   940,  -929,   941,  -929,
    1126,  -929,   313,   944,  -929,  -929,   945,    69,   292,   946,
     948,   595,  -929,  1128,  -929,  1132,   936,  -929,   950,  -929,
    -929,   951,   -25,  -929,  -929,   292,  -929,  -929,  -929,  -929,
     347,    51,  -929,    79,  -929,    52,  -929,   952,  1133,  -929,
    -929,  -929,  -929,  -929,  -929,  -929,   292,  -929,  -929,  -929,
    -929,    -1,  -929,  -929,  -929,   -61,  -929,  -929,  -929,  -929,
    -929,  -929,   953,   956,   957,  -929,  -929,   958,  -929,  -929,
    -929,   292,   959,  -929,  -929,   960,  -929,   961,  -929,  -929,
    1138,  -929,   362,  -929,  1103,  -929,    43,  -929,   963,  -929,
    -929,  -929,   494,   964,   333,  1049,  1148,   965,   116,  -929,
    1001,   116,   966,   970,    -3,    43,  -929,  -929,   969,  1093,
    1073,  -929,   972,   973,   974,   975,   976,   977,   978,  -929,
    -929,   979,   980,   981,  -929,   982,   983,  1049,    69,  1166,
     131,   985,   984,   -25,  -929,  -929,  -929,  -929,  -929,   987,
    1100,   292,    43,    43,    43,   -67,    25,  1174,  1056,  -929,
     989,   279,  -929,    43,  1178,  -929,  -929,  -929,  -929,   994,
     995,  1091,   997,  1183,  -929,   998,  -929,  -929,   239,   936,
    -929,  -929,  1000,  1002,  1098,  1188,    44,   116,    25,    28,
      28,     0,   -38,  1189,  -929,  1190,  1091,  -929,  -929,  1005,
    -929,  -929,  -929,  1191,  -929,  -929,  -929,  -929,  -929,   954,
    -929,  -929,  -929,  -929,  -929,  1158,  -929,   518,  1006,   333,
    1104,  1010,  1046,  1198,  1011,   116,  1013,  1049,  1119,  1121,
    -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,
    -929,  -929,  -929,  -929,  -929,  1018,  1046,   901,  -929,   770,
    1025,   -67,  1026,  1028,  1027,  -929,  -929,  -929,  -929,  1098,
    1030,  -929,  1029,  -929,  -929,  -929,  1091,  1033,  -929,  -929,
    -929,   770,  -929,    84,  -929,  -929,  -929,  1034,  -929,   116,
     347,  -929,  1046,  1122,   347,  -929,  1035,     0,  -929,  -929,
    -929,  -929,  -929,  -929,    65,  1036,   347,  -929,  -929,   928,
       0,  1037,  -929,  -929,  -929,  -929
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -929,  -929,  1204,  -929,  -929,  -929,  -929,  -929,  -929,  -929,
     132,  -929,  -929,   996,   -99,  -345,   818,  1004,  1149,  -457,
    -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,
    -929,  -929,  -929,  -929,  -929,  -929,  1156,  -929,  -929,  -929,
     308,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,
    -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,
     767,  1031,  -929,  -929,  -929,  -929,   707,  -929,  -929,  -929,
     336,  -929,  -929,  -929,  -595,   314,   256,  -929,  -929,   444,
     277,  -929,  -929,  -929,  -929,  -929,   206,  -929,  -929,  1070,
    -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,
    -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,
    -929,  -929,  -583,  -929,  -929,  -929,  -929,  -929,  -929,  -929,
    -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,  -929,
    -929,  -929,  -929,  -929,   268,   541,  -772,   242,  -929,  -864,
    -929,  -897,   203,  -928,  -441,  -574,  -929,  -929,  -929,   480,
     845,  -205,  -165,  -356,   732,   169,  -365,  -458,  -568,  -500,
    -572,  -490,  -562,  -153,   -68,  -929,   514,  -929,  -929,   758,
    -929,  -929,   967,  -158,   724,  -929,  -455,  -929,  -929,  -929,
    -929,  -929,  -164,  -929
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -225
static const yytype_int16 yytable[] =
{
     227,   254,   632,   709,   584,   356,   224,   469,   283,   284,
     237,   630,   268,   601,   602,   313,   314,   301,   316,   318,
     319,   498,   612,   257,   326,   327,   328,   598,   257,   334,
     497,   509,   337,   219,   917,   308,   345,   335,   336,   277,
     338,   339,   608,   322,   346,   634,   348,   219,  1013,   353,
     359,   360,   361,   976,   257,   542,   574,   340,   242,   742,
     349,   341,   342,   387,   814,   744,   590,   740,   594,   526,
    1077,   791,   529,   896,   804,   389,    16,   245,   246,  1026,
     302,   219,   268,   245,   246,   363,   269,   988,   862,   768,
       7,   340,   542,   280,   586,   341,   342,   258,  1050,   587,
       8,   281,   258,   303,   304,   781,   245,   246,   747,   920,
     259,   921,   492,     9,   621,   259,   363,   863,   864,   268,
     865,   622,   161,   425,   426,   333,   428,   809,   258,   866,
     989,  1014,   350,  1015,  1071,   340,   233,   867,   219,   341,
     342,   259,  1078,  1046,   542,   243,   897,   234,   235,  1062,
       1,     2,   364,   475,   555,   813,   269,   868,   374,   278,
     390,   556,   233,   323,   343,    11,   591,  1016,   784,   623,
     575,   827,   309,   234,   235,   785,   624,   160,   778,   625,
     249,   747,   595,    13,   233,   792,   626,  1007,   805,   930,
      14,   250,   701,   269,   627,   234,   235,   250,    15,   878,
     270,   628,   821,   787,   631,   219,   271,   823,   228,   822,
     210,   260,   251,   252,   824,   629,   260,   261,   218,   510,
     385,   163,   261,   378,   164,   379,   233,   272,   273,   380,
     381,   382,   165,   499,   220,   221,   222,   234,   235,   765,
     262,   263,   260,   600,   331,   262,   263,   515,   261,   221,
     222,   926,   581,   530,   531,   406,   735,   927,   725,   925,
     829,   840,   532,    74,   540,   541,   811,   736,   220,   539,
     270,   223,   166,   549,   226,   612,   271,   552,   245,   246,
     600,   569,   167,   221,   222,   223,   573,   825,   231,   474,
     578,   579,   580,   168,   826,   585,   505,   482,   483,   220,
     484,   485,   486,   487,   488,   545,   546,   270,   169,   874,
     690,   406,   875,   271,   221,   222,   170,   364,   500,   223,
     383,   518,   519,   609,   563,   220,   564,   565,   245,   246,
     506,   233,   600,   619,   272,   273,   633,   537,   807,   517,
     221,   222,   234,   235,   524,   171,   307,   172,   528,   232,
     223,   370,   371,   372,   320,   321,   173,   545,   546,   308,
     673,   816,   817,   368,   369,   370,   371,   372,   174,   554,
     682,   683,   945,   946,   947,   175,   223,   354,   810,   384,
     176,   249,   245,   246,   245,   246,  1020,  1021,   954,   604,
     177,   956,   385,   460,   247,   248,   386,   388,   233,   178,
     179,   180,   542,   181,   182,   183,   543,   221,   222,   234,
     235,   184,   185,   251,   252,   303,   304,   238,   239,   240,
     241,   374,   186,   187,   286,   287,   288,   289,   290,   256,
     291,   249,   292,   293,   294,   295,   296,   366,   188,   189,
     190,   312,   297,   223,   191,   192,   991,   368,   369,   370,
     371,   372,   193,   245,   246,   423,   424,   247,   248,   194,
     367,   195,   196,   251,   252,   979,   197,  1018,   995,   315,
     198,   996,   199,   298,   738,   924,   245,   246,  1019,  1066,
     286,   287,   288,   289,   290,   249,   291,   249,   292,   293,
     294,   295,   296,   200,   201,   358,   317,   331,   297,   202,
     368,   369,   370,   371,   372,  1044,   605,   203,   796,   368,
     369,   370,   371,   372,   204,   205,   207,   251,   252,   251,
     252,   208,   368,   369,   370,   371,   372,   769,   209,   298,
     215,   772,   216,   217,   775,   776,   229,   876,   727,   357,
     362,   399,   395,   537,   245,   246,   481,   400,   832,   833,
     834,   835,   800,   837,   838,   839,   249,   737,   365,  1068,
     646,   398,   798,   401,   647,   648,   402,   250,   403,   245,
     246,   406,   286,   287,   288,   289,   290,   815,   291,   249,
     292,   293,   294,   295,   296,   405,   616,   408,   251,   252,
     544,   368,   369,   370,   371,   372,   407,   368,   369,   370,
     371,   372,   409,   411,   836,   368,   369,   370,   371,   372,
     861,   251,   252,   412,   415,   545,   546,   416,  -224,   770,
     771,   298,   418,   774,   419,   649,   650,   651,   420,   652,
     421,   653,   422,   245,   246,   368,   369,   370,   371,   372,
     286,   287,   288,   289,   290,   427,   291,   249,   292,   293,
     294,   295,   296,   429,   430,   654,   655,   656,   297,   657,
     658,   659,   660,   431,   661,   286,   287,   288,   289,   290,
     433,   291,   249,   292,   293,   294,   295,   296,   435,   251,
     252,   901,   437,   297,   662,   663,   436,   438,   902,   298,
     960,   439,   931,   664,   665,   666,   667,   668,   669,   872,
     903,   440,   442,   441,   251,   252,   443,  1033,   444,   882,
     -15,   670,   445,   446,   298,  1069,   448,   904,   447,  1073,
     449,   617,   450,   451,   671,   452,   454,   985,   986,   987,
     636,  1082,   455,   456,   457,   458,   249,   368,   943,   370,
     371,   372,   459,   245,   246,   473,   461,   250,   462,   463,
     905,   906,   907,   908,   464,   909,   910,   911,   912,   913,
     465,   466,   467,   370,   371,   372,   468,   471,   251,   252,
     368,   369,   370,   371,   372,   472,    16,   476,   477,   368,
     369,   370,   371,   372,    17,   478,   480,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,   223,   489,
     490,    29,    30,    31,    32,    33,  1022,    34,   941,   642,
     491,   501,    35,    36,    37,    38,    39,    40,    41,    42,
     285,    43,    44,    45,   502,    46,    47,    48,    49,    50,
      51,    52,   503,   508,   512,    53,    54,   513,    55,   286,
     287,   288,   289,   290,    56,   291,   249,   292,   293,   294,
     295,   296,   514,   516,   521,   522,   523,   297,   368,   369,
     370,   371,   372,   525,    57,   527,   533,   645,   534,   535,
     536,   550,   677,   558,   559,   997,   551,    58,   251,   252,
     553,   560,   561,    59,   568,   562,    60,    61,   298,    62,
     570,    63,   696,   572,   571,   577,    64,    65,   879,   582,
      66,    67,  1075,    68,   589,    69,   592,   593,   597,  -173,
     607,   610,   620,   611,   635,  1084,   368,   369,   370,   371,
     372,   368,   369,   370,   371,   372,   637,   638,   640,   641,
    1052,   644,   676,   680,   681,    70,    71,    72,   684,    73,
     685,   368,   369,   370,   371,   372,   686,   368,   369,   370,
     371,   372,  1065,   687,   688,   689,   691,   692,   693,   695,
     694,   697,   698,    74,   841,   842,   699,   700,   701,    75,
      76,   702,   705,   703,   704,   706,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,   707,    76,   843,   844,   708,   710,
     845,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,   714,
     720,   711,   690,   713,   715,   846,   717,   716,   847,   848,
     849,   850,   851,   852,   853,   854,   855,   856,   857,   211,
      78,   719,    80,   721,    82,    77,    84,    79,    86,    81,
      88,    83,    90,    85,    92,    87,    94,    89,   722,    91,
     723,    93,   724,   726,   729,   730,   733,   731,   732,   734,
     745,   302,   746,   763,   747,   749,   748,   750,   751,   752,
     753,   754,   233,   755,   756,   757,   758,   759,   760,   761,
     764,   766,   762,   234,   235,   767,   537,   773,   777,   778,
     780,   781,   782,   783,   788,   790,   789,   795,   794,   797,
     799,   801,   812,   803,   831,   802,   828,   870,   873,   883,
     887,   877,   830,   880,   881,   884,   886,   889,   888,   890,
     892,   891,   915,   894,   895,   899,   916,   900,   929,   918,
     919,   939,   940,   932,   928,   950,   933,   934,   935,   936,
     937,   938,   952,   944,   953,   942,   955,   962,   957,   958,
     963,   961,   964,   965,   966,   967,   968,   969,   970,   971,
     972,   973,   975,   978,   974,   980,   983,   984,   992,   993,
     981,   994,   998,   999,  1001,  1000,  1003,  1004,  1005,  1008,
    1010,  1009,  1012,  1024,  1025,  1028,  1030,  1032,  1035,  1037,
    1039,  1040,  1042,  1043,  1031,  1045,  1047,  1048,  1072,    10,
    1049,  1083,   504,   355,  1053,  1056,   212,   206,  1055,  1057,
    1059,  1061,  1063,  1067,  1074,  1080,   583,   643,   397,  1085,
     959,   982,   977,  1027,   347,  1006,   893,  1054,   779,   267,
    1023,  1036,  1058,   860,   507,   639,   819,   618,     0,   674,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432
};

static const yytype_int16 yycheck[] =
{
      99,   166,   502,   577,   459,   210,    74,   352,   173,   174,
     163,   501,     3,   471,   472,   180,   181,   175,   183,   184,
     184,   386,   479,     3,   188,   189,   190,   468,     3,   194,
     386,     3,   197,     4,   806,    54,   201,   195,   196,    77,
     198,   199,    77,    77,   202,   503,   204,     4,     4,   207,
     215,   216,   217,   917,     3,     3,    77,     4,     4,   631,
       5,     8,     9,   228,    77,   633,    62,   629,    77,   414,
       5,    77,   417,     4,    77,   228,     6,    77,    78,   976,
     191,     4,     3,    77,    78,     8,    77,   154,    73,   684,
     190,     4,     3,    69,    72,     8,     9,    77,  1026,    77,
     190,    77,    77,   214,   215,   189,    77,    78,   192,   134,
      90,   136,     4,     0,   189,    90,     8,   102,   103,     3,
     105,   196,   110,   288,   289,   193,   291,   722,    77,   114,
     197,    87,    77,    89,  1062,     4,   197,   122,     4,     8,
       9,    90,    77,  1007,     3,    91,    77,   208,   209,  1046,
      15,    16,   220,   358,    70,   729,    77,   142,   226,   197,
     228,    77,   197,   197,    77,   109,   162,   123,    77,   189,
     191,   745,   191,   208,   209,    84,   196,   189,   189,   189,
     180,   192,   191,   197,   197,   191,   196,   959,   191,   190,
     197,   191,   193,    77,   189,   208,   209,   191,   190,   773,
     191,   196,   189,   703,   195,     4,   197,   189,   248,   196,
     193,   191,   212,   213,   196,   195,   191,   197,   199,   191,
     191,   190,   197,    22,   190,    24,   197,   218,   219,    28,
      29,    30,   190,   386,   191,   206,   207,   208,   209,   680,
     220,   221,   191,   191,   191,   220,   221,   405,   197,   206,
     207,   823,   457,   418,   419,     4,   621,   825,   603,   821,
     750,   761,   420,   193,   429,   430,   724,   623,   191,   427,
     191,   242,   190,   431,   191,   732,   197,   435,    77,    78,
     191,   446,   190,   206,   207,   242,   450,   189,     3,   357,
     454,   455,   456,   190,   196,   460,   395,   365,   366,   191,
     368,   369,   370,   371,   372,   216,   217,   191,   190,   189,
       3,     4,   192,   197,   206,   207,   190,   385,   386,   242,
     119,    82,    83,   476,    77,   191,    79,    80,    77,    78,
     398,   197,   191,   491,   218,   219,   195,    76,    77,   407,
     206,   207,   208,   209,   412,   190,   177,   190,   416,   189,
     242,   245,   246,   247,   185,   186,   190,   216,   217,    54,
     525,   202,   203,   243,   244,   245,   246,   247,   190,   437,
     535,   536,    39,    40,    41,   190,   242,   208,   723,   178,
     190,   180,    77,    78,    77,    78,   969,   970,   888,   132,
     190,   891,   191,   189,    81,    82,   195,   228,   197,   190,
     190,   190,     3,   190,   190,   190,     7,   206,   207,   208,
     209,   190,   190,   212,   213,   214,   215,    10,    11,    12,
      13,   489,   190,   190,   173,   174,   175,   176,   177,    95,
     179,   180,   181,   182,   183,   184,   185,   171,   190,   190,
     190,   197,   191,   242,   190,   190,   936,   243,   244,   245,
     246,   247,   190,    77,    78,   286,   287,    81,    82,   190,
     194,   190,   190,   212,   213,   920,   190,   967,   189,    88,
     190,   192,   190,   222,   627,   820,    77,    78,   968,  1053,
     173,   174,   175,   176,   177,   180,   179,   180,   181,   182,
     183,   184,   185,   190,   190,   193,   191,   191,   191,   190,
     243,   244,   245,   246,   247,  1005,   249,   190,   713,   243,
     244,   245,   246,   247,   190,   190,   190,   212,   213,   212,
     213,   190,   243,   244,   245,   246,   247,   685,   190,   222,
     190,   689,   190,   190,   692,   693,   190,   189,   606,   248,
     191,   249,   195,    76,    77,    78,   192,   189,   753,   754,
     755,   756,   717,   758,   759,   760,   180,   625,   193,  1059,
       4,   193,   715,   189,     8,     9,   189,   191,   188,    77,
      78,     4,   173,   174,   175,   176,   177,   730,   179,   180,
     181,   182,   183,   184,   185,   191,   194,   189,   212,   213,
     191,   243,   244,   245,   246,   247,   193,   243,   244,   245,
     246,   247,   189,     3,   757,   243,   244,   245,   246,   247,
     763,   212,   213,   193,     3,   216,   217,   193,   195,   687,
     688,   222,   189,   691,   189,    69,    70,    71,   189,    73,
     189,    75,   189,    77,    78,   243,   244,   245,   246,   247,
     173,   174,   175,   176,   177,   191,   179,   180,   181,   182,
     183,   184,   185,   191,   191,    99,   100,   101,   191,   103,
     104,   105,   106,   191,   108,   173,   174,   175,   176,   177,
     191,   179,   180,   181,   182,   183,   184,   185,   251,   212,
     213,    86,   193,   191,   128,   129,     4,   189,    93,   222,
     895,   189,   845,   137,   138,   139,   140,   141,   142,   767,
     105,   191,   189,     4,   212,   213,   189,   189,   189,   777,
     192,   155,   189,   189,   222,  1060,     4,   122,   189,  1064,
     189,   194,   189,   189,   168,     6,   189,   932,   933,   934,
     194,  1076,   189,   189,   189,     4,   180,   243,   244,   245,
     246,   247,   189,    77,    78,   194,   189,   191,   189,   189,
     155,   156,   157,   158,   189,   160,   161,   162,   163,   164,
     189,   189,   189,   245,   246,   247,   189,   189,   212,   213,
     243,   244,   245,   246,   247,   189,     6,   189,   189,   243,
     244,   245,   246,   247,    14,   189,   192,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,   242,   189,
     192,    31,    32,    33,    34,    35,   971,    37,   876,   194,
     190,   190,    42,    43,    44,    45,    46,    47,    48,    49,
     154,    51,    52,    53,   190,    55,    56,    57,    58,    59,
      60,    61,   190,     3,     3,    65,    66,   189,    68,   173,
     174,   175,   176,   177,    74,   179,   180,   181,   182,   183,
     184,   185,   194,   189,     5,   198,   189,   191,   243,   244,
     245,   246,   247,   189,    94,   189,    76,   194,    76,   189,
     189,   186,   194,    36,     4,   943,   192,   107,   212,   213,
     189,   189,    36,   113,   155,    38,   116,   117,   222,   119,
      85,   121,   194,    85,   189,   189,   126,   127,   194,   189,
     130,   131,  1067,   133,    89,   135,   159,    95,     4,   118,
     194,   124,   189,   125,   196,  1080,   243,   244,   245,   246,
     247,   243,   244,   245,   246,   247,     3,   189,   192,     4,
    1029,   197,   197,   189,   189,   165,   166,   167,   250,   169,
     189,   243,   244,   245,   246,   247,   192,   243,   244,   245,
     246,   247,  1051,   189,   189,   189,   193,   189,   189,     4,
     189,   189,   189,   193,    79,    80,     4,   189,   193,   199,
     200,   189,     4,   190,   189,   189,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,     4,   200,   111,   112,   189,   189,
     115,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,    76,
       4,   189,     3,   189,   189,   140,   190,   189,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   200,
     207,   189,   209,   189,   211,   206,   213,   208,   215,   210,
     217,   212,   219,   214,   221,   216,   223,   218,   189,   220,
     118,   222,   189,   249,   189,   189,   192,   190,   189,     4,
     189,   191,     3,   189,   192,   192,   198,   190,   190,   190,
     190,   190,   197,   190,   190,   190,   190,   190,   190,   190,
     189,     5,   192,   208,   209,   189,    76,   189,   191,   189,
       4,   189,   155,     4,   155,   189,    85,    85,   197,   189,
      63,   189,   195,   189,    97,    92,   189,   189,   192,     4,
       4,   192,    98,   192,   192,   189,   189,   194,   190,   189,
       4,   190,     4,   189,   189,   189,     4,   189,     5,   189,
     189,     3,    39,   190,   192,    96,   190,   190,   190,   190,
     190,   190,     4,   189,   189,   192,   155,    64,   192,   189,
      87,   192,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   189,     7,   192,   190,   189,    77,     4,   123,
     196,   192,     4,   189,    93,   190,   189,     4,   190,   189,
      92,   189,     4,     4,     4,   190,     5,    39,   192,    95,
     190,   155,     4,   192,   250,   192,    87,    86,    86,     5,
     192,  1079,   394,   209,   189,   187,    67,    61,   192,   192,
     190,   192,   189,   189,   189,   189,   459,   520,   232,   192,
     894,   923,   918,   977,   203,   958,   792,  1031,   697,   169,
     972,   999,  1039,   763,   399,   513,   732,   489,    -1,   525,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   295
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   253,   254,   255,   256,   190,   190,     0,
     254,   109,   264,   197,   197,   190,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    31,
      32,    33,    34,    35,    37,    42,    43,    44,    45,    46,
      47,    48,    49,    51,    52,    53,    55,    56,    57,    58,
      59,    60,    61,    65,    66,    68,    74,    94,   107,   113,
     116,   117,   119,   121,   126,   127,   130,   131,   133,   135,
     165,   166,   167,   169,   193,   199,   200,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   257,   258,   259,   266,   268,
     269,   270,   271,   273,   274,   282,   285,   286,   288,   289,
     296,   298,   299,   301,   303,   305,   309,   310,   311,   314,
     316,   323,   330,   333,   339,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   355,   356,   357,   358,   362,
     363,   365,   366,   367,   368,   369,   370,   371,   372,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     189,   110,   265,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   288,   190,   190,   190,
     193,   200,   270,   272,   290,   190,   190,   190,   199,     4,
     191,   206,   207,   242,   416,   426,   191,   266,   248,   190,
     300,     3,   189,   197,   208,   209,   402,   415,    10,    11,
      12,    13,     4,    91,   427,    77,    78,    81,    82,   180,
     191,   212,   213,   331,   404,   405,    95,     3,    77,    90,
     191,   197,   220,   221,   341,   413,   414,   341,     3,    77,
     191,   197,   218,   219,   335,   411,   412,    77,   197,   386,
      69,    77,   396,   404,   404,   154,   173,   174,   175,   176,
     177,   179,   181,   182,   183,   184,   185,   191,   222,   404,
     424,   425,   191,   214,   215,   407,   408,   407,    54,   191,
     434,   435,   197,   404,   404,    88,   404,   191,   404,   434,
     407,   407,    77,   197,   313,   387,   434,   434,   434,   403,
     416,   191,   312,   416,   404,   425,   425,   404,   425,   425,
       4,     8,     9,    77,   428,   404,   425,   313,   425,     5,
      77,   324,   306,   425,   407,   269,   403,   248,   193,   404,
     404,   404,   191,     8,   416,   193,   171,   194,   243,   244,
     245,   246,   247,   415,   416,   421,   422,   423,    22,    24,
      28,    29,    30,   119,   178,   191,   195,   404,   407,   415,
     416,   430,   431,   432,   304,   195,   267,   265,   193,   249,
     189,   189,   189,   188,   406,   191,     4,   193,   189,   189,
     318,     3,   193,   342,   340,     3,   193,   334,   189,   189,
     189,   189,   189,   407,   407,   404,   404,   191,   404,   191,
     191,   191,   424,   191,   425,   251,     4,   193,   189,   189,
     191,     4,   189,   189,   189,   189,   189,   189,     4,   189,
     189,   189,     6,   287,   189,   189,   189,   189,     4,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   267,
     307,   189,   189,   194,   416,   403,   189,   189,   189,   283,
     192,   192,   416,   416,   416,   416,   416,   416,   416,   189,
     192,   190,     4,   278,   279,   280,   281,   405,   408,   415,
     416,   190,   190,   190,   268,   266,   416,   402,     3,     3,
     191,   364,     3,   189,   194,   425,   189,   416,    82,    83,
     332,     5,   198,   189,   416,   189,   267,   189,   416,   267,
     404,   404,   425,    76,    76,   189,   189,    76,   326,   425,
     404,   404,     3,     7,   191,   216,   217,   409,   410,   425,
     186,   192,   425,   189,   416,    70,    77,   397,    36,     4,
     189,    36,    38,    77,    79,    80,   398,   401,   155,   404,
      85,   189,    85,   434,    77,   191,   319,   189,   434,   434,
     434,   403,   189,   312,   428,   404,    72,    77,   433,    89,
      62,   162,   159,    95,    77,   191,   320,     4,   396,   308,
     191,   409,   409,   302,   132,   249,   297,   194,    77,   415,
     124,   125,   271,   418,   419,   420,   194,   194,   421,   425,
     189,   189,   196,   189,   196,   189,   196,   189,   196,   195,
     413,   195,   411,   195,   409,   196,   194,     3,   189,   406,
     192,     4,   194,   318,   197,   194,     4,     8,     9,    69,
      70,    71,    73,    75,    99,   100,   101,   103,   104,   105,
     106,   108,   128,   129,   137,   138,   139,   140,   141,   142,
     155,   168,   343,   404,   426,   429,   197,   194,   336,   328,
     189,   189,   404,   404,   250,   189,   192,   189,   189,   189,
       3,   193,   189,   189,   189,     4,   194,   189,   189,     4,
     189,   193,   189,   190,   189,     4,   189,     4,   189,   397,
     189,   189,   354,   189,    76,   189,   189,   190,   359,   189,
       4,   189,   189,   118,   189,   267,   249,   416,   291,   189,
     189,   190,   189,   192,     4,   408,   405,   416,   415,   275,
     414,   276,   412,   277,   410,   189,     3,   192,   198,   192,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   192,   189,   189,   396,     5,   189,   326,   425,
     416,   416,   425,   189,   416,   425,   425,   191,   189,   387,
       4,   189,   155,     4,    77,    84,   400,   411,   155,    85,
     189,    77,   191,   321,   197,    85,   403,   189,   415,    63,
     404,   189,    92,   189,    77,   191,   322,    77,   325,   326,
     267,   409,   195,   397,    77,   415,   202,   203,   417,   418,
     284,   189,   196,   189,   196,   189,   196,   397,   189,   413,
      98,    97,   403,   403,   403,   403,   415,   403,   403,   403,
     411,    79,    80,   111,   112,   115,   140,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   337,   399,
     401,   415,    73,   102,   103,   105,   114,   122,   142,   329,
     189,   262,   416,   192,   189,   192,   189,   192,   397,   194,
     192,   192,   416,     4,   189,   388,   189,     4,   190,   194,
     189,   190,     4,   331,   189,   189,     4,    77,   327,   189,
     189,    86,    93,   105,   122,   155,   156,   157,   158,   160,
     161,   162,   163,   164,   360,     4,     4,   388,   189,   189,
     134,   136,   292,   293,   267,   414,   412,   410,   192,     5,
     190,   415,   190,   190,   190,   190,   190,   190,   190,     3,
      39,   416,   192,   244,   189,    39,    40,    41,   389,   390,
      96,   391,     4,   189,   411,   155,   411,   192,   189,   322,
     403,   192,    64,    87,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   192,   189,   391,   327,     7,   428,
     190,   196,   292,   189,    77,   403,   403,   403,   154,   197,
     338,   413,     4,   123,   192,   189,   192,   416,     4,   189,
     190,    93,   393,   189,     4,   190,   332,   388,   189,   189,
      92,   394,     4,     4,    87,    89,   123,   361,   411,   413,
     364,   364,   404,   386,     4,     4,   393,   328,   190,   295,
       5,   250,    39,   189,   260,   192,   389,    95,   392,   190,
     155,   395,     4,   192,   411,   192,   391,    87,    86,   192,
     395,   294,   266,   189,   338,   192,   187,   192,   394,   190,
     373,   192,   393,   189,   315,   266,   397,   189,   411,   267,
     374,   395,    86,   267,   189,   404,   317,     5,    77,   263,
     189,   261,   267,   262,   404,   192
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
#line 292 "lev_comp.y"
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
#line 315 "lev_comp.y"
    {
		      start_level_def(&splev, (yyvsp[(3) - (3)].map));
		      (yyval.map) = (yyvsp[(3) - (3)].map);
		  ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 320 "lev_comp.y"
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
#line 342 "lev_comp.y"
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
#line 355 "lev_comp.y"
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
#line 367 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_ROGUE,0,0,0,
					  0,0,0,0, SPO_INITLEVEL));
		  ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 373 "lev_comp.y"
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
#line 402 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_TILESET));
		  ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 408 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_BOUNDARY_TYPE));
		  ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 414 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 418 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 424 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_COPY));
		      (yyval.i) = 0;
		  ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 429 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 435 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 439 "lev_comp.y"
    {
		      (yyval.i) = what_map_char((char) (yyvsp[(2) - (2)].i));
		  ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 450 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 454 "lev_comp.y"
    {
		      add_opvars(splev, "io",
                                 VA_PASS2((int) (yyvsp[(3) - (3)].i), SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 461 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 465 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 471 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 475 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 481 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (3)].i);
		  ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 578 "lev_comp.y"
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

  case 112:

/* Line 1455 of yacc.c  */
#line 594 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 600 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 606 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 612 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 618 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 624 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 630 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 636 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 642 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 652 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 662 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 672 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 682 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 692 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 702 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 714 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 719 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 726 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 731 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 738 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 743 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 750 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 754 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 760 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 765 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 772 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 776 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 782 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 786 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 792 "lev_comp.y"
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

  case 142:

/* Line 1455 of yacc.c  */
#line 814 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 818 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 829 "lev_comp.y"
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

  case 145:

/* Line 1455 of yacc.c  */
#line 884 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 890 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 894 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 900 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 907 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 911 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 919 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 923 "lev_comp.y"
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

  case 153:

/* Line 1455 of yacc.c  */
#line 946 "lev_comp.y"
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

  case 156:

/* Line 1455 of yacc.c  */
#line 991 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1001 "lev_comp.y"
    {
		  ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1004 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1014 "lev_comp.y"
    {
		  ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1019 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1033 "lev_comp.y"
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

  case 164:

/* Line 1455 of yacc.c  */
#line 1070 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1075 "lev_comp.y"
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

  case 166:

/* Line 1455 of yacc.c  */
#line 1104 "lev_comp.y"
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

  case 167:

/* Line 1455 of yacc.c  */
#line 1118 "lev_comp.y"
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

  case 168:

/* Line 1455 of yacc.c  */
#line 1134 "lev_comp.y"
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

  case 169:

/* Line 1455 of yacc.c  */
#line 1154 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1166 "lev_comp.y"
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

  case 171:

/* Line 1455 of yacc.c  */
#line 1186 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1192 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1202 "lev_comp.y"
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

  case 174:

/* Line 1455 of yacc.c  */
#line 1220 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1230 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1236 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1241 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1246 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1253 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1260 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1269 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1277 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1288 "lev_comp.y"
    {
		      long rflags = (yyvsp[(8) - (11)].i);
		      long flmt = (long)(yyvsp[(9) - (11)].i);
		      long flt = (long)(yyvsp[(10) - (11)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      if (flmt == -1) flmt = ROOM;
		      if (flt == -1) flt = 0;

		      add_opvars(splev, "iiiiiiiiio",
				 VA_PASS10(flt, flmt, rflags, ERR, ERR,
					  (yyvsp[(5) - (11)].crd).x, (yyvsp[(5) - (11)].crd).y, (yyvsp[(7) - (11)].sze).width, (yyvsp[(7) - (11)].sze).height,
					  SPO_SUBROOM));
		      break_stmt_start();
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1304 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1311 "lev_comp.y"
    {
		      long rflags = (yyvsp[(10) - (13)].i);
		      long flmt = (long)(yyvsp[(11) - (13)].i);
		      long flt = (long)(yyvsp[(12) - (13)].i);

		      if (rflags == -1) rflags = (1 << 0);

		      add_opvars(splev, "iiiiiiiiio",
				 VA_PASS10(flt, flmt, rflags,
					  (yyvsp[(7) - (13)].crd).x, (yyvsp[(7) - (13)].crd).y, (yyvsp[(5) - (13)].crd).x, (yyvsp[(5) - (13)].crd).y,
					  (yyvsp[(9) - (13)].sze).width, (yyvsp[(9) - (13)].sze).height, SPO_ROOM));
		      break_stmt_start();
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1325 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1332 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1336 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1342 "lev_comp.y"
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

  case 190:

/* Line 1455 of yacc.c  */
#line 1352 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1358 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1367 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1373 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1378 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1384 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1389 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1395 "lev_comp.y"
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

  case 198:

/* Line 1455 of yacc.c  */
#line 1406 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1420 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1424 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1434 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1441 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1449 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1454 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1459 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1464 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1469 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1474 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1479 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1487 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1494 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1502 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1518 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1522 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1528 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1536 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1542 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1550 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1558 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1563 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1569 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1575 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1581 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1587 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1592 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1597 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1602 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1607 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1612 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1617 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1622 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1627 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1632 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1637 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1643 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1650 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1659 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1663 "lev_comp.y"
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

  case 249:

/* Line 1455 of yacc.c  */
#line 1676 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1684 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1694 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1702 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1711 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1718 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1726 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1732 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1737 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1742 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1747 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1752 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1757 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1762 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1767 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1772 "lev_comp.y"
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

  case 265:

/* Line 1455 of yacc.c  */
#line 1786 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1792 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1797 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1802 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1807 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1812 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1817 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1822 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1827 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1832 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1837 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1842 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1847 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x400000;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1852 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x800000;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1857 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x1000000;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1864 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1870 "lev_comp.y"
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

  case 282:

/* Line 1455 of yacc.c  */
#line 1899 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1904 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1912 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1918 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1922 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1930 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1937 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1944 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1954 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1964 "lev_comp.y"
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

  case 292:

/* Line 1455 of yacc.c  */
#line 1979 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1989 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1993 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1999 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2005 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2011 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2015 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2021 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2027 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2034 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2042 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2048 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2053 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2058 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2063 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2068 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2074 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2080 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2085 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2090 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2095 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_CATEGORY));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2100 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2105 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2115 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2121 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2127 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2132 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2139 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2146 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2152 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2158 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2164 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2169 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2176 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2182 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2192 "lev_comp.y"
    {
		      long irr;
		      long rt = (yyvsp[(7) - (11)].i);
		      long rflags = (yyvsp[(8) - (11)].i);
		      long flmt = (long)(yyvsp[(9) - (11)].i);
		      long flt = (long)(yyvsp[(10) - (11)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      if (flmt == -1) flmt = ROOM;
		      if (flt == -1) flt = 0;

		      if (!(rflags & 1)) rt += MAXRTYPE+1;
		      irr = ((rflags & 2) != 0);
		      add_opvars(splev, "iiiiio",
				 VA_PASS6((long)(yyvsp[(5) - (11)].i), rt, rflags, flmt, flt, SPO_REGION));
		      (yyval.i) = (irr || (rflags & 1) || rt != OROOM);
		      break_stmt_start();
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2211 "lev_comp.y"
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

  case 333:

/* Line 1455 of yacc.c  */
#line 2223 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2227 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2233 "lev_comp.y"
    {
		      add_opvars(splev, "Miio",
				 VA_PASS4(-1, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2238 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2245 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2249 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2255 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2261 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2267 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2271 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2277 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2281 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2286 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2293 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2297 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2304 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2308 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2313 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2320 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2326 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2333 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2337 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2344 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2355 "lev_comp.y"
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

  case 359:

/* Line 1455 of yacc.c  */
#line 2368 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2372 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2378 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2382 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2389 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2393 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2397 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2403 "lev_comp.y"
    {
			(yyval.i) = ROOM;
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2407 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2413 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2419 "lev_comp.y"
    {
			(yyval.i) = 0;
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2423 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2429 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2435 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2440 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2457 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2465 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2475 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2484 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2489 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2496 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2507 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2513 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2517 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2521 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2528 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2538 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2545 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2549 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2555 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2559 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2567 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2571 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2578 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2588 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2601 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2605 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2612 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2622 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2631 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2642 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2646 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2653 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2663 "lev_comp.y"
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

  case 413:

/* Line 1455 of yacc.c  */
#line 2674 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2683 "lev_comp.y"
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

  case 415:

/* Line 1455 of yacc.c  */
#line 2693 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2699 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2703 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2710 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2720 "lev_comp.y"
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

  case 420:

/* Line 1455 of yacc.c  */
#line 2731 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2740 "lev_comp.y"
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

  case 422:

/* Line 1455 of yacc.c  */
#line 2750 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2754 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2760 "lev_comp.y"
    { ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2762 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2768 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2772 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2776 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2780 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2788 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2797 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2801 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2805 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2809 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2813 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2816 "lev_comp.y"
    { ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2820 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2827 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2836 "lev_comp.y"
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

  case 444:

/* Line 1455 of yacc.c  */
#line 2877 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2881 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2888 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2895 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2905 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2909 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2917 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2921 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2925 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2929 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2933 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2938 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2942 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2946 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2951 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2956 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2961 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2965 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2970 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2975 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2979 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2983 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2988 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2992 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2999 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 3005 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 3009 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 3015 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3031 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3035 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3039 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3043 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3065 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3069 "lev_comp.y"
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

  case 491:

/* Line 1455 of yacc.c  */
#line 3095 "lev_comp.y"
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
#line 6996 "lev_comp.tab.c"
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
#line 3123 "lev_comp.y"


/*lev_comp.y*/

