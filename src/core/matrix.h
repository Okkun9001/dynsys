#ifndef MATRIX_H
#define MATRIX_H

#define MAT(m, r, c) ((m).data[(r) * (m).n_col + (c)])

#include "vmath.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct {
  size_t n_col, n_row;
  double *data;
} Matrix;

Matrix m_null(size_t n_row, size_t n_col);
Matrix m_create(size_t n_row, size_t n_col);
Matrix m_add_row(Matrix *m, Vec *row);
Matrix m_add_col(Matrix *m, Vec *col);
Matrix m_copy(Matrix *m_src);
void m_rand(Matrix *m, bool normalized);
void m_print(Matrix *m);
void m_destroy(Matrix *m);

// Operations
void m_scale(Matrix *m, double scale);
Vec m_vec_mul(Matrix *m, Vec *v);
Matrix m_mult(Matrix *m1, Matrix *m2);
Matrix m_transpose(Matrix *m1);

// Linear algebra
typedef enum {
  SOLVE_SUCCESS,
  SOLVE_SINGULAR, // det(A) = 0
  SOLVE_DIMENSION_MISMATCH,
  SOLVE_INVALID_INPUT
} SolveStatus;
SolveStatus m_solve(const Matrix *A, const Vec *b,
                    Vec *x); // TODO: implement gaussian elimination
#endif                       // !MATRIX_H
