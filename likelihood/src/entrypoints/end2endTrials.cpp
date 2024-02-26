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
#include <fstream>

std::tuple<scalar, scalar, scalar, scalar> findMaxAndMinLikelihood(
        std::map<scalar,
        scalar> Likelihoods,
        scalar start,
        scalar end)
{
    // set max and min to the closest key to start value
    scalar max = Likelihoods[start];
    scalar min = Likelihoods[start];
    scalar max_offset = start;
    scalar min_offset = start;

    for (auto const& [key, val] : Likelihoods) {
        if(key > start && key < end) {
            if (val > max) {
                max = val;
                max_offset = key;
            }
            if (val < min) {
                min = val;
                min_offset = key;
            }
        }
    }
    return std::make_tuple(max, max_offset, min, min_offset);
}


std::tuple<scalar, scalar> findNewMeshToSearch(std::map<scalar, scalar> Likelihoods, scalar threshold) {
    scalar max_offset_left = 0;
    scalar max_offset_right = 0;
    for (auto const& [key, val] : Likelihoods) {
        if (val > threshold) {
            max_offset_left = key;
            break;
        }
    }
    for (auto it = Likelihoods.rbegin(); it != Likelihoods.rend(); ++it) {
        if (it->second > threshold) {
            max_offset_right = it->first;
            break;
        }
    }
    return std::make_tuple(max_offset_left, max_offset_right);
}


std::map<scalar, scalar> doLikelihoodsWithOptimisedMesh(
    SignalAnalysis test_case,
    scalar rel_accuracy,
    scalar sweep_start,
    scalar sweep_end
        )
{
    // object to be returned
    std::map<scalar, scalar> Likelihoods;


    size_t n_steps = 100;
    bool accuracyAchieved = false;

    while(Likelihoods.size() < 300) {
        scalar increment = (sweep_end - sweep_start) / n_steps;

        // calculate Likelihoods
        for(scalar o = sweep_start; o < sweep_end; o += increment) {
            if (Likelihoods.count(o) == 0) {
                Likelihoods[o] = test_case.lag_log_likelihood(o, rel_accuracy, false);
            }
        }

        // print current size
        std::cout << "Number of Likelihoods calculated so far: " << Likelihoods.size() << std::endl;

        // find max and min Likelihoods
        auto [max, max_offset, min, min_offset] =
                findMaxAndMinLikelihood(Likelihoods, sweep_start, sweep_end);

        // find new mesh to search
        scalar threshold = (max + min) / 2;
        auto [a, b] = findNewMeshToSearch(Likelihoods, threshold);

        std::cout << "New mesh to search: " << a << " " << b << std::endl;

        // if certain conditions are is achieved, break
        bool accuracyAchieved = (max - min < 0.5) || (std::abs(sweep_start - a) < 0.001)
                || (std::abs(sweep_end - b) < 0.001);
        if(accuracyAchieved) {
            std::cout << "Accuracy achieved!" << std::endl;
            break;
        }

        // update sweeping parameters
        sweep_start = a;
        sweep_end = b;
        n_steps = 50;
    }

    std::cout << "Total number of Likelihoods in this Trial: " << Likelihoods.size() << std::endl;
    return Likelihoods;
}


int main(int argc, char **argv) {

    std::string inst = "222";

    // Create the variables needed. Let the background be generated in the Likelihood calculation
    Detector detector1 = Detector::SNOPlus, detector2 = Detector::SuperK;

    scalar sweep_start = -0.1;
    scalar sweep_end = 0.1;
    scalar rel_accuracy = 1E-3;
    scalar bin_width = 2E-3;
    bool poisson_vary_background = true;

    scalar front_buffer = 1;
    scalar window = 20;

    int NumOfTrials = 2000;

    // generate filename to store results
    std::string detector_names = "_" + detector_name(detector1) +"vs"+ detector_name(detector2) + "_";
    std::string sweep_range = std::to_string(sweep_end - sweep_start);
    std::string numT = std::to_string(NumOfTrials);
    std::string directory = "src\\analysis\\TrialsData/";

    std::string output_filename = directory + numT + "_runs_Sweep_Range_" + sweep_range + "_UID_" + inst +
            detector_names + "___" + get_timestamp() + ".json";

    // prepare output file
    std::ofstream outfile;
    outfile.open(output_filename);
    Json::StreamWriterBuilder builder;
    Json::Value outputs;

    // dummy variable
    scalar TrueTimeDiff = 0;


    for (size_t trial_number = 0; trial_number < NumOfTrials; trial_number++) {
        printf("Trial number %zu\n", trial_number);

        auto [test_case, true_d] = complete_test_case(
                detector1, detector2, inst,
                front_buffer, window,
                sweep_start, sweep_end,
                poisson_vary_background,
                bin_width
        );

        TrueTimeDiff = true_d;

        auto T1 = std::chrono::high_resolution_clock::now();
        std::map<scalar, scalar> samples = doLikelihoodsWithOptimisedMesh(
            test_case,
            rel_accuracy,
            sweep_start,
            sweep_end
        );

        auto T2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(T2 - T1).count();
        printf("Time taken = %lld ms\n", duration);

        // unpack results to vectors
        std::vector<scalar> L, T;
        for (auto const& [key, val] : samples) {
            T.push_back(key);
            L.push_back(val);
        }

        outputs[std::to_string(trial_number)]["Likelihoods"] = json_array(L);
        outputs[std::to_string(trial_number)]["Offsets"] = json_array(T);
    }

    outputs["detector1"] = detector_name(detector1);
    outputs["detector2"] = detector_name(detector2);
    outputs["True-Time-Diff"] = TrueTimeDiff;
    outputs["inst"] = inst;
    outputs["sweep-range"] = sweep_range;
    outputs["num-Trials"] = NumOfTrials;

    std::string output_string = Json::writeString(builder, outputs);
    outfile << output_string;
    outfile.close();
    std::cout << "Output written to " << output_filename << std::endl;
}

