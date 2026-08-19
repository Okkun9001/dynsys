#include "vmath.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "draw.h"

#define EPS 0.001

// PLOT
#define BG_COLOR 0xFF190d13
#define FG_COLOR 0xFF73c814
#define FG2_COLOR 0xFF73c8e6
#define FG3_COLOR 0xFFc41d6e

double square(double x) { return x * x; }
double square_slope(double x) { return 2 * x; }

// Approximate x^2 using Euler
void step(double a, int (*f)(double)) {

  // double Fa_p = f(a + EPS);
  // double Fa_m = f(a - EPS);
  // double derivative = (Fa_p - Fa_m) / (2 * EPS);
}

double euler_solve(const double a, double (*dXdt)(double), size_t steps) {

  double h = a / steps;
  double y = dXdt(0);

  for (size_t i = 0 + h; i < steps; i++) {

    double t = i * h;
    y += h * dXdt(t);
  }
  return y;
}

int main(void) {

  draw_init(500, 500);
  Circle c = {{250, 250}, 25};
  clear_framebuffer(BG_COLOR);
  fill_circle(c, FG3_COLOR);

  export_buffer_as_ppm("output.ppm");
  double a = 2;

  double Fa = square(a);
  double Ea = euler_solve(a, square_slope, 10000);

  printf("Analytic solution: %.5f\n", Fa);
  printf("Numeric solution: %.5f\n", Ea);

  return 0;
}
