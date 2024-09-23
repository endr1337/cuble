#pragma once

#include "include/SDL.h"
#include <string>

class Bullet {
public:
    Bullet(SDL_Renderer* renderer, const std::string& spriteSheetPath, float startX, float startY, int w, int h, float playerX, float playerY);
    ~Bullet();

    bool IsOffScreen() const;
    bool IsTouching(const SDL_Rect& rect) const;
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer) const;
    void SetPosition(float newX, float newY);
    bool IsDead() const;

private:
    SDL_Texture* texture;
    SDL_Rect srcRect;
    SDL_Rect destRect;
    float x, y;
    float vx, vy; // Velocity towards the player
    float frameTime;
    float frameTimer;
    int currentFrame;
    int numFrames;
    int width, height;
    bool isDead;
};
