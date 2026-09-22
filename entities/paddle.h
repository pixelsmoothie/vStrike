//
// Created by Niraj on 03-07-2026.
//

#ifndef PONGARENA_PADDLE_H
#define PONGARENA_PADDLE_H
#include "raylib.h"
#include "../global/charSelect.h"

struct Paddle
{
    float x;
    float y;
    float width;
    float height;
    float round{};
    int segments{};
    float speed;
    Color color;
    int downKey;
    int upKey;
    float hp;
    float maxHp;

    void Update(float deltaTime);
    void Draw();

    Paddle(float startX, float startY, const charData& data, int dKey, int uKey, float hp) :
    x(startX),
    y(startY),
    width(data.width),
    height(data.height),
    speed(data.speed),
    color(data.color),
    downKey(dKey),
    upKey(uKey),
    hp(hp),
    maxHp(data.maxHP) {}
};

#endif //PONGARENA_PADDLE_H
