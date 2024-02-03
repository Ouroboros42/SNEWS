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
public:
    FactorialCache(size_t max_n);

    // largest n for which log(n!) is stored
    size_t size();

    // log(n) for 0 < n <= size
    inline scalar log(size_t n) { return LogNumberVector[n-1]; }
    
    // log(n!) for 0 <= n <= size
    inline scalar log_factorial(size_t n) { return (n == 0) ? 0 : LogFactorialVector[n-1]; }

    // Construct the series log(x^n / n!) for 0 <= n < n_terms
    vec exp_series(scalar x, size_t n_terms);

    // log((r+s) C r)
    scalar log_binomial(size_t r, size_t s);
};

#endif