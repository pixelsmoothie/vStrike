//
// Created by Niraj on 16-07-2026.
//

#ifndef PONGARENA_AIVIEW_H
#define PONGARENA_AIVIEW_H
#include "gameView.h"

class AIView : public GameView
{
public:
    AIView() : GameView() {}

    GameStates Update(float dt) override
    {
        paddle1.Update(dt);
        ball.Update(dt);

        paddle2.color = VIOLET;

        updatePhysics(dt);

        return GameStates::STATE_AI_VIEW;
    }
};
#endif //PONGARENA_AIVIEW_H
