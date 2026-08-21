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

  Matrix m = m_create(3, 3);
  MAT(m, 1, 0) = -3;
  MAT(m, 1, 1) = -1;
  MAT(m, 1, 2) = 2;
  MAT(m, 0, 0) = 2;
  MAT(m, 0, 1) = 1;
  MAT(m, 0, 2) = -1;
  MAT(m, 2, 0) = -2;
  MAT(m, 2, 1) = 1;
  MAT(m, 2, 2) = 2;
  Vec b = vec_create(3, (double[]){8, -11, -3});
  Vec x = vec_null(3);

  m_solve(&m, &b, &x);

  vec_destroy(&b);
  vec_destroy(&x);
  m_destroy(&m);
  return 0;
}
