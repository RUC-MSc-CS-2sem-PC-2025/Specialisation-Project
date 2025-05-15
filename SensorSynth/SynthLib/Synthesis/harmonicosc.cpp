#include "harmonicosc.h"
#include "../Utils/scaler.h"

using namespace sensorsynth;

void HarmonicOsc::Process(float &out_l, float &out_r)
{

    if (n_voices_ > 0)
    {
        float mix = ProcessVoice() * 0.5f;
        out_l = mix;
        out_r = mix;
    }
    else
    {
        float mix = ProcessVoice() * 0.5f;
        out_l = mix;
        out_r = mix;
    }
}

float HarmonicOsc::ProcessVoice()
{
    if (glide_time_ > 0.0f)
    {
        float glide_step = (freq_ - current_freq_) / (glide_time_ * sr_);

        if (fabsf(freq_ - current_freq_) > fabsf(glide_step))
        {
            current_freq_ += glide_step;
        }
        else
        {
            current_freq_ = freq_;
        }
    }

    osc1_.SetFreq(current_freq_);
    osc2_.SetFreq(current_freq_ * pow(2.0f, 3.0f / 12.0f));
    osc3_.SetFreq(current_freq_ * pow(2.0f, 7.0f / 12.0f));
    osc4_.SetFreq(current_freq_ * pow(2.0f, ctrl_2nd / 12.0f));

    float out1 = osc1_.Process();
    float out2 = osc2_.Process();
    float out3 = osc3_.Process();
    float out4 = osc4_.Process();

    return (out1 + out2 + out3 + out4) * 0.25f * out4;
}