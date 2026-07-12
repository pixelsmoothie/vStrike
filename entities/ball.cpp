//
// Created by Niraj on 04-07-2026.
//
#include "ball.h"
#include "../global/constants.h"

void Ball ::  Draw()
{
    DrawCircle((int)Cx, (int)Cy, radius, color);
}

void Ball :: Update(float deltaTime)
{
    Cx += (speedX * deltaTime);
    Cy += (speedY * deltaTime);
    if (Cy - radius <= 0)             //Top Collision Detection
    {
        Cy = radius;                  //To avoid the endless jitter loop
        speedY *= -1;
    }

    if (Cy + radius >= HEIGHT)       //Bottom Collision Detection
    {
        Cy = HEIGHT - radius;
        speedY *= -1;
    }

    // if (Cx - radius <= 0)             //Left Collision Detection
    // {
    //     Cx = radius;
    //     speedX *= -1;
    // }
    //
    // if (Cx + radius >= WIDTH)         //Right Collision Detection
    // {
    //     Cx = WIDTH - radius;
    //     speedX *= -1;
    // }
}