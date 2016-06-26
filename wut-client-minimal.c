#include <stdio.h>
#include <unistd.h>

#include "wut-client.h"
#include "wut-core.h"

void client_run(void) {
	init_wayland();
	struct window *win = create_window(100, 100, "/dev/shm/wut-shm");
	sleep(10);
	destroy_window(win);
	close_wayland();
}
