#include "daisysp.h"
#include "daisy_seed.h"
#include "./SynthLib/synthlib.h"
#include "./Hardware/hardware.h"
#include "./Hardware/controls.h"

#include <cmath>

using namespace daisysp;
using namespace daisy;
using namespace sensorsynth;

static sensorsynth::Hardware hw;
static sensorsynth::Controls controls;
static sensorsynth::SubtractiveSynth subtractive;
static daisysp::LadderFilter filter, filterLP;
static daisysp::DelayLine<float, 48000> delayS, delayL;
static daisysp::Oscillator lfo;

const size_t bufferSize = 32;

float amplitude = 0.5f;
float pitch = 440;
float lfo_freq = 100.0f;
float filter_cutoff = 500.0f;
float resonance = 0;

static void AudioCallback(AudioHandle::InputBuffer in,
                          AudioHandle::OutputBuffer out,
                          size_t size)
{
    subtractive.SetAmplitude(amplitude);
    subtractive.SetFrequency(pitch);
    lfo.SetFreq(lfo_freq);

    filter.SetFreq(filter_cutoff);
    filter.SetRes(resonance);

    std::array<float, bufferSize> block{};

    subtractive.ProcessBlock(block.data(), size);

    for (size_t i = 0; i < size; i++)
    {
        block[i] *= lfo.Process() * 0.75;
    }

    filter.ProcessBlock(block.data(), size);

    for (size_t i = 0; i < size; ++i)
    {
        float delayed = delayS.Read();
        float input_with_feedback = block[i] + delayed * 0.7;
        delayS.Write(input_with_feedback);
        float wet = 0.5f * delayed;

        float delayed2 = delayL.Read();
        float input_with_feedback2 = block[i] + delayed2 * 0.7;
        delayS.Write(input_with_feedback2);
        float wet2 = 0.5f * delayed2;
        float dry = 0.5f * block[i];

        float drymix = (dry + wet + wet2);

        float out_sample = daisysp::SoftClip(drymix);

        block[i] = out_sample;
    }

    filterLP.ProcessBlock(block.data(), size);

    for (size_t i = 0; i < size; i++)
    {
        out[0][i] = block[i];
        out[1][i] = block[i];
    }
}

int main(void)
{
    float sample_rate = hw.Init(bufferSize);

    controls.Init(hw.hw_, sample_rate);

    subtractive.Init(sample_rate);
    subtractive.SetAmplitude(0.5f);
    subtractive.SetFrequency(440.0f);

    lfo.Init(sample_rate);
    lfo.SetAmp(0.3f);
    lfo.SetWaveform(daisysp::Oscillator::WAVE_SQUARE);

    filter.Init(sample_rate);
    filter.SetFilterMode(daisysp::LadderFilter::FilterMode::BP12);

    delayS.Init();
    delayS.SetDelay(14400.f);

    delayL.Init();
    delayL.SetDelay(72000.f);

    filterLP.Init(sample_rate);
    filterLP.SetFilterMode(daisysp::LadderFilter::FilterMode::LP12);
    filterLP.SetRes(0);

    hw.StartAudio(AudioCallback);

    while (1)
    {
        controls.Process();

        amplitude = controls.pot1.Value();

        pitch = controls.pot2.Value();
        pitch = 33.0f + pitch * (1000.0f - 33.0f);

        filter_cutoff = controls.pot3.Value();
        filter_cutoff = 20.0f + filter_cutoff * (15000.0f - 20.0f);

        resonance = controls.pot4.Value();
        if (resonance < 0.0f)
            resonance = 0.0f;
        else if (resonance > 1.0f)
            resonance = 1.0f;
        resonance *= 0.8f;

        lfo_freq = controls.ldr1.Value();
        lfo_freq = 20.0f + lfo_freq * (500.0f - 20.0f);
    }
}