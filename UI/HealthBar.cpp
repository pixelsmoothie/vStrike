//
// Created by Niraj on 12-07-2026.
//
#include "HealthBar.h"
#include "../global/constants.h"
#include <string>
#include "../global/customFont.h"

static float topPos = 60;

void RenderHealthBars(Paddle& paddle1, Paddle& paddle2)
{
    float percent1 = paddle1.hp / paddle1.maxHp;

    int reductionValue1 = paddle1.maxHp * percent1;
    std::string HP1 = std::to_string(reductionValue1);

    Rectangle rect1;
    rect1.width = percent1 * 400;
    rect1.height = 30;
    rect1.x = 80;
    rect1.y = (topPos - rect1.height) / 2;
    DrawRectangleRounded(rect1, 0.4f, 8, GREEN);

    Rectangle rect1Outline;
    rect1Outline.x = rect1.x;
    rect1Outline.y = rect1.y;
    rect1Outline.width = 400;
    rect1Outline.height = rect1.height;
    DrawRectangleRoundedLines(rect1Outline, 0.4f, 8, 2, RAYWHITE);

    DrawText(HP1.c_str(), (rect1.x + rect1Outline.width + 30), (topPos - rect1.height) / 2, 30, GREEN);

    float percent2 = paddle2.hp / paddle2.maxHp;

    int reductionValue2 = paddle2.maxHp * percent2;
    std::string HP2 = std::to_string(reductionValue2);

    Rectangle rect2;
    rect2.width = percent2 * 400;                            //on collision reduce HP
    rect2.height = 30;
    rect2.x = WIDTH - 480;
    rect2.y = (topPos - rect2.height) / 2;
    DrawRectangleRounded(rect2, 0.4f, 8, GREEN);

    Rectangle rect2Outline;
    rect2Outline.x = rect2.x;
    rect2Outline.y = rect2.y;
    rect2Outline.width = 400;
    rect2Outline.height = rect2.height;
    DrawRectangleRoundedLines(rect2Outline, 0.4f, 8, 2, RAYWHITE);

    DrawText(HP2.c_str(), (rect2.x - 70), (topPos - rect2.height) / 2, 30, GREEN);
}
