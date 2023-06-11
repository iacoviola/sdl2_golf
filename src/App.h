#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <chrono>
#include <vector>
#include <random>

#include "RenderWindow.h"
#include "Sprite.h" 
#include "Ball.h"
#include "Tile.h"

class App
{
    public:
        App();

        ~App();
        
        void run();

    private:

        void init();

        void handleEvents();

        void handleMouseButtonDown(const SDL_FRect& ball_rect);

        void handleMouseButtonUp(const SDL_FRect& ball_rect);

        void handleKeyDown(const SDL_Event& event);

        void resetGame();
        void randomize();

        void updatePhysics();
        void checkCollisions();
        void updateStatic();

        void render();

        sdl::RenderWindow* window;

        Ball ball;
        sdl::Sprite hole;
        sdl::Sprite field;
        sdl::Sprite arrow;
        sdl::Sprite powerbar;
        sdl::Sprite powerbar_bg;

        Mix_Chunk* swingSound;
        Mix_Chunk* collisionSound;
        Mix_Chunk* holeSound;

        std::vector<Tile> tiles;

        std::chrono::steady_clock::time_point current_time;
        std::chrono::steady_clock::time_point previous_time;

        std::chrono::microseconds dt;

        double accumulator = 0.0;
        bool lock = false, win = false, running = true, draw_aux = false;

        const double FIXED_DELTA_TIME = 0.016;

        #ifndef _WIN32
        std::mt19937 gen;
        #endif
};

#endif // APP_H