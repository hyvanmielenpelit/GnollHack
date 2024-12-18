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
extern struct passwd* FDECL(getpwuid, (uid_t));
extern struct passwd* FDECL(getpwnam, (const char*));

void
regularize(s)    /* normalize file name - we don't like .'s, /'s, spaces */
char *s;
{
    register char *lp;

    while((lp=index(s, '.')) || (lp=index(s, '/')) || (lp=index(s,' ')) || (lp=index(s,':')))
        *lp = '_';
}

static int
eraseoldlocks()
{
    register int i;

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
    if (unlink(fqname(lock, LEVELPREFIX, 0)))
        return(0);                /* cannot remove it */
    return(1);                    /* success! */
}

void
getlock()
{
    register int fd;
    const char *fq_lock;

    if (!lock_file(HLOCK, LOCKPREFIX, 10))
    {
        wait_synch();
        error("%s", "");
        return;
    }

    regularize(lock);
    set_levelfile_name(lock, 0);

    fq_lock = fqname(lock, LEVELPREFIX, 0);
    if((fd = open(fq_lock, 0)) == -1)
    {
        if(errno == ENOENT)
            goto gotlock;    /* no such file */
        perror(fq_lock);
        unlock_file(HLOCK);
        error("Cannot open %s", fq_lock);
        return;
    }
    (void) close(fd);

    if(!recover_savefile())
    {
        (void) eraseoldlocks();
        unlock_file(HLOCK);
        error("Couldn't recover old game.");
        (void)restore_backup_savefile(FALSE);
        return;
    }

gotlock:
    (void) eraseoldlocks();
    fd = creat(fq_lock, FCMASK);
    unlock_file(HLOCK);
    if(fd == -1)
    {
        error("cannot creat lock file (%s).", fq_lock);
    }
    else
    {
        if(write(fd, (genericptr_t) &hackpid, sizeof(hackpid)) != sizeof(hackpid))
        {
            error("cannot write lock (%s)", fq_lock);
        }
        if(close(fd) == -1)
        {
            error("cannot close lock (%s)", fq_lock);
        }
    }
}

int
dosh()
{
    return 0;
}

void
check_crash()
{
    if (!*recovery_plname)
        return;

    //list_files();

    char recover_lock[BUFSIZ] = "";
    if (!lock_file(HLOCK, LOCKPREFIX, 10))
    {
        wait_synch();
        error("%s", "");
        return;
    }

    if (!wizard)
        Sprintf(recover_lock, "%d%s", (int)getuid(), recovery_plname);

    register int fd;
    const char* fq_lock;

    regularize(recover_lock);
    set_levelfile_name(recover_lock, 0);

    fq_lock = fqname(recover_lock, LEVELPREFIX, 0);
    if ((fd = open(fq_lock, 0)) == -1)
    {
        if (errno == ENOENT)
            goto nofilefound;    /* no such file */
        perror(fq_lock);
        unlock_file(HLOCK);
        error("Cannot open %s", fq_lock);
        return;
    }
    (void)close(fd);

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
thread_lock_init(VOID_ARGS)
{
    if (!lock_init_result)
        thread_lock_destroy();
    lock_init_result = pthread_mutex_init(&threadlock, NULL);
}

void
thread_lock_destroy(VOID_ARGS)
{
    if (!lock_init_result)
    {
        (void)pthread_mutex_destroy(&threadlock);
        lock_init_result = -1;
    }
}

void
thread_lock_lock(VOID_ARGS)
{
    if (!lock_init_result)
    {
        (void)pthread_mutex_lock(&threadlock);
    }
}

void
thread_lock_unlock(VOID_ARGS)
{
    if (!lock_init_result)
    {
        (void)pthread_mutex_unlock(&threadlock);
    }
}

void
gnollhack_exit(code)
int code;
{
    if (exit_hack)
        exit_hack(exit_hack_code);

#if defined(EXIT_THREAD_ON_EXIT)
    char retbuf[BUFSZ];
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
append_slash(name)
char* name;
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
gnh_umask(VOID_ARGS)
{
    (void)umask(0777 & ~FCMASK);
}

uint64_t
sys_random_seed()
{
    uint64_t seed = 0L;
    uint64_t pid = (uint64_t)getpid();
    boolean no_seed = TRUE;
#ifdef DEV_RANDOM
    FILE* fptr;

    fptr = fopen(DEV_RANDOM, "r");
    if (fptr) 
    {
        fread(&seed, sizeof(int64_t), 1, fptr);
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
