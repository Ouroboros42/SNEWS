#include "core.hpp"
#include "test_data/data_load.hpp"
#include "fast_sum/converging.hpp"
#include "data_io/write_output.hpp"
#include "caching/factorials.hpp"
#include "detector_info/relation.hpp"
#include "fast_sum/sum_terms.hpp"
#include "data_io/timestamp.hpp"
#include "util/array_util.hpp"

#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <random>
#include <chrono>

int main(int argc, char* argv[]) {
    auto FULL_START = std::chrono::high_resolution_clock::now();

    // Test data to use
    Detector detector1 = Detector::IceCube, detector2 = Detector::SuperK;
    std::string inst = "121"; // Numerical identifier of test data (appears in file name)
    bool poisson_vary_background = false; // Use variable total background counts, according to poisson

    // Extra space to include around true event (more realistic to application where edges are not known)
    scalar front_buffer = 1;
    scalar window = 20;

    scalar bin_width = 2e-3;

    // Time difference sweep params
    size_t n_sweep_steps = 1000;
    scalar sweep_start = -0.05;
    scalar sweep_end = 0.05;
    
    scalar rel_accuracy = 1e-6;

    auto [test_case, true_d] = complete_test_case(
        detector1, detector2, inst,
        front_buffer, window,
        sweep_start, sweep_end,
        poisson_vary_background,
        bin_width
    );

    // Will hold values of all calculated likelihoods
    vec likelihoods(n_sweep_steps), time_differences(n_sweep_steps);

    // An index and time information will be printed every nth likelihood
    size_t print_every_n = 10;

    for (size_t i = 0; i < n_sweep_steps; i++) {
        // positive offset corresponds to signal 2 arriving after signal 1
        scalar offset = sweep_start + ((sweep_end - sweep_start) * i / n_sweep_steps);

        auto T1 = std::chrono::high_resolution_clock::now();

        Histogram hist_2 = test_case.bin_signal_2(offset);
        
        auto T2 = std::chrono::high_resolution_clock::now();

        scalar likelihood = test_case.lag_log_likelihood(offset, rel_accuracy, false);

        auto T3 = std::chrono::high_resolution_clock::now();
        
        // Display timing info
        if (i % print_every_n == 0) {
            int hist_time = std::chrono::duration_cast<std::chrono::milliseconds>(T2 - T1).count();
            int likelihood_time = std::chrono::duration_cast<std::chrono::milliseconds>(T3 - T2).count();
            std::printf("i = %u\nTime to build histograms: %u ms\nTime to compute likelihood: %u ms\n", i, hist_time, likelihood_time);
        }

        time_differences[i] = offset;
        likelihoods[i] = likelihood;
    }

    // Examples of the signal 2 binnings, for debugging (possibly remove later)
    std::vector<Histogram> hist_2_samples {
        test_case.bin_signal_2(sweep_start),
        test_case.bin_signal_2(0),
        test_case.bin_signal_2(sweep_end)
    };

    std::string outputname = "output/ldist_" + detector_name(detector1) + "-vs-" + detector_name(detector2) + "_src=" + inst + "_t=" + get_timestamp() + ".json";

    save_likelihoods(outputname, time_differences, likelihoods, test_case.signal_1, hist_2_samples, test_case.signal_1.range());

    // Identify max likelihood as heuristic
    scalar max_likelihood = max(likelihoods);
    scalar best = time_differences[index_of_max(likelihoods)];

    // Display final timing information
    auto FULL_END = std::chrono::high_resolution_clock::now();
    int total_time = std::chrono::duration_cast<std::chrono::seconds>(FULL_END - FULL_START).count();

    std::printf(
        "\n\nMax likelihood = %.10f\n\nMost probable time difference = %.10f\n\nTrue time difference = %.10f\n\nSweep completed in: %u s\n\n",
        max_likelihood, best, true_d, total_time
    );

    return 0;
}