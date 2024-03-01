#include "data_load.hpp"
#include "core.hpp"
#include "random.hpp"

#include <string>
#include <iostream>
#include <algorithm>

std::string test_data_path(Detector detector, std::string file_id) {
    return "../test-data/nlog-dump-" + detector_name(detector) + "-json-" + file_id + "-0.json";
}

TestSignal::TestSignal(Json::Value data, std::string detector_name) : TimeSeries(parse_json_array<scalar>(data["timeseries"]["times"])),
    true_time(data["truth"]["dets"][detector_name]["true_t"].asDouble())
{}

TestSignal::TestSignal(Detector detector, std::string file_id) : TestSignal(read_json_file(test_data_path(detector, file_id)), detector_name(detector)) {};

TestSignal::TestSignal(Detector detector, std::string file_id, scalar front_buffer, scalar end_buffer) : TestSignal(detector, file_id) {
    reframe(start - front_buffer, stop + end_buffer);
}

void TestSignal::filter_times() {
    std::remove_if(times.begin(), times.end(), [this](scalar time) { return start <= time && time <= stop; });
}

void TestSignal::reframe(scalar new_start, scalar new_stop) {
    bool needs_filter = new_start > start || new_stop < stop; 
    
    start = new_start;
    stop = new_stop;

    if (needs_filter) filter_times();
}

void TestSignal::add_background(size_t count) {
    times.reserve(times.size() + count);

    for (size_t i = 0; i < count; i++) {
        times.push_back(rand_in_range(start, stop));
    }
}

size_t TestSignal::add_background(scalar rate) {
    size_t n_background = std::llround(rate * range());
    add_background(n_background);
    return n_background;
}

size_t TestSignal::add_poisson_background(scalar rate) {
    size_t n_background = poisson_trial(rate * range());
    add_background(n_background);
    return n_background;
}

size_t add_background(Histogram& hist, scalar background_rate) {
    size_t n_events = background_rate * hist.range();

    for (size_t i = 0; i < n_events; i++) {
        hist.inc_bin(rand_int(hist.size()));
    }

    return n_events;
}

std::pair<SignalAnalysis, scalar> complete_test_case(
    Detector detector_1, Detector detector_2, std::string file_id,
    scalar start_buffer, scalar window_size,
    scalar min_offset, scalar max_offset,
    bool vary_background,
    scalar bin_width
) {
    TestSignal signal_1(detector_1, file_id), signal_2(detector_2, file_id);

    scalar true_lag = signal_2.true_time - signal_1.true_time;

    scalar background_1 = background_rate_s(detector_1);
    scalar background_2 = background_rate_s(detector_2);

    scalar base_window_start = std::min(signal_1.start, signal_2.start) - start_buffer;
    scalar base_window_stop = base_window_start + window_size;

    signal_1.reframe(base_window_start, base_window_stop);

    scalar sweep_window_start = base_window_start + min_offset;
    scalar sweep_window_stop = base_window_stop + max_offset;

    signal_2.reframe(sweep_window_start, sweep_window_stop);

    size_t n_background_1, n_background_2;
    if (vary_background) {
        n_background_1 = signal_1.add_poisson_background(background_1);
        n_background_2 = signal_2.add_poisson_background(background_2);
    } else {
        n_background_1 = signal_1.add_background(background_1);
        n_background_2 = signal_2.add_background(background_2);
    }
    
    std::printf("Generated background events:\n Detector 1: %u\n Detector 2: %u\n", n_background_1, n_background_2);

    Histogram binned_signal_1 = signal_1.to_hist(bin_width);

    SignalAnalysis analysis(binned_signal_1, signal_2, background_1, background_2);

    return { analysis, true_lag };
}
