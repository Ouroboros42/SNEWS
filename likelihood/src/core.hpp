#ifndef CORE_H
#define CORE_H

// For debugging
#include <iostream>
#include <chrono>

#include <vector>
#include <filesystem>

/* Type for all scalars - can be changed to adjust speed vs precision */
typedef double scalar;
typedef std::vector<scalar> vec;
typedef std::vector<std::vector<scalar>> mat;

typedef std::filesystem::path path;

inline std::chrono::high_resolution_clock::time_point mark_time() {
    return std::chrono::high_resolution_clock::now();
}

typedef std::chrono::seconds sec;
typedef std::chrono::milliseconds msec;

#define TIME_BETWEEN(units, start, end) std::chrono::duration_cast<units>(end - start).count()

#endif