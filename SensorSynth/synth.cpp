#include "daisysp.h"
#include "daisy_seed.h"
#include "./SynthLib/synthlib.h"

using namespace daisysp;
using namespace daisy;
using namespace sensorsynth;

static DaisySeed hw;
static GainControl out_gain;
static Photores photo1, photo2;

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

float InitHardware(daisy::DaisySeed hw)
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
    float sig1, out_sig_L, out_sig_R, lfo_gain;

    for (size_t i = 0; i < size; i += 2)
    {


        out_sig_L = sig1 * lfo_gain;
        out_sig_R = sig1 * lfo_gain;
        out[i] = out_gain.AddGain(out_sig_L);
        out[i + 1] = out_gain.AddGain(out_sig_R);
    }
}

int main(void)
{
    float sample_rate = InitHardware(hw);
    ConfigureADC(3, adc_pins);

    out_gain.Init();
    out_gain.SetGain(0.5f);

    osc1.Init(sample_rate);
    osc1.SetWaveform(osc1.WAVE_SIN);
    osc1.SetFreq(110);
    osc1.SetAmp(1);

    lfo1.Init(sample_rate);
    lfo1.SetWaveform(lfo1.WAVE_SIN);
    lfo1.SetFreq(0.5f);
    lfo1.SetAmp(1);

    hw.StartAudio(AudioCallback);

    while (1)
    {
        photo1.SetValue(hw.adc.GetFloat(0));
        photo2.SetValue(hw.adc.GetFloat(1));

        osc1.SetFreq(440 * (photo1.GetValue() * 2.0f));

        out_gain.SetGain(photo2.GetValue());
    }
}