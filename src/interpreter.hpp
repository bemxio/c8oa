#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <SDL2/SDL.h>

#include <algorithm>
#include <vector>

#include <cstdint>
#include <cstdio>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320

class Interpreter {
    public:
        Interpreter();
        ~Interpreter();

        void run(uint8_t* code, uint16_t length);

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Event event;

        const uint8_t* keys;
        const uint8_t keymap[16] = {
            SDL_SCANCODE_KP_0, // 0x0

            SDL_SCANCODE_KP_7, SDL_SCANCODE_KP_8, SDL_SCANCODE_KP_9, // 0x1, 0x2, 0x3
            SDL_SCANCODE_KP_4, SDL_SCANCODE_KP_5, SDL_SCANCODE_KP_6, // 0x4, 0x5, 0x6
            SDL_SCANCODE_KP_1, SDL_SCANCODE_KP_2, SDL_SCANCODE_KP_3, // 0x7, 0x8, 0x9

            SDL_SCANCODE_KP_DIVIDE, SDL_SCANCODE_KP_MULTIPLY, SDL_SCANCODE_KP_MINUS, // 0xA, 0xB, 0xC
            SDL_SCANCODE_KP_PLUS, SDL_SCANCODE_KP_ENTER, SDL_SCANCODE_KP_PERIOD // 0xD, 0xE, 0xF
        };

        uint8_t ram[4096] = {0x00};
        bool vram[2048] = {false};

        uint8_t v[16] = {0x00}; // registers V0 - VF
        uint16_t I = 0x0000; // I register

        std::vector<uint16_t> s; // stack

        uint8_t dt = 0x00; // delay timer
        uint8_t st = 0x00; // sound timer

        uint16_t pc = 0x0200; // program counter

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

        void rand_and();

        void sound_set();

        void timer_get();
        void timer_set();
};

#endif