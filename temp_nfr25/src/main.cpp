#include <Arduino.h>

#include <iostream>

#include "define.hpp"

#define ADC_CONST 3.3 / 4096;

void setup() {
    Serial.begin(115200);

    for (int i = 0; i < NUM_SENSORS; i++) {
        pinMode(getThermistorPin(i), INPUT);
        pinMode(getThermopilePin(i), INPUT);
    }
}

void loop() {
    float averageThemistorValue = 0;

    for (int i = 0; i < NUM_SENSORS; i++) {
        float thermistorRaw = analogRead(getThermistorPin(i)) * ADC_CONST;
        float thermopileRaw = analogRead(getThermopilePin(i)) * ADC_CONST;
        averageThemistorValue += thermistorRaw;

        float thermopileTransform = (thermopileRaw - THERMOPILE_OFFSET) / THERMOPILE_GAIN;
        Serial.print(thermopileTransform, 3);
        Serial.print(" ");
    }

    averageThemistorValue /= NUM_SENSORS;
    Serial.print(" | ");
    Serial.print(averageThemistorValue);
    Serial.print("\n");
    delay(100);
}