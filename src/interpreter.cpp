#include "interpreter.hpp"

Interpreter::Interpreter() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        SDL_Log("SDL could not be initialized! Error: %s\n", SDL_GetError());
    }

    window = SDL_CreateWindow(
        "CHIP-8",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        SDL_Log("Window could not be initialized! Error: %s\n", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (renderer == nullptr) {
        SDL_Log("Renderer could not be initialized! Error: %s\n", SDL_GetError());
    }

    keys = SDL_GetKeyboardState(nullptr);
}

Interpreter::~Interpreter() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

void Interpreter::run(uint8_t* code, uint16_t length) {
    if (length > 4096 - 512) {
        return; // TODO: throw an exception
    }

    std::copy(code, code + length, ram + 0x0200);

    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return;
            }
        }

        printf("Opcode: 0x%02X%02X, PC: 0x%02X, I: 0x%02X\n\n", ram[pc], ram[pc + 1], pc, I);

        for (uint8_t i = 0; i < 16; i++) {
            printf("V%X: 0x%02X", i, v[i]);

            if ((i + 1) % 4 != 0) {
                printf(", ");
            } else {
                printf("\n");
            }
        }

        printf("\nDT: 0x%02X, ST: 0x%02X\n--------------------------------------\n", dt, st);

        switch (ram[pc] & 0xF0) {
            case 0x00:
                switch (ram[pc + 1]) {
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
            case 0x60:
                const_set(); break;
            case 0x70:
                const_add(); break;
            case 0x80:
                switch (ram[pc + 1] & 0x0F) {
                    case 0x00:
                        math_set(); break;
                    case 0x01:
                        bitop_or(); break;
                    case 0x02:
                        bitop_and(); break;
                    case 0x03:
                        bitop_xor(); break;
                    case 0x04:
                        math_add(); break;
                    case 0x05:
                        math_sub_vx(); break;
                    case 0x06:
                        bitop_shift_right(); break;
                    case 0x07:
                        math_sub_vy(); break;
                    case 0x0E:
                        bitop_shift_left(); break;
                    default:
                        break;
                }

                break;
            case 0x90:
                cond_register_not_equal(); break;
            case 0xA0:
                mem_set(); break;
            case 0xB0:
                flow_jump_offset(); break;
            case 0xC0:
                rand_and(); break;
            case 0xD0:
                display_draw(); break;
            case 0xE0:
                switch (ram[pc + 1]) {
                    case 0x9E:
                        keyop_pressed(); break;
                    case 0xA1:
                        keyop_not_pressed(); break;
                    default:
                        break;
                }

                break;
            case 0xF0:
                switch (ram[pc + 1]) {
                    case 0x07:
                        timer_get(); break;
                    case 0x0A:
                        keyop_get(); break;
                    case 0x15:
                        timer_set(); break;
                    case 0x18:
                        sound_set(); break;
                    case 0x1E:
                        mem_add(); break;
                    case 0x29:
                        mem_font_get(); break;
                    case 0x33:
                        bcd(); break;
                    case 0x55:
                        mem_reg_dump(); break;
                    case 0x65:
                        mem_reg_load(); break;
                    default:
                        break;
                }

                break;
            default:
                break;
        }

        for (uint8_t y = 0; y < 32; y++) {
            for (uint8_t x = 0; x < 64; x++) {
                if (vram[x + (y * 64)]) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                } else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                }

                SDL_Rect pixel = {x * 10, y * 10, 10, 10};
                SDL_RenderFillRect(renderer, &pixel);
            }
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / 60);
        //getchar();

        if (dt > 0) dt--;
        if (st > 0) st--;

        pc += 2;
    }
}

void Interpreter::bcd() {
    ram[I] = v[ram[pc] & 0x0F] / 100;
    ram[I + 1] = (v[ram[pc] & 0x0F] / 10) % 10;
    ram[I + 2] = v[ram[pc] & 0x0F] % 10;
}

void Interpreter::bitop_or() {
    v[ram[pc] & 0x0F] |= v[ram[pc + 1] >> 4];
}

void Interpreter::bitop_and() {
    v[ram[pc] & 0x0F] &= v[ram[pc + 1] >> 4];
}

void Interpreter::bitop_xor() {
    v[ram[pc] & 0x0F] ^= v[ram[pc + 1] >> 4];
}

void Interpreter::bitop_shift_left() {
    v[0xF] = v[ram[pc] & 0x0F] >> 7;
    v[ram[pc] & 0x0F] <<= 1;
}

void Interpreter::bitop_shift_right() {
    v[0xF] = v[ram[pc] & 0x0F] & 0x01;
    v[ram[pc] & 0x0F] >>= 1;
}

void Interpreter::cond_const_equals() {
    if (v[ram[pc] & 0x0F] == ram[pc + 1]) {
        pc += 2;
    }
}

void Interpreter::cond_const_not_equal() {
    if (v[ram[pc] & 0x0F] != ram[pc + 1]) {
        pc += 2;
    }
}

void Interpreter::cond_register_equals() {
    if (v[ram[pc] & 0x0F] == v[ram[pc + 1] >> 4]) {
        pc += 2;
    }
}

void Interpreter::cond_register_not_equal() {
    if (v[ram[pc] & 0x0F] != v[ram[pc + 1] >> 4]) {
        pc += 2;
    }
}

void Interpreter::const_set() {
    v[ram[pc] & 0x0F] = ram[pc + 1];
}

void Interpreter::const_add() {
    v[ram[pc] & 0x0F] += ram[pc + 1];
}

void Interpreter::display_clear() {
    for (uint16_t i = 0; i < 2048; i++) {
        vram[i] = false;
    }
}

void Interpreter::display_draw() {
    uint8_t x = v[ram[pc] & 0x0F];
    uint8_t y = v[ram[pc + 1] >> 4];

    uint8_t height = ram[pc + 1] & 0x0F;

    for (uint8_t row = 0; row < height; row++) {
        for (uint8_t column = 0; column < 8; column++) {
            if (ram[I + row] & (0x80 >> column) && vram[(x + column) + ((y + row) * 64)]) {
                v[0xF] = 1;
            }

            vram[(x + column) + ((y + row) * 64)] ^= ram[I + row] & (0x80 >> column);
        }
    }
}

void Interpreter::flow_return() {
    pc = s.back() - 2; s.pop_back();
}

void Interpreter::flow_jump() {
    pc = ((ram[pc] & 0x0F) << 8 | ram[pc + 1]) - 2;
}

void Interpreter::flow_call() {
    s.push_back(pc + 2); pc = ((ram[pc] & 0x0F) << 8 | ram[pc + 1]) - 2;
}

void Interpreter::flow_jump_offset() {
    pc = ((ram[pc] & 0x0F) << 8 | ram[pc + 1]) + v[0x0] - 2;
}

void Interpreter::keyop_pressed() {
    if (keys[keymap[v[ram[pc] & 0x0F]]]) {
        pc += 2;
    }
}

void Interpreter::keyop_not_pressed() {
    if (!keys[keymap[v[ram[pc] & 0x0F]]]) {
        pc += 2;
    }
}

void Interpreter::keyop_get() {
    while (true) {
        for (uint8_t i = 0; i < 16; i++) {
            if (keys[keymap[i]]) {
                v[ram[pc] & 0x0F] = i; return;
            }
        }
    }
}

void Interpreter::math_set() {
    v[ram[pc] & 0x0F] = v[ram[pc + 1] >> 4];
}

void Interpreter::math_add() {
    v[ram[pc] & 0x0F] += v[ram[pc + 1] >> 4];
}

void Interpreter::math_sub_vx() {
    v[ram[pc] & 0x0F] -= v[ram[pc + 1] >> 4];
}

void Interpreter::math_sub_vy() {
    v[ram[pc] & 0x0F] = v[ram[pc + 1] >> 4] - v[ram[pc] & 0x0F];
}

void Interpreter::mem_set() {
    I = (ram[pc] & 0x0F) << 8 | ram[pc + 1];
}

void Interpreter::mem_add() {
    I += v[ram[pc] & 0x0F];
}

void Interpreter::mem_font_get() {
    I = ram[v[ram[pc] & 0x0F] * 5];
}

void Interpreter::mem_reg_dump() {
    for (uint8_t i = 0; i < 16; i++) {
        ram[I + i] = v[i];
    }
}

void Interpreter::mem_reg_load() {
    for (uint8_t i = 0; i < 16; i++) {
        v[i] = ram[I + i];
    }
}

void Interpreter::rand_and() {
    v[ram[pc] & 0x0F] = (rand() % 256) & ram[pc + 1];
}

void Interpreter::sound_set() {
    st = v[ram[pc] & 0x0F];
}

void Interpreter::timer_get() {
    v[ram[pc] & 0x0F] = dt;
}

void Interpreter::timer_set() {
    dt = v[ram[pc] & 0x0F];
}