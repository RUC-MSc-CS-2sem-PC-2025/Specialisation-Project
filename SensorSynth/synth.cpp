#include "daisysp.h"
#include "daisy_seed.h"
#include "synth.h"
#include "./SynthLib/synthlib.h"

using namespace daisysp;
using namespace daisy;
using namespace sensorsynth;

static sensorsynth::Synth synth;

// void ConfigureADC(unsigned char num_connections, daisy::Pin *pins)
// {
//     AdcChannelConfig adc_cfg[num_connections];

//     for (unsigned char i = 0; i < num_connections; i += 1)
//     {
//         adc_cfg[i].InitSingle(pins[i]);
//     }

//     hw.adc.Init(adc_cfg, num_connections);
//     hw.adc.Start();
// }



// float InitHardware(daisy::DaisySeed &hw)
// {
//     float sample_rate;

//     hw.Init();
//     hw.SetAudioBlockSize(4);
//     sample_rate = hw.AudioSampleRate();
//     return sample_rate;
// }

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    float out_sig_L, out_sig_R;

    for (size_t i = 0; i < size; i += 2)
    {
        harmonydrone.Process(out_sig_L, out_sig_R);
        out[i] = out_sig_L;
        out[i + 1] = out_sig_R;
    }
}

int main(void)
{
    // float sample_rate;
    // sample_rate = InitHardware(hw);
    // ConfigureADC(7, adc_pins);
    // harmonydrone.Init(sample_rate, key_freq);
    synth.Init();
    synth.StartAudio(AudioCallback);
    //hw.StartAudio(AudioCallback);

    while (1)
    {
        synth.GetSensorValue(synth.pot1,0);
        synth.GetSensorValue(synth.pot1,0);
        synth.GetSensorValue(synth.pot1,0);
        synth.GetSensorValue(synth.pot1,0);
        synth.GetSensorValue(synth.pot1,0);
        synth.GetSensorValue(synth.pot1,0);

        pot1.SetValue(hw.adc.GetFloat(0));
        pot2.SetValue(hw.adc.GetFloat(1));
        pot3.SetValue(hw.adc.GetFloat(2));
        pot4.SetValue(hw.adc.GetFloat(3));
        pot5.SetValue(hw.adc.GetFloat(4));
        pot6.SetValue(hw.adc.GetFloat(5));
        pot7.SetValue(hw.adc.GetFloat(6));

        harmonydrone.SetVolume(pot1.GetValue());
        harmonydrone.OscOneChangePitch(key_freq * pot2.GetValue());
        harmonydrone.OscTwoChangePitch(key_freq * pot3.GetValue());
        harmonydrone.SetDelayTimeL(fclamp(pot4.GetValue(), 0.0f, 1.0f));
        harmonydrone.SetDelayTimeR(fclamp(pot5.GetValue(), 0.0f, 1.0f));
        harmonydrone.SetFilterFreq(pot6.GetValue());
        harmonydrone.SetFilterRes(pot7.GetValue());
    }
}