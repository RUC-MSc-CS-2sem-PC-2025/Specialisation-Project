#include "daisysp.h"
#include "unison.h"
#include "gaincontrol.h"

#include <algorithm>

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
        void SetSampleRate(float sample_rate) { sample_rate_ = sample_rate; };
        void SetOutL(float in) { out_L_ = in; };
        void SetOutR(float in) { out_R_ = in; };
        

        float GetKeyFreq() { return key_freq_; };
        float GetSampleRate() { return sample_rate_; };
        float GetOutL() { return out_L_; };
        float GetOutR() { return out_R_; };

        void InitOsc(daisysp::Oscillator &osc, float sample_rate, float freq, float amp, uint8_t waveform);
        void InitDelay(float &sample_rate, daisysp::DelayLine<float, 48000> &delay_line_L, daisysp::DelayLine<float, 48000> &delay_line_R);
        void InitUnison(u_int8_t voices);
        void InitFilter(float sample_rate, float freq, float res);
        void InitGain();
        void Init(const float sample_rate, float key_freq);


        void Unison(float signal);
        void Delay();
        void Volume();
        
        void Process(float &L, float &R);

        void OscOneChangePitch(float new_feq);
        void OscTwoChangePitch(float new_feq);
        void UnisoneDetune(float value);
        void SetDelayTimeL(float value);
        void SetDelayTimeR(float value);
        void SetFilterFreq(float freq);
        void SetFilterRes(float res);
        void SetVolume(float value);


    private:
        sensorsynth::Unison uni_;
        sensorsynth::GainControl gain1_;
        daisysp::Oscillator osc1_, osc2_;
        daisysp::DelayLine<float, 48000> delay_line_L;
        daisysp::DelayLine<float, 48000> delay_line_R;
        daisysp::LadderFilter ladder_filter_;


        float key_freq_;
        float sample_rate_;
        float out_L_, out_R_;
    };

}
#endif
#endif