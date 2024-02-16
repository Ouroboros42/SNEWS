#include "core.hpp"
#include "test_data/data_load.hpp"
#include "converging.hpp"
#include "data_io/write_output.hpp"
#include "caching/factorials.hpp"
#include "detector_info/relation.hpp"
#include "sum_terms.hpp"

#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <random>
#include <chrono>

template <typename T>
bool contains(std::vector<T> collection, T element) {
    return std::find(collection.begin(), collection.end(), element) != collection.end();
}

int main(int argc, char* argv[]) {
    std::string inst = "121"; // Numerical identifier of test data (appears in file name)

    Detector detector1 = Detector::IceCube, detector2 = Detector::SuperK;

    std::string det_name_1 = detector_name(detector1), det_name_2 = detector_name(detector2);

    TestSignal signal_1(detector1, inst), signal_2(detector2, inst);

    scalar background_1 = background_rate_s(detector1);
    scalar background_2 = background_rate_s(detector2);

    scalar zero_time = std::min(signal_1.start, signal_2.start);

    // Shift times so zero is at first event (not really necessary)
    signal_1.rezero_times(zero_time);
    signal_2.rezero_times(zero_time);

    // Min and max time differences to test
    scalar sweep_start = -0.2;
    scalar sweep_end = 0.2;
    
    // Extra space to include around true event (more realistic to application where edges are not known)
    scalar front_buffer = 1;
    scalar back_buffer = 1;

    // Range over which detector 1 is sampled for all cases
    // Detector 2 is sampled over this window, with offsets varying between sweep start and sweep end
    scalar window_start = std::min(signal_1.start, signal_2.start) + sweep_start - front_buffer;
    scalar window_end = std::max(signal_1.stop, signal_2.stop) + sweep_end + back_buffer;

    scalar window_width = window_end - window_start;

    // Widest range over which detector 2 signal will be binned
    scalar window_start_min = window_start + sweep_start;
    scalar window_end_max = window_end + sweep_end;

    // Simulate uniform background rate across both datasets
    signal_1.reframe(window_start, window_end);
    signal_1.add_background(background_1);
    signal_2.reframe(window_start_min, window_end_max);
    signal_2.add_background(background_2);
    
    // Reuse log-factorial calculations in all cases
    FactorialCache cache;

    // Maximum acceptable (proportional) error in calculated likelihood
    scalar rel_accuracy = 0.0000001;

    // Number of bins to split data in range into
    size_t n_bins = 100;

    // Number of time differences to calculate likelihoods for
    size_t n_steps = 1000;

    // !! Calculate sensitivty once
    DetectorRelation detectors(background_1, background_2, signal_1, signal_2);

    // Will hold values of all calculated likelihoods
    vec likelihoods(n_steps), time_differences(n_steps);

    // Signal 1 is binned only once, as only signal 2 is offset each iteration
    Histogram hist_1 = signal_1.to_hist(n_bins);

    // Examples of the signal 2 binnings, for debugging (possibly remove later)
    std::vector<size_t> hist_2_sample_indices = { 0, n_steps/2, n_steps-1 };
    std::vector<Histogram> hist_2_samples;
    hist_2_samples.reserve(hist_2_sample_indices.size());

    // An index and time information will be printed every nth likelihood
    size_t print_every_n = 50;

    for (size_t i = 0; i < n_steps; i++) {
        // positive offset corresponds to signal 2 arriving after signal 1
        scalar offset = sweep_start + ((sweep_end - sweep_start) * i / n_steps);

        time_differences[i] = offset;

        auto T1 = std::chrono::high_resolution_clock::now();

        // Rebin signal 2
        Histogram hist_2 = signal_2.to_hist(n_bins, window_start + offset, window_end + offset);
        
        auto T2 = std::chrono::high_resolution_clock::now();

        // !! Recalculate sensitivity each time
        // DetectorRelation detectors(background_1, background_2, hist_1, hist_2);

        // Record sample of signal 2 binning
        if (contains(hist_2_sample_indices, i)) {
            hist_2_samples.push_back(hist_2);
        }

        scalar likelihood = log_likelihood(cache, detectors, hist_1, hist_2, rel_accuracy);

        likelihoods[i] = likelihood;

        auto T3 = std::chrono::high_resolution_clock::now();
        
        // Display timing info
        if (i % print_every_n == 0) {
            std::printf(
                "i = %u\nTime to build histograms: %u ms\nTime to compute likelihood: %u ms\n", i,
                std::chrono::duration_cast<std::chrono::milliseconds>(T2 - T1).count(),
                std::chrono::duration_cast<std::chrono::milliseconds>(T3 - T2).count()
            );
        }
    }

    std::string outputname = "output/ldist-" + det_name_1 + "-vs-" + det_name_2 + "-src=" + inst + "-steps=" + std::to_string(n_steps) + "-bins=" + std::to_string(n_bins) + ".json";

    save_likelihoods(outputname, time_differences, likelihoods, hist_1, hist_2_samples, window_width);

    // Identify max likelihood as heuristic
    scalar max_likelihood = max(likelihoods);
    scalar best = time_differences[index_of_max(likelihoods)];

    scalar true_d = signal_2.true_time - signal_1.true_time;

    std::cout << "\n\nMax likelihood = " << max_likelihood;

    std::cout << "\n\nMost probable time difference = " << best;

    std::cout << "\n\nTrue time difference = " << true_d << "\n" << std::endl;

    return 0;
}