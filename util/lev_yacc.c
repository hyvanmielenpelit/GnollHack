
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
     PORTAL_ID = 305,
     TELEPRT_ID = 306,
     BRANCH_ID = 307,
     LEV = 308,
     MINERALIZE_ID = 309,
     CORRIDOR_ID = 310,
     GOLD_ID = 311,
     ENGRAVING_ID = 312,
     FOUNTAIN_ID = 313,
     THRONE_ID = 314,
     MODRON_PORTAL_ID = 315,
     LEVEL_TELEPORTER_ID = 316,
     LEVEL_TELEPORT_DIRECTION_TYPE = 317,
     LEVEL_TELEPORT_END_TYPE = 318,
     POOL_ID = 319,
     SINK_ID = 320,
     NONE = 321,
     RAND_CORRIDOR_ID = 322,
     DOOR_STATE = 323,
     LIGHT_STATE = 324,
     CURSE_TYPE = 325,
     ENGRAVING_TYPE = 326,
     KEYTYPE_ID = 327,
     LEVER_ID = 328,
     NO_PICKUP_ID = 329,
     DIRECTION = 330,
     RANDOM_TYPE = 331,
     RANDOM_TYPE_BRACKET = 332,
     A_REGISTER = 333,
     ALIGNMENT = 334,
     LEFT_OR_RIGHT = 335,
     CENTER = 336,
     TOP_OR_BOT = 337,
     ALTAR_TYPE = 338,
     UP_OR_DOWN = 339,
     ACTIVE_OR_INACTIVE = 340,
     MODRON_PORTAL_TYPE = 341,
     NPC_TYPE = 342,
     FOUNTAIN_TYPE = 343,
     SPECIAL_OBJECT_TYPE = 344,
     CMAP_TYPE = 345,
     FLOOR_TYPE = 346,
     FLOOR_TYPE_ID = 347,
     FLOOR_ID = 348,
     FLOOR_MAIN_TYPE = 349,
     FLOOR_MAIN_TYPE_ID = 350,
     ELEMENTAL_ENCHANTMENT_TYPE = 351,
     EXCEPTIONALITY_TYPE = 352,
     EXCEPTIONALITY_ID = 353,
     ELEMENTAL_ENCHANTMENT_ID = 354,
     ENCHANTMENT_ID = 355,
     SECRET_DOOR_ID = 356,
     USES_UP_KEY_ID = 357,
     CHARGES_ID = 358,
     SPECIAL_QUALITY_ID = 359,
     SPEFLAGS_ID = 360,
     SUBROOM_ID = 361,
     NAME_ID = 362,
     FLAGS_ID = 363,
     FLAG_TYPE = 364,
     MON_ATTITUDE = 365,
     MON_ALERTNESS = 366,
     SUBTYPE_ID = 367,
     NON_PASSDOOR_ID = 368,
     MON_APPEARANCE = 369,
     ROOMDOOR_ID = 370,
     IF_ID = 371,
     ELSE_ID = 372,
     TERRAIN_ID = 373,
     HORIZ_OR_VERT = 374,
     REPLACE_TERRAIN_ID = 375,
     LOCATION_SUBTYPE_ID = 376,
     DOOR_SUBTYPE = 377,
     EXIT_ID = 378,
     SHUFFLE_ID = 379,
     QUANTITY_ID = 380,
     BURIED_ID = 381,
     LOOP_ID = 382,
     FOR_ID = 383,
     TO_ID = 384,
     SWITCH_ID = 385,
     CASE_ID = 386,
     BREAK_ID = 387,
     DEFAULT_ID = 388,
     ERODED_ID = 389,
     TRAPPED_STATE = 390,
     RECHARGED_ID = 391,
     INVIS_ID = 392,
     GREASED_ID = 393,
     INDESTRUCTIBLE_ID = 394,
     FEMALE_ID = 395,
     WAITFORU_ID = 396,
     CANCELLED_ID = 397,
     REVIVED_ID = 398,
     AVENGE_ID = 399,
     FLEEING_ID = 400,
     BLINDED_ID = 401,
     PARALYZED_ID = 402,
     STUNNED_ID = 403,
     CONFUSED_ID = 404,
     SEENTRAPS_ID = 405,
     ALL_ID = 406,
     MONTYPE_ID = 407,
     OBJTYPE_ID = 408,
     TERTYPE_ID = 409,
     TERTYPE2_ID = 410,
     LEVER_EFFECT_TYPE = 411,
     SWITCHABLE_ID = 412,
     CONTINUOUSLY_USABLE_ID = 413,
     TARGET_ID = 414,
     TRAPTYPE_ID = 415,
     EFFECT_FLAG_ID = 416,
     GRAVE_ID = 417,
     ERODEPROOF_ID = 418,
     FUNCTION_ID = 419,
     MSG_OUTPUT_TYPE = 420,
     COMPARE_TYPE = 421,
     UNKNOWN_TYPE = 422,
     rect_ID = 423,
     fillrect_ID = 424,
     line_ID = 425,
     randline_ID = 426,
     grow_ID = 427,
     selection_ID = 428,
     flood_ID = 429,
     rndcoord_ID = 430,
     circle_ID = 431,
     ellipse_ID = 432,
     filter_ID = 433,
     complement_ID = 434,
     gradient_ID = 435,
     GRADIENT_TYPE = 436,
     LIMITED = 437,
     HUMIDITY_TYPE = 438,
     STRING = 439,
     MAP_ID = 440,
     NQSTRING = 441,
     VARSTRING = 442,
     CFUNC = 443,
     CFUNC_INT = 444,
     CFUNC_STR = 445,
     CFUNC_COORD = 446,
     CFUNC_REGION = 447,
     VARSTRING_INT = 448,
     VARSTRING_INT_ARRAY = 449,
     VARSTRING_STRING = 450,
     VARSTRING_STRING_ARRAY = 451,
     VARSTRING_VAR = 452,
     VARSTRING_VAR_ARRAY = 453,
     VARSTRING_COORD = 454,
     VARSTRING_COORD_ARRAY = 455,
     VARSTRING_REGION = 456,
     VARSTRING_REGION_ARRAY = 457,
     VARSTRING_MAPCHAR = 458,
     VARSTRING_MAPCHAR_ARRAY = 459,
     VARSTRING_MONST = 460,
     VARSTRING_MONST_ARRAY = 461,
     VARSTRING_OBJ = 462,
     VARSTRING_OBJ_ARRAY = 463,
     VARSTRING_SEL = 464,
     VARSTRING_SEL_ARRAY = 465,
     METHOD_INT = 466,
     METHOD_INT_ARRAY = 467,
     METHOD_STRING = 468,
     METHOD_STRING_ARRAY = 469,
     METHOD_VAR = 470,
     METHOD_VAR_ARRAY = 471,
     METHOD_COORD = 472,
     METHOD_COORD_ARRAY = 473,
     METHOD_REGION = 474,
     METHOD_REGION_ARRAY = 475,
     METHOD_MAPCHAR = 476,
     METHOD_MAPCHAR_ARRAY = 477,
     METHOD_MONST = 478,
     METHOD_MONST_ARRAY = 479,
     METHOD_OBJ = 480,
     METHOD_OBJ_ARRAY = 481,
     METHOD_SEL = 482,
     METHOD_SEL_ARRAY = 483,
     DICE = 484
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
#line 527 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 539 "lev_comp.tab.c"

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
#define YYLAST   1239

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  247
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  182
/* YYNRULES -- Number of rules.  */
#define YYNRULES  484
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1069

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   484

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   242,   246,     2,
     186,   187,   240,   238,   184,   239,   244,   241,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   185,     2,
       2,   243,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   188,     2,   189,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   190,   245,   191,     2,     2,     2,     2,
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
     175,   176,   177,   178,   179,   180,   181,   182,   183,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237
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
     263,   267,   273,   277,   283,   289,   295,   299,   303,   309,
     315,   321,   329,   337,   345,   351,   353,   357,   359,   363,
     365,   369,   371,   375,   377,   381,   383,   387,   389,   393,
     394,   395,   404,   409,   411,   412,   414,   416,   422,   426,
     427,   428,   438,   439,   442,   443,   449,   450,   455,   457,
     460,   462,   469,   470,   474,   475,   482,   483,   488,   489,
     494,   496,   497,   502,   506,   508,   512,   516,   522,   528,
     536,   541,   542,   556,   557,   573,   574,   577,   583,   585,
     591,   593,   599,   601,   607,   609,   620,   627,   629,   631,
     633,   635,   637,   641,   643,   645,   646,   650,   654,   658,
     662,   664,   666,   668,   670,   672,   680,   686,   688,   690,
     692,   694,   698,   699,   705,   710,   711,   715,   717,   719,
     721,   723,   726,   728,   730,   732,   734,   736,   740,   744,
     748,   750,   752,   756,   758,   760,   762,   766,   770,   771,
     777,   780,   781,   785,   787,   791,   793,   797,   801,   803,
     805,   809,   811,   813,   815,   819,   821,   823,   825,   829,
     833,   837,   841,   845,   849,   853,   855,   857,   859,   865,
     873,   879,   888,   890,   894,   898,   904,   910,   918,   926,
     933,   939,   940,   943,   949,   953,   965,   979,   986,   987,
     991,   993,   997,  1001,  1005,  1009,  1011,  1013,  1017,  1021,
    1025,  1029,  1033,  1037,  1039,  1041,  1043,  1045,  1049,  1053,
    1055,  1061,  1071,  1077,  1081,  1085,  1091,  1101,  1107,  1113,
    1114,  1128,  1129,  1131,  1139,  1151,  1155,  1163,  1171,  1177,
    1183,  1193,  1199,  1205,  1209,  1215,  1223,  1233,  1235,  1237,
    1239,  1241,  1243,  1244,  1247,  1249,  1253,  1255,  1257,  1259,
    1260,  1264,  1266,  1267,  1271,  1273,  1274,  1278,  1280,  1282,
    1284,  1286,  1288,  1290,  1292,  1294,  1296,  1300,  1302,  1304,
    1309,  1311,  1313,  1318,  1320,  1322,  1327,  1329,  1334,  1340,
    1342,  1346,  1348,  1352,  1354,  1356,  1361,  1371,  1373,  1375,
    1380,  1382,  1388,  1390,  1392,  1397,  1399,  1401,  1407,  1409,
    1411,  1413,  1418,  1420,  1422,  1428,  1430,  1432,  1434,  1438,
    1440,  1442,  1446,  1448,  1453,  1457,  1461,  1465,  1469,  1473,
    1477,  1479,  1481,  1485,  1487,  1491,  1492,  1494,  1496,  1498,
    1500,  1504,  1505,  1507,  1509,  1512,  1515,  1520,  1527,  1532,
    1539,  1546,  1553,  1560,  1563,  1570,  1579,  1588,  1599,  1614,
    1617,  1619,  1623,  1625,  1629,  1631,  1633,  1635,  1637,  1639,
    1641,  1643,  1645,  1647,  1649,  1651,  1653,  1655,  1657,  1659,
    1661,  1663,  1665,  1667,  1678
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     248,     0,    -1,    -1,   249,    -1,   250,    -1,   250,   249,
      -1,   251,   259,   261,    -1,    16,   185,   192,    -1,    15,
     185,   192,   184,     3,    -1,    17,   185,    11,   184,   359,
      -1,    17,   185,    10,   184,     3,    -1,    17,   185,    13,
      -1,    17,   185,    12,   184,     3,   184,     3,   184,     5,
     184,     5,   184,   390,   184,   258,   257,    -1,    18,   185,
     420,    -1,    21,   185,    94,    -1,    -1,   184,   182,    -1,
      -1,   184,   397,    -1,    -1,   184,     3,    -1,     5,    -1,
      76,    -1,    -1,   108,   185,   260,    -1,   109,   184,   260,
      -1,   109,    -1,    -1,   263,   261,    -1,   190,   261,   191,
      -1,   304,    -1,   252,    -1,   253,    -1,   254,    -1,   373,
      -1,   372,    -1,   370,    -1,   371,    -1,   374,    -1,   375,
      -1,   348,    -1,   306,    -1,   269,    -1,   268,    -1,   362,
      -1,   318,    -1,   340,    -1,   377,    -1,   378,    -1,   350,
      -1,   351,    -1,   352,    -1,   376,    -1,   284,    -1,   294,
      -1,   296,    -1,   300,    -1,   298,    -1,   281,    -1,   291,
      -1,   277,    -1,   280,    -1,   343,    -1,   353,    -1,   325,
      -1,   341,    -1,   328,    -1,   334,    -1,   363,    -1,   358,
      -1,   346,    -1,   305,    -1,   364,    -1,   365,    -1,   366,
      -1,   367,    -1,   311,    -1,   309,    -1,   357,    -1,   361,
      -1,   360,    -1,   344,    -1,   345,    -1,   347,    -1,   339,
      -1,   342,    -1,   202,    -1,   204,    -1,   206,    -1,   208,
      -1,   210,    -1,   212,    -1,   214,    -1,   216,    -1,   218,
      -1,   201,    -1,   203,    -1,   205,    -1,   207,    -1,   209,
      -1,   211,    -1,   213,    -1,   215,    -1,   217,    -1,   264,
      -1,   265,    -1,   195,    -1,   195,    -1,   265,    -1,   124,
     185,   264,    -1,   266,   243,   409,    -1,   266,   243,   173,
     185,   418,    -1,   266,   243,   408,    -1,   266,   243,   425,
     185,   402,    -1,   266,   243,   424,   185,   404,    -1,   266,
     243,   423,   185,   406,    -1,   266,   243,   397,    -1,   266,
     243,   400,    -1,   266,   243,   190,   275,   191,    -1,   266,
     243,   190,   274,   191,    -1,   266,   243,   190,   273,   191,
      -1,   266,   243,   425,   185,   190,   272,   191,    -1,   266,
     243,   424,   185,   190,   271,   191,    -1,   266,   243,   423,
     185,   190,   270,   191,    -1,   266,   243,   190,   276,   191,
      -1,   407,    -1,   270,   184,   407,    -1,   405,    -1,   271,
     184,   405,    -1,   403,    -1,   272,   184,   403,    -1,   401,
      -1,   273,   184,   401,    -1,   398,    -1,   274,   184,   398,
      -1,   409,    -1,   275,   184,   409,    -1,   408,    -1,   276,
     184,   408,    -1,    -1,    -1,   164,   194,   186,   278,   413,
     187,   279,   262,    -1,   194,   186,   416,   187,    -1,   123,
      -1,    -1,     6,    -1,     6,    -1,   188,   409,   166,   409,
     189,    -1,   188,   409,   189,    -1,    -1,    -1,   130,   285,
     188,   396,   189,   286,   190,   287,   191,    -1,    -1,   288,
     287,    -1,    -1,   131,   421,   185,   289,   261,    -1,    -1,
     133,   185,   290,   261,    -1,   132,    -1,   244,   244,    -1,
     129,    -1,   128,   267,   243,   409,   292,   409,    -1,    -1,
     293,   295,   262,    -1,    -1,   127,   188,   396,   189,   297,
     262,    -1,    -1,   283,   185,   299,   263,    -1,    -1,   116,
     283,   301,   302,    -1,   262,    -1,    -1,   262,   303,   117,
     262,    -1,    14,   185,   408,    -1,    67,    -1,    67,   185,
     421,    -1,    67,   185,    76,    -1,    55,   185,   307,   184,
     307,    -1,    55,   185,   307,   184,   421,    -1,   186,     4,
     184,    75,   184,   322,   187,    -1,   380,   282,   184,   390,
      -1,    -1,   106,   185,   308,   184,   315,   184,   317,   381,
     384,   386,   388,   310,   262,    -1,    -1,    49,   185,   308,
     184,   314,   184,   316,   184,   317,   381,   384,   386,   388,
     312,   262,    -1,    -1,   184,     5,    -1,   186,     4,   184,
       4,   187,    -1,    76,    -1,   186,     4,   184,     4,   187,
      -1,    76,    -1,   186,   326,   184,   327,   187,    -1,    76,
      -1,   186,     4,   184,     4,   187,    -1,    76,    -1,   115,
     185,   319,   184,   389,   184,   320,   184,   322,   323,    -1,
      26,   185,   389,   184,   418,   323,    -1,     5,    -1,    76,
      -1,   321,    -1,    76,    -1,    75,    -1,    75,   245,   321,
      -1,     4,    -1,    76,    -1,    -1,   323,   184,   324,    -1,
     121,   185,   122,    -1,    72,   185,   406,    -1,   104,   185,
       4,    -1,   139,    -1,   101,    -1,   102,    -1,   113,    -1,
      20,    -1,    19,   185,   326,   184,   327,   313,   193,    -1,
      19,   185,   397,   313,   193,    -1,    80,    -1,    81,    -1,
      82,    -1,    81,    -1,    24,   185,   330,    -1,    -1,    24,
     185,   330,   329,   262,    -1,   404,   184,   397,   331,    -1,
      -1,   331,   184,   332,    -1,   408,    -1,   110,    -1,   111,
      -1,   392,    -1,   114,   408,    -1,   140,    -1,   137,    -1,
     142,    -1,   143,    -1,   144,    -1,   145,   185,   396,    -1,
     146,   185,   396,    -1,   147,   185,   396,    -1,   148,    -1,
     149,    -1,   150,   185,   333,    -1,   141,    -1,   192,    -1,
     151,    -1,   192,   245,   333,    -1,    22,   185,   336,    -1,
      -1,    23,   185,   336,   335,   262,    -1,   406,   337,    -1,
      -1,   337,   184,   338,    -1,    70,    -1,   152,   185,   404,
      -1,   422,    -1,   107,   185,   408,    -1,   125,   185,   396,
      -1,   126,    -1,    69,    -1,   134,   185,   396,    -1,   163,
      -1,    68,    -1,   135,    -1,   136,   185,   396,    -1,   137,
      -1,   138,    -1,   397,    -1,    99,   185,    96,    -1,    98,
     185,    97,    -1,   100,   185,   396,    -1,   103,   185,   396,
      -1,   104,   185,   396,    -1,   105,   185,   396,    -1,    72,
     185,   406,    -1,   139,    -1,   102,    -1,    74,    -1,    25,
     185,   379,   184,   397,    -1,    27,   185,   397,   184,    75,
     184,   389,    -1,    31,   185,   397,   184,    75,    -1,    31,
     185,   397,   184,    75,   184,     5,   257,    -1,    32,    -1,
      32,   185,   418,    -1,    32,   185,   151,    -1,    45,   185,
     397,   184,    84,    -1,    46,   185,   397,   184,    84,    -1,
      46,   185,   427,   184,   427,   184,    84,    -1,    50,   185,
     427,   184,   427,   184,   192,    -1,    51,   185,   427,   184,
     427,   349,    -1,    52,   185,   427,   184,   427,    -1,    -1,
     184,    84,    -1,    58,   185,   418,   184,    88,    -1,    59,
     185,   418,    -1,    60,   185,   397,   184,   159,   185,   397,
     184,    86,   184,    85,    -1,    60,   185,   397,   184,    61,
     184,    62,   184,    63,   184,    86,   184,    85,    -1,    73,
     185,   397,   184,   156,   354,    -1,    -1,   354,   184,   355,
      -1,    85,    -1,   152,   185,   404,    -1,   153,   185,   406,
      -1,   154,   185,   359,    -1,   155,   185,   359,    -1,   157,
      -1,   158,    -1,   159,   185,   397,    -1,   160,   185,   379,
      -1,   121,   185,   356,    -1,    92,   185,   387,    -1,   161,
     185,     4,    -1,   104,   185,     4,    -1,     4,    -1,    86,
      -1,   122,    -1,    88,    -1,    65,   185,   418,    -1,    64,
     185,   418,    -1,     3,    -1,   186,     3,   184,   390,   187,
      -1,   120,   185,   400,   184,   402,   184,   402,   184,     7,
      -1,   118,   185,   418,   184,   402,    -1,    47,   185,   400,
      -1,    48,   185,   400,    -1,    37,   185,   192,   184,    38,
      -1,    37,   185,   192,   184,    38,   184,   152,   185,   404,
      -1,    34,   185,   400,   184,    36,    -1,    35,   185,   427,
     184,    36,    -1,    -1,    33,   185,   400,   184,   390,   184,
     380,   381,   384,   386,   388,   368,   369,    -1,    -1,   262,
      -1,    42,   185,   397,   184,   391,   184,   393,    -1,    42,
     185,   397,   184,   391,   184,   393,   184,   152,   185,   404,
      -1,    43,   185,   397,    -1,    43,   185,   397,   184,   152,
     185,   404,    -1,    93,   185,   418,   184,    94,   184,    91,
      -1,   112,   185,   418,   184,     4,    -1,    44,   185,    87,
     184,   397,    -1,    44,   185,    87,   184,   397,   184,   152,
     185,   404,    -1,   162,   185,   397,   184,   408,    -1,   162,
     185,   397,   184,    76,    -1,   162,   185,   397,    -1,    56,
     185,   409,   184,   397,    -1,    57,   185,   397,   184,   426,
     184,   408,    -1,    54,   185,   396,   184,   396,   184,   396,
     184,   396,    -1,    54,    -1,   192,    -1,    76,    -1,   192,
      -1,    76,    -1,    -1,   184,   382,    -1,   383,    -1,   383,
     184,   382,    -1,    39,    -1,    40,    -1,    41,    -1,    -1,
      95,   185,   385,    -1,    94,    -1,    -1,    92,   185,   387,
      -1,    91,    -1,    -1,   152,   185,   404,    -1,    68,    -1,
      76,    -1,    69,    -1,    76,    -1,    79,    -1,   394,    -1,
      76,    -1,    79,    -1,   394,    -1,    78,   185,    76,    -1,
      83,    -1,    76,    -1,    78,   188,     4,   189,    -1,   192,
      -1,   203,    -1,   204,   188,   409,   189,    -1,   409,    -1,
     398,    -1,   175,   186,   418,   187,    -1,   207,    -1,   208,
     188,   409,   189,    -1,   186,     4,   184,     4,   187,    -1,
      76,    -1,    77,   399,   189,    -1,   183,    -1,   183,   184,
     399,    -1,   401,    -1,   209,    -1,   210,   188,   409,   189,
      -1,   186,     4,   184,     4,   184,     4,   184,     4,   187,
      -1,   403,    -1,   211,    -1,   212,   188,   409,   189,    -1,
       3,    -1,   186,     3,   184,   390,   187,    -1,   405,    -1,
     213,    -1,   214,   188,   409,   189,    -1,   192,    -1,     3,
      -1,   186,     3,   184,   192,   187,    -1,    76,    -1,   407,
      -1,   215,    -1,   216,   188,   409,   189,    -1,   192,    -1,
       3,    -1,   186,     3,   184,   192,   187,    -1,    76,    -1,
      89,    -1,   395,    -1,   408,   244,   395,    -1,     4,    -1,
     419,    -1,   186,     8,   187,    -1,   201,    -1,   202,   188,
     409,   189,    -1,   409,   238,   409,    -1,   409,   239,   409,
      -1,   409,   240,   409,    -1,   409,   241,   409,    -1,   409,
     242,   409,    -1,   186,   409,   187,    -1,   197,    -1,   198,
      -1,   266,   185,   410,    -1,   411,    -1,   412,   184,   411,
      -1,    -1,   412,    -1,   409,    -1,   408,    -1,   414,    -1,
     415,   184,   414,    -1,    -1,   415,    -1,   397,    -1,   168,
     400,    -1,   169,   400,    -1,   170,   397,   184,   397,    -1,
     171,   397,   184,   397,   184,   409,    -1,   172,   186,   418,
     187,    -1,   172,   186,   321,   184,   418,   187,    -1,   178,
     186,     7,   184,   418,   187,    -1,   178,   186,   418,   184,
     418,   187,    -1,   178,   186,   402,   184,   418,   187,    -1,
     174,   397,    -1,   176,   186,   397,   184,   409,   187,    -1,
     176,   186,   397,   184,   409,   184,    39,   187,    -1,   177,
     186,   397,   184,   409,   184,   409,   187,    -1,   177,   186,
     397,   184,   409,   184,   409,   184,    39,   187,    -1,   180,
     186,   181,   184,   186,   409,   239,   409,   255,   187,   184,
     397,   256,   187,    -1,   179,   417,    -1,   217,    -1,   186,
     418,   187,    -1,   417,    -1,   417,   246,   418,    -1,   237,
      -1,    90,    -1,     4,    -1,     8,    -1,     9,    -1,     4,
      -1,     8,    -1,     9,    -1,     4,    -1,   419,    -1,    28,
      -1,    22,    -1,    29,    -1,    24,    -1,    30,    -1,   118,
      -1,    71,    -1,    76,    -1,   428,    -1,    53,   186,     4,
     184,     4,   184,     4,   184,     4,   187,    -1,   186,     4,
     184,     4,   184,     4,   184,     4,   187,    -1
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
     536,   537,   538,   539,   540,   541,   544,   545,   546,   547,
     548,   549,   550,   551,   552,   555,   556,   557,   558,   559,
     560,   561,   562,   563,   566,   567,   568,   571,   572,   575,
     591,   597,   603,   609,   615,   621,   627,   633,   639,   649,
     659,   669,   679,   689,   699,   711,   716,   723,   728,   735,
     740,   747,   751,   757,   762,   769,   773,   779,   783,   790,
     812,   789,   826,   881,   888,   891,   897,   904,   908,   917,
     921,   916,   984,   985,   989,   988,  1002,  1001,  1016,  1026,
    1027,  1030,  1068,  1067,  1102,  1101,  1132,  1131,  1164,  1163,
    1189,  1200,  1199,  1227,  1233,  1238,  1243,  1250,  1257,  1266,
    1274,  1286,  1285,  1309,  1308,  1330,  1333,  1339,  1349,  1355,
    1364,  1370,  1375,  1381,  1386,  1392,  1403,  1409,  1410,  1413,
    1414,  1417,  1421,  1427,  1428,  1432,  1438,  1446,  1451,  1456,
    1461,  1466,  1471,  1476,  1484,  1491,  1499,  1507,  1508,  1511,
    1512,  1515,  1520,  1519,  1533,  1540,  1547,  1555,  1560,  1566,
    1572,  1578,  1584,  1589,  1594,  1599,  1604,  1609,  1614,  1619,
    1624,  1629,  1634,  1640,  1647,  1656,  1660,  1673,  1682,  1681,
    1699,  1709,  1715,  1723,  1729,  1734,  1739,  1744,  1749,  1754,
    1759,  1764,  1769,  1780,  1786,  1791,  1796,  1801,  1806,  1811,
    1816,  1821,  1826,  1831,  1836,  1841,  1846,  1851,  1858,  1864,
    1893,  1898,  1906,  1912,  1916,  1924,  1931,  1938,  1948,  1958,
    1973,  1984,  1987,  1993,  1999,  2005,  2009,  2015,  2022,  2028,
    2036,  2042,  2047,  2052,  2057,  2062,  2068,  2074,  2079,  2084,
    2089,  2094,  2099,  2106,  2106,  2106,  2106,  2109,  2115,  2121,
    2126,  2133,  2140,  2146,  2152,  2158,  2163,  2170,  2176,  2187,
    2186,  2218,  2221,  2227,  2232,  2239,  2243,  2249,  2255,  2261,
    2265,  2271,  2275,  2280,  2287,  2293,  2300,  2304,  2311,  2319,
    2322,  2332,  2336,  2339,  2345,  2349,  2356,  2360,  2364,  2371,
    2374,  2380,  2387,  2390,  2396,  2403,  2407,  2414,  2415,  2418,
    2419,  2422,  2423,  2424,  2430,  2431,  2432,  2438,  2439,  2442,
    2451,  2456,  2463,  2474,  2480,  2484,  2488,  2495,  2505,  2512,
    2516,  2522,  2526,  2534,  2538,  2545,  2555,  2568,  2572,  2579,
    2589,  2598,  2609,  2613,  2620,  2630,  2641,  2650,  2660,  2666,
    2670,  2677,  2687,  2698,  2707,  2717,  2721,  2728,  2729,  2735,
    2739,  2743,  2747,  2755,  2764,  2768,  2772,  2776,  2780,  2784,
    2787,  2794,  2803,  2836,  2837,  2840,  2841,  2844,  2848,  2855,
    2862,  2873,  2876,  2884,  2888,  2892,  2896,  2900,  2905,  2909,
    2913,  2918,  2923,  2928,  2932,  2937,  2942,  2946,  2950,  2955,
    2959,  2966,  2972,  2976,  2982,  2989,  2990,  2993,  2994,  2995,
    2998,  3002,  3006,  3010,  3016,  3017,  3020,  3021,  3024,  3025,
    3028,  3029,  3032,  3036,  3062
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
  "ROOM_ID", "PORTAL_ID", "TELEPRT_ID", "BRANCH_ID", "LEV",
  "MINERALIZE_ID", "CORRIDOR_ID", "GOLD_ID", "ENGRAVING_ID", "FOUNTAIN_ID",
  "THRONE_ID", "MODRON_PORTAL_ID", "LEVEL_TELEPORTER_ID",
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
  "REPLACE_TERRAIN_ID", "LOCATION_SUBTYPE_ID", "DOOR_SUBTYPE", "EXIT_ID",
  "SHUFFLE_ID", "QUANTITY_ID", "BURIED_ID", "LOOP_ID", "FOR_ID", "TO_ID",
  "SWITCH_ID", "CASE_ID", "BREAK_ID", "DEFAULT_ID", "ERODED_ID",
  "TRAPPED_STATE", "RECHARGED_ID", "INVIS_ID", "GREASED_ID",
  "INDESTRUCTIBLE_ID", "FEMALE_ID", "WAITFORU_ID", "CANCELLED_ID",
  "REVIVED_ID", "AVENGE_ID", "FLEEING_ID", "BLINDED_ID", "PARALYZED_ID",
  "STUNNED_ID", "CONFUSED_ID", "SEENTRAPS_ID", "ALL_ID", "MONTYPE_ID",
  "OBJTYPE_ID", "TERTYPE_ID", "TERTYPE2_ID", "LEVER_EFFECT_TYPE",
  "SWITCHABLE_ID", "CONTINUOUSLY_USABLE_ID", "TARGET_ID", "TRAPTYPE_ID",
  "EFFECT_FLAG_ID", "GRAVE_ID", "ERODEPROOF_ID", "FUNCTION_ID",
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
  "gold_detail", "engraving_detail", "mineralize", "trap_name",
  "room_type", "optroomregionflags", "roomregionflags", "roomregionflag",
  "optfloormaintype", "floormaintype", "optfloortype", "floortype",
  "optmontype", "door_state", "light_state", "alignment", "alignment_prfx",
  "altar_type", "a_register", "string_or_var", "integer_or_var",
  "coord_or_var", "encodecoord", "humidity_flags", "region_or_var",
  "encoderegion", "mapchar_or_var", "mapchar", "monster_or_var",
  "encodemonster", "object_or_var", "encodeobj", "string_expr",
  "math_expr_var", "func_param_type", "func_param_part", "func_param_list",
  "func_params_list", "func_call_param_part", "func_call_param_list",
  "func_call_params_list", "ter_selection_x", "ter_selection", "dice",
  "tileset_number", "all_integers", "all_ints_push", "objectid",
  "monsterid", "terrainid", "engraving_type", "lev_region", "region", 0
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
     435,   436,   437,   438,    44,    58,    40,    41,    91,    93,
     123,   125,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,    43,    45,
      42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   247,   248,   248,   249,   249,   250,   251,   251,   252,
     252,   252,   252,   253,   254,   255,   255,   256,   256,   257,
     257,   258,   258,   259,   259,   260,   260,   261,   261,   262,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   266,   266,   266,   267,   267,   268,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   270,   270,   271,   271,   272,
     272,   273,   273,   274,   274,   275,   275,   276,   276,   278,
     279,   277,   280,   281,   282,   282,   283,   283,   283,   285,
     286,   284,   287,   287,   289,   288,   290,   288,   291,   292,
     292,   293,   295,   294,   297,   296,   299,   298,   301,   300,
     302,   303,   302,   304,   305,   305,   305,   306,   306,   307,
     308,   310,   309,   312,   311,   313,   313,   314,   314,   315,
     315,   316,   316,   317,   317,   318,   318,   319,   319,   320,
     320,   321,   321,   322,   322,   323,   323,   324,   324,   324,
     324,   324,   324,   324,   325,   325,   325,   326,   326,   327,
     327,   328,   329,   328,   330,   331,   331,   332,   332,   332,
     332,   332,   332,   332,   332,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   333,   333,   333,   334,   335,   334,
     336,   337,   337,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   339,   340,
     341,   341,   342,   342,   342,   343,   344,   345,   346,   347,
     348,   349,   349,   350,   351,   352,   352,   353,   354,   354,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   356,   356,   356,   356,   357,   358,   359,
     359,   360,   361,   362,   363,   364,   364,   365,   366,   368,
     367,   369,   369,   370,   370,   371,   371,   372,   373,   374,
     374,   375,   375,   375,   376,   377,   378,   378,   379,   379,
     380,   380,   381,   381,   382,   382,   383,   383,   383,   384,
     384,   385,   386,   386,   387,   388,   388,   389,   389,   390,
     390,   391,   391,   391,   392,   392,   392,   393,   393,   394,
     395,   395,   395,   396,   397,   397,   397,   397,   398,   398,
     398,   399,   399,   400,   400,   400,   401,   402,   402,   402,
     403,   403,   404,   404,   404,   405,   405,   405,   405,   406,
     406,   406,   407,   407,   407,   407,   407,   408,   408,   409,
     409,   409,   409,   409,   409,   409,   409,   409,   409,   409,
     410,   410,   411,   412,   412,   413,   413,   414,   414,   415,
     415,   416,   416,   417,   417,   417,   417,   417,   417,   417,
     417,   417,   417,   417,   417,   417,   417,   417,   417,   417,
     417,   417,   418,   418,   419,   420,   420,   421,   421,   421,
     422,   422,   422,   422,   423,   423,   424,   424,   425,   425,
     426,   426,   427,   427,   428
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     5,     3,     5,     5,     5,     3,     3,     5,     5,
       5,     7,     7,     7,     5,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     0,
       0,     8,     4,     1,     0,     1,     1,     5,     3,     0,
       0,     9,     0,     2,     0,     5,     0,     4,     1,     2,
       1,     6,     0,     3,     0,     6,     0,     4,     0,     4,
       1,     0,     4,     3,     1,     3,     3,     5,     5,     7,
       4,     0,    13,     0,    15,     0,     2,     5,     1,     5,
       1,     5,     1,     5,     1,    10,     6,     1,     1,     1,
       1,     1,     3,     1,     1,     0,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     7,     5,     1,     1,     1,
       1,     3,     0,     5,     4,     0,     3,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     3,     3,     3,
       1,     1,     3,     1,     1,     1,     3,     3,     0,     5,
       2,     0,     3,     1,     3,     1,     3,     3,     1,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     5,     7,
       5,     8,     1,     3,     3,     5,     5,     7,     7,     6,
       5,     0,     2,     5,     3,    11,    13,     6,     0,     3,
       1,     3,     3,     3,     3,     1,     1,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     3,     3,     1,
       5,     9,     5,     3,     3,     5,     9,     5,     5,     0,
      13,     0,     1,     7,    11,     3,     7,     7,     5,     5,
       9,     5,     5,     3,     5,     7,     9,     1,     1,     1,
       1,     1,     0,     2,     1,     3,     1,     1,     1,     0,
       3,     1,     0,     3,     1,     0,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     4,
       1,     1,     4,     1,     1,     4,     1,     4,     5,     1,
       3,     1,     3,     1,     1,     4,     9,     1,     1,     4,
       1,     5,     1,     1,     4,     1,     1,     5,     1,     1,
       1,     4,     1,     1,     5,     1,     1,     1,     3,     1,
       1,     3,     1,     4,     3,     3,     3,     3,     3,     3,
       1,     1,     3,     1,     3,     0,     1,     1,     1,     1,
       3,     0,     1,     1,     2,     2,     4,     6,     4,     6,
       6,     6,     6,     2,     6,     8,     8,    10,    14,     2,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,    10,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    23,     0,     0,     1,
       5,     0,    27,     0,     7,     0,   146,     0,     0,     0,
       0,   214,     0,     0,     0,     0,     0,     0,     0,     0,
     282,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   347,     0,     0,     0,
       0,     0,     0,     0,     0,   174,     0,     0,     0,     0,
       0,     0,     0,     0,   143,     0,     0,     0,   149,   158,
       0,     0,     0,     0,   106,    95,    86,    96,    87,    97,
      88,    98,    89,    99,    90,   100,    91,   101,    92,   102,
      93,   103,    94,    31,    32,    33,     6,    27,   104,   105,
       0,    43,    42,    60,    61,    58,     0,    53,    59,   162,
      54,    55,    57,    56,    30,    71,    41,    77,    76,    45,
      64,    66,    67,    84,    46,    65,    85,    62,    81,    82,
      70,    83,    40,    49,    50,    51,    63,    78,    69,    80,
      79,    44,    68,    72,    73,    74,    75,    36,    37,    35,
      34,    38,    39,    52,    47,    48,     0,    26,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   168,     0,     0,     0,     0,   107,   108,     0,
       0,     0,     0,   419,     0,   422,     0,   464,     0,   420,
     441,    28,     0,   166,     0,     8,     0,   380,   381,     0,
     417,   173,     0,     0,     0,    11,   466,   465,    13,   389,
       0,   217,   218,     0,     0,   386,     0,     0,   185,   384,
      14,   413,   415,   416,     0,   412,   410,     0,   247,   251,
     409,   248,   406,   408,     0,   405,   403,     0,   221,     0,
     402,   349,   348,     0,   367,   368,     0,     0,     0,   284,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   460,   443,   462,   283,     0,   394,     0,     0,
     393,     0,     0,     0,     0,   482,     0,     0,   335,     0,
       0,     0,     0,     0,   323,   324,   351,   350,     0,   144,
       0,     0,     0,     0,   383,     0,     0,     0,     0,     0,
     294,     0,   318,   317,   469,   467,   468,   176,   175,     0,
       0,     0,     0,   197,   198,     0,     0,     0,     0,   109,
       0,     0,     0,   343,   139,     0,     0,     0,     0,   148,
       0,     0,     0,     0,     0,   438,   437,   439,   442,     0,
     475,   477,   474,   476,   478,   479,     0,     0,     0,   116,
     117,   112,   110,     0,     0,     0,     0,    27,   163,    25,
       0,     0,     0,     0,     0,   391,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   250,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   444,   445,     0,     0,     0,
     453,     0,     0,     0,   459,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   145,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   170,   169,     0,     0,   164,     0,     0,     0,   435,
     421,   429,     0,     0,   424,   425,   426,   427,   428,     0,
     142,     0,   419,     0,     0,     0,     0,   133,   131,   137,
     135,     0,     0,     0,   167,     0,     0,   418,    10,   319,
       0,     9,     0,     0,   390,     0,     0,     0,   220,   219,
     185,   186,   216,     0,     0,     0,   249,     0,     0,   223,
     225,   278,   205,     0,   280,     0,     0,   201,     0,     0,
       0,     0,   400,     0,     0,   398,     0,     0,   397,     0,
       0,   461,   463,     0,     0,   369,   370,     0,   327,     0,
       0,   328,   325,   373,     0,   371,     0,   372,     0,   339,
     285,     0,   286,     0,   188,     0,     0,     0,     0,   291,
     290,     0,     0,   177,   178,   344,   480,   481,     0,   293,
       0,     0,   298,     0,   190,     0,     0,   338,     0,     0,
       0,   322,     0,     0,   160,     0,     0,   150,   342,   341,
       0,   433,   436,     0,   423,   147,   440,   111,     0,     0,
     120,     0,   119,     0,   118,     0,   124,     0,   115,     0,
     114,     0,   113,    29,   382,     0,     0,   392,   385,     0,
     387,     0,     0,   411,   472,   470,   471,   262,   259,   253,
       0,   277,     0,     0,     0,   276,     0,     0,     0,     0,
       0,   258,     0,   263,     0,   265,   266,   275,     0,   261,
     252,   267,   473,   255,     0,   404,   224,   196,     0,     0,
     446,     0,     0,     0,   448,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   395,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   180,     0,     0,
     289,     0,     0,     0,     0,     0,   297,     0,     0,     0,
       0,     0,     0,   165,   159,   161,     0,     0,     0,   140,
       0,   132,   134,   136,   138,     0,   125,     0,   127,     0,
     129,     0,     0,   388,   215,   414,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   407,     0,
       0,   279,    19,     0,   202,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   352,     0,     0,     0,     0,
     378,   377,   333,   336,     0,   287,     0,   192,     0,     0,
     288,   292,     0,     0,   345,     0,     0,     0,   337,     0,
     194,     0,   352,   200,     0,   199,   172,     0,   152,   430,
     431,   432,   434,     0,     0,   123,     0,   122,     0,   121,
       0,     0,   274,   269,   268,   270,   271,   272,   273,   256,
     257,   260,   264,   254,     0,   374,   228,   229,     0,   233,
     232,   243,   234,   235,   236,     0,     0,     0,   240,   241,
       0,   226,   230,   375,   227,     0,   211,   212,     0,   213,
       0,   210,   206,     0,   281,   447,   449,     0,   454,     0,
     450,     0,   399,   452,   451,     0,     0,     0,   359,     0,
       0,     0,   379,     0,     0,     0,     0,     0,     0,   203,
     204,     0,     0,     0,   300,     0,     0,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,   299,     0,     0,
     359,     0,     0,     0,     0,     0,   152,   141,   126,   128,
     130,   320,     0,     0,   231,     0,     0,     0,     0,     0,
       0,     0,    20,     0,     0,   401,     0,     0,   356,   357,
     358,   353,   354,     0,   362,     0,     0,   326,     0,   340,
     187,     0,   352,   346,   179,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   189,     0,   362,
     205,   321,     0,   156,   151,   153,     0,   376,   237,   238,
     239,   245,   244,   242,   208,   209,   207,   455,     0,   456,
     425,     0,     0,     0,     0,   365,     0,     0,     0,     0,
     359,     0,     0,   364,   310,   312,   313,   314,   316,   315,
     309,   301,   302,   303,   304,   307,   308,   311,     0,   365,
     195,   154,    27,     0,     0,     0,     0,     0,   396,   355,
     361,   360,     0,     0,   329,     0,   484,   334,   191,   362,
       0,   295,   193,   181,    27,   157,     0,   246,   457,    16,
       0,   363,     0,   331,   483,   365,     0,     0,   155,     0,
       0,   366,   332,   330,   183,   296,   182,     0,    17,     0,
      21,    22,    19,     0,     0,   184,    12,    18,   458
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    93,    94,    95,  1017,  1064,
     854,  1062,    12,   158,    96,   388,    97,    98,    99,   100,
     209,   101,   102,   725,   727,   729,   483,   484,   485,   486,
     103,   469,   803,   104,   105,   445,   106,   107,   210,   716,
     905,   906,  1034,  1012,   108,   596,   109,   110,   224,   111,
     593,   112,   386,   113,   346,   462,   589,   114,   115,   116,
     326,   318,   117,  1047,   118,  1059,   402,   566,   586,   779,
     792,   119,   345,   794,   528,   881,   667,   852,   120,   247,
     510,   121,   409,   268,   666,   841,   973,   122,   406,   258,
     405,   660,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   700,   133,   134,   135,   136,   706,   897,  1000,
     137,   138,   501,   139,   140,   141,   142,   143,   144,   145,
     146,  1043,  1053,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   273,   319,   868,   931,   932,   934,  1021,   985,
     994,  1024,   276,   547,   556,   842,   772,   557,   230,   323,
     293,   249,   396,   299,   300,   537,   538,   269,   270,   259,
     260,   365,   324,   801,   601,   602,   603,   367,   368,   369,
     294,   426,   219,   238,   338,   663,   383,   384,   385,   578,
     304,   305
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -913
static const yytype_int16 yypact[] =
{
     119,   -96,   -79,   149,  -913,   119,    11,   -35,   -25,  -913,
    -913,     4,   715,   168,  -913,    92,  -913,    38,   120,   197,
     200,  -913,   203,   210,   217,   230,   233,   236,   244,   248,
     251,   253,   255,   257,   260,   262,   272,   278,   279,   281,
     284,   285,   287,   295,   335,   336,   347,   348,   354,   358,
     360,   361,   363,   364,   380,   381,   383,   386,   387,   388,
     390,    17,   396,   400,  -913,   403,    21,   856,  -913,  -913,
     404,    77,    29,   147,  -913,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,   715,  -913,  -913,
     176,  -913,  -913,  -913,  -913,  -913,   405,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,   427,   283,  -913,  -100,
     500,    90,   323,   341,    25,    25,   148,   -31,    86,    -4,
      -4,   783,  -111,  -111,    -1,   252,    -4,    -4,   415,    -4,
     241,  -111,  -111,   -22,    -1,    -1,    -1,    29,   411,    29,
      -4,   796,   796,    -4,   796,   796,    74,    -4,   796,   -22,
     796,    83,  -913,   796,  -111,   850,    29,  -913,  -913,   326,
     410,    -4,   414,  -913,    73,  -913,   421,  -913,   212,  -913,
      35,  -913,   204,  -913,   409,  -913,    92,  -913,  -913,   422,
    -913,   367,   431,   432,   434,  -913,  -913,  -913,  -913,  -913,
     436,  -913,  -913,   439,   629,  -913,   462,   460,   472,  -913,
    -913,  -913,  -913,  -913,   654,  -913,  -913,   470,  -913,  -913,
    -913,  -913,  -913,  -913,   663,  -913,  -913,   479,   478,   486,
    -913,  -913,  -913,   487,  -913,  -913,   489,   496,   504,  -913,
    -111,  -111,    -4,    -4,   503,    -4,   506,   513,   514,   796,
     517,   307,  -913,  -913,   444,  -913,   700,  -913,   519,   521,
    -913,   524,   523,   706,   532,  -913,   533,   535,   536,   538,
     539,   721,   542,   543,  -913,  -913,  -913,  -913,   544,   724,
     547,   559,   560,   561,   266,   749,   570,  -110,   571,   572,
    -913,   584,  -913,  -913,  -913,  -913,  -913,  -913,  -913,   592,
     593,   594,   597,  -913,  -913,   599,   409,   600,   601,  -913,
     598,    29,    29,   602,  -913,   603,   407,    29,    29,  -913,
      29,    29,    29,    29,    29,   367,   266,  -913,   605,   604,
    -913,  -913,  -913,  -913,  -913,  -913,   607,    82,   122,  -913,
    -913,   367,   266,   609,   611,   613,   715,   715,  -913,  -913,
      29,  -100,   792,    27,   803,   623,   620,   796,   626,    29,
      -2,   806,   619,   630,    29,   631,   409,   633,    29,   409,
      -4,    -4,   796,   738,   743,  -913,  -913,   635,   636,   507,
    -913,    -4,    -4,   384,  -913,   641,   637,   796,   639,    29,
     121,   789,   822,   644,   793,   798,   296,   682,    -4,   753,
     656,   760,    -1,   -20,  -913,   662,    -1,    -1,    -1,    29,
     669,    49,    -4,   116,   773,   -19,   707,   768,   -14,   866,
      86,   754,  -913,    81,    81,  -913,   184,   685,   228,   814,
    -913,  -913,   413,   423,   254,   254,  -913,  -913,  -913,    35,
    -913,   796,   691,    16,    28,    64,   157,  -913,  -913,   367,
     266,   153,   117,    67,  -913,   687,   562,  -913,  -913,  -913,
     873,  -913,   696,   436,  -913,   699,   883,   610,  -913,  -913,
     472,  -913,  -913,   703,   616,    39,  -913,   704,   627,  -913,
    -913,  -913,  -913,   713,   714,    -4,    -4,   655,   717,   701,
     720,   722,  -913,   723,   452,  -913,   711,   727,  -913,   728,
     729,  -913,  -913,   901,   643,  -913,  -913,   730,  -913,   751,
     904,  -913,   752,  -913,   750,  -913,   755,  -913,   756,   758,
    -913,   936,  -913,   759,  -913,   940,   761,   121,   762,   763,
    -913,   764,   874,  -913,  -913,  -913,  -913,  -913,   766,  -913,
     772,   795,  -913,   797,  -913,   973,   799,  -913,   800,   868,
     983,  -913,   804,   409,  -913,   745,    29,  -913,  -913,   367,
     802,  -913,   808,   807,  -913,  -913,  -913,  -913,   989,   809,
    -913,   -17,  -913,    29,  -913,  -100,  -913,   282,  -913,    24,
    -913,    32,  -913,  -913,  -913,   812,   994,  -913,  -913,   811,
    -913,   813,   815,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
     816,  -913,   820,   823,   827,  -913,   848,   849,   851,   853,
     854,  -913,   870,  -913,   885,  -913,  -913,  -913,   887,  -913,
    -913,  -913,  -913,  -913,   888,  -913,   869,   890,    86,  1002,
    -913,   892,   924,   796,  -913,    29,    29,   796,   893,    29,
     796,   796,   894,   895,  -913,   -22,  1031,   897,   926,  1078,
     268,   148,   931,   -82,  1000,   902,    -7,  -913,   896,  1001,
    -913,    29,   903,  -100,  1027,    -4,   906,  1003,   907,     0,
     245,   409,    81,  -913,  -913,   266,   905,   195,   814,  -913,
     145,  -913,  -913,   266,   367,   165,  -913,   166,  -913,   169,
    -913,   121,   909,  -913,  -913,  -913,    25,   999,  1004,    29,
      29,    29,    29,  -100,    29,    29,    29,   148,  -913,   900,
     234,  -913,   913,    29,  -913,   911,   250,   366,   912,   121,
     652,   914,   915,    29,  1101,   922,   923,  1104,   925,   920,
    -913,  -913,   927,  -913,   928,  -913,  1108,  -913,   317,   930,
    -913,  -913,   932,    87,   367,   933,   934,   482,  -913,  1111,
    -913,  1115,   922,  -913,   937,  -913,  -913,   938,    22,  -913,
    -913,  -913,  -913,   409,   282,  -913,    24,  -913,    32,  -913,
     939,  1118,  -913,  -913,  -913,  -913,  -913,  -913,  -913,   367,
    -913,  -913,  -913,  -913,   178,  -913,  -913,  -913,  -100,  -913,
    -913,  -913,  -913,  -913,  -913,   935,   942,   943,  -913,  -913,
     944,  -913,  -913,  -913,   367,   945,  -913,  -913,   946,  -913,
     947,  -913,  -913,  1121,  -913,   266,  -913,  1086,  -913,    29,
    -913,   948,  -913,  -913,  -913,   456,   949,   477,  1039,  1132,
     953,   148,  -913,   986,   148,   952,   956,     0,    29,  -913,
    -913,   954,  1079,  1057,  -913,   959,   960,   961,   962,   963,
     964,   965,  -913,  -913,   966,   967,   968,  -913,   969,   970,
    1039,    87,  1148,   372,   972,   971,    22,  -913,  -913,  -913,
    -913,  -913,   974,  1083,   367,    29,    29,    29,   -80,    25,
    1156,  1041,  -913,   977,   338,  -913,    29,  1157,  -913,  -913,
    -913,  -913,   981,   982,  1074,   984,  1165,  -913,   985,  -913,
    -913,    -2,   922,  -913,  -913,   987,   988,  1082,  1170,    59,
     148,    25,    27,    27,    -4,   -31,  1171,  -913,  1172,  1074,
    -913,  -913,   992,  -913,  -913,  -913,  1173,  -913,  -913,  -913,
    -913,  -913,   941,  -913,  -913,  -913,  -913,  -913,  1140,  -913,
     259,   993,   477,  1087,   997,  1032,  1179,   998,   148,  1005,
    1039,  1102,  1105,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  1006,  1032,
     890,  -913,   715,  1007,   -80,  1008,  1012,  1009,  -913,  -913,
    -913,  -913,  1082,  1013,  -913,  1010,  -913,  -913,  -913,  1074,
    1015,  -913,  -913,  -913,   715,  -913,   121,  -913,  -913,  -913,
    1016,  -913,   148,   409,  -913,  1032,  1116,   409,  -913,  1018,
      -4,  -913,  -913,  -913,  -913,  -913,  -913,   112,  1019,   409,
    -913,  -913,   913,    -4,  1017,  -913,  -913,  -913,  -913
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -913,  -913,  1182,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
     127,  -913,  -913,   979,   -97,  -341,   821,  1011,  1139,  -455,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  1147,  -913,  -913,  -913,
     303,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
     767,  1014,  -913,  -913,  -913,  -913,   702,  -913,  -913,  -913,
     333,  -913,  -913,  -913,  -587,   310,   261,  -913,  -913,   437,
     273,  -913,  -913,  -913,  -913,  -913,   205,  -913,  -913,  1052,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -543,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,   265,   537,  -758,   242,  -913,  -807,  -913,  -904,
     201,  -912,  -448,  -563,  -913,  -913,  -913,   476,   835,  -204,
    -161,  -347,   725,   -86,  -365,  -442,  -557,  -485,  -552,  -480,
    -507,  -156,   -62,  -913,   509,  -913,  -913,   757,  -913,  -913,
     950,  -154,   716,  -913,  -445,  -913,  -913,  -913,  -913,  -913,
    -160,  -913
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -223
static const yytype_int16 yytable[] =
{
     221,   248,   350,   231,   697,   461,   574,   620,   277,   278,
     218,   618,   588,   488,   600,   307,   308,   295,   310,   312,
     313,   591,   592,    16,   320,   321,   322,   262,   251,   328,
     499,   487,   331,   213,   900,   532,   339,   329,   330,   213,
     332,   333,   580,   634,   340,   271,   342,   635,   636,   347,
     353,   622,   302,   334,   316,  1009,   564,   335,   336,   239,
     240,   379,   584,   996,   730,   516,   381,   728,   519,   777,
     532,   971,   239,   240,   452,   296,   790,   213,   334,   508,
     509,   355,   335,   336,   532,   754,   482,   301,   343,     7,
     355,   879,   227,   959,   236,   314,   315,  1033,   297,   298,
     263,   252,   767,   228,   229,   733,     8,   637,   638,   639,
     726,   640,   972,   641,   253,   239,   240,  1060,   348,    11,
     262,   417,   418,   795,   420,  1045,   213,   327,   360,   361,
     362,   363,   364,  1054,     1,     2,   380,   642,   643,   644,
     581,   645,   646,   647,   648,   997,   649,   998,   467,     9,
     337,   262,   356,   903,   274,   904,   251,    13,   366,   344,
     382,   272,   275,   880,   650,   651,   565,    14,   810,   244,
     317,   243,   585,   652,   653,   654,   655,   656,   657,   778,
     237,   999,   244,  1029,   990,   303,   791,   576,  1061,    15,
     545,   658,   577,   263,   415,   416,   861,   546,   239,   240,
     609,   157,   659,   245,   246,    72,   773,   610,   213,   206,
     264,   254,   611,   500,   243,   214,   265,   255,   590,   612,
     751,   214,   489,   159,   263,   244,   370,   227,   371,   252,
     215,   216,   372,   373,   374,   325,   215,   216,   228,   229,
     256,   257,   253,   505,   721,   571,   245,   246,   613,   520,
     521,   910,   713,   590,   909,   614,   812,   621,   522,   214,
     530,   531,   823,   600,   722,   529,   217,   590,   214,   539,
     797,   212,   217,   542,   215,   216,   217,   559,   535,   536,
     239,   240,   563,   215,   216,   251,   568,   569,   570,   466,
     495,   575,   535,   536,   302,   472,   473,   908,   474,   475,
     476,   477,   478,   264,   598,   160,   845,   619,   377,   265,
     217,   398,   599,   594,   227,   356,   490,   239,   240,   217,
     527,   793,   375,   215,   216,   228,   229,   607,   496,   764,
     266,   267,   733,   220,   264,   846,   847,   507,   848,   254,
     265,   615,   514,   617,   770,   255,   518,   849,   616,   804,
     806,   771,   156,   808,   661,   850,   805,   807,   252,   217,
     809,   266,   267,   913,   670,   671,   689,   544,   256,   257,
     796,   253,   553,   851,   554,   555,   334,   376,   358,   243,
     335,   336,   161,   239,   240,   162,   937,   532,   163,   939,
     377,   533,   799,   800,   378,   164,   227,   241,   242,   239,
     240,   359,   165,   241,   242,   215,   216,   228,   229,  1003,
    1004,   245,   246,   297,   298,   166,   243,   366,   167,   222,
     227,   168,   360,   361,   362,   363,   364,   311,   595,   169,
     225,   228,   229,   170,   857,   250,   171,   858,   172,   974,
     173,   217,   174,  1016,   306,   175,   -15,   176,   245,   246,
     360,   361,   362,   363,   364,   678,   398,   177,   962,   724,
     239,   240,   907,   178,   179,  1001,   180,   226,   254,   181,
     182,  1002,   183,  1049,   255,   280,   281,   282,   283,   284,
     184,   285,   243,   286,   287,   288,   289,   290,   360,   361,
     362,   363,   364,   291,   362,   363,   364,   782,   243,   362,
     363,   364,   309,  1027,   360,   361,   362,   363,   364,   244,
     232,   233,   234,   235,   245,   246,   928,   929,   930,   755,
     185,   186,   978,   758,   292,   979,   761,   762,   239,   240,
     245,   246,   187,   188,   715,   815,   816,   817,   818,   189,
     820,   821,   822,   190,   786,   191,   192,   784,   193,   194,
     859,   723,   280,   281,   282,   283,   284,  1051,   285,   243,
     286,   287,   288,   289,   290,   195,   196,   884,   197,   351,
     534,   198,   199,   200,   885,   201,   360,   361,   362,   363,
     364,   203,   527,   239,   240,   204,   886,   819,   205,   211,
     223,   245,   246,   844,   471,   535,   536,   325,   352,   387,
     354,   292,   604,   887,   360,   361,   362,   363,   364,   357,
     390,   391,   605,   756,   757,   392,   393,   760,   394,   395,
     280,   281,   282,   283,   284,   397,   285,   243,   286,   287,
     288,   289,   290,   398,   888,   889,   890,   891,   291,   892,
     893,   894,   895,   896,   400,   360,   361,   362,   363,   364,
     399,   360,   361,   362,   363,   364,   401,   403,   404,   245,
     246,   360,   361,   362,   363,   364,   407,   408,  -222,   292,
     410,   411,   914,   412,   943,   280,   281,   282,   283,   284,
     413,   285,   243,   286,   287,   288,   289,   290,   414,   419,
     427,   855,   421,   291,   360,   926,   362,   363,   364,   422,
     423,   865,  1052,   425,   428,   430,  1056,   429,   431,   432,
     433,   968,   969,   970,   245,   246,   434,   435,  1065,   436,
     437,    16,   438,   439,   292,   440,   441,   442,   443,    17,
     444,   446,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,   447,   448,   449,    29,    30,    31,    32,
      33,   624,    34,   450,   451,   453,   454,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,   455,    46,
      47,    48,    49,    50,    51,    52,   456,   457,   458,    53,
      54,   459,    55,   460,   463,   464,   468,   465,    56,   479,
     470,   480,   481,  1005,   491,   498,   492,   924,   493,   630,
     360,   361,   362,   363,   364,   633,   502,   503,    57,   504,
     506,   511,   512,   523,   513,   515,   665,   517,   524,   525,
     526,    58,   540,   543,   541,   548,   549,    59,   550,   551,
      60,    61,   684,    62,   558,    63,   552,   560,    64,    65,
     561,   862,    66,    67,   562,    68,   567,    69,   360,   361,
     362,   363,   364,   572,   360,   361,   362,   363,   364,   239,
     240,   579,   583,   582,   980,   360,   361,   362,   363,   364,
     587,  -171,   239,   240,   597,   608,   625,    70,   623,    71,
     626,   360,   361,   362,   363,   364,   628,   629,   674,  1058,
     360,   361,   362,   363,   364,   632,   664,   668,   669,   679,
     672,   673,  1067,    72,   675,   683,   676,   677,   687,    73,
      74,   680,   681,   682,   685,  1035,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,   279,   686,   688,  1048,   689,   690,
     693,   691,   692,   694,   695,   696,   698,   699,   701,   702,
     703,   280,   281,   282,   283,   284,   704,   285,   243,   286,
     287,   288,   289,   290,   280,   281,   282,   283,   284,   291,
     285,   243,   286,   287,   288,   289,   290,   708,   824,   825,
     705,   707,   291,   709,   710,   711,   678,   717,   712,   714,
     245,   246,   718,   720,   719,   296,   731,   732,   733,   527,
     292,   736,   735,   245,   246,   737,   734,   752,   738,    74,
     826,   827,   739,   292,   828,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,   740,   741,   766,   742,   829,   743,   744,
     830,   831,   832,   833,   834,   835,   836,   837,   838,   839,
     840,   207,    76,   749,    78,   745,    80,    75,    82,    77,
      84,    79,    86,    81,    88,    83,    90,    85,    92,    87,
     746,    89,   747,    91,   750,   748,   753,   759,   768,   764,
     763,   767,   769,   774,   775,   781,   776,   783,   780,   785,
     787,   789,   227,   811,   788,   798,   813,   853,   856,   860,
     814,   863,   864,   228,   229,   866,   867,   869,   870,   872,
     871,   873,   875,   874,   877,   898,   878,   882,   883,   899,
     915,   901,   902,   912,   922,   923,   911,   916,   917,   918,
     919,   920,   921,   927,   933,   925,   935,   936,   938,   940,
     941,   944,   945,   946,   947,   948,   949,   950,   951,   952,
     953,   954,   955,   956,   958,   961,   957,   963,   966,   967,
     975,   981,   964,   976,   977,   982,   984,   983,   986,   987,
     988,   991,   992,   993,   995,  1007,  1008,  1011,  1013,  1015,
    1018,  1020,  1022,  1025,  1023,  1026,  1014,    10,  1030,  1066,
    1031,  1036,  1028,  1032,  1039,  1038,  1040,  1044,  1042,  1046,
    1050,  1055,  1057,  1063,  1068,   389,   208,   494,   202,   965,
     942,   960,   631,   341,   989,   876,   349,   261,   573,  1037,
    1006,  1010,   765,  1041,  1019,   843,   497,   802,   627,     0,
       0,   662,     0,     0,     0,     0,   606,     0,     0,   424
};

static const yytype_int16 yycheck[] =
{
      97,   162,   206,   159,   567,   346,   451,   492,   169,   170,
      72,   491,   460,   378,   469,   176,   177,   171,   179,   180,
     180,   463,   464,     6,   184,   185,   186,     3,     3,   190,
       3,   378,   193,     4,   792,     3,   197,   191,   192,     4,
     194,   195,    61,     4,   198,    76,   200,     8,     9,   203,
     211,   493,    53,     4,    76,   959,    76,     8,     9,    76,
      77,   222,    76,     4,   621,   406,   222,   619,   409,    76,
       3,   151,    76,    77,   184,   186,    76,     4,     4,    81,
      82,     8,     8,     9,     3,   672,     4,   173,     5,   185,
       8,     4,   192,   900,     4,   181,   182,  1009,   209,   210,
      76,    76,   184,   203,   204,   187,   185,    68,    69,    70,
     617,    72,   192,    74,    89,    76,    77,     5,   204,   108,
       3,   282,   283,   710,   285,  1029,     4,   189,   238,   239,
     240,   241,   242,  1045,    15,    16,   222,    98,    99,   100,
     159,   102,   103,   104,   105,    86,   107,    88,   352,     0,
      76,     3,   214,   131,    68,   133,     3,   192,   220,    76,
     222,   192,    76,    76,   125,   126,   186,   192,   731,   186,
     192,   175,   186,   134,   135,   136,   137,   138,   139,   186,
      90,   122,   186,   990,   942,   186,   186,    71,    76,   185,
      69,   152,    76,    76,   280,   281,   759,    76,    76,    77,
     184,   109,   163,   207,   208,   188,   691,   191,     4,   188,
     186,   186,   184,   186,   175,   186,   192,   192,   186,   191,
     668,   186,   378,   185,    76,   186,    22,   192,    24,    76,
     201,   202,    28,    29,    30,   186,   201,   202,   203,   204,
     215,   216,    89,   397,   609,   449,   207,   208,   184,   410,
     411,   808,   593,   186,   806,   191,   736,   190,   412,   186,
     421,   422,   747,   718,   611,   419,   237,   186,   186,   423,
     712,   194,   237,   427,   201,   202,   237,   438,   211,   212,
      76,    77,   442,   201,   202,     3,   446,   447,   448,   351,
     387,   452,   211,   212,    53,   357,   358,   804,   360,   361,
     362,   363,   364,   186,    76,   185,    72,   190,   186,   192,
     237,     4,   468,   129,   192,   377,   378,    76,    77,   237,
      75,    76,   118,   201,   202,   203,   204,   481,   390,   184,
     213,   214,   187,   186,   186,   101,   102,   399,   104,   186,
     192,   184,   404,   190,    76,   192,   408,   113,   191,   184,
     184,    83,   184,   184,   515,   121,   191,   191,    76,   237,
     191,   213,   214,   185,   525,   526,   188,   429,   215,   216,
     711,    89,    76,   139,    78,    79,     4,   173,   166,   175,
       8,     9,   185,    76,    77,   185,   871,     3,   185,   874,
     186,     7,   197,   198,   190,   185,   192,    80,    81,    76,
      77,   189,   185,    80,    81,   201,   202,   203,   204,   952,
     953,   207,   208,   209,   210,   185,   175,   479,   185,   243,
     192,   185,   238,   239,   240,   241,   242,   186,   244,   185,
       3,   203,   204,   185,   184,    94,   185,   187,   185,   919,
     185,   237,   185,   184,   192,   185,   187,   185,   207,   208,
     238,   239,   240,   241,   242,     3,     4,   185,   903,   615,
      76,    77,   803,   185,   185,   950,   185,   184,   186,   185,
     185,   951,   185,  1036,   192,   168,   169,   170,   171,   172,
     185,   174,   175,   176,   177,   178,   179,   180,   238,   239,
     240,   241,   242,   186,   240,   241,   242,   701,   175,   240,
     241,   242,    87,   988,   238,   239,   240,   241,   242,   186,
      10,    11,    12,    13,   207,   208,    39,    40,    41,   673,
     185,   185,   184,   677,   217,   187,   680,   681,    76,    77,
     207,   208,   185,   185,   596,   739,   740,   741,   742,   185,
     744,   745,   746,   185,   705,   185,   185,   703,   185,   185,
     184,   613,   168,   169,   170,   171,   172,  1042,   174,   175,
     176,   177,   178,   179,   180,   185,   185,    85,   185,   243,
     186,   185,   185,   185,    92,   185,   238,   239,   240,   241,
     242,   185,    75,    76,    77,   185,   104,   743,   185,   185,
     185,   207,   208,   749,   187,   211,   212,   186,   188,   190,
     186,   217,   189,   121,   238,   239,   240,   241,   242,   188,
     188,   244,   189,   675,   676,   184,   184,   679,   184,   183,
     168,   169,   170,   171,   172,   186,   174,   175,   176,   177,
     178,   179,   180,     4,   152,   153,   154,   155,   186,   157,
     158,   159,   160,   161,   184,   238,   239,   240,   241,   242,
     188,   238,   239,   240,   241,   242,   184,     3,   188,   207,
     208,   238,   239,   240,   241,   242,     3,   188,   190,   217,
     184,   184,   828,   184,   878,   168,   169,   170,   171,   172,
     184,   174,   175,   176,   177,   178,   179,   180,   184,   186,
     246,   753,   186,   186,   238,   239,   240,   241,   242,   186,
     186,   763,  1043,   186,     4,   184,  1047,   188,   184,   186,
       4,   915,   916,   917,   207,   208,   184,   184,  1059,   184,
     184,     6,   184,   184,   217,     4,   184,   184,   184,    14,
       6,   184,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,   184,   184,   184,    31,    32,    33,    34,
      35,   189,    37,     4,   184,   184,   184,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,   184,    54,
      55,    56,    57,    58,    59,    60,   184,   184,   184,    64,
      65,   184,    67,   184,   184,   184,   184,   189,    73,   184,
     187,   187,   185,   954,   185,     3,   185,   859,   185,   189,
     238,   239,   240,   241,   242,   189,     3,   184,    93,   189,
     184,     5,   193,    75,   184,   184,   189,   184,    75,   184,
     184,   106,   181,   184,   187,    36,     4,   112,   184,    36,
     115,   116,   189,   118,   152,   120,    38,    84,   123,   124,
     184,   189,   127,   128,    84,   130,   184,   132,   238,   239,
     240,   241,   242,   184,   238,   239,   240,   241,   242,    76,
      77,    88,    94,   156,   926,   238,   239,   240,   241,   242,
       4,   117,    76,    77,   189,   184,     3,   162,   191,   164,
     184,   238,   239,   240,   241,   242,   187,     4,   187,  1050,
     238,   239,   240,   241,   242,   192,   192,   184,   184,   188,
     245,   184,  1063,   188,   184,     4,   184,   184,     4,   194,
     195,   184,   184,   184,   184,  1012,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   151,   184,   184,  1034,   188,   184,
       4,   185,   184,   184,     4,   184,   184,   184,   184,    75,
     184,   168,   169,   170,   171,   172,   184,   174,   175,   176,
     177,   178,   179,   180,   168,   169,   170,   171,   172,   186,
     174,   175,   176,   177,   178,   179,   180,     4,    78,    79,
     185,   184,   186,   184,   184,   117,     3,   185,   184,   244,
     207,   208,   184,     4,   187,   186,   184,     3,   187,    75,
     217,   185,   187,   207,   208,   185,   193,     5,   185,   195,
     110,   111,   185,   217,   114,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   185,   185,     4,   185,   137,   185,   185,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   195,   202,   184,   204,   185,   206,   201,   208,   203,
     210,   205,   212,   207,   214,   209,   216,   211,   218,   213,
     185,   215,   185,   217,   184,   187,   184,   184,   152,   184,
     186,   184,     4,   152,    84,    84,   184,   184,   192,    62,
     184,   184,   192,   184,    91,   190,    97,   184,   187,   187,
      96,   187,   187,   203,   204,     4,   184,   184,     4,   189,
     185,   184,     4,   185,   184,     4,   184,   184,   184,     4,
     185,   184,   184,     5,     3,    39,   187,   185,   185,   185,
     185,   185,   185,   184,    95,   187,     4,   184,   152,   187,
     184,   187,    63,    86,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   184,     7,   187,   185,   184,    76,
       4,     4,   191,   122,   187,   184,    92,   185,   184,     4,
     185,   184,   184,    91,     4,     4,     4,   185,     5,    39,
     187,    94,   185,     4,   152,   187,   245,     5,    86,  1062,
      85,   184,   187,   187,   182,   187,   187,   187,   185,   184,
     184,    85,   184,   184,   187,   226,    67,   386,    61,   906,
     877,   901,   510,   199,   941,   778,   205,   165,   451,  1014,
     955,   960,   685,  1022,   982,   749,   391,   718,   503,    -1,
      -1,   515,    -1,    -1,    -1,    -1,   479,    -1,    -1,   289
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   248,   249,   250,   251,   185,   185,     0,
     249,   108,   259,   192,   192,   185,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    31,
      32,    33,    34,    35,    37,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    54,    55,    56,    57,
      58,    59,    60,    64,    65,    67,    73,    93,   106,   112,
     115,   116,   118,   120,   123,   124,   127,   128,   130,   132,
     162,   164,   188,   194,   195,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   252,   253,   254,   261,   263,   264,   265,
     266,   268,   269,   277,   280,   281,   283,   284,   291,   293,
     294,   296,   298,   300,   304,   305,   306,   309,   311,   318,
     325,   328,   334,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   350,   351,   352,   353,   357,   358,   360,
     361,   362,   363,   364,   365,   366,   367,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   184,   109,   260,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   283,   185,   185,   185,   188,   195,   265,   267,
     285,   185,   194,     4,   186,   201,   202,   237,   409,   419,
     186,   261,   243,   185,   295,     3,   184,   192,   203,   204,
     395,   408,    10,    11,    12,    13,     4,    90,   420,    76,
      77,    80,    81,   175,   186,   207,   208,   326,   397,   398,
      94,     3,    76,    89,   186,   192,   215,   216,   336,   406,
     407,   336,     3,    76,   186,   192,   213,   214,   330,   404,
     405,    76,   192,   379,    68,    76,   389,   397,   397,   151,
     168,   169,   170,   171,   172,   174,   176,   177,   178,   179,
     180,   186,   217,   397,   417,   418,   186,   209,   210,   400,
     401,   400,    53,   186,   427,   428,   192,   397,   397,    87,
     397,   186,   397,   427,   400,   400,    76,   192,   308,   380,
     427,   427,   427,   396,   409,   186,   307,   409,   397,   418,
     418,   397,   418,   418,     4,     8,     9,    76,   421,   397,
     418,   308,   418,     5,    76,   319,   301,   418,   400,   264,
     396,   243,   188,   397,   186,     8,   409,   188,   166,   189,
     238,   239,   240,   241,   242,   408,   409,   414,   415,   416,
      22,    24,    28,    29,    30,   118,   173,   186,   190,   397,
     400,   408,   409,   423,   424,   425,   299,   190,   262,   260,
     188,   244,   184,   184,   184,   183,   399,   186,     4,   188,
     184,   184,   313,     3,   188,   337,   335,     3,   188,   329,
     184,   184,   184,   184,   184,   400,   400,   397,   397,   186,
     397,   186,   186,   186,   417,   186,   418,   246,     4,   188,
     184,   184,   186,     4,   184,   184,   184,   184,   184,   184,
       4,   184,   184,   184,     6,   282,   184,   184,   184,   184,
       4,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   262,   302,   184,   184,   189,   409,   396,   184,   278,
     187,   187,   409,   409,   409,   409,   409,   409,   409,   184,
     187,   185,     4,   273,   274,   275,   276,   398,   401,   408,
     409,   185,   185,   185,   263,   261,   409,   395,     3,     3,
     186,   359,     3,   184,   189,   418,   184,   409,    81,    82,
     327,     5,   193,   184,   409,   184,   262,   184,   409,   262,
     397,   397,   418,    75,    75,   184,   184,    75,   321,   418,
     397,   397,     3,     7,   186,   211,   212,   402,   403,   418,
     181,   187,   418,   184,   409,    69,    76,   390,    36,     4,
     184,    36,    38,    76,    78,    79,   391,   394,   152,   397,
      84,   184,    84,   427,    76,   186,   314,   184,   427,   427,
     427,   396,   184,   307,   421,   397,    71,    76,   426,    88,
      61,   159,   156,    94,    76,   186,   315,     4,   389,   303,
     186,   402,   402,   297,   129,   244,   292,   189,    76,   408,
     266,   411,   412,   413,   189,   189,   414,   418,   184,   184,
     191,   184,   191,   184,   191,   184,   191,   190,   406,   190,
     404,   190,   402,   191,   189,     3,   184,   399,   187,     4,
     189,   313,   192,   189,     4,     8,     9,    68,    69,    70,
      72,    74,    98,    99,   100,   102,   103,   104,   105,   107,
     125,   126,   134,   135,   136,   137,   138,   139,   152,   163,
     338,   397,   419,   422,   192,   189,   331,   323,   184,   184,
     397,   397,   245,   184,   187,   184,   184,   184,     3,   188,
     184,   184,   184,     4,   189,   184,   184,     4,   184,   188,
     184,   185,   184,     4,   184,     4,   184,   390,   184,   184,
     349,   184,    75,   184,   184,   185,   354,   184,     4,   184,
     184,   117,   184,   262,   244,   409,   286,   185,   184,   187,
       4,   401,   398,   409,   408,   270,   407,   271,   405,   272,
     403,   184,     3,   187,   193,   187,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   187,   184,
     184,   389,     5,   184,   321,   418,   409,   409,   418,   184,
     409,   418,   418,   186,   184,   380,     4,   184,   152,     4,
      76,    83,   393,   404,   152,    84,   184,    76,   186,   316,
     192,    84,   396,   184,   408,    62,   397,   184,    91,   184,
      76,   186,   317,    76,   320,   321,   262,   402,   190,   197,
     198,   410,   411,   279,   184,   191,   184,   191,   184,   191,
     390,   184,   406,    97,    96,   396,   396,   396,   396,   408,
     396,   396,   396,   404,    78,    79,   110,   111,   114,   137,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   332,   392,   394,   408,    72,   101,   102,   104,   113,
     121,   139,   324,   184,   257,   409,   187,   184,   187,   184,
     187,   390,   189,   187,   187,   409,     4,   184,   381,   184,
       4,   185,   189,   184,   185,     4,   326,   184,   184,     4,
      76,   322,   184,   184,    85,    92,   104,   121,   152,   153,
     154,   155,   157,   158,   159,   160,   161,   355,     4,     4,
     381,   184,   184,   131,   133,   287,   288,   262,   407,   405,
     403,   187,     5,   185,   408,   185,   185,   185,   185,   185,
     185,   185,     3,    39,   409,   187,   239,   184,    39,    40,
      41,   382,   383,    95,   384,     4,   184,   404,   152,   404,
     187,   184,   317,   396,   187,    63,    86,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   187,   184,   384,
     322,     7,   421,   185,   191,   287,   184,    76,   396,   396,
     396,   151,   192,   333,   406,     4,   122,   187,   184,   187,
     409,     4,   184,   185,    92,   386,   184,     4,   185,   327,
     381,   184,   184,    91,   387,     4,     4,    86,    88,   122,
     356,   404,   406,   359,   359,   397,   379,     4,     4,   386,
     323,   185,   290,     5,   245,    39,   184,   255,   187,   382,
      94,   385,   185,   152,   388,     4,   187,   404,   187,   384,
      86,    85,   187,   388,   289,   261,   184,   333,   187,   182,
     187,   387,   185,   368,   187,   386,   184,   310,   261,   390,
     184,   404,   262,   369,   388,    85,   262,   184,   397,   312,
       5,    76,   258,   184,   256,   262,   257,   397,   187
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

  case 109:

/* Line 1455 of yacc.c  */
#line 576 "lev_comp.y"
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

  case 110:

/* Line 1455 of yacc.c  */
#line 592 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 598 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 604 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 610 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 616 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 622 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 628 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 634 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 640 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 650 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 660 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 670 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 680 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 690 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 700 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 712 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 717 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 724 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 729 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 736 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 741 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 748 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 752 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 758 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 763 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 770 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 774 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 780 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 784 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 790 "lev_comp.y"
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

  case 140:

/* Line 1455 of yacc.c  */
#line 812 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 816 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 827 "lev_comp.y"
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

  case 143:

/* Line 1455 of yacc.c  */
#line 882 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 888 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 892 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 898 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 905 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 909 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 917 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 921 "lev_comp.y"
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

  case 151:

/* Line 1455 of yacc.c  */
#line 944 "lev_comp.y"
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

  case 154:

/* Line 1455 of yacc.c  */
#line 989 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 999 "lev_comp.y"
    {
		  ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1002 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1012 "lev_comp.y"
    {
		  ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1017 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1031 "lev_comp.y"
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

  case 162:

/* Line 1455 of yacc.c  */
#line 1068 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1073 "lev_comp.y"
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

  case 164:

/* Line 1455 of yacc.c  */
#line 1102 "lev_comp.y"
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

  case 165:

/* Line 1455 of yacc.c  */
#line 1116 "lev_comp.y"
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

  case 166:

/* Line 1455 of yacc.c  */
#line 1132 "lev_comp.y"
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

  case 167:

/* Line 1455 of yacc.c  */
#line 1152 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1164 "lev_comp.y"
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

  case 169:

/* Line 1455 of yacc.c  */
#line 1184 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1190 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1200 "lev_comp.y"
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

  case 172:

/* Line 1455 of yacc.c  */
#line 1218 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1228 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1234 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1239 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1244 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1251 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1258 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1267 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1275 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1286 "lev_comp.y"
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

  case 182:

/* Line 1455 of yacc.c  */
#line 1302 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1309 "lev_comp.y"
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

  case 184:

/* Line 1455 of yacc.c  */
#line 1323 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1330 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1334 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1340 "lev_comp.y"
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

  case 188:

/* Line 1455 of yacc.c  */
#line 1350 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1356 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1365 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1371 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1376 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1382 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1387 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1393 "lev_comp.y"
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

  case 196:

/* Line 1455 of yacc.c  */
#line 1404 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1418 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1422 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1432 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1439 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1447 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1452 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1457 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1462 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1467 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1472 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1477 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1485 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1492 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1500 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1516 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1520 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1526 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1534 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1540 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1548 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1556 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1561 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1567 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1573 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1579 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1585 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1590 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1595 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1600 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1605 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1610 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1615 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1620 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1625 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1630 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1635 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1641 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1648 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1657 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1661 "lev_comp.y"
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

  case 247:

/* Line 1455 of yacc.c  */
#line 1674 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1682 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1692 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1700 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1709 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1716 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1724 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1730 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1735 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1740 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1745 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1750 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1755 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1760 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1765 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1770 "lev_comp.y"
    {
		      if ((yyvsp[(1) - (1)].i) == D_LOCKED) {
			  add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_LOCKED));
			  (yyval.i) = 0x0100;
		      } else if ((yyvsp[(1) - (1)].i) == D_BROKEN) {
			  add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BROKEN));
			  (yyval.i) = 0x0200;
		      } else
			  lc_error("DOOR state can only be locked or broken.");
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1781 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1787 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1792 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1797 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1802 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1807 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1812 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1817 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1822 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1827 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1832 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1837 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1842 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x400000;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1847 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x800000;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1852 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x1000000;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1859 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1865 "lev_comp.y"
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

  case 280:

/* Line 1455 of yacc.c  */
#line 1894 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1899 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1907 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1913 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1917 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1925 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1932 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1939 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1949 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1959 "lev_comp.y"
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

  case 290:

/* Line 1455 of yacc.c  */
#line 1974 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1984 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1988 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1994 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2000 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2006 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2010 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2016 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2022 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2029 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2037 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2043 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2048 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2053 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2058 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2063 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2069 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2075 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2080 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2085 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2090 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_CATEGORY));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2095 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2100 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2110 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2116 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2122 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2127 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2134 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2141 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2147 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2153 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2159 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2164 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2171 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2177 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2187 "lev_comp.y"
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

  case 330:

/* Line 1455 of yacc.c  */
#line 2206 "lev_comp.y"
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

  case 331:

/* Line 1455 of yacc.c  */
#line 2218 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2222 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2228 "lev_comp.y"
    {
		      add_opvars(splev, "Miio",
				 VA_PASS4(-1, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2233 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2240 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2244 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2250 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2256 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2262 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2266 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2272 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2276 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2281 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2288 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2294 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2301 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2305 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2312 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2323 "lev_comp.y"
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

  case 352:

/* Line 1455 of yacc.c  */
#line 2336 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2340 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2346 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2350 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2357 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2361 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2365 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2371 "lev_comp.y"
    {
			(yyval.i) = ROOM;
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2375 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2381 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2387 "lev_comp.y"
    {
			(yyval.i) = 0;
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2391 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2397 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2403 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2408 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2425 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2433 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2443 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2452 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2457 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2464 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2475 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2481 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2485 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2489 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2496 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2506 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2513 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2517 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2523 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2527 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2535 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2539 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2546 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2556 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2569 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2573 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2580 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2590 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2599 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2610 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2614 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2621 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2631 "lev_comp.y"
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

  case 406:

/* Line 1455 of yacc.c  */
#line 2642 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2651 "lev_comp.y"
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

  case 408:

/* Line 1455 of yacc.c  */
#line 2661 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2667 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2671 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2678 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2688 "lev_comp.y"
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

  case 413:

/* Line 1455 of yacc.c  */
#line 2699 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2708 "lev_comp.y"
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

  case 415:

/* Line 1455 of yacc.c  */
#line 2718 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2722 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2728 "lev_comp.y"
    { ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2730 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2736 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2740 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2744 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2748 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2756 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2765 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2769 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2773 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2777 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2781 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2784 "lev_comp.y"
    { ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2788 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2795 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2804 "lev_comp.y"
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

  case 437:

/* Line 1455 of yacc.c  */
#line 2845 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2849 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2856 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2863 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2873 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2877 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2885 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2889 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2893 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2897 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2901 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2906 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2910 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2914 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2919 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2924 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2929 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2933 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2938 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2943 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2947 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2951 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2956 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2960 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2967 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2973 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2977 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2983 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2999 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 3003 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 3007 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3011 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3033 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3037 "lev_comp.y"
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

  case 484:

/* Line 1455 of yacc.c  */
#line 3063 "lev_comp.y"
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
#line 6917 "lev_comp.tab.c"
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
#line 3091 "lev_comp.y"


/*lev_comp.y*/

