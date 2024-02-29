#ifndef ARRAY_UTIL_H
#define ARRAY_UTIL_H

#include "core.hpp"

#include <map>

template <typename T>
bool contains(std::vector<T> collection, T element) {
    return std::find(collection.begin(), collection.end(), element) != collection.end();
};

template <typename K, typename V>
std::pair<std::vector<K>, std::vector<V>> unzip(std::map<K, V> map) {
    std::vector<K> keys;
    std::vector<V> values;

    size_t map_size = map.size();
    keys.reserve(map_size);
    values.reserve(map_size);

    for (auto const& [key, value] : map) {
        keys.push_back(key);
        values.push_back(value);
    }

    return { keys, values };
}

/* Return maximum value in vector */
scalar max(vec values);

/* Return minimum value in vector*/
scalar min(vec values);

/* Return index of maximum value in vector */
size_t index_of_max(vec sequence);

/* Return index, clamped to be in range [0, collection_size)*/
size_t bound_index(size_t index, size_t collection_size);

#endif