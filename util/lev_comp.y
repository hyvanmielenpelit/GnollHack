%{
/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

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
extern struct opvar *FDECL(set_opvar_int, (struct opvar *, int64_t));
extern void VDECL(add_opvars, (sp_lev *, const char *, ...));
extern void FDECL(start_level_def, (sp_lev * *, char *));

extern struct lc_funcdefs *FDECL(funcdef_new, (int64_t,char *));
extern void FDECL(funcdef_free_all, (struct lc_funcdefs *));
extern struct lc_funcdefs *FDECL(funcdef_defined, (struct lc_funcdefs *,
                                                   char *, int));
extern char *FDECL(funcdef_paramtypes, (struct lc_funcdefs *));
extern char *FDECL(decode_parm_str, (char *));

extern struct lc_vardefs *FDECL(vardef_new, (int64_t,char *));
extern void FDECL(vardef_free_all, (struct lc_vardefs *));
extern struct lc_vardefs *FDECL(vardef_defined, (struct lc_vardefs *,
                                                 char *, int));

extern void NDECL(break_stmt_start);
extern void FDECL(break_stmt_end, (sp_lev *));
extern void FDECL(break_stmt_new, (sp_lev *, int64_t));

extern void FDECL(splev_add_from, (sp_lev *, sp_lev *));

extern void FDECL(check_vardef_type, (struct lc_vardefs *, char *, int64_t));
extern void FDECL(vardef_used, (struct lc_vardefs *, char *));
extern struct lc_vardefs *FDECL(add_vardef_type, (struct lc_vardefs *,
                                                  char *, int64_t));

extern int FDECL(reverse_jmp_opcode, (int64_t));

struct coord {
    int64_t x;
    int64_t y;
};

struct forloopdef {
    char *varname;
    int64_t jmp_point;
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
static int64_t switch_case_value[MAX_SWITCH_CASES];
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

%}

%union
{
    int64_t    i;
    char    *map;
    struct {
        int64_t room;
        int64_t wall;
        int64_t door;
    } corpos;
    struct {
        int64_t area;
        int64_t x1;
        int64_t y1;
        int64_t x2;
        int64_t y2;
    } lregn;
    struct {
        int64_t x;
        int64_t y;
    } crd;
    struct {
        int64_t ter;
        int64_t lit;
    } terr;
    struct {
        int64_t height;
        int64_t width;
    } sze;
    struct {
        int64_t die;
        int64_t num;
    } dice;
    struct {
        int64_t cfunc;
        char *varstr;
    } meth;
}


%token	<i> CHAR INTEGER BOOLEAN PERCENT SPERCENT
%token	<i> MINUS_INTEGER PLUS_INTEGER
%token	<i> MAZE_GRID_ID SOLID_FILL_ID MINES_ID ROGUELEV_ID
%token	<i> MESSAGE_ID MESSAGE_TYPE MESSAGE_ATTR MESSAGE_COLOR MESSAGE_SOUND_TYPE MESSAGE_SOUND_ID MAZE_ID LEVEL_ID LEV_INIT_ID TILESET_ID GEOMETRY_ID NOMAP_ID BOUNDARY_TYPE_ID SPECIAL_TILESET_ID TILESET_PARAM_ID DECOTYP_ID
%token	<i> OBJECT_ID COBJECT_ID MONSTER_ID TRAP_ID DOOR_ID DRAWBRIDGE_ID MONSTER_GENERATION_ID
%token	<i> object_ID monster_ID terrain_ID
%token	<i> MAZEWALK_ID WALLIFY_ID REGION_ID SPECIAL_REGION_ID SPECIAL_LEVREGION_ID SPECIAL_REGION_TYPE NAMING_ID NAMING_TYPE FILLING IRREGULAR JOINED
%token	<i> ALTAR_ID ANVIL_ID NPC_ID LADDER_ID STAIR_ID NON_DIGGABLE_ID NON_PASSWALL_ID ROOM_ID ARTIFACT_NAME_ID
%token	<i> PORTAL_ID TELEPRT_ID BRANCH_ID LEV MINERALIZE_ID AGE_ID
%token	<i> CORRIDOR_ID GOLD_ID ENGRAVING_ID FOUNTAIN_ID THRONE_ID MODRON_PORTAL_ID LEVEL_TELEPORTER_ID LEVEL_TELEPORT_DIRECTION_TYPE LEVEL_TELEPORT_END_TYPE POOL_ID SINK_ID NONE
%token	<i> RAND_CORRIDOR_ID DOOR_STATE LIGHT_STATE CURSE_TYPE MYTHIC_TYPE ENGRAVING_TYPE KEYTYPE_ID LEVER_ID NO_PICKUP_ID
%token	<i> DIRECTION RANDOM_TYPE RANDOM_TYPE_BRACKET A_REGISTER
%token	<i> ALIGNMENT LEFT_OR_RIGHT CENTER TOP_OR_BOT ALTAR_TYPE ALTAR_SUBTYPE UP_OR_DOWN ACTIVE_OR_INACTIVE
%token	<i> MODRON_PORTAL_TYPE NPC_TYPE FOUNTAIN_TYPE SPECIAL_OBJECT_TYPE CMAP_TYPE FLOOR_SUBTYPE FLOOR_SUBTYPE_ID FLOOR_ID FLOOR_TYPE FLOOR_TYPE_ID
%token	<i> DECORATION_ID DECORATION_TYPE DECORATION_DIR DECORATION_ITEM_STATE PAINTING_TYPE BANNER_TYPE WALL_SCULPTURE_TYPE
%token	<i> ELEMENTAL_ENCHANTMENT_TYPE EXCEPTIONALITY_TYPE EXCEPTIONALITY_ID ELEMENTAL_ENCHANTMENT_ID ENCHANTMENT_ID SECRET_DOOR_ID USES_UP_KEY_ID
%token	<i> MYTHIC_PREFIX_TYPE MYTHIC_SUFFIX_TYPE MYTHIC_PREFIX_ID MYTHIC_SUFFIX_ID MATERIAL_ID MATERIAL_TYPE
%token	<i> CHARGES_ID SPECIAL_QUALITY_ID SPEFLAGS_ID LEVEL_BOSS_ID BOSS_HOSTILITY_ID HAS_BACKUP_ID
%token	<i> SUBROOM_ID NAME_ID FLAGS_ID FLAG_TYPE MON_ATTITUDE MON_ALERTNESS SUBTYPE_ID NON_PASSDOOR_ID CARPET_ID CARPET_PIECE_ID CARPET_TYPE
%token	<i> MON_APPEARANCE ROOMDOOR_ID IF_ID ELSE_ID
%token	<i> TERRAIN_ID HORIZ_OR_VERT REPLACE_TERRAIN_ID LOCATION_SUBTYPE_ID DOOR_SUBTYPE BRAZIER_SUBTYPE SIGNPOST_SUBTYPE TREE_SUBTYPE FOREST_ID FOREST_TYPE INITIALIZE_TYPE
%token	<i> EXIT_ID SHUFFLE_ID MANUAL_TYPE_ID MANUAL_TYPE
%token	<i> QUANTITY_ID BURIED_ID LOOP_ID
%token	<i> FOR_ID TO_ID
%token	<i> SWITCH_ID CASE_ID BREAK_ID DEFAULT_ID
%token	<i> ERODED_ID TRAPPED_STATE RECHARGED_ID INVIS_ID GREASED_ID INDESTRUCTIBLE_ID
%token	<i> FEMALE_ID MALE_ID WAITFORU_ID PROTECTOR_ID CANCELLED_ID REVIVED_ID AVENGE_ID FLEEING_ID BLINDED_ID MAXHP_ID LEVEL_ADJUSTMENT_ID KEEP_ORIGINAL_INVENTORY_ID
%token	<i> PARALYZED_ID STUNNED_ID CONFUSED_ID SEENTRAPS_ID ALL_ID
%token	<i> MONTYPE_ID OBJTYPE_ID TERTYPE_ID TERTYPE2_ID LEVER_EFFECT_TYPE SWITCHABLE_ID CONTINUOUSLY_USABLE_ID TARGET_ID TRAPTYPE_ID EFFECT_FLAG_ID
%token	<i> GRAVE_ID BRAZIER_ID SIGNPOST_ID TREE_ID ERODEPROOF_ID
%token	<i> FUNCTION_ID
%token	<i> MSG_OUTPUT_TYPE
%token	<i> COMPARE_TYPE
%token  <i> UNKNOWN_TYPE
%token	<i> rect_ID fillrect_ID line_ID randline_ID grow_ID
%token	<i> selection_ID flood_ID
%token	<i> rndcoord_ID circle_ID ellipse_ID filter_ID complement_ID
%token	<i> gradient_ID GRADIENT_TYPE LIMITED HUMIDITY_TYPE
%token	<i> ',' ':' '(' ')' '[' ']' '{' '}'
%token	<map> STRING MAP_ID
%token	<map> NQSTRING VARSTRING
%token	<map> CFUNC CFUNC_INT CFUNC_STR CFUNC_COORD CFUNC_REGION
%token	<map> VARSTRING_INT VARSTRING_INT_ARRAY
%token	<map> VARSTRING_STRING VARSTRING_STRING_ARRAY
%token	<map> VARSTRING_VAR VARSTRING_VAR_ARRAY
%token	<map> VARSTRING_COORD VARSTRING_COORD_ARRAY
%token	<map> VARSTRING_REGION VARSTRING_REGION_ARRAY
%token	<map> VARSTRING_MAPCHAR VARSTRING_MAPCHAR_ARRAY
%token	<map> VARSTRING_MONST VARSTRING_MONST_ARRAY
%token	<map> VARSTRING_OBJ VARSTRING_OBJ_ARRAY
%token	<map> VARSTRING_SEL VARSTRING_SEL_ARRAY
%token	<meth> METHOD_INT METHOD_INT_ARRAY
%token	<meth> METHOD_STRING METHOD_STRING_ARRAY
%token	<meth> METHOD_VAR METHOD_VAR_ARRAY
%token	<meth> METHOD_COORD METHOD_COORD_ARRAY
%token	<meth> METHOD_REGION METHOD_REGION_ARRAY
%token	<meth> METHOD_MAPCHAR METHOD_MAPCHAR_ARRAY
%token	<meth> METHOD_MONST METHOD_MONST_ARRAY
%token	<meth> METHOD_OBJ METHOD_OBJ_ARRAY
%token	<meth> METHOD_SEL METHOD_SEL_ARRAY
%token	<dice> DICE
%type	<i> h_justif v_justif trap_name room_type door_state light_state
%type	<i> alignment altar_type a_register roomfill door_pos
%type	<i> alignment_prfx
%type	<i> door_wall walled secret
%type	<i> dir_list teleprt_detail
%type	<i> object_infos object_info monster_infos monster_info
%type	<i> levstatements stmt_block region_detail_end
%type	<i> engraving_type flag_list roomregionflag roomregionflags
%type	<i> optroomregionflags floorsubtype optfloorsubtype floortype optfloortype optmontype opttileset optdecotyp
%type	<i> humidity_flags
%type	<i> comparestmt encodecoord encoderegion mapchar
%type	<i> seen_trap_mask
%type	<i> encodemonster encodeobj encodeobj_list
%type	<i> integer_list string_list encodecoord_list encoderegion_list
%type	<i> mapchar_list encodemonster_list
%type	<i> opt_percent opt_fillchar
%type	<i> all_integers
%type	<i> ter_selection ter_selection_x
%type	<i> func_param_type
%type	<i> objectid monsterid terrainid
%type	<i> opt_coord_or_var opt_limited
%type	<map> level_def
%type	<map> any_var any_var_array any_var_or_arr any_var_or_unk
%type	<map> func_call_params_list func_call_param_list
%type	<i> func_call_param_part
%type	<corpos> corr_spec
%type	<lregn> region lev_region
%type	<crd> room_pos subroom_pos room_align
%type	<sze> room_size
%type	<terr> terrain_type
%left  '+' '-'
%left  '*' '/' '%'
%start	file

%%
file		: /* nothing */
		| levels
		;

levels		: level
		| level levels
		;

level		: level_def flags levstatements
		  {
			if (fatal_error > 0) {
				(void) fprintf(stderr,
              "%s: %d errors detected for level \"%s\". No output created!\n",
					       fname, fatal_error, $1);
				fatal_error = 0;
				got_errors++;
			} else if (!got_errors) {
				if (!write_level_file($1, splev)) {
                                    lc_error("Can't write output file for '%s'!",
                                             $1);
				    exit(EXIT_FAILURE);
				}
			}
			Free($1);
			Free(splev);
			splev = NULL;
			vardef_free_all(vardefs);
			vardefs = NULL;
		  }
		;

level_def	: LEVEL_ID ':' STRING
		  {
		      start_level_def(&splev, $3);
		      $$ = $3;
		  }
		| MAZE_ID ':' STRING ',' CHAR
		  {
		      start_level_def(&splev, $3);
		      if ($5 == -1) {
			  add_opvars(splev, "iiiiiiiio",
				     VA_PASS9(LVLINIT_MAZEGRID, HWALL, 0,0,
					      0,0,0,0, SPO_INITLEVEL));
		      } else {
			  int bg = (int)what_map_char((char) $5);

			  add_opvars(splev, "iiiiiiiio",
				     VA_PASS9(LVLINIT_SOLIDFILL, bg, 0, 0,
					      0,0,0,0, SPO_INITLEVEL));
		      }
		      add_opvars(splev, "io",
				 VA_PASS2(MAZELEVEL, SPO_LEVEL_FLAGS));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		      $$ = $3;
		  }
		| MAZE_ID ':' STRING ',' CHAR ',' light_state
		  {
              int lit = (int) $7;
		      start_level_def(&splev, $3);
		      if ($5 == -1) {
			  add_opvars(splev, "iiiiiiiio",
				     VA_PASS9(LVLINIT_MAZEGRID, HWALL, 0, lit,
					      0,0,0,0, SPO_INITLEVEL));
		      } else {
			  int bg = (int)what_map_char((char) $5);

			  add_opvars(splev, "iiiiiiiio",
				     VA_PASS9(LVLINIT_SOLIDFILL, bg, 0, lit,
					      0,0,0,0, SPO_INITLEVEL));
		      }
		      add_opvars(splev, "io",
				 VA_PASS2(MAZELEVEL, SPO_LEVEL_FLAGS));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		      $$ = $3;
		  }
		;

lev_init	: LEV_INIT_ID ':' SOLID_FILL_ID ',' terrain_type
		  {
		      int filling = (int) $5.ter;

		      if (filling == INVALID_LEVTYPE || filling >= MAX_LEVTYPE)
			  lc_error("INIT_MAP: Invalid fill char type.");
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_SOLIDFILL, filling,
                                          0, (int) $5.lit,
                                          0,0,0,0, SPO_INITLEVEL));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  }
		| LEV_INIT_ID ':' MAZE_GRID_ID ',' CHAR
		  {
		      int filling = (int) what_map_char((char) $5);

		      if (filling == INVALID_LEVTYPE || filling >= MAX_LEVTYPE)
			  lc_error("INIT_MAP: Invalid fill char type.");
                      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_MAZEGRID, filling, 0,0,
					  0,0,0,0, SPO_INITLEVEL));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  }
		| LEV_INIT_ID ':' ROGUELEV_ID
		  {
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_ROGUE,0,0,0,
					  0,0,0,0, SPO_INITLEVEL));
		  }
		| LEV_INIT_ID ':' MINES_ID ',' CHAR ',' CHAR ',' BOOLEAN ',' BOOLEAN ',' light_state ',' walled opt_fillchar
		  {
                      int fg = (int) what_map_char((char) $5),
                          bg = (int) what_map_char((char) $7);
                      int smoothed = (int) $9,
                          joined = (int) $11,
                          lit = (int) $13,
                          walled = (int) $15,
                          filling = (int) $16;

		      if (fg == INVALID_LEVTYPE || fg >= MAX_LEVTYPE)
			  lc_error("INIT_MAP: Invalid foreground type.");
		      if (bg == INVALID_LEVTYPE || bg >= MAX_LEVTYPE)
			  lc_error("INIT_MAP: Invalid background type.");
		      if (joined && fg != CORR && fg != ROOM && fg != GRASS && fg != GROUND && fg != AIR && fg != CLOUD)
			  lc_error("INIT_MAP: Invalid foreground type for joined map.");

		      if (filling == INVALID_LEVTYPE)
			  lc_error("INIT_MAP: Invalid fill char type.");

		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_MINES, filling, walled, lit,
					  joined, smoothed, bg, fg,
					  SPO_INITLEVEL));
			max_x_map = COLNO-1;
			max_y_map = ROWNO;
		  }
		;

tileset_detail : TILESET_ID ':' tileset_number
		  {
		      add_opvars(splev, "io", VA_PASS2((int)$<i>3, SPO_TILESET));
		  }
		;

forest_detail : FOREST_ID ':' FOREST_TYPE ',' INITIALIZE_TYPE
		  {
		      add_opvars(splev, "iio", VA_PASS3((int)$<i>5, (int)$<i>3, SPO_FOREST));
		  }
		| FOREST_ID ':' FOREST_TYPE
		  {
		      add_opvars(splev, "iio", VA_PASS3(0, (int)$<i>3, SPO_FOREST));
		  }
		;

monster_generation_detail : MONSTER_GENERATION_ID ':' INTEGER ',' encodemonster_list
  		  {
		      add_opvars(splev, "iio", VA_PASS3((int)$<i>5, (int)$<i>3, SPO_MONSTER_GENERATION));
		  }
		;

boundary_type_detail : BOUNDARY_TYPE_ID ':' FLOOR_TYPE
		  {
		      add_opvars(splev, "io", VA_PASS2((int)$<i>3, SPO_BOUNDARY_TYPE));
		  }
		;

opt_limited	: /* nothing */
		  {
		      $$ = 0;
		  }
		| ',' LIMITED
		  {
		      $$ = $2;
		  }
		;

opt_coord_or_var	: /* nothing */
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_COPY));
		      $$ = 0;
		  }
		| ',' coord_or_var
		  {
		      $$ = 1;
		  }
		;

opt_fillchar	: /* nothing */
		  {
		      $$ = -1;
		  }
		| ',' CHAR
		  {
		      $$ = what_map_char((char) $2);
		  }
		;


walled		: BOOLEAN
		| RANDOM_TYPE
		;

flags		: /* nothing */
		  {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_LEVEL_FLAGS));
		  }
		| FLAGS_ID ':' flag_list
		  {
		      add_opvars(splev, "io",
                                 VA_PASS2((int) $3, SPO_LEVEL_FLAGS));
		  }
		;

flag_list	: FLAG_TYPE ',' flag_list
		  {
		      $$ = ($1 | $3);
		  }
		| FLAG_TYPE
		  {
		      $$ = $1;
		  }
		;

levstatements	: /* nothing */
		  {
		      $$ = 0;
		  }
		| levstatement levstatements
		  {
		      $$ = 1 + $2;
		  }
		;

stmt_block	: '{' levstatements '}'
		  {
		      $$ = $2;
		  }
		;

levstatement 	: message
		| lev_init
		| tileset_detail
		| special_tileset_detail
		| boundary_type_detail
		| forest_detail
		| subtype_detail
		| carpet_detail
		| carpet_piece_detail
		| monster_generation_detail
		| floor_detail
		| decoration_detail
		| altar_detail
		| anvil_detail
		| npc_detail
		| grave_detail
		| brazier_detail
		| signpost_detail
		| tree_detail
		| branch_region
		| corridor
		| variable_define
		| shuffle_detail
		| diggable_detail
		| door_detail
		| drawbridge_detail
		| engraving_detail
		| mineralize
		| fountain_detail
		| throne_detail
		| modron_portal_detail
		| gold_detail
		| switchstatement
		| forstatement
		| loopstatement
		| ifstatement
		| chancestatement
		| exitstatement
		| breakstatement
		| function_define
		| function_call
		| ladder_detail
		| lever_detail
		| map_definition
		| mazewalk_detail
		| monster_detail
		| object_detail
		| passwall_detail
		| pool_detail
		| portal_region
		| random_corridors
		| naming_detail
		| special_region_detail
		| special_levregion_detail
		| region_detail
		| room_def
		| subroom_def
		| sink_detail
		| terrain_detail
		| replace_terrain_detail
		| stair_detail
		| stair_region
		| teleprt_region
		| trap_detail
		| wallify_detail
		;

any_var_array	: VARSTRING_INT_ARRAY
		| VARSTRING_STRING_ARRAY
		| VARSTRING_VAR_ARRAY
		| VARSTRING_COORD_ARRAY
		| VARSTRING_REGION_ARRAY
		| VARSTRING_MAPCHAR_ARRAY
		| VARSTRING_MONST_ARRAY
		| VARSTRING_OBJ_ARRAY
		| VARSTRING_SEL_ARRAY
		;

any_var		: VARSTRING_INT
		| VARSTRING_STRING
		| VARSTRING_VAR
		| VARSTRING_COORD
		| VARSTRING_REGION
		| VARSTRING_MAPCHAR
		| VARSTRING_MONST
		| VARSTRING_OBJ
		| VARSTRING_SEL
		;

any_var_or_arr	: any_var_array
		| any_var
		| VARSTRING
		;

any_var_or_unk	: VARSTRING
		| any_var
		;

shuffle_detail	: SHUFFLE_ID ':' any_var_array
		  {
		      struct lc_vardefs *vd;

		      if ((vd = vardef_defined(vardefs, $3, 1))) {
			  if (!(vd->var_type & SPOVAR_ARRAY))
			      lc_error("Trying to shuffle non-array variable '%s'",
                                       $3);
		      } else
                          lc_error("Trying to shuffle undefined variable '%s'",
                                   $3);
		      add_opvars(splev, "so", VA_PASS2($3, SPO_SHUFFLE_ARRAY));
		      Free($3);
		  }
		;

variable_define	: any_var_or_arr '=' math_expr_var
		  {
		      vardefs = add_vardef_type(vardefs, $1, SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, $1, SPO_VAR_INIT));
		      Free($1);
		  }
		| any_var_or_arr '=' selection_ID ':' ter_selection
		  {
		      vardefs = add_vardef_type(vardefs, $1, SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, $1, SPO_VAR_INIT));
		      Free($1);
		  }
		| any_var_or_arr '=' string_expr
		  {
		      vardefs = add_vardef_type(vardefs, $1, SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, $1, SPO_VAR_INIT));
		      Free($1);
		  }
		| any_var_or_arr '=' terrainid ':' mapchar_or_var
		  {
		      vardefs = add_vardef_type(vardefs, $1, SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, $1, SPO_VAR_INIT));
		      Free($1);
		  }
		| any_var_or_arr '=' monsterid ':' monster_or_var
		  {
		      vardefs = add_vardef_type(vardefs, $1, SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, $1, SPO_VAR_INIT));
		      Free($1);
		  }
		| any_var_or_arr '=' objectid ':' object_or_var
		  {
		      vardefs = add_vardef_type(vardefs, $1, SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, $1, SPO_VAR_INIT));
		      Free($1);
		  }
		| any_var_or_arr '=' coord_or_var
		  {
		      vardefs = add_vardef_type(vardefs, $1, SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, $1, SPO_VAR_INIT));
		      Free($1);
		  }
		| any_var_or_arr '=' region_or_var
		  {
		      vardefs = add_vardef_type(vardefs, $1, SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, $1, SPO_VAR_INIT));
		      Free($1);
		  }
		| any_var_or_arr '=' '{' integer_list '}'
		  {
		      int n_items = (int) $4;

		      vardefs = add_vardef_type(vardefs, $1,
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, $1, SPO_VAR_INIT));
		      Free($1);
		  }
		| any_var_or_arr '=' '{' encodecoord_list '}'
		  {
		      int n_items = (int) $4;

		      vardefs = add_vardef_type(vardefs, $1,
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, $1, SPO_VAR_INIT));
		      Free($1);
		  }
		| any_var_or_arr '=' '{' encoderegion_list '}'
		  {
                      int n_items = (int) $4;

		      vardefs = add_vardef_type(vardefs, $1,
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, $1, SPO_VAR_INIT));
		      Free($1);
		  }
		| any_var_or_arr '=' terrainid ':' '{' mapchar_list '}'
		  {
                      int n_items = (int) $6;

		      vardefs = add_vardef_type(vardefs, $1,
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, $1, SPO_VAR_INIT));
		      Free($1);
		  }
		| any_var_or_arr '=' monsterid ':' '{' encodemonster_list '}'
		  {
		      int n_items = (int) $6;

		      vardefs = add_vardef_type(vardefs, $1,
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, $1, SPO_VAR_INIT));
		      Free($1);
		  }
		| any_var_or_arr '=' objectid ':' '{' encodeobj_list '}'
		  {
                      int n_items = (int) $6;

		      vardefs = add_vardef_type(vardefs, $1,
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, $1, SPO_VAR_INIT));
		      Free($1);
		  }
		| any_var_or_arr '=' '{' string_list '}'
		  {
                      int n_items = (int) $4;

		      vardefs = add_vardef_type(vardefs, $1,
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, $1, SPO_VAR_INIT));
		      Free($1);
		  }
		;

encodeobj_list	: encodeobj
		  {
		      add_opvars(splev, "O", VA_PASS1($1));
		      $$ = 1;
		  }
		| encodeobj_list ',' encodeobj
		  {
		      add_opvars(splev, "O", VA_PASS1($3));
		      $$ = 1 + $1;
		  }
		;

encodemonster_list	: encodemonster
		  {
		      add_opvars(splev, "M", VA_PASS1($1));
		      $$ = 1;
		  }
		| encodemonster_list ',' encodemonster
		  {
		      add_opvars(splev, "M", VA_PASS1($3));
		      $$ = 1 + $1;
		  }
		;

mapchar_list	: mapchar
		  {
		      add_opvars(splev, "m", VA_PASS1($1));
		      $$ = 1;
		  }
		| mapchar_list ',' mapchar
		  {
		      add_opvars(splev, "m", VA_PASS1($3));
		      $$ = 1 + $1;
		  }
		;

encoderegion_list	: encoderegion
		  {
		      $$ = 1;
		  }
		| encoderegion_list ',' encoderegion
		  {
		      $$ = 1 + $1;
		  }
		;

encodecoord_list	: encodecoord
		  {
		      add_opvars(splev, "c", VA_PASS1($1));
		      $$ = 1;
		  }
		| encodecoord_list ',' encodecoord
		  {
		      add_opvars(splev, "c", VA_PASS1($3));
		      $$ = 1 + $1;
		  }
		;

integer_list	: math_expr_var
		  {
		      $$ = 1;
		  }
		| integer_list ',' math_expr_var
		  {
		      $$ = 1 + $1;
		  }
		;

string_list	: string_expr
		  {
		      $$ = 1;
		  }
		| string_list ',' string_expr
		  {
		      $$ = 1 + $1;
		  }
		;

function_define	: FUNCTION_ID NQSTRING '('
		  {
		      struct lc_funcdefs *funcdef;

		      if (in_function_definition)
			  lc_error("Recursively defined functions not allowed (function %s).", $2);

		      in_function_definition++;

		      if (funcdef_defined(function_definitions, $2, 1))
			  lc_error("Function '%s' already defined once.", $2);

		      funcdef = funcdef_new(-1, $2);
		      funcdef->next = function_definitions;
		      function_definitions = funcdef;
		      function_splev_backup = splev;
		      splev = &(funcdef->code);
		      Free($2);
		      curr_function = funcdef;
		      function_tmp_var_defs = vardefs;
		      vardefs = NULL;
		  }
		func_params_list ')'
		  {
		      /* nothing */
		  }
		stmt_block
		  {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  }
		;

function_call	: NQSTRING '(' func_call_params_list ')'
		  {
		      struct lc_funcdefs *tmpfunc;

		      tmpfunc = funcdef_defined(function_definitions, $1, 1);
		      if (tmpfunc) {
			  int l;
			  int nparams = (int) strlen($3);
			  char *fparamstr = funcdef_paramtypes(tmpfunc);

			  if (strcmp($3, fparamstr)) {
			      char *tmps = strdup(decode_parm_str(fparamstr));

			      lc_error("Function '%s' requires params '%s', got '%s' instead.",
                                       $1, tmps, decode_parm_str($3));
			      Free(tmps);
			  }
			  Free(fparamstr);
			  Free($3);
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
			  lc_error("Function '%s' not defined.", $1);
		      }
		      Free($1);
		  }
		;

exitstatement	: EXIT_ID
		  {
		      add_opcode(splev, SPO_EXIT, NULL);
		  }
		;

opt_percent	: /* nothing */
		  {
		      $$ = 100;
		  }
		| PERCENT
		  {
		      $$ = $1;
		  }
		;

comparestmt     : PERCENT
                  {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) $1, 100, SPO_RN2));
		      $$ = SPO_JG;
                  }
		| '[' math_expr_var COMPARE_TYPE math_expr_var ']'
                  {
		      $$ = $3;
                  }
		| '[' math_expr_var ']'
                  {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      $$ = SPO_JNE;
                  }
		;

switchstatement	: SWITCH_ID
		  {
		      is_inconstant_number = 0;
		  }
		  '[' integer_or_var ']'
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
		  }
		'{' switchcases '}'
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


		  }
		;

switchcases	: /* nothing */
		| switchcase switchcases
		;

switchcase	: CASE_ID all_integers ':'
		  {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = $2;
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  }
		levstatements
		  {
		  }
		| DEFAULT_ID ':'
		  {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  }
		levstatements
		  {
		  }
		;

breakstatement	: BREAK_ID
		  {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  }
		;

for_to_span	: '.' '.'
		| TO_ID
		;

forstmt_start	: FOR_ID any_var_or_unk '=' math_expr_var for_to_span math_expr_var
		  {
		      char buf[256], buf2[256];

		      if (n_forloops >= MAX_NESTED_IFS) {
			  lc_error("FOR: Too deeply nested loops.");
			  n_forloops = MAX_NESTED_IFS - 1;
		      }

		      /* first, define a variable for the for-loop end value */
		      Sprintf(buf, "%s end", $2);
		      /* the value of which is already in stack (the 2nd math_expr) */
		      add_opvars(splev, "iso", VA_PASS3(0, buf, SPO_VAR_INIT));

		      vardefs = add_vardef_type(vardefs, $2, SPOVAR_INT);
		      /* define the for-loop variable. value is in stack (1st math_expr) */
		      add_opvars(splev, "iso", VA_PASS3(0, $2, SPO_VAR_INIT));

		      /* calculate value for the loop "step" variable */
		      Sprintf(buf2, "%s step", $2);
		      /* end - start */
		      add_opvars(splev, "vvo",
				 VA_PASS3(buf, $2, SPO_MATH_SUB));
		      /* sign of that */
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SIGN));
		      /* save the sign into the step var */
		      add_opvars(splev, "iso",
				 VA_PASS3(0, buf2, SPO_VAR_INIT));

		      forloop_list[n_forloops].varname = strdup($2);
		      forloop_list[n_forloops].jmp_point = splev->n_opcodes;

		      n_forloops++;
		      Free($2);
		  }
		;

forstatement	: forstmt_start
		  {
		      /* nothing */
		      break_stmt_start();
		  }
		 stmt_block
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
		  }
		;

loopstatement	: LOOP_ID '[' integer_or_var ']'
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
		  }
		 stmt_block
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
		  }
		;

chancestatement	: comparestmt ':'
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

		      add_opcode(splev, reverse_jmp_opcode( $1 ), NULL);

		  }
		levstatement
		  {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  }
		;

ifstatement 	: IF_ID comparestmt
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

		      add_opcode(splev, reverse_jmp_opcode( $2 ), NULL);

		  }
		 if_ending
		  {
		     /* do nothing */
		  }
		;

if_ending	: stmt_block
		  {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  }
		| stmt_block
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
		  }
		 ELSE_ID stmt_block
		  {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  }
		;

message		: MESSAGE_ID ':' string_expr
		  {
		      add_opvars(splev, "iiiiiiio", VA_PASS8(0, 0, 0, 0, NO_COLOR, ATR_NONE, SPLEV_MESSAGE_TYPE_MESSAGE, SPO_MESSAGE));
		  }
		| MESSAGE_ID ':' string_expr ',' MESSAGE_TYPE ',' MESSAGE_ATTR ',' MESSAGE_COLOR ',' INTEGER
		  {
		      add_opvars(splev, "iiiiiiio", VA_PASS8((int)$11, 0, 0, 0, (int)$9, (int)$7, (int)$5, SPO_MESSAGE));
		  }
		| MESSAGE_ID ':' string_expr ',' MESSAGE_TYPE ',' MESSAGE_ATTR ',' MESSAGE_COLOR ',' MESSAGE_SOUND_TYPE ',' MESSAGE_SOUND_ID ',' INTEGER ',' INTEGER
		  {
		      add_opvars(splev, "iiiiiiio", VA_PASS8((int)$17, (int)$15, (int)$13, (int)$11, (int)$9, (int)$7, (int)$5, SPO_MESSAGE));
		  }		
		;

random_corridors: RAND_CORRIDOR_ID
		  {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  }
		| RAND_CORRIDOR_ID ':' all_integers
		  {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, $3, -1, -1, -1, -1, SPO_CORRIDOR));
		  }
		| RAND_CORRIDOR_ID ':' RANDOM_TYPE
		  {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  }
		;

corridor	: CORRIDOR_ID ':' corr_spec ',' corr_spec
		  {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7($3.room, $3.door, $3.wall,
					  $5.room, $5.door, $5.wall,
					  SPO_CORRIDOR));
		  }
		| CORRIDOR_ID ':' corr_spec ',' all_integers
		  {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7($3.room, $3.door, $3.wall,
					  -1, -1, (int64_t)$5,
					  SPO_CORRIDOR));
		  }
		;

corr_spec	: '(' INTEGER ',' DIRECTION ',' door_pos ')'
		  {
			$$.room = $2;
			$$.wall = $4;
			$$.door = $6;
		  }
		;

room_begin      : room_type opt_percent ',' light_state
                  {
		      if (($2 < 100) && ($1 == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((int64_t)$1, (int64_t)$2, (int64_t)$4));
		      }
                  }
                ;

subroom_def	: SUBROOM_ID ':' room_begin ',' subroom_pos ',' room_size optroomregionflags optfloortype optfloorsubtype opttileset optdecotyp optmontype
		  {
		      int64_t rflags = $8;
		      int64_t flmt = (int64_t)$<i>9;
		      int64_t flt = (int64_t)$<i>10;
		      int64_t tlset = (int64_t)$<i>11;
		      int64_t decotyp = (int64_t)$<i>12;

		      if (rflags == -1) rflags = ((int64_t)1 << 0);
		      //if (flmt == -1) flmt = ROOM;
		      //if (flt == -1) flt = 0;

		      add_opvars(splev, "iiiiiiiiiiio",
				 VA_PASS12(decotyp, tlset, flt, flmt, rflags, ERR, ERR,
					  $5.x, $5.y, $7.width, $7.height,
					  SPO_SUBROOM));
		      break_stmt_start();
		  }
		  stmt_block
		  {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  }
		;

room_def	: ROOM_ID ':' room_begin ',' room_pos ',' room_align ',' room_size optroomregionflags optfloortype optfloorsubtype opttileset optdecotyp optmontype
		  {
		      int64_t rflags = $10;
		      int64_t flmt = (int64_t)$<i>11;
		      int64_t flt = (int64_t)$<i>12;
		      int64_t tlset = (int64_t)$<i>13;
		      int64_t decotyp = (int64_t)$<i>14;

		      if (rflags == -1) rflags = ((int64_t)1 << 0);
		      //if (flmt == -1) flmt = ROOM;
		      //if (flt == -1) flt = 0;

		      add_opvars(splev, "iiiiiiiiiiio",
				 VA_PASS12(decotyp, tlset, flt, flmt, rflags,
					  $7.x, $7.y, $5.x, $5.y,
					  $9.width, $9.height, SPO_ROOM));
		      break_stmt_start();
		  }
		  stmt_block
		  {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  }
		;

roomfill	: /* nothing */
		  {
			$$ = 1;
		  }
		| ',' BOOLEAN
		  {
			$$ = $2;
		  }
		;

room_pos	: '(' INTEGER ',' INTEGER ')'
		  {
			if ( $2 < 1 || $2 > 5 ||
			    $4 < 1 || $4 > 5 ) {
			    lc_error("Room positions should be between 1-5: (%li,%li)!", $2, $4);
			} else {
			    $$.x = $2;
			    $$.y = $4;
			}
		  }
		| RANDOM_TYPE
		  {
			$$.x = $$.y = ERR;
		  }
		;

subroom_pos	: '(' INTEGER ',' INTEGER ')'
		  {
			if ( $2 < 0 || $4 < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", $2, $4);
			} else {
			    $$.x = $2;
			    $$.y = $4;
			}
		  }
		| RANDOM_TYPE
		  {
			$$.x = $$.y = ERR;
		  }
		;

room_align	: '(' h_justif ',' v_justif ')'
		  {
		      $$.x = $2;
		      $$.y = $4;
		  }
		| RANDOM_TYPE
		  {
		      $$.x = $$.y = ERR;
		  }
		;

room_size	: '(' INTEGER ',' INTEGER ')'
		  {
			$$.width = $2;
			$$.height = $4;
		  }
		| RANDOM_TYPE
		  {
			$$.height = $$.width = ERR;
		  }
		;

door_detail	: ROOMDOOR_ID ':' secret ',' door_state ',' door_wall ',' door_pos door_infos
		  {
			/* ERR means random here */
			if ($7 == ERR && $9 != ERR) {
			    lc_error("If the door wall is random, so must be its pos!");
			} else {
			    add_opvars(splev, "iiiio",
				       VA_PASS5((int64_t)$9, (int64_t)$5, (int64_t)$3,
						(int64_t)$7, SPO_ROOM_DOOR));
			}
		  }
		| DOOR_ID ':' door_state ',' ter_selection door_infos
		  {
		      add_opvars(splev, "io", VA_PASS2((int64_t)$3, SPO_DOOR));
		  }
		;

secret		: BOOLEAN
		| RANDOM_TYPE
		;

door_wall	: dir_list
		| RANDOM_TYPE
		;

dir_list	: DIRECTION
		  {
		      $$ = $1;
		  }
		| DIRECTION '|' dir_list
		  {
		      $$ = ($1 | $3);
		  }
		;

door_pos	: INTEGER
		| RANDOM_TYPE
		;

door_infos	: /* nothing */
		  {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      $<i>$ = 0x00;
		  }
		| door_infos ',' door_info
		  {
		      if (( $<i>1 & $<i>3 ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      $<i>$ = ( $<i>1 | $<i>3 );
		  }
		;

door_info	: LOCATION_SUBTYPE_ID ':' DOOR_SUBTYPE
		  {	
		      add_opvars(splev, "ii", VA_PASS2($<i>3, SP_D_V_SUBTYPE));
		      $<i>$ = 0x0001;
		  }
		| KEYTYPE_ID ':'  object_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      $<i>$ = 0x0002;
		  }
		| SPECIAL_QUALITY_ID ':' INTEGER
		  {
		      add_opvars(splev, "ii", VA_PASS2($<i>3, SP_D_V_SPECIAL_QUALITY));
		      $<i>$ = 0x0004;
		  }
		| INDESTRUCTIBLE_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      $<i>$ = 0x0008;
		  }
		| SECRET_DOOR_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      $<i>$ = 0x0010;
		  }
		| USES_UP_KEY_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      $<i>$ = 0x0020;
		  }
		| NON_PASSDOOR_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      $<i>$ = 0x0040;
		  }
		;


map_definition	: NOMAP_ID
		  {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  }
		| GEOMETRY_ID ':' h_justif ',' v_justif roomfill MAP_ID
		  {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(($3), ($5)),
					  1, (int) $6));
		      scan_map($7, splev);
		      Free($7);
		  }
		| GEOMETRY_ID ':' coord_or_var roomfill MAP_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) $4));
		      scan_map($5, splev);
		      Free($5);
		  }
		;

h_justif	: LEFT_OR_RIGHT
		| CENTER
		;

v_justif	: TOP_OR_BOT
		| CENTER
		;

monster_detail	: MONSTER_ID ':' monster_desc
		  {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  }
		| MONSTER_ID ':' monster_desc
		  {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  }
		stmt_block
		 {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 }
		;

monster_desc	: monster_or_var ',' coord_or_var monster_infos
		  {
		      /* nothing */
		  }
		;

monster_infos	: /* nothing */
		  {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      $$ = 0x00000000;
		  }
		| monster_infos ',' monster_info
		  {
		      if (( $1 & $3 ))
			  lc_error("MONSTER extra info defined twice.");
		      $$ = ( $1 | $3 );
		  }
		;

monster_info	: string_expr
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      $$ = 0x00000001;
		  }
		| MON_ATTITUDE
		  {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) $<i>1, SP_M_V_PEACEFUL));
		      $$ = 0x00000002;
		  }
		| MON_ALERTNESS
		  {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) $<i>1, SP_M_V_ASLEEP));
		      $$ = 0x00000004;
		  }
		| alignment_prfx
		  {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) $1, SP_M_V_ALIGN));
		      $$ = 0x00000008;
		  }
		| MON_APPEARANCE string_expr
		  {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) $<i>1, SP_M_V_APPEAR));
		      $$ = 0x00000010;
		  }
		| FEMALE_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      $$ = 0x00000020;
		  }
		| MALE_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(0, SP_M_V_FEMALE));
		      $$ = 0x00000020;
		  }
		| INVIS_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      $$ = 0x00000040;
		  }
		| CANCELLED_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      $$ = 0x00000080;
		  }
		| REVIVED_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      $$ = 0x00000100;
		  }
		| REVIVED_ID ':' integer_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_REVIVED));
		      $$ = 0x00000100;
		  }
		| AVENGE_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      $$ = 0x00000200;
		  }
		| FLEEING_ID ':' integer_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      $$ = 0x00000400;
		  }
		| BLINDED_ID ':' integer_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      $$ = 0x00000800;
		  }
		| PARALYZED_ID ':' integer_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      $$ = 0x00001000;
		  }
		| STUNNED_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      $$ = 0x00002000;
		  }
		| CONFUSED_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      $$ = 0x00004000;
		  }
		| SEENTRAPS_ID ':' seen_trap_mask
		  {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) $3, SP_M_V_SEENTRAPS));
		      $$ = 0x00008000;
		  }
		| SEENTRAPS_ID
		  {
		      add_opvars(splev, "ii",
				 VA_PASS2(0xFFFFFFFFL, SP_M_V_SEENTRAPS));
		      $$ = 0x00008000;
		  }
		| WAITFORU_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      $$ = 0x00010000;
		  }
		| PROTECTOR_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_PROTECTOR));
		      $$ = 0x00020000;
		  }
		| MAXHP_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_MAXHP));
		      $$ = 0x00040000;
		  }
		| LEVEL_ADJUSTMENT_ID ':' integer_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_LEVEL_ADJUSTMENT));
		      $$ = 0x00080000;
		  }
		| KEEP_ORIGINAL_INVENTORY_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_KEEP_ORIGINAL_INVENTORY));
		      $$ = 0x00100000;
		  }
		| LEVEL_BOSS_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_LEVEL_BOSS));
		      $$ = 0x00200000;
		  }
		| BOSS_HOSTILITY_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_BOSS_HOSTILITY));
		      $$ = 0x00400000;
		  }
		| HAS_BACKUP_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_HAS_BACKUP));
		      $$ = 0x00800000;
		  }
		;

seen_trap_mask	: STRING
		  {
		      int token = get_trap_type($1);

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", $1);
                      Free($1);
		      $$ = ((int64_t)1 << (token - 1));
		  }
		| ALL_ID
		  {
		      $$ = (int64_t) ~0;
		  }
		| STRING '|' seen_trap_mask
		  {
		      int token = get_trap_type($1);
		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", $1);

		      if (((int64_t)1 << (token - 1)) & (int64_t)($3))
			  lc_error("Monster seen_traps, trap '%s' listed twice.", $1);
                      Free($1);
		      $$ = (((int64_t)1 << (token - 1)) | (int64_t)($3));
		  }
		;

object_detail	: OBJECT_ID ':' object_desc
		  {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  }
		| COBJECT_ID ':' object_desc
		  {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  }
		stmt_block
		 {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 }
		;

object_desc	: object_or_var object_infos
		  {
		      if (( $2 & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( $2 & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  }
		;

object_infos	: /* nothing */
		  {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      $$ = 0x00;
		  }
		| object_infos ',' object_info
		  {
		      if (( $1 & $3 ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      $$ = ( $1 | $3 );
		  }
		;

object_info	: CURSE_TYPE
		  {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) $1, SP_O_V_CURSE));
		      $$ = 0x0001;
		  }
		| MONTYPE_ID ':' monster_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      $$ = 0x0002;
		  }
		| all_ints_push
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      $$ = 0x0004;
		  }
		| NAME_ID ':' string_expr
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      $$ = 0x0008;
		  }
		| QUANTITY_ID ':' integer_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      $$ = 0x0010;
		  }
		| BURIED_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      $$ = 0x0020;
		  }
		| LIGHT_STATE
		  {
		      add_opvars(splev, "ii", VA_PASS2((int) $1, SP_O_V_LIT));
		      $$ = 0x0040;
		  }
		| ERODED_ID ':' integer_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      $$ = 0x0080;
		  }
		| ERODEPROOF_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      $$ = 0x0080;
		  }
		| DOOR_STATE
		  {
		      if ($1 == D_LOCKED) {
			  add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_LOCKED));
			  $$ = 0x0100;
		      } else if ($1 == D_BROKEN) {
			  add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BROKEN));
			  $$ = 0x0200;
		      } else if ($1 == D_ISOPEN) {
			  add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_OPEN));
			  $$ = 0x2000000;
		      } else
			  lc_error("DOOR state can only be locked or broken.");
		  }
		| TRAPPED_STATE
		  {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) $1, SP_O_V_TRAPPED));
		      $$ = 0x0400;
		  }
		| RECHARGED_ID ':' integer_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      $$ = 0x0800;
		  }
		| INVIS_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      $$ = 0x1000;
		  }
		| GREASED_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      $$ = 0x2000;
		  }
		| coord_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      $$ = 0x4000;
		  }
		| ELEMENTAL_ENCHANTMENT_ID ':' ELEMENTAL_ENCHANTMENT_TYPE
		  {
		      add_opvars(splev, "ii", VA_PASS2((int)$<i>3, SP_O_V_ELEMENTAL_ENCHANTMENT));
		      $$ = 0x8000;
		  }
		| EXCEPTIONALITY_ID ':' EXCEPTIONALITY_TYPE
		  {
		      add_opvars(splev, "ii", VA_PASS2((int)$<i>3, SP_O_V_EXCEPTIONALITY));
		      $$ = 0x00010000;
		  }
		| ENCHANTMENT_ID ':' integer_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      $$ = 0x00020000;
		  }
		| CHARGES_ID ':' integer_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      $$ = 0x00040000;
		  }
		| SPECIAL_QUALITY_ID ':' integer_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      $$ = 0x00080000;
		  }
		| MANUAL_TYPE_ID ':' MANUAL_TYPE
		  {
		      add_opvars(splev, "ii", VA_PASS2((int)$<i>3, SP_O_V_SPECIAL_QUALITY));
		      $$ = 0x00080000;
		  }
		| SPEFLAGS_ID ':' integer_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      $$ = 0x00100000;
		  }
		| KEYTYPE_ID ':' object_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      $$ = 0x00200000;
		  }
		| INDESTRUCTIBLE_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      $$ = 0x00400000;
		  }
		| USES_UP_KEY_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      $$ = 0x00800000;
		  }
		| NO_PICKUP_ID
		  {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      $$ = 0x01000000;
		  }
        | MYTHIC_TYPE
		  {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) $1, SP_O_V_MYTHIC_TYPE));
		      $$ = 0x02000000;
		  }		
	    | MYTHIC_PREFIX_ID ':' MYTHIC_PREFIX_TYPE
		  {
		      add_opvars(splev, "ii", VA_PASS2((int)$<i>3, SP_O_V_MYTHIC_PREFIX));
		      $$ = 0x04000000;
		  }
		| MYTHIC_SUFFIX_ID ':' MYTHIC_SUFFIX_TYPE
		  {
		      add_opvars(splev, "ii", VA_PASS2((int)$<i>3, SP_O_V_MYTHIC_SUFFIX));
		      $$ = 0x08000000;
		  }
		| AGE_ID ':' integer_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      $$ = 0x10000000;
		  }
		| MATERIAL_ID ':' MATERIAL_TYPE
		  {
		      add_opvars(splev, "ii", VA_PASS2((int)$<i>3, SP_O_V_MATERIAL));
		      $$ = 0x20000000;
		  }
		;

trap_detail	: TRAP_ID ':' trap_name ',' coord_or_var
		  {
		      add_opvars(splev, "io", VA_PASS2((int) $3, SPO_TRAP));
		  }
		;

drawbridge_detail: DRAWBRIDGE_ID ':' coord_or_var ',' DIRECTION ',' door_state
		   {
		       int64_t dir, state = 0;

		       /* convert dir from a DIRECTION to a DB_DIR */
		       dir = $5;
		       switch (dir) {
		       case W_NORTH: dir = DB_NORTH; break;
		       case W_SOUTH: dir = DB_SOUTH; break;
		       case W_EAST:  dir = DB_EAST;  break;
		       case W_WEST:  dir = DB_WEST;  break;
		       default:
			   lc_error("Invalid drawbridge direction.");
			   break;
		       }

		       if ( $7 == D_ISOPEN )
			   state = 1;
		       else if ( $7 == D_CLOSED )
			   state = 0;
		       else if ( $7 == -1 )
			   state = -1;
		       else
			   lc_error("A drawbridge can only be open, closed or random!");
		       add_opvars(splev, "iio",
				  VA_PASS3(state, dir, SPO_DRAWBRIDGE));
		   }
		;

mazewalk_detail : MAZEWALK_ID ':' coord_or_var ',' DIRECTION
		  {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) $5, 1, 0, SPO_MAZEWALK));
		  }
		| MAZEWALK_ID ':' coord_or_var ',' DIRECTION ',' BOOLEAN opt_fillchar
		  {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) $5, (int) $<i>7,
					  (int) $8, SPO_MAZEWALK));
		  }
		;

wallify_detail	: WALLIFY_ID
		  {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  }
		| WALLIFY_ID ':' ter_selection
		  {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  }
		| WALLIFY_ID ':' ALL_ID
		  {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  }
		;

ladder_detail	: LADDER_ID ':' coord_or_var ',' UP_OR_DOWN
		  {
		      add_opvars(splev, "io",
				 VA_PASS2((int) $<i>5, SPO_LADDER));
		  }
		;

stair_detail	: STAIR_ID ':' coord_or_var ',' UP_OR_DOWN
		  {
		      add_opvars(splev, "io",
				 VA_PASS2((int) $<i>5, SPO_STAIR));
		  }
		;

stair_region	: STAIR_ID ':' lev_region ',' lev_region ',' UP_OR_DOWN
		  {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14($3.x1, $3.y1, $3.x2, $3.y2, $3.area,
					   $5.x1, $5.y1, $5.x2, $5.y2, $5.area,
				     (int64_t) (($7) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  }
		;

portal_region	: PORTAL_ID ':' lev_region ',' lev_region ',' STRING
		  {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14($3.x1, $3.y1, $3.x2, $3.y2, $3.area,
					   $5.x1, $5.y1, $5.x2, $5.y2, $5.area,
					   LR_PORTAL, 0, $7, SPO_LEVREGION));
		      Free($7);
		  }
		;

teleprt_region	: TELEPRT_ID ':' lev_region ',' lev_region teleprt_detail
		  {
		      int64_t rtyp = 0;
		      switch($6) {
		      case -1: rtyp = LR_TELE; break;
		      case  0: rtyp = LR_DOWNTELE; break;
		      case  1: rtyp = LR_UPTELE; break;
		      case  2: rtyp = LR_NOTELE; break;
		      }
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14($3.x1, $3.y1, $3.x2, $3.y2, $3.area,
					   $5.x1, $5.y1, $5.x2, $5.y2, $5.area,
					   rtyp, 0, (char *)0, SPO_LEVREGION));
		  }
		;

branch_region	: BRANCH_ID ':' lev_region ',' lev_region
		  {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14($3.x1, $3.y1, $3.x2, $3.y2, $3.area,
					   $5.x1, $5.y1, $5.x2, $5.y2, $5.area,
					   (int64_t) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  }
		;

teleprt_detail	: /* empty */
		  {
			$$ = -1;
		  }
		| ',' UP_OR_DOWN
		  {
			$$ = $2;
		  }
		;

fountain_detail : FOUNTAIN_ID ':' ter_selection ',' FOUNTAIN_TYPE
		  {
		      add_opvars(splev, "io", VA_PASS2((int)$5, SPO_FOUNTAIN));
		  }
		;

throne_detail : THRONE_ID ':' ter_selection
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  }
		;

modron_portal_detail : MODRON_PORTAL_ID ':' coord_or_var ',' TARGET_ID ':' coord_or_var ',' MODRON_PORTAL_TYPE ',' ACTIVE_OR_INACTIVE
		  {
		      add_opvars(splev, "iio", VA_PASS3((int) $<i>9, (int) $<i>11, SPO_MODRON_PORTAL));
		  }
		| MODRON_PORTAL_ID ':' coord_or_var ',' LEVEL_TELEPORTER_ID ',' LEVEL_TELEPORT_DIRECTION_TYPE ',' LEVEL_TELEPORT_END_TYPE ',' MODRON_PORTAL_TYPE ',' ACTIVE_OR_INACTIVE
		  {
		      add_opvars(splev, "iiiio", VA_PASS5((int) $<i>7, (int) $<i>9, (int) $<i>11, (int) $<i>13, SPO_MODRON_LEVEL_TELEPORTER));
		  }
		;

lever_detail : LEVER_ID ':' coord_or_var ',' LEVER_EFFECT_TYPE lever_infos
		  {
		      add_opvars(splev, "io", VA_PASS2((int) $<i>5, SPO_LEVER));
		  }
		;

lever_infos	: /* nothing */
		  {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      $<i>$ = 0x00;
		  }
		| lever_infos ',' lever_info
		  {
		      if (( $<i>1 & $<i>3 ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      $<i>$ = ( $<i>1 | $<i>3 );
		  }
		;

lever_info	: ACTIVE_OR_INACTIVE
		  {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) $<i>1, SP_L_V_ACTIVE));
		      $<i>$ = 0x0001;
		  }
		| MONTYPE_ID ':' monster_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      $<i>$ = 0x0002;
		  }
		| OBJTYPE_ID ':' object_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      $<i>$ = 0x0004;
		  }
		| TERTYPE_ID ':' terrain_type
		  {
		      add_opvars(splev, "ii", VA_PASS2((int)$3.ter, SP_L_V_TERRAIN));
		      $<i>$ = 0x0008;
		  }
		| TERTYPE2_ID ':' terrain_type
		  {
		      add_opvars(splev, "ii", VA_PASS2((int)$3.ter, SP_L_V_TERRAIN2));
		      $<i>$ = 0x0010;
		  }
	     | SWITCHABLE_ID
		  {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      $<i>$ = 0x0020;
		  }
	     | CONTINUOUSLY_USABLE_ID
		  {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      $<i>$ = 0x0040;
		  }
		| TARGET_ID ':' coord_or_var
		  {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      $<i>$ = 0x0080;
		  }
		| TRAPTYPE_ID ':' trap_name
		  {
		      add_opvars(splev, "ii", VA_PASS2($<i>3, SP_L_V_TRAP));
		      $<i>$ = 0x0100;
		  }
		| LOCATION_SUBTYPE_ID ':' valid_subtype
		  {
		      add_opvars(splev, "ii", VA_PASS2($<i>3, SP_L_V_SUBTYPE));
		      $<i>$ = 0x0200;
		  }
		| FLOOR_SUBTYPE_ID ':' floorsubtype
		  {
		      add_opvars(splev, "ii", VA_PASS2($<i>3, SP_L_V_FLOOR_SUBTYPE));
		      $<i>$ = 0x0400;
		  }
		| EFFECT_FLAG_ID ':' INTEGER
		  {
		      add_opvars(splev, "ii", VA_PASS2($<i>3, SP_L_V_EFFECT_FLAG));
		      $<i>$ = 0x0800;
		  }
		| SPECIAL_QUALITY_ID ':' INTEGER
		  {
		      add_opvars(splev, "ii", VA_PASS2($<i>3, SP_L_V_SPECIAL_QUALITY));
		      $<i>$ = 0x1000;
		  }
		;

valid_subtype : INTEGER | MODRON_PORTAL_TYPE | DOOR_SUBTYPE | FOUNTAIN_TYPE
        ;

sink_detail : SINK_ID ':' ter_selection
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  }
		;

pool_detail : POOL_ID ':' ter_selection
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  }
		;

terrain_type	: CHAR
		  {
		      $$.lit = -2;
		      $$.ter = what_map_char((char) $<i>1);
		  }
		| '(' CHAR ',' light_state ')'
		  {
		      $$.lit = $4;
		      $$.ter = what_map_char((char) $<i>2);
		  }
		;

replace_terrain_detail : REPLACE_TERRAIN_ID ':' region_or_var ',' mapchar_or_var ',' mapchar_or_var ',' SPERCENT
		  {
		      add_opvars(splev, "io",
				 VA_PASS2($9, SPO_REPLACETERRAIN));
		  }
		;

terrain_detail : TERRAIN_ID ':' ter_selection ',' mapchar_or_var
		 {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 }
	   | TERRAIN_ID ':' ter_selection ',' mapchar_or_var ',' FLOOR_SUBTYPE
		 {
		     add_opvars(splev, "io", VA_PASS2($<i>7, SPO_TERRAIN));
		 }
	       ;

diggable_detail : NON_DIGGABLE_ID ':' region_or_var
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  }
		;

passwall_detail : NON_PASSWALL_ID ':' region_or_var
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  }
		;

naming_detail : NAMING_ID ':' STRING ',' NAMING_TYPE
		  {
		      add_opvars(splev, "Miso", VA_PASS4(-1, $<i>5, $3, SPO_NAMING));
		      Free($3);
		  }
		| NAMING_ID ':' STRING ',' NAMING_TYPE ',' MONTYPE_ID ':' monster_or_var
		  {
		      add_opvars(splev, "iso", VA_PASS3($<i>5, $3, SPO_NAMING));
		      Free($3);
		  }
		;

special_region_detail : SPECIAL_REGION_ID ':' region_or_var ',' SPECIAL_REGION_TYPE
		  {
		      add_opvars(splev, "io", VA_PASS2($<i>5, SPO_SPECIAL_REGION));
		  }
		;

special_levregion_detail : SPECIAL_LEVREGION_ID ':' lev_region ',' SPECIAL_REGION_TYPE
		  {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14($3.x1, $3.y1, $3.x2, $3.y2, $3.area,
					   0, 0, 0, 0, 1,
					   $<i>5 == REGION_SPECIAL_LEVEL_SEEN ? (int64_t) LR_SPECIAL_MAP_SEEN : (int64_t) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  }
		;

special_tileset_detail : SPECIAL_TILESET_ID ':' ter_selection ',' CMAP_TYPE
		  {
		      add_opvars(splev, "io", VA_PASS2($<i>5, SPO_SPECIAL_TILESET));
		  }
		;

region_detail	: REGION_ID ':' region_or_var ',' light_state ',' room_type optroomregionflags optfloortype optfloorsubtype opttileset optdecotyp optmontype
		  {
		      int64_t irr;
		      int64_t rt = $7;
		      int64_t rflags = $8;
		      int64_t flmt = (int64_t)$<i>9;
		      int64_t flt = (int64_t)$<i>10;
		      int64_t tlset = (int64_t)$<i>11;
		      int64_t decotyp = (int64_t)$<i>12;

		      if (rflags == -1) rflags = ((int64_t)1 << 0);
		      //if (flmt == -1) flmt = 0;
		      //if (flt == -1) flt = 0;

		      if (!(rflags & 1)) rt += MAXRTYPE+1;
		      irr = ((rflags & 2) != 0);
		      add_opvars(splev, "iiiiiiio",
				 VA_PASS8((int64_t)$5, rt, rflags, flmt, flt, tlset, decotyp, SPO_REGION));
		      $<i>$ = (irr || (rflags & 1) || rt != OROOM);
		      break_stmt_start();
		  }
		  region_detail_end
		  {
		      break_stmt_end(splev);
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      /*if ( $<i>9 ||  $<i>10 ||  $<i>11 ) {
			  	add_opcode(splev, SPO_ENDROOM, NULL);
		       } else if ( $<i>12 )
			  	lc_error("Cannot use lev statements in non-permanent REGION");
		      */
		  }
		;

region_detail_end : /* nothing */
		  {
		      $$ = 0;
		  }
		| stmt_block
		  {
		      $$ = $1;
		  }
		;

altar_detail	: ALTAR_ID ':' coord_or_var ',' alignment ',' altar_type
		  {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (int64_t)$7, (int64_t)$5, SPO_ALTAR));
		  }
		| ALTAR_ID ':' coord_or_var ',' alignment ',' altar_type ',' MONTYPE_ID ':' monster_or_var
		  {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (int64_t)$7, (int64_t)$5, SPO_ALTAR));
		  }
        | ALTAR_ID ':' coord_or_var ',' alignment ',' altar_type ',' ALTAR_SUBTYPE
		  {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (int64_t)$9, (int64_t)$7, (int64_t)$5, SPO_ALTAR));
		  }
		| ALTAR_ID ':' coord_or_var ',' alignment ',' altar_type ',' ALTAR_SUBTYPE ',' MONTYPE_ID ':' monster_or_var
		  {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int64_t)$9, (int64_t)$7, (int64_t)$5, SPO_ALTAR));
		  }		;

anvil_detail : ANVIL_ID ':' coord_or_var
		  {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  }
		| ANVIL_ID ':' coord_or_var ',' MONTYPE_ID ':' monster_or_var
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  }
		;

decoration_detail : DECORATION_ID ':' coord_or_var ',' DECORATION_TYPE ',' INTEGER ',' DECORATION_DIR ',' DECORATION_ITEM_STATE
		  {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, (int)$11, (int)$9, (int)$7, (int)$5, SPO_DECORATION));
		  }
		| DECORATION_ID ':' coord_or_var ',' DECORATION_TYPE ',' INTEGER ',' DECORATION_DIR ',' DECORATION_ITEM_STATE ',' light_state
		  {
		      add_opvars(splev, "iiiiio", VA_PASS6((int)$13, (int)$11, (int)$9, (int)$7, (int)$5, SPO_DECORATION));
		  }
		| DECORATION_ID ':' coord_or_var ',' DECORATION_TYPE ',' PAINTING_TYPE ',' DECORATION_DIR
		  {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)$9, (int)$7, (int)$5, SPO_DECORATION));
		  }
		| DECORATION_ID ':' coord_or_var ',' DECORATION_TYPE ',' BANNER_TYPE ',' DECORATION_DIR
		  {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)$9, (int)$7, (int)$5, SPO_DECORATION));
		  }
		| DECORATION_ID ':' coord_or_var ',' DECORATION_TYPE ',' WALL_SCULPTURE_TYPE ',' DECORATION_DIR
		  {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)$9, (int)$7, (int)$5, SPO_DECORATION));
		  }
		| DECORATION_ID ':' coord_or_var ',' DECORATION_TYPE ',' DECORATION_DIR
		  {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, 1, (int)$7, 0, (int)$5, SPO_DECORATION));
		  }
		;

floor_detail : FLOOR_ID ':' ter_selection ',' FLOOR_TYPE ',' FLOOR_SUBTYPE
		  {
		      add_opvars(splev, "iio", VA_PASS3((int)$7, (int)$5, SPO_FLOOR));
		  }
		;

subtype_detail : SUBTYPE_ID ':' ter_selection ',' INTEGER
		  {
		      add_opvars(splev, "iio", VA_PASS3((int)$5, -1, SPO_SUBTYPE));
		  }
		| SUBTYPE_ID ':' ter_selection ',' FLOOR_TYPE ',' FLOOR_SUBTYPE
		  {
		      add_opvars(splev, "iio", VA_PASS3((int)$7, (int)$5, SPO_SUBTYPE));
		  }
		;

carpet_detail : CARPET_ID ':' region_or_var ',' CARPET_TYPE
		  {
		      add_opvars(splev, "io", VA_PASS2($<i>5, SPO_CARPET));
		  }
		;

carpet_piece_detail : CARPET_PIECE_ID ':' ter_selection ',' CARPET_TYPE ',' INTEGER ',' INTEGER
		  {
		      add_opvars(splev, "iiio", VA_PASS4((int)$9, (int)$7, (int)$5, SPO_CARPET_PIECE));
		  }
        | CARPET_PIECE_ID ':' ter_selection ',' CARPET_TYPE ',' INTEGER
		  {
		      add_opvars(splev, "iiio", VA_PASS4(0, (int)$7, (int)$5, SPO_CARPET_PIECE));
		  }		
		;

npc_detail : NPC_ID ':' NPC_TYPE ',' coord_or_var
		  {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)$3, SPO_NPC));
		  }
		|  NPC_ID ':' NPC_TYPE ',' coord_or_var ',' MONTYPE_ID ':' monster_or_var
		  {
		      add_opvars(splev, "io", VA_PASS2((int)$3, SPO_NPC));
		  }
		;

grave_detail	: GRAVE_ID ':' coord_or_var ',' string_expr
		  {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  }
		| GRAVE_ID ':' coord_or_var ',' RANDOM_TYPE
		  {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  }
		| GRAVE_ID ':' coord_or_var
		  {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  }
		;

brazier_detail	: BRAZIER_ID ':' coord_or_var ',' BRAZIER_SUBTYPE ',' light_state
		  {
		      add_opvars(splev, "iio", VA_PASS3((int)$7, (int)$5, SPO_BRAZIER));
		  }
		| BRAZIER_ID ':' coord_or_var
		  {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  }
		;

signpost_detail	: SIGNPOST_ID ':' coord_or_var ',' SIGNPOST_SUBTYPE ',' string_expr
		  {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)$5, SPO_SIGNPOST));
		  }
		| SIGNPOST_ID ':' coord_or_var ',' SIGNPOST_SUBTYPE ',' RANDOM_TYPE
		  {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)$5, SPO_SIGNPOST));
		  }
		| SIGNPOST_ID ':' coord_or_var
		  {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  }
		;

tree_detail	: TREE_ID ':' coord_or_var ',' TREE_SUBTYPE
		  {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)$5, SPO_TREE));
		  }
		| TREE_ID ':' coord_or_var ',' FOREST_TYPE
		  {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)$5, -1, SPO_TREE));
		  }
		| TREE_ID ':' coord_or_var
		  {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  }
		;


gold_detail	: GOLD_ID ':' math_expr_var ',' coord_or_var
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  }
		;

engraving_detail: ENGRAVING_ID ':' coord_or_var ',' engraving_type ',' string_expr
		  {
		      add_opvars(splev, "io",
				 VA_PASS2((int64_t)$5, SPO_ENGRAVING));
		  }
		;

mineralize	: MINERALIZE_ID ':' integer_or_var ',' integer_or_var ',' integer_or_var ',' integer_or_var
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  }
		| MINERALIZE_ID
		  {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  }
		;

trap_name	: STRING
		  {
			int token = get_trap_type($1);
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", $1);
			$$ = token;
			Free($1);
		  }
		| RANDOM_TYPE
		;

room_type	: STRING
		  {
			int token = get_room_type($1);
			if (token == ERR) {
			    lc_warning("Unknown room type \"%s\"!  Making ordinary room...", $1);
				$$ = OROOM;
			} else
				$$ = token;
			Free($1);
		  }
		| RANDOM_TYPE
		;

optroomregionflags : /* empty */
		  {
			$$ = -1;
		  }
		| ',' roomregionflags
		  {
			$$ = $2;
		  }
		;

roomregionflags : roomregionflag
		  {
			$$ = $1;
		  }
		| roomregionflag ',' roomregionflags
		  {
			$$ = $1 | $3;
		  }
		;

/* 0 is the "default" here */
roomregionflag : FILLING
		  {
		      $$ = ((int64_t)($1) << 0);
		  }
		| IRREGULAR
		  {
		      $$ = ((int64_t)($1) << 1);
		  }
		| JOINED
		  {
		      $$ = ((int64_t)($1) << 2);
		  }
		;

optfloortype : /* empty */
		  {
			$<i>$ = -1;
		  }
		| FLOOR_TYPE_ID ':' floortype
		  {
			$<i>$ = $<i>3;
		  }
		;

floortype : FLOOR_TYPE
		  {
		      $<i>$ = $<i>1;
		  }
		;

optfloorsubtype : /* empty */
		  {
			$<i>$ = -1;
		  }
		| FLOOR_SUBTYPE_ID ':' floorsubtype
		  {
			$<i>$ = $<i>3;
		  }
		;

floorsubtype : FLOOR_SUBTYPE
		  {
		      $<i>$ = $<i>1;
		  }
		;

optmontype : /* empty */
		  {
			add_opvars(splev, "M", VA_PASS1(-1));
			$<i>$ = -1;
		  }
		| MONTYPE_ID ':' monster_or_var
		  {
			$<i>$ = $<i>3;
		  }
		;

opttileset : /* empty */
		  {
			$<i>$ = -1;
		  }
		| TILESET_PARAM_ID ':' CMAP_TYPE
		  {
			$<i>$ = $<i>3;
		  }
		;

optdecotyp : /* empty */
		  {
			$<i>$ = -1;
		  }
		| DECOTYP_ID ':' INTEGER
		  {
			$<i>$ = $<i>3;
		  }
		;

door_state	: DOOR_STATE
		| RANDOM_TYPE
		;

light_state	: LIGHT_STATE
		| RANDOM_TYPE
		;

alignment	: ALIGNMENT
		| a_register
		| RANDOM_TYPE
		  {
			$$ = - MAX_REGISTERS - 1;
		  }
		;

alignment_prfx	: ALIGNMENT
		| a_register
		| A_REGISTER ':' RANDOM_TYPE
		  {
			$$ = - MAX_REGISTERS - 1;
		  }
		;

altar_type	: ALTAR_TYPE
		| RANDOM_TYPE
		;

a_register	: A_REGISTER '[' INTEGER ']'
		  {
			if ( $3 >= 3 )
				lc_error("Register Index overflow!");
			else
				$$ = - $3 - 1;
		  }
		;

string_or_var	: STRING
		  {
		      add_opvars(splev, "s", VA_PASS1($1));
		      Free($1);
		  }
		| VARSTRING_STRING
		  {
		      check_vardef_type(vardefs, $1, SPOVAR_STRING);
		      vardef_used(vardefs, $1);
		      add_opvars(splev, "v", VA_PASS1($1));
		      Free($1);
		  }
		| VARSTRING_STRING_ARRAY '[' math_expr_var ']'
		  {
		      check_vardef_type(vardefs, $1,
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, $1);
		      add_opvars(splev, "v", VA_PASS1($1));
		      Free($1);
		  }
		;


integer_or_var	: math_expr_var
		  {
		      /* nothing */
		  }
		;

coord_or_var	: encodecoord
		  {
		      add_opvars(splev, "c", VA_PASS1($1));
		  }
		| rndcoord_ID '(' ter_selection ')'
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  }
		| VARSTRING_COORD
		  {
		      check_vardef_type(vardefs, $1, SPOVAR_COORD);
		      vardef_used(vardefs, $1);
		      add_opvars(splev, "v", VA_PASS1($1));
		      Free($1);
		  }
		| VARSTRING_COORD_ARRAY '[' math_expr_var ']'
		  {
		      check_vardef_type(vardefs, $1,
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, $1);
		      add_opvars(splev, "v", VA_PASS1($1));
		      Free($1);
		  }
		;

encodecoord	: '(' INTEGER ',' INTEGER ')'
		  {
		      if ($2 < 0 || $4 < 0 || $2 >= COLNO || $4 >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   $2, $4);
		      $$ = SP_COORD_PACK($2, $4);
		  }
		| RANDOM_TYPE
		  {
		      $$ = SP_COORD_PACK_RANDOM(0);
		  }
		| RANDOM_TYPE_BRACKET humidity_flags ']'
		  {
		      $$ = SP_COORD_PACK_RANDOM($2);
		  }
		;

humidity_flags	: HUMIDITY_TYPE
		  {
		      $$ = $1;
		  }
		| HUMIDITY_TYPE ',' humidity_flags
		  {
		      if (($1 & $3))
			  lc_warning("Humidity flag used twice.");
		      $$ = ($1 | $3);
		  }
		;

region_or_var	: encoderegion
		  {
		      /* nothing */
		  }
		| VARSTRING_REGION
		  {
		      check_vardef_type(vardefs, $1, SPOVAR_REGION);
		      vardef_used(vardefs, $1);
		      add_opvars(splev, "v", VA_PASS1($1));
		      Free($1);
		  }
		| VARSTRING_REGION_ARRAY '[' math_expr_var ']'
		  {
		      check_vardef_type(vardefs, $1,
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, $1);
		      add_opvars(splev, "v", VA_PASS1($1));
		      Free($1);
		  }
		;

encoderegion	: '(' INTEGER ',' INTEGER ',' INTEGER ',' INTEGER ')'
		  {
		      int64_t r = SP_REGION_PACK($2, $4, $6, $8);

		      if ($2 > $6 || $4 > $8)
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   $2, $4, $6, $8);

		      add_opvars(splev, "r", VA_PASS1(r));
		      $$ = r;
		  }
		;

mapchar_or_var	: mapchar
		  {
		      add_opvars(splev, "m", VA_PASS1($1));
		  }
		| VARSTRING_MAPCHAR
		  {
		      check_vardef_type(vardefs, $1, SPOVAR_MAPCHAR);
		      vardef_used(vardefs, $1);
		      add_opvars(splev, "v", VA_PASS1($1));
		      Free($1);
		  }
		| VARSTRING_MAPCHAR_ARRAY '[' math_expr_var ']'
		  {
		      check_vardef_type(vardefs, $1,
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, $1);
		      add_opvars(splev, "v", VA_PASS1($1));
		      Free($1);
		  }
		;

mapchar		: CHAR
		  {
		      if (what_map_char((char) $1) != INVALID_LEVTYPE)
			  $$ = SP_MAPCHAR_PACK(what_map_char((char) $1), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", $1);
			  $$ = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  }
		| '(' CHAR ',' light_state ')'
		  {
		      if (what_map_char((char) $2) != INVALID_LEVTYPE)
			  $$ = SP_MAPCHAR_PACK(what_map_char((char) $2), $4);
		      else {
			  lc_error("Unknown map char type '%c'!", $2);
			  $$ = SP_MAPCHAR_PACK(STONE, $4);
		      }
		  }
		;

monster_or_var	: encodemonster
		  {
		      add_opvars(splev, "M", VA_PASS1($1));
		  }
		| VARSTRING_MONST
		  {
		      check_vardef_type(vardefs, $1, SPOVAR_MONST);
		      vardef_used(vardefs, $1);
		      add_opvars(splev, "v", VA_PASS1($1));
		      Free($1);
		  }
		| VARSTRING_MONST_ARRAY '[' math_expr_var ']'
		  {
		      check_vardef_type(vardefs, $1,
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, $1);
		      add_opvars(splev, "v", VA_PASS1($1));
		      Free($1);
		  }
		;

encodemonster	: STRING
                  {
                      int64_t m = get_monster_id($1, (char)0);
                      if (m == ERR) {
                          lc_error("Unknown monster \"%s\"!", $1);
                          $$ = -1;
                      } else
                          $$ = SP_MONST_PACK(m,
                                         def_monsyms[(int) mons[m].mlet].sym);
                      Free($1);
                  }
                | CHAR
                  {
                        if (check_monster_char((char) $1))
                            $$ = SP_MONST_PACK(-1, $1);
                        else {
                            lc_error("Unknown monster class '%c'!", $1);
                            $$ = -1;
                        }
                  }
                | '(' CHAR ',' STRING ')'
                  {
                      int64_t m = get_monster_id($4, (char) $2);
                      if (m == ERR) {
                          lc_error("Unknown monster ('%c', \"%s\")!", $2, $4);
                          $$ = -1;
                      } else
                          $$ = SP_MONST_PACK(m, $2);
                      Free($4);
                  }
                | RANDOM_TYPE
                  {
                      $$ = -1;
                  }
                ;

object_or_var	: encodeobj
		  {
		      add_opvars(splev, "O", VA_PASS1($1));
		  }
		| VARSTRING_OBJ
		  {
		      check_vardef_type(vardefs, $1, SPOVAR_OBJ);
		      vardef_used(vardefs, $1);
		      add_opvars(splev, "v", VA_PASS1($1));
		      Free($1);
		  }
		| VARSTRING_OBJ_ARRAY '[' math_expr_var ']'
		  {
		      check_vardef_type(vardefs, $1,
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, $1);
		      add_opvars(splev, "v", VA_PASS1($1));
		      Free($1);
		  }
		;

encodeobj	: STRING
		  {
		      int64_t m = get_object_id($1, (char)0);
		      if (m == ERR) {
			  lc_error("Unknown object \"%s\"!", $1);
			  $$ = -1;
		      } else
                          /* obj class != 0 to force generation of a specific item */
                          $$ = SP_OBJ_PACK(m, 1);
                      Free($1);
		  }
		| CHAR
		  {
			if (check_object_char((char) $1))
			    $$ = SP_OBJ_PACK(-1, $1);
			else {
			    lc_error("Unknown object class '%c'!", $1);
			    $$ = -1;
			}
		  }
		| '(' CHAR ',' STRING ')'
		  {
		      int64_t m = get_object_id($4, (char) $2);
		      if (m == ERR) {
			  lc_error("Unknown object ('%c', \"%s\")!", $2, $4);
			  $$ = -1;
		      } else
			  $$ = SP_OBJ_PACK(m, $2);
                      Free($4);
		  }
		| RANDOM_TYPE
		  {
		      $$ = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  }
		| SPECIAL_OBJECT_TYPE
		  {
  			  $$ = $1;
		  }
		;


string_expr	: string_or_var                 { }
		| string_expr '.' string_or_var
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  }
		;

math_expr_var	: INTEGER
		  {
		      add_opvars(splev, "i", VA_PASS1($1));
		  }
		| dice
		  {
		      is_inconstant_number = 1;
		  }
		| '(' MINUS_INTEGER ')'
		  {
		      add_opvars(splev, "i", VA_PASS1($2));
		  }
		| VARSTRING_INT
		  {
		      check_vardef_type(vardefs, $1, SPOVAR_INT);
		      vardef_used(vardefs, $1);
		      add_opvars(splev, "v", VA_PASS1($1));
		      Free($1);
		      is_inconstant_number = 1;
		  }
		| VARSTRING_INT_ARRAY '[' math_expr_var ']'
		  {
		      check_vardef_type(vardefs, $1,
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, $1);
		      add_opvars(splev, "v", VA_PASS1($1));
		      Free($1);
		      is_inconstant_number = 1;
		  }
		| math_expr_var '+' math_expr_var
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  }
		| math_expr_var '-' math_expr_var
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  }
		| math_expr_var '*' math_expr_var
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  }
		| math_expr_var '/' math_expr_var
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  }
		| math_expr_var '%' math_expr_var
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  }
		| '(' math_expr_var ')'             { }
		;

func_param_type	: CFUNC_INT
		  {
		      if (!strcmp("int", $1) || !strcmp("integer", $1)) {
			  $$ = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", $1);
		  }
		| CFUNC_STR
		  {
		      if (!strcmp("str", $1) || !strcmp("string", $1)) {
			  $$ = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", $1);
		  }
		;

func_param_part	: any_var_or_arr ':' func_param_type
		  {
		      struct lc_funcdefs_parm *tmp = New(struct lc_funcdefs_parm);

		      if (!curr_function) {
			  lc_error("Function parameters outside function definition.");
		      } else if (!tmp) {
			  lc_error("Could not alloc function params.");
		      } else {
			  int64_t vt = SPOVAR_NULL;

			  tmp->name = strdup($1);
			  tmp->parmtype = (char) $3;
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
			  vardefs = add_vardef_type( vardefs, $1, vt);
		      }
		      Free($1);
		  }
		;

func_param_list		: func_param_part
			| func_param_list ',' func_param_part
			;

func_params_list	: /* nothing */
			| func_param_list
			;

func_call_param_part	: math_expr_var
			  {
			      $$ = (int)'i';
			  }
			| string_expr
			  {
			      $$ = (int)'s';
			  }
			;


func_call_param_list   	: func_call_param_part
			  {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) $1;
			      tmpbuf[1] = '\0';
			      $$ = strdup(tmpbuf);
			  }
			| func_call_param_list ',' func_call_param_part
			  {
			      size_t len = strlen( $1 );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) $3, $1 );
			      Free( $1 );
			      $$ = tmp;
			  }
			;

func_call_params_list	: /* nothing */
			  {
			      $$ = strdup("");
			  }
			| func_call_param_list
			  {
			      char *tmp = strdup( $1 );
			      Free( $1 );
			      $$ = tmp;
			  }
			;

ter_selection_x	: coord_or_var
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  }
		| rect_ID region_or_var
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  }
		| fillrect_ID region_or_var
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  }
		| line_ID coord_or_var ',' coord_or_var
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  }
		| randline_ID coord_or_var ',' coord_or_var ',' math_expr_var
		  {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  }
		| grow_ID '(' ter_selection ')'
		  {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  }
		| grow_ID '(' dir_list ',' ter_selection ')'
		  {
		      add_opvars(splev, "io", VA_PASS2($3, SPO_SEL_GROW));
		  }
		| filter_ID '(' SPERCENT ',' ter_selection ')'
		  {
		      add_opvars(splev, "iio",
			     VA_PASS3($3, SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  }
		| filter_ID '(' ter_selection ',' ter_selection ')'
		  {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  }
		| filter_ID '(' mapchar_or_var ',' ter_selection ')'
		  {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  }
		| flood_ID coord_or_var
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  }
		| circle_ID '(' coord_or_var ',' math_expr_var ')'
		  {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  }
		| circle_ID '(' coord_or_var ',' math_expr_var ',' FILLING ')'
		  {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, $7, SPO_SEL_ELLIPSE));
		  }
		| ellipse_ID '(' coord_or_var ',' math_expr_var ',' math_expr_var ')'
		  {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  }
		| ellipse_ID '(' coord_or_var ',' math_expr_var ',' math_expr_var ',' FILLING ')'
		  {
		      add_opvars(splev, "io", VA_PASS2($9, SPO_SEL_ELLIPSE));
		  }
		| gradient_ID '(' GRADIENT_TYPE ',' '(' math_expr_var '-' math_expr_var opt_limited ')' ',' coord_or_var opt_coord_or_var ')'
		  {
		      add_opvars(splev, "iio",
				 VA_PASS3($9, $3, SPO_SEL_GRADIENT));
		  }
		| complement_ID ter_selection_x
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  }
		| VARSTRING_SEL
		  {
		      check_vardef_type(vardefs, $1, SPOVAR_SEL);
		      vardef_used(vardefs, $1);
		      add_opvars(splev, "v", VA_PASS1($1));
		      Free($1);
		  }
		| '(' ter_selection ')'
		  {
		      /* nothing */
		  }
		;

ter_selection	: ter_selection_x
		  {
		      /* nothing */
		  }
		| ter_selection_x '&' ter_selection
		  {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  }
		;

dice		: DICE
		  {
		      add_opvars(splev, "iio",
				 VA_PASS3($1.num, $1.die, SPO_DICE));
		  }
		;

tileset_number : CMAP_TYPE
		| INTEGER
		;

all_integers	: MINUS_INTEGER
		| PLUS_INTEGER
		| INTEGER
		;

all_ints_push	: MINUS_INTEGER
		  {
		      add_opvars(splev, "i", VA_PASS1($1));
		  }
		| PLUS_INTEGER
		  {
		      add_opvars(splev, "i", VA_PASS1($1));
		  }
		| INTEGER
		  {
		      add_opvars(splev, "i", VA_PASS1($1));
		  }
		| dice
		  {
		      /* nothing */
		  }
		;

objectid	: object_ID
		| OBJECT_ID
		;

monsterid	: monster_ID
		| MONSTER_ID
		;

terrainid	: terrain_ID
		| TERRAIN_ID
		;

engraving_type	: ENGRAVING_TYPE
		| RANDOM_TYPE
		;

lev_region	: region
		  {
			$$ = $1;
		  }
		| LEV '(' INTEGER ',' INTEGER ',' INTEGER ',' INTEGER ')'
		  {
			if ($3 <= 0 || $3 >= COLNO)
			    lc_error(
                          "Region (%ld,%ld,%ld,%ld) out of level range (x1)!",
                                     $3, $5, $7, $9);
			else if ($5 < 0 || $5 >= ROWNO)
			    lc_error(
                          "Region (%ld,%ld,%ld,%ld) out of level range (y1)!",
                                     $3, $5, $7, $9);
			else if ($7 <= 0 || $7 >= COLNO)
			    lc_error(
                          "Region (%ld,%ld,%ld,%ld) out of level range (x2)!",
                                     $3, $5, $7, $9);
			else if ($9 < 0 || $9 >= ROWNO)
			    lc_error(
                          "Region (%ld,%ld,%ld,%ld) out of level range (y2)!",
                                     $3, $5, $7, $9);
			$$.x1 = $3;
			$$.y1 = $5;
			$$.x2 = $7;
			$$.y2 = $9;
			$$.area = 1;
		  }
		;

region		: '(' INTEGER ',' INTEGER ',' INTEGER ',' INTEGER ')'
		  {
/* This series of if statements is a hack for MSC 5.1.  It seems that its
   tiny little brain cannot compile if these are all one big if statement. */
			if ($2 < 0 || $2 > (int) max_x_map)
			    lc_error(
                            "Region (%ld,%ld,%ld,%ld) out of map range (x1)!",
                                     $2, $4, $6, $8);
			else if ($4 < 0 || $4 > (int) max_y_map)
			    lc_error(
                            "Region (%ld,%ld,%ld,%ld) out of map range (y1)!",
                                     $2, $4, $6, $8);
			else if ($6 < 0 || $6 > (int) max_x_map)
			    lc_error(
                            "Region (%ld,%ld,%ld,%ld) out of map range (x2)!",
                                     $2, $4, $6, $8);
			else if ($8 < 0 || $8 > (int) max_y_map)
			    lc_error(
                            "Region (%ld,%ld,%ld,%ld) out of map range (y2)!",
                                     $2, $4, $6, $8);
			$$.area = 0;
			$$.x1 = $2;
			$$.y1 = $4;
			$$.x2 = $6;
			$$.y2 = $8;
		  }
		;


%%

/*lev_comp.y*/
