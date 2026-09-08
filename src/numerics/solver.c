#include "solver.h"
#include "matrix.h"
#include "vmath.h"
#include <stdlib.h>

Matrix euler_solve(const Vec *y0, size_t steps, double dt,
                   void (*derivative)(const Vec *, double, void *, Vec *),
                   void *params) {
  Matrix sol = m_create(steps, y0->size);

  Vec y = vec_copy(y0);
  Vec dydt = vec_null(y0->size);

  for (size_t i = 0; i < steps; i++) {

    // Store current state
    for (size_t j = 0; j < y.size; j++) {
      sol.data[i * y.size + j] = y.data[j];
    }

    // Calculate derivative
    derivative(&y, i * dt, params, &dydt);

    // Euler step
    for (size_t j = 0; j < y.size; j++) {
      y.data[j] += dt * dydt.data[j];
    }
  }

  vec_destroy(&y);
  vec_destroy(&dydt);

  return sol;
}
