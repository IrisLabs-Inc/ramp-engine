#include <SDL3/SDL.h>

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;
};

struct Body {
    Vec2 position;
    Vec2 velocity;
    Vec2 size;
};

struct Player {
    Body body;
    int facingDirection = 1; // -1 = stanga, 1 = dreapta
    bool grounded = false;
};

float moveToward(float value, float target, float amount) {
    if (value < target) {
        value += amount;
        if (value > target) {
            value = target;
        }
    } else if (value > target) {
        value -= amount;
        if (value < target) {
            value = target;
        }
    }

    return value;
}

float clamp(float value, float minValue, float maxValue) {
    if (value < minValue) {
        return minValue;
    }

    if (value > maxValue) {
        return maxValue;
    }

    return value;
}

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Simple2D", 800, 600, 0);
    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Player player;
    player.body.position = { 380.0f, 100.0f };
    player.body.velocity = { 0.0f, 0.0f };
    player.body.size = { 40.0f, 60.0f };

    const float gravity = 1400.0f;
    const float acceleration = 2200.0f;
    const float friction = 2600.0f;
    const float maxSpeed = 320.0f;
    const float jumpSpeed = 620.0f;
    const float floorY = 540.0f;

    bool running = true;
    SDL_Event event;

    Uint64 lastTime = SDL_GetPerformanceCounter();
    const double frequency = static_cast<double>(SDL_GetPerformanceFrequency());

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        Uint64 currentTime = SDL_GetPerformanceCounter();
        float dt = static_cast<float>((currentTime - lastTime) / frequency);
        lastTime = currentTime;

        if (dt > 0.033f) {
            dt = 0.033f;
        }

        const bool* keys = SDL_GetKeyboardState(nullptr);

        float inputDirection = 0.0f;

        if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]) {
            inputDirection -= 1.0f;
        }

        if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) {
            inputDirection += 1.0f;
        }

        if (inputDirection < 0.0f) {
            player.facingDirection = -1;
        }

        if (inputDirection > 0.0f) {
            player.facingDirection = 1;
        }

        if (inputDirection != 0.0f) {
            player.body.velocity.x += inputDirection * acceleration * dt;
            player.body.velocity.x = clamp(player.body.velocity.x, -maxSpeed, maxSpeed);
        } else {
            player.body.velocity.x = moveToward(player.body.velocity.x, 0.0f, friction * dt);
        }

        if (keys[SDL_SCANCODE_SPACE] && player.grounded) {
            player.body.velocity.y = -jumpSpeed;
            player.grounded = false;
        }

        player.body.velocity.y += gravity * dt;

        player.body.position.x += player.body.velocity.x * dt;
        player.body.position.y += player.body.velocity.y * dt;

        player.grounded = false;

        if (player.body.position.y + player.body.size.y > floorY) {
            player.body.position.y = floorY - player.body.size.y;
            player.body.velocity.y = 0.0f;
            player.grounded = true;
        }

        if (player.body.position.x < 0.0f) {
            player.body.position.x = 0.0f;
            player.body.velocity.x = 0.0f;
        }

        if (player.body.position.x + player.body.size.x > 800.0f) {
            player.body.position.x = 800.0f - player.body.size.x;
            player.body.velocity.x = 0.0f;
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 24, 255);
        SDL_RenderClear(renderer);

        SDL_FRect floorRect = {
            0.0f,
            floorY,
            800.0f,
            60.0f
        };

        SDL_SetRenderDrawColor(renderer, 80, 80, 90, 255);
        SDL_RenderFillRect(renderer, &floorRect);

        SDL_FRect playerRect = {
            player.body.position.x,
            player.body.position.y,
            player.body.size.x,
            player.body.size.y
        };

        SDL_SetRenderDrawColor(renderer, 220, 80, 80, 255);
        SDL_RenderFillRect(renderer, &playerRect);

        float eyeX = player.facingDirection == 1
            ? player.body.position.x + player.body.size.x - 10.0f
            : player.body.position.x + 4.0f;

        SDL_FRect eyeRect = {
            eyeX,
            player.body.position.y + 14.0f,
            6.0f,
            6.0f
        };

        SDL_SetRenderDrawColor(renderer, 255, 230, 120, 255);
        SDL_RenderFillRect(renderer, &eyeRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

