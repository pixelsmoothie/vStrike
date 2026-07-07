//
// Created by Niraj on 07-07-2026.
//
#include "physicsEngine.h"
#include "../common/global.h"


void ResolveCollision(Ball& ball, Paddle& paddle1, Paddle& paddle2)
{
    float rad = ball.radius;

    Vector2 center = {ball.Cx, ball.Cy};
    Rectangle Rec{paddle1.x, paddle1.y, paddle1.width, paddle1.height};
    Rectangle Rec1{paddle2.x, paddle2.y, paddle2.width, paddle2.height};

    if (CheckCollisionCircleRec(center, rad, Rec))
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
        paddle1.hp -= 50;
        ball.Cx = WIDTH/2;
        ball.Cy = HEIGHT/2;
    }

    if (ball.Cx > WIDTH)
    {
        paddle2.hp -= 50;
        ball.Cx = WIDTH/2;
        ball.Cy = HEIGHT/2;
    }
}