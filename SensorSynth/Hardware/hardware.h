#pragma once

#include "daisysp.h"
#include "daisy_seed.h"
#include <memory>

#ifndef HRDWR_H
#define HRDWR_H

#ifdef __cplusplus

using namespace daisysp;
using namespace daisy;
namespace sensorsynth
{
    class Hardware
    {
    public:
        daisy::DaisySeed hw_;

        Hardware() {};
        ~Hardware() {};

        float Init(size_t blocksize)
        {
            hw_.Init();
            hw_.SetAudioBlockSize(blocksize);

            ConfigurePins();
            sensor_count_ = adc_pins.size();

            ConfigureADC();

            return hw_.AudioSampleRate();
        };

        void StartAudio(daisy::AudioHandle::AudioCallback cb) { hw_.StartAudio(cb); };

    private:
        void ConfigureADC()
        {
            AdcChannelConfig adc_cfg[sensor_count_];

            for (u_int8_t i = 0; i < sensor_count_; i += 1)
            {
                adc_cfg[i].InitSingle(adc_pins[i]);
            }

            hw_.adc.Init(adc_cfg, sensor_count_);
            hw_.adc.Start();
        }
        void ConfigurePins()
        {
            adc_pins.push_back(daisy::seed::A0);
            adc_pins.push_back(daisy::seed::A1);
            adc_pins.push_back(daisy::seed::A2);
            adc_pins.push_back(daisy::seed::A3);
            adc_pins.push_back(daisy::seed::A4);
        }
        std::vector<daisy::Pin> adc_pins;

        u_int8_t sensor_count_;
    };
}

#endif
#endif
