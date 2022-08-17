#include "raylib.h"

int main()
{
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
    const char* WINDOW_NAME = "Dapper Dasher";

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME);
    SetTargetFPS(60);

    bool running = true;

    while(running)
    {
        if(WindowShouldClose())
        {
            running = false;
        }

        BeginDrawing();

        ClearBackground(WHITE);

        EndDrawing();
    }

    return 0;
}