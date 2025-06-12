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
        SpiHandle spi_handle;

        Hardware() {};
        ~Hardware() {};

        float Init(size_t blocksize)
        {
            hw_.Init();
            hw_.SetAudioBlockSize(blocksize);

            ConfigurePins();
            sensor_count_ = adc_pins.size();

            ConfigureADC();

            ConfigureSPI();

            return hw_.AudioSampleRate();
        };

        void StartAudio(daisy::AudioHandle::AudioCallback cb) { hw_.StartAudio(cb); };

    private:
        void ConfigureSPI()
        {
            // SpiHandle object and Spi Configuration object
            SpiHandle::Config spi_conf;

            // Set some configurations
            spi_conf.periph = SpiHandle::Config::Peripheral::SPI_1;
            spi_conf.mode = SpiHandle::Config::Mode::SLAVE;
            spi_conf.direction = SpiHandle::Config::Direction::TWO_LINES;
            spi_conf.nss = SpiHandle::Config::NSS::HARD_OUTPUT;
            spi_conf.pin_config.sclk = Pin(PORTG, 11);
            spi_conf.pin_config.miso = Pin(PORTB, 4);
            spi_conf.pin_config.mosi = Pin(PORTB, 5);
            spi_conf.pin_config.nss = Pin(PORTG, 10);

            // Initialize the handle using our configuration
            spi_handle.Init(spi_conf);
        }

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
