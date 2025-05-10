#include "harmonydrone.h"

using namespace sensorsynth;

daisysp::DelayLine<float, 48000> delay_line_L; // Left channel delay
daisysp::DelayLine<float, 48000> delay_line_R;

void HarmonyDrone::InitOsc(daisysp::Oscillator &osc, float sample_rate, float freq, float amp, uint8_t waveform)
{
    osc.Init(sample_rate);
    osc.SetWaveform(waveform);
    osc.SetFreq(freq);
    osc.SetAmp(amp);
};

void HarmonyDrone::Init(const float sample_rate, float key_freq)
{
    sample_rate_ = sample_rate;
    key_freq_ = key_freq;

    delay_line_L.Init();
    delay_line_R.Init();

    delay_line_L.SetDelay(sample_rate * 0.7f);
    delay_line_R.SetDelay(sample_rate * 0.8f);

    InitOsc(osc1_, sample_rate, key_freq * 0.5f, 0.1, daisysp::Oscillator::WAVE_SQUARE);
    InitOsc(osc2_, sample_rate, key_freq, 0.1, daisysp::Oscillator::WAVE_SQUARE);
};

void HarmonyDrone::OscOneChangePitch(float new_freq)
{
    osc1_.SetFreq(new_freq);
};

void HarmonyDrone::OscTwoChangePitch(float new_freq)
{
    osc2_.SetFreq(new_freq);
};

void HarmonyDrone::Unison(float signal)
{
    const int num_voices = 5;
    const float detune_amounts[num_voices] = {-0.08f, -0.04f, 0.0f, 0.04f, 0.08f};
    const float pan_positions[num_voices] = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};
    float left_output = 0.0f;
    float right_output = 0.0f;

    for (int i = 0; i < num_voices; ++i)
    {
        float detuned_signal = signal * (1.0f + detune_amounts[i]);

        float pan = pan_positions[i];
        left_output += detuned_signal * (1.0f - pan);
        right_output += detuned_signal * pan;
    }

    SetOutL(left_output / num_voices);
    SetOutR(right_output / num_voices);
}

void HarmonyDrone::Delay()
{
    float L = GetOutL();
    float R = GetOutR();

    // Read delayed samples
    float delayed_L = delay_line_L.Read();
    float delayed_R = delay_line_R.Read();

    // Add feedback to the delay lines
    delay_line_L.Write(L + delayed_L * 0.7f); // Feedback amount is 0.3
    delay_line_R.Write(R + delayed_R * 0.7f);

    // Mix the delayed signal with the original signal (feedback can be added here if needed)
    SetOutL(L + delayed_L); // Mix original and delayed signals
    SetOutR(R + delayed_R);
}

void HarmonyDrone::Process(float &L, float &R)
{
    float osc_output = osc1_.Process() * osc2_.Process();
    // SetOutL(osc_output);
    // SetOutR(osc_output);
    Unison(osc_output);
    Delay();

    L = GetOutL();
    R = GetOutR();
};
