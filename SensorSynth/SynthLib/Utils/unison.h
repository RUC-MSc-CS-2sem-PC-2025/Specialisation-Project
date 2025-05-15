#include "daisysp.h"
#include <vector>
#include <cmath>

#pragma once
#ifndef UNISON_H
#define UNISON_H

#ifdef __cplusplus

namespace sensorsynth
{
    class Unison
    {
    public:
        std::vector<float> voices;
        Unison() {};
        ~Unison() {};

        void Init(float sample_rate, uint8_t n_voices);

        void Process(float in, float &out_left, float &out_right);

    private:
        void GenerateVoices()
        {
            for (size_t i = 0; i < nv_; i++)
            {
            }
        }

        float sr_;
        float nv_;
    };
}

#endif
#endif