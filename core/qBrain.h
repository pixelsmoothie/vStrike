//
// Created by Niraj on 15-08-2026.
//

#ifndef PONGARENA_QBRAIN_H
#define PONGARENA_QBRAIN_H

#include <unordered_map>
#include <vector>

#include "../entities/ball.h"
#include "../entities/paddle.h"
#include "../global/constants.h"

class QBrain
{
private:
    std::unordered_map<int, std::vector<float>> qTable;

    float learningRate = 0.1f;
    float epsilon = 1.0f;
    float discountFactor = 0.9f;

public:
    QBrain()
    {
    };

    int getStateID(const Ball& ball, const Paddle& paddle)
    {
        int stateID = 0;

        // 1. Is the ball moving towards the AI (Right) or away (Left)?
        int movingTowards = (ball.speedX > 0) ? 1 : 0;
        
        // 2. Where is the ball relative to the paddle's center?
        float paddleCenter = paddle.y + (paddle.height / 2.0f);
        int relativeY = 1; // Default: Aligned
        
        if (ball.Cy < paddleCenter - 20.0f) relativeY = 0;      // Ball is Above
        else if (ball.Cy > paddleCenter + 20.0f) relativeY = 2; // Ball is Below

        // Combine into 6 possible states (2 * 3)
        stateID = movingTowards;
        stateID += relativeY * 2;

        return stateID;
    }

    int choseAction(int stateID)
    {
        float rand = GetRandomValue(0, 100.0f) / 100.0f;

        if (rand < epsilon)
        {
            return GetRandomValue(0, 2);
        }
        else
        {
            if (qTable.find(stateID) == qTable.end())
            {
                qTable[stateID] = {0.0f, 0.0f, 0.0f};
            }

            int bestAction = 0;
            float maxQ = qTable[stateID][0];

            for (int i = 1; i < 3; i++)
            {
                if (qTable[stateID][i] > maxQ)
                {
                    maxQ = qTable[stateID][i];
                    bestAction = i;
                }
            }
            return bestAction;
        }
    }

    void updateQTable(int stateID, int action, int nextStateID, float reward)
    {
        if (qTable.find(nextStateID) == qTable.end()) qTable[nextStateID] = {0.0f, 0.0f, 0.0f};

        float maxFutureQ = qTable[nextStateID][0];
        for (int i = 1; i < 3; i++)
        {
            if (qTable[nextStateID][i] > maxFutureQ)
            {
                maxFutureQ = qTable[nextStateID][i];
            }
        }

        float currentQ = qTable[stateID][action];
        float learnedValue = reward + discountFactor * maxFutureQ;

        qTable[stateID][action] = currentQ + learningRate * (learnedValue - currentQ);

        // Decay epsilon 10x faster so the bot grows up in 30 seconds
        if (epsilon > 0.05f)
        {
            epsilon -= 0.0005f; 
        }
    }
};

#endif //PONGARENA_QBRAIN_H
