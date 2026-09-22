//
// Created by Niraj on 18-08-2026.
//

#ifndef PONGARENA_CHARACTERSELECT_H
#define PONGARENA_CHARACTERSELECT_H
#include "gameScreen.h"
#include "raylib.h"

class CharacterSelection : public GameScreen
{
private:
    Texture2D BG;

public:
    CharacterSelection()
    {
        BG =LoadTexture("assets/UI/BG/main_menu_bg.png");
    }

    ~CharacterSelection()
    {
        UnloadTexture(BG);
    }

    GameStates Update(float dt) override
    {
        return GameStates::STATE_CHARACTER_SELECTION;
    }

    void Draw() override
    {
        DrawTexture(BG, 0, 0, WHITE);
    }
};

#endif //PONGARENA_CHARACTERSELECT_H
