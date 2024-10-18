#include "bullet.h"
#include "include/SDL_image.h"
#include <iostream>
#include <cmath>
//i think i need vector???? i really hate this 
Bullet::Bullet(SDL_Renderer* renderer, const std::string& spriteSheetPath, float startX, float startY, int w, int h, float playerX, float playerY)
    : x(startX), y(startY), width(w), height(h), frameTime(0.1f), frameTimer(0.0f), currentFrame(0), numFrames(4), isDead(false)
{
    texture = IMG_LoadTexture(renderer, spriteSheetPath.c_str());
    if (!texture) {
        std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
    }

    srcRect = { 0, 0, width, height };
    destRect = { static_cast<int>(x), static_cast<int>(y), width, height };

    // Calculate the direction vector towards the player
    float dx = playerX - x; 
    float dy = playerY - y;

    // Calculate the distance between the bullet and the player
    float distance = std::sqrt(dx * dx + dy * dy);

    // If distance is very small, prevent division by zero
    if (distance > 0.0f) {
        // Normalize and set velocity to move towards the player
        vx = (dx / distance) * 300.0f;  // Speed of 300 units per second
        vy = (dy / distance) * 300.0f;
    }
    else {
        vx = vy = 0.0f; // Bullet is already at the player's location
    }
}

Bullet::~Bullet() {
    SDL_DestroyTexture(texture);
}

bool Bullet::IsOffScreen() const {
    return x < 0 || x > 800 || y < 0 || y > 600;
}

bool Bullet::IsTouching(const SDL_Rect& rect) const {
    SDL_Rect bulletRect = { static_cast<int>(x), static_cast<int>(y), width, height };
    return SDL_HasIntersection(&bulletRect, &rect) != SDL_FALSE;
}

void Bullet::SetPosition(float newX, float newY) {
    x = newX;
    y = newY;
    destRect.x = static_cast<int>(x);
    destRect.y = static_cast<int>(y);
}

void Bullet::Update(float deltaTime) {
    // Update position based on velocity
    x += vx * deltaTime;
    y += vy * deltaTime;

    destRect.x = static_cast<int>(x);
    destRect.y = static_cast<int>(y);

    // Update animation frame
    frameTimer += deltaTime;
    if (frameTimer >= frameTime) {
        frameTimer = 0.0f;
        currentFrame = (currentFrame + 1) % numFrames;
        srcRect.x = currentFrame * width;
    }

    // Check if bullet is off-screen
    if (IsOffScreen()) {
        isDead = true;
    }
}

void Bullet::Render(SDL_Renderer* renderer) const {
    if (!isDead) {
        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
    }
}

bool Bullet::IsDead() const {
    return isDead;
}
