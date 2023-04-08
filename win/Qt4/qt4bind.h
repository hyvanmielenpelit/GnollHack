/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-03-17 */

// Copyright (c) Warwick Allison, 1999.
// Qt4 conversion copyright (c) Ray Chason, 2012-2014.
// NetHack may be freely redistributed.  See license for details.

// qt4bind.h -- bindings between the Qt 4 interface and the main code

#ifndef QT4BIND_H
#define QT4BIND_H

#include "qt4main.h"

namespace nethack_qt4 {

class NetHackQtClickBuffer;

#ifdef KDE
#define NetHackQtBindBase KApplication
#elif defined(QWS)
#define NetHackQtBindBase QPEApplication
#else
#define NetHackQtBindBase QApplication
#endif

class NetHackQtBind : NetHackQtBindBase {
private:
	// Single-instance preservation...
	NetHackQtBind(int& argc, char** argv);

	static NetHackQtBind* instance;

	static NetHackQtKeyBuffer keybuffer;
	static NetHackQtClickBuffer clickbuffer;

	static QFrame* splash;
	static NetHackQtMainWindow* main;

public:
	static void qt_init_nhwindows(int* argc, char** argv);
	static void qt_player_selection();
	static void qt_askname();
	static void qt_get_nh_event();
	static void qt_exit_nhwindows(const char *);
	static void qt_suspend_nhwindows(const char *);
	static void qt_resume_nhwindows();
	static winid qt_create_nhwindow_ex(int type, int style, int glyph, struct extended_create_window_info info);
	static void qt_clear_nhwindow(winid wid);
	static void qt_display_nhwindow(winid wid, BOOLEAN_P block);
	static void qt_destroy_nhwindow(winid wid);
	static void qt_curs(winid wid, int x, int y);
	static void qt_putstr(winid wid, int attr, const char *text);
	static void qt_putstr_ex(winid wid, int attr, const char* text, int app, int color);
	static void qt_putstr_ex2(winid wid, const char* text, const char* attrs, const char* colors, int attr, int color, int app);
	static void qt_putstr(winid wid, int attr, const std::string& text);
	static void qt_putstr(winid wid, int attr, const QString& text);
	static void qt_display_file(const char *filename, BOOLEAN_P must_exist);
	static void qt_start_menu_ex(winid wid, int style);
	static void qt_add_menu(winid wid, int glyph,
		const ANY_P * identifier, CHAR_P ch, CHAR_P gch, int attr, int color,
		const char *str, BOOLEAN_P presel);
	static void qt_add_extended_menu(winid wid, int glyph,
		const ANY_P* identifier, struct extended_menu_info info, CHAR_P ch, CHAR_P gch, int attr, int color,
		const char* str, BOOLEAN_P presel);
	static void qt_end_menu_ex(winid wid, const char *prompt, const char* subtitle);
	static int qt_select_menu(winid wid, int how, MENU_ITEM_P **menu_list);
	static void qt_update_inventory();
	static void qt_mark_synch();
	static void qt_wait_synch();

	static void qt_cliparound(int x, int y, BOOLEAN_P force);
	static void qt_cliparound_window(winid wid, int x, int y);
	static void qt_print_glyph(winid wid,XCHAR_P x,XCHAR_P y,struct layer_info layers);
	static void qt_issue_gui_command(int initid);
	static void qt_raw_print(const char *str);
	static void qt_raw_print_bold(const char *str);
	static int qt_nhgetch();
	static int qt_nh_poskey(int *x, int *y, int *mod);
	static void qt_nhbell();
	static int qt_doprev_message();
	static char qt_yn_function_ex(int style, int attr, int color, int glyph, const char* title, const char *question, const char *choices, CHAR_P def, const char* resp_desc, const char* introline, unsigned long ynflags);
	static void qt_getlin_ex(int style, int attr, int color, const char *prompt, char *line, const char* placeholder, const char* linesuffix, const char* introline);
	static int qt_get_ext_cmd();
	static void qt_number_pad(int);
	static void qt_delay_output();
	static void qt_delay_output_milliseconds(int);
	static void qt_delay_output_intervals(int);
	static void qt_start_screen();
	static void qt_end_screen();

        static char *qt_getmsghistory_ex(char** attrs_ptr, char** colors_ptr, BOOLEAN_P init);
        static void qt_putmsghistory_ex(const char *msg, const char* attrs, const char* colors, BOOLEAN_P is_restoring);

	static void qt_outrip(winid wid, int how, time_t when);
	static int qt_kbhit();

	static QWidget *mainWidget() { return main; }

private:
	virtual bool notify(QObject *receiver, QEvent *event);

        static QStringList *msgs_strings;
        static boolean msgs_saved;
        static boolean msgs_initd;
};

} // namespace nethack_qt4

#endif
