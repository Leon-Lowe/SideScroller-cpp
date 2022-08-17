/*
    * main.cpp
    * A game where you must jump over obstacles
    * in order to get a new high score
    * Made by Leon Lowe
*/

#include "raylib.h"

struct EntityData
{
    Rectangle textureRect;
    Vector2 position;
    Vector2 velocity;
    Vector2 animFrame;
    Vector2 maxAnimFrame;
    float animUpdateTime;
    float animRunningTime;
};

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

    /*
        * Player Creation
    */

    //Initialise player texture
    Texture2D playerTexture = LoadTexture("textures/scarfy.png");
    EntityData playerData{
        {0, 0, playerTexture.width/6, playerTexture.height},                                        /*Rectangle - Texture Rect*/
        {(WINDOW_WIDTH/2) - ((playerTexture.width/6) / 2), WINDOW_HEIGHT - playerTexture.height},   /*Vector2 - Position*/
        {0, 0},                                                                                     /*Vector2 - Velocity*/
        {0, 0},                                                                                     /*Vector2 - Animation Frame*/
        {5,0},                                                                                      /*Vector2 - Max Animation Frame*/
        1.0 / 12.0,                                                                                 /*Float - Animation Update Time*/
        0.0                                                                                         /*Float - Animation Running Time*/
    };

    //Initialise player jump variables
    const int PLAYER_JUMP_FORCE = 600; //Pixels per second
    bool playerIsInAir = false;

    /*
        * Nebula Creation
    */

    //Initialise nebula texture
    Texture2D nebulaTexture = LoadTexture("textures/12_nebula_spritesheet.png");
    EntityData nebulaData{
        {0, 0, nebulaTexture.width/8, nebulaTexture.height/8},      /*Rectangle - Texture Rect*/
        {WINDOW_WIDTH, WINDOW_HEIGHT - nebulaTexture.height/8},     /*Vector2 - Position*/
        {-600, 0},                                                  /*Vector2 - Velocity*/
        {0, 0},                                                     /*Vector2 - Animation Frame*/
        {7, 6},                                                     /*Vector2 - Max Animation Frame*/
        1.0 / 12.0,                                                 /*Float - Animation Update Time*/
        0.0                                                         /*Float - Animation Running Time*/
    };

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
        if(playerData.position.y >= WINDOW_HEIGHT - playerData.textureRect.height)
        {
            playerData.velocity.y = 0;
            playerData.position.y = WINDOW_HEIGHT - playerData.textureRect.height;
            playerIsInAir = false;
        }
        else
        {
            //Apply gravity to player
            playerData.velocity.y += GRAVITY * DELTA_TIME;
            playerIsInAir = true;
        }

        //Player jump check
        if(IsKeyPressed(KEY_SPACE) && !playerIsInAir)
        {
            playerData.velocity.y -= PLAYER_JUMP_FORCE;
        }

        //Update player position
        playerData.position.y += playerData.velocity.y * DELTA_TIME;

        //Update nebula position
        nebulaData.position.x += nebulaData.velocity.x * DELTA_TIME;

        //Update player animation frame
        if(!playerIsInAir)
        {
            playerData.animRunningTime += DELTA_TIME;
            if(playerData.animRunningTime >= playerData.animUpdateTime)
            {
                playerData.animRunningTime = 0;
                playerData.textureRect.x = playerData.animFrame.x * playerData.textureRect.width;
                playerData.textureRect.y = playerData.animFrame.y * playerData.textureRect.height;
                playerData.animFrame.x++;
                if(playerData.animFrame.x > playerData.maxAnimFrame.x)
                {
                    playerData.animFrame.x = 0;
                    playerData.animFrame.y++;
                }
                if(playerData.animFrame.y > playerData.maxAnimFrame.y)
                {
                    playerData.animFrame.y = 0;
                }
            }
        }

        //Update nebula animation frame
        nebulaData.animRunningTime += DELTA_TIME;
        if(nebulaData.animRunningTime >= nebulaData.animUpdateTime)
        {
            nebulaData.animRunningTime = 0;
            nebulaData.textureRect.x = nebulaData.animFrame.x * nebulaData.textureRect.width;
            nebulaData.textureRect.y = nebulaData.animFrame.y * nebulaData.textureRect.height;
            nebulaData.animFrame.x++;
            if(nebulaData.animFrame.x > 7)
            {
                nebulaData.animFrame.x = 0;
                nebulaData.animFrame.y++;
            }
            if(nebulaData.animFrame.y > 6)
            {
                nebulaData.animFrame.y = 0;
            }
        }

        //Draw nebula
        DrawTextureRec(nebulaTexture, nebulaData.textureRect, nebulaData.position, WHITE);

        //Draw player
        DrawTextureRec(playerTexture, playerData.textureRect, playerData.position, WHITE);

        EndDrawing();
    }
    //Unload textures
    UnloadTexture(playerTexture);
    UnloadTexture(nebulaTexture);

    //Close the game
    CloseWindow();

    return 0;
}