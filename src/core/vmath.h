#ifndef VMATH_H
#define VMATH_H
#include <stdbool.h>
#include <stddef.h>

typedef struct {
  size_t size;
  double *data;
} Vec;

Vec vec_null(size_t size);
Vec vec_create(size_t size, double v[]);
Vec vec_copy(const Vec *src_v);
void vec_rand(Vec *v, bool normalize);
void vec_destroy(Vec *v);
void vec_print(Vec *v);

// Operations
void vec_scale(Vec *v);
Vec vec_subtract(Vec *a, Vec *b);
// 2D
double euclidean_dist(Vec *a, Vec *b);

#endif // !VMATH_H
