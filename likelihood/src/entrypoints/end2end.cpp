#include "core.hpp"
#include "test_data/data_load.hpp"
#include "converging.hpp"

#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <random>
#include <chrono>


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

    FactorialCache cache;

    scalar log_accuracy = -5;

    size_t n_bins = 2000;
    scalar period = 20;

    scalar sweep_start = -0.2;
    scalar sweep_end = 0.2;
    size_t n = 50;

    vec likelihoods(n), offsets(n);

    for (size_t i = 0; i < n; i++) {
        std::cout << "i = " << i << std::endl;

        scalar start = std::max(data1.start_time, data2.start_time);
        scalar end = start + period;

        scalar offset = sweep_start + ((sweep_end - sweep_start) * i / n);
        offsets[i] = offset;

        auto T1 = std::chrono::high_resolution_clock::now();
        Histogram hist1(n_bins, start, end, data1.time_series), hist2(n_bins, start + offset, end + offset, data2.time_series);

        scalar background_1 = 1000 * background_rates_ms(detector1), background_2 = 1000 * background_rates_ms(detector2);

        add_background(hist1, background_1);
        add_background(hist2, background_2);

        auto T2 = std::chrono::high_resolution_clock::now();

        // std::cout << hist1.max_bin();
        // std::cout << hist2.max_bin();

        // std::cout << "\n" << hist1.display() << "\n" << hist2.display();

        scalar likelihood = log_likelihood(cache, background_1, background_2, hist1, hist2, n_bins, log_accuracy);
        likelihoods[i] = likelihood;
        auto T3 = std::chrono::high_resolution_clock::now();

        std::cout << "Time to build histograms: " << std::chrono::duration_cast<std::chrono::milliseconds>(T2 - T1).count() << " ms\n";
        std::cout << "Time to compute likelihood: " << std::chrono::duration_cast<std::chrono::milliseconds>(T3 - T2).count() << " ms\n";
        // std::cout << "\n\nTime Difference = " << offset << "\nLog Likelihood = " << likelihood;
    }

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