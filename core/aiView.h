//
// Created by Niraj on 16-07-2026.
//

#ifndef PONGARENA_AIVIEW_H
#define PONGARENA_AIVIEW_H
#include "gameView.h"
#include "qBrain.h"
#include "GLFW/glfw3.h"
#include <cmath>

class AIView : public GameView
{
private:
    QBrain brain;
public:
    AIView() : GameView() {}

    GameStates Update(float dt) override
    {
        int currState = brain.getStateID(ball, paddle2);

        int action = brain.choseAction(currState);

        if (action == 0) paddle2.y -= paddle2.speed * dt;
        else if (action == 1) paddle2.y += paddle2.speed * dt;

        if (paddle2.y < 40.0f) paddle2.y = 40.0f;
        if (paddle2.y + paddle2.height > HEIGHT) paddle2.y = HEIGHT - paddle2.height;

        paddle1.Update(dt);
        ball.Update(dt);
        paddle2.color = VIOLET;
        updatePhysics(dt);

        float reward = 0.0f;
        
        // 1. Sparse Rewards (The Big Events)
        if (ball.speedX < 0 && ball.Cx > WIDTH / 2) reward += 100.0f; // Hit!
        if (ball.Cx > WIDTH) reward -= 100.0f; // Missed!

        // 2. Dense Rewards (Hot and Cold)
        // Calculate how far the center of the paddle is from the ball
        float paddleCenterY = paddle2.y + (paddle2.height / 2.0f);
        float distance = std::abs(paddleCenterY - ball.Cy);
        
        // Punish the bot every frame based on how far away it is
        reward -= (distance * 0.05f); 

        int nextState = brain.getStateID(ball, paddle2);
        brain.updateQTable(currState, action, nextState, reward);

        return GameStates::STATE_AI_VIEW;
    }
};
#endif //PONGARENA_AIVIEW_H
