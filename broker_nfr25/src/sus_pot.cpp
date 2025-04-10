#include "sus_pot.h"

#include "define.h"
#include "lut.h"

SusPot::SusPot(HWPin analogPin, float r2, NumericLUT voltageToDisplacement) : _analogPin(analogPin), _r2(r2), _voltageToDisplacementLut(voltageToDisplacement) {}

void SusPot::initialize() {
    pinMode(_analogPin, INPUT);
}

float getDisplacement() {
    return 0;
}