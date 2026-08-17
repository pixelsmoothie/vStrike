//
// Created by Niraj on 17-08-2026.
//

#ifndef PONGARENA_SHAPEHELPERS_H
#define PONGARENA_SHAPEHELPERS_H
#include "raylib.h"

inline void DrawChamferedRectangleLines(Rectangle rect, float cutSize, float thick, Color color)
{
    float x = rect.x;
    float y = rect.y;
    float w = rect.width;
    float h = rect.height;
    float c = cutSize; // e.g. 15.0f for the 45-degree cut

    // The 8 vertices going clockwise from top-left:
    Vector2 points[9] = {
        { x + c, y },         // Top-left cut end
        { x + w - c, y },     // Top-right cut start
        { x + w, y + c },     // Top-right cut end
        { x + w, y + h - c }, // Bottom-right cut start
        { x + w - c, y + h }, // Bottom-right cut end
        { x + c, y + h },     // Bottom-left cut start
        { x, y + h - c },     // Bottom-left cut end
        { x, y + c },         // Top-left cut start
        { x + c, y }          // Close back to start
    };

    // Draw connected line strip with custom thickness
    DrawLineStrip(points, 9, color);
}

inline void DrawChamferedRectangleFilled(Rectangle rect, float cutSize, Color color)
{
    float x = rect.x;
    float y = rect.y;
    float w = rect.width;
    float h = rect.height;
    float c = cutSize;

    // 1. Center vertical & horizontal body rectangles
    DrawRectangle(x + c, y, w - 2 * c, h, color);         // Horizontal middle slab
    DrawRectangle(x, y + c, c, h - 2 * c, color);         // Left strip
    DrawRectangle(x + w - c, y + c, c, h - 2 * c, color); // Right strip

    // 2. The 4 Chamfered corner triangles (Counter-Clockwise order)
    DrawTriangle({ x, y + c }, { x + c, y + c }, { x + c, y }, color);                                 // Top-Left
    DrawTriangle({ x + w - c, y }, { x + w - c, y + c }, { x + w, y + c }, color);                     // Top-Right
    DrawTriangle({ x + w, y + h - c }, { x + w - c, y + h - c }, { x + w - c, y + h }, color);         // Bottom-Right
    DrawTriangle({ x + c, y + h }, { x + c, y + h - c }, { x, y + h - c }, color);                     // Bottom-Left
}

#endif //PONGARENA_SHAPEHELPERS_H
