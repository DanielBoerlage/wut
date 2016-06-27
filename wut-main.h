#ifndef WUT_CORE_H
#define WUT_CORE_H

#include <wayland-client.h>

#include "wut-display.h"

struct window {
	int x, y, w, h;
	struct wl_surface *surface;
	struct wl_shell_surface *shell_surface;
	struct wl_buffer *buffer[2];
	int shm_fd;
	int shm_data_len;
	pixel *shm_data;
};

void init_wayland(void);
void close_wayland(void);

struct window *create_window(int width, int height, int shm_fd);
void destroy_window(struct window *window);

//tmp
void display_dispatch(void);

#endif
