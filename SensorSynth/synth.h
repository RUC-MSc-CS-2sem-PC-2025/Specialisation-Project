#include "daisysp.h"
#include "daisy_seed.h"

#pragma once
#ifndef SNTH_H
#define SNTH_H

#ifdef __cplusplus

using namespace daisysp;
using namespace daisy;
namespace sensorsynth
{
    class Synth
    {
    public:
        sensorsynth::Sensor pot1, pot2, pot3, pot4, pot5, pot6, pot7;
        sensorsynth::HarmonyDrone harmonydrone;

        Synth();
        ~Synth();

        void SetKeyFreq(float key_freq) { key_freq_ = key_freq; };
        void SetSampleRate(float sample_rate) { sample_rate_ = sample_rate; };
        float GetKeyFreq() {return key_freq_;};
        float GetSampleRate(){return sample_rate_;};

        void Init()
        {
            sensor_count_ = sizeof(adc_pins) / sizeof(adc_pins[0]);
            key_freq_ = 440.0f;
            InitHardware();
            ConfigureADC();

            harmonydrone.Init(sample_rate_, key_freq_);
        };

        void StartAudio(daisy::AudioHandle::InterleavingAudioCallback cb)
        {
            hw_.StartAudio(cb);
        };

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

        void InitHardware()
        {
            hw_.Init();
            hw_.SetAudioBlockSize(4);
            sample_rate_ = hw_.AudioSampleRate();
        }

        static daisy::DaisySeed hw_;
        daisy::Pin adc_pins[7] =
            {
                daisy::seed::A0,
                daisy::seed::A1,
                daisy::seed::A2,
                daisy::seed::A3,
                daisy::seed::A4,
                daisy::seed::A5,
                daisy::seed::A6};

        u_int8_t sensor_count_;
        float key_freq_;
        float sample_rate_;
    };
}

#endif
#endif
