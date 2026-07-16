#include "raylib.h"
#include "global/constants.h"
#include "entities/paddle.h"
#include "entities/ball.h"
#include "UI/HealthBar.h"
#include "physics/physicsEngine.h"
#include "global/states/masterIncluder.h"
#include "UI/Fader.h"
#include "global/customFont.h"

Font globalFont;

int main()
{
    InitWindow(WIDTH, HEIGHT, "vStrike");
    SetTargetFPS(60);

    //GAME STATES
    GameStates currentState = GameStates::STATE_MENU;
    GameScreen* currentScreen = new MenuScreen();

    //FADER STATES
    Fader fader(0.6f);
    GameStates pendingState = currentState;

    globalFont = LoadFontEx("assets/BlockCraft.otf", 120, NULL, 0);
    SetTextureFilter(globalFont.texture, TEXTURE_FILTER_BILINEAR);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{20, 20, 20, 255});
        float dt = GetFrameTime();

        bool triggerToSwap = fader.Update(dt);

        if (fader.IsIdle() && currentScreen != nullptr)
        {
            GameStates nextState = currentScreen->Update(dt);
            if (nextState != currentState)
            {
                pendingState = nextState;
                fader.Start();
            }
        }

        //get the inputs from the screen for each frame (enter is pressed)
        if (triggerToSwap)
        {
            if (currentScreen != nullptr)
            {
                delete currentScreen; //critical!!! free the memory before allocating new screen
                currentScreen = nullptr;
            }
            currentState = pendingState;

            switch (pendingState)
            {
            case GameStates::STATE_LOCAL_VIEW:
                currentScreen = new LocalView();
                break;

            case GameStates::STATE_AI_VIEW:
                currentScreen = new AIView();
                break;

            case GameStates::STATE_MENU:
                currentScreen = new MenuScreen();
                break;

            case GameStates::STATE_SETTINGS:
                currentScreen = new SettingsScreen();
                break;

            case GameStates::STATE_MODE_SELECTION:
                currentScreen = new ModeSelection();
                break;
            }
        }
        if (currentScreen != nullptr) //critical, to avoid the segmentation fault
        {
            currentScreen->Draw();
        }
        fader.Draw();
        EndDrawing();
    }
    UnloadFont(globalFont);
    CloseWindow();
    return 0;
}
