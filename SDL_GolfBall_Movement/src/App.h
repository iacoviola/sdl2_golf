#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <chrono>

#include "RenderWindow.h"
#include "Sprite.h" 
#include "Ball.h"

class App
{
    public:
        App();

        ~App();
        
        void run();

    private:

        void initAdditional();

        void handleEvents();

        void update();

        void render();

        sdl::RenderWindow window;

        Ball ball;
        sdl::Sprite hole;
        sdl::Sprite field;
        sdl::Sprite arrow;
        sdl::Sprite powerbar;
        sdl::Sprite powerbar_bg;

        Mix_Chunk* swingSound;
        Mix_Chunk* collisionSound;
        Mix_Chunk* holeSound;

        std::chrono::steady_clock::time_point current_time;
        std::chrono::steady_clock::time_point previous_time;

        std::chrono::microseconds dt;

        double accumulator = 0.0;
        bool lock = false, win = false, running = true;

        const double FIXED_DELTA_TIME = 0.016;
};

#endif // APP_H