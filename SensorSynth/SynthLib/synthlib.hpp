#pragma once

#include "./Synthesis/subtractive.h"
#include "daisy_seed.h"
#include "daisysp.h"

namespace sensorsynth
{
    class SynthLib
    {
    public:
        void Init(float sample_rate);
        void ProcessAudio(daisy::AudioHandle::InputBuffer in, daisy::AudioHandle::OutputBuffer out, size_t size);
        void SetValues(uint8_t amp, uint8_t freq, uint8_t filter_cutoff, uint8_t amp_enable);
    private:
        float sample_rate;
        SubtractiveSynth subtractive;
        daisysp::LadderFilter filter, filterLP;
        daisysp::Oscillator lfo;
        daisysp::DelayLine<float, 48000> delayS;
        daisysp::Chorus chorus;
    };
}
