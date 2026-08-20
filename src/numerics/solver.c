#include "solver.h"

double euler_solve(const double a, double (*dXdt)(double), size_t steps) {

  double h = a / steps;
  double y = dXdt(0);

  for (size_t i = 0 + h; i < steps; i++) {

    double t = i * h;
    y += h * dXdt(t);
  }
  return y;
}
