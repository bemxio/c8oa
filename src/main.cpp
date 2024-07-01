#include "interpreter.hpp"

#include <iostream>
#include <fstream>

#include <cstdint>
#include <cstddef>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path to ROM>" << std::endl; return 1;
    }

    std::ifstream file(argv[1], std::ifstream::out | std::ifstream::binary);
    size_t size;

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << argv[1] << std::endl; return 1;
    }

    file.seekg(0, file.end);
    size = file.tellg();
    file.seekg(0, file.beg);

    Interpreter interpreter;
    uint8_t code[size];

    file.read(reinterpret_cast<char*>(code), size);
    interpreter.run(code, size);

    return 0;
}