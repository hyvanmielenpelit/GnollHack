/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2021-09-14 */

/* androidmain.c
 * based on unixmain.c
 */

#include "hack.h"
#include "dlb.h"
#include <setjmp.h>

#include <sys/stat.h>
#include <pwd.h>
#ifndef O_RDONLY
#include <fcntl.h>
#endif

extern void set_username();

static jmp_buf env;

extern struct passwd *FDECL( getpwuid, ( uid_t));
extern struct passwd *FDECL( getpwnam, (const char *));

static boolean NDECL( whoami);
static void FDECL( process_command_line_arguments, (int, char **));

static void NDECL( wd_message);

static char *make_lockname(filename, lockname)
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

void remove_lock_file(const char *filename)
{
	char locknambuf[BUFSZ];
	const char *lockname;

	lockname = make_lockname(filename, locknambuf);
	unlink(lockname);
}

void gnollhack_exit(int code)
{
	if (exit_hack)
		exit_hack(code);

	longjmp(env, code);
}

int GnollHackMain(int argc, char** argv)
{
	debuglog("Starting GnollHack!");

	int val;

	val = setjmp(env);
	if(val)
	{
		debuglog("exiting...");
		return 0;
	}


	register int fd;
	boolean exact_username;
	FILE* fp;

    boolean resuming = FALSE; /* assume new game */

    sys_early_init();

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

	choose_windows(DEFAULT_WINDOW_SYS);

	read_options();

	init_nhwindows(&argc, argv);
	//exact_username = whoami();

	/*
	 * It seems you really want to play.
	 */
	u.uhp = 1; /* prevent RIP on early quits */

	process_command_line_arguments(argc, argv); /* command line options */

#ifdef DEF_PAGER
	if(!(catmore = nh_getenv("HACKPAGER")) && !(catmore = nh_getenv("PAGER")))
	catmore = DEF_PAGER;
#endif

//#ifdef MAIL
//	getmailstatus();
//#endif

	display_gamewindows();
	plnamesuffix(); /* strip suffix from name; calls askname() */
					/* again if suffix was whole name */
					/* accepts any suffix */
#ifdef WIZARD
	if(!wizard)
#endif
	set_username();

	Sprintf(lock, "%d%s", (int)getuid(), plname);
	getlock();

	/* Set up level 0 file to keep the game state.
	 */
	fd = create_levelfile(0, (char *)0);
	if(fd < 0)
	{
		raw_print("Cannot create lock file");
	}
	else
	{
		hackpid = 1;
		write(fd, (genericptr_t) & hackpid, sizeof(hackpid));
		close(fd);
	}

	dlb_init(); /* must be before newgame() */

	/*
	 * Initialization of the boundaries of the mazes
	 * Both boundaries have to be even.
	 */
	x_maze_max = COLNO - 1;
	if(x_maze_max % 2)
		x_maze_max--;
	y_maze_max = ROWNO - 1;
	if(y_maze_max % 2)
		y_maze_max--;

	/*
	 *  Initialize the vision system.  This must be before mklev() on a
	 *  new game or before a level restore on a saved game.
	 */
	vision_init();

	if((fd = restore_saved_game()) >= 0)
	{
#ifdef WIZARD
		/* Since wizard is actually flags.debug, restoring might
		 * overwrite it.
		 */
		boolean remember_wiz_mode = wizard;
#endif
		const char *fq_save = fqname(SAVEF, SAVEPREFIX, 1);

#ifdef NEWS
		if(iflags.news)
		{
			display_file(NEWS, FALSE);
			iflags.news = FALSE; /* in case dorecover() fails */
		}
#endif
		pline("Restoring save file...");
		mark_synch(); /* flush output */
		if(!dorecover(fd))
			goto not_recovered;
		resuming = TRUE;
#ifdef WIZARD
		if(!wizard && remember_wiz_mode)
			wizard = TRUE;
#endif
		check_special_room(FALSE);
		wd_message();

		if(discover || wizard)
		{
			if(yn_query("Do you want to keep the save file?") == 'n')
			{
				(void)delete_savefile();
			}
			else
			{
				nh_compress(fq_save);
			}
		}

		encounter_init();
	}
	else
	{
		not_recovered: player_selection();
		resuming = FALSE;

		/* CHOOSE DIFFICULTY */
		choose_game_difficulty();

		newgame();
		wd_message();
	}

	moveloop(resuming);
    exit(EXIT_SUCCESS);

	return (0);
}

boolean authorize_wizard_mode()
{
	return TRUE;
}


static void process_command_line_arguments(argc, argv)
	int argc;char *argv[];
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
#ifdef WIZARD
			wizard = TRUE;
			break;
#endif
		/* otherwise fall thru to discover */
		case 'X':
			discover = TRUE;
			break;
		case 'M':
			ModernMode = TRUE;
			break;
		case 'C':
			CasualMode = TRUE;
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

static boolean whoami()
{
	/*
	 * Who am i? Algorithm: 1. Use name as specified in NETHACKOPTIONS
	 *			2. Use getlogin()		(if 1. fails)
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

static void wd_message()
{
	if(discover)
		You("are in non-scoring discovery mode.");
}

/*
 * Add a slash to any name not ending in /. There must
 * be room for the /
 */
void append_slash(name)
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
