#include "vmath.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vec vec_null(size_t size) {
  Vec v = {size, calloc(size, sizeof(double))};

  return v;
}

Vec vec_create(size_t size, double *v) {
  Vec new = {size, malloc(sizeof(double) * size)};
  mempcpy(new.data, v, size * sizeof(double));

  return new;
}

void vec_destroy(Vec *v) {
  free(v->data);
  v->data = NULL;
  v->size = 0;
}

void vec_print(Vec *v) {
  printf("[ ");
  for (size_t i = 0; i < v->size; i++) {
    printf("%.2f ", v->data[i]);
  }
  printf("]\n");
}

Vec vec_copy(Vec *src_v) {
  Vec new = vec_create(src_v->size, src_v->data);

  if (new.data == NULL) {
    fprintf(stderr, "NULL Vec copied.");
    return (Vec){0};
  }

  return new;
}
