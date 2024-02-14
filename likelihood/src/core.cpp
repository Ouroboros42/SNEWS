#include "core.hpp"

#include <iterator>
#include <algorithm>

scalar min(vec values) {
    return *std::min_element(std::begin(values), std::end(values));
}

scalar max(vec values) {
    return *std::max_element(std::begin(values), std::end(values));
}