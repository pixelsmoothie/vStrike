//
// Created by Niraj on 13-07-2026.
//

#ifndef PONGARENA_SETTINGSCREEN_H
#define PONGARENA_SETTINGSCREEN_H
#include "gameScreen.h"
#include "../constants.h"
#include "../ShapeHelpers.h"

class SettingsScreen : public GameScreen
{
private:
    float H = HEIGHT / 2.0f;
    float W = WIDTH / 2.0f;

    Texture2D settingsBG;

    Color btnBackOutline = RAYWHITE;
    Color textColorBack = RAYWHITE;

    Rectangle backButton = {(WIDTH - 340) / 2, H + 60.0f, 340, 70};
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
        if (CheckCollisionPointRec(mousePos, backButton))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                btnBackOutline = VIOLET;
                textColorBack  = VIOLET;
                return GameStates::STATE_MENU;
            }
        }
        return GameStates::STATE_SETTINGS;
    }

    void Draw() override
    {
        DrawTexture(settingsBG, 0, 0, WHITE);

        DrawChamferedRectangleFilled(backButton, 10.0f, BLACK);
        DrawChamferedRectangleLines(backButton, 10.0f, 2.0f, btnBackOutline);
        Vector2 textSizeBack = MeasureTextEx(globalFont,"BACK", 40, 2.0f);
        DrawCustomText("BACK", (WIDTH - textSizeBack.x) / 2, H + 75, 40, textColorBack);
    }
};
#endif //PONGARENA_SETTINGSCREEN_H
