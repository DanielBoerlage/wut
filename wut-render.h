#ifndef WUT_RENDER_H
#define WUT_RENDER_H

#include "wut-main.h"

struct rect render_text_size(char *text, char *font);

void render_set_font(char *font);

void render_draw_text(struct window *win, char *text, struct location loc, struct rect area, color fg, color bg);
void render_draw_rect(struct window *win, struct rect area, struct location loc, color c);

#endif
