#ifndef WUT_MAIN_H
#define WUT_MAIN_H

#include <wayland-client.h>

typedef uint32_t pixel;
const int pixel_format;

typedef uint32_t color;

struct rect {
	uint32_t w, h;
};

struct location {
	uint32_t x, y;
};

struct buffer {
	struct wl_buffer *buffer;
	pixel *buffer_pixels;
};

struct window {
	struct location loc;
	struct rect size;
	struct wl_surface *surface;
	void *surface_interface;
	struct buffer render, display;
	void *shm_data;
	char *shm_filename;
	uint32_t shm_data_len;
};

struct window *create_window(struct rect size);
void destroy_window(struct window *window);

//tmp
void display_dispatch(void);

void err(char *msg);
void err_exit(char *msg);
void *err_null(char *msg);


#endif
