#ifndef SOLVER_H
#define SOLVER_H

#include <stddef.h>

double euler_solve(const double a, double (*dXdt)(double), size_t steps);

#endif // !SOLVER_H
