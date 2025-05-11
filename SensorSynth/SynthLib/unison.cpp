#include "unison.h"

using namespace sensorsynth;

void Unison::GenerateDetuneAmounts()
{
    detune_amounts_.clear();
    if (num_voices_ > 1)
    {
        for (int i = 0; i < num_voices_; ++i)
        {
            float value = -1.0f + (2.0f * i) / (num_voices_ - 1);
            detune_amounts_.push_back(value);
        }
    }
    else
    {
        detune_amounts_.push_back(0.0f);
    }
};

void Unison::GeneratePanPositions()
{
    pan_positions_.clear();
    if (num_voices_ > 1)
    {
        for (int i = 0; i < num_voices_; ++i)
        {
            float value = static_cast<float>(i) / (num_voices_ - 1);
            pan_positions_.push_back(value);
        }
    }
    else
    {
        pan_positions_.push_back(0.5f);
    }
};

void Unison::Process(float &in_L, float &in_R)
{
    float signal = 0.5f * (in_L + in_R);

    float left_output = 0.0f;
    float right_output = 0.0f;


    for (int i = 0; i < num_voices_; ++i)
    {
        float detuned_signal = signal * (1.0f + detune_amounts_[i]);
        float pan = pan_positions_[i];

        left_output += detuned_signal * (1.0f - pan);
        right_output += detuned_signal * pan;
    }

    in_L = left_output;
    in_R = right_output;
};