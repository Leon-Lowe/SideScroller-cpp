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

struct BackgroundData
{
    Texture2D texture;
    Vector2 position;
    Vector2 duplicatePosition;
    float scale;
    int scrollVelocity;

    void SetDuplicatePositon()
    {
        Vector2 tempPos{position.x + (texture.width * scale), position.y};
        duplicatePosition = tempPos;
    }

    void Draw()
    {
        DrawTextureEx(texture, position, 0.0, scale, WHITE);
        DrawTextureEx(texture, duplicatePosition, 0.0, scale, WHITE);
    }

    void Scroll(float time)
    {
        position.x += scrollVelocity * time;
        duplicatePosition.x += scrollVelocity * time;
    }

    void CheckForReset()
    {
        if(position.x <= -texture.width * scale)
        {
            position.x = duplicatePosition.x + (texture.width * scale);
        }

        if(duplicatePosition.x <= -texture.width * scale)
        {
            duplicatePosition.x = position.x + (texture.width * scale);
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

    //Create nebulas
    const int NEBULA_SPACING = 900;
    const int NEBULA_AMOUNT = 3;
    EntityData nebulae[NEBULA_AMOUNT]{};

    for(int i = 0; i < NEBULA_AMOUNT; i++)
    {
        nebulae[i].textureRect.x = 0.0;
        nebulae[i].textureRect.y = 0.0;
        nebulae[i].textureRect.width = nebulaTexture.width/8;
        nebulae[i].textureRect.height = nebulaTexture.height/8;

        nebulae[i].position.x = WINDOW_DIMENSIONS.x + (NEBULA_SPACING * i);
        nebulae[i].position.y = WINDOW_DIMENSIONS.y - nebulaTexture.height/8;

        nebulae[i].velocity.x = -600;
        nebulae[i].velocity.y = 0;

        nebulae[i].animFrame.x = 0;
        nebulae[i].animFrame.y = 0;

        nebulae[i].maxAnimFrame.x = 7;
        nebulae[i].maxAnimFrame.y = 6;

        nebulae[i].animUpdateTime = 1.0 / 12.0;
        nebulae[i].animRunningTime = 0.0;
    }

    float finishLineX = nebulae[NEBULA_AMOUNT - 1].position.x + 300;

    /*
        * Background Creation
    */

    //Initialise background textures
    Texture2D backgroundTexture = LoadTexture("textures/far-buildings.png");
    Texture2D midgroundTexture = LoadTexture("textures/back-buildings.png");
    Texture2D foregroundTexture = LoadTexture("textures/foreground.png");

    //Initialise background
    BackgroundData background{
        backgroundTexture,
        {0.0, -240.0},
        {0.0, 0.0},
        5.0,
        -20
    };
    background.SetDuplicatePositon();

    //Initialise midground
    BackgroundData midground{
        midgroundTexture,
        {0.0, -240.0},
        {0.0, 0.0},
        5.0,
        -40
    };
    midground.SetDuplicatePositon();

    //Initialise foreground
    BackgroundData foreground{
        foregroundTexture,
        {0.0, -240.0},
        {0.0, 0.0},
        5.0,
        -80
    };
    foreground.SetDuplicatePositon();

    bool running = true;
    bool collidingWithNeb = false;
    bool crossedFinishedLine = false;

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

        //Draw backgrounds
        background.Draw();
        midground.Draw();
        foreground.Draw();

        //Update background positions
        background.Scroll(DELTA_TIME);
        midground.Scroll(DELTA_TIME);
        foreground.Scroll(DELTA_TIME);

        //Reset background positions
        background.CheckForReset();
        midground.CheckForReset();
        foreground.CheckForReset();

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

        //Update nebulas positions
        for(int i = 0; i < NEBULA_AMOUNT; i++)
        {
            nebulae[i].position.x += nebulae[i].velocity.x * DELTA_TIME; //Move
            nebulae[i].Animate(DELTA_TIME); //Animate
            DrawTextureRec(nebulaTexture, nebulae[i].textureRect, nebulae[i].position, WHITE); //Draw
        }

        //Update finish line position
        finishLineX += nebulae[0].velocity.x * DELTA_TIME;

        //Update player animations
        if(!playerIsInAir)
        {
            playerData.Animate(DELTA_TIME);
        }

        //Check for collision : player - nebula
        for(EntityData neb : nebulae)
        {
            float padding = 20;
            Rectangle nebRect{
                neb.position.x + padding,
                neb.position.y + padding,
                neb.textureRect.width - (2 * padding),
                neb.textureRect.height - (2 * padding)
            };

            Rectangle playerRect{
                playerData.position.x,
                playerData.position.y,
                playerData.textureRect.width,
                playerData.textureRect.height
            };

            if(CheckCollisionRecs(nebRect, playerRect))
            {
                collidingWithNeb = true;
            }
        }

        if(playerData.position.x >= finishLineX)
        {
            crossedFinishedLine = true;
        }

        if(collidingWithNeb && !crossedFinishedLine)
        {
            DrawText("You Died!", WINDOW_DIMENSIONS.x / 4, WINDOW_DIMENSIONS.y / 2, 160, RED);
        }
        else if(!collidingWithNeb && crossedFinishedLine)
        {
            DrawText("You Win!", WINDOW_DIMENSIONS.x / 4, WINDOW_DIMENSIONS.y / 2, 160, GREEN);
        }
        else
        {
            //Draw player
            DrawTextureRec(playerTexture, playerData.textureRect, playerData.position, WHITE);
        }

        EndDrawing();
    }
    //Unload textures
    UnloadTexture(playerTexture);
    UnloadTexture(nebulaTexture);
    UnloadTexture(backgroundTexture);
    UnloadTexture(midgroundTexture);
    UnloadTexture(foregroundTexture);

    //Close the game
    CloseWindow();

    return 0;
}