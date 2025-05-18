#include "synth.h"
#include "../Utils/scaler.h"

using namespace sensorsynth;

void Synth::Init(float sample_rate)
{
    hosc1.Init(sample_rate, 440.0f, 13);

    // shdrone_.Init(sample_rate);
    out_gain.Init();
    chorus_.Init(sample_rate);
    od_.Init();


    lf_.Init(sample_rate);
    lf_.SetFilterMode(daisysp::LadderFilter::FilterMode::BP12);
    lf_.SetRes(0.4f);

    od_.SetDrive(0.2);

    chorus_.SetLfoDepth(0.6f); // Set LFO depth (0.0 to 1.0)
    chorus_.SetLfoFreq(2.0f);  // Set LFO frequency in Hz
    chorus_.SetFeedback(0.3f);

    delay_L.initialize(sample_rate, 1.2f);
    delay_L.setDelayTime(0.6f);
    delay_L.setFeedback(0.4f);

    delay_R.initialize(sample_rate, 1.22f);
    delay_R.setDelayTime(0.62f);
    delay_R.setFeedback(0.4f);
}

void Synth::Process(float &out_left, float &out_right)
{
    hosc1.Process(out_left, out_right);

    float mix = od_.Process(out_left * 0.5 + out_right * 0.5) * 0.5f;

    //out_left += chorus_.Process(mix) * 0.75;
    //out_right += chorus_.GetRight() * 0.75;

    out_left = lf_.Process(mix);
    out_right = lf_.Process(mix);

    // Add gain and clamp output
    out_gain.AddGain(out_left);
    out_gain.AddGain(out_right);

    out_left = fminf(fmaxf(out_left, -1.0f), 1.0f);
    out_right = fminf(fmaxf(out_right, -1.0f), 1.0f);
};

void Synth::MacroOne(float macro_value)
{
    if (macro_value < 0.002f)
    {
        out_gain.SetGain(0.0f);
    }
    else
    {
        out_gain.SetGain(macro_value);
    }
}

void Synth::MacroTwo(float macro_value)
{

    int base_note = sensorsynth::Scaler::SensorValueToMidi(macro_value);
    float freq = sensorsynth::Scaler::MidiNoteToFrequency(base_note);

    hosc1.SetFrequency(freq);
}

void Synth::MacroThree(float macro_value)
{
    hosc1.SetSecondControl(macro_value);
}

void Synth::MacroFour(float macro_value)
{
    hosc1.SetDetuneAmount(macro_value);
}

void Synth::MacroFive(float macro_value)
{
    // Does nothing
}

void Synth::MacroSix(float macro_value)
{
    // Does nothing
}

void Synth::MacroSeven(float macro_value)
{
    // Does nothing
}