#include "sus_pot.h"

#include "define.h"
#include "lut.h"

SusPot::SusPot(HWPin analogPin, float r2, NumericLUT voltageToDisplacement) : _analogPin(analogPin), _r2(r2), _voltageToDisplacementLut(voltageToDisplacement) {}

void SusPot::initialize() {
    pinMode(_analogPin, INPUT);
}

float SusPot::getDisplacement() {
    float voltage = (float)(analogRead(_analogPin)) / 4096.0 * 3.3;
    // Serial.println(voltage);
    float travel = _voltageToDisplacementLut.getValue(voltage);
    // Serial.println(travel);
    return travel;
}