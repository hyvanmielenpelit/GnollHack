%{
/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

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

int NDECL(handle_varstring_check);

extern void VDECL(lc_error, (const char *, ...));
extern struct lc_vardefs *FDECL(vardef_defined,(struct lc_vardefs *,char *, int));

extern struct lc_vardefs *vardefs;

extern int64_t FDECL(method_defined, (char *, int64_t, int64_t *));

void FDECL(savetoken, (char *));
void NDECL(newline);
void FDECL(advancepos, (char *));

/*
 * This doesn't always get put in lev_comp.h
 * (esp. when using older versions of bison).
 */
extern YYSTYPE yylval;

int nh_line_number = 1;
size_t token_start_pos = 0;
char curr_token[512];
static char map[4096];
static size_t map_cnt = 0;

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
		  Strncpy(yylval.map, map, map_cnt);
		  yylval.map[map_cnt] = 0;
		  map_cnt = 0;
		  return MAP_ID;
		}
<MAPC>[-|}{+xABCISHKPLWTF\\#.,: 0123456789]*\r?\n	{
		  size_t len = yyleng < 0 ? 0 : (size_t)yyleng;
		  savetoken(yytext);
		  /* convert \r\n to \n */
		  if (len >= 2 && yytext[len - 2] == '\r')
		      len -= 1;
		  Strncpy(map + map_cnt, yytext, len);
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
tileset	    ST_RET(TILESET_PARAM_ID);
decoration_style	ST_RET(DECOTYP_ID);
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
DECORATION  ST_RET(DECORATION_ID);
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
CARPET		ST_RET(CARPET_ID);
CARPET_PIECE		ST_RET(CARPET_PIECE_ID);
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
material	ST_RET(MATERIAL_ID);
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
level_boss	ST_RET(LEVEL_BOSS_ID);
use_boss_hostility	ST_RET(BOSS_HOSTILITY_ID);
has_backup	ST_RET(HAS_BACKUP_ID);
level_adjustment  ST_RET(LEVEL_ADJUSTMENT_ID);
keep_original_inventory	ST_RET(KEEP_ORIGINAL_INVENTORY_ID);
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
decoration_cobweb { savetoken(yytext); yylval.i=DECORATION_COBWEB; return DECORATION_TYPE; }
decoration_cobweb_corner { savetoken(yytext); yylval.i=DECORATION_COBWEB_CORNER; return DECORATION_TYPE; }
decoration_torch { savetoken(yytext); yylval.i=DECORATION_TORCH; return DECORATION_TYPE; }
decoration_lantern { savetoken(yytext); yylval.i=DECORATION_LANTERN; return DECORATION_TYPE; }
decoration_fireplace { savetoken(yytext); yylval.i=DECORATION_FIREPLACE; return DECORATION_TYPE; }
decoration_fireplace2 { savetoken(yytext); yylval.i=DECORATION_ANOTHER_FIREPLACE; return DECORATION_TYPE; }
decoration_painting { savetoken(yytext); yylval.i=DECORATION_PAINTING; return DECORATION_TYPE; }
decoration_banner { savetoken(yytext); yylval.i=DECORATION_BANNER; return DECORATION_TYPE; }
decoration_wall_sculpture { savetoken(yytext); yylval.i=DECORATION_WALL_SCULPTURE; return DECORATION_TYPE; }
decoration_shield_with_swords { savetoken(yytext); yylval.i=DECORATION_SHIELD_WITH_SWORDS; return DECORATION_TYPE; }
decoration_gargoyle_statue { savetoken(yytext); yylval.i=DECORATION_GARGOYLE_NICHE; return DECORATION_TYPE; }
decoration_knight_statue { savetoken(yytext); yylval.i=DECORATION_KNIGHT_NICHE; return DECORATION_TYPE; }
decoration_priest_statue { savetoken(yytext); yylval.i=DECORATION_PRIEST_NICHE; return DECORATION_TYPE; }
direction_top { savetoken(yytext); yylval.i=0; return DECORATION_DIR; }
direction_left { savetoken(yytext); yylval.i=1; return DECORATION_DIR; }
direction_right { savetoken(yytext); yylval.i=2; return DECORATION_DIR; }
direction_bottom { savetoken(yytext); yylval.i=3; return DECORATION_DIR; }
empty_holder { savetoken(yytext); yylval.i=0; return DECORATION_ITEM_STATE; }
item_in_holder { savetoken(yytext); yylval.i=1; return DECORATION_ITEM_STATE; }
item2_in_holder { savetoken(yytext); yylval.i=2; return DECORATION_ITEM_STATE; }
item12_in_holder { savetoken(yytext); yylval.i=3; return DECORATION_ITEM_STATE; }
item3_in_holder { savetoken(yytext); yylval.i=4; return DECORATION_ITEM_STATE; }
item13_in_holder { savetoken(yytext); yylval.i=5; return DECORATION_ITEM_STATE; }
item23_in_holder { savetoken(yytext); yylval.i=6; return DECORATION_ITEM_STATE; }
items_in_holder { savetoken(yytext); yylval.i=7; return DECORATION_ITEM_STATE; }
painting_wizard_blue { savetoken(yytext); yylval.i=PAINTING_WIZARD_OF_YENDOR_BLUE; return PAINTING_TYPE; }
painting_wizard_red { savetoken(yytext); yylval.i=PAINTING_WIZARD_OF_YENDOR_RED; return PAINTING_TYPE; }
painting_vlad { savetoken(yytext); yylval.i=PAINTING_VLAD_THE_IMPALER; return PAINTING_TYPE; }
painting_vampire_male { savetoken(yytext); yylval.i=PAINTING_VAMPIRE_MALE; return PAINTING_TYPE; }
painting_vampire_female { savetoken(yytext); yylval.i=PAINTING_VAMPIRE_FEMALE; return PAINTING_TYPE; }
banner_red { savetoken(yytext); yylval.i=BANNER_RED; return BANNER_TYPE; }
banner_red_old { savetoken(yytext); yylval.i=BANNER_RED_OLD; return BANNER_TYPE; }
banner_purple { savetoken(yytext); yylval.i=BANNER_PURPLE; return BANNER_TYPE; }
banner_purple_old { savetoken(yytext); yylval.i=BANNER_PURPLE_OLD; return BANNER_TYPE; }
wall_sculpture_modron_spherical { savetoken(yytext); yylval.i=WALL_SCULPTURE_MODRON_SPHERICAL; return WALL_SCULPTURE_TYPE; }
wall_sculpture_modron_triangular { savetoken(yytext); yylval.i=WALL_SCULPTURE_MODRON_TRIANGULAR; return WALL_SCULPTURE_TYPE; }
message_sound_type_none { savetoken(yytext); yylval.i=0; return MESSAGE_SOUND_TYPE; }
message_sound_type_ugod { savetoken(yytext); yylval.i=1; return MESSAGE_SOUND_TYPE; }
message_sound_type_sfx { savetoken(yytext); yylval.i=2; return MESSAGE_SOUND_TYPE; }
message_sound_id_ugod_well_done { savetoken(yytext); yylval.i=GOD_LINE_WELL_DONE_MORTAL; return MESSAGE_SOUND_ID; }
message_sound_id_ugod_final_test { savetoken(yytext); yylval.i=GOD_LINE_FINAL_TEST; return MESSAGE_SOUND_ID; }
message_sound_id_ugod_prove_thyself { savetoken(yytext); yylval.i=GOD_LINE_PROVE_THYSELF; return MESSAGE_SOUND_ID; }
message_atr_none { savetoken(yytext); yylval.i=ATR_NONE; return MESSAGE_ATTR; }
message_clr_normal { savetoken(yytext); yylval.i=NO_COLOR; return MESSAGE_COLOR; }
message_clr_god { savetoken(yytext); yylval.i=CLR_MSG_GOD; return MESSAGE_COLOR; }
message_clr_positive { savetoken(yytext); yylval.i=CLR_MSG_POSITIVE; return MESSAGE_COLOR; }
message_clr_negative { savetoken(yytext); yylval.i=CLR_MSG_NEGATIVE; return MESSAGE_COLOR; }
message_clr_attention { savetoken(yytext); yylval.i=CLR_MSG_ATTENTION; return MESSAGE_COLOR; }
message_clr_hint { savetoken(yytext); yylval.i=CLR_MSG_HINT; return MESSAGE_COLOR; }
message_clr_spell { savetoken(yytext); yylval.i=CLR_MSG_SPELL; return MESSAGE_COLOR; }
message_clr_mystical { savetoken(yytext); yylval.i=CLR_MSG_MYSTICAL; return MESSAGE_COLOR; }
message_clr_success { savetoken(yytext); yylval.i=CLR_MSG_SUCCESS; return MESSAGE_COLOR; }
message_clr_fail { savetoken(yytext); yylval.i=CLR_MSG_FAIL; return MESSAGE_COLOR; }
message_type_message { savetoken(yytext); yylval.i=SPLEV_MESSAGE_TYPE_MESSAGE; return MESSAGE_TYPE; }
message_type_ugod { savetoken(yytext); yylval.i=SPLEV_MESSAGE_TYPE_UGOD; return MESSAGE_TYPE; }
message_type_plane_arrival { savetoken(yytext); yylval.i=SPLEV_MESSAGE_TYPE_PLANE_ARRIVAL; return MESSAGE_TYPE; }
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
undead_tileset { savetoken(yytext); yylval.i=CMAP_UNDEAD; return CMAP_TYPE; }
sokoban_tileset { savetoken(yytext); yylval.i=CMAP_SOKOBAN; return CMAP_TYPE; }
elemental_tileset { savetoken(yytext); yylval.i=CMAP_ELEMENTAL_PLANES; return CMAP_TYPE; }
astral_tileset { savetoken(yytext); yylval.i=CMAP_ASTRAL; return CMAP_TYPE; }
gehennom_cavernous_tileset { savetoken(yytext); yylval.i=CMAP_GEHENNOM_CAVERNOUS; return CMAP_TYPE; }
garden_tileset { savetoken(yytext); yylval.i=CMAP_GARDEN; return CMAP_TYPE; }
cityscape_tileset { savetoken(yytext); yylval.i=CMAP_CITYSCAPE; return CMAP_TYPE; }
nest_tileset { savetoken(yytext); yylval.i=CMAP_NEST; return CMAP_TYPE; }
temple_tileset { savetoken(yytext); yylval.i=CMAP_TEMPLE; return CMAP_TYPE; }
opulent_tileset { savetoken(yytext); yylval.i=CMAP_OPULENT; return CMAP_TYPE; }
modron_tileset { savetoken(yytext); yylval.i=CMAP_MODRON; return CMAP_TYPE; }
reinforced_tileset { savetoken(yytext); yylval.i=CMAP_REINFORCED; return CMAP_TYPE; }
tower_tileset { savetoken(yytext); yylval.i=CMAP_TOWER; return CMAP_TYPE; }
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
carpet_red { savetoken(yytext); yylval.i=CARPET_RED; return CARPET_TYPE; }
carpet_purple { savetoken(yytext); yylval.i=CARPET_PURPLE; return CARPET_TYPE; }
carpet_crimson { savetoken(yytext); yylval.i=CARPET_CRIMSON; return CARPET_TYPE; }
carpet_brown_hide { savetoken(yytext); yylval.i=CARPET_BROWN_ANIMAL_HIDE; return CARPET_TYPE; }
carpet_modron_spherical_plaque { savetoken(yytext); yylval.i=CARPET_MODRON_SPHERICAL_PLAQUE; return CARPET_TYPE; }
carpet_yellow { savetoken(yytext); yylval.i=CARPET_YELLOW; return CARPET_TYPE; }
carpet_beige { savetoken(yytext); yylval.i=CARPET_BEIGE; return CARPET_TYPE; }
carpet_blue { savetoken(yytext); yylval.i=CARPET_BLUE; return CARPET_TYPE; }
carpet_modron_triangular_plaque { savetoken(yytext); yylval.i=CARPET_MODRON_TRIANGULAR_PLAQUE; return CARPET_TYPE; }
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
planar_by_original_alignment { savetoken(yytext); yylval.i=EXCEPTIONALITY_PLANAR_BY_ORIGINAL_ALIGNMENT; return EXCEPTIONALITY_TYPE; }
no_mythic_prefix { savetoken(yytext); yylval.i=MYTHIC_PREFIX_NONE; return MYTHIC_PREFIX_TYPE; }
no_mythic_suffix { savetoken(yytext); yylval.i=MYTHIC_SUFFIX_NONE; return MYTHIC_SUFFIX_TYPE; }
material_none { savetoken(yytext); yylval.i=MAT_NONE; return MATERIAL_TYPE; }
material_silver { savetoken(yytext); yylval.i=MAT_SILVER; return MATERIAL_TYPE; }
material_orichalcum { savetoken(yytext); yylval.i=MAT_ORICHALCUM; return MATERIAL_TYPE; }
material_mithril { savetoken(yytext); yylval.i=MAT_MITHRIL; return MATERIAL_TYPE; }
material_adamantium { savetoken(yytext); yylval.i=MAT_ADAMANTIUM; return MATERIAL_TYPE; }
material_bronze { savetoken(yytext); yylval.i=MAT_BRONZE; return MATERIAL_TYPE; }
material_bone { savetoken(yytext); yylval.i=MAT_BONE; return MATERIAL_TYPE; }
material_iron { savetoken(yytext); yylval.i=MAT_IRON; return MATERIAL_TYPE; }
material_steel { savetoken(yytext); yylval.i=MAT_STEEL; return MATERIAL_TYPE; }
material_gold { savetoken(yytext); yylval.i=MAT_GOLD; return MATERIAL_TYPE; }
material_platinum { savetoken(yytext); yylval.i=MAT_PLATINUM; return MATERIAL_TYPE; }
material_gemstone { savetoken(yytext); yylval.i=MAT_GEMSTONE; return MATERIAL_TYPE; }
material_hard_crystal { savetoken(yytext); yylval.i=MAT_HARD_CRYSTAL; return MATERIAL_TYPE; }
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

int
handle_varstring_check()
{
    struct lc_vardefs *vd;

    yylval.map = dupstr(yytext);
    if ((vd = vardef_defined(vardefs, yytext, 1)) != 0) {
        int64_t l = vd->var_type;
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
