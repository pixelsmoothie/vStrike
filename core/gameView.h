//
// Created by Niraj on 16-07-2026.
//

#ifndef PONGARENA_GAMEVIEW_H
#define PONGARENA_GAMEVIEW_H
#include "../global/constants.h"
#include "../entities/paddle.h"
#include "../entities/ball.h"
#include "../global/states/gameScreen.h"
#include "../UI/HealthBar.h"
#include "../physics/physicsEngine.h"
#include "../UI/GameTimer.h"

class GameView : public GameScreen
{
protected:
    Paddle paddle1;
    Paddle paddle2;
    Ball ball;
    float multiplier;
    float topPos = 60;

public:
    GameView() : paddle1(10.0f, HEIGHT / 2 - 150.0f / 2, 30.0f, 150.0f, 400.0f, RAYWHITE, KEY_S, KEY_W, 100.0f, 100.0f),
                 paddle2(WIDTH - 40.0f, HEIGHT / 2 - 150.0f / 2, 30.0f, 150.0f, 400.0f, RAYWHITE, KEY_DOWN, KEY_UP, 100.0f, 100.0f),
                 ball(WIDTH / 2, HEIGHT / 2, 15.0f, 500.0f, 360.0f, RAYWHITE),
                 multiplier(1.3f) {}

    GameStates Update(float dt) override = 0;

    void Draw() override {
        paddle1.Draw();
        paddle2.Draw();
        DrawLine(WIDTH / 2, topPos, WIDTH / 2, HEIGHT , GRAY);
        ball.Draw();
        DrawLine(0, topPos, WIDTH, topPos, RAYWHITE);
        RenderHealthBars(paddle1, paddle2);
        DrawTimer();
        GameOutcomeAndRestart(ball, paddle1, paddle2, multiplier);
    }

    void updatePhysics(float dt){
        if (paddle1.hp > 0 && paddle2.hp > 0)
        {
            UpdateTimer(dt);
            ResolveCollision(ball, paddle1, paddle2);
            CheckScoreAndReset(ball, paddle1, paddle2);
        }
    }
};

#endif //PONGARENA_GAMEVIEW_H
