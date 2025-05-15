#include "../synthlib.h"

#pragma once
#ifndef SNTH_H
#define SNTH_H

#ifdef __cplusplus

namespace sensorsynth
{
    class Synth
    {
    public:
        sensorsynth::HarmonicOsc hosc1;
        sensorsynth::GainControl out_gain;
        sensorsynth::Delay delay_L, delay_R ;
        sensorsynth::SHDrone shdrone_;
        Synth() {};
        ~Synth() {};

        void Init(float sample_rate);

        void Process(float &out_left, float &out_right);

        void MacroOne(float macro_value);   
        void MacroTwo(float macro_value); 
        void MacroThree(float macro_value);
        void MacroFour(float macro_value0);
        void MacroFive(float macro_value);
        void MacroSix(float macro_value);
        void MacroSeven(float macro_value);

    private:
    };
}

#endif
#endif
