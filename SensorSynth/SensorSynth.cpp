#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;

int main(void) {

  hw.Init();

  AdcChannelConfig adc_cfg;
  adc_cfg.InitSingle(daisy::seed::A0);
  hw.adc.Init(&adc_cfg, 1);
  hw.adc.Start();

  hw.StartLog(true);

  hw.PrintLine("Hello World! This is the Daisy Seed!");

  while(1) {

    float analog_value = hw.adc.GetFloat(0);

    // Print the value to the log
    //!!! MAKEFILE: DISABBLE LOGGING FOR BETTER PERFORMANCE
    hw.PrintLine("Analog Value (A0): %f", analog_value);
  }
}