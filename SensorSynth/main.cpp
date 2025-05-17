#include "daisysp.h"
#include "daisy_seed.h"
#include "./SynthLib/synthlib.h"
#include "./Hardware/hardware.h"
#include "./Hardware/Logger.h"
#include <cmath>

using namespace daisysp;
using namespace daisy;
using namespace sensorsynth;

static sensorsynth::Hardware hw;
static sensorsynth::Synth synth;
AnalogControl pot;

static void AudioCallback(AudioHandle::InputBuffer in,
                          AudioHandle::OutputBuffer out,
                          size_t size)
{
    float out_left, out_right;
    for (size_t i = 0; i < size; i++)
    {

        synth.Process(out_left, out_right);

        out[0][i] = out_left;  
        out[1][i] = out_right; 
    }
}

int main(void)
{
    float sample_rate = hw.Init(512);
    synth.Init(sample_rate);

    hw.StartAudio(AudioCallback);
    size_t num_sensors = hw.GetNumberOfSensors();
    std::vector<float> previous_values(num_sensors, 0.0f);

    while (1)
    {
        hw.ReadSensors();

        for (size_t i = 0; i < num_sensors; ++i)
        {
            float current_value = hw.GetSensorValue(i);
            if (current_value != previous_values[i])
            {
                previous_values[i] = current_value;

                switch (i)
                {
                case 0:
                    synth.MacroOne(current_value);
                    break;
                case 1:
                    synth.MacroTwo(current_value);
                    break;
                case 2:
                    synth.MacroThree(current_value);
                    break;
                case 3:
                    synth.MacroFour(current_value);
                    break;
                case 4:
                    synth.MacroFive(current_value);
                    break;
                case 5:
                    synth.MacroSix(current_value);
                    break;
                case 6:
                    synth.MacroSeven(current_value);
                    break;
                }
            }
        }
    }
}