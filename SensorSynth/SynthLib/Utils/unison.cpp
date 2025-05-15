#include "unison.h"

using namespace sensorsynth;

void Unison::Init(float sample_rate, uint8_t n_voices)
{
    sr_ = sample_rate;
    nv_ = n_voices;
}

void Unison::Process(float in, float &out_left, float &out_right)
{


}
