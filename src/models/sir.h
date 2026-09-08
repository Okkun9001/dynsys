#ifndef SIR_H
#define SIR_H

#include "matrix.h"
#include "vmath.h"
#include <stddef.h>

typedef struct {
  double beta;
  double gamma;
} SirParams;

void sir_model(const Vec *Y, double t, void *params, Vec *dYdt);
void sir_run_simulation(const Vec *Y, size_t t, double beta, double gamma,
                        size_t days, Matrix *dYdt);

#endif // !SIR_H
