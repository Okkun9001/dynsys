#include "test_vmath.h"
#include <stdio.h>
int main(void) {

  printf("Runnning tests...\n");
  test_create();
  test_copy();
  test_m_vec_mult();
  test_m_mult();
  test_transpose();
  test_add_col();
  test_add_row();

  printf("All tests concluded.\n");

  return 0;
}
