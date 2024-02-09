#ifndef WRITE_OUT_H
#define WRITE_OUT_H

#include "core.hpp"

#include <string>

void save_likelihoods(std::string output_filename, vec time_diffs, vec likelihoods);

#endif