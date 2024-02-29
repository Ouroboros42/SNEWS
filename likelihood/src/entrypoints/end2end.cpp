#include "core.hpp"
#include "test_data/data_load.hpp"
#include "fast_sum/converging.hpp"
#include "data_io/write_output.hpp"
#include "caching/factorials.hpp"
#include "detector_info/relation.hpp"
#include "fast_sum/sum_terms.hpp"
#include "data_io/timestamp.hpp"
#include "util/array_util.hpp"
#include "mesh.hpp"

#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <random>
#include <chrono>

int main(int argc, char* argv[]) {
    auto FULL_START = std::chrono::high_resolution_clock::now();

    // Test data to use
    Detector detector1 = Detector::IceCube, detector2 = Detector::SNOPlus;
    std::string inst = "243"; // Numerical identifier of test data (appears in file name)
    bool poisson_vary_background = false; // Use variable total background counts, according to poisson

    // Extra space to include around true event (more realistic to application where edges are not known)
    scalar front_buffer = 1;
    scalar window = 20;

    scalar bin_width = 0.04;

    scalar sweep_start = -0.2;
    scalar sweep_end = 0.2;

    size_t initial_sweep_steps = 100;
    size_t resweep_steps = 50;
    size_t max_points = 400;

    scalar likelihood_range = 1/2;
    scalar min_window_shrink = 0.001;

    scalar rel_accuracy = 1e-3;

    auto [test_case, true_d] = complete_test_case(
        detector1, detector2, inst,
        front_buffer, window,
        sweep_start, sweep_end,
        poisson_vary_background,
        bin_width
    );

    mesh log_likelihoods = make_recursive_likelihood_mesh(
        test_case, rel_accuracy, sweep_start, sweep_end,
        initial_sweep_steps, resweep_steps,
        likelihood_range,
        min_window_shrink, max_points
    );

    // Examples of the signal 2 binnings, for debugging (possibly remove later)
    std::vector<Histogram> hist_2_samples {
        test_case.bin_signal_2(sweep_start),
        test_case.bin_signal_2(0),
        test_case.bin_signal_2(sweep_end)
    };

    std::string outputname = "output/ldist_" + detector_name(detector1) + "-vs-" + detector_name(detector2) + "_src=" + inst + "_t=" + get_timestamp() + ".json";

    save_likelihoods(outputname, log_likelihoods, test_case.signal_1, hist_2_samples, test_case.signal_1.range(), true_d);

    // Identify max likelihood as heuristic
    auto [best_offset, max_likelihood] = most_likely_offset(log_likelihoods);

    // Display final timing information
    auto FULL_END = std::chrono::high_resolution_clock::now();
    int total_time = std::chrono::duration_cast<std::chrono::seconds>(FULL_END - FULL_START).count();

    std::printf(
        "\n\nMax likelihood = %.10f\n\nMost probable time difference = %.10f\n\nTrue time difference = %.10f\n\nSweep completed in: %u s\n\n",
        max_likelihood, best_offset, true_d, total_time
    );

    return 0;
}