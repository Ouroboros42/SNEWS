#include "timestamp.hpp"

#include <iomanip>
#include <sstream>

std::string get_timestamp() {
    auto t = std::time(nullptr);
    auto loc_t = *std::localtime(&t);
    auto timestamp = std::put_time(&loc_t, "%d-%m-%Y_%H-%M-%S");
    std::ostringstream strstr;
    strstr << timestamp;
    return strstr.str();
}