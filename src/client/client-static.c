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
const color BLACK = 0xff000000;
const color WHITE = 0xffffffff;

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

void terminate(int sig) {
	destroy_window(win);
	exit(0);
}

int client_run(int argc, char **argv) {

	win = create_window((struct rect){100, 100});

	render_draw_rect(win, (struct rect){100, 100}, (struct location){0, 0}, WHITE);
	render_draw_rect(win, (struct rect){10, 10}, (struct location){20, 20}, BLACK);
	render_draw_rect(win, (struct rect){10, 10}, (struct location){70, 20}, BLACK);
	render_draw_rect(win, (struct rect){60, 10}, (struct location){20, 70}, BLACK);
	render_display(win);

	signal(SIGINT, terminate);

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
	return -1;
}

void client_cleanup(void) {
	if (win) destroy_window(win);
}

