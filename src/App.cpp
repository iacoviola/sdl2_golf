#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "App.h"

#include "RenderWindow.h"
#include "Vector2f.h"
#include "Texture.h"
#include "Sprite.h" 
#include "Ball.h"

App::App(){
    init();
}

App::~App(){
    Mix_FreeChunk(swingSound);
    Mix_FreeChunk(collisionSound);
    Mix_FreeChunk(holeSound);

    swingSound = nullptr;
    collisionSound = nullptr;
    holeSound = nullptr;

    delete window;

    sdl::quit();
}
        
void App::run(){
    while(running){
        current_time = std::chrono::steady_clock::now();
        dt = std::chrono::duration_cast<std::chrono::microseconds>(current_time - previous_time);
        previous_time = current_time;


        double acc = dt.count() / 1000000.0f;
        accumulator += acc;

        handleEvents();

        updateStatic();
        updatePhysics();
        
        render();
    }
}

void App::init(){

    int flags = SDL_INIT_VIDEO;
    int modules = sdl::SDL_IMAGE | sdl::SDL_MIXER;
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    sdl::initSDL(flags, modules, imgFlags);

    window = new sdl::RenderWindow("SDL2 Golf", 640, 480);

    ball.setTexture(window->loadTextureFromFile("../../res/imgs/golf_ball.png"));
    hole.setTexture(window->loadTextureFromFile("../../res/imgs/hole.png"));
    field.setTexture(window->loadTextureFromFile("../../res/imgs/field.jpg"));
    arrow.setTexture(window->loadTextureFromFile("../../res/imgs/arrow.png"));
    powerbar.setTexture(window->loadTextureFromFile("../../res/imgs/powerbar.png"));
    powerbar_bg.setTexture(window->loadTextureFromFile("../../res/imgs/powerbar_bg.png"));

    swingSound = Mix_LoadWAV("../../res/sounds/swing.wav");
    collisionSound = Mix_LoadWAV("../../res/sounds/collision.wav");
    holeSound = Mix_LoadWAV("../../res/sounds/hole.wav");

    int x, y;
    ball.setScale(ball.getTexture()->getWidth(), ball.getTexture()->getHeight());
    x = (rand() % (window->getWidth() - (int)ball.getScale().x * 2)) + ball.getScale().x;
    y = (rand() % (window->getHeight() - (int)ball.getScale().y * 2)) + ball.getScale().y;
    ball.setPosition(x ,y);

    x = (rand() % (window->getWidth() - (int)hole.getScale().x * 2)) + hole.getScale().x;
    y = (rand() % (window->getHeight() - (int)hole.getScale().y * 2)) + hole.getScale().y;
    hole.setPosition(x, y);
}

void App::handleEvents() {
    SDL_Event event;
    SDL_FRect ball_rect = ball.getRect();

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                handleMouseButtonDown(ball_rect);
                break;
            case SDL_MOUSEBUTTONUP:
                if(lock)
                    handleMouseButtonUp(ball_rect);
                break;
            case SDL_KEYDOWN:
                handleKeyDown(event);
                break;
            // Add more cases for other event types if needed
        }
    }
}

void App::handleMouseButtonDown(const SDL_FRect& ball_rect) {
    if (!ball.isMoving()) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (x > ball_rect.x && x < ball_rect.x + ball_rect.w &&
            y > ball_rect.y && y < ball_rect.y + ball_rect.h) {
            lock = true;
        }
    }
}

void App::handleMouseButtonUp(const SDL_FRect& ball_rect) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    math::Vector2f golf_ball_velocity = math::Vector2f(-(x - (ball_rect.x + ball_rect.w / 2)),
                                                        -(y - (ball_rect.y + ball_rect.h / 2)));
    float velocity1D = golf_ball_velocity.magnitude();

    if (velocity1D > 15.0f) {
        if (velocity1D > 100.0f) {
            velocity1D = 100.0f;
            float angle = atan2(golf_ball_velocity.y, golf_ball_velocity.x);
            golf_ball_velocity.x = cos(angle) * velocity1D;
            golf_ball_velocity.y = sin(angle) * velocity1D;
        }

        golf_ball_velocity *= 10.0f;

        ball.setVelocity(golf_ball_velocity);
        ball.setMoving(true);

        lock = false;
        draw_aux = false;

        Mix_PlayChannel(-1, swingSound, 0);
        Mix_Volume(-1, velocity1D * 1.28f);
    }
}

void App::handleKeyDown(const SDL_Event& event) {
    switch (event.key.keysym.sym) {
        case SDLK_SPACE:
            if (win) {
                resetGame();
            }
            break;
    }
}

void App::resetGame() {
    int x, y;
    ball.setScale(ball.getTexture()->getWidth(), ball.getTexture()->getHeight());
    x = (rand() % (window->getWidth() - (int)ball.getScale().x * 2)) + ball.getScale().x;
    y = (rand() % (window->getHeight() - (int)ball.getScale().y * 2)) + ball.getScale().y;
    ball.setPosition(x, y);

    x = (rand() % (window->getWidth() - (int)hole.getScale().x * 2)) + hole.getScale().x;
    y = (rand() % (window->getHeight() - (int)hole.getScale().y * 2)) + hole.getScale().y;
    hole.setPosition(x, y);

    win = false;
}

void App::updatePhysics(){
    while(accumulator >= FIXED_DELTA_TIME){
        ball.update(FIXED_DELTA_TIME);

        if(ball.getPosition().x < 0){
            ball.setPosition(0.0f, ball.getPosition().y);
            ball.setVelocity(-ball.getVelocity().x, ball.getVelocity().y);
            Mix_PlayChannel(-1, collisionSound, 0);
            Mix_Volume(-1, ball.getVelocity().magnitude() * 1.28f );
        }
        else if(ball.getPosition().x + ball.getScale().x > window->getWidth()){
            ball.setPosition(window->getWidth() - ball.getScale().x, ball.getPosition().y);
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
        else if(ball.getPosition().y + ball.getScale().y > window->getHeight()){
            ball.setPosition(ball.getPosition().x, window->getHeight() - ball.getScale().y);
            ball.setVelocity(ball.getVelocity().x, -ball.getVelocity().y);
            Mix_PlayChannel(-1, collisionSound, 0);
            Mix_Volume(-1, ball.getVelocity().magnitude() * 1.28f );
        }


        if(!win){
            float distance = sqrt(pow(ball.getCenter().x - hole.getCenter().x, 2) + pow(ball.getCenter().y - hole.getCenter().y, 2));
            if(distance < 7.5f
                /*ball.getPosition().x > hole.getPosition().x - 5 && 
                ball.getPosition().x + ball.getScale().x < hole.getPosition().x + hole.getScale().x + 5 && 
                ball.getPosition().y > hole.getPosition().y - 5 && 
                ball.getPosition().y + ball.getScale().y < hole.getPosition().y + hole.getScale().y + 5*/)
            {
                ball.setVelocity(0.0f, 0.0f);
                ball.setMoving(false);
                win = true;
                Mix_PlayChannel(-1, holeSound, 0);
            }
        }
        else {
            ball.shrink(0.5f);
        }

        accumulator -= FIXED_DELTA_TIME;
    }
}

void App::updateStatic(){
    if(lock){
        int x, y;
        SDL_GetMouseState(&x, &y);

        SDL_FRect ball_rect = ball.getRect();

        float startX = ball_rect.x + ball_rect.w / 2;
        float startY = ball_rect.y + ball_rect.h / 2;
        float endX = -(x - (ball_rect.x + ball_rect.w / 2)) + ball_rect.x + ball_rect.w / 2;
        float endY = -(y - (ball_rect.y + ball_rect.h / 2)) + ball_rect.y + ball_rect.h / 2;

        float segmentLength = sqrt(pow(endX - startX, 2) + pow(endY - startY, 2));

        if(segmentLength > ball_rect.w / 2){

            draw_aux = true;

            if(segmentLength > 100.0f){
                segmentLength = 100.0f;
            }

            float angle = atan2(endY - startY, endX - startX);

            arrow.setAngle(angle * 180 / M_PI);
            arrow.setPosition(startX, startY - arrow.getScale().y / 2);
            arrow.setRotationCenter(0, arrow.getScale().y / 2);

            powerbar_bg.setPosition(startX + 15 - ((powerbar_bg.getScale().x - powerbar.getRawScale().x) / 2), startY - powerbar_bg.getScale().y / 2);


            powerbar.setClip(0, powerbar.getRawScale().y - (powerbar.getRawScale().y * (segmentLength / 100)), powerbar.getRawScale().x, powerbar.getRawScale().y * (segmentLength / 100));
            powerbar.setScale(powerbar.getRawScale().x, powerbar.getRawScale().y * (segmentLength / 100));
            powerbar.setPosition(startX + 15, startY - powerbar.getRawScale().y / 2 + (powerbar.getRawScale().y - powerbar.getScale().y));
        }
        else {
            draw_aux = false;
        }
    }
}

void App::render(){
    window->clear();

    window->render(field);
    window->render(hole);

    if(lock && draw_aux){
        window->render(arrow);
        window->render(powerbar_bg);
        window->render(powerbar);
    }

    window->render(ball);

    window->display();

}