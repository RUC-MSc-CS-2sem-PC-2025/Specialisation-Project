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
static AdditiveSynth addsynth;
static FMSynthesizer fm;
static WavetableSynth wavetable;
CpuLoadMeter cpu;

static void AudioCallback(AudioHandle::InputBuffer in,
                          AudioHandle::OutputBuffer out,
                          size_t size)
{
    cpu.OnBlockStart();
    std::array<float, 256> block = subtractive.generateBlock();
    for (size_t i = 0; i < size; ++i)
    {
        out[0][i] = block[i];
        out[1][i] = block[i];
    }
    cpu.OnBlockEnd();
}

int main(void)
{
    float sample_rate = hw.Init(256);
    hw.hw_.StartLog();
    cpu.Init(sample_rate, 256);

    subtractive.Init(sample_rate);
    subtractive.setAmplitude(0.5f);
    subtractive.setFrequency(440.0f);

    addsynth.init(sample_rate, 440.0f);

    fm.init(sample_rate);

    wavetable.init();

    hw.StartAudio(AudioCallback);

    size_t print_counter = 0;
    const size_t print_interval = 10000;

    while (1)
    {
        if (++print_counter >= print_interval)
        {
            float avgcpu = cpu.GetAvgCpuLoad();

            hw.hw_.PrintLine("Avg: %f", avgcpu);
            print_counter = 0;
        }
    }
}