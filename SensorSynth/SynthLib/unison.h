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
        Unison() {};
        ~Unison() {};

        float GetNumVoices() { return num_voices_; };

        void SetDetuneAmmounts(float scale)
        {
            GenerateDetuneAmounts();
            for (auto &detune : detune_amounts_)
            {
                detune *= scale;
            }
        };

        void Init(unsigned char voices)
        {
            num_voices_ = voices;
            GenerateDetuneAmounts();
            GeneratePanPositions();

        };

        void Process(float &in_L, float &in_R);

    private:
        void GenerateDetuneAmounts();
        void GeneratePanPositions();
        unsigned char num_voices_;
        std::vector<float> detune_amounts_;
        std::vector<float> pan_positions_;
    };
}

#endif
#endif