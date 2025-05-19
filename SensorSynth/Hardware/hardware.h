#include "daisysp.h"
#include "daisy_seed.h"
#include "sensor.h"
#include <memory>

#pragma once
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
        Hardware() {};
        ~Hardware() {};

        float Init(size_t blocksize)
        {
            hw_.Init();
            // hw_.StartLog();
            hw_.SetAudioBlockSize(blocksize);

            ConfigurePins();
            sensor_count_ = adc_pins.size();

            ConfigureADC();
            InitSensors();

            return hw_.AudioSampleRate();
        };

        void StartAudio(daisy::AudioHandle::AudioCallback cb) { hw_.StartAudio(cb); };

        void ReadSensors()
        {
            for (int i = 0; i < sensor_count_; i++)
            {
                float value = ReadSensorValue(i);
                sensors[i]->SetValue(value);
            }
        };

        inline u_int8_t GetNumberOfSensors() { return sensor_count_; };

        inline float GetSensorValue(u_int8_t channel_num) { return sensors[channel_num]->GetValue(); }

        inline float GetSampleRate() { return hw_.AudioSampleRate(); };

        inline float GetBufferSize() { return hw_.AudioBlockSize(); };

        inline daisy::DaisySeed &GetDaisySeed() { return hw_; };

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

        void InitSensors()
        {
            sensors.push_back(std::make_unique<Potentiometer>());
            sensors.push_back(std::make_unique<Potentiometer>());
            sensors.push_back(std::make_unique<Potentiometer>());
            sensors.push_back(std::make_unique<Potentiometer>());
            sensors.push_back(std::make_unique<Potentiometer>());
            sensors.push_back(std::make_unique<Potentiometer>());
            sensors.push_back(std::make_unique<Potentiometer>());

            for (auto &sensor : sensors)
            {
                sensor->SetMaxValue(0.9690f);
            }
        }

        void ConfigurePins()
        {
            adc_pins.push_back(daisy::seed::A0);
            adc_pins.push_back(daisy::seed::A1);
            adc_pins.push_back(daisy::seed::A2);
            adc_pins.push_back(daisy::seed::A3);
            adc_pins.push_back(daisy::seed::A4);
            adc_pins.push_back(daisy::seed::A5);
            adc_pins.push_back(daisy::seed::A6);
        }

        inline float ReadSensorValue(u_int8_t pin) { return hw_.adc.GetFloat(pin); };

        std::vector<std::unique_ptr<Sensor>> sensors;
        std::vector<daisy::Pin> adc_pins;

        daisy::DaisySeed hw_;
        u_int8_t sensor_count_;
    };
}

#endif
#endif
