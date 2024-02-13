#include "write_output.hpp"

#include <string>
#include <fstream>


std::string string_vec(vec elems) {
    if (!elems.size()) return "[]";

    std::string out;

    out += "[" + std::to_string(elems[0]);
    for (size_t i = 1; i < elems.size(); i++) {
        out +=  ", " + std::to_string(elems[i]);
    }
    return out  + "]";
}

void save_likelihoods(std::string output_filename, vec time_diffs, vec likelihoods, Histogram dist1, std::vector<Histogram> dist2s) {
    std::ofstream output_file;
    output_file.open(output_filename);
    output_file << "{";
    output_file << "\"Time-Difference\":\n    " << string_vec(time_diffs) << "\n,\"Likelihood\":\n   " << string_vec(likelihoods) << ",\n";
    output_file << "\"Histogram1\": \n" << dist1.display() << ",\n";
    output_file << "\"Histograms2\": \n[\n";
    for (size_t i = 0; i < dist2s.size(); i++) { 
        if (i != 0) {
            output_file << ",\n";
        }
        output_file << dist2s[i].display();
    }
    output_file << "\n]\n";
    output_file << "\n}"; 
    output_file.close();
}
