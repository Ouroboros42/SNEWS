#include "fast_sum/converging.hpp"
#include "fast_sum/lazy.hpp"

#include <cmath>
#include <optional>

// Chokepoint of entire operation
inline scalar exp_scaled(scalar log_x, scalar log_rescale) {
    scalar result = std::exp(log_x - log_rescale);

    if (std::isinf(result)) {
        throw std::runtime_error("Rescaling did not suppress large term");
    }
    
    return result;
}

inline bool cutoff_criterion(scalar next_term, scalar total, scalar rel_precision) {
    return next_term <= total * rel_precision;
}

scalar converging_sum_exp(LazyArray const& log_terms, scalar total, scalar log_rescale, scalar term_rel_precision) {
    size_t n_terms = log_terms.size();

    for (size_t i = 0; i < n_terms; i++) {
        scalar next_term = exp_scaled(log_terms.get(i), log_rescale);
        
        if (cutoff_criterion(next_term, total, term_rel_precision)) break; // Subsequent terms negligible

        total += next_term;
    }

    return total;
}

/* Gives lazy arrays of the elements in row_i to the left and right of index split_j */
std::pair<LazySubArray, LazySubArray> split_row(Lazy2DArray& arr2D, size_t row_i, size_t split_j) {
    return std::make_pair(
        LazySubArray(arr2D, row_i, split_j - 1, false),
        LazySubArray(arr2D, row_i, split_j + 1, true)
    );
}

/* Return total plus the exp of significant terms in row_i, except for element lead_j */
scalar sum_exp_row_arms(Lazy2DArray& log_terms, size_t row_i, size_t lead_j, scalar total, scalar log_rescale, scalar term_rel_precision) {
    auto [left_arm, right_arm] = split_row(log_terms, row_i, lead_j);
    total = converging_sum_exp(left_arm, total, log_rescale, term_rel_precision);
    total = converging_sum_exp(right_arm, total, log_rescale, term_rel_precision);

    return total;
}

/* Return total plus exp of all significant terms in row_i
Returns nullopt if the lead term is insignificant */
std::optional<scalar> sum_exp_row(Lazy2DArray& log_terms, size_t row_i, size_t lead_j, scalar total, scalar log_rescale, scalar term_rel_precision) {
    scalar lead_term = exp_scaled(log_terms.get(row_i, lead_j), log_rescale);

    // Reject whole row
    if (cutoff_criterion(lead_term, total, term_rel_precision)) return std::nullopt;

    total += lead_term;

    return sum_exp_row_arms(log_terms, row_i, lead_j, total, log_rescale, term_rel_precision);
}

scalar log_sum_exp_peaked_2D(Lazy2DArray& log_terms, size_t lead_i, index_getter get_lead_j, scalar rel_precision) {
    size_t lead_j = get_lead_j(lead_i);

    scalar log_rescale = log_terms.get(lead_i, lead_j); // Rescale all subsequent terms to lead term

    scalar total = 1; // This is the relative value of the lead term

    scalar term_rel_precision = rel_precision / (log_terms.size_x() * log_terms.size_y());

    total = sum_exp_row_arms(log_terms, lead_i, lead_j, total, log_rescale, term_rel_precision);

    // unsigned int will wrap around to large number past 0, so loop condition is equivalent to i >= 0 if i were an int
    for (size_t i = lead_i - 1; i < lead_i; i--) {
        if(auto row_result = sum_exp_row(log_terms, i, get_lead_j(i), total, log_rescale, term_rel_precision)) {
            // Row was significant
            total = *row_result;
        } else {
            // Row was insignificant
            break;
        }
    }

    for (size_t i = lead_i + 1; i < log_terms.size_x(); i++) {
        if(auto row_result = sum_exp_row(log_terms, i, get_lead_j(i), total, log_rescale, term_rel_precision)) {
            // Row was significant
            total = *row_result;
        } else {
            // Row was insignificant
            break;
        }
    }

    return log_rescale + std::log(total);
}