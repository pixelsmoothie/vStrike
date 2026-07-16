//
// Created by Niraj on 16-07-2026.
//

#ifndef PONGARENA_CUSTOMFONT_H
#define PONGARENA_CUSTOMFONT_H
#include "raylib.h"

extern Font globalFont;

inline void DrawCustomText(const char* text, float x, float y, float size, Color color)
{
    DrawTextEx(globalFont, text, Vector2{x, y}, size, 1.0f, color);
}
#endif //PONGARENA_CUSTOMFONT_H
