#pragma once
#define PLAYER_IMG "textures/player.png"
#ifndef PLAYER_H
#define PLAYER_H

#include "include/SDL.h"
#include <string>

class World; // Forward declaration for collision detection

class Player {
public:
    Player(SDL_Renderer* renderer, const std::string& imagePath, float startX, float startY, int w, int h);
    ~Player();
    SDL_Rect GetRect() const;
    void HandleInput(const Uint8* state);
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);
    void ApplyDamage(int damage);
    bool IsDead() const;
    float GetX() const;
    float GetY() const;
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    void HandleCollision(const World& platform); // Handle collision with platforms
    void ApplyGravity(float deltaTime); // Apply gravity to the player
    void SetPosition(float newX, float newY);
    void SetVelocityY(float newVy);
    float vx, vy;
    float ax, ay;
private:
    float x, y;
    SDL_Texture* texture;
    SDL_Rect destRect;
    int width, height;
    float speed;
    bool isJumping;
    bool isOnGround; // Check if the player is on a platform
    int health;
};

#endif // PLAYER_H
