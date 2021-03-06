/*
 * sdlnuklear.c
 * GnollHack Nuklear Routines
 *
 * Copyright (c) Janne Gustafsson, 2021
 */

#ifdef GL2_GRAPHICS
#pragma comment( lib, "OpenGL32" )          
#endif
#ifdef GLES2_GRAPHICS
#pragma comment( lib, "OpenGLES2" )  // Put relevant OpenGLES2 library here        
#endif

#include "win10.h"
#include <windows.h>
#include "resource.h"
#include "hack.h"
#include "patchlevel.h"
#include "sdlnuklear.h"

 /* nuklear - 1.32.0 - public domain */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#if defined(SDL_GRAPHICS)
#include <SDL.h>
#if defined(GL2_GRAPHICS)
#include <SDL_opengl.h>
#elif defined(GLES2_GRAPHICS)
#include <SDL_opengles2.h>
#endif
#endif

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#include "nuklear.h"

#if defined(SDL_GRAPHICS)
#if defined(GL2_GRAPHICS)
#define NK_SDL_GL2_IMPLEMENTATION
#include "nuklear_sdl_gl2.h"
#elif defined(GLES2_GRAPHICS)
#define NK_SDL_GLES2_IMPLEMENTATION
#include "nuklear_sdl_gles2.h"
#endif
#endif

#include "sdlmisc.h"
#include "sdlproc.h"

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024


int shutdown_nuklear();
static boolean plselRandomize();

#ifdef NK_SDL_GL2_IMPLEMENTATION
/* ===============================================================
 *
 *                          EXAMPLE
 *
 * ===============================================================*/

SDL_GLContext glContext = 0;

struct media {
    GLint skin;
    GLint skin_buttons;
    GLint skin_window;
    GLint skin_cursors;
    /*
    SDL_Surface* surface_skin;
    SDL_Surface* surface_skin_buttons;
    SDL_Surface* surface_skin_window;
    SDL_Surface* surface_skin_cursors;
    */
    struct nk_image menu;
    struct nk_image check;
    struct nk_image check_cursor;
    struct nk_image option;
    struct nk_image option_cursor;
    struct nk_image header;
    struct nk_image window;
    struct nk_image scrollbar_inc_button;
    struct nk_image scrollbar_inc_button_hover;
    struct nk_image scrollbar_dec_button;
    struct nk_image scrollbar_dec_button_hover;
    struct nk_image button;
    struct nk_image button_hover;
    struct nk_image button_active;
    struct nk_image tab_minimize;
    struct nk_image tab_maximize;
    struct nk_image slider;
    struct nk_image slider_hover;
    struct nk_image slider_active;

    struct nk_font* diablo24;
    struct nk_font* diablo30;
    struct nk_font* diablo36;

    struct nk_cursor custom_cursor;
};

    static int
    overview(struct nk_context* ctx)
    {
        /* window flags */
        static int show_menu = nk_true;
        static int titlebar = nk_true;
        static int border = nk_true;
        static int resize = nk_true;
        static int movable = nk_true;
        static int no_scrollbar = nk_false;
        static int scale_left = nk_false;
        static nk_flags window_flags = 0;
        static int minimizable = nk_true;

        /* popups */
        static enum nk_style_header_align header_align = NK_HEADER_RIGHT;
        static int show_app_about = nk_false;

        /* window flags */
        window_flags = 0;
        ctx->style.window.header.align = header_align;
        if (border) window_flags |= NK_WINDOW_BORDER;
        if (resize) window_flags |= NK_WINDOW_SCALABLE;
        if (movable) window_flags |= NK_WINDOW_MOVABLE;
        if (no_scrollbar) window_flags |= NK_WINDOW_NO_SCROLLBAR;
        if (scale_left) window_flags |= NK_WINDOW_SCALE_LEFT;
        if (minimizable) window_flags |= NK_WINDOW_MINIMIZABLE;

        if (nk_begin(ctx, "Overview", nk_rect(10, 10, 500, 600), window_flags))
        {
            if (show_menu)
            {
                /* menubar */
                enum menu_states { MENU_DEFAULT, MENU_WINDOWS };
                static nk_size mprog = 60;
                static int mslider = 10;
                static int mcheck = nk_true;
                nk_menubar_begin(ctx);

                /* menu #1 */
                nk_layout_row_begin(ctx, NK_STATIC, 25, 5);
                nk_layout_row_push(ctx, 45);
                if (nk_menu_begin_label(ctx, "MENU", NK_TEXT_LEFT, nk_vec2(120, 200)))
                {
                    static size_t prog = 40;
                    static int slider = 10;
                    static int check = nk_true;
                    nk_layout_row_dynamic(ctx, 25, 1);
                    if (nk_menu_item_label(ctx, "Hide", NK_TEXT_LEFT))
                        show_menu = nk_false;
                    if (nk_menu_item_label(ctx, "About", NK_TEXT_LEFT))
                        show_app_about = nk_true;
                    nk_progress(ctx, &prog, 100, NK_MODIFIABLE);
                    nk_slider_int(ctx, 0, &slider, 16, 1);
                    nk_checkbox_label(ctx, "check", &check);
                    nk_menu_end(ctx);
                }
                /* menu #2 */
                nk_layout_row_push(ctx, 60);
                if (nk_menu_begin_label(ctx, "ADVANCED", NK_TEXT_LEFT, nk_vec2(200, 600)))
                {
                    enum menu_state { MENU_NONE, MENU_FILE, MENU_EDIT, MENU_VIEW, MENU_CHART };
                    static enum menu_state menu_state = MENU_NONE;
                    enum nk_collapse_states state;

                    state = (menu_state == MENU_FILE) ? NK_MAXIMIZED : NK_MINIMIZED;
                    if (nk_tree_state_push(ctx, NK_TREE_TAB, "FILE", &state)) {
                        menu_state = MENU_FILE;
                        nk_menu_item_label(ctx, "New", NK_TEXT_LEFT);
                        nk_menu_item_label(ctx, "Open", NK_TEXT_LEFT);
                        nk_menu_item_label(ctx, "Save", NK_TEXT_LEFT);
                        nk_menu_item_label(ctx, "Close", NK_TEXT_LEFT);
                        nk_menu_item_label(ctx, "Exit", NK_TEXT_LEFT);
                        nk_tree_pop(ctx);
                    }
                    else menu_state = (menu_state == MENU_FILE) ? MENU_NONE : menu_state;

                    state = (menu_state == MENU_EDIT) ? NK_MAXIMIZED : NK_MINIMIZED;
                    if (nk_tree_state_push(ctx, NK_TREE_TAB, "EDIT", &state)) {
                        menu_state = MENU_EDIT;
                        nk_menu_item_label(ctx, "Copy", NK_TEXT_LEFT);
                        nk_menu_item_label(ctx, "Delete", NK_TEXT_LEFT);
                        nk_menu_item_label(ctx, "Cut", NK_TEXT_LEFT);
                        nk_menu_item_label(ctx, "Paste", NK_TEXT_LEFT);
                        nk_tree_pop(ctx);
                    }
                    else menu_state = (menu_state == MENU_EDIT) ? MENU_NONE : menu_state;

                    state = (menu_state == MENU_VIEW) ? NK_MAXIMIZED : NK_MINIMIZED;
                    if (nk_tree_state_push(ctx, NK_TREE_TAB, "VIEW", &state)) {
                        menu_state = MENU_VIEW;
                        nk_menu_item_label(ctx, "About", NK_TEXT_LEFT);
                        nk_menu_item_label(ctx, "Options", NK_TEXT_LEFT);
                        nk_menu_item_label(ctx, "Customize", NK_TEXT_LEFT);
                        nk_tree_pop(ctx);
                    }
                    else menu_state = (menu_state == MENU_VIEW) ? MENU_NONE : menu_state;

                    state = (menu_state == MENU_CHART) ? NK_MAXIMIZED : NK_MINIMIZED;
                    if (nk_tree_state_push(ctx, NK_TREE_TAB, "CHART", &state)) {
                        size_t i = 0;
                        const float values[] = { 26.0f,13.0f,30.0f,15.0f,25.0f,10.0f,20.0f,40.0f,12.0f,8.0f,22.0f,28.0f };
                        menu_state = MENU_CHART;
                        nk_layout_row_dynamic(ctx, 150, 1);
                        nk_chart_begin(ctx, NK_CHART_COLUMN, NK_LEN(values), 0, 50);
                        for (i = 0; i < NK_LEN(values); ++i)
                            nk_chart_push(ctx, values[i]);
                        nk_chart_end(ctx);
                        nk_tree_pop(ctx);
                    }
                    else menu_state = (menu_state == MENU_CHART) ? MENU_NONE : menu_state;
                    nk_menu_end(ctx);
                }
                /* menu widgets */
                nk_layout_row_push(ctx, 70);
                nk_progress(ctx, &mprog, 100, NK_MODIFIABLE);
                nk_slider_int(ctx, 0, &mslider, 16, 1);
                nk_checkbox_label(ctx, "check", &mcheck);
                nk_menubar_end(ctx);
            }

            if (show_app_about)
            {
                /* about popup */
                static struct nk_rect s = { 20, 100, 300, 190 };
                if (nk_popup_begin(ctx, NK_POPUP_STATIC, "About", NK_WINDOW_CLOSABLE, s))
                {
                    nk_layout_row_dynamic(ctx, 20, 1);
                    nk_label(ctx, "Nuklear", NK_TEXT_LEFT);
                    nk_label(ctx, "By Micha Mettke", NK_TEXT_LEFT);
                    nk_label(ctx, "nuklear is licensed under the public domain License.", NK_TEXT_LEFT);
                    nk_popup_end(ctx);
                }
                else show_app_about = nk_false;
            }

            /* window flags */
            if (nk_tree_push(ctx, NK_TREE_TAB, "Window", NK_MINIMIZED)) {
                nk_layout_row_dynamic(ctx, 30, 2);
                nk_checkbox_label(ctx, "Titlebar", &titlebar);
                nk_checkbox_label(ctx, "Menu", &show_menu);
                nk_checkbox_label(ctx, "Border", &border);
                nk_checkbox_label(ctx, "Resizable", &resize);
                nk_checkbox_label(ctx, "Movable", &movable);
                nk_checkbox_label(ctx, "No Scrollbar", &no_scrollbar);
                nk_checkbox_label(ctx, "Minimizable", &minimizable);
                nk_checkbox_label(ctx, "Scale Left", &scale_left);
                nk_tree_pop(ctx);
            }

            if (nk_tree_push(ctx, NK_TREE_TAB, "Widgets", NK_MINIMIZED))
            {
                enum options { A, B, C };
                static int checkbox;
                static int option;
                if (nk_tree_push(ctx, NK_TREE_NODE, "Text", NK_MINIMIZED))
                {
                    /* Text Widgets */
                    nk_layout_row_dynamic(ctx, 20, 1);
                    nk_label(ctx, "Label aligned left", NK_TEXT_LEFT);
                    nk_label(ctx, "Label aligned centered", NK_TEXT_CENTERED);
                    nk_label(ctx, "Label aligned right", NK_TEXT_RIGHT);
                    nk_label_colored(ctx, "Blue text", NK_TEXT_LEFT, nk_rgb(0, 0, 255));
                    nk_label_colored(ctx, "Yellow text", NK_TEXT_LEFT, nk_rgb(255, 255, 0));
                    nk_text(ctx, "Text without /0", 15, NK_TEXT_RIGHT);

                    nk_layout_row_static(ctx, 100, 200, 1);
                    nk_label_wrap(ctx, "This is a very long line to hopefully get this text to be wrapped into multiple lines to show line wrapping");
                    nk_layout_row_dynamic(ctx, 100, 1);
                    nk_label_wrap(ctx, "This is another long text to show dynamic window changes on multiline text");
                    nk_tree_pop(ctx);
                }

                if (nk_tree_push(ctx, NK_TREE_NODE, "Button", NK_MINIMIZED))
                {
                    /* Buttons Widgets */
                    nk_layout_row_static(ctx, 30, 100, 3);
                    if (nk_button_label(ctx, "Button"))
                        fprintf(stdout, "Button pressed!\n");
                    nk_button_set_behavior(ctx, NK_BUTTON_REPEATER);
                    if (nk_button_label(ctx, "Repeater"))
                        fprintf(stdout, "Repeater is being pressed!\n");
                    nk_button_set_behavior(ctx, NK_BUTTON_DEFAULT);
                    nk_button_color(ctx, nk_rgb(0, 0, 255));

                    nk_layout_row_static(ctx, 25, 25, 8);
                    nk_button_symbol(ctx, NK_SYMBOL_CIRCLE_SOLID);
                    nk_button_symbol(ctx, NK_SYMBOL_CIRCLE_OUTLINE);
                    nk_button_symbol(ctx, NK_SYMBOL_RECT_SOLID);
                    nk_button_symbol(ctx, NK_SYMBOL_RECT_OUTLINE);
                    nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_UP);
                    nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_DOWN);
                    nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_LEFT);
                    nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_RIGHT);

                    nk_layout_row_static(ctx, 30, 100, 2);
                    nk_button_symbol_label(ctx, NK_SYMBOL_TRIANGLE_LEFT, "prev", NK_TEXT_RIGHT);
                    nk_button_symbol_label(ctx, NK_SYMBOL_TRIANGLE_RIGHT, "next", NK_TEXT_LEFT);
                    nk_tree_pop(ctx);
                }

                if (nk_tree_push(ctx, NK_TREE_NODE, "Basic", NK_MINIMIZED))
                {
                    /* Basic widgets */
                    static int int_slider = 5;
                    static float float_slider = 2.5f;
                    static size_t prog_value = 40;
                    static float property_float = 2;
                    static int property_int = 10;
                    static int property_neg = 10;

                    static float range_float_min = 0;
                    static float range_float_max = 100;
                    static float range_float_value = 50;
                    static int range_int_min = 0;
                    static int range_int_value = 2048;
                    static int range_int_max = 4096;
                    static const float ratio[] = { 120, 150 };

                    nk_layout_row_static(ctx, 30, 100, 1);
                    nk_checkbox_label(ctx, "Checkbox", &checkbox);

                    nk_layout_row_static(ctx, 30, 80, 3);
                    option = nk_option_label(ctx, "optionA", option == A) ? A : option;
                    option = nk_option_label(ctx, "optionB", option == B) ? B : option;
                    option = nk_option_label(ctx, "optionC", option == C) ? C : option;

                    nk_layout_row(ctx, NK_STATIC, 30, 2, ratio);
                    nk_labelf(ctx, NK_TEXT_LEFT, "Slider int");
                    nk_slider_int(ctx, 0, &int_slider, 10, 1);

                    nk_label(ctx, "Slider float", NK_TEXT_LEFT);
                    nk_slider_float(ctx, 0, &float_slider, 5.0, 0.5f);
                    nk_labelf(ctx, NK_TEXT_LEFT, "Progressbar: %zu", prog_value);
                    nk_progress(ctx, &prog_value, 100, NK_MODIFIABLE);

                    nk_layout_row(ctx, NK_STATIC, 25, 2, ratio);
                    nk_label(ctx, "Property float:", NK_TEXT_LEFT);
                    nk_property_float(ctx, "Float:", 0, &property_float, 64.0f, 0.1f, 0.2f);
                    nk_label(ctx, "Property int:", NK_TEXT_LEFT);
                    nk_property_int(ctx, "Int:", 0, &property_int, 100, 1, 1);
                    nk_label(ctx, "Property neg:", NK_TEXT_LEFT);
                    nk_property_int(ctx, "Neg:", -10, &property_neg, 10, 1, 1);

                    nk_layout_row_dynamic(ctx, 25, 1);
                    nk_label(ctx, "Range:", NK_TEXT_LEFT);
                    nk_layout_row_dynamic(ctx, 25, 3);
                    nk_property_float(ctx, "#min:", 0, &range_float_min, range_float_max, 1.0f, 0.2f);
                    nk_property_float(ctx, "#float:", range_float_min, &range_float_value, range_float_max, 1.0f, 0.2f);
                    nk_property_float(ctx, "#max:", range_float_min, &range_float_max, 100, 1.0f, 0.2f);

                    nk_property_int(ctx, "#min:", INT_MIN, &range_int_min, range_int_max, 1, 10);
                    nk_property_int(ctx, "#neg:", range_int_min, &range_int_value, range_int_max, 1, 10);
                    nk_property_int(ctx, "#max:", range_int_min, &range_int_max, INT_MAX, 1, 10);

                    nk_tree_pop(ctx);
                }

                if (nk_tree_push(ctx, NK_TREE_NODE, "Inactive", NK_MINIMIZED))
                {
                    static int inactive = 1;
                    nk_layout_row_dynamic(ctx, 30, 1);
                    nk_checkbox_label(ctx, "Inactive", &inactive);

                    nk_layout_row_static(ctx, 30, 80, 1);
                    if (inactive) {
                        struct nk_style_button button;
                        button = ctx->style.button;
                        ctx->style.button.normal = nk_style_item_color(nk_rgb(40, 40, 40));
                        ctx->style.button.hover = nk_style_item_color(nk_rgb(40, 40, 40));
                        ctx->style.button.active = nk_style_item_color(nk_rgb(40, 40, 40));
                        ctx->style.button.border_color = nk_rgb(60, 60, 60);
                        ctx->style.button.text_background = nk_rgb(60, 60, 60);
                        ctx->style.button.text_normal = nk_rgb(60, 60, 60);
                        ctx->style.button.text_hover = nk_rgb(60, 60, 60);
                        ctx->style.button.text_active = nk_rgb(60, 60, 60);
                        nk_button_label(ctx, "button");
                        ctx->style.button = button;
                    }
                    else if (nk_button_label(ctx, "button"))
                        fprintf(stdout, "button pressed\n");
                    nk_tree_pop(ctx);
                }


                if (nk_tree_push(ctx, NK_TREE_NODE, "Selectable", NK_MINIMIZED))
                {
                    if (nk_tree_push(ctx, NK_TREE_NODE, "List", NK_MINIMIZED))
                    {
                        static int selected[4] = { nk_false, nk_false, nk_true, nk_false };
                        nk_layout_row_static(ctx, 18, 100, 1);
                        nk_selectable_label(ctx, "Selectable", NK_TEXT_LEFT, &selected[0]);
                        nk_selectable_label(ctx, "Selectable", NK_TEXT_LEFT, &selected[1]);
                        nk_label(ctx, "Not Selectable", NK_TEXT_LEFT);
                        nk_selectable_label(ctx, "Selectable", NK_TEXT_LEFT, &selected[2]);
                        nk_selectable_label(ctx, "Selectable", NK_TEXT_LEFT, &selected[3]);
                        nk_tree_pop(ctx);
                    }
                    if (nk_tree_push(ctx, NK_TREE_NODE, "Grid", NK_MINIMIZED))
                    {
                        int i;
                        static int selected[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
                        nk_layout_row_static(ctx, 50, 50, 4);
                        for (i = 0; i < 16; ++i) {
                            if (nk_selectable_label(ctx, "Z", NK_TEXT_CENTERED, &selected[i])) {
                                int x = (i % 4), y = i / 4;
                                if (x > 0) selected[i - 1] ^= 1;
                                if (x < 3) selected[i + 1] ^= 1;
                                if (y > 0) selected[i - 4] ^= 1;
                                if (y < 3) selected[i + 4] ^= 1;
                            }
                        }
                        nk_tree_pop(ctx);
                    }
                    nk_tree_pop(ctx);
                }

                if (nk_tree_push(ctx, NK_TREE_NODE, "Combo", NK_MINIMIZED))
                {
                    /* Combobox Widgets
                     * In this library comboboxes are not limited to being a popup
                     * list of selectable text. Instead it is a abstract concept of
                     * having something that is *selected* or displayed, a popup window
                     * which opens if something needs to be modified and the content
                     * of the popup which causes the *selected* or displayed value to
                     * change or if wanted close the combobox.
                     *
                     * While strange at first handling comboboxes in a abstract way
                     * solves the problem of overloaded window content. For example
                     * changing a color value requires 4 value modifier (slider, property,...)
                     * for RGBA then you need a label and ways to display the current color.
                     * If you want to go fancy you even add rgb and hsv ratio boxes.
                     * While fine for one color if you have a lot of them it because
                     * tedious to look at and quite wasteful in space. You could add
                     * a popup which modifies the color but this does not solve the
                     * fact that it still requires a lot of cluttered space to do.
                     *
                     * In these kind of instance abstract comboboxes are quite handy. All
                     * value modifiers are hidden inside the combobox popup and only
                     * the color is shown if not open. This combines the clarity of the
                     * popup with the ease of use of just using the space for modifiers.
                     *
                     * Other instances are for example time and especially date picker,
                     * which only show the currently activated time/data and hide the
                     * selection logic inside the combobox popup.
                     */
                    static float chart_selection = 8.0f;
                    static int current_weapon = 0;
                    static int check_values[5];
                    static float position[3];
                    static struct nk_color combo_color = { 130, 50, 50, 255 };
                    static struct nk_colorf combo_color2 = { 0.509f, 0.705f, 0.2f, 1.0f };
                    static size_t prog_a = 20, prog_b = 40, prog_c = 10, prog_d = 90;
                    static const char* weapons[] = { "Fist","Pistol","Shotgun","Plasma","BFG" };

                    char buffer[64];
                    size_t sum = 0;

                    /* default combobox */
                    nk_layout_row_static(ctx, 25, 200, 1);
                    current_weapon = nk_combo(ctx, weapons, NK_LEN(weapons), current_weapon, 25, nk_vec2(200, 200));

                    /* slider color combobox */
                    if (nk_combo_begin_color(ctx, combo_color, nk_vec2(200, 200))) {
                        float ratios[] = { 0.15f, 0.85f };
                        nk_layout_row(ctx, NK_DYNAMIC, 30, 2, ratios);
                        nk_label(ctx, "R:", NK_TEXT_LEFT);
                        combo_color.r = (nk_byte)nk_slide_int(ctx, 0, combo_color.r, 255, 5);
                        nk_label(ctx, "G:", NK_TEXT_LEFT);
                        combo_color.g = (nk_byte)nk_slide_int(ctx, 0, combo_color.g, 255, 5);
                        nk_label(ctx, "B:", NK_TEXT_LEFT);
                        combo_color.b = (nk_byte)nk_slide_int(ctx, 0, combo_color.b, 255, 5);
                        nk_label(ctx, "A:", NK_TEXT_LEFT);
                        combo_color.a = (nk_byte)nk_slide_int(ctx, 0, combo_color.a, 255, 5);
                        nk_combo_end(ctx);
                    }
                    /* complex color combobox */
                    if (nk_combo_begin_color(ctx, nk_rgb_cf(combo_color2), nk_vec2(200, 400))) {
                        enum color_mode { COL_RGB, COL_HSV };
                        static int col_mode = COL_RGB;
    #ifndef DEMO_DO_NOT_USE_COLOR_PICKER
                        nk_layout_row_dynamic(ctx, 120, 1);
                        combo_color2 = nk_color_picker(ctx, combo_color2, NK_RGBA);
    #endif

                        nk_layout_row_dynamic(ctx, 25, 2);
                        col_mode = nk_option_label(ctx, "RGB", col_mode == COL_RGB) ? COL_RGB : col_mode;
                        col_mode = nk_option_label(ctx, "HSV", col_mode == COL_HSV) ? COL_HSV : col_mode;

                        nk_layout_row_dynamic(ctx, 25, 1);
                        if (col_mode == COL_RGB) {
                            combo_color2.r = nk_propertyf(ctx, "#R:", 0, combo_color2.r, 1.0f, 0.01f, 0.005f);
                            combo_color2.g = nk_propertyf(ctx, "#G:", 0, combo_color2.g, 1.0f, 0.01f, 0.005f);
                            combo_color2.b = nk_propertyf(ctx, "#B:", 0, combo_color2.b, 1.0f, 0.01f, 0.005f);
                            combo_color2.a = nk_propertyf(ctx, "#A:", 0, combo_color2.a, 1.0f, 0.01f, 0.005f);
                        }
                        else {
                            float hsva[4];
                            nk_colorf_hsva_fv(hsva, combo_color2);
                            hsva[0] = nk_propertyf(ctx, "#H:", 0, hsva[0], 1.0f, 0.01f, 0.05f);
                            hsva[1] = nk_propertyf(ctx, "#S:", 0, hsva[1], 1.0f, 0.01f, 0.05f);
                            hsva[2] = nk_propertyf(ctx, "#V:", 0, hsva[2], 1.0f, 0.01f, 0.05f);
                            hsva[3] = nk_propertyf(ctx, "#A:", 0, hsva[3], 1.0f, 0.01f, 0.05f);
                            combo_color2 = nk_hsva_colorfv(hsva);
                        }
                        nk_combo_end(ctx);
                    }
                    /* progressbar combobox */
                    sum = prog_a + prog_b + prog_c + prog_d;
                    sprintf(buffer, "%zu", sum);
                    if (nk_combo_begin_label(ctx, buffer, nk_vec2(200, 200))) {
                        nk_layout_row_dynamic(ctx, 30, 1);
                        nk_progress(ctx, &prog_a, 100, NK_MODIFIABLE);
                        nk_progress(ctx, &prog_b, 100, NK_MODIFIABLE);
                        nk_progress(ctx, &prog_c, 100, NK_MODIFIABLE);
                        nk_progress(ctx, &prog_d, 100, NK_MODIFIABLE);
                        nk_combo_end(ctx);
                    }

                    /* checkbox combobox */
                    sum = (size_t)(check_values[0] + check_values[1] + check_values[2] + check_values[3] + check_values[4]);
                    sprintf(buffer, "%zu", sum);
                    if (nk_combo_begin_label(ctx, buffer, nk_vec2(200, 200))) {
                        nk_layout_row_dynamic(ctx, 30, 1);
                        nk_checkbox_label(ctx, weapons[0], &check_values[0]);
                        nk_checkbox_label(ctx, weapons[1], &check_values[1]);
                        nk_checkbox_label(ctx, weapons[2], &check_values[2]);
                        nk_checkbox_label(ctx, weapons[3], &check_values[3]);
                        nk_combo_end(ctx);
                    }

                    /* complex text combobox */
                    sprintf(buffer, "%.2f, %.2f, %.2f", position[0], position[1], position[2]);
                    if (nk_combo_begin_label(ctx, buffer, nk_vec2(200, 200))) {
                        nk_layout_row_dynamic(ctx, 25, 1);
                        nk_property_float(ctx, "#X:", -1024.0f, &position[0], 1024.0f, 1, 0.5f);
                        nk_property_float(ctx, "#Y:", -1024.0f, &position[1], 1024.0f, 1, 0.5f);
                        nk_property_float(ctx, "#Z:", -1024.0f, &position[2], 1024.0f, 1, 0.5f);
                        nk_combo_end(ctx);
                    }

                    /* chart combobox */
                    sprintf(buffer, "%.1f", chart_selection);
                    if (nk_combo_begin_label(ctx, buffer, nk_vec2(200, 250))) {
                        size_t i = 0;
                        static const float values[] = { 26.0f,13.0f,30.0f,15.0f,25.0f,10.0f,20.0f,40.0f, 12.0f, 8.0f, 22.0f, 28.0f, 5.0f };
                        nk_layout_row_dynamic(ctx, 150, 1);
                        nk_chart_begin(ctx, NK_CHART_COLUMN, NK_LEN(values), 0, 50);
                        for (i = 0; i < NK_LEN(values); ++i) {
                            nk_flags res = nk_chart_push(ctx, values[i]);
                            if (res & NK_CHART_CLICKED) {
                                chart_selection = values[i];
                                nk_combo_close(ctx);
                            }
                        }
                        nk_chart_end(ctx);
                        nk_combo_end(ctx);
                    }

                    {
                        static int time_selected = 0;
                        static int date_selected = 0;
                        static struct tm sel_time;
                        static struct tm sel_date;
                        if (!time_selected || !date_selected) {
                            /* keep time and date updated if nothing is selected */
                            time_t cur_time = time(0);
                            struct tm* n = localtime(&cur_time);
                            if (!time_selected)
                                memcpy(&sel_time, n, sizeof(struct tm));
                            if (!date_selected)
                                memcpy(&sel_date, n, sizeof(struct tm));
                        }

                        /* time combobox */
                        sprintf(buffer, "%02d:%02d:%02d", sel_time.tm_hour, sel_time.tm_min, sel_time.tm_sec);
                        if (nk_combo_begin_label(ctx, buffer, nk_vec2(200, 250))) {
                            time_selected = 1;
                            nk_layout_row_dynamic(ctx, 25, 1);
                            sel_time.tm_sec = nk_propertyi(ctx, "#S:", 0, sel_time.tm_sec, 60, 1, 1);
                            sel_time.tm_min = nk_propertyi(ctx, "#M:", 0, sel_time.tm_min, 60, 1, 1);
                            sel_time.tm_hour = nk_propertyi(ctx, "#H:", 0, sel_time.tm_hour, 23, 1, 1);
                            nk_combo_end(ctx);
                        }

                        /* date combobox */
                        sprintf(buffer, "%02d-%02d-%02d", sel_date.tm_mday, sel_date.tm_mon + 1, sel_date.tm_year + 1900);
                        if (nk_combo_begin_label(ctx, buffer, nk_vec2(350, 400)))
                        {
                            int i = 0;
                            const char* month[] = { "January", "February", "March",
                                "April", "May", "June", "July", "August", "September",
                                "October", "November", "December" };
                            const char* week_days[] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
                            const int month_days[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
                            int year = sel_date.tm_year + 1900;
                            int leap_year = (!(year % 4) && ((year % 100))) || !(year % 400);
                            int days = (sel_date.tm_mon == 1) ?
                                month_days[sel_date.tm_mon] + leap_year :
                                month_days[sel_date.tm_mon];

                            /* header with month and year */
                            date_selected = 1;
                            nk_layout_row_begin(ctx, NK_DYNAMIC, 20, 3);
                            nk_layout_row_push(ctx, 0.05f);
                            if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_LEFT)) {
                                if (sel_date.tm_mon == 0) {
                                    sel_date.tm_mon = 11;
                                    sel_date.tm_year = NK_MAX(0, sel_date.tm_year - 1);
                                }
                                else sel_date.tm_mon--;
                            }
                            nk_layout_row_push(ctx, 0.9f);
                            sprintf(buffer, "%s %d", month[sel_date.tm_mon], year);
                            nk_label(ctx, buffer, NK_TEXT_CENTERED);
                            nk_layout_row_push(ctx, 0.05f);
                            if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_RIGHT)) {
                                if (sel_date.tm_mon == 11) {
                                    sel_date.tm_mon = 0;
                                    sel_date.tm_year++;
                                }
                                else sel_date.tm_mon++;
                            }
                            nk_layout_row_end(ctx);

                            /* good old week day formula (double because precision) */
                            {int year_n = (sel_date.tm_mon < 2) ? year - 1 : year;
                            int y = year_n % 100;
                            int c = year_n / 100;
                            int y4 = (int)((float)y / 4);
                            int c4 = (int)((float)c / 4);
                            int m = (int)(2.6 * (double)(((sel_date.tm_mon + 10) % 12) + 1) - 0.2);
                            int week_day = (((1 + m + y + y4 + c4 - 2 * c) % 7) + 7) % 7;

                            /* weekdays  */
                            nk_layout_row_dynamic(ctx, 35, 7);
                            for (i = 0; i < (int)NK_LEN(week_days); ++i)
                                nk_label(ctx, week_days[i], NK_TEXT_CENTERED);

                            /* days  */
                            if (week_day > 0) nk_spacing(ctx, week_day);
                            for (i = 1; i <= days; ++i) {
                                sprintf(buffer, "%d", i);
                                if (nk_button_label(ctx, buffer)) {
                                    sel_date.tm_mday = i;
                                    nk_combo_close(ctx);
                                }
                            }}
                            nk_combo_end(ctx);
                        }
                    }

                    nk_tree_pop(ctx);
                }

                if (nk_tree_push(ctx, NK_TREE_NODE, "Input", NK_MINIMIZED))
                {
                    static const float ratio[] = { 120, 150 };
                    static char field_buffer[64];
                    static char text[9][64];
                    static int text_len[9];
                    static char box_buffer[512];
                    static int field_len;
                    static int box_len;
                    nk_flags active;

                    nk_layout_row(ctx, NK_STATIC, 25, 2, ratio);
                    nk_label(ctx, "Default:", NK_TEXT_LEFT);

                    nk_edit_string(ctx, NK_EDIT_SIMPLE, text[0], &text_len[0], 64, nk_filter_default);
                    nk_label(ctx, "Int:", NK_TEXT_LEFT);
                    nk_edit_string(ctx, NK_EDIT_SIMPLE, text[1], &text_len[1], 64, nk_filter_decimal);
                    nk_label(ctx, "Float:", NK_TEXT_LEFT);
                    nk_edit_string(ctx, NK_EDIT_SIMPLE, text[2], &text_len[2], 64, nk_filter_float);
                    nk_label(ctx, "Hex:", NK_TEXT_LEFT);
                    nk_edit_string(ctx, NK_EDIT_SIMPLE, text[4], &text_len[4], 64, nk_filter_hex);
                    nk_label(ctx, "Octal:", NK_TEXT_LEFT);
                    nk_edit_string(ctx, NK_EDIT_SIMPLE, text[5], &text_len[5], 64, nk_filter_oct);
                    nk_label(ctx, "Binary:", NK_TEXT_LEFT);
                    nk_edit_string(ctx, NK_EDIT_SIMPLE, text[6], &text_len[6], 64, nk_filter_binary);

                    nk_label(ctx, "Password:", NK_TEXT_LEFT);
                    {
                        int i = 0;
                        int old_len = text_len[8];
                        char buffer[64];
                        for (i = 0; i < text_len[8]; ++i) buffer[i] = '*';
                        nk_edit_string(ctx, NK_EDIT_FIELD, buffer, &text_len[8], 64, nk_filter_default);
                        if (old_len < text_len[8])
                            memcpy(&text[8][old_len], &buffer[old_len], (nk_size)(text_len[8] - old_len));
                    }

                    nk_label(ctx, "Field:", NK_TEXT_LEFT);
                    nk_edit_string(ctx, NK_EDIT_FIELD, field_buffer, &field_len, 64, nk_filter_default);

                    nk_label(ctx, "Box:", NK_TEXT_LEFT);
                    nk_layout_row_static(ctx, 180, 278, 1);
                    nk_edit_string(ctx, NK_EDIT_BOX, box_buffer, &box_len, 512, nk_filter_default);

                    nk_layout_row(ctx, NK_STATIC, 25, 2, ratio);
                    active = nk_edit_string(ctx, NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, text[7], &text_len[7], 64, nk_filter_ascii);
                    if (nk_button_label(ctx, "Submit") ||
                        (active & NK_EDIT_COMMITED))
                    {
                        text[7][text_len[7]] = '\n';
                        text_len[7]++;
                        memcpy(&box_buffer[box_len], &text[7], (nk_size)text_len[7]);
                        box_len += text_len[7];
                        text_len[7] = 0;
                    }
                    nk_tree_pop(ctx);
                }
                nk_tree_pop(ctx);
            }

            if (nk_tree_push(ctx, NK_TREE_TAB, "Chart", NK_MINIMIZED))
            {
                /* Chart Widgets
                 * This library has two different rather simple charts. The line and the
                 * column chart. Both provide a simple way of visualizing values and
                 * have a retained mode and immediate mode API version. For the retain
                 * mode version `nk_plot` and `nk_plot_function` you either provide
                 * an array or a callback to call to handle drawing the graph.
                 * For the immediate mode version you start by calling `nk_chart_begin`
                 * and need to provide min and max values for scaling on the Y-axis.
                 * and then call `nk_chart_push` to push values into the chart.
                 * Finally `nk_chart_end` needs to be called to end the process. */
                float id = 0;
                static int col_index = -1;
                static int line_index = -1;
                float step = (2 * 3.141592654f) / 32;

                int i;
                int index = -1;
                struct nk_rect bounds;

                /* line chart */
                id = 0;
                index = -1;
                nk_layout_row_dynamic(ctx, 100, 1);
                bounds = nk_widget_bounds(ctx);
                if (nk_chart_begin(ctx, NK_CHART_LINES, 32, -1.0f, 1.0f)) {
                    for (i = 0; i < 32; ++i) {
                        nk_flags res = nk_chart_push(ctx, (float)cos(id));
                        if (res & NK_CHART_HOVERING)
                            index = (int)i;
                        if (res & NK_CHART_CLICKED)
                            line_index = (int)i;
                        id += step;
                    }
                    nk_chart_end(ctx);
                }

                if (index != -1)
                    nk_tooltipf(ctx, "Value: %.2f", (float)cos((float)index * step));
                if (line_index != -1) {
                    nk_layout_row_dynamic(ctx, 20, 1);
                    nk_labelf(ctx, NK_TEXT_LEFT, "Selected value: %.2f", (float)cos((float)index * step));
                }

                /* column chart */
                nk_layout_row_dynamic(ctx, 100, 1);
                bounds = nk_widget_bounds(ctx);
                if (nk_chart_begin(ctx, NK_CHART_COLUMN, 32, 0.0f, 1.0f)) {
                    for (i = 0; i < 32; ++i) {
                        nk_flags res = nk_chart_push(ctx, (float)fabs(sin(id)));
                        if (res & NK_CHART_HOVERING)
                            index = (int)i;
                        if (res & NK_CHART_CLICKED)
                            col_index = (int)i;
                        id += step;
                    }
                    nk_chart_end(ctx);
                }
                if (index != -1)
                    nk_tooltipf(ctx, "Value: %.2f", (float)fabs(sin(step * (float)index)));
                if (col_index != -1) {
                    nk_layout_row_dynamic(ctx, 20, 1);
                    nk_labelf(ctx, NK_TEXT_LEFT, "Selected value: %.2f", (float)fabs(sin(step * (float)col_index)));
                }

                /* mixed chart */
                nk_layout_row_dynamic(ctx, 100, 1);
                bounds = nk_widget_bounds(ctx);
                if (nk_chart_begin(ctx, NK_CHART_COLUMN, 32, 0.0f, 1.0f)) {
                    nk_chart_add_slot(ctx, NK_CHART_LINES, 32, -1.0f, 1.0f);
                    nk_chart_add_slot(ctx, NK_CHART_LINES, 32, -1.0f, 1.0f);
                    for (id = 0, i = 0; i < 32; ++i) {
                        nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
                        nk_chart_push_slot(ctx, (float)cos(id), 1);
                        nk_chart_push_slot(ctx, (float)sin(id), 2);
                        id += step;
                    }
                }
                nk_chart_end(ctx);

                /* mixed colored chart */
                nk_layout_row_dynamic(ctx, 100, 1);
                bounds = nk_widget_bounds(ctx);
                if (nk_chart_begin_colored(ctx, NK_CHART_LINES, nk_rgb(255, 0, 0), nk_rgb(150, 0, 0), 32, 0.0f, 1.0f)) {
                    nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0, 0, 255), nk_rgb(0, 0, 150), 32, -1.0f, 1.0f);
                    nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0, 255, 0), nk_rgb(0, 150, 0), 32, -1.0f, 1.0f);
                    for (id = 0, i = 0; i < 32; ++i) {
                        nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
                        nk_chart_push_slot(ctx, (float)cos(id), 1);
                        nk_chart_push_slot(ctx, (float)sin(id), 2);
                        id += step;
                    }
                }
                nk_chart_end(ctx);
                nk_tree_pop(ctx);
            }

            if (nk_tree_push(ctx, NK_TREE_TAB, "Popup", NK_MINIMIZED))
            {
                static struct nk_color color = { 255,0,0, 255 };
                static int select[4];
                static int popup_active;
                const struct nk_input* in = &ctx->input;
                struct nk_rect bounds;

                /* menu contextual */
                nk_layout_row_static(ctx, 30, 160, 1);
                bounds = nk_widget_bounds(ctx);
                nk_label(ctx, "Right click me for menu", NK_TEXT_LEFT);

                if (nk_contextual_begin(ctx, 0, nk_vec2(100, 300), bounds)) {
                    static size_t prog = 40;
                    static int slider = 10;

                    nk_layout_row_dynamic(ctx, 25, 1);
                    nk_checkbox_label(ctx, "Menu", &show_menu);
                    nk_progress(ctx, &prog, 100, NK_MODIFIABLE);
                    nk_slider_int(ctx, 0, &slider, 16, 1);
                    if (nk_contextual_item_label(ctx, "About", NK_TEXT_CENTERED))
                        show_app_about = nk_true;
                    nk_selectable_label(ctx, select[0] ? "Unselect" : "Select", NK_TEXT_LEFT, &select[0]);
                    nk_selectable_label(ctx, select[1] ? "Unselect" : "Select", NK_TEXT_LEFT, &select[1]);
                    nk_selectable_label(ctx, select[2] ? "Unselect" : "Select", NK_TEXT_LEFT, &select[2]);
                    nk_selectable_label(ctx, select[3] ? "Unselect" : "Select", NK_TEXT_LEFT, &select[3]);
                    nk_contextual_end(ctx);
                }

                /* color contextual */
                nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
                nk_layout_row_push(ctx, 120);
                nk_label(ctx, "Right Click here:", NK_TEXT_LEFT);
                nk_layout_row_push(ctx, 50);
                bounds = nk_widget_bounds(ctx);
                nk_button_color(ctx, color);
                nk_layout_row_end(ctx);

                if (nk_contextual_begin(ctx, 0, nk_vec2(350, 60), bounds)) {
                    nk_layout_row_dynamic(ctx, 30, 4);
                    color.r = (nk_byte)nk_propertyi(ctx, "#r", 0, color.r, 255, 1, 1);
                    color.g = (nk_byte)nk_propertyi(ctx, "#g", 0, color.g, 255, 1, 1);
                    color.b = (nk_byte)nk_propertyi(ctx, "#b", 0, color.b, 255, 1, 1);
                    color.a = (nk_byte)nk_propertyi(ctx, "#a", 0, color.a, 255, 1, 1);
                    nk_contextual_end(ctx);
                }

                /* popup */
                nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
                nk_layout_row_push(ctx, 120);
                nk_label(ctx, "Popup:", NK_TEXT_LEFT);
                nk_layout_row_push(ctx, 50);
                if (nk_button_label(ctx, "Popup"))
                    popup_active = 1;
                nk_layout_row_end(ctx);

                if (popup_active)
                {
                    static struct nk_rect s = { 20, 100, 220, 90 };
                    if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Error", 0, s))
                    {
                        nk_layout_row_dynamic(ctx, 25, 1);
                        nk_label(ctx, "A terrible error as occured", NK_TEXT_LEFT);
                        nk_layout_row_dynamic(ctx, 25, 2);
                        if (nk_button_label(ctx, "OK")) {
                            popup_active = 0;
                            nk_popup_close(ctx);
                        }
                        if (nk_button_label(ctx, "Cancel")) {
                            popup_active = 0;
                            nk_popup_close(ctx);
                        }
                        nk_popup_end(ctx);
                    }
                    else popup_active = nk_false;
                }

                /* tooltip */
                nk_layout_row_static(ctx, 30, 150, 1);
                bounds = nk_widget_bounds(ctx);
                nk_label(ctx, "Hover me for tooltip", NK_TEXT_LEFT);
                if (nk_input_is_mouse_hovering_rect(in, bounds))
                    nk_tooltip(ctx, "This is a tooltip");

                nk_tree_pop(ctx);
            }

            if (nk_tree_push(ctx, NK_TREE_TAB, "Layout", NK_MINIMIZED))
            {
                if (nk_tree_push(ctx, NK_TREE_NODE, "Widget", NK_MINIMIZED))
                {
                    float ratio_two[] = { 0.2f, 0.6f, 0.2f };
                    float width_two[] = { 100, 200, 50 };

                    nk_layout_row_dynamic(ctx, 30, 1);
                    nk_label(ctx, "Dynamic fixed column layout with generated position and size:", NK_TEXT_LEFT);
                    nk_layout_row_dynamic(ctx, 30, 3);
                    nk_button_label(ctx, "button");
                    nk_button_label(ctx, "button");
                    nk_button_label(ctx, "button");

                    nk_layout_row_dynamic(ctx, 30, 1);
                    nk_label(ctx, "static fixed column layout with generated position and size:", NK_TEXT_LEFT);
                    nk_layout_row_static(ctx, 30, 100, 3);
                    nk_button_label(ctx, "button");
                    nk_button_label(ctx, "button");
                    nk_button_label(ctx, "button");

                    nk_layout_row_dynamic(ctx, 30, 1);
                    nk_label(ctx, "Dynamic array-based custom column layout with generated position and custom size:", NK_TEXT_LEFT);
                    nk_layout_row(ctx, NK_DYNAMIC, 30, 3, ratio_two);
                    nk_button_label(ctx, "button");
                    nk_button_label(ctx, "button");
                    nk_button_label(ctx, "button");

                    nk_layout_row_dynamic(ctx, 30, 1);
                    nk_label(ctx, "Static array-based custom column layout with generated position and custom size:", NK_TEXT_LEFT);
                    nk_layout_row(ctx, NK_STATIC, 30, 3, width_two);
                    nk_button_label(ctx, "button");
                    nk_button_label(ctx, "button");
                    nk_button_label(ctx, "button");

                    nk_layout_row_dynamic(ctx, 30, 1);
                    nk_label(ctx, "Dynamic immediate mode custom column layout with generated position and custom size:", NK_TEXT_LEFT);
                    nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 3);
                    nk_layout_row_push(ctx, 0.2f);
                    nk_button_label(ctx, "button");
                    nk_layout_row_push(ctx, 0.6f);
                    nk_button_label(ctx, "button");
                    nk_layout_row_push(ctx, 0.2f);
                    nk_button_label(ctx, "button");
                    nk_layout_row_end(ctx);

                    nk_layout_row_dynamic(ctx, 30, 1);
                    nk_label(ctx, "Static immediate mode custom column layout with generated position and custom size:", NK_TEXT_LEFT);
                    nk_layout_row_begin(ctx, NK_STATIC, 30, 3);
                    nk_layout_row_push(ctx, 100);
                    nk_button_label(ctx, "button");
                    nk_layout_row_push(ctx, 200);
                    nk_button_label(ctx, "button");
                    nk_layout_row_push(ctx, 50);
                    nk_button_label(ctx, "button");
                    nk_layout_row_end(ctx);

                    nk_layout_row_dynamic(ctx, 30, 1);
                    nk_label(ctx, "Static free space with custom position and custom size:", NK_TEXT_LEFT);
                    nk_layout_space_begin(ctx, NK_STATIC, 60, 4);
                    nk_layout_space_push(ctx, nk_rect(100, 0, 100, 30));
                    nk_button_label(ctx, "button");
                    nk_layout_space_push(ctx, nk_rect(0, 15, 100, 30));
                    nk_button_label(ctx, "button");
                    nk_layout_space_push(ctx, nk_rect(200, 15, 100, 30));
                    nk_button_label(ctx, "button");
                    nk_layout_space_push(ctx, nk_rect(100, 30, 100, 30));
                    nk_button_label(ctx, "button");
                    nk_layout_space_end(ctx);

                    nk_layout_row_dynamic(ctx, 30, 1);
                    nk_label(ctx, "Row template:", NK_TEXT_LEFT);
                    nk_layout_row_template_begin(ctx, 30);
                    nk_layout_row_template_push_dynamic(ctx);
                    nk_layout_row_template_push_variable(ctx, 80);
                    nk_layout_row_template_push_static(ctx, 80);
                    nk_layout_row_template_end(ctx);
                    nk_button_label(ctx, "button");
                    nk_button_label(ctx, "button");
                    nk_button_label(ctx, "button");

                    nk_tree_pop(ctx);
                }

                if (nk_tree_push(ctx, NK_TREE_NODE, "Group", NK_MINIMIZED))
                {
                    static int group_titlebar = nk_false;
                    static int group_border = nk_true;
                    static int group_no_scrollbar = nk_false;
                    static int group_width = 320;
                    static int group_height = 200;

                    nk_flags group_flags = 0;
                    if (group_border) group_flags |= NK_WINDOW_BORDER;
                    if (group_no_scrollbar) group_flags |= NK_WINDOW_NO_SCROLLBAR;
                    if (group_titlebar) group_flags |= NK_WINDOW_TITLE;

                    nk_layout_row_dynamic(ctx, 30, 3);
                    nk_checkbox_label(ctx, "Titlebar", &group_titlebar);
                    nk_checkbox_label(ctx, "Border", &group_border);
                    nk_checkbox_label(ctx, "No Scrollbar", &group_no_scrollbar);

                    nk_layout_row_begin(ctx, NK_STATIC, 22, 3);
                    nk_layout_row_push(ctx, 50);
                    nk_label(ctx, "size:", NK_TEXT_LEFT);
                    nk_layout_row_push(ctx, 130);
                    nk_property_int(ctx, "#Width:", 100, &group_width, 500, 10, 1);
                    nk_layout_row_push(ctx, 130);
                    nk_property_int(ctx, "#Height:", 100, &group_height, 500, 10, 1);
                    nk_layout_row_end(ctx);

                    nk_layout_row_static(ctx, (float)group_height, group_width, 2);
                    if (nk_group_begin(ctx, "Group", group_flags)) {
                        int i = 0;
                        static int selected[16];
                        nk_layout_row_static(ctx, 18, 100, 1);
                        for (i = 0; i < 16; ++i)
                            nk_selectable_label(ctx, (selected[i]) ? "Selected" : "Unselected", NK_TEXT_CENTERED, &selected[i]);
                        nk_group_end(ctx);
                    }
                    nk_tree_pop(ctx);
                }
                if (nk_tree_push(ctx, NK_TREE_NODE, "Tree", NK_MINIMIZED))
                {
                    static int root_selected = 0;
                    int sel = root_selected;
                    if (nk_tree_element_push(ctx, NK_TREE_NODE, "Root", NK_MINIMIZED, &sel)) {
                        static int selected[8];
                        int i = 0, node_select = selected[0];
                        if (sel != root_selected) {
                            root_selected = sel;
                            for (i = 0; i < 8; ++i)
                                selected[i] = sel;
                        }
                        if (nk_tree_element_push(ctx, NK_TREE_NODE, "Node", NK_MINIMIZED, &node_select)) {
                            int j = 0;
                            static int sel_nodes[4];
                            if (node_select != selected[0]) {
                                selected[0] = node_select;
                                for (i = 0; i < 4; ++i)
                                    sel_nodes[i] = node_select;
                            }
                            nk_layout_row_static(ctx, 18, 100, 1);
                            for (j = 0; j < 4; ++j)
                                nk_selectable_symbol_label(ctx, NK_SYMBOL_CIRCLE_SOLID, (sel_nodes[j]) ? "Selected" : "Unselected", NK_TEXT_RIGHT, &sel_nodes[j]);
                            nk_tree_element_pop(ctx);
                        }
                        nk_layout_row_static(ctx, 18, 100, 1);
                        for (i = 1; i < 8; ++i)
                            nk_selectable_symbol_label(ctx, NK_SYMBOL_CIRCLE_SOLID, (selected[i]) ? "Selected" : "Unselected", NK_TEXT_RIGHT, &selected[i]);
                        nk_tree_element_pop(ctx);
                    }
                    nk_tree_pop(ctx);
                }
                if (nk_tree_push(ctx, NK_TREE_NODE, "Notebook", NK_MINIMIZED))
                {
                    static int current_tab = 0;
                    struct nk_rect bounds;
                    float step = (2 * 3.141592654f) / 32;
                    enum chart_type { CHART_LINE, CHART_HISTO, CHART_MIXED };
                    const char* names[] = { "Lines", "Columns", "Mixed" };
                    float id = 0;
                    int i;

                    /* Header */
                    nk_style_push_vec2(ctx, &ctx->style.window.spacing, nk_vec2(0, 0));
                    nk_style_push_float(ctx, &ctx->style.button.rounding, 0);
                    nk_layout_row_begin(ctx, NK_STATIC, 20, 3);
                    for (i = 0; i < 3; ++i) {
                        /* make sure button perfectly fits text */
                        const struct nk_user_font* f = ctx->style.font;
                        float text_width = f->width(f->userdata, f->height, names[i], nk_strlen(names[i]));
                        float widget_width = text_width + 3 * ctx->style.button.padding.x;
                        nk_layout_row_push(ctx, widget_width);
                        if (current_tab == i) {
                            /* active tab gets highlighted */
                            struct nk_style_item button_color = ctx->style.button.normal;
                            ctx->style.button.normal = ctx->style.button.active;
                            current_tab = nk_button_label(ctx, names[i]) ? i : current_tab;
                            ctx->style.button.normal = button_color;
                        }
                        else current_tab = nk_button_label(ctx, names[i]) ? i : current_tab;
                    }
                    nk_style_pop_float(ctx);

                    /* Body */
                    nk_layout_row_dynamic(ctx, 140, 1);
                    if (nk_group_begin(ctx, "Notebook", NK_WINDOW_BORDER))
                    {
                        nk_style_pop_vec2(ctx);
                        switch (current_tab) {
                        default: break;
                        case CHART_LINE:
                            nk_layout_row_dynamic(ctx, 100, 1);
                            bounds = nk_widget_bounds(ctx);
                            if (nk_chart_begin_colored(ctx, NK_CHART_LINES, nk_rgb(255, 0, 0), nk_rgb(150, 0, 0), 32, 0.0f, 1.0f)) {
                                nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0, 0, 255), nk_rgb(0, 0, 150), 32, -1.0f, 1.0f);
                                for (i = 0, id = 0; i < 32; ++i) {
                                    nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
                                    nk_chart_push_slot(ctx, (float)cos(id), 1);
                                    id += step;
                                }
                            }
                            nk_chart_end(ctx);
                            break;
                        case CHART_HISTO:
                            nk_layout_row_dynamic(ctx, 100, 1);
                            bounds = nk_widget_bounds(ctx);
                            if (nk_chart_begin_colored(ctx, NK_CHART_COLUMN, nk_rgb(255, 0, 0), nk_rgb(150, 0, 0), 32, 0.0f, 1.0f)) {
                                for (i = 0, id = 0; i < 32; ++i) {
                                    nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
                                    id += step;
                                }
                            }
                            nk_chart_end(ctx);
                            break;
                        case CHART_MIXED:
                            nk_layout_row_dynamic(ctx, 100, 1);
                            bounds = nk_widget_bounds(ctx);
                            if (nk_chart_begin_colored(ctx, NK_CHART_LINES, nk_rgb(255, 0, 0), nk_rgb(150, 0, 0), 32, 0.0f, 1.0f)) {
                                nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0, 0, 255), nk_rgb(0, 0, 150), 32, -1.0f, 1.0f);
                                nk_chart_add_slot_colored(ctx, NK_CHART_COLUMN, nk_rgb(0, 255, 0), nk_rgb(0, 150, 0), 32, 0.0f, 1.0f);
                                for (i = 0, id = 0; i < 32; ++i) {
                                    nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
                                    nk_chart_push_slot(ctx, (float)fabs(cos(id)), 1);
                                    nk_chart_push_slot(ctx, (float)fabs(sin(id)), 2);
                                    id += step;
                                }
                            }
                            nk_chart_end(ctx);
                            break;
                        }
                        nk_group_end(ctx);
                    }
                    else nk_style_pop_vec2(ctx);
                    nk_tree_pop(ctx);
                }

                if (nk_tree_push(ctx, NK_TREE_NODE, "Simple", NK_MINIMIZED))
                {
                    nk_layout_row_dynamic(ctx, 300, 2);
                    if (nk_group_begin(ctx, "Group_Without_Border", 0)) {
                        int i = 0;
                        char buffer[64];
                        nk_layout_row_static(ctx, 18, 150, 1);
                        for (i = 0; i < 64; ++i) {
                            sprintf(buffer, "0x%02x", i);
                            nk_labelf(ctx, NK_TEXT_LEFT, "%s: scrollable region", buffer);
                        }
                        nk_group_end(ctx);
                    }
                    if (nk_group_begin(ctx, "Group_With_Border", NK_WINDOW_BORDER)) {
                        int i = 0;
                        char buffer[64];
                        nk_layout_row_dynamic(ctx, 25, 2);
                        for (i = 0; i < 64; ++i) {
                            sprintf(buffer, "%08d", ((((i % 7) * 10) ^ 32)) + (64 + (i % 2) * 2));
                            nk_button_label(ctx, buffer);
                        }
                        nk_group_end(ctx);
                    }
                    nk_tree_pop(ctx);
                }

                if (nk_tree_push(ctx, NK_TREE_NODE, "Complex", NK_MINIMIZED))
                {
                    int i;
                    nk_layout_space_begin(ctx, NK_STATIC, 500, 64);
                    nk_layout_space_push(ctx, nk_rect(0, 0, 150, 500));
                    if (nk_group_begin(ctx, "Group_left", NK_WINDOW_BORDER)) {
                        static int selected[32];
                        nk_layout_row_static(ctx, 18, 100, 1);
                        for (i = 0; i < 32; ++i)
                            nk_selectable_label(ctx, (selected[i]) ? "Selected" : "Unselected", NK_TEXT_CENTERED, &selected[i]);
                        nk_group_end(ctx);
                    }

                    nk_layout_space_push(ctx, nk_rect(160, 0, 150, 240));
                    if (nk_group_begin(ctx, "Group_top", NK_WINDOW_BORDER)) {
                        nk_layout_row_dynamic(ctx, 25, 1);
                        nk_button_label(ctx, "#FFAA");
                        nk_button_label(ctx, "#FFBB");
                        nk_button_label(ctx, "#FFCC");
                        nk_button_label(ctx, "#FFDD");
                        nk_button_label(ctx, "#FFEE");
                        nk_button_label(ctx, "#FFFF");
                        nk_group_end(ctx);
                    }

                    nk_layout_space_push(ctx, nk_rect(160, 250, 150, 250));
                    if (nk_group_begin(ctx, "Group_buttom", NK_WINDOW_BORDER)) {
                        nk_layout_row_dynamic(ctx, 25, 1);
                        nk_button_label(ctx, "#FFAA");
                        nk_button_label(ctx, "#FFBB");
                        nk_button_label(ctx, "#FFCC");
                        nk_button_label(ctx, "#FFDD");
                        nk_button_label(ctx, "#FFEE");
                        nk_button_label(ctx, "#FFFF");
                        nk_group_end(ctx);
                    }

                    nk_layout_space_push(ctx, nk_rect(320, 0, 150, 150));
                    if (nk_group_begin(ctx, "Group_right_top", NK_WINDOW_BORDER)) {
                        static int selected[4];
                        nk_layout_row_static(ctx, 18, 100, 1);
                        for (i = 0; i < 4; ++i)
                            nk_selectable_label(ctx, (selected[i]) ? "Selected" : "Unselected", NK_TEXT_CENTERED, &selected[i]);
                        nk_group_end(ctx);
                    }

                    nk_layout_space_push(ctx, nk_rect(320, 160, 150, 150));
                    if (nk_group_begin(ctx, "Group_right_center", NK_WINDOW_BORDER)) {
                        static int selected[4];
                        nk_layout_row_static(ctx, 18, 100, 1);
                        for (i = 0; i < 4; ++i)
                            nk_selectable_label(ctx, (selected[i]) ? "Selected" : "Unselected", NK_TEXT_CENTERED, &selected[i]);
                        nk_group_end(ctx);
                    }

                    nk_layout_space_push(ctx, nk_rect(320, 320, 150, 150));
                    if (nk_group_begin(ctx, "Group_right_bottom", NK_WINDOW_BORDER)) {
                        static int selected[4];
                        nk_layout_row_static(ctx, 18, 100, 1);
                        for (i = 0; i < 4; ++i)
                            nk_selectable_label(ctx, (selected[i]) ? "Selected" : "Unselected", NK_TEXT_CENTERED, &selected[i]);
                        nk_group_end(ctx);
                    }
                    nk_layout_space_end(ctx);
                    nk_tree_pop(ctx);
                }

                if (nk_tree_push(ctx, NK_TREE_NODE, "Splitter", NK_MINIMIZED))
                {
                    const struct nk_input* in = &ctx->input;
                    nk_layout_row_static(ctx, 20, 320, 1);
                    nk_label(ctx, "Use slider and spinner to change tile size", NK_TEXT_LEFT);
                    nk_label(ctx, "Drag the space between tiles to change tile ratio", NK_TEXT_LEFT);

                    if (nk_tree_push(ctx, NK_TREE_NODE, "Vertical", NK_MINIMIZED))
                    {
                        static float a = 100, b = 100, c = 100;
                        struct nk_rect bounds;

                        float row_layout[5];
                        row_layout[0] = a;
                        row_layout[1] = 8;
                        row_layout[2] = b;
                        row_layout[3] = 8;
                        row_layout[4] = c;

                        /* header */
                        nk_layout_row_static(ctx, 30, 100, 2);
                        nk_label(ctx, "left:", NK_TEXT_LEFT);
                        nk_slider_float(ctx, 10.0f, &a, 200.0f, 10.0f);

                        nk_label(ctx, "middle:", NK_TEXT_LEFT);
                        nk_slider_float(ctx, 10.0f, &b, 200.0f, 10.0f);

                        nk_label(ctx, "right:", NK_TEXT_LEFT);
                        nk_slider_float(ctx, 10.0f, &c, 200.0f, 10.0f);

                        /* tiles */
                        nk_layout_row(ctx, NK_STATIC, 200, 5, row_layout);

                        /* left space */
                        if (nk_group_begin(ctx, "left", NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
                            nk_layout_row_dynamic(ctx, 25, 1);
                            nk_button_label(ctx, "#FFAA");
                            nk_button_label(ctx, "#FFBB");
                            nk_button_label(ctx, "#FFCC");
                            nk_button_label(ctx, "#FFDD");
                            nk_button_label(ctx, "#FFEE");
                            nk_button_label(ctx, "#FFFF");
                            nk_group_end(ctx);
                        }

                        /* scaler */
                        bounds = nk_widget_bounds(ctx);
                        nk_spacing(ctx, 1);
                        if ((nk_input_is_mouse_hovering_rect(in, bounds) ||
                            nk_input_is_mouse_prev_hovering_rect(in, bounds)) &&
                            nk_input_is_mouse_down(in, NK_BUTTON_LEFT))
                        {
                            a = row_layout[0] + in->mouse.delta.x;
                            b = row_layout[2] - in->mouse.delta.x;
                        }

                        /* middle space */
                        if (nk_group_begin(ctx, "center", NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
                            nk_layout_row_dynamic(ctx, 25, 1);
                            nk_button_label(ctx, "#FFAA");
                            nk_button_label(ctx, "#FFBB");
                            nk_button_label(ctx, "#FFCC");
                            nk_button_label(ctx, "#FFDD");
                            nk_button_label(ctx, "#FFEE");
                            nk_button_label(ctx, "#FFFF");
                            nk_group_end(ctx);
                        }

                        /* scaler */
                        bounds = nk_widget_bounds(ctx);
                        nk_spacing(ctx, 1);
                        if ((nk_input_is_mouse_hovering_rect(in, bounds) ||
                            nk_input_is_mouse_prev_hovering_rect(in, bounds)) &&
                            nk_input_is_mouse_down(in, NK_BUTTON_LEFT))
                        {
                            b = (row_layout[2] + in->mouse.delta.x);
                            c = (row_layout[4] - in->mouse.delta.x);
                        }

                        /* right space */
                        if (nk_group_begin(ctx, "right", NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
                            nk_layout_row_dynamic(ctx, 25, 1);
                            nk_button_label(ctx, "#FFAA");
                            nk_button_label(ctx, "#FFBB");
                            nk_button_label(ctx, "#FFCC");
                            nk_button_label(ctx, "#FFDD");
                            nk_button_label(ctx, "#FFEE");
                            nk_button_label(ctx, "#FFFF");
                            nk_group_end(ctx);
                        }

                        nk_tree_pop(ctx);
                    }

                    if (nk_tree_push(ctx, NK_TREE_NODE, "Horizontal", NK_MINIMIZED))
                    {
                        static float a = 100, b = 100, c = 100;
                        struct nk_rect bounds;

                        /* header */
                        nk_layout_row_static(ctx, 30, 100, 2);
                        nk_label(ctx, "top:", NK_TEXT_LEFT);
                        nk_slider_float(ctx, 10.0f, &a, 200.0f, 10.0f);

                        nk_label(ctx, "middle:", NK_TEXT_LEFT);
                        nk_slider_float(ctx, 10.0f, &b, 200.0f, 10.0f);

                        nk_label(ctx, "bottom:", NK_TEXT_LEFT);
                        nk_slider_float(ctx, 10.0f, &c, 200.0f, 10.0f);

                        /* top space */
                        nk_layout_row_dynamic(ctx, a, 1);
                        if (nk_group_begin(ctx, "top", NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER)) {
                            nk_layout_row_dynamic(ctx, 25, 3);
                            nk_button_label(ctx, "#FFAA");
                            nk_button_label(ctx, "#FFBB");
                            nk_button_label(ctx, "#FFCC");
                            nk_button_label(ctx, "#FFDD");
                            nk_button_label(ctx, "#FFEE");
                            nk_button_label(ctx, "#FFFF");
                            nk_group_end(ctx);
                        }

                        /* scaler */
                        nk_layout_row_dynamic(ctx, 8, 1);
                        bounds = nk_widget_bounds(ctx);
                        nk_spacing(ctx, 1);
                        if ((nk_input_is_mouse_hovering_rect(in, bounds) ||
                            nk_input_is_mouse_prev_hovering_rect(in, bounds)) &&
                            nk_input_is_mouse_down(in, NK_BUTTON_LEFT))
                        {
                            a = a + in->mouse.delta.y;
                            b = b - in->mouse.delta.y;
                        }

                        /* middle space */
                        nk_layout_row_dynamic(ctx, b, 1);
                        if (nk_group_begin(ctx, "middle", NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER)) {
                            nk_layout_row_dynamic(ctx, 25, 3);
                            nk_button_label(ctx, "#FFAA");
                            nk_button_label(ctx, "#FFBB");
                            nk_button_label(ctx, "#FFCC");
                            nk_button_label(ctx, "#FFDD");
                            nk_button_label(ctx, "#FFEE");
                            nk_button_label(ctx, "#FFFF");
                            nk_group_end(ctx);
                        }

                        {
                            /* scaler */
                            nk_layout_row_dynamic(ctx, 8, 1);
                            bounds = nk_widget_bounds(ctx);
                            if ((nk_input_is_mouse_hovering_rect(in, bounds) ||
                                nk_input_is_mouse_prev_hovering_rect(in, bounds)) &&
                                nk_input_is_mouse_down(in, NK_BUTTON_LEFT))
                            {
                                b = b + in->mouse.delta.y;
                                c = c - in->mouse.delta.y;
                            }
                        }

                        /* bottom space */
                        nk_layout_row_dynamic(ctx, c, 1);
                        if (nk_group_begin(ctx, "bottom", NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER)) {
                            nk_layout_row_dynamic(ctx, 25, 3);
                            nk_button_label(ctx, "#FFAA");
                            nk_button_label(ctx, "#FFBB");
                            nk_button_label(ctx, "#FFCC");
                            nk_button_label(ctx, "#FFDD");
                            nk_button_label(ctx, "#FFEE");
                            nk_button_label(ctx, "#FFFF");
                            nk_group_end(ctx);
                        }
                        nk_tree_pop(ctx);
                    }
                    nk_tree_pop(ctx);
                }
                nk_tree_pop(ctx);
            }
        }
        nk_end(ctx);
        return !nk_window_is_closed(ctx, "Overview");
    }


    static void
    calculator(struct nk_context* ctx)
    {
        if (nk_begin(ctx, "Calculator", nk_rect(10, 10, 180, 250),
            NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_MOVABLE))
        {
            static int set = 0, prev = 0, op = 0;
            static const char numbers[] = "789456123";
            static const char ops[] = "+-*/";
            static double a = 0, b = 0;
            static double* current = &a;

            size_t i = 0;
            int solve = 0;
            {int len; char buffer[256];
            nk_layout_row_dynamic(ctx, 35, 1);
            len = snprintf(buffer, 256, "%.2f", *current);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 255, nk_filter_float);
            buffer[len] = 0;
            *current = atof(buffer); }

            nk_layout_row_dynamic(ctx, 35, 4);
            for (i = 0; i < 16; ++i) {
                if (i >= 12 && i < 15) {
                    if (i > 12) continue;
                    if (nk_button_label(ctx, "C")) {
                        a = b = op = 0; current = &a; set = 0;
                    } if (nk_button_label(ctx, "0")) {
                        *current = *current * 10.0f; set = 0;
                    } if (nk_button_label(ctx, "=")) {
                        solve = 1; prev = op; op = 0;
                    }
                }
                else if (((i + 1) % 4)) {
                    if (nk_button_text(ctx, &numbers[(i / 4) * 3 + i % 4], 1)) {
                        *current = *current * 10.0f + numbers[(i / 4) * 3 + i % 4] - '0';
                        set = 0;
                    }
                }
                else if (nk_button_text(ctx, &ops[i / 4], 1)) {
                    if (!set) {
                        if (current != &b) {
                            current = &b;
                        }
                        else {
                            prev = op;
                            solve = 1;
                        }
                    }
                    op = ops[i / 4];
                    set = 1;
                }
            }
            if (solve) {
                if (prev == '+') a = a + b;
                if (prev == '-') a = a - b;
                if (prev == '*') a = a * b;
                if (prev == '/') a = a / b;
                current = &a;
                if (set) current = &b;
                b = 0; set = 0;
            }
        }
        nk_end(ctx);
    }


    /* GUI */
    struct nk_context* ctx;
    struct nk_colorf bg;
    struct media media;
    struct nk_font_atlas* atlas;

    int
    StartNuklearExample(HINSTANCE hInstance)
    {
        /* Platform */
        SDL_Window* win;
        int win_width, win_height;
        int running = 1;
    
        SDL_DisplayMode dm;
        SDL_GetDesktopDisplayMode(0, &dm);

        /* SDL setup */
        SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
        SDL_Init(SDL_INIT_VIDEO);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        win = SDL_CreateWindow("Demo",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            min(dm.w, 1200), min(dm.h, 800), SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
        glContext = SDL_GL_CreateContext(win);
        SDL_GetWindowSize(win, &win_width, &win_height);

        /* GUI */
        ctx = nk_sdl_init(win);
        /* Load Fonts: if none of these are loaded a default font will be used  */
        /* Load Cursor: if you uncomment cursor loading please hide the cursor */
        {struct nk_font_atlas* atlas;
        nk_sdl_font_stash_begin(&atlas);

        /* Install font from resource */
        struct nk_font* diablo = 0;
        HRSRC res = FindResource(hInstance, MAKEINTRESOURCE(IDR_RCDATA_FONT_DIABLO), RT_RCDATA);

        if (res)
        {
            HGLOBAL mem = LoadResource(hInstance, res);
            void* data = LockResource(mem);
            size_t len = SizeofResource(hInstance, res);

            diablo = nk_font_atlas_add_from_memory(atlas, data, (nk_size)len, 24, 0);
        }

        /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 16, 0);*/
        /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
        /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
        /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
        /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
        nk_sdl_font_stash_end();
        /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
        if(diablo)
            nk_style_set_font(ctx, &diablo->handle); }

    #ifdef INCLUDE_STYLE
        /*set_style(ctx, THEME_WHITE);*/
        /*set_style(ctx, THEME_RED);*/
        /*set_style(ctx, THEME_BLUE);*/
        /*set_style(ctx, THEME_DARK);*/
    #endif

        bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

        glEnable(GL_TEXTURE_2D);
    
        int x, y;
        size_t n;

        media.skin = gl2_image_load_from_resource(hInstance, IDB_PNG_SDL_NUKLEAR_TEST, &x, &y, &n);
        media.skin_buttons = gl2_image_load_from_resource(hInstance, IDB_PNG_SDL_BUTTONS, &x, &y, &n);
        media.skin_window = gl2_image_load_from_resource(hInstance, IDB_PNG_SDL_WINDOW, &x, &y, &n);
        media.check = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 32, 15, 15));
        media.check_cursor = nk_subimage_id(media.skin, 512, 512, nk_rect(450, 34, 11, 11));
        media.option = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 64, 15, 15));
        media.option_cursor = nk_subimage_id(media.skin, 512, 512, nk_rect(451, 67, 9, 9));
        media.header = nk_subimage_id(media.skin, 512, 512, nk_rect(128, 0, 127, 24));
        media.window = nk_subimage_id(media.skin_window, 1261, 851, nk_rect(0, 0, 1261, 851));
        media.scrollbar_inc_button = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 256, 15, 15));
        media.scrollbar_inc_button_hover = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 320, 15, 15));
        media.scrollbar_dec_button = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 224, 15, 15));
        media.scrollbar_dec_button_hover = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 288, 15, 15));
        media.button = nk_subimage_id(media.skin_buttons, 300, 300, nk_rect(0, 0, 300, 87));
        media.button_hover = nk_subimage_id(media.skin_buttons, 300, 300, nk_rect(0, 174, 300, 87));
        media.button_active = nk_subimage_id(media.skin_buttons, 300, 300, nk_rect(0, 87, 300, 87));
        media.tab_minimize = nk_subimage_id(media.skin, 512, 512, nk_rect(451, 99, 9, 9));
        media.tab_maximize = nk_subimage_id(media.skin, 512, 512, nk_rect(467, 99, 9, 9));
        media.slider = nk_subimage_id(media.skin, 512, 512, nk_rect(418, 33, 11, 14));
        media.slider_hover = nk_subimage_id(media.skin, 512, 512, nk_rect(418, 49, 11, 14));
        media.slider_active = nk_subimage_id(media.skin, 512, 512, nk_rect(418, 64, 11, 14));

        /* window */
        ctx->style.window.background = nk_rgb(204, 204, 204);
        ctx->style.window.fixed_background = nk_style_item_image(media.window);
        ctx->style.window.border_color = nk_rgb(67, 67, 67);
        ctx->style.window.combo_border_color = nk_rgb(67, 67, 67);
        ctx->style.window.contextual_border_color = nk_rgb(67, 67, 67);
        ctx->style.window.menu_border_color = nk_rgb(67, 67, 67);
        ctx->style.window.group_border_color = nk_rgb(67, 67, 67);
        ctx->style.window.tooltip_border_color = nk_rgb(67, 67, 67);
        ctx->style.window.scrollbar_size = nk_vec2(16, 16);
        ctx->style.window.border_color = nk_rgba(0, 0, 0, 0);
        ctx->style.window.padding = nk_vec2(16, 16);
        ctx->style.window.border = 3;

        /* window header */
        ctx->style.window.header.normal = nk_style_item_image(media.header);
        ctx->style.window.header.hover = nk_style_item_image(media.header);
        ctx->style.window.header.active = nk_style_item_image(media.header);
        ctx->style.window.header.label_normal = nk_rgb(95, 95, 95);
        ctx->style.window.header.label_hover = nk_rgb(50, 50, 50);
        ctx->style.window.header.label_active = nk_rgb(95, 95, 95);


        while (running)
        {
            /* Input */
            SDL_Event evt;
            nk_input_begin(ctx);
            while (SDL_PollEvent(&evt)) {
                if (evt.type == SDL_QUIT) goto cleanup;
                nk_sdl_handle_event(&evt);
            }
            nk_input_end(ctx);


            /* scrollbar */
            ctx->style.scrollv.normal = nk_style_item_color(nk_rgb(184, 184, 184));
            ctx->style.scrollv.hover = nk_style_item_color(nk_rgb(184, 184, 184));
            ctx->style.scrollv.active = nk_style_item_color(nk_rgb(184, 184, 184));
            ctx->style.scrollv.cursor_normal = nk_style_item_color(nk_rgb(220, 220, 220));
            ctx->style.scrollv.cursor_hover = nk_style_item_color(nk_rgb(235, 235, 235));
            ctx->style.scrollv.cursor_active = nk_style_item_color(nk_rgb(99, 202, 255));
            ctx->style.scrollv.dec_symbol = NK_SYMBOL_NONE;
            ctx->style.scrollv.inc_symbol = NK_SYMBOL_NONE;
            ctx->style.scrollv.show_buttons = nk_true;
            ctx->style.scrollv.border_color = nk_rgb(81, 81, 81);
            ctx->style.scrollv.cursor_border_color = nk_rgb(81, 81, 81);
            ctx->style.scrollv.border = 1;
            ctx->style.scrollv.rounding = 0;
            ctx->style.scrollv.border_cursor = 1;
            ctx->style.scrollv.rounding_cursor = 2;

            /* scrollbar buttons */
            ctx->style.scrollv.inc_button.normal = nk_style_item_image(media.scrollbar_inc_button);
            ctx->style.scrollv.inc_button.hover = nk_style_item_image(media.scrollbar_inc_button_hover);
            ctx->style.scrollv.inc_button.active = nk_style_item_image(media.scrollbar_inc_button_hover);
            ctx->style.scrollv.inc_button.border_color = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.inc_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.inc_button.text_normal = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.inc_button.text_hover = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.inc_button.text_active = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.inc_button.border = 0.0f;

            ctx->style.scrollv.dec_button.normal = nk_style_item_image(media.scrollbar_dec_button);
            ctx->style.scrollv.dec_button.hover = nk_style_item_image(media.scrollbar_dec_button_hover);
            ctx->style.scrollv.dec_button.active = nk_style_item_image(media.scrollbar_dec_button_hover);
            ctx->style.scrollv.dec_button.border_color = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.dec_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.dec_button.text_normal = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.dec_button.text_hover = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.dec_button.text_active = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.dec_button.border = 0.0f;

            /* checkbox toggle */
            {struct nk_style_toggle* toggle;
            toggle = &ctx->style.checkbox;
            toggle->normal = nk_style_item_image(media.check);
            toggle->hover = nk_style_item_image(media.check);
            toggle->active = nk_style_item_image(media.check);
            toggle->cursor_normal = nk_style_item_image(media.check_cursor);
            toggle->cursor_hover = nk_style_item_image(media.check_cursor);
            toggle->text_normal = nk_rgb(95, 95, 95);
            toggle->text_hover = nk_rgb(95, 95, 95);
            toggle->text_active = nk_rgb(95, 95, 95); }

            /* option toggle */
            {struct nk_style_toggle* toggle;
            toggle = &ctx->style.option;
            toggle->normal = nk_style_item_image(media.option);
            toggle->hover = nk_style_item_image(media.option);
            toggle->active = nk_style_item_image(media.option);
            toggle->cursor_normal = nk_style_item_image(media.option_cursor);
            toggle->cursor_hover = nk_style_item_image(media.option_cursor);
            toggle->text_normal = nk_rgb(95, 95, 95);
            toggle->text_hover = nk_rgb(95, 95, 95);
            toggle->text_active = nk_rgb(95, 95, 95); }

            /* default button */
            ctx->style.button.normal = nk_style_item_image(media.button);
            ctx->style.button.hover = nk_style_item_image(media.button_hover);
            ctx->style.button.active = nk_style_item_image(media.button_active);
            ctx->style.button.border_color = nk_rgba(0, 0, 0, 0);
            ctx->style.button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.button.text_normal = nk_rgb(95, 95, 95);
            ctx->style.button.text_hover = nk_rgb(95, 95, 95);
            ctx->style.button.text_active = nk_rgb(95, 95, 95);

            /* default text */
            ctx->style.text.color = nk_rgb(95, 95, 95);

            /* contextual button */
            ctx->style.contextual_button.normal = nk_style_item_color(nk_rgb(206, 206, 206));
            ctx->style.contextual_button.hover = nk_style_item_color(nk_rgb(229, 229, 229));
            ctx->style.contextual_button.active = nk_style_item_color(nk_rgb(99, 202, 255));
            ctx->style.contextual_button.border_color = nk_rgba(0, 0, 0, 0);
            ctx->style.contextual_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.contextual_button.text_normal = nk_rgb(95, 95, 95);
            ctx->style.contextual_button.text_hover = nk_rgb(95, 95, 95);
            ctx->style.contextual_button.text_active = nk_rgb(95, 95, 95);

            /* menu button */
            ctx->style.menu_button.normal = nk_style_item_color(nk_rgb(206, 206, 206));
            ctx->style.menu_button.hover = nk_style_item_color(nk_rgb(229, 229, 229));
            ctx->style.menu_button.active = nk_style_item_color(nk_rgb(99, 202, 255));
            ctx->style.menu_button.border_color = nk_rgba(0, 0, 0, 0);
            ctx->style.menu_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.menu_button.text_normal = nk_rgb(95, 95, 95);
            ctx->style.menu_button.text_hover = nk_rgb(95, 95, 95);
            ctx->style.menu_button.text_active = nk_rgb(95, 95, 95);

            /* tree */
            ctx->style.tab.text = nk_rgb(95, 95, 95);
            ctx->style.tab.tab_minimize_button.normal = nk_style_item_image(media.tab_minimize);
            ctx->style.tab.tab_minimize_button.hover = nk_style_item_image(media.tab_minimize);
            ctx->style.tab.tab_minimize_button.active = nk_style_item_image(media.tab_minimize);
            ctx->style.tab.tab_minimize_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.tab_minimize_button.text_normal = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.tab_minimize_button.text_hover = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.tab_minimize_button.text_active = nk_rgba(0, 0, 0, 0);

            ctx->style.tab.tab_maximize_button.normal = nk_style_item_image(media.tab_maximize);
            ctx->style.tab.tab_maximize_button.hover = nk_style_item_image(media.tab_maximize);
            ctx->style.tab.tab_maximize_button.active = nk_style_item_image(media.tab_maximize);
            ctx->style.tab.tab_maximize_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.tab_maximize_button.text_normal = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.tab_maximize_button.text_hover = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.tab_maximize_button.text_active = nk_rgba(0, 0, 0, 0);

            ctx->style.tab.node_minimize_button.normal = nk_style_item_image(media.tab_minimize);
            ctx->style.tab.node_minimize_button.hover = nk_style_item_image(media.tab_minimize);
            ctx->style.tab.node_minimize_button.active = nk_style_item_image(media.tab_minimize);
            ctx->style.tab.node_minimize_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.node_minimize_button.text_normal = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.node_minimize_button.text_hover = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.node_minimize_button.text_active = nk_rgba(0, 0, 0, 0);

            ctx->style.tab.node_maximize_button.normal = nk_style_item_image(media.tab_maximize);
            ctx->style.tab.node_maximize_button.hover = nk_style_item_image(media.tab_maximize);
            ctx->style.tab.node_maximize_button.active = nk_style_item_image(media.tab_maximize);
            ctx->style.tab.node_maximize_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.node_maximize_button.text_normal = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.node_maximize_button.text_hover = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.node_maximize_button.text_active = nk_rgba(0, 0, 0, 0);

            /* selectable */
            ctx->style.selectable.normal = nk_style_item_color(nk_rgb(206, 206, 206));
            ctx->style.selectable.hover = nk_style_item_color(nk_rgb(206, 206, 206));
            ctx->style.selectable.pressed = nk_style_item_color(nk_rgb(206, 206, 206));
            ctx->style.selectable.normal_active = nk_style_item_color(nk_rgb(185, 205, 248));
            ctx->style.selectable.hover_active = nk_style_item_color(nk_rgb(185, 205, 248));
            ctx->style.selectable.pressed_active = nk_style_item_color(nk_rgb(185, 205, 248));
            ctx->style.selectable.text_normal = nk_rgb(95, 95, 95);
            ctx->style.selectable.text_hover = nk_rgb(95, 95, 95);
            ctx->style.selectable.text_pressed = nk_rgb(95, 95, 95);
            ctx->style.selectable.text_normal_active = nk_rgb(95, 95, 95);
            ctx->style.selectable.text_hover_active = nk_rgb(95, 95, 95);
            ctx->style.selectable.text_pressed_active = nk_rgb(95, 95, 95);

            /* slider */
            ctx->style.slider.normal = nk_style_item_hide();
            ctx->style.slider.hover = nk_style_item_hide();
            ctx->style.slider.active = nk_style_item_hide();
            ctx->style.slider.bar_normal = nk_rgb(156, 156, 156);
            ctx->style.slider.bar_hover = nk_rgb(156, 156, 156);
            ctx->style.slider.bar_active = nk_rgb(156, 156, 156);
            ctx->style.slider.bar_filled = nk_rgb(156, 156, 156);
            ctx->style.slider.cursor_normal = nk_style_item_image(media.slider);
            ctx->style.slider.cursor_hover = nk_style_item_image(media.slider_hover);
            ctx->style.slider.cursor_active = nk_style_item_image(media.slider_active);
            ctx->style.slider.cursor_size = nk_vec2(16.5f, 21);
            ctx->style.slider.bar_height = 1;

            /* progressbar */
            ctx->style.progress.normal = nk_style_item_color(nk_rgb(231, 231, 231));
            ctx->style.progress.hover = nk_style_item_color(nk_rgb(231, 231, 231));
            ctx->style.progress.active = nk_style_item_color(nk_rgb(231, 231, 231));
            ctx->style.progress.cursor_normal = nk_style_item_color(nk_rgb(63, 242, 93));
            ctx->style.progress.cursor_hover = nk_style_item_color(nk_rgb(63, 242, 93));
            ctx->style.progress.cursor_active = nk_style_item_color(nk_rgb(63, 242, 93));
            ctx->style.progress.border_color = nk_rgb(114, 116, 115);
            ctx->style.progress.padding = nk_vec2(0, 0);
            ctx->style.progress.border = 2;
            ctx->style.progress.rounding = 1;

            /* combo */
            ctx->style.combo.normal = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.combo.hover = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.combo.active = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.combo.border_color = nk_rgb(95, 95, 95);
            ctx->style.combo.label_normal = nk_rgb(95, 95, 95);
            ctx->style.combo.label_hover = nk_rgb(95, 95, 95);
            ctx->style.combo.label_active = nk_rgb(95, 95, 95);
            ctx->style.combo.border = 1;
            ctx->style.combo.rounding = 1;

            /* combo button */
            ctx->style.combo.button.normal = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.combo.button.hover = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.combo.button.active = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.combo.button.text_background = nk_rgb(216, 216, 216);
            ctx->style.combo.button.text_normal = nk_rgb(95, 95, 95);
            ctx->style.combo.button.text_hover = nk_rgb(95, 95, 95);
            ctx->style.combo.button.text_active = nk_rgb(95, 95, 95);

            /* property */
            ctx->style.property.normal = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.hover = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.active = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.border_color = nk_rgb(81, 81, 81);
            ctx->style.property.label_normal = nk_rgb(95, 95, 95);
            ctx->style.property.label_hover = nk_rgb(95, 95, 95);
            ctx->style.property.label_active = nk_rgb(95, 95, 95);
            ctx->style.property.sym_left = NK_SYMBOL_TRIANGLE_LEFT;
            ctx->style.property.sym_right = NK_SYMBOL_TRIANGLE_RIGHT;
            ctx->style.property.rounding = 10;
            ctx->style.property.border = 1;

            /* edit */
            ctx->style.edit.normal = nk_style_item_color(nk_rgb(240, 240, 240));
            ctx->style.edit.hover = nk_style_item_color(nk_rgb(240, 240, 240));
            ctx->style.edit.active = nk_style_item_color(nk_rgb(240, 240, 240));
            ctx->style.edit.border_color = nk_rgb(62, 62, 62);
            ctx->style.edit.cursor_normal = nk_rgb(99, 202, 255);
            ctx->style.edit.cursor_hover = nk_rgb(99, 202, 255);
            ctx->style.edit.cursor_text_normal = nk_rgb(95, 95, 95);
            ctx->style.edit.cursor_text_hover = nk_rgb(95, 95, 95);
            ctx->style.edit.text_normal = nk_rgb(95, 95, 95);
            ctx->style.edit.text_hover = nk_rgb(95, 95, 95);
            ctx->style.edit.text_active = nk_rgb(95, 95, 95);
            ctx->style.edit.selected_normal = nk_rgb(99, 202, 255);
            ctx->style.edit.selected_hover = nk_rgb(99, 202, 255);
            ctx->style.edit.selected_text_normal = nk_rgb(95, 95, 95);
            ctx->style.edit.selected_text_hover = nk_rgb(95, 95, 95);
            ctx->style.edit.border = 1;
            ctx->style.edit.rounding = 2;

            /* property buttons */
            ctx->style.property.dec_button.normal = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.dec_button.hover = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.dec_button.active = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.dec_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.property.dec_button.text_normal = nk_rgb(95, 95, 95);
            ctx->style.property.dec_button.text_hover = nk_rgb(95, 95, 95);
            ctx->style.property.dec_button.text_active = nk_rgb(95, 95, 95);
            ctx->style.property.inc_button = ctx->style.property.dec_button;

            /* property edit */
            ctx->style.property.edit.normal = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.edit.hover = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.edit.active = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.edit.border_color = nk_rgba(0, 0, 0, 0);
            ctx->style.property.edit.cursor_normal = nk_rgb(95, 95, 95);
            ctx->style.property.edit.cursor_hover = nk_rgb(95, 95, 95);
            ctx->style.property.edit.cursor_text_normal = nk_rgb(216, 216, 216);
            ctx->style.property.edit.cursor_text_hover = nk_rgb(216, 216, 216);
            ctx->style.property.edit.text_normal = nk_rgb(95, 95, 95);
            ctx->style.property.edit.text_hover = nk_rgb(95, 95, 95);
            ctx->style.property.edit.text_active = nk_rgb(95, 95, 95);
            ctx->style.property.edit.selected_normal = nk_rgb(95, 95, 95);
            ctx->style.property.edit.selected_hover = nk_rgb(95, 95, 95);
            ctx->style.property.edit.selected_text_normal = nk_rgb(216, 216, 216);
            ctx->style.property.edit.selected_text_hover = nk_rgb(216, 216, 216);

            /* chart */
            ctx->style.chart.background = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.chart.border_color = nk_rgb(81, 81, 81);
            ctx->style.chart.color = nk_rgb(95, 95, 95);
            ctx->style.chart.selected_color = nk_rgb(255, 0, 0);
            ctx->style.chart.border = 1;


            /* GUI */
            if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
                NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
            {
                enum { EASY, HARD };
                static int op = EASY;
                static int property = 20;

                nk_layout_row_static(ctx, 45, 245, 1);
                if (nk_button_label(ctx, "button"))
                    fprintf(stdout, "button pressed\n");
                nk_layout_row_dynamic(ctx, 30, 2);
                if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
                if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
                nk_layout_row_dynamic(ctx, 25, 1);
                nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

                nk_layout_row_dynamic(ctx, 20, 1);
                nk_label(ctx, "background:", NK_TEXT_LEFT);
                nk_layout_row_dynamic(ctx, 25, 1);
                if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400))) {
                    nk_layout_row_dynamic(ctx, 120, 1);
                    bg = nk_color_picker(ctx, bg, NK_RGBA);
                    nk_layout_row_dynamic(ctx, 25, 1);
                    bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
                    bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
                    bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
                    bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
                    nk_combo_end(ctx);
                }
            }
            nk_end(ctx);

            /* -------------- EXAMPLES ---------------- */
            ctx->style.window.padding = nk_vec2(16, 16);
            calculator(ctx);
            ctx->style.window.padding = nk_vec2(0, 8);
            ctx->style.window.border = 44;
            overview(ctx);
            ctx->style.window.padding = nk_vec2(16, 16);
            ctx->style.window.border = 3;
    #ifdef INCLUDE_NODE_EDITOR
            ctx->style.window.padding = nk_vec2(16, 16);
            node_editor(ctx);
    #endif
            /* ----------------------------------------- */

            /* Draw */
            SDL_GetWindowSize(win, &win_width, &win_height);
            glViewport(0, 0, win_width, win_height);
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(bg.r, bg.g, bg.b, bg.a);
            /* IMPORTANT: `nk_sdl_render` modifies some global OpenGL state
             * with blending, scissor, face culling, depth test and viewport and
             * defaults everything back into a default state.
             * Make sure to either a.) save and restore or b.) reset your own state after
             * rendering the UI. */

#ifdef NK_SDL_GL2_IMPLEMENTATION
            nk_sdl_render(NK_ANTI_ALIASING_ON);
#endif
#ifdef NK_SDL_GLES2_IMPLEMENTATION
            nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
#endif

            SDL_GL_SwapWindow(win);
        }

    cleanup:
        nk_sdl_shutdown();
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 0;
    }















    int
    init_nuklear(HINSTANCE hInstance, PGHSdlApp sdlapp)
    {
        SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");

#ifdef NK_SDL_GL2_IMPLEMENTATION
        SDL_Init(SDL_INIT_VIDEO);
#endif
#ifdef NK_SDL_GLES2_IMPLEMENTATION
        //No SDL initialization on GLES
#endif
        
        SDL_DisplayMode dm;
        SDL_GetDesktopDisplayMode(0, &dm);
        int sdl_win_width = min(1200, dm.w);
        int sdl_win_height = min(800, dm.h);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#ifdef NK_SDL_GL2_IMPLEMENTATION
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif
#ifdef NK_SDL_GLES2_IMPLEMENTATION

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

        sdlapp->win = sdl.win = SDL_CreateWindow("GnollHack",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            sdl_win_width, sdl_win_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI  /* | SDL_WINDOW_FULLSCREEN_DESKTOP */);
        SDL_GetWindowSize(sdl.win, &sdlapp->win_width, &sdlapp->win_height);
        glContext = SDL_GL_CreateContext(sdl.win);

        /* OpenGL setup */
        glViewport(0, 0, sdl_win_width, sdl_win_height);

        /* GUI */
        //ctx = nk_sdl_gnh_init(sdl_gnh.win, sdl_gnh.renderer);
        ctx = nk_sdl_init(sdl.win);
        sdlapp->running = 1;

        /* Create drawing surface */
        //sdl_gnh.device.draw_surface = SDL_CreateRGBSurfaceWithFormat(0, sdlapp->win_width, sdlapp->win_height, 32, SDL_PIXELFORMAT_ARGB8888);

        /* Load Fonts: if none of these are loaded a default font will be used  */
        /* Load Cursor: if you uncomment cursor loading please hide the cursor */
        {
        nk_sdl_font_stash_begin(&atlas);

        /* Install font from resource */
        struct nk_font* diablo = 0, *diablo30 = 0, *diablo36 = 0;
        HRSRC res = FindResource(hInstance, MAKEINTRESOURCE(IDR_RCDATA_FONT_DIABLO), RT_RCDATA);

        if (res)
        {
            HGLOBAL mem = LoadResource(hInstance, res);
            void* data = LockResource(mem);
            size_t len = SizeofResource(hInstance, res);

            if (data)
            {
                diablo = nk_font_atlas_add_from_memory(atlas, data, (nk_size)len, 24, 0);
                diablo30 = nk_font_atlas_add_from_memory(atlas, data, (nk_size)len, 30, 0);
                diablo36 = nk_font_atlas_add_from_memory(atlas, data, (nk_size)len, 36, 0);
            }
        }

        /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 16, 0);*/
        /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
        /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
        /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
        /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
        media.diablo24 = diablo;
        media.diablo30 = diablo30;
        media.diablo36 = diablo36;
        nk_sdl_font_stash_end();
        /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
        if (diablo)
            nk_style_set_font(ctx, &diablo->handle); }

    #ifdef INCLUDE_STYLE
        /*set_style(ctx, THEME_WHITE);*/
        /*set_style(ctx, THEME_RED);*/
        /*set_style(ctx, THEME_BLUE);*/
        /*set_style(ctx, THEME_DARK);*/
    #endif

        bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
        int x, y;
        size_t n;

        glEnable(GL_TEXTURE_2D);
        media.skin = gl2_image_load_from_resource(hInstance, IDB_PNG_SDL_NUKLEAR_TEST, &x, &y, &n);
        media.skin_buttons = gl2_image_load_from_resource(hInstance, IDB_PNG_SDL_BUTTONS, &x, &y, &n);
        media.skin_window = gl2_image_load_from_resource(hInstance, IDB_PNG_SDL_WINDOW, &x, &y, &n);
        media.skin_cursors = gl2_image_load_from_resource(hInstance, IDB_PNG_SDL_CURSORS, &x, &y, &n);
        /*
        media.surface_skin = sdl_surface_image_load_from_resource(hInstance, IDB_PNG_SDL_NUKLEAR_TEST, &x, &y, &n);
        media.surface_skin_buttons = sdl_surface_image_load_from_resource(hInstance, IDB_PNG_SDL_BUTTONS, &x, &y, &n);
        media.surface_skin_window = sdl_surface_image_load_from_resource(hInstance, IDB_PNG_SDL_WINDOW, &x, &y, &n);
        media.surface_skin_cursors = sdl_surface_image_load_from_resource(hInstance, IDB_PNG_SDL_CURSORS, &x, &y, &n);
        */
        media.check = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 32, 15, 15));
        media.check_cursor = nk_subimage_id(media.skin, 512, 512, nk_rect(450, 34, 11, 11));
        media.option = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 64, 15, 15));
        media.option_cursor = nk_subimage_id(media.skin, 512, 512, nk_rect(451, 67, 9, 9));
        media.header = nk_subimage_id(media.skin, 512, 512, nk_rect(128, 0, 127, 24));
        media.window = nk_subimage_id(media.skin_window, 1261, 851, nk_rect(0, 0, 1261, 851));
        media.scrollbar_inc_button = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 256, 15, 15));
        media.scrollbar_inc_button_hover = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 320, 15, 15));
        media.scrollbar_dec_button = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 224, 15, 15));
        media.scrollbar_dec_button_hover = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 288, 15, 15));
        media.button = nk_subimage_id(media.skin_buttons, 300, 300, nk_rect(0, 0, 300, 87));
        media.button_hover = nk_subimage_id(media.skin_buttons, 300, 300, nk_rect(0, 174, 300, 87));
        media.button_active = nk_subimage_id(media.skin_buttons, 300, 300, nk_rect(0, 87, 300, 87));
        media.tab_minimize = nk_subimage_id(media.skin, 512, 512, nk_rect(451, 99, 9, 9));
        media.tab_maximize = nk_subimage_id(media.skin, 512, 512, nk_rect(467, 99, 9, 9));
        media.slider = nk_subimage_id(media.skin, 512, 512, nk_rect(418, 33, 11, 14));
        media.slider_hover = nk_subimage_id(media.skin, 512, 512, nk_rect(418, 49, 11, 14));
        media.slider_active = nk_subimage_id(media.skin, 512, 512, nk_rect(418, 64, 11, 14));

        /* window */
        ctx->style.window.background = nk_rgb(204, 204, 204);
        ctx->style.window.fixed_background = nk_style_item_image(media.window);
        ctx->style.window.border_color = nk_rgb(67, 67, 67);
        ctx->style.window.combo_border_color = nk_rgb(67, 67, 67);
        ctx->style.window.contextual_border_color = nk_rgb(67, 67, 67);
        ctx->style.window.menu_border_color = nk_rgb(67, 67, 67);
        ctx->style.window.group_border_color = nk_rgb(67, 67, 67);
        ctx->style.window.tooltip_border_color = nk_rgb(67, 67, 67);
        ctx->style.window.scrollbar_size = nk_vec2(16, 16);
        ctx->style.window.border_color = nk_rgba(0, 0, 0, 0);
        ctx->style.window.padding = nk_vec2(16, 16);
        ctx->style.window.border = 3;

        /* window header */
        ctx->style.window.header.normal = nk_style_item_image(media.header);
        ctx->style.window.header.hover = nk_style_item_image(media.header);
        ctx->style.window.header.active = nk_style_item_image(media.header);
        ctx->style.window.header.label_normal = nk_rgb(95, 95, 95);
        ctx->style.window.header.label_hover = nk_rgb(50, 50, 50);
        ctx->style.window.header.label_active = nk_rgb(95, 95, 95);

        /* cursors */
        media.custom_cursor.img = nk_subimage_id(media.skin_cursors, 32, 32, nk_rect(0, 0, 32, 32));
        media.custom_cursor.size.x = 32;
        media.custom_cursor.size.y = 32;
        media.custom_cursor.offset.x = 0;
        media.custom_cursor.offset.y = 0;

        nk_style_load_cursor(ctx, NK_CURSOR_ARROW, &media.custom_cursor);
        nk_style_show_cursor(ctx);
        (void)SDL_ShowCursor(SDL_DISABLE);

        return 1;
    }


    int
    shutdown_nuklear()
    {
        nk_sdl_shutdown();
        SDL_DestroyWindow(sdl.win);
        SDL_GL_DeleteContext(glContext);
        SDL_Quit();
        return 1;
    }


    int
    nuklear_main_loop(PGHSdlApp sdlapp)
    {
        int key = 0;
        while (sdlapp->running && key == 0)
        {
            /* Input */
            SDL_Event evt;
            nk_input_begin(ctx);
            while (SDL_PollEvent(&evt)) 
            {
                if (evt.type == SDL_QUIT) 
                    goto cleanup;
                key = 0;
                nk_sdl_handle_event(&evt);
                if (evt.type == SDL_KEYUP)
                {
                    SDL_KeyCode keycode = evt.key.keysym.sym;
                    if (keycode == SDLK_ESCAPE)
                        goto cleanup;
                    else if (keycode >= SDLK_SPACE && keycode <= SDLK_z)
                    {
                        key = (int)keycode;
                        if (keycode >= SDLK_a && keycode <= SDLK_z && SDL_GetModState() & KMOD_SHIFT)
                            key -= 32;
                    }
                    break;
                }
            }
            nk_input_end(ctx);


            /* scrollbar */
            ctx->style.scrollv.normal = nk_style_item_color(nk_rgb(184, 184, 184));
            ctx->style.scrollv.hover = nk_style_item_color(nk_rgb(184, 184, 184));
            ctx->style.scrollv.active = nk_style_item_color(nk_rgb(184, 184, 184));
            ctx->style.scrollv.cursor_normal = nk_style_item_color(nk_rgb(220, 220, 220));
            ctx->style.scrollv.cursor_hover = nk_style_item_color(nk_rgb(235, 235, 235));
            ctx->style.scrollv.cursor_active = nk_style_item_color(nk_rgb(99, 202, 255));
            ctx->style.scrollv.dec_symbol = NK_SYMBOL_NONE;
            ctx->style.scrollv.inc_symbol = NK_SYMBOL_NONE;
            ctx->style.scrollv.show_buttons = nk_true;
            ctx->style.scrollv.border_color = nk_rgb(81, 81, 81);
            ctx->style.scrollv.cursor_border_color = nk_rgb(81, 81, 81);
            ctx->style.scrollv.border = 1;
            ctx->style.scrollv.rounding = 0;
            ctx->style.scrollv.border_cursor = 1;
            ctx->style.scrollv.rounding_cursor = 2;

            /* scrollbar buttons */
            ctx->style.scrollv.inc_button.normal = nk_style_item_image(media.scrollbar_inc_button);
            ctx->style.scrollv.inc_button.hover = nk_style_item_image(media.scrollbar_inc_button_hover);
            ctx->style.scrollv.inc_button.active = nk_style_item_image(media.scrollbar_inc_button_hover);
            ctx->style.scrollv.inc_button.border_color = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.inc_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.inc_button.text_normal = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.inc_button.text_hover = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.inc_button.text_active = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.inc_button.border = 0.0f;

            ctx->style.scrollv.dec_button.normal = nk_style_item_image(media.scrollbar_dec_button);
            ctx->style.scrollv.dec_button.hover = nk_style_item_image(media.scrollbar_dec_button_hover);
            ctx->style.scrollv.dec_button.active = nk_style_item_image(media.scrollbar_dec_button_hover);
            ctx->style.scrollv.dec_button.border_color = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.dec_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.dec_button.text_normal = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.dec_button.text_hover = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.dec_button.text_active = nk_rgba(0, 0, 0, 0);
            ctx->style.scrollv.dec_button.border = 0.0f;

            /* checkbox toggle */
            {struct nk_style_toggle* toggle;
            toggle = &ctx->style.checkbox;
            toggle->normal = nk_style_item_image(media.check);
            toggle->hover = nk_style_item_image(media.check);
            toggle->active = nk_style_item_image(media.check);
            toggle->cursor_normal = nk_style_item_image(media.check_cursor);
            toggle->cursor_hover = nk_style_item_image(media.check_cursor);
            toggle->text_normal = nk_rgb(95, 95, 95);
            toggle->text_hover = nk_rgb(95, 95, 95);
            toggle->text_active = nk_rgb(95, 95, 95); }

            /* option toggle */
            {struct nk_style_toggle* toggle;
            toggle = &ctx->style.option;
            toggle->normal = nk_style_item_image(media.option);
            toggle->hover = nk_style_item_image(media.option);
            toggle->active = nk_style_item_image(media.option);
            toggle->cursor_normal = nk_style_item_image(media.option_cursor);
            toggle->cursor_hover = nk_style_item_image(media.option_cursor);
            toggle->text_normal = nk_rgb(95, 95, 95);
            toggle->text_hover = nk_rgb(95, 95, 95);
            toggle->text_active = nk_rgb(95, 95, 95); }

            /* default button */
            ctx->style.button.normal = nk_style_item_image(media.button);
            ctx->style.button.hover = nk_style_item_image(media.button_hover);
            ctx->style.button.active = nk_style_item_image(media.button_active);
            ctx->style.button.border_color = nk_rgba(0, 0, 0, 0);
            ctx->style.button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.button.text_normal = nk_rgb(95, 95, 95);
            ctx->style.button.text_hover = nk_rgb(95, 95, 95);
            ctx->style.button.text_active = nk_rgb(95, 95, 95);

            /* default text */
            ctx->style.text.color = nk_rgb(95, 95, 95);

            /* contextual button */
            ctx->style.contextual_button.normal = nk_style_item_color(nk_rgb(206, 206, 206));
            ctx->style.contextual_button.hover = nk_style_item_color(nk_rgb(229, 229, 229));
            ctx->style.contextual_button.active = nk_style_item_color(nk_rgb(99, 202, 255));
            ctx->style.contextual_button.border_color = nk_rgba(0, 0, 0, 0);
            ctx->style.contextual_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.contextual_button.text_normal = nk_rgb(95, 95, 95);
            ctx->style.contextual_button.text_hover = nk_rgb(95, 95, 95);
            ctx->style.contextual_button.text_active = nk_rgb(95, 95, 95);

            /* menu button */
            ctx->style.menu_button.normal = nk_style_item_color(nk_rgb(206, 206, 206));
            ctx->style.menu_button.hover = nk_style_item_color(nk_rgb(229, 229, 229));
            ctx->style.menu_button.active = nk_style_item_color(nk_rgb(99, 202, 255));
            ctx->style.menu_button.border_color = nk_rgba(0, 0, 0, 0);
            ctx->style.menu_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.menu_button.text_normal = nk_rgb(95, 95, 95);
            ctx->style.menu_button.text_hover = nk_rgb(95, 95, 95);
            ctx->style.menu_button.text_active = nk_rgb(95, 95, 95);

            /* tree */
            ctx->style.tab.text = nk_rgb(95, 95, 95);
            ctx->style.tab.tab_minimize_button.normal = nk_style_item_image(media.tab_minimize);
            ctx->style.tab.tab_minimize_button.hover = nk_style_item_image(media.tab_minimize);
            ctx->style.tab.tab_minimize_button.active = nk_style_item_image(media.tab_minimize);
            ctx->style.tab.tab_minimize_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.tab_minimize_button.text_normal = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.tab_minimize_button.text_hover = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.tab_minimize_button.text_active = nk_rgba(0, 0, 0, 0);

            ctx->style.tab.tab_maximize_button.normal = nk_style_item_image(media.tab_maximize);
            ctx->style.tab.tab_maximize_button.hover = nk_style_item_image(media.tab_maximize);
            ctx->style.tab.tab_maximize_button.active = nk_style_item_image(media.tab_maximize);
            ctx->style.tab.tab_maximize_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.tab_maximize_button.text_normal = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.tab_maximize_button.text_hover = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.tab_maximize_button.text_active = nk_rgba(0, 0, 0, 0);

            ctx->style.tab.node_minimize_button.normal = nk_style_item_image(media.tab_minimize);
            ctx->style.tab.node_minimize_button.hover = nk_style_item_image(media.tab_minimize);
            ctx->style.tab.node_minimize_button.active = nk_style_item_image(media.tab_minimize);
            ctx->style.tab.node_minimize_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.node_minimize_button.text_normal = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.node_minimize_button.text_hover = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.node_minimize_button.text_active = nk_rgba(0, 0, 0, 0);

            ctx->style.tab.node_maximize_button.normal = nk_style_item_image(media.tab_maximize);
            ctx->style.tab.node_maximize_button.hover = nk_style_item_image(media.tab_maximize);
            ctx->style.tab.node_maximize_button.active = nk_style_item_image(media.tab_maximize);
            ctx->style.tab.node_maximize_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.node_maximize_button.text_normal = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.node_maximize_button.text_hover = nk_rgba(0, 0, 0, 0);
            ctx->style.tab.node_maximize_button.text_active = nk_rgba(0, 0, 0, 0);

            /* selectable */
            ctx->style.selectable.normal = nk_style_item_color(nk_rgb(206, 206, 206));
            ctx->style.selectable.hover = nk_style_item_color(nk_rgb(206, 206, 206));
            ctx->style.selectable.pressed = nk_style_item_color(nk_rgb(206, 206, 206));
            ctx->style.selectable.normal_active = nk_style_item_color(nk_rgb(185, 205, 248));
            ctx->style.selectable.hover_active = nk_style_item_color(nk_rgb(185, 205, 248));
            ctx->style.selectable.pressed_active = nk_style_item_color(nk_rgb(185, 205, 248));
            ctx->style.selectable.text_normal = nk_rgb(95, 95, 95);
            ctx->style.selectable.text_hover = nk_rgb(95, 95, 95);
            ctx->style.selectable.text_pressed = nk_rgb(95, 95, 95);
            ctx->style.selectable.text_normal_active = nk_rgb(95, 95, 95);
            ctx->style.selectable.text_hover_active = nk_rgb(95, 95, 95);
            ctx->style.selectable.text_pressed_active = nk_rgb(95, 95, 95);

            /* slider */
            ctx->style.slider.normal = nk_style_item_hide();
            ctx->style.slider.hover = nk_style_item_hide();
            ctx->style.slider.active = nk_style_item_hide();
            ctx->style.slider.bar_normal = nk_rgb(156, 156, 156);
            ctx->style.slider.bar_hover = nk_rgb(156, 156, 156);
            ctx->style.slider.bar_active = nk_rgb(156, 156, 156);
            ctx->style.slider.bar_filled = nk_rgb(156, 156, 156);
            ctx->style.slider.cursor_normal = nk_style_item_image(media.slider);
            ctx->style.slider.cursor_hover = nk_style_item_image(media.slider_hover);
            ctx->style.slider.cursor_active = nk_style_item_image(media.slider_active);
            ctx->style.slider.cursor_size = nk_vec2(16.5f, 21);
            ctx->style.slider.bar_height = 1;

            /* progressbar */
            ctx->style.progress.normal = nk_style_item_color(nk_rgb(231, 231, 231));
            ctx->style.progress.hover = nk_style_item_color(nk_rgb(231, 231, 231));
            ctx->style.progress.active = nk_style_item_color(nk_rgb(231, 231, 231));
            ctx->style.progress.cursor_normal = nk_style_item_color(nk_rgb(63, 242, 93));
            ctx->style.progress.cursor_hover = nk_style_item_color(nk_rgb(63, 242, 93));
            ctx->style.progress.cursor_active = nk_style_item_color(nk_rgb(63, 242, 93));
            ctx->style.progress.border_color = nk_rgb(114, 116, 115);
            ctx->style.progress.padding = nk_vec2(0, 0);
            ctx->style.progress.border = 2;
            ctx->style.progress.rounding = 1;

            /* combo */
            ctx->style.combo.normal = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.combo.hover = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.combo.active = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.combo.border_color = nk_rgb(95, 95, 95);
            ctx->style.combo.label_normal = nk_rgb(95, 95, 95);
            ctx->style.combo.label_hover = nk_rgb(95, 95, 95);
            ctx->style.combo.label_active = nk_rgb(95, 95, 95);
            ctx->style.combo.border = 1;
            ctx->style.combo.rounding = 1;

            /* combo button */
            ctx->style.combo.button.normal = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.combo.button.hover = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.combo.button.active = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.combo.button.text_background = nk_rgb(216, 216, 216);
            ctx->style.combo.button.text_normal = nk_rgb(95, 95, 95);
            ctx->style.combo.button.text_hover = nk_rgb(95, 95, 95);
            ctx->style.combo.button.text_active = nk_rgb(95, 95, 95);

            /* property */
            ctx->style.property.normal = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.hover = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.active = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.border_color = nk_rgb(81, 81, 81);
            ctx->style.property.label_normal = nk_rgb(95, 95, 95);
            ctx->style.property.label_hover = nk_rgb(95, 95, 95);
            ctx->style.property.label_active = nk_rgb(95, 95, 95);
            ctx->style.property.sym_left = NK_SYMBOL_TRIANGLE_LEFT;
            ctx->style.property.sym_right = NK_SYMBOL_TRIANGLE_RIGHT;
            ctx->style.property.rounding = 10;
            ctx->style.property.border = 1;

            /* edit */
            ctx->style.edit.normal = nk_style_item_color(nk_rgb(240, 240, 240));
            ctx->style.edit.hover = nk_style_item_color(nk_rgb(240, 240, 240));
            ctx->style.edit.active = nk_style_item_color(nk_rgb(240, 240, 240));
            ctx->style.edit.border_color = nk_rgb(62, 62, 62);
            ctx->style.edit.cursor_normal = nk_rgb(99, 202, 255);
            ctx->style.edit.cursor_hover = nk_rgb(99, 202, 255);
            ctx->style.edit.cursor_text_normal = nk_rgb(95, 95, 95);
            ctx->style.edit.cursor_text_hover = nk_rgb(95, 95, 95);
            ctx->style.edit.text_normal = nk_rgb(95, 95, 95);
            ctx->style.edit.text_hover = nk_rgb(95, 95, 95);
            ctx->style.edit.text_active = nk_rgb(95, 95, 95);
            ctx->style.edit.selected_normal = nk_rgb(99, 202, 255);
            ctx->style.edit.selected_hover = nk_rgb(99, 202, 255);
            ctx->style.edit.selected_text_normal = nk_rgb(95, 95, 95);
            ctx->style.edit.selected_text_hover = nk_rgb(95, 95, 95);
            ctx->style.edit.border = 1;
            ctx->style.edit.rounding = 2;

            /* property buttons */
            ctx->style.property.dec_button.normal = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.dec_button.hover = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.dec_button.active = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.dec_button.text_background = nk_rgba(0, 0, 0, 0);
            ctx->style.property.dec_button.text_normal = nk_rgb(95, 95, 95);
            ctx->style.property.dec_button.text_hover = nk_rgb(95, 95, 95);
            ctx->style.property.dec_button.text_active = nk_rgb(95, 95, 95);
            ctx->style.property.inc_button = ctx->style.property.dec_button;

            /* property edit */
            ctx->style.property.edit.normal = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.edit.hover = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.edit.active = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.property.edit.border_color = nk_rgba(0, 0, 0, 0);
            ctx->style.property.edit.cursor_normal = nk_rgb(95, 95, 95);
            ctx->style.property.edit.cursor_hover = nk_rgb(95, 95, 95);
            ctx->style.property.edit.cursor_text_normal = nk_rgb(216, 216, 216);
            ctx->style.property.edit.cursor_text_hover = nk_rgb(216, 216, 216);
            ctx->style.property.edit.text_normal = nk_rgb(95, 95, 95);
            ctx->style.property.edit.text_hover = nk_rgb(95, 95, 95);
            ctx->style.property.edit.text_active = nk_rgb(95, 95, 95);
            ctx->style.property.edit.selected_normal = nk_rgb(95, 95, 95);
            ctx->style.property.edit.selected_hover = nk_rgb(95, 95, 95);
            ctx->style.property.edit.selected_text_normal = nk_rgb(216, 216, 216);
            ctx->style.property.edit.selected_text_hover = nk_rgb(216, 216, 216);

            /* chart */
            ctx->style.chart.background = nk_style_item_color(nk_rgb(216, 216, 216));
            ctx->style.chart.border_color = nk_rgb(81, 81, 81);
            ctx->style.chart.color = nk_rgb(95, 95, 95);
            ctx->style.chart.selected_color = nk_rgb(255, 0, 0);
            ctx->style.chart.border = 1;


            /* GUI */
            if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
                NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
            {
                enum { EASY, HARD };
                static int op = EASY;
                static int property = 20;

                nk_layout_row_static(ctx, 45, 245, 1);
                if (nk_button_label(ctx, "button"))
                    fprintf(stdout, "button pressed\n");
                nk_layout_row_dynamic(ctx, 30, 2);
                if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
                if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
                nk_layout_row_dynamic(ctx, 25, 1);
                nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

                nk_layout_row_dynamic(ctx, 20, 1);
                nk_label(ctx, "background:", NK_TEXT_LEFT);
                nk_layout_row_dynamic(ctx, 25, 1);
                if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400))) {
                    nk_layout_row_dynamic(ctx, 120, 1);
                    bg = nk_color_picker(ctx, bg, NK_RGBA);
                    nk_layout_row_dynamic(ctx, 25, 1);
                    bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
                    bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
                    bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
                    bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
                    nk_combo_end(ctx);
                }
            }
            nk_end(ctx);

            /* -------------- EXAMPLES ---------------- */
            ctx->style.window.padding = nk_vec2(16, 16);
            calculator(ctx);
            ctx->style.window.padding = nk_vec2(0, 8);
            ctx->style.window.border = 44;
            overview(ctx);
            ctx->style.window.padding = nk_vec2(16, 16);
            ctx->style.window.border = 3;
    #ifdef INCLUDE_NODE_EDITOR
            ctx->style.window.padding = nk_vec2(16, 16);
            node_editor(ctx);
    #endif
            /* ----------------------------------------- */

            /* Draw */
            SDL_GetWindowSize(sdl.win, &sdlapp->win_width, &sdlapp->win_height);
            glViewport(0, 0, sdlapp->win_width, sdlapp->win_height);
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(bg.r, bg.g, bg.b, bg.a);
            /* IMPORTANT: `nk_sdl_render` modifies some global OpenGL state
             * with blending, scissor, face culling, depth test and viewport and
             * defaults everything back into a default state.
             * Make sure to either a.) save and restore or b.) reset your own state after
             * rendering the UI. */

#ifdef NK_SDL_GL2_IMPLEMENTATION
            nk_sdl_render(NK_ANTI_ALIASING_ON);
#endif
#ifdef NK_SDL_GLES2_IMPLEMENTATION
            nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
#endif

            SDL_GL_SwapWindow(sdl.win);
        }

        return key;

    cleanup:
        if (sdlapp->running)
        {
            sdlapp->running = 0;
            (void)shutdown_nuklear();
        }
        return -1;
    }


    boolean
    nuklear_player_selection(PGHSdlApp sdlapp)
    {
        int win_width = 0, win_height = 0;
        int plsel_width = 840, plsel_height = 740;
        SDL_GetWindowSize(sdl.win, &win_width, &win_height);
        int plsel_x = max(0, (win_width - plsel_width) / 2);
        int plsel_y = max(0, (win_height - plsel_height) / 2);

        struct ghsound_music_info info = { 0 };
        info.ghsound = GHSOUND_MUSIC_PLAYER_SELECTION;
        info.volume = 1.0f;
        sdl_play_ghsound_music(info);

        plselRandomize();

        int current_role = flags.initrole;
        int current_race = flags.initrace;
        int current_gender = flags.initgend;
        int current_alignment = flags.initalign;

        int player_glyph_level = 0;
        int player_glyph_index = 0;
        int glyph = 0;
        int ntile = 0;
        int base_t_x = 0;
        int base_t_y = 0;

        int pet_glyph = NO_GLYPH;
        int pet_tile = 0;
        int pet_x = 0;
        int pet_y = 0;

        int pet_glyph2 = NO_GLYPH;
        int pet_tile2 = 0;
        int pet_x2 = 0;
        int pet_y2 = 0;

        int tilemap_width, tilemap_height;
        size_t tilemap_size;
        GLuint pic = gl2_image_load_from_resource(sdlapp->hApp, IDB_PNG_TILES, &tilemap_width, &tilemap_height, &tilemap_size);
        struct nk_image nkimg;
        struct nk_image petimg, petimg2;

        /* option toggle */
        struct nk_style_toggle* toggle;
        toggle = &ctx->style.option;
        toggle->normal = nk_style_item_image(media.option);
        toggle->hover = nk_style_item_image(media.option);
        toggle->active = nk_style_item_image(media.option);
        toggle->cursor_normal = nk_style_item_image(media.option_cursor);
        toggle->cursor_hover = nk_style_item_image(media.option_cursor);
        toggle->text_normal = nk_rgb(60, 60, 60);
        toggle->text_hover = nk_rgb(60, 60, 60);
        toggle->text_active = nk_rgb(60, 60, 60);
    
        /* default button */
        ctx->style.button.normal = nk_style_item_image(media.button);
        ctx->style.button.hover = nk_style_item_image(media.button_hover);
        ctx->style.button.active = nk_style_item_image(media.button_active);
        ctx->style.button.border_color = nk_rgba(0, 0, 0, 0);
        ctx->style.button.text_background = nk_rgba(0, 0, 0, 0);
        ctx->style.button.text_normal = nk_rgb(180, 180, 180);
        ctx->style.button.text_hover = nk_rgb(240, 240, 240);
        ctx->style.button.text_active = nk_rgb(120, 120, 120);
    
        /* default text */
        ctx->style.text.color = nk_rgb(60, 60, 60);

        /* window */
        ctx->style.window.padding = nk_vec2(64, 64);
        ctx->style.window.border = 32;

        struct ghsound_immediate_info sound_info = { 0 };
        sound_info.ghsound = GHSOUND_UI_BUTTON_DOWN;
        sound_info.volume = 1.0f;
        sound_info.sound_type = IMMEDIATE_SOUND_UI;

        const char* role_names_male[NUM_ROLES] = { 0 };
        const char* role_names_female[NUM_ROLES] = { 0 };
        const char* race_names[NUM_RACES] = { 0 };
        const char* gender_names[2] = { "Male", "Female" };
        const char* alignment_names[3] = { "Lawful", "Neutral", "Chaotic" };

        for (int i = 0; i < NUM_ROLES; i++)
        {
            role_names_male[i] = roles[i].name.m;
            role_names_female[i] = roles[i].name.f;
        }

        for (int i = 0; i < NUM_RACES; i++)
        {
            race_names[i] = races[i].noun;
        }

        int res = 0;
        boolean comboIsPressed = 0, comboWasPressed = 0;
        boolean raceIsPressed[NUM_RACES] = { 0 };
        boolean raceWasPressed[NUM_RACES] = { 0 };
        boolean genderIsPressed[2] = { 0 };
        boolean genderWasPressed[2] = { 0 };
        boolean alignmentIsPressed[3] = { 0 };
        boolean alignmentWasPressed[3] = { 0 };

        SDL_RaiseWindow(sdl.win);
        //SDL_SetWindowInputFocus(sdlapp->win);

        while (sdlapp->running && res == 0)
        {
            /* Input */
            SDL_Event evt;
            nk_input_begin(ctx);
            while (SDL_PollEvent(&evt))
            {
                if (evt.type == SDL_QUIT)
                    goto cleanup;
                nk_sdl_handle_event(&evt);
            }
            nk_input_end(ctx);

            player_glyph_index = player_to_glyph_index(current_role, current_race, current_gender, aligns[current_alignment].value, player_glyph_level);
            glyph = player_glyph_index + GLYPH_PLAYER_OFF;
            ntile = glyph2tile[glyph];
            base_t_x = SDL_TILEBMP_X(ntile);
            base_t_y = SDL_TILEBMP_Y(ntile);
            nkimg = nk_subimage_id(pic, tilemap_width, tilemap_height, nk_rect((float)base_t_x, (float)base_t_y, (float)TILE_X, (float)TILE_Y));

            pet_glyph = NO_GLYPH;
            pet_glyph2 = NO_GLYPH;
            if (current_role >= 0 && roles[current_role].petnum > NON_PM)
            {
                if(current_role == ROLE_KNIGHT && current_race == RACE_DWARF)
                    pet_glyph = current_role >= 0 ? monnum_to_glyph(PM_RAM) : NO_GLYPH;
                else
                    pet_glyph = current_role >= 0 ? monnum_to_glyph(roles[current_role].petnum) : NO_GLYPH;
                pet_tile = glyph2tile[pet_glyph];
                pet_x = SDL_TILEBMP_X(pet_tile);
                pet_y = SDL_TILEBMP_Y(pet_tile);
                petimg = nk_subimage_id(pic, tilemap_width, tilemap_height, nk_rect((float)pet_x, (float)pet_y, (float)TILE_X, (float)TILE_Y));
            }
            else
            {
                pet_glyph = PM_LITTLE_DOG + GLYPH_MON_OFF;
                pet_tile = glyph2tile[pet_glyph];
                pet_x = SDL_TILEBMP_X(pet_tile);
                pet_y = SDL_TILEBMP_Y(pet_tile);
                petimg = nk_subimage_id(pic, tilemap_width, tilemap_height, nk_rect((float)pet_x, (float)pet_y, (float)TILE_X, (float)TILE_Y));

                pet_glyph2 = PM_KITTEN + GLYPH_MON_OFF;
                pet_tile2 = glyph2tile[pet_glyph2];
                pet_x2 = SDL_TILEBMP_X(pet_tile2);
                pet_y2 = SDL_TILEBMP_Y(pet_tile2);
                petimg2 = nk_subimage_id(pic, tilemap_width, tilemap_height, nk_rect((float)pet_x2, (float)pet_y2, (float)TILE_X, (float)TILE_Y));
            }

            /* GUI */
            if (nk_begin(ctx, "PlayerSelection", nk_rect((float)plsel_x, (float)plsel_y, (float)plsel_width, (float)plsel_height),
                NK_WINDOW_NO_SCROLLBAR))
            {
                if (media.diablo36)
                    nk_style_set_font(ctx, &media.diablo36->handle);            
                nk_layout_row_dynamic(ctx, 30, 1);
                nk_label(ctx, "Character", NK_TEXT_CENTERED);
                if (media.diablo36 && media.diablo24)
                    nk_style_set_font(ctx, &media.diablo24->handle);

                nk_layout_row_static(ctx, 25, 300, 2);
                nk_label(ctx, "Player Name:", NK_TEXT_LEFT);
                nk_label(ctx, plname, NK_TEXT_LEFT);

                if (media.diablo30)
                    nk_style_set_font(ctx, &media.diablo30->handle);
                nk_layout_row_dynamic(ctx, 10, 1);
                nk_layout_row_dynamic(ctx, 25, 1);
                nk_label(ctx, "Class", NK_TEXT_LEFT);
                if (media.diablo24)
                    nk_style_set_font(ctx, &media.diablo24->handle);

                /* default combobox */
                nk_layout_row_dynamic(ctx, 25, 1);
                comboWasPressed = comboIsPressed;
                comboIsPressed = nk_widget_has_mouse_click_down(ctx, NK_BUTTON_LEFT, TRUE);
                int previous_role = current_role;
                current_role = nk_combo(ctx, role_names_male, NK_LEN(role_names_male), current_role, 25, nk_vec2(300, NUM_ROLES * (25 + 2 * ctx->style.combo.content_padding.y)));
                if (current_role != previous_role)
                {
                    sound_info.ghsound = GHSOUND_UI_MENU_SELECT;
                    sdl_play_immediate_ghsound(sound_info);
                    sound_info.ghsound = GHSOUND_UI_BUTTON_DOWN;
                }

                if (media.diablo30)
                    nk_style_set_font(ctx, &media.diablo30->handle);
                nk_layout_row_dynamic(ctx, 10, 1);
                nk_layout_row_dynamic(ctx, 25, 1);
                nk_label(ctx, "Race", NK_TEXT_LEFT);
                if (media.diablo24)
                    nk_style_set_font(ctx, &media.diablo24->handle);
                nk_layout_row_static(ctx, 25, 150, NUM_RACES);
                boolean curraceok = ok_race(current_role, current_race, ROLE_RANDOM, ROLE_RANDOM);
                for (int i = 0; i < NUM_RACES; i++)
                {
                    raceWasPressed[i] = raceIsPressed[i];
                    raceIsPressed[i] = nk_widget_has_mouse_click_down(ctx, NK_BUTTON_LEFT, TRUE);

                    if (ok_race(current_role, i, ROLE_RANDOM, ROLE_RANDOM))
                    {
                        if (!curraceok)
                            current_race = i;
                        if (nk_option_label(ctx, race_names[i], current_race == i))
                        {
                            if (!raceIsPressed[i] && raceWasPressed[i]) 
                            {
                                sound_info.ghsound = GHSOUND_UI_MENU_SELECT;
                                sdl_play_immediate_ghsound(sound_info);
                                sound_info.ghsound = GHSOUND_UI_BUTTON_DOWN;
                            }

                            current_race = i;
                        }
                    }
                    else
                    {
                        toggle->text_normal = nk_rgb(180, 180, 180);
                        toggle->text_hover = nk_rgb(180, 180, 180);
                        toggle->text_active = nk_rgb(180, 180, 180);
                        (void)nk_option_label(ctx, race_names[i], FALSE);
                        toggle->text_normal = nk_rgb(60, 60, 60);
                        toggle->text_hover = nk_rgb(60, 60, 60);
                        toggle->text_active = nk_rgb(60, 60, 60);
                    }
                }

                if (media.diablo30)
                    nk_style_set_font(ctx, &media.diablo30->handle);
                nk_layout_row_dynamic(ctx, 10, 1);
                nk_layout_row_dynamic(ctx, 25, 1);
                nk_label(ctx, "Gender", NK_TEXT_LEFT);
                if (media.diablo24)
                    nk_style_set_font(ctx, &media.diablo24->handle);
                nk_layout_row_static(ctx, 25, 150, 2);
                boolean curgenderok = ok_gend(current_role, current_race, current_gender, ROLE_RANDOM);
                for (int i = 0; i < 2; i++)
                {
                    genderWasPressed[i] = genderIsPressed[i];
                    genderIsPressed[i] = nk_widget_has_mouse_click_down(ctx, NK_BUTTON_LEFT, TRUE);

                    if (ok_gend(current_role, current_race, i, ROLE_RANDOM))
                    {
                        if (!curgenderok)
                            current_gender = i;

                        if(nk_option_label(ctx, gender_names[i], current_gender == i))
                        {
                            if (!genderIsPressed[i] && genderWasPressed[i])
                            {
                                sound_info.ghsound = GHSOUND_UI_MENU_SELECT;
                                sdl_play_immediate_ghsound(sound_info);
                                sound_info.ghsound = GHSOUND_UI_BUTTON_DOWN;
                            }

                            current_gender = i;
                        }
                    }
                    else
                    {
                        toggle->text_normal = nk_rgb(180, 180, 180);
                        toggle->text_hover = nk_rgb(180, 180, 180);
                        toggle->text_active = nk_rgb(180, 180, 180);
                        (void)nk_option_label(ctx, gender_names[i], FALSE);
                        toggle->text_normal = nk_rgb(60, 60, 60);
                        toggle->text_hover = nk_rgb(60, 60, 60);
                        toggle->text_active = nk_rgb(60, 60, 60);
                    }
                }

                if (media.diablo30)
                    nk_style_set_font(ctx, &media.diablo30->handle);
                nk_layout_row_dynamic(ctx, 10, 1);
                nk_layout_row_dynamic(ctx, 25, 1);
                nk_label(ctx, "Alignment", NK_TEXT_LEFT);
                if (media.diablo24)
                    nk_style_set_font(ctx, &media.diablo24->handle);
                nk_layout_row_static(ctx, 25, 150, 3);
                boolean curalignok = ok_align(current_role, current_race, current_gender, current_alignment);
                for (int i = 0; i < 3; i++)
                {
                    alignmentWasPressed[i] = alignmentIsPressed[i];
                    alignmentIsPressed[i] = nk_widget_has_mouse_click_down(ctx, NK_BUTTON_LEFT, TRUE);

                    if (ok_align(current_role, current_race, current_gender, i))
                    {
                        if (!curalignok)
                            current_alignment = i;
                        if (nk_option_label(ctx, alignment_names[i], current_alignment == i))
                        {
                            if (!alignmentIsPressed[i] && alignmentWasPressed[i])
                            {
                                sound_info.ghsound = GHSOUND_UI_MENU_SELECT;
                                sdl_play_immediate_ghsound(sound_info);
                                sound_info.ghsound = GHSOUND_UI_BUTTON_DOWN;
                            }

                            current_alignment = i;
                        }
                    }
                    else
                    {
                        toggle->text_normal = nk_rgb(180, 180, 180);
                        toggle->text_hover = nk_rgb(180, 180, 180);
                        toggle->text_active = nk_rgb(180, 180, 180);
                        (void)nk_option_label(ctx, alignment_names[i], FALSE);
                        toggle->text_normal = nk_rgb(60, 60, 60);
                        toggle->text_hover = nk_rgb(60, 60, 60);
                        toggle->text_active = nk_rgb(60, 60, 60);
                    }
                }

                flags.initrole = current_role;
                flags.initrace = current_race;
                flags.initgend = current_gender;
                flags.initalign = current_alignment;

                if (media.diablo30)
                    nk_style_set_font(ctx, &media.diablo30->handle);
                nk_layout_row_dynamic(ctx, 10, 1);
                nk_layout_row_dynamic(ctx, 25, 1);
                nk_label(ctx, "Preview", NK_TEXT_LEFT);

                if (media.diablo36)
                    nk_style_set_font(ctx, &media.diablo36->handle);

                nk_layout_row_static(ctx, (float)TILE_Y, TILE_X, 5);
                nk_image(ctx, nkimg);
                nk_label(ctx, pet_glyph == NO_GLYPH ? "" : "+", NK_TEXT_CENTERED);
                if(pet_glyph != NO_GLYPH)
                    nk_image(ctx, petimg);
                else
                    nk_label(ctx, "", NK_TEXT_LEFT);

                if (media.diablo24)
                    nk_style_set_font(ctx, &media.diablo24->handle);

                nk_label(ctx, pet_glyph2 == NO_GLYPH ? "" : "or", NK_TEXT_CENTERED);

                if (pet_glyph2 != NO_GLYPH)
                    nk_image(ctx, petimg2);
                else
                    nk_label(ctx, "", NK_TEXT_LEFT);

                nk_layout_row_static(ctx, 25, 75, 1);
                nk_layout_row_static(ctx, 45, 150, 3);
                if (nk_button_label(ctx, "Play"))
                {
                    sound_info.ghsound = GHSOUND_UI_BUTTON_DOWN;
                    res = 1;
                }
                if (nk_button_label(ctx, "Random"))
                {
                    plselRandomize();
                    current_role = flags.initrole;
                    current_race = flags.initrace;
                    current_gender = flags.initgend;
                    current_alignment = flags.initalign;
                    sdl_play_immediate_ghsound(sound_info);
                }
                if (nk_button_label(ctx, "Quit"))
                {
                    sound_info.ghsound = GHSOUND_UI_BUTTON_DOWN;
                    res = 2;
                }

            }
            nk_end(ctx);

            /* Draw */
            SDL_GetWindowSize(sdl.win, &sdlapp->win_width, &sdlapp->win_height);
            glViewport(0, 0, sdlapp->win_width, sdlapp->win_height);
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(bg.r, bg.g, bg.b, bg.a);
            /* IMPORTANT: `nk_sdl_render` modifies some global OpenGL state
             * with blending, scissor, face culling, depth test and viewport and
             * defaults everything back into a default state.
             * Make sure to either a.) save and restore or b.) reset your own state after
             * rendering the UI. */

#ifdef NK_SDL_GL2_IMPLEMENTATION
            nk_sdl_render(NK_ANTI_ALIASING_ON);
#endif
#ifdef NK_SDL_GLES2_IMPLEMENTATION
            nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
#endif

            SDL_GL_SwapWindow(sdl.win);

        }
        sdl_play_immediate_ghsound(sound_info);
        return res == 1 ? 1 : 0;

    cleanup:
        return 0;

    }

    enum nuklear_main_menu_command
    nuklear_main_menu(PGHSdlApp sdlapp)
    {
        int win_width = 0, win_height = 0;
        int buttons_width = 400, buttons_height = 360;
        SDL_GetWindowSize(sdl.win, &win_width, &win_height);
        int buttons_x = max(0, (win_width - buttons_width) / 2);
        int buttons_y = max(0, (win_height - buttons_height) / 2);
        enum nuklear_main_menu_command res = MAIN_MENU_NONE;
        char buf[BUFSZ];
        Sprintf(buf, "GnollHack %d.%d.%d", VERSION_MAJOR, VERSION_MINOR, PATCHLEVEL);

        struct ghsound_music_info music_info = { 0 };
        music_info.ghsound = GHSOUND_MUSIC_MAIN_MENU;
        music_info.volume = 1.0f;
        sdl_play_ghsound_music(music_info);

        /* default button */
        ctx->style.button.normal = nk_style_item_image(media.button);
        ctx->style.button.hover = nk_style_item_image(media.button_hover);
        ctx->style.button.active = nk_style_item_image(media.button_active);
        ctx->style.button.border_color = nk_rgba(0, 0, 0, 0);
        ctx->style.button.text_background = nk_rgba(0, 0, 0, 0);
        ctx->style.button.text_normal = nk_rgb(180, 180, 180);
        ctx->style.button.text_hover = nk_rgb(240, 240, 240);
        ctx->style.button.text_active = nk_rgb(120, 120, 120);

        /* default text */
        ctx->style.text.color = nk_rgb(60, 60, 60);

        /* window */
        ctx->style.window.padding = nk_vec2(32, 32);
        ctx->style.window.border = 16;

        struct ghsound_immediate_info sound_info = { 0 };
        sound_info.ghsound = GHSOUND_UI_BUTTON_DOWN;
        sound_info.volume = 1.0f;
        sound_info.sound_type = IMMEDIATE_SOUND_UI;

        while (sdlapp->running && res == MAIN_MENU_NONE)
        {
            /* Input */
            SDL_Event evt;
            nk_input_begin(ctx);
            while (SDL_PollEvent(&evt))
            {
                if (evt.type == SDL_QUIT)
                    goto cleanup;
                nk_sdl_handle_event(&evt);
            }
            nk_input_end(ctx);

            /* GUI */
            if (nk_begin(ctx, "Buttons", nk_rect((float)buttons_x, (float)buttons_y, (float)buttons_width, (float)buttons_height),
                NK_WINDOW_NO_SCROLLBAR))
            {
                if (media.diablo36)
                    nk_style_set_font(ctx, &media.diablo36->handle);
                nk_layout_row_dynamic(ctx, 50, 1);
                nk_label(ctx, "Main Menu", NK_TEXT_CENTERED);
                if (media.diablo24)
                    nk_style_set_font(ctx, &media.diablo24->handle);

                nk_layout_row_dynamic(ctx, 50, 1);
                nk_label(ctx, buf, NK_TEXT_CENTERED);

                nk_layout_row_dynamic(ctx, 87, 1);
                if (nk_button_label(ctx, "Start Game"))
                {
                    res = MAIN_MENU_START_GAME;
                }

                nk_layout_row_dynamic(ctx, 87, 1);
                if (nk_button_label(ctx, "Exit Game"))
                {
                    res = MAIN_MENU_EXIT_GAME;
                }
            }
            nk_end(ctx);

            /* ----------------------------------------- */

            /* Draw */
            SDL_GetWindowSize(sdl.win, &sdlapp->win_width, &sdlapp->win_height);
            glViewport(0, 0, sdlapp->win_width, sdlapp->win_height);
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(bg.r, bg.g, bg.b, bg.a);
            /* IMPORTANT: `nk_sdl_render` modifies some global OpenGL state
             * with blending, scissor, face culling, depth test and viewport and
             * defaults everything back into a default state.
             * Make sure to either a.) save and restore or b.) reset your own state after
             * rendering the UI. */

#ifdef NK_SDL_GL2_IMPLEMENTATION
            nk_sdl_render(NK_ANTI_ALIASING_ON);
#endif
#ifdef NK_SDL_GLES2_IMPLEMENTATION
            nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
#endif

            SDL_GL_SwapWindow(sdl.win);
        }
        sdl_play_immediate_ghsound(sound_info);
        return res;

    cleanup:
        return MAIN_MENU_EXIT_GAME;

    }


    boolean
    nuklear_splash_screen(PGHSdlApp sdlapp)
    {
        int win_width = 0, win_height = 0;
        SDL_GetWindowSize(sdl.win, &win_width, &win_height);
        int res = 0;
        char buf[BUFSZ];
        Sprintf(buf, "GnollHack %d.%d.%d", VERSION_MAJOR, VERSION_MINOR, PATCHLEVEL);

        int logo_width, logo_height;
        size_t logo_size;
        GLuint pic = gl2_image_load_from_resource(sdlapp->hApp, IDB_PNG_GNHLOGO, &logo_width, &logo_height, &logo_size);
        struct nk_image logoimg;

        int left_padding = 32;
        int buttons_width = max(240, logo_width) + 2 * left_padding, buttons_height = 200 + logo_height;
        int buttons_x = max(0, (win_width - buttons_width) / 2);
        int buttons_y = max(0, (win_height - buttons_height) / 2);

        struct ghsound_music_info music_info = { 0 };
        music_info.ghsound = GHSOUND_MUSIC_SPLASH;
        music_info.volume = 1.0f;
        sdl_play_ghsound_music(music_info);

        /* default button */
        ctx->style.button.normal = nk_style_item_image(media.button);
        ctx->style.button.hover = nk_style_item_image(media.button_hover);
        ctx->style.button.active = nk_style_item_image(media.button_active);
        ctx->style.button.border_color = nk_rgba(0, 0, 0, 0);
        ctx->style.button.text_background = nk_rgba(0, 0, 0, 0);
        ctx->style.button.text_normal = nk_rgb(180, 180, 180);
        ctx->style.button.text_hover = nk_rgb(240, 240, 240);
        ctx->style.button.text_active = nk_rgb(120, 120, 120);

        /* default text */
        ctx->style.text.color = nk_rgb(60, 60, 60);

        /* window */
        ctx->style.window.padding = nk_vec2((float)left_padding, (float)32);
        ctx->style.window.border = 16;

        struct ghsound_immediate_info sound_info = { 0 };
        sound_info.ghsound = GHSOUND_UI_BUTTON_DOWN;
        sound_info.volume = 1.0f;
        sound_info.sound_type = IMMEDIATE_SOUND_UI;

        while (sdlapp->running && res == MAIN_MENU_NONE)
        {
            /* Input */
            SDL_Event evt;
            nk_input_begin(ctx);
            while (SDL_PollEvent(&evt))
            {
                if (evt.type == SDL_QUIT)
                    goto cleanup;
                nk_sdl_handle_event(&evt);
            }
            nk_input_end(ctx);

            /* GUI */
            if (nk_begin(ctx, "Buttons", nk_rect((float)buttons_x, (float)buttons_y, (float)buttons_width, (float)buttons_height),
                NK_WINDOW_NO_SCROLLBAR))
            {
                nk_layout_row_static(ctx, (float)logo_height, logo_width, 1);
                logoimg = nk_image_id(pic);
                nk_image(ctx, logoimg);

                nk_layout_row_dynamic(ctx, 50, 1);
                nk_label(ctx, buf, NK_TEXT_CENTERED);

                nk_layout_row_dynamic(ctx, 44, 1);
                if (nk_button_label(ctx, "OK"))
                {
                    res = 1;
                }

            }
            nk_end(ctx);

            /* ----------------------------------------- */

            /* Draw */
            SDL_GetWindowSize(sdl.win, &sdlapp->win_width, &sdlapp->win_height);
            glViewport(0, 0, sdlapp->win_width, sdlapp->win_height);
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(bg.r, bg.g, bg.b, bg.a);
            /* IMPORTANT: `nk_sdl_render` modifies some global OpenGL state
                * with blending, scissor, face culling, depth test and viewport and
                * defaults everything back into a default state.
                * Make sure to either a.) save and restore or b.) reset your own state after
                * rendering the UI. */

#ifdef NK_SDL_GL2_IMPLEMENTATION
            nk_sdl_render(NK_ANTI_ALIASING_ON);
#endif
#ifdef NK_SDL_GLES2_IMPLEMENTATION
            nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
#endif

            SDL_GL_SwapWindow(sdl.win);
        }
        sdl_play_immediate_ghsound(sound_info);
        return res == 1 ? 1 : 0;

    cleanup:
        return 0;

    }
#endif





    static boolean plselRandomize()
    {
        boolean fully_specified = TRUE;

        if (!flags.randomall) {
            if (flags.initrole == ROLE_NONE || flags.initrace == ROLE_NONE
                || flags.initgend == ROLE_NONE || flags.initalign == ROLE_NONE)
                fully_specified = FALSE;
        }

        flags.initrole = ROLE_RANDOM;
        flags.initrace = ROLE_RANDOM;
        flags.initgend = ROLE_RANDOM;
        flags.initalign = ROLE_RANDOM;

        rigid_role_checks();

        int role = flags.initrole;
        int race = flags.initrace;
        int gender = flags.initgend;
        int alignment = flags.initalign;

        assert(role != ROLE_RANDOM && role != ROLE_NONE);
        assert(race != ROLE_RANDOM && race != ROLE_NONE);
        assert(gender != ROLE_RANDOM && gender != ROLE_NONE);
        assert(alignment != ROLE_RANDOM && alignment != ROLE_NONE);

        if (!ok_role(role, race, gender, alignment)) {
            fully_specified = FALSE;
            flags.initrole = ROLE_RANDOM;
        }

        if (!ok_race(role, race, gender, alignment)) {
            fully_specified = FALSE;
            flags.initrace = ROLE_RANDOM;
        }

        if (!ok_gend(role, race, gender, alignment)) {
            fully_specified = FALSE;
            flags.initgend = ROLE_RANDOM;
        }

        if (!ok_align(role, race, gender, alignment))
        {
            fully_specified = FALSE;
            flags.initalign = ROLE_RANDOM;
        }

        rigid_role_checks();

        role = flags.initrole;
        race = flags.initrace;
        gender = flags.initgend;
        alignment = flags.initalign;

        assert(role != ROLE_RANDOM && role != ROLE_NONE);
        assert(race != ROLE_RANDOM && race != ROLE_NONE);
        assert(gender != ROLE_RANDOM && gender != ROLE_NONE);
        assert(alignment != ROLE_RANDOM && alignment != ROLE_NONE);
        assert(ok_role(role, race, gender, alignment));
        assert(ok_race(role, race, gender, alignment));
        assert(ok_gend(role, race, gender, alignment));
        assert(ok_align(role, race, gender, alignment));

        return fully_specified;
    }


 /* sdlnuklear.c */