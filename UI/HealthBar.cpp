//
// Created by Niraj on 12-07-2026.
//
#include "HealthBar.h"
#include "../global/constants.h"

void RenderHealthBars(Paddle& paddle1, Paddle& paddle2)
{
    float percent1 = paddle1.hp / paddle1.maxHp;
    float percent2 = paddle2.hp / paddle2.maxHp;

    Rectangle rect;
    rect.x = WIDTH - 480;
    rect.y = 10;
    rect.width = percent2 * 400;                            //on collision reduce HP
    rect.height = 20;
    DrawRectangleRounded(rect, 0.4f, 8, GREEN);

    Rectangle rect1;
    rect1.x = 80;
    rect1.y = 10;
    rect1.width = percent1 * 400;
    rect1.height = 20;
    DrawRectangleRounded(rect1, 0.4f, 8, GREEN);
}
