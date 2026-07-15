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
    Rectangle backButton = {(WIDTH/2) - 40, (HEIGHT/2) - 20, 80, 40};
    bool isPressed = false;
public:
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
        int textSize = MeasureText("This Is Settings IN DEV", 60);
        DrawText("This Is Settings IN DEV", (WIDTH - textSize) / 2, (HEIGHT / 2) - 200, 60, RED);
        DrawRectangleRoundedLines(backButton, 0.4f, 16, 3.0f, BLUE);
    }
};
#endif //PONGARENA_SETTINGSCREEN_H
