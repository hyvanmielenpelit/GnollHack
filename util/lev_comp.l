%{
/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-05 */

/* GnollHack 4.0  lev_comp.l	$NHDT-Date: 1543371690 2018/11/28 02:21:30 $  $NHDT-Branch: NetHack-3.6.2-beta01 $:$NHDT-Revision: 1.25 $ */
/*	Copyright (c) 1989 by Jean-Christophe Collet */
/* GnollHack may be freely redistributed.  See license for details. */

#define LEV_LEX_C

#include "hack.h"
#include "lev_comp.h"
#include "sp_lev.h"

/* Most of these don't exist in flex, yywrap is macro and
 * yyunput is properly declared in flex.skel.
 */
#if !defined(FLEX_SCANNER) && !defined(FLEXHACK_SCANNER)
int FDECL(yyback, (int *,int));
int NDECL(yylook);
int NDECL(yyinput);
int NDECL(yywrap);
int NDECL(yylex);
	/* Traditional lexes let yyunput() and yyoutput() default to int;
	 * newer ones may declare them as void since they don't return
	 * values.  For even more fun, the lex supplied as part of the
	 * newer unbundled compiler for SunOS 4.x adds the void declarations
	 * (under __STDC__ or _cplusplus ifdefs -- otherwise they remain
	 * int) while the bundled lex and the one with the older unbundled
	 * compiler do not.  To detect this, we need help from outside --
	 * sys/unix/Makefile.utl.
	 *
	 * Digital UNIX is difficult and still has int in spite of all
	 * other signs.
	 */
# if defined(NeXT) || defined(SVR4) || defined(_AIX32)
#  define VOIDYYPUT
# endif
# if !defined(VOIDYYPUT) && defined(POSIX_TYPES)
#  if !defined(BOS) && !defined(HISX) && !defined(_M_UNIX) && !defined(VMS)
#   define VOIDYYPUT
#  endif
# endif
# if !defined(VOIDYYPUT) && defined(WEIRD_LEX)
#  if defined(SUNOS4) && defined(__STDC__) && (WEIRD_LEX > 1)
#   define VOIDYYPUT
#  endif
# endif
# if defined(VOIDYYPUT) && defined(__osf__)
#  undef VOIDYYPUT
# endif
# ifdef VOIDYYPUT
void FDECL(yyunput, (int));
void FDECL(yyoutput, (int));
# else
int FDECL(yyunput, (int));
int FDECL(yyoutput, (int));
# endif

#else	/* !FLEX_SCANNER && !FLEXHACK_SCANNER */
/* most recent flex allows suppressing yyunput() altogether when not needed */
#define YY_NO_UNPUT
#define YY_NO_INPUT
#endif

#if defined(FLEX_SCANNER) || defined(FLEXHACK_SCANNER)
/* older flex wants this */
#ifdef _CRTDBG_MAP_ALLOC
#define YY_MALLOC_DECL  genericptr_t FDECL(_malloc_dbg, (size_t, int, const char*, int)); \
                        genericptr_t FDECL(_realloc_dbg, (genericptr_t, size_t, int, const char*, int));
#else
#define YY_MALLOC_DECL  genericptr_t FDECL(malloc, (size_t)); \
                        genericptr_t FDECL(realloc, (genericptr_t, size_t));
/* newer flex assumes <stdlib.h> so needs this in case it's been suppressed */
#endif
YY_MALLOC_DECL
#endif

void FDECL(init_yyin, (FILE *));
void FDECL(init_yyout, (FILE *));

long NDECL(handle_varstring_check);
long FDECL(corefunc_str_check, (char *, long));

extern void VDECL(lc_error, (const char *, ...));
extern struct lc_vardefs *FDECL(vardef_defined,(struct lc_vardefs *,char *, int));

extern struct lc_vardefs *vardefs;

extern long FDECL(method_defined, (char *, long, long *));

void FDECL(savetoken, (char *));
void NDECL(newline);
void FDECL(advancepos, (char *));

/*
 * This doesn't always get put in lev_comp.h
 * (esp. when using older versions of bison).
 */
extern YYSTYPE yylval;

int nh_line_number = 1;
int token_start_pos = 0;
char curr_token[512];
static char map[4096];
static int map_cnt = 0;

FILE *orig_yyin = NULL;

#define ST_RET(x) do { savetoken(yytext); return x; } while (0);
#define ST_RETF(y, x) do { savetoken(yytext); y; return x; } while (0);

%}
%e 2500
%p 10000
%n 1500
%s MAPC
%%
<MAPC>ENDMAP	{
		  savetoken(yytext);
		  BEGIN(INITIAL);
		  yylval.map = (char *) alloc(map_cnt + 1);
		  (void) strncpy(yylval.map, map, map_cnt);
		  yylval.map[map_cnt] = 0;
		  map_cnt = 0;
		  return MAP_ID;
		}
<MAPC>[-|}{+xABCISHKPLWTF\\#.,: 0123456789]*\r?\n	{
		  int len = yyleng;
		  savetoken(yytext);
		  /* convert \r\n to \n */
		  if (len >= 2 && yytext[len - 2] == '\r')
		      len -= 1;
		  (void) strncpy(map + map_cnt, yytext, len);
		  map_cnt += len;
		  map[map_cnt - 1] = '\n';
		  map[map_cnt] = '\0';
		  newline();
		}
^[ \t]*#.*\n	{ savetoken(yytext); newline(); }
MESSAGE		ST_RET(MESSAGE_ID);
NOMAP		ST_RET(NOMAP_ID);
MAZE		ST_RET(MAZE_ID);
LEVEL		ST_RET(LEVEL_ID);
INIT_MAP	ST_RET(LEV_INIT_ID);
TILESET	    ST_RET(TILESET_ID);
SPECIAL_TILESET	    ST_RET(SPECIAL_TILESET_ID);
BOUNDARY_TYPE	ST_RET(BOUNDARY_TYPE_ID);
FOREST	    ST_RET(FOREST_ID);
floorsubtype	ST_RET(FLOOR_SUBTYPE_ID);
floortype	ST_RET(FLOOR_TYPE_ID);
mazegrid	ST_RET(MAZE_GRID_ID);
solidfill	ST_RET(SOLID_FILL_ID);
mines		ST_RET(MINES_ID);
rogue		ST_RET(ROGUELEV_ID);
FLAGS		ST_RET(FLAGS_ID);
GEOMETRY	ST_RET(GEOMETRY_ID);
^MAP\r?\n	{ savetoken(yytext); BEGIN(MAPC); newline(); }
obj(ect)?	ST_RET(object_ID);
OBJECT		ST_RET(OBJECT_ID);
CONTAINER	ST_RET(COBJECT_ID);
MONSTER		ST_RET(MONSTER_ID);
monster		ST_RET(monster_ID);
MONSTER_GENERATION		ST_RET(MONSTER_GENERATION_ID);
keytype		ST_RET(KEYTYPE_ID);
TRAP		ST_RET(TRAP_ID);
DOOR		ST_RET(DOOR_ID);
ROOMDOOR	ST_RET(ROOMDOOR_ID);
DRAWBRIDGE	ST_RET(DRAWBRIDGE_ID);
MAZEWALK	ST_RET(MAZEWALK_ID);
WALLIFY		ST_RET(WALLIFY_ID);
REGION		ST_RET(REGION_ID);
SPECIAL_REGION		ST_RET(SPECIAL_REGION_ID);
SPECIAL_LEVREGION	ST_RET(SPECIAL_LEVREGION_ID);
NAMING		ST_RET(NAMING_ID);
ALTAR		ST_RET(ALTAR_ID);
ANVIL		ST_RET(ANVIL_ID);
NPC		    ST_RET(NPC_ID);
LADDER		ST_RET(LADDER_ID);
STAIR		ST_RET(STAIR_ID);
PORTAL		ST_RET(PORTAL_ID);
LEVER		ST_RET(LEVER_ID);
TELEPORT_REGION	ST_RET(TELEPRT_ID);
BRANCH		ST_RET(BRANCH_ID);
FOUNTAIN	ST_RET(FOUNTAIN_ID);
FLOOR	    ST_RET(FLOOR_ID);
THRONE	    ST_RET(THRONE_ID);
MODRON_PORTAL	ST_RET(MODRON_PORTAL_ID);
SINK		ST_RET(SINK_ID);
POOL		ST_RET(POOL_ID);
NON_DIGGABLE	ST_RET(NON_DIGGABLE_ID);
NON_PASSWALL	ST_RET(NON_PASSWALL_ID);
IF		ST_RET(IF_ID);
ELSE		ST_RET(ELSE_ID);
EXIT		ST_RET(EXIT_ID);
ROOM		ST_RET(ROOM_ID);
SUBROOM		ST_RET(SUBROOM_ID);
SUBTYPE		ST_RET(SUBTYPE_ID);
RANDOM_CORRIDORS	ST_RET(RAND_CORRIDOR_ID);
CORRIDOR	ST_RET(CORRIDOR_ID);
TERRAIN		ST_RET(TERRAIN_ID);
terrain		ST_RET(terrain_ID);
REPLACE_TERRAIN	ST_RET(REPLACE_TERRAIN_ID);
GOLD		ST_RET(GOLD_ID);
GRAVE		ST_RET(GRAVE_ID);
BRAZIER		ST_RET(BRAZIER_ID);
SIGNPOST	ST_RET(SIGNPOST_ID);
TREE		ST_RET(TREE_ID);
ENGRAVING	ST_RET(ENGRAVING_ID);
MINERALIZE	ST_RET(MINERALIZE_ID);
(NAME|name)	ST_RET(NAME_ID);
FOR		ST_RET(FOR_ID);
TO		ST_RET(TO_ID);
LOOP		ST_RET(LOOP_ID);
SWITCH		ST_RET(SWITCH_ID);
CASE		ST_RET(CASE_ID);
BREAK		ST_RET(BREAK_ID);
DEFAULT		ST_RET(DEFAULT_ID);
FUNCTION	ST_RET(FUNCTION_ID);
SHUFFLE		ST_RET(SHUFFLE_ID);
montype		ST_RET(MONTYPE_ID);
objtype		ST_RET(OBJTYPE_ID);
traptype	ST_RET(TRAPTYPE_ID);
target		ST_RET(TARGET_ID);
tertype		ST_RET(TERTYPE_ID);
tertype2	ST_RET(TERTYPE2_ID);
effect_flags	ST_RET(EFFECT_FLAG_ID);
artifact_name   ST_RET(ARTIFACT_NAME_ID)
selection	ST_RET(selection_ID);
rect		ST_RET(rect_ID);
fillrect	ST_RET(fillrect_ID);
line		ST_RET(line_ID);
randline	ST_RET(randline_ID);
grow		ST_RET(grow_ID);
floodfill	ST_RET(flood_ID);
rndcoord	ST_RET(rndcoord_ID);
circle		ST_RET(circle_ID);
ellipse		ST_RET(ellipse_ID);
filter		ST_RET(filter_ID);
gradient	ST_RET(gradient_ID);
complement	ST_RET(complement_ID);
radial		{ savetoken(yytext); yylval.i=SEL_GRADIENT_RADIAL;  return GRADIENT_TYPE; }
square		{ savetoken(yytext); yylval.i=SEL_GRADIENT_SQUARE;  return GRADIENT_TYPE; }
dry		{ savetoken(yytext); yylval.i=DRY;  return HUMIDITY_TYPE; }
wet		{ savetoken(yytext); yylval.i=WET;  return HUMIDITY_TYPE; }
hot		{ savetoken(yytext); yylval.i=HOT;  return HUMIDITY_TYPE; }
solid		{ savetoken(yytext); yylval.i=SOLID;  return HUMIDITY_TYPE; }
any		{ savetoken(yytext); yylval.i=ANY_LOC;  return HUMIDITY_TYPE; }
ice		{ savetoken(yytext); yylval.i=ICELOC;  return HUMIDITY_TYPE; }
air		{ savetoken(yytext); yylval.i=AIRLOC;  return HUMIDITY_TYPE; }
floor   { savetoken(yytext); yylval.i=FLOORLOC;  return HUMIDITY_TYPE; }
levregion	ST_RET(LEV);
quantity	ST_RET(QUANTITY_ID);
elemental_enchantment	ST_RET(ELEMENTAL_ENCHANTMENT_ID);
exceptionality	ST_RET(EXCEPTIONALITY_ID);
mythic_prefix	ST_RET(MYTHIC_PREFIX_ID);
mythic_suffix	ST_RET(MYTHIC_SUFFIX_ID);
enchantment	ST_RET(ENCHANTMENT_ID);
charges	    ST_RET(CHARGES_ID);
special_quality	ST_RET(SPECIAL_QUALITY_ID);
manual_type	ST_RET(MANUAL_TYPE_ID);
speflags	ST_RET(SPEFLAGS_ID);
age	        ST_RET(AGE_ID);
buried		ST_RET(BURIED_ID);
eroded		ST_RET(ERODED_ID);
erodeproof	ST_RET(ERODEPROOF_ID);
trapped     { savetoken(yytext); yylval.i=1; return TRAPPED_STATE; }
not_trapped { savetoken(yytext); yylval.i=0; return TRAPPED_STATE; }
recharged	ST_RET(RECHARGED_ID);
invisible	ST_RET(INVIS_ID);
greased		ST_RET(GREASED_ID);
indestructible    ST_RET(INDESTRUCTIBLE_ID);
level_teleporter  ST_RET(LEVEL_TELEPORTER_ID);
no_pickup	ST_RET(NO_PICKUP_ID);
female		ST_RET(FEMALE_ID);
male		ST_RET(MALE_ID);
cancelled	ST_RET(CANCELLED_ID);
revived		ST_RET(REVIVED_ID);
avenge		ST_RET(AVENGE_ID);
fleeing		ST_RET(FLEEING_ID);
blinded		ST_RET(BLINDED_ID);
paralyzed	ST_RET(PARALYZED_ID);
stunned		ST_RET(STUNNED_ID);
confused	ST_RET(CONFUSED_ID);
seen_traps	ST_RET(SEENTRAPS_ID);
waitforu	ST_RET(WAITFORU_ID);
protector	ST_RET(PROTECTOR_ID);
maxhp		ST_RET(MAXHP_ID);
level_adjustment  ST_RET(LEVEL_ADJUSTMENT_ID);
switchable	ST_RET(SWITCHABLE_ID);
secret_door	ST_RET(SECRET_DOOR_ID);
uses_up_key	ST_RET(USES_UP_KEY_ID);
non_passdoor ST_RET(NON_PASSDOOR_ID);
continuously_usable	ST_RET(CONTINUOUSLY_USABLE_ID);
subtype		ST_RET(LOCATION_SUBTYPE_ID);
all		ST_RET(ALL_ID);
horizontal	ST_RETF((yylval.i=1), HORIZ_OR_VERT);
vertical	{ savetoken(yytext); yylval.i=2; return HORIZ_OR_VERT; }
open		{ savetoken(yytext); yylval.i=D_ISOPEN; return DOOR_STATE; }
closed		{ savetoken(yytext); yylval.i=D_CLOSED; return DOOR_STATE; }
locked		{ savetoken(yytext); yylval.i=D_LOCKED; return DOOR_STATE; }
nodoor		{ savetoken(yytext); yylval.i=D_NODOOR; return DOOR_STATE; }
broken		{ savetoken(yytext); yylval.i=D_BROKEN; return DOOR_STATE; }
secret		{ savetoken(yytext); yylval.i=D_SECRET; return DOOR_STATE; }
north		{ savetoken(yytext); yylval.i=W_NORTH; return DIRECTION; }
east		{ savetoken(yytext); yylval.i=W_EAST; return DIRECTION; }
south		{ savetoken(yytext); yylval.i=W_SOUTH; return DIRECTION; }
west		{ savetoken(yytext); yylval.i=W_WEST; return DIRECTION; }
random		{ savetoken(yytext); yylval.i = -1; return RANDOM_TYPE; }
random\[	{ savetoken(yytext); yylval.i = -1; return RANDOM_TYPE_BRACKET; }
none		{ savetoken(yytext); yylval.i = -2; return NONE; }
align		ST_RET(A_REGISTER);
left		{ savetoken(yytext); yylval.i=1; return LEFT_OR_RIGHT; }
half-left	{ savetoken(yytext); yylval.i=2; return LEFT_OR_RIGHT; }
center		{ savetoken(yytext); yylval.i=3; return CENTER; }
half-right	{ savetoken(yytext); yylval.i=4; return LEFT_OR_RIGHT; }
right		{ savetoken(yytext); yylval.i=5; return LEFT_OR_RIGHT; }
top		{ savetoken(yytext); yylval.i=1; return TOP_OR_BOT; }
bottom		{ savetoken(yytext); yylval.i=5; return TOP_OR_BOT; }
lit		{ savetoken(yytext); yylval.i=1; return LIGHT_STATE; }
unlit		{ savetoken(yytext); yylval.i=0; return LIGHT_STATE; }
filled		{ savetoken(yytext); yylval.i=1; return FILLING; }
unfilled	{ savetoken(yytext); yylval.i=0; return FILLING; }
regular		{ savetoken(yytext); yylval.i=0; return IRREGULAR; }
irregular	{ savetoken(yytext); yylval.i=1; return IRREGULAR; }
unjoined	{ savetoken(yytext); yylval.i=1; return JOINED; }
joined		{ savetoken(yytext); yylval.i=0; return JOINED; }
limited		{ savetoken(yytext); yylval.i=1; return LIMITED; }
unlimited	{ savetoken(yytext); yylval.i=0; return LIMITED; }
noalign		{ savetoken(yytext); yylval.i= AM_NONE; return ALIGNMENT; }
law		{ savetoken(yytext); yylval.i= AM_LAWFUL; return ALIGNMENT; }
neutral		{ savetoken(yytext); yylval.i= AM_NEUTRAL; return ALIGNMENT; }
chaos		{ savetoken(yytext); yylval.i= AM_CHAOTIC; return ALIGNMENT; }
coaligned	{ savetoken(yytext); yylval.i= AM_SPLEV_CO; return ALIGNMENT; }
noncoaligned	{ savetoken(yytext); yylval.i= AM_SPLEV_NONCO; return ALIGNMENT; }
altar_normal	{ savetoken(yytext); yylval.i= ALTAR_SUBTYPE_NORMAL; return ALTAR_SUBTYPE; }
altar_high	{ savetoken(yytext); yylval.i= ALTAR_SUBTYPE_HIGH; return ALTAR_SUBTYPE; }
altar_moloch	{ savetoken(yytext); yylval.i= ALTAR_SUBTYPE_MOLOCH; return ALTAR_SUBTYPE; }
peaceful	{ savetoken(yytext); yylval.i=1; return MON_ATTITUDE; }
hostile		{ savetoken(yytext); yylval.i=0; return MON_ATTITUDE; }
asleep		{ savetoken(yytext); yylval.i=1; return MON_ALERTNESS; }
awake		{ savetoken(yytext); yylval.i=0; return MON_ALERTNESS; }
m_feature	{ savetoken(yytext); yylval.i= M_AP_FURNITURE; return MON_APPEARANCE; }
m_monster	{ savetoken(yytext); yylval.i= M_AP_MONSTER;   return MON_APPEARANCE; }
m_object	{ savetoken(yytext); yylval.i= M_AP_OBJECT;    return MON_APPEARANCE; }
sanctum		{ savetoken(yytext); yylval.i=2; return ALTAR_TYPE; }
shrine		{ savetoken(yytext); yylval.i=1; return ALTAR_TYPE; }
altar		{ savetoken(yytext); yylval.i=0; return ALTAR_TYPE; }
notele		{ savetoken(yytext); yylval.i=2; return UP_OR_DOWN; }
up		    { savetoken(yytext); yylval.i=1; return UP_OR_DOWN; }
down		{ savetoken(yytext); yylval.i=0; return UP_OR_DOWN; }
active		{ savetoken(yytext); yylval.i=1; return ACTIVE_OR_INACTIVE; }
inactive	{ savetoken(yytext); yylval.i=0; return ACTIVE_OR_INACTIVE; }
baseportal	{ savetoken(yytext); yylval.i=MODRON_PORTAL_SUBTYPE_BASE; return MODRON_PORTAL_TYPE; }
spherical	{ savetoken(yytext); yylval.i=MODRON_PORTAL_SUBTYPE_SPHERICAL; return MODRON_PORTAL_TYPE; }
cylindrical	{ savetoken(yytext); yylval.i=MODRON_PORTAL_SUBTYPE_CYLINDRICAL; return MODRON_PORTAL_TYPE; }
tetrahedral	{ savetoken(yytext); yylval.i=MODRON_PORTAL_SUBTYPE_TETRAHEDRAL; return MODRON_PORTAL_TYPE; }
cubical		{ savetoken(yytext); yylval.i=MODRON_PORTAL_SUBTYPE_CUBICAL; return MODRON_PORTAL_TYPE; }
octahedral	{ savetoken(yytext); yylval.i=MODRON_PORTAL_SUBTYPE_OCTAHEDRAL; return MODRON_PORTAL_TYPE; }
dodecahedral { savetoken(yytext); yylval.i=MODRON_PORTAL_SUBTYPE_DODECAHEDRAL; return MODRON_PORTAL_TYPE; }
icosahedral { savetoken(yytext); yylval.i=MODRON_PORTAL_SUBTYPE_ICOSAHEDRAL; return MODRON_PORTAL_TYPE; }
npc_random { savetoken(yytext); yylval.i=MAX_NPC_SUBTYPES; return NPC_TYPE; }
npc_artificer { savetoken(yytext); yylval.i=NPC_ARTIFICER; return NPC_TYPE; }
npc_geologist { savetoken(yytext); yylval.i=NPC_GEOLOGIST; return NPC_TYPE; }
npc_warp_engineer { savetoken(yytext); yylval.i=NPC_WARP_ENGINEER; return NPC_TYPE; }
npc_hermit { savetoken(yytext); yylval.i=NPC_HERMIT; return NPC_TYPE; }
npc_hermit2 { savetoken(yytext); yylval.i=NPC_HERMIT2; return NPC_TYPE; }
npc_hermit3 { savetoken(yytext); yylval.i=NPC_HERMIT3; return NPC_TYPE; }
npc_orc_hermit3 { savetoken(yytext); yylval.i=NPC_ORC_HERMIT3; return NPC_TYPE; }
npc_quantum_mechanic { savetoken(yytext); yylval.i=NPC_QUANTUM_MECHANIC; return NPC_TYPE; }
fountain_natural { savetoken(yytext); yylval.i=FOUNTAIN_NATURAL; return FOUNTAIN_TYPE; }
fountain_magic { savetoken(yytext); yylval.i=FOUNTAIN_MAGIC; return FOUNTAIN_TYPE; }
fountain_healing { savetoken(yytext); yylval.i=FOUNTAIN_HEALING; return FOUNTAIN_TYPE; }
fountain_mana { savetoken(yytext); yylval.i=FOUNTAIN_MANA; return FOUNTAIN_TYPE; }
fountain_power { savetoken(yytext); yylval.i=FOUNTAIN_POWER; return FOUNTAIN_TYPE; }
fountain_poison { savetoken(yytext); yylval.i=FOUNTAIN_POISON; return FOUNTAIN_TYPE; }
fountain_water { savetoken(yytext); yylval.i=FOUNTAIN_WATER; return FOUNTAIN_TYPE; }
fountain_random { savetoken(yytext); yylval.i=MAX_FOUNTAIN_SUBTYPES; return FOUNTAIN_TYPE; }
class_treasure_weapon { savetoken(yytext); yylval.i=OBJECT_SPECIAL_CREATE_TYPE_CLASS_TREASURE_WEAPON; return SPECIAL_OBJECT_TYPE; }
class_treasure_armor { savetoken(yytext); yylval.i=OBJECT_SPECIAL_CREATE_TYPE_CLASS_TREASURE_ARMOR; return SPECIAL_OBJECT_TYPE; }
artifact { savetoken(yytext); yylval.i=OBJECT_SPECIAL_CREATE_TYPE_ARTIFACT; return SPECIAL_OBJECT_TYPE; }
class_artifact { savetoken(yytext); yylval.i=OBJECT_SPECIAL_CREATE_TYPE_CLASS_ARTIFACT; return SPECIAL_OBJECT_TYPE; }
orc_artifact { savetoken(yytext); yylval.i=OBJECT_SPECIAL_CREATE_TYPE_ORC_ARTIFACT; return SPECIAL_OBJECT_TYPE; }
random_contents { savetoken(yytext); yylval.i=OBJECT_SPECIAL_CREATE_TYPE_RANDOM_CONTENTS; return SPECIAL_OBJECT_TYPE; }
dungeons_of_doom_tileset { savetoken(yytext); yylval.i=CMAP_NORMAL; return CMAP_TYPE; }
gnomish_mines_tileset { savetoken(yytext); yylval.i=CMAP_GNOMISH_MINES; return CMAP_TYPE; }
gehennom_tileset { savetoken(yytext); yylval.i=CMAP_GEHENNOM; return CMAP_TYPE; }
undead_tileset { savetoken(yytext); yylval.i=CMAP_UNDEAD_STYLE; return CMAP_TYPE; }
sokoban_tileset { savetoken(yytext); yylval.i=CMAP_SOKOBAN; return CMAP_TYPE; }
elemental_tileset { savetoken(yytext); yylval.i=CMAP_ELEMENTAL_PLANES; return CMAP_TYPE; }
astral_tileset { savetoken(yytext); yylval.i=CMAP_ASTRAL; return CMAP_TYPE; }
gehennom_cavernous_tileset { savetoken(yytext); yylval.i=CMAP_GEHENNOM_CAVERNOUS; return CMAP_TYPE; }
garden_tileset { savetoken(yytext); yylval.i=CMAP_GARDEN; return CMAP_TYPE; }
floor_normal { savetoken(yytext); yylval.i=FLOOR_SUBTYPE_NORMAL; return FLOOR_SUBTYPE; }
floor_marble { savetoken(yytext); yylval.i=FLOOR_SUBTYPE_MARBLE; return FLOOR_SUBTYPE; }
floor_parquet { savetoken(yytext); yylval.i=FLOOR_SUBTYPE_PARQUET; return FLOOR_SUBTYPE; }
floor_cobblestone { savetoken(yytext); yylval.i=FLOOR_SUBTYPE_COBBLESTONE; return FLOOR_SUBTYPE; }
grass_normal { savetoken(yytext); yylval.i=GRASS_SUBTYPE_NORMAL; return FLOOR_SUBTYPE; }
grass_swampy { savetoken(yytext); yylval.i=GRASS_SUBTYPE_SWAMPY; return FLOOR_SUBTYPE; }
ground_normal { savetoken(yytext); yylval.i=GROUND_SUBTYPE_NORMAL; return FLOOR_SUBTYPE; }
ground_swampy { savetoken(yytext); yylval.i=GROUND_SUBTYPE_SWAMPY; return FLOOR_SUBTYPE; }
ground_desert_sand { savetoken(yytext); yylval.i=GROUND_SUBTYPE_DESERT_SAND; return FLOOR_SUBTYPE; }
location_floor { savetoken(yytext); yylval.i=ROOM; return FLOOR_TYPE; }
location_corridor { savetoken(yytext); yylval.i=CORR; return FLOOR_TYPE; }
location_grass { savetoken(yytext); yylval.i=GRASS; return FLOOR_TYPE; }
location_ground { savetoken(yytext); yylval.i=GROUND; return FLOOR_TYPE; }
flaming { savetoken(yytext); yylval.i=FIRE_ENCHANTMENT; return ELEMENTAL_ENCHANTMENT_TYPE; }
freezing { savetoken(yytext); yylval.i=COLD_ENCHANTMENT; return ELEMENTAL_ENCHANTMENT_TYPE; }
electrified { savetoken(yytext); yylval.i=LIGHTNING_ENCHANTMENT; return ELEMENTAL_ENCHANTMENT_TYPE; }
death-magical { savetoken(yytext); yylval.i=DEATH_ENCHANTMENT; return ELEMENTAL_ENCHANTMENT_TYPE; }
normal_exceptionality { savetoken(yytext); yylval.i=EXCEPTIONALITY_NORMAL; return EXCEPTIONALITY_TYPE; }
exceptional { savetoken(yytext); yylval.i=EXCEPTIONALITY_EXCEPTIONAL; return EXCEPTIONALITY_TYPE; }
elite { savetoken(yytext); yylval.i=EXCEPTIONALITY_ELITE; return EXCEPTIONALITY_TYPE; }
celestial { savetoken(yytext); yylval.i=EXCEPTIONALITY_CELESTIAL; return EXCEPTIONALITY_TYPE; }
primordial { savetoken(yytext); yylval.i=EXCEPTIONALITY_PRIMORDIAL; return EXCEPTIONALITY_TYPE; }
infernal { savetoken(yytext); yylval.i=EXCEPTIONALITY_INFERNAL; return EXCEPTIONALITY_TYPE; }
no_mythic_prefix { savetoken(yytext); yylval.i=MYTHIC_PREFIX_NONE; return MYTHIC_PREFIX_TYPE; }
no_mythic_suffix { savetoken(yytext); yylval.i=MYTHIC_SUFFIX_NONE; return MYTHIC_SUFFIX_TYPE; }
region_general { savetoken(yytext); yylval.i=REGION_GENERAL; return SPECIAL_REGION_TYPE; }
region_special_level_seen { savetoken(yytext); yylval.i=REGION_SPECIAL_LEVEL_SEEN; return SPECIAL_REGION_TYPE; }
region_special_level_true_nature_revealed { savetoken(yytext); yylval.i=REGION_SPECIAL_LEVEL_TRUE_NATURE_REVEALED; return SPECIAL_REGION_TYPE; }
name_revealed_never { savetoken(yytext); yylval.i=SPECIAL_LEVEL_NAMING_REVEALED_NEVER; return NAMING_TYPE; }
name_revealed_on_arrival { savetoken(yytext); yylval.i=SPECIAL_LEVEL_NAMING_REVEALED_ON_ARRIVAL; return NAMING_TYPE; }
name_revealed_seeing_monster { savetoken(yytext); yylval.i=SPECIAL_LEVEL_NAMING_REVEALED_ON_SEEING_MONSTER; return NAMING_TYPE; }
lever_effect_none { savetoken(yytext); yylval.i=LEVER_EFFECT_NONE; return LEVER_EFFECT_TYPE; }
lever_effect_open_door { savetoken(yytext); yylval.i=LEVER_EFFECT_OPEN_DOOR; return LEVER_EFFECT_TYPE; }
lever_effect_lock_door { savetoken(yytext); yylval.i=LEVER_EFFECT_LOCK_DOOR; return LEVER_EFFECT_TYPE; }
lever_effect_open_lock_door { savetoken(yytext); yylval.i=LEVER_EFFECT_OPEN_LOCK_DOOR; return LEVER_EFFECT_TYPE; }
lever_effect_create_closed_door { savetoken(yytext); yylval.i=LEVER_EFFECT_CREATE_CLOSED_DOOR; return LEVER_EFFECT_TYPE; }
lever_effect_create_location { savetoken(yytext); yylval.i=LEVER_EFFECT_CREATE_LOCATION_TYPE; return LEVER_EFFECT_TYPE; }
lever_effect_create_uncreate_location { savetoken(yytext); yylval.i=LEVER_EFFECT_CREATE_UNCREATE_LOCATION_TYPE; return LEVER_EFFECT_TYPE; }
lever_effect_create_trap { savetoken(yytext); yylval.i=LEVER_EFFECT_CREATE_TRAP; return LEVER_EFFECT_TYPE; }
lever_effect_create_object { savetoken(yytext); yylval.i=LEVER_EFFECT_CREATE_OBJECT; return LEVER_EFFECT_TYPE; }
lever_effect_create_monster { savetoken(yytext); yylval.i=LEVER_EFFECT_CREATE_MONSTER; return LEVER_EFFECT_TYPE; }
door_wooden { savetoken(yytext); yylval.i=DOOR_SUBTYPE_WOODEN; return DOOR_SUBTYPE; }
door_wooden_windowed { savetoken(yytext); yylval.i=DOOR_SUBTYPE_WOODEN_WINDOWED; return DOOR_SUBTYPE; }
door_iron { savetoken(yytext); yylval.i=DOOR_SUBTYPE_IRON; return DOOR_SUBTYPE; }
door_iron_windowed { savetoken(yytext); yylval.i=DOOR_SUBTYPE_IRON_WINDOWED; return DOOR_SUBTYPE; }
door_iron_bars { savetoken(yytext); yylval.i=DOOR_SUBTYPE_IRON_BARS; return DOOR_SUBTYPE; }
door_magic { savetoken(yytext); yylval.i=DOOR_SUBTYPE_MAGIC; return DOOR_SUBTYPE; }
door_magic_windowed { savetoken(yytext); yylval.i=DOOR_SUBTYPE_MAGIC_WINDOWED; return DOOR_SUBTYPE; }
door_modron { savetoken(yytext); yylval.i=DOOR_SUBTYPE_MODRON; return DOOR_SUBTYPE; }
door_reinforced { savetoken(yytext); yylval.i=DOOR_SUBTYPE_REINFORCED; return DOOR_SUBTYPE; }
door_stone { savetoken(yytext); yylval.i=DOOR_SUBTYPE_STONE; return DOOR_SUBTYPE; }
door_obsidian { savetoken(yytext); yylval.i=DOOR_SUBTYPE_OBSIDIAN; return DOOR_SUBTYPE; }
door_black_gate { savetoken(yytext); yylval.i=DOOR_SUBTYPE_BLACK_GATE; return DOOR_SUBTYPE; }
tree_normal { savetoken(yytext); yylval.i=TREE_SUBTYPE_NORMAL; return TREE_SUBTYPE; }
tree_oak { savetoken(yytext); yylval.i=TREE_SUBTYPE_OAK; return TREE_SUBTYPE; }
tree_spruce { savetoken(yytext); yylval.i=TREE_SUBTYPE_SPRUCE; return TREE_SUBTYPE; }
tree_fir { savetoken(yytext); yylval.i=TREE_SUBTYPE_FIR; return TREE_SUBTYPE; }
tree_date_palm { savetoken(yytext); yylval.i=TREE_SUBTYPE_DATE_PALM; return TREE_SUBTYPE; }
tree_apple { savetoken(yytext); yylval.i=TREE_SUBTYPE_APPLE; return TREE_SUBTYPE; }
tree_fig { savetoken(yytext); yylval.i=TREE_SUBTYPE_FIG; return TREE_SUBTYPE; }
tree_banana { savetoken(yytext); yylval.i=TREE_SUBTYPE_BANANA; return TREE_SUBTYPE; }
tree_orange { savetoken(yytext); yylval.i=TREE_SUBTYPE_ORANGE; return TREE_SUBTYPE; }
tree_pitaya { savetoken(yytext); yylval.i=TREE_SUBTYPE_PITAYA; return TREE_SUBTYPE; }
tree_small { savetoken(yytext); yylval.i=TREE_SUBTYPE_SMALL; return TREE_SUBTYPE; }
tree_fir_snowy { savetoken(yytext); yylval.i=TREE_SUBTYPE_FIR_SNOWY; return TREE_SUBTYPE; }
uninitialized { savetoken(yytext); yylval.i=0; return INITIALIZE_TYPE; }
initialized { savetoken(yytext); yylval.i=1; return INITIALIZE_TYPE; }
forest_normal { savetoken(yytext); yylval.i=FOREST_TYPE_NORMAL; return FOREST_TYPE; }
forest_deciduous { savetoken(yytext); yylval.i=FOREST_TYPE_DECIDUOUS; return FOREST_TYPE; }
forest_coniferous { savetoken(yytext); yylval.i=FOREST_TYPE_CONIFEROUS; return FOREST_TYPE; }
forest_tropical { savetoken(yytext); yylval.i=FOREST_TYPE_TROPICAL; return FOREST_TYPE; }
forest_desert { savetoken(yytext); yylval.i=FOREST_TYPE_DESERT; return FOREST_TYPE; }
forest_garden { savetoken(yytext); yylval.i=FOREST_TYPE_GARDEN; return FOREST_TYPE; }
forest_swamp { savetoken(yytext); yylval.i=FOREST_TYPE_SWAMP; return FOREST_TYPE; }
forest_islands { savetoken(yytext); yylval.i=FOREST_TYPE_ISLANDS; return FOREST_TYPE; }
forest_undead { savetoken(yytext); yylval.i=FOREST_TYPE_UNDEAD; return FOREST_TYPE; }
forest_gehennom { savetoken(yytext); yylval.i=FOREST_TYPE_GEHENNOM; return FOREST_TYPE; }
forest_snowy { savetoken(yytext); yylval.i=FOREST_TYPE_SNOWY; return FOREST_TYPE; }
manual_infernal_inhabitants { savetoken(yytext); yylval.i=MANUAL_INFERNAL_INHABITANTS_OF_GEHENNOM; return MANUAL_TYPE; }
teleport_up    { savetoken(yytext); yylval.i=-1; return LEVEL_TELEPORT_DIRECTION_TYPE; }
teleport_down  { savetoken(yytext); yylval.i=1; return LEVEL_TELEPORT_DIRECTION_TYPE; }
matching_portal { savetoken(yytext); yylval.i=0; return LEVEL_TELEPORT_END_TYPE; }
no_other_end  { savetoken(yytext); yylval.i=1; return LEVEL_TELEPORT_END_TYPE; }
false		{ savetoken(yytext); yylval.i=0; return BOOLEAN; }
true		{ savetoken(yytext); yylval.i=1; return BOOLEAN; }
dust		{ savetoken(yytext); yylval.i=DUST; return ENGRAVING_TYPE; }
engrave		{ savetoken(yytext); yylval.i=ENGRAVE; return ENGRAVING_TYPE; }
burn		{ savetoken(yytext); yylval.i=BURN; return ENGRAVING_TYPE; }
mark		{ savetoken(yytext); yylval.i=MARK; return ENGRAVING_TYPE; }
blood		{ savetoken(yytext); yylval.i=ENGR_BLOOD; return ENGRAVING_TYPE; }
blessed		{ savetoken(yytext); yylval.i=1; return CURSE_TYPE; }
uncursed	{ savetoken(yytext); yylval.i=2; return CURSE_TYPE; }
cursed		{ savetoken(yytext); yylval.i=3; return CURSE_TYPE; }
notcursed	{ savetoken(yytext); yylval.i=4; return CURSE_TYPE; }
notblessed	{ savetoken(yytext); yylval.i=5; return CURSE_TYPE; }
mythic_or_legendary	{ savetoken(yytext); yylval.i=0; return MYTHIC_TYPE; }
mythic		{ savetoken(yytext); yylval.i=1; return MYTHIC_TYPE; }
legendary	{ savetoken(yytext); yylval.i=2; return MYTHIC_TYPE; }
noteleport	{ savetoken(yytext); yylval.i=NOTELEPORT; return FLAG_TYPE; }
hardfloor	{ savetoken(yytext); yylval.i=HARDFLOOR; return FLAG_TYPE; }
nommap		{ savetoken(yytext); yylval.i=NOMMAP; return FLAG_TYPE; }
arboreal	{ savetoken(yytext); yylval.i=ARBOREAL; return FLAG_TYPE; } /* KMH */
swampy	    { savetoken(yytext); yylval.i=SWAMPY; return FLAG_TYPE; } /* JG */
desert	    { savetoken(yytext); yylval.i=DESERT; return FLAG_TYPE; } /* JG */
throne_on_ground { savetoken(yytext); yylval.i=THRONE_ON_GROUND; return FLAG_TYPE; } /* JG */
fountain_on_grass { savetoken(yytext); yylval.i=FOUNTAIN_ON_GRASS; return FLAG_TYPE; } /* JG */
fountain_on_ground { savetoken(yytext); yylval.i=FOUNTAIN_ON_GROUND; return FLAG_TYPE; } /* JG */
tree_on_ground { savetoken(yytext); yylval.i=TREE_ON_GROUND; return FLAG_TYPE; } /* JG */
mapping_does_not_reveal_special { savetoken(yytext); yylval.i=MAPPING_DOES_NOT_REVEAL_SPECIAL; return FLAG_TYPE; } /* JG */
shortsighted	{ savetoken(yytext); yylval.i=SHORTSIGHTED; return FLAG_TYPE; }
brazier_normal { savetoken(yytext); yylval.i=BRAZIER_SUBTYPE_NORMAL; return BRAZIER_SUBTYPE; }
brazier_subtype_1 { savetoken(yytext); yylval.i=BRAZIER_SUBTYPE_1; return BRAZIER_SUBTYPE; }
brazier_subtype_2 { savetoken(yytext); yylval.i=BRAZIER_SUBTYPE_2; return BRAZIER_SUBTYPE; }
signpost_normal { savetoken(yytext); yylval.i=BRAZIER_SUBTYPE_NORMAL; return SIGNPOST_SUBTYPE; }
signpost_subtype_1 { savetoken(yytext); yylval.i=BRAZIER_SUBTYPE_1; return SIGNPOST_SUBTYPE; }
signpost_subtype_2 { savetoken(yytext); yylval.i=BRAZIER_SUBTYPE_2; return SIGNPOST_SUBTYPE; }
mazelevel	{ savetoken(yytext); yylval.i=MAZELEVEL; return FLAG_TYPE; }
premapped	{ savetoken(yytext); yylval.i=PREMAPPED; return FLAG_TYPE; }
shroud		{ savetoken(yytext); yylval.i=SHROUD; return FLAG_TYPE; }
graveyard	{ savetoken(yytext); yylval.i=GRAVEYARD; return FLAG_TYPE; }
icedpools	{ savetoken(yytext); yylval.i=ICEDPOOLS; return FLAG_TYPE; }
solidify	{ savetoken(yytext); yylval.i=SOLIDIFY; return FLAG_TYPE; }
corrmaze        { savetoken(yytext); yylval.i=CORRMAZE; return FLAG_TYPE; }
inaccessibles   { savetoken(yytext); yylval.i=CHECK_INACCESSIBLES; return FLAG_TYPE; }
no_map_padding  { savetoken(yytext); yylval.i=NO_MAP_PADDING; return FLAG_TYPE; }
[0-9]+d[0-9]+	{ char *p = index(yytext, 'd');
		  savetoken(yytext);
		  if (p) {
			*p++ = '\0';
			yylval.dice.num = atoi(yytext);
			yylval.dice.die = atoi(p);
		  } else {
			yylval.dice.num = yylval.dice.die = 1;
		  }
		  return DICE;
		}
\[\ *[0-9]+\%\ *\] { savetoken(yytext); yylval.i = atoi(yytext + 1);
		     if (yylval.i < 0 || yylval.i > 100)
			lc_error("Unexpected percentile '%li%%'", yylval.i);
		     return PERCENT; }
-[0-9]+		{ savetoken(yytext); yylval.i=atoi(yytext); return MINUS_INTEGER; }
\+[0-9]+	{ savetoken(yytext); yylval.i=atoi(yytext); return PLUS_INTEGER; }
[0-9]+\%	{ savetoken(yytext); yylval.i = atoi(yytext);
		  if (yylval.i < 0 || yylval.i > 100)
		     lc_error("Unexpected percentile '%li%%'", yylval.i);
		  return SPERCENT; }
[0-9]+		{ savetoken(yytext); yylval.i=atoi(yytext); return INTEGER; }
\"[^"]*\"	{ savetoken(yytext);
		  yytext[yyleng - 1] = '\0'; /* discard the trailing \" */
		  yylval.map = dupstr(yytext + 1); /* skip the first \" */
		  return STRING; }
\$[a-zA-Z_]+	{ savetoken(yytext); return handle_varstring_check(); }
"=="		{ savetoken(yytext); yylval.i = SPO_JE; return COMPARE_TYPE; }
"!="		{ savetoken(yytext); yylval.i = SPO_JNE; return COMPARE_TYPE; }
"<>"		{ savetoken(yytext); yylval.i = SPO_JNE; return COMPARE_TYPE; }
"<="		{ savetoken(yytext); yylval.i = SPO_JLE; return COMPARE_TYPE; }
">="		{ savetoken(yytext); yylval.i = SPO_JGE; return COMPARE_TYPE; }
"<"		{ savetoken(yytext); yylval.i = SPO_JL; return COMPARE_TYPE; }
">"		{ savetoken(yytext); yylval.i = SPO_JG; return COMPARE_TYPE; }
\r?\n		{ newline(); }
[ \t]+		{ advancepos(yytext); }
'\\.'		{ savetoken(yytext); yylval.i = yytext[2]; return CHAR; }
'.'		{ savetoken(yytext); yylval.i = yytext[1]; return CHAR; }
[-_a-zA-Z0-9]+	ST_RET(UNKNOWN_TYPE);
.		{ savetoken(yytext); return yytext[0]; }
%%
#ifdef	AMIGA
long *
alloc(n)
    unsigned n;
{
    return (long *) malloc(n);
}
#endif

/* routine to switch to another input file; needed for flex */
void
init_yyin( input_f )
FILE *input_f;
{
#if defined(FLEX_SCANNER) || defined(FLEXHACK_SCANNER)
    if (yyin)
        yyrestart(input_f);
    else
#endif
        yyin = input_f;

    if (!orig_yyin)
        orig_yyin = yyin;
}
/* analogous routine (for completeness) */
void
init_yyout( output_f )
FILE *output_f;
{
    yyout = output_f;
}

long
handle_varstring_check()
{
    struct lc_vardefs *vd;

    yylval.map = dupstr(yytext);
    if ((vd = vardef_defined(vardefs, yytext, 1)) != 0) {
        long l = vd->var_type;
        int a = ((l & SPOVAR_ARRAY) == SPOVAR_ARRAY);

        l &= ~SPOVAR_ARRAY;
        if (l == SPOVAR_INT)
	    return (a ? VARSTRING_INT_ARRAY : VARSTRING_INT);
        if (l == SPOVAR_STRING)
	    return (a ? VARSTRING_STRING_ARRAY : VARSTRING_STRING);
        if (l == SPOVAR_VARIABLE)
	    return (a ? VARSTRING_VAR_ARRAY : VARSTRING_VAR);
        if (l == SPOVAR_COORD)
	    return (a ? VARSTRING_COORD_ARRAY : VARSTRING_COORD);
        if (l == SPOVAR_REGION)
	    return (a ? VARSTRING_REGION_ARRAY : VARSTRING_REGION);
        if (l == SPOVAR_MAPCHAR)
	    return (a ? VARSTRING_MAPCHAR_ARRAY : VARSTRING_MAPCHAR);
        if (l == SPOVAR_MONST)
	    return (a ? VARSTRING_MONST_ARRAY : VARSTRING_MONST);
        if (l == SPOVAR_OBJ)
	    return (a ? VARSTRING_OBJ_ARRAY : VARSTRING_OBJ);
        if (l == SPOVAR_SEL)
	    return (a ? VARSTRING_SEL_ARRAY : VARSTRING_SEL);
    }
    return VARSTRING;
}

void
newline()
{
    nh_line_number++;
    token_start_pos = 0;
    (void) memset((genericptr_t) curr_token, 0, 512);
}

void
savetoken(s)
char *s;
{
    Sprintf(curr_token, "%s", s);
    advancepos(s);
}

void
advancepos(s)
char *s;
{
    token_start_pos += strlen(s);
}

/*lev_comp.l*/
