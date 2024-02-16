#ifndef CORE_H
#define CORE_H

// For debugging
#include<iostream>

#include<vector>

typedef double scalar;
typedef std::vector<scalar> vec;
typedef std::vector<std::vector<scalar>> mat;

scalar max(vec values);
scalar min(vec values);
size_t index_of_max(vec sequence);

size_t bound_index(size_t index, size_t collection_size);
size_t normalise_index(size_t max_index, scalar index_low_bound);

#endif