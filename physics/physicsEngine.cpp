//
// Created by Niraj on 07-07-2026.
//
#include "physicsEngine.h"
#include "../global/constants.h"
#include "../global/customFont.h"
#include "../global/audioManager.h"
#include "../UI/GameTimer.h"

void ResolveCollision(Ball& ball, Paddle& paddle1, Paddle& paddle2)
{
    float rad = ball.radius;

    Vector2 center = {ball.Cx, ball.Cy};
    Rectangle Rec{paddle1.x, paddle1.y, paddle1.width, paddle1.height};
    Rectangle Rec1{paddle2.x, paddle2.y, paddle2.width, paddle2.height};

    if (CheckCollisionCircleRec(center, rad, Rec))      //center, radius, paddle's position and dimensions
    {
        if (ball.speedX < 0)
        {
            if (IsKeyDown(paddle1.upKey))   ball.speedY -= 120.0f;
            if (IsKeyDown(paddle1.downKey)) ball.speedY += 120.0f;     //boosts speed of ball on contact moving paddle
            ball.Cx = paddle1.x + paddle1.width + ball.radius;
            ball.speedX *= -1;
            PlaySound(AudioManager::hitSound);
        }
    };

    if (CheckCollisionCircleRec(center, rad, Rec1))
    {
        if (ball.speedX > 0)
        {
            if (IsKeyDown(paddle2.upKey))   ball.speedY -= 120.0f;
            if (IsKeyDown(paddle2.downKey)) ball.speedY += 120.0f;
            ball.Cx = paddle2.x - ball.radius;
            ball.speedX *= -1;
            PlaySound(AudioManager::hitSound);
        }
    };
}

void CheckScoreAndReset(Ball& ball, Paddle& paddle1, Paddle& paddle2)
{
    if (ball.Cx < 0)
    {
        paddle1.hp -= 20.0f;                          //amount of hp to be reduced on miss
        if (paddle1.hp < 0.0f) paddle2.hp = 0.0f;
        ball.Cx = WIDTH/2;
        ball.Cy = HEIGHT/2;
        PlaySound(AudioManager::scoreSound);
    }

    if (ball.Cx > WIDTH)
    {
        paddle2.hp -= 20.0f;
        if (paddle2.hp < 0.0f) paddle2.hp = 0.0f;
        ball.Cx = WIDTH/2;
        ball.Cy = HEIGHT/2;
        PlaySound(AudioManager::scoreSound);
    }
}

extern float GameTime;

void StopAll(Ball& ball)
{
    DrawRectangle(0, 0, WIDTH, HEIGHT, Fade(BLACK, 0.6f));
    ball.speedX = 0;
    ball.speedY = 0;
    ball.Cx = WIDTH/2;
    ball.Cy = HEIGHT/2;
}

bool flag = true;
bool gameOverSoundPlayed = false;

void ResetAll(Ball& ball, Paddle& paddle1, Paddle& paddle2, float& multiplier)
{
    paddle1.hp = paddle1.maxHp;
    paddle2.hp = paddle2.maxHp;
    ball.speedX += 300 * multiplier;
    ball.speedY += 280 * multiplier;
    multiplier += 0.4f;
    GameTime = 90.0f;
    gameOverSoundPlayed = false;
}

void GameOutcomeAndRestart(Ball& ball, Paddle& paddle1, Paddle& paddle2, float& multiplier)
{
    if (GameTime <= 0)
    {
        flag = false;

        if (!gameOverSoundPlayed)
        {
            PlaySound(AudioManager::gameOverSound);
            gameOverSoundPlayed = true;
        }

        StopAll(ball);

        int warningWidth = MeasureText("Time's UP!!", 40);
        DrawCustomText("Time's UP!!", (WIDTH - warningWidth) / 2, 350, 40, WHITE);

        Vector2 RestartWidth = MeasureTextEx(globalFont, "Press [R] to restart", 40, 2);
        DrawCustomText("Press [R] to restart", (WIDTH - RestartWidth.x) / 2, 450, 40, RED);

        if (IsKeyPressed(KEY_R))
        {
            ResetAll(ball, paddle1, paddle2, multiplier);
        }
    }

    if (paddle1.hp == 0 || paddle2.hp == 0 && flag)
    {
        if (!gameOverSoundPlayed)
        {
            PlaySound(AudioManager::gameOverSound);
            gameOverSoundPlayed = true;
        }

        StopAll(ball);

        if (paddle1.hp == 0)
        {
            int textWidth = MeasureText("Player 2 Wins", 40);
            DrawCustomText("Player 2 Wins", (WIDTH - textWidth) / 2, 350, 40, WHITE);
        }else
        {
            int textWidth = MeasureText("Player 1 Wins", 40);
            DrawCustomText("Player 1 Wins", (WIDTH - textWidth) / 2, 350, 40, WHITE);
        }

        // Draw restart text below the winner text instead of above it
        Vector2 RestartWidth = MeasureTextEx(globalFont, "Press [R] to restart", 40, 2);
        DrawCustomText("Press [R] to restart", (WIDTH - RestartWidth.x) / 2, 450, 40, RED);

        if (IsKeyPressed(KEY_R))
        {
            ResetAll(ball, paddle1, paddle2, multiplier);
        }
    }
}
