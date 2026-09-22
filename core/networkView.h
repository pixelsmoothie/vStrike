#ifndef VSTRIKE_NETWORK_VIEW_H
#define VSTRIKE_NETWORK_VIEW_H

#include "gameView.h"
#include "settingsOverlay.h"
#include "../networking/ENetManager.h"

class NetworkView : public GameView
{
private:
    bool isPaused = false;
    SettingsOV settingsOv;
    ENetManager netManager;
    bool matchStarted = false;
    bool botMode = false; // Press [B] to let a bot play on this window for easy 1-player testing!

public:
    NetworkView(charData p1, charData p2) : GameView(p1, p2)
    {
        paddle2.upKey = KEY_UP;
        paddle2.downKey = KEY_DOWN;

        ball.Cx = WIDTH / 2;
        ball.Cy = HEIGHT / 2;
        ball.speedX = 0.0f;
        ball.speedY = 0.0f;
    }

    ~NetworkView() { netManager.Disconnect(); }

    GameStates Update(float dt) override
    {
        GameStates next = settingsOv.ReturnScreen(isPaused, GameStates::STATE_NETWORK_VIEW);
        if (next != GameStates::STATE_NETWORK_VIEW)
        {
            netManager.Disconnect();
            return next;
        }

        // 1. Lobby Mode
        if (!netManager.IsConnected())
        {
            if (IsKeyPressed(KEY_H)) netManager.StartHost(7777);
            if (IsKeyPressed(KEY_J)) netManager.StartClient("127.0.0.1", 7777);
            netManager.Update(dt);

            ball.Cx = WIDTH / 2;
            ball.Cy = HEIGHT / 2;
            return GameStates::STATE_NETWORK_VIEW;
        }

        // Toggle Test Bot with [B] key
        if (IsKeyPressed(KEY_B))
        {
            botMode = !botMode;
        }

        // 2. Launch ball when connected
        if (!matchStarted)
        {
            matchStarted = true;
            ball.speedX = 500.0f;
            ball.speedY = 360.0f;
        }

        // Reset match if [R] is pressed
        if (IsKeyPressed(KEY_R))
        {
            paddle1.hp = 100.0f;
            paddle2.hp = 100.0f;
            ball.Cx = WIDTH / 2;
            ball.Cy = HEIGHT / 2;
            ball.speedX = (netManager.GetRole() == NetworkRole::HOST) ? 500.0f : -500.0f;
            ball.speedY = 360.0f;
        }

        netManager.Update(dt);
        if (isPaused) return GameStates::STATE_NETWORK_VIEW;

        // 3. Gameplay loop
        if (netManager.GetRole() == NetworkRole::HOST)
        {
            paddle1.Update(dt); // Host moves P1

            PacketClientInput input{};
            if (netManager.ConsumeClientInput(input)) paddle2.y = input.paddleY;

            ball.Update(dt);
            updatePhysics(dt);

            netManager.SendServerState(ball.Cx, ball.Cy, ball.speedX, ball.speedY,
                                       paddle1.y, paddle2.y, paddle1.hp, paddle2.hp);
        }
        else // CLIENT
        {
            if (botMode)
            {
                // Auto-Bot: Smoothly tracks the ball so you can test without touching this window!
                float targetY = ball.Cy - paddle2.height / 2.0f;
                if (paddle2.y < targetY) paddle2.y += paddle2.speed * dt;
                if (paddle2.y > targetY) paddle2.y -= paddle2.speed * dt;
            }
            else
            {
                paddle2.Update(dt); // Manual control with Up/Down
            }

            netManager.SendClientInput(paddle2.y, 0.0f);

            PacketServerState state{};
            if (netManager.ConsumeServerState(state))
            {
                ball.Cx = state.ballX;
                ball.Cy = state.ballY;
                paddle1.y = state.paddle1Y;
                paddle1.hp = state.paddle1Hp;
                paddle2.hp = state.paddle2Hp;
            }
        }

        return GameStates::STATE_NETWORK_VIEW;
    }

    void Draw() override
    {
        GameView::Draw();

        if (!netManager.IsConnected())
        {
            DrawRectangle(0, 0, WIDTH, HEIGHT, Fade(BLACK, 0.85f));
            DrawText("MULTIPLAYER LOBBY", WIDTH / 2 - 170, HEIGHT / 2 - 70, 35, RAYWHITE);

            if (netManager.GetRole() == NetworkRole::HOST)
            {
                DrawText("HOSTING ON PORT 7777 - WAITING FOR P2...", WIDTH / 2 - 230, HEIGHT / 2, 22, YELLOW);
            }
            else if (netManager.GetRole() == NetworkRole::CLIENT)
            {
                DrawText("CONNECTING TO 127.0.0.1:7777...", WIDTH / 2 - 180, HEIGHT / 2, 22, SKYBLUE);
            }
            else
            {
                DrawText("Press [ H ] to Host Room (P1)", WIDTH / 2 - 140, HEIGHT / 2 - 10, 22, GREEN);
                DrawText("Press [ J ] to Join Room (P2)", WIDTH / 2 - 140, HEIGHT / 2 + 30, 22, SKYBLUE);
            }
        }
        else
        {
            const char* role = (netManager.GetRole() == NetworkRole::HOST) ? "HOST (P1)" : "CLIENT (P2)";
            Color col = (netManager.GetRole() == NetworkRole::HOST) ? GREEN : SKYBLUE;
            const char* botStr = botMode ? " | BOT: ON" : "";

            DrawText(TextFormat("NET: %s | PING: %d ms%s | [R] Reset | [B] Bot", role, netManager.GetPingMs(), botStr), 20, HEIGHT - 30, 18, col);
        }

        if (isPaused) settingsOv.DrawMenuOV();
    }
};

#endif // VSTRIKE_NETWORK_VIEW_H
