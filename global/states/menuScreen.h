//
// Created by Niraj on 13-07-2026.
//

#ifndef PONGARENA_MENUSCREEN_H
#define PONGARENA_MENUSCREEN_H
#include "gameStates.h"
#include "gameScreen.h"
#include "raylib.h"

class MenuScreen : public GameScreen
{
public:
    GameStates Update(float dt) override
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            return GameStates::STATE_GAMEPLAY;
        }
        return GameStates::STATE_MENU;
    }

    void Draw() override
    {
        // Flat, pure dark background
        ClearBackground(Color{ 20, 20, 20, 255 }); 

        // Large, clean white title
        DrawText("vstrike", 1280 / 2 - 120, 300, 70, RAYWHITE);

        // Faded, quiet start instruction
        DrawText("press enter", 1280 / 2 - 70, 420, 24, Color{ 80, 80, 80, 255 });
    }
};
#endif //PONGARENA_MENUSCREEN_H
