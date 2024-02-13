#include "core.hpp"
#include "test_data/data_load.hpp"
#include "converging.hpp"
#include "write_output.hpp"

#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <random>
#include <chrono>

int main(int argc, char* argv[]) {
    std::string inst = "121";

    Detector detector1 = Detector::IceCube, detector2 = Detector::SuperK;
    DetectorSignal data1(detector1, inst), data2(detector2, inst);

    scalar background_1 = 1000 * background_rates_ms(detector1);
    scalar background_2 = 1000 * background_rates_ms(detector2);

    scalar sweep_start = -0.2;
    scalar sweep_end = 0.2;

    scalar event_margin = 1;

    scalar window_start = std::min(data1.start_time, data2.start_time) - event_margin;
    scalar window_end = window_start + 20; // std::max(data1.end_time, data2.end_time) + event_margin;

    // Widest range over which detector 2 signal will be binned
    scalar window_start_min = window_start + sweep_start;
    scalar window_end_max = window_end + sweep_end;

    // data1.add_background(background_1, window_start, window_end);
    data2.add_background(background_2, window_start_min, window_end_max);

    FactorialCache cache;

    scalar log_accuracy = -5;

    size_t n_bins = 2000;

    size_t n = 400;

    vec likelihoods(n), offsets(n);

    Histogram hist1(n_bins, window_start, window_end, data1.time_series);
    add_background(hist1, background_1);

    for (size_t i = 0; i < n; i++) {
        std::cout << "i = " << i << std::endl;

        scalar offset = sweep_start + ((sweep_end - sweep_start) * i / n);
        offsets[i] = offset;

        auto T1 = std::chrono::high_resolution_clock::now();
        Histogram hist2(n_bins, window_start + offset, window_end + offset, data2.time_series);
        //add_background(hist2, background_2);
        auto T2 = std::chrono::high_resolution_clock::now();

        // std::cout << hist1.max_bin();
        // std::cout << hist2.max_bin();

        // std::cout << "\n" << hist1.display() << "\n" << hist2.display();

        scalar likelihood = log_likelihood(cache, background_1, background_2, hist1, hist2, n_bins, log_accuracy);
        likelihoods[i] = likelihood;
        auto T3 = std::chrono::high_resolution_clock::now();

        std::cout << "Time to build histograms: " << std::chrono::duration_cast<std::chrono::milliseconds>(T2 - T1).count() << " ms\n";
        std::cout << "Time to compute likelihood: " << std::chrono::duration_cast<std::chrono::milliseconds>(T3 - T2).count() << " ms\n";
    }

    std::string outputname = "output/ldist-src=" + inst + "-steps=" + std::to_string(n) + "-bins=" + std::to_string(n_bins) + ".json";

    save_likelihoods(outputname, offsets, likelihoods);

    scalar max_likelihood = *std::max_element(likelihoods.begin(), likelihoods.end());
    size_t max_i = std::distance(likelihoods.begin(), std::max_element(likelihoods.begin(), likelihoods.end()));
    scalar best = offsets[max_i];

    scalar true_d = data2.true_time - data1.true_time;

    std::cout << "\n\nMax likelihood = " << max_likelihood;

    std::cout << "\n\nMost probable time difference = " << best;

    std::cout << "\n\nTrue time difference = " << true_d << "\n" << std::endl;

    return 0;
}