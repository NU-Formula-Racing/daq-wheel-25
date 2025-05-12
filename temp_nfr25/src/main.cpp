#include <Arduino.h>
#include <memory>
#include <iostream>

#include "thermopile.hpp"
#include "define.hpp"

#define ADC_CONST 3.3 / 4096;

std::array<Thermopile, NUM_SENSORS> g_sensors;

void setup() {
    Serial.begin(115200);

    ThermopileConfig config = {
        .thermistorBeta = 3960,
        .thermistorVin = 3.3,
        .thermistorR2 = 10000,
        .thermistorRoomTempResistance = 100000,
        .thermopileSignalOffset = 0.55,
        .thermopileSignalGain = 246,
        .calibrationConstant = (1.6 * 1.6e-12),
        .thermopileLut = std::make_shared<NumericLUT>(__thermopileLUT)
    };

    // create the sensors
    for (int i = 0; i < NUM_SENSORS; i++) {
        new (&g_sensors[i]) Thermopile(
            getThermopilePin(i),
            getThermistorPin(i),
            config
        );
    }
}

void loop() {
    // float averageThemistorValue = 0;

    for (int i = 0; i < NUM_SENSORS; i++) {
        Thermopile thermopile = g_sensors[i];
        Serial.printf("%0.2fC (%0.4fmv) %0.2fC | ", thermopile.getObjectTemperature(), thermopile.getThermopileVoltage() * 1000, thermopile.getAmbientTemperature());
    }

    // averageThemistorValue /= NUM_SENSORS;
    // Serial.print(" | ");
    // Serial.print(averageThemistorValue);
    Serial.print("\n");
    delay(100);
}