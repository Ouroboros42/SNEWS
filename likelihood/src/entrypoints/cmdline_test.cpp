#include <tclap/CmdLine.h>

#include <iostream>

int main(int argc, char** argv) {
    try {
        TCLAP::CmdLine cmd("Test command description", ' ', "0.0");

        TCLAP::ValueArg<std::string> egarg("a", "arg", "An exampkle argument", true, "", "string");

        cmd.add(egarg);

        cmd.parse(argc, argv);

        std::string egarg_val = egarg.getValue();

        std::cout << "User said: " << egarg_val << std::endl;
    } catch (TCLAP::ArgException& arg_err) {
        std::cerr << "Error parsing arg: " << arg_err.argId() << "\n" << arg_err.error() << " for arg "  << std::endl;
    }
}