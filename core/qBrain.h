//
// Created by Niraj on 15-08-2026.
//

#ifndef PONGARENA_QBRAIN_H
#define PONGARENA_QBRAIN_H

#include <unordered_map>
#include <vector>
#include <fstream>

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

    int currEpisode = 1;
    float episodeReward = 0.0f;

public:
    std::vector<float> rewardHistory;

    QBrain() {};

    int getStateID(const Ball& ball, const Paddle& paddle)
    {
        int stateID = 0;

        int ballX_cuts = static_cast<int>(ball.Cx / (WIDTH / 4.0f));
        if (ballX_cuts > 3) ballX_cuts = 3;

        int ballY_cuts = static_cast<int>(ball.Cy / (HEIGHT / 8.0f));
        if (ballY_cuts > 7) ballY_cuts = 7;

        int paddleY_cuts = static_cast<int>(paddle.y / (HEIGHT / 8.0f));
        if (paddleY_cuts > 7) paddleY_cuts = 7;

        int dirX = (ball.speedX > 0) ? 1 : 0;

        stateID += ballX_cuts;
        stateID += ballY_cuts * (4);
        stateID += paddleY_cuts * (4 * 8);
        stateID += dirX * (4 * 8 * 8);
        return stateID;
    }

    int chooseAction(int stateID)
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

        episodeReward += reward;
    }

    void logEpisode(bool aiWon)
    {
        std::ofstream file;
        file.open("D:/iso-space/vStrike/metrics.csv", std::ios::app);

        if (currEpisode == 1)
        {
            file << "Episode,AI_Won,Total_Reward,Epsilon\n";
        }

        file << currEpisode << "," << (aiWon ? 1 : 0) << "," << episodeReward << "," << epsilon << "\n";
        file.close();

        // Decay epsilon per-episode (not per-frame!)
        if (epsilon > 0.01f)
        {
            epsilon -= 0.005f;  // ~200 episodes to fully train
        }

        rewardHistory.push_back(episodeReward);
        currEpisode++;
        episodeReward = 0.0f;
    }
};

#endif //PONGARENA_QBRAIN_H
