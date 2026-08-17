//
// Created by Niraj on 17-08-2026.
//
#include "..\global/constants.h"
#include "GameTimer.h"

#include <ctime>
#include <string>

static float topPos = 60.0f;
float GameTime = 90.0f;

void UpdateTimer(float dt)
{
    if (GameTime >= 0)
    {
        GameTime -= dt;
    }else
    {
        GameTime = 0.0f;
    }
}

void DrawTimer()
{
    int totalSeconds = static_cast<int>(GameTime);
    int mins = totalSeconds / 60;
    int secs = totalSeconds % 60;

    std::string mStr = (totalSeconds < 10 ? "0" : "") + std::to_string(mins);
    std::string sStr = (totalSeconds < 10 ? "0" : "") + std::to_string(secs);
    std::string timeText = mStr + ":" + sStr;

    int fontSize = 30;
    int timerSize = MeasureText(timeText.c_str(), fontSize);

    int xPos = (WIDTH - timerSize) / 2;
    int yPos = static_cast<int>((topPos - fontSize) / 2.0f);
    DrawText(timeText.c_str(), xPos, yPos, fontSize, RAYWHITE);
}