#include "detectors.hpp"

#include <iostream>

std::string detector_name(Detector detector) {
    switch (detector) {
        case Detector::IceCube:
            return "IC";
        case Detector::SNOPlus:
            return "SNOP";
        case Detector::SuperK:
            return "SK";
        case Detector::DarkSide:
            return "DS-20K";
        default:
            throw std::invalid_argument("Undefined detector");
    }
}

scalar background_rate_ms(Detector detector) {
    switch (detector) { 
        case Detector::IceCube:
            return 1500;
        case Detector::SNOPlus:
            return 0.0003;
        case Detector::SuperK:
            return 0.0001;
        case Detector::DarkSide:
            return 0.00025;
        default:
            throw std::invalid_argument("Undefined detector");
    }
}

scalar background_rate_s(Detector detector) {
    return background_rate_ms(detector) * 1000;
}
