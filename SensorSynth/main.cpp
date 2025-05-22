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
static daisysp::LadderFilter filter;
static AnalogControl pot1, pot2, pot3, pot4, pot5, pot6, photo1, photo2;
CpuLoadMeter cpu;
const size_t bufferSize = 256;

static void AudioCallback(AudioHandle::InputBuffer in,
                          AudioHandle::OutputBuffer out,
                          size_t size)
{
    cpu.OnBlockStart();
    std::array<float, bufferSize> block{};
    subtractive.ProcessBlock(block.data(), size);
    filter.ProcessBlock(block.data(), size);


        for (size_t i = 0; i < size; ++i)
    {
        out[0][i] = daisysp::SoftClip(block[i]);
        out[1][i] = daisysp::SoftClip(block[i]);
    }
    cpu.OnBlockEnd();
}

int main(void)
{
    float sample_rate = hw.Init(256);
    cpu.Init(sample_rate, 256);

    pot1.Init(hw.hw_.adc.GetPtr(0), sample_rate);
    pot2.Init(hw.hw_.adc.GetPtr(1), sample_rate);
    pot3.Init(hw.hw_.adc.GetPtr(2), sample_rate);
    pot4.Init(hw.hw_.adc.GetPtr(3), sample_rate);
    pot5.Init(hw.hw_.adc.GetPtr(4), sample_rate);
    pot6.Init(hw.hw_.adc.GetPtr(5), sample_rate);
    photo1.Init(hw.hw_.adc.GetPtr(6), sample_rate);
    photo2.Init(hw.hw_.adc.GetPtr(7), sample_rate);

    subtractive.Init(sample_rate);
    subtractive.SetAmplitude(0.5f);
    subtractive.SetFrequency(440.0f);

    filter.Init(sample_rate);
    filter.SetFilterMode(daisysp::LadderFilter::FilterMode::BP12);



    hw.StartAudio(AudioCallback);

    while (1)
    {

        pot1.Process();
        pot2.Process();
        pot3.Process();
        photo1.Process();
        photo2.Process();

        float amplitude = pot1.Value();
        subtractive.SetAmplitude(amplitude);

        float filter_cutoff = pot2.Value();
        filter_cutoff = 20.0f + filter_cutoff * (15000.0f - 20.0f);
        filter.SetFreq(filter_cutoff);

        float resonance = pot3.Value();
        if (resonance < 0.0f)
            resonance = 0.0f;
        else if (resonance > 1.0f)
            resonance = 1.0f;
        resonance *= 0.8f;
        filter.SetRes(resonance);
    }
}