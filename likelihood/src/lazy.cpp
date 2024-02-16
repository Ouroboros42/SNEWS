#include "lazy.hpp"

LazyArray::~LazyArray() {}

Lazy2DArray::~Lazy2DArray() {}

LazySubArray::LazySubArray(const Lazy2DArray& source, size_t row, size_t start_index, size_t end_index) : 
    source(source), row(row),
    direction(end_index >= start_index),
    start_index(start_index),
    _size((start_index < 0 || start_index >= source.size_y()) ? 0 :
        direction ?
            (1 + end_index - start_index) :
            (1 + start_index - end_index)
    )
{}

size_t LazySubArray::size() const { return _size; }

scalar LazySubArray::get(size_t i) const {
    return source.get(row, direction ? (start_index + i) : (start_index - i));
}