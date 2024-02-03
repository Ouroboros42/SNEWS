#include "pascal.hpp"

#include<iostream>

mat pascal_rhombus(size_t n_rows, size_t n_cols) {
    mat rhombus(n_rows, vec(n_cols, 1)); // Initialise nxm array of 1s

    for (size_t y = 1; y < n_rows; y++) {
        for (size_t x = 1; x < n_cols; x++) {
            rhombus[y][x] = rhombus[y-1][x] + rhombus[y][x-1]; // Populate with recursion relation
        }
    }
    
    return rhombus;
}

//
// Created by vansh on 29-01-2024.
//

mat scaled_rhombus(size_t n_rows, size_t n_cols, scalar row_mul, scalar col_mul) {
    mat rhombus(n_rows, vec(n_cols)); // Initialise nxm array

    scalar running_power = 1;
    
    for (size_t y = 0; y < n_rows; y++) {
        rhombus[y][0] = running_power;
        running_power *= row_mul;
    }

    running_power = 1;

    for (size_t x = 0; x < n_cols; x++) {
        rhombus[0][x] = running_power;
        running_power *= col_mul;
    }

    for (size_t y = 1; y < n_rows; y++) {
        for (size_t x = 1; x < n_cols; x++) {
            rhombus[y][x] = row_mul * rhombus[y-1][x] + col_mul * rhombus[y][x-1]; // Populate with recursion relation
        }
    }
    
    return rhombus;
}