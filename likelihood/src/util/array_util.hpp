#ifndef ARRAY_UTIL_H
#define ARRAY_UTIL_H

#include "core.hpp"

template <typename T>
bool contains(std::vector<T> collection, T element) {
    return std::find(collection.begin(), collection.end(), element) != collection.end();
};

scalar max(vec values);

scalar min(vec values);

size_t index_of_max(vec sequence);

size_t bound_index(size_t index, size_t collection_size);

#endif