#include "raylib.h"
#include "global/constants.h"
#include "entities/paddle.h"
#include "entities/ball.h"
#include "physics/physicsEngine.h"
#include "UI/HealthBar.h"
#include "global/gameStates.h"

int main() {
    InitWindow(WIDTH, HEIGHT, "vStrike");
    SetTargetFPS(60);

    //PADDLE OBJECTS
    constexpr float HT = 150.0f;
    Paddle paddle1(10.0f, HEIGHT/2 - HT/2, 30.0f, HT, 400.0f, BLACK, KEY_S, KEY_W, 100.0f, 100.0f);
    Paddle paddle2(WIDTH - 40.0f, HEIGHT/2 - HT/2, 30.0f, HT, 400.0f, RAYWHITE, KEY_DOWN, KEY_UP, 100.0f, 100.0f);

    Ball ball(WIDTH/2, HEIGHT/2, 15.0f, 300.0f, 280.0f, GREEN);

    float multiplier = 1.3f;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);

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

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
