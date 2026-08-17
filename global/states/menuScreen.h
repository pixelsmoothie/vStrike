//
// Created by Niraj on 13-07-2026.
//

#ifndef PONGARENA_MENUSCREEN_H
#define PONGARENA_MENUSCREEN_H
#include "gameStates.h"
#include "gameScreen.h"
#include "raylib.h"
#include "../customFont.h"
#include "../ShapeHelpers.h"
#include "../constants.h"

class MenuScreen : public GameScreen
{
private:
    float W = WIDTH / 2;
    float H = HEIGHT / 2;

    Texture2D BG;
    
    Color btnBack = BLACK;
    Color btnOutlineStart = RAYWHITE;
    Color btnOutlineSettings = RAYWHITE;
    Color textColorStart = RAYWHITE;
    Color textColorSettings = RAYWHITE;

    Rectangle startButton = {(WIDTH - 340) / 2, H - 10, 340, 70};
    Rectangle settingsButton = {(WIDTH - 340) / 2, H + 80, 340, 70};
    bool isPressed = false;
public:
    MenuScreen()
    {
        BG = LoadTexture("assets/UI/BG/main_menu_bg.png");
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
                btnOutlineSettings = VIOLET;
                textColorSettings = VIOLET;
                return GameStates::STATE_SETTINGS;
            }
        }
        else if (CheckCollisionPointRec(mousePos, startButton))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                btnOutlineStart = VIOLET;
                textColorStart = VIOLET;
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
        Vector2 gameTitleSize = MeasureTextEx(gameNameFont, "Strike", 200, 2.0f);
        DrawGameName("Strike", (WIDTH + 40 - gameTitleSize.x) / 2, 150, 200, RAYWHITE);

        DrawGameName("v", (WIDTH - 60 - gameTitleSize.x) / 2, 150, 200, VIOLET);

        // START BUTTON
        DrawChamferedRectangleFilled(startButton, 10.0f, BLACK);
        DrawChamferedRectangleLines(startButton, 10.0f, 4.0f, btnOutlineStart);
        Vector2 textSizeStart = MeasureTextEx(globalFont,"START", 40, 2.0f);
        DrawCustomText("START", (WIDTH - textSizeStart.x) / 2, H + 4, 40, textColorStart);

        // SETTINGS BUTTON
        DrawChamferedRectangleFilled(settingsButton, 10.0f, BLACK);
        DrawChamferedRectangleLines(settingsButton, 10.0f, 4.0f, btnOutlineSettings);
        Vector2 textSizeSettings = MeasureTextEx(globalFont,"SETTINGS", 40, 2.0f);
        DrawCustomText("SETTINGS", (WIDTH - textSizeSettings.x) / 2, H + 95, 40, textColorSettings);

        //debug
        //DrawLine(0, H, WIDTH, H, WHITE);
        //DrawLine(W, 0, W, HEIGHT, WHITE);
    }
};
#endif //PONGARENA_MENUSCREEN_H
