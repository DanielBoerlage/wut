#ifndef WUT_RENDER_H
#define WUT_RENDER_H

#include <inttypes.h>
#include <wayland-client.h>

typedef uint32_t pixel;
const int pixel_format;

typedef uint32_t color;

struct rect {
	int w, h;
};

struct rect render_text_size(char *text, char *font);

struct window;
void render_draw_text(struct window *win, char *text, char *font, int x, int y, color fg, color bg);
void render_draw_rect(struct window *win, struct rect area, int x, int y, color c);

#endif
