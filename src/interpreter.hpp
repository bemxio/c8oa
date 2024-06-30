#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <SDL2/SDL.h>

#include <iostream>
#include <algorithm>
#include <vector>

#include <cstdint>

class Interpreter {
    public:
        Interpreter();
        ~Interpreter();

        void run(uint8_t* code, uint16_t length);

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Event event;

        uint8_t memory[4096] = {0x00};

        uint8_t v[16] = {0x00}; // registers V0 - VF
        uint16_t I = 0x0000; // I register

        std::vector<uint16_t> s; // stack

        uint8_t dt = 0x00; // delay timer
        uint8_t st = 0x00; // sound timer

        uint16_t pc = 0x0000; // program counter (originally 0x0200, set to 0x0000 for testing)

        uint16_t opcode_address() {
            return (memory[pc] & 0x0F) << 8 | memory[pc + 1];
        }

        void display_clear();

        void flow_return();
        void flow_jump(bool call = false);
};

#endif