#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wayland-client.h>
#include <string.h>

#include "wut-client.h"
#include "wut-core.h"

void client_run(void) {
	init_wayland();
	int fd = open("/dev/shm/wut-shm", O_RDWR);
	if (fd < 0) {
		puts("Failed to open shm file");
		exit(1);
	}
	struct window *win = create_window(100, 100, fd);
	close(fd);

	wl_surface_attach(win->surface, win->buffer[0], 0, 0);
	wl_surface_damage(win->surface, 0, 0, win->w, win->h);
	wl_surface_commit(win->surface);

	display_dispatch();

	destroy_window(win);
	close_wayland();
}
