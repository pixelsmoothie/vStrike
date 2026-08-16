//
// Created by Niraj on 13-07-2026.
//

#ifndef PONGARENA_SETTINGSCREEN_H
#define PONGARENA_SETTINGSCREEN_H
#include "gameScreen.h"
#include "../constants.h"

class SettingsScreen : public GameScreen
{
private:
    float H = HEIGHT / 2.0f;
    float W = WIDTH / 2.0f;

    Texture2D settingsBG;

    Rectangle backButton = {W - 120.0f, H + 60.0f, 240.0f, 60.0f};
    bool isPressed = false;
public:
    SettingsScreen()
    {
        settingsBG = LoadTexture("assets/UI/BG/menu_bg.png");
    }

    ~SettingsScreen()
    {
        UnloadTexture(settingsBG);
    }

    GameStates Update(float dt) override
    {
        isPressed = false;
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, backButton))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                return GameStates::STATE_MENU;
            }
        }
        return GameStates::STATE_SETTINGS;
    }

    void Draw() override
    {
        DrawTexture(settingsBG, 0, 0, WHITE);

        DrawRectangleRoundedLines(backButton, 0.4f, 16, 2.0f, RAYWHITE);
        const int textSizeSettings = MeasureText("SETTINGS", 45);
        DrawCustomText("BACK", (WIDTH + 47 - textSizeSettings) / 2, (HEIGHT / 2) + 68, 45, RAYWHITE);
    }
};
#endif //PONGARENA_SETTINGSCREEN_H
