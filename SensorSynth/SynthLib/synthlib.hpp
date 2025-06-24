#pragma once

#include "./Synthesis/subtractive.h"
#include "daisy_seed.h"
#include "daisysp.h"

struct daisy_data {
	uint8_t sound_on;
	uint8_t left_hand;
	uint8_t right_hand;
	uint8_t placeholder;
};

typedef struct daisy_data daisy_data_t;

namespace sensorsynth
{
    class SynthLib
    {
    public:
        void Init(float sample_rate);
        void ProcessAudio(daisy::AudioHandle::InputBuffer in, daisy::AudioHandle::OutputBuffer out, size_t size);
        void SetValues(daisy_data_t *input);
    private:
        float sample_rate;
        SubtractiveSynth subtractive;
        daisysp::LadderFilter filterHP, filterLP, filterBP;
        daisysp::Oscillator lfo;
        daisysp::DelayLine<float, 48000> delayS;
        daisysp::Chorus chorus;
    };
}
