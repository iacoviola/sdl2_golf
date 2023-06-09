#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

#include "Vector2f.h"

namespace sdl {

class Texture {
    public:
        Texture(SDL_Renderer* renderer);

        ~Texture();

        int loadFromFile(const std::string path);

        void free();

        SDL_Texture* getTexture() const;

        float getWidth() const;

        float getHeight() const;

        math::Vector2f getSize() const;

    private:
        SDL_Texture* texture;
        SDL_Renderer* renderer;
        math::Vector2f size;

};
}

#endif // TEXTURE_H