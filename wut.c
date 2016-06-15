#include <stdio.h>

#include <wayland-client.h>

int main() {

	struct wl_display *disp = wl_display_connect(NULL);
	if (disp == NULL) {
		fprintf(stderr, "Can't connect to display\n");
		return 1;
	}
	printf("Connected to display\n");

	wl_display_disconnect(disp);
	printf("Disconnected from display\n");




	return 0;
}
