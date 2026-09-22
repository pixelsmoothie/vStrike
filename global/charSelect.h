//
// Created by Niraj on 18-08-2026.
//

#ifndef PONGARENA_CHARSELECT_H
#define PONGARENA_CHARSELECT_H
#include "raylib.h"
#include <string>
enum class Characters
{
    CHAR_MEGANO,
    CHAR_CYRAX,
    CHAR_XLR8,
    CHAR_MIMO
};

struct charData
{
    std::string name;
    std::string title;
    float maxHP;
    float speed;
    float height;
    float width;
    Color color;
};

inline charData getCharData(Characters type)
{
    switch (type)
    {
    case Characters::CHAR_MEGANO:
        return{"MEGANO", "THE TANK", 120.0f, 320.0f, 180.0f, 30.0f, Color{80, 180, 255, 255}};
    case Characters::CHAR_CYRAX:
        return {"CYRAX", "THE TRICKSTER", 90.0f, 450.0f, 140.0f, 30.0f, Color{188, 140, 255, 255}};
    case Characters::CHAR_XLR8:
        return {"XLR8", "THE SPEEDSTER", 80.0f, 540.0f, 100.0f, 30.0f, Color{255, 90, 90, 255}};
    case Characters::CHAR_MIMO:
        return{"MIMO", "THE ORDINARY", 100.0f, 400.0f, 150.0f, 30.0f, RAYWHITE};
    }
    return{"MIMO", "THE ORDINARY", 100.0f, 400.0f, 150.0f, 30.0f, RAYWHITE};
}
#endif //PONGARENA_CHARSELECT_H
