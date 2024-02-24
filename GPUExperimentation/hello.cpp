#include <iostream>
#include <map>
#include <cmath>
#include <vector>


std::tuple<double, double, double, double> findMaxAndMinLikelihood(std::map<double, double> Likelihoods, double start, double end) {
    // set max and min to the closest key to start value
    double max = Likelihoods[start];
    double min = Likelihoods[start];
    double max_offset = start;
    double min_offset = start;

    for (auto const& [key, val] : Likelihoods) {
        if(key > start and key < end) {
            if (val > max) {
                max = val;
                max_offset = key;
            }
            if (val < min) {
                min = val;
                min_offset = key;
            }
        }
    }
    return std::make_tuple(max, max_offset, min, min_offset);
}




std::tuple<double, double> findNewMeshToSearch(std::map<double, double> Likelihoods, double threshold) {
    double max_offset_left = 0;
    double max_offset_right = 0;
    for (auto const& [key, val] : Likelihoods) {
        if (val > threshold) {
            max_offset_left = key;
            break;
        }
    }
    for (auto it = Likelihoods.rbegin(); it != Likelihoods.rend(); ++it) {
        if (it->second > threshold) {
            max_offset_right = it->first;
            break;
        }
    }
    return std::make_tuple(max_offset_left, max_offset_right);
}


int main() {

    std::map<double, double> Likelihoods;

    double start = -0.1;
    double end = 0.1;
    int n_steps = 100;
    double step = (end - start) / n_steps;

    bool accuracyAchieved = false;

    while(Likelihoods.size() < 300) {
        double increment = (end - start) / n_steps;

        // calculate Likelihoods
        for(double o = start; o < end; o += increment) {
            if (Likelihoods.count(o) == 0) {
                Likelihoods[o] = std::cos(o);
            }
        }

        // size after calculation
        std::cout << "Size after calculation: " << Likelihoods.size() << std::endl;
        std::cout << start << " " << end << std::endl;


        // find max and min Likelihoods
        auto [max, max_offset, min, min_offset] = findMaxAndMinLikelihood(Likelihoods, start, end);
        std::cout << "Max: " << max << " at " << max_offset << std::endl;
        std::cout << "Min: " << min << " at " << min_offset << std::endl;


        // if certain accuracy is achieved, break
        if(max - min < 0.001) {
            std::cout << "Accuracy achieved!" << std::endl;
            std::cout << "Max: " << max << std::endl;
            std::cout << "Min: " << min << std::endl;
            accuracyAchieved = true;
            break;
        }

        // find new mesh to search
        double threshold = (max + min) / 2;
        auto [a, b] = findNewMeshToSearch(Likelihoods, threshold);

        std::cout << "New mesh to search: " << a << " " << b << std::endl;

        start = a;
        end = b;
        n_steps = 50;
    }

    std::cout << "Size: " << Likelihoods.size() << std::endl;
}
