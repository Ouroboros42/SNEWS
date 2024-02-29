#include "mesh.hpp"

#include <cfloat>

std::tuple<scalar, scalar> min_max_likelihood_in_range(mesh likelihoods, scalar start_offset, scalar stop_offset) {
    bool extrema_uninitialised = true;
    scalar min_offset;
    scalar min_likelihood;
    scalar max_offset;
    scalar max_likelihood;

    for (auto const& [offset, likelihood] : likelihoods) {
        if(offset >= start_offset && offset <= stop_offset) {
            // Initialise extrema to first offset in chosen range
            if (extrema_uninitialised) {
                min_offset = offset;
                min_likelihood = likelihood;
                max_offset = offset;
                max_likelihood = likelihood;
                extrema_uninitialised = false;
                continue;
            } 
            
            if (likelihood < min_likelihood) {
                min_likelihood = likelihood;
                min_offset = offset;
            }

            if (likelihood > max_likelihood) {
                max_likelihood = likelihood;
                max_offset = offset;
            }
        }
    }

    if(extrema_uninitialised)
        throw std::invalid_argument("No offsets in specified range");

    return { min_offset, max_offset };
}

std::tuple<scalar, scalar> range_over_likelihood_threshold(mesh likelihoods, scalar threshold) {
    bool extrema_uninitialised = true;
    scalar min_offset;
    scalar max_offset;

    for (auto const& [offset, likelihood] : likelihoods) {
        if (likelihood > threshold) {
            if (extrema_uninitialised) {
                // Initialise extrema to first offset with likelihood over threshold
                min_offset = offset;
                max_offset = offset;
                extrema_uninitialised = false;
                continue;
            }

            if (offset < min_offset) {
                min_offset = offset;
            }
            
            if (offset > max_offset) {
                max_offset = offset;
            }
        }
    }

    if (extrema_uninitialised)
        throw std::invalid_argument("No likelihoods greater than threshold");

    return { min_offset, max_offset };
}

std::tuple<scalar, scalar> most_likely_offset(mesh likelihoods) {
    scalar likeliest_offset;
    scalar max_likelihood = DBL_MIN;

    for (auto const& [offset, likelihood] : likelihoods) {
        if (likelihood > max_likelihood) {
            max_likelihood = likelihood;
            likeliest_offset = offset;
        }
    }

    if (max_likelihood == DBL_MIN) throw std::invalid_argument("Empty likelihood mesh");

    return { likeliest_offset, max_likelihood };
}

mesh make_recursive_likelihood_mesh(
    SignalAnalysis signals,
    scalar rel_accuracy,
    scalar sweep_start,
    scalar sweep_stop,
    size_t initial_sweep_steps,
    size_t resweep_steps,
    scalar required_accuracy,
    scalar min_window_shrink,
    size_t max_points
) {
    mesh likelihoods;
    
    bool accuracyAchieved = false;

    size_t sweep_steps = initial_sweep_steps;

    while(likelihoods.size() < max_points) {
        scalar step_size = (sweep_stop - sweep_start) / (sweep_steps - 1);

        // calculate Likelihoods
        for(scalar offset = sweep_start; offset <= sweep_stop; offset += step_size) {
            if (likelihoods.count(offset) == 0) {
                likelihoods[offset] = signals.lag_log_likelihood(offset, rel_accuracy, false);
            }
        }

        // print current size
        std::cout << "Number of Likelihoods calculated so far: " << likelihoods.size() << std::endl;

        // find max and min Likelihoods
        auto [min_likelihood, max_likelihood] = min_max_likelihood_in_range(likelihoods, sweep_start, sweep_stop);

        bool accuracy_achieved = max_likelihood - min_likelihood < required_accuracy;
        if(accuracy_achieved) {
            std::cout << "Accuracy achieved!" << std::endl;
            break;
        }

        // Define new mesh to search
        scalar threshold = (min_likelihood + max_likelihood) / 2;

        auto [new_sweep_start, new_sweep_stop] = range_over_likelihood_threshold(likelihoods, threshold);

        bool range_not_shrinking = (std::abs(sweep_start - new_sweep_start) + std::abs(sweep_stop - new_sweep_stop)) < min_window_shrink;
        if(range_not_shrinking) {
            std::cout << "Range not shrinking, giving up." << std::endl;
            break;
        }

        std::cout << "New mesh to search: " << new_sweep_start << ", " << new_sweep_stop << std::endl;

        // update sweeping parameters
        sweep_start = new_sweep_start;
        sweep_stop = new_sweep_stop;

        sweep_steps = resweep_steps;
    }
    
    std::cout << "Total number of likelihoods calculated = " << likelihoods.size() << std::endl;
    
    return likelihoods;
}