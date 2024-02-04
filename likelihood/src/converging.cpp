#include "converging.hpp"

#include <cmath>

scalar log_converging_double_sum(size_t n, size_t m, std::function<scalar(size_t i, size_t j)> terms, scalar log_rel_accuracy) {
    scalar log_total = terms(0, 0);

    for (size_t i = 0; i < n; i++) {
        for (size_t j = (i == 0 ? 1 : 0) ; j < m; j++) { // Skip first term, has already been included
            scalar log_next_term_rel = terms(i, j) - log_total;

            if (log_next_term_rel < log_rel_accuracy) { // Identify negligible terms
                if (j == 0) return log_total; // No more siginificant terms in later rows
                else break; // No more siginificant terms in this row
            }

            scalar log_rel_sum = log(1 + exp(log_next_term_rel));

            log_total += log_rel_sum;
        }    
    }

    return log_total;
}