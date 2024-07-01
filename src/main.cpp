#include "interpreter.hpp"

int main() {
    Interpreter interpreter;

    uint8_t code[2048] = {0x00};
    std::fill(code + 0x200, code + 0x400, 0xFF);

    interpreter.run(code, sizeof code);

    return 0;
}