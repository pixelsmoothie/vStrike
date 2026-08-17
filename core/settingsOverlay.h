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

        DrawChamferedRectangleFilled(OverlayBG, 20.0f, GAME_BG);
        DrawChamferedRectangleLines(OverlayOutline, 20.0f, 2.0f, RAYWHITE);

        //BUTTONS
        Vector2 textSizeResume = MeasureTextEx(globalFont,"RESUME", 40, 2.0f);
        DrawCustomText("RESUME", (WIDTH - textSizeResume.x) / 2, 265, 40, RAYWHITE);
        DrawChamferedRectangleLines(Resume, 10.0f, 2.0f, RAYWHITE);

        Vector2 textSizeSettings = MeasureTextEx(globalFont,"SETTINGS", 40, 2.0f);
        DrawCustomText("SETTINGS", (WIDTH - textSizeSettings.x) / 2, 375, 40, RAYWHITE);
        DrawChamferedRectangleLines(Settings, 10.0f, 2.0f, RAYWHITE);

        Vector2 textSizeExit = MeasureTextEx(globalFont,"EXIT", 40, 2.0f);
        DrawCustomText("EXIT", (WIDTH - textSizeExit.x) / 2, 495, 40, RAYWHITE);
        DrawChamferedRectangleLines(Exit, 10.0f, 2.0f, RAYWHITE);
    }
};
#endif //PONGARENA_SETTINGSOVERLAY_H
