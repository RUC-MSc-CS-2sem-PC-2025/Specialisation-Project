#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed  hw;
static Oscillator osc;
static Fm2        fm2;
static DelayLine<float, 48000> delay;

static float      amp = 0.5f;
static float      delay_time = 1.0f;
static float      amp2 = 0.5f;

static void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t                                size)
{
    float sig1, sig2, mixed_sig, delayed_sig;
    for(size_t i = 0; i < size; i += 2)
    {
        sig1 = fm2.Process() * amp2;
        sig2 = osc.Process() * amp2;

        mixed_sig = sig1 + sig2;

        delayed_sig = delay.Read();
        delay.Write(mixed_sig + (delayed_sig * 0.8f));

        out[i] = delayed_sig;
        out[i + 1] = delayed_sig;
    }
}

int main(void)
{
    float sample_rate;
    //hardware configuration
    hw.Init();
    hw.SetAudioBlockSize(4);
    sample_rate = hw.AudioSampleRate();

    AdcChannelConfig adc_cfg[3];
    adc_cfg[0].InitSingle(daisy::seed::A0);
    adc_cfg[1].InitSingle(daisy::seed::A1);
    adc_cfg[2].InitSingle(daisy::seed::A2);
    hw.adc.Init(adc_cfg, 3);
    hw.adc.Start();
    //hw.StartLog(true);

    //oscilator configuration
    osc.Init(sample_rate);
    osc.SetWaveform(osc.WAVE_SAW);
    osc.SetFreq(110);
    osc.SetAmp(1);

    //fm2 configuration
    fm2.Init(sample_rate);
    fm2.SetFrequency(440);
    fm2.SetRatio(1.0f);
    fm2.SetIndex(1.0f);

    // Delay configuration
    delay.Init();
    delay.SetDelay(sample_rate * delay_time);

    hw.StartAudio(AudioCallback);
    
    while(1) {
        float adc_value_1 = hw.adc.GetFloat(0);
        float adc_value_2 = hw.adc.GetFloat(1);
        float adc_value_3 = hw.adc.GetFloat(2);

        fm2.SetRatio(1.0f + (adc_value_1 * 2.0f));

        delay_time = adc_value_2 * 0.8f;
        amp2 = adc_value_3;
    }
}