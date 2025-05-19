#include "harmonicosc.h"
#include "../Utils/scaler.h"

using namespace sensorsynth;

const float SEMITONE_0_2 = roundf(pow(2.0f, 0.2f / 12.0f) * 100.0f) / 1000.0f;
const float SEMITONE_0_3 = roundf(pow(2.0f, 0.3f / 12.0f) * 100.0f) / 1000.0f;
const float SEMITONE_1_0 = roundf(pow(2.0f, 1.0f / 12.0f) * 100.0f) / 1000.0f;
const float SEMITONE_1_3 = roundf(pow(2.0f, 1.3f / 12.0f) * 100.0f) / 1000.0f;
const float SEMITONE_3_0 = roundf(pow(2.0f, 3.0f / 12.0f) * 100.0f) / 1000.0f;
const float SEMITONE_7_0 = roundf(pow(2.0f, 7.0f / 12.0f) * 100.0f) / 1000.0f;
const float SEMITONE_10_0 = roundf(pow(2.0f, 10.0f / 12.0f) * 100.0f) / 1000.0f;

void HarmonicOsc::ProcessBlock(size_t size, daisy::AudioHandle::OutputBuffer &out)
{
    float voice1[size];
    float voice2[size];

    if (glide_time_ > 0.0f)
    {
        float glide_step = (freq_ - current_freq_) / (glide_time_ * sr_);
        for (size_t i = 0; i < size; i++)
        {
            if (fabsf(freq_ - current_freq_) > fabsf(glide_step))
            {
                current_freq_ += glide_step;
            }
            else
            {
                current_freq_ = freq_;
            }
        }
    }

    ProcessVoiceBlock(size, current_freq_, voice1);
    ProcessVoiceBlock(size, current_freq_ + SEMITONE_0_2, voice2);

    for (size_t i = 0; i < size; i++)
    {
        out[0][i] = voice1[i] * 0.1f;
        out[1][i] = voice2[i] * 0.1f;
    }

    // float mix3 = ProcessVoice(current_freq_ + SEMITONE_0_3);
    // float mix4 = ProcessVoice(current_freq_ + SEMITONE_1_0);
    // float mix5 = ProcessVoice(current_freq_ + SEMITONE_1_3);

    // float mix6 = ProcessVoice(current_freq_ - SEMITONE_0_2);
    // float mix7 = ProcessVoice(current_freq_ - SEMITONE_0_3);
    // float mix8 = ProcessVoice(current_freq_ - SEMITONE_1_0);
    // float mix9 = ProcessVoice(current_freq_ - SEMITONE_1_3);

    // float mix_sum = mix1 + mix2 + mix3 + mix4 + mix5 + mix6 + mix7;
    // float normalised_mix = mix_sum * 0.125f;

    // float mix_l = mix1 + mix2 + mix3 * 0.3f + mix5 * 0.5f  + mix7 * 0.8f  + mix9;
    // float mix_r = mix1 + mix2 + mix4* 0.3f + mix6* 0.5f  +mix8 * 0.8f;

    // out_l = mix_l * 0.1f;
    // out_r = mix_r * 0.1f;
}

void HarmonicOsc::ProcessVoiceBlock(size_t size, float freq, float *output)
{
    float note[size];
    float note2[size];
    float note3[size];
    float note4[size];

    osc1_.SetFreq(freq);
    osc2_.SetFreq(freq * SEMITONE_3_0);
    osc3_.SetFreq(freq * SEMITONE_7_0);
    osc4_.SetFreq(freq * SEMITONE_10_0);

    ProcessOscBlock(size, osc1_, note);
    ProcessOscBlock(size, osc2_, note2);
    ProcessOscBlock(size, osc3_, note3);
    ProcessOscBlock(size, osc4_, note4);

    for (size_t i = 0; i < size; i++)
    {

        output[i] = (note[i] + note2[i] + note3[i] + note4[i]) * 0.25f * note4[i];
    }
}

void HarmonicOsc::ProcessOscBlock(size_t size, daisysp::Oscillator &osc, float *out)
{
    for (size_t i = 0; i < size; i++)
    {
        out[i] = osc.Process();
    }
}
