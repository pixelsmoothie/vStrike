#include "raylib.h"
#include "global/constants.h"
#include "entities/paddle.h"
#include "entities/ball.h"
#include "UI/healthBar.h"
#include "physics/physicsEngine.h"
#include "global/states/masterIncluder.h"
#include "UI/fader.h"
#include "global/customFont.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

Font globalFont;
Font gameNameFont;

struct GameContext
{
    GameStates currentState = GameStates::STATE_MENU;
    GameStates pendingState = currentState;
    GameScreen* currentScreen = nullptr;
    Fader fader{0.6f};
} gC;

void UpdateFrame()
{
    BeginDrawing();
    ClearBackground(GAME_BG);
    float dt = GetFrameTime();

    bool triggerToSwap = gC.fader.Update(dt);

    if (gC.fader.IsIdle() && gC.currentScreen != nullptr)
    {
        GameStates nextState = gC.currentScreen->Update(dt);
        if (nextState != gC.currentState)
        {
            gC.pendingState = nextState;
            gC.fader.Start();
        }
    }

    //get the inputs from the screen for each frame (enter is pressed)
    if (triggerToSwap)
    {
        if (gC.currentScreen != nullptr)
        {
            delete gC.currentScreen; //critical!!! free the memory before allocating new screen
            gC.currentScreen = nullptr;
        }
        gC.currentState = gC.pendingState;

        switch (gC.pendingState)
        {
        case GameStates::STATE_LOCAL_VIEW:
            gC.currentScreen = new LocalView();
            break;

        case GameStates::STATE_AI_VIEW:
            gC.currentScreen = new AIView();
            break;

        case GameStates::STATE_MENU:
            gC.currentScreen = new MenuScreen();
            break;

        case GameStates::STATE_SETTINGS:
            gC.currentScreen = new SettingsScreen();
            break;

        case GameStates::STATE_MODE_SELECTION:
            gC.currentScreen = new ModeSelection();
            break;
        }
    }
    if (gC.currentScreen != nullptr) //critical, to avoid the segmentation fault
    {
        gC.currentScreen->Draw();
    }
    gC.fader.Draw();
    EndDrawing();
}


int main()
{
    InitWindow(WIDTH, HEIGHT, "vStrike");
    SetTargetFPS(60);

    //called here after initializing the window so the openGL texture could load
    gC.currentScreen = new MenuScreen();

    globalFont = LoadFontEx("assets/IBM_Plex_Mono/IBMPlexMono-Regular.ttf", 80, NULL, 0);
    SetTextureFilter(globalFont.texture, TEXTURE_FILTER_BILINEAR);

    gameNameFont = LoadFontEx("assets/ElectroGarden.ttf", 200, NULL, 0);
    if (IsFontReady(gameNameFont))
    {
        SetTextureFilter(gameNameFont.texture, TEXTURE_FILTER_BILINEAR);
    }

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateFrame, 0, 1);
#else
    while (!WindowShouldClose())
    {
        UpdateFrame();
    }
#endif

    UnloadFont(globalFont);
    CloseWindow();
    return 0;
}
