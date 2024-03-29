#define _POSIX_C_SOURCE 210112L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <inttypes.h>
#include <wayland-client.h>

#include "main.h"
#include "../render/render.h"
#include "../client/client.h"

struct wl_display *display;
struct wl_compositor *compositor;
struct wl_shell *shell;
struct wl_shm *shm;

void err(char *msg) {
	fprintf(stderr, "%s\n", msg);
}

void err_exit(char *msg) {
	err(msg);
	exit(1);
}

void *err_null(char *msg) {
	err(msg);
	return NULL;
}

void shell_surface_ping(void *data, struct wl_shell_surface *shell_surface, uint32_t serial) {
	wl_shell_surface_pong(shell_surface, serial);
}

void shell_surface_configure(void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height) { }

const struct wl_shell_surface_listener shell_surface_listener = {
	.ping = shell_surface_ping,
	.configure = shell_surface_configure,
};

// most of theses errors dont free win
struct window *create_window(struct rect size) {
	if (size.w * size.h == 0) return err_null("[create_window] Can't create a window with no area");

	struct window *win = malloc(sizeof(struct window));
	if (!win) return err_null("[create_window] Failed to malloc window");

	win->size = size;

	win->surface = wl_compositor_create_surface(compositor);
	if (!win->surface) return err_null("[create_window] Failed to create window surface");

	win->surface_interface = wl_shell_get_shell_surface(shell, win->surface);
	if (!win->surface_interface) return err_null("[create_window] Failed to create window shell surface");

	wl_shell_surface_add_listener(win->surface_interface, &shell_surface_listener, NULL);
	wl_shell_surface_set_toplevel(win->surface_interface);

	size_t buffer_size = size.w * size.h * sizeof(pixel);
	win->shm_data_len = buffer_size * 2;

	win->shm_filename = malloc(24);
	strcpy(win->shm_filename, "/dev/shm/wut_shm_XXXXXX");
	int fd = mkstemp(win->shm_filename);
	ftruncate(fd, win->shm_data_len);

	win->shm_data = mmap(NULL, win->shm_data_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (win->shm_data == MAP_FAILED) return err_null("[create_window] Failed to mmap shm file");

	win->display.buffer_pixels = (pixel *)win->shm_data;
	win->render.buffer_pixels  = (pixel *)win->shm_data + (size.w * size.h);

	struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, win->shm_data_len);
	if (!pool) return err_null("[create_window] Failed to create shm pool");

	win->display.buffer = wl_shm_pool_create_buffer(pool, 0,           size.w, size.h, size.w * sizeof(pixel), pixel_format);
	if (!win->display.buffer) return err_null("[create_window] Failed to create display buffer");
	win->render.buffer  = wl_shm_pool_create_buffer(pool, buffer_size, size.w, size.h, size.w * sizeof(pixel), pixel_format);
	if (!win->render.buffer) return err_null("[create_window] Failed to create render buffer");

	wl_shm_pool_destroy(pool);
	close(fd);

	render_init_window(win);

	return win;
}

// struct window *create_window(struct rect size) {
// 	char filename[] = "/dev/shm/wut_shm_tmp_XXXXXX";
// 	int fd = mkstemp(filename);
// 	ftruncate(fd, size.w * size.h * sizeof(pixel) * 2);
// 	struct window *win = create_window_fd(size, fd, 0);
// 	close(fd);
// 	return win;
// }

void destroy_window(struct window *win) {
	wl_shell_surface_destroy((struct wl_shell_surface *)win->surface_interface);
	wl_surface_destroy(win->surface);
	wl_buffer_destroy(win->display.buffer);
	wl_buffer_destroy(win->render.buffer);
	munmap(win->shm_data, win->shm_data_len);
	unlink(win->shm_filename);
	free(win->shm_filename);
	free(win);
}

//tmp
void display_dispatch(void) {
	int r = 1;
	while(r) r = wl_display_dispatch(display);
}

void global_registry_handler(void *data, struct wl_registry *reg, uint32_t id, const char *interface, uint32_t version) {
	if (strcmp(interface, "wl_compositor") == 0) compositor = wl_registry_bind(reg, id, &wl_compositor_interface, 1);
	else if (strcmp(interface, "wl_shell") == 0) shell = wl_registry_bind(reg, id, &wl_shell_interface, 1);
	else if (strcmp(interface, "wl_shm") == 0) shm = wl_registry_bind(reg, id, &wl_shm_interface, 1);
}

void global_registry_remove_handler(void *data, struct wl_registry *reg, uint32_t id) { }

const struct wl_registry_listener global_registry_listener = {
	.global = global_registry_handler,
	.global_remove = global_registry_remove_handler,
};

void init_wayland(void) {
	display = wl_display_connect(NULL);
	if (!display) err_exit("Failed to connect to a wayland display");

	struct wl_registry *registry = wl_display_get_registry(display);
	wl_registry_add_listener(registry, &global_registry_listener, NULL);
	wl_display_roundtrip(display);

	if (!compositor) err_exit("Failed to find a compositor");
	if (!shell) err_exit("Failed to get wl_shell protocol");
	if (!shm) err_exit("Failed to get wl_shm protocol");

	wl_registry_destroy(registry);
}

void close_wayland(void) {
	wl_compositor_destroy(compositor);
	wl_display_disconnect(display);
}

int main(int argc, char **argv) {
	init_wayland();
	render_init();
	int exit_code = client_run(argc, argv);
	render_close();
	close_wayland();
	return exit_code;
}
