#ifndef VMATH_H
#define VMATH_H
#include <stddef.h>

typedef struct {
  size_t size;
  double *data;
} Vec;

Vec vec_create(size_t size);
Vec vec_copy(Vec *src_v);
void vec_destroy(Vec *v);
void vec_print(Vec *v);

// Operations
void vec_scale(Vec *v);
// 2D
double euclidean_dist(Vec *a, Vec *b);

#endif // !VMATH_H
