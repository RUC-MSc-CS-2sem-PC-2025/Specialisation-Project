#include "harmonydrone.h"

using namespace sensorsynth;

HarmonyDrone::HarmonyDrone(float samplerate, float key_freq)
{
    sample_rate_ = samplerate;
    key_freq_ = key_freq;
};

void HarmonyDrone::SetKeyFreq(float freq)
{
    key_freq_ = freq;
};

float HarmonyDrone::GetKeyFreq()
{
    return key_freq_;
};

void HarmonyDrone::SetSampleRate(float sample_rate)
{
    sample_rate_ = sample_rate;
};

float HarmonyDrone::GetSampleRate()
{
    return sample_rate_;
};

void HarmonyDrone::InitOsc(daisysp::Oscillator &osc, float sample_rate, float freq = 440, float amp = 1, uint8_t waveform = 0)
{
    osc.Init(sample_rate);
    osc.SetWaveform(waveform);
    osc.SetFreq(freq);
    osc.SetAmp(amp);
};

void HarmonyDrone::Init()
{
    float sample_rate = GetSampleRate();

    float key_freq = GetKeyFreq();
    float half_key_freq = key_freq * 0.5f;
    float double_key_freq = key_freq * 2.0f;
    float quadruple_key_freq = key_freq * 4.0f;

    InitOsc(osc1, sample_rate, key_freq * 0.5f, 1, daisysp::Oscillator::WAVE_SIN);
    InitOsc(osc2, sample_rate, half_key_freq, 1, daisysp::Oscillator::WAVE_SIN);
    InitOsc(osc3, sample_rate, double_key_freq * 2, 1, daisysp::Oscillator::WAVE_SIN);
    InitOsc(osc4, sample_rate, quadruple_key_freq * 4, 1, daisysp::Oscillator::WAVE_SIN);

    InitOsc(lfo1, sample_rate, 0.5f, 1, daisysp::Oscillator::WAVE_SIN);
    InitOsc(lfo2, sample_rate, 0.25f, 1, daisysp::Oscillator::WAVE_SIN);
};

float HarmonyDrone::Process(float sample_rate)
{
    return 0.0f;
};
