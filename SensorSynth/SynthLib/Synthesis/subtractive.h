
#pragma once

#ifndef SUBTRACTIVE_H
#define SUBTRACTIVE_H
#include <array>
#include <cmath>
#include <algorithm>
namespace sensorsynth
{

    class SubtractiveSynth
    {
    public:
        SubtractiveSynth() {};
        ~SubtractiveSynth() {};

        void Init(float sampleRate)
        {
            this->sampleRate_ = sampleRate;
            frequency_ = 440.0f;
            amplitude_ = 1.0f;
            phase_ = 0.0f;
        }

        void SetFrequency(float freq) { frequency_ = freq; }
        void SetAmplitude(float amp) { amplitude_ = amp; }

        void ProcessBlock(float *buf, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
            {
                float sample = amplitude_ * (2.0f * (phase_ / (2.0f * M_PI)) - 1.0f);
                phase_ += 2.0f * M_PI * frequency_ / sampleRate_;
                phase_ = fmodf(phase_, 2.0f * M_PI);
                buf[i] = sample;
            }
        }

    private:
        float sampleRate_, size_;
        float frequency_;
        float amplitude_;
        float phase_;
    };
}

#endif // SUBTRACTIVE_H
