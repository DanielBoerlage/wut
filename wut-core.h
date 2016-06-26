#ifndef WUT_CORE_H
#define WUT_CORE_H

#include <wayland-client.h>

#include "wut-display.h"

struct window {
	int x, y, w, h;
	struct wl_surface *surface;
	struct wl_shell_surface *shell_surface;
	struct wl_buffer *buffer;
	pixel *shm_data;
	char *shm_filename;
};

void init_wayland(void);
void close_wayland(void);

struct window *create_window(int width, int height, char *shm_filename);
void destroy_window(struct window *window);

#endif
