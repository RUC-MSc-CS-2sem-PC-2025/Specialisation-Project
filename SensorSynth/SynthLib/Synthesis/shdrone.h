#include "../synthlib.h"
#include "daisysp.h"

#pragma once
#ifndef SHDRN_H
#define SHDRN_H

#ifdef __cplusplus

namespace sensorsynth
{
    class SHDrone
    {
    public:
        SHDrone(){};
        ~SHDrone(){};

        void Init(float sample_rate)
        {
            sr_ = sample_rate;

            osc1_.Init(sample_rate);
            osc1_.SetAmp(0.1f);
            osc1_.SetFreq(440);
            osc1_.SetWaveform(daisysp::Oscillator::WAVE_SQUARE);

        };

        void SetSHRate(float rate)
        {
            sh_rate_ = rate;
            sh_interval_ = 0.25f / sh_rate_; // Halve the interval for faster triggering
        }

        float Process()
        {
            // Update the S&H trigger based on the interval
            sh_timer_ += 1.0f / sr_; // Increment timer by one sample duration
            bool trigger = false;
            if (sh_timer_ >= sh_interval_)
            {
                trigger = true;
                sh_timer_ = 0.0f; // Reset timer
            }

            // Use SampleHold to modulate the pitch
            float base_pitch = 440.0f; // Base frequency in Hz
            float modulation_depth = 100.0f; // Modulation depth in Hz
            float modulated_pitch = sh_.Process(trigger, base_pitch + (rand() / (float)RAND_MAX) * modulation_depth, daisysp::SampleHold::MODE_SAMPLE_HOLD);

            // Set the modulated pitch to osc1_
            osc1_.SetFreq(modulated_pitch);

            // Process the oscillator (if needed, output the result)
            float output = osc1_.Process();

            return output;
        }

    private:
        daisysp::Oscillator osc1_;
        daisysp::SampleHold sh_;
        float sr_;
        float sh_rate_ = 1;  // S&H rate in Hz (default 1 Hz)
        float sh_interval_;  // Interval between triggers in seconds
        float sh_timer_ = 0;
    };
}

#endif
#endif
