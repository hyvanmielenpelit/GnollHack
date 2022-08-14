/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-14 */

#include <string.h>
#include <errno.h>
#include <jni.h>
#include <ctype.h>

#include "hack.h"
#include "func_tab.h"   /* for extended commands */
#include "dlb.h"

static void FDECL(and_init_nhwindows, (int *, char **));
static void NDECL(and_player_selection);
static void NDECL(and_askname);
static void NDECL(and_get_nh_event) ;
static void FDECL(and_exit_nhwindows, (const char *));
static void FDECL(and_suspend_nhwindows, (const char *));
static void NDECL(and_resume_nhwindows);
static winid FDECL(and_create_nhwindow_ex, (int, int, int, struct extended_create_window_info));
static void FDECL(and_clear_nhwindow, (winid));
static void FDECL(and_display_nhwindow, (winid, BOOLEAN_P));
static void FDECL(and_dismiss_nhwindow, (winid));
static void FDECL(and_destroy_nhwindow, (winid));
static void FDECL(and_curs, (winid,int,int));
static void FDECL(and_putstr_ex, (winid, int, const char *, int, int));
static void FDECL(and_putmixed_ex, (winid, int, const char *, int, int));
static void FDECL(and_display_file, (const char *, BOOLEAN_P));
static void FDECL(and_start_menu_ex, (winid, int));
static void FDECL(and_add_menu, (winid,int,const ANY_P *, CHAR_P,CHAR_P,int,const char *, BOOLEAN_P));
static void FDECL(and_add_extended_menu, (winid, int, const ANY_P*, struct extended_menu_info, CHAR_P, CHAR_P, int, const char*, BOOLEAN_P));
static void FDECL(and_end_menu_ex, (winid, const char *, const char*));
static int FDECL(and_select_menu, (winid, int, MENU_ITEM_P **));
static char FDECL(and_message_menu, (CHAR_P, int, const char *));
static void NDECL(and_update_inventory);
static void NDECL(and_mark_synch);
static void NDECL(and_wait_synch);
#ifdef CLIPPING
static void FDECL(and_cliparound, (int, int, BOOLEAN_P));
#endif
#ifdef POSITIONBAR
static void FDECL(and_update_positionbar, (char *));
#endif
static void FDECL(and_print_glyph, (winid,XCHAR_P,XCHAR_P,struct layer_info));
static void FDECL(and_issue_gui_command, (int));
static void FDECL(and_raw_print, (const char *));
static void FDECL(and_raw_print_bold, (const char *));
static int NDECL(and_nhgetch);
static int FDECL(and_nh_poskey, (int *, int *, int *));
static void NDECL(and_nhbell);
static int NDECL(and_doprev_message);
static char FDECL(and_yn_function_ex, (int, int, int, int, const char *, const char *, const char *, CHAR_P, const char*, const char*, unsigned long));
static void FDECL(and_getlin_ex, (int, int, int, const char *,char *, const char*, const char*, const char*));
static int NDECL(and_get_ext_cmd);
static void FDECL(and_number_pad, (int));
static void NDECL(and_delay_output);
static void FDECL(and_delay_output_milliseconds, (int));
static void FDECL(and_delay_output_intervals, (int));
#ifdef CHANGE_COLOR
static void FDECL(and_change_color,(int color,long rgb,int reverse));
static char * NDECL(and_get_color_string);
#endif
static void NDECL(and_start_screen);
static void NDECL(and_end_screen);
static char* FDECL(and_getmsghistory_ex, (int*, int*, BOOLEAN_P));
static void FDECL(and_putmsghistory_ex, (const char *, int, int, BOOLEAN_P));
static void save_msg(const char* msg);
static void FDECL(and_status_update, (int, genericptr_t, int, int, int, unsigned long *));
static void and_status_flush();

int GnollHackMain(int argc, char** argv);

extern int glyph2tile[];

struct window_procs and_procs = {
	"and",
	WC_COLOR | WC_HILITE_PET | WC_INVERSE,	/* window port capability options supported */
	WC2_HILITE_STATUS | WC2_FLUSH_STATUS,	/* additional window port capability options supported */
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},   /* color availability */
	and_init_nhwindows,
	and_player_selection,
	and_askname,
	and_get_nh_event,
	and_exit_nhwindows,
	and_suspend_nhwindows,
	and_resume_nhwindows,
	and_create_nhwindow_ex,
	and_clear_nhwindow,
	and_display_nhwindow,
	and_destroy_nhwindow,
	and_curs,
	and_putstr_ex,
	and_putmixed_ex,
	and_display_file,
	and_start_menu_ex,
	and_add_menu,
	and_add_extended_menu,
	and_end_menu_ex,
	and_select_menu,
	and_message_menu,
	and_update_inventory,
	and_mark_synch,
	and_wait_synch,
#ifdef CLIPPING
	and_cliparound,
#endif
#ifdef POSITIONBAR
	and_update_positionbar,
#endif
	and_print_glyph,
	and_issue_gui_command,
	and_raw_print,
	and_raw_print_bold,
	and_nhgetch,
	and_nh_poskey,
	and_nhbell,
	and_doprev_message,
	and_yn_function_ex,
	and_getlin_ex,
	and_get_ext_cmd,
	and_number_pad,
	and_delay_output,
	and_delay_output_milliseconds,
	and_delay_output_intervals,
#ifdef CHANGE_COLOR
	and_change_color,
	and_get_color_string,
#endif
	and_start_screen,
	and_end_screen,
	genl_outrip,
	genl_preference_update,
	and_getmsghistory_ex,
	and_putmsghistory_ex,
	genl_status_init,
	genl_status_finish,
	genl_status_enablefield,
	and_status_update,
	genl_can_suspend_no
};

static void and_n_getline(const char* question, char* buf, int nMax, int showLog);
static void and_n_getline_r(const char* question, char* buf, int nMax, int showLog, int reentry);

//____________________________________________________________________________________
// Java objects. Make sure they are not garbage collected!
static JNIEnv* jEnv;
static jclass jApp;
static jobject jAppInstance;
static jmethodID jDebugLog;
static jmethodID jReceiveKey;
static jmethodID jReceivePosKey;
static jmethodID jCreateWindow;
static jmethodID jDisplayWindow;
static jmethodID jClearWindow;
static jmethodID jDestroyWindow;
static jmethodID jPutString;
static jmethodID jSetHealthColor;
static jmethodID jRedrawStatus;
static jmethodID jRawPrint;
static jmethodID jSetCursorPos;
static jmethodID jPrintTile;
static jmethodID jYNFunction;
static jmethodID jGetLine;
static jmethodID jStartMenu;
static jmethodID jAddMenu;
static jmethodID jEndMenu;
static jmethodID jSelectMenu;
static jmethodID jCliparound;
static jmethodID jDelayOutput;
static jmethodID jShowDPad;
static jmethodID jShowLog;
static jmethodID jSetUsername;
static jmethodID jSetNumPadOption;
static jmethodID jAskName;
static jmethodID jLoadSound;
static jmethodID jPlaySound;
static jmethodID jGetDumplogDir;

static boolean quit_if_possible;
static boolean restoring_msghistory;

static char* msghistory[32];
static int msghistory_idx;
static int msghistory_idx0;

extern const char *status_fieldfmt[MAXBLSTATS];
// Need to separate conditions in order to color them properly
enum bl_conditions {
	BL_COND_STONE,
	BL_COND_SLIME,
	BL_COND_STRNGL,
	BL_COND_SUFFOC,
	BL_COND_FOODPOIS,
	BL_COND_TERMILL,
	BL_COND_BLIND,
	BL_COND_DEAF,
	BL_COND_STUN,
	BL_COND_CONF,
	BL_COND_HALLU,
	BL_COND_LEV,
	BL_COND_FLY,
	BL_COND_RIDE,
	BL_COND_SLOWED,
	BL_COND_PARALYZED,
	BL_COND_FEARFUL,
	BL_COND_SLEEPING,
	BL_COND_CANCELLED,
	BL_COND_SILENCED,
	BL_COND_GRAB
};

#define MAXBLCONDITIONS 21
extern char *status_vals[MAXBLSTATS];
static int status_colors[MAXBLSTATS];
extern boolean status_activefields[MAXBLSTATS];
static unsigned long* cond_hilites;
static unsigned long active_conditions;
static const char* cond_names[] = {
	"Stone", "Slime", "Strngl", "Suffoc", "FoodPois", "TermIll", "Blind",
	"Deaf", "Stun", "Conf", "Hallu", "Lev", "Fly", "Ride", "Slow", "Paral", "Fear", "Sleep", "Cancl", "Silent", "Grab", "Rot", "Lyca"
};


//____________________________________________________________________________________
//
// Helpers
//____________________________________________________________________________________
jbyteArray create_bytearray(const char* str)
{
	int len = str ? strlen(str) : 0;
	jbyteArray a = (*jEnv)->NewByteArray(jEnv, len);
	jbyte* e = (*jEnv)->GetByteArrayElements(jEnv, a, 0);
	memcpy(e, str, len);
	(*jEnv)->ReleaseByteArrayElements(jEnv, a, e, 0);
	return a;
	// return (*jEnv)->NewStringUTF(jEnv, str);
}

//____________________________________________________________________________________
void destroy_jobject(jstring jstr)
{
	(*jEnv)->DeleteLocalRef(jEnv, jstr);
}

#define JNICallV(func, ...) (*jEnv)->CallVoidMethod(jEnv, jAppInstance, func, ## __VA_ARGS__);
#define JNICallI(func, ...) (*jEnv)->CallIntMethod(jEnv, jAppInstance, func, ## __VA_ARGS__);
#define JNICallO(func, ...) (*jEnv)->CallObjectMethod(jEnv, jAppInstance, func, ## __VA_ARGS__);

//____________________________________________________________________________________
void Java_com_tbd_forkfront_NetHackIO_RunNetHack(JNIEnv* env, jobject thiz, jstring path, jstring username)
{
	char* params[10];
	const char* pChars;

	jEnv = env;
	jAppInstance = thiz;
	jApp = (*jEnv)->GetObjectClass(jEnv, jAppInstance);
	jDebugLog = (*jEnv)->GetMethodID(jEnv, jApp, "debugLog", "([B)V");
	jReceiveKey = (*jEnv)->GetMethodID(jEnv, jApp, "receiveKeyCmd", "()I");
	jReceivePosKey = (*jEnv)->GetMethodID(jEnv, jApp, "receivePosKeyCmd", "(I[I)I");
	jCreateWindow = (*jEnv)->GetMethodID(jEnv, jApp, "createWindow", "(I)I");
	jClearWindow = (*jEnv)->GetMethodID(jEnv, jApp, "clearWindow", "(II)V");
	jDisplayWindow = (*jEnv)->GetMethodID(jEnv, jApp, "displayWindow", "(II)V");
	jDestroyWindow = (*jEnv)->GetMethodID(jEnv, jApp, "destroyWindow", "(I)V");
	jPutString = (*jEnv)->GetMethodID(jEnv, jApp, "putString", "(II[BII)V");
	jSetHealthColor = (*jEnv)->GetMethodID(jEnv, jApp, "setHealthColor", "(I)V");
	jRedrawStatus = (*jEnv)->GetMethodID(jEnv, jApp, "redrawStatus", "()V");
	jRawPrint = (*jEnv)->GetMethodID(jEnv, jApp, "rawPrint", "(I[B)V");
	jSetCursorPos = (*jEnv)->GetMethodID(jEnv, jApp, "setCursorPos", "(III)V");
	jPrintTile = (*jEnv)->GetMethodID(jEnv, jApp, "printTile", "(IIIIIII)V");
	jYNFunction = (*jEnv)->GetMethodID(jEnv, jApp, "ynFunction", "([B[BI)V");
	jGetLine = (*jEnv)->GetMethodID(jEnv, jApp, "getLine", "([BIII)Ljava/lang/String;");
	jStartMenu = (*jEnv)->GetMethodID(jEnv, jApp, "startMenu", "(I)V");
	jAddMenu = (*jEnv)->GetMethodID(jEnv, jApp, "addMenu", "(IIIIII[BII)V");
	jEndMenu = (*jEnv)->GetMethodID(jEnv, jApp, "endMenu", "(I[B)V");
	jSelectMenu = (*jEnv)->GetMethodID(jEnv, jApp, "selectMenu", "(III)[I");
	jCliparound = (*jEnv)->GetMethodID(jEnv, jApp, "cliparound", "(IIII)V");
	jDelayOutput = (*jEnv)->GetMethodID(jEnv, jApp, "delayOutput", "()V");
	jShowDPad = (*jEnv)->GetMethodID(jEnv, jApp, "askDirection", "()V");
	jShowLog = (*jEnv)->GetMethodID(jEnv, jApp, "showLog", "(I)V");
	jSetUsername = (*jEnv)->GetMethodID(jEnv, jApp, "setUsername", "([B)V");
	jSetNumPadOption = (*jEnv)->GetMethodID(jEnv, jApp, "setNumPadOption", "(I)V");
	jAskName = (*jEnv)->GetMethodID(jEnv, jApp, "askName", "(I[Ljava/lang/String;)Ljava/lang/String;");
	jLoadSound = (*jEnv)->GetMethodID(jEnv, jApp, "loadSound", "([B)V");
	jPlaySound = (*jEnv)->GetMethodID(jEnv, jApp, "playSound", "([BI)V");
	jGetDumplogDir = (*jEnv)->GetMethodID(jEnv, jApp, "getDumplogDir", "()Ljava/lang/String;");

	if(!(jReceiveKey && jReceivePosKey && jCreateWindow && jClearWindow && jDisplayWindow &&
			jDestroyWindow && jPutString && jRawPrint && jSetCursorPos && jPrintTile &&
			jYNFunction && jGetLine && jStartMenu && jAddMenu && jEndMenu && jSelectMenu &&
			jCliparound && jDelayOutput && jShowDPad && jShowLog && jSetUsername &&
			jSetNumPadOption && jAskName && jSetHealthColor && jRedrawStatus &&
			jLoadSound && jPlaySound && jGetDumplogDir))
	{
		debuglog("baaaaad");
		return;
	}

	pChars = (*jEnv)->GetStringUTFChars(jEnv, path, 0);
	if(chdir(pChars) != 0)
		debuglog("chdir failed %d", errno);
	(*jEnv)->ReleaseStringUTFChars(jEnv, path, pChars);

	params[0] = "gnollhack";
	params[1] = 0;

	GnollHackMain(1, params);
}

//____________________________________________________________________________________
boolean SaveAndExit()
{
	if(!program_state.gameover && program_state.something_worth_saving)
	{
		program_state.done_hup = 0;
		clear_nhwindow(WIN_MESSAGE);
		pline("Saving...");
		if(dosave0(FALSE))
		{
			program_state.something_worth_saving = 0;
			u.uhp = -1;		/* universal game's over indicator */
			/* make sure they see the Saving message */
			display_nhwindow(WIN_MESSAGE, TRUE);
			exit_nhwindows("Be seeing you...");
			nh_terminate(EXIT_SUCCESS);
		}
		return FALSE;
	}
	return TRUE;
}

//____________________________________________________________________________________
void Java_com_tbd_forkfront_NetHackIO_SaveNetHackState(JNIEnv* env, jobject thiz)
{
	if(!program_state.gameover && program_state.something_worth_saving)
		save_currentstate();
}

//____________________________________________________________________________________
void quit_possible()
{
	if(quit_if_possible)
	{
		quit_if_possible = FALSE;
		if(!SaveAndExit())
		{
			if(and_yn_function_ex(YN_STYLE_GENERAL, ATR_NONE, NO_COLOR, NO_GLYPH, (const char*)0, "Error saving game. Quit anyway?", ynchars, 'n', yndescs, (const char*)0, 0UL) == 'y')
				nh_terminate(EXIT_SUCCESS);
		}
	}
}

//____________________________________________________________________________________
void set_username()
{
	jstring username = create_bytearray(plname);
	JNICallV(jSetUsername, username);
	destroy_jobject(username);

}

//____________________________________________________________________________________
void debuglog(const char *fmt, ...)
{
	char buf[256];

	if(fmt != 0)
	{
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, sizeof(buf), fmt, args);
		va_end(args);
	}
	else
	{
		strcpy(buf, "(null)");
	}

	jbyteArray jstr = create_bytearray(buf);
	JNICallV(jDebugLog, jstr);
	destroy_jobject(jstr);
}

//____________________________________________________________________________________
//init_nhwindows(int* argcp, char** argv)
//		-- Initialize the windows used by NetHack.  This can also
//		   create the standard windows listed at the top, but does
//		   not display them.
//		-- Any commandline arguments relevant to the windowport
//		   should be interpreted, and *argcp and *argv should
//		   be changed to remove those arguments.
//		-- When the message window is created, the variable
//		   iflags.window_inited needs to be set to TRUE.  Otherwise
//		   all plines() will be done via raw_print().
//		** Why not have init_nhwindows() create all of the "standard"
//		** windows?  Or at least all but WIN_INFO?	-dean
void and_init_nhwindows(int* argcp, char** argv)
{
	//debuglog("and_init_nhwindows()");
	iflags.window_inited = TRUE;
}

//____________________________________________________________________________________
//player_selection()
//		-- Do a window-port specific player type selection.  If
//		   player_selection() offers a Quit option, it is its
//		   responsibility to clean up and terminate the process.
//		   You need to fill in pl_character[0].
void and_player_selection()
{
	int i, result;
	char pick4u = 'n', thisch, lastch = 0;
	char pbuf[QBUFSZ], plbuf[QBUFSZ];
	//int state[] = {0,0,0,0};
	int state = 0;
	winid win;
	anything any;
	menu_item *selected = 0;

	//debuglog("and_player_selection()");

	/* prevent an unnecessary prompt */
	rigid_role_checks();

	while(flags.initalign < 0)
	{
		if(state < 2)
		{
			if(!state)
				flags.initrole = -1;
			flags.initrace = -1;
			state = 0;
		}
		else
			state &= 1;
		flags.initgend = -1;
		flags.initalign = -1;

		/* Select a role */
		result = 1;
		if(flags.initrole < 0)
		{
			/* Prompt for a role */
			win = create_nhwindow(NHW_MENU);
			and_start_menu_ex(win, 0);
			any.a_void = 0; /* zero out all bits */
			any.a_int = randrole(TRUE)+1;
			and_add_menu(win, NO_GLYPH, &any, '*', 0, ATR_NONE, "Random", MENU_UNSELECTED);
			for(i = 0; roles[i].name.m; i++)
			{
				if(ok_role(i, flags.initrace, flags.initgend, flags.initalign))
				{
					any.a_int = i + 1; /* must be non-zero */
					thisch = lowc(roles[i].name.m[0]);
					if(thisch == lastch)
						thisch = highc(thisch);
					and_add_menu(win, NO_GLYPH, &any, thisch, 0, ATR_NONE, roles[i].name.m, MENU_UNSELECTED);
					lastch = thisch;
				}
			}
			and_end_menu_ex(win, "Pick a role", (const char*)0);
			result = and_select_menu(win, PICK_ONE, &selected);
			and_destroy_nhwindow(win);

			if(result > 0)
				flags.initrole = selected[0].item.a_int - 1;
			free((genericptr_t)selected), selected = 0;
		}

		if(result <= 0)
		{
		    clearlocks();
		    and_exit_nhwindows("bye");
		    nh_terminate(EXIT_SUCCESS);
		}

		/* Select a race, if necessary */
		if(flags.initrace < 0)
			flags.initrace = pick_race(flags.initrole, flags.initgend, flags.initalign, PICK_RIGID);

		result = 1;
		if(flags.initrace < 0)
		{
			/* tty_clear_nhwindow(BASE_WINDOW); */
			/* tty_putstr(BASE_WINDOW, 0, "Choosing Race"); */
			win = create_nhwindow(NHW_MENU);
			and_start_menu_ex(win, 0);
			any.a_void = 0; /* zero out all bits */
			any.a_int = randrace(flags.initrole)+1;
			and_add_menu(win, NO_GLYPH, &any, '*', 0, ATR_NONE, "random", MENU_UNSELECTED);
			for(i = 0; races[i].noun; i++)
				if(ok_race(flags.initrole, i, flags.initgend, flags.initalign))
				{
					any.a_int = i + 1; /* must be non-zero */
					and_add_menu(win, NO_GLYPH, &any, races[i].noun[0], 0, ATR_NONE, races[i].noun, MENU_UNSELECTED);
				}
			and_end_menu_ex(win, "Pick a race", (const char*)0);
			result = and_select_menu(win, PICK_ONE, &selected);
			and_destroy_nhwindow(win);

			if(result > 0)
			{
				flags.initrace = selected[0].item.a_int - 1;
				state |= 1;
			}
			free((genericptr_t)selected), selected = 0;
		}

		if(result <= 0)
			continue;

		/* Select a gender, if necessary */
		flags.initgend = pick_gend(flags.initrole, flags.initrace, flags.initalign, PICK_RIGID);

		result = 1;
		/* force compatibility with role/race, try for compatibility with
		 * pre-selected alignment */
		if(flags.initgend < 0)
		{
			/* tty_clear_nhwindow(BASE_WINDOW); */
			/* tty_putstr(BASE_WINDOW, 0, "Choosing Gender"); */
			win = create_nhwindow(NHW_MENU);
			and_start_menu_ex(win, 0);
			any.a_void = 0; /* zero out all bits */
			any.a_int = randgend(flags.initrole, flags.initrace)+1;
			and_add_menu(win, NO_GLYPH, &any, '*', 0, ATR_NONE, "random", MENU_UNSELECTED);
			for(i = 0; i < ROLE_GENDERS; i++)
				if(ok_gend(flags.initrole, flags.initrace, i, flags.initalign))
				{
					any.a_int = i + 1;
					and_add_menu(win, NO_GLYPH, &any, genders[i].adj[0], 0, ATR_NONE, genders[i].adj, MENU_UNSELECTED);
				}
			and_end_menu_ex(win, "Pick a gender", (const char*)0);
			result = and_select_menu(win, PICK_ONE, &selected);
			and_destroy_nhwindow(win);

			if(result > 0)
			{
				flags.initgend = selected[0].item.a_int - 1;
				state |= 2;
			}
			free((genericptr_t)selected), selected = 0;
		}

		if(result <= 0)
			continue;

		/* Select an alignment, if necessary */
		flags.initalign = pick_align(flags.initrole, flags.initrace, flags.initgend, PICK_RIGID);

		result = 1;
		if(flags.initalign < 0)
		{
			/* tty_clear_nhwindow(BASE_WINDOW); */
			/* tty_putstr(BASE_WINDOW, 0, "Choosing Alignment"); */
			win = and_create_nhwindow_ex(NHW_MENU, 0, NO_GLYPH, zerocreatewindowinfo);
			and_start_menu_ex(win, 0);
			any.a_void = 0; /* zero out all bits */
			any.a_int = randalign(flags.initrole, flags.initrace)+1;
			and_add_menu(win, NO_GLYPH, &any, '*', 0, ATR_NONE, "random", MENU_UNSELECTED);
			for(i = 0; i < ROLE_ALIGNS; i++)
				if(ok_align(flags.initrole, flags.initrace, flags.initgend, i))
				{
					any.a_int = i + 1;
					and_add_menu(win, NO_GLYPH, &any, aligns[i].adj[0], 0, ATR_NONE, aligns[i].adj, MENU_UNSELECTED);
				}
			and_end_menu_ex(win, "Pick an alignment", (const char*)0);
			result = and_select_menu(win, PICK_ONE, &selected);
			and_destroy_nhwindow(win);

			if(result > 0)
				flags.initalign = selected[0].item.a_int - 1;
			free((genericptr_t)selected), selected = 0;
		}
	}
}

//____________________________________________________________________________________
//get_nh_event()	-- Does window event processing (e.g. exposure events).
//		   A noop for the tty and X window-ports.
void and_get_nh_event()
{
}

//____________________________________________________________________________________
//exit_nhwindows(str)
//		-- Exits the window system.  This should dismiss all windows,
//		   except the "window" used for raw_print().  str is printed
//		   if possible.
void and_exit_nhwindows(const char *str)
{
	//debuglog("exit_nhwindows");
	iflags.window_inited = FALSE;
}

//____________________________________________________________________________________
//suspend_nhwindows(str)
//		-- Prepare the window to be suspended.
void and_suspend_nhwindows(const char *str)
{
}

//____________________________________________________________________________________
//resume_nhwindows()
//		-- Restore the windows after being suspended.
void and_resume_nhwindows()
{
}

//____________________________________________________________________________________
//window = create_nhwindow(type)
//		-- Create a window of type "type."
winid and_create_nhwindow_ex(int type, int style, int glyph, struct extended_create_window_info)
{
	return JNICallI(jCreateWindow, type);
}

//____________________________________________________________________________________
//clear_nhwindow(window)
//		-- Clear the given window, when appropriate.
void and_clear_nhwindow(winid wid)
{
	//debuglog("and_clear_nhwindow(%d)", wid);
	JNICallV(jClearWindow, wid, Is_rogue_level(&u.uz));
}

//____________________________________________________________________________________
//display_nhwindow(window, boolean blocking)
//		-- Display the window on the screen.  If there is data
//		   pending for output in that window, it should be sent.
//		   If blocking is TRUE, display_nhwindow() will not
//		   return until the data has been displayed on the screen,
//		   and acknowledged by the user where appropriate.
//		-- All calls are blocking in the tty window-port.
//		-- Calling display_nhwindow(WIN_MESSAGE,???) will do a
//		   --more--, if necessary, in the tty window-port.
void and_display_nhwindow(winid wid, BOOLEAN_P blocking)
{
	//debuglog("display_nhwindow(%d)", wid);
	if(wid != WIN_MESSAGE && /*wid != WIN_STATUS && */wid != WIN_MAP)
		blocking = TRUE;
	JNICallV(jDisplayWindow, wid, blocking);
	if(blocking)
		and_nhgetch();
}

//____________________________________________________________________________________
//destroy_nhwindow(window)
//		-- Destroy will dismiss the window if the window has not
//		   already been dismissed.
void and_destroy_nhwindow(winid wid)
{
	JNICallV(jDestroyWindow, wid);
}

//____________________________________________________________________________________
//curs(window, x, y)
//		-- Next output to window will start at (x,y), also moves
//		   displayable cursor to (x,y).  For backward compatibility,
//		   1 <= x < cols, 0 <= y < rows, where cols and rows are
//		   the size of window.
//		-- For variable sized windows, like the status window, the
//		   behavior when curs() is called outside the window's limits
//		   is unspecified. The mac port wraps to 0, with the status
//		   window being 2 lines high and 80 columns wide.
//		-- Still used by curs_on_u(), status updates, screen locating
//		   (identify, teleport).
//		-- NHW_MESSAGE, NHW_MENU and NHW_TEXT windows do not
//		   currently support curs in the tty window-port.
void and_curs(winid wid, int x, int y)
{
	JNICallV(jSetCursorPos, wid, x, y);
}

// For TEXTCOLOR
static int text_attribs = 0;
static int text_color = CLR_WHITE;

static int palette[CLR_MAX] = {
	0xFF555555,	// CLR_BLACK
	0xFFFF0000,	// CLR_RED
	0xFF008800,	// CLR_GREEN
	0xFF664411, // CLR_BROWN
	0xFF0000FF,	// CLR_BLUE
	0xFFFF00FF,	// CLR_MAGENTA
	0xFF00FFFF,	// CLR_CYAN
	0xFF888888,	// CLR_GRAY
	0xFFFFFFFF,	// NO_COLOR
	0xFFFF9900,	// CLR_ORANGE
	0xFF00FF00,	// CLR_BRIGHT_GREEN
	0xFFFFFF00,	// CLR_YELLOW
	0xFF0088FF,	// CLR_BRIGHT_BLUE
	0xFFFF77FF,	// CLR_BRIGHT_MAGENTA
	0xFF77FFFF,	// CLR_BRIGHT_CYAN
	0xFFFFFFFF	// CLR_WHITE
};
int nhcolor_to_RGB(int c)
{
	if(c >= 0 && c < CLR_MAX)
		return palette[c];
	return 0xFF000000;
}

const char* colname(int color)
{
	switch(color)
	{
	case CLR_BLACK: return "black";
	case CLR_RED: return "red";
	case CLR_GREEN: return "green";
	case CLR_BROWN: return "brown";
	case CLR_BLUE: return "blue";
	case CLR_MAGENTA: return "magenta";
	case CLR_CYAN: return "cyan";
	case CLR_GRAY: return "gray";
	case NO_COLOR: return "no color";
	case CLR_ORANGE: return "orange";
	case CLR_BRIGHT_GREEN: return "bright green";
	case CLR_YELLOW: return "yellow";
	case CLR_BRIGHT_BLUE: return "bright blue";
	case CLR_BRIGHT_MAGENTA: return "bright magenta";
	case CLR_BRIGHT_CYAN: return "bright cyan";
	case CLR_WHITE: return "white";
	default: return "black";
	}
}

void
term_start_attr(attr)
int attr;
{
	text_attribs |= 1<<attr;
}

void
term_end_attr(attr)
int attr;
{
	text_attribs &= ~(1<<attr);
}

void
term_start_color(color)
int color;
{
	//debuglog("term_start_color %s", colname(color));
	text_color = color;
}

void
term_end_color()
{
	text_color = CLR_WHITE;
}

//____________________________________________________________________________________
//putstr(window, attr, str)
//		-- Print str on the window with the given attribute.  Only
//		   printable ASCII characters (040-0126) must be supported.
//		   Multiple putstr()s are output on separate lines.  Attributes
//		   can be one of
//			ATR_NONE (or 0)
//			ATR_ULINE
//			ATR_BOLD
//			ATR_BLINK
//			ATR_INVERSE
//		   If a window-port does not support all of these, it may map
//		   unsupported attributes to a supported one (e.g. map them
//		   all to ATR_INVERSE).  putstr() may compress spaces out of
//		   str, break str, or truncate str, if necessary for the
//		   display.  Where putstr() breaks a line, it has to clear
//		   to end-of-line.
//		-- putstr should be implemented such that if two putstr()s
//		   are done consecutively the user will see the first and
//		   then the second.  In the tty port, pline() achieves this
//		   by calling more() or displaying both on the same line.
//____________________________________________________________________________________
void and_putstr_ex(winid wid, int attr, const char *str, int append, int nhcolor)
{
	if(!str || !*str)
		return;
	jbyteArray jstr = create_bytearray(str);
	JNICallV(jPutString, wid, attr, jstr, append, nhcolor_to_RGB(nhcolor));
	destroy_jobject(jstr);
}

void and_putstr(winid wid, int attr, const char *str)
{
	if(attr)
		attr = 1<<attr;
	else
		attr = text_attribs;

	and_putstr_ex(wid, attr, str, 0, text_color);

	if(wid == NHW_MESSAGE)
	{
		save_msg(str);
#ifdef USER_SOUNDS
		if(!restoring_msghistory)
			play_sound_for_message(str);
#endif
	}
}

void and_set_health_color(int nhcolor)
{
	JNICallV(jSetHealthColor, nhcolor_to_RGB(nhcolor));
}

void and_bot_updated()
{
	JNICallV(jRedrawStatus);
}

//____________________________________________________________________________________
//status_update(int fldindex, genericptr_t ptr, int chg, int percentage, int color, long *colormasks)
//		-- update the value of a status field.
//		-- the fldindex identifies which field is changing and
//		   is an integer index value from botl.h
//		-- fldindex could be any one of the following from botl.h:
//		   BL_TITLE, BL_STR, BL_DX, BL_CO, BL_IN, BL_WI, BL_CH,
//		   BL_ALIGN, BL_SCORE, BL_CAP, BL_GOLD, BL_ENE, BL_ENEMAX,
//		   BL_XP, BL_AC, BL_HD, BL_TIME, BL_HUNGER, BL_HP, BL_HPMAX,
//		   BL_LEVELDESC, BL_EXP, BL_CONDITION
//		-- The value passed for BL_GOLD includes a leading
//		   symbol for GOLD "$:nnn". If the window port needs to use
//		   the textual gold amount without the leading "$:" the port
//		   will have to add 2 to the passed "ptr" for the BL_GOLD case.
//		-- fldindex could also be BL_FLUSH (-1), which is not really
//		   a field index, but is a special trigger to tell the
//		   windowport that it should redisplay all its status fields,
//		   even if no changes have been presented to it.
//		-- ptr is usually a "char *", unless fldindex is BL_CONDITION.
//		   If fldindex is BL_CONDITION, then ptr is a long value with
//		   any or none of the following bits set (from botl.h):
//                        BL_MASK_STONE           0x00000001L
//                        BL_MASK_SLIME           0x00000002L
//                        BL_MASK_STRNGL          0x00000004L
//                        BL_MASK_FOODPOIS        0x00000008L
//                        BL_MASK_TERMILL         0x00000010L
//                        BL_MASK_BLIND           0x00000020L
//                        BL_MASK_DEAF            0x00000040L
//                        BL_MASK_STUN            0x00000080L
//                        BL_MASK_CONF            0x00000100L
//                        BL_MASK_HALLU           0x00000200L
//                        BL_MASK_LEV             0x00000400L
//                        BL_MASK_FLY             0x00000800L
//                        BL_MASK_RIDE            0x00001000L
//      -- color is an unsigned int.
//             int & 0x00FF = color CLR_*
//             int >> 8 = attribute (if any)
//         This contains the color and attribute that the field should
//         be displayed in.
//         This is relevant for everything except BL_CONDITION fldindex.
//         If fldindex is BL_CONDITION, this parameter should be ignored,
//         as condition hilighting is done via the next colormasks
//         parameter instead.
//      -- colormasks - pointer to cond_hilites[] array of colormasks.
//         Only relevant for BL_CONDITION fldindex. The window port
//         should ignore this parameter for other fldindex values.
//         Each condition bit must only ever appear in one of the
//         CLR_ array members, but can appear in multiple HL_ATTCLR_
//         offsets (because more than one attribute can co-exist).
//         For the user's chosen set of BL_MASK_ condition bits,
//         They are stored internally in the cond_hilites[] array,
//         at the array offset aligned to the color those condtion
//         bits should display in.
//         For example, if the user has chosen to display strngl
//         and stone and termill in red and inverse,
//              BL_MASK_SLIME           0x00000002
//              BL_MASK_STRNGL          0x00000004
//              BL_MASK_TERMILL         0x00000010
//         The bitmask corresponding to those conditions is
//         0x00000016 (or 00010110 in binary) and the color
//         is at offset 1 (CLR_RED).
//         Here is how that is stored in the cond_hilites[] array:
//         +------+----------------------+--------------------+
//         |array |                      |                    |
//         |offset| macro for indexing   |   bitmask          |
//         |------+----------------------+--------------------+
//         |   0  |   CLR_BLACK          |                    |
//         +------+----------------------+--------------------+
//         |   1  |   CLR_RED            |   00010110         |
//         +------+----------------------+--------------------+
//         |   2  |   CLR_GREEN          |                    |
//         +------+----------------------+--------------------+
//         |   3  |   CLR_BROWN          |                    |
//         +------+----------------------+--------------------+
//         |   4  |   CLR_BLUE           |                    |
//         +------+----------------------+--------------------+
//         |   5  |   CLR_MAGENTA        |                    |
//         +------+----------------------+--------------------+
//         |   6  |   CLR_CYAN           |                    |
//         +------+----------------------+--------------------+
//         |   7  |   CLR_GRAY           |                    |
//         +------+----------------------+--------------------+
//         |   8  |   NO_COLOR           |                    |
//         +------+----------------------+--------------------+
//         |   9  |   CLR_ORANGE         |                    |
//         +------+----------------------+--------------------+
//         |  10  |   CLR_BRIGHT_GREEN   |                    |
//         +------+----------------------+--------------------+
//         |  11  |   CLR_BRIGHT_YELLOW  |                    |
//         +------+----------------------+--------------------+
//         |  12  |   CLR_BRIGHT_BLUE    |                    |
//         +------+----------------------+--------------------+
//         |  13  |   CLR_BRIGHT_MAGENTA |                    |
//         +------+----------------------+--------------------+
//         |  14  |   CLR_BRIGHT_CYAN    |                    |
//         +------+----------------------+--------------------+
//         |  15  |   CLR_WHITE          |                    |
//         +------+----------------------+--------------------+
//         |  16  |   HL_ATTCLR_DIM      |                    | CLR_MAX
//         +------+----------------------+--------------------+
//         |  17  |   HL_ATTCLR_BLINK    |                    |
//         +------+----------------------+--------------------+
//         |  18  |   HL_ATTCLR_ULINE    |                    |
//         +------+----------------------+--------------------+
//         |  19  |   HL_ATTCLR_INVERSE  |   00010110         |
//         +------+----------------------+--------------------+
//         |  20  |   HL_ATTCLR_BOLD     |                    |
//         +------+----------------------+--------------------+
//         |  21  |  beyond array boundary                    | BL_ATTCLR_MAX
//         The window port can AND (&) the bits passed in the
//         ptr argument to status_update() with any non-zero
//         entries in the cond_hilites[] array to determine
//         the color and attributes for displaying the
//         condition on the screen for the user.
//         If the bit for a particular condition does not
//         appear in any of the cond_hilites[] array offsets,
//         that condition should be displayed in the default
//         color and attributes.
//____________________________________________________________________________________
int hl_attridx_to_attrmask(int idx)
{
	switch(idx)
	{
	case HL_ATTCLR_DIM: 	return (1<<ATR_DIM);
	case HL_ATTCLR_BLINK:	return (1<<ATR_BLINK);
	case HL_ATTCLR_ULINE:   return (1<<ATR_ULINE);
	case HL_ATTCLR_INVERSE:	return (1<<ATR_INVERSE);
	case HL_ATTCLR_BOLD:	return (1<<ATR_BOLD);
	}
	return 0;
}

int hl_attrmask_to_attrmask(int mask)
{
	int attr = 0;
	if(mask & HL_DIM) attr |= (1<<ATR_DIM);
	if(mask & HL_BLINK) attr |= (1<<ATR_BLINK);
	if(mask & HL_ULINE) attr |= (1<<ATR_ULINE);
	if(mask & HL_INVERSE) attr |= (1<<ATR_INVERSE);
	if(mask & HL_BOLD) attr |= (1<<ATR_BOLD);
	return attr;
}

void and_status_update(int idx, genericptr_t ptr, int chg, int percent, int color, unsigned long *colormasks)
{
	long cond, *condptr = (long *) ptr;
	char *nb, *text = (char *) ptr;
	int i;

	if(idx == BL_FLUSH)
	{
		if(cond_hilites)
			and_status_flush();
	}
	else if(status_activefields[idx])
	{
		if(idx == BL_CONDITION)
		{
			cond_hilites = colormasks;
			active_conditions = condptr ? *condptr : 0L;
			*status_vals[idx] = 0;
		}
		else if(idx == BL_GOLD && *text == '\\')
		{
			// Remove encoded glyph value. (This might break in the future if the format is changed in botl.c)
			text += 10;
			Sprintf(status_vals[idx], "$%s", text);
			status_colors[idx] = color;
		}
		else
		{
			Sprintf(status_vals[idx], status_fieldfmt[idx] ? status_fieldfmt[idx] : "%s", text ? text : "");
			status_colors[idx] = color;
		}
	}
}

int get_condition_color(int cond_mask)
{
	int i;
	for(i = 0; i < CLR_MAX; i++)
		if(cond_hilites[i] & cond_mask)
			return i;
	return CLR_WHITE;
}

int get_condition_attr(int cond_mask)
{
	int i;
	int attr = 0;
	for(i = CLR_MAX; i < BL_ATTCLR_MAX; i++)
		if(cond_hilites[i] & cond_mask)
			attr |= hl_attridx_to_attrmask(i);
	return attr;
}

void print_conditions(const char** names)
{
	int i;
	for(i = 0; i < MAXBLCONDITIONS; i++) {
		int cond_mask = 1 << i;
		if(active_conditions & cond_mask)
		{
			const char* name = names[i];
			int color = get_condition_color(cond_mask);
			int attr = get_condition_attr(cond_mask);
			//debuglog("cond '%s' active. col=%s attr=%x", name, colname(color), attr);
			and_putstr_ex(WIN_STATUS, ATR_NONE, " ", 0, CLR_WHITE);
			and_putstr_ex(WIN_STATUS, attr, name, 0, color);
		}
	}
}

void print_status_field(int idx, boolean first_field)
{
	if(!status_activefields[idx])
		return;

	const char* val = status_vals[idx];

	if(first_field && *val == ' ')
	{
		// Remove leading space of first field
		val++;
	}
	else if(idx == BL_LEVELDESC && !first_field)
	{
		/* leveldesc has no leading space, so if we've moved
		   it past the first position, provide one */
		and_putstr_ex(WIN_STATUS, ATR_NONE, " ", 0, CLR_WHITE);
	}

	// Don't want coloring on leading spaces (ATR_INVERSE would show), so print those first
	while(*val == ' ')
	{
		and_putstr_ex(WIN_STATUS, ATR_NONE, " ", 0, CLR_WHITE);
		val++;
	}

	if(idx == BL_CONDITION)
	{
		print_conditions(cond_names);
	}
	else
	{
		int attr = (status_colors[idx] >> 8) & 0xFF;
		int color = status_colors[idx] & 0xFF;
		if(idx == BL_HP)
		{
			and_set_health_color(color);
		}
		else if(idx == BL_HPMAX)
		{
			// Set hp-max to same color as hp if it's not explicitly defined
			if(color == NO_COLOR && attr == ATR_NONE && status_activefields[BL_HP])
			{
				attr = (status_colors[BL_HP] >> 8) & 0xFF;
				color = status_colors[BL_HP] & 0xFF;
			}
		}
		else if(idx == BL_ENEMAX)
		{
			// Set power-max to same color as power if it's not explicitly defined
			if(color == NO_COLOR && attr == ATR_NONE && status_activefields[BL_ENE])
			{
				attr = (status_colors[BL_ENE] >> 8) & 0xFF;
				color = status_colors[BL_ENE] & 0xFF;
			}
		}
		and_putstr_ex(WIN_STATUS, hl_attrmask_to_attrmask(attr), val, 0, color);
	//	debuglog("field %d: %s color %s", idx+1, val, colname(color));
	}
}

void and_status_flush()
{
	enum statusfields idx, *fieldlist;
	register int i;

	static enum statusfields fieldorder_line1[] = {
		BL_TITLE, BL_STR, BL_DX, BL_CO, BL_IN, BL_WI, BL_CH, BL_GOLD, BL_FLUSH,
		BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH,
		BL_FLUSH
	};

	static enum statusfields fieldorder_line2[] = {
		BL_MODE, BL_LEVELDESC, BL_HP, BL_HPMAX, BL_ENE, BL_ENEMAX, BL_AC, BL_MC_LVL, BL_MC_PCT, BL_MOVE, BL_UWEP, BL_UWEP2, BL_XP,
		BL_EXP, BL_HD, BL_TIME, BL_2WEP, BL_SKILL, BL_HUNGER, BL_CAP, BL_CONDITION,
		BL_FLUSH
	};

	curs(WIN_STATUS, 1, 0);
	for(i = 0; (idx = fieldorder_line1[i]) != BL_FLUSH; ++i)
		print_status_field(idx, i == 0);

	curs(WIN_STATUS, 1, 1);
	for(i = 0; (idx = fieldorder_line2[i]) != BL_FLUSH; ++i)
		print_status_field(idx, i == 0);

	and_bot_updated();
}

//____________________________________________________________________________________
void and_putmixed_ex(window, attr, str, app, color)
winid window;
int attr, app, color;
const char *str;
{
	//debuglog("put mixed: %s", str);
	genl_putmixed_ex(window, attr, str, app, color);
}

//____________________________________________________________________________________
//display_file(str, boolean complain)
//		-- Display the file named str.  Complain about missing files
//		   iff complain is TRUE.
void and_display_file(const char *name, BOOLEAN_P complain)
{
	//debuglog("and_display_file(%s, %d)", name, complain);

	dlb* f;
	char buf[BUFSZ];
	char *cr;

	and_clear_nhwindow(WIN_MESSAGE);
	f = dlb_fopen(name, "r");
	if(f)
	{
		winid datawin = and_create_nhwindow_ex(NHW_TEXT, 0, NO_GLYPH, zerocreatewindowinfo);
		boolean empty = TRUE;
		while(dlb_fgets(buf, BUFSZ, f))
		{
			if((cr = index(buf, '\n')) != 0)
				*cr = 0;
			if(index(buf, '\t') != 0)
				(void)tabexpand(buf);
			empty = FALSE;
			and_putstr(datawin, 0, buf);
		}
		(void)dlb_fclose(f);
		if(!empty)
			and_display_nhwindow(datawin, TRUE);
		and_destroy_nhwindow(datawin);
	}
}

//____________________________________________________________________________________
//start_menu(window)
//		-- Start using window as a menu.  You must call start_menu()
//		   before add_menu().  After calling start_menu() you may not
//		   putstr() to the window.  Only windows of type NHW_MENU may
//		   be used for menus.
void and_start_menu_ex(winid wid, int style)
{
	JNICallV(jStartMenu, wid);
}

//____________________________________________________________________________________
//add_menu(windid window, int glyph, const anything identifier,
//				char accelerator, char groupacc,
//				int attr, char *str, boolean preselected)
//		-- Add a text line str to the given menu window.  If identifier
//		   is 0, then the line cannot be selected (e.g. a title).
//		   Otherwise, identifier is the value returned if the line is
//		   selected.  Accelerator is a keyboard key that can be used
//		   to select the line.  If the accelerator of a selectable
//		   item is 0, the window system is free to select its own
//		   accelerator.  It is up to the window-port to make the
//		   accelerator visible to the user (e.g. put "a - " in front
//		   of str).  The value attr is the same as in putstr().
//		   Glyph is an optional glyph to accompany the line.  If
//		   window port cannot or does not want to display it, this
//		   is OK.  If there is no glyph applicable, then this
//		   value will be NO_GLYPH.
//		-- All accelerators should be in the range [A-Za-z],
//		   but there are a few exceptions such as the tty player
//		   selection code which uses '*'.
//	        -- It is expected that callers do not mix accelerator
//		   choices.  Either all selectable items have an accelerator
//		   or let the window system pick them.  Don't do both.
//		-- Groupacc is a group accelerator.  It may be any character
//		   outside of the standard accelerator (see above) or a
//		   number.  If 0, the item is unaffected by any group
//		   accelerator.  If this accelerator conflicts with
//		   the menu command (or their user defined alises), it loses.
//		   The menu commands and aliases take care not to interfere
//		   with the default object class symbols.
//		-- If you want this choice to be preselected when the
//		   menu is displayed, set preselected to TRUE.
void and_add_menu(winid wid, int glyph, const ANY_P *ident, CHAR_P accelerator, CHAR_P groupacc, int attr, const char *str, BOOLEAN_P preselected)
{
	int tile, color;
	if(glyph == NO_GLYPH)
		tile = -1;
	else
		tile = glyph2tile[glyph];

	if(iflags.use_menu_color && get_menu_coloring((char*)str, &color, &attr)) {
		color = nhcolor_to_RGB(color);
	} else {
		color = -1;
	}

	//debuglog("add menu %d: %s", attr, str);

	if(attr)
		attr = 1<<attr;

	jbyteArray jstr = create_bytearray(str);
	JNICallV(jAddMenu, wid, tile, ident->a_int, (int)accelerator, (int)groupacc, attr, jstr, (int)preselected, color);
	destroy_jobject(jstr);
}

void and_add_extended_menu(winid wid, int glyph, const ANY_P* ident, struct extended_menu_info info, CHAR_P accelerator, CHAR_P groupacc, int attr, const char* str, BOOLEAN_P preselected)
{
	and_add_menu(wid, glyph, ident, accelerator, groupacc, attr, str, preselected);
}
//____________________________________________________________________________________
//end_menu(window, prompt)
//		-- Stop adding entries to the menu and flushes the window
//		   to the screen (brings to front?).  Prompt is a prompt
//		   to give the user.  If prompt is NULL, no prompt will
//		   be printed.
//		** This probably shouldn't flush the window any more (if
//		** it ever did).  That should be select_menu's job.  -dean
void and_end_menu_ex(winid wid, const char *prompt, const char* subtitle)
{
	jbyteArray jstr;
	if(prompt)
		jstr = create_bytearray(prompt);
	else
		jstr = create_bytearray("");
	JNICallV(jEndMenu, wid, jstr);
	destroy_jobject(jstr);
}

//____________________________________________________________________________________
//int select_menu(windid window, int how, menu_item **selected)
//		-- Return the number of items selected; 0 if none were chosen,
//		   -1 when explicitly cancelled.  If items were selected, then
//		   selected is filled in with an allocated array of menu_item
//		   structures, one for each selected line.  The caller must
//		   free this array when done with it.  The "count" field
//		   of selected is a user supplied count.  If the user did
//		   not supply a count, then the count field is filled with
//		   -1 (meaning all).  A count of zero is equivalent to not
//		   being selected and should not be in the list.  If no items
//		   were selected, then selected is NULL'ed out.  How is the
//		   mode of the menu.  Three valid values are PICK_NONE,
//		   PICK_ONE, and PICK_ANY, meaning: nothing is selectable,
//		   only one thing is selectable, and any number valid items
//		   may selected.  If how is PICK_NONE, this function should
//		   never return anything but 0 or -1.
//		-- You may call select_menu() on a window multiple times --
//		   the menu is saved until start_menu() or destroy_nhwindow()
//		   is called on the window.
//		-- Note that NHW_MENU windows need not have select_menu()
//		   called for them. There is no way of knowing whether
//		   select_menu() will be called for the window at
//		   create_nhwindow() time.
int and_select_menu_r(winid wid, int how, MENU_ITEM_P **selected, int reentry)
{
	int i, n;
	jintArray a;
	jint* p;
	jint* q;

	//debuglog("and_select_menu");

	a = (jintArray)JNICallO(jSelectMenu, wid, how, reentry);

	*selected = 0;

	//debuglog("returned %d", a);
	if(a == 0)
		return -1;

	n = (*jEnv)->GetArrayLength(jEnv, a);

	if(n > 1) // n should always be 2k (id, count) pairs
	{
		n >>= 1;

		q = p = (*jEnv)->GetIntArrayElements(jEnv, a, 0);
		*selected = (MENU_ITEM_P*)malloc(sizeof(MENU_ITEM_P) * n);
		for(i = 0; i < n; i++)
		{
			(*selected)[i].item.a_int = *p++;
			(*selected)[i].count = *p++;
		}
		(*jEnv)->ReleaseIntArrayElements(jEnv, a, q, 0);
	}
	else if(n == 1)
	{
		// special case: ABORT
		if(!program_state.gameover && program_state.something_worth_saving)
			n = 0;
		else
			n = and_select_menu_r(wid, how, selected, 1);
	}

	destroy_jobject(a);

	return n;
}

int and_select_menu(winid wid, int how, MENU_ITEM_P **selected)
{
	return and_select_menu_r(wid, how, selected, 0);
}

//____________________________________________________________________________________
//char message_menu(char let, int how, const char *mesg)
//		-- tty-specific hack to allow single line context-sensitive
//		   help to behave compatibly with multi-line help menus.
//		-- This should only be called when a prompt is active; it
//		   sends `mesg' to the message window.  For tty, it forces
//		   a --More-- prompt and enables `let' as a viable keystroke
//		   for dismissing that prompt, so that the original prompt
//		   can be answered from the message line "help menu".
//		-- Return value is either `let', '\0' (no selection was made),
//		   or '\033' (explicit cancellation was requested).
//		-- Interfaces which issue prompts and messages to separate
//		   windows typically won't need this functionality, so can
//		   substitute genl_message_menu (windows.c) instead.
char and_message_menu(CHAR_P let, int how, const char* mesg)
{
	//debuglog("message_menu: %s", mesg);

    pline("%s", mesg);
    return 0;
}

//____________________________________________________________________________________
//update_inventory()
//		-- Indicate to the window port that the inventory has been
//		   changed.
//		-- Merely calls display_inventory() for window-ports that
//		   leave the window up, otherwise empty.
void and_update_inventory()
{
	//debuglog("and_update_inventory");
}

//____________________________________________________________________________________
//mark_synch()	-- Don't go beyond this point in I/O on any channel until
//		   all channels are caught up to here.  Can be an empty call
//		   for the moment
void and_mark_synch()
{
	//debuglog("and_mark_synch");
}

//____________________________________________________________________________________
//wait_synch()	-- Wait until all pending output is complete (*flush*() for
//		   streams goes here).
//		-- May also deal with exposure events etc. so that the
//		   display is OK when return from wait_synch().
void and_wait_synch()
{
	//debuglog("and_wait_synch");
}

//____________________________________________________________________________________
#ifdef CLIPPING
//cliparound(x, y)-- Make sure that the user is more-or-less centered on the
//		   screen if the playing area is larger than the screen.
//		-- This function is only defined if CLIPPING is defined.
void and_cliparound(int x, int y, BOOLEAN_P force)
{
	//debuglog("and_cliparound %dx%d (%dx%d)", x, y, u.ux, u.uy);
	JNICallV(jCliparound, x, y, u.ux, u.uy);
}

#endif

//____________________________________________________________________________________
#ifdef POSITIONBAR
//update_positionbar(char *features)
//		-- Optional, POSITIONBAR must be defined. Provide some
//		   additional information for use in a horizontal
//		   position bar (most useful on clipped displays).
//		   Features is a series of char pairs.  The first char
//		   in the pair is a symbol and the second char is the
//		   column where it is currently located.
//		   A '<' is used to mark an upstairs, a '>'
//		   for a downstairs, and an '@' for the current player
//		   location. A zero char marks the end of the list.
void and_update_positionbar(char *features)
{
	//debuglog("and_update_positionbar");
}

#endif

//____________________________________________________________________________________
//print_glyph(window, x, y, glyph)
//		-- Print the glyph at (x,y) on the given window.  Glyphs are
//		   integers at the interface, mapped to whatever the window-
//		   port wants (symbol, font, color, attributes, ...there's
//		   a 1-1 map between glyphs and distinct things on the map).

void and_issue_gui_command(int initid)
{
	return;
}

void and_print_glyph(winid wid, XCHAR_P x, XCHAR_P y, struct layer_info layers)
{
	int glyph = layers.glyph;
	int bkglyph = layers.bkglyph;

	//debuglog("and_print_glyph wid=%d %dx%d", wid, x, y);
	int tile;
	if(glyph == NO_GLYPH)
		tile = -1;
	else
		tile = glyph2tile[glyph];
	nhsym ch;
	int col;
	unsigned int special;
	mapglyph(glyph, &ch, &col, &special, x, y); // , 0);

	special &= ~(MG_CORPSE|MG_INVIS|MG_RIDDEN|MG_STATUE); // TODO support
	if(!iflags.hilite_pet)
		special &= ~MG_PET;
	if(!iflags.hilite_pile)
		special &= ~MG_OBJPILE;
	if(!iflags.use_inverse)
		special &= ~MG_DETECT;

	JNICallV(jPrintTile, wid, x, y, tile, (int)ch, nhcolor_to_RGB(col), special);
}

//____________________________________________________________________________________
// raw_print(str)	-- Print directly to a screen, or otherwise guarantee that
// 		   the user sees str.  raw_print() appends a newline to str.
// 		   It need not recognize ASCII control characters.  This is
// 		   used during startup (before windowing system initialization
// 		   -- maybe this means only error startup messages are raw),
// 		   for error messages, and maybe other "msg" uses.  E.g.
// 		   updating status for micros (i.e, "saving").
void and_raw_print(const char* str)
{
	jbyteArray jstr = create_bytearray(str);
	JNICallV(jRawPrint, ATR_NONE, jstr);
	destroy_jobject(jstr);
}

//____________________________________________________________________________________
// raw_print_bold(str)
// 		-- Like raw_print(), but prints in bold/standout (if possible).
void and_raw_print_bold(const char* str)
{
	jbyteArray jstr = create_bytearray(str);
	JNICallV(jRawPrint, ATR_BOLD, jstr);
	destroy_jobject(jstr);
}

//____________________________________________________________________________________
//int nhgetch()	-- Returns a single character input from the user.
//		-- In the tty window-port, nhgetch() assumes that tgetch()
//		   will be the routine the OS provides to read a character.
//		   Returned character _must_ be non-zero and it must be
//                   non meta-zero too (zero with the meta-bit set).
int and_nhgetch()
{
	//debuglog("and_nhgetch");
	int c = JNICallI(jReceiveKey);

	quit_if_possible = FALSE;
	if(c == 0x80)
	{
		if(!program_state.gameover && program_state.something_worth_saving)
		{
			c = '\033';
			quit_if_possible = TRUE;
		}
		else
			c = and_nhgetch();
	}
	return c;
}

void and_you_die()
{
	JNICallV(jShowLog, 1);
	and_nhgetch();
}

//____________________________________________________________________________________
//int nh_poskey(int *x, int *y, int *mod)
//		-- Returns a single character input from the user or a
//		   a positioning event (perhaps from a mouse).  If the
//		   return value is non-zero, a character was typed, else,
//		   a position in the MAP window is returned in x, y and mod.
//		   mod may be one of
//
//			CLICK_1		/* mouse click type 1 */
//			CLICK_2		/* mouse click type 2 */
//
//		   The different click types can map to whatever the
//		   hardware supports.  If no mouse is supported, this
//		   routine always returns a non-zero character.

// hack: don't accept dir commands from touch events when mouse is locked. translate to tile position instead
static boolean bMouseLock;
void lock_mouse_cursor(boolean bLock)
{
	bMouseLock = bLock;
}

int and_nh_poskey(int *x, int *y, int *mod)
{
	//debuglog("and_nh_poskey");
	jintArray a = (*jEnv)->NewIntArray(jEnv, 2);
	int c = JNICallI(jReceivePosKey, bMouseLock, a);
	if(!c)
	{
		int* e = (*jEnv)->GetIntArrayElements(jEnv, a, 0);
		*x = e[0];
		*y = e[1];
		*mod = CLICK_1;
		(*jEnv)->ReleaseIntArrayElements(jEnv, a, e, 0);
	}
	quit_if_possible = FALSE;
	if(c == 0x80)
	{
		if(!program_state.gameover && program_state.something_worth_saving)
		{
			c = '\033';
			quit_if_possible = TRUE;
		}
		else
			c = and_nh_poskey(x, y, mod);
	}
	destroy_jobject(a);
	return c;
}

//____________________________________________________________________________________
//nhbell()	-- Beep at user.  [This will exist at least until sounds are
//		   redone, since sounds aren't attributable to windows anyway.]
void and_nhbell()
{
//	debuglog("and_nhbell");
}

//____________________________________________________________________________________
//doprev_message()
//		-- Display previous messages.  Used by the ^P command.
//		-- On the tty-port this scrolls WIN_MESSAGE back one line.
int and_doprev_message()
{
//	debuglog("and_doprev_message");
	JNICallV(jShowLog, 1);
	and_nhgetch();
	return 0;
}

//____________________________________________________________________________________
// char yn_function(const char *ques, const char *choices, char default)
//		-- Print a prompt made up of ques, choices and default.
//		   Read a single character response that is contained in
//		   choices or default.  If choices is NULL, all possible
//		   inputs are accepted and returned.  This overrides
//		   everything else.  The choices are expected to be in
//		   lower case.  Entering ESC always maps to 'q', or 'n',
//		   in that order, if present in choices, otherwise it maps
//		   to default.  Entering any other quit character (SPACE,
//		   RETURN, NEWLINE) maps to default.
//		-- If the choices string contains ESC, then anything after
//		   it is an acceptable response, but the ESC and whatever
//		   follows is not included in the prompt.
//		-- If the choices string contains a '#' then accept a count.
//		   Place this value in the global "yn_number" and return '#'.
//		-- This uses the top line in the tty window-port, other
//		   ports might use a popup.
//		-- If choices is NULL, all possible inputs are accepted and
//		   returned, preserving case (upper or lower.) This means that
//		   if the calling function needs an exact match, it must handle
//		   user input correctness itself.
char and_yn_function_ex(int style, int attr, int color, int glyph, const char* title, const char *question, const char *choices, CHAR_P def, const char* resp_desc, const char* introline, unsigned long ynflags)
{
	char ch;
	char message[BUFSZ];
	char res_ch[2];
	boolean digit_ok, allow_num;
	int esc;
	int nChoices;

	if(choices)
	{
		nChoices = strlen(choices);
		esc = (int)index(choices, '\033');
		if(esc)
			esc -= (int)choices;
		else
			esc = -1;
	}
	else
	{
		esc = -1;
	}
	allow_num = choices && index(choices, '#');

	//if(choices)
	//	debuglog("yn %s [%s](%c)", question, choices, def);
	//else
	//	debuglog("yn %s", question);

	if(iflags.force_invmenu && choices && nChoices <= 4 && esc < 0 && !allow_num)
	{
		int i;
		// pop up dialog
		jbyteArray jq = create_bytearray(question);
		jbyteArray jb = (*jEnv)->NewByteArray(jEnv, nChoices);
		jbyte* pTmp = (*jEnv)->GetByteArrayElements(jEnv, jb, 0);
		memcpy(pTmp, choices, nChoices);
		(*jEnv)->ReleaseByteArrayElements(jEnv, jb, pTmp, 0);
		JNICallV(jYNFunction, jq, jb, def);
		destroy_jobject(jq);
		destroy_jobject(jb);

		ch = and_nhgetch();
		return ch;
	}

	if(choices)
	{
		char choicebuf[QBUFSZ];

		strcpy(choicebuf, choices);
		if(esc >= 0)
		{
			/* anything beyond <esc> is hidden */
			choicebuf[esc] = '\0';
		}
		sprintf(message, "%s [%s]", question, choicebuf);
		if(def)
			sprintf(eos(message), "(%c) ", def);
	}
	else
	{
		strcpy(message, question);
		strcat(message, " ");
	}

	if(strstr(question, "what direction"))
	{
		// directional choice
		and_clear_nhwindow(WIN_MESSAGE);
		and_putstr(WIN_MESSAGE, ATR_BOLD, message);
		if(iflags.force_invmenu)
		{
			JNICallV(jShowDPad);
			ch = and_nhgetch();
			return ch;
		}
		else
		{
			int x = u.ux, y = u.uy, mod = 0;
			int ch = and_nh_poskey(&x, &y, &mod);
			if(!ch)
			{
				x -= u.ux;
				y -= u.uy;
		        if(x > 2*abs(y))
		            x = 1, y = 0;
		        else if(y > 2*abs(x))
		            x = 0, y = 1;
		        else if(x < -2*abs(y))
		            x = -1, y = 0;
		        else if(y < -2*abs(x))
		            x = 0, y = -1;
		        else
		            x = sgn(x), y = sgn(y);

		        if(x == 0 && y == 0)	/* map click on player to "rest" command */
		        	ch = '.';
		        else
		        	ch = xytod(x, y);
			}
			return ch;
		}
	}

	// and_clear_nhwindow(WIN_MESSAGE);
	and_putstr(WIN_MESSAGE, ATR_BOLD, message);

	ch = 0;
	do
	{
		ch = and_nhgetch();
		if(choices)
			ch = lowc(ch);
		else
			break; /* If choices is NULL, all possible inputs are accepted and returned. */

		digit_ok = allow_num && digit(ch);
		if(ch == '\033')
		{
			if(index(choices, 'q'))
				ch = 'q';
			else if(index(choices, 'n'))
				ch = 'n';
			else
				ch = def;
			break;
		}
		else if(index(quitchars, ch))
		{
			ch = def;
			break;
		}
		else if(!index(choices, ch) && !digit_ok)
		{
			and_nhbell();
			ch = (char)0;
			/* and try again... */
		}
		else if(ch == '#' || digit_ok)
		{
			char z, digit_string[2];
			int n_len = 0;
			long value = 0;
			and_putstr_ex(WIN_MESSAGE, 1<<ATR_BOLD, "#", 1, CLR_WHITE);
			n_len++;
			digit_string[1] = '\0';
			if(ch != '#')
			{
				digit_string[0] = ch;
				and_putstr_ex(WIN_MESSAGE, 1<<ATR_BOLD, digit_string, 1, CLR_WHITE);
				n_len++;
				value = ch - '0';
				ch = '#';
			}
			do
			{ /* loop until we get a non-digit */
				z = lowc(readchar());
				if(digit(z))
				{
					value = (10 * value) + (z - '0');
					if(value < 0)
						break; /* overflow: try again */
					digit_string[0] = z;
					and_putstr_ex(WIN_MESSAGE, 1<<ATR_BOLD, digit_string, 0, CLR_WHITE);
					n_len++;
				}
				else if(z == 'y' || index(quitchars, z))
				{
					if(z == '\033')
						value = -1; /* abort */
					z = '\n'; /* break */
				}
				else if(z == 0x7f)
				{
					if(n_len <= 1)
					{
						value = -1;
						break;
					}
					else
					{
						value /= 10;
						and_putstr_ex(WIN_MESSAGE, 1<<ATR_BOLD, digit_string, -2, CLR_WHITE);
						n_len--;
					}
				}
				else
				{
					value = -1; /* abort */
					and_nhbell();
					break;
				}
			}
			while(z != '\n');
			if(value > 0)
				yn_number = value;
			else if(value == 0)
				ch = 'n'; /* 0 => "no" */
			else
			{ /* remove number from top line, then try again */
				and_putstr_ex(WIN_MESSAGE, 1<<ATR_BOLD, digit_string, -n_len-1, CLR_WHITE);
				n_len = 0;
				ch = (char)0;
			}
		}
	}
	while(!ch);

	/* display selection in the message window */
	if(choices)
	{
		if(isprint(ch) && ch != '#')
		{
			res_ch[0] = ch;
			res_ch[1] = '\x0';
			and_putstr_ex(WIN_MESSAGE, 1<<ATR_BOLD, res_ch, 1, CLR_WHITE);
		}
	}
	else
	{
		and_clear_nhwindow(WIN_MESSAGE);
	}

	return ch;
}

//____________________________________________________________________________________
void and_n_getline(const char* question, char* buf, int nMax, int showLog)
{
	and_n_getline_r(question, buf, nMax, showLog, 0);
}

void and_n_getline_r(const char* question, char* buf, int nMax, int showLog, int reentry)
{
	int i, n;
	const jchar* pChars;
	jstring jstr;
	jbyteArray jq;

	jq = create_bytearray(question);
	jstr = (jstring)JNICallO(jGetLine, jq, nMax, showLog, reentry);
	destroy_jobject(jq);


	n = (*jEnv)->GetStringLength(jEnv, jstr);
	if(n >= nMax)
		n = nMax - 1;
    i = 0;
	if(n > 0)
	{
		pChars = (*jEnv)->GetStringChars(jEnv, jstr, 0);
	//debuglog("    returned %c %s", *pChars, pChars);
		if(*pChars == 0x80)
		{
			// special case: ABORT
			if(!program_state.gameover && program_state.something_worth_saving)
			{
				buf[0] = '\033';
				i = 1;
			}
			else
			{
				(*jEnv)->ReleaseStringChars(jEnv, jstr, pChars);
				destroy_jobject(jstr);
				and_n_getline_r(question, buf, nMax, showLog, 1);
				return;
			}
		}
		else if(*pChars == '\033')
		{
			buf[0] = '\033';
			i = 1;
		}
		else
		{
			for(; i < n; i++)
			{
				if(isprint(pChars[i]))
					buf[i] = pChars[i];
				else
					buf[i] = '?';
			}
		}
		(*jEnv)->ReleaseStringChars(jEnv, jstr, pChars);
	}
	destroy_jobject(jstr);
	buf[i] = 0;
}

//____________________________________________________________________________________
// getlin(const char *ques, char *input)
//		-- Prints ques as a prompt and reads a single line of text,
//		   up to a newline.  The string entered is returned without the
//		   newline.  ESC is used to cancel, in which case the string
//		   "\033\000" is returned.
//		-- getlin() must call flush_screen(1) before doing anything.
//		-- This uses the top line in the tty window-port, other
//		   ports might use a popup.
//		-- getlin() can assume the input buffer is at least BUFSZ
//		   bytes in size and must truncate inputs to fit, including
//		   the nul character.
void and_getlin_ex(int style, int attr, int color, const char *question, char *input, const char* placeholder, const char* linesuffix, const char* introline)
{
//	debuglog("and_getlin '%s'", question);
	char promptbuf[BUFSZ] = "";
	//Do not show introline
	//if (introline && *introline)
	//	Sprintf(promptbuf, "%s", introline);
	if (question)
	{
		if (*promptbuf)
			Strcat(promptbuf, " ");
		Sprintf(eos(promptbuf), "%s", question);
	}
	if (placeholder)
		Sprintf(eos(promptbuf), " [%s]", placeholder);
	if (linesuffix)
		Sprintf(eos(promptbuf), " %s", linesuffix);
	and_n_getline(promptbuf, input, BUFSZ, FALSE);
}

void and_getlin_log(const char *question, char *input)
{
	and_n_getline(question, input, BUFSZ, TRUE);
}

//____________________________________________________________________________________
//askname()	-- Ask the user for a player name.
void and_askname()
{
//	debuglog("ask name");

	int i, n, w;
	const jchar* pChars;
	jstring jstr;

	char** saves = get_saved_games();

	int nSaves = 0;
	while(saves && saves[nSaves])
		nSaves++;

	jclass stringClass = (*jEnv)->FindClass(jEnv, "java/lang/String");
	jobjectArray strings = (*jEnv)->NewObjectArray(jEnv, nSaves, stringClass, 0);
    for(i = 0; i < nSaves; i++)
    	(*jEnv)->SetObjectArrayElement(jEnv, strings, i, (*jEnv)->NewStringUTF(jEnv, saves[i]));

	jstr = (jstring)JNICallO(jAskName, PL_NSIZ, strings);

    for(i = 0; i < nSaves; i++)
    	destroy_jobject((*jEnv)->GetObjectArrayElement(jEnv, strings, i));
	destroy_jobject(strings);

	n = (*jEnv)->GetStringLength(jEnv, jstr) - 1;
	w = n;
	if(n >= PL_NSIZ)
		n = PL_NSIZ - 1;
    i = 0;
	if(n > 0)
	{
		pChars = (*jEnv)->GetStringChars(jEnv, jstr, 0);
		if(*pChars == 0x80 || *pChars == '\033')
		{
			clearlocks();
			and_exit_nhwindows("bye");
			nh_terminate(EXIT_SUCCESS);
		}

		if( pChars[w] == '1' )
			wizard = TRUE;

		for(; i < n; i++)
		{
			if(isprint(pChars[i]))
				plname[i] = pChars[i];
			else
				plname[i] = '?';
		}
		(*jEnv)->ReleaseStringChars(jEnv, jstr, pChars);
	}
	plname[i] = 0;
	destroy_jobject(jstr);
}

//____________________________________________________________________________________
//int get_ext_cmd(void)
//		-- Get an extended command in a window-port specific way.
//		   An index into extcmdlist[] is returned on a successful
//		   selection, -1 otherwise.
int do_ext_cmd_menu(BOOLEAN_P complete)
{
//	debuglog("and_get_ext_cmd");

	winid wid;
	int i, count, what, flgs;
	menu_item *selected = NULL;
	anything any;
	char accelerator = 'a', tmp_acc = 0;
	const char *ptr;

	wid = and_create_nhwindow_ex(NHW_MENU, 0, NO_GLYPH, zerocreatewindowinfo);
	and_start_menu_ex(wid, 0);
	for(i = 0; (ptr = extcmdlist[i].ef_txt); i++)
	{
		flgs = extcmdlist[i].flags;
		if((flgs & WIZMODECMD) && !wizard)
			continue;

		if(!complete && !(flgs & AUTOCOMPLETE) && !(flgs & WIZMODECMD))
			continue;

		any.a_int = i+1;
		and_add_menu(wid, NO_GLYPH, &any, accelerator, 0, ATR_NONE, ptr, FALSE);

		if(accelerator == 'z')
			accelerator = 'A';
		else if(accelerator == 'Z')
			accelerator = 0;
		else
			accelerator++;
	}
	any.a_int = i+1;
	if(!complete)
		and_add_menu(wid, NO_GLYPH, &any, '*', 0, ATR_NONE, "(list everything)", FALSE);
	and_end_menu_ex(wid, "Extended command", (const char*)0);
	count = and_select_menu(wid, PICK_ONE, &selected);
	what = count > 0 ? selected->item.a_int - 1 : -1;
	if(selected)
		free(selected);
	and_destroy_nhwindow(wid);

	return what == any.a_int-1 ? do_ext_cmd_menu(TRUE) : what;
}

const char* complete_ext_cmd(const char* base)
{
	int i, icmd = -1;

	for(i = 0; extcmdlist[i].ef_txt != (char *)0; i++)
	{
		if(!strncmpi(base, extcmdlist[i].ef_txt, strlen(base)))
		{
			if(icmd == -1)	/* no matches yet */
				icmd = i;
			else			/* more than 1 match */
			    return 0;
		}
	}

	if(icmd >= 0)
		return extcmdlist[icmd].ef_txt;

	return 0;
}

void get_ext_cmd_auto(const char *query, register char *bufp)
{
	register int n = 0, nl = 0;
	const char* complete = 0;
	register int c;
	const int maxc = COLNO >= BUFSZ ? BUFSZ-1 : COLNO;

	pline("%s ", query);
	bufp[n] = 0;
	for(;;)
	{
		c = and_nhgetch();
		if(c == EOF || c == '\n')
		{
			bufp[n] = 0;
			if(complete)
				strcpy(bufp, complete);
			save_msg(bufp);
			break;
		}
		if(c == '\033')
		{
			bufp[0] = c;
			bufp[1] = 0;
			break;
		}
		if(c == 0x7f)
		{
			if(n > 0)
				bufp[--n] = 0;
		}
		else if(' ' <= (unsigned char) c && n < maxc)
		{
			bufp[n] = c;
			bufp[++n] = 0;
		}
		complete = complete_ext_cmd(bufp);
		and_putstr_ex(WIN_MESSAGE, 0, bufp, -nl-1, CLR_WHITE);
		if(complete) {
			and_putstr_ex(WIN_MESSAGE, 1<<ATR_INVERSE, complete + n, 1, CLR_WHITE);
		}
		nl = complete ? strlen(complete) : n;
	}
	clear_nhwindow(WIN_MESSAGE);	/* clean up after ourselves */
}

/*
 * Read in an extended command, doing command line completion.  We
 * stop when we have found enough characters to make a unique command.
 */
int do_ext_cmd_text()
{
	int i;
	char buf[BUFSZ];

	get_ext_cmd_auto("#", buf);

	(void) mungspaces(buf);
	if (buf[0] == 0 || buf[0] == '\033') return -1;

	for (i = 0; extcmdlist[i].ef_txt != (char *)0; i++)
		if (!strcmpi(buf, extcmdlist[i].ef_txt)) break;

	if (!in_doagain) {
	    int j;
	    for (j = 0; buf[j]; j++)
			savech(buf[j]);
	    savech('\n');
	}

	if (extcmdlist[i].ef_txt == (char *)0) {
		pline("%s: unknown extended command.", buf);
		i = -1;
	}

	return i;
}

int and_get_ext_cmd()
{
	if(iflags.extmenu)
		return do_ext_cmd_menu(FALSE);
	return do_ext_cmd_text();
}

//____________________________________________________________________________________
//number_pad(state)
//		-- Initialize the number pad to the given state.
void and_number_pad(int state)
{
//	debuglog("and_number_pad(%d)", state);
	JNICallV(jSetNumPadOption, state);
}

//____________________________________________________________________________________
//delay_output()	-- Causes a visible delay of 50ms in the output.
//		   Conceptually, this is similar to wait_synch() followed
//		   by a nap(50ms), but allows asynchronous operation.
void and_delay_output()
{
//	debuglog("and_delay_output()");
	JNICallV(jDelayOutput);
}

void and_delay_output_milliseconds(int interval)
{
	//	debuglog("and_delay_output()");
	JNICallV(jDelayOutput);
}

void and_delay_output_intervals(int intervals)
{
	//	debuglog("and_delay_output()");
	JNICallV(jDelayOutput);
}

//____________________________________________________________________________________
#ifdef CHANGE_COLOR
void and_change_color(int color_number, long rgb, int reverse)
{
	// debuglog("and_change_color %d == 0x%X %s", color_number, rgb, reverse?" reverse":"");
	if(color_number >= 0 && color_number < CLR_MAX)
		palette[color_number] = 0xFF000000 | rgb;
}

//____________________________________________________________________________________
char* and_get_color_string()
{
//	debuglog("and_get_color_string");
	return "";
}
#endif

//____________________________________________________________________________________
//start_screen()	-- Only used on Unix tty ports, but must be declared for
//		   completeness.  Sets up the tty to work in full-screen
//		   graphics mode.  Look at win/tty/termcap.c for an
//		   example.  If your window-port does not need this function
//		   just declare an empty function.
void and_start_screen()
{
//	debuglog("and_start_screen");
}

//____________________________________________________________________________________
//end_screen()	-- Only used on Unix tty ports, but must be declared for
//		   completeness.  The complement of start_screen().
void and_end_screen()
{
//	debuglog("and_end_screen");
}

//____________________________________________________________________________________
// and_getmsghistory_ex(init)
// 		window ports can provide their own getmsghistory() routine to
// 		preserve message history between games. The routine is called
// 		repeatedly from the core save routine, and the window port is
// 		expected to successively return each message that it wants
// 		saved, starting with the oldest message first, finishing with
// 		the most recent. Return null pointer when finished.
int add_msghistory_idx(int idx)
{
	return (idx + 1) % (sizeof(msghistory)/sizeof(char*));
}
char* and_getmsghistory_ex(int* attr_ptr, int* color_ptr, BOOLEAN_P init)
{
	if(init)
	{
		msghistory_idx0 = msghistory_idx;
		while(1)
		{
			if(msghistory[msghistory_idx0])
				return msghistory[msghistory_idx0];
			msghistory_idx0 = add_msghistory_idx(msghistory_idx0);
			if(msghistory_idx0 == msghistory_idx)
				return 0;
		};
	}
	else
	{
		msghistory_idx0 = add_msghistory_idx(msghistory_idx0);
		if(msghistory_idx0 == msghistory_idx)
			return 0;
		return msghistory[msghistory_idx0];
	}
}

// and_putmsghistory(msg, restoring)
//		window ports can provide their own putmsghistory() routine
//		to load message history from a saved game. The routine is
//		called repeatedly from the core restore routine, starting
//		with the oldest saved message first, and finishing with
//		the latest. The window port routine is expected to load
//		the message recall buffers in such a way that the ordering
//		is preserved. The window port routine should make no
// 		assumptions about how many messages are forthcoming, nor
//		should it assume that another message will follow this
//		one, so it should keep all pointers/indexes intact at the
//		end of each call.
void and_putmsghistory_ex(const char *msg, int attr, int color, BOOLEAN_P restoring)
{
	if(!msg) return;
	if(restoring)
	{
//		debuglog("restore msghistory: %s", msg);
		restoring_msghistory = TRUE;
		and_putstr(WIN_MESSAGE, ATR_NONE, msg);
		restoring_msghistory = FALSE;
	}
	else
	{
//		debuglog("put msghistory: %s", msg);
	}
}

void save_msg(const char* msg)
{
	if(!msg || !*msg || !strcmp("Restoring save file...", msg))
		return;
	if(msghistory[msghistory_idx])
		free(msghistory[msghistory_idx]);
	msghistory[msghistory_idx] = strdup(msg);
	msghistory_idx = add_msghistory_idx(msghistory_idx);
}

int doshowlog()
{
//	debuglog("doshowlog");
	JNICallV(jShowLog, 0);
	return 0;
}

#ifdef USER_SOUNDS
void load_usersound(const char *filename)
{
	//debuglog("load_usersound(%s)", filename);
	jbyteArray jstr = create_bytearray(filename);
	JNICallV(jLoadSound, jstr);
	destroy_jobject(jstr);
}

void play_usersound(const char *filename, int volume)
{
	//debuglog("play_usersound(%s, %d)", filename, volume);
	jbyteArray jstr = create_bytearray(filename);
	JNICallV(jPlaySound, jstr, volume);
	destroy_jobject(jstr);
}
#endif

#ifdef DUMPLOG
void and_get_dumplog_dir(char* buf)
{
	int i, n;
	const jchar* pChars;
	jstring jstr;

	jstr = (jstring)JNICallO(jGetDumplogDir);
	n = (*jEnv)->GetStringLength(jEnv, jstr);

	if(n > 0 && n < BUFSZ - 1)
	{
		pChars = (*jEnv)->GetStringChars(jEnv, jstr, 0);
		for(i = 0; i < n; i++)
			buf[i] = pChars[i];
		(*jEnv)->ReleaseStringChars(jEnv, jstr, pChars);
		if(buf[n - 1] != '/')
			buf[n++] = '/';
	}
	else
		n = 0;
	buf[n] = 0;
	destroy_jobject(jstr);
}
#endif

