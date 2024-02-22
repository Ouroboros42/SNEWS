#ifndef LAZY_H
#define LAZY_H

#include "core.hpp"

class LazyArray {
public:
    virtual ~LazyArray();

    virtual size_t size() const = 0;

    virtual scalar get(size_t i) const = 0;
};

class Lazy2DArray {
public:
    virtual ~Lazy2DArray();

    virtual size_t size_x() const = 0;
    virtual size_t size_y() const = 0;

    virtual scalar get(size_t i_x, size_t i_y) const = 0;
};

class LazySubArray : public LazyArray {
private:
    const Lazy2DArray& source;

    size_t row;

    size_t start_index;

    // true: increasing indices, false: decreasing indices
    bool direction;

public:
    /* Array accessing elements in row of source, from start_index (inclusive) to the edge of the array
    If start_index out of bounds, has size 0
    */
    LazySubArray(const Lazy2DArray& source, size_t row, size_t start_index, bool direction);

    bool is_empty() const;
    
    size_t size() const;

    scalar get(size_t i) const;
};

#endif