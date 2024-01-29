#include "common.hpp"

PascalTriangle::PascalTriangle() = default;
PascalTriangle::PascalTriangle(int size) : PascalTriangle::PascalTriangle() { expand(size); }

size_t PascalTriangle::getSize() {
    return combinations.size();
}

void PascalTriangle::expand(int newSize) {
    
}