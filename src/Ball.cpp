#include "Ball.h"
#include "Vector2f.h"
#include "Sprite.h"

Ball::Ball() : Sprite(), velocity(0.0f, 0.0f), moving(false) {};

Ball::Ball(sdl::Texture* texture) : Sprite(texture), velocity(0.0f, 0.0f), moving(false) {};

Ball::Ball(sdl::Texture* texture, math::Vector2f position, math::Vector2f velocity)
: Sprite(texture, position), velocity(velocity), moving(false) {};

Ball::~Ball(){ }

void Ball::update(float dt){

    if(moving){
        //velocity1D = (velocity / 10).magnitude();
        velocity.x *= pow(friction, dt) * ((velocity1D < 10.0f) ? 0.99f : 1.0f);
        velocity.y *= pow(friction, dt) * ((velocity1D < 10.0f) ? 0.99f : 1.0f);
        velocity1D = (velocity / 10).magnitude();

        setPosition(getPosition() + velocity * dt);

        if(velocity.x < 0.5f && velocity.x > -0.5f){
            velocity.x = 0.0f;
        }

        if(velocity.y < 0.5f && velocity.y > -0.5f){
            velocity.y = 0.0f;
        }

        if(velocity.x == 0.0f && velocity.y == 0.0f){
            moving = false;
        }
    }
}

void Ball::shrink(float shrink_factor){
    setScale(getScale().x - shrink_factor, getScale().y - shrink_factor);
    setPosition(getPosition().x + shrink_factor / 2.0f, getPosition().y + shrink_factor / 2.0f);
}

void Ball::setVelocity(math::Vector2f velocity){
    this->velocity = velocity;
}

void Ball::setVelocity(float x, float y){
    this->velocity.x = x;
    this->velocity.y = y;
}

void Ball::setVelocity1D(float velocity){
    this->velocity1D = velocity;
}

float Ball::getVelocity1D(){
    return velocity1D;
}

sdl::sdlDirection Ball::collidesWith(Tile& obstacle){
    if(position.x + scale.x < obstacle.getPosition().x){
        return sdl::sdlDirection::SDL_LEFT;
    }

    if(position.x > obstacle.getPosition().x + obstacle.getScale().x){
        return sdl::sdlDirection::SDL_RIGHT;
    }

    if(position.y + scale.y < obstacle.getPosition().y){
        return sdl::sdlDirection::SDL_UP;
    }

    if(position.y > obstacle.getPosition().y + obstacle.getScale().y){
        return sdl::sdlDirection::SDL_DOWN;
    }

    return sdl::sdlDirection::SDL_NONE;
}

void Ball::setMoving(bool moving){
    this->moving = moving;
}

bool Ball::isMoving() const {
    return moving;
}

SDL_FRect Ball::getRect() {
    return {getPosition().x, getPosition().y, getScale().x, getScale().y};
}

math::Vector2f& Ball::getVelocity(){
    return velocity;
}