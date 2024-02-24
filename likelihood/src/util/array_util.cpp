#include "util/array_util.hpp"

#include <cmath>
#include <iterator>
#include <algorithm>

scalar min(vec values) {
    return *std::min_element(std::begin(values), std::end(values));
}

scalar max(vec values) {
    return *std::max_element(std::begin(values), std::end(values));
}

size_t index_of_max(vec sequence) {
    return std::distance(sequence.begin(), std::max_element(std::begin(sequence), std::end(sequence)));
}

size_t bound_index(size_t index, size_t collection_size) {
    return std::min(collection_size - 1, std::max(index, (size_t) 0));
}