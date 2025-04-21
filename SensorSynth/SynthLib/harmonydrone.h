#include "daisysp.h"

#pragma once
#ifndef INSTRMNS_H
#define INSTRMNS_H

#ifdef __cplusplus

namespace sensorsynth
{
    class HarmonyDrone
    {

    public:
        HarmonyDrone() {};
        ~HarmonyDrone() {};

        float Process(float sample_rate);

    private:
        static daisysp::Oscillator osc1, osc2, osc3, osc4;
        static daisysp::Oscillator lfo1, lfo2;

        
    };

}
#endif
#endif