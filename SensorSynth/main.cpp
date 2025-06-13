#include "./Hardware/hardware.hpp"
#include "./Hardware/controls.h"
#include "./SynthLib/synthlib.hpp"
#include "daisy_seed.h"
#include "daisysp.h"

using namespace sensorsynth;

static Hardware hw;
static SynthLib synth;

uint8_t DMA_BUFFER_MEM_SECTION buffer[4];

static void audio_callback(daisy::AudioHandle::InputBuffer in, daisy::AudioHandle::OutputBuffer out, size_t size)
{
    synth.ProcessAudio(in, out, size);
}

static void spi_receive_callback(void *context, daisy::SpiHandle::Result result)
{
    synth.SetValues(buffer[0], buffer[1], buffer[2], buffer[3]);

    hw.spi_handle.DmaReceive(buffer, 4, NULL, spi_receive_callback, NULL);
}

int main(void)
{
    hw.InitSensorSynth();

    float sample_rate = hw.AudioSampleRate();

    synth.Init(sample_rate);

    hw.StartAudio(audio_callback);

    hw.spi_handle.DmaReceive(buffer, 4, NULL, spi_receive_callback, NULL);

    hw.PrintLine("Hello World!");

    while (1)
    {
    }
}
