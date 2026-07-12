//
// Created by Niraj on 07-07-2026.
//

#ifndef PONGARENA_PHYSICENGINE_H
#define PONGARENA_PHYSICENGINE_H

#include "../entities/paddle.h"
#include "../entities/ball.h"

void ResolveCollision(Ball& ball, Paddle& paddle1, Paddle& paddle2);

void CheckScoreAndReset(Ball& ball, Paddle& paddle1, Paddle& paddle2);

void GameOutcomeAndRestart(Ball& ball, Paddle& paddle1, Paddle& paddle2, float& multiplier);

#endif //PONGARENA_PHYSICENGINE_H
