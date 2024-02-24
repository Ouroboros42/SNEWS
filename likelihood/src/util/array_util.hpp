#ifndef ARRAY_UTIL_H
#define ARRAY_UTIL_H

#include "core.hpp"

template <typename T>
bool contains(std::vector<T> collection, T element) {
    return std::find(collection.begin(), collection.end(), element) != collection.end();
};

/* Return maximum value in vector */
scalar max(vec values);

/* Return minimum value in vector*/
scalar min(vec values);

/* Return index of maximum value in vector */
size_t index_of_max(vec sequence);

/* Return index, clamped to be in range [0, collection_size)*/
size_t bound_index(size_t index, size_t collection_size);

#endif