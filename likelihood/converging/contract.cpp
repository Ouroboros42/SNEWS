#include "converging.hpp"

#include <cmath>

scalar log_mvv_contract(mat M, vec V, vec U, scalar rel_accuracy) {
    // Assumes M, V, U are of compatible sizes
    size_t n = V.size(), m = U.size();

    scalar log_total = M[0][0] + V[0] + U[0];
    scalar log_eps = log(rel_accuracy / n / m); // Account for potentially many terms of similar order by increasing sensitivity

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            if (i == 0 && j == 0) continue; // First term has already been calculated

            scalar log_next_term_rel = M[i][j] + V[i] + U[j] - log_total;

            if (log_next_term_rel < log_eps) { // Identify negligible terms
                if (j == 0) return log_total; // No more siginificant terms in later rows
                else break; // No more siginificant terms in this row
            }

            scalar log_rel_sum = log(1 + exp(log_next_term_rel));

            log_total += log_rel_sum;
        }    
    }

    return log_total;
}