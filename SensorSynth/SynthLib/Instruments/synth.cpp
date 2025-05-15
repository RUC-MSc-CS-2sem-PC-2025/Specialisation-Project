#include "synth.h"
#include "../Utils/scaler.h"

using namespace sensorsynth;

void Synth::Init(float sample_rate)
{
    hosc1.Init(sample_rate, 440.0f, 5);
    //shdrone_.Init(sample_rate);
    out_gain.Init();
    
    // delay_L.initialize(sample_rate, 0.5);
    // delay_L.setDelayTime(0.5f); 
    // delay_L.setFeedback(0.7f);

    // delay_R.initialize(sample_rate, 0.5);
    // delay_R.setDelayTime(0.4f); 
    // delay_R.setFeedback(0.7f);
}

void Synth::Process(float &out_left, float &out_right)
{
    //float out = hosc1.Process() * 0.95f + shdrone_.Process() * 0.05f;
    hosc1.Process(out_left,out_right);
    //float voice2 = hosc1.Process() * 0.98f; // Slight detune
    //float voice3 = hosc1.Process() * 1.02f; // Slight detune

    // out_left = (voice1 + voice2 * 0.5f) * 0.7f;  // Pan slightly left
    // out_right = (voice1 + voice3 * 0.5f) * 0.7f;
    
    //out_left = delay_L.process(out);
    //out_right = delay_R.process(out);
    // out_left = voice1 * 0.5f;
    // out_right = voice1 * 0.5f;

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