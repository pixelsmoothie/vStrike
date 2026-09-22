#ifndef VSTRIKE_NETWORK_VIEW_H
#define VSTRIKE_NETWORK_VIEW_H

#include "gameView.h"
#include "settingsOverlay.h"
#include "../networking/ENetManager.h"
#include "../global/audioManager.h"

class NetworkView : public GameView
{
private:
    bool isPaused = false;
    SettingsOV settingsOv;
    ENetManager netManager;
    bool matchStarted = false;
    bool botMode = false; // Press [B] to let a bot play on this window for easy 1-player testing!

    // LAN IP input — client types the host's local IP (e.g. 192.168.x.x) before pressing J
    // TODO: replace this with a proper room-code system backed by a matchmaking server
    //       so two players over the internet can connect without sharing IPs manually.
    //       Need to learn: NAT traversal (STUN/TURN), or a simple relay server in Python.
    char ipInput[64] = "127.0.0.1";
    int  ipLen       = 9; // length of the default string above

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
            // --- IP text input: backspace to delete, printable chars to type ---
            if (netManager.GetRole() == NetworkRole::NONE || netManager.GetRole() == NetworkRole::CLIENT)
            {
                int ch = GetCharPressed();
                while (ch > 0)
                {
                    // allow digits and dots only (valid IPv4 characters)
                    if ((ch >= '0' && ch <= '9') || ch == '.' )
                    {
                        if (ipLen < 63) { ipInput[ipLen++] = (char)ch; ipInput[ipLen] = '\0'; }
                    }
                    ch = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE) && ipLen > 0)
                    ipInput[--ipLen] = '\0';
            }

            if (IsKeyPressed(KEY_H)) netManager.StartHost(7777);
            if (IsKeyPressed(KEY_J)) netManager.StartClient(ipInput, 7777);
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
            PlaySound(AudioManager::startSound);
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
            DrawRectangle(0, 0, WIDTH, HEIGHT, Fade(BLACK, 0.92f));
            DrawText("MULTIPLAYER LOBBY", WIDTH / 2 - 170, HEIGHT / 2 - 190, 35, RAYWHITE);

            if (netManager.GetRole() == NetworkRole::HOST)
            {
                DrawText("HOSTING ON PORT 7777 — WAITING FOR P2...", WIDTH / 2 - 230, HEIGHT / 2 - 60, 22, YELLOW);
            }
            else if (netManager.GetRole() == NetworkRole::CLIENT)
            {
                DrawText("CONNECTING...", WIDTH / 2 - 90, HEIGHT / 2 - 60, 22, SKYBLUE);
            }
            else
            {
                DrawText("Press [ H ] to Host (P1)",      WIDTH / 2 - 130, HEIGHT / 2 - 100, 22, GREEN);
                DrawText("Press [ J ] to Join (P2)",      WIDTH / 2 - 130, HEIGHT / 2 - 55,  22, SKYBLUE);

                // IP input box — client types host's LAN IP here before pressing J
                DrawText("Host IP:",                       WIDTH / 2 - 130, HEIGHT / 2 + 10,  20, GRAY);
                DrawRectangleLines(WIDTH / 2 - 130, HEIGHT / 2 + 38, 260, 32, DARKGRAY);
                DrawText(ipInput,                          WIDTH / 2 - 120, HEIGHT / 2 + 45,  20, WHITE);
                DrawText("(type to edit, backspace to clear)", WIDTH / 2 - 130, HEIGHT / 2 + 82, 16, DARKGRAY);
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
