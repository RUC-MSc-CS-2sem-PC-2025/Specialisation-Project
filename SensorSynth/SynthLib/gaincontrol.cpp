#include "gaincontrol.h"

using namespace sensorsynth;

void GainControl::Init() { gain_ = 1.0f; };

void GainControl::SetGain(float gain) { gain_ = gain; };

float GainControl::AddGain(float in) { return in * gain_; };