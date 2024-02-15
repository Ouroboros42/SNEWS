#include "core.hpp"

#include <string>

enum Detector {
    IceCube,
    SuperK,
    SNOPlus,
};

std::string detector_name(Detector detector);

scalar background_rates_ms(Detector detector);
