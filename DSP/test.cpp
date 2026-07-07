#include "audio_engine.h"
#include <cmath>

// ============================================================================
// 🧠 YOUR DSP PLAYGROUND
// ----------------------------------------------------------------------------
// outputBuffer: an array of floats. Set each index to a value between -1.0f and 1.0f.
// frameCount: the size of the array (number of samples to write).
// ============================================================================

// Tip: You will need a variable to keep track of the wave's phase (angle) 
// across callbacks so the sound doesn't glitch. Declare it globally or static.
float phase = 0.0f;
float frequency = 400.0f;

void generate_audio(float* outputBuffer, int frameCount) 
{
    // Write your sine wave math loop here!
    // Fill outputBuffer[i] for each sample up to frameCount.

    for (int i = 0; i < frameCount; ++i) 
    {
        outputBuffer[i] = sinf(phase) * 0.1f;   // Silence (change this!
        phase += (2.0f * 3.14159f * frequency) / 48000.0f;
        if (phase > 2.0f * 3.14159f)
        {
            phase -= 2.0f * 3.14159f;
        }
        frequency += 0.1f;
    }
}

int main()
{
    StartAudioEngine(generate_audio);
    return 0;
}
