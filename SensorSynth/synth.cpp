#include "daisysp.h"
#include "daisy_seed.h"
#include "./SynthLib/synthlib.h"

using namespace daisysp;
using namespace daisy;
using namespace sensorsynth;

static DaisySeed hw;
static GainControl out_gain;
static Sensor pot1, pot2, pot3, pot4, pot5, pot6;
static HarmonyDrone harmonydrone;

daisy::Pin adc_pins[] =
    {
        daisy::seed::A0,
        daisy::seed::A1,
        daisy::seed::A2,
        daisy::seed::A3,
        daisy::seed::A4,
        daisy::seed::A5

    };

static float key_freq = 440.0f;

void ConfigureADC(unsigned char num_connections, daisy::Pin* pins)
{
    AdcChannelConfig adc_cfg[num_connections];

    for (unsigned char i = 0; i < num_connections; i += 1)
    {
        adc_cfg[i].InitSingle(pins[i]);
    }

    hw.adc.Init(adc_cfg, num_connections);
    hw.adc.Start();
}

float InitHardware(daisy::DaisySeed& hw)
{
    float sample_rate;

    hw.Init();
    hw.SetAudioBlockSize(4);
    sample_rate = hw.AudioSampleRate();
    return sample_rate;
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    float out_sig_L, out_sig_R;

    for (size_t i = 0; i < size; i += 2)
    {
        harmonydrone.Process(out_sig_L, out_sig_R);
        out[i] = out_gain.AddGain(out_sig_L);
        out[i + 1] = out_gain.AddGain(out_sig_R);
    }
}

int main(void)
{
    float sample_rate;
    sample_rate = InitHardware(hw);

    ConfigureADC(6, adc_pins);

    harmonydrone.Init(sample_rate, key_freq);

    out_gain.Init();
    out_gain.SetGain(0.5f);

    hw.StartAudio(AudioCallback);

    while (1)
    {
        pot1.SetValue(hw.adc.GetFloat(0));
        pot2.SetValue(hw.adc.GetFloat(1));
        pot3.SetValue(hw.adc.GetFloat(2));
        pot4.SetValue(hw.adc.GetFloat(3));
        pot5.SetValue(hw.adc.GetFloat(4));
        pot6.SetValue(hw.adc.GetFloat(5));


        out_gain.SetGain(pot1.GetValue());
        harmonydrone.OscOneChangePitch(key_freq * pot2.GetValue());
        harmonydrone.OscTwoChangePitch(key_freq * pot3.GetValue());

    }
}