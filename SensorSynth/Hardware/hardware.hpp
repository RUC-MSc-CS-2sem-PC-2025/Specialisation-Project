#pragma once

#include "daisy_seed.h"

namespace sensorsynth
{
    class Hardware: public daisy::DaisySeed
    {
    public:
        daisy::SpiHandle spi_handle;

        void InitSensorSynth();

    private:
        daisy::SpiHandle::Config spi_conf;

        void ConfigureSPI();
    };
}
