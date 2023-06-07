#include <SDL2/SDL.h>
#include <math.h>

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const float FIXED_TIMESTEP = 0.016f;

bool init(){
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("Could not initialize SDL, SDL_Error: %s", SDL_GetError());
        success = false;
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
        puts("Warning: linear texture filtering not enabled!");
    }

    gWindow = SDL_CreateWindow((char*)"Cube Jump Testing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if(gWindow == NULL){
        printf("Could not create SDL window, SDL_Error: %s", SDL_GetError());
        success = false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, 0, SDL_RENDERER_ACCELERATED); //| SDL_RENDERER_PRESENTVSYNC);

    if(gRenderer == NULL){
        printf("Could not create SDL renderer, SDL_Error: %s", SDL_GetError());
        success = false;
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    return success;

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
        puts("Error initializing SDL");
        return 0;
    }

    SDL_Event e;
    bool quit = false;
    SDL_FRect rect = {(WINDOW_WIDTH - 10) / 2, (WINDOW_HEIGHT - 10) / 2, 10, 10};
    float xVel = 0.0f;
    float yVel = 0.0f;
    float friction = 0.6f;

    Uint32 currentTime, previousTime = SDL_GetTicks();
    float dt = 0.0f;
    float accumulator = 0.0f;
    bool lock = false, moving = false;

    while(!quit){
        currentTime = SDL_GetTicks();
        dt = (currentTime - previousTime) / 1000.0f;
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
                
                    xVel = -(x - (rect.x + rect.w / 2)) * 2.0f;
                    yVel = -(y - (rect.y + rect.h / 2)) * 2.0f;

                    lock = false;
                    moving = true;

                    printf("Released at x: %d, y: %d\n", x, y);
                }
            }
        }

        while(accumulator >= FIXED_TIMESTEP){

            if(moving){
                xVel *= pow(friction, FIXED_TIMESTEP);
                yVel *= pow(friction, FIXED_TIMESTEP);

                printf("xVel: %f, yVel: %f, dt: %f\n", xVel, yVel, dt);

                rect.x += xVel * FIXED_TIMESTEP;
                rect.y += yVel * FIXED_TIMESTEP;

                if(rect.x < 0){
                    rect.x = 0.0f;
                    xVel = -xVel;

                }else if(rect.x + rect.w > WINDOW_WIDTH){
                    rect.x = WINDOW_WIDTH - rect.w;
                    xVel = -xVel;

                }

                if(rect.y < 0){
                    rect.y = 0.0f;
                    yVel = -yVel;

                }else if(rect.y + rect.h > WINDOW_HEIGHT){
                    rect.y = WINDOW_HEIGHT - rect.h;
                    yVel = -yVel;

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

            accumulator -= FIXED_TIMESTEP;
        }

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

        SDL_RenderClear(gRenderer);

        if(lock){
            int x, y;
            SDL_GetMouseState(&x, &y);

            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

            SDL_RenderDrawLine(gRenderer, rect.x + rect.w / 2, rect.y + rect.h / 2, -(x - (rect.x + rect.w / 2)) + rect.x + rect.w / 2, -(y - (rect.y + rect.h / 2)) + rect.y + rect.h / 2);
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);

        SDL_RenderFillRectF(gRenderer, &rect);

        SDL_RenderPresent(gRenderer);
    }

    close();

    return 0;
}