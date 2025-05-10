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

        void SetKeyFreq(float freq) { key_freq_ = freq; };
        float GetKeyFreq() { return key_freq_; };
        void SetSampleRate(float sample_rate) { sample_rate_ = sample_rate; };
        float GetSampleRate() { return sample_rate_; };

        void SetOutL(float in) { out_L_ = in; };
        void SetOutR(float in) { out_R_ = in; };
        float GetOutL() { return out_L_; };
        float GetOutR() { return out_R_; };

        void InitOsc(daisysp::Oscillator &osc, float sample_rate, float freq, float amp, uint8_t waveform);
        void Init(const float sample_rate, float key_freq);

        void OscOneChangePitch(float new_feq);
        void OscTwoChangePitch(float new_feq);
        void Unison(float signal);
        void Delay();
        void Process(float &L, float &R);

    private:
        daisysp::Oscillator osc1_, osc2_;

        float key_freq_;
        float sample_rate_;
        float out_L_, out_R_;
    };

}
#endif
#endif