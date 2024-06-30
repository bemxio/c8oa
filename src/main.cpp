#include "interpreter.hpp"

int main() {
    Interpreter interpreter;
    uint8_t code[] = {0xFF, 0xFF, 0x00};

    interpreter.run(code, sizeof code);

    return 0;
}