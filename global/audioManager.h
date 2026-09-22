//
// Created by Niraj on 23-09-2026.
//

#ifndef PONGARENA_AUDIOMANAGER_H
#define PONGARENA_AUDIOMANAGER_H

#include "raylib.h"
#include <cmath>

class AudioManager
{
private:
    static Sound GenerateBeep(float frequency, float duration, float volume)
    {
        const int sampleRate = 44100;
        const int samples = static_cast<int>(sampleRate * duration);

        short* data = (short*)MemAlloc(samples * sizeof(short));

        for (int i = 0; i < samples; i++)
        {
            float t = static_cast<float>(i) / sampleRate;
            float wave = sinf(2.0f * PI * frequency * t);

            float envelope = 1.0f;
            if (t < 0.005f)
            {
                envelope = t / 0.005f;
            }
            if (t > duration - 0.01f)
            {
                envelope = (duration - t) / 0.01f;
            }

            data[i] = static_cast<short>(wave * envelope * volume * 32767);
        }

        Wave wave = {0};
        wave.frameCount = (unsigned int)samples;
        wave.sampleRate = (unsigned int)sampleRate;
        wave.sampleSize = 16;
        wave.channels   = 1;
        wave.data       = data;

        Sound sound = LoadSoundFromWave(wave);
        UnloadWave(wave);

        return sound;
    }

public:
    static inline Sound hitSound;
    static inline Sound wallSound;
    static inline Sound scoreSound;
    static inline Sound startSound;
    static inline Sound gameOverSound;
    static inline bool initialized = false;

    static void Init()
    {
        if (!initialized)
        {
            InitAudioDevice();
            hitSound      = GenerateBeep(480.0f, 0.05f, 0.35f);
            wallSound     = GenerateBeep(240.0f, 0.03f, 0.20f);
            scoreSound    = GenerateBeep(130.0f, 0.18f, 0.40f);
            startSound    = GenerateBeep(587.33f, 0.12f, 0.35f);
            gameOverSound = GenerateBeep(98.0f, 0.35f, 0.45f);
            initialized   = true;
        }
    }

    static void Cleanup()
    {
        if (initialized)
        {
            UnloadSound(hitSound);
            UnloadSound(wallSound);
            UnloadSound(scoreSound);
            UnloadSound(startSound);
            UnloadSound(gameOverSound);
            CloseAudioDevice();
            initialized = false;
        }
    }
};

#endif //PONGARENA_AUDIOMANAGER_H
