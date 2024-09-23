#include "world.h"
#include "include/SDL.h"

// Constructor definition
World::World(float startX, float startY, float w, float h, SDL_Color color)
    : x(startX), y(startY), width(w), height(h), color(color), speed(100.0f) {} // Example speed
// Getter methods
float World::GetX() const { return x; }
float World::GetY() const { return y; }
float World::GetWidth() const { return width; }
float World::GetHeight() const { return height; }

// GetRect method
SDL_Rect World::GetRect() const {
    SDL_Rect rect = { static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height) };
    return rect;
}
void World::Update(float deltaTime) {
    x += -speed * deltaTime; // Move the platform
    // Optionally add logic to reverse direction if needed
}

// Render method
void World::Render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); // Ensure 4 parameters for color
    SDL_Rect rect = { static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height) };
    SDL_RenderFillRect(renderer, &rect);
}
