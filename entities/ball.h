//
// Created by Niraj on 04-07-2026.
//

#ifndef PONGARENA_BALL_H
#define PONGARENA_BALL_H
#include "raylib.h"

struct Ball
{
    float Cx;   //to avoid the integer truncation (int) cast when drawing
    float Cy;
    float radius;
    float speedX;
    float speedY;
    Color color;

    Ball(float Cx, float Cy, float radius, float speedX, float speedY, Color color)
    : Cx(Cx), Cy(Cy), radius(radius), speedX(speedX), speedY(speedY), color(color){}  //Constructor Init List (better performance)

    void Draw();
    void Update(float deltaTime);
};

#endif //PONGARENA_BALL_H
