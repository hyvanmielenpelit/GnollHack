/* 
 * gnhmain.c
 */

#include "hack.h"
#include "libproc.h"
#include "dlb.h"
extern void NDECL(gnh_umask);

STATIC_DCL void FDECL(process_command_line_arguments, (int, char **));
STATIC_DCL void NDECL(notify_gui_pregame);

STATIC_OVL char*
make_lockname(filename, lockname)
const char *filename;
char *lockname;
{
#  ifdef NO_FILE_LINKS
    Strcpy(lockname, LOCKDIR);
    Strcat(lockname, "/");
    Strcat(lockname, filename);
#  else
    Strcpy(lockname, filename);
#  endif
    Strcat(lockname, "_lock");
    return lockname;
}

STATIC_OVL void
remove_lock_file(filename)
const char* filename;
{
    char locknambuf[BUFSZ];
    const char *lockname;

    lockname = make_lockname(filename, locknambuf);
    unlink(lockname);
}

int
GnollHackMain(argc, argv)
int argc;
char** argv;
{
    FILE* fp;
    uchar resuming = FALSE; /* assume new game */
    int exit_hack_code_at_start = exit_hack_code; /* if 1, then the game is restarting after saving; sys_early_init will set to zero */

    sys_early_init();
    lib_init_platform();

    hname = argv[0];
    hackpid = getpid();

    gnh_umask();

    // hack
    // remove dangling locks
    remove_lock_file(RECORD);
    remove_lock_file(HLOCK);
    // make sure RECORD exists
    fp = fopen_datafile(RECORD, "a", SCOREPREFIX);
    fclose(fp);

    check_recordfile((char*)0);

    /* Now initialize windows */
    choose_windows(DEFAULT_WINDOW_SYS);
    maybe_issue_simple_gui_command(!exit_hack_code_at_start, GUI_CMD_LOAD_GLYPHS);
    init_nhwindows(&argc, argv);
    maybe_issue_simple_gui_command(!exit_hack_code_at_start, GUI_CMD_SET_TO_BLACK);
    process_options_file();

    /*
     * It seems you really want to play.
     */
    u.uhp = 1; /* prevent RIP on early quits */

    process_command_line_arguments(argc, argv); /* command line options */
    make_dumplog_dir();

#ifdef DEF_PAGER
    if(!(catmore = nh_getenv("HACKPAGER")) && !(catmore = nh_getenv("PAGER")))
    catmore = DEF_PAGER;
#endif

//#ifdef MAIL
//    getmailstatus();
//#endif
    display_gamewindows();
    check_crash();
    plnamesuffix(); /* strip suffix from name; calls askname() */
                    /* again if suffix was whole name */
                    /* accepts any suffix */

    set_playmode(); /* sets plname to "wizard" for wizard mode */

    Sprintf(lock, "%d%s", (int)getuid(), plname);
    getlock();

    create_gamestate_levelfile();

    dlb_init(); /* must be before newgame() */

    /*
     * Initialization of the boundaries of the mazes
     * Both boundaries have to be even.
     */
    maze_init();

    /*
     *  Initialize the vision system.  This must be before mklev() on a
     *  new game or before a level restore on a saved game.
     */
    vision_init();

    issue_simple_gui_command(GUI_CMD_GAME_START);

    if (!load_saved_game(exit_hack_code_at_start))
    {
        resuming = FALSE;
        maybe_issue_simple_gui_command(exit_hack_code_at_start, GUI_CMD_SET_TO_BLACK);
        player_selection();

        /* CHOOSE DIFFICULTY */
        choose_game_difficulty();

        newgame();
        mode_message();
        set_mouse_buttons();
    }
    else
    {
        resuming = exit_hack_code_at_start == 1 ? 2 : TRUE;
    }
    notify_gui_pregame();
    moveloop(resuming);
    gnollhack_exit(EXIT_SUCCESS);
    return (0);
}

STATIC_OVL void
notify_gui_pregame(VOID_ARGS)
{
    issue_simple_gui_command(wizard ? GUI_CMD_ENABLE_WIZARD_MODE : GUI_CMD_DISABLE_WIZARD_MODE); /* Notification may be needed if loaded a wizard mode saved game */
    issue_simple_gui_command(CasualMode ? GUI_CMD_ENABLE_CASUAL_MODE : GUI_CMD_DISABLE_CASUAL_MODE); /* Notification may be needed if loaded a casual mode saved game */
    issue_simple_gui_command(TournamentMode ? GUI_CMD_ENABLE_TOURNAMENT_MODE : GUI_CMD_DISABLE_TOURNAMENT_MODE); /* Notification may be needed if loaded a tournament mode saved game */
    issue_boolean_gui_command(GUI_CMD_TOGGLE_CHARACTER_CLICK_ACTION, flags.self_click_action); /* Notification is needed */
    issue_boolean_gui_command(GUI_CMD_TOGGLE_AUTODIG, flags.autodig); /* Notification is needed */
    issue_boolean_gui_command(GUI_CMD_TOGGLE_IGNORE_STOPPING, flags.ignore_stopping); /* Notification is needed */
    issue_boolean_gui_command(GUI_CMD_TOGGLE_DICE_AS_RANGES, iflags.show_dice_as_ranges); /* Since this is an iflag, notification is really not needed but good to make sure that things align and for debug purposes */
    issue_boolean_gui_command(GUI_CMD_TOGGLE_GETPOS_ARROWS, iflags.getpos_arrows); /* Since this is an iflag, notification is really not needed but good to make sure that things align and for debug purposes */
    issue_gui_command(GUI_CMD_REPORT_MOUSE_COMMAND, (int)flags.right_click_command, 0, (const char*)0); /* Notification is needed */
    issue_gui_command(GUI_CMD_REPORT_MOUSE_COMMAND, (int)flags.middle_click_command, 1, (const char*)0); /* Notification is needed */
    
    if (context.quick_cast_spell_set)
    {
        const char* spellnam = spl_book[context.quick_cast_spell_no].sp_id > STRANGE_OBJECT ? OBJ_NAME(objects[spl_book[context.quick_cast_spell_no].sp_id]) : "";
        issue_gui_command(GUI_CMD_TOGGLE_QUICK_CAST_SPELL, spell_to_glyph(context.quick_cast_spell_no), spellid(context.quick_cast_spell_no), spellnam); /* Notification is needed */
    }
    else
        issue_gui_command(GUI_CMD_TOGGLE_QUICK_CAST_SPELL, NO_GLYPH, 0, "");

    struct obj* obj;
    if (context.quick_zap_wand_oid > 0 && (obj = o_on(context.quick_zap_wand_oid, invent)) != 0)
        issue_gui_command(GUI_CMD_TOGGLE_QUICK_ZAP_WAND, (int)obj_to_glyph(obj, rn2_on_display_rng), Hallucination ? 0 : (int)obj->exceptionality, cxname(obj)); /* Notification is needed */
    else
        issue_gui_command(GUI_CMD_TOGGLE_QUICK_ZAP_WAND, NO_GLYPH, 0, "");
}

boolean 
authorize_wizard_mode(VOID_ARGS)
{
    return TRUE;
}

STATIC_OVL void 
process_command_line_arguments(argc, argv)
int argc;
char *argv[];
{
    int i;

    /*
     * Process options.
     */
    while(argc > 1 && argv[1][0] == '-')
    {
        argv++;
        argc--;
        switch(argv[0][1])
        {
        case 'D':
            wizard = TRUE, discover = FALSE;
            break;
        /* otherwise fall thru to discover */
        case 'X':
            discover = TRUE, wizard = FALSE;
            break;
        case 'M':
            ModernMode = TRUE;
            break;
        case 'C':
            CasualMode = TRUE;
            break;
        case 'b':
            flags.bones = FALSE;
            break;
        case 'B':
            flags.bones = TRUE;
            break;
        case 'P': /* Petless */
            preferred_pet = 'n';
            flags.no_pets_preference = TRUE;
            break;
        case 'T':
            TournamentMode = TRUE;
            break;
#ifdef NEWS
            case 'n':
            iflags.news = FALSE;
            break;
#endif
        case 'u':
            if(!*plname)
            {
                if(argv[0][2])
                    Strncpy(plname, argv[0] + 2, sizeof(plname) - 1);
                else if(argc > 1)
                {
                    argc--;
                    argv++;
                    Strncpy(plname, argv[0], sizeof(plname) - 1);
                }
                else
                    raw_print("Player name expected after -u");
            }
            break;
        case 'p': /* profession (role) */
            if(argv[0][2])
            {
                if((i = str2role(&argv[0][2])) >= 0)
                    flags.initrole = i;
            }
            else if(argc > 1)
            {
                argc--;
                argv++;
                if((i = str2role(argv[0])) >= 0)
                    flags.initrole = i;
            }
            break;
        case 'r': /* race */
            if(argv[0][2])
            {
                if((i = str2race(&argv[0][2])) >= 0)
                    flags.initrace = i;
            }
            else if(argc > 1)
            {
                argc--;
                argv++;
                if((i = str2race(argv[0])) >= 0)
                    flags.initrace = i;
            }
            break;
        case '@':
            flags.randomall = 1;
            break;
        default:
            if((i = str2role(&argv[0][1])) >= 0)
            {
                flags.initrole = i;
                break;
            }
            /* else raw_printf("Unknown option: %s", *argv); */
        }
    }
}

#if 0
STATIC_OVL boolean
whoami()
{
    /*
     * Who am i? Algorithm: 1. Use name as specified in NETHACKOPTIONS
     *            2. Use getlogin()        (if 1. fails)
     * The resulting name is overridden by command line options.
     * If everything fails, or if the resulting name is some generic
     * account like "games", "play", "player", "hack" then eventually
     * we'll ask him.
     * Note that we trust the user here; it is possible to play under
     * somebody else's name.
     */
    register char *s;

    if(*plname)
        return FALSE;
    if((s = getlogin()))
        Strncpy(plname, s, sizeof(plname) - 1);
    return TRUE;
}
#endif

#ifdef PORT_HELP
void
port_help()
{
    /*
     * Display unix-specific help.   Just show contents of the helpfile
     * named by PORT_HELP.
     */
    display_file(PORT_HELP, TRUE);
}
#endif
