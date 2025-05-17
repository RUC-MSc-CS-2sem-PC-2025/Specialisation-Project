#include "harmonicosc.h"
#include "../Utils/scaler.h"

using namespace sensorsynth;

void HarmonicOsc::Process(float &out_l, float &out_r)
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

        float mix1 = ProcessVoice(current_freq_);

        float mix2 = ProcessVoice(current_freq_ + pow(2.0f, 0.2f / 12.0f));
        float mix3 = ProcessVoice(current_freq_ + pow(2.0f, 0.3f / 12.0f));
        float mix4 = ProcessVoice(current_freq_ + pow(2.0f, 1.0f / 12.0f));
        float mix5 = ProcessVoice(current_freq_ + pow(2.0f, 1.3f / 12.0f));

        float mix6 = ProcessVoice(current_freq_ - pow(2.0f, 0.2f / 12.0f));
        float mix7 = ProcessVoice(current_freq_ - pow(2.0f, 0.3f / 12.0f));
        float mix8 = ProcessVoice(current_freq_ - pow(2.0f, 1.0f / 12.0f));
        float mix9 = ProcessVoice(current_freq_ - pow(2.0f, 1.3f / 12.0f));

        float mix_sum = mix1 + mix2 + mix3 + mix4 + mix5 + mix6 + mix7;
        float normalised_mix = mix_sum * 0.125f; 

        float mix_l = mix1 + mix2 + mix3 * 0.3f + mix5 * 0.5f  + mix7 * 0.8f  + mix9;
        float mix_r = mix1 + mix2 + mix4* 0.3f + mix6* 0.5f  +mix8 * 0.8f;

        out_l = mix_l * 0.1f;
        out_r = mix_r * 0.1f;

}

float HarmonicOsc::ProcessVoice(float freq)
{
    osc1_.SetFreq(freq);
    osc2_.SetFreq(freq * pow(2.0f, 3.0f / 12.0f));
    osc3_.SetFreq(freq * pow(2.0f, 7.0f / 12.0f));
    osc4_.SetFreq(freq * pow(2.0f, 10.0f / 12.0f));

    float out1 = osc1_.Process();
    float out2 = osc2_.Process();
    float out3 = osc3_.Process();
    float out4 = osc4_.Process();

    return (out1 + out2 + out3 + out4) * 0.25f * out4;
}