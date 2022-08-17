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

    //Initialise player texture
    Texture2D playerTexture = LoadTexture("textures/scarfy.png");

    //Initialise player tecture rect
    Rectangle playerTextureRect;
    playerTextureRect.width = playerTexture.width/6;
    playerTextureRect.height = playerTexture.height;
    playerTextureRect.x = 0;
    playerTextureRect.y = 0;

    //Initialise player position
    Vector2 playerPosition;
    playerPosition.x = WINDOW_WIDTH/2 - playerTextureRect.width/2;
    playerPosition.y = WINDOW_HEIGHT - playerTextureRect.height;

    //Initialise player velocity
    Vector2 playerVelocity;
    playerVelocity.x = 0;
    playerVelocity.y = 0;

    //Initialise player variables
    const int PLAYER_JUMP_FORCE = 22;
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
        if(playerPosition.y >= WINDOW_HEIGHT - playerTextureRect.height)
        {
            playerVelocity.y = 0;
            playerPosition.y = WINDOW_HEIGHT - playerTextureRect.height;
            isInAir = false;
        }
        else
        {
            //Apply gravity
            playerVelocity.y += GRAVITY;
            isInAir = true;
        }

        //Jump check
        if(IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            playerVelocity.y -= PLAYER_JUMP_FORCE;
        }

        //Update position
        playerPosition.y += playerVelocity.y;

        DrawTextureRec(playerTexture, playerTextureRect, playerPosition, WHITE);

        EndDrawing();
    }

    //Close the game
    CloseWindow();

    return 0;
}