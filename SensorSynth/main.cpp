#include "daisysp.h"
#include "daisy_seed.h"
#include "./SynthLib/synthlib.h"
#include "./Hardware/hardware.h"
#include "./Hardware/logger.h"
#include <cmath>

using namespace daisysp;
using namespace daisy;
using namespace sensorsynth;

static sensorsynth::Hardware hw;
static sensorsynth::Synth synth;
CpuLoadMeter cpu;

static void AudioCallback(AudioHandle::InputBuffer in,
                          AudioHandle::OutputBuffer out,
                          size_t size)
{
    cpu.OnBlockStart();

    synth.Process(size, out);

    cpu.OnBlockEnd();
}

int main(void)
{
    float sample_rate = hw.Init(256);
    synth.Init(sample_rate);

    daisy::DaisySeed hw_ = hw.GetDaisySeed();
    hw_.StartLog();

    cpu.Init(sample_rate, 256);

    hw.StartAudio(AudioCallback);
    size_t num_sensors = hw.GetNumberOfSensors();
    std::vector<float> previous_values(num_sensors, 0.0f);

    size_t print_counter = 0;
    const size_t print_interval = 10000;

    while (1)
    {
        hw.ReadSensors();

        if (++print_counter >= print_interval)
        {
            float avgcpu = cpu.GetAvgCpuLoad();

            hw_.PrintLine("Avg: %f", avgcpu);
            print_counter = 0;
        }

        // for (size_t i = 0; i < num_sensors; ++i)
        // {
        //     float current_value = hw.GetSensorValue(i);
        //     if (current_value != previous_values[i])
        //     {
        //         previous_values[i] = current_value;

        //         switch (i)
        //         {
        //         case 0:
        //             synth.MacroOne(current_value);
        //             break;
        //         case 1:
        //             synth.MacroTwo(current_value);
        //             break;
        //         case 2:
        //             synth.MacroThree(current_value);
        //             break;
        //         case 3:
        //             synth.MacroFour(current_value);
        //             break;
        //         case 4:
        //             synth.MacroFive(current_value);
        //             break;
        //         case 5:
        //             synth.MacroSix(current_value);
        //             break;
        //         case 6:
        //             synth.MacroSeven(current_value);
        //             break;
        //         }
        //     }
        // }
    }
}