#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <cmath>
#include <cstdio>
#include <string>

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const float FIXED_TIMESTEP = 0.016f;

bool init(){
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("Could not initialize SDL, SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
        puts("Warning: linear texture filtering not enabled!\n");
    }

    gWindow = SDL_CreateWindow("SDL2 Golf", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if(gWindow == NULL){
        printf("Could not create SDL window, SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, 0, SDL_RENDERER_ACCELERATED); //| SDL_RENDERER_PRESENTVSYNC);

    if(gRenderer == NULL){
        printf("Could not create SDL renderer, SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG)){
        printf("Could not initialize SDL_image, IMG_Error: %s\n", IMG_GetError());
        success = false;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        printf("Could not initialize SDL_mixer, Mix_Error: %s\n", Mix_GetError());
        success = false;
    }

    return success;

}

SDL_Texture* loadMedia(std::string path){
    SDL_Surface* surface = IMG_Load(path.c_str());

    if(surface == NULL){
        printf("Could not load image %s, IMG_Error: %s\n", path.c_str(), IMG_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);

    if(texture == NULL){
        printf("Could not create texture from %s, SDL_Error: %s\n", path.c_str(), SDL_GetError());
        return NULL;
    }

    SDL_FreeSurface(surface);

    return texture;
}

void close(){
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    gRenderer = NULL;
    gWindow = NULL;

    SDL_Quit();
}

int main(int argc, char* argv[]){
    if(!init()){
        puts("Error initializing SDL\n");
        return 0;
    }

    SDL_Texture* ballTexture = loadMedia("../../res/golf_ball.png");

    if(ballTexture == NULL){
        puts("Error loading ball media\n");
        return 0;
    }
    
    SDL_Texture* fieldTexture = loadMedia("../../res/field.jpg");

    if(fieldTexture == NULL){
        puts("Error loading field media\n");
        return 0;
    }

    SDL_Texture* holeTexture = loadMedia("../../res/hole.png");

    if(holeTexture == NULL){
        puts("Error loading hole media\n");
        return 0;
    }

    SDL_Texture* arrowTexture = loadMedia("../../res/arrow.png");

    if(arrowTexture == NULL){
        puts("Error loading arrow media\n");
        return 0;
    }

    SDL_Texture* arrowTipTexture = loadMedia("../../res/arrow_tip.png");

    if(arrowTipTexture == NULL){
        puts("Error loading arrow tip media\n");
        return 0;
    }

    Mix_Chunk* swingSound = Mix_LoadWAV("../../res/swing.wav");
    Mix_Chunk* collisionSound = Mix_LoadWAV("../../res/collision.wav");
    Mix_Chunk* holeSound = Mix_LoadWAV("../../res/hole.wav");

    SDL_Event e;
    bool quit = false;
    SDL_FRect rect = {(rand() % (WINDOW_WIDTH - 30)) + 15.0f, (rand() % (WINDOW_HEIGHT - 30)) + 15.0f, 15, 15};
    SDL_FRect hole = {(rand() % (WINDOW_WIDTH - 40)) + 20.0f, (rand() % (WINDOW_HEIGHT - 40)) + 20.0f, 20, 20};
    float xVel = 0.0f;
    float yVel = 0.0f;
    float velocity1D = 0.0f;
    float friction = 0.6f;

    Uint32 currentTime, previousTime = SDL_GetTicks();
    float dt = 0.0f;
    float delta = 0.0f;
    float frameTime = SDL_GetTicks();
    float accumulator = 0.0f;
    bool lock = false, moving = false;
    bool win = false;

    while(!quit){
        currentTime = SDL_GetTicks();
        dt = (currentTime - previousTime) / 1000.0f;
        delta = currentTime - frameTime;
        previousTime = currentTime;

        accumulator += dt;

        if(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                quit = true;
            }
            if(e.type == SDL_MOUSEBUTTONDOWN){
                if(!moving){
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    if(x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h){
                        printf("x: %d, y: %d\n", x, y);
                        lock = true;
                    }
                }
            }
            if(e.type == SDL_MOUSEBUTTONUP){
                if(lock){
                    int x, y;
                    printf("getting mouse state\n");
                    SDL_GetMouseState(&x, &y);


                
                    xVel = -(x - (rect.x + rect.w / 2));// * 2.0f;
                    yVel = -(y - (rect.y + rect.h / 2));// * 2.0f;

                    //xVel *= 5.0f;
                    //yVel *= 5.0f;

                    velocity1D = sqrt(pow(xVel, 2) + pow(yVel, 2));

                    if(velocity1D > 100.0f){
                        velocity1D = 100.0f;
                        float angle = atan2(yVel, xVel);
                        xVel = cos(angle) * velocity1D;
                        yVel = sin(angle) * velocity1D;
                    }

                    xVel *= 10.0f;
                    yVel *= 10.0f;

                    lock = false;
                    moving = true;

                    Mix_PlayChannel(-1, swingSound, 0);
                    Mix_Volume(-1, velocity1D * 1.28f );

                    printf("Released at x: %d, y: %d\n", x, y);
                }
            }
            if(e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                    case SDLK_SPACE:
                        if(win){
                            rect.x = (rand() % (WINDOW_WIDTH - 30)) + 15.0f;
                            rect.y = (rand() % (WINDOW_HEIGHT - 30)) + 15.0f;
                            rect.w = 15.0f;
                            rect.h = 15.0f;
                            hole.x = (rand() % (WINDOW_WIDTH - 40)) + 20.0f;
                            hole.y = (rand() % (WINDOW_HEIGHT - 40)) + 20.0f;
                            win = false;
                        }
                }
            }
        }

        /*if(delta <= 1000/15){
            continue;
        }*/

        printf("framerate: %f\n", 1000.0f / delta);

        while(accumulator >= FIXED_TIMESTEP){

            if(moving){
                xVel *= pow(friction, FIXED_TIMESTEP) * ((velocity1D < 10.0f) ? 0.99f : 1.0f);
                yVel *= pow(friction, FIXED_TIMESTEP) * ((velocity1D < 10.0f) ? 0.99f : 1.0f);
                velocity1D = sqrt(pow(xVel, 2) + pow(yVel, 2));

                //printf("xVel: %f, yVel: %f, velocity1d: %f\n", xVel, yVel, velocity1D);

                rect.x += xVel * FIXED_TIMESTEP;
                rect.y += yVel * FIXED_TIMESTEP;

                if(rect.x < 0){
                    rect.x = 0.0f;
                    xVel = -xVel;
                    Mix_PlayChannel(-1, collisionSound, 0);
                    Mix_Volume(-1, velocity1D * 1.28f );
                }
                else if(rect.x + rect.w > WINDOW_WIDTH){
                    rect.x = WINDOW_WIDTH - rect.w;
                    xVel = -xVel;
                    Mix_PlayChannel(-1, collisionSound, 0);
                    Mix_Volume(-1, velocity1D * 1.28f );
                }

                if(rect.y < 0){
                    rect.y = 0.0f;
                    yVel = -yVel;
                    Mix_PlayChannel(-1, collisionSound, 0);
                    Mix_Volume(-1, velocity1D * 1.28f );
                }else if(rect.y + rect.h > WINDOW_HEIGHT){
                    rect.y = WINDOW_HEIGHT - rect.h;
                    yVel = -yVel;
                    Mix_PlayChannel(-1, collisionSound, 0);
                    Mix_Volume(-1, velocity1D * 1.28f );
                }

                if(xVel < 0.5f && xVel > -0.5f){
                    xVel = 0.0f;
                }

                if(yVel < 0.5f && yVel > -0.5f){
                    yVel = 0.0f;
                }

                if(xVel == 0.0f && yVel == 0.0f){
                    moving = false;
                }
            }

            if(!win){
                if(rect.x > hole.x - 5 && rect.x + rect.w < hole.x + hole.w + 5 && rect.y > hole.y - 5 && rect.y + rect.h < hole.y + hole.h + 5){
                    xVel = 0.0f;
                    yVel = 0.0f;
                    velocity1D = 0.0f;
                    moving = false;
                    win = true;
                    Mix_PlayChannel(-1, holeSound, 0);
                }
            }
            else {
                rect.w -= 0.5f;
                rect.h -= 0.5f;
                rect.x += 0.25f;
                rect.y += 0.25f;
            }

            accumulator -= FIXED_TIMESTEP;
        }

        SDL_RenderClear(gRenderer);

        SDL_RenderCopy(gRenderer, fieldTexture, NULL, NULL);

        SDL_RenderCopyF(gRenderer, holeTexture, NULL, &hole);

        if(lock){
            int x, y;
            SDL_GetMouseState(&x, &y);

            float startX = rect.x + rect.w / 2;
            float startY = rect.y + rect.h / 2;
            float endX = -(x - (rect.x + rect.w / 2)) + rect.x + rect.w / 2;
            float endY = -(y - (rect.y + rect.h / 2)) + rect.y + rect.h / 2;

            float segmentLength = sqrt(pow(endX - startX, 2) + pow(endY - startY, 2));

            /*if(segmentLength <= 33.3f){
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
            }
            else if(segmentLength > 33.3f && segmentLength <= 66.7f){
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
            }
            else if(segmentLength > 66.7f && segmentLength < 100.0f){
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x80, 0x00, 0xFF);
            }
            else {*/

float angle = atan2(endY - startY, endX - startX);
            
            if(segmentLength > 100.0f){
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
                segmentLength = 100.0f;
                
                endX = cos(angle) * segmentLength + startX;
                endY = sin(angle) * segmentLength + startY;
            }

            int w, h;

            SDL_QueryTexture(arrowTexture, NULL, NULL, &w, &h);

            float computedW =segmentLength;

            SDL_FRect arrow = {startX, startY - h / 20.0f / 2, computedW, h / 20.0f};

            SDL_FPoint center = {0, (arrow.h / 2)};

            SDL_RenderCopyExF(gRenderer, arrowTexture, NULL, &arrow, angle * 180 / M_PI, &center, SDL_FLIP_NONE);

            SDL_QueryTexture(arrowTipTexture, NULL, NULL, &w, &h);

            computedW = w / 20.0f;
            float computedH = h / 20.0f;

            SDL_FRect tip = {endX - (float)(sqrt(pow(computedW / 2, 2) + pow(computedH / 2, 2))) / 2, endY - (float)(sqrt(pow(computedW / 2, 2) + pow(computedH / 2, 2))) / 2, w / 20.0f, h / 20.0f};

            SDL_RenderCopyExF(gRenderer, arrowTipTexture, NULL, &tip, angle * 180 / M_PI, &center, SDL_FLIP_NONE);

            SDL_RenderDrawLine(gRenderer, startX, startY, endX, endY);
        }

        SDL_RenderCopyF(gRenderer, ballTexture, NULL, &rect);

        SDL_RenderPresent(gRenderer);

        frameTime = currentTime;
    }

    close();
    SDL_DestroyTexture(ballTexture);
    SDL_DestroyTexture(fieldTexture);

    return 0;
}