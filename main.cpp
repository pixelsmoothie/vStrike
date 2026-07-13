#include "raylib.h"
#include "global/constants.h"
#include "entities/paddle.h"
#include "entities/ball.h"
#include "global/states/gameScreen.h"
#include "physics/physicsEngine.h"
#include "UI/HealthBar.h"
#include "global/states/gameStates.h"
#include "global/states/menuScreen.h"

int main() {
    InitWindow(WIDTH, HEIGHT, "vStrike");
    SetTargetFPS(60);
    float multiplier = 1.3f;

    //PADDLE OBJECTS
    constexpr float HT = 150.0f;
    Paddle paddle1(10.0f, HEIGHT/2 - HT/2, 30.0f, HT, 400.0f, BLACK, KEY_S, KEY_W, 100.0f, 100.0f);
    Paddle paddle2(WIDTH - 40.0f, HEIGHT/2 - HT/2, 30.0f, HT, 400.0f, RAYWHITE, KEY_DOWN, KEY_UP, 100.0f, 100.0f);

    //BALL OBJECTS
    Ball ball(WIDTH/2, HEIGHT/2, 15.0f, 300.0f, 280.0f, GREEN);

    //GAME STATES
    GameStates currentState = GameStates::STATE_MENU;
    GameScreen* currentScreen = new MenuScreen();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        if (currentState == GameStates::STATE_GAMEPLAY)
        {
            //DRAWING ENTITIES
            paddle1.Draw();
            paddle2.Draw();
            ball.Draw();

            //ENTITY MOVEMENT
            paddle1.Update(GetFrameTime());
            paddle2.Update(GetFrameTime());
            ball.Update(GetFrameTime());

            //PHYSICS ENGINE
            ResolveCollision(ball, paddle1, paddle2);
            CheckScoreAndReset(ball, paddle1, paddle2);
            GameOutcomeAndRestart(ball, paddle1, paddle2, multiplier);

            //UI ELEMENTS
            DrawLine(WIDTH / 2, 0, WIDTH / 2, HEIGHT, GRAY);
            RenderHealthBars(paddle1, paddle2);
        }else
        {
            GameStates nextState = currentScreen->Update(GetFrameTime());             //get the inputs from the screen for each frame (enter is pressed)
            if (nextState != currentState)
            {
                delete currentScreen;                                                     //critical!!! free the memory before allocating new screen
                currentState = nextState;
                if (nextState == GameStates::STATE_MENU)
                {
                    currentScreen = new MenuScreen();
                }
                if (nextState == GameStates::STATE_GAMEPLAY)
                {
                    currentScreen = nullptr;                                              //why? nullptr, because this state rn is not a child class of GameScreen
                }
            }
            if (currentScreen != nullptr)                                                 //critical, to avoid the segmentation fault
            {
                currentScreen->Draw();
            }
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
