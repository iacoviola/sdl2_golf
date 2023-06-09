#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "Sprite.h"
#include "Vector2f.h"

namespace sdl {

enum sdlModules {
    SDL_IMAGE = 0x00000001,
    SDL_MIXER = 0x00000010,
    SDL_TTF = 0x00000100,
    SDL_ALL = 0x00000111
};

class RenderWindow
{
    public:
        RenderWindow(const std::string title, const int width, const int height);
        
        ~RenderWindow();

        void clear();

        void render(sdl::Sprite& sprite);

        sdl::Texture* loadTextureFromFile(const std::string path);

        void display();

        SDL_Renderer* getRenderer() const;

        int getWidth() const;

        int getHeight() const;

    private:
        std::string title;
        math::Vector2f size;

        SDL_Window* window;
        SDL_Renderer* renderer;
};

inline bool init(int flags = SDL_INIT_EVERYTHING, int modules = SDL_ALL, int imgFlags = IMG_INIT_PNG){
    if(SDL_Init(flags) < 0){
        return false;
    }

    if(modules & SDL_IMAGE){
        if(IMG_Init(imgFlags) != imgFlags){
            return false;
        }
    }

    if(modules & SDL_MIXER){
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
            return false;
        }
    }

    if(modules & SDL_TTF){
        if(TTF_Init() == -1){
            return false;
        }
    }

    return true;
}

inline void quit(int modules = SDL_ALL){
    if(modules & SDL_IMAGE){
        IMG_Quit();
    }

    if(modules & SDL_MIXER){
        Mix_Quit();
    }

    if(modules & SDL_TTF){
        TTF_Quit();
    }

    SDL_Quit();
}

}

#endif // RENDERWINDOW_H