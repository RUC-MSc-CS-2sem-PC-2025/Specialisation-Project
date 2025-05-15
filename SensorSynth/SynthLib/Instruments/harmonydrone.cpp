#include "harmonydrone.h"

using namespace sensorsynth;

void HarmonyDrone::InitOsc(daisysp::Oscillator &osc, float sample_rate, float freq, float amp, uint8_t waveform)
{
    osc.Init(sample_rate);
    osc.SetWaveform(waveform);
    osc.SetFreq(freq);
    osc.SetAmp(amp);
};

void HarmonyDrone::InitDelay(float &sample_rate, daisysp::DelayLine<float, 48000> &delay_line_L, daisysp::DelayLine<float, 48000> &delay_line_R)
{
    delay_line_L.Init();
    delay_line_R.Init();

    delay_line_L.SetDelay(sample_rate * 1.2f);
    delay_line_R.SetDelay(sample_rate * 0.9f);
};

void HarmonyDrone::InitUnison(u_int8_t voices)
{
}

void HarmonyDrone::InitFilter(float sample_rate, float freq, float res)
{
    ladder_filter_.Init(sample_rate);
    ladder_filter_.SetFilterMode(daisysp::LadderFilter::FilterMode::BP12);
    ladder_filter_.SetFreq(freq);
    ladder_filter_.SetRes(res);
}

void HarmonyDrone::InitGain()
{
    gain1_.Init();
}

void HarmonyDrone::Init(const float sample_rate, float key_freq)
{
    sample_rate_ = sample_rate;
    key_freq_ = key_freq;

    //InitDelay(sample_rate_, delay_line_L, delay_line_R);
    InitOsc(osc1_, sample_rate, key_freq * 0.5f, 0.1, daisysp::Oscillator::WAVE_SQUARE);
    InitOsc(osc2_, sample_rate, key_freq, 0.1, daisysp::Oscillator::WAVE_SQUARE);
    InitUnison(3);
    InitFilter(sample_rate_, 1000.0f, 0.7f);
};


void HarmonyDrone::Delay()
{
    float L = GetOutL();
    float R = GetOutR();

    //float delayed_L = delay_line_L.Read();
    //float delayed_R = delay_line_R.Read();

    //delay_line_L.Write(L + delayed_L * 0.6f);
    //delay_line_R.Write(R + delayed_R * 0.6f);

    SetOutL(0.6f * L + 0.38f );
    SetOutR(0.6f * R + 0.38f);
}

void HarmonyDrone::Volume()
{
    float L = GetOutL();
    float R = GetOutR();

    gain1_.AddGain(L);
    gain1_.AddGain(R);

    SetOutL(L);
    SetOutR(R);
}

void HarmonyDrone::Process(float &L, float &R)
{
    float osc_output = osc1_.Process() * osc2_.Process();
    //float filtered_output = ladder_filter_.Process(osc_output);
    SetOutL(osc_output);
    SetOutR(osc_output);
    // Unison(osc_output);
    //Delay();
    Volume();

    L = GetOutL();
    R = GetOutR();
};

void HarmonyDrone::OscOneChangePitch(float new_freq)
{
    osc1_.SetFreq(new_freq);
};

void HarmonyDrone::OscTwoChangePitch(float new_freq)
{
    osc2_.SetFreq(new_freq);
};

void HarmonyDrone::UnisoneDetune(float value)
{

}

void HarmonyDrone::SetDelayTimeL(float value)
{
    //delay_line_L.SetDelay(sample_rate_ * value);
}

void HarmonyDrone::SetDelayTimeR(float value)
{
    //delay_line_R.SetDelay(sample_rate_ * value);
}

void HarmonyDrone::SetFilterFreq(float value)
{
    ladder_filter_.SetFreq(key_freq_ * 4 * value);
}

void HarmonyDrone::SetFilterRes(float value)
{
    ladder_filter_.SetRes(value);
}

void HarmonyDrone::SetVolume(float value)
{
    gain1_.SetGain(value);
}