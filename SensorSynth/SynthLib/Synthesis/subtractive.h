#ifndef SUBTRACTIVE_H
#define SUBTRACTIVE_H

#include <array>
#include <cmath>
#include <algorithm>
namespace sensorsynth
{

    class SubtractiveSynth
    {
    public:
        static constexpr size_t BLOCK_SIZE = 256;

        SubtractiveSynth() {};
        ~SubtractiveSynth() {};

        void Init(float sampleRate)
        {
            this->sampleRate = sampleRate;
            frequency = 440.0f;
            amplitude = 1.0f;
            filterCutoff = 1000.0f;
            phase = 0.0f;
            prevSample = 0.0f;
        }
        
        void setFrequency(float freq) { frequency = freq; }
        void setAmplitude(float amp) { amplitude = amp; }
        void setFilterCutoff(float cutoff) { filterCutoff = cutoff; }

        std::array<float, BLOCK_SIZE> generateBlock()
        {
            std::array<float, BLOCK_SIZE> block{};
            for (size_t i = 0; i < BLOCK_SIZE; ++i)
            {
                float sample = amplitude * (2.0f * (phase / (2.0f * M_PI)) - 1.0f);
                phase += 2.0f * M_PI * frequency / sampleRate;
                if (phase >= 2.0f * M_PI)
                    phase -= 2.0f * M_PI;
                sample = lowPassFilter(sample);

                block[i] = sample;
            }
            return block;
        }

    private:
        float sampleRate;
        float frequency;
        float amplitude;
        float filterCutoff;
        float phase;
        float prevSample;

        float lowPassFilter(float input)
        {
            float alpha = filterCutoff / (filterCutoff + sampleRate);
            float output = alpha * input + (1.0f - alpha) * prevSample;
            prevSample = output;
            return output;
        }
    };
}

#endif // SUBTRACTIVE_H
