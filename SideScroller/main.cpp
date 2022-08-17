/*
    * main.cpp
    * A game where you must jump over obstacles
    * in order to get a new high score
    * Made by Leon Lowe
*/

#include "raylib.h"

int main()
{
    //Window variabes
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
    const char* WINDOW_NAME = "Dapper Dasher";

    //Initialise window and set traget fps
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
    SetTargetFPS(60);

    const int GRAVITY = 1;

    //Rectangle variables
    const int RECT_WIDTH = 50;
    const int RECT_HEIGHT = 80;
    int rectPosY = WINDOW_HEIGHT - RECT_HEIGHT;
    int rectVelocity = 0;
    const int RECT_JUMP_FORCE = 22;
    bool isInAir = false;

    bool running = true;

    //Main game loop
    while(running)
    {
        if(WindowShouldClose())
        {
            running = false;
        }

        BeginDrawing();

        ClearBackground(WHITE);

        //Ground check
        if(rectPosY >= WINDOW_HEIGHT - RECT_HEIGHT)
        {
            rectVelocity = 0;
            rectPosY = WINDOW_HEIGHT - RECT_HEIGHT;
            isInAir = false;
        }
        else
        {
            //Apply gravity
            rectVelocity += GRAVITY;
            isInAir = true;
        }

        //Jump check
        if(IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            rectVelocity -= RECT_JUMP_FORCE;
        }

        //Update position
        rectPosY += rectVelocity;

        DrawRectangle((WINDOW_WIDTH/2) - (RECT_WIDTH/2), rectPosY, RECT_WIDTH, RECT_HEIGHT, BLUE);

        EndDrawing();
    }

    //Close the game
    CloseWindow();

    return 0;
}