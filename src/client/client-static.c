#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#include "client.h"
#include "../main/main.h"
#include "../render/render.h"

struct window *win;

char *read_file(FILE *file) {
	size_t buff_size = 1024;
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

int client_run(int argc, char **argv) {

	char *text = read_file(stdin);
	struct rect area = render_text_area(text);

	win = create_window(area);
	if (!win) return -1;
	render_draw_text(win, text, (struct location) {0,0}, 0, 0);

	render_display(win);

	display_dispatch();


	// char *text = read_file(stdin);
	// char *font = (argc > 0) ? argv[0] : "monospaced";
	// struct rect size = render_text_size(text, font);
	// size.w += 10;
	// size.h += 10;

	// win = create_window_fd(size, 0, 0);

	// render_draw_rect(win, size, (struct location){0, 0}, BLACK);
	// render_set_font(font);
	// render_draw_text(win, text, (struct location){5, 5}, size, WHITE, BLACK);

	// free(text);

	// display_dispatch();

	// wl_surface_attach(win->surface, win->buffer[0], 0, 0);
	// wl_surface_damage(win->surface, 0, 0, win->w, win->h);
	// wl_surface_commit(win->surface);
	return 0;
}

void client_exit(void) {
	if (win) destroy_window(win);
}

