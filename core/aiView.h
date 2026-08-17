//
// Created by Niraj on 16-07-2026.
//

#ifndef PONGARENA_AIVIEW_H
#define PONGARENA_AIVIEW_H
#include "gameView.h"
#include "qBrain.h"
#include "settingsOverlay.h"
#include <cmath>

class AIView : public GameView
{
private:
    bool isPaused = false;
    SettingsOV settingsOv;

    QBrain brain;
public:
    AIView() : GameView() {}

    GameStates Update(float dt) override
    {
        GameStates nextState = settingsOv.ReturnScreen(isPaused, GameStates::STATE_AI_VIEW);

        if (nextState != GameStates::STATE_AI_VIEW)
        {
            return nextState;
        }

        if (!isPaused)
        {
            int currState = brain.getStateID(ball, paddle2);

            int action = brain.chooseAction(currState);

            if (action == 0) paddle2.y -= paddle2.speed * dt;
            else if (action == 1) paddle2.y += paddle2.speed * dt;

            if (paddle2.y < 40.0f) paddle2.y = 40.0f;
            if (paddle2.y + paddle2.height > HEIGHT) paddle2.y = HEIGHT - paddle2.height;

            paddle1.Update(dt);
            ball.Update(dt);
            paddle2.color = VIOLET;

            // 3. Episode Tracking (Did someone score?)
            // MUST run before updatePhysics resets the ball!
            if (ball.Cx > WIDTH)
            {
                brain.logEpisode(false);
            }
            else if (ball.Cx < 0)
            {
                brain.logEpisode(true);
            }

            updatePhysics(dt); // Now it's safe to resolve collisions and reset!

            float reward = 0.0f;

            // 1. Sparse Rewards (The Big Events)
            if (ball.speedX < 0 && ball.Cx > WIDTH / 2) reward += 100.0f; // Hit!
            if (ball.Cx > WIDTH) reward -= 100.0f; // Missed!

            // 2. Dense Rewards (Negative Penalty for distance)
            float paddleCenterY = paddle2.y + (paddle2.height / 2.0f);
            float distance = std::abs(paddleCenterY - ball.Cy);

            // Punish the bot every frame based on how far away it is!
            reward -= (distance * 0.1f);

            int forwardState = brain.getStateID(ball, paddle2);
            brain.updateQTable(currState, action, forwardState, reward);
        }
        return GameStates::STATE_AI_VIEW;
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
#endif //PONGARENA_AIVIEW_H
