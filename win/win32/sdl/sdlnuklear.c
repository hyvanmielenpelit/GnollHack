/*
 * sdlnuklear.c
 * GnollHack Nuklear Routines
 *
 * Copyright (c) Janne Gustafsson, 2021
 */

#pragma comment( lib, "OpenGL32" )          

#include "win10.h"
#include <windows.h>
#include "resource.h"


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

#include <SDL.h>
#include <SDL_opengl.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL2_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_sdl_gl2.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

/* ===============================================================
 *
 *                          EXAMPLE
 *
 * ===============================================================*/
 /* This are some code examples to provide a small overview of what can be
  * done with this library. To try out an example uncomment the defines */
  /*#define INCLUDE_ALL */
  /*#define INCLUDE_STYLE */
  /*#define INCLUDE_CALCULATOR */
  /*#define INCLUDE_OVERVIEW */
  /*#define INCLUDE_NODE_EDITOR */

#ifdef INCLUDE_ALL
#define INCLUDE_STYLE
#define INCLUDE_CALCULATOR
#define INCLUDE_OVERVIEW
#define INCLUDE_NODE_EDITOR
#endif

#ifdef INCLUDE_STYLE
#include "../style.c"
#endif
#ifdef INCLUDE_CALCULATOR
#include "../calculator.c"
#endif
#ifdef INCLUDE_OVERVIEW
#include "../overview.c"
#endif
#ifdef INCLUDE_NODE_EDITOR
#include "../node_editor.c"
#endif

#if 0
        if(1)
        {   /* skin */
glEnable(GL_TEXTURE_2D);
media.skin = image_load_from_resource(hInstance, IDB_PNG_SDL_NUKLEAR_TEST);
media.check = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 32, 15, 15));
media.check_cursor = nk_subimage_id(media.skin, 512, 512, nk_rect(450, 34, 11, 11));
media.option = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 64, 15, 15));
media.option_cursor = nk_subimage_id(media.skin, 512, 512, nk_rect(451, 67, 9, 9));
media.header = nk_subimage_id(media.skin, 512, 512, nk_rect(128, 0, 127, 24));
media.window = nk_subimage_id(media.skin, 512, 512, nk_rect(128, 23, 127, 104));
media.scrollbar_inc_button = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 256, 15, 15));
media.scrollbar_inc_button_hover = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 320, 15, 15));
media.scrollbar_dec_button = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 224, 15, 15));
media.scrollbar_dec_button_hover = nk_subimage_id(media.skin, 512, 512, nk_rect(464, 288, 15, 15));
media.button = nk_subimage_id(media.skin, 512, 512, nk_rect(384, 336, 127, 31));
media.button_hover = nk_subimage_id(media.skin, 512, 512, nk_rect(384, 368, 127, 31));
media.button_active = nk_subimage_id(media.skin, 512, 512, nk_rect(384, 400, 127, 31));
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
ctx->style.window.padding = nk_vec2(8, 4);
ctx->style.window.border = 3;

/* window header */
ctx->style.window.header.normal = nk_style_item_image(media.header);
ctx->style.window.header.hover = nk_style_item_image(media.header);
ctx->style.window.header.active = nk_style_item_image(media.header);
ctx->style.window.header.label_normal = nk_rgb(95, 95, 95);
ctx->style.window.header.label_hover = nk_rgb(50, 50, 50);
ctx->style.window.header.label_active = nk_rgb(95, 95, 95);

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
        }
#endif

/* ===============================================================
 *
 *                          EXAMPLE
 *
 * ===============================================================*/
 /* This are some code examples to provide a small overview of what can be
  * done with this library. To try out an example uncomment the defines */
  /*#define INCLUDE_ALL */
  /*#define INCLUDE_STYLE */
  /*#define INCLUDE_CALCULATOR */
  /*#define INCLUDE_OVERVIEW */
  /*#define INCLUDE_NODE_EDITOR */

#ifdef INCLUDE_ALL
#define INCLUDE_STYLE
#define INCLUDE_CALCULATOR
#define INCLUDE_OVERVIEW
#define INCLUDE_NODE_EDITOR
#endif

#ifdef INCLUDE_STYLE
#include "../style.c"
#endif
#ifdef INCLUDE_CALCULATOR
#include "../calculator.c"
#endif
#ifdef INCLUDE_OVERVIEW
#include "../overview.c"
#endif
#ifdef INCLUDE_NODE_EDITOR
#include "../node_editor.c"
#endif


struct media {
    GLint skin;
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
};

/* ===============================================================
 *
 *                          EXAMPLE
 *
 * ===============================================================*/
 /* This are some code examples to provide a small overview of what can be
  * done with this library. To try out an example uncomment the include
  * and the corresponding function. */
  /*#include "../style.c"*/
  /*#include "../calculator.c"*/
  /*#include "../overview.c"*/
  /*#include "../node_editor.c"*/

  /* ===============================================================
   *
   *                          DEMO
   *
   * ===============================================================*/




int
StartNuklearExample(HINSTANCE hInstance)
{
    /* Platform */
    SDL_Window* win;
    SDL_GLContext glContext;
    int win_width, win_height;
    int running = 1;

    /* GUI */
    struct nk_context* ctx;
    struct nk_colorf bg;

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
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    glContext = SDL_GL_CreateContext(win);
    SDL_GetWindowSize(win, &win_width, &win_height);

    /* GUI */
    ctx = nk_sdl_init(win);
    /* Load Fonts: if none of these are loaded a default font will be used  */
    /* Load Cursor: if you uncomment cursor loading please hide the cursor */
    {struct nk_font_atlas* atlas;
    nk_sdl_font_stash_begin(&atlas);
    /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
    /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 16, 0);*/
    /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
    /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
    /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
    /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
    nk_sdl_font_stash_end();
    /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
    /*nk_style_set_font(ctx, &roboto->handle)*/; }

#ifdef INCLUDE_STYLE
    /*set_style(ctx, THEME_WHITE);*/
    /*set_style(ctx, THEME_RED);*/
    /*set_style(ctx, THEME_BLUE);*/
    /*set_style(ctx, THEME_DARK);*/
#endif

    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
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

        /* GUI */
        if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
            NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
        {
            enum { EASY, HARD };
            static int op = EASY;
            static int property = 20;

            nk_layout_row_static(ctx, 30, 80, 1);
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
#ifdef INCLUDE_CALCULATOR
        calculator(ctx);
#endif
#ifdef INCLUDE_OVERVIEW
        overview(ctx);
#endif
#ifdef INCLUDE_NODE_EDITOR
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
        nk_sdl_render(NK_ANTI_ALIASING_ON);
        SDL_GL_SwapWindow(win);
    }

cleanup:
    nk_sdl_shutdown();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}


 /* sdlnuklear.c */