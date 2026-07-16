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

    Rectangle Trigger = {W - 75, H - 75, 150, 150};

    int selectedState = 0;

public:
    GameStates Update(float dt) override
    {
        Vector2 mousePos = GetMousePosition();

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
        ClearBackground(Color{20, 20, 20, 255});
        int Label = MeasureText("SELECT MODE", 60);
        DrawText("SELECT MODE", (WIDTH - Label) / 2, 200, 60, RAYWHITE);

        if (selectedState == 0)
        {
            DrawRectangleRoundedLines(Trigger, 0.4f, 16, 3.0f, RAYWHITE);
        }
        else if (selectedState == 1)
        {
            DrawRectangleRoundedLines(Trigger, 0.4f, 16, 3.0f, GREEN);
        }
        else if (selectedState == 2)
        {
            DrawRectangleRoundedLines(Trigger, 0.4f, 16, 3.0f, VIOLET);
        }
    }
};
#endif //PONGARENA_MODESELECTION_H
