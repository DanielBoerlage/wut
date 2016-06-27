#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wayland-client.h>
#include <string.h>

#include "wut-client.h"
#include "wut-main.h"

char *read_stdin(void);

void client_run(void) {
	printf("\nfinal output:\n%s\n", read_stdin());
	// init_wayland();
	// int fd = open("/dev/shm/wut-shm", O_RDWR);
	// if (fd < 0) {
	// 	puts("Failed to open shm file");
	// 	exit(1);
	// }
	// struct window *win = create_window(100, 100, fd);
	// close(fd);

	// wl_surface_attach(win->surface, win->buffer[0], 0, 0);
	// wl_surface_damage(win->surface, 0, 0, win->w, win->h);
	// wl_surface_commit(win->surface);

	// display_dispatch();

	// destroy_window(win);
	// close_wayland();
}

char *read_stdin(void) {
	size_t buff_size = 1;  // initial size
	size_t chars_read = 0;
	char *out = malloc(buff_size);
	char c;
	while ((c = fgetc(stdin)) != EOF) {
		printf("Got char: %c, chars_read: %d, buff_size: %d\n", c, chars_read, buff_size);
		if (chars_read + 2 > buff_size) {
			buff_size <<= 1;
			printf("Increasing buff_size; now %d\n", buff_size);
			out = realloc(out, buff_size);
		}
		out[chars_read++] = c;
	}
	out[chars_read] = '\0';
	out = realloc(out, chars_read + 1);
	return out;
}
