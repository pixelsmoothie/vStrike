//
// Created by Niraj on 13-07-2026.
//

#ifndef PONGARENA_SETTINGSCREEN_H
#define PONGARENA_SETTINGSCREEN_H
#include "gameScreen.h"
#include "../constants.h"
#include "../ShapeHelpers.h"
#include "../appSettings.h"

class SettingsScreen : public GameScreen
{
private:
    float H = HEIGHT / 2.0f;
    float W = WIDTH / 2.0f;

    Texture2D settingsBG;

    Color btnBackOutline = RAYWHITE;
    Color textColorBack = RAYWHITE;

    Color btnFullscreenOutline = RAYWHITE;
    Color textColorFullscreen = RAYWHITE;

    Color btnFpsOutline = RAYWHITE;
    Color textColorFps = RAYWHITE;

    Rectangle fullscreenButton = {(WIDTH - 340) / 2, H - 100.0f, 340, 70};
    Rectangle fpsButton        = {(WIDTH - 340) / 2, H - 10.0f, 340, 70};
    Rectangle backButton       = {(WIDTH - 340) / 2, H + 80.0f, 340, 70};
    bool isPressed = false;
public:
    SettingsScreen()
    {
        settingsBG = LoadTexture("assets/UI/BG/main_menu_bg.png");
    }

    ~SettingsScreen()
    {
        UnloadTexture(settingsBG);
    }

    GameStates Update(float dt) override
    {
        isPressed = false;
        Vector2 mousePos = GetMousePosition();

        if (CheckCollisionPointRec(mousePos, fullscreenButton))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                btnFullscreenOutline = VIOLET;
                textColorFullscreen  = VIOLET;
                AppSettings::isFullscreen = !AppSettings::isFullscreen;
                ToggleFullscreen();
            }
        }
        else
        {
            btnFullscreenOutline = RAYWHITE;
            textColorFullscreen  = RAYWHITE;
        }

        if (CheckCollisionPointRec(mousePos, fpsButton))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                btnFpsOutline = VIOLET;
                textColorFps  = VIOLET;
                AppSettings::showFPS = !AppSettings::showFPS;
            }
        }
        else
        {
            btnFpsOutline = RAYWHITE;
            textColorFps  = RAYWHITE;
        }

        if (CheckCollisionPointRec(mousePos, backButton))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                btnBackOutline = VIOLET;
                textColorBack  = VIOLET;
                return GameStates::STATE_MENU;
            }
        }
        else
        {
            btnBackOutline = RAYWHITE;
            textColorBack  = RAYWHITE;
        }

        return GameStates::STATE_SETTINGS;
    }

    void Draw() override
    {
        DrawTexture(settingsBG, 0, 0, WHITE);

        // FULLSCREEN BUTTON
        DrawChamferedRectangleFilled(fullscreenButton, 10.0f, BLACK);
        DrawChamferedRectangleLines(fullscreenButton, 10.0f, 2.0f, btnFullscreenOutline);
        const char* fsText = "FULLSCREEN: OFF";
        if (AppSettings::isFullscreen)
        {
            fsText = "FULLSCREEN: ON";
        }
        Vector2 textSizeFs = MeasureTextEx(globalFont, fsText, 28, 2.0f);
        DrawCustomText(fsText, (WIDTH - textSizeFs.x) / 2, H - 85.0f, 28, textColorFullscreen);

        // FPS BUTTON
        DrawChamferedRectangleFilled(fpsButton, 10.0f, BLACK);
        DrawChamferedRectangleLines(fpsButton, 10.0f, 2.0f, btnFpsOutline);
        const char* fpsText = "FPS: OFF";
        if (AppSettings::showFPS)
        {
            fpsText = "FPS: ON";
        }
        Vector2 textSizeFps = MeasureTextEx(globalFont, fpsText, 32, 2.0f);
        DrawCustomText(fpsText, (WIDTH - textSizeFps.x) / 2, H + 5.0f, 32, textColorFps);

        // BACK BUTTON
        DrawChamferedRectangleFilled(backButton, 10.0f, BLACK);
        DrawChamferedRectangleLines(backButton, 10.0f, 2.0f, btnBackOutline);
        Vector2 textSizeBack = MeasureTextEx(globalFont, "BACK", 40, 2.0f);
        DrawCustomText("BACK", (WIDTH - textSizeBack.x) / 2, H + 95.0f, 40, textColorBack);
    }
};

#endif //PONGARENA_SETTINGSCREEN_H
