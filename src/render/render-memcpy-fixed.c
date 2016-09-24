#include <wayland-client.h>
#include <string.h>

#include "render.h"
#include "render-memcpy-fixed.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdbool.h>

const int pixel_format = WL_SHM_FORMAT_ARGB8888;
static pixel *glyphs;

void render_init(void) {
	int glyph_file = open(glyph_file_name, O_RDONLY);
	glyphs = mmap(NULL, glyph_file_size, PROT_READ, MAP_PRIVATE, glyph_file, 0);  //rember to munmap
}

void render_close(void) {
	munmap(glyphs, glyph_file_size);
}

static void attach_buffer(struct window *win) {
	wl_surface_attach(win->surface, win->render.buffer, 0, 0);
}

void render_init_window(struct window *win) {
	attach_buffer(win);
}

static void page_flip(struct window *win) {
	struct buffer tmp_buffer = win->display;
	win->display = win->render;
	win->render = tmp_buffer;
}

// void page_copy(struct window *win) {

// }

void render_display(struct window *win) {
	wl_surface_commit(win->surface);
	page_flip(win);
	attach_buffer(win);
}

void render_draw_rect(struct window *win, dim x, dim y, dim w, dim h, color c) {
	dim end_col = x + w, end_row = y + h;
	for (dim row = y; row < end_row; ++row) {
		for (dim col = x; col < end_col; ++col) {
			*(win->render.buffer_pixels + (row * win->size.w + col)) = c;
		}
	}
}

struct rect render_text_area(char *text) {
	if (!text) return (struct rect) { 0, 0 };
	struct rect area = { 0, glyph.h };
	uint32_t current_width;
	for (; *text; ++text) {
		if (current_width > area.w) area.w = current_width;
		if (*text == '\n') {
			current_width = 0;
			area.h += glyph.h;
		} else if (*text >= ' ') {
			current_width += glyph.w;
		}
	}
	return area;
}

void render_set_font(char *font) { }

// static inline void draw_char()

void render_draw_text(struct window *win, char *text, struct location loc, color fg, color bg) {

	struct rect area = { 0, 0 };
	uint32_t current_width = 0;
	struct location cursor = loc;

	for (; *text; ++text) {
		if (*text == '\n') {
			current_width = 0;
			area.h += glyph.h;

			cursor.x = loc.x;
			cursor.y += glyph.h;
		} else if (*text >= ' ') {

			pixel *display_draw_point = win->render.buffer_pixels + (cursor.y * win->size.w + cursor.x);
			pixel *glyph_read_point = glyphs + ((*text - ' ') * glyph.w * glyph.h);

			for (int row = 0; row < glyph.h; ++row) {
				memcpy(display_draw_point + (row * win->size.w), glyph_read_point + (row * glyph.w), glyph.w * sizeof(pixel));
			}

			current_width += glyph.w;
			cursor.x += glyph.w;
		}
		if (current_width > area.w) area.w = current_width;
	}

	wl_surface_damage(win->surface, loc.x, loc.y, area.w, area.h);
}

// void render_draw_text_damage(struct )


