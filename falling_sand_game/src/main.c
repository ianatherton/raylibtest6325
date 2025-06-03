#include "raylib.h"
#include <stdlib.h> // For GetRandomValue
#include <stdio.h>  // For sprintf

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define CELL_SIZE 5
#define GRID_WIDTH (SCREEN_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / CELL_SIZE)

typedef enum {
    EMPTY = 0,
    SAND,
    WATER
} CellType;

CellType grid[GRID_HEIGHT][GRID_WIDTH];
CellType currentBrushType = SAND;

void InitGrid(void) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[y][x] = EMPTY;
        }
    }
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Falling Sand Game - Add Water!");
    InitGrid();
    SetTargetFPS(60);

    char uiText[256];

    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        Vector2 mousePos = GetMousePosition();
        int gridX = (int)(mousePos.x / CELL_SIZE);
        int gridY = (int)(mousePos.y / CELL_SIZE);

        // Brush selection
        if (IsKeyPressed(KEY_ONE)) currentBrushType = SAND;
        if (IsKeyPressed(KEY_TWO)) currentBrushType = WATER;

        // Mouse input for placing/erasing particles
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT) {
                grid[gridY][gridX] = currentBrushType;
            }
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT) {
                grid[gridY][gridX] = EMPTY;
            }
        }

        // Simulate particle physics
        for (int y = GRID_HEIGHT - 2; y >= 0; y--) { // Iterate from bottom-up
            for (int x = 0; x < GRID_WIDTH; x++) { // Iterate columns (can alternate direction for more natural flow)
                if (grid[y][x] == SAND) {
                    int nextY = y + 1;

                    if (nextY < GRID_HEIGHT) { // Ensure not at bottom row
                        // Attempt 1: Move/Swap straight down
                        if (grid[nextY][x] == EMPTY) {
                            grid[nextY][x] = SAND;
                            grid[y][x] = EMPTY;
                        } else if (grid[nextY][x] == WATER) {
                            grid[nextY][x] = SAND; // Sand takes water's spot
                            grid[y][x] = WATER;  // Water moves to sand's original spot (swap)
                        } else { // Blocked by SAND or other, try diagonal
                            // Attempt 2: Move/Swap diagonally down
                            int diagLeftX = x - 1;
                            int diagRightX = x + 1;

                            // Check potential diagonal destinations (can be EMPTY or WATER)
                            bool canGoLeft = (diagLeftX >= 0 && (grid[nextY][diagLeftX] == EMPTY || grid[nextY][diagLeftX] == WATER));
                            bool canGoRight = (diagRightX < GRID_WIDTH && (grid[nextY][diagRightX] == EMPTY || grid[nextY][diagRightX] == WATER));

                            if (canGoLeft && canGoRight) { // Both directions possible
                                if (GetRandomValue(0, 1) == 0) { // Choose left
                                    CellType particleInTarget = grid[nextY][diagLeftX];
                                    grid[nextY][diagLeftX] = SAND; // Sand moves to target
                                    grid[y][x] = particleInTarget; // Original spot gets what was in target (EMPTY or WATER)
                                } else { // Choose right
                                    CellType particleInTarget = grid[nextY][diagRightX];
                                    grid[nextY][diagRightX] = SAND;
                                    grid[y][x] = particleInTarget;
                                }
                            } else if (canGoLeft) { // Only left possible
                                CellType particleInTarget = grid[nextY][diagLeftX];
                                grid[nextY][diagLeftX] = SAND;
                                grid[y][x] = particleInTarget;
                            } else if (canGoRight) { // Only right possible
                                CellType particleInTarget = grid[nextY][diagRightX];
                                grid[nextY][diagRightX] = SAND;
                                grid[y][x] = particleInTarget;
                            }
                            // If neither diagonal move is possible, sand stays put.
                        }
                    }
                } else if (grid[y][x] == WATER) {
                    // 1. Try to move down
                    if (y + 1 < GRID_HEIGHT && grid[y+1][x] == EMPTY) {
                        grid[y+1][x] = WATER;
                        grid[y][x] = EMPTY;
                    } else {
                        // 2. Try to move diagonally down
                        bool canFallLeft = (y + 1 < GRID_HEIGHT && x > 0 && grid[y+1][x-1] == EMPTY);
                        bool canFallRight = (y + 1 < GRID_HEIGHT && x < GRID_WIDTH - 1 && grid[y+1][x+1] == EMPTY);

                        if (canFallLeft && canFallRight) {
                            if (GetRandomValue(0, 1) == 0) grid[y+1][x-1] = WATER;
                            else grid[y+1][x+1] = WATER;
                            grid[y][x] = EMPTY;
                        } else if (canFallLeft) {
                            grid[y+1][x-1] = WATER;
                            grid[y][x] = EMPTY;
                        } else if (canFallRight) {
                            grid[y+1][x+1] = WATER;
                            grid[y][x] = EMPTY;
                        } else {
                            // 3. Try to move horizontally
                            bool canFlowLeft = (x > 0 && grid[y][x-1] == EMPTY);
                            bool canFlowRight = (x < GRID_WIDTH - 1 && grid[y][x+1] == EMPTY);

                            if (canFlowLeft && canFlowRight) {
                                if (GetRandomValue(0, 1) == 0) grid[y][x-1] = WATER;
                                else grid[y][x+1] = WATER;
                                grid[y][x] = EMPTY;
                            } else if (canFlowLeft) {
                                grid[y][x-1] = WATER;
                                grid[y][x] = EMPTY;
                            } else if (canFlowRight) {
                                grid[y][x+1] = WATER;
                                grid[y][x] = EMPTY;
                            }
                        }
                    }
                }
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(DARKGRAY);

            for (int y = 0; y < GRID_HEIGHT; y++) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    if (grid[y][x] == SAND) {
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, YELLOW);
                    } else if (grid[y][x] == WATER) {
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLUE);
                    }
                }
            }
            sprintf(uiText, "LMB: Add | RMB: Erase | Brush: %s | [1] Sand [2] Water | FPS: %d", 
                    (currentBrushType == SAND) ? "SAND" : "WATER", GetFPS());
            DrawText(uiText, 10, 10, 20, RAYWHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();
    return 0;
}
