#include "raylib.h"
#include "common/global.h"
#include "entities/paddle.h"
#include "entities/ball.h"

int main() {
    InitWindow(WIDTH, HEIGHT, "Raylib Linkage Test");
    SetTargetFPS(60);

    float HT = 150.0f;

    Paddle paddle1(10.0f, HEIGHT/2 - HT/2, 30.0f, HT, 400.0f, BLACK, KEY_S, KEY_W, 100.0f, 100.0f);
    Paddle paddle2(WIDTH - 40.0f, HEIGHT/2 - HT/2, 30.0f, HT, 400.0f, RAYWHITE, KEY_DOWN, KEY_UP, 100.0f, 100.0f);

    Ball ball(WIDTH/2, HEIGHT/2, 15.0f, 300.0f, 280.0f, GREEN);

    float multiplier = 1.3f;

    float rad = ball.radius;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawLine(WIDTH / 2, 0, WIDTH / 2, HEIGHT, GRAY);
        paddle1.Draw();
        paddle2.Draw();

        ball.Draw();

        paddle1.Update(GetFrameTime());
        paddle2.Update(GetFrameTime());

        ball.Update(GetFrameTime());

        Vector2 center = {ball.Cx, ball.Cy};
        Rectangle Rec{paddle1.x, paddle1.y, paddle1.width, paddle1.height};
        Rectangle Rec1{paddle2.x, paddle2.y, paddle2.width, paddle2.height};

        if (CheckCollisionCircleRec(center, rad, Rec))
        {
            ball.Cx = paddle1.x + paddle1.width + ball.radius;
            ball.speedX *= -1;
        };

        if (CheckCollisionCircleRec(center, rad, Rec1))
        {
            ball.Cx = paddle2.x - ball.radius;
            ball.speedX *= -1;
        };

        if (ball.Cx < 0)
        {
            paddle1.hp -= 50;
            ball.Cx = WIDTH/2;
            ball.Cy = HEIGHT/2;
        }

        if (ball.Cx > WIDTH)
        {
            paddle2.hp -= 50;
            ball.Cx = WIDTH/2;
            ball.Cy = HEIGHT/2;
        }

        if (paddle1.hp == 0 || paddle2.hp == 0)
        {
            DrawRectangle(0, 0, WIDTH, HEIGHT, Fade(BLACK, 0.6f));
            ball.speedX = 0;
            ball.speedY = 0;
            ball.Cx = WIDTH/2;
            ball.Cy = HEIGHT/2;
            if (paddle1.hp == 0)
            {
                DrawText("Player 2 Wins", (WIDTH - 350) / 2, (HEIGHT - 60) / 2, 60, WHITE);
            }else
            {
                DrawText("Player 1 Wins", (WIDTH - 350) / 2, (HEIGHT - 60) / 2, 60, WHITE);
            }

            DrawText("Press [R] to restart", (WIDTH - 500) / 2, (HEIGHT - 300) / 2, 50, RED);
            if (IsKeyPressed(KEY_R))
            {
                paddle1.hp = 100.0f;
                paddle2.hp = 100.0f;
                ball.speedX += 300 * multiplier;
                ball.speedY += 280 * multiplier;
                multiplier += 0.4f;
            }
        }

        float percent1 = paddle1.hp / paddle1.maxHp;
        float percent2 = paddle2.hp / paddle2.maxHp;

        Rectangle rect;
        rect.x = WIDTH - 480;
        rect.y = 10;
        rect.width = percent2 * 400;
        rect.height = 20;
        DrawRectangleRounded(rect, 0.4f, 8, GREEN);

        Rectangle rect1;
        rect1.x = 80;
        rect1.y = 10;
        rect1.width = percent1 * 400;
        rect1.height = 20;
        DrawRectangleRounded(rect1, 0.4f, 8, GREEN);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
