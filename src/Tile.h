#ifndef TILE_H
#define TILE_H

#include "Sprite.h"

class Tile : public sdl::Sprite {
    public:
        Tile() : sdl::Sprite() {}

        Tile(sdl::Texture* texture) : sdl::Sprite(texture) {}
};

#endif // TILE_H