#include <stdio.h>
#include <stdlib.h>

// nonstandard BSD extension
#include <err.h>

#include <wayland-client.h>


struct display {
	struct wl_display *display;
	struct wl_registry *registry;
};

struct display *create_display(void) {
	struct display *disp = malloc(sizeof(struct display));
	if (!disp) err(1, NULL);

	disp->display = wl_display_connect(NULL);
	if (!disp->display) errx(1, "Could not connect to a wayland display");

	return disp;
}

void destroy_display(struct display *disp) {
	wl_display_disconnect(disp->display);
	free(disp);
}

int main() {

	struct display *disp = create_display();

	printf("Connected to display\n");

	// struct wl_registry *reg = wl_display_get_registry(disp);

	// wl_display_disconnect(disp);

	destroy_display(disp);

	printf("Disconnected from display\n");

	return 0;
}
