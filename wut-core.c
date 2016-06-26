#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <inttypes.h>
#include <wayland-client.h>

#include "wut-core.h"
#include "wut-display.h"
#include "wut-client.h"

struct wl_display *display;
struct wl_compositor *compositor;
struct wl_shell *shell;
struct wl_shm *shm;

void err(char *src, char *msg) {
	fprintf(stderr, "%s: %s\n", src, msg);
}

void err_exit(char *src, char *msg) {
	err(src, msg);
	exit(1);
}

void shell_surface_ping(void *data, struct wl_shell_surface *shell_surface, uint32_t serial) {
	puts("pong!");
	wl_shell_surface_pong(shell_surface, serial);
}

void shell_surface_configure(void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height) { }

const struct wl_shell_surface_listener shell_surface_listener = {
	.ping = shell_surface_ping,
	.configure = shell_surface_configure,
};

struct window *create_window(int width, int height, char *shm_filename) {
	struct window *window = malloc(sizeof(struct window));
	if (!window) return NULL;

	window->w = width;
	window->h = height;
	window->surface = wl_compositor_create_surface(compositor);
	if (window->surface) puts("Got surface");
	window->shell_surface = wl_shell_get_shell_surface(shell, window->surface);
	if (window->shell_surface) puts("Got shell_surface");
	else puts("Error getting shell_surface");
	wl_shell_surface_add_listener(window->shell_surface, &shell_surface_listener, 0);
	wl_shell_surface_set_toplevel(window->shell_surface);

	window->shm_filename = shm_filename;
	int fd = open(shm_filename, O_RDWR);
	if (fd < 0) {
		err("create_window", "Failed to open window buffer shm file");
		return NULL;
	}

	window->shm_data = mmap(NULL, 65536, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (window->shm_data == MAP_FAILED) {
		err("create_window", "Failed to mmap shm file");
		close(fd);
		return NULL;
	}
	memset(window->shm_data, 0x88, (width * height) * 2);
	memset(window->shm_data + ((width * height)/2), 0xFF, (width * height) * 2);

	struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, 65536);
	if (pool) puts("Got pool");
	window->buffer = wl_shm_pool_create_buffer(pool, 0, width, height, width * 4, WL_SHM_FORMAT_XRGB8888);
	if (window->buffer) puts("Got buffer");
	wl_shm_pool_destroy(pool);
	close(fd);

	wl_surface_attach(window->surface, window->buffer, 0, 0);
	wl_surface_damage(window->surface, 0, 0, width, height);
	wl_surface_commit(window->surface);


	return window;
}

void destroy_window(struct window *window) {
	wl_surface_destroy(window->surface);
	shm_unlink(window->shm_filename);
	free(window->shm_filename);
	//wl_buffer_destroy(window->buffer); //causeing segfault
	free(window);
}

void global_registry_handler(void *data, struct wl_registry *reg, uint32_t id, const char *interface, uint32_t version) {
	printf("Got a registry event for %s id %d\n", interface, id);
	if (strcmp(interface, "wl_compositor") == 0) compositor = wl_registry_bind(reg, id, &wl_compositor_interface, 1);
	else if (strcmp(interface, "wl_shell") == 0) shell = wl_registry_bind(reg, id, &wl_shell_interface, 1);
	else if (strcmp(interface, "wl_shm") == 0) shm = wl_registry_bind(reg, id, &wl_shm_interface, 1);
}

void global_registry_remove_handler(void *data, struct wl_registry *reg, uint32_t id) {
	printf("Got a registry losing event for %d\n", id);
}

const struct wl_registry_listener global_registry_listener = {
	.global = global_registry_handler,
	.global_remove = global_registry_remove_handler,
};

void init_wayland(void) {
	display = wl_display_connect(NULL);
	// if (!display) errx(1, "Could not connect to a wayland display");
	if (!display) err_exit("init_wayland", "Could not connect to a wayland display");

	struct wl_registry *registry = wl_display_get_registry(display);
	wl_registry_add_listener(registry, &global_registry_listener, NULL);
	wl_display_roundtrip(display);
	// assert have all necessary wl_ objects from registry
	wl_registry_destroy(registry);
}

void close_wayland(void) {
	wl_compositor_destroy(compositor);
	// wl_display_flush(display);
	wl_display_disconnect(display);
}

int main() {
	client_run();
}
