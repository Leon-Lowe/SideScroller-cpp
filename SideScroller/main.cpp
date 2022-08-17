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

    const int GRAVITY = 1000; //(Pixels per second) per second

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

    //Initialise player animation
    int playerFrame = 0;
    const float PLAYER_ANIM_UPDATE_TIME = 1.0 / 12.0;
    float playerAnimRunningTime = 0;

    //Initialise player variables
    const int PLAYER_JUMP_FORCE = 600; //Pixels per second
    bool playerIsInAir = false;

    bool running = true;

    //Main game loop
    while(running)
    {
        if(WindowShouldClose())
        {
            running = false;
        }

        //Delta time (time since last frame)
        const float DELTA_TIME = GetFrameTime();

        BeginDrawing();

        ClearBackground(WHITE);

        //Player ground check
        if(playerPosition.y >= WINDOW_HEIGHT - playerTextureRect.height)
        {
            playerVelocity.y = 0;
            playerPosition.y = WINDOW_HEIGHT - playerTextureRect.height;
            playerIsInAir = false;
        }
        else
        {
            //Apply gravity to player
            playerVelocity.y += GRAVITY * DELTA_TIME;
            playerIsInAir = true;
        }

        //Player jump check
        if(IsKeyPressed(KEY_SPACE) && !playerIsInAir)
        {
            playerVelocity.y -= PLAYER_JUMP_FORCE;
        }

        //Update player position
        playerPosition.y += playerVelocity.y * DELTA_TIME;

        //Update player animation frame
        playerAnimRunningTime += DELTA_TIME;
        if(playerAnimRunningTime >= PLAYER_ANIM_UPDATE_TIME)
        {
            playerAnimRunningTime = 0;
            playerTextureRect.x = playerFrame * playerTextureRect.width;
            playerFrame++;
            if(playerFrame > 5)
            {
                playerFrame = 0;
            }
        }

        //Draw player
        DrawTextureRec(playerTexture, playerTextureRect, playerPosition, WHITE);

        EndDrawing();
    }
    //Unload textures
    UnloadTexture(playerTexture);

    //Close the game
    CloseWindow();

    return 0;
}