#ifndef FM_SYNTHESIZER_H
#define FM_SYNTHESIZER_H

#include <cmath>
#include <array>

class SubtractiveSynth {
public:
    static constexpr size_t BLOCK_SIZE = 256; // Example block size
};

class FMSynthesizer {
public:
    FMSynthesizer() 
        : sampleRate(0.0f), carrierFreq(440.0f), modulatorFreq(220.0f),
          modulationIndex(1.0f), phaseCarrier(0.0f), phaseModulator(0.0f) {}

    void init(float sampleRate) {
        this->sampleRate = sampleRate;
        carrierFreq = 440.0f;
        modulatorFreq = 220.0f;
        modulationIndex = 1.0f;
        phaseCarrier = 0.0f;
        phaseModulator = 0.0f;
    }

    void setCarrierFrequency(float freq) {
        carrierFreq = freq;
    }

    void setModulatorFrequency(float freq) {
        modulatorFreq = freq;
    }

    void setModulationIndex(float index) {
        modulationIndex = index;
    }

    std::array<float, SubtractiveSynth::BLOCK_SIZE> processBlock() {
        const float twoPi = 2.0f * M_PI;
        float phaseIncrementCarrier = twoPi * carrierFreq / sampleRate;
        float phaseIncrementModulator = twoPi * modulatorFreq / sampleRate;

        std::array<float, SubtractiveSynth::BLOCK_SIZE> output;

        for (size_t i = 0; i < output.size(); ++i) {
            float modulator = std::sin(phaseModulator) * modulationIndex;
            output[i] = std::sin(phaseCarrier + modulator);

            phaseCarrier += phaseIncrementCarrier;
            phaseModulator += phaseIncrementModulator;

            if (phaseCarrier >= twoPi) phaseCarrier -= twoPi;
            if (phaseModulator >= twoPi) phaseModulator -= twoPi;
        }

        return output;
    }

private:
    float sampleRate;
    float carrierFreq;
    float modulatorFreq;
    float modulationIndex;
    float phaseCarrier;
    float phaseModulator;
};

#endif // FM_SYNTHESIZER_H