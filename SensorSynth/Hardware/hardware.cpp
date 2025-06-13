#include "hardware.hpp"
#include "../constants.hpp"
#include "daisy_seed.h"

using namespace sensorsynth;
using namespace daisy;

void Hardware::InitSensorSynth()
{
    Init();
    StartLog();
    SetAudioBlockSize(BLOCK_SIZE);

    ConfigureSPI();
}

void Hardware::ConfigureSPI()
{
    spi_conf.periph = SpiHandle::Config::Peripheral::SPI_1;
    spi_conf.mode = SpiHandle::Config::Mode::SLAVE;
    spi_conf.direction = SpiHandle::Config::Direction::TWO_LINES_RX_ONLY;
    spi_conf.clock_polarity = SpiHandle::Config::ClockPolarity::HIGH;
    spi_conf.clock_phase = SpiHandle::Config::ClockPhase::ONE_EDGE;
    spi_conf.nss = SpiHandle::Config::NSS::HARD_INPUT;
    spi_conf.pin_config.sclk = Pin(PORTG, 11);
    spi_conf.pin_config.miso = Pin(PORTB, 4);
    spi_conf.pin_config.mosi = Pin(PORTB, 5);
    spi_conf.pin_config.nss = Pin(PORTG, 10);

    spi_handle.Init(spi_conf);
}
