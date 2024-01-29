#include <iostream>
#include <vector>
#include <cmath>

std::vector<std::vector<int>> makeTriangle(int m, int n) {
    // Make array of 1s
    std::vector<std::vector<int>> p(m + n, std::vector<int>(n, 1));
    for (int i = 1; i < m + n; ++i) {
        for (int j = 1; j < n; ++j) {
            p[i][j] = p[i - 1][j] + p[i][j - 1];
        }
    }
    return p;
}

void check(const std::vector<std::vector<int>>& p, int m, int n) {
    for (int i = 0; i < m + n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (std::tgamma(i + j + 1) / (std::tgamma(j + 1) * std::tgamma(i + 1)) == p[i][j]) {
                std::cout << "True" << std::endl;
            } else {
                std::cout << "False" << std::endl;
                std::cout << i << " " << j << std::endl;
                std::cout << std::tgamma(i + j + 1) / (std::tgamma(j + 1) * std::tgamma(i + 1)) << " " << p[i][j] << std::endl;
            }
        }
    }
}

int main() {
    int m = 10;
    int n = 5;

    std::vector<std::vector<int>> p = makeTriangle(m, n);

    for (int i = 0; i < m + n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << p[i][j] << " ";
        }
        std::cout << std::endl;
    }

    check(p, m, n);

    return 0;
}
//
// Created by vansh on 29-01-2024.
//
