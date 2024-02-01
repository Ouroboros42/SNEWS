#include "converging.hpp"

#include <cmath>

/*Assumes M is compatible size with V, U*/
scalar mvv_contract(mat M, vec V, vec U, scalar accuracy) {
    scalar total = 0;

    scalar eps = abs(accuracy * M[0][0] * V[0] * U[0]);

    for (size_t i = 0; i < V.size(); i++) {
        for (size_t j = 0; j < U.size(); j++) {
            scalar term = M[i][j] * V[i] * U[j];

            total += term;

            if (abs(term) < accuracy) {
                if (j == 0) return total;
                else break;
            }
        }    
    }

    return total;
}