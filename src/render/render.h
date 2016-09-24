#ifndef WUT_RENDER_H
#define WUT_RENDER_H

#include "../main/main.h"

#include <stdbool.h>

void render_init(void);
void render_close(void);
void render_init_window(struct window *win);
void render_display(struct window *win);

struct rect render_text_area(char *text);
void render_set_font(char *font);
void render_draw_text(struct window *win, char *text, struct location loc, color fg, color bg);
void render_draw_rect(struct window *win, dim x, dim y, dim w, dim h, color c);

#endif
