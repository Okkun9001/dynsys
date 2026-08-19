#include "draw.h"
#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t width;
static uint32_t height;
static uint32_t *framebuffer;

uint32_t *draw_init(uint32_t w, uint32_t h) {
  width = w;
  height = h;

  framebuffer = malloc((size_t)width * height * sizeof(*framebuffer));
  return framebuffer;
}

void put_pixel(V2 p, uint32_t color) {
  if (p.x < 0 || p.y < 0 || (uint32_t)p.x >= width || (uint32_t)p.y >= height)
    return;

  framebuffer[p.y * width + p.x] = color;
}

void put_pixel_alpha(V2 p, uint32_t color) {
  if (p.x < 0 || p.y < 0 || (uint32_t)p.x >= width || (uint32_t)p.y >= height)
    return;

  uint32_t dst = framebuffer[p.y * width + p.x];

  uint8_t src_a = (color >> 24) & 0xFF;

  if (src_a == 255) {
    framebuffer[p.y * width + p.x] = color;
    return;
  }

  if (src_a == 0)
    return;

  uint8_t sr = (color >> 0) & 0xFF;
  uint8_t sg = (color >> 8) & 0xFF;
  uint8_t sb = (color >> 16) & 0xFF;

  uint8_t dr = (dst >> 0) & 0xFF;
  uint8_t dg = (dst >> 8) & 0xFF;
  uint8_t db = (dst >> 16) & 0xFF;

  uint8_t r = (sr * src_a + dr * (255 - src_a)) / 255;
  uint8_t g = (sg * src_a + dg * (255 - src_a)) / 255;
  uint8_t b = (sb * src_a + db * (255 - src_a)) / 255;

  framebuffer[p.y * width + p.x] = (255 << 24) | (b << 16) | (g << 8) | r;
}

void clear_framebuffer(uint32_t color) {
  for (size_t i = 0; i < width * height; i++)
    framebuffer[i] = color;
}

void draw_destroy(void) {
  free(framebuffer);
  framebuffer = NULL;
}

void swap(V2 *p0, V2 *p1) {
  V2 tmp = *p0;
  *p0 = *p1;
  *p1 = tmp;
}

void draw_line(Line l, uint32_t color) {

  V2 p0 = l.p0;
  V2 p1 = l.p1;

  int dx = abs(p1.x - p0.x);
  int sx = p0.x < p1.x ? 1 : -1;

  int dy = -abs(p1.y - p0.y);
  int sy = p0.y < p1.y ? 1 : -1;

  int err = dx + dy;

  while (1) {

    put_pixel(p0, color);

    if (p0.x == p1.x && p0.y == p1.y)
      break;

    int e2 = 2 * err;

    if (e2 >= dy) {
      err += dy;
      p0.x += sx;
    }

    if (e2 <= dx) {
      err += dx;
      p0.y += sy;
    }
  }
}

void draw_triangle(Triangle t, uint32_t color) {

  V2 p0 = t.p0;
  V2 p1 = t.p1;
  V2 p2 = t.p2;

  draw_line((Line){p0, p1}, color);
  draw_line((Line){p1, p2}, color);
  draw_line((Line){p2, p0}, color);
}

void draw_rectangle(Rectangle s, uint32_t color) {
  draw_line((Line){{s.p0.x, s.p0.y}, {s.p0.x, s.p1.y}}, color);
  draw_line((Line){{s.p0.x, s.p0.y}, {s.p1.x, s.p0.y}}, color);
  draw_line((Line){{s.p1.x, s.p0.y}, {s.p1.x, s.p1.y}}, color);
  draw_line((Line){{s.p0.x, s.p1.y}, {s.p1.x, s.p1.y}}, color);
}

void draw_quad(Quadrilateral q, uint32_t color) {
  draw_line((Line){q.p0, q.p1}, color);
  draw_line((Line){q.p1, q.p2}, color);
  draw_line((Line){q.p2, q.p3}, color);
  draw_line((Line){q.p3, q.p0}, color);
}

void draw_circle(Circle c, uint32_t color) {
  int x = c.r;
  int y = 0;
  int decision = 1 - c.r;

  while (x >= y) {
    put_pixel((V2){c.o.x + x, c.o.y + y}, color);
    put_pixel((V2){c.o.x + y, c.o.y + x}, color);
    put_pixel((V2){c.o.x - y, c.o.y + x}, color);
    put_pixel((V2){c.o.x - x, c.o.y + y}, color);

    put_pixel((V2){c.o.x - x, c.o.y - y}, color);
    put_pixel((V2){c.o.x - y, c.o.y - x}, color);
    put_pixel((V2){c.o.x + y, c.o.y - x}, color);
    put_pixel((V2){c.o.x + x, c.o.y - y}, color);

    y++;

    if (decision <= 0) {
      decision += 2 * y + 1;
    } else {
      x--;
      decision += 2 * (y - x) + 1;
    }
  }
}

// void load_image(const char *file_path, Image *img) {
//
//   int w, h, channels;
//
//   uint8_t *pixels = stbi_load(file_path, &w, &h, &channels, 4);
//
//   if (!pixels) {
//     fprintf(stderr, "Failed to load image: %s\n", stbi_failure_reason());
//     return;
//   }
//   *img = (Image){w, h, channels, pixels};
// }

// void draw_img(V2 pos, const char *file_path) {
//
//   draw_img_scale(pos, file_path, 1.0f);
// }
//
// void draw_img_scale(V2 pos, const char *file_path, float scale) {
//
//   Image img;
//   load_image(file_path, &img);
//
//   int new_w = (int)(img.w * scale);
//   int new_h = (int)(img.h * scale);
//
//   for (int y = 0; y < new_h; y++) {
//     for (int x = 0; x < new_w; x++) {
//
//       int src_x = (int)(x / scale);
//       int src_y = (int)(y / scale);
//
//       const uint8_t *p = &img.pixels[(src_y * img.w + src_x) * 4];
//
//       uint32_t color = ((uint32_t)p[3] << 24) | ((uint32_t)p[2] << 16) |
//                        ((uint32_t)p[1] << 8) | ((uint32_t)p[0]);
//
//       put_pixel_alpha((V2){pos.x + x, pos.y - y}, color);
//     }
//   }
//
//   stbi_image_free(img.pixels);
// }

void fill_triangle(Triangle t, uint32_t color) {
  // Sort vertices
  if (t.p0.y > t.p1.y) {

    V2 tmp = t.p0;
    t.p0 = t.p1;
    t.p1 = tmp;
  }
  if (t.p1.y > t.p2.y) {
    V2 tmp = t.p1;
    t.p1 = t.p2;
    t.p2 = tmp;
  }

  if (t.p0.y > t.p1.y) {
    V2 tmp = t.p0;
    t.p0 = t.p1;
    t.p1 = tmp;
  }

  for (int y = t.p0.y; y < t.p2.y; y++) {
    float t_long = (float)(y - t.p0.y) / (float)(t.p2.y - t.p0.y);
    int x_long = lerp_int(t.p0.x, t.p2.x, t_long);
    int x_short;

    if (y < t.p1.y) {
      float t_short = (float)(y - t.p0.y) / (float)(t.p1.y - t.p0.y);
      x_short = lerp_int(t.p0.x, t.p1.x, t_short);
    } else {

      float t_short = (float)(y - t.p1.y) / (float)(t.p2.y - t.p1.y);
      x_short = lerp_int(t.p1.x, t.p2.x, t_short);
    }

    if (x_short > x_long) {
      int tmp = x_short;
      x_short = x_long;
      x_long = tmp;
    }

    for (int x = x_short; x < x_long; x++) {
      put_pixel((V2){x, y}, color);
    }
  }
}

void fill_rectangle(Rectangle r, uint32_t color) {
  size_t ymin = r.p0.y;
  size_t ymax = r.p1.y;
  size_t xmin = r.p0.x;
  size_t xmax = r.p1.x;

  if (r.p0.y > r.p1.y) {
    ymax = r.p0.y;
    ymin = r.p1.y;
  }
  if (r.p0.x > r.p1.x) {
    xmax = r.p0.x;
    xmin = r.p1.x;
  }

  for (size_t y = ymin; y < ymax; y++) {
    uint32_t *row = &framebuffer[y * width + xmin];

    for (size_t x = xmin; x < xmax; x++)
      row[x - xmin] = color;
  }
}

void fill_quad(Quadrilateral q, uint32_t color) {

  fill_triangle((Triangle){q.p0, q.p1, q.p2}, color);
  fill_triangle((Triangle){q.p0, q.p2, q.p3}, color);
}

void fill_circle(Circle c, uint32_t color) {
  int x = c.r;
  int y = 0;
  int decision = 1 - c.r;

  while (x >= y) {
    // Fill horizontal spans for all 8 symmetric sections
    for (int dx = -x; dx <= x; dx++) {
      put_pixel((V2){c.o.x + dx, c.o.y + y}, color);
      put_pixel((V2){c.o.x + dx, c.o.y - y}, color);
    }

    for (int dx = -y; dx <= y; dx++) {
      put_pixel((V2){c.o.x + dx, c.o.y + x}, color);
      put_pixel((V2){c.o.x + dx, c.o.y - x}, color);
    }

    y++;

    if (decision <= 0) {
      decision += 2 * y + 1;
    } else {
      x--;
      decision += 2 * (y - x) + 1;
    }
  }
}

void export_buffer_as_ppm(const char *file_path) {

  printf("file_path = '%s'\n", file_path);
  FILE *f = fopen(file_path, "wb");
  if (f == NULL) {
    fprintf(stderr, "ERROR: Could not write into file %s: %s\n", file_path,
            strerror(errno));
    exit(1);
  }
  fprintf(f, "P6\n%d %d 255\n", width, height);

  for (int y = height - 1; y >= 0; y--) {
    for (int x = 0; x < (int)width; x++) {
      // 0xAABBGGRR
      uint32_t color = framebuffer[y * width + x];
      uint8_t bytes[3] = {
          (color & 0x0000FF) >> 8 * 0,
          (color & 0x00FF00) >> 8 * 1,
          (color & 0xFF0000) >> 8 * 2,
      };
      fwrite(bytes, sizeof(bytes), 1, f);
      assert(!ferror(f));
    }
  }

  int ret = fclose(f);
  assert(ret == 0);
}
