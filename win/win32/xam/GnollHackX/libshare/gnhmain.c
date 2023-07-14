/* 
 * gnhmain.c
 */

#include "hack.h"
#include "libproc.h"

#include "dlb.h"
#include <sys/stat.h>
#include <pwd.h>
#include <sys/types.h>
#include <dirent.h>
#ifndef O_RDONLY
#include <fcntl.h>
#endif

#include <pthread.h>

extern struct passwd *FDECL( getpwuid, ( uid_t));
extern struct passwd *FDECL( getpwnam, (const char *));

#if 0
STATIC_DCL boolean NDECL( whoami);
#endif
STATIC_DCL void FDECL( process_command_line_arguments, (int, char **));
STATIC_DCL void NDECL(make_dumplog_dir);

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

void
gnollhack_exit(code)
int code;
{
    if (exit_hack)
        exit_hack(code);

#if defined(EXIT_THREAD_ON_EXIT)
    char retbuf[BUFSZ];
    Sprintf(retbuf, "GnollHack thread exit with value %d", code);

    pthread_exit(retbuf);
#else
    exit(code);
#endif
}

int
GnollHackMain(argc, argv)
int argc;
char** argv;
{
    FILE* fp;
    boolean resuming = FALSE; /* assume new game */

    sys_early_init();
    lib_init_platform();

    hname = argv[0];
    hackpid = getpid();
    (void)umask(0777 & ~FCMASK);

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
    init_nhwindows(&argc, argv);
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

    if (!load_saved_game(0))
    {
        player_selection();
        resuming = FALSE;

        /* CHOOSE DIFFICULTY */
        choose_game_difficulty();

        newgame();
        mode_message();
    }
    else
    {
        resuming = TRUE;
    }

    if(wizard)
        issue_simple_gui_command(GUI_CMD_ENABLE_WIZARD_MODE); /* Notification may be needed if loaded a wizard mode saved game */
    else
        issue_simple_gui_command(GUI_CMD_DISABLE_WIZARD_MODE); /* Notification may be needed */

    if (CasualMode)
        issue_simple_gui_command(GUI_CMD_ENABLE_CASUAL_MODE); /* Notification may be needed if loaded a casual mode saved game */
    else
        issue_simple_gui_command(GUI_CMD_DISABLE_CASUAL_MODE); /* Notification may be needed */

    moveloop(resuming);
    gnollhack_exit(EXIT_SUCCESS);
    return (0);
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
#ifdef NEWS
            case 'n':
            iflags.news = FALSE;
            break;
#endif
        case 'u':
            if(!*plname)
            {
                if(argv[0][2])
                    (void)strncpy(plname, argv[0] + 2, sizeof(plname) - 1);
                else if(argc > 1)
                {
                    argc--;
                    argv++;
                    (void)strncpy(plname, argv[0], sizeof(plname) - 1);
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
        (void)strncpy(plname, s, sizeof(plname) - 1);
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

#ifdef NOCWD_ASSUMPTIONS
/*
 * Add a slash to any name not ending in /. There must
 * be room for the /
 */
void 
append_slash(name)
char *name;
{
    char *ptr;

    if(!*name)
        return;
    ptr = name + (strlen(name) - 1);
    if(*ptr != '/')
    {
        *++ptr = '/';
        *++ptr = '\0';
    }
    return;
}
#endif

unsigned long
sys_random_seed()
{
    unsigned long seed = 0L;
    unsigned long pid = (unsigned long) getpid();
    boolean no_seed = TRUE;
#ifdef DEV_RANDOM
    FILE *fptr;

    fptr = fopen(DEV_RANDOM, "r");
    if (fptr) {
        fread(&seed, sizeof (long), 1, fptr);
        has_strong_rngseed = TRUE;  /* decl.c */
        no_seed = FALSE;
        (void) fclose(fptr);
    } else {
        /* leaves clue, doesn't exit */
        paniclog("sys_random_seed", "falling back to weak seed");
    }
#endif
    if (no_seed) {
        seed = (unsigned long) getnow(); /* time((TIME_type) 0) */
        /* Quick dirty band-aid to prevent PRNG prediction */
        if (pid) {
            if (!(pid & 3L))
                pid -= 1L;
            seed *= pid;
        }
    }
    return seed;
}

STATIC_OVL void
make_dumplog_dir(VOID_ARGS)
{
#if defined(DUMPLOG) && defined(DUMPLOG_DIR)
    /* Make DUMPLOG_DIR if defined */
    struct stat st = { 0 };

    if (stat(DUMPLOG_DIR, &st) == -1) {
        (void)mkdir(DUMPLOG_DIR, 0700);
    }
#endif
}
