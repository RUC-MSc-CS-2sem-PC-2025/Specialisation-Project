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

        void Init();


        void SetGain(float gain);

        float AddGain(float in);

    private:
        float gain_;
    };

}
#endif
#endif