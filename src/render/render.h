#ifndef WUT_RENDER_H
#define WUT_RENDER_H

#include "../main/main.h"

void render_init_window(struct window *win);

struct rect render_text_size(char *text, char *font);

void render_set_font(char *font);

void render_draw_text(struct window *win, char *text, struct location loc, struct rect area, color fg, color bg);
void render_draw_rect(struct window *win, struct rect area, struct location loc, color c);

//tmp
void render_display(struct window *win);

#endif
