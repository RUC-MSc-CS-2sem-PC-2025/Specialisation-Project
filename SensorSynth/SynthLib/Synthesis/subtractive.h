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

        float lowPassFilter(float input) {
            static float prevOutput1 = 0.0f;
            static float prevOutput2 = 0.0f;
            static float prevInput1 = 0.0f;
            static float prevInput2 = 0.0f;

            float RC = 1.0f / (2.0f * M_PI * filterCutoff);
            float dt = 1.0f / sampleRate;
            float alpha = dt / (RC + dt);

            // Resonance control
            float resonance = 0.6f; // Adjust this value (0.0f to 1.0f) to control resonance
            float feedback = resonance * (1.0f - alpha);

            // Second-order filter implementation
            float output = alpha * input 
                 + feedback * prevOutput1 
                 - (1.0f - alpha) * prevOutput2;

            prevOutput2 = prevOutput1;
            prevOutput1 = output;

            prevInput2 = prevInput1;
            prevInput1 = input;

            return output;
        }
    };
}

#endif // SUBTRACTIVE_H
