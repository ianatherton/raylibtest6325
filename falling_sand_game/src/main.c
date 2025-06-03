#include "raylib.h"
#include <stdlib.h> // For GetRandomValue, though raylib.h might include it

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define CELL_SIZE 5
#define GRID_WIDTH (SCREEN_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / CELL_SIZE)

typedef enum {
    EMPTY = 0,
    SAND
} CellType;

CellType grid[GRID_HEIGHT][GRID_WIDTH];

void InitGrid(void) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[y][x] = EMPTY;
        }
    }
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Falling Sand Game - Click to add sand, Right-click to erase");
    InitGrid();
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        Vector2 mousePos = GetMousePosition();
        int gridX = mousePos.x / CELL_SIZE;
        int gridY = mousePos.y / CELL_SIZE;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT) {
                grid[gridY][gridX] = SAND;
            }
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT) {
                grid[gridY][gridX] = EMPTY;
            }
        }

        // Simulate sand falling
        for (int y = GRID_HEIGHT - 2; y >= 0; y--) { // Iterate from bottom-up, excluding last row (nothing below it)
            for (int x = 0; x < GRID_WIDTH; x++) {
                if (grid[y][x] == SAND) {
                    // Try to move straight down
                    if (grid[y+1][x] == EMPTY) {
                        grid[y+1][x] = SAND;
                        grid[y][x] = EMPTY;
                    } else {
                        // Blocked below, try to move diagonally
                        bool canMoveLeft = (x > 0 && grid[y+1][x-1] == EMPTY);
                        bool canMoveRight = (x < GRID_WIDTH - 1 && grid[y+1][x+1] == EMPTY);

                        if (canMoveLeft && canMoveRight) {
                            if (GetRandomValue(0, 1) == 0) { // Randomly choose left or right
                                grid[y+1][x-1] = SAND;
                                grid[y][x] = EMPTY;
                            } else {
                                grid[y+1][x+1] = SAND;
                                grid[y][x] = EMPTY;
                            }
                        } else if (canMoveLeft) {
                            grid[y+1][x-1] = SAND;
                            grid[y][x] = EMPTY;
                        } else if (canMoveRight) {
                            grid[y+1][x+1] = SAND;
                            grid[y][x] = EMPTY;
                        }
                    }
                }
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(DARKGRAY); // Changed background to see empty cells if needed

            for (int y = 0; y < GRID_HEIGHT; y++) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    if (grid[y][x] == SAND) {
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, YELLOW);
                    }
                }
            }
            DrawText("LMB: Add Sand | RMB: Erase Sand", 10, 10, 20, RAYWHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();
    return 0;
}
