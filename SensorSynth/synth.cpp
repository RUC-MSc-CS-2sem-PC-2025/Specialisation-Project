#include "daisysp.h"
#include "daisy_seed.h"
#include "./SynthLib/synthlib.h"

using namespace daisysp;
using namespace daisy;
using namespace sensorsynth;

static DaisySeed hw;
static Oscillator osc1, osc2, osc3, osc4;
static Oscillator lfo1, lfo2;
static GainControl out_gain;

static float key_freq = 440.0f;

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    float sig1, out_sig_L, out_sig_R, lfo_gain;

    for (size_t i = 0; i < size; i += 2)
    {
        sig1 = osc1.Process();
        lfo_gain = (lfo1.Process() + 1.0f) * 0.5f;

        out_sig_L = sig1 * lfo_gain;
        out_sig_R = sig1 * lfo_gain;
        out[i] = out_gain.AddGain(out_sig_L);
        out[i + 1] = out_gain.AddGain(out_sig_R);
    }
}

int main(void)
{
    float sample_rate;

    // hardware configuration
    hw.Init();
    hw.SetAudioBlockSize(4);
    sample_rate = hw.AudioSampleRate();

    AdcChannelConfig adc_cfg[3];
    adc_cfg[0].InitSingle(daisy::seed::A0);
    adc_cfg[1].InitSingle(daisy::seed::A1);
    adc_cfg[2].InitSingle(daisy::seed::A2);
    hw.adc.Init(adc_cfg, 3);
    hw.adc.Start();

    // gaincontrole configuration
    out_gain.Init();
    out_gain.SetGain(0.5f);

    // oscilator configuration
    osc1.Init(sample_rate);
    osc1.SetWaveform(osc1.WAVE_SIN);
    osc1.SetFreq(110);
    osc1.SetAmp(1);

    // LFO configuration
    lfo1.Init(sample_rate);
    lfo1.SetWaveform(lfo1.WAVE_SIN);
    lfo1.SetFreq(0.5f);
    lfo1.SetAmp(1);

    hw.StartAudio(AudioCallback);

    while (1)
    {
        float photoA0 = hw.adc.GetFloat(0);
        float photoA1 = hw.adc.GetFloat(1);
        float potA2 = hw.adc.GetFloat(2);

        osc1.SetFreq(440 * (photoA0 * 2.0f));

        out_gain.SetGain(potA2);
    }
}