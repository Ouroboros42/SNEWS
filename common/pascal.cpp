#include "common.hpp"

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