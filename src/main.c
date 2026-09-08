#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "solver.h"
#include "vmath.h"
#define PLOT_H_IMPLEMENTATION
#include "plot.h"
#include "sir.h"

#define EPS 0.001

double square(double x) { return x * x; }
double square_slope(double x) { return 2 * x; }

int main(void) {

  Vec Y = vec_create(3, (double[]){990, 10, 0});
  size_t days = 365;
  SirParams params = {.beta = 0.3, .gamma = 0.1};

  Matrix sol = euler_solve(&Y, days, 1.0, sir_model, &params);

  m_print(&sol);

  vec_destroy(&Y);
  m_destroy(&sol);
  return 0;
}
