#ifndef DELAY_H
#define DELAY_H

#include <vector>
#include <algorithm>
#include <cmath>

namespace sensorsynth
{
    // Custom clamp function for C++14 compatibility
    template <typename T>
    T clamp(T value, T min, T max)
    {
        return (value < min) ? min : (value > max ? max : value);
    }

    class Delay
    {
    public:
        void initialize(float sampleRate, float maxDelayTime)
        {
            this->sampleRate = sampleRate;
            this->maxDelayTime = maxDelayTime;
            maxBufferSize = static_cast<size_t>(std::ceil(maxDelayTime * sampleRate));
            buffer.assign(maxBufferSize, 0.0f);
        }

        void setDelayTime(float delayTime)
        {
            targetDelayTime = clamp(delayTime, 0.0f, maxDelayTime);
        }

        void setFeedback(float feedback)
        {
            this->feedback = clamp(feedback, 0.0f, 1.0f);
        }

        float process(float input)
        {
            delayTime += smoothingFactor * (targetDelayTime - delayTime);
            delaySamples = static_cast<size_t>(delayTime * sampleRate);
            updateReadIndex();

            float delayedSample = buffer[readIndex];
            float output = input + delayedSample;

            buffer[writeIndex] = input + delayedSample * feedback;

            incrementIndices();
            return output;
        }

    private:
        float sampleRate = 0.0f;
        float maxDelayTime = 0.0f;
        size_t maxBufferSize = 0;

        std::vector<float> buffer;
        size_t writeIndex = 0;
        size_t readIndex = 0;
        size_t delaySamples = 0;

        float delayTime = 0.5f;
        float targetDelayTime = 0.5f; // New variable for target delay time
        float feedback = 0.5f;
        float smoothingFactor = 0.01f;

        void incrementIndices()
        {
            writeIndex = (writeIndex + 1) % maxBufferSize;
            updateReadIndex();
        }

        void updateReadIndex()
        {
            readIndex = (writeIndex + maxBufferSize - delaySamples) % maxBufferSize;
        }
    };

}
#endif