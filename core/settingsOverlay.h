//
// Created by Niraj on 16-08-2026.
//

#ifndef PONGARENA_SETTINGSOVERLAY_H
#define PONGARENA_SETTINGSOVERLAY_H

#include "raylib.h"
#include "../global/constants.h"
#include "../global/states/gameStates.h"
#include "../global/customFont.h"
#include "../global/appSettings.h"

class SettingsOV
{
private:
    Rectangle OverlayBG      = {(WIDTH / 2) - 200, 200, 400, 400};
    Rectangle OverlayOutline = {(WIDTH / 2) - 201, 201, 402, 402};

    // Main menu buttons
    Rectangle Resume   = {(WIDTH / 2) - 150, 250, 300, 55};
    Rectangle Settings = {(WIDTH / 2) - 150, 335, 300, 55};
    Rectangle Exit     = {(WIDTH / 2) - 150, 420, 300, 55};

    // Settings sub-panel
    bool showSettingsPanel = false;

    Rectangle FullscreenBtn = {(WIDTH / 2) - 150, 295, 300, 55};
    Rectangle FpsBtn        = {(WIDTH / 2) - 150, 380, 300, 55};
    Rectangle BackBtn       = {(WIDTH / 2) - 150, 490, 300, 45};

    void DrawSettingBtn(Rectangle r, const char* label, bool active)
    {
        Color bg = Color{40, 40, 40, 220};
        Color textCol = RAYWHITE;
        if (active)
        {
            bg = RAYWHITE;
            textCol = Color{10, 10, 10, 255};
        }
        DrawChamferedRectangleFilled(r, 8.0f, bg);
        DrawChamferedRectangleLines(r, 8.0f, 1.5f, RAYWHITE);
        Vector2 sz = MeasureTextEx(globalFont, label, 26, 1.5f);
        DrawCustomText(label, r.x + (r.width - sz.x) / 2, r.y + (r.height - sz.y) / 2, 26, textCol);
    }

public:
    static inline bool& showFPS      = AppSettings::showFPS;
    static inline bool& isFullscreen = AppSettings::isFullscreen;

    GameStates ReturnScreen(bool& isPaused, GameStates Return)
    {
        Vector2 mousePos = GetMousePosition();
        bool clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        if (IsKeyPressed(KEY_SPACE))
            isPaused = !isPaused;

        if (isPaused)
        {
            if (!showSettingsPanel)
            {
                if (clicked)
                {
                    if      (CheckCollisionPointRec(mousePos, Resume))   isPaused = false;
                    else if (CheckCollisionPointRec(mousePos, Settings))  showSettingsPanel = true;
                    else if (CheckCollisionPointRec(mousePos, Exit))    { showSettingsPanel = false; return GameStates::STATE_MENU; }
                }
            }
            else
            {
                if (clicked)
                {
                    if (CheckCollisionPointRec(mousePos, BackBtn))
                    {
                        showSettingsPanel = false;
                    }
                    else if (CheckCollisionPointRec(mousePos, FullscreenBtn))
                    {
                        isFullscreen = !isFullscreen;
                        ToggleFullscreen();
                    }
                    else if (CheckCollisionPointRec(mousePos, FpsBtn))
                    {
                        showFPS = !showFPS;
                    }
                }
            }
        }
        return Return;
    }

    void DrawMenuOV()
    {
        Color color1 = Color{10, 10, 10, 200};
        Color color2 = Color{20, 20, 20, 240};
        DrawRectangleGradientV(0, 0, (int)WIDTH, (int)HEIGHT, color1, color2);

        DrawChamferedRectangleFilled(OverlayBG,     20.0f, GAME_BG);
        DrawChamferedRectangleLines(OverlayOutline, 20.0f, 2.0f, RAYWHITE);

        if (!showSettingsPanel)
        {
            Vector2 ts = MeasureTextEx(globalFont, "PAUSED", 38, 2.0f);
            DrawCustomText("PAUSED", (WIDTH - ts.x) / 2, 218, 38, RAYWHITE);

            DrawSettingBtn(Resume,   "RESUME",   false);
            DrawSettingBtn(Settings, "SETTINGS", false);
            DrawSettingBtn(Exit,     "EXIT",     false);
        }
        else
        {
            Vector2 ts = MeasureTextEx(globalFont, "SETTINGS", 34, 2.0f);
            DrawCustomText("SETTINGS", (WIDTH - ts.x) / 2, 218, 34, RAYWHITE);

            const char* fsText = "FULLSCREEN: OFF";
            if (isFullscreen)
            {
                fsText = "FULLSCREEN: ON";
            }

            const char* fpsText = "FPS: OFF";
            if (showFPS)
            {
                fpsText = "FPS: ON";
            }

            DrawSettingBtn(FullscreenBtn, fsText, isFullscreen);
            DrawSettingBtn(FpsBtn,        fpsText, showFPS);
            DrawSettingBtn(BackBtn,       "< BACK", false);
        }
    }
};
#endif //PONGARENA_SETTINGSOVERLAY_H
