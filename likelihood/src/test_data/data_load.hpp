#ifndef DATA_LOAD_H
#define DATA_LOAD_H

#include "core.hpp"
#include "data_io/histogram.hpp"
#include "detector_info/detectors.hpp"
#include "data_io/read_signal.hpp"
#include "full_calc.hpp"

#include <json.hpp>

#include <vector>
#include <string>

/* Get the file path of the simulated signal for 27 solar masses, for detector, trial number specified by file_id */
path test_data_file(Detector detector, std::string file_id);

/* A time-series with the true arrival time known, and methods to add background events */
struct TestSignal : public TimeSeries {
    scalar true_time;

    /* Load time-series and true time from a JSON object in nlog-dump format, for specified detector */
    TestSignal(Json::Value generated_signal, std::string detector_name);

    /* Load the 27 solar signal output for detector, trial number specified by file_id */
    TestSignal(Detector detector, path signal_file);

    /* Remove all times outside of set time range */
    void filter_times();

    /* Adjust time range, cutting data outside range */
    void reframe(scalar new_start, scalar new_stop);

    /* Add a fixed number of background events, distributed uniformly over the time range */
    void add_background(size_t count);

    /* Add background events at a uniform rate over the time range, return number of events added */
    size_t add_background(scalar rate);

    /* Add background events accoring to a Poisson distrbution, return number of events added */
    size_t add_poisson_background(scalar rate);
};

/* Add random background events to the bins of hist at the given rate*/
size_t add_background(Histogram& hist, scalar rate);

/*
Setup an analysis of the simulated signals found at the specified locations
For detector_1 being the larger detector
Trial number specified by file_id
Start specifies how much to extend both signals (with background) before the true events (positive means widen the interval)
Window size specifies how much of both event should be included
Min and max offset specify bounds of time differences that can be tested (min will likely be negative) 
If vary_background is true, total number of background events will follow a poisson distribution, otherwise it will be constant
Signal one will be binned in a histogram of specified bin_width
*/
std::pair<SignalAnalysis, scalar> complete_test_case(
    Detector detector_1, Detector detector_2,
    path signal_file_1, path signal_file_2,
    scalar start_buffer, scalar window_size,
    scalar min_offset, scalar max_offset,
    bool vary_background,
    scalar bin_width
);

/*
DEPRECATED
Setup an analysis of the simulated signal for 27 solar masses
For detector_1 being the larger detector
Trial number specified by file_id
Start specifies how much to extend both signals (with background) before the true events (positive means widen the interval)
Window size specifies how much of both event should be included
Min and max offset specify bounds of time differences that can be tested (min will likely be negative) 
If vary_background is true, total number of background events will follow a poisson distribution, otherwise it will be constant
Signal one will be binned in a histogram of specified bin_width
*/
std::pair<SignalAnalysis, scalar> complete_test_case(
    Detector detector_1, Detector detector_2, std::string file_id,
    scalar start_buffer, scalar window_size,
    scalar min_offset, scalar max_offset,
    bool vary_background,
    scalar bin_width
);

#endif