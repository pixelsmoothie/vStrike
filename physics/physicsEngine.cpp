//
// Created by Niraj on 07-07-2026.
//
#include "physicsEngine.h"
#include "../global/constants.h"


void ResolveCollision(Ball& ball, Paddle& paddle1, Paddle& paddle2)
{
    float rad = ball.radius;

    Vector2 center = {ball.Cx, ball.Cy};
    Rectangle Rec{paddle1.x, paddle1.y, paddle1.width, paddle1.height};
    Rectangle Rec1{paddle2.x, paddle2.y, paddle2.width, paddle2.height};

    if (CheckCollisionCircleRec(center, rad, Rec))      //center, radius, paddle's position and dimensions
    {
        ball.Cx = paddle1.x + paddle1.width + ball.radius;
        ball.speedX *= -1;
    };

    if (CheckCollisionCircleRec(center, rad, Rec1))
    {
        ball.Cx = paddle2.x - ball.radius;
        ball.speedX *= -1;
    };
}

void CheckScoreAndReset(Ball& ball, Paddle& paddle1, Paddle& paddle2)
{
    if (ball.Cx < 0)
    {
        paddle1.hp -= 20;                          //amount of hp to be reduced on miss
        ball.Cx = WIDTH/2;
        ball.Cy = HEIGHT/2;
    }

    if (ball.Cx > WIDTH)
    {
        paddle2.hp -= 20;
        ball.Cx = WIDTH/2;
        ball.Cy = HEIGHT/2;
    }
}

void GameOutcomeAndRestart(Ball& ball, Paddle& paddle1, Paddle& paddle2, float& multiplier)
{
    if (paddle1.hp == 0 || paddle2.hp == 0)
    {
        DrawRectangle(0, 0, WIDTH, HEIGHT, Fade(BLACK, 0.6f));
        ball.speedX = 0;
        ball.speedY = 0;
        ball.Cx = WIDTH/2;
        ball.Cy = HEIGHT/2;
        
        if (paddle1.hp == 0)
        {
            int textWidth = MeasureText("Player 2 Wins", 60);
            DrawText("Player 2 Wins", (WIDTH - textWidth) / 2, 350, 60, WHITE);
        }else
        {
            int textWidth = MeasureText("Player 2 Wins", 60);
            DrawText("Player 1 Wins", (WIDTH - textWidth) / 2, 350, 60, WHITE);
        }

        // Draw restart text below the winner text instead of above it
        int textWidth = MeasureText("Player 2 Wins", 60);
        DrawText("Press [R] to restart", (WIDTH - textWidth) / 2, 450, 50, RED);

        if (IsKeyPressed(KEY_R))
        {
            paddle1.hp = 100.0f;
            paddle2.hp = 100.0f;
            ball.speedX += 300 * multiplier;
            ball.speedY += 280 * multiplier;
            multiplier += 0.4f;
        }
    }
}
