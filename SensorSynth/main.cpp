#include "daisysp.h"
#include "daisy_seed.h"
#include "./SynthLib/synthlib.h"
#include "./Hardware/hardware.h"

#include <cmath>

using namespace daisysp;
using namespace daisy;
using namespace sensorsynth;

static sensorsynth::Hardware hw;
static sensorsynth::SubtractiveSynth subtractive;
static daisysp::LadderFilter filter, filterLP;
static daisysp::DelayLine<float, 48000> delayS, delayL;
static daisysp::Oscillator lfo;
static AnalogControl pot1, pot2, pot3, pot4, pot5, photo1;
const size_t bufferSize = 256;

float resonance = 0;
float pitch = 440;
float lfo_freq = 100.0f;
float freqLP = 1000.0f;
float amplitude = 0.5f;
float filter_cutoff = 500.0f;

static void AudioCallback(AudioHandle::InputBuffer in,
                          AudioHandle::OutputBuffer out,
                          size_t size)
{
    subtractive.SetAmplitude(amplitude);

    filter.SetFreq(filter_cutoff);
    filter.SetRes(resonance);

    filterLP.SetFreq(freqLP);

    subtractive.SetFrequency(pitch);

    lfo.SetFreq(lfo_freq);

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
    float sample_rate = hw.Init(256);

    pot1.Init(hw.hw_.adc.GetPtr(0), sample_rate);
    pot2.Init(hw.hw_.adc.GetPtr(1), sample_rate);
    pot3.Init(hw.hw_.adc.GetPtr(2), sample_rate);
    pot4.Init(hw.hw_.adc.GetPtr(3), sample_rate);
    pot5.Init(hw.hw_.adc.GetPtr(4), sample_rate);
    photo1.Init(hw.hw_.adc.GetPtr(5), sample_rate);

    lfo.Init(sample_rate);
    lfo.SetAmp(0.3f);
    lfo.SetWaveform(daisysp::Oscillator::WAVE_SQUARE);

    subtractive.Init(sample_rate);
    subtractive.SetAmplitude(0.5f);
    subtractive.SetFrequency(440.0f);

    filter.Init(sample_rate);
    filter.SetFilterMode(daisysp::LadderFilter::FilterMode::BP24);

    filterLP.Init(sample_rate);
    filter.SetFilterMode(daisysp::LadderFilter::FilterMode::LP12);
    filter.SetRes(0);

    delayS.Init();
    delayS.SetDelay(14400.f);

    delayL.Init();
    delayL.SetDelay(72000.f);


    hw.StartAudio(AudioCallback);

    while (1)
    {
        pot1.Process();
        pot2.Process();
        pot3.Process();
        pot4.Process();
        pot5.Process();
        photo1.Process();

        amplitude = pot1.Value();

        filter_cutoff = pot2.Value();
        filter_cutoff = 20.0f + filter_cutoff * (15000.0f - 20.0f);

        resonance = pot3.Value();
        if (resonance < 0.0f)
            resonance = 0.0f;
        else if (resonance > 1.0f)
            resonance = 1.0f;
        resonance *= 0.8f;

        lfo_freq = pot4.Value();
        lfo_freq = 33.0f + lfo_freq * (1000.0f - 33.0f);

        freqLP = pot5.Value();
        freqLP = 20.0f + freqLP * (15000.0f - 20.0f);

        pitch = photo1.Value();
        pitch = 20.0f + pitch * (500.0f - 20.0f);
    }
}