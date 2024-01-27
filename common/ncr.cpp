#include "common.hpp"

#include <algorithm>
#include <iostream>

using namespace std;

float choose(int n, int r) {
    r = min(r, n-r); // Take advantage of nCr = nC(n-r)

    // (n-r)! has been factored out of n!, product all remaining terms
    float out = 1;
    for (int denom = 1; denom <= r; denom++) {
        out *= (n - denom + 1) / (float) denom;
    }

    return out;
}