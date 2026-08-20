#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "matrix.h"
#include "solver.h"
#include "vmath.h"
#define PLOT_H_IMPLEMENTATION
#include "plot.h"

#define EPS 0.001

double square(double x) { return x * x; }
double square_slope(double x) { return 2 * x; }

int main(void) {

  // export_buffer_as_ppm("output.ppm");
  double a = 2;

  double Fa = square(a);
  double Ea = euler_solve(a, square_slope, 10000);

  return 0;
}
