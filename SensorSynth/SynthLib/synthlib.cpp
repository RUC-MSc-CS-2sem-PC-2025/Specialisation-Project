#include "synthlib.hpp"
#include "../constants.hpp"
#include "daisy_seed.h"

using namespace sensorsynth;

void SynthLib::Init(float sample_rate)
{
    this->sample_rate = sample_rate;

    subtractive.Init(sample_rate);
    subtractive.SetAmplitude(0.5f);
    subtractive.SetFrequency(55.0f);

    filterHP.Init(sample_rate);
    filterHP.SetRes(0.1f);
    filterHP.SetFilterMode(daisysp::LadderFilter::FilterMode::HP12);
    filterHP.SetFreq(70.0f);


    filterLP.Init(sample_rate);
    filterLP.SetRes(0.1f);
    filterLP.SetFilterMode(daisysp::LadderFilter::FilterMode::LP12);
    filterLP.SetFreq(13000.0f);

    filterBP.Init(sample_rate);
    filterBP.SetRes(0.9f);
    filterBP.SetFilterMode(daisysp::LadderFilter::FilterMode::BP12);

    lfo.Init(sample_rate);
    lfo.SetAmp(0.3f);
    lfo.SetFreq(440.0f);
    lfo.SetWaveform(daisysp::Oscillator::WAVE_SQUARE);

    delayS.Init();
    delayS.SetDelay(14400.f);

    chorus.Init(sample_rate);
}

void SynthLib::ProcessAudio(daisy::AudioHandle::InputBuffer in, daisy::AudioHandle::OutputBuffer out, size_t size)
{
    std::array<float, BLOCK_SIZE> block{};
    subtractive.ProcessBlock(block.data(), size);
    filterLP.ProcessBlock(block.data(), size);
    filterBP.ProcessBlock(block.data(), size);

    for (size_t i = 0; i < size; i++)
    {
        block[i] *= lfo.Process() * 0.75;
        block[i] = chorus.Process(block[i]);
    }

    for (size_t i = 0; i < size; ++i)
    {
        float delayed = delayS.Read();
        float input_with_feedback = block[i] + delayed * 0.7;
        delayS.Write(input_with_feedback);
        float wet = 0.5f * delayed;
        float dry = 0.5f * block[i];
        float drymix = dry + wet;
        float out_sample = daisysp::SoftClip(drymix);
        block[i] = out_sample * 0.5f;
    }

    filterHP.ProcessBlock(block.data(), size);

    for (size_t i = 0; i < size; i++)
    {
        out[0][i] = block[i];
        out[1][i] = block[i];
    }
}

void SynthLib::SetValues(uint8_t amp, uint8_t freq, uint8_t filter_cutoff, uint8_t amp_enable)
{
    float amplitude = 0.1f + ((float)amp / 255.0f) * (0.5f - 0.1f);
    float pitch = 100.0f + ((float)freq / 255.0f) * (5000.0f - 100.0f);
    float cutoff = 55.0f + ((float)freq / 255.0f) * (3000.0f - 55.0f);
    float chorus_delay = (float)filter_cutoff / 255.0f;
    float lfo_freq = 20.0f + ((float)filter_cutoff / 255.0f) * (500.0f - 20.0f);

    //subtractive.SetFrequency(pitch);
    subtractive.SetAmplitude((float)amp_enable * 0.5f);
    filterBP.SetFreq(cutoff);
    chorus.SetDelay(chorus_delay);
}
