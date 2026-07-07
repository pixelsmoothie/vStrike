//
// Created by Niraj on 03-07-2026.
//

#ifndef PONGARENA_PADDLE_H
#define PONGARENA_PADDLE_H
#include "raylib.h"

struct Paddle
{
    float x;
    float y;
    float width;
    float height;
    float round;
    int segments;
    float speed;
    Color color;
    int downKey;
    int upKey;
    float hp;
    float maxHp;

    void Update(float deltaTime);
    void Draw();

    Paddle(float startX, float startY, float w, float h, float s, Color c, int dKey, int uKey, float hp, float maxHp) :
    x(startX), y(startY), width(w), height(h), speed(s), color(c), downKey(dKey), upKey(uKey), hp(hp), maxHp(maxHp) {}
};

#endif //PONGARENA_PADDLE_H
