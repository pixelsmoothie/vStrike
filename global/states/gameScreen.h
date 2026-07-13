//
// Created by Niraj on 13-07-2026.
//

#ifndef PONGARENA_GAMESCREEN_H
#define PONGARENA_GAMESCREEN_H
#include "gameStates.h"

class GameScreen
{
public:
    virtual ~GameScreen() = default;

    virtual GameStates Update(float dt) = 0;
    virtual void Draw() = 0;
};
#endif //PONGARENA_GAMESCREEN_H
