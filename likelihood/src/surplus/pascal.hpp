#ifndef PASCAL_H
#define PASCAL_H

#include "core.hpp"

mat pascal_rhombus(size_t n_rows, size_t n_cols);

mat scaled_rhombus(size_t n_rows, size_t n_cols, scalar row_mul, scalar col_mul);

#endif