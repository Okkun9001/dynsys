#include "vmath.h"
#include <assert.h>

void test_create(void) {
  Vec v = vec_create(5);

  assert(v.size == 5);

  for (size_t i = 0; i < v.size; i++)
    assert(v.data[i] == 0.0);

  vec_destroy(&v);
}

void test_copy(void) {
  Vec a = vec_create(3);
  a.data[0] = 1.0;
  a.data[1] = 2.0;
  a.data[2] = 3.0;

  Vec b = vec_copy(&a);

  assert(b.size == a.size);
  assert(b.data != a.data);

  for (size_t i = 0; i < a.size; i++)
    assert(b.data[i] == a.data[i]);

  vec_destroy(&a);
  vec_destroy(&b);
}
