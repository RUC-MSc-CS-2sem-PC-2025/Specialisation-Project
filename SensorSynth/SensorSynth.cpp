#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed  hw;
static Oscillator osc;
static Fm2        fm2;
static float      amp = 0.5f;

static void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t                                size)
{
    float sig1, sig2, mixed_sig;
    for(size_t i = 0; i < size; i += 2)
    {
        sig1 = fm2.Process() * amp;
        sig2 = osc.Process() * amp;

        mixed_sig = sig1 + sig2;

        out[i] = mixed_sig;
        out[i + 1] = mixed_sig;
    }
}

int main(void)
{
    float sample_rate;
    //hardware configuration
    hw.Init();
    hw.SetAudioBlockSize(4);
    sample_rate = hw.AudioSampleRate();

    AdcChannelConfig adc_cfg[2];
    adc_cfg[0].InitSingle(daisy::seed::A0);
    adc_cfg[1].InitSingle(daisy::seed::A2);
    hw.adc.Init(adc_cfg, 2);
    hw.adc.Start();
    //hw.StartLog(true);

    //oscilator configuration
    osc.Init(sample_rate);
    osc.SetWaveform(osc.WAVE_SAW);
    osc.SetFreq(55);
    osc.SetAmp(1);

    //fm2 configuration
    fm2.Init(sample_rate);
    fm2.SetFrequency(440);
    fm2.SetRatio(1.0f);
    fm2.SetIndex(1.0f);


    hw.StartAudio(AudioCallback);
    

    while(1) {
        float adc_value_1 = hw.adc.GetFloat(0);
        float adc_value_2 = hw.adc.GetFloat(1);

        fm2.SetRatio(1.0f + (adc_value_1 * 2.0f));
        amp = adc_value_2 * 0.1f;
    }
}