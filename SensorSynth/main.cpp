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
static AnalogControl pot1, pot2, pot3, photo1, photo2;
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
    cpu.Init(sample_rate, 256);

    // Initialize AnalogControl objects
    pot1.Init(hw.hw_.adc.GetPtr(0), sample_rate);
    pot2.Init(hw.hw_.adc.GetPtr(1), sample_rate);
    pot3.Init(hw.hw_.adc.GetPtr(2), sample_rate);
    photo1.Init(hw.hw_.adc.GetPtr(3), sample_rate);
    photo2.Init(hw.hw_.adc.GetPtr(4), sample_rate);

    subtractive.Init(sample_rate);
    subtractive.setAmplitude(0.5f);
    subtractive.setFrequency(440.0f);

    hw.StartAudio(AudioCallback);

    size_t print_counter = 0;
    const size_t print_interval = 10000;

    while (1)
    {
        // Update AnalogControl objects
        pot1.Process();
        pot2.Process();
        pot3.Process();
        photo1.Process();
        photo2.Process();

        // Use pot1 to control amplitude
        float amplitude = pot1.Value(); // Read normalized value (0.0 to 1.0)
        subtractive.setAmplitude(amplitude);

        float filter_cutoff = pot2.Value();
        filter_cutoff = 15000 / filter_cutoff;
        subtractive.setFilterCutoff(filter_cutoff);
    }
}