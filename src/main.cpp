#include "App.h"

int main(int argc, char* args[]){

    int init_flags = SDL_INIT_VIDEO;
    int modules_flags = sdl::SDL_IMAGE | sdl::SDL_MIXER;
    int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!sdl::init(init_flags, modules_flags, img_flags)){
        throw std::runtime_error("Failed to initialize SDL2");
    }
    
    App app;

    app.run();

    return 0;
}