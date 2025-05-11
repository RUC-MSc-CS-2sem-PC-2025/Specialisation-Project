#pragma once
#ifndef GN_CNTRL_H
#define GN_CNTRL_H

#ifdef __cplusplus

namespace sensorsynth
{
    class GainControl
    {
    public:
        GainControl() {};
        ~GainControl() {};

        void Init()  { gain_ = 1.0f; };

        void SetGain(float gain) { gain_ = gain; };

        void AddGain(float &in) { in =  in * gain_; };

    private:
        float gain_;
    };

}
#endif
#endif