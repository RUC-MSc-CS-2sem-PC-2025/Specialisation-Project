#ifndef WAVETABLE_H
#define WAVETABLE_H

#include <array>
#include <cmath>
#include <vector>

class WavetableSynth {
public:
    WavetableSynth() : sampleRate(44100.0), frequency(440.0), phase(0.0), phaseIncrement(0.0) {
        init();
    }

    void init() {
        // Generate a saw wave wavetable
        wavetable.resize(wavetableSize);
        for (size_t i = 0; i < wavetableSize; ++i) {
            wavetable[i] = 2.0 * (static_cast<double>(i) / wavetableSize) - 1.0; // Saw wave
        }
        updatePhaseIncrement();
    }

    std::array<float, 256> processBlock() {
        std::array<float, 256> output{};
        for (size_t i = 0; i < output.size(); ++i) {
            output[i] = static_cast<float>(wavetable[static_cast<size_t>(phase)]);
            phase += phaseIncrement;
            if (phase >= wavetableSize) {
                phase -= wavetableSize;
            }
        }
        return output;
    }

    void setFrequency(double freq) {
        frequency = freq;
        updatePhaseIncrement();
    }

    void setSampleRate(double rate) {
        sampleRate = rate;
        updatePhaseIncrement();
    }

private:
    void updatePhaseIncrement() {
        phaseIncrement = (frequency / sampleRate) * wavetableSize;
    }

    static constexpr size_t wavetableSize = 1024;
    std::vector<double> wavetable;
    double sampleRate;
    double frequency;
    double phase;
    double phaseIncrement;
};

#endif // WAVETABLE_H