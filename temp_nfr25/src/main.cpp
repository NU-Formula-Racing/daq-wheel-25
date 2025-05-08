#include <Arduino.h>

#include <iostream>

#include "define.hpp"

#define ADC_CONST 3.3 / 4096;

static double __readVoltage(HWPin pin) {
    double reading = analogRead(pin);  // Reference voltage is 3v3 so maximum reading is 3v3 = 4095 in range 0 to 4095
    if (reading < 1 || reading > 4095) return 0;
    // return -0.000000000009824 * pow(reading,3) + 0.000000016557283 * pow(reading,2) + 0.000854596860691 * reading + 0.065440348345433;
    return -0.000000000000016 * pow(reading, 4) + 0.000000000118171 * pow(reading, 3) - 0.000000301211691 * pow(reading, 2) + 0.001109019271794 * reading + 0.034143524634089;
}

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
        float thermistorRaw = __readVoltage(getThermistorPin(i));
        float thermopileRaw = __readVoltage(getThermopilePin(i));
        averageThemistorValue += thermistorRaw;

        float thermopileTransform = (thermopileRaw - THERMOPILE_OFFSET) / THERMOPILE_GAIN;
        float temp = __thermopileLUT.getValue(thermopileTransform);
        Serial.printf("%0.5f (%0.5f) ", temp, thermopileTransform);
    }

    averageThemistorValue /= NUM_SENSORS;
    Serial.print(" | ");
    Serial.print(averageThemistorValue);
    Serial.print("\n");
    delay(100);
}