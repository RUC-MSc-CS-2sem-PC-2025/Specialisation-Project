#include "daisysp.h"

#pragma once
#ifndef INSTRMNS_H
#define INSTRMNS_H

#ifdef __cplusplus

namespace sensorsynth
{
    class HarmonyDrone
    {

    public:
        HarmonyDrone() {};
        ~HarmonyDrone() {};

        void Init(float sample_rate, float key_freq);

        void SetKeyFreq(float freq);
        float GetKeyFreq();
        void SetSampleRate(float sample_rate);
        float GetSampleRate();

        void InitOsc(daisysp::Oscillator &osc, float sample_rate, float freq, float amp, uint8_t waveform);
        float Process();

    private:
        daisysp::Oscillator osc1, osc2, osc3, osc4;
        daisysp::Oscillator lfo1, lfo2;
        float key_freq_;
        float sample_rate_;
    };

}
#endif
#endif