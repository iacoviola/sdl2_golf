#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "App.h"

#include "RenderWindow.h"
#include "Vector2f.h"
#include "Texture.h"
#include "Sprite.h" 
#include "Ball.h"

App::App()
: window("SDL2 Golf", 640, 480), 
  ball(window.loadTextureFromFile("../../res/imgs/golf_ball.png")),
  hole(window.loadTextureFromFile("../../res/imgs/hole.png")),
  field(window.loadTextureFromFile("../../res/imgs/field.jpg")),
  arrow(window.loadTextureFromFile("../../res/imgs/arrow.png")),
  powerbar(window.loadTextureFromFile("../../res/imgs/powerbar.png")),
  powerbar_bg(window.loadTextureFromFile("../../res/imgs/powerbar_bg.png"))
{
    initAdditional();
}

App::~App(){
    sdl::quit();
}
        
void App::run(){
    while(running){
        current_time = std::chrono::steady_clock::now();
        dt = std::chrono::duration_cast<std::chrono::microseconds>(current_time - previous_time);
        previous_time = current_time;


        double acc = dt.count() / 1000000.0f;
        accumulator += acc;

        printf("dt: %f\n", acc);

        handleEvents();
        update();
        render();
    }
}

void App::initAdditional(){
    swingSound = Mix_LoadWAV("../../res/sounds/swing.wav");
    collisionSound = Mix_LoadWAV("../../res/sounds/collision.wav");
    holeSound = Mix_LoadWAV("../../res/sounds/hole.wav");

    int x, y;
    ball.setScale(ball.getTexture()->getWidth(), ball.getTexture()->getHeight());
    x = (rand() % (window.getWidth() - (int)ball.getScale().x * 2)) + ball.getScale().x;
    y = (rand() % (window.getHeight() - (int)ball.getScale().y * 2)) + ball.getScale().y;
    ball.setPosition(x ,y);

    x = (rand() % (window.getWidth() - (int)hole.getScale().x * 2)) + hole.getScale().x;
    y = (rand() % (window.getHeight() - (int)hole.getScale().y * 2)) + hole.getScale().y;
    hole.setPosition(x, y);
}

void App::handleEvents(){
    SDL_Event event;
    SDL_FRect golf_ball_rect = {ball.getPosition().x, ball.getPosition().y, ball.getScale().x, ball.getScale().y};

    if(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            running = false;
        }
        if(event.type == SDL_MOUSEBUTTONDOWN){
            if(!ball.isMoving()){
                int x, y;
                SDL_GetMouseState(&x, &y);

                if(x > golf_ball_rect.x && x < golf_ball_rect.x + golf_ball_rect.w && y > golf_ball_rect.y && y < golf_ball_rect.y + golf_ball_rect.h){
                    lock = true;
                }
            }
        }
        if(event.type == SDL_MOUSEBUTTONUP){
            if(lock){
                int x, y;
                SDL_GetMouseState(&x, &y);

                math::Vector2f golf_ball_velocity = math::Vector2f(-(x - (golf_ball_rect.x + golf_ball_rect.w / 2)), -(y - (golf_ball_rect.y + golf_ball_rect.h / 2)));
                float velocity1D = golf_ball_velocity.magnitude();

                if(velocity1D > 15.0f){

                    if(velocity1D > 100.0f){
                        velocity1D = 100.0f;
                        float angle = atan2(golf_ball_velocity.y, golf_ball_velocity.x);
                        golf_ball_velocity.x = cos(angle) * velocity1D;
                        golf_ball_velocity.y = sin(angle) * velocity1D;
                    }

                    golf_ball_velocity *= 10.0f;

                    ball.setVelocity(golf_ball_velocity);

                    lock = false;
                    ball.setMoving(true);

                    Mix_PlayChannel(-1, swingSound, 0);
                    Mix_Volume(-1, velocity1D * 1.28f);
                }
            }
        }
        if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_SPACE:
                    if(win){
                        int x, y;
                        ball.setScale(ball.getTexture()->getWidth(), ball.getTexture()->getHeight());
                        x = (rand() % (window.getWidth() - (int)ball.getScale().x * 2)) + ball.getScale().x;
                        y = (rand() % (window.getHeight() - (int)ball.getScale().y * 2)) + ball.getScale().y;
                        ball.setPosition(x ,y);

                        x = (rand() % (window.getWidth() - (int)hole.getScale().x * 2)) + hole.getScale().x;
                        y = (rand() % (window.getHeight() - (int)hole.getScale().y * 2)) + hole.getScale().y;
                        hole.setPosition(x, y);

                        win = false;
                    }
            }
        }
    }    
}

void App::update(){
    while(accumulator >= FIXED_DELTA_TIME){
        ball.update(FIXED_DELTA_TIME);

        if(ball.getPosition().x < 0){
            ball.setPosition(0.0f, ball.getPosition().y);
            ball.setVelocity(-ball.getVelocity().x, ball.getVelocity().y);
            Mix_PlayChannel(-1, collisionSound, 0);
            Mix_Volume(-1, ball.getVelocity().magnitude() * 1.28f );
        }
        else if(ball.getPosition().x + ball.getScale().x > window.getWidth()){
            ball.setPosition(window.getWidth() - ball.getScale().x, ball.getPosition().y);
            ball.setVelocity(-ball.getVelocity().x, ball.getVelocity().y);
            Mix_PlayChannel(-1, collisionSound, 0);
            Mix_Volume(-1, ball.getVelocity().magnitude() * 1.28f );
        }

        if(ball.getPosition().y < 0){
            ball.setPosition(ball.getPosition().x, 0.0f);
            ball.setVelocity(ball.getVelocity().x, -ball.getVelocity().y);
            Mix_PlayChannel(-1, collisionSound, 0);
            Mix_Volume(-1, ball.getVelocity().magnitude() * 1.28f );
        }
        else if(ball.getPosition().y + ball.getScale().y > window.getHeight()){
            ball.setPosition(ball.getPosition().x, window.getHeight() - ball.getScale().y);
            ball.setVelocity(ball.getVelocity().x, -ball.getVelocity().y);
            Mix_PlayChannel(-1, collisionSound, 0);
            Mix_Volume(-1, ball.getVelocity().magnitude() * 1.28f );
        }


        if(!win){
            if(ball.getPosition().x > hole.getPosition().x - 5 && 
                ball.getPosition().x + ball.getScale().x < hole.getPosition().x + hole.getScale().x + 5 && 
                ball.getPosition().y > hole.getPosition().y - 5 && 
                ball.getPosition().y + ball.getScale().y < hole.getPosition().y + hole.getScale().y + 5)
            {
                ball.setVelocity(0.0f, 0.0f);
                ball.setMoving(false);
                win = true;
                Mix_PlayChannel(-1, holeSound, 0);
            }
        }
        else {
            ball.shrink(0.5f, FIXED_DELTA_TIME);
        }


        accumulator -= FIXED_DELTA_TIME;
    }
}

void App::render(){
    window.clear();

    window.render(field);
    window.render(hole);
    window.render(ball);

    //Need to render also: arrow, powerbar, powerbar background

    window.display();

}