/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    func_tab.h    $NHDT-Date: 1543797823 2018/12/03 00:43:43 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.11 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Pasi Kallinen, 2016. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef FUNC_TAB_H
#define FUNC_TAB_H

/* extended command flags */
#define IFBURIED                    0x0001 /* can do command when buried */
#define AUTOCOMPLETE                0x0002 /* command autocompletes */
#define WIZMODECMD                  0x0004 /* wizard-mode command */
#define GENERALCMD                  0x0008 /* general command, does not take game time */
#define CMD_NOT_AVAILABLE           0x0010 /* recognized but non-functional (!SHELL,&c) */
#define INCMDMENU                   0x0020 /* Displayed in command menu under M('c') */
#define INSPELLMENU                 0x0040 /* Displayed in spell menu under '+' */
#define SINGLE_OBJ_CMD_GENERAL      0x0080 /* Displayed in inventory for single objects under general commands */
#define SINGLE_OBJ_CMD_SPECIFIC     0x0100 /* Displayed in inventory for single objects under specific commands */
#define SINGLE_OBJ_CMD_INFO         0x0200 /* Displayed in inventory for single objects under info commands */
#define CASUALMODECMD               0x0400 /* casual/explore/wizard-mode command */
#define ALLOW_RETURN_TO_INVENTORY   0x0800 /* if invoked from inventory, return to inventory when done */
#define ALLOW_RETURN_TO_CMD_MENU    0x1000 /* if invoked from inventory, return to cmd menu when done */
#define CMD_MENU_AUTO_CLICK_OK      0x2000 /* adds auto-click-ok to the respective command menu item */

struct ext_func_tab {
    uchar key;
    const char *ef_txt, *ef_desc;
    int NDECL((*ef_funct));
    uint64_t flags;
    const char *f_text;
    const char* getobj_classes;
    const char* getobj_word;
    const char* ef_txt_word;
    uchar bound_key; /* Last key binding for the function */
};

extern struct ext_func_tab extcmdlist[];

#endif /* FUNC_TAB_H */
