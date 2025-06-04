#include "strain_gauge.h"
#include "util.h"

#include "define.h"
#include "lut.h"

StrainGauge::    StrainGauge(HWPin analogPin, float gain, float offset) :
    _analogPin(analogPin), _gain(gain), _offset(offset) {}

void StrainGauge::initialize() {
    pinMode(_analogPin, INPUT);
}

float StrainGauge::getStrainSignal() {
    float raw = Util::readVoltage(_analogPin);
    return (raw - _offset) / _gain;
}