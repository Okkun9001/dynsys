#include "vmath.h"
#include <math.h>
#include <stdio.h>

#define EPS 0.001

double square(double x) { return x * x; }

int main(void) {

  double a = 2;

  double Fa = square(a);

  double Fa_p = square(a + EPS);
  double Fa_m = square(a - EPS);

  printf("plus: %.3f\nminus: %.3f\n", Fa_p, Fa_m);
  double derivative = (Fa_p - Fa_m) / (2 * EPS);
  printf("diff/2 = %.3f\n", derivative);

  return 0;
}
