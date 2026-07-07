#pragma once
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <iostream>

// Function pointer type for our simple C++ float buffer generator
typedef void (*AudioGeneratorFunc)(float* outputBuffer, int frameCount);

// Global pointer to track the user's sound generator function
static AudioGeneratorFunc g_userGenerator = nullptr;

// Internal Miniaudio callback that maps to our clean float generator
static void miniaudio_internal_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) 
{
    float* pOut = (float*)pOutput;

    if (g_userGenerator != nullptr) 
    {
        // Call the user's clean float math function
        g_userGenerator(pOut, (int)frameCount);
    } 
    else 
    {
        // Fallback to silence if no function is provided
        for (ma_uint32 i = 0; i < frameCount; ++i) 
        {
            pOut[i] = 0.0f;
        }
    }
    
    (void)pDevice;
    (void)pInput;
}

// Starts the hardware device and binds it to your sound math function
inline void StartAudioEngine(AudioGeneratorFunc generator) 
{
    g_userGenerator = generator;

    ma_device_config deviceConfig  = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = ma_format_f32;   // Float 32-bit samples
    deviceConfig.playback.channels = 1;               // Mono
    deviceConfig.sampleRate        = 48000;           // 48 kHz
    deviceConfig.dataCallback      = miniaudio_internal_callback;

    ma_device device;
    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) 
    {
        std::cerr << "Failed to initialize playback device.\n";
        return;
    }

    if (ma_device_start(&device) != MA_SUCCESS) 
    {
        std::cerr << "Failed to start audio device.\n";
        ma_device_uninit(&device);
        return;
    }

    std::cout << "--------------------------------------------------\n";
    std::cout << "🔊 Audio engine started successfully! (Mono @ 48kHz)\n";
    std::cout << "Press ENTER to stop and exit.\n";
    std::cout << "--------------------------------------------------\n";
    
    std::cin.get(); // Blocks main thread to keep audio device running

    ma_device_uninit(&device);
}
