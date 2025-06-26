/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    sp_lev.h    $NHDT-Date: 1544930819 2018/12/16 03:26:59 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.25 $ */
/* Copyright (c) 1989 by Jean-Christophe Collet              */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef SP_LEV_H
#define SP_LEV_H

/* wall directions */
#define W_NORTH 1
#define W_SOUTH 2
#define W_EAST 4
#define W_WEST 8
#define W_ANY (W_NORTH | W_SOUTH | W_EAST | W_WEST)

/* MAP limits */
#define MAP_X_LIM 76
#define MAP_Y_LIM 21

/* Per level flags */
#define NOTELEPORT      0x00000001L
#define HARDFLOOR       0x00000002L
#define NOMMAP          0x00000004L
#define SHORTSIGHTED    0x00000008L
#define ARBOREAL        0x00000010L
#define MAZELEVEL       0x00000020L
#define PREMAPPED       0x00000040L  /* premapped level & sokoban rules */
#define SHROUD          0x00000080L
#define GRAVEYARD       0x00000100L
#define ICEDPOOLS       0x00000200L  /* for ice locations: ICED_POOL vs ICED_MOAT */
#define SOLIDIFY        0x00000400L  /* outer areas are nondiggable & nonpasswall */
#define CORRMAZE        0x00000800L  /* for maze levels only */
#define CHECK_INACCESSIBLES                 0x00001000L /* check for inaccessible areas and generate ways to escape from them */
#define SWAMPY                              0x00002000L
#define THRONE_ON_GROUND                    0x00004000L
#define FOUNTAIN_ON_GRASS                   0x00008000L
#define FOUNTAIN_ON_GROUND                  0x00010000L
#define TREE_ON_GROUND                      0x00020000L
#define MAPPING_DOES_NOT_REVEAL_SPECIAL     0x00040000L
#define NO_MAP_PADDING                      0x00080000L
#define DESERT                              0x00100000L

/* different level layout initializers */
enum lvlinit_types {
    LVLINIT_NONE = 0,
    LVLINIT_SOLIDFILL,
    LVLINIT_MAZEGRID,
    LVLINIT_MINES,
    LVLINIT_ROGUE
};

/* max. layers of object containment */
#define MAX_CONTAINMENT 10

/* max. # of random registers */
#define MAX_REGISTERS 10

/* max. nested depth of subrooms */
#define MAX_NESTED_ROOMS 5

/* max. # of opcodes per special level */
#define SPCODER_MAX_RUNTIME 65536

/* Opcodes for creating the level
 * If you change these, also change opcodestr[] in util/lev_main.c
 */
enum opcode_defs {
    SPO_NULL = 0,
    SPO_MESSAGE,
    SPO_MONSTER,
    SPO_OBJECT,
    SPO_ENGRAVING,
    SPO_ROOM,
    SPO_SUBROOM,
    SPO_DOOR,
    SPO_STAIR,
    SPO_LADDER,
    SPO_ALTAR,
    SPO_ANVIL,
    SPO_DECORATION,
    SPO_FLOOR,
    SPO_SUBTYPE,
    SPO_CARPET,
    SPO_CARPET_PIECE,
    SPO_NPC,
    SPO_FOUNTAIN,
    SPO_THRONE,
    SPO_MODRON_PORTAL,
    SPO_MODRON_LEVEL_TELEPORTER,
    SPO_SINK,
    SPO_POOL,
    SPO_TRAP,
    SPO_LEVER,
    SPO_GOLD,
    SPO_CORRIDOR,
    SPO_LEVREGION,
    SPO_DRAWBRIDGE,
    SPO_MAZEWALK,
    SPO_NON_DIGGABLE,
    SPO_NON_PASSWALL,
    SPO_WALLIFY,
    SPO_MAP,
    SPO_ROOM_DOOR,
    SPO_REGION,
    SPO_SPECIAL_REGION,
    SPO_SPECIAL_TILESET,
    SPO_NAMING,
    SPO_MINERALIZE,
    SPO_CMP,
    SPO_JMP,
    SPO_JL,
    SPO_JLE,
    SPO_JG,
    SPO_JGE,
    SPO_JE,
    SPO_JNE,
    SPO_TERRAIN,
    SPO_REPLACETERRAIN,
    SPO_EXIT,
    SPO_ENDROOM,
    SPO_POP_CONTAINER,
    SPO_PUSH,
    SPO_POP,
    SPO_RN2,
    SPO_DEC,
    SPO_INC,
    SPO_MATH_ADD,
    SPO_MATH_SUB,
    SPO_MATH_MUL,
    SPO_MATH_DIV,
    SPO_MATH_MOD,
    SPO_MATH_SIGN,
    SPO_COPY,
    SPO_END_MONINVENT,
    SPO_GRAVE,
    SPO_BRAZIER,
    SPO_SIGNPOST,
    SPO_TREE,
    SPO_FRAME_PUSH,
    SPO_FRAME_POP,
    SPO_CALL,
    SPO_RETURN,
    SPO_INITLEVEL,
    SPO_LEVEL_FLAGS,
    SPO_TILESET,
    SPO_FOREST,
    SPO_MONSTER_GENERATION,
    SPO_BOUNDARY_TYPE,
    SPO_VAR_INIT, /* variable_name data */
    SPO_SHUFFLE_ARRAY,
    SPO_DICE,

    SPO_SEL_ADD,
    SPO_SEL_POINT,
    SPO_SEL_RECT,
    SPO_SEL_FILLRECT,
    SPO_SEL_LINE,
    SPO_SEL_RNDLINE,
    SPO_SEL_GROW,
    SPO_SEL_FLOOD,
    SPO_SEL_RNDCOORD,
    SPO_SEL_ELLIPSE,
    SPO_SEL_FILTER,
    SPO_SEL_GRADIENT,
    SPO_SEL_COMPLEMENT,

    MAX_SP_OPCODES
};

/* MONSTER and OBJECT can take a variable number of parameters,
 * they also pop different # of values from the stack. So,
 * first we pop a value that tells what the _next_ value will
 * mean.
 */
/* MONSTER */
enum sp_mon_var_flags {
    SP_M_V_PEACEFUL = 0,
    SP_M_V_ALIGN,
    SP_M_V_ASLEEP,
    SP_M_V_APPEAR,
    SP_M_V_NAME,
    SP_M_V_FEMALE,
    SP_M_V_INVIS,
    SP_M_V_CANCELLED,
    SP_M_V_REVIVED,
    SP_M_V_AVENGE,
    SP_M_V_FLEEING,
    SP_M_V_BLINDED,
    SP_M_V_PARALYZED,
    SP_M_V_STUNNED,
    SP_M_V_CONFUSED,
    SP_M_V_SEENTRAPS,
    SP_M_V_WAITFORU,
    SP_M_V_PROTECTOR,
    SP_M_V_MAXHP,
    SP_M_V_LEVEL_ADJUSTMENT,
    SP_M_V_KEEP_ORIGINAL_INVENTORY,
    SP_M_V_LEVEL_BOSS,
    SP_M_V_BOSS_HOSTILITY,
    SP_M_V_HAS_BACKUP,

    SP_M_V_END
};

/* OBJECT */
enum sp_obj_var_flags {
    SP_O_V_SPE = 0,
    SP_O_V_CURSE,
    SP_O_V_CORPSENM,
    SP_O_V_NAME,
    SP_O_V_QUAN,
    SP_O_V_BURIED,
    SP_O_V_LIT,
    SP_O_V_ERODED,
    SP_O_V_LOCKED,
    SP_O_V_TRAPPED,
    SP_O_V_RECHARGED,
    SP_O_V_INVIS,
    SP_O_V_GREASED,
    SP_O_V_BROKEN,
    SP_O_V_COORD,
    SP_O_V_ENCHANTMENT,
    SP_O_V_CHARGES,
    SP_O_V_SPECIAL_QUALITY,
    SP_O_V_SPEFLAGS,
    SP_O_V_MATERIAL,
    SP_O_V_ELEMENTAL_ENCHANTMENT,
    SP_O_V_EXCEPTIONALITY,
    SP_O_V_MYTHIC_TYPE,
    SP_O_V_MYTHIC_PREFIX,
    SP_O_V_MYTHIC_SUFFIX,
    SP_O_V_AGE,
    SP_O_V_KEY_TYPE,
    SP_O_V_INDESTRUCTIBLE,
    SP_O_V_USES_UP_KEY,
    SP_O_V_NO_PICKUP,
    SP_O_V_OPEN,

    SP_O_V_END
};

/* LEVER */
enum sp_lever_var_flags {
    SP_L_V_ACTIVE = 0,
    SP_L_V_SWITCHABLE,
    SP_L_V_CONTINUOUS,
    SP_L_V_MONSTER,
    SP_L_V_OBJECT,
    SP_L_V_TRAP,
    SP_L_V_TERRAIN,
    SP_L_V_TERRAIN2,
    SP_L_V_SPECIAL_QUALITY,
    SP_L_V_SUBTYPE,
    SP_L_V_FLOOR_SUBTYPE,
    SP_L_V_EFFECT_FLAG,
    SP_L_V_COORD,

    SP_L_V_END
};

/* DOOR */
enum sp_door_var_flags {
    SP_D_V_SUBTYPE = 0,
    SP_D_V_KEY_TYPE,
    SP_D_V_SPECIAL_QUALITY,
    SP_D_V_INDESTRUCTIBLE,
    SP_D_V_SECRET_DOOR,
    SP_D_V_USES_UP_KEY,
    SP_D_V_NON_PASSDOOR,

    SP_D_V_END
};


/* When creating objects, we need to know whether
 * it's a container and/or contents.
 */
#define SP_OBJ_CONTENT 0x1
#define SP_OBJ_CONTAINER 0x2

/* SPO_FILTER types */
#define SPOFILTER_PERCENT 0
#define SPOFILTER_SELECTION 1
#define SPOFILTER_MAPCHAR 2

/* gradient filter types */
#define SEL_GRADIENT_RADIAL 0
#define SEL_GRADIENT_SQUARE 1

/* variable types */
#define SPOVAR_NULL 0x00
#define SPOVAR_INT 0x01      /* l */
#define SPOVAR_STRING 0x02   /* str */
#define SPOVAR_VARIABLE 0x03 /* str (contains the variable name) */
#define SPOVAR_COORD \
    0x04 /* coordinate, encoded in l; use SP_COORD_X() and SP_COORD_Y() */
#define SPOVAR_REGION 0x05  /* region, encoded in l; use SP_REGION_X1() etc \
                               */
#define SPOVAR_MAPCHAR 0x06 /* map char, in l */
#define SPOVAR_MONST                                                         \
    0x07 /* monster class & specific monster, encoded in l; use SP_MONST_... \
            */
#define SPOVAR_OBJ                                                 \
    0x08 /* object class & specific object type, encoded in l; use \
            SP_OBJ_... */
#define SPOVAR_SEL 0x09   /* selection. char[COLNO][ROWNO] in str */
#define SPOVAR_ARRAY 0x40 /* used in splev_var & lc_vardefs, not in opvar */

#define SP_COORD_IS_RANDOM 0x01000000L
/* Humidity flags for get_location() and friends, used with
 * SP_COORD_PACK_RANDOM() */
#define FLOORLOC    0x0001 /* any normal solid floor is ok */
#define WET         0x0002 /* pool */
#define HOT         0x0004 /* lava */
#define SOLID       0x0008 /* solid stone */
#define ANY_LOC     0x0010 /* even outside the level */
#define NO_LOC_WARN 0x0020 /* no complaints and set x & y to -1, if no loc */
#define SPACELOC    0x0040 /* like DRY, but accepts furniture too */
#define ICELOC      0x0080 /* ice is ok */
#define AIRLOC      0x0100 /* air is ok */
#define DRY         (FLOORLOC | AIRLOC | ICELOC)
#define DRY_NO_ICE  (FLOORLOC | AIRLOC)

#define SP_COORD_X(l) (l & 0xff)
#define SP_COORD_Y(l) ((l >> 16) & 0xff)
#define SP_COORD_PACK(x, y) (((x) & 0xff) + (((y) & 0xff) << 16))
#define SP_COORD_PACK_RANDOM(f) (SP_COORD_IS_RANDOM | (f))

#define SP_REGION_X1(l) (l & 0xff)
#define SP_REGION_Y1(l) ((l >> 8) & 0xff)
#define SP_REGION_X2(l) ((l >> 16) & 0xff)
#define SP_REGION_Y2(l) ((l >> 24) & 0xff)
#define SP_REGION_PACK(x1, y1, x2, y2) \
    (((x1) & 0xff) + (((y1) & 0xff) << 8) + (((x2) & 0xff) << 16) \
     + (((y2) & 0xff) << 24))

/* permonst index, object index, and lit value might be negative;
 * add 10 to accept -1 through -9 while forcing non-negative for bit shift
 */
#define SP_MONST_CLASS(l) ((l) & 0xff)
#define SP_MONST_PM(l) ((((l) >> 8) & 0xffff) - 10)
#define SP_MONST_PACK(pm, cls) (((10 + (pm)) << 8) | ((cls) & 0xff))

#define SP_OBJ_CLASS(l) ((l) & 0xff)
#define SP_OBJ_TYP(l) ((((l) >> 8) & 0xffff) - 10)
#define SP_OBJ_PACK(ob, cls) (((10 + (ob)) << 8) | ((cls) & 0xff))

#define SP_MAPCHAR_TYP(l) ((l) & 0xff)
#define SP_MAPCHAR_LIT(l) ((((l) >> 8) & 0xffff) - 10)
#define SP_MAPCHAR_PACK(typ, lit) (((10 + (lit)) << 8) | ((typ) & 0xff))

struct splev_var {
    struct splev_var *next;
    char *name;
    xchar svtyp; /* SPOVAR_foo */
    union {
        struct opvar *value;
        struct opvar **arrayvalues;
    } data;
    int64_t array_len;
};

struct splevstack {
    int64_t depth;
    int64_t depth_alloc;
    struct opvar **stackdata;
};

struct sp_frame {
    struct sp_frame *next;
    struct splevstack *stack;
    struct splev_var *variables;
    int64_t n_opcode;
};

struct sp_coder {
    struct splevstack *stack;
    struct sp_frame *frame;
    int premapped;
    boolean solidify;
    struct mkroom *croom;
    struct mkroom *tmproomlist[MAX_NESTED_ROOMS + 1];
    boolean failed_room[MAX_NESTED_ROOMS + 1];
    int n_subroom;
    boolean exit_script;
    int lvl_is_joined;
    boolean check_inaccessibles;
    boolean no_map_padding;
    boolean throne_on_ground;
    boolean fountain_on_ground;
    boolean fountain_on_grass;
    boolean tree_on_ground;

    int opcode;          /* current opcode */
    struct opvar *opdat; /* current push data (req. opcode == SPO_PUSH) */
};

/* special level coder CPU flags */
#define SP_CPUFLAG_LT 1
#define SP_CPUFLAG_GT 2
#define SP_CPUFLAG_EQ 4
#define SP_CPUFLAG_ZERO 8

/*
 * Structures manipulated by the special levels loader & compiler
 */

#define packed_coord int64_t
typedef struct {
    xchar is_random;
    int64_t getloc_flags;
    int x, y;
} unpacked_coord;

typedef struct {
    int cmp_what;
    int cmp_val;
} opcmp;

typedef struct {
    int64_t jmp_target;
} opjmp;

typedef union str_or_len {
    char *str;
    int len;
} Str_or_Len;

typedef struct {
    xchar init_style; /* one of LVLINIT_foo */
    int64_t flags;
    schar filling;
    boolean init_present, padding;
    char fg, bg;
    boolean smoothed, joined;
    xchar lit, walled;
    boolean icedpools;
    boolean throne_on_ground;
    boolean fountain_on_ground;
    boolean fountain_on_grass;
    boolean tree_on_ground;
} lev_init;

typedef struct {
    xchar wall, pos, secret;
    short mask;
    unsigned short dflags;
    int key_otyp, key_special_quality, subtype;
} room_door;

typedef struct {
    packed_coord coord;
    xchar x, y, type;
} spltrap;

typedef struct {
    packed_coord coord;
    packed_coord target_coord;
    int lever_effect;
    int64_t effect_parameter1;
    int64_t effect_parameter2;
    int64_t effect_parameter3;
    int64_t effect_parameter4;
    int64_t effect_subtype;
    int64_t effect_subtype2;
    uint64_t effect_flags;
    uint64_t lever_flags;
} spllever;

typedef struct {
    Str_or_Len name, appear_as;
    short id;
    aligntyp align;
    packed_coord coord;
    xchar x, y, class, appear;
    schar peaceful, asleep;
    schar female, invis, cancelled, revived, avenge, fleeing, blinded,
        paralyzed, stunned, confused, waitforu, protector, maxhp, level_adjustment, keep_original_invent,
        level_boss, use_boss_hostility, has_backup;
    int64_t seentraps;
    schar has_invent;
} monster;

typedef struct {
    Str_or_Len name;
    int corpsenm;
    short id, enchantment;
    short charges;
    int special_quality;
    uint64_t speflags;
    packed_coord coord;
    xchar x, y, class, containment;
    schar curse_state;
    int quan;
    short buried;
    short lit;
    short eroded, locked, trapped, recharged, invis, greased, broken, indestructible, uses_up_key, no_pickup, open;
    short material;
    int elemental_enchantment;
    int exceptionality;
    schar mythic_type;
    int mythic_prefix;
    int mythic_suffix;
    int age;
} object;

typedef struct {
    packed_coord coord;
    xchar x, y;
    aligntyp align;
    xchar shrine;
    xchar subtyp;
    int mtype;
} altar;

typedef struct {
    packed_coord coord;
    xchar x, y;
    int mtype;
} anvil;

typedef struct {
    packed_coord coord;
    xchar x, y;
    schar typ;
    schar subtyp;
    schar dir;
    uchar item_in_holder;
    boolean lit;
} decoration;

typedef struct {
    packed_coord coord;
    xchar x, y;
    uchar typ;
    int mtype;
} npc_create_info;


typedef struct {
    packed_coord coord;
    packed_coord t_coord;
    uchar typ;
    boolean activated;
    boolean level_teleporter;
    int tele_direction;
    boolean end_type;
} modron_portal;

typedef struct {
    xchar x1, y1, x2, y2;
    xchar rtype, rlit, rirreg, rfloortyp, rfloorsubtyp, mtype;
} region;

typedef struct {
    xchar ter, subter, tlit;
} terrain;

typedef struct {
    xchar chance;
    xchar x1, y1, x2, y2;
    xchar fromter, toter, tolit;
} replaceterrain;

/* values for rtype are defined in dungeon.h */
typedef struct {
    struct {
        xchar x1, y1, x2, y2;
    } inarea;
    struct {
        xchar x1, y1, x2, y2;
    } delarea;
    boolean in_islev, del_islev;
    xchar rtype, padding;
    Str_or_Len rname;
} lev_region;

typedef struct {
    struct {
        xchar room;
        xchar wall;
        xchar door;
    } src, dest;
} corridor;

typedef struct _room {
    Str_or_Len name;
    Str_or_Len parent;
    xchar x, y, w, h;
    xchar xalign, yalign;
    xchar rtype, chance, rlit, filled, joined, rfloortyp, rfloorsubtyp;
    short mtype, tileset, decotyp;
} room;

typedef struct {
    schar zaligntyp;
    schar keep_region;
    schar halign, valign;
    char xsize, ysize;
    char **map;
} mazepart;

typedef struct {
    int opcode;
    struct opvar *opdat;
} _opcode;

typedef struct {
    _opcode *opcodes;
    int64_t n_opcodes;
} sp_lev;

typedef struct {
    xchar x, y, direction, count, lit;
    char typ;
} spill;

/* only used by lev_comp */
struct lc_funcdefs_parm {
    char *name;
    char parmtype;
    struct lc_funcdefs_parm *next;
};

struct lc_funcdefs {
    struct lc_funcdefs *next;
    char *name;
    int64_t addr;
    sp_lev code;
    int64_t n_called;
    struct lc_funcdefs_parm *params;
    int64_t n_params;
};

struct lc_vardefs {
    struct lc_vardefs *next;
    char *name;
    int64_t var_type; /* SPOVAR_foo */
    int64_t n_used;
};

struct lc_breakdef {
    struct lc_breakdef *next;
    struct opvar *breakpoint;
    int break_depth;
};

struct lev_msg {
    struct lev_msg* next;
    char* message;
    int msg_type;
    int attr;
    int color;
    int sound_type;
    int sound_id;
    int sound_param;
    uint64_t msgflags;
};

enum splev_message_types {
    SPLEV_MESSAGE_TYPE_MESSAGE = 0,
    SPLEV_MESSAGE_TYPE_UGOD,
    SPLEV_MESSAGE_TYPE_PLANE_ARRIVAL,
};

enum object_special_create_types {
    OBJECT_SPECIAL_CREATE_TYPE_RANDOM = -1,
    OBJECT_SPECIAL_CREATE_TYPE_CLASS_TREASURE_WEAPON = -2,
    OBJECT_SPECIAL_CREATE_TYPE_CLASS_TREASURE_ARMOR = -3,
    OBJECT_SPECIAL_CREATE_TYPE_ARTIFACT = -4,
    OBJECT_SPECIAL_CREATE_TYPE_CLASS_ARTIFACT = -5,
    OBJECT_SPECIAL_CREATE_TYPE_ORC_ARTIFACT = -6,
    OBJECT_SPECIAL_CREATE_TYPE_RANDOM_CONTENTS = -7
};

/*
 * Quick! Avert your eyes while you still have a chance!
 */
#ifdef SPEC_LEV
/* compiling lev_comp rather than GnollHack */
/* clang format off */
#ifdef USE_OLDARGS
#ifndef VA_TYPE
typedef const char *vA;
#define VA_TYPE
#endif
/* hack to avoid "warning: cast to 'vA' (aka 'const char *') from smaller
   integer type 'int' [-Wint-to-pointer-cast]" */
#define vA_(a) ((vA) (int64_t) a)
#undef VA_ARGS  /* redefine with the maximum number actually used */
#undef VA_SHIFT /* ditto */
#undef VA_PASS1
#define VA_ARGS                                                         \
    arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, \
        arg12, arg13, arg14
/* Unlike in the core, lev_comp's VA_SHIFT should be completely safe,
   because callers always pass all these arguments. */
#define VA_SHIFT()                                                       \
    (arg1 = arg2, arg2 = arg3, arg3 = arg4, arg4 = arg5, arg5 = arg6,    \
     arg6 = arg7, arg7 = arg8, arg8 = arg9, arg9 = arg10, arg10 = arg11, \
     arg11 = arg12, arg12 = arg13, arg13 = arg14, arg14 = 0)
/* standard NULL may be either (void *)0 or plain 0, both of
   which would need to be explicitly cast to (char *) here */
#define VA_PASS1(a1) \
    vA_(a1), vA_(0), vA_(0), vA_(0), vA_(0), vA_(0), vA_(0), vA_(0),    \
        vA_(0), vA_(0), vA_(0), vA_(0), vA_(0), vA_(0)
#define VA_PASS2(a1,a2) \
    vA_(a1), vA_(a2), vA_(0), vA_(0), vA_(0), vA_(0), vA_(0), vA_(0),   \
        vA_(0), vA_(0), vA_(0), vA_(0), vA_(0), vA_(0)
#define VA_PASS3(a1,a2,a3) \
    vA_(a1), vA_(a2), vA_(a3), vA_(0), vA_(0), vA_(0), vA_(0), vA_(0),  \
        vA_(0), vA_(0), vA_(0), vA_(0), vA_(0), vA_(0)
#define VA_PASS4(a1,a2,a3,a4) \
    vA_(a1), vA_(a2), vA_(a3), vA_(a4), vA_(0), vA_(0), vA_(0), vA_(0), \
        vA_(0), vA_(0), vA_(0), vA_(0), vA_(0), vA_(0)
#define VA_PASS5(a1,a2,a3,a4,a5) \
    vA_(a1), vA_(a2), vA_(a3), vA_(a4), vA_(a5), vA_(0), vA_(0), vA_(0), \
        vA_(0), vA_(0), vA_(0), vA_(0), vA_(0), vA_(0)
#define VA_PASS6(a1,a2,a3,a4,a5,a6) \
    vA_(a1), vA_(a2), vA_(a3), vA_(a4), vA_(a5), vA_(a6), vA_(0), vA_(0), \
        vA_(0), vA_(0), vA_(0), vA_(0), vA_(0), vA_(0)
#define VA_PASS7(a1,a2,a3,a4,a5,a6,a7) \
    vA_(a1), vA_(a2), vA_(a3), vA_(a4), vA_(a5), vA_(a6), vA_(a7), vA_(0), \
        vA_(0), vA_(0), vA_(0), vA_(0), vA_(0), vA_(0)
#define VA_PASS8(a1,a2,a3,a4,a5,a6,a7,a8) \
    vA_(a1), vA_(a2), vA_(a3), vA_(a4), vA_(a5), vA_(a6), vA_(a7), vA_(a8), \
        vA_(0), vA_(0), vA_(0), vA_(0), vA_(0), vA_(0)
#define VA_PASS9(a1,a2,a3,a4,a5,a6,a7,a8,a9) \
    vA_(a1), vA_(a2), vA_(a3), vA_(a4), vA_(a5), vA_(a6), vA_(a7), vA_(a8), \
        vA_(a9), vA_(0), vA_(0), vA_(0), vA_(0), vA_(0)
#define VA_PASS10(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) \
    vA_(a1), vA_(a2), vA_(a3), vA_(a4), vA_(a5), vA_(a6), vA_(a7), vA_(a8), \
        vA_(a9), vA_(a10), vA_(0), vA_(0), vA_(0), vA_(0)
#define VA_PASS11(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11) \
    vA_(a1), vA_(a2), vA_(a3), vA_(a4), vA_(a5), vA_(a6), vA_(a7), vA_(a8), \
        vA_(a9), vA_(a10), vA_(a11), vA_(0), vA_(0), vA_(0)
#define VA_PASS12(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12) \
    vA_(a1), vA_(a2), vA_(a3), vA_(a4), vA_(a5), vA_(a6), vA_(a7), vA_(a8), \
        vA_(a9), vA_(a10), vA_(a11), vA_(a12), vA_(0), vA_(0)
#define VA_PASS14(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14) \
    vA_(a1), vA_(a2), vA_(a3), vA_(a4), vA_(a5), vA_(a6), vA_(a7), vA_(a8), \
        vA_(a9), vA_(a10), vA_(a11), vA_(a12), vA_(a13), vA_(a14)
#else /*!USE_OLDARGS*/
/* USE_STDARG and USE_VARARGS don't need to pass dummy arguments
   or cast real ones */
#define VA_PASS1(a1) a1
#define VA_PASS2(a1,a2) a1, a2
#define VA_PASS3(a1,a2,a3) a1, a2, a3
#define VA_PASS4(a1,a2,a3,a4) a1, a2, a3, a4
#define VA_PASS5(a1,a2,a3,a4,a5) a1, a2, a3, a4, a5
#define VA_PASS6(a1,a2,a3,a4,a5,a6) a1, a2, a3, a4, a5, a6
#define VA_PASS7(a1,a2,a3,a4,a5,a6,a7) a1, a2, a3, a4, a5, a6, a7
#define VA_PASS8(a1,a2,a3,a4,a5,a6,a7,a8) a1, a2, a3, a4, a5, a6, a7, a8
#define VA_PASS9(a1,a2,a3,a4,a5,a6,a7,a8,a9) a1, a2, a3, a4, a5, a6, a7, a8, a9
#define VA_PASS10(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) a1, a2, a3, a4, a5, a6, a7, a8, a9, a10
#define VA_PASS11(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11) a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11
#define VA_PASS12(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12) a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12
#define VA_PASS14(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14) \
    a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14
#endif /*?USE_OLDARGS*/
/* clang format on */
/* You were warned to avert your eyes.... */
#endif /*SPEC_LEV*/

#endif /* SP_LEV_H */
