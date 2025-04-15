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
    float sig;
    for(size_t i = 0; i < size; i += 2)
    {
        sig = fm2.Process();

        // left out
        out[i] = sig;

        // right out
        out[i + 1] = sig;
    }
}

int main(void)
{
    float sample_rate;
    //hardware configuration
    hw.Init();
    hw.SetAudioBlockSize(4);
    sample_rate = hw.AudioSampleRate();

    AdcChannelConfig adc_cfg;
    adc_cfg.InitSingle(daisy::seed::A0);
    adc_cfg.InitSingle(daisy::seed::A2);
    hw.adc.Init(&adc_cfg, 1);
    hw.adc.Start();
    //hw.StartLog(true);

    //oscilator configuration
    osc.Init(sample_rate);
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetFreq(440);
    osc.SetAmp(1);

    //fm2 configuration
    fm2.Init(sample_rate);
    fm2.SetFrequency(55);
    fm2.SetRatio(1.0f);
    fm2.SetIndex(1.0f);


    hw.StartAudio(AudioCallback);
    

    while(1) {
        float adc_value_1 = hw.adc.GetFloat(0);
        float adc_value_2 = hw.adc.GetFloat(2);
        fm2.SetRatio(1.0f + (adc_value_1 * 2.0f));
    }
}