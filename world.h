#ifndef WORLD_H
#define WORLD_H

#include "include/SDL.h"

class World {
private:
    float x, y;
    float width, height;
    float speed; // Speed of the platform
    SDL_Color color;

public:
    World(float startX, float startY, float w, float h, SDL_Color c);

    float GetX() const;
    float GetY() const;
    float GetWidth() const;
    float GetHeight() const;
    void Update(float deltaTime);
    SDL_Rect GetRect() const;

    void Render(SDL_Renderer* renderer) const ;
};

#endif
