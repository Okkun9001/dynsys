#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>

typedef struct {
  int x, y;
} V2;

typedef struct {
  V2 p0, p1;
} Line;

typedef struct {
  V2 p0, p1, p2;
} Triangle;

typedef struct {
  V2 p0, p1;
} Rectangle;

typedef struct {
  V2 p0, p1, p2, p3;
} Quadrilateral;

typedef struct {
  V2 o;
  uint32_t r;
} Circle;

typedef struct {
  int w, h, n;
  uint8_t *pixels;
} Image;

uint32_t *draw_init(uint32_t w, uint32_t h);
void clear_framebuffer(uint32_t color);
void draw_destroy(void);

// Drawing methods
void put_pixel(V2 p, uint32_t color);
void put_pixel_alpha(V2 p, uint32_t color);
void draw_line(Line l, uint32_t color);
void draw_triangle(Triangle t, uint32_t color);
void draw_rectangle(Rectangle r, uint32_t color);
void draw_quad(Quadrilateral q, uint32_t color);
void draw_circle(Circle c, uint32_t color);
void draw_img(V2 pos, const char *file_path); // -- draws img on canvas
void draw_img_scale(V2 pos, const char *file_path, float scale);

// Fill
void fill_triangle(Triangle t, uint32_t color);
void fill_rectangle(Rectangle r, uint32_t color);
void fill_quad(Quadrilateral q, uint32_t color);
void fill_circle(Circle c, uint32_t color);

// Utils
static int lerp_int(int a, int b, float t) { return a + (int)((b - a) * t); }
void export_buffer_as_ppm(const char *file_path);

#endif
