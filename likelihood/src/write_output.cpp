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

void save_likelihoods(std::string output_filename, vec time_diffs, vec likelihoods) {
    std::ofstream output_file;
    output_file.open(output_filename);
    output_file << "{ \"Time-Difference\": " << string_vec(time_diffs) << "," <<
    "\"Likelihood\": " << string_vec(likelihoods) << "}"; 
    output_file.close();
}
