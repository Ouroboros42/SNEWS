#include "core.hpp"
#include "binomials.hpp"

#include <algorithm>
#include <cmath>

scalar std_log_factorial(scalar n) {
    return lgamma(n + 1);
}

scalar std_binomial(scalar n, scalar r) {
    return 1/((n+1)*std::beta(n-r+1, r+1)); // tgamma(n + 1) / (tgamma(n - r + 1) * tgamma(r + 1));
}

scalar std_log_binomial(scalar n, scalar r) {
    return std_log_factorial(n) - std_log_factorial(n - r) - std_log_factorial(r);
}

scalar binomial(int n, int r) {
    if (r < 0 || n < r) return 0;

    // (n-r)! has been factored out of n!, product all remaining terms
    scalar out = 1;
    for (int denom = 1; denom <= r; denom++) {
        out *= (n - denom + 1) / (scalar) denom;
    }

    return out;
}