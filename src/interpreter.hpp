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

        void bcd();

        void bitop_or();
        void bitop_and();
        void bitop_xor();
        void bitop_shift_left();
        void bitop_shift_right();

        void cond_const_equals();
        void cond_const_not_equal();
        void cond_register_equals();
        void cond_register_not_equal();

        void const_set();
        void const_add();

        void display_clear();
        void display_draw();

        void flow_return();
        void flow_jump();
        void flow_call();
        void flow_jump_offset();

        void keyop_pressed();
        void keyop_not_pressed();
        void keyop_get();

        void math_set();
        void math_add();
        void math_sub_vx();
        void math_sub_vy();

        void mem_set();
        void mem_add();
        void mem_font_get();
        void mem_reg_dump();
        void mem_reg_load();

        void rand();

        void sound_set();

        void timer_set();
        void timer_get();
};

#endif