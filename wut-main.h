#ifndef WUT_MAIN_H
#define WUT_MAIN_H

#include <wayland-client.h>

#include "wut-render.h"

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

struct window *create_window(int width, int height);
struct window *create_window_fd(int width, int height, int shm_fd);
void destroy_window(struct window *window);

//tmp
void display_dispatch(void);

void err(char *src, char *msg);
void err_exit(char *src, char *msg);
void *err_null(char *src, char *msg);


#endif
