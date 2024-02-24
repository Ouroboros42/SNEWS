#ifndef WRITE_OUT_H
#define WRITE_OUT_H

#include "core.hpp"
#include "data_io/histogram.hpp"

#include <string>

// Store a calculated likelihood distribution and associated information in specified filename
void save_likelihoods(
    std::string output_filename,
    vec time_diffs, vec likelihoods,
    Histogram dist1, std::vector<Histogram> dist2s,
    scalar window_width, scalar true_time_difference
);

#endif