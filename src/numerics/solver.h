#ifndef SOLVER_H
#define SOLVER_H

#include "matrix.h"
#include "vmath.h"
#include <stddef.h>

Matrix euler_solve(const Vec *y0, size_t steps, double dt,
                   void (*derivative)(const Vec *, double, void *, Vec *),
                   void *params);

#endif // !SOLVER_H
