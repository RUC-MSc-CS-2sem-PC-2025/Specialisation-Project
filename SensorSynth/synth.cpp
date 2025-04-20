#include "daisysp.h"
#include "daisy_seed.h"
#include "./SynthLib/synthlib.h"

using namespace daisysp;
using namespace daisy;
using namespace sensorsynth;

static DaisySeed  hw;
static Oscillator osc, lfo1;
static Fm2        fm2;
static DelayLine<float, 48000> delay;
static GainControl out_gain, gain1;

static float      delay_time = 1.0f;

static void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t                                size)
{
    float sig1, sig2, out_sig, delayed_sig, lfo_gain;
    for(size_t i = 0; i < size; i += 2)
    {
        sig1 = fm2.Process();
        sig2 = osc.Process();
        lfo_gain = lfo1.Process();

        out_sig = (sig1 + sig2) * lfo_gain;

        delayed_sig = delay.Read();
        delay.Write(out_sig + (delayed_sig * 0.3f));

        out[i] = out_gain.AddGain(out_sig);
        out[i + 1] = out_gain.AddGain(out_sig);
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

    //gaincontrole configuration
    out_gain.Init();
    out_gain.SetGain(0.5f);

    gain1.Init();
    out_gain.SetGain(1);

    //oscilator configuration
    osc.Init(sample_rate);
    osc.SetWaveform(osc.WAVE_SAW);
    osc.SetFreq(110);
    osc.SetAmp(1);

    //LFO configuration
    lfo1.Init(sample_rate);
    lfo1.SetWaveform(osc.WAVE_SQUARE);
    lfo1.SetFreq(2.0f);
    lfo1.SetAmp(1);

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
        float photoA0 = hw.adc.GetFloat(0);
        float photoA1 = hw.adc.GetFloat(1);
        float potA2 = hw.adc.GetFloat(2);

        fm2.SetRatio(1.0f + (photoA0 * 2.0f));
        osc.SetFreq(440 * (photoA0 * 2.0f));

        out_gain.SetGain(potA2);
    }
}