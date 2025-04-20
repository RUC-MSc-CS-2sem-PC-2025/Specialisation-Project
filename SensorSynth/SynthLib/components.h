#pragma once
#ifndef CMPNNTS_H
#define CMPNNTS_H

#ifdef __cplusplus

#include <cstdint>

namespace sensorsynth
{

    class Pot
    {

    public:
        Pot() {};
        ~Pot() {};
        uint8_t GetPin() {};
        float GetValue() {};
        uint8_t GetType() {};

    private:
        uint8_t pin_;
        float value_;
        uint8_t type_;
    };

    class Photores
    {
        public:
        Photores(){};
        ~Photores(){};

        private:
        uint8_t pin_;
        float value_;
        uint8_t type_;
    };

}

#endif
#endif