/*
 * Nuklear - 1.32.0 - public domain
 * no warrenty implied; use at your own risk.
 * authored from 2015-2017 by Micha Mettke
 * 
 * Actualizacion por Jhoson Ozuna(slam) - hbiblia@g
 */
/*
 * ==============================================================
 *
 *                              API
 *
 * ===============================================================
 */
#pragma comment(lib, "SDL2_ttf")

#ifndef NK_SDL_GFX_H_
#define NK_SDL_GFX_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>

typedef struct NKSdlFont NKSdlFont;
NK_API struct nk_context*   nk_sdl_native_init(SDL_Window *win, SDL_Renderer *renderer);
NK_API void                 nk_sdl_native_handle_event(SDL_Event *evt);
NK_API void                 nk_sdl_native_render(void);
NK_API void                 nk_sdl_native_shutdown(void);

NK_API void                 nk_sdl_native_font_stash_begin(struct nk_font_atlas **atlas);
NK_API void                 nk_sdl_native_font_stash_end(void);
NK_API void                 nk_sdl_native_font_create_from_file(const char *file_name, int font_size, int flags);
NK_API void                 nk_sdl_native_font_del(void);

#endif
/*
 * ==============================================================
 *
 *                          IMPLEMENTATION
 *
 * ===============================================================
 */
#ifdef NK_SDL_GFX_IMPLEMENTATION

#ifndef NK_SDL_TEXT_MAX
#define NK_SDL_TEXT_MAX 256
#endif

static struct nk_sdl_native {
    SDL_Window *win;
    SDL_Renderer *renderer;
    struct nk_context ctx;
    struct nk_buffer cmds;
    // font data
    TTF_Font *ttf_font;
    struct nk_user_font *user_font;
    int font_height;
    struct nk_font_atlas atlas;
} sdl_native;

NK_API void nk_sdl_native_font_create_from_file(const char *file_name, int font_size, int flags)
{
    TTF_Init();

    sdl_native.ttf_font = TTF_OpenFont(file_name, font_size);
    if (sdl_native.ttf_font == NULL) {
        fprintf(stdout, "Unable to load font file: %s\n", file_name);
    }
}

NK_API void
nk_sdl_native_font_del(void)
{
    if(!sdl_native.ttf_font) return;
    TTF_CloseFont(sdl_native.ttf_font);
}

NK_INTERN void
nk_sdl_native_device_upload_atlas(const void *image, int width, int height)
{
    #if 0
    struct nk_sdl_native_device *dev = &sdl_native.ogl;
    glGenTextures(1, &dev->font_tex);
    glBindTexture(GL_TEXTURE_2D, dev->font_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, image);
    #endif
}

void sdl_draw_text(TTF_Font *font, const char *str, int x, int y, struct nk_color c) {
    SDL_Surface *surface = TTF_RenderText_Blended(font, str, (SDL_Color){c.r, c.g, c.b});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(sdl_native.renderer, surface);
    int texW = 0, texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {x, y, texW, texH };
    SDL_RenderCopy(sdl_native.renderer, texture, NULL, &dstrect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void sdl_draw_rect(int x, int y, int w, int h, int rounding, struct nk_color color) {
    roundedRectangleRGBA(sdl_native.renderer, x, y, (x + w), (y + h),
    rounding, color.r, color.g, color.b, color.a);
}

void sdl_draw_rect_fill(int x, int y, int w, int h, int rounding, struct nk_color color) {
    roundedBoxRGBA(sdl_native.renderer, x, y, (x + w), (y + h),
    NK_MAX(rounding, 2), color.r, color.g, color.b, color.a);
}

void sdl_draw_line(int x, int y, int x2, int y2, struct nk_color color) {
    lineRGBA(sdl_native.renderer, x, y, x2, y2, color.r, color.g, color.b, color.a);
}

void sdl_draw_ellipse(int x, int y, int rx, int ry, struct nk_color color) {
    ellipseRGBA(sdl_native.renderer, x, y, rx, ry, color.r, color.g, color.b, color.a);
}

void sdl_draw_ellipse_filled(int x, int y, int rx, int ry, struct nk_color color) {
    filledEllipseRGBA(sdl_native.renderer, x, y, rx, ry, color.r, color.g, color.b, color.a);
}

void sdl_draw_circle_filled(int x, int y, int rad, struct nk_color color) {
    filledCircleRGBA(sdl_native.renderer, x, y, rad, color.r, color.g, color.b, color.a);
}

void sdl_draw_triangle(int x1,int y1, int x2, int y2, int x3, int y3, struct nk_color color) {
    trigonRGBA(sdl_native.renderer, x1,y1, x2,y2, x3,y3, color.r, color.g, color.b, color.a);
}

void sdl_draw_filled_triangle(int x1,int y1, int x2, int y2, int x3, int y3, struct nk_color color) {
    filledTrigonRGBA(sdl_native.renderer, x1,y1, x2,y2, x3,y3, color.r, color.g, color.b, color.a);
}

void sdl_draw_polyline(const Sint16 *vx, const Sint16 *vy, int n, struct nk_color color) {
    polygonRGBA(sdl_native.renderer, vx, vy, n, color.r, color.g, color.b, color.a);
}

void sdl_draw_arc(int x, int y, int rad, int start, int end, struct nk_color color) {
    arcRGBA(sdl_native.renderer, x,y, rad, start,end, color.r, color.g, color.b, color.a);
}

void sdl_draw_filled_polygon(const Sint16 *vx, const Sint16 *vy, int n, struct nk_color color) {
    filledPolygonRGBA(sdl_native.renderer, vx, vy, n, color.r, color.g, color.b, color.a);
}

void sdl_draw_image(const struct nk_command_image *image, int x, int y, int w, int h) 
{
    SDL_Texture *t = SDL_CreateTexture(sdl_native.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, w, h);
    SDL_SetTextureColorMod(t, image->col.r,image->col.g, image->col.b);
    SDL_Rect rect = (SDL_Rect){ x, y, w, h };
    SDL_UpdateTexture(t, &rect, (const void*)image->img.handle.ptr, w * 4); // sizeof(*image->img.handle.ptr));
    SDL_RenderCopy(sdl_native.renderer, t, NULL, &rect);
}

NK_API void
nk_sdl_native_render(void)
{
    const struct nk_command *cmd;
    nk_foreach(cmd, &sdl_native.ctx)
    {
    //    Uint32 color; temporalmente no tiene uso. 
       switch (cmd->type) {
            case NK_COMMAND_NOP: {}break;
            case NK_COMMAND_SCISSOR: {
                const struct nk_command_scissor *s = (const struct nk_command_scissor*)cmd;
                SDL_RenderSetClipRect(sdl_native.renderer, &(SDL_Rect){(int)s->x, (int)s->y, (int)s->w, (int)s->h} );
            }break;
            case NK_COMMAND_LINE: {
                const struct nk_command_line *l = (const struct nk_command_line *)cmd;
                sdl_draw_line((int)l->begin.x, (int)l->begin.y, (int)l->end.x, (int)l->end.y, l->color);
            }break;
            case NK_COMMAND_RECT: {
                const struct nk_command_rect *r = (const struct nk_command_rect *)cmd;
                sdl_draw_rect(r->x, r->y, r->w, r->h, r->rounding, r->color);
            }break;
            case NK_COMMAND_RECT_FILLED: {
                const struct nk_command_rect_filled *r = (const struct nk_command_rect_filled *)cmd;
                sdl_draw_rect_fill(r->x, r->y, r->w, r->h, r->rounding, r->color);
            }break;
            case NK_COMMAND_CIRCLE: {
                const struct nk_command_circle *c = (const struct nk_command_circle *)cmd;
                int xr, yr;
                xr = c->w/2;
                yr = c->h/2;
                sdl_draw_ellipse(((int)(c->x)) + xr, ((int)c->y) + yr, xr, yr, c->color);
            }break;
            case NK_COMMAND_CIRCLE_FILLED: {
                const struct nk_command_circle_filled *c = (const struct nk_command_circle_filled *)cmd;
                int xr, yr;
                xr = c->w/2;
                yr = c->h/2;
                sdl_draw_ellipse_filled(((int)(c->x)) + xr, ((int)c->y) + yr, xr, yr, c->color);
            }break;
            case NK_COMMAND_TRIANGLE: {
                const struct nk_command_triangle*t = (const struct nk_command_triangle*)cmd;
                sdl_draw_triangle((int)t->a.x, (int)t->a.y, (int)t->b.x, (int)t->b.y,
                    (int)t->c.x, (int)t->c.y, t->color);
            }break;
            case NK_COMMAND_TRIANGLE_FILLED: {
                const struct nk_command_triangle_filled *t = (const struct nk_command_triangle_filled *)cmd;
                sdl_draw_filled_triangle((int)t->a.x, (int)t->a.y, (int)t->b.x,
                    (int)t->b.y, (int)t->c.x, (int)t->c.y, t->color);
            }break;
            case NK_COMMAND_POLYGON: {
                const struct nk_command_polygon *p = (const struct nk_command_polygon*)cmd;
                int i;
                Sint16* verticesx = (Sint16*)malloc((size_t)(p->point_count * 2) * sizeof(Sint16)); // [p->point_count * 2] ;
                Sint16* verticesy = (Sint16*)malloc((size_t)(p->point_count * 2) * sizeof(Sint16)); // [p->point_count * 2] ;
                for (i = 0; i < p->point_count; i++) {
                    verticesx[i * 2] = p->points[i].x;
                    verticesy[(i * 2) + 1] = p->points[i].y;
                }
                sdl_draw_polyline((const Sint16*)verticesx, (const Sint16*)verticesy, (int)p->point_count, p->color);
                free(verticesx);
                free(verticesy);
            }break;
            case NK_COMMAND_POLYGON_FILLED: {
                const struct nk_command_polygon_filled *p = (const struct nk_command_polygon_filled *)cmd;
                int i;
                Sint16* verticesx = (Sint16*)malloc((size_t)(p->point_count * 2) * sizeof(Sint16)); // [p->point_count * 2] ;
                Sint16* verticesy = (Sint16*)malloc((size_t)(p->point_count * 2) * sizeof(Sint16)); // [p->point_count * 2] ;
                for (i = 0; i < p->point_count; i++) {
                    verticesx[i * 2] = p->points[i].x;
                    verticesy[(i * 2) + 1] = p->points[i].y;
                }
                sdl_draw_filled_polygon((const Sint16*)&verticesx, (const Sint16*)&verticesy, (int)p->point_count, p->color);
                free(verticesx);
                free(verticesy);

            }break;
            case NK_COMMAND_POLYLINE: {}break;
            case NK_COMMAND_TEXT: {
                const struct nk_command_text *t = (const struct nk_command_text*)cmd;
                TTF_Font *font = (TTF_Font*)t->font->userdata.ptr;
                sdl_draw_text(font, (const char*)t->string, t->x, t->y, t->foreground);
            }break;
            case NK_COMMAND_CURVE: {}break;
            case NK_COMMAND_ARC: {
                const struct nk_command_arc *a = (const struct nk_command_arc *)cmd;
                sdl_draw_arc((int)a->cx, (int)a->cy, (int)a->r, (int)a->a[0], (int)a->a[1], a->color);
            }break;
            case NK_COMMAND_IMAGE: {
                const struct nk_command_image *i = (const struct nk_command_image *)cmd;
                sdl_draw_image(i, i->x, i->y, i->w, i->h);
            }break;
            case NK_COMMAND_RECT_MULTI_COLOR:
            case NK_COMMAND_ARC_FILLED:
            default: break;
       }
    }
    nk_clear(&sdl_native.ctx);
}

static void
nk_sdl_native_clipboard_paste(nk_handle usr, struct nk_text_edit *edit)
{
    const char *text = SDL_GetClipboardText();
    if (text) nk_textedit_paste(edit, text, nk_strlen(text));
    (void)usr;
}

static void
nk_sdl_native_clipboard_copy(nk_handle usr, const char *text, int len)
{
    char *str = 0;
    (void)usr;
    if (!len) return;
    str = (char*)malloc((size_t)len+1);
    if (!str) return;
    memcpy(str, text, (size_t)len);
    str[len] = '\0';
    SDL_SetClipboardText(str);
    free(str);
}

static float
nk_sdl_native_font_get_text_width(nk_handle handle, float height, const char *text, int len)
{
    TTF_Font *font = (TTF_Font*)handle.ptr;
    if (!font || !text) {
        return 0;
    }
    /* We must copy into a new buffer with exact length null-terminated
       as nuklear uses variable size buffers and al_get_text_width doesn't
       accept a length, it infers length from null-termination
       (which is unsafe API design by allegro devs!) */
    char* tmp_buffer = (char*)malloc((size_t)len * sizeof(char));
    strncpy((char*)&tmp_buffer, text, len);
    tmp_buffer[len] = '\0';
    
    int w, h;
    TTF_SizeText(font, tmp_buffer, &w, &h);
    free(tmp_buffer);
    return (float)w;
}

NK_API struct nk_context*
nk_sdl_native_init(SDL_Window *win, SDL_Renderer *renderer)
{
    struct nk_user_font *font = sdl_native.user_font;
    font->userdata = nk_handle_ptr(sdl_native.ttf_font);
    font->height = (float)TTF_FontHeight(sdl_native.ttf_font);
    font->width = nk_sdl_native_font_get_text_width;

    sdl_native.win = win;
    sdl_native.renderer = renderer;
    
    nk_init_default(&sdl_native.ctx, font);
    nk_buffer_init_default(&sdl_native.cmds);
    
    sdl_native.ctx.clip.copy = nk_sdl_native_clipboard_copy;
    sdl_native.ctx.clip.paste = nk_sdl_native_clipboard_paste;
    sdl_native.ctx.clip.userdata = nk_handle_ptr(0);

    return &sdl_native.ctx;
}

NK_API void
nk_sdl_native_font_stash_begin(struct nk_font_atlas **atlas)
{
    nk_font_atlas_init_default(&sdl_native.atlas);
    nk_font_atlas_begin(&sdl_native.atlas);
    *atlas = &sdl_native.atlas;
}

NK_API void
nk_sdl_native_font_stash_end(void)
{
    const void *image; int w, h;
    image = nk_font_atlas_bake(&sdl_native.atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    nk_sdl_native_device_upload_atlas(image, w, h);
    //nk_font_atlas_end(&sdl_native.atlas, nk_handle_id((int)sdl_native.ogl.font_tex), &sdl_native.ogl.null);
    if (sdl_native.atlas.default_font)
        nk_style_set_font(&sdl_native.ctx, &sdl_native.atlas.default_font->handle);
}

NK_API void
nk_sdl_native_handle_event(SDL_Event *evt)
{
    struct nk_context *ctx = &sdl_native.ctx;

    /* optional grabbing behavior */
    if (ctx->input.mouse.grab) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        ctx->input.mouse.grab = 0;
    } else if (ctx->input.mouse.ungrab) {
        int x = (int)ctx->input.mouse.prev.x, y = (int)ctx->input.mouse.prev.y;
        SDL_SetRelativeMouseMode(SDL_FALSE);
        SDL_WarpMouseInWindow(sdl_native.win, x, y);
        ctx->input.mouse.ungrab = 0;
    }
    if (evt->type == SDL_KEYUP || evt->type == SDL_KEYDOWN) {
        /* key events */
        int down = evt->type == SDL_KEYDOWN;
        const Uint8* state = SDL_GetKeyboardState(0);
        SDL_Keycode sym = evt->key.keysym.sym;
        if (sym == SDLK_RSHIFT || sym == SDLK_LSHIFT)
            nk_input_key(ctx, NK_KEY_SHIFT, down);
        else if (sym == SDLK_DELETE)
            nk_input_key(ctx, NK_KEY_DEL, down);
        else if (sym == SDLK_RETURN)
            nk_input_key(ctx, NK_KEY_ENTER, down);
        else if (sym == SDLK_TAB)
            nk_input_key(ctx, NK_KEY_TAB, down);
        else if (sym == SDLK_BACKSPACE)
            nk_input_key(ctx, NK_KEY_BACKSPACE, down);
        else if (sym == SDLK_HOME) {
            nk_input_key(ctx, NK_KEY_TEXT_START, down);
            nk_input_key(ctx, NK_KEY_SCROLL_START, down);
        } else if (sym == SDLK_END) {
            nk_input_key(ctx, NK_KEY_TEXT_END, down);
            nk_input_key(ctx, NK_KEY_SCROLL_END, down);
        } else if (sym == SDLK_PAGEDOWN) {
            nk_input_key(ctx, NK_KEY_SCROLL_DOWN, down);
        } else if (sym == SDLK_PAGEUP) {
            nk_input_key(ctx, NK_KEY_SCROLL_UP, down);
        } else if (sym == SDLK_z)
            nk_input_key(ctx, NK_KEY_TEXT_UNDO, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_r)
            nk_input_key(ctx, NK_KEY_TEXT_REDO, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_c)
            nk_input_key(ctx, NK_KEY_COPY, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_v)
            nk_input_key(ctx, NK_KEY_PASTE, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_x)
            nk_input_key(ctx, NK_KEY_CUT, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_b)
            nk_input_key(ctx, NK_KEY_TEXT_LINE_START, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_e)
            nk_input_key(ctx, NK_KEY_TEXT_LINE_END, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_UP)
            nk_input_key(ctx, NK_KEY_UP, down);
        else if (sym == SDLK_DOWN)
            nk_input_key(ctx, NK_KEY_DOWN, down);
        else if (sym == SDLK_LEFT) {
            if (state[SDL_SCANCODE_LCTRL])
                nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT, down);
            else nk_input_key(ctx, NK_KEY_LEFT, down);
        } else if (sym == SDLK_RIGHT) {
            if (state[SDL_SCANCODE_LCTRL])
                nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, down);
            else nk_input_key(ctx, NK_KEY_RIGHT, down);
        }
    } else if (evt->type == SDL_MOUSEBUTTONDOWN || evt->type == SDL_MOUSEBUTTONUP) {
        /* mouse button */
        int down = evt->type == SDL_MOUSEBUTTONDOWN;
        const int x = evt->button.x, y = evt->button.y;
        if (evt->button.button == SDL_BUTTON_LEFT) {
            if (evt->button.clicks > 1)
                nk_input_button(ctx, NK_BUTTON_DOUBLE, x, y, down);
            nk_input_button(ctx, NK_BUTTON_LEFT, x, y, down);
        } else if (evt->button.button == SDL_BUTTON_MIDDLE)
            nk_input_button(ctx, NK_BUTTON_MIDDLE, x, y, down);
        else if (evt->button.button == SDL_BUTTON_RIGHT)
            nk_input_button(ctx, NK_BUTTON_RIGHT, x, y, down);
    } else if (evt->type == SDL_MOUSEMOTION) {
        /* mouse motion */
        if (ctx->input.mouse.grabbed) {
            int x = (int)ctx->input.mouse.prev.x, y = (int)ctx->input.mouse.prev.y;
            nk_input_motion(ctx, x + evt->motion.xrel, y + evt->motion.yrel);
        } else nk_input_motion(ctx, evt->motion.x, evt->motion.y);
    } else if (evt->type == SDL_TEXTINPUT) {
        /* text input */
        nk_glyph glyph;
        memcpy(glyph, evt->text.text, NK_UTF_SIZE);
        nk_input_glyph(ctx, glyph);
    } else if (evt->type == SDL_MOUSEWHEEL) {
        /* mouse wheel */
        nk_input_scroll(ctx,nk_vec2((float)evt->wheel.x,(float)evt->wheel.y));
    }
}

NK_API
void nk_sdl_native_shutdown(void)
{
    nk_free(&sdl_native.ctx);
    nk_buffer_free(&sdl_native.cmds);
    memset(&sdl_native, 0, sizeof(sdl_native));
}

#endif