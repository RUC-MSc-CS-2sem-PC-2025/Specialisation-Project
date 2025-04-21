#include "daisysp.h"
#include "daisy_seed.h"
#include "./SynthLib/synthlib.h"

using namespace daisysp;
using namespace daisy;
using namespace sensorsynth;

static DaisySeed hw;
static GainControl out_gain;
static Photores photo1, photo2;
static HarmonyDrone harmonydrone;
static Oscillator osc;

daisy::Pin adc_pins[] =
    {
        daisy::seed::A0,
        daisy::seed::A1,
        daisy::seed::A2};

static float key_freq = 440.0f;

void ConfigureADC(unsigned char num_connections, daisy::Pin pins[])
{
    AdcChannelConfig adc_cfg[num_connections];

    for (unsigned char i = 0; i < num_connections; i += 1)
    {
        adc_cfg[i].InitSingle(pins[i]);
    }

    hw.adc.Init(adc_cfg, num_connections);
}

float InitHardware(daisy::DaisySeed& hw)
{
    float sample_rate;

    hw.Init();
    hw.SetAudioBlockSize(4);
    sample_rate = hw.AudioSampleRate();
    hw.adc.Start();

    return sample_rate;
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    float sig1, out_sig_L, out_sig_R;

    for (size_t i = 0; i < size; i += 2)
    {
        sig1 = harmonydrone.Process();

        out_sig_L = sig1;
        out_sig_R = sig1;

        out[i] = out_sig_L;
        out[i + 1] = out_sig_R;
    }
}

int main(void)
{
    float sample_rate;

    hw.Init();
    hw.SetAudioBlockSize(4);
    sample_rate = hw.AudioSampleRate();

    ConfigureADC(3, adc_pins);
    hw.adc.Start();

    harmonydrone.Init(sample_rate, key_freq);

    osc.Init(sample_rate);
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetFreq(key_freq);
    osc.SetAmp(1);

    out_gain.Init();
    out_gain.SetGain(0.5f);

    hw.StartAudio(AudioCallback);

    while (1)
    {
        //photo1.SetValue(hw.adc.GetFloat(0));
        //photo2.SetValue(hw.adc.GetFloat(1));
        //out_gain.SetGain(photo2.GetValue());
    }
}