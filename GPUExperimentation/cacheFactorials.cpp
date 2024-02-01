#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>

class LogVector {
private:
    int max_value;
    std::vector<double> LogNumberVector;
    std::vector<double> LogFactorialVector;

public:
    LogVector(int max) {
        max_value = max;
        LogNumberVector.resize(max_value);
        LogFactorialVector.resize(max_value);
    }

    void createLogNumberVector() {
        for (int i = 1; i <= max_value; ++i) {
            LogNumberVector[i - 1] = std::log(i);
        }
    }

    void createLogFactorialVector() {
        double prefix_sum = 0.0;
        for (int i = 1; i <= max_value; ++i) {
            prefix_sum += LogNumberVector[i - 1];
            LogFactorialVector[i - 1] = prefix_sum;
        }
    }

    std::string verifyFactorials(double precision) {
        int i = 1;
        try {
            for (; i <= max_value; ++i) {
                if (std::log(std::tgamma(i + 1)) - LogFactorialVector[i - 1] > precision) {
                    std::cout << std::log(std::tgamma(i + 1))
                              << LogFactorialVector[i - 1]
                              << std::endl;
                    return "False at " + std::to_string(i);
                }
            }
        }
        catch (std::overflow_error&) {
            return "OverflowError after " + std::to_string(i);
        }
        return "True";
    }

    void printFactorialVector() {
        for (int i = 1; i <= 100; ++i) {
            std::cout << i << " " << std::exp(LogFactorialVector[i - 1]) << std::endl;
        }
    }
};

int main() {
    int max_value = 3000;
    LogVector ob(max_value);

    ob.createLogNumberVector();

    // Benchmark generation time (just because we can...)
    auto start = std::chrono::steady_clock::now();
    ob.createLogFactorialVector();
    auto end = std::chrono::steady_clock::now();

    // Just for testing to see the factorials are currently generated upto a certain precision
    double precision = 0.001;
    std::cout << ob.verifyFactorials(precision) << std::endl;

    std::cout << "Time taken to create LogFactorialVector: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms" << std::endl;

    return 0;
}
