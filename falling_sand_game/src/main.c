#include "raylib.h"
#include <stdlib.h> // For GetRandomValue
#include <stdio.h>  // For sprintf
#include <stdbool.h> // For bool type

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
CellType nextGrid[GRID_HEIGHT][GRID_WIDTH]; // Double buffer to prevent processing moved particles multiple times
CellType currentBrushType = SAND;

void InitGrid(void) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[y][x] = EMPTY;
            nextGrid[y][x] = EMPTY;
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

        // Copy current grid to next grid for double buffering
        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                nextGrid[y][x] = grid[y][x];
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
                            nextGrid[nextY][x] = SAND;
                            nextGrid[y][x] = EMPTY;
                        } else if (grid[nextY][x] == WATER) {
                            // Sand displaces water (swap positions)
                            nextGrid[nextY][x] = SAND;
                            nextGrid[y][x] = WATER;
                        } else { // Blocked by SAND or other, try diagonal
                            // Attempt 2: Move/Swap diagonally down
                            int diagLeftX = x - 1;
                            int diagRightX = x + 1;

                            // Randomize direction choice for more natural movement
                            bool tryLeftFirst = GetRandomValue(0, 1) == 0;
                            int firstX = tryLeftFirst ? diagLeftX : diagRightX;
                            int secondX = tryLeftFirst ? diagRightX : diagLeftX;
                            
                            // Try first diagonal direction
                            if (firstX >= 0 && firstX < GRID_WIDTH) {
                                if (grid[nextY][firstX] == EMPTY) {
                                    nextGrid[nextY][firstX] = SAND;
                                    nextGrid[y][x] = EMPTY;
                                    continue;
                                } else if (grid[nextY][firstX] == WATER) {
                                    nextGrid[nextY][firstX] = SAND;
                                    nextGrid[y][x] = WATER;
                                    continue;
                                }
                            }
                            
                            // Try second diagonal direction
                            if (secondX >= 0 && secondX < GRID_WIDTH) {
                                if (grid[nextY][secondX] == EMPTY) {
                                    nextGrid[nextY][secondX] = SAND;
                                    nextGrid[y][x] = EMPTY;
                                } else if (grid[nextY][secondX] == WATER) {
                                    nextGrid[nextY][secondX] = SAND;
                                    nextGrid[y][x] = WATER;
                                }
                            }
                            // If neither diagonal move is possible, sand stays put
                        }
                    }
                } else if (grid[y][x] == WATER) {
                    // Only process water if it hasn't been affected by sand in this frame
                    if (nextGrid[y][x] == WATER) {
                        // 1. Try to move down
                        if (y + 1 < GRID_HEIGHT && grid[y+1][x] == EMPTY && nextGrid[y+1][x] == EMPTY) {
                            nextGrid[y+1][x] = WATER;
                            nextGrid[y][x] = EMPTY;
                        } else {
                            // 2. Try to move diagonally down
                            bool canFallLeft = (y + 1 < GRID_HEIGHT && x > 0 && 
                                               grid[y+1][x-1] == EMPTY && nextGrid[y+1][x-1] == EMPTY);
                            bool canFallRight = (y + 1 < GRID_HEIGHT && x < GRID_WIDTH - 1 && 
                                                grid[y+1][x+1] == EMPTY && nextGrid[y+1][x+1] == EMPTY);

                            if (canFallLeft && canFallRight) {
                                int direction = GetRandomValue(0, 1);
                                if (direction == 0) {
                                    nextGrid[y+1][x-1] = WATER;
                                } else {
                                    nextGrid[y+1][x+1] = WATER;
                                }
                                nextGrid[y][x] = EMPTY;
                            } else if (canFallLeft) {
                                nextGrid[y+1][x-1] = WATER;
                                nextGrid[y][x] = EMPTY;
                            } else if (canFallRight) {
                                nextGrid[y+1][x+1] = WATER;
                                nextGrid[y][x] = EMPTY;
                            } else {
                                // 3. Try to move horizontally
                                bool canFlowLeft = (x > 0 && grid[y][x-1] == EMPTY && nextGrid[y][x-1] == EMPTY);
                                bool canFlowRight = (x < GRID_WIDTH - 1 && grid[y][x+1] == EMPTY && nextGrid[y][x+1] == EMPTY);

                                if (canFlowLeft && canFlowRight) {
                                    int direction = GetRandomValue(0, 1);
                                    if (direction == 0) {
                                        nextGrid[y][x-1] = WATER;
                                    } else {
                                        nextGrid[y][x+1] = WATER;
                                    }
                                    nextGrid[y][x] = EMPTY;
                                } else if (canFlowLeft) {
                                    nextGrid[y][x-1] = WATER;
                                    nextGrid[y][x] = EMPTY;
                                } else if (canFlowRight) {
                                    nextGrid[y][x+1] = WATER;
                                    nextGrid[y][x] = EMPTY;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        // Swap the grids (update the current grid to the next state)
        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                grid[y][x] = nextGrid[y][x];
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
