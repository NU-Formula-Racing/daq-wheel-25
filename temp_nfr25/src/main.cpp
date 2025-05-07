#include <Arduino.h>

#include <iostream>

#include "define.hpp"

#define ADC_CONST 3.3 / 4096;

void setup() {
    Serial.begin(115200);
    pinMode(HWPin::IR1_THERMISTOR, INPUT);
    pinMode(HWPin::IR1_THERMOPILE, INPUT);
}

void loop() {
    float thermistorRaw = analogRead(HWPin::IR1_THERMISTOR) * ADC_CONST;
    float thermopileRaw = analogRead(HWPin::IR1_THERMOPILE) * ADC_CONST;

    Serial.println((thermopileRaw - THERMOPILE_OFFSET) / THERMOPILE_GAIN, 5);
    delay(100);
}