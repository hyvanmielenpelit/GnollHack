/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-14 */

/* GnollHack 4.0    dlb.c    $NHDT-Date: 1446975464 2015/11/08 09:37:44 $  $NHDT-Branch: master $:$NHDT-Revision: 1.15 $ */
/* Copyright (c) Kenneth Lorber, Bethesda, Maryland, 1993. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "config.h"
#include "dlb.h"

#ifdef __DJGPP__
#include <string.h>
#endif

#define DATAPREFIX 4

#if defined(OVERLAY)
#define STATIC_DCL extern
#define STATIC_OVL
#define STATIC_VAR
#else /* !OVERLAY */
#define STATIC_DCL static
#define STATIC_OVL static
#define STATIC_VAR static
#endif /* OVERLAY */

#ifdef DLB
/*
 * Data librarian.  Present a STDIO-like interface to GnollHack while
 * multiplexing on one or more "data libraries".  If a file is not found
 * in a given library, look for it outside the libraries.
 */

typedef struct dlb_procs {
    boolean NDECL((*dlb_init_proc));
    void NDECL((*dlb_cleanup_proc));
    boolean FDECL((*dlb_fopen_proc), (DLB_P, const char *, const char *));
    int FDECL((*dlb_fclose_proc), (DLB_P));
    long FDECL((*dlb_fread_proc), (char *, long, long, DLB_P));
    int FDECL((*dlb_fseek_proc), (DLB_P, long, int));
    char *FDECL((*dlb_fgets_proc), (char *, int, DLB_P));
    int FDECL((*dlb_fgetc_proc), (DLB_P));
    long FDECL((*dlb_ftell_proc), (DLB_P));
} dlb_procs_t;

/* without extern.h via hack.h, these haven't been declared for us */
extern FILE *FDECL(fopen_datafile, (const char *, const char *, int));

#if defined (DLBLIB) || defined (DLBMEM)
/*
 * Library Implementation:
 *
 * When initialized, we open all library files and read in their tables
 * of contents.  The library files stay open all the time.  When
 * a open is requested, the libraries' directories are searched.  If
 * successful, we return a descriptor that contains the library, file
 * size, and current file mark.  This descriptor is used for all
 * successive calls.
 *
 * The ability to open more than one library is supported but used
 * only in the Amiga port (the second library holds the sound files).
 * For Unix, the idea would be to split the GnollHack library
 * into text and binary parts, where the text version could be shared.
 */

#define MAX_LIBS 4
STATIC_VAR library dlb_libs[MAX_LIBS];

STATIC_DCL boolean FDECL(readlibdir, (library * lp));
STATIC_DCL boolean FDECL(find_file, (const char *name, library **lib,
                                     long *startp, long *sizep));

STATIC_DCL boolean NDECL(lib_dlb_init);
STATIC_DCL void NDECL(lib_dlb_cleanup);
STATIC_DCL boolean FDECL(lib_dlb_fopen, (dlb *, const char *, const char *));
STATIC_DCL int FDECL(lib_dlb_fclose, (dlb *));
STATIC_DCL long FDECL(lib_dlb_fread, (char *, long, long, dlb *));
STATIC_DCL int FDECL(lib_dlb_fseek, (dlb *, long, int));
STATIC_DCL char *FDECL(lib_dlb_fgets, (char *, int, dlb *));
STATIC_DCL int FDECL(lib_dlb_fgetc, (dlb *));
STATIC_DCL long FDECL(lib_dlb_ftell, (dlb *));

#if defined(DLBLIB)
#define dlb_scanf fscanf
#define dlb_seek fseek
#define dlb_open_datafile fopen_datafile
#define dlb_close fclose
#define dlb_read fread
#define dlb_data fdata
#define lp_dlb_data_bytes lp->fdata
#define dlb_ptr fp
#define dlb_gets fgets
#define dlb_getc fgetc
#define dlb_tell ftell
#elif defined(DLBMEM)
STATIC_DCL int FDECL(memseek, (memory_block*, long, int));
STATIC_DCL memory_block* FDECL(memopen_datafile, (const char*, const char*, int));
STATIC_DCL int FDECL(memclose, (memory_block*));
STATIC_DCL size_t FDECL(memread, (genericptr_t, size_t, size_t, memory_block*));
//STATIC_DCL int FDECL(memscanf, (memory_block*, const char*, ...)) PRINTF_F(2, 3);
STATIC_DCL char* FDECL(memgets, (char*, int, memory_block*));
STATIC_DCL int FDECL(memgetc, (memory_block*));
STATIC_DCL long FDECL(memtell, (memory_block*));

#define dlb_scanf sscanf
#define dlb_seek memseek
#define dlb_open_datafile memopen_datafile
#define dlb_close memclose
#define dlb_read memread
#define dlb_data mdata
#define lp_dlb_data_bytes lp->mdata->bytes + lp->mdata->pos
#define dlb_ptr mp
#define dlb_gets memgets
#define dlb_getc memgetc
#define dlb_tell memtell
#endif
/* not static because shared with dlb_main.c */
boolean FDECL(open_library, (const char *lib_name, library *lp));
void FDECL(close_library, (library * lp));

/* without extern.h via hack.h, these haven't been declared for us */
extern char *FDECL(eos, (char *));

/*
 * Read the directory out of the library.  Return 1 if successful,
 * 0 if it failed.
 *
 * NOTE: An improvement of the file structure should be the file
 * size as part of the directory entry or perhaps in place of the
 * offset -- the offset can be calculated by a running tally of
 * the sizes.
 *
 * Library file structure:
 *
 * HEADER:
 * %3ld library FORMAT revision (currently rev 1)
 * %1c  space
 * %8ld # of files in archive (includes 1 for directory)
 * %1c  space
 * %8ld size of allocation for string space for directory names
 * %1c  space
 * %8ld library offset - sanity check - lseek target for start of first file
 * %1c  space
 * %8ld size - sanity check - byte size of complete archive file
 *
 * followed by one DIRECTORY entry for each file in the archive, including
 *  the directory itself:
 * %1c  handling information (compression, etc.)  Always ' ' in rev 1.
 * %s   file name
 * %1c  space
 * %8ld offset in archive file of start of this file
 * %c   newline
 *
 * followed by the contents of the files
 */
#define DLB_MIN_VERS 1 /* min library version readable by this code */
#define DLB_MAX_VERS 1 /* max library version readable by this code */

/*
 * Read the directory from the library file.   This will allocate and
 * fill in our globals.  The file pointer is reset back to position
 * zero.  If any part fails, leave nothing that needs to be deallocated.
 *
 * Return TRUE on success, FALSE on failure.
 */
#if defined(DLBMEM)
STATIC_OVL int memseek (memory_block* stream, long offset, int origin)
{
    size_t base;
    switch (origin)
    {
    case SEEK_CUR:
        base = stream->pos;
        break;
    default:
    case SEEK_SET:
        base = 0;
        break;
    case SEEK_END:
        base = stream->length;
        break;
    }
    return stream->pos = offset;
}
STATIC_OVL memory_block* memopen_datafile (const char* filename, const char* mode, int prefix)
{
    FILE* fp = fopen_datafile(filename, mode, prefix);
    memory_block* mp = 0;
    if (fp)
    {
        mp = (memory_block*)alloc(sizeof(memory_block));
        memset((genericptr_t)mp, 0, sizeof(memory_block));
        if (mp)
        {
            if (fseek(fp, 0, SEEK_END) == 0)
            {
                long bytes = ftell(fp);
                if (bytes > 0)
                {
                    (void)fseek(fp, 0, SEEK_SET);
                    size_t file_size = (size_t)bytes;
                    mp->bytes = (char*)alloc(file_size);
                    if(mp->bytes)
                    {
                        mp->length = file_size;
                        memset((genericptr_t)mp->bytes, 0, file_size); /* Just in case */
                        size_t readbytes;
                        readbytes = fread((genericptr_t)mp->bytes, 1, file_size, fp);
                        fclose(fp);
                    }
                }
            }
        }
    }
    return mp;
}
STATIC_OVL int memclose (memory_block* stream)
{
    free((genericptr_t)stream->bytes);
    stream->bytes = 0;
    stream->length = 0;
    stream->pos = 0;
    free((genericptr_t)stream);
    return 0;
}
STATIC_OVL size_t memread(genericptr_t buffer, size_t size, size_t count, memory_block* stream)
{
    if (!size || !count)
        return 0;
    if (stream->pos >= stream->length)
        return 0;
    size_t mem_remaining = stream->length - stream->pos;
    size_t elements_remaining = mem_remaining / size;
    size_t read_count = min(elements_remaining, count);
    memcpy(buffer, stream->bytes + stream->pos, size * read_count);
    stream->pos += size * read_count;
    return read_count;
}

//int memscanfres = 0;
//STATIC_OVL int memscanf VA_DECL2(memory_block*, stream, const char*, format)
//{
//    VA_START(format);
//    VA_INIT(format, char*);
//    memscanfres = sscanf(stream->bytes, format, VA_ARGS);
//    VA_END();
//    return memscanfres;
//}
//
STATIC_OVL char* memgets(char* buffer, int maxcount, memory_block* stream)
{
    if (stream->pos >= stream->length)
        return buffer;

    size_t mem_remaining = stream->length - stream->pos;
    size_t read_count = min(mem_remaining, maxcount);
    memcpy(buffer, stream->bytes + stream->pos, (size_t)read_count);
    stream->pos += read_count;
    return buffer;
}

STATIC_OVL int memgetc(memory_block* stream)
{
    if (stream->pos >= stream->length)
        return 0;

    return (int)stream->bytes[stream->pos++];
}

STATIC_OVL long memtell(memory_block* stream)
{
    return stream->pos;
}

#endif


STATIC_OVL boolean
readlibdir(lp)
library *lp; /* library pointer to fill in */
{
    int i;
    char *sp;
    long liboffset;
    long totalsize;

    if (dlb_scanf(lp_dlb_data_bytes, "%ld %ld %ld %ld %ld\n", &lp->rev, &lp->nentries,
               &lp->strsize, &liboffset, &totalsize) != 5)
        return FALSE;

#ifdef DLBMEM
    {
        while(*(lp_dlb_data_bytes) != '\n')
            lp->mdata->pos++;
        lp->mdata->pos++;
    }
#endif

    if (lp->rev > DLB_MAX_VERS || lp->rev < DLB_MIN_VERS)
        return FALSE;

    lp->dir = (libdir *) alloc((size_t)lp->nentries * sizeof(libdir));
    lp->sspace = (char *) alloc((size_t)lp->strsize);

    /* read in each directory entry */
    for (i = 0, sp = lp->sspace; i < lp->nentries; i++) {
        lp->dir[i].fname = sp;
        if (dlb_scanf(lp_dlb_data_bytes, "%c%s %ld\n", &lp->dir[i].handling, sp,
                   &lp->dir[i].foffset) != 3) 
        {
            free((genericptr_t) lp->dir);
            free((genericptr_t) lp->sspace);
            lp->dir = (libdir *) 0;
            lp->sspace = (char *) 0;
            return FALSE;
        }
#ifdef DLBMEM
        {
            while (*(lp_dlb_data_bytes) != '\n')
                lp->mdata->pos++;
            lp->mdata->pos++;
        }
#endif
        sp = eos(sp) + 1;
    }

    /* calculate file sizes using offset information */
    for (i = 0; i < lp->nentries; i++) {
        if (i == lp->nentries - 1)
            lp->dir[i].fsize = totalsize - lp->dir[i].foffset;
        else
            lp->dir[i].fsize = lp->dir[i + 1].foffset - lp->dir[i].foffset;
    }

    (void)dlb_seek(lp->dlb_data, 0L, SEEK_SET); /* reset back to zero */
    lp->fmark = 0;

    return TRUE;
}

/*
 * Look for the file in our directory structure.  Return 1 if successful,
 * 0 if not found.  Fill in the size and starting position.
 */
STATIC_OVL boolean
find_file(name, lib, startp, sizep)
const char *name;
library **lib;
long *startp, *sizep;
{
    int i, j;
    library *lp;

    for (i = 0; i < MAX_LIBS && dlb_libs[i].dlb_data; i++) {
        lp = &dlb_libs[i];
        for (j = 0; j < lp->nentries; j++) {
            if (FILENAME_CMP(name, lp->dir[j].fname) == 0) {
                *lib = lp;
                *startp = lp->dir[j].foffset;
                *sizep = lp->dir[j].fsize;
                return TRUE;
            }
        }
    }
    *lib = (library *) 0;
    *startp = *sizep = 0;
    return FALSE;
}

/*
 * Open the library of the given name and fill in the given library
 * structure.  Return TRUE if successful, FALSE otherwise.
 */
boolean
open_library(lib_name, lp)
const char *lib_name;
library *lp;
{
    boolean status = FALSE;

    lp->dlb_data = dlb_open_datafile(lib_name, RDBMODE, DATAPREFIX);
    if (lp->dlb_data) {
        if (readlibdir(lp)) {
            status = TRUE;
        } else {
            (void) dlb_close(lp->dlb_data);
            lp->dlb_data = 0;
        }
    }
    return status;
}

void
close_library(lp)
library *lp;
{
    (void)dlb_close(lp->dlb_data);
    free((genericptr_t) lp->dir);
    free((genericptr_t) lp->sspace);

    (void) memset((char *) lp, 0, sizeof(library));
}

/*
 * Open the library file once using stdio.  Keep it open, but
 * keep track of the file position.
 */
STATIC_OVL boolean
lib_dlb_init(VOID_ARGS)
{
    /* zero out array */
    (void) memset((char *) &dlb_libs[0], 0, sizeof(dlb_libs));

    /* To open more than one library, add open library calls here. */
    if (!open_library(DLBFILE, &dlb_libs[0]))
        return FALSE;
#ifdef DLBFILE2
    if (!open_library(DLBFILE2, &dlb_libs[1])) {
        close_library(&dlb_libs[0]);
        return FALSE;
    }
#endif
    return TRUE;
}

STATIC_OVL void
lib_dlb_cleanup(VOID_ARGS)
{
    int i;

    /* close the data file(s) */
    for (i = 0; i < MAX_LIBS && dlb_libs[i].dlb_data; i++)
        close_library(&dlb_libs[i]);
}

/*ARGSUSED*/
STATIC_OVL boolean
lib_dlb_fopen(dp, name, mode)
dlb *dp;
const char *name;
const char *mode UNUSED;
{
    long start, size;
    library *lp;

    /* look up file in directory */
    if (find_file(name, &lp, &start, &size)) {
        dp->lib = lp;
        dp->start = start;
        dp->size = size;
        dp->mark = 0;
        return TRUE;
    }

    return FALSE; /* failed */
}

/*ARGUSED*/
STATIC_OVL int
lib_dlb_fclose(dp)
dlb *dp UNUSED;
{
    /* nothing needs to be done */
    return 0;
}

STATIC_OVL long
lib_dlb_fread(buf, size, quan, dp)
char *buf;
long size, quan;
dlb *dp;
{
    long pos, nread, nbytes;

    /* make sure we don't read into the next file */
    if ((dp->size - dp->mark) < (size * quan))
        quan = (dp->size - dp->mark) / size;
    if (quan == 0)
        return 0;

    pos = dp->start + dp->mark;
    if (dp->lib->fmark != pos) {
        int seek_error = dlb_seek(dp->lib->dlb_data, pos, SEEK_SET); /* check for error??? */
        if(!seek_error)
            dp->lib->fmark = pos;
    }

    nread = (long)dlb_read(buf, (size_t)size, (size_t)quan, dp->lib->dlb_data);
    nbytes = nread * size;
    dp->mark += nbytes;
    dp->lib->fmark += nbytes;

    return nread;
}

STATIC_OVL int
lib_dlb_fseek(dp, pos, whence)
dlb *dp;
long pos;
int whence;
{
    long curpos;

    switch (whence) {
    case SEEK_CUR:
        curpos = dp->mark + pos;
        break;
    case SEEK_END:
        curpos = dp->size - pos;
        break;
    default: /* set */
        curpos = pos;
        break;
    }
    if (curpos < 0)
        curpos = 0;
    if (curpos > dp->size)
        curpos = dp->size;

    dp->mark = curpos;
    return 0;
}

STATIC_OVL char *
lib_dlb_fgets(buf, len, dp)
char *buf;
int len;
dlb *dp;
{
    int i;
    char *bp, c = 0;

    if (len <= 0)
        return buf; /* sanity check */

    /* return NULL on EOF */
    if (dp->mark >= dp->size)
        return (char *) 0;

    len--; /* save room for null */
    for (i = 0, bp = buf; i < len && dp->mark < dp->size && c != '\n';
         i++, bp++) {
        if (dlb_fread(bp, 1, 1, dp) <= 0)
            break; /* EOF or error */
        c = *bp;
    }
    *bp = '\0';

#if defined(MSDOS) || defined(WIN32)
    if ((bp = index(buf, '\r')) != 0) {
        *bp++ = '\n';
        *bp = '\0';
    }
#endif

    return buf;
}

STATIC_OVL int
lib_dlb_fgetc(dp)
dlb *dp;
{
    char c;

    if (lib_dlb_fread(&c, 1, 1, dp) != 1)
        return EOF;
    return (int) c;
}

STATIC_OVL long
lib_dlb_ftell(dp)
dlb *dp;
{
    return dp->mark;
}

const dlb_procs_t lib_dlb_procs = { lib_dlb_init,  lib_dlb_cleanup,
                                    lib_dlb_fopen, lib_dlb_fclose,
                                    lib_dlb_fread, lib_dlb_fseek,
                                    lib_dlb_fgets, lib_dlb_fgetc,
                                    lib_dlb_ftell };
#endif /* DLBLIB */

#ifdef DLBRSRC
const dlb_procs_t rsrc_dlb_procs = { rsrc_dlb_init,  rsrc_dlb_cleanup,
                                     rsrc_dlb_fopen, rsrc_dlb_fclose,
                                     rsrc_dlb_fread, rsrc_dlb_fseek,
                                     rsrc_dlb_fgets, rsrc_dlb_fgetc,
                                     rsrc_dlb_ftell };
#endif

/* Global wrapper functions ------------------------------------------------
 */

#define do_dlb_init (*dlb_procs->dlb_init_proc)
#define do_dlb_cleanup (*dlb_procs->dlb_cleanup_proc)
#define do_dlb_fopen (*dlb_procs->dlb_fopen_proc)
#define do_dlb_fclose (*dlb_procs->dlb_fclose_proc)
#define do_dlb_fread (*dlb_procs->dlb_fread_proc)
#define do_dlb_fseek (*dlb_procs->dlb_fseek_proc)
#define do_dlb_fgets (*dlb_procs->dlb_fgets_proc)
#define do_dlb_fgetc (*dlb_procs->dlb_fgetc_proc)
#define do_dlb_ftell (*dlb_procs->dlb_ftell_proc)

STATIC_VAR const dlb_procs_t *dlb_procs;
STATIC_VAR boolean dlb_initialized = FALSE;

boolean
dlb_init()
{
    if (!dlb_initialized) {
#ifdef DLBLIB
        dlb_procs = &lib_dlb_procs;
#endif
#ifdef DLBMEM
        dlb_procs = &lib_dlb_procs;
#endif
#ifdef DLBRSRC
        dlb_procs = &rsrc_dlb_procs;
#endif

        if (dlb_procs)
            dlb_initialized = do_dlb_init();
    }

    return dlb_initialized;
}

void
dlb_cleanup()
{
    if (dlb_initialized) {
        do_dlb_cleanup();
        dlb_initialized = FALSE;
    }
}

dlb *
dlb_fopen(name, mode)
const char *name, *mode;
{
#ifdef DLBMEM
    memory_block* fp;
#else
    FILE* fp;
#endif
    dlb *dp;

    if (!dlb_initialized)
        return (dlb *) 0;

    /* only support reading; ignore possible binary flag */
    if (!mode || mode[0] != 'r')
        return (dlb *) 0;

    dp = (dlb *) alloc(sizeof(dlb));
    if (do_dlb_fopen(dp, name, mode))
        dp->dlb_ptr = 0;
    else if ((fp = dlb_open_datafile(name, mode, DATAPREFIX)) != 0)
        dp->dlb_ptr = fp;
    else {
        /* can't find anything */
        free((genericptr_t) dp);
        dp = (dlb *) 0;
    }

    return dp;
}

int
dlb_fclose(dp)
dlb *dp;
{
    int ret = 0;

    if (dlb_initialized) {
        if (dp->dlb_ptr)
            ret = dlb_close(dp->dlb_ptr);
        else
            ret = do_dlb_fclose(dp);

        free((genericptr_t) dp);
    }
    return ret;
}

long
dlb_fread(buf, size, quan, dp)
char *buf;
long size, quan;
dlb *dp;
{
    if (!dlb_initialized || size <= 0 || quan <= 0)
        return 0;
    if (dp->dlb_ptr)
        return (long) dlb_read(buf, (size_t)size, (size_t)quan, dp->dlb_ptr);
    return do_dlb_fread(buf, size, quan, dp);
}

int
dlb_fseek(dp, pos, whence)
dlb *dp;
long pos;
int whence;
{
    if (!dlb_initialized)
        return EOF;
    if (dp->dlb_ptr)
        return dlb_seek(dp->dlb_ptr, pos, whence);
    return do_dlb_fseek(dp, pos, whence);
}

char *
dlb_fgets(buf, len, dp)
char *buf;
int len;
dlb *dp;
{
    if (!dlb_initialized)
        return (char *) 0;
    if (dp->dlb_ptr)
        return dlb_gets(buf, len, dp->dlb_ptr);
    return do_dlb_fgets(buf, len, dp);
}

int
dlb_fgetc(dp)
dlb *dp;
{
    if (!dlb_initialized)
        return EOF;
    if (dp->dlb_ptr)
        return dlb_getc(dp->dlb_ptr);
    return do_dlb_fgetc(dp);
}

long
dlb_ftell(dp)
dlb *dp;
{
    if (!dlb_initialized)
        return 0;
    if (dp->dlb_ptr)
        return dlb_tell(dp->dlb_ptr);
    return do_dlb_ftell(dp);
}

#endif /* DLB */

/*dlb.c*/
