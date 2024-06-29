#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <SDL2/SDL.h>

#include <iostream>
#include <cstdint>

class Interpreter {
    public:
        Interpreter();
        ~Interpreter();

        void run();

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Event event;
};

#endif