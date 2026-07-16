//
// Created by Niraj on 13-07-2026.
//

#ifndef PONGARENA_MENUSCREEN_H
#define PONGARENA_MENUSCREEN_H
#include "gameStates.h"
#include "gameScreen.h"
#include "raylib.h"
#include "../customFont.h"

class MenuScreen : public GameScreen
{
private:
    float W = WIDTH / 2;
    float H = HEIGHT / 2;

    Rectangle startButton = {W - 120, H - 30, 240, 60};
    Rectangle settingsButton = {W - 120, H + 60, 240, 60};
    bool isPressed = false;
public:
    GameStates Update(float dt) override
    {
        isPressed = false;
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, settingsButton))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                return GameStates::STATE_SETTINGS;
            }
        }
        else if (CheckCollisionPointRec(mousePos, startButton))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                return GameStates::STATE_MODE_SELECTION;
            }
        }
        return GameStates::STATE_MENU;
    }

    void Draw() override
    {
        // Flat, pure dark background
        ClearBackground(Color{ 20, 20, 20, 255 }); 

        // Large, clean white title
        DrawCustomText("vstrike", W - 120, 220, 70, RAYWHITE);

        // START BUTTON
        DrawRectangleRoundedLines(startButton, 0.4f, 16, 3.0f, RAYWHITE);
        const int textSizeStart = MeasureText("START", 34);
        DrawCustomText("START", (WIDTH - textSizeStart) / 2, H - 17, 34, Color{ 100, 100, 100, 255 });

        // SETTINGS BUTTON
        DrawRectangleRoundedLines(settingsButton, 0.4f, 16, 3.0f, RAYWHITE);
        const int textSizeSettings = MeasureText("SETTINGS", 34);
        DrawCustomText("SETTINGS", (WIDTH - textSizeSettings) / 2, H + 75, 34, Color{ 100, 100, 100, 255 });
    }
};
#endif //PONGARENA_MENUSCREEN_H
