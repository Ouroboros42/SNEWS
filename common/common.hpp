#ifndef COMMON_H
#define COMMON_H

#include<array>
#include<vector>

using namespace std;

typedef double scalar;
typedef vector<scalar> vec;
typedef vector<vector<scalar>> mat;

float combinations_split(int r1, int r2);

float combinations(int n, int r);

mat pascal_rhombus(size_t n_rows, size_t n_cols);

mat scaled_rhombus(size_t n_rows, size_t n_cols, scalar row_mul, scalar col_mul);

class FactorialCache {
private:
    size_t max_n;
    vec LogNumberVector;
    vec LogFactorialVector;
    void buildVectors();
public:
    FactorialCache(size_t max_n);
    size_t size();
    inline scalar log(size_t n) { return LogNumberVector[n-1]; }
    inline scalar log_factorial(size_t n) { return LogFactorialVector[n-1]; }
};

#endif