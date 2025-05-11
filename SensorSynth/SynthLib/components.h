#pragma once
#ifndef CMPNNTS_H
#define CMPNNTS_H

#ifdef __cplusplus

#include <cstdint>

namespace sensorsynth
{
    class Sensor
    {
    public:
        Sensor() {};
        ~Sensor() {};

        void SetValue(float value);
        float GetValue();

    private:
        float value_;
    };
}

#endif
#endif