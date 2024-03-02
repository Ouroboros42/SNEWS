#ifndef CORE_H
#define CORE_H

// For debugging
#include <iostream>

#include <vector>
#include <filesystem>

/* Type for all scalars - can be changed to adjust speed vs precision */
typedef double scalar;
typedef std::vector<scalar> vec;
typedef std::vector<std::vector<scalar>> mat;

typedef std::filesystem::path path;

#endif