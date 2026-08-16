//
// Created by Niraj on 16-08-2026.
//

#ifndef PONGARENA_SETTINGSOVERLAY_H
#define PONGARENA_SETTINGSOVERLAY_H

#include "raylib.h"
#include "../global/constants.h"
#include "../global/states/gameStates.h"
#include "../global/customFont.h"

class SettingsOV
{
private:
    //BACKGROUND
    Rectangle OverlayBG = {(WIDTH / 2) - 200, 200, 400, 400};
    Rectangle OverlayOutline = {(WIDTH / 2) - 201, 201, 402, 402};

    //BUTTONS
    Rectangle Resume = {(WIDTH / 2) - 150, 250, 300, 70};
    Rectangle Settings = {(WIDTH / 2) - 150, 360, 300, 70};
    Rectangle Exit = {(WIDTH / 2) - 150, 480, 300, 70};

public:
    GameStates ReturnScreen(bool& isPaused, GameStates Return)
    {
        Vector2 mousePos = GetMousePosition();

        if (IsKeyPressed(KEY_SPACE))
        {
            isPaused = !isPaused;
        }

        if (isPaused)
        {
            if (CheckCollisionPointRec(mousePos, Resume))
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    isPaused = false;
                }
            }
            else if (CheckCollisionPointRec(mousePos, Exit))
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    return GameStates::STATE_MENU;
                }
            }
        }
        return Return;
    }

    void DrawMenuOV()
    {
        Color color1 = Color{10, 10, 10, 200};
        Color color2 = Color{20, 20, 20, 240};

        //BACKGROUND
        DrawRectangleGradientV(0, 0, WIDTH, HEIGHT, color1, color2);
        DrawRectangleRounded(OverlayBG, 0.4f, 16, color2);
        DrawRectangleRoundedLines(OverlayOutline, 0.4f, 16, 1.0f, BLACK);

        //BUTTONS
        DrawCustomText("RESUME", (WIDTH / 2) - 100, 255, 60, RAYWHITE);
        DrawRectangleRoundedLines(Resume, 0.4f, 16, 2.0f,BLACK);

        DrawCustomText("SETTINGS", (WIDTH / 2) - 115, 360, 60, RAYWHITE);
        DrawRectangleRoundedLines(Settings, 0.4f, 16, 2.0f, BLACK);

        DrawCustomText("EXIT", (WIDTH / 2) - 60, 480, 60, RAYWHITE);
        DrawRectangleRoundedLines(Exit, 0.4f, 16, 2.0f, BLACK);
    }
};
#endif //PONGARENA_SETTINGSOVERLAY_H
