#include "matrix.h"
#include "vmath.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#define EPSILON 1e-9

void test_create(void) {
  Vec v = vec_null(5);

  assert(v.size == 5);

  for (size_t i = 0; i < v.size; i++)
    assert(v.data[i] == 0.0);

  vec_destroy(&v);
}

void test_copy(void) {
  Vec a = vec_null(3);
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

void test_m_vec_mult(void) {
  Matrix m = m_create(2, 2);
  MAT(m, 0, 0) = 1;
  MAT(m, 0, 1) = 2;
  MAT(m, 1, 0) = 3;
  MAT(m, 1, 1) = 4;

  Vec v = vec_create(2, (double[]){1, 2});

  Vec res = m_vec_mul(&m, &v);

  assert(res.data[0] == 5 && res.data[1] == 11);
  m_destroy(&m);
  vec_destroy(&v);
  vec_destroy(&res);
}

void test_m_mult(void) {
  Matrix m = m_create(2, 3);
  m_rand(&m, 1);

  Matrix m2 = m_create(3, 4);
  m_rand(&m2, 1);
  Matrix m3 = m_create(4, 1);
  m_rand(&m3, 1);

  // A(BC)
  Matrix left = m_mult(&m2, &m3);
  left = m_mult(&m, &left);
  // (AB)C
  Matrix right = m_mult(&m, &m2);
  right = m_mult(&right, &m3);

  m_destroy(&m);
  m_destroy(&m2);
  m_destroy(&m3);

  assert(MAT(left, 0, 0) - MAT(right, 0, 0) < EPSILON);
  assert(MAT(left, 1, 0) - MAT(right, 1, 0) < EPSILON);

  m_destroy(&left);
  m_destroy(&right);
}

void test_transpose(void) {
  Matrix m = m_create(2, 3);
  MAT(m, 0, 0) = 1;
  MAT(m, 0, 1) = 2;
  MAT(m, 0, 2) = 3;
  MAT(m, 1, 0) = 4;
  MAT(m, 1, 1) = 5;
  MAT(m, 1, 2) = 6;

  Matrix t = m_transpose(&m);

  m_destroy(&m);
  m_destroy(&t);
}

void test_add_col(void) {
  Matrix m = m_create(3, 3);
  MAT(m, 0, 0) = -3;
  MAT(m, 0, 1) = -1;
  MAT(m, 0, 2) = 2;
  MAT(m, 1, 0) = 2;
  MAT(m, 1, 1) = 1;
  MAT(m, 1, 2) = -1;
  MAT(m, 2, 0) = -2;
  MAT(m, 2, 1) = 1;
  MAT(m, 2, 2) = 2;

  Vec v = vec_create(3, (double[]){-11, 8, -3});

  Matrix m2 = m_add_col(&m, &v);

  for (size_t i = 0; i < v.size; i++) {
    assert(MAT(m2, i, 3) == v.data[i]);
  }

  m_destroy(&m);
  vec_destroy(&v);
  m_destroy(&m2);
}

void test_add_row(void) {
  Matrix m = m_create(3, 3);
  MAT(m, 0, 0) = -3;
  MAT(m, 0, 1) = -1;
  MAT(m, 0, 2) = 2;
  MAT(m, 1, 0) = 2;
  MAT(m, 1, 1) = 1;
  MAT(m, 1, 2) = -1;
  MAT(m, 2, 0) = -2;
  MAT(m, 2, 1) = 1;
  MAT(m, 2, 2) = 2;

  Vec v = vec_create(3, (double[]){-11, 8, -3});

  Matrix m2 = m_add_row(&m, &v);

  for (size_t i = 0; i < v.size; i++) {
    assert(MAT(m2, 3, i) == v.data[i]);
  }

  m_destroy(&m);
  vec_destroy(&v);
  m_destroy(&m2);
}
