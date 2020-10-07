
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
     CHARGES_ID = 355,
     SPECIAL_QUALITY_ID = 356,
     SPEFLAGS_ID = 357,
     SUBROOM_ID = 358,
     NAME_ID = 359,
     FLAGS_ID = 360,
     FLAG_TYPE = 361,
     MON_ATTITUDE = 362,
     MON_ALERTNESS = 363,
     SUBTYPE_ID = 364,
     MON_APPEARANCE = 365,
     ROOMDOOR_ID = 366,
     IF_ID = 367,
     ELSE_ID = 368,
     TERRAIN_ID = 369,
     HORIZ_OR_VERT = 370,
     REPLACE_TERRAIN_ID = 371,
     LOCATION_SUBTYPE_ID = 372,
     DOOR_SUBTYPE = 373,
     EXIT_ID = 374,
     SHUFFLE_ID = 375,
     QUANTITY_ID = 376,
     BURIED_ID = 377,
     LOOP_ID = 378,
     FOR_ID = 379,
     TO_ID = 380,
     SWITCH_ID = 381,
     CASE_ID = 382,
     BREAK_ID = 383,
     DEFAULT_ID = 384,
     ERODED_ID = 385,
     TRAPPED_STATE = 386,
     RECHARGED_ID = 387,
     INVIS_ID = 388,
     GREASED_ID = 389,
     INDESTRUCTIBLE_ID = 390,
     FEMALE_ID = 391,
     WAITFORU_ID = 392,
     CANCELLED_ID = 393,
     REVIVED_ID = 394,
     AVENGE_ID = 395,
     FLEEING_ID = 396,
     BLINDED_ID = 397,
     PARALYZED_ID = 398,
     STUNNED_ID = 399,
     CONFUSED_ID = 400,
     SEENTRAPS_ID = 401,
     ALL_ID = 402,
     MONTYPE_ID = 403,
     OBJTYPE_ID = 404,
     TERTYPE_ID = 405,
     TERTYPE2_ID = 406,
     LEVER_EFFECT_TYPE = 407,
     SWITCHABLE_ID = 408,
     CONTINUOUSLY_USABLE_ID = 409,
     TARGET_ID = 410,
     TRAPTYPE_ID = 411,
     EFFECT_FLAG_ID = 412,
     GRAVE_ID = 413,
     ERODEPROOF_ID = 414,
     FUNCTION_ID = 415,
     MSG_OUTPUT_TYPE = 416,
     COMPARE_TYPE = 417,
     UNKNOWN_TYPE = 418,
     rect_ID = 419,
     fillrect_ID = 420,
     line_ID = 421,
     randline_ID = 422,
     grow_ID = 423,
     selection_ID = 424,
     flood_ID = 425,
     rndcoord_ID = 426,
     circle_ID = 427,
     ellipse_ID = 428,
     filter_ID = 429,
     complement_ID = 430,
     gradient_ID = 431,
     GRADIENT_TYPE = 432,
     LIMITED = 433,
     HUMIDITY_TYPE = 434,
     STRING = 435,
     MAP_ID = 436,
     NQSTRING = 437,
     VARSTRING = 438,
     CFUNC = 439,
     CFUNC_INT = 440,
     CFUNC_STR = 441,
     CFUNC_COORD = 442,
     CFUNC_REGION = 443,
     VARSTRING_INT = 444,
     VARSTRING_INT_ARRAY = 445,
     VARSTRING_STRING = 446,
     VARSTRING_STRING_ARRAY = 447,
     VARSTRING_VAR = 448,
     VARSTRING_VAR_ARRAY = 449,
     VARSTRING_COORD = 450,
     VARSTRING_COORD_ARRAY = 451,
     VARSTRING_REGION = 452,
     VARSTRING_REGION_ARRAY = 453,
     VARSTRING_MAPCHAR = 454,
     VARSTRING_MAPCHAR_ARRAY = 455,
     VARSTRING_MONST = 456,
     VARSTRING_MONST_ARRAY = 457,
     VARSTRING_OBJ = 458,
     VARSTRING_OBJ_ARRAY = 459,
     VARSTRING_SEL = 460,
     VARSTRING_SEL_ARRAY = 461,
     METHOD_INT = 462,
     METHOD_INT_ARRAY = 463,
     METHOD_STRING = 464,
     METHOD_STRING_ARRAY = 465,
     METHOD_VAR = 466,
     METHOD_VAR_ARRAY = 467,
     METHOD_COORD = 468,
     METHOD_COORD_ARRAY = 469,
     METHOD_REGION = 470,
     METHOD_REGION_ARRAY = 471,
     METHOD_MAPCHAR = 472,
     METHOD_MAPCHAR_ARRAY = 473,
     METHOD_MONST = 474,
     METHOD_MONST_ARRAY = 475,
     METHOD_OBJ = 476,
     METHOD_OBJ_ARRAY = 477,
     METHOD_SEL = 478,
     METHOD_SEL_ARRAY = 479,
     DICE = 480
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
#line 523 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 535 "lev_comp.tab.c"

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
#define YYLAST   1203

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  243
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  181
/* YYNRULES -- Number of rules.  */
#define YYNRULES  473
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1045

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   480

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   238,   242,     2,
     182,   183,   236,   234,   180,   235,   240,   237,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   181,     2,
       2,   239,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   184,     2,   185,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   186,   241,   187,     2,     2,     2,     2,
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
     175,   176,   177,   178,   179,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233
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
     660,   662,   664,   672,   678,   680,   682,   684,   686,   690,
     691,   697,   702,   703,   707,   709,   711,   713,   715,   718,
     720,   722,   724,   726,   728,   732,   736,   740,   742,   744,
     748,   750,   752,   754,   758,   762,   763,   769,   772,   773,
     777,   779,   783,   785,   789,   793,   795,   797,   801,   803,
     805,   807,   811,   813,   815,   817,   821,   825,   829,   833,
     837,   841,   845,   847,   853,   861,   867,   876,   878,   882,
     888,   894,   902,   910,   917,   923,   924,   927,   933,   937,
     949,   963,   970,   971,   975,   977,   981,   985,   989,   993,
     995,   997,  1001,  1005,  1009,  1013,  1017,  1021,  1023,  1025,
    1027,  1029,  1033,  1037,  1039,  1045,  1055,  1061,  1065,  1069,
    1075,  1085,  1091,  1097,  1098,  1111,  1112,  1114,  1122,  1126,
    1134,  1140,  1146,  1152,  1158,  1162,  1168,  1176,  1186,  1188,
    1190,  1192,  1194,  1196,  1197,  1200,  1202,  1206,  1208,  1210,
    1212,  1213,  1217,  1219,  1220,  1224,  1226,  1228,  1230,  1232,
    1234,  1236,  1238,  1240,  1242,  1244,  1248,  1250,  1252,  1257,
    1259,  1261,  1266,  1268,  1270,  1275,  1277,  1282,  1288,  1290,
    1294,  1296,  1300,  1302,  1304,  1309,  1319,  1321,  1323,  1328,
    1330,  1336,  1338,  1340,  1345,  1347,  1349,  1355,  1357,  1359,
    1361,  1366,  1368,  1370,  1376,  1378,  1380,  1382,  1386,  1388,
    1390,  1394,  1396,  1401,  1405,  1409,  1413,  1417,  1421,  1425,
    1427,  1429,  1433,  1435,  1439,  1440,  1442,  1444,  1446,  1448,
    1452,  1453,  1455,  1457,  1460,  1463,  1468,  1475,  1480,  1487,
    1494,  1501,  1508,  1511,  1518,  1527,  1536,  1547,  1562,  1565,
    1567,  1571,  1573,  1577,  1579,  1581,  1583,  1585,  1587,  1589,
    1591,  1593,  1595,  1597,  1599,  1601,  1603,  1605,  1607,  1609,
    1611,  1613,  1615,  1626
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     244,     0,    -1,    -1,   245,    -1,   246,    -1,   246,   245,
      -1,   247,   255,   257,    -1,    16,   181,   188,    -1,    15,
     181,   188,   180,     3,    -1,    17,   181,    11,   180,   355,
      -1,    17,   181,    10,   180,     3,    -1,    17,   181,    13,
      -1,    17,   181,    12,   180,     3,   180,     3,   180,     5,
     180,     5,   180,   385,   180,   254,   253,    -1,    18,   181,
     415,    -1,    21,   181,    93,    -1,    -1,   180,   178,    -1,
      -1,   180,   392,    -1,    -1,   180,     3,    -1,     5,    -1,
      75,    -1,    -1,   105,   181,   256,    -1,   106,   180,   256,
      -1,   106,    -1,    -1,   259,   257,    -1,   186,   257,   187,
      -1,   300,    -1,   248,    -1,   249,    -1,   250,    -1,   369,
      -1,   368,    -1,   366,    -1,   367,    -1,   370,    -1,   371,
      -1,   344,    -1,   302,    -1,   265,    -1,   264,    -1,   358,
      -1,   314,    -1,   336,    -1,   373,    -1,   374,    -1,   346,
      -1,   347,    -1,   348,    -1,   372,    -1,   280,    -1,   290,
      -1,   292,    -1,   296,    -1,   294,    -1,   277,    -1,   287,
      -1,   273,    -1,   276,    -1,   339,    -1,   349,    -1,   321,
      -1,   337,    -1,   324,    -1,   330,    -1,   359,    -1,   354,
      -1,   342,    -1,   301,    -1,   360,    -1,   361,    -1,   362,
      -1,   363,    -1,   307,    -1,   305,    -1,   353,    -1,   357,
      -1,   356,    -1,   340,    -1,   341,    -1,   343,    -1,   335,
      -1,   338,    -1,   198,    -1,   200,    -1,   202,    -1,   204,
      -1,   206,    -1,   208,    -1,   210,    -1,   212,    -1,   214,
      -1,   197,    -1,   199,    -1,   201,    -1,   203,    -1,   205,
      -1,   207,    -1,   209,    -1,   211,    -1,   213,    -1,   260,
      -1,   261,    -1,   191,    -1,   191,    -1,   261,    -1,   120,
     181,   260,    -1,   262,   239,   404,    -1,   262,   239,   169,
     181,   413,    -1,   262,   239,   403,    -1,   262,   239,   420,
     181,   397,    -1,   262,   239,   419,   181,   399,    -1,   262,
     239,   418,   181,   401,    -1,   262,   239,   392,    -1,   262,
     239,   395,    -1,   262,   239,   186,   271,   187,    -1,   262,
     239,   186,   270,   187,    -1,   262,   239,   186,   269,   187,
      -1,   262,   239,   420,   181,   186,   268,   187,    -1,   262,
     239,   419,   181,   186,   267,   187,    -1,   262,   239,   418,
     181,   186,   266,   187,    -1,   262,   239,   186,   272,   187,
      -1,   402,    -1,   266,   180,   402,    -1,   400,    -1,   267,
     180,   400,    -1,   398,    -1,   268,   180,   398,    -1,   396,
      -1,   269,   180,   396,    -1,   393,    -1,   270,   180,   393,
      -1,   404,    -1,   271,   180,   404,    -1,   403,    -1,   272,
     180,   403,    -1,    -1,    -1,   160,   190,   182,   274,   408,
     183,   275,   258,    -1,   190,   182,   411,   183,    -1,   119,
      -1,    -1,     6,    -1,     6,    -1,   184,   404,   162,   404,
     185,    -1,   184,   404,   185,    -1,    -1,    -1,   126,   281,
     184,   391,   185,   282,   186,   283,   187,    -1,    -1,   284,
     283,    -1,    -1,   127,   416,   181,   285,   257,    -1,    -1,
     129,   181,   286,   257,    -1,   128,    -1,   240,   240,    -1,
     125,    -1,   124,   263,   239,   404,   288,   404,    -1,    -1,
     289,   291,   258,    -1,    -1,   123,   184,   391,   185,   293,
     258,    -1,    -1,   279,   181,   295,   259,    -1,    -1,   112,
     279,   297,   298,    -1,   258,    -1,    -1,   258,   299,   113,
     258,    -1,    14,   181,   403,    -1,    67,    -1,    67,   181,
     416,    -1,    67,   181,    75,    -1,    55,   181,   303,   180,
     303,    -1,    55,   181,   303,   180,   416,    -1,   182,     4,
     180,    74,   180,   318,   183,    -1,   376,   278,   180,   385,
      -1,    -1,   103,   181,   304,   180,   311,   180,   313,   377,
     380,   382,   306,   258,    -1,    -1,    49,   181,   304,   180,
     310,   180,   312,   180,   313,   377,   380,   382,   308,   258,
      -1,    -1,   180,     5,    -1,   182,     4,   180,     4,   183,
      -1,    75,    -1,   182,     4,   180,     4,   183,    -1,    75,
      -1,   182,   322,   180,   323,   183,    -1,    75,    -1,   182,
       4,   180,     4,   183,    -1,    75,    -1,   111,   181,   315,
     180,   384,   180,   316,   180,   318,   319,    -1,    26,   181,
     384,   180,   413,   319,    -1,     5,    -1,    75,    -1,   317,
      -1,    75,    -1,    74,    -1,    74,   241,   317,    -1,     4,
      -1,    75,    -1,    -1,   319,   180,   320,    -1,   117,   181,
     118,    -1,    72,   181,   401,    -1,   101,   181,     4,    -1,
     135,    -1,    20,    -1,    19,   181,   322,   180,   323,   309,
     189,    -1,    19,   181,   392,   309,   189,    -1,    79,    -1,
      80,    -1,    81,    -1,    80,    -1,    24,   181,   326,    -1,
      -1,    24,   181,   326,   325,   258,    -1,   399,   180,   392,
     327,    -1,    -1,   327,   180,   328,    -1,   403,    -1,   107,
      -1,   108,    -1,   387,    -1,   110,   403,    -1,   136,    -1,
     133,    -1,   138,    -1,   139,    -1,   140,    -1,   141,   181,
     391,    -1,   142,   181,   391,    -1,   143,   181,   391,    -1,
     144,    -1,   145,    -1,   146,   181,   329,    -1,   137,    -1,
     188,    -1,   147,    -1,   188,   241,   329,    -1,    22,   181,
     332,    -1,    -1,    23,   181,   332,   331,   258,    -1,   401,
     333,    -1,    -1,   333,   180,   334,    -1,    70,    -1,   148,
     181,   399,    -1,   417,    -1,   104,   181,   403,    -1,   121,
     181,   391,    -1,   122,    -1,    69,    -1,   130,   181,   391,
      -1,   159,    -1,    68,    -1,   131,    -1,   132,   181,   391,
      -1,   133,    -1,   134,    -1,   392,    -1,    98,   181,    95,
      -1,    97,   181,    96,    -1,    99,   181,   391,    -1,   100,
     181,   391,    -1,   101,   181,   391,    -1,   102,   181,   391,
      -1,    72,   181,   401,    -1,   135,    -1,    25,   181,   375,
     180,   392,    -1,    27,   181,   392,   180,    74,   180,   384,
      -1,    31,   181,   392,   180,    74,    -1,    31,   181,   392,
     180,    74,   180,     5,   253,    -1,    32,    -1,    32,   181,
     413,    -1,    45,   181,   392,   180,    83,    -1,    46,   181,
     392,   180,    83,    -1,    46,   181,   422,   180,   422,   180,
      83,    -1,    50,   181,   422,   180,   422,   180,   188,    -1,
      51,   181,   422,   180,   422,   345,    -1,    52,   181,   422,
     180,   422,    -1,    -1,   180,    83,    -1,    58,   181,   413,
     180,    87,    -1,    59,   181,   413,    -1,    60,   181,   392,
     180,   155,   181,   392,   180,    85,   180,    84,    -1,    60,
     181,   392,   180,    61,   180,    62,   180,    63,   180,    85,
     180,    84,    -1,    73,   181,   392,   180,   152,   350,    -1,
      -1,   350,   180,   351,    -1,    84,    -1,   148,   181,   399,
      -1,   149,   181,   401,    -1,   150,   181,   355,    -1,   151,
     181,   355,    -1,   153,    -1,   154,    -1,   155,   181,   392,
      -1,   156,   181,   375,    -1,   117,   181,   352,    -1,    91,
     181,   383,    -1,   157,   181,     4,    -1,   101,   181,     4,
      -1,     4,    -1,    85,    -1,   118,    -1,    87,    -1,    65,
     181,   413,    -1,    64,   181,   413,    -1,     3,    -1,   182,
       3,   180,   385,   183,    -1,   116,   181,   395,   180,   397,
     180,   397,   180,     7,    -1,   114,   181,   413,   180,   397,
      -1,    47,   181,   395,    -1,    48,   181,   395,    -1,    37,
     181,   188,   180,    38,    -1,    37,   181,   188,   180,    38,
     180,   148,   181,   399,    -1,    34,   181,   395,   180,    36,
      -1,    35,   181,   422,   180,    36,    -1,    -1,    33,   181,
     395,   180,   385,   180,   376,   377,   380,   382,   364,   365,
      -1,    -1,   258,    -1,    42,   181,   392,   180,   386,   180,
     388,    -1,    43,   181,   392,    -1,    92,   181,   413,   180,
      93,   180,    90,    -1,   109,   181,   413,   180,     4,    -1,
      44,   181,    86,   180,   392,    -1,   158,   181,   392,   180,
     403,    -1,   158,   181,   392,   180,    75,    -1,   158,   181,
     392,    -1,    56,   181,   404,   180,   392,    -1,    57,   181,
     392,   180,   421,   180,   403,    -1,    54,   181,   391,   180,
     391,   180,   391,   180,   391,    -1,    54,    -1,   188,    -1,
      75,    -1,   188,    -1,    75,    -1,    -1,   180,   378,    -1,
     379,    -1,   379,   180,   378,    -1,    39,    -1,    40,    -1,
      41,    -1,    -1,    94,   181,   381,    -1,    93,    -1,    -1,
      91,   181,   383,    -1,    90,    -1,    68,    -1,    75,    -1,
      69,    -1,    75,    -1,    78,    -1,   389,    -1,    75,    -1,
      78,    -1,   389,    -1,    77,   181,    75,    -1,    82,    -1,
      75,    -1,    77,   184,     4,   185,    -1,   188,    -1,   199,
      -1,   200,   184,   404,   185,    -1,   404,    -1,   393,    -1,
     171,   182,   413,   183,    -1,   203,    -1,   204,   184,   404,
     185,    -1,   182,     4,   180,     4,   183,    -1,    75,    -1,
      76,   394,   185,    -1,   179,    -1,   179,   180,   394,    -1,
     396,    -1,   205,    -1,   206,   184,   404,   185,    -1,   182,
       4,   180,     4,   180,     4,   180,     4,   183,    -1,   398,
      -1,   207,    -1,   208,   184,   404,   185,    -1,     3,    -1,
     182,     3,   180,   385,   183,    -1,   400,    -1,   209,    -1,
     210,   184,   404,   185,    -1,   188,    -1,     3,    -1,   182,
       3,   180,   188,   183,    -1,    75,    -1,   402,    -1,   211,
      -1,   212,   184,   404,   185,    -1,   188,    -1,     3,    -1,
     182,     3,   180,   188,   183,    -1,    75,    -1,    88,    -1,
     390,    -1,   403,   240,   390,    -1,     4,    -1,   414,    -1,
     182,     8,   183,    -1,   197,    -1,   198,   184,   404,   185,
      -1,   404,   234,   404,    -1,   404,   235,   404,    -1,   404,
     236,   404,    -1,   404,   237,   404,    -1,   404,   238,   404,
      -1,   182,   404,   183,    -1,   193,    -1,   194,    -1,   262,
     181,   405,    -1,   406,    -1,   407,   180,   406,    -1,    -1,
     407,    -1,   404,    -1,   403,    -1,   409,    -1,   410,   180,
     409,    -1,    -1,   410,    -1,   392,    -1,   164,   395,    -1,
     165,   395,    -1,   166,   392,   180,   392,    -1,   167,   392,
     180,   392,   180,   404,    -1,   168,   182,   413,   183,    -1,
     168,   182,   317,   180,   413,   183,    -1,   174,   182,     7,
     180,   413,   183,    -1,   174,   182,   413,   180,   413,   183,
      -1,   174,   182,   397,   180,   413,   183,    -1,   170,   392,
      -1,   172,   182,   392,   180,   404,   183,    -1,   172,   182,
     392,   180,   404,   180,    39,   183,    -1,   173,   182,   392,
     180,   404,   180,   404,   183,    -1,   173,   182,   392,   180,
     404,   180,   404,   180,    39,   183,    -1,   176,   182,   177,
     180,   182,   404,   235,   404,   251,   183,   180,   392,   252,
     183,    -1,   175,   412,    -1,   213,    -1,   182,   413,   183,
      -1,   412,    -1,   412,   242,   413,    -1,   233,    -1,    89,
      -1,     4,    -1,     8,    -1,     9,    -1,     4,    -1,     8,
      -1,     9,    -1,     4,    -1,   414,    -1,    28,    -1,    22,
      -1,    29,    -1,    24,    -1,    30,    -1,   114,    -1,    71,
      -1,    75,    -1,   423,    -1,    53,   182,     4,   180,     4,
     180,     4,   180,     4,   183,    -1,   182,     4,   180,     4,
     180,     4,   180,     4,   183,    -1
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
    1461,  1469,  1476,  1484,  1492,  1493,  1496,  1497,  1500,  1505,
    1504,  1518,  1525,  1532,  1540,  1545,  1551,  1557,  1563,  1569,
    1574,  1579,  1584,  1589,  1594,  1599,  1604,  1609,  1614,  1619,
    1625,  1632,  1641,  1645,  1658,  1667,  1666,  1684,  1694,  1700,
    1708,  1714,  1719,  1724,  1729,  1734,  1739,  1744,  1749,  1754,
    1765,  1771,  1776,  1781,  1786,  1791,  1796,  1801,  1806,  1811,
    1816,  1821,  1826,  1833,  1839,  1868,  1873,  1881,  1887,  1893,
    1900,  1907,  1917,  1927,  1942,  1953,  1956,  1962,  1968,  1974,
    1978,  1984,  1991,  1997,  2005,  2011,  2016,  2021,  2026,  2031,
    2037,  2043,  2048,  2053,  2058,  2063,  2068,  2075,  2075,  2075,
    2075,  2078,  2084,  2090,  2095,  2102,  2109,  2115,  2121,  2127,
    2131,  2137,  2143,  2154,  2153,  2185,  2188,  2194,  2201,  2207,
    2213,  2219,  2225,  2229,  2234,  2241,  2247,  2254,  2258,  2265,
    2273,  2276,  2286,  2290,  2293,  2299,  2303,  2310,  2314,  2318,
    2325,  2328,  2334,  2341,  2344,  2350,  2356,  2357,  2360,  2361,
    2364,  2365,  2366,  2372,  2373,  2374,  2380,  2381,  2384,  2393,
    2398,  2405,  2416,  2422,  2426,  2430,  2437,  2447,  2454,  2458,
    2464,  2468,  2476,  2480,  2487,  2497,  2510,  2514,  2521,  2531,
    2540,  2551,  2555,  2562,  2572,  2583,  2592,  2602,  2608,  2612,
    2619,  2629,  2640,  2649,  2659,  2663,  2670,  2671,  2677,  2681,
    2685,  2689,  2697,  2706,  2710,  2714,  2718,  2722,  2726,  2729,
    2736,  2745,  2778,  2779,  2782,  2783,  2786,  2790,  2797,  2804,
    2815,  2818,  2826,  2830,  2834,  2838,  2842,  2847,  2851,  2855,
    2860,  2865,  2870,  2874,  2879,  2884,  2888,  2892,  2897,  2901,
    2908,  2914,  2918,  2924,  2931,  2932,  2935,  2936,  2937,  2940,
    2944,  2948,  2952,  2958,  2959,  2962,  2963,  2966,  2967,  2970,
    2971,  2974,  2978,  3004
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
  "ELEMENTAL_ENCHANTMENT_ID", "ENCHANTMENT_ID", "CHARGES_ID",
  "SPECIAL_QUALITY_ID", "SPEFLAGS_ID", "SUBROOM_ID", "NAME_ID", "FLAGS_ID",
  "FLAG_TYPE", "MON_ATTITUDE", "MON_ALERTNESS", "SUBTYPE_ID",
  "MON_APPEARANCE", "ROOMDOOR_ID", "IF_ID", "ELSE_ID", "TERRAIN_ID",
  "HORIZ_OR_VERT", "REPLACE_TERRAIN_ID", "LOCATION_SUBTYPE_ID",
  "DOOR_SUBTYPE", "EXIT_ID", "SHUFFLE_ID", "QUANTITY_ID", "BURIED_ID",
  "LOOP_ID", "FOR_ID", "TO_ID", "SWITCH_ID", "CASE_ID", "BREAK_ID",
  "DEFAULT_ID", "ERODED_ID", "TRAPPED_STATE", "RECHARGED_ID", "INVIS_ID",
  "GREASED_ID", "INDESTRUCTIBLE_ID", "FEMALE_ID", "WAITFORU_ID",
  "CANCELLED_ID", "REVIVED_ID", "AVENGE_ID", "FLEEING_ID", "BLINDED_ID",
  "PARALYZED_ID", "STUNNED_ID", "CONFUSED_ID", "SEENTRAPS_ID", "ALL_ID",
  "MONTYPE_ID", "OBJTYPE_ID", "TERTYPE_ID", "TERTYPE2_ID",
  "LEVER_EFFECT_TYPE", "SWITCHABLE_ID", "CONTINUOUSLY_USABLE_ID",
  "TARGET_ID", "TRAPTYPE_ID", "EFFECT_FLAG_ID", "GRAVE_ID",
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
  "tileset_detail", "boundary_type_detail", "opt_limited",
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
      44,    58,    40,    41,    91,    93,   123,   125,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,    43,    45,    42,    47,    37,    61,
      46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   243,   244,   244,   245,   245,   246,   247,   247,   248,
     248,   248,   248,   249,   250,   251,   251,   252,   252,   253,
     253,   254,   254,   255,   255,   256,   256,   257,   257,   258,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   262,   262,   262,   263,   263,   264,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   266,   266,   267,   267,   268,
     268,   269,   269,   270,   270,   271,   271,   272,   272,   274,
     275,   273,   276,   277,   278,   278,   279,   279,   279,   281,
     282,   280,   283,   283,   285,   284,   286,   284,   287,   288,
     288,   289,   291,   290,   293,   292,   295,   294,   297,   296,
     298,   299,   298,   300,   301,   301,   301,   302,   302,   303,
     304,   306,   305,   308,   307,   309,   309,   310,   310,   311,
     311,   312,   312,   313,   313,   314,   314,   315,   315,   316,
     316,   317,   317,   318,   318,   319,   319,   320,   320,   320,
     320,   321,   321,   321,   322,   322,   323,   323,   324,   325,
     324,   326,   327,   327,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   329,   329,   329,   330,   331,   330,   332,   333,   333,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   335,   336,   337,   337,   338,   338,   339,
     340,   341,   342,   343,   344,   345,   345,   346,   347,   348,
     348,   349,   350,   350,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   352,   352,   352,
     352,   353,   354,   355,   355,   356,   357,   358,   359,   360,
     360,   361,   362,   364,   363,   365,   365,   366,   367,   368,
     369,   370,   371,   371,   371,   372,   373,   374,   374,   375,
     375,   376,   376,   377,   377,   378,   378,   379,   379,   379,
     380,   380,   381,   382,   382,   383,   384,   384,   385,   385,
     386,   386,   386,   387,   387,   387,   388,   388,   389,   390,
     390,   390,   391,   392,   392,   392,   392,   393,   393,   393,
     394,   394,   395,   395,   395,   396,   397,   397,   397,   398,
     398,   399,   399,   399,   400,   400,   400,   400,   401,   401,
     401,   402,   402,   402,   402,   402,   403,   403,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   405,
     405,   406,   407,   407,   408,   408,   409,   409,   410,   410,
     411,   411,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   413,   413,   414,   415,   415,   416,   416,   416,   417,
     417,   417,   417,   418,   418,   419,   419,   420,   420,   421,
     421,   422,   422,   423
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
       1,     1,     7,     5,     1,     1,     1,     1,     3,     0,
       5,     4,     0,     3,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     3,     3,     3,     1,     1,     3,
       1,     1,     1,     3,     3,     0,     5,     2,     0,     3,
       1,     3,     1,     3,     3,     1,     1,     3,     1,     1,
       1,     3,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     1,     5,     7,     5,     8,     1,     3,     5,
       5,     7,     7,     6,     5,     0,     2,     5,     3,    11,
      13,     6,     0,     3,     1,     3,     3,     3,     3,     1,
       1,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     3,     3,     1,     5,     9,     5,     3,     3,     5,
       9,     5,     5,     0,    12,     0,     1,     7,     3,     7,
       5,     5,     5,     5,     3,     5,     7,     9,     1,     1,
       1,     1,     1,     0,     2,     1,     3,     1,     1,     1,
       0,     3,     1,     0,     3,     1,     1,     1,     1,     1,
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
       2,     0,     0,     0,     3,     4,    23,     0,     0,     1,
       5,     0,    27,     0,     7,     0,   146,     0,     0,     0,
       0,   211,     0,     0,     0,     0,     0,     0,     0,     0,
     277,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   338,     0,     0,     0,
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
       0,     0,     0,   408,     0,   411,     0,   453,     0,   409,
     430,    28,     0,   166,     0,     8,     0,   369,   370,     0,
     406,   173,     0,     0,     0,    11,   455,   454,    13,   378,
       0,   214,   215,     0,     0,   375,     0,     0,   185,   373,
      14,   402,   404,   405,     0,   401,   399,     0,   244,   248,
     398,   245,   395,   397,     0,   394,   392,     0,   218,     0,
     391,   340,   339,     0,   356,   357,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   449,   432,   451,   278,     0,   383,     0,     0,   382,
       0,     0,     0,     0,   471,     0,     0,   328,     0,     0,
       0,     0,     0,   317,   318,   342,   341,     0,   144,     0,
       0,     0,     0,   372,     0,     0,     0,     0,     0,   288,
       0,   312,   311,   458,   456,   457,   176,   175,     0,     0,
       0,     0,   197,   198,     0,     0,     0,     0,   109,     0,
       0,     0,   334,   139,     0,     0,     0,     0,   148,     0,
       0,     0,     0,     0,   427,   426,   428,   431,     0,   464,
     466,   463,   465,   467,   468,     0,     0,     0,   116,   117,
     112,   110,     0,     0,     0,     0,    27,   163,    25,     0,
       0,     0,     0,     0,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   247,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,   434,     0,     0,     0,   442,
       0,     0,     0,   448,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   170,
     169,     0,     0,   164,     0,     0,     0,   424,   410,   418,
       0,     0,   413,   414,   415,   416,   417,     0,   142,     0,
     408,     0,     0,     0,     0,   133,   131,   137,   135,     0,
       0,     0,   167,     0,     0,   407,    10,   313,     0,     9,
       0,     0,   379,     0,     0,     0,   217,   216,   185,   186,
     213,     0,     0,     0,   246,     0,     0,   220,   222,   273,
     205,     0,   275,     0,     0,   201,     0,     0,     0,     0,
     389,     0,     0,   387,     0,     0,   386,     0,     0,   450,
     452,     0,     0,   358,   359,     0,   321,     0,     0,   322,
     319,   362,     0,   360,     0,   361,   331,   279,     0,   280,
       0,   188,     0,     0,     0,     0,   285,   284,     0,     0,
     177,   178,   335,   469,   470,     0,   287,     0,     0,   292,
       0,   190,     0,     0,   330,     0,     0,     0,   316,     0,
       0,   160,     0,     0,   150,   333,   332,     0,   422,   425,
       0,   412,   147,   429,   111,     0,     0,   120,     0,   119,
       0,   118,     0,   124,     0,   115,     0,   114,     0,   113,
      29,   371,     0,     0,   381,   374,     0,   376,     0,     0,
     400,   461,   459,   460,   259,   256,   250,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   255,     0,   260,     0,
     262,   263,   272,     0,   258,   249,   264,   462,   252,     0,
     393,   221,   196,     0,     0,   435,     0,     0,     0,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   384,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     180,     0,     0,   283,     0,     0,     0,     0,     0,   291,
       0,     0,     0,     0,     0,     0,   165,   159,   161,     0,
       0,     0,   140,     0,   132,   134,   136,   138,     0,   125,
       0,   127,     0,   129,     0,     0,   377,   212,   403,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   396,     0,     0,   274,    19,     0,   202,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   343,     0,
       0,     0,     0,   367,   366,   327,   281,     0,   192,     0,
       0,   282,   286,     0,     0,   336,     0,     0,     0,   329,
       0,   194,     0,   343,   200,     0,   199,   172,     0,   152,
     419,   420,   421,   423,     0,     0,   123,     0,   122,     0,
     121,     0,     0,   271,   266,   265,   267,   268,   269,   270,
     253,   254,   257,   261,   251,     0,   363,   225,   226,     0,
     230,   229,   240,   231,   232,   233,     0,     0,     0,   237,
     238,     0,   223,   227,   364,   224,     0,     0,     0,   210,
     206,     0,   276,   436,   438,     0,   443,     0,   439,     0,
     388,   441,   440,     0,     0,     0,   350,     0,     0,     0,
     368,     0,     0,     0,     0,   203,   204,     0,     0,     0,
     294,     0,     0,     0,     0,     0,     0,     0,   299,   300,
       0,     0,     0,   293,     0,     0,   350,     0,     0,     0,
       0,     0,   152,   141,   126,   128,   130,   314,     0,     0,
     228,     0,     0,     0,     0,     0,     0,     0,    20,     0,
       0,   390,     0,     0,   347,   348,   349,   344,   345,     0,
     353,     0,     0,   320,   187,     0,   343,   337,   179,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   189,     0,   353,   205,   315,     0,   156,   151,   153,
       0,   365,   234,   235,   236,   242,   241,   239,   208,   209,
     207,   444,     0,   445,   414,     0,     0,     0,     0,   323,
       0,     0,     0,   350,     0,     0,   355,   304,   306,   307,
     308,   310,   309,   303,   295,   296,   297,   298,   301,   302,
     305,     0,   181,   195,   154,    27,     0,     0,     0,     0,
       0,   385,   346,   352,   351,     0,   325,     0,   473,   191,
     353,     0,   289,   193,     0,    27,   157,     0,   243,   446,
      16,     0,   354,   326,   324,   472,   183,     0,   182,   155,
       0,     0,     0,   290,     0,    17,   184,    21,    22,    19,
       0,     0,    12,    18,   447
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    93,    94,    95,  1000,  1041,
     842,  1039,    12,   158,    96,   387,    97,    98,    99,   100,
     209,   101,   102,   718,   720,   722,   481,   482,   483,   484,
     103,   467,   794,   104,   105,   443,   106,   107,   210,   709,
     891,   892,  1015,   995,   108,   593,   109,   110,   224,   111,
     590,   112,   385,   113,   345,   460,   586,   114,   115,   116,
     325,   317,   117,  1014,   118,  1032,   401,   563,   583,   770,
     783,   119,   344,   785,   526,   867,   662,   840,   120,   247,
     508,   121,   408,   268,   661,   832,   957,   122,   405,   258,
     404,   655,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   693,   133,   134,   135,   136,   699,   883,   983,
     137,   138,   499,   139,   140,   141,   142,   143,   144,   145,
     146,  1006,  1024,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   273,   318,   856,   917,   918,   920,  1004,   969,
     977,   276,   545,   554,   833,   765,   555,   230,   322,   292,
     249,   395,   298,   299,   535,   536,   269,   270,   259,   260,
     364,   323,   792,   598,   599,   600,   366,   367,   368,   293,
     425,   219,   238,   337,   658,   382,   383,   384,   575,   303,
     304
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -875
static const yytype_int16 yypact[] =
{
      93,  -120,   -56,   157,  -875,    93,    60,     3,    28,  -875,
    -875,    63,   702,    50,  -875,   140,  -875,    73,    78,   100,
     104,  -875,   132,   138,   148,   161,   164,   171,   173,   184,
     186,   189,   190,   193,   199,   201,   208,   212,   243,   257,
     258,   263,   264,   265,   268,   269,   279,   280,   281,   282,
     285,   286,   291,   292,   299,   307,   308,   315,   316,   319,
     320,    34,   321,   323,  -875,   325,    91,   769,  -875,  -875,
     337,   131,    55,   153,  -875,  -875,  -875,  -875,  -875,  -875,
    -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,
    -875,  -875,  -875,  -875,  -875,  -875,  -875,   702,  -875,  -875,
     203,  -875,  -875,  -875,  -875,  -875,   340,  -875,  -875,  -875,
    -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,
    -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,
    -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,
    -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,
    -875,  -875,  -875,  -875,  -875,  -875,   495,   342,  -875,   -65,
     396,    94,   415,   432,   335,   335,   231,   -21,    13,   -18,
     -18,   478,   -99,   -99,   -24,   338,   -18,   -18,   459,   -18,
      11,   -99,   -99,   -19,   -24,   -24,   -24,    55,   362,    55,
     -18,   478,   478,   -18,   478,   478,    95,   -18,   478,   -19,
     478,    65,  -875,   478,   -99,   763,    55,  -875,  -875,   309,
     366,   -18,   373,  -875,    89,  -875,   374,  -875,   219,  -875,
     158,  -875,   197,  -875,   371,  -875,   140,  -875,  -875,   380,
    -875,   334,   398,   399,   400,  -875,  -875,  -875,  -875,  -875,
     397,  -875,  -875,   393,   577,  -875,   401,   403,   404,  -875,
    -875,  -875,  -875,  -875,   584,  -875,  -875,   405,  -875,  -875,
    -875,  -875,  -875,  -875,   588,  -875,  -875,   416,   423,   421,
    -875,  -875,  -875,   422,  -875,  -875,   430,   433,   434,   -99,
     -99,   -18,   -18,   435,   -18,   438,   440,   441,   478,   452,
     395,  -875,  -875,   379,  -875,   612,  -875,   472,   477,  -875,
     479,   481,   657,   486,  -875,   487,   490,  -875,   491,   499,
     676,   503,   504,  -875,  -875,  -875,  -875,   505,   680,   508,
     509,   518,   522,   439,   699,   524,   563,   525,   526,  -875,
     527,  -875,  -875,  -875,  -875,  -875,  -875,  -875,   534,   535,
     537,   538,  -875,  -875,   550,   371,   552,   558,  -875,   556,
      55,    55,   562,  -875,   572,   390,    55,    55,  -875,    55,
      55,    55,    55,    55,   334,   439,  -875,   583,   581,  -875,
    -875,  -875,  -875,  -875,  -875,   587,   110,   128,  -875,  -875,
     334,   439,   589,   590,   591,   702,   702,  -875,  -875,    55,
     -65,   762,    38,   773,   597,   600,   478,   607,    55,    61,
     790,   613,   627,    55,   628,   371,   630,    55,   371,   -18,
     -18,   478,   741,   743,  -875,  -875,   639,   640,   465,  -875,
     -18,   -18,   261,  -875,   647,   644,   478,   651,    55,    25,
     811,   850,   675,   820,   819,   300,   -18,   775,   679,   778,
     -24,   -43,  -875,   688,   -24,   -24,   -24,    55,   689,    69,
     -18,    21,   791,    -8,   727,   788,   -10,   879,    13,   772,
    -875,   165,   165,  -875,   273,   704,    -4,   745,  -875,  -875,
     358,   546,   178,   178,  -875,  -875,  -875,   158,  -875,   478,
     707,   -85,   -75,   -70,   -61,  -875,  -875,   334,   439,   317,
     202,    31,  -875,   710,   555,  -875,  -875,  -875,   887,  -875,
     711,   397,  -875,   734,   890,   638,  -875,  -875,   404,  -875,
    -875,   736,   685,    76,  -875,   737,   692,  -875,  -875,  -875,
    -875,   782,   784,   -18,   -18,   738,   801,   800,   804,   805,
    -875,   806,   311,  -875,   803,   808,  -875,   809,   810,  -875,
    -875,   987,   697,  -875,  -875,   812,  -875,   813,   990,  -875,
     815,  -875,   814,  -875,   816,  -875,  -875,  -875,   993,  -875,
     821,  -875,   995,   822,    25,   823,   824,  -875,   825,   926,
    -875,  -875,  -875,  -875,  -875,   826,  -875,   827,   828,  -875,
     830,  -875,  1004,   831,  -875,   832,   900,  1011,  -875,   835,
     371,  -875,   776,    55,  -875,  -875,   334,   836,  -875,   838,
     837,  -875,  -875,  -875,  -875,  1015,   839,  -875,   -26,  -875,
      55,  -875,   -65,  -875,    40,  -875,   117,  -875,    49,  -875,
    -875,  -875,   842,  1020,  -875,  -875,   841,  -875,   840,   843,
    -875,  -875,  -875,  -875,  -875,  -875,  -875,   844,   846,   847,
     849,   851,   852,   853,   854,   855,  -875,   856,  -875,   857,
    -875,  -875,  -875,   858,  -875,  -875,  -875,  -875,  -875,   848,
    -875,   860,   861,    13,  1037,  -875,   863,   970,   478,  -875,
      55,    55,   478,   865,    55,   478,   478,   864,   867,  -875,
     -19,  1044,   869,   902,  1047,    68,  -104,   969,   873,    -3,
    -875,   866,   972,  -875,    55,   876,   -65,   996,   -18,   877,
     971,   880,    -1,   125,   371,   165,  -875,  -875,   439,   878,
     101,   745,  -875,     7,  -875,  -875,   439,   334,    87,  -875,
     144,  -875,   154,  -875,    25,   882,  -875,  -875,  -875,   335,
     963,   968,    55,    55,    55,    55,   -65,    55,    55,    55,
     231,  -875,   696,   455,  -875,   885,    55,  -875,   883,   369,
     629,   884,    25,   703,   886,   888,    55,  1064,   892,   893,
    1066,   894,   889,  -875,  -875,  -875,  -875,  1072,  -875,   253,
     897,  -875,  -875,   898,    41,   334,   899,   901,   695,  -875,
    1076,  -875,  1078,   892,  -875,   903,  -875,  -875,   904,   136,
    -875,  -875,  -875,  -875,   371,    40,  -875,   117,  -875,    49,
    -875,   905,  1080,  -875,  -875,  -875,  -875,  -875,  -875,  -875,
     334,  -875,  -875,  -875,  -875,    52,  -875,  -875,  -875,   -65,
    -875,  -875,  -875,  -875,  -875,  -875,   906,   908,   909,  -875,
    -875,   910,  -875,  -875,  -875,   334,   911,   912,   913,  -875,
    -875,  1083,  -875,   439,  -875,  1056,  -875,    55,  -875,   914,
    -875,  -875,  -875,   458,   916,   381,  1005,  1094,   920,   231,
    -875,   918,   922,    -1,    55,  -875,  -875,   921,  1040,  1021,
    -875,   924,   927,   928,   929,   930,   931,   932,  -875,  -875,
     933,   934,   935,  -875,   936,   937,  1005,    41,  1100,   129,
     939,   938,   136,  -875,  -875,  -875,  -875,  -875,   941,  1043,
     334,    55,    55,    55,   -87,   335,  1118,  1006,  -875,   940,
     475,  -875,    55,  1122,  -875,  -875,  -875,  -875,   947,   948,
    1039,   951,  1124,  -875,  -875,    61,   892,  -875,  -875,   952,
     953,  1045,  1130,    42,   231,   335,    38,    38,   -18,   -21,
    1132,  -875,  1133,  1039,  -875,  -875,   957,  -875,  -875,  -875,
    1134,  -875,  -875,  -875,  -875,  -875,   907,  -875,  -875,  -875,
    -875,  -875,  1101,  -875,   113,   958,   381,  1049,   962,  -875,
    1140,   964,   966,  1005,  1060,  1062,  -875,  -875,  -875,  -875,
    -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,
    -875,   967,  -875,   861,  -875,   702,   973,   -87,   974,   976,
     975,  -875,  -875,  -875,  -875,  1045,   371,   977,  -875,  -875,
    1039,   979,  -875,  -875,   371,   702,  -875,    25,  -875,  -875,
    -875,   981,  -875,  -875,  -875,  -875,  -875,  1067,  -875,  -875,
     982,   -18,   371,  -875,    84,   983,  -875,  -875,  -875,   885,
     -18,   984,  -875,  -875,  -875
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -875,  -875,  1147,  -875,  -875,  -875,  -875,  -875,  -875,  -875,
     116,  -875,  -875,   942,   -97,  -342,   771,   959,  1098,  -455,
    -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,
    -875,  -875,  -875,  -875,  -875,  -875,  1105,  -875,  -875,  -875,
     277,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,
    -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,
     721,   978,  -875,  -875,  -875,  -875,   663,  -875,  -875,  -875,
     310,  -875,  -875,  -875,  -592,   287,   228,  -875,  -875,   406,
     251,  -875,  -875,  -875,  -875,  -875,   181,  -875,  -875,  1014,
    -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,
    -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,
    -875,  -875,  -577,  -875,  -875,  -875,  -875,  -875,  -875,  -875,
    -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,  -875,
    -875,  -875,   241,   501,  -755,   216,  -875,  -819,  -875,  -874,
     179,  -431,  -558,  -875,  -875,  -875,   443,   793,  -202,  -160,
    -366,   686,   -91,  -363,  -436,  -597,  -483,  -585,  -488,  -566,
    -154,   -59,  -875,   480,  -875,  -875,   709,  -875,  -875,   915,
    -156,   677,  -875,  -441,  -875,  -875,  -875,  -875,  -875,  -162,
    -875
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -220
static const yytype_int16 yytable[] =
{
     221,   615,   248,   459,   349,   231,   690,   617,   571,   277,
     278,   485,   597,   218,   486,   294,   306,   307,   312,   309,
     311,   723,   319,   320,   321,   588,   589,   585,   886,   301,
     327,   721,   561,   330,   530,   328,   329,   338,   331,   332,
      16,   497,   339,   251,   341,   865,   979,   346,   719,   239,
     240,   352,   530,   577,   271,   619,   315,   239,   240,   213,
     955,     7,   378,   514,   301,   581,   517,   943,   380,   992,
     342,   595,   768,   333,   781,   747,   760,   334,   335,   726,
     631,   274,   300,   295,   632,   633,   239,   240,   275,  1037,
     313,   314,   573,   213,   543,   606,   574,   354,   236,   333,
     544,   956,   607,   334,   335,   608,   296,   297,     1,     2,
     610,   786,   609,   347,   480,   252,   866,   611,   354,   612,
     262,   416,   417,   227,   419,     8,   613,   980,   253,   981,
     326,   379,   213,   333,   228,   229,  1026,   334,   335,   562,
     343,   506,   507,   763,   634,   635,   636,   578,   637,   465,
     764,   239,   240,   243,  1010,   355,   244,     9,   302,  1038,
     982,   365,   213,   381,   244,    11,   801,   272,   530,   316,
     336,   973,   582,   638,   639,   640,   641,   642,   643,   769,
     644,   782,   243,   237,   227,   245,   246,   757,   414,   415,
     726,    13,   263,   310,   849,   228,   229,   645,   646,   525,
     784,   213,   896,   239,   240,   262,   647,   648,   649,   650,
     651,   652,   895,   587,   245,   246,    14,   618,    72,   369,
     498,   370,   254,   487,   653,   371,   372,   373,   255,   894,
     156,   587,   744,   899,   262,   654,   684,   214,   533,   534,
     503,   803,   715,   714,    15,   568,   157,   243,   706,   518,
     519,   324,   215,   216,   159,   520,   597,   814,   244,   160,
     528,   529,   527,   889,   530,   890,   537,   795,   531,   788,
     540,   214,   239,   240,   796,   206,   556,   263,   560,   245,
     246,   161,   565,   566,   567,   162,   215,   216,   217,   493,
     572,   464,   214,   999,   790,   791,   -15,   470,   471,   264,
     472,   473,   474,   475,   476,   265,   263,   215,   216,   217,
     376,   374,   596,   163,   673,   397,   227,   355,   488,   164,
     251,   212,   217,   604,   797,   215,   216,   228,   229,   165,
     494,   798,   241,   242,   799,   220,   239,   240,   251,   505,
     214,   800,   166,   217,   512,   167,   227,   587,   516,   361,
     362,   363,   168,   656,   169,   215,   216,   228,   229,   986,
     987,   217,   787,   665,   666,   170,   375,   171,   243,   542,
     172,   173,   533,   534,   174,   551,   923,   552,   553,   376,
     175,   357,   176,   377,   264,   227,   239,   240,   616,   177,
     265,   217,   252,   178,   215,   216,   228,   229,   591,   397,
     245,   246,   296,   297,   358,   253,   232,   233,   234,   235,
     252,   266,   267,   264,   361,   362,   363,   958,   365,   265,
     914,   915,   916,   253,   179,   279,   280,   281,   282,   283,
     217,   284,   243,   285,   286,   287,   288,   289,   180,   181,
     266,   267,   222,   532,   182,   183,   184,   985,   946,   185,
     186,   984,   893,   359,   360,   361,   362,   363,   717,  1030,
     187,   188,   189,   190,   245,   246,   191,   192,   533,   534,
     239,   240,   193,   194,   291,   279,   280,   281,   282,   283,
     195,   284,   243,   285,   286,   287,   288,   289,   196,   197,
     239,   240,   773,   290,   241,   242,   198,   199,   225,   254,
     200,   201,   203,   614,   204,   255,   205,   359,   360,   361,
     362,   363,   748,   592,   245,   246,   751,   254,   211,   754,
     755,   223,   226,   255,   291,   250,   305,   836,   256,   257,
     806,   807,   808,   809,   708,   811,   812,   813,   777,   525,
     239,   240,   775,   601,   324,   308,   256,   257,   350,   845,
     351,   716,   846,   239,   240,   353,   837,   386,   356,   279,
     280,   281,   282,   283,   389,   284,   243,   285,   286,   287,
     288,   289,   838,   469,   390,   396,   394,   290,   391,   392,
     393,   397,   810,   399,   400,   398,   243,   402,   835,   403,
     839,   406,   359,   360,   361,   362,   363,   244,   245,   246,
     407,   409,   410,   359,   360,   361,   362,   363,   291,  -219,
     411,   749,   750,   412,   413,   753,   427,   418,   245,   246,
     420,   426,   421,   422,   359,   360,   361,   362,   363,   279,
     280,   281,   282,   283,   424,   284,   243,   285,   286,   287,
     288,   289,   279,   280,   281,   282,   283,   290,   284,   243,
     285,   286,   287,   288,   289,   962,   428,   429,   963,   430,
     290,   432,   927,   431,  1023,   900,   433,   434,   245,   246,
     435,   436,  1028,   359,   360,   361,   362,   363,   291,   437,
     438,   245,   246,   439,   440,   441,   442,   843,   444,   445,
    1036,   291,   359,   912,   361,   362,   363,   853,   446,   952,
     953,   954,   447,   448,   449,   451,   452,   453,    16,   359,
     360,   361,   362,   363,   454,   455,    17,   456,   457,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
     458,   602,   461,    29,    30,    31,    32,    33,   462,    34,
     621,   463,   466,   450,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,   468,    46,    47,    48,    49,
      50,    51,    52,   477,   478,   496,    53,    54,   479,    55,
     489,   490,   491,   815,   816,    56,   500,   501,   988,   870,
     359,   360,   361,   362,   363,   502,   871,   504,   910,   359,
     360,   361,   362,   363,    57,   509,   872,   359,   360,   361,
     362,   363,   510,   817,   818,    58,   819,   511,   513,   847,
     515,    59,   873,    60,    61,   521,    62,   522,    63,   523,
     524,    64,    65,   627,   538,    66,    67,   539,    68,   820,
      69,   541,   821,   822,   823,   824,   825,   826,   827,   828,
     829,   830,   831,   874,   875,   876,   877,   546,   878,   879,
     880,   881,   882,   964,   547,   548,   549,   550,   557,   558,
      70,   559,    71,   359,   360,   361,   362,   363,   564,   569,
     630,  1035,   359,   360,   361,   362,   363,   660,   576,   579,
    1043,   580,   679,   584,   227,  -171,    72,   605,   850,   594,
     622,   623,    73,    74,   626,   228,   229,   620,  1016,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,   625,  1029,   359,
     360,   361,   362,   363,   629,   659,   359,   360,   361,   362,
     363,   359,   360,   361,   362,   363,    74,   359,   360,   361,
     362,   363,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
     207,    76,   663,    78,   664,    80,    75,    82,    77,    84,
      79,    86,    81,    88,    83,    90,    85,    92,    87,   667,
      89,   668,    91,   669,   670,   671,   672,   674,   675,   676,
     677,   678,   680,   681,   682,   683,   685,   686,   684,   688,
     695,   687,   689,   691,   692,   694,   696,   697,   701,   698,
     700,   702,   703,   704,   673,   705,   707,   710,   711,   713,
     712,   295,   724,   725,   726,   729,   728,   730,   731,   727,
     732,   741,   733,   734,   735,   736,   737,   738,   739,   740,
     742,   743,   745,   746,   525,   752,   756,   757,   759,   760,
     761,   762,   766,   767,   771,   772,   774,   778,   776,   804,
     780,   779,   802,   805,   789,   841,   844,   848,   854,   851,
     858,   852,   855,   857,   860,   859,   861,   863,   864,   868,
     884,   869,   885,   887,   888,   898,   908,   901,   897,   902,
     903,   904,   905,   906,   907,   909,   913,   911,   921,   919,
     922,   924,   925,   929,   928,   931,   930,   945,   932,   933,
     934,   935,   936,   937,   938,   939,   940,   942,   951,   941,
     947,   950,   959,   961,   960,   948,   965,   966,   971,   967,
     968,   970,   974,   975,   978,   976,   990,   991,   994,   996,
     998,  1001,  1003,  1005,  1007,  1011,  1012,  1008,   997,  1009,
    1013,  1033,    10,  1017,  1020,  1042,   492,  1019,  1021,  1027,
    1025,  1031,  1034,  1040,   348,   208,   202,  1044,   388,   949,
     570,   628,   993,   926,   944,   862,   972,   340,  1018,   261,
     989,   758,  1002,   495,  1022,   834,   603,   624,     0,     0,
     657,   793,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   423
};

static const yytype_int16 yycheck[] =
{
      97,   489,   162,   345,   206,   159,   564,   490,   449,   169,
     170,   377,   467,    72,   377,   171,   176,   177,   180,   179,
     180,   618,   184,   185,   186,   461,   462,   458,   783,    53,
     190,   616,    75,   193,     3,   191,   192,   197,   194,   195,
       6,     3,   198,     3,   200,     4,     4,   203,   614,    75,
      76,   211,     3,    61,    75,   491,    75,    75,    76,     4,
     147,   181,   222,   405,    53,    75,   408,   886,   222,   943,
       5,    75,    75,     4,    75,   667,   180,     8,     9,   183,
       4,    68,   173,   182,     8,     9,    75,    76,    75,     5,
     181,   182,    71,     4,    69,   180,    75,     8,     4,     4,
      75,   188,   187,     8,     9,   180,   205,   206,    15,    16,
     180,   703,   187,   204,     4,    75,    75,   187,     8,   180,
       3,   281,   282,   188,   284,   181,   187,    85,    88,    87,
     189,   222,     4,     4,   199,   200,  1010,     8,     9,   182,
      75,    80,    81,    75,    68,    69,    70,   155,    72,   351,
      82,    75,    76,   171,   973,   214,   182,     0,   182,    75,
     118,   220,     4,   222,   182,   105,   724,   188,     3,   188,
      75,   926,   182,    97,    98,    99,   100,   101,   102,   182,
     104,   182,   171,    89,   188,   203,   204,   180,   279,   280,
     183,   188,    75,   182,   752,   199,   200,   121,   122,    74,
      75,     4,   799,    75,    76,     3,   130,   131,   132,   133,
     134,   135,   797,   182,   203,   204,   188,   186,   184,    22,
     182,    24,   182,   377,   148,    28,    29,    30,   188,   795,
     180,   182,   663,   181,     3,   159,   184,   182,   207,   208,
     396,   729,   608,   606,   181,   447,   106,   171,   590,   409,
     410,   182,   197,   198,   181,   411,   711,   740,   182,   181,
     420,   421,   418,   127,     3,   129,   422,   180,     7,   705,
     426,   182,    75,    76,   187,   184,   436,    75,   440,   203,
     204,   181,   444,   445,   446,   181,   197,   198,   233,   386,
     450,   350,   182,   180,   193,   194,   183,   356,   357,   182,
     359,   360,   361,   362,   363,   188,    75,   197,   198,   233,
     182,   114,   466,   181,     3,     4,   188,   376,   377,   181,
       3,   190,   233,   479,   180,   197,   198,   199,   200,   181,
     389,   187,    79,    80,   180,   182,    75,    76,     3,   398,
     182,   187,   181,   233,   403,   181,   188,   182,   407,   236,
     237,   238,   181,   513,   181,   197,   198,   199,   200,   936,
     937,   233,   704,   523,   524,   181,   169,   181,   171,   428,
     181,   181,   207,   208,   181,    75,   859,    77,    78,   182,
     181,   162,   181,   186,   182,   188,    75,    76,   186,   181,
     188,   233,    75,   181,   197,   198,   199,   200,   125,     4,
     203,   204,   205,   206,   185,    88,    10,    11,    12,    13,
      75,   209,   210,   182,   236,   237,   238,   905,   477,   188,
      39,    40,    41,    88,   181,   164,   165,   166,   167,   168,
     233,   170,   171,   172,   173,   174,   175,   176,   181,   181,
     209,   210,   239,   182,   181,   181,   181,   935,   889,   181,
     181,   934,   794,   234,   235,   236,   237,   238,   612,  1017,
     181,   181,   181,   181,   203,   204,   181,   181,   207,   208,
      75,    76,   181,   181,   213,   164,   165,   166,   167,   168,
     181,   170,   171,   172,   173,   174,   175,   176,   181,   181,
      75,    76,   694,   182,    79,    80,   181,   181,     3,   182,
     181,   181,   181,   186,   181,   188,   181,   234,   235,   236,
     237,   238,   668,   240,   203,   204,   672,   182,   181,   675,
     676,   181,   180,   188,   213,    93,   188,    72,   211,   212,
     732,   733,   734,   735,   593,   737,   738,   739,   698,    74,
      75,    76,   696,   185,   182,    86,   211,   212,   239,   180,
     184,   610,   183,    75,    76,   182,   101,   186,   184,   164,
     165,   166,   167,   168,   184,   170,   171,   172,   173,   174,
     175,   176,   117,   183,   240,   182,   179,   182,   180,   180,
     180,     4,   736,   180,   180,   184,   171,     3,   742,   184,
     135,     3,   234,   235,   236,   237,   238,   182,   203,   204,
     184,   180,   180,   234,   235,   236,   237,   238,   213,   186,
     180,   670,   671,   180,   180,   674,     4,   182,   203,   204,
     182,   242,   182,   182,   234,   235,   236,   237,   238,   164,
     165,   166,   167,   168,   182,   170,   171,   172,   173,   174,
     175,   176,   164,   165,   166,   167,   168,   182,   170,   171,
     172,   173,   174,   175,   176,   180,   184,   180,   183,   180,
     182,     4,   864,   182,  1006,   819,   180,   180,   203,   204,
     180,   180,  1014,   234,   235,   236,   237,   238,   213,   180,
       4,   203,   204,   180,   180,   180,     6,   746,   180,   180,
    1032,   213,   234,   235,   236,   237,   238,   756,   180,   901,
     902,   903,   180,     4,   180,   180,   180,   180,     6,   234,
     235,   236,   237,   238,   180,   180,    14,   180,   180,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
     180,   185,   180,    31,    32,    33,    34,    35,   180,    37,
     185,   185,   180,   180,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,   183,    54,    55,    56,    57,
      58,    59,    60,   180,   183,     3,    64,    65,   181,    67,
     181,   181,   181,    77,    78,    73,     3,   180,   938,    84,
     234,   235,   236,   237,   238,   185,    91,   180,   847,   234,
     235,   236,   237,   238,    92,     5,   101,   234,   235,   236,
     237,   238,   189,   107,   108,   103,   110,   180,   180,   180,
     180,   109,   117,   111,   112,    74,   114,    74,   116,   180,
     180,   119,   120,   185,   177,   123,   124,   183,   126,   133,
     128,   180,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   148,   149,   150,   151,    36,   153,   154,
     155,   156,   157,   912,     4,   180,    36,    38,    83,   180,
     158,    83,   160,   234,   235,   236,   237,   238,   180,   180,
     185,  1031,   234,   235,   236,   237,   238,   185,    87,   152,
    1040,    93,   185,     4,   188,   113,   184,   180,   185,   185,
       3,   180,   190,   191,     4,   199,   200,   187,   995,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   183,  1015,   234,
     235,   236,   237,   238,   188,   188,   234,   235,   236,   237,
     238,   234,   235,   236,   237,   238,   191,   234,   235,   236,
     237,   238,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     191,   198,   180,   200,   180,   202,   197,   204,   199,   206,
     201,   208,   203,   210,   205,   212,   207,   214,   209,   241,
     211,   180,   213,   183,   180,   180,   180,   184,   180,   180,
     180,     4,   180,   180,     4,   180,   180,     4,   184,     4,
      74,   180,   180,   180,   180,   180,   180,   180,     4,   181,
     180,   180,   180,   113,     3,   180,   240,   181,   180,     4,
     183,   182,   180,     3,   183,   181,   183,   181,   181,   189,
     181,   183,   181,   181,   181,   181,   181,   181,   181,   181,
     180,   180,     5,   180,    74,   180,   182,   180,     4,   180,
     148,     4,    83,   180,   188,    83,   180,   180,    62,    96,
     180,    90,   180,    95,   186,   180,   183,   183,     4,   183,
       4,   183,   180,   180,   185,   181,     4,   180,   180,   180,
       4,   180,     4,   180,   180,     5,     3,   181,   183,   181,
     181,   181,   181,   181,   181,    39,   180,   183,     4,    94,
     180,   183,   180,    63,   183,   181,    85,     7,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   180,    75,   183,
     181,   180,     4,   183,   118,   187,     4,   180,     4,   181,
      91,   180,   180,   180,     4,    90,     4,     4,   181,     5,
      39,   183,    93,   181,     4,    85,    84,   183,   241,   183,
     183,    84,     5,   180,   178,  1039,   385,   183,   183,   180,
     183,   180,   180,   180,   205,    67,    61,   183,   226,   892,
     449,   508,   944,   863,   887,   769,   925,   199,   997,   165,
     939,   680,   966,   390,  1005,   742,   477,   501,    -1,    -1,
     513,   711,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   288
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   244,   245,   246,   247,   181,   181,     0,
     245,   105,   255,   188,   188,   181,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    31,
      32,    33,    34,    35,    37,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    54,    55,    56,    57,
      58,    59,    60,    64,    65,    67,    73,    92,   103,   109,
     111,   112,   114,   116,   119,   120,   123,   124,   126,   128,
     158,   160,   184,   190,   191,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   248,   249,   250,   257,   259,   260,   261,
     262,   264,   265,   273,   276,   277,   279,   280,   287,   289,
     290,   292,   294,   296,   300,   301,   302,   305,   307,   314,
     321,   324,   330,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   346,   347,   348,   349,   353,   354,   356,
     357,   358,   359,   360,   361,   362,   363,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   180,   106,   256,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   279,   181,   181,   181,   184,   191,   261,   263,
     281,   181,   190,     4,   182,   197,   198,   233,   404,   414,
     182,   257,   239,   181,   291,     3,   180,   188,   199,   200,
     390,   403,    10,    11,    12,    13,     4,    89,   415,    75,
      76,    79,    80,   171,   182,   203,   204,   322,   392,   393,
      93,     3,    75,    88,   182,   188,   211,   212,   332,   401,
     402,   332,     3,    75,   182,   188,   209,   210,   326,   399,
     400,    75,   188,   375,    68,    75,   384,   392,   392,   164,
     165,   166,   167,   168,   170,   172,   173,   174,   175,   176,
     182,   213,   392,   412,   413,   182,   205,   206,   395,   396,
     395,    53,   182,   422,   423,   188,   392,   392,    86,   392,
     182,   392,   422,   395,   395,    75,   188,   304,   376,   422,
     422,   422,   391,   404,   182,   303,   404,   392,   413,   413,
     392,   413,   413,     4,     8,     9,    75,   416,   392,   413,
     304,   413,     5,    75,   315,   297,   413,   395,   260,   391,
     239,   184,   392,   182,     8,   404,   184,   162,   185,   234,
     235,   236,   237,   238,   403,   404,   409,   410,   411,    22,
      24,    28,    29,    30,   114,   169,   182,   186,   392,   395,
     403,   404,   418,   419,   420,   295,   186,   258,   256,   184,
     240,   180,   180,   180,   179,   394,   182,     4,   184,   180,
     180,   309,     3,   184,   333,   331,     3,   184,   325,   180,
     180,   180,   180,   180,   395,   395,   392,   392,   182,   392,
     182,   182,   182,   412,   182,   413,   242,     4,   184,   180,
     180,   182,     4,   180,   180,   180,   180,   180,     4,   180,
     180,   180,     6,   278,   180,   180,   180,   180,     4,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   258,
     298,   180,   180,   185,   404,   391,   180,   274,   183,   183,
     404,   404,   404,   404,   404,   404,   404,   180,   183,   181,
       4,   269,   270,   271,   272,   393,   396,   403,   404,   181,
     181,   181,   259,   257,   404,   390,     3,     3,   182,   355,
       3,   180,   185,   413,   180,   404,    80,    81,   323,     5,
     189,   180,   404,   180,   258,   180,   404,   258,   392,   392,
     413,    74,    74,   180,   180,    74,   317,   413,   392,   392,
       3,     7,   182,   207,   208,   397,   398,   413,   177,   183,
     413,   180,   404,    69,    75,   385,    36,     4,   180,    36,
      38,    75,    77,    78,   386,   389,   392,    83,   180,    83,
     422,    75,   182,   310,   180,   422,   422,   422,   391,   180,
     303,   416,   392,    71,    75,   421,    87,    61,   155,   152,
      93,    75,   182,   311,     4,   384,   299,   182,   397,   397,
     293,   125,   240,   288,   185,    75,   403,   262,   406,   407,
     408,   185,   185,   409,   413,   180,   180,   187,   180,   187,
     180,   187,   180,   187,   186,   401,   186,   399,   186,   397,
     187,   185,     3,   180,   394,   183,     4,   185,   309,   188,
     185,     4,     8,     9,    68,    69,    70,    72,    97,    98,
      99,   100,   101,   102,   104,   121,   122,   130,   131,   132,
     133,   134,   135,   148,   159,   334,   392,   414,   417,   188,
     185,   327,   319,   180,   180,   392,   392,   241,   180,   183,
     180,   180,   180,     3,   184,   180,   180,   180,     4,   185,
     180,   180,     4,   180,   184,   180,     4,   180,     4,   180,
     385,   180,   180,   345,   180,    74,   180,   180,   181,   350,
     180,     4,   180,   180,   113,   180,   258,   240,   404,   282,
     181,   180,   183,     4,   396,   393,   404,   403,   266,   402,
     267,   400,   268,   398,   180,     3,   183,   189,   183,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   183,   180,   180,   384,     5,   180,   317,   413,   404,
     404,   413,   180,   404,   413,   413,   182,   180,   376,     4,
     180,   148,     4,    75,    82,   388,    83,   180,    75,   182,
     312,   188,    83,   391,   180,   403,    62,   392,   180,    90,
     180,    75,   182,   313,    75,   316,   317,   258,   397,   186,
     193,   194,   405,   406,   275,   180,   187,   180,   187,   180,
     187,   385,   180,   401,    96,    95,   391,   391,   391,   391,
     403,   391,   391,   391,   399,    77,    78,   107,   108,   110,
     133,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   328,   387,   389,   403,    72,   101,   117,   135,
     320,   180,   253,   404,   183,   180,   183,   180,   183,   385,
     185,   183,   183,   404,     4,   180,   377,   180,     4,   181,
     185,     4,   322,   180,   180,     4,    75,   318,   180,   180,
      84,    91,   101,   117,   148,   149,   150,   151,   153,   154,
     155,   156,   157,   351,     4,     4,   377,   180,   180,   127,
     129,   283,   284,   258,   402,   400,   398,   183,     5,   181,
     403,   181,   181,   181,   181,   181,   181,   181,     3,    39,
     404,   183,   235,   180,    39,    40,    41,   378,   379,    94,
     380,     4,   180,   399,   183,   180,   313,   391,   183,    63,
      85,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   183,   180,   380,   318,     7,   416,   181,   187,   283,
     180,    75,   391,   391,   391,   147,   188,   329,   401,     4,
     118,   183,   180,   183,   404,     4,   180,   181,    91,   382,
     180,     4,   323,   377,   180,   180,    90,   383,     4,     4,
      85,    87,   118,   352,   399,   401,   355,   355,   392,   375,
       4,     4,   382,   319,   181,   286,     5,   241,    39,   180,
     251,   183,   378,    93,   381,   181,   364,     4,   183,   183,
     380,    85,    84,   183,   306,   285,   257,   180,   329,   183,
     178,   183,   383,   258,   365,   183,   382,   180,   258,   257,
     385,   180,   308,    84,   180,   392,   258,     5,    75,   254,
     180,   252,   253,   392,   183
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
#line 1470 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1477 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1485 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1501 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1505 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1511 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1519 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1525 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1533 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1541 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1546 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1552 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1558 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1564 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1570 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1575 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1580 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1585 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1590 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1595 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1600 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1605 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1610 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1615 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1620 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1626 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1633 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1642 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1646 "lev_comp.y"
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

  case 244:

/* Line 1455 of yacc.c  */
#line 1659 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1667 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1677 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1685 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1694 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1701 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1709 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1715 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1720 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1725 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1730 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1735 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1740 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1745 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1750 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1755 "lev_comp.y"
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

  case 260:

/* Line 1455 of yacc.c  */
#line 1766 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1772 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1777 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1782 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1787 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1792 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1797 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1802 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1807 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1812 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1817 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1822 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1827 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x400000;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1834 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1840 "lev_comp.y"
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

  case 275:

/* Line 1455 of yacc.c  */
#line 1869 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1874 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1882 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1888 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1894 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1901 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1908 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1918 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1928 "lev_comp.y"
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

  case 284:

/* Line 1455 of yacc.c  */
#line 1943 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1953 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1957 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1963 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1969 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1975 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1979 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1985 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1991 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1998 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2006 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2012 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2017 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2022 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2027 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2032 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2038 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2044 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2049 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2054 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2059 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_CATEGORY));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2064 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2069 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2079 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2085 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2091 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2096 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2103 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2110 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2116 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2122 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2128 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2132 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2138 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2144 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2154 "lev_comp.y"
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

  case 324:

/* Line 1455 of yacc.c  */
#line 2173 "lev_comp.y"
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

  case 325:

/* Line 1455 of yacc.c  */
#line 2185 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2189 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2195 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2202 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2208 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2214 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2220 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2226 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2230 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2235 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2242 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2248 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2255 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2259 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2266 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2277 "lev_comp.y"
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

  case 343:

/* Line 1455 of yacc.c  */
#line 2290 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2294 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2300 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2304 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2311 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2315 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2319 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2325 "lev_comp.y"
    {
			(yyval.i) = ROOM;
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2329 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2335 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2341 "lev_comp.y"
    {
			(yyval.i) = 0;
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2345 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2351 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2367 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2375 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2385 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2394 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2399 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2406 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2417 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2423 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2427 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2431 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2438 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2448 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2455 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2459 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2465 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2469 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2477 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2481 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2488 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2498 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2511 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2515 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2522 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2532 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2541 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2552 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2556 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2563 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2573 "lev_comp.y"
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

  case 395:

/* Line 1455 of yacc.c  */
#line 2584 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2593 "lev_comp.y"
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

  case 397:

/* Line 1455 of yacc.c  */
#line 2603 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2609 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2613 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2620 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2630 "lev_comp.y"
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

  case 402:

/* Line 1455 of yacc.c  */
#line 2641 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2650 "lev_comp.y"
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

  case 404:

/* Line 1455 of yacc.c  */
#line 2660 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2664 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2670 "lev_comp.y"
    { ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2672 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2678 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2682 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2686 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2690 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2698 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2707 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2711 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2715 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2719 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2723 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2726 "lev_comp.y"
    { ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2730 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2737 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2746 "lev_comp.y"
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

  case 426:

/* Line 1455 of yacc.c  */
#line 2787 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2791 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2798 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2805 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2815 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2819 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2827 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2831 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2835 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2839 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2843 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2848 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2852 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2856 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2861 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2866 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2871 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2875 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2880 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2885 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2889 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2893 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2898 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2902 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2909 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2915 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2919 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2925 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2941 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2945 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2949 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2953 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 2975 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 2979 "lev_comp.y"
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

  case 473:

/* Line 1455 of yacc.c  */
#line 3005 "lev_comp.y"
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
#line 6781 "lev_comp.tab.c"
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
#line 3033 "lev_comp.y"


/*lev_comp.y*/

