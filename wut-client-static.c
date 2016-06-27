#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wayland-client.h>
#include <string.h>

#include "wut-client.h"
#include "wut-main.h"

char *read_file(FILE *f);

struct window *win;
const color BLACK = 0xff000000;
const color WHITE = 0xffffffff;

int client_run(int argc, char **argv) {

	char *text = read_file(stdin);
	char *font = (argc > 0) ? argv[0] : "monospaced";
	struct rect size = render_text_size(text, font);
	size.w += 10;
	size.h += 10;

	win = create_window(size.w, size.h);

	render_draw_rect(win, size, 0, 0, BLACK);
	render_draw_text(win, text, font, 5, 5, WHITE, BLACK);

	free(text);

	display_dispatch();

	// wl_surface_attach(win->surface, win->buffer[0], 0, 0);
	// wl_surface_damage(win->surface, 0, 0, win->w, win->h);
	// wl_surface_commit(win->surface);
	return -1;
}

void client_cleanup(void) {
	if (win) destroy_window(win);
}

char *read_file(FILE *file) {
	size_t buff_size = 1024;  // initial size
	size_t chars_read = 0;
	char *out = malloc(buff_size);
	char c;
	while ((c = fgetc(file)) != EOF) {
		if (chars_read + 2 > buff_size) {
			buff_size <<= 1;
			out = realloc(out, buff_size);
		}
		out[chars_read++] = c;
	}
	out[chars_read] = '\0';
	out = realloc(out, chars_read + 1);
	return out;
}
