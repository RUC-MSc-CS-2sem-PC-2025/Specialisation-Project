#ifndef ADDITIVE_H
#define ADDITIVE_H

#include <vector>
#include <cmath>
#include <array>

constexpr size_t BLOCK_SIZE = 256;

class AdditiveSynth {
public:
    AdditiveSynth() : sampleRate(44100.0f), frequency(440.0f), phase(0.0f) {}

    void init(float sampleRate, float frequency) {
        this->sampleRate = sampleRate;
        this->frequency = frequency;
        this->phase = 0.0f;
        calculateHarmonics();
    }

    void setFrequency(float freq) {
        frequency = freq;
        calculateHarmonics();
    }

    std::array<float, BLOCK_SIZE> processBlock() {
        std::array<float, BLOCK_SIZE> block{};

        const float phaseIncrement = (2.0f * M_PI * frequency) / sampleRate;

        for (size_t i = 0; i < BLOCK_SIZE; ++i) {
            block[i] = 0.0f;

            // Add harmonics to approximate a sawtooth wave
            for (size_t h = 1; h <= harmonics.size(); ++h) {
                block[i] += harmonics[h - 1] * std::sin(phase * h);
            }

            // Normalize the output
            block[i] *= amplitudeScale;

            // Increment phase and wrap around
            phase += phaseIncrement;
            if (phase >= 2.0f * M_PI) {
                phase -= 2.0f * M_PI;
            }
        }

        return block;
    }

private:
    float sampleRate;
    float frequency;
    float phase;
    std::vector<float> harmonics;
    float amplitudeScale;

    void calculateHarmonics() {
        harmonics.clear();
        const size_t maxHarmonics = 20; // Number of harmonics to include
        amplitudeScale = 0.0f;

        for (size_t h = 1; h <= maxHarmonics; ++h) {
            float amplitude = 1.0f / h; // Amplitude decreases with harmonic number
            harmonics.push_back(amplitude);
            amplitudeScale += amplitude;
        }

        // Normalize amplitude scale
        amplitudeScale = 1.0f / amplitudeScale;
    }
};

#endif // ADDITIVE_H