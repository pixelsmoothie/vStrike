//
// Created by Niraj on 16-07-2026.
//

#ifndef PONGARENA_LOCALVIEW_H
#define PONGARENA_LOCALVIEW_H
#include "gameView.h"
#include "settingsOverlay.h"

class LocalView : public GameView
{
private:
    bool isPaused = false;
    SettingsOV settingsOv;
public:
    LocalView() : GameView() {}

    GameStates Update(float dt) override
    {
        GameStates nextState = settingsOv.ReturnScreen(isPaused, GameStates::STATE_LOCAL_VIEW);

        if (nextState != GameStates::STATE_LOCAL_VIEW)
        {
            return nextState;
        }

        if (!isPaused)
        {
            paddle1.Update(dt);
            paddle2.Update(dt);
            ball.Update(dt);
            updatePhysics(dt);
        }
        return GameStates::STATE_LOCAL_VIEW;
    }

    void Draw() override
    {
        GameView::Draw();

        if (isPaused)
        {
            settingsOv.DrawMenuOV();
        }
    }
};
#endif //PONGARENA_LOCALVIEW_H
