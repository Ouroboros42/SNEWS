//#include "core.hpp"
//#include "test_data/data_load.hpp"
//#include "converging.hpp"
//#include "data_io/write_output.hpp"
//#include "caching/factorials.hpp"
//#include "detector_info/relation.hpp"
//#include "sum_terms.hpp"
//#include "data_io/timestamp.hpp"
//
//#include <iostream>
//#include <iomanip>
//#include <iterator>
//#include <algorithm>
//#include <random>
//#include <chrono>
//#include <fstream>
//
//
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wc++17-extensions"
//template <typename T>
//bool contains(std::vector<T> collection, T element) {
//    return std::find(collection.begin(), collection.end(), element) != collection.end();
//}
//
//template <typename T>
//Json::Value json_array(std::vector<T> arr) {
//    Json::Value arr_out(Json::arrayValue);
//    for (T elem : arr) {
//        arr_out.append(elem);
//    }
//    return arr_out;
//}
//
//template <typename T1>
//Json::Value json_2D_array(std::vector<T1> arr2D) {
//    Json::Value arr2D_out(Json::arrayValue);
//    for (T1 arr1D : arr2D) {
//        arr2D_out.append(json_array(arr1D));
//    }
//    return arr2D_out;
//}
//
//
//std::tuple<scalar, scalar, scalar, scalar> findMaxAndMinLikelihood(std::map<scalar, scalar> Likelihoods, scalar start, scalar end) {
//    // set max and min to the closest key to start value
//    scalar max = Likelihoods[start];
//    scalar min = Likelihoods[start];
//    scalar max_offset = start;
//    scalar min_offset = start;
//
//    for (auto const& [key, val] : Likelihoods) {
//        if(key > start and key < end) {
//            if (val > max) {
//                max = val;
//                max_offset = key;
//            }
//            if (val < min) {
//                min = val;
//                min_offset = key;
//            }
//        }
//    }
//    return std::make_tuple(max, max_offset, min, min_offset);
//}
//
//
//std::tuple<scalar, scalar> findNewMeshToSearch(std::map<scalar, scalar> Likelihoods, scalar threshold) {
//    scalar max_offset_left = 0;
//    scalar max_offset_right = 0;
//    for (auto const& [key, val] : Likelihoods) {
//        if (val > threshold) {
//            max_offset_left = key;
//            break;
//        }
//    }
//    for (auto it = Likelihoods.rbegin(); it != Likelihoods.rend(); ++it) {
//        if (it->second > threshold) {
//            max_offset_right = it->first;
//            break;
//        }
//    }
//    return std::make_tuple(max_offset_left, max_offset_right);
//}
//
//
//std::map<scalar, scalar> doLikelihoodsWithOptimisedMesh(
//    TestSignal signal_1,
//    TestSignal signal_2,
//    scalar background_1,
//    scalar background_2,
//    FactorialCache cache,
//    scalar rel_accuracy,
//    size_t n_bins,
//    scalar sweep_start,
//    scalar sweep_end,
//    scalar front_buffer,
//    scalar back_buffer,
//    int print_every_x_iterations
//        )
//{
//    // object to be returned
//    std::map<scalar, scalar> Likelihoods;
//
//    // rezero times
//    scalar zero_time = std::min(signal_1.start, signal_2.start);
//    signal_1.rezero_times(zero_time);
//    signal_2.rezero_times(zero_time);
//
//    // build windows
//    scalar window_start = std::min(signal_1.start, signal_2.start) + sweep_start - front_buffer;
//    scalar window_end = std::max(signal_1.stop, signal_2.stop) + sweep_end + back_buffer;
//
//    scalar window_width = window_end - window_start;
//
//    // Widest range over which detector 2 signal will be binned
//    scalar window_start_min = window_start + sweep_start;
//    scalar window_end_max = window_end + sweep_end;
//
//    // Simulate uniform background rate across both datasets
//    signal_1.reframe(window_start, window_end);
//    signal_1.add_background(background_1);
//    signal_2.reframe(window_start_min, window_end_max);
//    signal_2.add_background(background_2);
//
//    // Signal 1 is binned only once, as only signal 2 is offset each iteration
//    Histogram hist_1 = signal_1.to_hist(n_bins);
//
//    // ------------------- Begin Mesh -------------------
//
//    size_t n_steps = 100;
//    bool accuracyAchieved = false;
//
//    while(Likelihoods.size() < 300) {
//        scalar increment = (sweep_end - sweep_start) / n_steps;
//
//        // calculate Likelihoods
//        for(scalar o = sweep_start; o < sweep_end; o += increment) {
//            if (Likelihoods.count(o) == 0) {
//                // rebin
//                Histogram hist_2 = signal_2.to_hist(n_bins, window_start + o, window_end + o);
//
//                // calculate Likelihood
//                DetectorRelation detectors(background_1, background_2, hist_1, hist_2);
//                Likelihoods[o] = log_likelihood(cache, detectors, hist_1, hist_2, rel_accuracy);
//            }
//        }
//
//        // print current size
//        std::cout << "Size after calculation: " << Likelihoods.size() << std::endl;
//
//        // find max and min Likelihoods
//        auto [max, max_offset, min, min_offset] =
//                findMaxAndMinLikelihood(Likelihoods, sweep_start, sweep_end);
//
//        // find new mesh to search
//        scalar threshold = (max + min) / 2;
//        auto [a, b] = findNewMeshToSearch(Likelihoods, threshold);
//
//        std::cout << "New mesh to search: " << a << " " << b << std::endl;
//
//        // if certain conditions are is achieved, break
//        bool accuracyAchieved = (max - min < 0.5) or (std::abs(sweep_start - a) < 0.001) or (std::abs(sweep_end - b) < 0.001);
//        if(accuracyAchieved) {
//            std::cout << "Accuracy achieved!" << std::endl;
//            accuracyAchieved = true;
//            break;
//        }
//
//
//        sweep_start = a;
//        sweep_end = b;
//        n_steps = 50;
//    }
//
//    std::cout << "Size: " << Likelihoods.size() << std::endl;
//    return Likelihoods;
//
//}
//
//
//int main(int argc, char **argv) {
//
//    std::string inst = "121";
//
//    // Create the variables needed. Let the background be generated in the Likelihood calculation
//    // otherwise we are essentially doing the same analysis
//
//    Detector detector1 = Detector::SNOPlus, detector2 = Detector::SuperK;
//    TestSignal signal_1(detector1, inst), signal_2(detector2, inst);
//    scalar background_1 = background_rate_s(detector1);
//    scalar background_2 = background_rate_s(detector2);
//
//    scalar sweep_start = -0.1;
//    scalar sweep_end = 0.1;
//    scalar log_accuracy = 1E-6;
//    size_t data_into_n_bins = 10000;
//    scalar front_buffer = 1;
//    scalar back_buffer = 1;
//    int print_every_x_iterations = 10;
//
//    FactorialCache cache;
//
//    // store results
//    std::string for_which_detectors = detector_name(detector1) +"vs"+ detector_name(detector2) + "_";
//    std::string sweep_range = std::to_string(sweep_start - sweep_end);
//    std::string output_filename = "src\\analysis\\Trials/1000_runs_Sweep_Range_" + sweep_range
//             + for_which_detectors + get_timestamp() + "_ID_" + inst + ".json";
//
//    // prepare output file
//    std::ofstream outfile;
//    outfile.open(output_filename);
//    Json::StreamWriterBuilder builder;
//    Json::Value outputs;
//
//    // write actual answer and parameters to file
//    outputs["detector1"] = detector_name(detector1);
//    outputs["detector2"] = detector_name(detector2);
//    outputs["True-Time-Diff"] = signal_2.true_time - signal_1.true_time;
//
//    for (size_t trial_number = 0; trial_number < 100; trial_number++) {
//
//        printf("Trial number %zu\n", trial_number);
//        auto T1 = std::chrono::high_resolution_clock::now();
//        std::map<scalar, scalar> samples = doLikelihoodsWithOptimisedMesh(
//            signal_1,
//            signal_2,
//            background_1,
//            background_2,
//            cache,
//            log_accuracy,
//            data_into_n_bins,
//            sweep_start,
//            sweep_end,
//            front_buffer,
//            back_buffer,
//            print_every_x_iterations
//
//        );
//        auto T2 = std::chrono::high_resolution_clock::now();
//        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(T2 - T1).count();
//        printf("Time taken = %lld ms\n", duration);
//
//        // unpack results to vectors
//        std::vector<scalar> L, T;
//        for (auto const& [key, val] : samples) {
//            T.push_back(key);
//            L.push_back(val);
//        }
//
//        outputs[std::to_string(trial_number)]["Likelihoods"] = json_array(L);
//        outputs[std::to_string(trial_number)]["Offsets"] = json_array(T);
//    }
//
//    std::string output_string = Json::writeString(builder, outputs);
//    outfile << output_string;
//    outfile.close();
//    std::cout << "Output written to " << output_filename << std::endl;
//}
//
//
//
//
//
//#pragma clang diagnostic pop