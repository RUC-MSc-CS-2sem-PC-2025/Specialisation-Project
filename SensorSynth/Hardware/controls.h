#pragma once
#include "daisy_seed.h"

#ifndef CTRLS_H
#define CTRLS_H
#ifdef __cplusplus

namespace sensorsynth
{
    class Controls
    {
    public:
        daisy::AnalogControl pot1, pot2, pot3, pot4, ldr1;

        void Init(const daisy::DaisySeed &hw, float sample_rate)
        {
            pot1.Init(hw.adc.GetPtr(0), sample_rate);
            pot2.Init(hw.adc.GetPtr(1), sample_rate);
            pot3.Init(hw.adc.GetPtr(2), sample_rate);
            pot4.Init(hw.adc.GetPtr(3), sample_rate);
            ldr1.Init(hw.adc.GetPtr(4), sample_rate);
        }

        void Process()
        {
            pot1.Process();
            pot2.Process();
            pot3.Process();
            pot4.Process();
            ldr1.Process();
        }
    };
}
#endif
#endif