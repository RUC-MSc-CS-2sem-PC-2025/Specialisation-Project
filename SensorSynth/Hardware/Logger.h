#include "daisysp.h"
#include "daisy_seed.h"
#include "hardware.h"
#include "sensor.h"
#include <memory>

#pragma once
#ifndef LGGR_H
#define LGGR_H

#ifdef __cplusplus

using namespace daisysp;
using namespace daisy;
namespace sensorsynth
{
    class Logger
    {
    public:
        Logger(sensorsynth::Hardware &hw): hw_(hw) {};
        ~Logger() {};

        void Init()
        {

            hw_.GetDaisySeed().StartLog();
        };

        void PrintPinValue(uint8_t pin)
        {
            if (pin < hw_.GetNumberOfSensors())
            {
                float value = hw_.GetSensorValue(pin);
                hw_.GetDaisySeed().PrintLine("%d PV: %f", pin, value);
            }
            else
            {
                hw_.GetDaisySeed().PrintLine("Invalid pin number: %d", pin);
            }
        }

        void PrintSensorValue(uint8_t pin)
        {
            if (pin < hw_.GetNumberOfSensors())
            {
                float sensor_value = hw_.GetSensorValue(pin);
                hw_.GetDaisySeed().PrintLine("%d SV: %f", pin, sensor_value);
            }
            else
            {
                hw_.GetDaisySeed().PrintLine("Invalid pin number: %d", pin);
            }
        }

    private:
        sensorsynth::Hardware &hw_;
    };
}

#endif
#endif
