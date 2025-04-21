#include "harmonydrone.h"

using namespace sensorsynth;

void HarmonyDrone::InitOsc(float sample_rate) 
{
    osc1.Init(sample_rate);
    osc1.SetWaveform(osc1.WAVE_SIN);
    osc1.SetFreq(110);
    osc1.SetAmp(1);
};

float HarmonyDrone::Process(float sample_rate)
{


    lfo1.Init(sample_rate);
    lfo1.SetWaveform(lfo1.WAVE_SIN);
    lfo1.SetFreq(0.5f);
    lfo1.SetAmp(1);

    osc1.SetFreq(440 * (photo1.GetValue() * 2.0f));
};
