#ifndef DETECTORS_H
#define DETECTORS_H

#include "core.hpp"

#include <string>

enum Detector {
    IceCube,
    SuperK,
    SNOPlus,
    DarkSide
};

std::string detector_name(Detector detector);

scalar background_rate_ms(Detector detector);
scalar background_rate_s(Detector detector);

#endif