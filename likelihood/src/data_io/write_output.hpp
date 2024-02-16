#ifndef WRITE_OUT_H
#define WRITE_OUT_H

#include "core.hpp"
#include "histogram.hpp"

#include <string>

// Store a calculated likelihood distribution and associated information in 
void save_likelihoods(
    std::string output_filename,
    vec time_diffs, vec likelihoods,
    Histogram dist1, std::vector<Histogram> dist2s,
    scalar window_width
);

#endif