#ifndef INFERENCE_H
#define INFERENCE_H

#include "core.hpp"

scalar sensitivity_ratio(size_t n_top, size_t n_bottom, scalar period, scalar background_top, scalar background_bottom);

#endif