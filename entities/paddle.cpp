//
// Created by Niraj on 03-07-2026.
//
#include "paddle.h"
#include "../global/constants.h"
#include "ball.h"

void Paddle :: Draw()
{
    Rectangle paddleRect;
    paddleRect.x = x;
    paddleRect.y = y;
    paddleRect.width = width;
    paddleRect.height = height;

    DrawRectangleRoundedLines(paddleRect, 0.4f, 16, 1.0f, color);
}

void Paddle :: Update(float deltaTime)
{
    if (IsKeyDown(upKey))
    {
        y -= (speed * deltaTime);                   //top boundary
        if (y < 62)
        {
             y = 62;
        }
    }

    if (IsKeyDown(downKey))                        //bottom boundary
    {
        y += (speed * deltaTime);
        if (y + height > HEIGHT)
        {
            y = (HEIGHT - height) - 8.0f;
        }
    }
}