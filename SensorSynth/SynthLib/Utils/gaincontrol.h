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

        void Init() { gain_ = 0.0f; };

        void SetGain(float gain)
        {
            if (gain > 1.0f)
            {
                gain_ = 1.0f;
            }
            else if (gain < 0.0f)
            {
                gain_ = 0.0f;
            }
            else
            {
                gain_ = gain;
            }
        }

        void AddGain(float &in)
        {
            static float previous_gain = gain_;
            float current_gain = gain_;
            float interpolated_gain = previous_gain * 0.9f + current_gain * 0.1f;
            in = in * interpolated_gain;
            previous_gain = interpolated_gain;
        };

    private:
        float gain_;
    };

}
#endif
#endif