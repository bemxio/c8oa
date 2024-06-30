#include "interpreter.hpp"

Interpreter::Interpreter() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "Error! " << SDL_GetError() << std::endl;
    }

    window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

Interpreter::~Interpreter() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

void Interpreter::run(uint8_t* code, uint16_t length) {
    if (length > 4096) {
        return; // TODO: throw an exception
    }

    std::copy(code, code + length, memory);

    while (memory[pc] != 0x00) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return;
            }
        }

        std::cout << "Opcode: 0x" << std::hex << (int)memory[pc] << (int)memory[pc + 1] << std::endl;
        std::cout << "PC: 0x" << std::hex << (int)pc << std::endl;
        std::cout << "I: 0x" << std::hex << (int)I << std::endl;

        switch (memory[pc] & 0xF0) {
            case 0x00:
                switch (memory[pc + 1]) {
                    case 0xE0:
                        display_clear(); break;
                    case 0xEE:
                        flow_return(); break;
                    default:
                        break;
                }

                break;
            
            case 0x10:
                flow_jump(); break;

            case 0x20:
                flow_call(); break;

            case 0x30:
                cond_const_equals(); break;
            
            case 0x40:
                cond_const_not_equal(); break;
            
            case 0x50:
                cond_register_equals(); break;

            case 0x90:
                cond_register_not_equal(); break;

            default:
                break;
        }

        SDL_RenderPresent(renderer);
        pc += 2;
    }
}

void Interpreter::bitop_or() {
    v[memory[pc] & 0x0F] |= v[memory[pc + 1] >> 4];
}

void Interpreter::bitop_and() {
    v[memory[pc] & 0x0F] &= v[memory[pc + 1] >> 4];
}

void Interpreter::bitop_xor() {
    v[memory[pc] & 0x0F] ^= v[memory[pc + 1] >> 4];
}

void Interpreter::bitop_shift_left() {
    v[0xf] = v[memory[pc] & 0x0F] >> 7;
    v[memory[pc] & 0x0F] <<= 1;
}

void Interpreter::bitop_shift_right() {
    v[0xf] = v[memory[pc] & 0x0F] & 0x01;
    v[memory[pc] & 0x0F] >>= 1;
}

void Interpreter::cond_const_equals() {
    if (v[memory[pc] & 0x0F] == memory[pc + 1]) {
        pc += 2;
    }
}

void Interpreter::cond_const_not_equal() {
    if (v[memory[pc] & 0x0F] != memory[pc + 1]) {
        pc += 2;
    }
}

void Interpreter::cond_register_equals() {
    if (v[memory[pc] & 0x0F] == v[memory[pc + 1] >> 4]) {
        pc += 2;
    }
}

void Interpreter::cond_register_not_equal() {
    if (v[memory[pc] & 0x0F] != v[memory[pc + 1] >> 4]) {
        pc += 2;
    }
}

void Interpreter::const_set() {
    v[memory[pc] & 0x0F] = memory[pc + 1];
}

void Interpreter::const_add() {
    v[memory[pc] & 0x0F] += memory[pc + 1];
}

void Interpreter::display_clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Interpreter::flow_return() {
    pc = s.back(); s.pop_back();
}

void Interpreter::flow_jump() {
    pc = opcode_address();
}

void Interpreter::flow_call() {
    s.push_back(pc + 2); pc = opcode_address();
}

void Interpreter::flow_jump_offset() {
    pc = opcode_address() + v[0x0];
}

void Interpreter::math_set() {
    v[memory[pc] & 0x0F] = v[memory[pc + 1] >> 4];
}

void Interpreter::math_add() {
    v[memory[pc] & 0x0F] += v[memory[pc + 1] >> 4];
}

void Interpreter::math_sub_vx() {
    v[memory[pc] & 0x0F] -= v[memory[pc + 1] >> 4];
}

void Interpreter::math_sub_vy() {
    v[memory[pc] & 0x0F] = v[memory[pc + 1] >> 4] - v[memory[pc] & 0x0F];
}

void Interpreter::mem_set() {
    I = opcode_address();
}

void Interpreter::mem_add() {
    I += v[memory[pc] & 0x0F];
}

void Interpreter::mem_font_get() {
    I = memory[v[memory[pc] & 0x0F] * 20];
}

void Interpreter::mem_reg_dump() {
    for (uint8_t i = 0; i <= 16; i++) {
        memory[I + i] = v[i];
    }
}

void Interpreter::mem_reg_load() {
    for (uint8_t i = 0; i <= 16; i++) {
        v[i] = memory[I + i];
    }
}

void Interpreter::rand_and() {
    v[memory[pc] & 0x0F] = (rand() % 256) & memory[pc + 1];
}

void Interpreter::sound_set() {
    st = v[memory[pc] & 0x0F];
}

void Interpreter::timer_get() {
    v[memory[pc] & 0x0F] = dt;
}

void Interpreter::timer_set() {
    dt = v[memory[pc] & 0x0F];
}