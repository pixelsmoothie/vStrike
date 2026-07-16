//
// Created by Niraj on 16-07-2026.
//

#ifndef PONGARENA_MODESELECTION_H
#define PONGARENA_MODESELECTION_H
#include "gameScreen.h"
#include "raylib.h"

class ModeSelection : public GameScreen
{
private:
    float W = WIDTH / 2;
    float H = HEIGHT / 2;

    Texture2D modeBG, Local, Multiplayer, AI;
    Texture2D L, R;

    Rectangle Trigger = {W - 130, H + 165, 260, 60};
    Rectangle LeftNav = {100, H - 65, 150, 150};
    Rectangle RightNav = {WIDTH - 220, H - 65, 150, 150};

    int selectedState = 0;

public:

    ModeSelection()
    {
        modeBG = LoadTexture("assets/UI/BG/menu_bg.png");
        Local = LoadTexture("assets/UI/Modes/icon_local_1v1@2x.png");
        Multiplayer = LoadTexture("assets/UI/Modes/icon_multiplayer@2x.png");
        AI = LoadTexture("assets/UI/Modes/icon_vs_bot@2x.png");

        L = LoadTexture("assets/UI/Modes/icon_arrow_left_v2.png");
        R = LoadTexture("assets/UI/Modes/icon_arrow_right_v2.png");
    }

    ~ModeSelection()
    {
        UnloadTexture(modeBG);
        UnloadTexture(Local);
        UnloadTexture(Multiplayer);
        UnloadTexture(AI);

        UnloadTexture(L);
        UnloadTexture(R);
    }


    GameStates Update(float dt) override
    {
        Vector2 mousePos = GetMousePosition();

        if (CheckCollisionPointRec(mousePos, RightNav))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                selectedState = (selectedState + 1) % 3;
            }
        }
        else if (CheckCollisionPointRec(mousePos, LeftNav))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                selectedState = (selectedState - 1 + 3) % 3;
            }
        }

        float scrollState = GetMouseWheelMove();
        if (scrollState > 0)
        {
            selectedState = (selectedState + 1) % 3;
        }
        else if (scrollState < 0)
        {
            selectedState = (selectedState - 1 + 3) % 3;
        }

        if (CheckCollisionPointRec(mousePos, Trigger))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (selectedState == 0) return GameStates::STATE_LOCAL_VIEW;
                if (selectedState == 1) return GameStates::STATE_AI_VIEW;
            }
        }
        return GameStates::STATE_MODE_SELECTION;
    }

    void Draw() override
    {
        DrawTexture(modeBG, 0, 0, WHITE);

        int Label = MeasureText("SELECT MODE", 60);
        DrawText("SELECT MODE", (WIDTH - Label) / 2, 150, 60, RAYWHITE);

        //DrawLine(W, 0, W, HEIGHT, RAYWHITE);
        //DrawLine(0, H, WIDTH, H, RAYWHITE);

        DrawTexture(L, 100, H - 65, WHITE);
        DrawTexture(R, WIDTH - 220, H - 65, WHITE);

        DrawRectangleRoundedLines(Trigger, 0.4f, 16, 1.0f, RAYWHITE);

        if (selectedState == 0)
        {
            DrawTexture(Local, W - 128 , H - 128, WHITE);
            DrawCustomText("LOCAL 1v1", W - 108, H + 170, 50, RAYWHITE);
        }
        else if (selectedState == 1)
        {
            DrawTexture(AI, W - 128, H - 128, WHITE);
            DrawCustomText("R-BOT", W - 68, H + 170, 50, RAYWHITE);
        }
        else if (selectedState == 2)
        {
            DrawTexture(Multiplayer, W - 128, H - 128, WHITE);
            DrawCustomText("ONLINE", W - 78, H + 170, 50, RAYWHITE);
        }
    }
};
#endif //PONGARENA_MODESELECTION_H
