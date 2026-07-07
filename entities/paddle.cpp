//
// Created by Niraj on 03-07-2026.
//
#include "paddle.h"
#include "../common/global.h"
#include "ball.h"

void Paddle :: Draw()
{
    Rectangle rect;
    rect.x = x;
    rect.y = y;
    rect.width = width;
    rect.height = height;

    DrawRectangleRounded(rect, 0.4f, 8, color);
}

void Paddle :: Update(float deltaTime)
{
    if (IsKeyDown(upKey))
    {
        y -= (speed * deltaTime);
        if (y < 0)
        {
             y = 0;
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