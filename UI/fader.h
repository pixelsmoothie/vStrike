//
// Created by Niraj on 16-07-2026.
//

#ifndef PONGARENA_FADER_H
#define PONGARENA_FADER_H
#include "raylib.h"
#include "../global/constants.h"
#include <vector>
#include <string>

enum class FadeStates
{
    STATE_NONE,
    STATE_IN,
    STATE_OUT
};

class Fader
{
private:
    FadeStates state;
    float alpha = 0.0f;
    float speed = 2.0f;

public:
    Fader(float duration) : state(FadeStates::STATE_NONE), alpha(0.0f)
    {
        speed = 1.0f / duration;
    }

    void Start()
    {
        if (state == FadeStates::STATE_NONE)
        {
            state = FadeStates::STATE_IN;
            alpha = 0.0f;
        }
    }

    bool Update(float dt)
    {
        if (state == FadeStates::STATE_IN)
        {
            alpha += speed * dt;

            if (alpha >= 1.0f)
            {
                alpha = 1.0f;
                state = FadeStates::STATE_OUT;
                return true;
            }
        }
        else if (state == FadeStates::STATE_OUT)
        {
            alpha -= speed * dt;



            if (alpha <= 0.0f)
            {
                alpha = 0.0f;
                state = FadeStates::STATE_NONE;
            }
        }
        return false;
    }

    void Draw()
    {
        if (state != FadeStates::STATE_NONE)
        {
            unsigned alphaRate = static_cast<unsigned char>(alpha * 255.0f);
            DrawRectangle(0, 0, WIDTH, HEIGHT, Color{0,0,0, alphaRate});
        }
    }
    bool IsIdle() const
    {
        return state == FadeStates::STATE_NONE;
    }
};
#endif //PONGARENA_FADER_H
