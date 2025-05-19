#include <cmath>

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

        inline void SetValue(float value)
        {
            value_ = ProcessValue(value);
        };
        inline float GetValue() { return value_; };

        inline void SetMaxValue(float max_value) { max_value_ = max_value; };
        inline float GetMaxValue() { return max_value_; };

    protected:
        virtual float ProcessValue(float value)
        {
            return roundf((value) * 1000) / 1000.0f;
        }

        float value_;
        float max_value_;
    };

    class Potentiometer : public Sensor
    {
        float ProcessValue(float value) override
        {
            return roundf((value / max_value_) * 1000) / 1000.0f;
        }
    };

    class Photoresistor : public Sensor
    {
    protected:
        float ProcessValue(float value) override
        {
            float scaled_value = (value / max_value_);
            return std::min(std::max(scaled_value, 0.0f), 1.0f); 
        }
    };
}

#endif
#endif