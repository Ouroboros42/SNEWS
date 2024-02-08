#include "core.hpp"
#include "test_data/data_load.hpp"
#include "converging.hpp"

#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <random>
#include <chrono>

// If you run End2End you will see that roughly 20% of the time is used to regenrate the histogram and 80% of the time is used to compute the likelihood.
// However, The likelihood compute time increases if you put the larger detector as the first one TODO: Investigate why this happens

// In End2EndExp, we have  the first historgram (for the bigger detector, which is argument 2) generated only once
// The histogram for the smaller detector is regenerated for each iteration of the loop and basically shaves off 200 ms from each likelihood calc

// THe cache is only generated once but has basically a negligible effect because it's too quick anyway


void print_vec(vec elems) {
    std::cout << "[" << elems[0];
    for (size_t i = 1; i < elems.size(); i++) {
        std::cout << ", " << elems[i];
    }
    std::cout << "]\n";
}

int main(int argc, char* argv[]) {
    Detector detector1 = Detector::SuperK, detector2 = Detector::IceCube;
    DetectorSignal data1(detector1), data2(detector2);

    // histogram parameters
    size_t n_bins = 2000;
    scalar period = 20;
    scalar start = std::max(data1.start_time, data2.start_time);

    // computing parameters
    scalar log_accuracy = -5;

    // Sweep parameters
    scalar sweep_start = -0.2;
    scalar sweep_end = 0.2;
    size_t n = 50;

    // first offset is basically sweep_start
    scalar offset = sweep_start;

    // create histograms
    Histogram hist1(n_bins, start + offset, start + offset + period, data1.time_series);
    Histogram hist2(n_bins, start, start + period, data2.time_series);

    // add background (get rate in ms, and multiply by 1000 for rate in s)
    scalar b1 = 1000 * background_rates_ms(detector1);
    scalar b2 = 1000 * background_rates_ms(detector2);
    add_background(hist1, b1);
    add_background(hist2, b2);

    // build cache only once
    auto Ti = std::chrono::high_resolution_clock::now();
    FactorialCache cache;
    cache.build_upto(hist1.max_bin() + hist2.max_bin());
    auto Tf = std::chrono::high_resolution_clock::now();
    std::cout << "Time to build cache: " << std::chrono::duration_cast<std::chrono::milliseconds>(Tf - Ti).count() << " ms\n";

    vec likelihoods(n), offsets(n);

    //iteration number 0
    int i = 0;
    std::cout << "i = " << i << std::endl;
    std::cout << "offset = " << offset << std::endl;
    likelihoods[i] = log_likelihood(cache, b1, b2, hist1, hist2, n_bins, log_accuracy);
    offsets[i] = offset;

    for (i=1; i < n; i++) {
        std::cout << "i = " << i << std::endl;
        std::cout << "offset = " << offset << std::endl;
        auto T1 = std::chrono::high_resolution_clock::now();

        // regenrate the second histogram again with new offset and add background
        offset += ((sweep_end - sweep_start) * i / n);
        Histogram new_hist1(n_bins, start + offset, start + period + offset, data1.time_series);
        add_background(new_hist1, b1);

        auto T2 = std::chrono::high_resolution_clock::now();

        // compute likelihood
        likelihoods[i] = log_likelihood(cache, b1, b2, new_hist1, hist2, n_bins, log_accuracy);
        offsets[i] = offset;

        auto T3 = std::chrono::high_resolution_clock::now();
        std::cout << "Time to regenerate histogram: " << std::chrono::duration_cast<std::chrono::milliseconds>(T2 - T1).count() << " ms\n";
        std::cout << "Time to compute likelihood: " << std::chrono::duration_cast<std::chrono::milliseconds>(T3 - T2).count() << " ms\n";
    }

    // results
    scalar max_likelihood = *std::max_element(likelihoods.begin(), likelihoods.end());
    size_t max_i = std::distance(likelihoods.begin(), std::max_element(likelihoods.begin(), likelihoods.end()));
    scalar best = offsets[max_i];

    scalar true_d = data2.true_time - data1.true_time;

    std::cout << std::setprecision(10);

    std::cout << "\nt = ";
    print_vec(offsets);
    std::cout <<"\nL = ";
    print_vec(likelihoods);

    std::cout << "\n\nMost probable time difference = " << best;

    std::cout << "\n\nTrue time difference = " << true_d;

    std::cout << "\n\nMax likelihood = " << max_likelihood;

    return 0;
}