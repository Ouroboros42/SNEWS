#include "core.hpp"
#include "test_data/data_load.hpp"
#include "converging.hpp"
#include "data_io/write_output.hpp"
#include "caching/factorials.hpp"
#include "detector_info/relation.hpp"
#include "sum_terms.hpp"
#include "data_io/timestamp.hpp"

#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>


template <typename T>
bool contains(std::vector<T> collection, T element) {
    return std::find(collection.begin(), collection.end(), element) != collection.end();
}

template <typename T>
Json::Value json_array(std::vector<T> arr) {
    Json::Value arr_out(Json::arrayValue);
    for (T elem : arr) {
        arr_out.append(elem);
    }
    return arr_out;
}

template <typename T1>
Json::Value json_2D_array(std::vector<T1> arr2D) {
    Json::Value arr2D_out(Json::arrayValue);
    for (T1 arr1D : arr2D) {
        arr2D_out.append(json_array(arr1D));
    }
    return arr2D_out;
}


std::vector<std::vector<double>> doLikelihoodCalculation(
    TestSignal signal_1,
    TestSignal signal_2,
    scalar background_1,
    scalar background_2,
    FactorialCache cache,
    scalar rel_accuracy,
    size_t n_bins,
    size_t n_steps,
    scalar sweep_start,
    scalar sweep_end,
    scalar front_buffer,
    scalar back_buffer,
    int print_every_x_iterations
        )
{
    // object to be returned
    std::vector<std::vector<double>> returnVector(2);

    // rezero times
    scalar zero_time = std::min(signal_1.start, signal_2.start);
    signal_1.rezero_times(zero_time);
    signal_2.rezero_times(zero_time);

    // build windows
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

    // Signal 1 is binned only once, as only signal 2 is offset each iteration
    Histogram hist_1 = signal_1.to_hist(n_bins);

    vec L(n_steps), T(n_steps);

    for (size_t i = 0; i < n_steps; i++) {

        scalar offset = sweep_start + ((sweep_end - sweep_start) * i / n_steps);
        Histogram hist_2 = signal_2.to_hist(n_bins, window_start + offset, window_end + offset);

        DetectorRelation detectors(background_1, background_2, hist_1, hist_2);

        L[i] = log_likelihood(cache, detectors, hist_1, hist_2, rel_accuracy);
        T[i] = offset;

    if (i % print_every_x_iterations == 0) {
            std::cout << i << " / " << n_steps << " done" << std::endl;
        }

    }

    returnVector[0] = L;
    returnVector[1] = T;

    return returnVector;

}


int main(int argc, char **argv) {

    std::string inst = "121";

    // Create the variables needed. Let the background be generated in the Likelihood calculation
    // otherwise we are essentially doing the same analysis

    Detector detector1 = Detector::SNOPlus, detector2 = Detector::SuperK;
    TestSignal signal_1(detector1, inst), signal_2(detector2, inst);
    scalar background_1 = background_rate_s(detector1);
    scalar background_2 = background_rate_s(detector2);

    scalar sweep_start = -0.2;
    scalar sweep_end = 0.2;
    scalar log_accuracy = 1E-6;
    size_t data_into_n_bins = 10000;
    // size_t n_steps = 1000;
    scalar front_buffer = 1;
    scalar back_buffer = 1;
    int print_every_x_iterations = 10;

    FactorialCache cache;

    // store results
    std::string output_filename = "output/dummy" + get_timestamp() + ".json";
    std::ofstream outfile;
    outfile.open(output_filename);
    Json::StreamWriterBuilder builder;
    Json::Value outputs;

    // suppose see the difference in Likelihoods as a function of n_steps. set those parameters
    int n_steps_start = 100;
    int n_steps_end = 1000;
    int n_steps_step = 100;

    // Let the output file know
    outputs["UID"]["Name"] = "n_steps";
    outputs["UID"]["Start"] = n_steps_start;
    outputs["UID"]["End"] = n_steps_end;
    outputs["UID"]["Step"] = n_steps_step;
    outputs["UID"]["True-Time-Difference"] = signal_2.true_time - signal_1.true_time;

    for (size_t n_steps = n_steps_start; n_steps < n_steps_end; n_steps = n_steps + n_steps_step) {

        printf("n_steps = %zu\n", n_steps);

        auto T1 = std::chrono::high_resolution_clock::now();
        std::vector<std::vector<double>> samples = doLikelihoodCalculation(
            signal_1,
            signal_2,
            background_1,
            background_2,
            cache,
            log_accuracy,
            data_into_n_bins,
            n_steps,
            sweep_start,
            sweep_end,
            front_buffer,
            back_buffer,
            print_every_x_iterations

        );
        auto T2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(T2 - T1).count();
        printf("Time taken = %lld ms\n", duration);

        // write current results to file
        outputs[std::to_string(n_steps)]["Likelihoods"] = json_array(samples[0]);
        outputs[std::to_string(n_steps)]["Offsets"] = json_array(samples[1]);
    }

    std::string output_string = Json::writeString(builder, outputs);
    outfile << output_string;
    outfile.close();
}




