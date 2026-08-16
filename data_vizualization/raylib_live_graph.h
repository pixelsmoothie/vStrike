// Live Raylib Reward Graph Snippet
// To be integrated into AIView::Draw() or dedicated visualizer screen tomorrow.

#include "raylib.h"
#include <vector>
#include <cmath>

void DrawRewardGraph(const std::vector<float>& rewardHistory, int screenWidth, int screenHeight)
{
    if (rewardHistory.size() < 2) return;

    float maxReward = 100.0f; // baseline scale
    for (float r : rewardHistory)
    {
        if (std::abs(r) > maxReward)
            maxReward = std::abs(r);
    }

    // Zero baseline
    DrawLine(0, screenHeight / 2, screenWidth, screenHeight / 2, Fade(GRAY, 0.4f));

    // Connect the dots across history
    for (size_t i = 1; i < rewardHistory.size(); i++)
    {
        float x1 = (float)(i - 1) * ((float)screenWidth / rewardHistory.size());
        float x2 = (float)i * ((float)screenWidth / rewardHistory.size());

        float centerY = screenHeight / 2.0f;
        float scaleY = (screenHeight / 2.0f) - 60.0f;

        float y1 = centerY - (rewardHistory[i - 1] / maxReward) * scaleY;
        float y2 = centerY - (rewardHistory[i] / maxReward) * scaleY;

        DrawLine((int)x1, (int)y1, (int)x2, (int)y2, GREEN);
    }
}
