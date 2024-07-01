#include "interpreter.hpp"

#include <cstdint>
#include <cstddef>
#include <cstdio>

#include <fstream>

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path to ROM>\n", argv[0]); return 1;
    }

    std::ifstream file(argv[1], std::ifstream::out | std::ifstream::binary);
    size_t size;

    if (!file.is_open()) {
        fprintf(stderr, "Failed to open file: %s\n", argv[1]); return 1;
    }

    file.seekg(0, file.end);
    size = file.tellg();
    file.seekg(0, file.beg);

    Interpreter interpreter;
    char code[size];

    file.read(code, size);
    file.close();

    interpreter.run(reinterpret_cast<uint8_t*>(code), size);

    return 0;
}