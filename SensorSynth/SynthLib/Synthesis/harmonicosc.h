#include "daisysp.h"

#pragma once
#ifndef HARMONICOSC_H
#define HARMONICOSC_H

#ifdef __cplusplus

namespace sensorsynth
{
    class HarmonicOsc
    {
    public:
        HarmonicOsc() {};
        ~HarmonicOsc() {};

        void Init(float sample_rate, float freq, u_int8_t voices)
        {
            InitOsc(sample_rate);

            sr_ = sample_rate;
            freq_ = freq;
            current_freq_ = freq;
            slew_rate_ = 10.0f;
            glide_time_ = 0.01f;
            ctrl_2nd = 3.0f;
            d_amount = 0.01f;

            n_voices_ = voices;
        };

        void Process(float &out_l, float &out_r);

        float ProcessVoice();

        inline void SetFrequency(float freq) { freq_ = freq; };

        inline float GetFrequency() { return freq_; };

        inline void SetGlidetime(float glide_time) { glide_time_ = glide_time; };

        inline void SetSecondControl(float value) { ctrl_2nd = roundf(value * 12.0f); };

        inline void SetDetuneAmount(float value)
        {
            d_amount = value * 100.0f; 
        };

    private:
        void InitOsc(float sample_rate)
        {
            osc1_.Init(sample_rate);
            osc2_.Init(sample_rate);
            osc3_.Init(sample_rate);
            osc4_.Init(sample_rate);

            osc1_.SetFreq(freq_);
            osc2_.SetFreq(freq_ * pow(2.0f, 3.0f / 12.0f));
            osc3_.SetFreq(freq_ * 1.5f);
            osc4_.SetFreq(freq_ * pow(2.0f, 10.0f / 12.0f));

            osc1_.SetAmp(0.25f);
            osc2_.SetAmp(0.25f);
            osc3_.SetAmp(0.25f);
            osc4_.SetAmp(0.25f);

            osc4_.PhaseAdd(0.25f);

            osc1_.SetWaveform(daisysp::Oscillator::WAVE_SQUARE);
            osc2_.SetWaveform(daisysp::Oscillator::WAVE_SIN);
            osc3_.SetWaveform(daisysp::Oscillator::WAVE_SIN);
            osc4_.SetWaveform(daisysp::Oscillator::WAVE_SIN);
        };

        daisysp::Oscillator osc1_, osc2_, osc3_, osc4_;
        std::vector<float> voices_;
        float freq_;
        float sr_;
        float current_freq_, slew_rate_, glide_time_;
        float ctrl_2nd;
        float d_amount;
        u_int8_t n_voices_;
    };
}

#endif
#endif // HARMONICOSC_H