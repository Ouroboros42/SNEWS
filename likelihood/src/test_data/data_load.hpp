#ifndef DATA_LOAD_H
#define DATA_LOAD_H

#include "core.hpp"
#include "histogram.hpp"
#include "detector_info/detectors.hpp"
#include "data_io/read_signal.hpp"

#include <json.hpp>

#include <vector>
#include <string>

std::string test_data_path(Detector detector, std::string file_id);

Json::Value read_json_file(std::string path);

struct TestSignal : public TimeSeries {
    scalar true_time;

    TestSignal(Json::Value generated_signal, std::string detector_name);

    TestSignal(Detector detector, std::string file_id);
   
    /* Remove all times outside of range */
    void filter_times();

    /* Adjust bounds, cutting data outside range */
    void reframe(scalar new_start, scalar new_stop);

    /* Add a random background events in the range start to stop */
    size_t add_background(scalar rate);
};

/* Add random background events to bins at the given rate*/
size_t add_background(Histogram& hist, scalar rate);

#endif