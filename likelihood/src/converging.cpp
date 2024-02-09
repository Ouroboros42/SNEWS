#include "converging.hpp"

#include <cmath>
#include <iostream>
#include <chrono>

scalar log_converging_double_sum(size_t n, size_t m, std::function<scalar(size_t i, size_t j)> terms, scalar log_rel_accuracy) {
    scalar log_total = terms(0, 0);

    // scalar NN = 1;                          // just count number of terms you add to the sum
    // scalar TOTAL = m*n == 0? 1 : m*n;       // total number of terms in the double sum

    for (size_t i = 0; i < n; i++) {
        for (size_t j = (i == 0 ? 1 : 0) ; j < m; j++) {
            // Skip first term, has already been included

            scalar log_next_term_rel = terms(i, j) - log_total;

            if (log_next_term_rel < log_rel_accuracy) {
                // Identify negligible terms
                if (j == 0) {
                    // No more siginificant terms in later rows
                    // std::cout << "Ended at i = " << i << std::endl;
                    // std::cout << "Number of terms added: " << NN/TOTAL << std::endl;  ######
                    return log_total;
                } 
                else {
                    // No more siginificant terms in this row
                    // std::cout << "New row at j = " << j << std::endl;
                    break; 
                }
            }

            scalar log_rel_sum = log(1 + exp(log_next_term_rel));

            log_total += log_rel_sum;
            // NN += 1;
        }    
    }
    // std::cout << "Number of terms added: " << NN/TOTAL << std::endl;
    return log_total;
}


scalar log_converging_bin_likelihood(FactorialCache cache, DetectorComparison comp, size_t count_1, size_t count_2, scalar log_accuracy) {
    // Scale termwise accuracy to number of terms to achieve reliable overall accuracy
    return log_converging_double_sum(count_1, count_2, log_sum_terms(cache, comp, count_1, count_2), log_accuracy - cache.log(count_1) - cache.log(count_2));
}


scalar log_likelihood(FactorialCache cache, scalar background_rate_1, scalar background_rate_2, Histogram time_dist_1, Histogram time_dist_2, size_t n_bins, scalar log_accuracy) {
    scalar log_bin_accuracy = log_accuracy; // TODO Identify correct error propagation

    DetectorComparison comp(background_rate_1, background_rate_2, time_dist_1, time_dist_2);

    scalar total = comp.log_likelihood_prefactor(time_dist_1.get_n_data(), time_dist_2.get_n_data());

    /*
        uncomment the lines below if running end2end, not end2endExp
    */
    // auto T1 = std::chrono::high_resolution_clock::now();
    // cache.build_upto(time_dist_1.max_bin() + time_dist_2.max_bin());
    // auto T2 = std::chrono::high_resolution_clock::now();
    // std::cout << "Time to build cache: " << std::chrono::duration_cast<std::chrono::milliseconds>(T2 - T1).count() << " ms\n";

    auto T3 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n_bins; i++) {
        //std::cout << "bin number  = " << i << "  ";
        total += log_converging_bin_likelihood(cache, comp, time_dist_1.get_bin(i), time_dist_2.get_bin(i), log_bin_accuracy);
    }
    auto T4 = std::chrono::high_resolution_clock::now();
    std::cout << "Time to compute double sum: " << std::chrono::duration_cast<std::chrono::milliseconds>(T4 - T3).count() << " ms\n";

    return total;
}