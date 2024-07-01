/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-14 */

/* GnollHack 4.0	macpopup.h	$NHDT-Date: 1432512781 2015/05/25 00:13:01 $  $NHDT-Branch: master $:$NHDT-Revision: 1.10 $ */
/* Copyright (c) Nethack Development Team, 1999.		*/
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef MACPOPUP_H
#define MACPOPUP_H

/* ### mmodal.c ### */

extern void FlashButton(DialogRef, short);
extern char queued_resp(char *resp);
extern char topl_yn_function_ex(int style, int attr, int color, int glyph, const char* title, const char *query, const char *resp, char def, const char* resp_desc, const char* introline, uint64_t ynflags);
extern int get_line_from_key_queue(char *bufp);

#endif /* MACPOPUP_H */
