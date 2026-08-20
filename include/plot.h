#ifndef PLOT_H
#define PLOT_H

#include "draw.h"
#include "vmath.h"
#include <stddef.h>

#define WIDTH 960
#define HEIGHT 1080

#define BG_COLOR 0xFF190d13
#define FG_COLOR 0xFF73c814
#define FG2_COLOR 0xFF73c8e6
#define FG3_COLOR 0xFFc41d6e

void plot(Vec *data) {

  draw_init(WIDTH, HEIGHT);
  clear_framebuffer(BG_COLOR);
  for (size_t i = 0; i < data->size; i++) {

    Circle c = {{i * 100, data->data[i] * 10}, 5};
    fill_circle(c, FG3_COLOR);
  }
}

#endif // !PLOT_H
