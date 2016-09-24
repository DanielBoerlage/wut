#ifndef WUT_MAIN_H
#define WUT_MAIN_H

#include <wayland-client.h>

typedef uint32_t color;
typedef color pixel;
const int pixel_format;

typedef int64_t dim;

struct rect {
	dim w, h;
};

// struct rect_area {
// 	dim w, h, area;
// }

struct location {
	dim x, y;
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
	size_t shm_data_len;
};

struct window *create_window(struct rect size);
void destroy_window(struct window *window);

//tmp
void display_dispatch(void);

void err(char *msg);
void err_exit(char *msg);
void *err_null(char *msg);


#endif
