
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

/* NetHack 3.6  lev_comp.y	$NHDT-Date: 1543371691 2018/11/28 02:21:31 $  $NHDT-Branch: NetHack-3.6.2-beta01 $:$NHDT-Revision: 1.22 $ */
/*      Copyright (c) 1989 by Jean-Christophe Collet */
/* NetHack may be freely redistributed.  See license for details. */

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
     DIRECTION = 329,
     RANDOM_TYPE = 330,
     RANDOM_TYPE_BRACKET = 331,
     A_REGISTER = 332,
     ALIGNMENT = 333,
     LEFT_OR_RIGHT = 334,
     CENTER = 335,
     TOP_OR_BOT = 336,
     ALTAR_TYPE = 337,
     UP_OR_DOWN = 338,
     ACTIVE_OR_INACTIVE = 339,
     MODRON_PORTAL_TYPE = 340,
     NPC_TYPE = 341,
     FOUNTAIN_TYPE = 342,
     SPECIAL_OBJECT_TYPE = 343,
     CMAP_TYPE = 344,
     FLOOR_TYPE = 345,
     FLOOR_TYPE_ID = 346,
     FLOOR_ID = 347,
     FLOOR_MAIN_TYPE = 348,
     FLOOR_MAIN_TYPE_ID = 349,
     ELEMENTAL_ENCHANTMENT_TYPE = 350,
     EXCEPTIONALITY_TYPE = 351,
     EXCEPTIONALITY_ID = 352,
     ELEMENTAL_ENCHANTMENT_ID = 353,
     ENCHANTMENT_ID = 354,
     SECRET_DOOR_ID = 355,
     USES_UP_KEY_ID = 356,
     CHARGES_ID = 357,
     SPECIAL_QUALITY_ID = 358,
     SPEFLAGS_ID = 359,
     SUBROOM_ID = 360,
     NAME_ID = 361,
     FLAGS_ID = 362,
     FLAG_TYPE = 363,
     MON_ATTITUDE = 364,
     MON_ALERTNESS = 365,
     SUBTYPE_ID = 366,
     NON_PASSDOOR_ID = 367,
     MON_APPEARANCE = 368,
     ROOMDOOR_ID = 369,
     IF_ID = 370,
     ELSE_ID = 371,
     TERRAIN_ID = 372,
     HORIZ_OR_VERT = 373,
     REPLACE_TERRAIN_ID = 374,
     LOCATION_SUBTYPE_ID = 375,
     DOOR_SUBTYPE = 376,
     EXIT_ID = 377,
     SHUFFLE_ID = 378,
     QUANTITY_ID = 379,
     BURIED_ID = 380,
     LOOP_ID = 381,
     FOR_ID = 382,
     TO_ID = 383,
     SWITCH_ID = 384,
     CASE_ID = 385,
     BREAK_ID = 386,
     DEFAULT_ID = 387,
     ERODED_ID = 388,
     TRAPPED_STATE = 389,
     RECHARGED_ID = 390,
     INVIS_ID = 391,
     GREASED_ID = 392,
     INDESTRUCTIBLE_ID = 393,
     FEMALE_ID = 394,
     WAITFORU_ID = 395,
     CANCELLED_ID = 396,
     REVIVED_ID = 397,
     AVENGE_ID = 398,
     FLEEING_ID = 399,
     BLINDED_ID = 400,
     PARALYZED_ID = 401,
     STUNNED_ID = 402,
     CONFUSED_ID = 403,
     SEENTRAPS_ID = 404,
     ALL_ID = 405,
     MONTYPE_ID = 406,
     OBJTYPE_ID = 407,
     TERTYPE_ID = 408,
     TERTYPE2_ID = 409,
     LEVER_EFFECT_TYPE = 410,
     SWITCHABLE_ID = 411,
     CONTINUOUSLY_USABLE_ID = 412,
     TARGET_ID = 413,
     TRAPTYPE_ID = 414,
     EFFECT_FLAG_ID = 415,
     GRAVE_ID = 416,
     ERODEPROOF_ID = 417,
     FUNCTION_ID = 418,
     MSG_OUTPUT_TYPE = 419,
     COMPARE_TYPE = 420,
     UNKNOWN_TYPE = 421,
     rect_ID = 422,
     fillrect_ID = 423,
     line_ID = 424,
     randline_ID = 425,
     grow_ID = 426,
     selection_ID = 427,
     flood_ID = 428,
     rndcoord_ID = 429,
     circle_ID = 430,
     ellipse_ID = 431,
     filter_ID = 432,
     complement_ID = 433,
     gradient_ID = 434,
     GRADIENT_TYPE = 435,
     LIMITED = 436,
     HUMIDITY_TYPE = 437,
     STRING = 438,
     MAP_ID = 439,
     NQSTRING = 440,
     VARSTRING = 441,
     CFUNC = 442,
     CFUNC_INT = 443,
     CFUNC_STR = 444,
     CFUNC_COORD = 445,
     CFUNC_REGION = 446,
     VARSTRING_INT = 447,
     VARSTRING_INT_ARRAY = 448,
     VARSTRING_STRING = 449,
     VARSTRING_STRING_ARRAY = 450,
     VARSTRING_VAR = 451,
     VARSTRING_VAR_ARRAY = 452,
     VARSTRING_COORD = 453,
     VARSTRING_COORD_ARRAY = 454,
     VARSTRING_REGION = 455,
     VARSTRING_REGION_ARRAY = 456,
     VARSTRING_MAPCHAR = 457,
     VARSTRING_MAPCHAR_ARRAY = 458,
     VARSTRING_MONST = 459,
     VARSTRING_MONST_ARRAY = 460,
     VARSTRING_OBJ = 461,
     VARSTRING_OBJ_ARRAY = 462,
     VARSTRING_SEL = 463,
     VARSTRING_SEL_ARRAY = 464,
     METHOD_INT = 465,
     METHOD_INT_ARRAY = 466,
     METHOD_STRING = 467,
     METHOD_STRING_ARRAY = 468,
     METHOD_VAR = 469,
     METHOD_VAR_ARRAY = 470,
     METHOD_COORD = 471,
     METHOD_COORD_ARRAY = 472,
     METHOD_REGION = 473,
     METHOD_REGION_ARRAY = 474,
     METHOD_MAPCHAR = 475,
     METHOD_MAPCHAR_ARRAY = 476,
     METHOD_MONST = 477,
     METHOD_MONST_ARRAY = 478,
     METHOD_OBJ = 479,
     METHOD_OBJ_ARRAY = 480,
     METHOD_SEL = 481,
     METHOD_SEL_ARRAY = 482,
     DICE = 483
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
#line 526 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 538 "lev_comp.tab.c"

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
#define YYLAST   1197

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  246
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  181
/* YYNRULES -- Number of rules.  */
#define YYNRULES  477
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1049

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   483

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   241,   245,     2,
     185,   186,   239,   237,   183,   238,   243,   240,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   184,     2,
       2,   242,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   187,     2,   188,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   189,   244,   190,     2,     2,     2,     2,
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
     175,   176,   177,   178,   179,   180,   181,   182,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236
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
     536,   541,   542,   555,   556,   571,   572,   575,   581,   583,
     589,   591,   597,   599,   605,   607,   618,   625,   627,   629,
     631,   633,   635,   639,   641,   643,   644,   648,   652,   656,
     660,   662,   664,   666,   668,   670,   678,   684,   686,   688,
     690,   692,   696,   697,   703,   708,   709,   713,   715,   717,
     719,   721,   724,   726,   728,   730,   732,   734,   738,   742,
     746,   748,   750,   754,   756,   758,   760,   764,   768,   769,
     775,   778,   779,   783,   785,   789,   791,   795,   799,   801,
     803,   807,   809,   811,   813,   817,   819,   821,   823,   827,
     831,   835,   839,   843,   847,   851,   853,   855,   861,   869,
     875,   884,   886,   890,   896,   902,   910,   918,   925,   931,
     932,   935,   941,   945,   957,   971,   978,   979,   983,   985,
     989,   993,   997,  1001,  1003,  1005,  1009,  1013,  1017,  1021,
    1025,  1029,  1031,  1033,  1035,  1037,  1041,  1045,  1047,  1053,
    1063,  1069,  1073,  1077,  1083,  1093,  1099,  1105,  1106,  1119,
    1120,  1122,  1130,  1134,  1142,  1148,  1154,  1160,  1166,  1170,
    1176,  1184,  1194,  1196,  1198,  1200,  1202,  1204,  1205,  1208,
    1210,  1214,  1216,  1218,  1220,  1221,  1225,  1227,  1228,  1232,
    1234,  1236,  1238,  1240,  1242,  1244,  1246,  1248,  1250,  1252,
    1256,  1258,  1260,  1265,  1267,  1269,  1274,  1276,  1278,  1283,
    1285,  1290,  1296,  1298,  1302,  1304,  1308,  1310,  1312,  1317,
    1327,  1329,  1331,  1336,  1338,  1344,  1346,  1348,  1353,  1355,
    1357,  1363,  1365,  1367,  1369,  1374,  1376,  1378,  1384,  1386,
    1388,  1390,  1394,  1396,  1398,  1402,  1404,  1409,  1413,  1417,
    1421,  1425,  1429,  1433,  1435,  1437,  1441,  1443,  1447,  1448,
    1450,  1452,  1454,  1456,  1460,  1461,  1463,  1465,  1468,  1471,
    1476,  1483,  1488,  1495,  1502,  1509,  1516,  1519,  1526,  1535,
    1544,  1555,  1570,  1573,  1575,  1579,  1581,  1585,  1587,  1589,
    1591,  1593,  1595,  1597,  1599,  1601,  1603,  1605,  1607,  1609,
    1611,  1613,  1615,  1617,  1619,  1621,  1623,  1634
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     247,     0,    -1,    -1,   248,    -1,   249,    -1,   249,   248,
      -1,   250,   258,   260,    -1,    16,   184,   191,    -1,    15,
     184,   191,   183,     3,    -1,    17,   184,    11,   183,   358,
      -1,    17,   184,    10,   183,     3,    -1,    17,   184,    13,
      -1,    17,   184,    12,   183,     3,   183,     3,   183,     5,
     183,     5,   183,   388,   183,   257,   256,    -1,    18,   184,
     418,    -1,    21,   184,    93,    -1,    -1,   183,   181,    -1,
      -1,   183,   395,    -1,    -1,   183,     3,    -1,     5,    -1,
      75,    -1,    -1,   107,   184,   259,    -1,   108,   183,   259,
      -1,   108,    -1,    -1,   262,   260,    -1,   189,   260,   190,
      -1,   303,    -1,   251,    -1,   252,    -1,   253,    -1,   372,
      -1,   371,    -1,   369,    -1,   370,    -1,   373,    -1,   374,
      -1,   347,    -1,   305,    -1,   268,    -1,   267,    -1,   361,
      -1,   317,    -1,   339,    -1,   376,    -1,   377,    -1,   349,
      -1,   350,    -1,   351,    -1,   375,    -1,   283,    -1,   293,
      -1,   295,    -1,   299,    -1,   297,    -1,   280,    -1,   290,
      -1,   276,    -1,   279,    -1,   342,    -1,   352,    -1,   324,
      -1,   340,    -1,   327,    -1,   333,    -1,   362,    -1,   357,
      -1,   345,    -1,   304,    -1,   363,    -1,   364,    -1,   365,
      -1,   366,    -1,   310,    -1,   308,    -1,   356,    -1,   360,
      -1,   359,    -1,   343,    -1,   344,    -1,   346,    -1,   338,
      -1,   341,    -1,   201,    -1,   203,    -1,   205,    -1,   207,
      -1,   209,    -1,   211,    -1,   213,    -1,   215,    -1,   217,
      -1,   200,    -1,   202,    -1,   204,    -1,   206,    -1,   208,
      -1,   210,    -1,   212,    -1,   214,    -1,   216,    -1,   263,
      -1,   264,    -1,   194,    -1,   194,    -1,   264,    -1,   123,
     184,   263,    -1,   265,   242,   407,    -1,   265,   242,   172,
     184,   416,    -1,   265,   242,   406,    -1,   265,   242,   423,
     184,   400,    -1,   265,   242,   422,   184,   402,    -1,   265,
     242,   421,   184,   404,    -1,   265,   242,   395,    -1,   265,
     242,   398,    -1,   265,   242,   189,   274,   190,    -1,   265,
     242,   189,   273,   190,    -1,   265,   242,   189,   272,   190,
      -1,   265,   242,   423,   184,   189,   271,   190,    -1,   265,
     242,   422,   184,   189,   270,   190,    -1,   265,   242,   421,
     184,   189,   269,   190,    -1,   265,   242,   189,   275,   190,
      -1,   405,    -1,   269,   183,   405,    -1,   403,    -1,   270,
     183,   403,    -1,   401,    -1,   271,   183,   401,    -1,   399,
      -1,   272,   183,   399,    -1,   396,    -1,   273,   183,   396,
      -1,   407,    -1,   274,   183,   407,    -1,   406,    -1,   275,
     183,   406,    -1,    -1,    -1,   163,   193,   185,   277,   411,
     186,   278,   261,    -1,   193,   185,   414,   186,    -1,   122,
      -1,    -1,     6,    -1,     6,    -1,   187,   407,   165,   407,
     188,    -1,   187,   407,   188,    -1,    -1,    -1,   129,   284,
     187,   394,   188,   285,   189,   286,   190,    -1,    -1,   287,
     286,    -1,    -1,   130,   419,   184,   288,   260,    -1,    -1,
     132,   184,   289,   260,    -1,   131,    -1,   243,   243,    -1,
     128,    -1,   127,   266,   242,   407,   291,   407,    -1,    -1,
     292,   294,   261,    -1,    -1,   126,   187,   394,   188,   296,
     261,    -1,    -1,   282,   184,   298,   262,    -1,    -1,   115,
     282,   300,   301,    -1,   261,    -1,    -1,   261,   302,   116,
     261,    -1,    14,   184,   406,    -1,    67,    -1,    67,   184,
     419,    -1,    67,   184,    75,    -1,    55,   184,   306,   183,
     306,    -1,    55,   184,   306,   183,   419,    -1,   185,     4,
     183,    74,   183,   321,   186,    -1,   379,   281,   183,   388,
      -1,    -1,   105,   184,   307,   183,   314,   183,   316,   380,
     383,   385,   309,   261,    -1,    -1,    49,   184,   307,   183,
     313,   183,   315,   183,   316,   380,   383,   385,   311,   261,
      -1,    -1,   183,     5,    -1,   185,     4,   183,     4,   186,
      -1,    75,    -1,   185,     4,   183,     4,   186,    -1,    75,
      -1,   185,   325,   183,   326,   186,    -1,    75,    -1,   185,
       4,   183,     4,   186,    -1,    75,    -1,   114,   184,   318,
     183,   387,   183,   319,   183,   321,   322,    -1,    26,   184,
     387,   183,   416,   322,    -1,     5,    -1,    75,    -1,   320,
      -1,    75,    -1,    74,    -1,    74,   244,   320,    -1,     4,
      -1,    75,    -1,    -1,   322,   183,   323,    -1,   120,   184,
     121,    -1,    72,   184,   404,    -1,   103,   184,     4,    -1,
     138,    -1,   100,    -1,   101,    -1,   112,    -1,    20,    -1,
      19,   184,   325,   183,   326,   312,   192,    -1,    19,   184,
     395,   312,   192,    -1,    79,    -1,    80,    -1,    81,    -1,
      80,    -1,    24,   184,   329,    -1,    -1,    24,   184,   329,
     328,   261,    -1,   402,   183,   395,   330,    -1,    -1,   330,
     183,   331,    -1,   406,    -1,   109,    -1,   110,    -1,   390,
      -1,   113,   406,    -1,   139,    -1,   136,    -1,   141,    -1,
     142,    -1,   143,    -1,   144,   184,   394,    -1,   145,   184,
     394,    -1,   146,   184,   394,    -1,   147,    -1,   148,    -1,
     149,   184,   332,    -1,   140,    -1,   191,    -1,   150,    -1,
     191,   244,   332,    -1,    22,   184,   335,    -1,    -1,    23,
     184,   335,   334,   261,    -1,   404,   336,    -1,    -1,   336,
     183,   337,    -1,    70,    -1,   151,   184,   402,    -1,   420,
      -1,   106,   184,   406,    -1,   124,   184,   394,    -1,   125,
      -1,    69,    -1,   133,   184,   394,    -1,   162,    -1,    68,
      -1,   134,    -1,   135,   184,   394,    -1,   136,    -1,   137,
      -1,   395,    -1,    98,   184,    95,    -1,    97,   184,    96,
      -1,    99,   184,   394,    -1,   102,   184,   394,    -1,   103,
     184,   394,    -1,   104,   184,   394,    -1,    72,   184,   404,
      -1,   138,    -1,   101,    -1,    25,   184,   378,   183,   395,
      -1,    27,   184,   395,   183,    74,   183,   387,    -1,    31,
     184,   395,   183,    74,    -1,    31,   184,   395,   183,    74,
     183,     5,   256,    -1,    32,    -1,    32,   184,   416,    -1,
      45,   184,   395,   183,    83,    -1,    46,   184,   395,   183,
      83,    -1,    46,   184,   425,   183,   425,   183,    83,    -1,
      50,   184,   425,   183,   425,   183,   191,    -1,    51,   184,
     425,   183,   425,   348,    -1,    52,   184,   425,   183,   425,
      -1,    -1,   183,    83,    -1,    58,   184,   416,   183,    87,
      -1,    59,   184,   416,    -1,    60,   184,   395,   183,   158,
     184,   395,   183,    85,   183,    84,    -1,    60,   184,   395,
     183,    61,   183,    62,   183,    63,   183,    85,   183,    84,
      -1,    73,   184,   395,   183,   155,   353,    -1,    -1,   353,
     183,   354,    -1,    84,    -1,   151,   184,   402,    -1,   152,
     184,   404,    -1,   153,   184,   358,    -1,   154,   184,   358,
      -1,   156,    -1,   157,    -1,   158,   184,   395,    -1,   159,
     184,   378,    -1,   120,   184,   355,    -1,    91,   184,   386,
      -1,   160,   184,     4,    -1,   103,   184,     4,    -1,     4,
      -1,    85,    -1,   121,    -1,    87,    -1,    65,   184,   416,
      -1,    64,   184,   416,    -1,     3,    -1,   185,     3,   183,
     388,   186,    -1,   119,   184,   398,   183,   400,   183,   400,
     183,     7,    -1,   117,   184,   416,   183,   400,    -1,    47,
     184,   398,    -1,    48,   184,   398,    -1,    37,   184,   191,
     183,    38,    -1,    37,   184,   191,   183,    38,   183,   151,
     184,   402,    -1,    34,   184,   398,   183,    36,    -1,    35,
     184,   425,   183,    36,    -1,    -1,    33,   184,   398,   183,
     388,   183,   379,   380,   383,   385,   367,   368,    -1,    -1,
     261,    -1,    42,   184,   395,   183,   389,   183,   391,    -1,
      43,   184,   395,    -1,    92,   184,   416,   183,    93,   183,
      90,    -1,   111,   184,   416,   183,     4,    -1,    44,   184,
      86,   183,   395,    -1,   161,   184,   395,   183,   406,    -1,
     161,   184,   395,   183,    75,    -1,   161,   184,   395,    -1,
      56,   184,   407,   183,   395,    -1,    57,   184,   395,   183,
     424,   183,   406,    -1,    54,   184,   394,   183,   394,   183,
     394,   183,   394,    -1,    54,    -1,   191,    -1,    75,    -1,
     191,    -1,    75,    -1,    -1,   183,   381,    -1,   382,    -1,
     382,   183,   381,    -1,    39,    -1,    40,    -1,    41,    -1,
      -1,    94,   184,   384,    -1,    93,    -1,    -1,    91,   184,
     386,    -1,    90,    -1,    68,    -1,    75,    -1,    69,    -1,
      75,    -1,    78,    -1,   392,    -1,    75,    -1,    78,    -1,
     392,    -1,    77,   184,    75,    -1,    82,    -1,    75,    -1,
      77,   187,     4,   188,    -1,   191,    -1,   202,    -1,   203,
     187,   407,   188,    -1,   407,    -1,   396,    -1,   174,   185,
     416,   186,    -1,   206,    -1,   207,   187,   407,   188,    -1,
     185,     4,   183,     4,   186,    -1,    75,    -1,    76,   397,
     188,    -1,   182,    -1,   182,   183,   397,    -1,   399,    -1,
     208,    -1,   209,   187,   407,   188,    -1,   185,     4,   183,
       4,   183,     4,   183,     4,   186,    -1,   401,    -1,   210,
      -1,   211,   187,   407,   188,    -1,     3,    -1,   185,     3,
     183,   388,   186,    -1,   403,    -1,   212,    -1,   213,   187,
     407,   188,    -1,   191,    -1,     3,    -1,   185,     3,   183,
     191,   186,    -1,    75,    -1,   405,    -1,   214,    -1,   215,
     187,   407,   188,    -1,   191,    -1,     3,    -1,   185,     3,
     183,   191,   186,    -1,    75,    -1,    88,    -1,   393,    -1,
     406,   243,   393,    -1,     4,    -1,   417,    -1,   185,     8,
     186,    -1,   200,    -1,   201,   187,   407,   188,    -1,   407,
     237,   407,    -1,   407,   238,   407,    -1,   407,   239,   407,
      -1,   407,   240,   407,    -1,   407,   241,   407,    -1,   185,
     407,   186,    -1,   196,    -1,   197,    -1,   265,   184,   408,
      -1,   409,    -1,   410,   183,   409,    -1,    -1,   410,    -1,
     407,    -1,   406,    -1,   412,    -1,   413,   183,   412,    -1,
      -1,   413,    -1,   395,    -1,   167,   398,    -1,   168,   398,
      -1,   169,   395,   183,   395,    -1,   170,   395,   183,   395,
     183,   407,    -1,   171,   185,   416,   186,    -1,   171,   185,
     320,   183,   416,   186,    -1,   177,   185,     7,   183,   416,
     186,    -1,   177,   185,   416,   183,   416,   186,    -1,   177,
     185,   400,   183,   416,   186,    -1,   173,   395,    -1,   175,
     185,   395,   183,   407,   186,    -1,   175,   185,   395,   183,
     407,   183,    39,   186,    -1,   176,   185,   395,   183,   407,
     183,   407,   186,    -1,   176,   185,   395,   183,   407,   183,
     407,   183,    39,   186,    -1,   179,   185,   180,   183,   185,
     407,   238,   407,   254,   186,   183,   395,   255,   186,    -1,
     178,   415,    -1,   216,    -1,   185,   416,   186,    -1,   415,
      -1,   415,   245,   416,    -1,   236,    -1,    89,    -1,     4,
      -1,     8,    -1,     9,    -1,     4,    -1,     8,    -1,     9,
      -1,     4,    -1,   417,    -1,    28,    -1,    22,    -1,    29,
      -1,    24,    -1,    30,    -1,   117,    -1,    71,    -1,    75,
      -1,   426,    -1,    53,   185,     4,   183,     4,   183,     4,
     183,     4,   186,    -1,   185,     4,   183,     4,   183,     4,
     183,     4,   186,    -1
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
    1816,  1821,  1826,  1831,  1836,  1841,  1846,  1853,  1859,  1888,
    1893,  1901,  1907,  1913,  1920,  1927,  1937,  1947,  1962,  1973,
    1976,  1982,  1988,  1994,  1998,  2004,  2011,  2017,  2025,  2031,
    2036,  2041,  2046,  2051,  2057,  2063,  2068,  2073,  2078,  2083,
    2088,  2095,  2095,  2095,  2095,  2098,  2104,  2110,  2115,  2122,
    2129,  2135,  2141,  2147,  2151,  2157,  2163,  2174,  2173,  2205,
    2208,  2214,  2221,  2227,  2233,  2239,  2245,  2249,  2254,  2261,
    2267,  2274,  2278,  2285,  2293,  2296,  2306,  2310,  2313,  2319,
    2323,  2330,  2334,  2338,  2345,  2348,  2354,  2361,  2364,  2370,
    2376,  2377,  2380,  2381,  2384,  2385,  2386,  2392,  2393,  2394,
    2400,  2401,  2404,  2413,  2418,  2425,  2436,  2442,  2446,  2450,
    2457,  2467,  2474,  2478,  2484,  2488,  2496,  2500,  2507,  2517,
    2530,  2534,  2541,  2551,  2560,  2571,  2575,  2582,  2592,  2603,
    2612,  2622,  2628,  2632,  2639,  2649,  2660,  2669,  2679,  2683,
    2690,  2691,  2697,  2701,  2705,  2709,  2717,  2726,  2730,  2734,
    2738,  2742,  2746,  2749,  2756,  2765,  2798,  2799,  2802,  2803,
    2806,  2810,  2817,  2824,  2835,  2838,  2846,  2850,  2854,  2858,
    2862,  2867,  2871,  2875,  2880,  2885,  2890,  2894,  2899,  2904,
    2908,  2912,  2917,  2921,  2928,  2934,  2938,  2944,  2951,  2952,
    2955,  2956,  2957,  2960,  2964,  2968,  2972,  2978,  2979,  2982,
    2983,  2986,  2987,  2990,  2991,  2994,  2998,  3024
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
  "CURSE_TYPE", "ENGRAVING_TYPE", "KEYTYPE_ID", "LEVER_ID", "DIRECTION",
  "RANDOM_TYPE", "RANDOM_TYPE_BRACKET", "A_REGISTER", "ALIGNMENT",
  "LEFT_OR_RIGHT", "CENTER", "TOP_OR_BOT", "ALTAR_TYPE", "UP_OR_DOWN",
  "ACTIVE_OR_INACTIVE", "MODRON_PORTAL_TYPE", "NPC_TYPE", "FOUNTAIN_TYPE",
  "SPECIAL_OBJECT_TYPE", "CMAP_TYPE", "FLOOR_TYPE", "FLOOR_TYPE_ID",
  "FLOOR_ID", "FLOOR_MAIN_TYPE", "FLOOR_MAIN_TYPE_ID",
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
  "door_state", "light_state", "alignment", "alignment_prfx", "altar_type",
  "a_register", "string_or_var", "integer_or_var", "coord_or_var",
  "encodecoord", "humidity_flags", "region_or_var", "encoderegion",
  "mapchar_or_var", "mapchar", "monster_or_var", "encodemonster",
  "object_or_var", "encodeobj", "string_expr", "math_expr_var",
  "func_param_type", "func_param_part", "func_param_list",
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
     435,   436,   437,    44,    58,    40,    41,    91,    93,   123,
     125,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,    43,    45,    42,
      47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   246,   247,   247,   248,   248,   249,   250,   250,   251,
     251,   251,   251,   252,   253,   254,   254,   255,   255,   256,
     256,   257,   257,   258,   258,   259,   259,   260,   260,   261,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   265,   265,   265,   266,   266,   267,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   269,   269,   270,   270,   271,
     271,   272,   272,   273,   273,   274,   274,   275,   275,   277,
     278,   276,   279,   280,   281,   281,   282,   282,   282,   284,
     285,   283,   286,   286,   288,   287,   289,   287,   290,   291,
     291,   292,   294,   293,   296,   295,   298,   297,   300,   299,
     301,   302,   301,   303,   304,   304,   304,   305,   305,   306,
     307,   309,   308,   311,   310,   312,   312,   313,   313,   314,
     314,   315,   315,   316,   316,   317,   317,   318,   318,   319,
     319,   320,   320,   321,   321,   322,   322,   323,   323,   323,
     323,   323,   323,   323,   324,   324,   324,   325,   325,   326,
     326,   327,   328,   327,   329,   330,   330,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   332,   332,   332,   333,   334,   333,
     335,   336,   336,   337,   337,   337,   337,   337,   337,   337,
     337,   337,   337,   337,   337,   337,   337,   337,   337,   337,
     337,   337,   337,   337,   337,   337,   337,   338,   339,   340,
     340,   341,   341,   342,   343,   344,   345,   346,   347,   348,
     348,   349,   350,   351,   351,   352,   353,   353,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   355,   355,   355,   355,   356,   357,   358,   358,   359,
     360,   361,   362,   363,   363,   364,   365,   367,   366,   368,
     368,   369,   370,   371,   372,   373,   374,   374,   374,   375,
     376,   377,   377,   378,   378,   379,   379,   380,   380,   381,
     381,   382,   382,   382,   383,   383,   384,   385,   385,   386,
     387,   387,   388,   388,   389,   389,   389,   390,   390,   390,
     391,   391,   392,   393,   393,   393,   394,   395,   395,   395,
     395,   396,   396,   396,   397,   397,   398,   398,   398,   399,
     400,   400,   400,   401,   401,   402,   402,   402,   403,   403,
     403,   403,   404,   404,   404,   405,   405,   405,   405,   405,
     406,   406,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   408,   408,   409,   410,   410,   411,   411,
     412,   412,   413,   413,   414,   414,   415,   415,   415,   415,
     415,   415,   415,   415,   415,   415,   415,   415,   415,   415,
     415,   415,   415,   415,   415,   416,   416,   417,   418,   418,
     419,   419,   419,   420,   420,   420,   420,   421,   421,   422,
     422,   423,   423,   424,   424,   425,   425,   426
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
       4,     0,    12,     0,    14,     0,     2,     5,     1,     5,
       1,     5,     1,     5,     1,    10,     6,     1,     1,     1,
       1,     1,     3,     1,     1,     0,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     7,     5,     1,     1,     1,
       1,     3,     0,     5,     4,     0,     3,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     3,     3,     3,
       1,     1,     3,     1,     1,     1,     3,     3,     0,     5,
       2,     0,     3,     1,     3,     1,     3,     3,     1,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     5,     7,     5,
       8,     1,     3,     5,     5,     7,     7,     6,     5,     0,
       2,     5,     3,    11,    13,     6,     0,     3,     1,     3,
       3,     3,     3,     1,     1,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     3,     3,     1,     5,     9,
       5,     3,     3,     5,     9,     5,     5,     0,    12,     0,
       1,     7,     3,     7,     5,     5,     5,     5,     3,     5,
       7,     9,     1,     1,     1,     1,     1,     0,     2,     1,
       3,     1,     1,     1,     0,     3,     1,     0,     3,     1,
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
       2,     0,     0,     0,     3,     4,    23,     0,     0,     1,
       5,     0,    27,     0,     7,     0,   146,     0,     0,     0,
       0,   214,     0,     0,     0,     0,     0,     0,     0,     0,
     281,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   342,     0,     0,     0,
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
       0,     0,     0,   412,     0,   415,     0,   457,     0,   413,
     434,    28,     0,   166,     0,     8,     0,   373,   374,     0,
     410,   173,     0,     0,     0,    11,   459,   458,    13,   382,
       0,   217,   218,     0,     0,   379,     0,     0,   185,   377,
      14,   406,   408,   409,     0,   405,   403,     0,   247,   251,
     402,   248,   399,   401,     0,   398,   396,     0,   221,     0,
     395,   344,   343,     0,   360,   361,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   453,   436,   455,   282,     0,   387,     0,     0,   386,
       0,     0,     0,     0,   475,     0,     0,   332,     0,     0,
       0,     0,     0,   321,   322,   346,   345,     0,   144,     0,
       0,     0,     0,   376,     0,     0,     0,     0,     0,   292,
       0,   316,   315,   462,   460,   461,   176,   175,     0,     0,
       0,     0,   197,   198,     0,     0,     0,     0,   109,     0,
       0,     0,   338,   139,     0,     0,     0,     0,   148,     0,
       0,     0,     0,     0,   431,   430,   432,   435,     0,   468,
     470,   467,   469,   471,   472,     0,     0,     0,   116,   117,
     112,   110,     0,     0,     0,     0,    27,   163,    25,     0,
       0,     0,     0,     0,   384,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   250,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,   438,     0,     0,     0,   446,
       0,     0,     0,   452,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   170,
     169,     0,     0,   164,     0,     0,     0,   428,   414,   422,
       0,     0,   417,   418,   419,   420,   421,     0,   142,     0,
     412,     0,     0,     0,     0,   133,   131,   137,   135,     0,
       0,     0,   167,     0,     0,   411,    10,   317,     0,     9,
       0,     0,   383,     0,     0,     0,   220,   219,   185,   186,
     216,     0,     0,     0,   249,     0,     0,   223,   225,   277,
     205,     0,   279,     0,     0,   201,     0,     0,     0,     0,
     393,     0,     0,   391,     0,     0,   390,     0,     0,   454,
     456,     0,     0,   362,   363,     0,   325,     0,     0,   326,
     323,   366,     0,   364,     0,   365,   335,   283,     0,   284,
       0,   188,     0,     0,     0,     0,   289,   288,     0,     0,
     177,   178,   339,   473,   474,     0,   291,     0,     0,   296,
       0,   190,     0,     0,   334,     0,     0,     0,   320,     0,
       0,   160,     0,     0,   150,   337,   336,     0,   426,   429,
       0,   416,   147,   433,   111,     0,     0,   120,     0,   119,
       0,   118,     0,   124,     0,   115,     0,   114,     0,   113,
      29,   375,     0,     0,   385,   378,     0,   380,     0,     0,
     404,   465,   463,   464,   262,   259,   253,     0,     0,     0,
       0,   276,     0,     0,     0,     0,     0,   258,     0,   263,
       0,   265,   266,   275,     0,   261,   252,   267,   466,   255,
       0,   397,   224,   196,     0,     0,   439,     0,     0,     0,
     441,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     388,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   180,     0,     0,   287,     0,     0,     0,     0,     0,
     295,     0,     0,     0,     0,     0,     0,   165,   159,   161,
       0,     0,     0,   140,     0,   132,   134,   136,   138,     0,
     125,     0,   127,     0,   129,     0,     0,   381,   215,   407,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,   278,    19,     0,   202,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,   371,   370,   331,   285,     0,   192,
       0,     0,   286,   290,     0,     0,   340,     0,     0,     0,
     333,     0,   194,     0,   347,   200,     0,   199,   172,     0,
     152,   423,   424,   425,   427,     0,     0,   123,     0,   122,
       0,   121,     0,     0,   274,   269,   268,   270,   271,   272,
     273,   256,   257,   260,   264,   254,     0,   367,   228,   229,
       0,   233,   232,   243,   234,   235,   236,     0,     0,     0,
     240,   241,     0,   226,   230,   368,   227,     0,   211,   212,
       0,   213,     0,   210,   206,     0,   280,   440,   442,     0,
     447,     0,   443,     0,   392,   445,   444,     0,     0,     0,
     354,     0,     0,     0,   372,     0,     0,     0,     0,   203,
     204,     0,     0,     0,   298,     0,     0,     0,     0,     0,
       0,     0,   303,   304,     0,     0,     0,   297,     0,     0,
     354,     0,     0,     0,     0,     0,   152,   141,   126,   128,
     130,   318,     0,     0,   231,     0,     0,     0,     0,     0,
       0,     0,    20,     0,     0,   394,     0,     0,   351,   352,
     353,   348,   349,     0,   357,     0,     0,   324,   187,     0,
     347,   341,   179,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   189,     0,   357,   205,   319,
       0,   156,   151,   153,     0,   369,   237,   238,   239,   245,
     244,   242,   208,   209,   207,   448,     0,   449,   418,     0,
       0,     0,     0,   327,     0,     0,     0,   354,     0,     0,
     359,   308,   310,   311,   312,   314,   313,   307,   299,   300,
     301,   302,   305,   306,   309,     0,   181,   195,   154,    27,
       0,     0,     0,     0,     0,   389,   350,   356,   355,     0,
     329,     0,   477,   191,   357,     0,   293,   193,     0,    27,
     157,     0,   246,   450,    16,     0,   358,   330,   328,   476,
     183,     0,   182,   155,     0,     0,     0,   294,     0,    17,
     184,    21,    22,    19,     0,     0,    12,    18,   451
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    93,    94,    95,  1004,  1045,
     846,  1043,    12,   158,    96,   387,    97,    98,    99,   100,
     209,   101,   102,   719,   721,   723,   481,   482,   483,   484,
     103,   467,   795,   104,   105,   443,   106,   107,   210,   710,
     895,   896,  1019,   999,   108,   593,   109,   110,   224,   111,
     590,   112,   385,   113,   345,   460,   586,   114,   115,   116,
     325,   317,   117,  1018,   118,  1036,   401,   563,   583,   771,
     784,   119,   344,   786,   526,   871,   663,   844,   120,   247,
     508,   121,   408,   268,   662,   833,   961,   122,   405,   258,
     404,   656,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   694,   133,   134,   135,   136,   700,   887,   987,
     137,   138,   499,   139,   140,   141,   142,   143,   144,   145,
     146,  1010,  1028,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   273,   318,   860,   921,   922,   924,  1008,   973,
     981,   276,   545,   554,   834,   766,   555,   230,   322,   292,
     249,   395,   298,   299,   535,   536,   269,   270,   259,   260,
     364,   323,   793,   598,   599,   600,   366,   367,   368,   293,
     425,   219,   238,   337,   659,   382,   383,   384,   575,   303,
     304
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -873
static const yytype_int16 yypact[] =
{
     290,  -124,   -77,   120,  -873,   290,    61,    10,    23,  -873,
    -873,    49,   754,    76,  -873,   179,  -873,    95,   112,   130,
     163,  -873,   178,   181,   189,   196,   200,   209,   210,   211,
     222,   223,   227,   228,   232,   233,   235,   236,   238,   239,
     240,   242,   249,   251,   252,   253,   256,   257,   258,   270,
     271,   272,   274,   276,   278,   286,   288,   293,   299,   300,
     302,    19,   303,   304,  -873,   305,   145,   650,  -873,  -873,
     307,   183,   110,   225,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,   754,  -873,  -873,
     166,  -873,  -873,  -873,  -873,  -873,   311,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,   472,   313,  -873,    18,
     339,    50,   295,   404,   291,   291,   192,    -2,    24,   -23,
     -23,   824,   -96,   -96,   -13,   308,   -23,   -23,   414,   -23,
     -10,   -96,   -96,    -1,   -13,   -13,   -13,   110,   318,   110,
     -23,   824,   824,   -23,   824,   824,    82,   -23,   824,    -1,
     824,    52,  -873,   824,   -96,   727,   110,  -873,  -873,   262,
     321,   -23,   326,  -873,    80,  -873,   322,  -873,   226,  -873,
     124,  -873,   368,  -873,   323,  -873,   179,  -873,  -873,   327,
    -873,   275,   336,   344,   345,  -873,  -873,  -873,  -873,  -873,
     333,  -873,  -873,   353,   537,  -873,   360,   365,   367,  -873,
    -873,  -873,  -873,  -873,   551,  -873,  -873,   371,  -873,  -873,
    -873,  -873,  -873,  -873,   552,  -873,  -873,   373,   377,   384,
    -873,  -873,  -873,   389,  -873,  -873,   390,   400,   422,   -96,
     -96,   -23,   -23,   426,   -23,   437,   438,   441,   824,   446,
     539,  -873,  -873,   382,  -873,   628,  -873,   448,   450,  -873,
     453,   457,   646,   468,  -873,   469,   471,  -873,   473,   474,
     654,   476,   478,  -873,  -873,  -873,  -873,   479,   550,   480,
     481,   485,   486,   208,   666,   488,   162,   490,   492,  -873,
     495,  -873,  -873,  -873,  -873,  -873,  -873,  -873,   504,   508,
     510,   512,  -873,  -873,   513,   323,   514,   517,  -873,   497,
     110,   110,   518,  -873,   519,   148,   110,   110,  -873,   110,
     110,   110,   110,   110,   275,   208,  -873,   528,   533,  -873,
    -873,  -873,  -873,  -873,  -873,   546,    92,    28,  -873,  -873,
     275,   208,   547,   548,   554,   754,   754,  -873,  -873,   110,
      18,   691,    25,   741,   564,   565,   824,   571,   110,    17,
     751,   566,   574,   110,   576,   323,   578,   110,   323,   -23,
     -23,   824,   688,   689,  -873,  -873,   581,   582,   719,  -873,
     -23,   -23,   418,  -873,   586,   583,   824,   584,   110,    60,
     734,   780,   624,   779,   778,   104,   -23,   737,   634,   739,
     -13,   -44,  -873,   640,   -13,   -13,   -13,   110,   641,    73,
     -23,   137,   738,    -6,   671,   735,   -29,   825,    24,   714,
    -873,   146,   146,  -873,   191,   643,   -17,   841,  -873,  -873,
     341,   361,    33,    33,  -873,  -873,  -873,   124,  -873,   824,
     655,   -75,   -72,   -58,   -57,  -873,  -873,   275,   208,   224,
      56,   144,  -873,   647,   442,  -873,  -873,  -873,   842,  -873,
     664,   333,  -873,   662,   845,   496,  -873,  -873,   367,  -873,
    -873,   660,   502,   101,  -873,   670,   511,  -873,  -873,  -873,
    -873,   672,   680,   -23,   -23,   609,   684,   686,   687,   695,
    -873,   696,   470,  -873,   704,   699,  -873,   718,   720,  -873,
    -873,   870,   595,  -873,  -873,   722,  -873,   723,   903,  -873,
     725,  -873,   724,  -873,   726,  -873,  -873,  -873,   906,  -873,
     729,  -873,   912,   736,    60,   740,   744,  -873,   746,   844,
    -873,  -873,  -873,  -873,  -873,   748,  -873,   750,   753,  -873,
     756,  -873,   916,   760,  -873,   767,   805,   921,  -873,   768,
     323,  -873,   709,   110,  -873,  -873,   275,   769,  -873,   790,
     788,  -873,  -873,  -873,  -873,   982,   802,  -873,   -27,  -873,
     110,  -873,    18,  -873,    31,  -873,    26,  -873,    38,  -873,
    -873,  -873,   806,   985,  -873,  -873,   804,  -873,   812,   810,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,   821,   822,   823,
     826,  -873,   827,   828,   829,   830,   831,  -873,   832,  -873,
     833,  -873,  -873,  -873,   834,  -873,  -873,  -873,  -873,  -873,
     835,  -873,   837,   839,    24,  1003,  -873,   840,   945,   824,
    -873,   110,   110,   824,   843,   110,   824,   824,   847,   846,
    -873,    -1,  1020,   850,   874,  1024,    20,    32,   951,   853,
      -7,  -873,   868,   954,  -873,   110,   877,    18,   999,   -23,
     879,   973,   881,     3,   268,   323,   146,  -873,  -873,   208,
     876,   171,   841,  -873,   102,  -873,  -873,   208,   275,   -30,
    -873,   -24,  -873,     4,  -873,    60,   883,  -873,  -873,  -873,
     291,   971,   974,   110,   110,   110,   110,    18,   110,   110,
     110,   192,  -873,   836,   378,  -873,   885,   110,  -873,   884,
     285,   369,   886,    60,   602,   887,   888,   110,  1067,   892,
     893,  1073,   894,   891,  -873,  -873,  -873,  -873,  1076,  -873,
     279,   898,  -873,  -873,   899,    75,   275,   900,   901,   569,
    -873,  1081,  -873,  1082,   892,  -873,   904,  -873,  -873,   905,
      13,  -873,  -873,  -873,  -873,   323,    31,  -873,    26,  -873,
      38,  -873,   907,  1084,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,   275,  -873,  -873,  -873,  -873,   151,  -873,  -873,  -873,
      18,  -873,  -873,  -873,  -873,  -873,  -873,   908,   910,   911,
    -873,  -873,   913,  -873,  -873,  -873,   275,   914,  -873,  -873,
     915,  -873,   917,  -873,  -873,  1087,  -873,   208,  -873,  1052,
    -873,   110,  -873,   918,  -873,  -873,  -873,   380,   919,   281,
    1002,  1096,   920,   192,  -873,   922,   923,     3,   110,  -873,
    -873,   924,  1042,  1022,  -873,   925,   927,   928,   929,   930,
     931,   932,  -873,  -873,   933,   934,   935,  -873,   936,   937,
    1002,    75,  1114,   332,   939,   938,    13,  -873,  -873,  -873,
    -873,  -873,   941,  1050,   275,   110,   110,   110,   -74,   291,
    1122,  1006,  -873,   943,   324,  -873,   110,  1126,  -873,  -873,
    -873,  -873,   948,   949,  1041,   952,  1130,  -873,  -873,    17,
     892,  -873,  -873,   953,   955,  1047,  1135,    59,   192,   291,
      25,    25,   -23,    -2,  1136,  -873,  1137,  1041,  -873,  -873,
     958,  -873,  -873,  -873,  1138,  -873,  -873,  -873,  -873,  -873,
     902,  -873,  -873,  -873,  -873,  -873,  1105,  -873,  -103,   959,
     281,  1054,   964,  -873,  1145,   965,   966,  1002,  1065,  1069,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,   968,  -873,   839,  -873,   754,
     972,   -74,   970,   976,   975,  -873,  -873,  -873,  -873,  1047,
     323,   977,  -873,  -873,  1041,   979,  -873,  -873,   323,   754,
    -873,    60,  -873,  -873,  -873,   981,  -873,  -873,  -873,  -873,
    -873,  1074,  -873,  -873,   983,   -23,   323,  -873,    65,   984,
    -873,  -873,  -873,   885,   -23,   986,  -873,  -873,  -873
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -873,  -873,  1154,  -873,  -873,  -873,  -873,  -873,  -873,  -873,
     117,  -873,  -873,   942,   -97,  -344,   784,   960,  1103,  -455,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,  1110,  -873,  -873,  -873,
     277,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,
     728,   980,  -873,  -873,  -873,  -873,   667,  -873,  -873,  -873,
     309,  -873,  -873,  -873,  -581,   283,   230,  -873,  -873,   410,
     254,  -873,  -873,  -873,  -873,  -873,   180,  -873,  -873,  1017,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,  -559,  -873,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,   241,   505,  -763,   215,  -873,  -823,  -873,  -872,
     182,  -413,  -560,  -873,  -873,  -873,   444,   798,  -203,  -160,
    -366,   692,   -88,  -363,  -435,  -568,  -485,  -545,  -482,  -542,
    -153,   -59,  -873,   477,  -873,  -873,   713,  -873,  -873,   909,
    -156,   679,  -873,  -441,  -873,  -873,  -873,  -873,  -873,  -162,
    -873
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -223
static const yytype_int16 yytable[] =
{
     221,   459,   248,   349,   691,   617,   231,   615,   571,   277,
     278,   485,   597,   218,   486,   294,   306,   307,   312,   309,
     311,   890,   319,   320,   321,    16,   588,   589,   497,   262,
     327,   561,   213,   330,   251,   328,   329,   338,   331,   332,
     301,   530,   339,   301,   341,   585,   581,   346,   239,   240,
     724,   352,   239,   240,   236,   577,   619,   342,   595,   262,
       7,   514,   378,   983,   517,   239,   240,   947,   769,   380,
    1041,   722,   720,   271,   315,   996,   959,   333,   782,   869,
    1003,   334,   335,   -15,   213,   300,   333,   748,   354,   295,
     334,   335,   274,   313,   314,   764,   480,   506,   507,   275,
     354,   263,   765,   239,   240,   631,   252,     8,   606,   632,
     633,   608,   296,   297,   213,   607,   347,   960,   609,   253,
       9,   416,   417,   787,   419,   610,   612,   343,   213,   543,
     326,   263,   611,   613,   379,   544,   361,   362,   363,   237,
    1042,   562,  1030,   893,   984,   894,   985,   530,   465,   530,
     870,   243,   578,   796,  1014,   355,   582,   336,   244,   798,
     797,   365,   244,   381,   243,   802,   799,   977,    11,   634,
     635,   636,   302,   637,   227,   310,   239,   240,   770,   551,
     986,   552,   553,   245,   246,   228,   229,   800,   783,   272,
     316,   414,   415,   853,   801,   262,   245,   246,   638,   639,
     640,    13,   641,   642,   643,   644,    72,   645,   573,   227,
     498,   264,   574,   376,    14,   761,   254,   265,   727,   227,
     228,   229,   255,   587,   487,   646,   647,   251,   215,   216,
     228,   229,   900,    15,   648,   649,   650,   651,   652,   653,
     503,   264,   716,   715,   568,   616,   707,   265,   804,   518,
     519,   745,   654,   899,   898,   520,   815,   597,   324,   156,
     528,   529,   527,   655,   217,   214,   537,   263,   266,   267,
     540,   789,   361,   362,   363,   243,   556,   214,   560,   159,
     215,   216,   565,   566,   567,   758,   244,   157,   727,   493,
     572,   464,   215,   216,   251,   214,   160,   470,   471,   252,
     472,   473,   474,   475,   476,     1,     2,   245,   246,   214,
     215,   216,   253,   596,   161,   227,   217,   355,   488,   591,
     918,   919,   920,   604,   215,   216,   228,   229,   217,   587,
     494,   587,   206,   618,   469,   903,   333,   217,   685,   505,
     334,   335,   525,   785,   512,   450,   217,   162,   516,   232,
     233,   234,   235,   657,   533,   534,   533,   534,   241,   242,
     217,   788,   163,   666,   667,   164,   252,   791,   792,   542,
     239,   240,   213,   165,   241,   242,   212,   264,   927,   253,
     166,   990,   991,   265,   167,   359,   360,   361,   362,   363,
     369,   357,   370,   168,   169,   170,   371,   372,   373,   359,
     360,   361,   362,   363,   266,   267,   171,   172,   222,   254,
     220,   173,   174,   614,   358,   255,   175,   176,   365,   177,
     178,   530,   179,   180,   181,   531,   182,   962,   359,   360,
     361,   362,   363,   183,   592,   184,   185,   186,   256,   257,
     187,   188,   189,   239,   240,   359,   360,   361,   362,   363,
     837,   897,   950,   988,   190,   191,   192,   989,   193,   718,
     194,  1034,   195,   359,   360,   361,   362,   363,   849,   243,
     196,   850,   197,   674,   397,   225,   254,   198,   838,   839,
     244,   840,   255,   199,   200,   374,   201,   203,   204,   205,
     841,   211,   774,   239,   240,   223,   226,   250,   842,   305,
     308,   245,   246,   324,   350,   256,   257,   966,   351,   356,
     967,   353,   386,   749,   389,   394,   843,   752,   390,   391,
     755,   756,   359,   360,   361,   362,   363,   392,   393,   601,
     807,   808,   809,   810,   709,   812,   813,   814,   396,   778,
     375,   397,   243,   397,   776,   239,   240,   398,   399,   602,
     400,   717,   851,   376,   402,   406,   442,   377,   403,   227,
     407,   359,   360,   361,   362,   363,  -222,   409,   215,   216,
     228,   229,   410,   411,   245,   246,   296,   297,   359,   360,
     361,   362,   363,   412,   811,   279,   280,   281,   282,   283,
     836,   284,   243,   285,   286,   287,   288,   289,   359,   360,
     361,   362,   363,   532,   217,   413,   359,   360,   361,   362,
     363,   418,   750,   751,   239,   240,   754,   359,   916,   361,
     362,   363,   420,   421,   245,   246,   422,   426,   533,   534,
     621,   424,   427,   429,   291,   428,   430,   279,   280,   281,
     282,   283,   431,   284,   243,   285,   286,   287,   288,   289,
     432,   433,   434,   874,   435,   290,   436,   437,   438,   439,
     875,   440,   441,   444,   445,   931,  1027,   904,   446,   447,
     448,   449,   876,   451,  1032,   452,   245,   246,   453,   359,
     360,   361,   362,   363,   627,   463,   291,   454,   847,   877,
     630,   455,  1040,   456,   496,   457,   458,   461,   857,   661,
     462,   466,   956,   957,   958,   468,   279,   280,   281,   282,
     283,   477,   284,   243,   285,   286,   287,   288,   289,   478,
     878,   879,   880,   881,   290,   882,   883,   884,   885,   886,
     479,   489,   490,   359,   360,   361,   362,   363,   491,   359,
     360,   361,   362,   363,   500,   245,   246,   501,   359,   360,
     361,   362,   363,   502,   504,   291,   509,   511,   510,   513,
      16,   515,   521,   522,   523,   524,   538,   541,    17,   539,
     546,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,   992,   680,   547,    29,    30,    31,    32,    33,
     854,    34,   914,   525,   239,   240,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,   548,    46,    47,
      48,    49,    50,    51,    52,   549,   550,   558,    53,    54,
     557,    55,   559,   564,   569,   576,   579,    56,   580,   584,
    -171,   594,   359,   360,   361,   362,   363,   620,   605,   359,
     360,   361,   362,   363,   207,   622,    57,   623,   625,   626,
      75,   629,    77,   668,    79,   664,    81,   968,    83,    58,
      85,   660,    87,   665,    89,    59,    91,   669,    60,    61,
     671,    62,   670,    63,   679,  1039,    64,    65,   672,   673,
      66,    67,   676,    68,  1047,    69,   279,   280,   281,   282,
     283,   675,   284,   243,   285,   286,   287,   288,   289,   239,
     240,   677,  1020,   678,   290,   681,   682,   683,   684,   686,
     687,   685,   688,   816,   817,    70,   689,    71,   696,   690,
     702,   705,  1033,   692,   674,   245,   246,   693,    76,   695,
      78,   697,    80,   698,    82,   291,    84,   699,    86,   701,
      88,    72,    90,   703,    92,   818,   819,    73,    74,   820,
     704,   706,   708,   711,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,   821,   712,   713,   822,   823,   824,   825,   826,
     827,   828,   829,   830,   831,   832,   714,   295,   726,   725,
     727,   279,   280,   281,   282,   283,   729,   284,   243,   285,
     286,   287,   288,   289,   728,   730,   731,   732,   746,   290,
     733,   734,   735,   736,   737,   738,   739,   740,   741,   525,
     743,   742,   744,   747,   760,   762,   753,   227,   763,   758,
     245,   246,   757,   761,   767,    74,   768,   773,   228,   229,
     291,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,   772,
     775,   777,   779,   780,   781,   790,   803,   805,   845,   806,
     848,   858,   852,   855,   856,   859,   861,   862,   863,   864,
     865,   867,   868,   872,   873,   888,   889,   891,   892,   902,
     912,   913,   905,   901,   906,   907,   923,   908,   909,   910,
     925,   911,   917,   926,   915,   933,   929,   934,   928,   935,
     932,   936,   937,   938,   939,   940,   941,   942,   943,   944,
     946,   949,   945,   951,   954,   955,   963,   964,   952,   965,
     969,   970,   972,   971,   975,   974,   978,   980,   979,   982,
     994,   995,   998,  1000,  1002,  1005,  1001,  1007,  1009,  1011,
    1015,  1012,  1013,  1016,  1017,  1021,  1023,  1024,  1037,    10,
    1046,  1025,  1031,  1029,  1035,   348,  1038,  1044,   388,   492,
     208,   202,  1048,   953,   948,   628,   930,   570,   997,   340,
     866,  1022,   261,   976,   993,  1006,   759,   835,   495,   794,
     603,  1026,   658,   624,     0,     0,     0,   423
};

static const yytype_int16 yycheck[] =
{
      97,   345,   162,   206,   564,   490,   159,   489,   449,   169,
     170,   377,   467,    72,   377,   171,   176,   177,   180,   179,
     180,   784,   184,   185,   186,     6,   461,   462,     3,     3,
     190,    75,     4,   193,     3,   191,   192,   197,   194,   195,
      53,     3,   198,    53,   200,   458,    75,   203,    75,    76,
     618,   211,    75,    76,     4,    61,   491,     5,    75,     3,
     184,   405,   222,     4,   408,    75,    76,   890,    75,   222,
       5,   616,   614,    75,    75,   947,   150,     4,    75,     4,
     183,     8,     9,   186,     4,   173,     4,   668,     8,   185,
       8,     9,    68,   181,   182,    75,     4,    80,    81,    75,
       8,    75,    82,    75,    76,     4,    75,   184,   183,     8,
       9,   183,   208,   209,     4,   190,   204,   191,   190,    88,
       0,   281,   282,   704,   284,   183,   183,    75,     4,    69,
     189,    75,   190,   190,   222,    75,   239,   240,   241,    89,
      75,   185,  1014,   130,    85,   132,    87,     3,   351,     3,
      75,   174,   158,   183,   977,   214,   185,    75,   185,   183,
     190,   220,   185,   222,   174,   725,   190,   930,   107,    68,
      69,    70,   185,    72,   191,   185,    75,    76,   185,    75,
     121,    77,    78,   206,   207,   202,   203,   183,   185,   191,
     191,   279,   280,   753,   190,     3,   206,   207,    97,    98,
      99,   191,   101,   102,   103,   104,   187,   106,    71,   191,
     185,   185,    75,   185,   191,   183,   185,   191,   186,   191,
     202,   203,   191,   185,   377,   124,   125,     3,   200,   201,
     202,   203,   800,   184,   133,   134,   135,   136,   137,   138,
     396,   185,   608,   606,   447,   189,   590,   191,   730,   409,
     410,   664,   151,   798,   796,   411,   741,   712,   185,   183,
     420,   421,   418,   162,   236,   185,   422,    75,   212,   213,
     426,   706,   239,   240,   241,   174,   436,   185,   440,   184,
     200,   201,   444,   445,   446,   183,   185,   108,   186,   386,
     450,   350,   200,   201,     3,   185,   184,   356,   357,    75,
     359,   360,   361,   362,   363,    15,    16,   206,   207,   185,
     200,   201,    88,   466,   184,   191,   236,   376,   377,   128,
      39,    40,    41,   479,   200,   201,   202,   203,   236,   185,
     389,   185,   187,   189,   186,   184,     4,   236,   187,   398,
       8,     9,    74,    75,   403,   183,   236,   184,   407,    10,
      11,    12,    13,   513,   210,   211,   210,   211,    79,    80,
     236,   705,   184,   523,   524,   184,    75,   196,   197,   428,
      75,    76,     4,   184,    79,    80,   193,   185,   863,    88,
     184,   940,   941,   191,   184,   237,   238,   239,   240,   241,
      22,   165,    24,   184,   184,   184,    28,    29,    30,   237,
     238,   239,   240,   241,   212,   213,   184,   184,   242,   185,
     185,   184,   184,   189,   188,   191,   184,   184,   477,   184,
     184,     3,   184,   184,   184,     7,   184,   909,   237,   238,
     239,   240,   241,   184,   243,   184,   184,   184,   214,   215,
     184,   184,   184,    75,    76,   237,   238,   239,   240,   241,
      72,   795,   893,   938,   184,   184,   184,   939,   184,   612,
     184,  1021,   184,   237,   238,   239,   240,   241,   183,   174,
     184,   186,   184,     3,     4,     3,   185,   184,   100,   101,
     185,   103,   191,   184,   184,   117,   184,   184,   184,   184,
     112,   184,   695,    75,    76,   184,   183,    93,   120,   191,
      86,   206,   207,   185,   242,   214,   215,   183,   187,   187,
     186,   185,   189,   669,   187,   182,   138,   673,   243,   183,
     676,   677,   237,   238,   239,   240,   241,   183,   183,   188,
     733,   734,   735,   736,   593,   738,   739,   740,   185,   699,
     172,     4,   174,     4,   697,    75,    76,   187,   183,   188,
     183,   610,   183,   185,     3,     3,     6,   189,   187,   191,
     187,   237,   238,   239,   240,   241,   189,   183,   200,   201,
     202,   203,   183,   183,   206,   207,   208,   209,   237,   238,
     239,   240,   241,   183,   737,   167,   168,   169,   170,   171,
     743,   173,   174,   175,   176,   177,   178,   179,   237,   238,
     239,   240,   241,   185,   236,   183,   237,   238,   239,   240,
     241,   185,   671,   672,    75,    76,   675,   237,   238,   239,
     240,   241,   185,   185,   206,   207,   185,   245,   210,   211,
     188,   185,     4,   183,   216,   187,   183,   167,   168,   169,
     170,   171,   185,   173,   174,   175,   176,   177,   178,   179,
       4,   183,   183,    84,   183,   185,   183,   183,     4,   183,
      91,   183,   183,   183,   183,   868,  1010,   820,   183,   183,
       4,   183,   103,   183,  1018,   183,   206,   207,   183,   237,
     238,   239,   240,   241,   188,   188,   216,   183,   747,   120,
     188,   183,  1036,   183,     3,   183,   183,   183,   757,   188,
     183,   183,   905,   906,   907,   186,   167,   168,   169,   170,
     171,   183,   173,   174,   175,   176,   177,   178,   179,   186,
     151,   152,   153,   154,   185,   156,   157,   158,   159,   160,
     184,   184,   184,   237,   238,   239,   240,   241,   184,   237,
     238,   239,   240,   241,     3,   206,   207,   183,   237,   238,
     239,   240,   241,   188,   183,   216,     5,   183,   192,   183,
       6,   183,    74,    74,   183,   183,   180,   183,    14,   186,
      36,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,   942,   188,     4,    31,    32,    33,    34,    35,
     188,    37,   851,    74,    75,    76,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,   183,    54,    55,
      56,    57,    58,    59,    60,    36,    38,   183,    64,    65,
      83,    67,    83,   183,   183,    87,   155,    73,    93,     4,
     116,   188,   237,   238,   239,   240,   241,   190,   183,   237,
     238,   239,   240,   241,   194,     3,    92,   183,   186,     4,
     200,   191,   202,   244,   204,   183,   206,   916,   208,   105,
     210,   191,   212,   183,   214,   111,   216,   183,   114,   115,
     183,   117,   186,   119,     4,  1035,   122,   123,   183,   183,
     126,   127,   183,   129,  1044,   131,   167,   168,   169,   170,
     171,   187,   173,   174,   175,   176,   177,   178,   179,    75,
      76,   183,   999,   183,   185,   183,   183,     4,   183,   183,
       4,   187,   183,    77,    78,   161,     4,   163,    74,   183,
       4,   116,  1019,   183,     3,   206,   207,   183,   201,   183,
     203,   183,   205,   183,   207,   216,   209,   184,   211,   183,
     213,   187,   215,   183,   217,   109,   110,   193,   194,   113,
     183,   183,   243,   184,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   136,   183,   186,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,     4,   185,     3,   183,
     186,   167,   168,   169,   170,   171,   186,   173,   174,   175,
     176,   177,   178,   179,   192,   184,   184,   184,     5,   185,
     184,   184,   184,   184,   184,   184,   184,   184,   184,    74,
     183,   186,   183,   183,     4,   151,   183,   191,     4,   183,
     206,   207,   185,   183,    83,   194,   183,    83,   202,   203,
     216,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   191,
     183,    62,   183,    90,   183,   189,   183,    96,   183,    95,
     186,     4,   186,   186,   186,   183,   183,     4,   184,   188,
       4,   183,   183,   183,   183,     4,     4,   183,   183,     5,
       3,    39,   184,   186,   184,   184,    94,   184,   184,   184,
       4,   184,   183,   183,   186,    63,   183,    85,   186,   184,
     186,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     183,     7,   186,   184,   183,    75,     4,   121,   190,   186,
       4,   183,    91,   184,     4,   183,   183,    90,   183,     4,
       4,     4,   184,     5,    39,   186,   244,    93,   184,     4,
      85,   186,   186,    84,   186,   183,   186,   181,    84,     5,
    1043,   186,   183,   186,   183,   205,   183,   183,   226,   385,
      67,    61,   186,   896,   891,   508,   867,   449,   948,   199,
     770,  1001,   165,   929,   943,   970,   681,   743,   390,   712,
     477,  1009,   513,   501,    -1,    -1,    -1,   288
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   247,   248,   249,   250,   184,   184,     0,
     248,   107,   258,   191,   191,   184,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    31,
      32,    33,    34,    35,    37,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    54,    55,    56,    57,
      58,    59,    60,    64,    65,    67,    73,    92,   105,   111,
     114,   115,   117,   119,   122,   123,   126,   127,   129,   131,
     161,   163,   187,   193,   194,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   251,   252,   253,   260,   262,   263,   264,
     265,   267,   268,   276,   279,   280,   282,   283,   290,   292,
     293,   295,   297,   299,   303,   304,   305,   308,   310,   317,
     324,   327,   333,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   349,   350,   351,   352,   356,   357,   359,
     360,   361,   362,   363,   364,   365,   366,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   183,   108,   259,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   282,   184,   184,   184,   187,   194,   264,   266,
     284,   184,   193,     4,   185,   200,   201,   236,   407,   417,
     185,   260,   242,   184,   294,     3,   183,   191,   202,   203,
     393,   406,    10,    11,    12,    13,     4,    89,   418,    75,
      76,    79,    80,   174,   185,   206,   207,   325,   395,   396,
      93,     3,    75,    88,   185,   191,   214,   215,   335,   404,
     405,   335,     3,    75,   185,   191,   212,   213,   329,   402,
     403,    75,   191,   378,    68,    75,   387,   395,   395,   167,
     168,   169,   170,   171,   173,   175,   176,   177,   178,   179,
     185,   216,   395,   415,   416,   185,   208,   209,   398,   399,
     398,    53,   185,   425,   426,   191,   395,   395,    86,   395,
     185,   395,   425,   398,   398,    75,   191,   307,   379,   425,
     425,   425,   394,   407,   185,   306,   407,   395,   416,   416,
     395,   416,   416,     4,     8,     9,    75,   419,   395,   416,
     307,   416,     5,    75,   318,   300,   416,   398,   263,   394,
     242,   187,   395,   185,     8,   407,   187,   165,   188,   237,
     238,   239,   240,   241,   406,   407,   412,   413,   414,    22,
      24,    28,    29,    30,   117,   172,   185,   189,   395,   398,
     406,   407,   421,   422,   423,   298,   189,   261,   259,   187,
     243,   183,   183,   183,   182,   397,   185,     4,   187,   183,
     183,   312,     3,   187,   336,   334,     3,   187,   328,   183,
     183,   183,   183,   183,   398,   398,   395,   395,   185,   395,
     185,   185,   185,   415,   185,   416,   245,     4,   187,   183,
     183,   185,     4,   183,   183,   183,   183,   183,     4,   183,
     183,   183,     6,   281,   183,   183,   183,   183,     4,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   261,
     301,   183,   183,   188,   407,   394,   183,   277,   186,   186,
     407,   407,   407,   407,   407,   407,   407,   183,   186,   184,
       4,   272,   273,   274,   275,   396,   399,   406,   407,   184,
     184,   184,   262,   260,   407,   393,     3,     3,   185,   358,
       3,   183,   188,   416,   183,   407,    80,    81,   326,     5,
     192,   183,   407,   183,   261,   183,   407,   261,   395,   395,
     416,    74,    74,   183,   183,    74,   320,   416,   395,   395,
       3,     7,   185,   210,   211,   400,   401,   416,   180,   186,
     416,   183,   407,    69,    75,   388,    36,     4,   183,    36,
      38,    75,    77,    78,   389,   392,   395,    83,   183,    83,
     425,    75,   185,   313,   183,   425,   425,   425,   394,   183,
     306,   419,   395,    71,    75,   424,    87,    61,   158,   155,
      93,    75,   185,   314,     4,   387,   302,   185,   400,   400,
     296,   128,   243,   291,   188,    75,   406,   265,   409,   410,
     411,   188,   188,   412,   416,   183,   183,   190,   183,   190,
     183,   190,   183,   190,   189,   404,   189,   402,   189,   400,
     190,   188,     3,   183,   397,   186,     4,   188,   312,   191,
     188,     4,     8,     9,    68,    69,    70,    72,    97,    98,
      99,   101,   102,   103,   104,   106,   124,   125,   133,   134,
     135,   136,   137,   138,   151,   162,   337,   395,   417,   420,
     191,   188,   330,   322,   183,   183,   395,   395,   244,   183,
     186,   183,   183,   183,     3,   187,   183,   183,   183,     4,
     188,   183,   183,     4,   183,   187,   183,     4,   183,     4,
     183,   388,   183,   183,   348,   183,    74,   183,   183,   184,
     353,   183,     4,   183,   183,   116,   183,   261,   243,   407,
     285,   184,   183,   186,     4,   399,   396,   407,   406,   269,
     405,   270,   403,   271,   401,   183,     3,   186,   192,   186,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   186,   183,   183,   387,     5,   183,   320,   416,
     407,   407,   416,   183,   407,   416,   416,   185,   183,   379,
       4,   183,   151,     4,    75,    82,   391,    83,   183,    75,
     185,   315,   191,    83,   394,   183,   406,    62,   395,   183,
      90,   183,    75,   185,   316,    75,   319,   320,   261,   400,
     189,   196,   197,   408,   409,   278,   183,   190,   183,   190,
     183,   190,   388,   183,   404,    96,    95,   394,   394,   394,
     394,   406,   394,   394,   394,   402,    77,    78,   109,   110,
     113,   136,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   331,   390,   392,   406,    72,   100,   101,
     103,   112,   120,   138,   323,   183,   256,   407,   186,   183,
     186,   183,   186,   388,   188,   186,   186,   407,     4,   183,
     380,   183,     4,   184,   188,     4,   325,   183,   183,     4,
      75,   321,   183,   183,    84,    91,   103,   120,   151,   152,
     153,   154,   156,   157,   158,   159,   160,   354,     4,     4,
     380,   183,   183,   130,   132,   286,   287,   261,   405,   403,
     401,   186,     5,   184,   406,   184,   184,   184,   184,   184,
     184,   184,     3,    39,   407,   186,   238,   183,    39,    40,
      41,   381,   382,    94,   383,     4,   183,   402,   186,   183,
     316,   394,   186,    63,    85,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   186,   183,   383,   321,     7,
     419,   184,   190,   286,   183,    75,   394,   394,   394,   150,
     191,   332,   404,     4,   121,   186,   183,   186,   407,     4,
     183,   184,    91,   385,   183,     4,   326,   380,   183,   183,
      90,   386,     4,     4,    85,    87,   121,   355,   402,   404,
     358,   358,   395,   378,     4,     4,   385,   322,   184,   289,
       5,   244,    39,   183,   254,   186,   381,    93,   384,   184,
     367,     4,   186,   186,   383,    85,    84,   186,   309,   288,
     260,   183,   332,   186,   181,   186,   386,   261,   368,   186,
     385,   183,   261,   260,   388,   183,   311,    84,   183,   395,
     261,     5,    75,   257,   183,   255,   256,   395,   186
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
		      long rflags = (yyvsp[(8) - (10)].i);
		      long flmt = (long)(yyvsp[(9) - (10)].i);
		      long flt = (long)(yyvsp[(10) - (10)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      if (flmt == -1) flmt = ROOM;
		      if (flt == -1) flt = 0;

		      add_opvars(splev, "iiiiiiiiio",
				 VA_PASS10(flt, flmt, rflags, ERR, ERR,
					  (yyvsp[(5) - (10)].crd).x, (yyvsp[(5) - (10)].crd).y, (yyvsp[(7) - (10)].sze).width, (yyvsp[(7) - (10)].sze).height,
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
		      long rflags = (yyvsp[(10) - (12)].i);
		      long flmt = (long)(yyvsp[(11) - (12)].i);
		      long flt = (long)(yyvsp[(12) - (12)].i);

		      if (rflags == -1) rflags = (1 << 0);

		      add_opvars(splev, "iiiiiiiiio",
				 VA_PASS10(flt, flmt, rflags,
					  (yyvsp[(7) - (12)].crd).x, (yyvsp[(7) - (12)].crd).y, (yyvsp[(5) - (12)].crd).x, (yyvsp[(5) - (12)].crd).y,
					  (yyvsp[(9) - (12)].sze).width, (yyvsp[(9) - (12)].sze).height, SPO_ROOM));
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
#line 1854 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1860 "lev_comp.y"
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

  case 279:

/* Line 1455 of yacc.c  */
#line 1889 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1894 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1902 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1908 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1914 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1921 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1928 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1938 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1948 "lev_comp.y"
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

  case 288:

/* Line 1455 of yacc.c  */
#line 1963 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1973 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1977 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1983 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1989 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1995 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1999 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2005 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2011 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2018 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2026 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2032 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2037 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2042 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2047 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2052 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2058 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2064 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2069 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2074 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2079 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_CATEGORY));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2084 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2089 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2099 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2105 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2111 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2116 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2123 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2130 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2136 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2142 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2148 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2152 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2158 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2164 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2174 "lev_comp.y"
    {
		      long irr;
		      long rt = (yyvsp[(7) - (10)].i);
		      long rflags = (yyvsp[(8) - (10)].i);
		      long flmt = (long)(yyvsp[(9) - (10)].i);
		      long flt = (long)(yyvsp[(10) - (10)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      if (flmt == -1) flmt = ROOM;
		      if (flt == -1) flt = 0;

		      if (!(rflags & 1)) rt += MAXRTYPE+1;
		      irr = ((rflags & 2) != 0);
		      add_opvars(splev, "iiiiio",
				 VA_PASS6((long)(yyvsp[(5) - (10)].i), rt, rflags, flmt, flt, SPO_REGION));
		      (yyval.i) = (irr || (rflags & 1) || rt != OROOM);
		      break_stmt_start();
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2193 "lev_comp.y"
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

  case 329:

/* Line 1455 of yacc.c  */
#line 2205 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2209 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2215 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2222 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2228 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2234 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2240 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2246 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2250 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2255 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2262 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2268 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2275 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2279 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2286 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2297 "lev_comp.y"
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

  case 347:

/* Line 1455 of yacc.c  */
#line 2310 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2314 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2320 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2324 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2331 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2335 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2339 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2345 "lev_comp.y"
    {
			(yyval.i) = ROOM;
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2349 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2355 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2361 "lev_comp.y"
    {
			(yyval.i) = 0;
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2365 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2371 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2387 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2395 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2405 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2414 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2419 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2426 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2437 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2443 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2447 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2451 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2458 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2468 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2475 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2479 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2485 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2489 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2497 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2501 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2508 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2518 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2531 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2535 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2542 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2552 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2561 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2572 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2576 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2583 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2593 "lev_comp.y"
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

  case 399:

/* Line 1455 of yacc.c  */
#line 2604 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2613 "lev_comp.y"
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

  case 401:

/* Line 1455 of yacc.c  */
#line 2623 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2629 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2633 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2640 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2650 "lev_comp.y"
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

  case 406:

/* Line 1455 of yacc.c  */
#line 2661 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2670 "lev_comp.y"
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

  case 408:

/* Line 1455 of yacc.c  */
#line 2680 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2684 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2690 "lev_comp.y"
    { ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2692 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2698 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2702 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2706 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2710 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2718 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2727 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2731 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2735 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2739 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2743 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2746 "lev_comp.y"
    { ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2750 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2757 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2766 "lev_comp.y"
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

  case 430:

/* Line 1455 of yacc.c  */
#line 2807 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2811 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2818 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2825 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2835 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2839 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2847 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2851 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2855 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2859 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2863 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2868 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2872 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2876 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2881 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2886 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2891 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2895 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2900 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2905 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2909 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2913 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2918 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2922 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2929 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2935 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2939 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2945 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2961 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2965 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2969 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2973 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 2995 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 2999 "lev_comp.y"
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

  case 477:

/* Line 1455 of yacc.c  */
#line 3025 "lev_comp.y"
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
#line 6824 "lev_comp.tab.c"
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
#line 3053 "lev_comp.y"


/*lev_comp.y*/

