#pragma once
#ifndef CMPNNTS_H
#define CMPNNTS_H

#ifdef __cplusplus

#include <cstdint>

namespace sensorsynth
{
    class Photores
    {
    public:
        Photores(){};
        ~Photores(){};

        void SetValue(float value);
        float GetValue();

    private:
        float value_;
    };
}

#endif
#endif