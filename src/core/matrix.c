#include "matrix.h"
#include "vmath.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Matrix m_null(size_t n_row, size_t n_col) {

  Matrix m = {n_col, n_row, calloc(n_col * n_row, sizeof(double))};

  return m;
}

Matrix m_create(size_t n_row, size_t n_col) {

  if (n_col == 0 || n_row == 0) {
    fprintf(stderr, "Not a valid size: row - %zu, col - %zu", n_row, n_col);
    exit(1);
  }

  Matrix m = {n_col, n_row, malloc(n_col * n_row * sizeof(double))};

  if (m.data == NULL) {
    fprintf(stderr, "Failed to allocate matrix\n");
    exit(1);
  }
  return m;
}

Matrix m_copy(const Matrix *m_src) {

  Matrix new = m_create(m_src->n_row, m_src->n_col);
  memcpy(new.data, m_src->data, m_src->n_col * m_src->n_row * sizeof(double));

  return new;
}

void m_rand(Matrix *m, bool normalized) {

  for (size_t c = 0; c < m->n_col; c++) {
    for (size_t r = 0; r < m->n_row; r++) {
      MAT(*m, r, c) = rand();
    }
  }

  if (normalized) {
    m_scale(m, RAND_MAX);
  }
}

void m_scale(Matrix *m, double scale) {
  for (size_t c = 0; c < m->n_col; c++) {
    for (size_t r = 0; r < m->n_row; r++) {
      MAT(*m, r, c) /= scale;
    }
  }
}

Vec m_vec_mul(Matrix *m, Vec *v) {

  if (m->n_col != v->size) {
    fprintf(stderr, "Dimensions don't match: m_cols - %zu, v_size - %zu\n",
            m->n_col, v->size);
  }

  Vec res = vec_null(m->n_row);
  for (size_t r = 0; r < m->n_row; r++) {
    for (size_t c = 0; c < m->n_col; c++) {
      res.data[r] += MAT(*m, r, c) * v->data[c];
    }
  }
  return res;
}

Matrix m_mult(Matrix *m1, Matrix *m2) {

  if (m1->n_col != m2->n_row) {
    fprintf(stderr, "Dimensions don't match: m1_cols - %zu, m2_rows- %zu\n",
            m1->n_col, m2->n_row);
    exit(1);
  }

  Matrix res = m_null(m1->n_row, m2->n_col);
  for (size_t r = 0; r < m1->n_row; r++) {
    for (size_t c = 0; c < m2->n_col; c++) {
      for (size_t x = 0; x < m1->n_col; x++) {

        MAT(res, r, c) += MAT(*m1, r, x) * MAT(*m2, x, c);
      }
    }
  }
  return res;
}

Matrix m_transpose(Matrix *m1) {

  Matrix res = m_create(m1->n_col, m1->n_row);
  for (size_t r = 0; r < m1->n_row; r++) {
    for (size_t c = 0; c < m1->n_col; c++) {
      MAT(res, c, r) = MAT(*m1, r, c);
    }
  }

  return res;
}

void m_print(Matrix *m) {

  printf("[\n");
  for (size_t r = 0; r < m->n_row; r++) {
    printf("[");
    for (size_t c = 0; c < m->n_col; c++) {
      printf("%.3f ", MAT(*m, r, c));
    }
    printf("]\n");
  }
  printf("]\n");
}

void m_destroy(Matrix *m) {
  free(m->data);
  m->data = NULL;
  m->n_col = 0;
  m->n_row = 0;
}

Matrix m_add_row(Matrix *m, Vec *row) {

  if (m->n_col != row->size) {
    fprintf(stderr,
            "ERROR: Dimensions don't match. m.col=%zu, row length = %zu\n",
            m->n_col, row->size);
    exit(1);
  }

  size_t row_bytes = m->n_col * sizeof(double);
  Matrix new = m_create(m->n_row + 1, m->n_col);

  memcpy(new.data, m->data, m->n_row * row_bytes);

  memcpy(new.data + m->n_row * m->n_col, row->data, row_bytes);

  return new;
}

Matrix m_add_col(Matrix *m, Vec *col) {

  if (m->n_row != col->size) {
    fprintf(stderr,
            "ERROR: Dimensions don't match. m.row=%zu, col length = %zu\n",
            m->n_row, col->size);
    exit(1);
  }

  Matrix new = m_create(m->n_row, m->n_col + 1);

  for (size_t r = 0; r < m->n_row; r++) {
    memcpy(&MAT(new, r, 0), &MAT(*m, r, 0), m->n_col * sizeof(double));

    MAT(new, r, m->n_col) = col->data[r];
  }

  return new;
}

// Linear algebra

double fabs(double a) { return a < 0 ? a * -1 : a; }

void swap_rows(Matrix *m, size_t i1, size_t i2) {

  for (size_t c = 0; c < m->n_col; c++) {
    double temp = MAT(*m, i1, c);
    MAT(*m, i1, c) = MAT(*m, i2, c);
    MAT(*m, i2, c) = temp;
  }
}

size_t max_col(const Matrix *m, size_t i_col) {
  size_t i_max = i_col;
  for (size_t r = i_col; r < m->n_row; r++) {
    if (fabs(MAT(*m, r, i_col)) > fabs(MAT(*m, i_max, i_col))) {
      i_max = r;
    }
  }
  return i_max;
}

SolveStatus m_solve(const Matrix *A, const Vec *b, Vec *x) {
  // TODO: Add edge case checking

  Matrix M = m_copy(A);
  Vec rhs = vec_copy(b);

  for (size_t c = 0; c < M.n_col; c++) {
    size_t i_pivot = max_col(&M, c); // i of largest
    if (MAT(M, c, i_pivot) < 1e-9) {
      fprintf(stderr, "ERROR: near 0 pivot\n");
      return SOLVE_SINGULAR;
    }
    if (i_pivot != c) {
      swap_rows(&M, c, i_pivot);
      double tmp = rhs.data[c];
      rhs.data[c] = rhs.data[i_pivot];
      rhs.data[i_pivot] = tmp;
    }

    for (size_t r = c + 1; r < M.n_row; r++) {
      double m = MAT(M, r, c) / MAT(M, c, c);
      for (size_t j = c; j < M.n_col; j++) {
        double mR = m * MAT(M, c, j);
        MAT(M, r, j) -= mR;
      }

      rhs.data[r] -= m * rhs.data[c];
    }
  }
  for (int r = x->size - 1; r >= 0; r--) {

    double sum = rhs.data[r];
    for (size_t j = r + 1; j < x->size; j++) {
      sum -= MAT(M, r, j) * x->data[j];
    }

    x->data[r] = sum / MAT(M, r, r);
  }

  m_destroy(&M);
  vec_destroy(&rhs);
  return SOLVE_SUCCESS;
}

const char *solve_status_str(SolveStatus s) {
  switch (s) {
  case SOLVE_SUCCESS:
    return "SUCCESS";
  case SOLVE_SINGULAR:
    return "SINGULAR";
  case SOLVE_DIMENSION_MISMATCH:
    return "DIMENSION_MISMATCH";
  case SOLVE_INVALID_INPUT:
    return "INVALID_INPUT";
  default:
    return "UNKNOWN";
  }
}
