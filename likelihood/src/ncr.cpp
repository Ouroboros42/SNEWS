#include "core.hpp"

#include <algorithm>

float combinations_split(int r1, int r2) {
    return combinations(r1 + r2, std::min(r1, r2));
}

float combinations(int n, int r) {
    if (r < 0) return 0;

    // (n-r)! has been factored out of n!, product all remaining terms
    float out = 1;
    for (int denom = 1; denom <= r; denom++) {
        out *= (n - denom + 1) / (float) denom;
    }

    return out;
}