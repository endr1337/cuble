#include "player.h"
#include "bullet.h"
#include "world.h"
#include "include/SDL.h"
#include "include/SDL_image.h"
#include <iostream>
#include <vector>
#include <cstdlib> 
#include <ctime>  
#include <algorithm> 

// Function to spawn bullets
void SpawnBullet(SDL_Renderer* renderer, const Player& player, std::vector<Bullet>& bullets) {
    float startX = static_cast<float>(rand() % 800);
    float startY = static_cast<float>(rand() % 600);

    float distanceX = std::abs(startX - player.GetX());
    float distanceY = std::abs(startY - player.GetY());

    if (distanceX < 100 && distanceY < 100) {
        return; // Prevent immediate collision
    }

    bullets.emplace_back(renderer, "textures/bullet.png", startX, startY, 45, 45, player.GetX(), player.GetY());
}
// Function to spawn new platforms
void SpawnPlatform(std::vector<World>& platforms, float playerX, const Player& player) {
    float newY = static_cast<float>(rand() % 600);
    float newX = platforms.empty() ? 800 : platforms.back().GetX() + rand() % 200 + 200; // Randomize distance
    Uint8 r = rand() % 234;
    Uint8 g = rand() % 235;
    Uint8 b = rand() % 225;
    float distanceX = std::abs(newX - player.GetX());
    float distanceY = std::abs(newY - player.GetY());
    if (distanceX > 1.5 && distanceY > 6.5) {
        return;
    }
    if (platforms.empty() || platforms.back().GetX() < playerX + 800) { // Spawn in front of the player
        platforms.emplace_back(newX, newY, 200, 50, SDL_Color{ r, g, b, 255}); // Adjust platform size and position as needed
    }
}
//#workhardernotsmarter this is not optimized at all i hate my life so much im going to kill myself :pensive:
int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::vector<Bullet> bullets;
    std::vector<World> platforms;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
        return -1;
    }

    window = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return -1;
    }

    Player player(renderer, "/textures/player.png", 160.0f, 100.0f, 50, 50);

    srand(static_cast<unsigned>(time(0)));

    Uint32 lastTime = SDL_GetTicks();
    bool running = true;
    SDL_Event event;

    // Initial platform spawn
    platforms.emplace_back(100, 500, 600, 50, SDL_Color{ 255, 0, 0, 255 });

    while (running) {
        // Event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        const Uint8* state = SDL_GetKeyboardState(nullptr);
        player.HandleInput(state);

        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        player.Update(deltaTime);

        // Platform collision check and updating
        // Inside your main loop
        for (auto& platform : platforms) {
            platform.Update(deltaTime); // Update platform position
            player.HandleCollision(platform); // Check collision here
        }

        // Spawn platforms ahead of the player
        SpawnPlatform(platforms, player.GetX(), player);

        // Remove platforms that are far behind the player
        platforms.erase(std::remove_if(platforms.begin(), platforms.end(), [&](const World& platform) {
            return platform.GetX() < player.GetX() - 800; // Off-screen behind the player
            }), platforms.end());

        // Bullet spawning every 2 seconds
        if (currentTime % 2000 < 16) {
            SpawnBullet(renderer, player, bullets);
        }

        // Update bullets
       /*
        for (auto it = bullets.begin(); it != bullets.end();) {
            it->Update(deltaTime);
            if (it->IsOffScreen()) {
                it = bullets.erase(it);
            }
            else {
                if (it->IsTouching(player.GetRect())) {
                    player.ApplyDamage(20);
                    it = bullets.erase(it);
                }
                else {
                    ++it;
                }
            }
        }*/

        // Render
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        player.Render(renderer);
        for (const auto& bullet : bullets) {
            bullet.Render(renderer);
        }
        for (const auto& platform : platforms) {
            platform.Render(renderer);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
