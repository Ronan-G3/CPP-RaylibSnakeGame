#include "raylib.h"
#include <vector>

const int cellSize = 20;
const int screenWidth = 800;
const int screenHeight = 600;

enum Direction {
    UP, DOWN, LEFT, RIGHT
};

// Score
int score = 0;
bool gameOver = false;


struct Snake {
    Direction dir = RIGHT;
    std::vector<Vector2> body;
    bool grew = false;
};

int main() {
    InitWindow(screenWidth, screenHeight, "Snake Game with Raylib");
    SetTargetFPS(10);

    Snake snake;
    snake.body.push_back({ 0.0f, 0.0f });

    Vector2 food = { static_cast<float>(5 * cellSize), static_cast<float>(5 * cellSize) };

    while (!WindowShouldClose()) {
        // Input
        if (IsKeyPressed(KEY_UP) && snake.dir != DOWN) snake.dir = UP;
        if (IsKeyPressed(KEY_DOWN) && snake.dir != UP) snake.dir = DOWN;
        if (IsKeyPressed(KEY_LEFT) && snake.dir != RIGHT) snake.dir = LEFT;
        if (IsKeyPressed(KEY_RIGHT) && snake.dir != LEFT) snake.dir = RIGHT;

        // Movement
        for (size_t i = snake.body.size() - 1; i > 0; i--) {
            snake.body[i] = snake.body[i - 1];
        }

        switch (snake.dir) {
        case UP:    snake.body[0].y -= cellSize; break;
        case DOWN:  snake.body[0].y += cellSize; break;
        case LEFT:  snake.body[0].x -= cellSize; break;
        case RIGHT: snake.body[0].x += cellSize; break;
        }

        // Collision with food
        if (snake.body[0].x == food.x && snake.body[0].y == food.y) {
            snake.grew = true;
            food = {
                static_cast<float>(GetRandomValue(0, (screenWidth / cellSize) - 1) * cellSize),
                static_cast<float>(GetRandomValue(0, (screenHeight / cellSize) - 1) * cellSize)
            };
        }

        // Grow snake
        if (snake.grew) {
            Vector2 newSegment = snake.body[snake.body.size() - 1];
            snake.body.push_back(newSegment);
            snake.grew = false;
        }

        // Self collision
        for (size_t i = 1; i < snake.body.size(); i++) {
            if (snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y) {
                // Restart
                snake.body.clear();
                snake.body.push_back({ 0.0f, 0.0f });
                snake.dir = RIGHT;
                food = { static_cast<float>(5 * cellSize), static_cast<float>(5 * cellSize) };
            }
        }

        // Draw everything
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (auto& segment : snake.body) {
            DrawRectangle(static_cast<int>(segment.x), static_cast<int>(segment.y), cellSize, cellSize, DARKGREEN);
        }

        DrawRectangle(static_cast<int>(food.x), static_cast<int>(food.y), cellSize, cellSize, RED);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
