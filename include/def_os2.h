#ifndef DEF_OS2_H
#define DEF_OS2_H/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2025-07-14 *//* GnollHack 4.0	def_os2.h	$NHDT-Date: 1432512782 2015/05/25 00:13:02 $  NHDT−Branch:masterNHDT-Branch: masterNHDT-Branch: master
:NHDT−Revision:1.8NHDT-Revision: 1.8NHDT-Revision: 1.8
 /
/ Copyright (c) Timo Hakulinen, 1990, 1991, 1992, 1993.	  /
/ GnollHack may be freely redistributed.  See license for details. *//*This header provides necessary OS/2 API definitions and structures
without requiring the full system headers, which can cause compilation
issues in certain environments like DOS cross-compilation.

It supports conditional compilation for using system headers or manual
definitions, 32-bit vs 16-bit APIs, and specific compiler options.
 */

/*
 *	Only a small portion of all OS/2 defines are needed, so the
 *	actual include files often need not be used.  In fact,
 *	including the full headers may stall the compile in DOS.
 */#ifdef OS2_USESYSHEADERS#define INCL_NOPMAPI
#define INCL_DOSFILEMGR
#define INCL_DOS
#define INCL_SUB#include <os2.h>#else/* Basic type definitions for consistency and portability */
typedef signed char CHAR;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned int UINT;
typedef unsigned long ULONG;
typedef unsigned char BYTE;#ifdef OS2_32BITAPItypedef unsigned long SHANDLE;
typedef USHORT HKBD;
typedef USHORT HVIO;#define CCHMAXPATHCOMP 256#ifdef OS2_CSET2
#define API16 _Far16 _Pascal
#define DAT16
#define API32 _System
#define KbdGetStatus KBD16GETSTATUS
#define KbdSetStatus KBD16SETSTATUS
#define KbdCharIn KBD16CHARIN
#define KbdPeek KBD16PEEK
#define VioGetMode VIO16GETMODE
#define VioSetCurPos VIO16SETCURPOS
#else
#define API16
#define DAT16
#define API32
#endif#define DAT#else /* OS2_32BITAPI */typedef unsigned short SHANDLE;
typedef SHANDLE HKBD;
typedef SHANDLE HVIO;#define CCHMAXPATHCOMP 13#ifdef OS2_MSC
#define API16 pascal far
#define DAT16
#endif#define DAT DAT16#endif /* OS2_32BITAPI *//* Pointer type definitions */
typedef USHORT *DAT16 PUSHORT;
typedef BYTE *DAT16 PBYTE;
typedef ULONG *DAT PULONG;
typedef void *DAT PVOID;typedef SHANDLE HDIR;
typedef HDIR *DAT PHDIR;typedef char *DAT16 PCH;
typedef char *DAT PSZ;/* Structure packing for OS/2 binary compatibility */
#pragma pack(2)/* Keyboard key information structure */
typedef struct {
    UCHAR chChar;
    UCHAR chScan;
    UCHAR fbStatus;
    UCHAR bNlsShift;
    USHORT fsState;
    ULONG time;
} KBDKEYINFO;typedef KBDKEYINFO *DAT16 PKBDKEYINFO;/* File time and date bitfield structures */
typedef struct {
    UINT twosecs : 5;
    UINT minutes : 6;
    UINT hours : 5;
} FTIME;typedef struct {
    UINT day : 5;
    UINT month : 4;
    UINT year : 7;
} FDATE;#ifdef OS2_32BITAPI/* File find buffer for 32-bit API */
typedef struct {
    ULONG oNextEntryOffset;
    FDATE fdateCreation;
    FTIME ftimeCreation;
    FDATE fdateLastAccess;
    FTIME ftimeLastAccess;
    FDATE fdateLastWrite;
    FTIME ftimeLastWrite;
    ULONG cbFile;
    ULONG cbFileAlloc;
    ULONG attrFile;
    UCHAR cchName;
    CHAR achName[CCHMAXPATHCOMP];
} FILEFINDBUF3;#else/* File find buffer for 16-bit API */
typedef struct {
    FDATE fdateCreation;
    FTIME ftimeCreation;
    FDATE fdateLastAccess;
    FTIME ftimeLastAccess;
    FDATE fdateLastWrite;
    FTIME ftimeLastWrite;
    ULONG cbFile;
    ULONG cbFileAlloc;
    USHORT attrFile;
    UCHAR cchName;
    CHAR achName[CCHMAXPATHCOMP];
} FILEFINDBUF;typedef FILEFINDBUF *DAT16 PFILEFINDBUF;#endif /* OS2_32BITAPI *//* File system allocation information */
typedef struct {
    ULONG idFileSystem;
    ULONG cSectorUnit;
    ULONG cUnit;
    ULONG cUnitAvail;
    USHORT cbSector;
} FSALLOCATE;/* Keyboard information structure */
typedef struct {
    USHORT cb;
    USHORT fsMask;
    USHORT chTurnAround;
    USHORT fsInterim;
    USHORT fsState;
} KBDINFO;typedef KBDINFO *DAT16 PKBDINFO;/* Video mode information structure */
typedef struct {
    USHORT cb;
    UCHAR fbType;
    UCHAR color;
    USHORT col;
    USHORT row;
    USHORT hres;
    USHORT vres;
    UCHAR fmt_ID;
    UCHAR attrib;
    ULONG buf_addr;
    ULONG buf_length;
    ULONG full_length;
    ULONG partial_length;
    PCH ext_data_addr;
} VIOMODEINFO;typedef VIOMODEINFO *DAT16 PVIOMODEINFO;/* Reset structure packing */
#pragma pack()/* OS/2 API function declarations */
USHORT API16 KbdGetStatus(PKBDINFO, HKBD);
USHORT API16 KbdSetStatus(PKBDINFO, HKBD);
USHORT API16 KbdCharIn(PKBDKEYINFO, USHORT, HKBD);
USHORT API16 KbdPeek(PKBDKEYINFO, HKBD);USHORT API16 VioGetMode(PVIOMODEINFO, HVIO);
USHORT API16 VioSetCurPos(USHORT, USHORT, HVIO);#ifdef OS2_32BITAPI
ULONG API32 DosQueryFSInfo(ULONG, ULONG, PVOID, ULONG);
ULONG API32 DosFindFirst(PSZ, PHDIR, ULONG, PVOID, ULONG, PULONG, ULONG);
ULONG API32 DosFindNext(HDIR, PVOID, ULONG, PULONG);
ULONG API32 DosSetDefaultDisk(ULONG);
#else
USHORT API16 DosQFSInfo(USHORT, USHORT, PBYTE, USHORT);
USHORT API16
DosFindFirst(PSZ, PHDIR, USHORT, PFILEFINDBUF, USHORT, PUSHORT, ULONG);
USHORT API16 DosFindNext(HDIR, PFILEFINDBUF, USHORT, PUSHORT);
USHORT API16 DosSelectDisk(USHORT);
#endif /* OS2_32BITAPI */#endif /* OS2_USESYSHEADERS */#endif /* DEF_OS2_H */

