#include <wayland-client.h>

#include "wut-render.h"

const int pixel_format = WL_SHM_FORMAT_XRGB8888;

// prefixes:
//     m: matrix
//     w: window
//     g: gylph
// suffixes (all in pixels):
//     w: width
//     h: height
//     s: size
// const int m_r = 10,
//           m_c = 10,
//           g_w = 10,
//           g_h = 10,
//           g_s = g_w * g_h,
//           w_w = m_c * g_w,
//           w_h = m_r * g_h,
//           w_s = w_w * w_h;

// starting locations for graphical objects
// pixel *shm,
//       *buffer_start[2],
//       *g_start,
//       *null_glyph,
//       *test_glyph;



// pixel *glyph_position(int glyph) {
// 	return g_start + (glyph * g_s);
// }

// pixel *matrix_position(int buffer, int row, int col) {
// 	return buffer_start[buffer] + (row * g_h * w_w + col * g_w);
// }

struct rect render_text_size(char *text, char *font) {
  struct rect ret = {
    0, 0
  };
  return ret;
}

void render_set_font(char *font) {

}

void render_draw_text(struct window *win, char *text, struct location loc, struct rect area, color fg, color bg) {}
void render_draw_rect(struct window *win, struct rect area, struct location loc, color c) {}






