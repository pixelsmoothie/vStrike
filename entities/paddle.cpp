//
// Created by Niraj on 03-07-2026.
//
#include "paddle.h"
#include "../global/constants.h"
#include "ball.h"

void Paddle :: Draw()
{
    Rectangle rect;
    rect.x = x;
    rect.y = y;
    rect.width = width;
    rect.height = height;

    DrawRectangleRoundedLines(rect, 0.4f, 16, 1.0f, color);
}

void Paddle :: Update(float deltaTime)
{
    if (IsKeyDown(upKey))
    {
        y -= (speed * deltaTime);
        if (y < 40)
        {
             y = 40;
        }
    }

    if (IsKeyDown(downKey))
    {
        y += (speed * deltaTime);
        if (y + height > HEIGHT)
        {
            y = HEIGHT - height;
        }
    }
}