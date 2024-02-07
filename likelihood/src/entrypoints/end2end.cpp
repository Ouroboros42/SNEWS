#include "core.hpp"
#include "test_data/data_load.hpp"
#include "caching/terms.hpp"
#include "inference.hpp"

#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <random>

void print_vec(vec elems) {
    std::cout << "[" << elems[0];
    for (size_t i = 1; i < elems.size(); i++) {
        std::cout << ", " << elems[i];
    }
    std::cout << "]\n";
}

int main(int argc, char* argv[]) {
    Detector detector1 = Detector::SNOPlus, detector2 = Detector::IceCube;
    DetectorSignal data1(detector1), data2(detector2);

    std::cout << data2.time_series.size();

    size_t n_bins = 2000;
    scalar period = 20;

    scalar sweep_start = -0.2;
    scalar sweep_end = 0.2;
    size_t n = 1000;
    vec likelihoods(n), offsets(n);

    for (size_t i = 0; i < n; i++) {

        scalar start = std::max(data1.start_time, data2.start_time);
        scalar end = start + period;

        scalar offset = sweep_start + ((sweep_end - sweep_start) * i / n);
        offsets[i] = offset;

        Histogram hist1(n_bins, start, end, data1.time_series), hist2(n_bins, start + offset, end + offset, data2.time_series);

        scalar background_1 = background_rates_ms(detector1), background_2 = background_rates_ms(detector2);

        add_background(hist1, background_1);
        add_background(hist2, background_2);

        scalar ratio = sensitivity_ratio(hist2.get_n_data(), hist1.get_n_data(), period, background_2, background_1);

        // std::cout << hist1.max_bin();
        // std::cout << hist2.max_bin();

        // std::cout << "\n" << hist1.display() << "\n" << hist2.display();

        BinLikelihoodCache cache(
            hist1.max_bin(),
            hist2.max_bin(),
            background_1 / hist1.get_delta(),
            background_2 / hist2.get_delta(),
            1 / (1 + ratio)
        );

        scalar likelihood = cache.log_likelihood(hist1, hist2, n_bins, -5);
        likelihoods[i] = likelihood;

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