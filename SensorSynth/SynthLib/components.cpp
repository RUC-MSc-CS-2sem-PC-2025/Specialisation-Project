#include "components.h"

using namespace sensorsynth;

Pot::Pot() {};
Pot::~Pot() {};
uint8_t Pot::GetPin() {
    return pin_;
}

float Pot::GetValue() {
    return value_;
}

uint8_t Pot::GetType() {
    return 0;
}