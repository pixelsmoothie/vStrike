//
// Created by Niraj on 16-07-2026.
//

#ifndef PONGARENA_CUSTOMFONT_H
#define PONGARENA_CUSTOMFONT_H
#include "raylib.h"

extern Font globalFont;
extern Font gameNameFont;

inline void DrawCustomText(const char* text, float x, float y, float size, Color color)
{
    DrawTextEx(globalFont, text, Vector2{(float)(int)x, (float)(int)y}, size, 2.0f, color);
}

inline void DrawGameName(const char* text, float x, float y, float size, Color color)
{
    DrawTextEx(gameNameFont, text, Vector2{(float)(int)x, (float)(int)y}, size, 8.0f, color);
}
#endif //PONGARENA_CUSTOMFONT_H
