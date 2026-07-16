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

    Texture2D BG;

    Rectangle startButton = {W - 120, H - 30, 240, 60};
    Rectangle settingsButton = {W - 120, H + 60, 240, 60};
    bool isPressed = false;
public:
    MenuScreen()
    {
        BG = LoadTexture("assets/UI/BG/menu_bg.png");
    }

    ~MenuScreen()
    {
        UnloadTexture(BG);
    }

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
        DrawTexture(BG, 0, 0, WHITE);

        // Large, clean white title
        DrawCustomText("vstrike", W - 120, 220, 70, RAYWHITE);

        // START BUTTON
        DrawRectangleRoundedLines(startButton, 0.4f, 16, 2.0f, RAYWHITE);
        const int textSizeStart = MeasureText("START", 45);
        DrawCustomText("START", (WIDTH + 35 - textSizeStart) / 2, H - 24, 45, RAYWHITE);

        // SETTINGS BUTTON
        DrawRectangleRoundedLines(settingsButton, 0.4f, 16, 2.0f, RAYWHITE);
        const int textSizeSettings = MeasureText("SETTINGS", 45);
        DrawCustomText("SETTINGS", (WIDTH + 47 - textSizeSettings) / 2, H + 68, 45, RAYWHITE);
    }
};
#endif //PONGARENA_MENUSCREEN_H
