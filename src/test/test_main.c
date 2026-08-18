#include "test_vmath.h"
#include <stdio.h>
int main(void) {

  printf("Runnning tests...\n");
  test_create();
  test_copy();

  printf("All tests concluded.\n");

  return 0;
}
