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

    /* 
       * Animate()
       * Takes a float called 'time'
       * Animates the current entity by increasing both
       * the x and y of the animation frames depending on
       * what frame the entity is currently on
    */

    void Animate(float time)
    {
        animRunningTime += time;
        if(animRunningTime >= animUpdateTime)
        {
            animRunningTime = 0;
            textureRect.x = animFrame.x * textureRect.width;
            textureRect.y = animFrame.y * textureRect.height;
            animFrame.x++;
            if(animFrame.x > maxAnimFrame.x)
            {
                animFrame.x = 0;
                animFrame.y++;
            }
            if(animFrame.y > maxAnimFrame.y)
            {
                animFrame.y = 0;
            }
        }
    }
};

int main()
{
    //Window variabes
    const Vector2 WINDOW_DIMENSIONS{1280, 720};
    const char* WINDOW_NAME = "Dapper Dasher";

    //Initialise window and set traget fps
    InitWindow(WINDOW_DIMENSIONS.x, WINDOW_DIMENSIONS.y, WINDOW_NAME);
    SetTargetFPS(60);

    const int GRAVITY = 1000; //(Pixels per second) per second

    /*
        * Player Creation
    */

    //Initialise player texture
    Texture2D playerTexture = LoadTexture("textures/scarfy.png");
    EntityData playerData{
        {0, 0, playerTexture.width/6, playerTexture.height},                                                     /*Rectangle - Texture Rect*/
        {(WINDOW_DIMENSIONS.x/2) - ((playerTexture.width/6) / 2), WINDOW_DIMENSIONS.y - playerTexture.height},   /*Vector2 - Position*/
        {0, 0},                                                                                                  /*Vector2 - Velocity*/
        {0, 0},                                                                                                  /*Vector2 - Animation Frame*/
        {5,0},                                                                                                   /*Vector2 - Max Animation Frame*/
        1.0 / 12.0,                                                                                              /*Float - Animation Update Time*/
        0.0                                                                                                      /*Float - Animation Running Time*/
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
        {0, 0, nebulaTexture.width/8, nebulaTexture.height/8},                   /*Rectangle - Texture Rect*/
        {WINDOW_DIMENSIONS.x, WINDOW_DIMENSIONS.y - nebulaTexture.height/8},     /*Vector2 - Position*/
        {-600, 0},                                                               /*Vector2 - Velocity*/
        {0, 0},                                                                  /*Vector2 - Animation Frame*/
        {7, 6},                                                                  /*Vector2 - Max Animation Frame*/
        1.0 / 12.0,                                                              /*Float - Animation Update Time*/
        0.0                                                                      /*Float - Animation Running Time*/
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
        if(playerData.position.y >= WINDOW_DIMENSIONS.y - playerData.textureRect.height)
        {
            playerData.velocity.y = 0;
            playerData.position.y = WINDOW_DIMENSIONS.y - playerData.textureRect.height;
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

        //Update player animations
        if(!playerIsInAir)
        {
            playerData.Animate(DELTA_TIME);
        }

        //Update nebula animations
        nebulaData.Animate(DELTA_TIME);

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