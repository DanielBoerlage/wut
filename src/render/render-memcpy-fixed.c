#include <wayland-client.h>
#include <string.h>

#include "render.h"
#include "render-memcpy-fixed.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

const int pixel_format = WL_SHM_FORMAT_ARGB8888;

void attach_buffer(struct window *win) {
	wl_surface_attach(win->surface, win->render.buffer, 0, 0);
}


void render_init_window(struct window *win) {
	attach_buffer(win);
}


void commit_buffer(struct window *win) {
	wl_surface_commit(win->surface);
}

void page_flip(struct window *win) {
	struct buffer tmp_buffer = win->display;
	win->display = win->render;
	win->render = tmp_buffer;
}

void render_draw_rect(struct window *win, struct rect area, struct location loc, color c) {
	// pixel *buf = win->render.buffer_pixels;
	struct location end = {
		.x = loc.x + area.w,
		.y = loc.y + area.h,
	};
	for (int row = loc.y; row < end.y; ++row) {
		for (int col = loc.x; col < end.x; ++col) {
			*(win->render.buffer_pixels + (row * win->size.w + col)) = c;
		}
	}

	wl_surface_damage(win->surface, loc.x, loc.y, area.w, area.h);
}

// tmp
void render_display(struct window *win) {
	commit_buffer(win);
	page_flip(win);
	attach_buffer(win);
}

pixel *glyphs;

struct rect render_text_area(char *text) {
	struct rect area = { 0, 0 };
	uint32_t current_width;
	for (; text; ++text) {
		if (*text == '\n') {
			if (current_width > area.w) area.w = current_width;
			current_width = 0;
			area.h += glyph.h;
		} else if (*text >= ' ') {
			current_width += glyph.w;
		}
	}
	return area;
}

// in render-memcpy font is treated as a filename to a font file
void render_set_font(char *font) {
	int glyph_file = open(font, O_RDONLY);
	glyphs = mmap(NULL, glyph_file_size, PROT_READ, MAP_PRIVATE, glyph_file, 0);  //rember to munmap
}

void render_draw_text(struct window *win, char *text, struct location loc, struct rect hahaimanotuseuyou, color fg, color bg) {

	struct rect area = { 0, 0 };
	uint32_t current_width = 0;
	struct location cursor = loc;

	for (; *text; ++text) {
		if (*text == '\n') {
			if (current_width > area.w) area.w = current_width;
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
	}

	wl_surface_damage(win->surface, loc.x, loc.y, area.w, area.h);
}


