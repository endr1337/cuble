#include "player.h"
#include "world.h"
#include "include/SDL_image.h"
#include <iostream>
#include <Windows.h>

Player::Player(SDL_Renderer* renderer, const std::string& imagePath, float startX, float startY, int w, int h)
    : x(startX), y(startY), width(w), height(h), vx(0.0f), vy(0.0f), ax(0.0f), ay(300.0f), speed(200.0f), isJumping(false), isOnGround(false), health(100)
{
    texture = IMG_LoadTexture(renderer, imagePath.c_str());
    if (!texture) {
        std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
    }

    destRect = { static_cast<int>(x), static_cast<int>(y), width, height };
}
void Player::SetPosition(float newX, float newY) {
    x = newX;
    destRect.x = static_cast<int>(x);
    destRect.y = static_cast<int>(y);
}

void Player::SetVelocityY(float newVy) {
    vy = newVy;
}

Player::~Player() {
    SDL_DestroyTexture(texture);
}

void Player::HandleInput(const Uint8* state) {
    vx = 0;

    if (state[SDL_SCANCODE_LEFT]) {
        vx = -speed;
    }

    if (state[SDL_SCANCODE_RIGHT]) {
        vx = speed;
    }

    if (state[SDL_SCANCODE_SPACE] && !isJumping) {
        vy = -250.0f;
        isJumping = true;
        isOnGround = false;
    }
}

void Player::Update(float deltaTime) {
    ApplyGravity(deltaTime);

    x += vx * deltaTime;
    y += vy * deltaTime;

    destRect.x = static_cast<int>(x);
    destRect.y = static_cast<int>(y);

    if (y + height > 600) {
        y = 600 - height;
        if (vy > 300.0f) {
            int damage = static_cast<int>((vy - 300) / 10);
            ApplyDamage(damage);
        }
        vy = 0;
        isJumping = false;
    }
    std::cout << "Is on ground: " << isOnGround << std::endl;
    std::cout << "Is jumping: " << isJumping << std::endl;
    std::cout << "Vertical velocity: " << vy << std::endl;
    if (x + width > 800) {
        x = 800 - width;
        vx = 0;
    }
    else if (x < 0) {
        x = 0;
        vx = 0;
    }
}

void Player::ApplyGravity(float deltaTime) {
    if (!isOnGround) {
        vy += ay * deltaTime;
    }
}

SDL_Rect Player::GetRect() const {
    return destRect;
}

void Player::Render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}

void Player::ApplyDamage(int damage) {
    health -= damage;
    std::cout << health << std::endl;
    if (health <= 0) {

        std::cout << "Player has died!" << std::endl;
        health = 0;
        Sleep(500);
        exit(0);
        
    }
}

float Player::GetX() const {
    return x;
}

float Player::GetY() const {
    return y;
}

bool Player::IsDead() const {
    return health <= 0;
    exit(0);
}


void Player::HandleCollision(const World& platform) {
    SDL_Rect playerRect = { static_cast<int>(x), static_cast<int>(y), width, height };
    SDL_Rect platformRect = platform.GetRect();

    bool wasOnGround = isOnGround; // Track the previous state

    // Check for collision
    if (SDL_HasIntersection(&playerRect, &platformRect)) {
        // Player falling onto the platform
        if (vy > 0 && (y + height) > platformRect.y && (y + height) <= (platformRect.y + 10)) {
            y = platformRect.y - height; 
            vy = 0; 
            isOnGround = true;
        }
    }
    else {
        isOnGround = false; // Player is not on the platform
    }

    // Reset jump status if player is not on ground anymore
    if (wasOnGround && !isOnGround) {
        isJumping = false; // Allow the player to jump again
    }
    // Update the destination rectangle
    destRect.x = static_cast<int>(x);
    destRect.y = static_cast<int>(y);
}
