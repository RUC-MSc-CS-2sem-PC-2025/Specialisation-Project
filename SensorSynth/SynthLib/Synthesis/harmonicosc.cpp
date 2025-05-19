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
    float voice3[size];

    voice1[size] = {0};
    voice2[size] = {0};

    // if (glide_time_ > 0.0f)
    // {
    //     float glide_step = (freq_ - current_freq_) / (glide_time_ * sr_);
    //     for (size_t i = 0; i < size; i++)
    //     {
    //         current_freq_ += glide_step;
    //         if (fabsf(freq_ - current_freq_) > fabsf(glide_step))
    //         {
    //             current_freq_ += glide_step;
    //         }
    //         else
    //         {
    //             current_freq_ = freq_;
    //         }
    //     }
    // }
    memset(voice1, 0, sizeof(voice1));
    memset(voice2, 0, sizeof(voice2));
    memset(voice3, 0, sizeof(voice3));

    ProcessVoiceBlock(size, freq_, voice1);
    ProcessVoiceBlock(size, freq_ + SEMITONE_0_2, voice1);
    ProcessVoiceBlock(size, freq_ - SEMITONE_0_3, voice2);

    for (size_t i = 0; i < size; i++)
    {
        out[0][i] = (voice1[i] + voice2[i] + voice3[i]); // Left channel
        out[1][i] = (voice1[i] - voice2[i] - voice3[i]);
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
    // float note[size];
    // float note2[size];
    // float note3[size];
    // float note4[size];
    osc1_.SetFreq(freq);
    osc2_.SetFreq(freq + SEMITONE_3_0);
    osc3_.SetFreq(freq + SEMITONE_7_0);
    osc4_.SetFreq(freq + SEMITONE_10_0);

    // ProcessOscBlock(size, osc1_, note);
    // ProcessOscBlock(size, osc2_, note2);
    // ProcessOscBlock(size, osc3_, note3);
    // ProcessOscBlock(size, osc4_, note4);

    for (size_t i = 0; i < size; i++)
    {
        float m1 = osc1_.Process();
        float m2 = osc2_.Process();
        float m3 = osc3_.Process();
        float m4 = osc4_.Process();

        output[i] = (m1 + m2 + m3 + m4) * 0.25f ;
    }

    osc1_.Reset();
    osc2_.Reset();
    osc3_.Reset();
    osc4_.Reset();
}

void HarmonicOsc::SetFrequency(float freq)
{
    freq_ = freq;

    // Update oscillator frequencies immediately
    osc1_.SetFreq(freq_);
    osc2_.SetFreq(freq_ + SEMITONE_3_0);
    osc3_.SetFreq(freq_ + SEMITONE_7_0);
    osc4_.SetFreq(freq_ + SEMITONE_10_0);
}

void HarmonicOsc::ProcessOscBlock(size_t size, daisysp::Oscillator &osc, float *out)
{
    for (size_t i = 0; i < size; i++)
    {
        out[i] = osc.Process();
    }
}
