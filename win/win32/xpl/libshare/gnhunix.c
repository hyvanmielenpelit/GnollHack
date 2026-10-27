/* 
 * gnhunix.c
 * based on unixunix.c
 */

/* This file collects some Unix dependencies */

#include "hack.h"    /* mainly for index() which depends on BSD */

#include <pthread.h>
#include <errno.h>
#include <sys/stat.h>
#if defined(NO_FILE_LINKS) || defined(SUNOS4) || defined(POSIX_TYPES) || defined(O_RDONLY)
#include <fcntl.h>
#endif
#include <pwd.h>
#include <sys/types.h>
#include <dirent.h>
extern struct passwd* getpwuid(uid_t);
extern struct passwd* getpwnam(const char*);

void
regularize(char *s)    /* normalize file name - we don't like .'s, /'s, spaces */
{
    register char *lp;

    while((lp=index(s, '.')) || (lp=index(s, '/')) || (lp=index(s,' ')) || (lp=index(s,':')))
        *lp = '_';
}

#if defined(GNH_LINUX)
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>

/* flock() on "<level 0 path>.lock": the lock goes with the process, and O_CLOEXEC keeps it out
   of child processes. The file is never deleted; unlinking a lock file others may open races. */
static int game_instance_fd = -1;
static char game_instance_path[FQN_MAX_FILENAME + BUFSZ] = "";

void
release_game_instance_lock(void)
{
    if (game_instance_fd >= 0)
    {
        (void) close(game_instance_fd);
        game_instance_fd = -1;
    }
    game_instance_path[0] = '\0';
}

int
acquire_game_instance_lock(const char *fq_level0)
{
    char path[FQN_MAX_FILENAME + BUFSZ];
    int fd, err, tries;

    /* One game runs per process, so a descriptor still held belongs to a finished game */
    release_game_instance_lock();
    Sprintf(path, "%s.lock", fq_level0);
    fd = open(path, O_RDWR | O_CREAT | O_CLOEXEC, FCMASK);
    if (fd < 0)
    {
        debugprint("acquire_game_instance_lock: open failed (errno %d)", errno);
        issue_breadcrumb("acquire_game_instance_lock: unavailable");
        return GAME_INSTANCE_LOCK_UNSUPPORTED;
    }
    /* Another process's probe_game_instance_lock() holds the lock for an instant, so a
       conflict is retried twice before it counts as a running game */
    for (tries = 0; flock(fd, LOCK_EX | LOCK_NB) != 0; tries++)
    {
        err = errno;
        if (err == EWOULDBLOCK && tries < 2)
        {
            (void) usleep(20000);
            continue;
        }
        (void) close(fd);
        if (err == EWOULDBLOCK)
            return GAME_INSTANCE_LOCK_ELSEWHERE;
        debugprint("acquire_game_instance_lock: flock failed (errno %d)", err);
        issue_breadcrumb("acquire_game_instance_lock: unavailable");
        return GAME_INSTANCE_LOCK_UNSUPPORTED;
    }
    game_instance_fd = fd;
    Strcpy(game_instance_path, path);
    return GAME_INSTANCE_LOCK_ACQUIRED;
}

int
probe_game_instance_lock(const char *fq_level0)
{
    char path[FQN_MAX_FILENAME + BUFSZ];
    int fd, res;

    Sprintf(path, "%s.lock", fq_level0);
    if (game_instance_fd >= 0 && !strcmp(path, game_instance_path))
        return GAME_INSTANCE_LOCK_FREE;
    fd = open(path, O_RDONLY | O_CLOEXEC);
    if (fd < 0)
        return errno == ENOENT ? GAME_INSTANCE_LOCK_FREE : GAME_INSTANCE_LOCK_UNSUPPORTED;
    if (flock(fd, LOCK_EX | LOCK_NB) == 0)
        res = GAME_INSTANCE_LOCK_FREE; /* the close below releases it */
    else
        res = errno == EWOULDBLOCK ? GAME_INSTANCE_LOCK_ELSEWHERE : GAME_INSTANCE_LOCK_UNSUPPORTED;
    (void) close(fd);
    return res;
}

int
game_instance_lock_state(void)
{
    return game_instance_fd >= 0 ? GAME_INSTANCE_LOCK_ACQUIRED : GAME_INSTANCE_LOCK_NOT_HELD;
}
#else
/* Android and iOS run one process per app, so no other process can hold a game */
void
release_game_instance_lock(void)
{
    return;
}

int
acquire_game_instance_lock(const char *fq_level0 UNUSED)
{
    return GAME_INSTANCE_LOCK_UNSUPPORTED;
}

int
probe_game_instance_lock(const char *fq_level0 UNUSED)
{
    return GAME_INSTANCE_LOCK_UNSUPPORTED;
}

int
game_instance_lock_state(void)
{
    return GAME_INSTANCE_LOCK_UNSUPPORTED;
}
#endif /* GNH_LINUX */

static int
eraseoldlocks(void)
{
    int i, res;

    /* level 0 first, so an interrupted erase leaves no checkpoint to recover from */
    set_levelfile_name(lock, 0);
    res = unlink(fqname(lock, LEVELPREFIX, 0));

    /* cannot use maxledgerno() here, because we need to find a lock name
     * before starting everything (including the dungeon initialization
     * that sets astral_level, needed for maxledgerno()) up
     */
    for(i = 1; i <= MAXDUNGEON * MAXLEVEL + 1; i++)
    {
        /* try to remove all */
        set_levelfile_name(lock, i);
        (void) unlink(fqname(lock, LEVELPREFIX, 0));
    }
    set_levelfile_name(lock, 0);
    if (res)
        return(0);                /* cannot remove it */
    return(1);                    /* success! */
}

void
getlock(void)
{
    int fd;
    const char *fq_lock;

    if (!lock_file(HLOCK, LOCKPREFIX, 10))
    {
        wait_synch();
        fatal_error("Cannot acquire the game lock file.");
    }

    regularize(lock);
    set_levelfile_name(lock, 0);

    fq_lock = fqname(lock, LEVELPREFIX, 0);
    if (acquire_game_instance_lock(fq_lock) == GAME_INSTANCE_LOCK_ELSEWHERE)
    {
        unlock_file(HLOCK);
        refuse_game_in_progress_elsewhere(); /* does not return */
    }
    program_state.levelfiles_claimed = 1;
    if((fd = open(fq_lock, 0)) == -1)
    {
        if(errno == ENOENT)
            goto gotlock;    /* no such file */
        perror(fq_lock);
        unlock_file(HLOCK);
        fatal_error("Cannot open the game lock file %s.", fq_lock);
    }
    (void) close(fd);

    if (level0_file_lacks_checkpoint(fq_lock))
    {
        /* No checkpoint was ever written; a missing save file falls back to its backup */
        debugprint("getlock: level 0 file has no checkpoint");
        issue_breadcrumb("getlock: removed a level 0 file with no checkpoint");
        (void)restore_backup_savefile_for_name(plname, "getlock");
        goto gotlock;
    }

    if(!recover_savefile())
    {
        (void) eraseoldlocks();
        unlock_file(HLOCK);
        /* Not fatal: the backup save file below is the fallback. The reason
           is a fixed phrase, so the reporter groups these by cause. */
        nonfatal_error("Couldn't recover the old game: %s.",
                       get_recover_savefile_failure_reason());
        (void)restore_backup_savefile_for_name(plname, "getlock");
        return;
    }

gotlock:
    (void) eraseoldlocks();
    set_levelfile_name(lock, 0);
    fq_lock = fqname(lock, LEVELPREFIX, 0);
    fd = creat(fq_lock, FCMASK);
    unlock_file(HLOCK);
    if(fd == -1)
    {
        fatal_error("Cannot create the game lock file (%s).", fq_lock);
    }
    if(write(fd, (genericptr_t) &hackpid, sizeof(hackpid)) != sizeof(hackpid))
    {
        fatal_error("Cannot write the game lock file (%s).", fq_lock);
    }
    if(close(fd) == -1)
    {
        nonfatal_error("Cannot close the game lock file (%s).", fq_lock);
    }
}

int
dosh(void)
{
    return 0;
}

void
check_crash(void)
{
    if (!*recovery_plname)
        return;

    //list_files();

    char recover_lock[BUFSIZ] = "";
    if (!lock_file(HLOCK, LOCKPREFIX, 10))
    {
        wait_synch();
        silent_nonfatal_error("Cannot acquire the lock file for the crash check.");
        return;
    }

    if (!wizard)
        Sprintf(recover_lock, "%d%s", (int)getuid(), recovery_plname);

    int fd;
    const char* fq_lock;

    regularize(recover_lock);
    set_levelfile_name(recover_lock, 0);

    fq_lock = fqname(recover_lock, LEVELPREFIX, 0);
    if (probe_game_instance_lock(fq_lock) == GAME_INSTANCE_LOCK_ELSEWHERE)
    {
        debugprint("check_crash: game in progress in another process");
        issue_breadcrumb("check_crash: game in progress in another process");
        goto nofilefound;
    }
    if ((fd = open(fq_lock, 0)) == -1)
    {
        if (errno == ENOENT)
            goto nofilefound;    /* no such file */
        perror(fq_lock);
        unlock_file(HLOCK);
        silent_nonfatal_error("Cannot open the crash-check lock file %s.", fq_lock);
        return;
    }
    (void)close(fd);

    if (level0_file_lacks_checkpoint(fq_lock))
    {
        /* Nothing to recover; a missing save file falls back to its backup, so the game is listed */
        if (!wizard)
            (void)restore_backup_savefile_for_name(recovery_plname, "check_crash");
        goto nofilefound;
    }

    struct special_view_info info = { 0 };
    info.viewtype = SPECIAL_VIEW_CRASH_DETECTED;
    (void)open_special_view(info);

    /* Use recover_plname instead */
    //if(!wizard)
    //    Strcpy(plname, recovery_plname);

nofilefound:
    unlock_file(HLOCK);
}

int lock_init_result = -1;
pthread_mutex_t threadlock = { 0 };

void
thread_lock_init(void)
{
    if (!lock_init_result)
        thread_lock_destroy();
    lock_init_result = pthread_mutex_init(&threadlock, NULL);
}

void
thread_lock_destroy(void)
{
    if (!lock_init_result)
    {
        (void)pthread_mutex_destroy(&threadlock);
        lock_init_result = -1;
    }
}

void
thread_lock_lock(void)
{
    if (!lock_init_result)
    {
        (void)pthread_mutex_lock(&threadlock);
    }
}

void
thread_lock_unlock(void)
{
    if (!lock_init_result)
    {
        (void)pthread_mutex_unlock(&threadlock);
    }
}

void
gnollhack_exit(int code)
{
    /* Before exit_hack(), which may start the next game on a new thread */
    release_game_instance_lock();
    if (exit_hack)
        exit_hack(exit_hack_code);

#if defined(EXIT_THREAD_ON_EXIT)
    /* Ends only the calling thread; the frontend starts the next game on a new thread */
    static char retbuf[BUFSZ];
    Sprintf(retbuf, "GnollHack thread exit with value %d", code);

    pthread_exit(retbuf);
#else
    exit(code);
#endif
}

#if defined (NOCWD_ASSUMPTIONS)
/*
 * Add a slash to any name not ending in /. There must
 * be room for the /
 */
void
append_slash(char *name)
{
    char* ptr;

    if (!*name)
        return;
    ptr = name + (strlen(name) - 1);
    if (*ptr != '/')
    {
        *++ptr = '/';
        *++ptr = '\0';
    }
    return;
}
#endif

void
gnh_umask(void)
{
    /*
     * FCMASK (0664) describes the permissions we want on created *files*, and
     * deriving the umask from it alone also masks off the execute bit.  That
     * makes every directory created afterwards non-traversable, so nothing
     * inside it can be opened -- not by other users, and not by us either.
     * It broke our own mkdir() calls, and it broke the temporary staging
     * directories Apple's NSItemProvider creates on our threads, aborting the
     * process from _NSIPCloneURLToTemporaryFolder (Sentry GNOLLHACK-G6/GE).
     * OR in 0111 so directories keep their search permission.  File modes are
     * unchanged: nothing here is created with an execute bit in the first
     * place, so 0664 stays 0664.
     */
    (void)umask(0777 & ~(FCMASK | 0111));
}

uint64_t
sys_random_seed(void)
{
    uint64_t seed = 0L;
    uint64_t pid = (uint64_t)getpid();
    boolean no_seed = TRUE;
#ifdef DEV_RANDOM
    FILE* fptr;

    fptr = fopen(DEV_RANDOM, "r");
    if (fptr) 
    {
        IGNORE_RESULT fread(&seed, sizeof(int64_t), 1, fptr);
        has_strong_rngseed = TRUE;  /* decl.c */
        no_seed = FALSE;
        (void)fclose(fptr);
    }
    else {
        /* leaves clue, doesn't exit */
        paniclog("sys_random_seed", "falling back to weak seed");
    }
#endif
    if (no_seed) 
    {
        seed = (uint64_t)getnow(); /* time((TIME_type) 0) */
        /* Quick dirty band-aid to prevent PRNG prediction */
        if (pid) 
        {
            if (!(pid & 3L))
                pid -= 1L;
            seed *= pid;
        }
    }
    return seed;
}
