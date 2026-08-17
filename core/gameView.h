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

    Texture2D gameBG;

public:
    GameView() : paddle1(20.0f, HEIGHT / 2 - 150.0f / 2, 30.0f, 150.0f, 400.0f, VIOLET, KEY_S, KEY_W, 100.0f, 100.0f),
                 paddle2(WIDTH - 50.0f, HEIGHT / 2 - 150.0f / 2, 30.0f, 150.0f, 400.0f, VIOLET, KEY_DOWN, KEY_UP, 100.0f, 100.0f),
                 ball(WIDTH / 2, HEIGHT / 2, 15.0f, 500.0f, 360.0f, VIOLET),
                 multiplier(1.3f)
    {
        gameBG = LoadTexture("assets/UI/BG/game_bg_view.png");
    }

    ~GameView()
    {
        UnloadTexture(gameBG);
    }

    GameStates Update(float dt) override = 0;

    void TopBar()
    {
        RenderHealthBars(paddle1, paddle2);
        DrawTimer();
        DrawCustomText("P1", 30.0f, 18.0f, 30.0f, RAYWHITE);
        DrawCustomText("P2", WIDTH - 60.0f, 18.0f, 30.0f, RAYWHITE);
    }

    void Draw() override {
        DrawTexture(gameBG, 0, 0, WHITE);
        DrawChamferedRectangleLines({6.0f, 6.0f, WIDTH - 12.0f, HEIGHT - 12.0f}, 10.0f, 4.0f, RAYWHITE);

        paddle1.Draw();
        paddle2.Draw();
        DrawLine(WIDTH / 2, topPos, WIDTH / 2, HEIGHT - 6.0f , RAYWHITE);
        ball.Draw();
        DrawLine(6.0f, topPos, WIDTH - 6.0f, topPos, RAYWHITE);
        TopBar();
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
