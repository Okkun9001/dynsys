#include "sir.h"
#include "vmath.h"
#include <stddef.h>

// Y = [S, I, R]
void sir_model(const Vec *Y, double t, void *params, Vec *dYdt) {
  SirParams *p = params;

  double S = Y->data[0];
  double I = Y->data[1];
  double N = vec_sum(Y);

  dYdt->data[0] = -p->beta * S * I / N;
  dYdt->data[1] = p->beta * S * I / N - p->gamma * I;
  dYdt->data[2] = p->gamma * I;
}
